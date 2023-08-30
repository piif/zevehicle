/**
 * 
 *  Vcc -> +5 or Vin ?
 *  GND -> Gnd
 *  DIN -> 12
 *  CS  -> SDA = A4
 *  CLK -> SCL = A5
 */

#include "screen.h"
#include "ledMatrix.h"

byte intensity = 0;
LedMatrix ledMatrix;

void screen_setup() {
    ledMatrix.setup(4, A5, A4, 12, intensity);
    // ledMatrix.inverted = true;
    ledMatrix.drawStringRight("ready...");
    ledMatrix.flush();
}

void screen_intensity_incr() {
    intensity = (intensity + 1) & 7;
    Serial.println(intensity);
    ledMatrix.setIntensity(intensity);
    displayValue("lum", intensity+1);
}

void displayValue(const char *message, int value) {
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

void displayChar(char c) {
    ledMatrix.clear();
    ledMatrix.drawChar(0, c);
    ledMatrix.flush();
}

// X = ledMatrix.drawString_P(X, progmem string);
// X = ledMatrix.drawString(X, string);
// X = ledMatrix.drawChar(X, char);
// ledMatrix.drawPixel(0, 7, 1);
// ledMatrix.flush();
