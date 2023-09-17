#include "stepper.h"

static const byte fullSteps[] = { 0x1, 0x2, 0x4, 0x8 };
static const byte halfSteps[] = { 0x1, 0x3, 0x2, 0x6, 0x4, 0xc, 0x8, 0x9  };

void stepper_doStep(Stepper *stepper) {
    const byte *steps = stepper->half ? halfSteps : fullSteps;
    byte maxSteps = stepper->half ? 7 : 3;

    digitalWrite(stepper->outs[0], steps[stepper->step] & 1);
	digitalWrite(stepper->outs[1], steps[stepper->step] & 2);
	digitalWrite(stepper->outs[2], steps[stepper->step] & 4);
	digitalWrite(stepper->outs[3], steps[stepper->step] & 8);

	if (stepper->clockwise) {
		stepper->step = (stepper->step + 1) & maxSteps;
	} else {
		stepper->step = (stepper->step + maxSteps) & maxSteps;
	}
}

void stepper_setHalf(Stepper *stepper, bool half) {
    if (stepper->half == half) {
        return;
    }
    stepper->half = half;
    if (half) {
        stepper->step <<= 1;
    } else {
        stepper->step >>= 1;
    }
}

void stepper_setup(Stepper *stepper) {
    pinMode(stepper->outs[0], OUTPUT);
	pinMode(stepper->outs[1], OUTPUT);
	pinMode(stepper->outs[2], OUTPUT);
	pinMode(stepper->outs[3], OUTPUT);
}
