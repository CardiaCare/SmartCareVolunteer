//
// Created by nikolay on 20.01.16.
//

#ifndef SMARTCARE_VOLUNTEER_GLOBALS_H
#define SMARTCARE_VOLUNTEER_GLOBALS_H

#endif //SMARTCARE_VOLUNTEER_GLOBALS_H
#include "ontology/smartcare.h"
#include <jni.h>

extern jobject *GlobalMainActivityClassObject;
extern jobject *GlobalPatientHelpActivityClassObject;
extern JavaVM* JVM;
extern jclass *GlobalMainActivityClass;
extern jclass *GlobalPatientHelpActivityClass;
extern sslog_node_t *GlobalNode;
