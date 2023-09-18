#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

typedef enum { StepperFull, StepperHalf, StepperHalfSlow } stepper_mode;
typedef struct _stepper {
    byte outs[4];      // for output pins
    stepper_mode mode; // full/half steps
    bool clockwise;    // current direction
    // internal fields, leave blank when calling setup
    byte step;         // current step
    const byte *steps;
    byte maxSteps;
} Stepper;

void stepper_doStep(Stepper *stepper);
void stepper_setup(Stepper *stepper);
void stepper_setMode(Stepper *stepper, stepper_mode mode);

#endif // STEPPER_H