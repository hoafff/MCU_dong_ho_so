#include "display7seg.h"
#include "board.h"

static volatile uint8_t s_disp_digits[4] = {0, 0, 0, 0};
static volatile uint8_t s_disp_blink_mask = 0u;
static volatile uint8_t s_disp_dp_mask = BIT(1);
static volatile bool s_blink_on = true;

static const uint8_t SEG_LUT[10] = {
    0x3F, /* 0 */
    0x06, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F  /* 9 */
};

static void Display_WriteSegments(uint8_t pattern)
{
    uint8_t i;

    for (i = 0; i < 8u; i++) {
        gpio_write(BOARD_SEG[i], (pattern & BIT(i)) != 0u);
    }
}

static void Display_AllComOff(void)
{
    uint8_t i;

    for (i = 0; i < 4u; i++) {
        gpio_clr(BOARD_COM[i]);
    }
}

void Display_SetBlinkOn(bool on)
{
    s_blink_on = on;
}

void Display_Scan1ms(void)
{
    static uint8_t index = 0;
    uint8_t pattern;
    bool visible;

    pattern = 0u;

    Display_AllComOff();

    visible = ((s_disp_blink_mask & BIT(index)) == 0u) || s_blink_on;

    if (visible) {
        pattern = SEG_LUT[s_disp_digits[index] % 10u];

        if (s_disp_dp_mask & BIT(index)) {
            pattern |= 0x80u;
        }

        Display_WriteSegments(pattern);
        gpio_set(BOARD_COM[index]);
    } else {
        Display_WriteSegments(0u);
    }

    index = (uint8_t)((index + 1u) & 0x03u);
}

void Display_ShowTime(uint8_t hour, uint8_t minute, uint8_t blink_mask)
{
    s_disp_digits[0] = hour / 10u;
    s_disp_digits[1] = hour % 10u;
    s_disp_digits[2] = minute / 10u;
    s_disp_digits[3] = minute % 10u;

    s_disp_blink_mask = blink_mask;
    s_disp_dp_mask = BIT(1);
}