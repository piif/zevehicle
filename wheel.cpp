#include "wheel.h"

volatile byte wheelPos1;
volatile byte wheelPos2;
volatile byte wheelState1;
volatile byte wheelState2;

byte readWheelState() {
    return digitalRead(WHEEL1A) << 1 | digitalRead(WHEEL1B);
}

void wheel_setup() {
    pinMode(WHEEL1A, INPUT);
    pinMode(WHEEL1B, INPUT);
    readWheelState();
    Serial.print("setupWheel  "); Serial.println(wheelState1);
}

bool wheel_update() {
    byte newState = readWheelState();
    if (newState != wheelState1) {
        wheelState1 = newState;
        Serial.print("updateWheel "); Serial.println(wheelState1);
        return 1;
    }
    return 0;
}
