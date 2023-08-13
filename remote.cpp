#define REMOTE_CPP
#include "remote.h"

#define MAX_FRAME_LEN 2
volatile unsigned long recBufferTime[MAX_FRAME_LEN] = { 0, };
volatile int recBufferSize = 0;
volatile unsigned long recordingTimeout = 0;
volatile unsigned long recordingTimestamp = 0;

volatile enum {
    NOT_RECORDING,
    WAITING_HEADER,
    READING_PREFIX,
    READING_KEY
} recordingStatus = NOT_RECORDING;
volatile word recordingBit = 0;
volatile word recordingValue = 0;
volatile word recordingPrefix = 0;

#define MARGIN 100
bool near(unsigned long value, unsigned long reference) {
    return ((value > reference - MARGIN) && (value < reference + MARGIN));
}

word interpretResult(byte state, unsigned long now) {
    if (recordingStatus == NOT_RECORDING) {
        // first call -> reset buffer
        recBufferSize = 0;
        recordingTimestamp = now;
        recordingStatus = WAITING_HEADER;
        return 0;
    }
    
	if (recBufferSize < MAX_FRAME_LEN) {
		recordingTimeout = now + 100000;
        if (recBufferSize & 1 == state) {
            // buffer[0] must store a 0->1 change delay and buffer[1] a 1->1 change delay
            // thus index must be negation of state
            // signal error
            return 0xFFFF;
        }
		recBufferTime[recBufferSize] = now - recordingTimestamp; // store elapsed time since previous state change
		recBufferSize++;
        recordingTimestamp = now;
	}

    if (recBufferSize == MAX_FRAME_LEN) {
        recBufferSize = 0;
        // interpret 2 durations
        if (recordingStatus == WAITING_HEADER) {
            if (near(recBufferTime[0], 9000) && near(recBufferTime[0], 4400)) {
                recordingStatus = READING_PREFIX;
                recordingBit = 0x8000;
                recordingValue = 0;
            } else {
                return 0xFFFE;
            }
        } else {
            if (!near(recBufferTime[0], 630)) {
                return 0xFFFD;
            }
            if (near(recBufferTime[1], 1600)) {
                recordingValue |= recordingBit;
            } else if (!near(recBufferTime[1], 500)) {
                return 0xFFFC;
            }
            if (recordingBit == 1) {
                if(recordingStatus == READING_PREFIX) {
                    recordingPrefix = recordingValue;
                    recordingBit = 0x8000;
                    recordingValue = 0;
                } else {
                    if (recordingValue == 0) {
                        return 0xFFFB;
                    }
                    return recordingValue;
                }
            }
            recordingBit >>= 1;
        }
    }
}

void recordInterruptHandler() {
    byte state = digitalRead(RECORD_INPUT);
    digitalWrite(13, state);
    unsigned long now = micros();

    word result = interpretResult(state, now);
    if (result != 0) {
        recordingStatus = NOT_RECORDING;
        callback(result);
    }
}

void setupIR() {
    Serial.println("setupIR");
    pinMode(RECORD_INPUT, INPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, 0);
	attachInterrupt(RECORD_INTR, recordInterruptHandler, CHANGE);
}

void dumpIRbuffer() {
    Serial.print(recordingPrefix, HEX);Serial.print(' / ');Serial.println(recordingValue, HEX);
    // Serial.print(recBufferValue[0]);Serial.print(':');Serial.print(0);Serial.print('\t');
    // for(int i=1; i<recBufferSize; i++) {
    //     Serial.print(recBufferValue[i]);Serial.print(':');
    //     Serial.print(recBufferTime[i]-recBufferTime[i-1]);
    //     Serial.print(i%10 == 9 ? '\n' : '\t');
    // }
}