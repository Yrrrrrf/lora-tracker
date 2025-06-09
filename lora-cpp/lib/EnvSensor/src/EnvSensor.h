#ifndef ENV_SENSOR_H
#define ENV_SENSOR_H

#include <Arduino.h>
#include <DHT.h>

struct SensorData {
    float temperatureCelsius;
    float humidityPercent;
    bool isValid;
};

class EnvironmentSensor {
public:
    EnvironmentSensor(uint8_t pin, uint8_t type);
    void begin();
    SensorData readData();

private:
    DHT _dht;
};

#endif
