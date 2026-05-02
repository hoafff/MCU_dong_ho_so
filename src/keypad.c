#include "keypad.h"
#include "board.h"

/*
 * Mapping theo layout th?t trên board SN32F407_EVK mà b?n quan sát:
 *
 * SW2 d?ng riêng, không thu?c ma tr?n 4x4 dùng cho bài này.
 *
 * Ma tr?n 4x4:
 * Row 0: SW3   SW4   SW5   SW6
 * Row 1: SW7   SW8   SW9   SW10
 * Row 2: SW11  SW12  SW13  SW14
 * Row 3: SW16  SW15  SW17  SW18
 *
 * Bài yêu c?u ch? dùng:
 * SW3  = SETUP
 * SW16 = H?N GI?
 * SW6  = +
 * SW10 = -
 */
static const uint8_t KEY_MAP[4][4] = {
    { KEY_SW3,  0u,       0u,       KEY_SW6  },
    { 0u,       0u,       0u,       KEY_SW10 },
    { 0u,       0u,       0u,       0u       },
    { KEY_SW16, 0u,       0u,       0u       }
};

static void Row_ReleaseAll(void)
{
    uint8_t r;

    for (r = 0u; r < 4u; r++) {
        gpio_cfg(BOARD_ROW[r], GPIO_CFG_PULLUP);
        gpio_input(BOARD_ROW[r]);
    }
}

static void Row_DriveLow(uint8_t r)
{
    gpio_clr(BOARD_ROW[r]);
    gpio_output(BOARD_ROW[r]);
}

static void SmallDelay(void)
{
    volatile uint32_t i;

    for (i = 0u; i < 120u; i++) {
        __NOP();
    }
}

static uint8_t Keypad_ReadRaw(void)
{
    uint8_t r;
    uint8_t c;
    uint8_t raw;

    raw = 0u;

    for (r = 0u; r < 4u; r++) {
        Row_ReleaseAll();
        Row_DriveLow(r);
        SmallDelay();

        for (c = 0u; c < 4u; c++) {
            if (!gpio_read(BOARD_COL[c])) {
                raw |= KEY_MAP[r][c];
            }
        }
    }

    Row_ReleaseAll();

    return raw;
}

uint8_t Keypad_Task10ms(void)
{
    static uint8_t last_raw = 0u;
    static uint8_t stable = 0u;
    static uint8_t count = 0u;

    uint8_t raw;
    uint8_t pressed;

    raw = Keypad_ReadRaw();

    if (raw == last_raw) {
        if (count < 3u) {
            count++;
        }
    } else {
        count = 0u;
        last_raw = raw;
    }

    if (count == 3u && stable != raw) {
        pressed = raw & (uint8_t)(~stable);
        stable = raw;
        return pressed;
    }

    return 0u;
}