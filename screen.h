#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

void screen_setup();
void screen_intensity_incr();

void screen_clear();
void screen_displayChar(byte y, char c);
void screen_flush();

#endif // SCREEN_H
