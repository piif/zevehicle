#include <Arduino.h>
#include "serialInput.h"
#include "remote.h"

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

volatile word irKey = 0;
void callback(word key) {
    irKey = key;
}

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

    setupIR();

	registerInput(sizeof(inputs), inputs);
	Serial.println("setup ok");
}

void loop() {
	handleInput();

    if (recordingTimeout != 0 && micros() > recordingTimeout) {
        recordingTimeout = 0;
        irKey = 99;
    }

    if (irKey != 0) {
        Serial.print("Pressed "); Serial.println(irKey, HEX);
        irKey = 0;
        dumpIRbuffer();
    }
}
