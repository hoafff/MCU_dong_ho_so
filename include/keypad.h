#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include "sn32f407_regs.h"

#define KEY_SW3    BIT(0)
#define KEY_SW6    BIT(1)
#define KEY_SW10   BIT(2)
#define KEY_SW16   BIT(3)

/* Nut chinh nhanh bo sung */
#define KEY_SW5    BIT(4)
#define KEY_SW9    BIT(5)

uint8_t Keypad_Task10ms(void);

#endif