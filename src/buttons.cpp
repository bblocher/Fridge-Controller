#include "Buttons.h"
#include <Arduino.h>

Buttons::Buttons(int leftTouchPin, int rightTouchPin) :
        leftTouchPin(leftTouchPin), rightTouchPin(rightTouchPin){}

void Buttons::setup() {
    pinMode(leftTouchPin, INPUT);
    pinMode(rightTouchPin, INPUT);

    Serial.println("Buttons initialized");
}

void Buttons::resetButtons() {
    leftLastDebounceTime = 0;
    rightLastDebounceTime = 0;
    leftTouched = false;
    rightTouched = false;
}

void Buttons::loop() {
    bool isLeftTouched = digitalRead(leftTouchPin) == HIGH;
    unsigned long now = millis();

    stateChanged = false;
    if (isLeftTouched && leftLastDebounceTime == 0) {
        leftLastDebounceTime = now;
    }

    bool isRightTouched = digitalRead(rightTouchPin) == HIGH;

    if (isRightTouched && rightLastDebounceTime == 0) {
        rightLastDebounceTime = now;
    }

    bothTouched = isLeftTouched && isRightTouched;

    if (leftTouched && rightTouched && !bothTouched) {
        resetButtons();
        return;
    }

    if (!isLeftTouched && !isRightTouched) {
        resetButtons();
        return;
    }

    if (isLeftTouched == leftTouched && isRightTouched == rightTouched) {
        return;
    }

    if ((isLeftTouched && now - leftLastDebounceTime < DEBOUNCE) || 
        (isRightTouched && now - rightLastDebounceTime < DEBOUNCE)) {
        return;
    }

    leftTouched = isLeftTouched;
    rightTouched = isRightTouched;
    stateChanged = true;
}

bool Buttons::wasLeftTouched() const {
    return leftTouched;
}

bool Buttons::wasRightTouched() const {
    return rightTouched;
}

bool Buttons::wasBothTouched() const {
    return bothTouched;
}

bool Buttons::wasStateChanged() const {
    return stateChanged;
}
