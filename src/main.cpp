#include <Adafruit_DS3502.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#define REFER_PIN 0x28
#define FREEZER_PIN 0x29
#define LEFT_TOUCH_PIN 0x10
#define RIGHT_TOUCH_PIN 0x11

#define DEBOUNCE 100
#define VIEW_TOGGLE 5000

Adafruit_DS3502 refrigeratorCompressor = Adafruit_DS3502();
Adafruit_DS3502 freezerCompressor = Adafruit_DS3502();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char tempString[6];

// UI state
bool showFreezer = false;
bool editTemp = false;
int fridgeTemp = 40;
int freezerTemp = -4;
bool freezerRunning = false;
bool refrigeratorRunning = true;
int runningIndex = 0;

// Track when the button was last pressed
unsigned long lastViewToggle = 0;
unsigned long leftLastDebounceTime = 0;
unsigned long rightLastDebounceTime = 0;
bool wasLeftTouched = false;
bool wasRightTouched = false;

void displaySetup();
void compressorSetup();

void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.flush();
  delay(1000);

  // Wait until serial port is opened
  while (!Serial) { delay(1); }

  pinMode(LEFT_TOUCH_PIN, INPUT);
  pinMode(RIGHT_TOUCH_PIN, INPUT);

  compressorSetup();
  displaySetup();
}

void compressorSetup() {
  if (!refrigeratorCompressor.begin(REFER_PIN)) {
    Serial.println("Couldn't find Refrigerator compressor");
    while (1);
  }
  Serial.println("Found Refrigerator compressor");

  if (!freezerCompressor.begin(FREEZER_PIN)) {
    Serial.println("Couldn't find Freezer compressor");
    while (1);
  }
  Serial.println("Found Freezer compressor");
}

void displaySetup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextWrap(false);
  display.display();  
}

void drawTemp(String name, bool running, int temp) {
  display.clearDisplay();

  display.setTextSize(2, 1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println(name);

  // Invert the display to indicate edit mode
  display.invertDisplay(editTemp);

  // Show if the compressor is running
  if (running) {
    display.setTextSize(1, 1);
    display.setCursor(runningIndex++, SCREEN_HEIGHT - 10);

    // Cycle through the running characters
    String runningChar = "|/-\\";
    display.print(runningChar[runningIndex % 4]);
    
    if (runningIndex > SCREEN_WIDTH / 2) {
      runningIndex = 0;
    }
  }
    
  // Show current temperature
  sprintf(tempString, "%dF", temp);
  display.setTextSize(2, 3);
  
  // Get the size of the text
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(tempString, 0, 0, &x1, &y1, &w, &h);
  
  display.setCursor(SCREEN_WIDTH - w, SCREEN_HEIGHT - h);
  display.print(tempString);
  display.display();
}

void drawFreezer(int temp) {
  drawTemp("Freezer", freezerRunning, temp);
}

void drawRefrigerator(int temp) {
  drawTemp("Fridge", refrigeratorRunning, temp);
}

void resetButtons() {
  leftLastDebounceTime = 0;
  rightLastDebounceTime = 0;
  wasLeftTouched = false;
  wasRightTouched = false;
}

void toggleView() {
  lastViewToggle = millis();
  showFreezer = !showFreezer;
}

void processButtons() {
  bool isLeftTouched = digitalRead(LEFT_TOUCH_PIN) == HIGH;
  unsigned long now = millis();

  // If the button is pressed, track when it was last pressed
  if (isLeftTouched && leftLastDebounceTime == 0) {
    leftLastDebounceTime = now;
  }
  
  bool isRightTouched = digitalRead(RIGHT_TOUCH_PIN) == HIGH;

  // If the button is pressed, track when it was last pressed
  if (isRightTouched && rightLastDebounceTime == 0) {
    rightLastDebounceTime = now;
  }

  bool bothTouched = isLeftTouched && isRightTouched;

  // Abort if both were touched and one now not touched
  if (wasLeftTouched && wasRightTouched && !bothTouched) {
    resetButtons();
    return;
  }

  // Abort if nothing touched
  if (!isLeftTouched && !isRightTouched)
  {
    resetButtons();
    return;
  }

  // Abort if state doesn't change
  if (isLeftTouched == wasLeftTouched && isRightTouched == wasRightTouched) {
    return;
  }

  // Abort if the button was not pressed for at least 1s
  if ((isLeftTouched && now - leftLastDebounceTime < DEBOUNCE) || 
      (isRightTouched && now - rightLastDebounceTime < DEBOUNCE))
  {
    return;
  }

  // Track for next time
  lastViewToggle = now;
  wasLeftTouched = isLeftTouched;
  wasRightTouched = isRightTouched;
  
  if (bothTouched)
  {
      editTemp = !editTemp;
  }
  else if (editTemp)
  { 
    if (isLeftTouched) {
      if (showFreezer) {
        freezerTemp--;
      } else {
        fridgeTemp--;
      }
    } else if (isRightTouched) {
      if (showFreezer) {
        freezerTemp++;
      } else {
        fridgeTemp++;
      }
    } else {
      editTemp = false;
    }
  }
  else
  {
    if (isLeftTouched || isRightTouched) {
      toggleView();
    }
  }
}

void loop() {
  unsigned long now = millis();
  processButtons();

  // Toggle view between freezer and refrigerator if no buttons pressed in 5s
  if (!editTemp && now - lastViewToggle > VIEW_TOGGLE) {
    toggleView();
  }

  if (showFreezer) {
    drawFreezer(freezerTemp);
  } else {
    drawRefrigerator(fridgeTemp);
  }
  
  delay(100);  
}