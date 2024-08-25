#include "View.h"

View::View()
    : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET) { }

void View::setup() {
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.clearDisplay();
    display.setTextWrap(false);
    display.display();
}

void View::displayTemp(const String& name, int temp, bool running) {
    display.clearDisplay();
    display.setTextSize(2, 1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(name);

    display.invertDisplay(editTemp);

    if (running)
        drawSnowflake();

    sprintf(tempString, "%dF", temp);
    display.setTextSize(2, 3);

    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(tempString, 0, 0, &x1, &y1, &w, &h);

    display.setCursor(SCREEN_WIDTH - w, SCREEN_HEIGHT - h);
    display.print(tempString);
    display.display();
}

void View::toggleView() {
    showFreezer = !showFreezer;
}

bool View::isShowingFreezer() const {
    return showFreezer;
}

bool View::isEditMode() const {
    return editTemp;
}

void View::setEditMode(bool mode) {
    editTemp = mode;
}

void View::drawSnowflake() {
    
    display.drawBitmap(LOGO_WIDTH, LOGO_HEIGHT, snowflake_bmp[runningIndex], LOGO_WIDTH, LOGO_HEIGHT, SSD1306_WHITE);

    if (++runningIndex > 3) {
        runningIndex = 0;
    }
}