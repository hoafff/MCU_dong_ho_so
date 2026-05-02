#include <stdint.h>

#include "clock.h"
#include "board.h"
#include "systick.h"
#include "i2c_hw.h"
#include "rtc.h"
#include "keypad.h"
#include "app.h"

int main(void)
{
    uint32_t last_ms;
    uint8_t key_event;

    last_ms = 0u;

    Clock_Init_48MHz_IHRC();

    Board_GPIO_Init();

    SysTick_Init1ms();

    /*
     * Ban sua: khong dung EEPROM I2C ngoai nua.
     * Bao thuc duoc luu bang Flash emulation trong eeprom.c,
     * nen tranh loi treo/khong doc duoc neu board khong gan EEPROM I2C.
     */

    RTC_Init_1Hz_ELS();

    App_Init();

    while (1) {
        if (g_ms != last_ms) {
            last_ms++;
            App_Task1ms();
        }

        if (g_flag_10ms) {
            g_flag_10ms = 0u;

            key_event = Keypad_Task10ms();
            App_HandleKeys(key_event);
        }

        if (RTC_ConsumeSecondFlag()) {
            App_OnRtcSecond();
        }
    }
}