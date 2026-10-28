#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// Initialize 0.96" OLED (SSD1306 128x64 I2C)
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Pin Definitions
const int sensorPins[4] = {2, 3, 4, 5};    // Slot 1 to 4 IR Sensors
const int ledPins[4]    = {8, 9, 10, 11};  // Slot 1 to 4 LEDs

// Timing variables for LED blinking
unsigned long previousMillis = 0;
const long blinkInterval = 350; // Blink rate in ms
bool ledState = LOW;

// Timing variables for cycling slot numbers
unsigned long lastDisplaySwitch = 0;
const long displayInterval = 1800; // Switch display every 1.8 sec
int currentSlotDisplay = 0;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(sensorPins[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  u8g2.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  // 1. Read Sensors (HIGH = Empty, LOW = Car detected)
  bool slotAvailable[4];
  int availableCount = 0;

  for (int i = 0; i < 4; i++) {
    if (digitalRead(sensorPins[i]) == HIGH) {
      slotAvailable[i] = true;
      availableCount++;
    } else {
      slotAvailable[i] = false;
    }
  }

  // 2. Blink LEDs for free slots, turn OFF for occupied slots
  if (currentMillis - previousMillis >= blinkInterval) {
    previousMillis = currentMillis;
    ledState = !ledState;

    for (int i = 0; i < 4; i++) {
      if (slotAvailable[i]) {
        digitalWrite(ledPins[i], ledState);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }

  // 3. Rotate through empty slots for the display
  if (currentMillis - lastDisplaySwitch >= displayInterval) {
    lastDisplaySwitch = currentMillis;
    currentSlotDisplay = (currentSlotDisplay + 1) % 4;
  }

  // 4. Find which slot to highlight
  int slotToShow = -1;
  if (availableCount > 0) {
    for (int offset = 0; offset < 4; offset++) {
      int checkIndex = (currentSlotDisplay + offset) % 4;
      if (slotAvailable[checkIndex]) {
        slotToShow = checkIndex;
        break;
      }
    }
  }

  // 5. Draw to OLED Screen
  u8g2.firstPage();
  do {
    // Header
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(22, 10, "GATE DISPLAY");
    u8g2.drawHLine(0, 13, 128);

    if (availableCount == 0) {
      u8g2.setFont(u8g2_font_fur14_tf);
      u8g2.drawStr(25, 36, "PARKING");
      u8g2.drawStr(40, 56, "FULL");
    } else {
      u8g2.setFont(u8g2_font_7x14_tf);
      u8g2.drawStr(20, 28, "AVAILABLE:");

      // Big Slot Number
      u8g2.setFont(u8g2_font_fur14_tf);
      char slotBuffer[12];
      sprintf(slotBuffer, "SLOT %d", slotToShow + 1);
      u8g2.drawStr(30, 47, slotBuffer);

      // Total Available
      u8g2.setFont(u8g2_font_5x7_tf);
      char countBuffer[20];
      sprintf(countBuffer, "Total Free: %d / 4", availableCount);
      u8g2.drawStr(28, 60, countBuffer);
    }
  } while (u8g2.nextPage());
}