#include <Arduino.h>

// Custom Libraries
#include <Tools.h>
#include <LcdDisplay.h>
#include <EnvSensor.h>
#include <Gps.h>
#include <LoRaRadio.h>

// --- Configuration ---
// LoRa Radio
const int LORA_CS_PIN = 10;   // NSS pin
const int LORA_RESET_PIN = 9; // RESET pin
const int LORA_IRQ_PIN = 2;   // DIO0 pin
const long LORA_FREQUENCY = 433E6;
const byte NODE_ADDRESS = 0xBB;      // This tracker's address
const byte BASE_ADDRESS = 0xAA;      // The base station's address

// GPS
const uint8_t GPS_RX_PIN = 8;
const uint8_t GPS_TX_PIN = 9; // Note: Conflicts with LoRa reset pin, check wiring. Let's assume wiring is correct and LoRa reset is on another pin, e.g. A0
// **ACTION REQUIRED**: Physical wiring might need to change if pins conflict. For this example, let's assume LORA_RESET_PIN is on A0.
const long GPS_BAUD_RATE = 9600;

// LCD
const int LCD_RS_PIN = 7, LCD_E_PIN = 6, LCD_D4_PIN = 5, LCD_D5_PIN = 4, LCD_D6_PIN = 3, LCD_D7_PIN = 2;
const uint8_t LCD_COLS = 16, LCD_ROWS = 2;

// DHT Sensor
const uint8_t DHT_SENSOR_PIN = 12, DHT_SENSOR_TYPE = DHT11;

// Intervals
const unsigned long LORA_SEND_INTERVAL_MS = 3000; // Send every 3 seconds
const unsigned long LCD_UPDATE_INTERVAL_MS = 1000;

// --- Global Objects ---
LoRaRadio lora(LORA_CS_PIN, A0, LORA_IRQ_PIN); // Using A0 for reset to avoid pin 9 conflict
Gps gps(GPS_RX_PIN, GPS_TX_PIN, GPS_BAUD_RATE);
LcdDisplay lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
EnvironmentSensor dhtSensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// --- Timing Variables ---
unsigned long lastLoraSendTime = 0;
unsigned long lastLcdUpdateTime = 0;

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);

    gps.begin();
    lcd.begin(LCD_COLS, LCD_ROWS);
    dhtSensor.begin();

    lcd.print(0, 0, "LoRa Tracker");
    lcd.print(0, 1, "Initializing...");
    
    if (lora.begin(LORA_FREQUENCY, NODE_ADDRESS, BASE_ADDRESS)) {
        Serial.println("LoRa Radio OK.");
        lcd.print(10, 1, "LoRa OK");
    } else {
        Serial.println("LoRa Radio Failed.");
        lcd.print(10, 1, "LoRa Fail");
        while(1); // Halt if radio fails
    }
    
    delay(1000);
    lcd.clear();
    Serial.println("System Initialized.");
}

void loop() {
    unsigned long currentTime = millis();

    // Transmit data over LoRa periodically
    if (currentTime - lastLoraSendTime >= LORA_SEND_INTERVAL_MS) {
        lastLoraSendTime = currentTime;
        GpsReading gpsData = gps.getCurrentReading();

        if (gpsData.isLocationValid) {
            digitalWrite(LED_BUILTIN, HIGH);
            
            // Construct JSON payload
            String payload = "{";
            payload += "\"lat\":" + String(gpsData.latitude, 6);
            payload += ",\"lng\":" + String(gpsData.longitude, 6);
            if (gpsData.isAltitudeValid) { payload += ",\"alt\":" + String(gpsData.altitudeMeters, 1); }
            if (gpsData.isSpeedValid) { payload += ",\"spd\":" + String(gpsData.speedMps, 1); }
            if (gpsData.isHdopValid) { payload += ",\"acc_hdop\":" + String(gpsData.hdop, 1); }
            payload += "}";

            // Send it!
            lora.sendData(payload);
            Serial.println("Sent LoRa packet: " + payload);

            digitalWrite(LED_BUILTIN, LOW);
        } else {
            Serial.println("No valid GPS data to send.");
        }
    }

    // Update the local LCD display
    if (currentTime - lastLcdUpdateTime >= LCD_UPDATE_INTERVAL_MS) {
        lastLcdUpdateTime = currentTime;
        
        lcd.print(0, 0, getFormattedElapsedTime());
        
        GpsReading gpsData = gps.getCurrentReading();
        if(gpsData.isLocationValid && gpsData.fixAgeMs < 5000) {
             lcd.print(10, 0, "GPS OK");
        } else {
             lcd.print(10, 0, "GPS No");
        }

        // Display RSSI of last received packet (if you implement 2-way)
        // For now, it will be 0 as we are only transmitting.
        lcd.print(10, 1, "RSSI " + String(lora.getLastRssi()));
    }
}