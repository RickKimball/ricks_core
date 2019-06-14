/*----------------------------------------------------------------------
  sysclock.h - configure system clock, flash wait states, and bus speeds
 */
#pragma once

#include <stm32f303xc.h>

template <const unsigned MCLK_FREQ>
static void sysclock_set()
{
  /* Enable HSE */
#ifdef USE_HSEBYP
  SET_BIT(RCC->CR, RCC_CR_HSEON | RCC_CR_HSEBYP);
#else
  SET_BIT(RCC->CR, RCC_CR_HSEON);
#endif

  /* Wait until HSE is ready */
  while (READ_BIT(RCC->CR, RCC_CR_HSERDY) == 0) {
    ;
  }

  CLEAR_BIT(FLASH->ACR, FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY);
#if F_CPU > 48000000U
  /* Enable Prefetch Buffer & set Flash access to 2 wait states */
  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_2);
#elif F_CPU > 24000000U
  /* Enable Prefetch Buffer & set Flash access to 1 wait states */
  SET_BIT(FLASH->ACR, FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY_1);
#else
  /* Enable Prefetch Buffer & set Flash access to 1 wait states */
  SET_BIT(FLASH->ACR, FLASH_ACR_LATENCY_0);
#endif

  SET_BIT(RCC->CFGR,( RCC_CFGR_HPRE_DIV1  /* SysClk/1 72 MHz */
                    | RCC_CFGR_PPRE2_DIV1 /* APB2/1 High Speed 72MHz */
#if F_CPU > 36000000L
                    | RCC_CFGR_PPRE1_DIV2 /* APB1/2 Low Speed  36MHz max */
#else
                    | RCC_CFGR_PPRE1_DIV1 /* APB1/1 Low Speed same as F_CPU */
#endif
                    | RCC_CFGR_PLLSRC
#if   0
#elif F_CPU == 80000000L /* overclock */
                    | RCC_CFGR_PLLMUL10
#elif F_CPU == 72000000L
                    | RCC_CFGR_PLLMUL9
#elif F_CPU == 64000000L
                    | RCC_CFGR_PLLMUL8
#elif F_CPU == 56000000L
                    | RCC_CFGR_PLLMUL7
#elif F_CPU == 48000000L
                    | RCC_CFGR_PLLMUL6
#elif F_CPU == 40000000L
                    | RCC_CFGR_PLLMUL5
#elif F_CPU == 32000000L
                    | RCC_CFGR_PLLMUL4
#elif F_CPU == 24000000L
                    | RCC_CFGR_PLLMUL3
#elif F_CPU == 16000000L
                    | RCC_CFGR_PLLMUL2
#endif
                    ) );

  /* Enable PLL */
  SET_BIT(RCC->CR, RCC_CR_PLLON);

  /* Wait until PLL is ready */
  while (READ_BIT(RCC->CR, RCC_CR_PLLRDY) == 0) {
    ;
  }

  /* Select PLL as system clock source */
  SET_BIT(RCC->CFGR, RCC_CFGR_SW_PLL);

  /* Wait until PLL is used as system clock source */
  while (READ_BIT(RCC->CFGR, RCC_CFGR_SWS_1) == 0) {
    ;
  }
  return;
}

// specialized 8MHz HSI setup
template<>
__always_inline
void sysclock_set<8000000>(void) {
  /* do nothing just run from the default HSI config */
  /* default is Prefetch enabled and 0 wait states */
  /* turn on half cycle flash */

  SET_BIT(RCC->CR, RCC_CR_HSION);

  /* Wait until HSE is ready */
  while (READ_BIT(RCC->CR, RCC_CR_HSIRDY) == 0) {
    ;
  }

  /* Select HSI as system clock source */
  CLEAR_BIT(RCC->CFGR, RCC_CFGR_SW);

  /* Wait until HSI is used as system clock source */
  while ( READ_BIT(RCC->CFGR, RCC_CFGR_SWS_1|RCC_CFGR_SWS_0)) {
    ;
  }

  return;
}

/* vim: set ts=2 sw=2 expandtab: */
