//
// Created by nikolay on 19.01.16.
//
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "handlers.h"
#include <stdbool.h>
#include <android/log.h>
#include <jni.h>
#include "globals.h"

#define TAG "SS"

void help_request_notification(){
    __android_log_print(ANDROID_LOG_INFO, TAG,"in help_request_notification");


    JNIEnv *env = NULL;
    bool attached = JNI_FALSE;

    if(JVM == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "JVM NULL");
        return;
    }
    /* Initialize `env` pointer in current thread */
    switch((*JVM)->GetEnv(JVM, (void **)&env, JNI_VERSION_1_6)) {
        case JNI_OK:
            __android_log_print(ANDROID_LOG_INFO, TAG, "ENV OK");
            break;
        case JNI_EDETACHED:
            __android_log_print(ANDROID_LOG_INFO, TAG, "ENV DETACHED");
            (*JVM)->AttachCurrentThread(JVM, &env, NULL);
            __android_log_print(ANDROID_LOG_INFO, TAG, "ENV ATTACHED");
            attached = JNI_TRUE;
            break;
        case JNI_EVERSION:
            __android_log_print(ANDROID_LOG_ERROR, TAG, "invalid JNI version");
            break;
    }
    if(env == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "env NULL");
        return;
    }

    if(GlobalMainActivityClass == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "MainActivity class is NULL");
        return;
    }

    jmethodID getHelpRequestNotification = (*env)->GetMethodID(env, GlobalMainActivityClass, "getHelpRequestNotification", "()V");
    if(getHelpRequestNotification == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "method is NULL");
        return;
    }

    (*env)->CallVoidMethod(env, GlobalMainActivityClassObject,
                                   getHelpRequestNotification);

    if(attached)
        (*JVM)->DetachCurrentThread(JVM);
}
void change_patient_location_handler(char* latitude, char* longitude){
    __android_log_print(ANDROID_LOG_INFO, TAG,"in change_patient_location_handler");
    JNIEnv *env = NULL;

    bool attached = JNI_FALSE;
    /* Initialize `env` pointer in current thread */
    switch((*JVM)->GetEnv(JVM, (void **)&env, JNI_VERSION_1_6)) {
        case JNI_OK:
            __android_log_print(ANDROID_LOG_INFO, TAG, "ENV OK");
            break;
        case JNI_EDETACHED:
            __android_log_print(ANDROID_LOG_INFO, TAG, "ENV DETACHED");
            (*JVM)->AttachCurrentThread(JVM, &env, NULL);
            __android_log_print(ANDROID_LOG_INFO, TAG, "ENV ATTACHED");
            attached = JNI_TRUE;
            break;
        case JNI_EVERSION:
            __android_log_print(ANDROID_LOG_ERROR, TAG, "invalid JNI version");
            break;
    }
    if(env == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "env NULL");
        return;
    }

    if(GlobalPatientHelpActivityClass == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "PatientHelpActivity class is NULL");
        return;
    }


    jmethodID changePatientLocation = (*env)->GetMethodID(env, GlobalPatientHelpActivityClass, "changePatientLocation", "(Ljava/lang/String;Ljava/lang/String;)V");
    if(changePatientLocation == NULL){
        return;
    }


    (*env)->CallVoidMethod(
            env, GlobalPatientHelpActivityClassObject,
            changePatientLocation,
            (*env)->NewStringUTF(env, latitude),
            (*env)->NewStringUTF(env, longitude)
            );

}
void patient_location_update(const list_t *new_location){
    __android_log_print(ANDROID_LOG_INFO, TAG,"in patient_location_update");

    JNIEnv *env = NULL;

    bool attached = JNI_FALSE;

    /* Initialize `env` pointer in current thread */
    switch((*JVM)->GetEnv(JVM, (void **)&env, JNI_VERSION_1_6)) {
        case JNI_OK:
            break;

        case JNI_EDETACHED:
            (*JVM)->AttachCurrentThread(JVM, &env, NULL);
            attached = JNI_TRUE;
            break;

        case JNI_EVERSION:
            __android_log_print(ANDROID_LOG_ERROR, TAG, "invalid JNI version");
            break;
    }

    if(GlobalPatientHelpActivityClass == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "GlobalPatientHelpActivityClass NULL");
        return;
    }

    jmethodID changePatientLocation = (*env)->GetMethodID(env, GlobalPatientHelpActivityClass, "changePatientLocation", "(Ljava/lang/String;Ljava/lang/String;)V");

    if(changePatientLocation == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "changePatientLocation NULL");
        return;
    }

    if(GlobalPatientHelpActivityClassObject == NULL){
        __android_log_print(ANDROID_LOG_INFO, TAG, "patientHelpActivityClassObject NULL");
        return;
    }

    list_head_t *iterator = NULL;

    list_for_each(iterator, &new_location->links) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "in list_for_each");
        list_t *list_node = list_entry(iterator, list_t, links);

        sslog_triple_t *location_triple =  (sslog_triple_t *) list_node->data;
        char  *location_uri = location_triple->subject;
        sslog_individual_t *location = sslog_get_individual(location_uri);
        char *latitude =  sslog_node_get_property(GlobalNode, location, PROPERTY_LAT);
        char *longitude =  sslog_node_get_property(GlobalNode, location, PROPERTY_LONG);

        __android_log_print(ANDROID_LOG_INFO, TAG, "new lat %s", latitude);
        __android_log_print(ANDROID_LOG_INFO, TAG, "new long %s", longitude);
        __android_log_print(ANDROID_LOG_INFO, TAG, "trying to enter java changePatientLocation");
        if(latitude != NULL && longitude != NULL){
            (*env)->CallVoidMethod(env, GlobalPatientHelpActivityClassObject,
                                   changePatientLocation, (*env)->NewStringUTF(env, latitude), (*env)->NewStringUTF(env, longitude));
        }
    }

    if(attached)
        (*JVM)->DetachCurrentThread(JVM);
}