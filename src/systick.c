#include "systick.h"
#include "display7seg.h"
#include "buzzer_led.h"
#include "rtc.h"

volatile uint32_t g_ms = 0;
volatile uint8_t g_flag_10ms = 0;

void SysTick_Init1ms(void)
{
    SysTick_Config(SYSCLK_HZ / 1000u);
}

void SysTick_Handler(void)
{
    g_ms++;

    Display_Scan1ms();
    Buzzer_Task1ms();
    RTC_Tick1ms();

    if ((g_ms % 10u) == 0u) {
        g_flag_10ms = 1u;
    }
}

void DelayMs(uint32_t ms)
{
    uint32_t start;

    start = g_ms;

    while ((uint32_t)(g_ms - start) < ms) {
        ;
    }
}