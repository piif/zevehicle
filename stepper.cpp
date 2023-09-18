#include "stepper.h"

static const byte modeFullSteps[] = {
    0x1, 0x2, 0x4, 0x8
};
static const byte modeHalfSteps[] = {
    0x1, 0x3, 0x2, 0x6, 0x4, 0xc, 0x8, 0x9
};
static const byte modeHalfSlowSteps[] = {
    0x1, 0x1, 0x3, 0x3, 0x2, 0x2, 0x6, 0x6,
    0x4, 0x4, 0xc, 0xc, 0x8, 0x8, 0x9, 0x9
};

void stepper_doStep(Stepper *stepper) {
    digitalWrite(stepper->outs[0], stepper->steps[stepper->step] & 1);
	digitalWrite(stepper->outs[1], stepper->steps[stepper->step] & 2);
	digitalWrite(stepper->outs[2], stepper->steps[stepper->step] & 4);
	digitalWrite(stepper->outs[3], stepper->steps[stepper->step] & 8);

	if (stepper->clockwise) {
		stepper->step = (stepper->step + 1) & stepper->maxSteps;
	} else {
		stepper->step = (stepper->step + stepper->maxSteps) & stepper->maxSteps;
	}
}

void stepper_endSetup(Stepper *stepper) {
    switch (stepper->mode) {
        case StepperFull:
            stepper->steps = modeFullSteps;
            stepper->maxSteps = sizeof(modeFullSteps) - 1;
        break;
        case StepperHalf:
            stepper->steps = modeHalfSteps;
            stepper->maxSteps = sizeof(modeHalfSteps) - 1;
        break;
        case StepperHalfSlow:
            stepper->steps = modeHalfSlowSteps;
            stepper->maxSteps = sizeof(modeHalfSlowSteps) - 1;
        break;
    }
}

void stepper_setMode(Stepper *stepper, stepper_mode mode) {
    stepper->mode = mode;
    stepper_endSetup(stepper);
}

void stepper_setup(Stepper *stepper) {
    pinMode(stepper->outs[0], OUTPUT);
	pinMode(stepper->outs[1], OUTPUT);
	pinMode(stepper->outs[2], OUTPUT);
	pinMode(stepper->outs[3], OUTPUT);
    stepper_endSetup(stepper);
}
