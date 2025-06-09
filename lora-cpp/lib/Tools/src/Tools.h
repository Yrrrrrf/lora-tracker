#ifndef TOOLS_H
#define TOOLS_H

#include <Arduino.h>

void blinkLed(int pin, int onTimeMs, int offTimeMs, int repetitions = 1);
String getFormattedElapsedTime();

#endif
