#include <Arduino.h>

// Custom Libraries
#include <Tools.h>
#include <LcdDisplay.h>
#include <EnvSensor.h>
#include <Gps.h>

// --- Configuration ---
// GPS
const uint8_t GPS_RX_PIN = 8;
const uint8_t GPS_TX_PIN = 9;
const long GPS_BAUD_RATE = 9600;

// LCD
const int LCD_RS_PIN = 7;
const int LCD_E_PIN = 6;
const int LCD_D4_PIN = 5;
const int LCD_D5_PIN = 4;
const int LCD_D6_PIN = 3;
const int LCD_D7_PIN = 2;
const uint8_t LCD_COLS = 16;
const uint8_t LCD_ROWS = 2;

// DHT Sensor
const uint8_t DHT_SENSOR_PIN = 12;
const uint8_t DHT_SENSOR_TYPE = DHT11; // Or DHT22, DHT21

// Intervals
const unsigned long SERIAL_SEND_INTERVAL_MS = 2000;
const unsigned long LCD_UPDATE_INTERVAL_MS = 1000;

// --- Global Objects ---
Gps gps(GPS_RX_PIN, GPS_TX_PIN, GPS_BAUD_RATE); // Changed class name
LcdDisplay lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
EnvironmentSensor dhtSensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

// --- Timing Variables ---
unsigned long lastSerialSendTime = 0;
unsigned long lastLcdUpdateTime = 0;

// void setup() {
//     Serial.begin(9600);
//     pinMode(LED_BUILTIN, OUTPUT);

//     gps.begin(); // Changed object name
//     lcd.begin(LCD_COLS, LCD_ROWS);
//     dhtSensor.begin();

//     lcd.print(0, 0, "LoRa Tracker");
//     lcd.print(0, 1, "Initializing...");
//     blinkLed(LED_BUILTIN, 100, 100, 3);
//     lcd.clear();
//     Serial.println("System Initialized.");
// }

// void loop() {
//     bool newGpsDataProcessed = gps.processIncomingData(); // Changed object name

//     unsigned long currentTime = millis();

//     if (currentTime - lastSerialSendTime >= SERIAL_SEND_INTERVAL_MS) {
//         lastSerialSendTime = currentTime;
//         GpsReading gpsData = gps.getCurrentReading(); // Changed object name

//         if (gpsData.isLocationValid) {
//             digitalWrite(LED_BUILTIN, HIGH);
//             Serial.print("{\"lat\":"); Serial.print(gpsData.latitude, 6);
//             Serial.print(",\"lng\":"); Serial.print(gpsData.longitude, 6);
//             if (gpsData.isAltitudeValid) { Serial.print(",\"alt\":"); Serial.print(gpsData.altitudeMeters, 1); }
//             if (gpsData.isSpeedValid) { Serial.print(",\"spd\":"); Serial.print(gpsData.speedMps, 1); }
//             if (gpsData.isHdopValid) { Serial.print(",\"acc_hdop\":"); Serial.print(gpsData.hdop, 1); }
//             Serial.print(",\"ts_millis\":"); Serial.print(currentTime);
//             Serial.println("}");
//             digitalWrite(LED_BUILTIN, LOW);
//         }
//     }

//     if (currentTime - lastLcdUpdateTime >= LCD_UPDATE_INTERVAL_MS) {
//         lastLcdUpdateTime = currentTime;
        
//         lcd.print(0, 0, getFormattedElapsedTime());

//         SensorData sensorReadings = dhtSensor.readData();
//         if (sensorReadings.isValid) {
//             String tempStr = String(sensorReadings.temperatureCelsius, 1) + "C";
//             String humStr = String(sensorReadings.humidityPercent, 0) + "% ";
//             lcd.print(0, 1, tempStr + " " + humStr);
//         } else {
//             lcd.print(0, 1, "Sensor Err");
//         }

//         GpsReading gpsData = gps.getCurrentReading(); // Changed object name
//         if(gpsData.isLocationValid && gpsData.fixAgeMs < 5000) {
//              lcd.print(10, 0, "GPS OK");
//         } else {
//              lcd.print(10, 0, "GPS No");
//         }
//     }
// }