#ifndef DISPLAY7SEG_H
#define DISPLAY7SEG_H

#include <stdint.h>
#include <stdbool.h>

void Display_Scan1ms(void);
void Display_ShowTime(uint8_t hour, uint8_t minute, uint8_t blink_mask);
void Display_SetBlinkOn(bool on);

#endif