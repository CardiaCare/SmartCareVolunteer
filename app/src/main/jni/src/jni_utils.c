//
// Created by nikolay on 20.01.16.
//
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <jni.h>
#include "jni_utils.h"
#include "globals.h"

int init_global_instances(JNIEnv* env, jobject obj){

    GlobalMainActivityClassObject = (jobject *)(*env)->NewGlobalRef(env, obj);
    if( NULL == GlobalMainActivityClassObject ){
        return -1;
    }

    jclass localMainActivityClass = (*env)->FindClass(env, "com/petrsu/cardiacare/smartcarevolunteer/MainActivity");
    if( NULL == localMainActivityClass ){
        return -1;
    }
    GlobalMainActivityClass = (jclass *)(*env)->NewGlobalRef(env, localMainActivityClass);
    if( NULL == GlobalMainActivityClass ){
        return -1;
    }

    jclass localPatientHelpActivityClass = (*env)->FindClass(env, "com/petrsu/cardiacare/smartcarevolunteer/PatientHelpActivity");
    if( NULL == localPatientHelpActivityClass ){
        return -1;
    }
    GlobalPatientHelpActivityClass = (jclass *)(*env)->NewGlobalRef(env, localPatientHelpActivityClass);
    if( NULL == GlobalPatientHelpActivityClass ){
        return -1;
    }

    return 0;
}
int init_global_patient_help_activity_object(JNIEnv* env, jobject obj){
    GlobalPatientHelpActivityClassObject = (jobject *)(*env)->NewGlobalRef(env, obj);
    if( NULL == GlobalPatientHelpActivityClassObject ){
        return -1;
    }
    return 0;
}
int init_JVM_instance(JNIEnv* env){
    if((*env)->GetJavaVM(env, &JVM) != 0){
        return -1;
    }
    return 0;
}