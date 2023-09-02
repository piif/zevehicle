#ifndef PROGRAM_H
#define PROGRAM_H

#include <Arduino.h>

#define PROGRAM_MAX 20

extern bool program_running;

void program_display();

bool program_push(char instruction);
bool program_pop();

void program_nextInstruction();
void program_run();
void program_break();
void program_moveCursor(short direction);

#endif // PROGRAM_H
