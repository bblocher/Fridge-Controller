#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <OneWire.h>
#include <DallasTemperature.h>

class TemperatureSensor {
public:
    TemperatureSensor(int pin); // Constructor that takes the pin number as a parameter
    void setup(); // Initialize the sensor
    float readTemperature(); // Read the temperature in Celsius

private:
    OneWire oneWire;
    DallasTemperature sensors;
    DeviceAddress sensorAddress;
};

#endif // TEMPERATURE_SENSOR_H
