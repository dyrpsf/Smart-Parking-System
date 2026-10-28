#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

// --- PIN DEFINITIONS ---
const int trigPin = 6;
const int echoPin = 7;
const int distanceThreshold = 10; 

const int irPins[3] = {3, 4, 5}; 
const int ledPins[4] = {8, 9, 10, 11};

// Timing and logic variables
unsigned long lastDisplaySwitch = 0;
const long displayInterval = 1800; 
int currentSlotDisplay = 0;

// Glitch filter variable for the ultrasonic sensor
int sonarEmptyCounter = 0; 
bool slotAvailable[4];

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  for (int i = 0; i < 3; i++) {
    pinMode(irPins[i], INPUT);
  }
  
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
  
  u8g2.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  int availableCount = 0;

  // --- 1. READ SLOT 1 (ULTRASONIC) WITH GLITCH FILTER ---
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); 
  int distance = duration * 0.034 / 2;
  
  if (distance == 0 || distance > distanceThreshold) {
    sonarEmptyCounter++; 
    
    if (sonarEmptyCounter >= 5) {
      slotAvailable[0] = true;
      sonarEmptyCounter = 5; 
    } else {
      slotAvailable[0] = false; 
    }
  } else {
    sonarEmptyCounter = 0; 
    slotAvailable[0] = false;
  }

  if (slotAvailable[0]) availableCount++;

  // --- 2. READ SLOTS 2, 3, 4 (IR) ---
  for (int i = 0; i < 3; i++) {
    if (digitalRead(irPins[i]) == HIGH) { 
      slotAvailable[i + 1] = true; 
      availableCount++;
    } else {
      slotAvailable[i + 1] = false;
    }
  }

  // --- 3. SOLID LEDS FOR FREE SLOTS ---
  for (int i = 0; i < 4; i++) {
    if (slotAvailable[i]) {
      digitalWrite(ledPins[i], HIGH); // LED stays solid ON when empty
    } else {
      digitalWrite(ledPins[i], LOW);  // LED turns OFF when occupied
    }
  }

  // --- 4. ROTATE DISPLAY ---
  if (currentMillis - lastDisplaySwitch >= displayInterval) {
    lastDisplaySwitch = currentMillis;
    currentSlotDisplay = (currentSlotDisplay + 1) % 4;
  }

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

  // --- 5. DRAW TO OLED SCREEN ---
  u8g2.firstPage();
  do {
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

      u8g2.setFont(u8g2_font_fur14_tf);
      char slotBuffer[12];
      sprintf(slotBuffer, "SLOT %d", slotToShow + 1);
      u8g2.drawStr(30, 47, slotBuffer);

      u8g2.setFont(u8g2_font_5x7_tf);
      char countBuffer[20];
      sprintf(countBuffer, "Total Free: %d / 4", availableCount);
      u8g2.drawStr(28, 60, countBuffer);
    }
  } while (u8g2.nextPage());

  delay(30); 
}
