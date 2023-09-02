#include "remote.h"
#include "defines.h"

bool near(unsigned long value, unsigned long reference) {
    return ((value > reference - IR_PROTO_MARGIN) && (value < reference + IR_PROTO_MARGIN));
}

#define REMOTE_IR_ERROR_CODE_BASE 0xFFF00000
#define REMOTE_IR_ERROR(n) (REMOTE_IR_ERROR_CODE_BASE | ((n)<<16))

void remoteInterruptHandler();

void remoteIR_Setup() {
    Serial.println("setupIR");
    pinMode(REMOTE_INPUT, INPUT);
    pinMode(13, OUTPUT);
    digitalWrite(13, 0);
	attachInterrupt(REMOTE_INTR, remoteInterruptHandler, CHANGE);
}

// signal is high by default -> REMOTE_NONE
// when interrupt is called for first time, it's because signal change from high to low
// thus, it's low and we must wait it becomes high again -> REMOTE_WAIT_HIGH
// then, it becomes high and we are waiting to pass low again -> REMOTE_WAIT_LOW
// when last bit is received, we just passed low during "trail", we are wxaiting trail end -> REMOTE_WAIT_TRAIL
// finaly, signal go back to default, remote ends -> REMOTE_END
// next call to check function will return decoded value and set back to REMOTE_NONE
enum { REMOTE_NONE, REMOTE_WAIT_HIGH, REMOTE_WAIT_LOW,  REMOTE_WAIT_TRAIL, REMOTE_END } remoteState = REMOTE_NONE;

// 0 = waiting for prefix, 1<<(IR_PROTO_FRAME_LEN-1) -> 1 = waiting next bit
volatile unsigned long remoteMask = 0;
volatile unsigned long remoteMaskBit = 0;
volatile unsigned long remoteValue= 0;

volatile unsigned long remoteTimeout = 0;

unsigned long remoteIR_check() {
    if (remoteState == REMOTE_END) {
        remoteTimeout = 0;
        remoteState = REMOTE_NONE;
        if ((remoteValue & REMOTE_IR_ERROR_CODE_BASE) == REMOTE_IR_ERROR_CODE_BASE) {
            // return error code "as is"
            return remoteValue;
        } else if ((remoteValue & IR_PROTO_PREFIX_MASK) == IR_PROTO_PREFIX) {
            // if prefix OK, return keycode without prefix
            return remoteValue & IR_PROTO_DATA_MASK;
        } else {
            // bad prefix
            return REMOTE_IR_ERROR(IR_ERROR_BAD_PREFIX);
        }
    }
    if (remoteTimeout != 0 && micros() > remoteTimeout) {
        remoteTimeout = 0;
        remoteState = REMOTE_NONE;
        return REMOTE_IR_ERROR(IR_ERROR_TIMEOUT) | (remoteValue & 0xFFFF);
    }
    return 0;
}

volatile unsigned long timeLow, timeHigh;

void remoteInterruptHandler() {
    byte state = digitalRead(REMOTE_INPUT);
    digitalWrite(13, !state);

    unsigned long now = micros();
    remoteTimeout = now + IR_MAX_DELAY;


    if (remoteState == REMOTE_WAIT_TRAIL) {
        // trail received, that's the end
        remoteState = REMOTE_END;
        return;
    }

    if (remoteState == REMOTE_NONE /*|| remoteState == REMOTE_END*/) {
        // new frame, just store timestamp for next iteration
        // if we are in REMOTE_END state, that means check function was not called before next frame begin
        // => previous frame is skipped (or we should ignore following ones ?)
        timeLow = now;
        remoteState = REMOTE_WAIT_HIGH;
        return;
    }
    if (remoteState == REMOTE_WAIT_HIGH) {
        timeLow = now - timeLow;
        timeHigh = now;
        remoteState = REMOTE_WAIT_LOW;
        return;
    }
    if (remoteState == REMOTE_WAIT_LOW) {
        timeHigh = now - timeHigh;

        // received a top -> interpret signal length
        if (remoteMask==0) {
            // waiting for header
            if (near(timeLow, IR_PROTO_HLOW) && near(timeHigh, IR_PROTO_HHIGH)) {
                remoteMask  = 1L<<(IR_PROTO_FRAME_LEN-1);
                remoteMaskBit = IR_PROTO_FRAME_LEN-1;
                remoteValue = 0;
            } else if (near(timeLow, IR_PROTO_RLOW) && near(timeHigh, IR_PROTO_RHIGH)) {
                // convention : repeat key = prefix + key code ffff
                remoteValue = IR_PROTO_PREFIX | IR_PROTO_DATA_MASK; 
                timeLow = now;
                remoteState = REMOTE_WAIT_TRAIL;
                return;
            } else {
                remoteValue = REMOTE_IR_ERROR(IR_ERROR_BAD_HEADER);
                remoteMask  = 0;
                remoteState = REMOTE_END;
            }
        } else {
            if (near(timeLow, IR_PROTO_1LOW) && near(timeHigh, IR_PROTO_1HIGH)) {
                remoteValue |= remoteMask;
            } else if (!near(timeLow, IR_PROTO_0LOW) || !near(timeHigh, IR_PROTO_0HIGH)) {
                remoteValue = REMOTE_IR_ERROR(IR_ERROR_BAD_BIT);
                remoteMask = 0;
                remoteState = REMOTE_END;
                return;
            }
            remoteMask >>= 1;
            remoteMaskBit--;
            if (remoteMask == 0) {
                // all bits interpreted, send result.
                timeLow = now;
                remoteState = REMOTE_WAIT_TRAIL;
                return;
            }
        }

        timeLow = now;
        remoteState = REMOTE_WAIT_HIGH;
        return;
    }
}
