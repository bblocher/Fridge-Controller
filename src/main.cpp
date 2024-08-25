#include "View.h"
#include "Controls.h"
#include "Buttons.h"

#ifndef FREEZER_TEMP_PIN
#define FREEZER_TEMP_PIN 0x12 // 18
#endif

#ifndef FRIDGE_TEMP_PIN
#define FRIDGE_TEMP_PIN 0x13 // 19
#endif

#ifndef REFER_ADDRESS
#define REFER_ADDRESS 0x28
#endif

#ifndef FREEZER_ADDRESS
#define FREEZER_ADDRESS 0x29
#endif

#ifndef LEFT_TOUCH_PIN
#define LEFT_TOUCH_PIN 0x10
#endif  

#ifndef RIGHT_TOUCH_PIN
#define RIGHT_TOUCH_PIN 0x11
#endif

View view;
Controls controls(REFER_ADDRESS, FREEZER_ADDRESS, FRIDGE_TEMP_PIN, FREEZER_TEMP_PIN);
Buttons buttons(LEFT_TOUCH_PIN, RIGHT_TOUCH_PIN);

unsigned long lastViewToggle = 0;

void setup() {
    Serial.begin(9600);
    Serial.flush();
    delay(1000);

    while (!Serial) {
        delay(1);
    }

    controls.setup();
    view.setup();
    buttons.setup();
}

void loop() {
    unsigned long now = millis();
    controls.loop();
    
    if (!view.isEditMode() && now - lastViewToggle > 5000) {
        lastViewToggle = now;
        view.toggleView();
    }

    if (view.isShowingFreezer()) {
        view.displayTemp("Freezer", view.isEditMode() ? controls.getDesiredFreezerTemp() : controls.getFreezerTemp(), controls.isFreezerRunning());
    } else {
        view.displayTemp("Fridge", view.isEditMode() ? controls.getDesiredFridgeTemp() : controls.getFridgeTemp(), controls.isRefrigeratorRunning());
    }

    buttons.loop();
    if (buttons.wasStateChanged())
    {
        if (buttons.wasBothTouched()) {
            view.setEditMode(!view.isEditMode());
        } else if (view.isEditMode()) {
            if (buttons.wasLeftTouched()) {
                if (view.isShowingFreezer()) {
                    controls.decrementFreezer();
                } else {
                    controls.decrementFridge();
                }
            } else if (buttons.wasRightTouched()) {
                if (view.isShowingFreezer()) {
                    controls.incrementFreezer();
                } else {
                    controls.incrementFridge();
                }
            } else {
                view.setEditMode(false);
            }
        } else {
            if (buttons.wasLeftTouched() || buttons.wasRightTouched()) {
                view.toggleView();
            }
        }
    }

    delay(100);
}
