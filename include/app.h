#ifndef APP_H
#define APP_H

#include <stdint.h>

void App_Init(void);
void App_Task1ms(void);
void App_HandleKeys(uint8_t keys);
void App_OnRtcSecond(void);

#endif