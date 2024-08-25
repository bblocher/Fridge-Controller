#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <Adafruit_DS3502.h>

#define MODE_OFF INT16_MAX
#define MODE_LEVEL_1 0
#define MODE_LEVEL_2 277
#define MODE_LEVEL_3 692
#define MODE_LEVEL_4 1780

#define WIPER_MIN 0
#define WIPER_MAX 127

#define ERROR_BATTERY_VOLTAGE_PROTECTION 0x01
#define ERROR_COOLING_FAN_OVERCURRENT 0x02
#define ERROR_COMPRESSOR_FAILED_TO_START 0x03
#define ERROR_MIN_ROTARY_SPEED 0x04
#define ERROR_CONTROLLER_OVERHEAT 0x05
    
class Compressor {
public:
    Compressor(int address);

    void setup();
    void loop();
    void setMode(int mode);
    int getMode() const;

private:    
    Adafruit_DS3502 compressor;
    int address;
    int mode;

    int modeToWiper(int mode);
};

#endif // COMPRESSOR_H
