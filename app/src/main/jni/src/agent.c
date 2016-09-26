//
// Created by nikolay on 13.01.16.
//

#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "agent.h"
#include "handlers.h"
#include "globals.h"


/*
 * Connecting to smartspace
 */
long kp_connect_smartspace(char *hostname_,char *ip_, int port){
    sslog_node_t *node;
    sslog_init();

    //__android_log_print(ANDROID_LOG_INFO, TAG, "sslog_init");

    node = sslog_new_node("KP_Volunteer", hostname_, ip_, port);
    /*
     * Delete this line when test
     */
    GlobalNode = node;

    register_ontology(node);

    //__android_log_print(ANDROID_LOG_INFO, TAG, "register_ontology");

    if (sslog_node_join(node) != SSLOG_ERROR_NO) {
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Can't join to SS");
        return -1;
    }
    else{
        //__android_log_print(ANDROID_LOG_INFO, TAG, "KP joins to SS.");
        return (long) node;
    }
}

int kp_init_volunteer(char **uri, long nodeDescriptor){
    //get node from SmartSpace
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }
    /*
     * Generate new volunteer URI
     * Firstly, we use SmartSlog functions
     * Secondly we generate custom additional part
     * to provide really unique identifier,
     * because SmartSlog function works incorrect sometimes
     */
    char *_volunteer_uri = sslog_generate_uri(CLASS_CARING);
    char *volunteer_uri  = generate_uri(_volunteer_uri);
    /*
     * Create new individual of CARING class
     */
    sslog_individual_t *volunteer = sslog_new_individual(CLASS_CARING, volunteer_uri);
    if (volunteer == NULL) {
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Error volunteer: %s", sslog_error_get_last_text());
        return -1;
    }

    // insert volunteer status (ready to help)
    sslog_insert_property(volunteer, PROPERTY_HELPPROVIDINGSTATUS, "READY TO HELP");
    // insert individual with properties to SmartSpace
    sslog_node_insert_individual(node, volunteer);
    /*
     * Populate Volunteer uri to use it at top level
     */
    *uri = volunteer_uri;
    return 0;
}

int kp_disconnect_smartspace(long nodeDescriptor){
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }

    //sslog_sbcr_unsubscribe_all(node, true);
    sslog_node_leave(node);
    sslog_shutdown();
    return 0;
}
int kp_remove_individual(long nodeDescriptor, char *individual_uri){
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }

    sslog_individual_t *ind = sslog_node_get_individual_by_uri(node, individual_uri);

    if (ind == NULL){
        return -1;
    }

    sslog_node_remove_individual_with_local(node, ind);
    return 0;
}
/*
 * We need authentication to correct work of application
 * This function send needed auth request
 */
int kp_init_auth_request(char **uri, long nodeDescriptor, char *volunteer_uri){

    return 0;
}
/*
 * Recieving auth response from
 */
int kp_get_auth_responce(long nodeDescriptor, char *auth_uri){


}
int kp_insert_person_name(long nodeDescriptor, char *volunteer_uri, char *new_name){
    //get node from SmartSpace
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }

    //get patient from SmartSpace
    sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);

    // properties initialization
    const char *previous_name = (const char *) sslog_node_get_property(node, volunteer, PROPERTY_NAME);
    if (previous_name == NULL){
        sslog_node_insert_property(node, volunteer, PROPERTY_NAME, new_name);
    }
    else{
        sslog_node_update_property(node, volunteer, PROPERTY_NAME, previous_name, new_name);
    }
    return 0;

}

int kp_update_person_name(long nodeDescriptor, char *volunteer_uri, char *new_name){
    //get node from SmartSpace
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }

    //get patient from SmartSpace
    sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);

    // properties initialization
    const char *previous_name = (const char *) sslog_node_get_property(node, volunteer, PROPERTY_NAME);

    if (previous_name  == NULL) {
        //__android_log_print(ANDROID_LOG_INFO, TAG,"No name");
    }
    sslog_node_update_property(node, volunteer, PROPERTY_NAME, previous_name, new_name);
    return 0;
}

int kp_set_volunteer_status(long  nodeDescriptor, char*  volunteer_uri, char*  new_status){
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }

    //get patient from SmartSpace
    sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);

    // properties initialization
    const char *previous_status = (const char *) sslog_node_get_property(node, volunteer, PROPERTY_HELPPROVIDINGSTATUS);
    if (previous_status == NULL) {
        //__android_log_print(ANDROID_LOG_INFO, TAG,"No name");
    }

    sslog_node_update_property(node, volunteer, PROPERTY_HELPPROVIDINGSTATUS, previous_status, new_status);
    return 0;

}

int kp_init_location(char **uri, long nodeDescriptor, char *individual_uri){
     //get node from SmartSpace
     sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
     if (node == NULL){
         //__android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
         return -1;
     }

     sslog_individual_t *ind = sslog_node_get_individual_by_uri(node, individual_uri);


     //Create Location individual with latitude and longitude properties
     char * _location_uri = sslog_generate_uri(CLASS_LOCATION);
     char *location_uri = generate_uri(_location_uri);

     //__android_log_print(ANDROID_LOG_INFO, TAG, "location_uri %s.", location_uri);

     sslog_individual_t *location = sslog_new_individual(CLASS_LOCATION, location_uri);

     if (location == NULL) {
         //__android_log_print(ANDROID_LOG_INFO, TAG, "Error location: %s", sslog_error_get_last_text());
         return -1;
     }

     // Location properties initialization
     sslog_insert_property(location, PROPERTY_LAT, "0.0");
     sslog_insert_property(location, PROPERTY_LONG, "0.0");

     // insert individual with properties to SmartSpace
     sslog_node_insert_individual(node, location);
     // insert location property to specific individual
     sslog_node_insert_property(node, ind, PROPERTY_HASPERSLOCATION, location);

     *uri = location_uri;
     return 0;
}


int kp_send_location(long nodeDescriptor, char *volunteer_uri, char *location_uri, char *latitude, char *longitude){
    //get node from SmartSpace
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        //__android_log_print(ANDROID_LOG_INFO, TAG, "Node Error");
        return -1;
    }

    //get patient and location from SmartSpace
    sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);
    sslog_individual_t *location = sslog_node_get_individual_by_uri(node, location_uri);

    //update in SmartSpace Location's property: latitude
    char *latitude_from_ss = sslog_node_get_property(node, location,  PROPERTY_LAT);
    //__android_log_print(ANDROID_LOG_INFO, TAG, "latitude_from_ss %s",latitude_from_ss);
    //__android_log_print(ANDROID_LOG_INFO, TAG, "latitude %s",latitude);
    sslog_node_update_property(node, location,  PROPERTY_LAT, latitude_from_ss,latitude);

    //update in SmartSpace Location's property: longitude
    char *longitude_from_ss = sslog_node_get_property(node, location,  PROPERTY_LONG);
    //__android_log_print(ANDROID_LOG_INFO, TAG, "longitudee_from_ss %s",longitude_from_ss);
    //__android_log_print(ANDROID_LOG_INFO, TAG, "longitude %s",longitude);
    sslog_node_update_property(node, location,  PROPERTY_LONG, longitude_from_ss,longitude);

    return 0;
}

//--------------SUBSCRIPTIONS------------------//
int kp_init_patient_list_sbscr(long nodeDescriptor, char* volunteer_uri){
    sslog_node_t *node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }
    sslog_individual_t *volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);

    extern void patient_list_notification_handler(sslog_subscription_t *);
    void (*pPatientListHandler)(sslog_subscription_t *) = &patient_list_notification_handler;

    sslog_subscription_t *patientListSubscriptionContainer = NULL;

    sslog_subscription_t *sbcrContainer = NULL;

    patientListSubscriptionContainer = sslog_new_subscription(node, true);

    list_t* properties = list_new();
    list_add_data(properties, PROPERTY_HELPPROVIDINGSTATUS);
    sslog_sbcr_add_individual(patientListSubscriptionContainer, volunteer, properties);

    sbcrContainer = patientListSubscriptionContainer;

    if(sbcrContainer == NULL) {
        return -1;
    }

    sslog_sbcr_set_changed_handler(sbcrContainer, pPatientListHandler);

    if(sslog_sbcr_subscribe(sbcrContainer) != SSLOG_ERROR_NO) {
        return -1;
    }

    return 0;
}
void patient_list_notification_handler(sslog_subscription_t *sbscrDescriptor) {

    sslog_sbcr_changes_t *changes = sslog_sbcr_get_changes_last(sbscrDescriptor);

    const list_t *status_list =
            sslog_sbcr_ch_get_triples(changes, SSLOG_ACTION_INSERT);

    if( status_list != NULL ){

        list_head_t *iterator = NULL;

        list_for_each(iterator, &status_list->links){

            list_t *list_node = list_entry(iterator, list_t, links);

            sslog_triple_t *status = (sslog_triple_t *) list_node->data;

            if (strcmp(status->object, "HELP REQUEST") == 0) {
                /*
                 * Function to call handler (JAVA or C, depends on requirements)
                 * This function should be described anywhere
                 */
                help_request_notification();
            }
            break;
        }
    }
}
/*
 * Handling volunteers'answer to help_request
 */
int kp_answer_help_request(long nodeDescriptor, char *volunteer_uri, char *new_status){

    sslog_individual_t *volunteer;
    sslog_node_t *node;
    const char *previous_status;

    node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }

    volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);
    if( volunteer == NULL ){
        return -1;
    }

    previous_status = (const char *) sslog_node_get_property(node, volunteer, PROPERTY_HELPPROVIDINGSTATUS);
    if (previous_status  == NULL) {
        return -1;
    }

    sslog_node_update_property(node, volunteer, PROPERTY_HELPPROVIDINGSTATUS, previous_status, new_status);
    return 0;
}

int kp_get_patient_uri_by_alarm(char **uri, long nodeDescriptor, char *volunteer_uri){

    sslog_node_t *node;
    sslog_individual_t *volunteer;
    sslog_individual_t *alarm;
    const char *alarm_uri;
    char *patient_uri = NULL;

    node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }

    sleep(3);

    volunteer = sslog_node_get_individual_by_uri(node, volunteer_uri);

    alarm = (sslog_individual_t *)sslog_node_get_property(node, volunteer, PROPERTY_RESPONSETOALARM);
    if(alarm == NULL){
        return -1;
    }

    alarm_uri = sslog_entity_get_uri(alarm);
    if(alarm_uri == NULL){
        return -1;
    }
    /*
     * Get patient via alarm
     */
    sslog_triple_t *req_triple = sslog_new_triple_detached(
            SS_RDF_SIB_ANY,
            "http://oss.fruct.org/smartcare#sendAlarm",
            alarm_uri,
            SS_RDF_TYPE_URI, SS_RDF_TYPE_URI);

    list_t *patients = sslog_node_query_triple(node, req_triple);
    sslog_free_triple(req_triple);

    list_head_t *iterator = NULL;

    list_for_each(iterator, &patients->links){
        list_t *list_node = list_entry(iterator, list_t, links);
        char *_patient_uri = (char *) ((sslog_triple_t*) list_node->data)->subject;
        if(_patient_uri != NULL){
            patient_uri = _patient_uri;
        }
        break;
    }
    list_free_with_nodes(patients, NULL);

    if(patient_uri == NULL){
        return -1;
    }
    *uri = patient_uri;
    return 0;
}

int kp_get_current_patient_location(long nodeDescriptor, char* patient_uri){

    sslog_node_t *node;
    sslog_individual_t *patient;
    sslog_individual_t *location;
    char *latitude;
    char *longitude;

    node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }

    patient = sslog_node_get_individual_by_uri(node, patient_uri);
    if(patient == NULL){
        return -1;
    }

    location = sslog_node_get_property(node, patient, PROPERTY_HASPERSLOCATION);
    if(location == NULL){
        return -1;
    }
    latitude =  sslog_node_get_property(node, location, PROPERTY_LAT);
    longitude =  sslog_node_get_property(node, location, PROPERTY_LONG);

    if(latitude != NULL && longitude != NULL){
        change_patient_location_handler(latitude, longitude);
    }

    return 0;
}

int kp_init_current_patient_location_sbcr(long nodeDescriptor, char* patient_uri){


    sslog_node_t *node;
    sslog_individual_t *patient;
    sslog_individual_t *location;

    node = (sslog_node_t *) nodeDescriptor;
    if (node == NULL){
        return -1;
    }

    patient = sslog_node_get_individual_by_uri(node, patient_uri);
    if(patient == NULL){
        return -1;
    }

    location = sslog_node_get_property(node, patient, PROPERTY_HASPERSLOCATION);
    if(location == NULL){
        return -1;
    }


    extern void patient_location_handler(sslog_subscription_t *);
    void (*pPatientLocHandler)(sslog_subscription_t *) = &patient_location_handler;

    /*Subscribe on patient location changes*/
    sslog_subscription_t *locationSubscriptionContainer = NULL;
    sslog_subscription_t *locationClassSubscriptionContainer = NULL;
    sslog_subscription_t *sbcrContainer = NULL;

    locationSubscriptionContainer = sslog_new_subscription(node, true);
    locationClassSubscriptionContainer = sslog_new_subscription(node, true);

    list_t* properties = list_new();
    list_add_data(properties, PROPERTY_LAT);
    list_add_data(properties, PROPERTY_LONG);
    sslog_sbcr_add_individual(locationSubscriptionContainer, location, properties);

    sbcrContainer = locationSubscriptionContainer;

    if(sbcrContainer == NULL) {
        return -1;
    }

    sslog_sbcr_set_changed_handler(sbcrContainer, pPatientLocHandler);

    if(sslog_sbcr_subscribe(sbcrContainer) != SSLOG_ERROR_NO) {
        return -1;
    }


    return 0;
}
void patient_location_handler(sslog_subscription_t *sbscrDescriptor) {

    sslog_sbcr_changes_t *changes = sslog_sbcr_get_changes_last(sbscrDescriptor);

    const list_t *new_location =
            sslog_sbcr_ch_get_triples(changes, SSLOG_ACTION_INSERT);

    if( new_location != NULL ){
        patient_location_update(new_location);
    }
}

/*
 * Генерация добавки для URI
 */
char* generate_uri(char *uri){
    static int i = 0;

    int rnd_num = rand() % 100000 + i;
    i++;

    char hash[50];

    sprintf(hash, "%i", rnd_num);

    strncat(uri, hash, strlen(hash));

    return uri;
}