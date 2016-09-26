package com.petrsu.cardiacare.smartcarevolunteer;

/**
 * Created by Iuliia Zavialova on 12.04.16.
 */
import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.Preference;
import android.preference.PreferenceFragment;
import android.support.v7.app.ActionBarActivity;
import android.util.Log;

public class MyPreferencesActivity extends ActionBarActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        android.support.v7.widget.Toolbar toolbar = (android.support.v7.widget.Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        getFragmentManager().beginTransaction().replace(R.id.content_frame, new MyPreferenceFragment()).commit();
    }


    public static class MyPreferenceFragment extends PreferenceFragment
    {

        @Override
        public void onCreate(final Bundle savedInstanceState)
        {
            super.onCreate(savedInstanceState);

            addPreferencesFromResource(R.xml.preferences);

            final CheckBoxPreference checkboxPref = (CheckBoxPreference) getPreferenceManager().findPreference("applicationUpdates");

            checkboxPref.setOnPreferenceChangeListener(new Preference.OnPreferenceChangeListener() {
                public boolean onPreferenceChange(Preference preference, Object newValue) {
                    Log.d("MyApp", "Pref " + preference.getKey() + " changed to " + newValue.toString());
                    MainActivity.setVolunteerStatus(MainActivity.nodeDescriptor, MainActivity.volunteerUri, "READY TO HELP");
                    return true;
                }
            });
        }
    }

}