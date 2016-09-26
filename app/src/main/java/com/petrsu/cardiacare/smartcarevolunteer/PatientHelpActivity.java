package com.petrsu.cardiacare.smartcarevolunteer;

import android.content.Context;
import android.graphics.drawable.Drawable;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import org.osmdroid.DefaultResourceProxyImpl;
import org.osmdroid.ResourceProxy;
import org.osmdroid.api.IMapController;
import org.osmdroid.tileprovider.tilesource.TileSourceFactory;
import org.osmdroid.util.GeoPoint;
import org.osmdroid.views.MapController;
import org.osmdroid.views.MapView;
import org.osmdroid.views.overlay.ItemizedIconOverlay;
import org.osmdroid.views.overlay.MyLocationOverlay;
import org.osmdroid.views.overlay.OverlayItem;
import org.osmdroid.views.overlay.ScaleBarOverlay;
import org.osmdroid.views.overlay.SimpleLocationOverlay;

import java.util.ArrayList;

/**
 * Created by cardiacare on 26.11.15.
 */
public class PatientHelpActivity  extends AppCompatActivity {
    int stats = 0;
    private MapView mapView;

    private IMapController mapController;
    private SimpleLocationOverlay mMyLocationOverlay;
    private ScaleBarOverlay mScaleBarOverlay;
    ItemizedIconOverlay<OverlayItem> currentLocationOverlay;
    DefaultResourceProxyImpl resourceProxy;
    LocationManager locationManager;
    ArrayList<OverlayItem> overlayItemArray;
    ArrayList<OverlayItem> patOverlayItemArray;
    ItemizedIconOverlay<OverlayItem> myLocationOverlay;
    ItemizedIconOverlay<OverlayItem> patLocationOverlay;


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_help_patient);

        mapView = (MapView) this.findViewById(R.id.map);
        mapView.setTileSource(TileSourceFactory.MAPNIK);

        mapView.setMultiTouchControls(true);

        mapController = this.mapView.getController();
        mapController.setZoom(14);

        this.mScaleBarOverlay = new ScaleBarOverlay(this);
        this.mapView.getOverlays().add(mScaleBarOverlay);


        /////////////////

        //GeoPoint currentLocation2 = new GeoPoint(63.557413, -156.102119);

        resourceProxy = new DefaultResourceProxyImpl(getApplicationContext());
        GeoPoint currentLocation = new GeoPoint(61.784464, 34.344272);

        mapController.setCenter(currentLocation);

        OverlayItem myLocationOverlayItem = new OverlayItem("Here", "Current Position", currentLocation);
        //Drawable myCurrentLocationMarker = this.getResources().getDrawable(R.drawable.ic_launcher);
        //myLocationOverlayItem.setMarker(myCurrentLocationMarker);

        final ArrayList<OverlayItem> items = new ArrayList<OverlayItem>();
        items.add(myLocationOverlayItem);

        currentLocationOverlay = new ItemizedIconOverlay<OverlayItem>(items,
                new ItemizedIconOverlay.OnItemGestureListener<OverlayItem>() {
                    public boolean onItemSingleTapUp(final int index, final OverlayItem item) {
                        return true;
                    }

                    public boolean onItemLongPress(final int index, final OverlayItem item) {
                        return true;
                    }
                }, resourceProxy);


        this.mapView.getOverlays().add(this.currentLocationOverlay);

        //MY LOCATION
        overlayItemArray = new ArrayList<OverlayItem>();
        //PAT LOCATION

        //this.mapView.getOverlays().add(this.myLocationOverlay);

        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

        //for demo, getLastKnownLocation from GPS only, not from NETWORK
        Location lastLocation
                = locationManager.getLastKnownLocation(
                LocationManager.GPS_PROVIDER);
        if(lastLocation != null){
            updateLoc(lastLocation);
        }
         /*
         * Subscribe to Patient Location
         */
        MainActivity.initCurrentPatientLocationSbcr(MainActivity.nodeDescriptor, MainActivity.needPatientUri, this);
    }

    public void changePatientLocation(String lat, String longi){
        final double latitude = Double.parseDouble(lat);
        final double longitude = Double.parseDouble(longi);
        Log.i("SS", "Enter to java changePatient");
        runOnUiThread(new Thread(new Runnable() {
            public void run() {
                GeoPoint curPatientLocation = new GeoPoint(latitude, longitude);

                patOverlayItemArray = new ArrayList<OverlayItem>();
                OverlayItem patientLocationOverlayItem = new OverlayItem("Here", "Current Patient Position", curPatientLocation);
                patOverlayItemArray.add(patientLocationOverlayItem);

                if (patLocationOverlay != null) {
                    mapView.getOverlays().remove(patLocationOverlay);
                    mapView.invalidate();
                } else {
                    patLocationOverlay = new ItemizedIconOverlay(patOverlayItemArray, null, resourceProxy);
                    mapView.invalidate();
                }
                mapView.getOverlays().add(patLocationOverlay);
                mapView.invalidate();
            }
        }));
    }


            @Override
            protected void onResume() {
                // TODO Auto-generated method stub
                super.onResume();
                locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, myLocationListener);
                locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, myLocationListener);
            }

            @Override
            protected void onPause() {
                // TODO Auto-generated method stub
                super.onPause();
                locationManager.removeUpdates(myLocationListener);
            }

            private void updateLoc(Location loc) {
                GeoPoint locGeoPoint = new GeoPoint(loc.getLatitude(), loc.getLongitude());
                mapController.setCenter(locGeoPoint);
                //TextView textView =  (TextView) findViewById(R.id.textView);
                //textView.setText("getLatitude" + locGeoPoint.getLatitude() + "\n getLongitude" + locGeoPoint.getLongitude());

                overlayItemArray.clear();
                OverlayItem myLocationOverlayItem = new OverlayItem("Here", "Current Position", locGeoPoint);
                //Drawable myCurrentLocationMarker = this.getResources().getDrawable(R.drawable.ic_launcher);
                //myLocationOverlayItem.setMarker(myCurrentLocationMarker);

                overlayItemArray.add(myLocationOverlayItem);

                if (myLocationOverlay != null) {
                    mapView.getOverlays().remove(myLocationOverlay);
                    mapView.invalidate();
                } else {
                    myLocationOverlay = new ItemizedIconOverlay(overlayItemArray, null, resourceProxy);
                    mapView.invalidate();
                }

                mapView.getOverlays().add(myLocationOverlay);


                //textView.setText("getLatitude" + locGeoPoint.getLatitude() + "\n getLongitude" + locGeoPoint.getLongitude() + "\noverlayItemArray" + overlayItemArray.size());
                mapView.invalidate();
            }

            private LocationListener myLocationListener
                    = new LocationListener() {

                @Override
                public void onLocationChanged(Location location) {
                    // TODO Auto-generated method stub
                    updateLoc(location);
                }

                @Override
                public void onProviderDisabled(String provider) {
                    // TODO Auto-generated method stub

                }

                @Override
                public void onProviderEnabled(String provider) {
                    // TODO Auto-generated method stub

                }

                @Override
                public void onStatusChanged(String provider, int status, Bundle extras) {
                    // TODO Auto-generated method stub

                }

            };

        }
