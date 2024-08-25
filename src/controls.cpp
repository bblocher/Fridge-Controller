#include "Controls.h"
#include <Arduino.h>

Controls::Controls(int fridgeCompressorAddress, int freezerCompressorAddress, 
                    int fridgeTempPin, int freezerTempPin)
    :   refrigeratorCompressor(fridgeCompressorAddress), freezerCompressor(freezerCompressorAddress),
        fridgeTempSensor(fridgeTempPin), freezerTempSensor(freezerTempPin){}

void Controls::setup() {
    refrigeratorCompressor.setup();
    freezerCompressor.setup();
    fridgeTempSensor.setup();
    freezerTempSensor.setup();
}

void Controls::loop() {
    unsigned long now = millis();
    if (now - lastTempCheck < TEMP_CHECK_INTERVAL) {
        return;
    }

    lastTempCheck = now;
    fridgeTemp = queryFridgeTemp();
    freezerTemp = queryFreezerTemp();

    int fridgeMode = calculateMode(fridgeTemp, desiredFridgeTemp);
    if (fridgeMode != refrigeratorCompressor.getMode()) {
        refrigeratorCompressor.setMode(fridgeMode);
        Serial.println("Changing fridge mode to " + String(fridgeMode));
    }

    int freezerMode = calculateMode(freezerTemp, desiredFreezerTemp);
    if (freezerMode != freezerCompressor.getMode()) {
        freezerCompressor.setMode(freezerMode);
        Serial.println("Changing freezer mode to " + String(freezerMode));
    }
}

int Controls::getFridgeTemp() const {
    return fridgeTemp;
}

int Controls::getDesiredFridgeTemp() const {
    return desiredFridgeTemp;
}

void Controls::decrementFridge() {
    desiredFridgeTemp--;
}

void Controls::incrementFridge() {
    desiredFridgeTemp++;
}

bool Controls::isRefrigeratorRunning() const {
    return refrigeratorCompressor.getMode() != MODE_OFF;
}

int Controls::getFreezerTemp() const {
    return freezerTemp;
}

int Controls::getDesiredFreezerTemp() const {
    return desiredFreezerTemp;
}

void Controls::decrementFreezer() {
    desiredFreezerTemp--;
}

void Controls::incrementFreezer() {
    desiredFreezerTemp++;
}

bool Controls::isFreezerRunning() const {
    return freezerCompressor.getMode() != MODE_OFF;
}

int Controls::calculateMode(float temp, int desiredTemp) {
    if (temp <= desiredTemp + TEMPERATURE_HYSTERESIS_TABLE[0])
        return MODE_OFF;
    else if (temp <= desiredTemp + TEMPERATURE_HYSTERESIS_TABLE[1])
        return MODE_LEVEL_1;
    else if (temp <= desiredTemp + TEMPERATURE_HYSTERESIS_TABLE[2])
        return MODE_LEVEL_2;
    else if (temp <= desiredTemp + TEMPERATURE_HYSTERESIS_TABLE[3])
        return MODE_LEVEL_3;
    else
        return MODE_LEVEL_4;
}

float Controls::queryFridgeTemp() {
    return fridgeTempSensor.readTemperature();
}

float Controls::queryFreezerTemp() {
    return freezerTempSensor.readTemperature();
}

