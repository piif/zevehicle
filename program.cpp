#include "program.h"
#include "defines.h"
#include "move.h"
#include "screen.h"

char program[PROGRAM_MAX+1];
byte currentInstruction = 0;
byte lastInstruction = 0;

bool program_running = false;


void program_display() {
    // afficher une tranche autour de current
    // en collant à gauche ou à droite si on est prêt du bord
    // en collant à gauche s'il y a moins de 16 instructions
    for(byte i=0; i<lastInstruction; i++) {
        if (i == currentInstruction) {
            Serial.print('>');
        }
        Serial.print(program[i]);
    }
    Serial.println();

    byte offset;
    if (lastInstruction < SCREEN_COLS || currentInstruction  < SCREEN_COLS/2) {
        offset = 0;
    } else if (lastInstruction - currentInstruction < SCREEN_COLS/2) {
        offset = lastInstruction - (SCREEN_COLS-1);
    } else {
        offset = currentInstruction - SCREEN_COLS/2;
    }

    screen_clear();

    if (program_running) {
        screen_print("Execution :");
    } else {
        screen_print("Programme :");
    }

    screen_setPos(1, 0);
    for (short col = 0; (col < SCREEN_COLS) && (offset <= lastInstruction); col++, offset++) {
        if (col == 0 && offset != 0) {
            screen_print(CHAR_BEFORE);
        } else if (col == SCREEN_COLS-1 && offset < lastInstruction-1) {
            screen_print(CHAR_AFTER);
        } else if (offset == currentInstruction) {
            if (offset == lastInstruction) {
                screen_print(CHAR_CURSOR);
            } else {
                switch(program[offset]) {
                    case CHAR_UP:
                        screen_print(CHAR_UP_CURSOR);
                    break;
                    case CHAR_DOWN:
                        screen_print(CHAR_DOWN_CURSOR);
                    break;
                    case CHAR_LEFT:
                        screen_print(CHAR_LEFT_CURSOR);
                    break;
                    case CHAR_RIGHT:
                        screen_print(CHAR_RIGHT_CURSOR);
                    break;
                }
            }
        } else if (offset != lastInstruction) {
            screen_print(program[offset]);
        }
    }
}

// replace current instruction, or append if at end
// return false if trying to append while program stack is full
bool program_push(char instruction) {
    if (program_running) {
        return false;
    }

    if (currentInstruction == lastInstruction) {
        if (lastInstruction == PROGRAM_MAX) {
            return false;
        }
        lastInstruction++;
    }
    program[currentInstruction] = instruction;
    currentInstruction++;
    program_display();
    return true;
}

// remove current instruction
bool program_pop() {
    if (program_running) {
        return false;
    }

    if (lastInstruction == 0) {
        return false;
    }
    for(byte i = currentInstruction; i < lastInstruction-1; i++) {
        program[i] = program[i+1];
    }
    lastInstruction--;
    program_display();
    return true;
}

void program_nextInstruction() {
    program_display();
    if (currentInstruction == lastInstruction) {
        program_break();
        return;
    }
    switch(program[currentInstruction]) {
        case CHAR_UP:
            move_start(0, 0, FORWARD_STEPS);
        break;
        case CHAR_DOWN:
            move_start(1, 1, FORWARD_STEPS);
        break;
        case CHAR_LEFT:
            move_start(1, 0, TURN_STEPS);
        break;
        case CHAR_RIGHT:
            move_start(0, 1, TURN_STEPS);
        break;
    }
    currentInstruction++;
}

void program_run() {
    currentInstruction = 0;
    program_running = true;
    program_nextInstruction();
}

void program_break() {
    move_stop();
    program_running = false;
    program_display();
}

// edition mode : move cursor to previous/next instruction
void program_moveCursor(short direction) {
    if (program_running) {
        return;
    }
    if (direction == -1 && currentInstruction > 0) {
        currentInstruction--;
    }
    if (direction == 1 && currentInstruction < lastInstruction) {
        currentInstruction++;
    }
    program_display();
}