#include <Arduino.h>
#include "serialInput.h"

#include "defines.h"
#include "remote.h"
#include "move.h"
#include "screen.h"
#include "program.h"

void(* resetFunc) (void) = 0;

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
    Serial.println("Begin setup");

    move_setup();

    remoteIR_Setup();

    screen_setup();

    program_display();
    Serial.println("End setup");
}

void loop() {

    remoteIR_keyCode irKey = remoteIR_check();

    if (irKey != 0) {
        // Serial.print("Pressed "); Serial.println(irKey, HEX);
        switch (irKey) {
            case IR_DIRECTION_UP:
                Serial.println("UP");
                program_push(CHAR_UP);
            break;
            case IR_DIRECTION_DOWN:
                Serial.println("DOWN");
                program_push(CHAR_DOWN);
            break;
            case IR_DIRECTION_LEFT:
                Serial.println("LEFT");
                program_push(CHAR_LEFT);
            break;
            case IR_DIRECTION_RIGHT:
                Serial.println("RIGHT");
                program_push(CHAR_RIGHT);
            break;
            case IR_PROGRAM_DOWN:
                Serial.println("Cursor down");
                program_moveCursor(-1);
            break;
            case IR_PROGRAM_UP:
                Serial.println("Cursor up");
                program_moveCursor(+1);
            break;
            case IR_CANCEL:
                Serial.println("Remove");
                program_pop();
            break;
            case IR_OK:
                Serial.println("RUN");
                if (program_running) {
                    program_break();
                } else {
                    program_run();
                }
            break;
            case IR_POWER:
                Serial.println("REBOOT");
                move_stop();
                resetFunc();
            break;
            case IR_TEXT:
                Serial.println("TEXT");
                screen_intensity_incr();
            break;
        }
    }

    // TODO : use setInterval or timer + interuptions instead ?
    if (program_running) {
        if (move_update()) {
            #ifdef DELAY_BETWEEN_INSTRUCTIONS
            delay(DELAY_BETWEEN_INSTRUCTIONS);
            #endif
            program_nextInstruction();
        }
    }
}
