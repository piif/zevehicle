
#include <Arduino.h>
#include "defines.h"
#include "move.h"
#include "stepper.h"

static Stepper left = {
    { STEPPER_LEFT_A, STEPPER_LEFT_B, STEPPER_LEFT_C, STEPPER_LEFT_D },
    0, LEFT_MODE_HALF, 0
};
static Stepper right = {
    { STEPPER_RIGHT_A, STEPPER_RIGHT_B, STEPPER_RIGHT_C, STEPPER_RIGHT_D },
    0, RIGHT_MODE_HALF, 0
};

static int currentStep, nbSteps;

static unsigned int forward_steps, turn_steps;

void move_setup(unsigned int forward_steps_init, unsigned int turn_steps_init) {
    forward_steps = forward_steps_init;
    turn_steps = turn_steps_init;
    currentStep = nbSteps = 0;

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

void move_forward() {
    move_start(0, 0, forward_steps);
}
void move_backward() {
    move_start(1, 1, forward_steps);
}
void move_left() {
    move_start(1, 0, turn_steps);
}
void move_right() {
    move_start(0, 1, turn_steps);
}

void move_stop() {
    currentStep = nbSteps = 0;
    digitalWrite(ENABLE_STEPPERS, 0);
}

bool move_update() {
    stepper_doStep(&left);
    stepper_doStep(&right);
    // motorState();
    if (currentStep < LOW_SPEED_STEPS || currentStep >= nbSteps-LOW_SPEED_STEPS) {
        delay(LOW_SPEED);
    } else {
        delay(HIGH_SPEED);
    }
    currentStep++;
    return currentStep == nbSteps;
}

void move_state() {
	Serial.print("left  ");  Serial.print(left.clockwise);
    Serial.print(" / ");     Serial.println(left.step);
	Serial.print("right ");  Serial.print(right.clockwise);
    Serial.print(" / ");     Serial.println(right.step);
    Serial.print("remains ");Serial.println(nbSteps);
}
