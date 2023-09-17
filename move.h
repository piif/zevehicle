#ifndef MOVE_H
#define MOVE_H

#include <Arduino.h>

void move_setup(unsigned int forward_steps_init, unsigned int turn_steps_init);

void move_forward();
void move_backward();
void move_left();
void move_right();

void move_stop();

// make motors move for one step, then wait required delay
// return true if it was last step of current move
bool move_update();

#endif // MOVE_H

