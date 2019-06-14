/*---------------------------------------------------------------------
 * vector.h ISR handlers
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* typedef for vector table */
typedef void (* const vector)(void); /* const pointer to function(void) return void */

/* special inline instruction for when you run VTOR from RAM */
#define BootRAM 0xF108F85F

/* addresses from stm32f303cc.ld */
extern unsigned _estack;   

/* make sure these all have C signatures */
extern void __libc_init_array(void);
extern void Reset_Handler(void) __attribute__((used, naked));
extern void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

/* vim: set ts=2 sw=2 expandtab: */
