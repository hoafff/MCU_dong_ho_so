#include "board.h"

const Pin BOARD_SEG[8] = {
    {GPIO_PORT0, 0}, /* A  */
    {GPIO_PORT0, 1}, /* B  */
    {GPIO_PORT0, 2}, /* C  */
    {GPIO_PORT0, 3}, /* D  */
    {GPIO_PORT0, 4}, /* E  */
    {GPIO_PORT0, 5}, /* F  */
    {GPIO_PORT0, 6}, /* G  */
    {GPIO_PORT0, 7}  /* DP */
};

const Pin BOARD_COM[4] = {
    {GPIO_PORT1, 9},
    {GPIO_PORT1, 10},
    {GPIO_PORT1, 11},
    {GPIO_PORT1, 12}
};

const Pin BOARD_ROW[4] = {
    {GPIO_PORT1, 4},
    {GPIO_PORT1, 5},
    {GPIO_PORT1, 6},
    {GPIO_PORT1, 7}
};

const Pin BOARD_COL[4] = {
    {GPIO_PORT2, 4},
    {GPIO_PORT2, 5},
    {GPIO_PORT2, 6},
    {GPIO_PORT2, 7}
};

const Pin PIN_BUZZER = {GPIO_PORT3, 0};
const Pin PIN_LED_D6 = {GPIO_PORT3, 8};

const Pin PIN_I2C_SCL = {GPIO_PORT0, 10};
const Pin PIN_I2C_SDA = {GPIO_PORT0, 11};

void Board_GPIO_Init(void)
{
    uint8_t i;

    for (i = 0; i < 8u; i++) {
        gpio_cfg(BOARD_SEG[i], GPIO_CFG_INACTIVE);
        gpio_clr(BOARD_SEG[i]);
        gpio_output(BOARD_SEG[i]);
    }

    for (i = 0; i < 4u; i++) {
        gpio_cfg(BOARD_COM[i], GPIO_CFG_INACTIVE);
        gpio_clr(BOARD_COM[i]);
        gpio_output(BOARD_COM[i]);
    }

    gpio_cfg(PIN_BUZZER, GPIO_CFG_INACTIVE);
    gpio_clr(PIN_BUZZER);
    gpio_output(PIN_BUZZER);

    gpio_cfg(PIN_LED_D6, GPIO_CFG_INACTIVE);
    gpio_set(PIN_LED_D6);
    gpio_output(PIN_LED_D6);

    for (i = 0; i < 4u; i++) {
        gpio_cfg(BOARD_ROW[i], GPIO_CFG_PULLUP);
        gpio_input(BOARD_ROW[i]);

        gpio_cfg(BOARD_COL[i], GPIO_CFG_PULLUP);
        gpio_input(BOARD_COL[i]);
    }

    gpio_cfg(PIN_I2C_SCL, GPIO_CFG_PULLUP);
    gpio_cfg(PIN_I2C_SDA, GPIO_CFG_PULLUP);
    gpio_input(PIN_I2C_SCL);
    gpio_input(PIN_I2C_SDA);
}