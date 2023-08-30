#include <Arduino.h>
#include "serialInput.h"

#include "remote.h"
#include "stepper.h"
#include "screen.h"

void(* resetFunc) (void) = 0;

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define ENABLE_STEPPERS 3

// left motor is 96 steps, right one is 48 steps
// -> by setting rigth motor on half steps, wheels are 96 steps per rotation
// todo : try to use micro steps instead (4th and 8th steps) to have smooth movement ?

Stepper left = {
    { 4, 5, 6, 7 }, 0, 0, 0
};
Stepper right = {
    { 8, 9, 10, 11 }, 0, 1, 0
};

int stepsLen = 96;
int nbSteps = 0;
int speed = 20;
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
	Serial.print("s: set speed (ms between steps) , current = "); Serial.println(speed);
	Serial.print("l: set how many steps to do , current = ");     Serial.println(stepsLen);
	Serial.println("h/f: set half/full");

    motorState();
}

void setDirection(char const *d) {
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

    screen_setup();

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {

	handleInput();

    remoteIR_keyCode irKey = remoteIR_check();
    DEBUG(if (debug != debugBefore) {
        Serial.print("D="); Serial.println(debug, HEX);
        debugBefore = debug;
    })

    if (irKey != 0) {
        // Serial.print("Pressed "); Serial.println(irKey, HEX);
        switch (irKey) {
            case IR_DIRECTION_UP:
                Serial.println("UP");
                displayChar(ARROW_UP);
                setDirection("F");
            break;
            case IR_DIRECTION_DOWN:
                Serial.println("DOWN");
                displayChar(ARROW_DOWN);
                setDirection("B");
            break;
            case IR_DIRECTION_LEFT:
                Serial.println("LEFT");
                displayChar(ARROW_LEFT);
                setDirection("L");
            break;
            case IR_DIRECTION_RIGHT:
                Serial.println("RIGHT");
                displayChar(ARROW_RIGHT);
                setDirection("R");
            break;
            case IR_OK:
                // TODO ..
            break;
            case IR_VOLUME_UP:
                speed += 10;
                displayValue("v", speed);
            break;
            case IR_VOLUME_DOWN:
                if (speed > 10) {
                    speed -= 10;
                }
                displayValue("v", speed);
            break;
            case IR_PROGRAM_UP:
                stepsLen++;
                displayValue("p", stepsLen);
            break;
            case IR_PROGRAM_DOWN:
                if (stepsLen > 1) {
                    stepsLen--;
                }
                displayValue("p", stepsLen);
            break;
            case IR_POWER:
                stop();
                resetFunc();
            break;
            case IR_TEXT:
                Serial.println("TEXT");
                screen_intensity_incr();
            break;
        }
    }

    if (nbSteps != 0) {
        nbSteps--;
        stepper_doStep(&left);
        stepper_doStep(&right);
        // motorState();
        delay(speed);
        if (nbSteps == 0) {
            stop();
        }
    }
}
