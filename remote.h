/**
 * read IR controler input thru interruption
 * 
 */

#ifndef REMOTE_H
#define REMOTE_H

#include <Arduino.h>

// TODO : define this as const struct to make it dynamically passed from IR_setup
// but in this case, key type must be fixed
#include "netgem_remote.h"
// here, we may include other protocole definitions
// each of them is conditionned on IR_REMOTE_* define to pass to command line

#ifndef IR_REMOTE_PROTO_DEFINED
// each *_remote.h must define IR_REMOTE_PROTO_DEFINED to detect if one was included
#error no protocol defined : no IR_REMOTE_* defined or invalid
#endif

#define IR_ERROR_TIMEOUT    1L
#define IR_ERROR_BAD_HEADER 2L
#define IR_ERROR_BAD_PREFIX 3L
#define IR_ERROR_BAD_BIT    4L

// TODO : pass these values as setup parameters
#define RECORD_INPUT 2
#define RECORD_INTR 0

remoteIR_keyCode remoteIR_check();

void remoteIR_Setup();

#endif // REMOTE_H
