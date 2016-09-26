
/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdlib.h>
#include <string.h>
#include <jni.h>
#include "ontology/smartcare.h"
#include <android/log.h>
#include <errno.h>
#include "agent.h"

#include "handlers.h"
#include "jni_utils.h"
#include "globals.h"

#define TAG "SS"
#define KP_NS_URI "http://oss.fruct.org/smartcare#"
#define KP_PREDICATE KP_NS_URI"sendAlarm"

/*
 *   Подключаемся к Интелектуальному пространству
 *  SmartSpace node initialization by hostname_, ip_,port
 */
JNIEXPORT jlong JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_connectSmartSpace( JNIEnv* env,
                                                  jobject thiz , jstring hostname, jstring ip, jint port) {

    const char *hostname_ = (*env)->GetStringUTFChars(env, hostname, NULL);
    if( hostname_ == NULL) {
        return -1;
    }
    const char *ip_ = (*env)->GetStringUTFChars(env, ip, NULL);
    if( ip_ == NULL ){
        return -1;
    }
    long node = kp_connect_smartspace( hostname_, ip_, port);

    if (node == -1) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Can't join to SS");
        return -1;
    } else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "KP joins to SS.");
    }
    /*
     * Initialize global references to java classes
     * to use'em in handlers etc.
     */
    if( -1 == init_global_instances(env, thiz) ) {
        return -1;
    }
    /*
     * Get Java Virtual Machine Instance
     * to use it in callbacks generally
     */
    if( -1 == init_JVM_instance(env) ){
        return -1;
    }
    /*
     * Return node descriptor
     * to use it further in subscriptions etc.
     */
    return (jlong) node;

}
/*
 *  Disconnect from smartspace
 *
 */
JNIEXPORT void JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_disconnectSmartSpace( JNIEnv* env,
                                                  jobject thiz , jlong nodeDescriptor){
    int result = kp_disconnect_smartspace(nodeDescriptor);

    if (result == -1)
        __android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
    else
        __android_log_print(ANDROID_LOG_INFO, TAG, "KP leaves SS...");
}

/*
 *  Init volunteer individual + person individual
 *
 */
JNIEXPORT jstring JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_initVolunteer
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor)
{
    char *volunteer_uri;

    if( -1 != kp_init_volunteer(&volunteer_uri, nodeDescriptor) ){
        __android_log_print(ANDROID_LOG_INFO, TAG, "volunteer: %s", volunteer_uri);
    } else {
        return NULL;
    }

    return (*env)->NewStringUTF(env, volunteer_uri);
}

/*
 * Удаляем произвольного индивида по uri
 *
*/
JNIEXPORT void JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_removeIndividual
( JNIEnv* env, jobject thiz , jlong nodeDescriptor, jstring individualUri){

    const char * individual_uri= (*env)->GetStringUTFChars(env, individualUri, 0);
    kp_remove_individual(nodeDescriptor, individual_uri);
}

JNIEXPORT jstring JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_initAuthRequest
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri){

    const char * volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    char *auth_request_uri;

    if(-1 != kp_init_auth_request(&auth_request_uri, nodeDescriptor, volunteer_uri) ){
        return (*env)->NewStringUTF(env, auth_request_uri);
    } else {
        return NULL;
    }

}

JNIEXPORT jint JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_getAuthResponce
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring authDescriptor) {
    //get node from SmartSpace
    sleep(5);

    const char * auth_uri= (*env)->GetStringUTFChars(env, authDescriptor, 0);

    int result = kp_get_auth_responce(nodeDescriptor, auth_uri);
    if (result == -1) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "NOT PASSED");
        return -1;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "PASSED");
        return 0;
    }
}

/*
 *  Push person name to SIB
 *
 */
JNIEXPORT jint JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_insertPersonName
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri, jstring name)
{
    const char *volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    const char *new_name = (*env)->GetStringUTFChars(env, name, 0);

    int result = kp_insert_person_name( nodeDescriptor, volunteer_uri, new_name );
    if (result == -1) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Insert person name");
        return 0;
    }
}

/*
 * Обновление имени при его изменении в настройках
 *
*/
JNIEXPORT jint JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_updatePersonName
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri, jstring name) {
    const char *volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    const char *new_name = (*env)->GetStringUTFChars(env, name, 0);

    int result = kp_update_person_name(nodeDescriptor, volunteer_uri, new_name);

    if (result == -1) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Update person name");
        return 0;
    }
}

/*
 * Initialize patients list when volunteer run app at first time
 * Update patients list subscription
 *
 */
JNIEXPORT jlong JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_initPatientListSbscr
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri)
{
    const char * volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    if(-1 != kp_init_patient_list_sbscr(nodeDescriptor, volunteer_uri) ){
        return 0;
    }
}



JNIEXPORT jlong JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_helpRequestAnswer
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri, jstring answer)
{

    const char * volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    const char *new_status = (*env)->GetStringUTFChars(env, answer, 0);

    if( -1 == kp_answer_help_request(nodeDescriptor, volunteer_uri, new_status) ){
        return -1;
    }
    return 0;
}

/*
 * Инициализируем индивид локации (возвращает сгенерированный ЮРИ локации)
 *
 */
JNIEXPORT jstring JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_initLocation
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring individualUri)
{
    //get individual from SmartSpace
    const char * individual_uri= (*env)->GetStringUTFChars(env, individualUri, 0);

    char*  location_uri;

    if(-1 != kp_init_location (&location_uri, nodeDescriptor, individual_uri) ){
    return (*env)->NewStringUTF(env, location_uri);
    } else {
        return NULL;
    }
}
/*
 *
 * Отпралвяем локацию. В разработке
 */
JNIEXPORT jint JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_sendLocation
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri, jstring locationUri, jstring latitudeJ, jstring longitudeJ)
{
    const char *volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    const char * location_uri= (*env)->GetStringUTFChars(env, locationUri, 0);
    const char *latitude = (*env)->GetStringUTFChars(env, latitudeJ, 0);
    const char *longitude = (*env)->GetStringUTFChars(env, longitudeJ, 0);

    int result = kp_send_location(nodeDescriptor, volunteer_uri, location_uri, latitude, longitude);
    if (result == -1) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Send Location");
        return 0;
    }
}

/*
 * Get patient after alarm confirming
 */
JNIEXPORT jstring JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_getPatientUriWithAlarm
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri)
{

    const char * volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    char *patient_uri;
    if( -1 == kp_get_patient_uri_by_alarm(&patient_uri, nodeDescriptor, volunteer_uri)){
        return NULL;
    }

    return (*env)->NewStringUTF(env, patient_uri);
}

JNIEXPORT jint JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_initCurrentPatientLocationSbcr
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring patientUri, jobject obj)
{
    const char * patient_uri = (*env)->GetStringUTFChars(env, patientUri, 0);

    if( -1 == init_global_patient_help_activity_object(env, obj) ){
        return -1;
    }

    if( -1 == kp_get_current_patient_location(nodeDescriptor, patient_uri) ){
        return -1;
    }

    if( -1 == kp_init_current_patient_location_sbcr(nodeDescriptor, patient_uri)){
        return -1;
    }

    return 0;

}
JNIEXPORT jint JNICALL Java_com_petrsu_cardiacare_smartcarevolunteer_MainActivity_setVolunteerStatus
        (JNIEnv *env, jobject thiz, jlong nodeDescriptor, jstring volunteerUri, jstring status)
{
        const char *volunteer_uri= (*env)->GetStringUTFChars(env, volunteerUri, 0);
    const char *new_status = (*env)->GetStringUTFChars(env, status, 0);

    int result = kp_set_volunteer_status(nodeDescriptor, volunteer_uri, new_status);

    if (result == -1) {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }
    else {
        __android_log_print(ANDROID_LOG_INFO, TAG, "Update person name");
        return 0;
    }

}
