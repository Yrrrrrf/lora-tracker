#include "LcdDisplay.h"

LcdDisplay::LcdDisplay(int rs, int enable, int d4, int d5, int d6, int d7)
    : _lcd(rs, enable, d4, d5, d6, d7) {
}

void LcdDisplay::begin(uint8_t cols, uint8_t rows) {
    _lcd.begin(cols, rows);
}

void LcdDisplay::print(uint8_t col, uint8_t row, const String& message) {
    _lcd.setCursor(col, row);
    _lcd.print(message);
}

void LcdDisplay::clear() {
    _lcd.clear();
}
