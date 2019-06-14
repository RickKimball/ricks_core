#include "wiring_digital.h"

void pinMode(pin_size_t pinNumber, PinMode pinMode)
{
  const uint32_t moder_offset = (pinNumber % 16)*2;
  GPIO_TypeDef & GPIOx = PinToPort(pinNumber);

  switch(pinMode) {
  case INPUT:
  case INPUT_PULLUP:
  case INPUT_PULLDOWN:
    CLEAR_BIT(GPIOx.MODER, PinToMask(pinNumber,2));
    break;

  case OUTPUT:
    CLEAR_BIT(GPIOx.OTYPER, PinToMask(pinNumber,1));
    MODIFY_REG(GPIOx.MODER, PinToMask(pinNumber,2), (0b01U << moder_offset));
    break;

  default:
    while(1); // TBD
  }
}

void digitalWrite(pin_size_t pinNumber, PinStatus status)
{
    const uint32_t pin_number = pinNumber % 16;

    if ( status )
        WRITE_REG(PinToPort(pinNumber).BSRR, (1 << pin_number));
    else
        WRITE_REG(PinToPort(pinNumber).BRR, (1 << pin_number));
}

PinStatus digitalRead(pin_size_t pinNumber)
{
    const uint32_t pin_number = pinNumber % 16;

    return READ_BIT(PinToPort(pinNumber).IDR, (1 << pin_number) ) ? HIGH : LOW;
}

/* vim: set ts=2 sw=2 expandtab: */
