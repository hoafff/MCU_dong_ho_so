#ifndef RTC_APP_H
#define RTC_APP_H

#include <stdbool.h>

void RTC_Init_1Hz_ELS(void);
void RTC_Tick1ms(void);
bool RTC_ConsumeSecondFlag(void);

#endif