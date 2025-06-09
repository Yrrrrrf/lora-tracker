#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>

void blinkLed(int pin, int onTimeMs, int offTimeMs);
String getFormattedElapsedTime();

#endif
