#include <Arduino.h>
#include <avr/eeprom.h>

#include "defines.h"
#include "remote.h"
#include "move.h"
#include "screen.h"
#include "program.h"

void(* resetFunc) (void) = 0;

#ifndef DEFAULT_BAUDRATE
	#define DEFAULT_BAUDRATE 115200
#endif

// on second lego prototype :
// 336 steps to move forward/backward 26cm
// 91 steps to turn a quarter

static unsigned int EEMEM forward_steps, turn_steps;

void setupMove() {
    unsigned int fs, ts;
    eeprom_read_block(&fs, &forward_steps, sizeof(int));
    eeprom_read_block(&ts, &turn_steps, sizeof(int));

    move_setup(fs, ts);
}

void setup() {
	Serial.begin(DEFAULT_BAUDRATE);
    Serial.println("Begin setup");

    setupMove();

    remoteIR_Setup();

    screen_setup();

    program_display();
    Serial.println("End setup");
}

void setup_menu(const char *header, unsigned int *valueAddress) {
    unsigned int value;
    eeprom_read_block(&value, valueAddress, sizeof(int));

    Serial.print(header);Serial.print(" -> ");Serial.println(value);
    screen_clear();
    screen_print(header);
    screen_setPos(1, 0);
    screen_print(value);

    for(;;) {
        remoteIR_keyCode irKey = remoteIR_check();

        if (irKey != 0) {
            switch (irKey) {
                case IR_DIRECTION_UP:
                    value++;
                break;
                case IR_DIRECTION_DOWN:
                    value--;
                break;
                case IR_VOLUME_UP:
                case IR_PROGRAM_UP:
                    value+=10;
                break;
                case IR_VOLUME_DOWN:
                case IR_PROGRAM_DOWN:
                    value-=10;
                break;
                case IR_OK:
                    eeprom_update_block(&value, valueAddress, sizeof(int));
                    return;
                break;
            }
            screen_setPos(1, 0);
            screen_print(value);
            screen_print("   ");
        }
    }
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
            case IR_RED:
                setup_menu("forward steps", &forward_steps);
                setupMove();
                program_display();
            break;
            case IR_GREEN:
                setup_menu("turn steps", &turn_steps);
                setupMove();
                program_display();
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
