#include "Compressor.h"
#include <Arduino.h>

Compressor::Compressor(int address) : address(address) {}

void Compressor::setup() {
    if (!compressor.begin(address)) {
        Serial.print("Couldn't find compressor: ");
        Serial.println(address, HEX);
        while (1);
    }
    Serial.println("Found compressor");
}

void Compressor::loop() {
    
}

int Compressor::modeToWiper(int mode) {
    // TODO: Implement this function
    return MODE_LEVEL_4;
}

void Compressor::setMode(int mode) {
    compressor.setWiper(modeToWiper(mode));
    this->mode = mode;
}

int Compressor::getMode() const {
    return mode;
}