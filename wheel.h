#ifndef WHEEL_H
#define WHEEL_H

#include <Arduino.h>

#define WHEEL1A A4
#define WHEEL1B A5

void wheel_setup();
bool wheel_update();

#endif // WHEEL_H
