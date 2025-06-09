#include <Arduino.h>
#include <Tools.h> // Only need this for getFormattedElapsedTime if you want it, or can mock time too

// --- Configuration ---
const unsigned long SERIAL_SEND_INTERVAL_MS = 2000; // Send data every 2 seconds

// --- Mock Data Variables ---
double mockLatitude = 19.432600;
double mockLongitude = -99.133200;
float mockAltitude = 2250.0;
float mockSpeed = 1.5;
float mockHdop = 1.2;
unsigned long lastSerialSendTime = 0;

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Mock GPS System Initialized. Sending simulated data...");
}

void loop() {
    // unsigned long currentTime = millis();

    // if (currentTime - lastSerialSendTime >= SERIAL_SEND_INTERVAL_MS) {
    //     lastSerialSendTime = currentTime;

        blinkLed(LED_BUILTIN, 500, 100);

        // Simulate some movement/change in data
        mockLatitude += (random(-10, 11) / 100000.0); // Tiny random change
        mockLongitude += (random(-10, 11) / 100000.0); // Tiny random change
        mockAltitude += (random(-10, 11) / 10.0); // +/- 1.0 m
        mockSpeed = (random(0, 50) / 10.0); // Random speed 0.0 to 5.0 m/s
        mockHdop = (random(8, 25) / 10.0);  // Random HDOP 0.8 to 2.5

        // Ensure latitude and longitude stay within reasonable bounds for demo
        if (mockLatitude > 90.0) mockLatitude = 90.0;
        if (mockLatitude < -90.0) mockLatitude = -90.0;
        if (mockLongitude > 180.0) mockLongitude = 180.0;
        if (mockLongitude < -180.0) mockLongitude = -180.0;
        if (mockAltitude < 0) mockAltitude = 0;
        if (mockSpeed < 0) mockSpeed = 0;

        // digitalWrite(LED_BUILTIN, HIGH); // Indicate sending data

        Serial.print("{\"lat\":"); Serial.print(mockLatitude, 6);
        Serial.print(",\"lng\":"); Serial.print(mockLongitude, 6);
        Serial.print(",\"alt\":"); Serial.print(mockAltitude, 1);
        Serial.print(",\"spd\":"); Serial.print(mockSpeed, 1);
        Serial.print(",\"acc_hdop\":"); Serial.print(mockHdop, 1);
        // Serial.print(",\"ts_millis\":"); Serial.print(currentTime); // Use current millis as timestamp
        Serial.println("}");

        // digitalWrite(LED_BUILTIN, LOW);
    // }
}