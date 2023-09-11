#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

#define CHAR_UP            0x00
#define CHAR_DOWN          0x01
#define CHAR_LEFT          0x02
#define CHAR_RIGHT         0x03
#define CHAR_UP_CURSOR     0x04
#define CHAR_DOWN_CURSOR   0x05
#define CHAR_LEFT_CURSOR   0x06
#define CHAR_RIGHT_CURSOR  0x07
#define CHAR_CURSOR        0x5F // '_'
#define CHAR_ARROW         'v'
#define CHAR_AFTER         0x7E // '->'
#define CHAR_BEFORE        0x7F // '<-'

void screen_setup();
void screen_intensity_incr();

void screen_clear();
void screen_setPos(byte row, byte col);

void screen_print(char c);
void screen_print(const char *s);

#endif // SCREEN_H
