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

// on lego prototype :
// 51 steps to move forward/backward 5cm
// 76 steps to turn a quarter
// speed 20 a bit slow, but with speed 10 first steps are weird
// -> TODO start  at 20 and continue at 10 after some steps, then slow back to 20 before last steps ?

#define TURN_STEPS 86    // for a quarter turn
#define FORWARD_STEPS 51 // for 5cm

#define INIT_SPEED 20
#define MAX_SPEED  10

// left motor is 96 steps, right one is 48 steps
// -> by setting rigth motor on half steps, wheels are 96 steps per rotation
// todo : try to use micro steps instead (4th and 8th steps) to have smooth movement ?

Stepper left = {
    { 4, 5, 6, 7 }, 0, 0, 0
};
Stepper right = {
    { 8, 9, 10, 11 }, 0, 1, 0
};

int currentStep, nbSteps = 0;
int speed = INIT_SPEED;


void stop() {
    currentStep = nbSteps = 0;
    digitalWrite(ENABLE_STEPPERS, 0);
}

void start(int len) {
    nbSteps = len;
    currentStep = 0;
    digitalWrite(ENABLE_STEPPERS, 1);
}
#define PROGRAM_MAX 20
char program[PROGRAM_MAX+1];
bool programRunning = false;
byte currentInstruction = 0;
byte lastInstruction = 0;

void displayProgram() {
    // afficher une tanche autour de current
    // en collant en haut ou en bas si on est prêt du bord
    // en collant en haut s'il y a moins de 4 instructions (ou pas : partir du bas ?)
    for(byte i=0; i<lastInstruction; i++) {
        Serial.print(program[i]);
    }
    Serial.println();

    displayClear();
    displayChar(0, '?');
    displayChar(0, 0x7f);
    byte y=1, instr=lastInstruction;
    while(y<=3 && instr>0) {
        displayChar(y, program[instr-1]);
        y++;
        instr--;
    }
    displayFlush();
}

bool pushProgram(char instruction) {
    // TODO : remplacer en current
    if (lastInstruction == PROGRAM_MAX) {
        return false;
    }
    program[lastInstruction] = instruction;
    lastInstruction++;
    displayProgram();
    return true;
}

bool popProgram() {
    // TODO : décaler vers le bas de current+1 à last
    if (lastInstruction == 0) {
        return false;
    }
    lastInstruction--;
    displayProgram();
    return true;
}

void nextInstruction() {
    if (currentInstruction == lastInstruction) {
        stop();
        programRunning = false;
    }
    switch(program[currentInstruction]) {
        case ARROW_UP:
            left.clockwise  = 0;
            right.clockwise = 0;
            start(FORWARD_STEPS);
        break;
        case ARROW_DOWN:
            left.clockwise  = 1;
            right.clockwise = 1;
            start(FORWARD_STEPS);
        break;
        case ARROW_LEFT:
            left.clockwise  = 0;
            right.clockwise = 1;
            start(TURN_STEPS);
        break;
        case ARROW_RIGHT:
            left.clockwise  = 1;
            right.clockwise = 0;
            start(TURN_STEPS);
        break;
    }
    currentInstruction++;
}

void runProgram() {
    currentInstruction = 0;
    programRunning = true;
    nextInstruction();
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

    motorState();
}

InputItem inputs[] = {
	{ '?', 'f', (void *)status },
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

    if (irKey != 0) {
        // Serial.print("Pressed "); Serial.println(irKey, HEX);
        switch (irKey) {
            case IR_DIRECTION_UP:
                Serial.println("UP");
                pushProgram(ARROW_UP);
            break;
            case IR_DIRECTION_DOWN:
                Serial.println("DOWN");
                pushProgram(ARROW_DOWN);
            break;
            case IR_DIRECTION_LEFT:
                Serial.println("LEFT");
                pushProgram(ARROW_LEFT);
            break;
            case IR_DIRECTION_RIGHT:
                Serial.println("RIGHT");
                pushProgram(ARROW_RIGHT);
            break;
            case IR_PROGRAM_DOWN:
                if (!running && currentStep>0) {
                    currentStep--;
                }
            break;
            case IR_PROGRAM_UP:
                if (!running && currentStep<lastInstruction) {
                    currentStep++;
                }
            break;
            case IR_CANCEL:
                popProgram();
            break;
            case IR_OK:
                runProgram();
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

    // TODO : use setInterval or timer + interuptions instead ?
    if (programRunning) {  // currentStep != nbSteps
        stepper_doStep(&left);
        stepper_doStep(&right);
        // motorState();
        delay(speed);
        currentStep++;
        if (currentStep == nbSteps) {
            nextInstruction();
        }
    }
}
