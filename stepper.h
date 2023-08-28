#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>

typedef struct _stepper {
    byte outs[4];   // for output pins
    byte step;      // current step
    bool half;      // full/half steps
    bool clockwise; // current direction
} Stepper;

void stepper_doStep(Stepper *stepper);
void stepper_setup(Stepper *stepper);
void stepper_setHalf(Stepper *stepper, bool half);

#endif // STEPPER_H