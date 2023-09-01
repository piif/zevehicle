#include <Arduino.h>

// from ledMatrix.h
#define ARROW_UP    '{'
#define ARROW_DOWN  '|'
#define ARROW_LEFT  '}'
#define ARROW_RIGHT '~'

void screen_setup();
void screen_intensity_incr();
void displayValue(const char *message, int value);
void displayClear();
void displayChar(byte y, char c);
void displayFlush();
