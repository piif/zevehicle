#include "program.h"
#include "hardware.h"
#include "move.h"
#include "screen.h"

#define PROGRAM_MAX 20
char program[PROGRAM_MAX+1];
byte currentInstruction = 0;
byte lastInstruction = 0;

bool program_running = false;


void program_display() {
    // afficher une tranche autour de current
    // en collant en haut ou en bas si on est prêt du bord
    // en collant en haut s'il y a moins de 4 instructions (ou pas : partir du bas ?)
    for(byte i=0; i<lastInstruction; i++) {
        if (i == currentInstruction) {
            Serial.print('>');
        }
        Serial.print(program[i]);
    }
    Serial.println();

    screen_clear();

    byte offset;
    if (lastInstruction < SCREEN_SIZE || currentInstruction == 0) {
        offset = 0;
    } else if (lastInstruction - currentInstruction < SCREEN_SIZE-1) {
        offset = lastInstruction - (SCREEN_SIZE-1);
    } else {
        offset = currentInstruction - 1;
    }

    for (short y = SCREEN_SIZE-1; (y >= 0) && (offset <= lastInstruction); y--, offset++) {
        if (offset == lastInstruction) {
            screen_displayChar(y, '?');
        } else {
            screen_displayChar(y, program[offset]);
        }
        if (offset == currentInstruction) {
            screen_displayChar(y, 0x7f);
        }
    }
    screen_flush();
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
    if (currentInstruction == lastInstruction) {
        move_stop();
        program_running = false;
    }
    switch(program[currentInstruction]) {
        case ARROW_UP:
            move_start(0, 0, FORWARD_STEPS);
        break;
        case ARROW_DOWN:
            move_start(1, 1, FORWARD_STEPS);
        break;
        case ARROW_LEFT:
            move_start(0, 1, TURN_STEPS);
        break;
        case ARROW_RIGHT:
            move_start(1, 0, TURN_STEPS);
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
    program_running = false;
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