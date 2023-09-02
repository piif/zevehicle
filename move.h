#ifndef MOVE_H
#define MOVE_H

#include <Arduino.h>

void move_setup();

void move_start(bool left_clockwise, bool right_clockwise, int len);
void move_stop();

// make motors move for one step, then wait required delay
// return true if it was last step of current move
bool move_update();

#endif // MOVE_H

