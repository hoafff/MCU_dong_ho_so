#include "buzzer_led.h"
#include "board.h"

static volatile uint16_t s_pip_ms = 0;
static volatile uint16_t s_alarm_ms = 0;
static volatile uint16_t s_alarm_phase_ms = 0;
static volatile bool s_alarm_buz_on = false;

#define BUZZER_HCLK_HZ       12000000UL
#define BUZZER_SHORT_RELOAD  ((uint16_t)(BUZZER_HCLK_HZ / 440UL))
#define BUZZER_ALARM_RELOAD  ((uint16_t)(BUZZER_HCLK_HZ / 587UL))

static bool s_buzzer_hw_ready = false;

static void Buzzer_HW_Init(void)
{
    if (s_buzzer_hw_ready) {
        return;
    }

    /* Enable CT16B0 clock va route PWM0 ra P3.0 nhu sample SONiX. */
    SN_SYS1->AHBCLKEN_b.CT16B0CLKEN = 1u;
    SN_PFPA->CT16B0_b.PWM0 = 1u;

    SN_CT16B0->MR9 = BUZZER_SHORT_RELOAD;
    SN_CT16B0->MR0 = 0u;
    SN_CT16B0->MCTRL = 1UL << 30;
    SN_CT16B0->PWMCTRL = (1UL << 0) | (1UL << 4) | (1UL << 20);

    SN_CT16B0->TMRCTRL = 1UL << 1;
    while (SN_CT16B0->TMRCTRL & (1UL << 1)) {
        ;
    }
    SN_CT16B0->TMRCTRL = 1u;

    s_buzzer_hw_ready = true;
}

static void Buzzer_SetReload(uint16_t reload)
{
    Buzzer_HW_Init();

    if (reload == 0u) {
        SN_CT16B0->MR0 = 0u;
        return;
    }

    SN_CT16B0->MR9 = reload;
    SN_CT16B0->MR0 = reload >> 1;
    SN_CT16B0->TMRCTRL = 0u;
    SN_CT16B0->TMRCTRL = 1u;
}

void Buzzer_Set(bool on)
{
    Buzzer_SetReload(on ? BUZZER_SHORT_RELOAD : 0u);
}

void LedD6_Set(bool on)
{
    gpio_write(PIN_LED_D6, !on);
}

void Buzzer_Pip300ms(void)
{
    if (s_alarm_ms == 0u) {
        s_pip_ms = 300u;
    }
}

void Buzzer_AlarmStart5s(void)
{
    s_alarm_ms = 5000u;
    s_alarm_phase_ms = 500u;
    s_alarm_buz_on = true;
}

void Buzzer_Task1ms(void)
{
    if (s_alarm_ms > 0u) {
        Buzzer_SetReload(s_alarm_buz_on ? BUZZER_ALARM_RELOAD : 0u);
        s_alarm_ms--;

        if (s_alarm_phase_ms > 0u) {
            s_alarm_phase_ms--;
        }

        if (s_alarm_phase_ms == 0u) {
            s_alarm_buz_on = !s_alarm_buz_on;
            s_alarm_phase_ms = 500u;
        }

        if (s_alarm_ms == 0u) {
            Buzzer_Set(false);
            s_alarm_buz_on = false;
        }

        return;
    }

    if (s_pip_ms > 0u) {
        Buzzer_Set(true);
        s_pip_ms--;

        if (s_pip_ms == 0u) {
            Buzzer_Set(false);
        }
    } else {
        Buzzer_Set(false);
    }
}

void Buzzer_AlarmStop(void)
{
    s_alarm_ms = 0u;
    s_alarm_phase_ms = 0u;
    s_alarm_buz_on = false;
    Buzzer_Set(false);
}

bool Buzzer_IsAlarmRinging(void)
{
    return s_alarm_ms > 0u;
}