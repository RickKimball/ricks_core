/*
   Blink_ws281x_leds_wDMA.ino - DMA/TIMER based ws281x/neopixel pixel routines

   Generate a ws281x compatible pulse train using PWM timer and DMA
   provided duty cycle data.  Connect pin PA1 to the DIN of a ws281x
   led strip and watch as Blue Green Red White pixels march.

   This code written for a robotodyn stm32f303cc blackpill uses the
   wiringEx core for arduino.

   Author: rick kimball

   Log: 
   2019-06-14  use new robotdyn_f303 core
   2018-07-11  tweaks to reduce size and style
   2018-07-10: converted to wiringEx core and register only calls 
               using stm32f103xb.h from stm32cube headers
   2018-07-07: simplified using RGB values, compute T0/T1 based on F_CPU
   2018-07-05: Initial version
*/
#include "Arduino.h"
#include "wiringex.h"

#define sizeofs(a) sizeof(a)/sizeof(a[0])

typedef uint16_t pulse_t;                            // pulse times are less than 65535
static const uint32_t HZ=36000;
static const uint16_t T_PRE=(F_CPU/HZ/65535)+1;     
static const uint16_t PERIOD=(F_CPU/T_PRE/HZ);
static const uint16_t PERIOD_DIV2=(pulse_t)((float)PERIOD*.50f); //
static const uint16_t PERIOD_DIV4=(pulse_t)((float)PERIOD*.25f); //

//----------------------------------------------------------------------
// global variables

const pin_size_t ws281x_pin = PA1;

//----------------------------------------------------------------------
//

void setup() {
  // enable blackpill built-in led (PC13)
  wiringEx::pinModeEx(LED_BUILTIN,wiringEx::OUTPUT,0b00);
  // set PA1 to Push Pull Alternate IO TIM2_CH2 uses DMA1 CH7
  pinModeEx(ws281x_pin, wiringEx::ALT_FUNC, 0b11, 0b0001);
  pinModeEx(PA8, wiringEx::OUTPUT_MCO);

  //------------------------------------------------------------------
  // PA1 - TIM2 CH2 setup

  TIM2->CR1 = 0;              // reset/disable timer to make changes
  TIM2->CR1 = TIM_CR1_ARPE;   // preload ARR value

  TIM2->PSC = T_PRE-1;        // prescaler of 1
  TIM2->ARR = PERIOD-1;           // auto reload register period 
  TIM2->CCR2 = PERIOD_DIV2;            // capture compare channel 2 duty cycle of 1/2 TT 
  TIM2->CCMR1 |= 0
              | TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 // (0b110) CCR2 PWM mode 1 
              | TIM_CCMR1_OC2PE;                    // (0b1)   CCR2 preload enabled

  // output capture compare channel 2 enabled, pulse is active high
  TIM2->CCER |= TIM_CCER_CC2E;

  // turn on the timer
  SET_BIT(TIM2->CR1,TIM_CR1_CEN);
  SET_BIT(TIM2->EGR,TIM_EGR_UG);
}

void loop() {
  static int state;
  if ( state ) {
    wiringEx::digitalWriteEx(LED_BUILTIN,LOW);
    TIM2->CCR2 = 0;
  }
  else {
    wiringEx::digitalWriteEx(LED_BUILTIN,HIGH);
    TIM2->CCR2 = PERIOD_DIV2;
  }
  state ^= 1;
  delay(100);
}
