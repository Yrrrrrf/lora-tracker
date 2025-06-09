#include "Gps.h"

Gps::Gps(uint8_t rxPin, uint8_t txPin, long baudRate)
    : _gpsSerial(rxPin, txPin), _newDataAvailable(false) {
    _gpsSerial.begin(baudRate);
}

void Gps::begin() {
    // SoftwareSerial is begun in constructor
}

bool Gps::processIncomingData() {
    _newDataAvailable = false;
    while (_gpsSerial.available() > 0) {
        if (_gpsParser.encode(_gpsSerial.read())) {
            _newDataAvailable = true; 
            
            _currentReading.isLocationValid = _gpsParser.location.isValid();
            if (_currentReading.isLocationValid) {
                _currentReading.latitude = _gpsParser.location.lat();
                _currentReading.longitude = _gpsParser.location.lng();
            }

            _currentReading.isAltitudeValid = _gpsParser.altitude.isValid();
            if (_currentReading.isAltitudeValid) {
                _currentReading.altitudeMeters = _gpsParser.altitude.meters();
            }

            _currentReading.isSpeedValid = _gpsParser.speed.isValid();
            if (_currentReading.isSpeedValid) {
                _currentReading.speedMps = _gpsParser.speed.mps();
            }

            _currentReading.isHdopValid = _gpsParser.hdop.isValid();
            if(_currentReading.isHdopValid){
                _currentReading.hdop = _gpsParser.hdop.value() / 100.0;
            }
            _currentReading.fixAgeMs = _gpsParser.location.age();
        }
    }
    return _newDataAvailable && _currentReading.isLocationValid;
}

GpsReading Gps::getCurrentReading() {
    return _currentReading;
}
