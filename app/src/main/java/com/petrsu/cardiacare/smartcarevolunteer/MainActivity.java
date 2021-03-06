package com.petrsu.cardiacare.smartcarevolunteer;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.AsyncTask;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;

import android.util.Log;

import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;


public class MainActivity extends AppCompatActivity {

    public MainActivity() {
    }
    // Native code part begin
    static{
        System.loadLibrary("smartcare");
    }
    /*
    * Common functions for interaction with SmartSpace
     */
    public native long connectSmartSpace(String name, String ip, int port);
    public native void disconnectSmartSpace(long nodeDescriptor);
    public native String initVolunteer (long nodeDescriptor);
    public native String initLocation (long nodeDescriptor, String patientUri);

    public native String initAuthRequest (long nodeDescriptor, String patientUri);
    public native int getAuthResponce(long nodeDescriptor, String authUri);
    /*
    * here we describe functions for subscribing to list of patients
    * firstly, we get initial patients' list
    * secondly, we run waiting for inserting new patients into SIB
    */
    public native long initPatientListSbscr(long nodeDescriptor, String volunteerUri);
    public native long helpRequestAnswer(long nodeDescriptor, String volunteerUri, String answer);

    public native String getPatientUriWithAlarm(long nodeDescriptor, String volunteerUri);

    static public native int initCurrentPatientLocationSbcr(long nodeDescriptor, String needPatientUri, PatientHelpActivity obj);

    public native void updatePatientList(long nodeDescriptor, long patientSbcrDescriptor);
    public native void stopPatientListSbscr(long nodeDescriptor, long patientSbcrDescriptor);
    /*
    * here we describe functions for subscribing to alarms
    */
    public native long initAlarmSbcr(long nodeDescriptor, String volunteerUri);
    public native void updateAlarmList(long nodeDescriptor, long alarmSbcrDescriptor, String volunteerUri);
    public native void stopAlarmListSbcr(long nodeDescriptor, long alarmSbcrDescriptor);

    static public native int insertPersonName(long nodeDescriptor, String volunteerUri, String name);
    static public native int updatePersonName(long nodeDescriptor, String volunteerUri, String name);

    static public native int setVolunteerStatus(long nodeDescriptor, String volunteerUri, String name);

    public native void removeIndividual (long nodeDescriptor, String individualUri);
    /*
    * Location-service functions
    */
    static public native int  sendLocation(long nodeDescriptor, String patientUri , String locationUri ,String latitude, String longitude);
    /*
    * Descriptors - native pointers' indentifiers.
    */
    static protected long nodeDescriptor;
    static protected long patientSbcrDescriptor;
    static protected long alarmSbcrDescriptor;
    /*
    * Apllication owner URIs
    */
    static protected String volunteerUri;
    static protected String locationUri;
    static protected String authUri;
    static protected String needPatientUri;

    // Native code part end


    Toolbar mToolbar;
    Button alarmButton;
    Button nextButton;
    EditText etFirstName;
    EditText etSecondName;
    AccountStorage storage;
    SwipeRefreshLayout mSwipeRefreshLayout;
    private ViewPager viewPager;

    Timer timer;
    int slide = 0;

    protected static final String TAG = "location";

    private LocationService gps;
    // for patients list
    private RecyclerView recyclerView;
    private PatientListAdapter plAdapter;
    private List<PatientListItemData> plData;
    PatientListTask patientList;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        /*****************************
         * SS init
         *****************************/
        nodeDescriptor = connectSmartSpace("X", "78.46.130.194", 10010);
        if (nodeDescriptor == -1){
            return;
        }
        volunteerUri = initVolunteer(nodeDescriptor);
        if (volunteerUri == null){
            return;
        }

        ///authUri = initAuthRequest(nodeDescriptor, volunteerUri);
        //if (authUri == null){
        //    return;
       // }

//        if (getAuthResponce(nodeDescriptor, authUri) != 0){
//            AlertDialog.Builder builder =
//                    new AlertDialog.Builder(MainActivity.this, R.style.AppCompatAlertDialogStyle);
//            builder.setTitle(R.string.auth_dialog_title);
//            builder.setMessage(R.string.auth_dialog_message);
//            builder.setPositiveButton(R.string.dialog_ok, new DialogInterface.OnClickListener() {
//                @Override
//                public void onClick(DialogInterface dialog, int which) {
//                    onDestroy();
//                }
//            });
//            builder.show();
//        }


        locationUri = initLocation(nodeDescriptor, volunteerUri);
        if (locationUri == null) {
            return;
        }



        /*****************************
         * get lastKnown location
         *****************************/
         gps = new LocationService(MainActivity.this);
         if(gps.canGetLocation() != false) {
             double latitude = gps.getLatitude();
             double longitude = gps.getLongitude();
             sendLocation(nodeDescriptor, volunteerUri, locationUri, Double.toString(latitude), Double.toString(longitude));
         }else{
             //can't get location
             //GPS or Network is not enabled
             //Ask user to enable GPS/network in settings
             gps.showSettingsAlert();
         }

        //
        storage = new AccountStorage();
        storage.sPref = getSharedPreferences(storage.ACCOUNT_PREFERENCES,  MODE_PRIVATE);

        if (storage.getAccountFirstName().isEmpty() || storage.getAccountSecondName().isEmpty()){
            setUnregisteredActivity();
        }else{
           setRegisteredActivity();
        }

    }

    public void setRegisteredActivity(){
        setContentView(R.layout.activity_main);

        mToolbar = (Toolbar) findViewById(R.id.toolbar);
        //TODO : Error
        setSupportActionBar(mToolbar);
        getSupportActionBar().setDisplayShowHomeEnabled(true);

        storage.sPref = getSharedPreferences(storage.ACCOUNT_PREFERENCES, MODE_PRIVATE);
        insertPersonName(nodeDescriptor, volunteerUri, storage.getAccountFirstName() + " " + storage.getAccountSecondName());

        //Init alarm sbcr
        initPatientListView();

        viewPager = (ViewPager) findViewById(R.id.container);
        viewPager.setAdapter(new CustomPagerAdapter(this));
        viewPager.addOnPageChangeListener(new ViewPager.SimpleOnPageChangeListener(){
            @Override
            public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
                slide = position;
            }
        });

        slideSwitcher(6);

//        mSwipeRefreshLayout = (SwipeRefreshLayout) findViewById(R.id.swiperefresh);
//        mSwipeRefreshLayout.setOnRefreshListener(new SwipeRefreshLayout.OnRefreshListener() {
//            @Override
//            public void onRefresh() {
//                refreshAll();
//            }
//        });
    }

    public void setUnregisteredActivity(){
        setContentView(R.layout.activity_main_account_connection);

        mToolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(mToolbar);
        getSupportActionBar().setDisplayShowHomeEnabled(true);

        etFirstName = (EditText) findViewById(R.id.etFirstName);
        etSecondName = (EditText) findViewById(R.id.etSecondName);

        etSecondName.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int id, KeyEvent keyEvent) {
                if (id == EditorInfo.IME_NULL) {
                    registration(etFirstName.getText().toString(), etSecondName.getText().toString());
                    return true;
                }
                return false;
            }
        });


        nextButton = (Button)findViewById(R.id.nextButton);
        nextButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                registration(etFirstName.getText().toString(), etSecondName.getText().toString());
            }
        });
    }

    public void registration(String first, String second){
        if (first.isEmpty() ||second.isEmpty()) {
            AlertDialog.Builder builder =
                    new AlertDialog.Builder(MainActivity.this, R.style.AppCompatAlertDialogStyle);
            builder.setTitle(R.string.dialog_title);
            builder.setMessage(R.string.dialog_message);
            builder.setPositiveButton(R.string.dialog_ok, null);
            builder.setNegativeButton(R.string.dialog_cancle, null);
            builder.show();
        } else {
            storage.sPref = getSharedPreferences(storage.ACCOUNT_PREFERENCES, MODE_PRIVATE);
            storage.setAccountPreferences(first,second, "", "", "", "");
            setRegisteredActivity();
        }
    }

    public void initPatientListView(){

//        recyclerView = (RecyclerView) findViewById(R.id.recyclerView);
//        recyclerView.setLayoutManager(new LinearLayoutManager(this));
//
//        plData = new ArrayList<>();
//        plAdapter = new PatientListAdapter(plData);
//        recyclerView.setAdapter(plAdapter);
//
//        //animation
//        recyclerView.setItemAnimator(new DefaultItemAnimator());
//
        if (initPatientListSbscr(nodeDescriptor, volunteerUri) == -1){
           //FIXME
            return;
        }
    }


    /*
    * get new healp request notifitation
    * TODO: Add alarm paramerter (status, code, boolean?)
    */
    public void getHelpRequestNotification(){
        Log.i(TAG, "in java getHelpRequeset");
        runOnUiThread(new Thread(new Runnable() {
            public void run() {
                AlertDialog.Builder builder =
                        new AlertDialog.Builder(MainActivity.this, R.style.AppCompatAlertDialogStyle);
                builder.setTitle(R.string.help_dialog_title);
                builder.setMessage(R.string.help_dialog_message);
                builder.setPositiveButton(R.string.dialog_confirm, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        helpRequestAnswer(nodeDescriptor, volunteerUri, "CONFIRMED");
                        needPatientUri = getPatientUriWithAlarm(nodeDescriptor, volunteerUri);
                        if(needPatientUri == null) {
                            Log.e(TAG, "Error! Patient not found!");
                        } else {
                            Log.i(TAG, "intent PatientHelpActivity");
                            Intent intentPatientHelpActivity = new Intent(getApplicationContext(), PatientHelpActivity.class);
                            startActivity(intentPatientHelpActivity);
                        }
                    }
                });
                builder.setNegativeButton(R.string.dialog_reject, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        helpRequestAnswer(nodeDescriptor, volunteerUri, "REJECTED");
                    }
                });
                builder.show();
            }
        }));
    }

    public void changePatientLocation(String latitude, String longitude){
        Log.i(TAG, latitude);
        Log.i(TAG, longitude);
    }

    /*
    * this adds new elements to recyclerView and update it
    * TODO: Replace patientURI to something useful, e.g. name, surname...
    * А лучше передавать сразу объект со всеми данными пациента, которые
    * будут выводиться в списке. Формировать объект, наверное, лучше
    * в другом месте
    */
    public void addPatient(String patientUri, String patientName) {

        boolean patientExist = false;
        // Add data locally to the list.
        for(PatientListItemData patient : plData){
            if(patientUri.equals(patient.getSsPatientUri())){
                patientExist = true;
            }
        }
        if(!patientExist) {
            PatientListItemData patient = new PatientListItemData();
            patient.setSsPatientUri(patientUri);
            patient.setTitle(patientName);
            patient.setImageUrl(R.drawable.ic_android);

            plData.add(patient);
            plAdapter.notifyDataSetChanged();
        }
    }
    /*
    * this update patient record in recyclerView, e.g. add alarmIMG or smth else
    * TODO: Add alarm paramerter (status, code, boolean?)
    */
    public void updatePatient(String patientUri){
        for(PatientListItemData patient : plData){
            if(patientUri.equals(patient.getSsPatientUri())){
                Log.i("SS", "Start to Change patient con");
                patient.setImageUrl(R.drawable.ic_notifications_black_24dp);
            }
        }
        plAdapter.notifyDataSetChanged();
    }
    public void updatePatientList(Patient patient, String action){

        switch(action){
            case "create":
                //this.addPatient(patient.uri, patient.name);
                break;
            case "update":
                //this.updatePatient(patient);
                break;
            case "remove":
                //this.removePatient(patient.uri);
                break;
        }
        plAdapter.notifyDataSetChanged();
    }
    /*
    *TODO: ТУт должна быть такая же функция для удаления элементов из списка
    * Ее я еще не нагуглил, а писать в 2 часа ночи мне лень :)
    *
    */
    public void removePatient(View view) {
        //
    }
    /*
    * Alarm request
     */
    public void alarmRequestConfirmation(){
        AlertDialog.Builder builder =
                new AlertDialog.Builder(MainActivity.this, R.style.AppCompatAlertDialogStyle);
        builder.setTitle("New Alarm");
        builder.setMessage("It is decided that you are the best volunteer to help");
        builder.setPositiveButton(R.string.dialog_ok, null);
        builder.setNegativeButton(R.string.dialog_cancle, null);
        builder.show();
    }
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            Intent intentAccount = new Intent(this, MyPreferencesActivity.class);
            startActivity(intentAccount);
        }

        return super.onOptionsItemSelected(item);
    }

    public void refreshAll() {
                removeIndividual(nodeDescriptor, locationUri);
                //removeIndividual(nodeDescriptor, authUri);
                //TODO delete alarm
                //removeIndividual(nodeDescriptor, alarmUri);
                removeIndividual(nodeDescriptor, volunteerUri);

                disconnectSmartSpace(nodeDescriptor);

                nodeDescriptor = connectSmartSpace("X", "78.46.130.194", 10010);
                if (nodeDescriptor == -1){
                    return;
                }
                volunteerUri = initVolunteer(nodeDescriptor);
                if (volunteerUri == null){
                    return;
                }

//                authUri = initAuthRequest(nodeDescriptor, volunteerUri);
//                if (authUri == null){
//                    return;
//                }
//
//                if (getAuthResponce(nodeDescriptor, authUri) != 0){
//                    AlertDialog.Builder builder =
//                            new AlertDialog.Builder(MainActivity.this, R.style.AppCompatAlertDialogStyle);
//                    builder.setTitle(R.string.auth_dialog_title);
//                    builder.setMessage(R.string.auth_dialog_message);
//                    builder.setPositiveButton(R.string.dialog_ok, new DialogInterface.OnClickListener() {
//                        @Override
//                        public void onClick(DialogInterface dialog, int which) {
//                            onDestroy();
//                        }
//                    });
//                    builder.show();
//                }

                locationUri = initLocation(nodeDescriptor, volunteerUri);
                if (locationUri == null) {
                    return;
                }

                if(gps.canGetLocation() != false) {
                    double latitude = gps.getLatitude();
                    double longitude = gps.getLongitude();
                    sendLocation(nodeDescriptor, volunteerUri, locationUri, Double.toString(latitude), Double.toString(longitude));
                }else{
                    gps.showSettingsAlert();
                }
                initPatientListView();
                mSwipeRefreshLayout.setRefreshing(false);
    }


    public void onDestroy() {
        moveTaskToBack(true);
        super.onDestroy();

        removeIndividual(nodeDescriptor, locationUri);
        //TODO delete alarm
        //removeIndividual(nodeDescriptor, authUri);
        removeIndividual(nodeDescriptor, volunteerUri);

        gps.stopUsingGPS();
        if (patientList != null) {
            patientList.cancel(true);
        }

        disconnectSmartSpace(nodeDescriptor);
        System.exit(0);
    }

    class PatientListTask extends AsyncTask<Void, Void, Void> {

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            patientSbcrDescriptor = initPatientListSbscr(nodeDescriptor, volunteerUri);
            if (patientSbcrDescriptor == -1){
                //FIXME
                return;
            }
        }
        @Override
        protected Void doInBackground(Void... params) {
            Log.i("SS", "patient doInBackground");

            if (patientSbcrDescriptor == -1){
                 Log.i("SS", "patient DIB NULL");
            }
            else {
                updatePatientList(nodeDescriptor, patientSbcrDescriptor);
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            stopPatientListSbscr(nodeDescriptor, patientSbcrDescriptor);
        }
    }

    class AlarmTask extends AsyncTask<Void, Void, Void> {

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            alarmSbcrDescriptor = initAlarmSbcr(nodeDescriptor, volunteerUri);
            if(alarmSbcrDescriptor == -1){
                //FIXME
                return;
            }
        }

        @Override
        protected Void doInBackground(Void... params) {
            Log.i("SS", "alarmDIB");
            updateAlarmList(nodeDescriptor, alarmSbcrDescriptor, volunteerUri);
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            stopAlarmListSbcr(nodeDescriptor, alarmSbcrDescriptor);
        }
    }

    /*****************************************************************
     *  Slides part
     ****************************************************************/

    public enum CustomPagerEnum {

        FIRST(R.layout.fragment_first),
        SECOND(R.layout.fragmen_second),
        THIRD(R.layout.fragmen_third),
        FOUR(R.layout.fragmen_four),
        FIFE(R.layout.fragmen_fife),
        SIX(R.layout.fragmen_six);

        private int mLayoutResId;

        CustomPagerEnum(int layoutResId) {
            mLayoutResId = layoutResId;
        }

        public int getLayoutResId() {
            return mLayoutResId;
        }

    }

    public class CustomPagerAdapter extends PagerAdapter {

        private Context mContext;

        public CustomPagerAdapter(Context context) {
            mContext = context;
        }

        @Override
        public Object instantiateItem(ViewGroup collection, int position) {
            CustomPagerEnum customPagerEnum = CustomPagerEnum.values()[position];
            LayoutInflater inflater = LayoutInflater.from(mContext);
            ViewGroup layout = (ViewGroup) inflater.inflate(customPagerEnum.getLayoutResId(), collection, false);
            collection.addView(layout);
            return layout;
        }

        @Override
        public void destroyItem(ViewGroup collection, int position, Object view) {
            collection.removeView((View) view);
        }

        @Override
        public int getCount() {
            return CustomPagerEnum.values().length;
        }

        @Override
        public boolean isViewFromObject(View view, Object object) {
            return view == object;
        }

    }

    public void slideSwitcher(int seconds) {
        timer = new Timer(); // At this line a new Thread will be created
        timer.scheduleAtFixedRate(new SwitchSlidesTask(), 0, seconds * 1000); // delay
        // in
        // milliseconds
    }

    class SwitchSlidesTask extends TimerTask {

        @Override
        public void run() {
            runOnUiThread(new Runnable() {
                public void run() {

                    if (slide > 5) {
                        slide = 0;
                        viewPager.setCurrentItem(slide++);
                    } else {
                        viewPager.setCurrentItem(slide++);
                    }
                }
            });

        }
    }
}

