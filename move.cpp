
#include <Arduino.h>
#include "hardware.h"
#include "move.h"
#include "stepper.h"

Stepper left = {
    { STEPPER_LEFT_A, STEPPER_LEFT_B, STEPPER_LEFT_C, STEPPER_LEFT_D },
    0, LEFT_MODE_HALF, 0
};
Stepper right = {
    { STEPPER_RIGHT_A, STEPPER_RIGHT_B, STEPPER_RIGHT_C, STEPPER_RIGHT_D },
    0, RIGHT_MODE_HALF, 0
};

int currentStep, nbSteps = 0;
int speed = INIT_SPEED;

void move_setup() {
    pinMode(ENABLE_STEPPERS, OUTPUT);
    digitalWrite(ENABLE_STEPPERS, 0);
    stepper_setup(&left);
    stepper_setup(&right);
}

void move_start(bool left_clockwise, bool right_clockwise, int len) {
    left.clockwise  = left_clockwise;
    right.clockwise = right_clockwise;
    nbSteps = len;
    currentStep = 0;
    digitalWrite(ENABLE_STEPPERS, 1);
}

void move_stop() {
    currentStep = nbSteps = 0;
    digitalWrite(ENABLE_STEPPERS, 0);
}

bool move_update() {
    stepper_doStep(&left);
    stepper_doStep(&right);
    // motorState();
    delay(speed);
    currentStep++;
    return currentStep == nbSteps;
}

void move_state() {
	Serial.print("left  ");  Serial.print(left.clockwise);
    Serial.print(" / ");     Serial.println(left.step);
	Serial.print("right ");  Serial.print(right.clockwise);
    Serial.print(" / ");     Serial.println(right.step);
    Serial.print("remains ");Serial.print(nbSteps);
    Serial.print(" at ");    Serial.println(speed  );
}
