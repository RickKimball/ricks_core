#pragma once

#include "wiring_digital.h"

namespace wiringEx {

  typedef enum {
    INPUT               = 0x0,
    OUTPUT              = 0x1,  /* push pull output */
    INPUT_PULLUP        = 0x2,
    INPUT_PULLDOWN      = 0x3,
    INPUT_ANALOG        = 0x4,
    OUTPUT_OPEN_DRAIN   = 0x5,
    ALT_FUNC            = 0x6,  /* push pull output */
    ALT_FUNC_OPEN_DRAIN = 0x7,
    OUTPUT_MCO          = 0x8
  } PinModeEx;

  __always_inline void pinModeEx(const pin_size_t pinNumber, PinModeEx pinMode,
                 const uint32_t speed=0b11, const uint32_t alt_id=0b0000)
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
    case OUTPUT_OPEN_DRAIN:
    case ALT_FUNC:
    case OUTPUT_MCO:
      MODIFY_REG(GPIOx.OSPEEDR, PinToMask(pinNumber,2), (speed << moder_offset));

      if ( pinMode == OUTPUT_OPEN_DRAIN  ) {
        MODIFY_REG(GPIOx.MODER, PinToMask(pinNumber,2), (0b01U << moder_offset));
        SET_BIT(GPIOx.OTYPER, PinToMask(pinNumber,1));
      }
      else {
        CLEAR_BIT(GPIOx.OTYPER, PinToMask(pinNumber,1));
        MODIFY_REG(GPIOx.MODER, PinToMask(pinNumber,2), (0b01U << moder_offset));
      }

      if ( (pinMode == ALT_FUNC || pinMode == OUTPUT_MCO) ) {
        const uint32_t afr_offset = (pinNumber % 8)*4;

        MODIFY_REG(GPIOx.MODER, PinToMask(pinNumber,2), (0b10U << moder_offset));
        MODIFY_REG( ((pinNumber % 16) < 8 ? GPIOx.AFR[0] : GPIOx.AFR[1]),
                      PinToMask((pinNumber % 8),4), (alt_id << afr_offset) );

        if ( pinMode == OUTPUT_MCO) {
          #if F_CPU > 36000000
            MODIFY_REG(RCC->CFGR,RCC_CFGR_MCO,RCC_CFGR_MCO_PLL);
          #else
            MODIFY_REG(RCC->CFGR,RCC_CFGR_MCO,RCC_CFGR_MCO_SYSCLK);
          #endif
        }
      }
      break;

    default:
      while(1); // TBD
    }
}
 
  __always_inline void digitalWriteEx(const pin_size_t pinNumber, PinStatus status) {
    if ( status )
      WRITE_REG(PinToPort(pinNumber).BSRR, PinToMask(pinNumber));
    else
      WRITE_REG(PinToPort(pinNumber).BRR, PinToMask(pinNumber));
  }

  __always_inline PinStatus digitalReadEx(const pin_size_t pinNumber) {
    return ( READ_REG(PinToPort(pinNumber)).IDR & PinToMask(pinNumber) ) ? HIGH : LOW;
  }

} /* end namespace */

/* vim: set ts=2 sw=2 expandtab : */
