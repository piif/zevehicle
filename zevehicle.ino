#include <Arduino.h>
#include "serialInput.h"

#include "remote.h"
#include "wheel.h"

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

#define MOTOR_LEFT_EN   4
#define MOTOR_LEFT_A    5
#define MOTOR_LEFT_B    6

#define MOTOR_RIGHT_EN  8
#define MOTOR_RIGHT_A   9
#define MOTOR_RIGHT_B  10

int left = 0, right = 0;

void updateSpeed() {
    if (left == 0) {
        digitalWrite(MOTOR_LEFT_EN,  0);
    } else if(left > 0) {
        analogWrite( MOTOR_LEFT_A,  left);
        digitalWrite(MOTOR_LEFT_B,  0);
        digitalWrite(MOTOR_LEFT_EN, 1);
    } else  { // < 0
        digitalWrite(MOTOR_LEFT_A,  0);
        analogWrite( MOTOR_LEFT_B,  -left);
        digitalWrite(MOTOR_LEFT_EN, 1);
    }

    if (right == 0) {
        digitalWrite(MOTOR_RIGHT_EN,  0);
    } else if(right > 0) {
        analogWrite( MOTOR_RIGHT_A,  right);
        digitalWrite(MOTOR_RIGHT_B,  0);
        digitalWrite(MOTOR_RIGHT_EN, 1);
    } else  { // < 0
        digitalWrite(MOTOR_RIGHT_A,  0);
        analogWrite( MOTOR_RIGHT_B,  -right);
        digitalWrite(MOTOR_RIGHT_EN, 1);
    }
}

void status() {
	Serial.println("Commands :");
	Serial.println("? : status");
	Serial.println("l: set left motor speed");
	Serial.println("r: set rightt motor speed");
	Serial.println("z: stop all motors");
	Serial.print("current speed = ");
	Serial.print(left);
	Serial.print(" / ");
	Serial.println(right);
}

void stop() {
    left = right = 0;
    updateSpeed();
}

void setLeft(int speed) {
    left = speed;
    updateSpeed();
}

void setRight(int speed) {
    right = speed;
    updateSpeed();
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status   },
	{ 'l', 'I', (void *)setLeft  },
	{ 'r', 'I', (void *)setRight },
	{ 'z', 'f', (void *)stop     },
};

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);

    pinMode(MOTOR_LEFT_EN,  OUTPUT);
    pinMode(MOTOR_LEFT_A,   OUTPUT);
    pinMode(MOTOR_LEFT_B,   OUTPUT);
    pinMode(MOTOR_RIGHT_EN, OUTPUT);
    pinMode(MOTOR_RIGHT_A,  OUTPUT);
    pinMode(MOTOR_RIGHT_B,  OUTPUT);

    updateSpeed();

    remoteIR_Setup();

    wheel_setup();

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

unsigned long nextActionTS = 0;
void nextAction() {
    stop();
}

void addAction(word command) {
    if (command == IR_OK) {
        stop();
        nextActionTS = 0;
        return;
    }
    nextActionTS = millis() + 1000;
    switch(command) {
        case IR_DIRECTION_UP:
            left = right = 255;
            updateSpeed();
        break;
        case IR_DIRECTION_DOWN:
            left = right = -255;
            updateSpeed();
        break;
        case IR_DIRECTION_LEFT:
            left  = -255;
            right =  255;
            updateSpeed();
        break;
        case IR_DIRECTION_RIGHT:
            left  =  255;
            right = -255;
            updateSpeed();
        break;
    }
}

DEBUG(unsigned long debugBefore=0);
void loop() {

	handleInput();

    if (nextActionTS != 0 && millis() > nextActionTS) {
        nextActionTS = 0;
        nextAction();
    }

    remoteIR_keyCode irKey = remoteIR_check();
    DEBUG(if (debug != debugBefore) {
        Serial.print("D="); Serial.println(debug, HEX);
        debugBefore = debug;
    })

    if (irKey != 0) {
        Serial.print("Pressed "); Serial.println(irKey, HEX);
        switch (irKey) {
            case IR_DIRECTION_UP:
                Serial.println("UP");
                addAction(irKey);
            break;
            case IR_DIRECTION_DOWN:
                Serial.println("DOWN");
                addAction(irKey);
            break;
            case IR_DIRECTION_LEFT:
                Serial.println("LEFT");
                addAction(irKey);
            break;
            case IR_DIRECTION_RIGHT:
                Serial.println("RIGHT");
                addAction(irKey);
            break;
            case IR_OK:
                Serial.println("OK");
                addAction(irKey);
            break;
            default:
                Serial.println("Other");
            break;
        }
    }
    wheel_update();
}
