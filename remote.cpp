#include "remote.h"

bool near(unsigned long value, unsigned long reference) {
    return ((value > reference - IR_PROTO_MARGIN) && (value < reference + IR_PROTO_MARGIN));
}

#define REMOTE_IR_ERROR_CODE_BASE 0xFFF00000
#define REMOTE_IR_ERROR(n) (REMOTE_IR_ERROR_CODE_BASE | ((n)<<16))

// signal is high by default -> RECORD_NONE
// when interrupt is called for first time, it's because signal change from high to low
// thus, it's low and we must wait it becomes high again -> RECORD_WAIT_HIGH
// then, it becomes high and we are waiting to pass low again -> RECORD_WAIT_LOW
// when last bit is received, we just passed low during "trail", we are wxaiting trail end -> RECORD_WAIT_TRAIL
// finaly, signal go back to default, record ends -> RECORD_END
// next call to check function will return decoded value and set back to RECORD_NONE
enum { RECORD_NONE, RECORD_WAIT_HIGH, RECORD_WAIT_LOW,  RECORD_WAIT_TRAIL, RECORD_END } recordState = RECORD_NONE;

// 0 = waiting for prefix, 1<<(IR_PROTO_FRAME_LEN-1) -> 1 = waiting next bit
volatile unsigned long recordMask = 0;
volatile unsigned long recordMaskBit = 0;
volatile unsigned long recordValue= 0;
DEBUG(volatile unsigned long debug);

volatile unsigned long recordingTimeout = 0;

unsigned long remoteIR_check() {
    if (recordState == RECORD_END) {
        recordingTimeout = 0;
        recordState = RECORD_NONE;
        DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
        if ((recordValue & REMOTE_IR_ERROR_CODE_BASE) == REMOTE_IR_ERROR_CODE_BASE) {
            // return error code "as is"
            return recordValue;
        } else if ((recordValue & IR_PROTO_PREFIX_MASK) == IR_PROTO_PREFIX) {
            // if prefix OK, return keycode without prefix
            return recordValue & IR_PROTO_DATA_MASK;
        } else {
            // bad prefix
            return REMOTE_IR_ERROR(IR_ERROR_BAD_PREFIX);
        }
    }
    if (recordingTimeout != 0 && micros() > recordingTimeout) {
        recordingTimeout = 0;
        recordState = RECORD_NONE;
        DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
        return REMOTE_IR_ERROR(IR_ERROR_TIMEOUT) | (recordValue & 0xFFFF);
    }
    return 0;
}

volatile unsigned long timeLow, timeHigh;

void recordInterruptHandler() {
    byte state = digitalRead(RECORD_INPUT);
    digitalWrite(13, !state);

    unsigned long now = micros();
    recordingTimeout = now + IR_MAX_DELAY;

    DEBUG(debug = (debug & 0xFFFFF0FF) | (recordState<<8));

    if (recordState == RECORD_WAIT_TRAIL) {
        // trail received, that's the end
        recordState = RECORD_END;
        DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
        return;
    }

    if (recordState == RECORD_NONE /*|| recordState == RECORD_END*/) {
        // new frame, just store timestamp for next iteration
        // if we are in RECORD_END state, that means check function was not called before next frame begin
        // => previous frame is skipped (or we should ignore following ones ?)
        timeLow = now;
        recordState = RECORD_WAIT_HIGH;
        DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
        return;
    }
    if (recordState == RECORD_WAIT_HIGH) {
        timeLow = now - timeLow;
        timeHigh = now;
        recordState = RECORD_WAIT_LOW;
        DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
        return;
    }
    if (recordState == RECORD_WAIT_LOW) {
        timeHigh = now - timeHigh;

        // received a top -> interpret signal length
        if (recordMask==0) {
            // waiting for header
            if (near(timeLow, IR_PROTO_HLOW) && near(timeHigh, IR_PROTO_HHIGH)) {
                recordMask  = 1L<<(IR_PROTO_FRAME_LEN-1);
                recordMaskBit = IR_PROTO_FRAME_LEN-1;
                DEBUG(debug = (debug & 0xFFFFFF00) | recordMaskBit);
                recordValue = 0;
            } else if (near(timeLow, IR_PROTO_RLOW) && near(timeHigh, IR_PROTO_RHIGH)) {
                // convention : repeat key = prefix + key code ffff
                recordValue = IR_PROTO_PREFIX | IR_PROTO_DATA_MASK; 
                timeLow = now;
                recordState = RECORD_WAIT_TRAIL;
                DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
                return;
            } else {
                recordValue = REMOTE_IR_ERROR(IR_ERROR_BAD_HEADER);
                recordMask  = 0;
                DEBUG(debug = (debug & 0xFFFF0F00) | (recordState<<12));
                recordState = RECORD_END;
            }
        } else {
            if (near(timeLow, IR_PROTO_1LOW) && near(timeHigh, IR_PROTO_1HIGH)) {
                recordValue |= recordMask;
            } else if (!near(timeLow, IR_PROTO_0LOW) || !near(timeHigh, IR_PROTO_0HIGH)) {
                recordValue = REMOTE_IR_ERROR(IR_ERROR_BAD_BIT);
                recordMask = 0;
                recordState = RECORD_END;
                DEBUG(debug = (debug & 0xFFFF0F00) | (recordState<<12));
                return;
            }
            recordMask >>= 1;
            recordMaskBit--;
            DEBUG(debug = (debug & 0xFFFFFF00) | (recordMaskBit & 0xFF));
            if (recordMask == 0) {
                // all bits interpreted, send result.
                timeLow = now;
                recordState = RECORD_WAIT_TRAIL;
                DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
                return;
            }
        }

        timeLow = now;
        recordState = RECORD_WAIT_HIGH;
        DEBUG(debug = (debug & 0xFFFF0FFF) | (recordState<<12));
        return;
    }
}

void remoteIR_Setup() {
    Serial.println("setupIR");
    pinMode(RECORD_INPUT, INPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, 0);
	attachInterrupt(RECORD_INTR, recordInterruptHandler, CHANGE);
}
