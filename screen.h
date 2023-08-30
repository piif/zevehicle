#include <Arduino.h>

// from ledMatrix.h
#define ARROW_UP    '{'
#define ARROW_DOWN  '|'
#define ARROW_LEFT  '}'
#define ARROW_RIGHT '~'

void screen_setup();
void screen_intensity_incr();
void displayValue(const char *message, int value);
void displayChar(char c);
void displayRight(const char *str);
