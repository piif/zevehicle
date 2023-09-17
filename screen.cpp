#include "screen.h"
#include "defines.h"

#include <LiquidCrystal.h>

// Création de l'objet lcd (avec les différents ports numériques qu'il utilise)
// = RS, Enable, D0, D1, D2, D3
static LiquidCrystal lcd(SCREEN_READ_SEL, SCREEN_ENABLE, SCREEN_D4, SCREEN_D5, SCREEN_D6, SCREEN_D7);

static byte char_up[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00000,
  0b00000,
  0b00000,
};
static byte char_down[8] = {
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
  0b00000,
  0b00000,
};
static byte char_left[8] = {
  0b00100,
  0b01000,
  0b11110,
  0b01001,
  0b00101,
  0b00001,
  0b00000,
  0b00000,
};
static byte char_right[8] = {
  0b00100,
  0b00010,
  0b01111,
  0b10010,
  0b10100,
  0b10000,
  0b00000,
  0b00000,
};
static byte char_up_ul[8] = {
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b00100,
  0b00000,
  0b11111,
  0b00000,
};
static byte char_down_ul[8] = {
  0b00100,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00000,
  0b11111,
  0b00000,
};
static byte char_left_ul[8] = {
  0b00100,
  0b01000,
  0b11110,
  0b01001,
  0b00101,
  0b00000,
  0b11111,
  0b00000,
};
static byte char_right_ul[8] = {
  0b00100,
  0b00010,
  0b01111,
  0b10010,
  0b10100,
  0b00000,
  0b11111,
  0b00000,
};

static byte screen_light = 0x40;

void screen_setup() {
    pinMode(SCREEN_LIGHT, OUTPUT);
    analogWrite(SCREEN_LIGHT, screen_light);
    lcd.begin(16, 2);
    lcd.createChar(0, char_up);
    lcd.createChar(1, char_down);
    lcd.createChar(2, char_left);
    lcd.createChar(3, char_right);
    lcd.createChar(4, char_up_ul);
    lcd.createChar(5, char_down_ul);
    lcd.createChar(6, char_left_ul);
    lcd.createChar(7, char_right_ul);
    lcd.display();
}

void screen_intensity_incr() {
    screen_light += 0x40;
    analogWrite(SCREEN_LIGHT, screen_light);
}

void screen_clear() {
    lcd.clear();
}

void screen_setPos(byte row, byte col) {
    lcd.setCursor(col, row);
}

void screen_print(char c) {
    lcd.print(c);
}
void screen_print(const char *s) {
    lcd.print(s);
}
void screen_print(unsigned int v) {
    if (v == 0) {
        screen_print('0');
        return;
    }
    unsigned int mask = 10000;
    while (v < mask) {
        mask /= 10;
    }
    while (mask > 0) {
        char digit = (v / mask) + '0';
        screen_print(digit);

        v = v % mask;
        mask /= 10;
    }
}
