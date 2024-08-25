#ifndef BUTTONS_H
#define BUTTONS_H

#include <Adafruit_DS3502.h>
#include "TemperatureSensor.h"

#define DEBOUNCE 100
    
class Buttons {
public:
    Buttons(int leftTouchPin, int rightTouchPin);

    void setup();
    void loop();
    bool wasLeftTouched() const;
    bool wasRightTouched() const;
    bool wasBothTouched() const;
    bool wasStateChanged() const;

private:
    void resetButtons();
    
    // Button state tracking
    unsigned long leftLastDebounceTime = 0;
    unsigned long rightLastDebounceTime = 0;
    bool leftTouched;
    bool rightTouched;
    bool bothTouched;
    bool stateChanged;

    // Button pins
    int leftTouchPin;
    int rightTouchPin;
};

#endif // BUTTONS_H
