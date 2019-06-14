#pragma once

#include "stm32f3xx.h"
#include "api/ArduinoAPI.h"

__always_inline GPIO_TypeDef & PinToPort(const pin_size_t pinNumber) {
  if ( pinNumber < 16)        /* PA0..PA15 */
    return *GPIOA;
  else if ( pinNumber < 32)   /* PB0..PB15 */
    return *GPIOB;
  else if ( pinNumber < 48 )  /* PC0..PC15 */
    return *GPIOC;
  else
    return *GPIOF;            /* PF0..PF1 */
}

__always_inline uint32_t PinToMask(const pin_size_t pinNumber, const int width=1) { 
  return ((1U<<width)-1) << ((pinNumber % 16)*width);
}
