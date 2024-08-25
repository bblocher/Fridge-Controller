#ifndef CONTROLS_H
#define CONTROLS_H

#include <Compressor.h>
#include "TemperatureSensor.h"

#define DEFAULT_FRIDGE_TEMP 78
#define DEFAULT_FREEZER_TEMP -4
#define TEMP_CHECK_INTERVAL 10000

// Hysteresis table    
const int TEMPERATURE_HYSTERESIS_TABLE[] = {-1, 0, 2, 5};
    
class Controls {
public:
    Controls(int fridgeCompressorAddress, int freezerCompressorAddress, 
        int fridgeTempPin, int freezerTempPin);

    void setup();
    void loop();

    // Refrigerator control
    int getFridgeTemp() const;
    int getDesiredFridgeTemp() const;
    void decrementFreezer();
    void incrementFreezer();
    bool isFreezerRunning() const;

    // Freezer control
    int getFreezerTemp() const;
    int getDesiredFreezerTemp() const;
    void decrementFridge();
    void incrementFridge();
    bool isRefrigeratorRunning() const;

private:
    Compressor refrigeratorCompressor;
    Compressor freezerCompressor;
    TemperatureSensor fridgeTempSensor;
    TemperatureSensor freezerTempSensor;

    // Temperatures and states
    long lastTempCheck;
    float fridgeTemp;
    float freezerTemp;

    int desiredFridgeTemp = DEFAULT_FRIDGE_TEMP;    
    int desiredFreezerTemp = DEFAULT_FREEZER_TEMP;

    int calculateMode(float temp, int desiredTemp);
    float queryFridgeTemp();
    float queryFreezerTemp();
};

#endif // CONTROLS_H
