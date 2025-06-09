#include "EnvSensor.h"

EnvironmentSensor::EnvironmentSensor(uint8_t pin, uint8_t type)
    : _dht(pin, type) {
}

void EnvironmentSensor::begin() {
    _dht.begin();
}

SensorData EnvironmentSensor::readData() {
    SensorData data;
    data.humidityPercent = _dht.readHumidity();
    data.temperatureCelsius = _dht.readTemperature(); // Reads in Celsius

    if (isnan(data.humidityPercent) || isnan(data.temperatureCelsius)) {
        data.isValid = false;
        data.temperatureCelsius = 0.0f; 
        data.humidityPercent = 0.0f;
    } else {
        data.isValid = true;
    }
    return data;
}
