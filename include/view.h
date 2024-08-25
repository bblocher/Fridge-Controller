#ifndef VIEW_H
#define VIEW_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define SCREEN_ADDRESS 0x3C
#define OLED_RESET -1

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16

static const unsigned char PROGMEM snowflake_bmp[4][32] =
{
    {
        0b00000000, 0b11000000,
        0b00000001, 0b11000000,
        0b00000001, 0b11000000,
        0b00000011, 0b11100000,
        0b11110011, 0b11100000,
        0b11111110, 0b11111000,
        0b01111110, 0b11111111,
        0b00110011, 0b10011111,
        0b00011111, 0b11111100,
        0b00001101, 0b01110000,
        0b00011011, 0b10100000,
        0b00111111, 0b11100000,
        0b00111111, 0b11110000,
        0b01111100, 0b11110000,
        0b01110000, 0b01110000,
        0b00000000, 0b00110000
    },
    {
        0b00000000, 0b00110000,
        0b01100000, 0b01110000,
        0b01111000, 0b11110000,
        0b01111101, 0b11110000,
        0b00111111, 0b01100000,
        0b00111011, 0b01100000,
        0b00011101, 0b11111000,
        0b00011111, 0b10011110,
        0b00111101, 0b11111111,
        0b01111011, 0b01111111,
        0b11111111, 0b01111000,
        0b11110011, 0b11100000,
        0b00000001, 0b11100000,
        0b00000001, 0b11000000,
        0b00000000, 0b11000000,
        0b00000000, 0b11000000
    },
    {
        0b00001100, 0b00000000,
        0b00001110, 0b00001110,
        0b00001111, 0b00111110,
        0b00001111, 0b11111100,
        0b00000111, 0b11111100,
        0b00000101, 0b11011000,
        0b00001110, 0b10110000,
        0b00111111, 0b11111000,
        0b11111001, 0b11001100,
        0b11111111, 0b01111110,
        0b00011111, 0b01111111,
        0b00000111, 0b11001111,
        0b00000111, 0b11000000,
        0b00000011, 0b10000000,
        0b00000011, 0b10000000,
        0b00000011, 0b00000000
    },
    {
        0b00000011, 0b00000000,
        0b00000011, 0b00000000,
        0b00000011, 0b10000000,
        0b00000111, 0b10000000,
        0b00000111, 0b11001111,
        0b00011110, 0b11111111,
        0b11111110, 0b11011110,
        0b11111111, 0b10111100,
        0b01111001, 0b11111000,
        0b00011111, 0b10111000,
        0b00000110, 0b11011100,
        0b00000110, 0b11111100,
        0b00001111, 0b10111110,
        0b00001111, 0b00011110,
        0b00001110, 0b00000110,
        0b00001100, 0b00000000
    }
};

class View {
public:
    View(); // Constructor
    void setup(); // Initialize the display
    void loop();
    void displayTemp(const String& name, int temp, bool running);
    void toggleView();

    // Accessors for UI state
    bool isShowingFreezer() const;
    bool isEditMode() const;
    void setEditMode(bool mode);

private:
    Adafruit_SSD1306 display;
    char tempString[6];
    
    // UI state
    bool showFreezer = false;
    bool editTemp = false;
    int runningIndex = 0;
    
    void drawSnowflake();
};

#endif // VIEW_H
