#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

// from ledMatrix.h
#define ARROW_UP    '{'
#define ARROW_DOWN  '|'
#define ARROW_LEFT  '}'
#define ARROW_RIGHT '~'

void screen_setup();
void screen_intensity_incr();

void screen_displayValue(const char *message, int value);

void screen_clear();
void screen_displayChar(byte y, char c);
void screen_flush();

#endif // SCREEN_H
