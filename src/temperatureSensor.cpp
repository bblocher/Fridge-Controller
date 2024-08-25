#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(int pin)
    : oneWire(pin), sensors(&oneWire) {}

void TemperatureSensor::setup() {
    
    sensors.begin();

    if (!sensors.getAddress(sensorAddress, 0)) {
        Serial.println("Unable to find temperature sensor");
        while (1);
    }
    sensors.setResolution(sensorAddress, 12);
    Serial.println("Found temperature sensor.");    
}

float TemperatureSensor::readTemperature() {
    sensors.requestTemperatures();
    return sensors.getTempF(sensorAddress);
}
