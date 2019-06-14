/*---------------------------------------------------------------------
  main.cpp - example09 - using float point and math functions newlib
*/
#include "api/ArduinoAPI.h"
#include "main.h"
#include <stdlib.h>

//volatile unsigned tickcnt;

/*---------------------------------------------------------------------
  Reset_Handler() - the reset exception handler

  Sets stack pointer and inits .bss and .data, optionally "color" memory,
  call global constructors, and finally calls main.
  */
void Reset_Handler(void) {
  /* set stack pointer to top of stack memory (Not Neccessary) */
  //asm volatile ("ldr r0, =%0" :: "i" (&_estack));
  //asm volatile ("mov sp, r0");

  /* setup SYSCLK early */
  sysclock_set<F_CPU>();

#if VECT_ADDR > 0x08000000
  SCB->VTOR = VECT_ADDR;
#endif

  /* Copy the .data segment initializers from flash to SRAM */
  __asm__ volatile(
  "data_init:\n"
  " ldr   r0,=_sidata\n" /* start address of initialized data in flash */
  " ldr   r1,=_sdata\n" /* start address of data in SRAM */
  " ldr   r2,=_edata\n" /* end address of data in SRAM */
  "1:\n"
  " cmp   r1,r2\n"
  " ittt  lo\n"         /* exec following 3 instructions if r1 < r2 */
  " ldrlo r3,[r0],#4\n" /* post increment r0 after ldr */
  " strlo r3,[r1],#4\n" /* post increment r1 after str */
  " blo.n 1b\n"         /* try again */
  );

  /* Copy the .ccmram segment initializers from flash to CCM SRAM */
  __asm__ volatile(
  "ccm_init:\n"
  " ldr   r0,=_siccmram\n" /* start address of ISR code in flash */
  " ldr   r1,=_sccmram\n"  /* start address of data in SRAM */
  " ldr   r2,=_eccmram\n"  /* end address of data in SRAM */
  "1:\n"
  " cmp   r1,r2\n"
  " ittt  lo\n"         /* exec following 3 instructions if r1 < r2 */
  " ldrlo r3,[r0],#4\n" /* post increment r0 after ldr */
  " strlo r3,[r1],#4\n" /* post increment r1 after str */
  " blo.n 1b\n"         /* try again */
  );

  /* Zero fill the .bss segment, assumes aligned on 4 bytes */
  __asm__ volatile(
  "bss_init:\n"
  " ldr   r0, =_sbss\n" /* start address of non initialized data in SRAM */
  " ldr   r1, =_ebss\n" /* end address of non initialized data in SRAM */
  " movs  r2, #0\n"     /* word sized zero constant */
  "2:\n"
  " cmp   r0,r1\n"
  " itt   ne\n"         /* exec following 2 instructions if r0 < r1 */
  " strne r2,[r0],#4\n" /* post increment r0 after storing zero */
  " bne.n 2b\n"         /* try again */
  );

  /* _init() board, then call any global c/c++ constructors from newlib */
  __libc_init_array();

  main();

  while(1); /* trap if main exits */
}

/*---------------------------------------------------------------------
  _init() - initialize board

  NOTE: called from __libc_init() before main(). It must be a "C" routine.
  */
void _init(void) {
  RCC->AHBENR  |= 0
               | RCC_AHBENR_GPIOAEN             /* GPIO A-C,E */
               | RCC_AHBENR_GPIOBEN
               | RCC_AHBENR_GPIOCEN
               | RCC_AHBENR_GPIOEEN
               ;

  RCC->APB2ENR |= 0
               | RCC_APB2ENR_USART1EN           /* USART1         */
               ;

  RCC->APB1ENR |= 0
               | RCC_APB1ENR_PWREN
               ;

#if 0
  /*---------------------------------------------------------------------- */
  // PA8 configured as Master Clock Out (0b10 ALT FUNC PP | 0b11 50MHz max output)
  GPIOA->CRH = (GPIOC->CRH & ~(0b1111 << ((8-8)*4))) | (0b1011 << ((8-8)*4));
  MODIFY_REG(RCC->CFGR, RCC_CFGR_MCOSEL,
    ((F_CPU > 36000000) ? RCC_CFGR_MCOSEL_PLL_DIV2 : RCC_CFGR_MCOSEL_SYSCLK));

  /*---------------------------------------------------------------------- */
  //  PC13 set to opendrain max 2MHz and set it high
  MODIFY_REG(GPIOC->CRH,(0b1111 << ((13-8)*4)),(0b0110 << ((13-8)*4)));
  GPIOC->BSRR = 1 << 13;

  /*---------------------------------------------------------------------- */
  //  PA9/PA10 configured as USART1
  SET_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST);
  CLEAR_BIT(RCC->APB2RSTR, RCC_APB2RSTR_USART1RST);
  SET_BIT(RCC->APB2ENR,RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN);

  GPIOA->CRH = (GPIOA->CRH & ~(0b11111111 << (9-8)*4))
                                    | (0b01001011 << (9-8)*4);  // PA10-RX1, PA9-TX1
  USART1->BRR = ((F_CPU) / BAUD) / APB2_DIV;                    // BUS SPEED is MCLK/1
  USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;     // enable TX, RX, USART

  /*---------------------------------------------------------------------- */
  // enable SWD debugging
  MODIFY_REG(AFIO->MAPR, AFIO_MAPR_SWJ_CFG, AFIO_MAPR_SWJ_CFG_JTAGDISABLE);
#endif

  // enable DWT to provide tick_count()
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = -100L;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

  /*---------------------------------------------------------------------- */
  // enable msec Systick ISR
  tickcnt=-500L; // test tickcnt wraparound, start @ .5 seconds before rollover
  SysTick_Config(F_CPU/1000U);
}

/*---------------------------------------------------------------------
  int main() -
  */
int main(void) {
  setup();

  while(1) {
    loop();
    // if (serialEventRun) serialEventRun();
  }

  return 0;
}

/* vim: set expandtab ts=2 sw=2: */
