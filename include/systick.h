#ifndef SYSTICK_APP_H
#define SYSTICK_APP_H

#include <stdint.h>
#include "sn32f407_regs.h"

extern volatile uint32_t g_ms;
extern volatile uint8_t g_flag_10ms;

void SysTick_Init1ms(void);
void DelayMs(uint32_t ms);

#endif