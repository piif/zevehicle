/**
 * 
 *  Vcc -> +5 or Vin ?
 *  GND -> Gnd
 *  DIN -> 12
 *  CS  -> SDA = A4
 *  CLK -> SCL = A5
 */

#include "screen.h"
#include "defines.h"
// #include "ledMatrix.h"

// byte intensity = 0;
// LedMatrix ledMatrix;

// void screen_setup() {
//     ledMatrix.setup(SCREEN_SIZE, SCREEN_CLK, SCREEN_DS, SCREEN_DIN, intensity);
//     // ledMatrix.inverted = true;
//     ledMatrix.drawStringRight("\0x0A\0x0B\0x0C\0x0D");
//     ledMatrix.flush();
// }

// void screen_intensity_incr() {
//     intensity = (intensity + 1) & 7;
//     Serial.println(intensity);
//     ledMatrix.setIntensity(intensity);
// }

// void screen_clear() {
//     ledMatrix.clear();
// }

// void screen_displayChar(byte y, char c) {
//     ledMatrix.drawVChar(y, c);
// }

// void screen_flush() {
//     ledMatrix.flush();
// }
