//
// Created by nikolay on 13.01.16.
//
#ifdef	__cplusplus
extern "C" {
#endif

#ifndef SMARTCARE_VOLUNTEER_AGENT_H
#define SMARTCARE_VOLUNTEER_AGENT_H

#endif //SMARTCARE_VOLUNTEER_AGENT_H
#include "ontology/smartcare.h"

char* generate_uri(char *);

long kp_connect_smartspace(char*, char*, int);
int kp_disconnect_smartpace();
int kp_init_volunteer(char**, long);
int kp_remove_individual(long, char*);
int kp_init_auth_request(char**, long, char*);
int kp_get_auth_responce(long, char *);
int kp_init_patient_list_subscription();
void kp_patient_list_notification_handler();
int kp_insert_person_name(long, char *, char *);
int kp_update_person_name(long, char *, char *);
int kp_init_location(char**, long, char *);
int kp_send_location(long, char *, char *, char *, char *);

int kp_init_patient_list_sbscr(long, char*);
void patient_list_notification_handler(sslog_subscription_t *);

int kp_answer_help_request(long, char*, char*);
int kp_get_patient_uri_by_alarm(char**, long, char*);

int kp_get_current_patient_location(long nodeDescriptor, char* patient_uri);
int kp_init_current_patient_location_sbcr(long, char*);
void patient_location_handler(sslog_subscription_t*);
int kp_set_volunteer_status(long  nodeDescriptor, char*  volunteer_uri, char*  new_status);

#ifdef	__cplusplus
}
#endif