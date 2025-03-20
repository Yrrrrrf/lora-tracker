#include <Arduino.h>

// ^ Declarations...
void blinkPin(int, int, int);


// * Some fn's declarations...
void blinkPin(int pin, int onTime, int offTime) {
  for (int i = 0; i < 2; i++) {
    digitalWrite(pin, i % 2 == 0 ? HIGH : LOW);
    delay(i % 2 == 0 ? onTime : offTime);
  }
}

// * Main code --------------------------------------------------------

// * Setup fn (runs once at the beginning)
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

// * Main loop
void loop() {
  blinkPin(LED_BUILTIN, 500, 100);
}
