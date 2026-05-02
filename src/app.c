#include "app.h"
#include "sn32f407_regs.h"
#include "display7seg.h"
#include "buzzer_led.h"
#include "keypad.h"
#include "eeprom.h"

typedef struct {
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
} ClockTime;

typedef enum {
    MODE_NORMAL = 0,
    MODE_SET_HOUR,
    MODE_SET_MINUTE,
    MODE_ALARM_HOUR,
    MODE_ALARM_MINUTE
} AppMode;

static ClockTime s_now = {0u, 0u, 0u};
static ClockTime s_alarm = {0u, 0u, 0u};

static AppMode s_mode = MODE_NORMAL;
static uint32_t s_idle_ms = 0u;
static bool s_alarm_dirty = false;

static bool s_blink_on = true;

static bool InEditMode(void)
{
    return s_mode != MODE_NORMAL;
}

static bool InAlarmEditMode(void)
{
    return (s_mode == MODE_ALARM_HOUR) || (s_mode == MODE_ALARM_MINUTE);
}

static void App_LoadAlarm(void)
{
    uint8_t magic;
    uint8_t h;
    uint8_t m;
    uint8_t chk;

    if (EEPROM_ReadByte(0u, &magic) &&
        EEPROM_ReadByte(1u, &h) &&
        EEPROM_ReadByte(2u, &m) &&
        EEPROM_ReadByte(3u, &chk) &&
        magic == EEPROM_MAGIC &&
        h < 24u &&
        m < 60u &&
        chk == (uint8_t)(EEPROM_MAGIC ^ h ^ m)) {

        s_alarm.hour = h;
        s_alarm.minute = m;
        s_alarm.second = 0u;
    } else {
        s_alarm.hour = 0u;
        s_alarm.minute = 0u;
        s_alarm.second = 0u;
    }
}

static void App_SaveAlarm(void)
{
    uint8_t h;
    uint8_t m;
    uint8_t chk;

    h = s_alarm.hour;
    m = s_alarm.minute;
    chk = (uint8_t)(EEPROM_MAGIC ^ h ^ m);

    EEPROM_WriteByte(0u, EEPROM_MAGIC);
    EEPROM_WriteByte(1u, h);
    EEPROM_WriteByte(2u, m);
    EEPROM_WriteByte(3u, chk);

    s_alarm_dirty = false;
}

static void ExitToNormal(void)
{
    if (s_alarm_dirty) {
        App_SaveAlarm();
    }

    s_mode = MODE_NORMAL;
    s_idle_ms = 0u;
    LedD6_Set(false);
}

static void Clock_Increment1Second(void)
{
    s_now.second++;

    if (s_now.second >= 60u) {
        s_now.second = 0u;
        s_now.minute++;

        if (s_now.minute >= 60u) {
            s_now.minute = 0u;
            s_now.hour++;

            if (s_now.hour >= 24u) {
                s_now.hour = 0u;
            }
        }

        if (s_now.hour == s_alarm.hour &&
            s_now.minute == s_alarm.minute &&
            s_now.second == 0u) {

            Buzzer_AlarmStart5s();
        }
    }
}

static void AdjustHour(ClockTime *t, int8_t delta)
{
    if (delta > 0) {
        t->hour = (uint8_t)((t->hour + 1u) % 24u);
    } else {
        t->hour = (t->hour == 0u) ? 23u : (uint8_t)(t->hour - 1u);
    }
}

static void AdjustMinute(ClockTime *t, int8_t delta)
{
    if (delta > 0) {
        t->minute = (uint8_t)((t->minute + 1u) % 60u);
    } else {
        t->minute = (t->minute == 0u) ? 59u : (uint8_t)(t->minute - 1u);
    }
}

static void App_UpdateDisplay(void)
{
    uint8_t blink_mask;
    ClockTime show;

    blink_mask = 0u;
    show = s_now;

    switch (s_mode) {
    case MODE_SET_HOUR:
        blink_mask = BIT(0) | BIT(1);
        show = s_now;
        break;

    case MODE_SET_MINUTE:
        blink_mask = BIT(2) | BIT(3);
        show = s_now;
        break;

    case MODE_ALARM_HOUR:
        blink_mask = BIT(0) | BIT(1);
        show = s_alarm;
        break;

    case MODE_ALARM_MINUTE:
        blink_mask = BIT(2) | BIT(3);
        show = s_alarm;
        break;

    case MODE_NORMAL:
    default:
        blink_mask = 0u;
        show = s_now;
        break;
    }

    Display_ShowTime(show.hour, show.minute, blink_mask);
}

void App_Init(void)
{
    App_LoadAlarm();

    s_now.hour = 0u;
    s_now.minute = 0u;
    s_now.second = 0u;

    s_mode = MODE_NORMAL;
    s_idle_ms = 0u;
    s_alarm_dirty = false;
    s_blink_on = true;

    Display_SetBlinkOn(true);
    LedD6_Set(false);

    App_UpdateDisplay();
}

void App_HandleKeys(uint8_t keys)
{
    if (keys == 0u) {
        return;
    }
		/* N?u còi alarm dang kêu, nh?n b?t k? nút nào thì t?t còi tru?c */
    if (Buzzer_IsAlarmRinging()) {
        Buzzer_AlarmStop();
    }

    Buzzer_Pip300ms();

    if (InEditMode()) {
        s_idle_ms = 0u;
    }

    if (keys & KEY_SW3) {
        if (s_mode == MODE_NORMAL) {
            s_mode = MODE_SET_HOUR;
            s_idle_ms = 0u;
        } else if (s_mode == MODE_SET_HOUR) {
            s_mode = MODE_SET_MINUTE;
            s_idle_ms = 0u;
        } else if (s_mode == MODE_SET_MINUTE) {
            ExitToNormal();
        }

        return;
    }

    if (keys & KEY_SW16) {
        if (s_mode == MODE_NORMAL) {
            s_mode = MODE_ALARM_HOUR;
            s_idle_ms = 0u;
        } else if (s_mode == MODE_ALARM_HOUR) {
            s_mode = MODE_ALARM_MINUTE;
            s_idle_ms = 0u;
        } else if (s_mode == MODE_ALARM_MINUTE) {
            s_alarm_dirty = true;   
            ExitToNormal();
        }

        return;
    }

    if (keys & KEY_SW6) {
        if (s_mode == MODE_SET_HOUR) {
            AdjustHour(&s_now, +1);
            s_now.second = 0u;
        } else if (s_mode == MODE_SET_MINUTE) {
            AdjustMinute(&s_now, +1);
            s_now.second = 0u;
        } else if (s_mode == MODE_ALARM_HOUR) {
            AdjustHour(&s_alarm, +1);
            s_alarm.second = 0u;
            s_alarm_dirty = true;
        } else if (s_mode == MODE_ALARM_MINUTE) {
            AdjustMinute(&s_alarm, +1);
            s_alarm.second = 0u;
            s_alarm_dirty = true;
        }
    }

    if (keys & KEY_SW10) {
        if (s_mode == MODE_SET_HOUR) {
            AdjustHour(&s_now, -1);
            s_now.second = 0u;
        } else if (s_mode == MODE_SET_MINUTE) {
            AdjustMinute(&s_now, -1);
            s_now.second = 0u;
        } else if (s_mode == MODE_ALARM_HOUR) {
            AdjustHour(&s_alarm, -1);
            s_alarm.second = 0u;
            s_alarm_dirty = true;
        } else if (s_mode == MODE_ALARM_MINUTE) {
            AdjustMinute(&s_alarm, -1);
            s_alarm.second = 0u;
            s_alarm_dirty = true;
        }
    }
}

void App_Task1ms(void)
{
    static uint16_t blink_ms = 0u;

    blink_ms++;

    if (blink_ms >= 500u) {
        blink_ms = 0u;
        s_blink_on = !s_blink_on;
        Display_SetBlinkOn(s_blink_on);
    }

    if (InAlarmEditMode()) {
        LedD6_Set(s_blink_on);
    } else {
        LedD6_Set(false);
    }

    if (InEditMode()) {
        s_idle_ms++;

        if (s_idle_ms >= 30000u) {
            ExitToNormal();
            Buzzer_Pip300ms();
        }
    }

    App_UpdateDisplay();
}

void App_OnRtcSecond(void)
{
    Clock_Increment1Second();
}