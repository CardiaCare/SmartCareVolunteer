//
// Created by nikolay on 20.01.16.
//
#include <jni.h>
#include "ontology/smartcare.h"
#include "globals.h"

jobject *GlobalMainActivityClassObject;
jobject *GlobalPatientHelpActivityClassObject;
JavaVM* JVM;
JNIEnv *envGlob;
jclass *GlobalMainActivityClass;
jclass *GlobalPatientHelpActivityClass;
jmethodID GlobalGetHelpRequestNotification;
sslog_node_t *GlobalNode;