#include "api/ArduinoAPI.h"
#include "stm32f3xx.h"

static const uint32_t us_cycles = F_CPU/1000000U;

volatile unsigned tickcnt;

static inline uint32_t tick_count() {
  return DWT->CYCCNT;
}

uint32_t micros(void) {
  return tick_count() / us_cycles;
}

uint32_t millis() {
    return tickcnt;
}

void delay(uint32_t msecs) {
  if (msecs) {
    const uint32_t t0 = millis();

    while ((millis() - t0) < msecs) {
      yield();
    }

  }
}

/**
 * @brief Delay the given number of microseconds.
 *
 * @param us Number of microseconds to delay.
 */

static void delay_us(uint32_t usecs) {
  static const uint32_t func_overhead = 40; // only really valid for 72MHz
  const uint32_t cycle_start = tick_count();
  const uint32_t cycle_end = ((usecs) * us_cycles) - func_overhead;

  __asm__ __volatile(" .align 2"); // place loop on a 4 byte boundary
                                   // to help flash prefetch
  while ((tick_count() - cycle_start) < cycle_end)
    ;

}

void delayMicroseconds(unsigned int us) {
  delay_us(us);
}

/*---------------------------------------------------------------------
  SysTick_Handler() - SYSTICK exception handler.

    Increment a 1 msec tick counter each time we get an interrupt
    from the SYSTICK.
  */
void SysTick_Handler(void) __attribute__((section(".ccmram")));

void SysTick_Handler(void) {
  ++tickcnt;
}

void yield() {
  // user can provide their own typically used with ArduinoScheduler
}

/* vim: set ts=2 sw=2 expandtab : */
