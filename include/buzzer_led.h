#ifndef BUZZER_LED_H
#define BUZZER_LED_H

#include <stdbool.h>

void Buzzer_Set(bool on);
void Buzzer_Pip300ms(void);
void Buzzer_AlarmStart5s(void);
void Buzzer_Task1ms(void);

void LedD6_Set(bool on);
void Buzzer_AlarmStop(void);
bool Buzzer_IsAlarmRinging(void);

#endif