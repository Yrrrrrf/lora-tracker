#ifndef GPS_H
#define GPS_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>
#include <limits.h> // For ULONG_MAX

struct GpsReading {
    double latitude = 0.0;
    double longitude = 0.0;
    float altitudeMeters = 0.0;
    float speedMps = 0.0;
    float hdop = 0.0;
    bool isLocationValid = false;
    bool isAltitudeValid = false;
    bool isSpeedValid = false;
    bool isHdopValid = false;
    unsigned long fixAgeMs = ULONG_MAX;
};

class Gps {
public:
    Gps(uint8_t rxPin, uint8_t txPin, long baudRate = 9600);
    void begin();
    bool processIncomingData();
    GpsReading getCurrentReading();

private:
    SoftwareSerial _gpsSerial;
    TinyGPSPlus _gpsParser;
    GpsReading _currentReading;
    bool _newDataAvailable;
};

#endif