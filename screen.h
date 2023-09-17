#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

#define CHAR_UP            ((char)0x00)
#define CHAR_DOWN          ((char)0x01)
#define CHAR_LEFT          ((char)0x02)
#define CHAR_RIGHT         ((char)0x03)
#define CHAR_UP_CURSOR     ((char)0x04)
#define CHAR_DOWN_CURSOR   ((char)0x05)
#define CHAR_LEFT_CURSOR   ((char)0x06)
#define CHAR_RIGHT_CURSOR  ((char)0x07)
#define CHAR_CURSOR        ((char)0x5F) // '_'
#define CHAR_ARROW         ((char)'v' )
#define CHAR_AFTER         ((char)0x7E) // '->'
#define CHAR_BEFORE        ((char)0x7F) // '<-'

void screen_setup();
void screen_intensity_incr();

void screen_clear();
void screen_setPos(byte row, byte col);

void screen_print(char c);
void screen_print(const char *s);
void screen_print(unsigned int v);

#endif // SCREEN_H
