/**
 * 
 *  Vcc -> +5 or Vin ?
 *  GND -> Gnd
 *  DIN -> 12
 *  CS  -> SDA = A4
 *  CLK -> SCL = A5
 */

#include "screen.h"
#include "hardware.h"
#include "ledMatrix.h"

byte intensity = 0;
LedMatrix ledMatrix;

void screen_setup() {
    ledMatrix.setup(SCREEN_SIZE, SCREEN_CLK, SCREEN_DS, SCREEN_DIN, intensity);
    // ledMatrix.inverted = true;
    ledMatrix.drawStringRight("ready...");
    ledMatrix.flush();
}

void screen_intensity_incr() {
    intensity = (intensity + 1) & 7;
    Serial.println(intensity);
    ledMatrix.setIntensity(intensity);
    screen_displayValue("lum", intensity+1);
}

void screen_displayValue(const char *message, int value) {
    ledMatrix.clear();
    byte x = ledMatrix.drawString(0, message);
    x = ledMatrix.drawChar(x, ':');

    int mask = 10000;
    while(value < mask) {
        mask /= 10;
    }
    while(mask >= 1) {
        byte digit = value / mask;
        x = ledMatrix.drawChar(x, '0' + digit);
        value = value % mask;
        mask /= 10;
    }
    ledMatrix.flush();
}

void screen_clear() {
    ledMatrix.clear();
}

void screen_displayChar(byte y, char c) {
    ledMatrix.drawVChar(y, c);
}

void screen_flush() {
    ledMatrix.flush();
}
