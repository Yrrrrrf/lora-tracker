#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Arduino.h>
#include <LiquidCrystal.h>

class LcdDisplay {
public:
    LcdDisplay(int rs, int enable, int d4, int d5, int d6, int d7);
    void begin(uint8_t cols, uint8_t rows);
    void print(uint8_t col, uint8_t row, const String& message);
    void clear();

private:
    LiquidCrystal _lcd;
};

#endif
