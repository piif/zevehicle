/**
 * read IR controler input thru interruption
 * 
 */

#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>

#define RECORD_INPUT 2
#define RECORD_INTR 0

#ifndef REMOTE_CPP
extern volatile unsigned long recordingTimeout;
#endif

void callback(word key);

void setupIR();

void dumpIRbuffer();

#endif // REMOTE_H
