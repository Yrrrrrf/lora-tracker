#include <Arduino.h>
#include <LoRaRadio.h>

// --- Configuration ---
// LoRa Radio
const int LORA_CS_PIN = 10;
const int LORA_RESET_PIN = 9;
const int LORA_IRQ_PIN = 2;
const long LORA_FREQUENCY = 433E6;
const byte NODE_ADDRESS = 0xAA;      // This base station's address
const byte TRACKER_ADDRESS = 0xBB;   // The tracker's address (not strictly needed for receiving, but good practice)

// --- Global Objects ---
LoRaRadio lora(LORA_CS_PIN, LORA_RESET_PIN, LORA_IRQ_PIN);

// void setup() {
//     Serial.begin(9600); // This must match the baud rate in your Python script
//     pinMode(LED_BUILTIN, OUTPUT);

//     if (lora.begin(LORA_FREQUENCY, NODE_ADDRESS, TRACKER_ADDRESS)) {
//         Serial.println("LoRa Receiver Base Station Initialized.");
//         digitalWrite(LED_BUILTIN, HIGH);
//         delay(500);
//         digitalWrite(LED_BUILTIN, LOW);
//     } else {
//         Serial.println("LoRa Radio Failed. Halting.");
//         while(1);
//     }
// }

// void loop() {
//     // Check for incoming data
//     String payload = lora.receiveData();

//     // If a valid packet was received, print it to the serial port
//     if (payload.length() > 0) {
//         digitalWrite(LED_BUILTIN, HIGH);
//         Serial.println(payload); // This is what the Python bridge will read
//         digitalWrite(LED_BUILTIN, LOW);

//         // Optional: Print RSSI to debug console for signal strength
//         // Note: This won't be read by the Python bridge unless you modify it.
//         // Serial.print("-- RSSI: ");
//         // Serial.println(lora.getLastRssi());
//     }
// }