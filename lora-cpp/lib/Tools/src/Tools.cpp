#include "Tools.h"

void blinkLed(int pin, int onTimeMs, int offTimeMs) {
        digitalWrite(pin, HIGH);
        delay(onTimeMs);
        digitalWrite(pin, LOW);
        delay(offTimeMs);
}

String getFormattedElapsedTime() {
    unsigned long totalSeconds = millis() / 1000;
    unsigned long hours = totalSeconds / 3600;
    unsigned long minutes = (totalSeconds % 3600) / 60;
    unsigned long seconds = totalSeconds % 60;

    char timeBuffer[9]; // HH:MM:SS\0
    sprintf(timeBuffer, "%02lu:%02lu:%02lu", hours, minutes, seconds);
    return String(timeBuffer);
}
