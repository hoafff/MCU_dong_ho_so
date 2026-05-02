#ifndef SN32F407_REGS_H
#define SN32F407_REGS_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Header device cua SONiX.
 * File nay da dinh nghia:
 *   SN_GPIO0, SN_GPIO1, SN_GPIO2, SN_GPIO3
 *   SN_SYS0, SN_SYS1
 *   SN_RTC
 *   SN_I2C0
 *   SysTick_Config, NVIC_EnableIRQ, IRQn_Type, __NOP
 */
#include "SN32F400.h"

#define SYSCLK_HZ   12000000UL

#ifndef BIT
#define BIT(n)      (1UL << (n))
#endif

/*
 * PFPA_I2C0 register.
 * Neu header SONiX da co macro rieng cho PFPA_I2C0 thi co the dung macro cua hang.
 * Neu chua co, ta giu dia chi truc tiep nay.
 */
#define APP_PFPA_I2C0_REG (*(volatile uint32_t *)(0x40042000UL + 0x40UL))

#ifdef RTC_IRQn
#define APP_RTC_IRQN RTC_IRQn
#else
#define APP_RTC_IRQN ((IRQn_Type)23)
#endif

#endif