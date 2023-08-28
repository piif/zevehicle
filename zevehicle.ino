#include <Arduino.h>
#include "serialInput.h"

#include "remote.h"
#include "stepper.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define ENABLE_STEPPERS 3

Stepper left = {
    { 4, 5, 6, 7 }, 0, 0, 0
};
Stepper right = {
    { 8, 9, 10, 11 }, 0, 0, 0
};

int stepsLen = 5;
int nbSteps = 0;
int speed = 1000;
char direction = 'S';

void setHalf() {
    stepper_setHalf(&left,  1);
    stepper_setHalf(&right, 1);
}
void setFull() {
    stepper_setHalf(&left,  0);
    stepper_setHalf(&right, 0);
}

void stop() {
    nbSteps = 0;
    digitalWrite(ENABLE_STEPPERS, 0);
}

void start() {
    nbSteps = stepsLen;
    digitalWrite(ENABLE_STEPPERS, 1);
}

void motorState() {
	Serial.print("left  ");  Serial.print(left.clockwise);
    Serial.print(" / ");     Serial.println(left.step);
	Serial.print("right ");  Serial.print(right.clockwise);
    Serial.print(" / ");     Serial.println(right.step);
    Serial.print("remains ");Serial.print(nbSteps);
    Serial.print(" at ");    Serial.println(speed  );
}

void status() {
	Serial.println("Commands :");
	Serial.println("? : status");
	Serial.println("d: set direction F (forward) B (backward) L (left) R (right) S (stop)");
	Serial.println("s: set speed (ms between steps)");
	Serial.println("l: set how many steps to do");
	Serial.println("h/f: set half/full");

    motorState();
}

void setDirection(char *d) {
    direction = d[0];
    switch(d[0]) {
        case 'f':
        case 'F':
            left.clockwise  = 1;
            right.clockwise = 1;
            start();
            break;
        case 'b':
        case 'B':
            left.clockwise  = 0;
            right.clockwise = 0;
            start();
            break;
        case 'l':
        case 'L':
            left.clockwise  = 0;
            right.clockwise = 1;
            start();
            break;
        case 'r':
        case 'R':
            left.clockwise  = 1;
            right.clockwise = 0;
            start();
            break;
        case 's':
        case 'S':
            stop();
        break;
    }
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status       },
	{ 'l', 'i', (void *)&stepsLen    },
	{ 's', 'i', (void *)&speed       },
	{ 'd', 'S', (void *)setDirection },
	{ 'f', 'f', (void *)setFull      },
	{ 'h', 'f', (void *)setHalf      }
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    pinMode(ENABLE_STEPPERS, OUTPUT);
    digitalWrite(ENABLE_STEPPERS, 0);
    stepper_setup(&left);
    stepper_setup(&right);

    remoteIR_Setup();

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {

	handleInput();

    if (nbSteps != 0) {
        nbSteps--;
        stepper_doStep(&left);
        stepper_doStep(&right);
        motorState();
        delay(speed);
        if (nbSteps == 0) {
            stop();
        }
    }
}
