package com.petrsu.cardiacare.smartcarevolunteer;

import android.app.Activity;
import android.content.SharedPreferences;

/**
 * Created by cardiacare on 17.09.15.
 */
public class AccountStorage {

    SharedPreferences sPref;

    public static final String ACCOUNT_PREFERENCES = "accountsettings";

    public static final String ACCOUNT_PREFERENCES_FIRSTNAME = "firstname";
    public static final String ACCOUNT_PREFERENCES_SECONDNAME = "secondname";
    public static final String ACCOUNT_PREFERENCES_PHONENUMBER = "phonenumber";
    public static final String ACCOUNT_PREFERENCES_HEIGHT = "height";
    public static final String ACCOUNT_PREFERENCES_WEIGHT = "weight";
    public static final String ACCOUNT_PREFERENCES_AGE = "age";

    String strFirstName;
    String strSecondName;
    String strPhoneNumber;
    String strHeight;
    String strWeight;
    String strAge;


    public void setAccountPreferences(String name, String second, String phone, String heih, String weig, String age){


        SharedPreferences.Editor editor = sPref.edit();

        editor.putString(ACCOUNT_PREFERENCES_FIRSTNAME, name);
        editor.putString(ACCOUNT_PREFERENCES_SECONDNAME, second);
        editor.putString(ACCOUNT_PREFERENCES_PHONENUMBER, phone);
        editor.putString(ACCOUNT_PREFERENCES_HEIGHT, heih);
        editor.putString(ACCOUNT_PREFERENCES_WEIGHT, weig);
        editor.putString(ACCOUNT_PREFERENCES_AGE, age);
        editor.commit();

        MainActivity.insertPersonName(MainActivity.nodeDescriptor, MainActivity.volunteerUri, name + " "+ second);
    }

    public String getAccountFirstName(){
        //sPref = getSharedPreferences(ACCOUNT_PREFERENCES,  MODE_PRIVATE);
        if (sPref.contains(ACCOUNT_PREFERENCES_FIRSTNAME)) {
            strFirstName = sPref.getString(ACCOUNT_PREFERENCES_FIRSTNAME, "");
        }
        else strFirstName = "";
        return strFirstName;
    }

    public String getAccountSecondName(){
        //sPref = getSharedPreferences(ACCOUNT_PREFERENCES,  MODE_PRIVATE);
        if (sPref.contains(ACCOUNT_PREFERENCES_SECONDNAME)) {
            strSecondName = sPref.getString(ACCOUNT_PREFERENCES_SECONDNAME, "");
        }
        else strSecondName = "";
        return strSecondName;
    }

    public String getAccountPhoneNumber(){
        //sPref = getSharedPreferences(ACCOUNT_PREFERENCES, MODE_PRIVATE);
        if (sPref.contains(ACCOUNT_PREFERENCES_PHONENUMBER)) {
            strPhoneNumber = sPref.getString(ACCOUNT_PREFERENCES_PHONENUMBER, "");
        }
        else   strPhoneNumber = "";
        return strPhoneNumber;
    }

    public String getAccountHeight(){
        //sPref = getSharedPreferences(ACCOUNT_PREFERENCES, MODE_PRIVATE);
        if (sPref.contains(ACCOUNT_PREFERENCES_HEIGHT)) {
            strHeight = sPref.getString(ACCOUNT_PREFERENCES_HEIGHT, "");
        }else strHeight = "";
    return strHeight;
    }

    public String getAccountWeight(){
        //sPref = getSharedPreferences(ACCOUNT_PREFERENCES, MODE_PRIVATE);
        if (sPref.contains(ACCOUNT_PREFERENCES_WEIGHT)) {
            strWeight = sPref.getString(ACCOUNT_PREFERENCES_WEIGHT, "");
        } else strWeight = "";
    return strWeight;
    }
    public String getAccountAge(){
        //sPref = getSharedPreferences(ACCOUNT_PREFERENCES, MODE_PRIVATE);
        if (sPref.contains(ACCOUNT_PREFERENCES_AGE)) {
            strAge = sPref.getString(ACCOUNT_PREFERENCES_AGE, "");
        }else strAge = "";
        return strAge;
    }
}
