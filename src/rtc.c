#include "rtc.h"

/*
 * Ban sua cho SN32F407_EVK:
 * Khong cho chuong trinh doi thach anh RTC ngoai 32.768 kHz nua.
 * Neu board/setting khong co ELS, lenh while doi CSST se lam project treo truoc khi hien thi.
 * Ta tao tick 1 giay tu SysTick 1 ms giong ban fix dang chay duoc.
 */
static volatile unsigned char s_rtc_second_flag = 0u;
static unsigned short s_1s_divider = 0u;

void RTC_Init_1Hz_ELS(void)
{
    s_rtc_second_flag = 0u;
    s_1s_divider = 0u;
}

void RTC_Tick1ms(void)
{
    s_1s_divider++;
    if (s_1s_divider >= 1000u) {
        s_1s_divider = 0u;
        s_rtc_second_flag = 1u;
    }
}

bool RTC_ConsumeSecondFlag(void)
{
    if (s_rtc_second_flag) {
        s_rtc_second_flag = 0u;
        return true;
    }

    return false;
}

/* Giu symbol cu de startup/linker khong bi anh huong neu co tham chieu. */
void RTC_IRQHandler(void)
{
}

void RTCIRQ_Handler(void)
{
}