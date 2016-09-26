//
// Created by nikolay on 19.01.16.
//
#ifdef	__cplusplus
extern "C" {
#endif
#ifndef SMARTCARE_VOLUNTEER_HANDLERS_H
#define SMARTCARE_VOLUNTEER_HANDLERS_H

#endif //SMARTCARE_VOLUNTEER_HANDLERS_H

#include "ontology/smartcare.h"

void help_request_notification();
void patient_location_update(const list_t*);
void change_patient_location_handler(char*, char*);


#ifdef	__cplusplus
}
#endif