#ifndef BOARD_H
#define BOARD_H

#include "gpio.h"

extern const Pin BOARD_SEG[8];
extern const Pin BOARD_COM[4];
extern const Pin BOARD_ROW[4];
extern const Pin BOARD_COL[4];

extern const Pin PIN_BUZZER;
extern const Pin PIN_LED_D6;

extern const Pin PIN_I2C_SCL;
extern const Pin PIN_I2C_SDA;

void Board_GPIO_Init(void);

#endif