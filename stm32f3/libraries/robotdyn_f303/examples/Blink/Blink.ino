/*
 * Blink - robotdyn_f303 blink
 */
#include "Arduino.h"
#include "wiringex.h"

using namespace wiringEx;

#define GPIO_LOW_SPEED 0b00

void setup(void) {
  // builtin led is active low, it is connected to VCC
  // PC13/PC14/PC15 have speed restrictions, only use with low speed gpio
  // max current must not exceed 3mA for source or sink
  pinModeEx(LED_BUILTIN, OUTPUT_OPEN_DRAIN, GPIO_LOW_SPEED);
  digitalWriteEx(LED_BUILTIN, LOW);
}

void loop() {
  digitalWriteEx(LED_BUILTIN,LOW); // use fast inline GPIO
  delay(50);
  digitalWriteEx(LED_BUILTIN,HIGH);
  delay(950);
}
