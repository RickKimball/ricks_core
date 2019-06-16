/*
  Blink the blue LED on the RobotDyn stm32f303cc board
  Based on the Arduino Blink without Delay example
  This example code is in the public domain

  The blue LED on the RobotDyn stm32f303cc is connected to PC13

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/
#include "Arduino.h"

PinStatus ledState = LOW;

unsigned long previousMillis = 0;
const long interval = 1000;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (ledState == LOW) {
      ledState = HIGH;  // Note that this switches the LED *off*
    } else {
      ledState = LOW;  // Note that this switches the LED *on*
    }
    digitalWrite(LED_BUILTIN, ledState);
  }
}
