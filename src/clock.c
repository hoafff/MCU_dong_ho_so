#include "clock.h"
#include "sn32f407_regs.h"

void Clock_Init_48MHz_IHRC(void)
{
    /*
     * Ban fix cua thay/project mau chay on dinh voi IHRC 12 MHz mac dinh.
     * Giu ten ham cu de khong phai doi cac file khac, nhung cau hinh ve 12 MHz
     * de SysTick, KeyScan va PWM buzzer khop voi thu vien sample SONiX.
     */
    SN_FLASH->LPCTRL = 0x5AFA0000UL;          /* HCLK < 24 MHz */

    SN_SYS0->ANBCTRL_b.IHRCFREQ = 0u;         /* IHRC = 12 MHz */
    SN_SYS0->AHBCP = 0u;                      /* HCLK = SYSCLK / 1 */
    SN_SYS0->CLKCFG = 0x0u;                   /* SYSCLK = IHRC */

    while ((SN_SYS0->CLKCFG & 0x70u) != 0x0u) {
        ;
    }
}