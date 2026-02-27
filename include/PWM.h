#pragma once
#include "stm32f1xx_hal.h"
void PWM_Init();
void PWM_SetCompare2(uint32_t Compare);
void PWM_SetCompare1(uint32_t Compare);
void PWM_Led_Stop();
void PWM_Led_Start();

void PWM_Buzzer_Toggle(void);
void PWM_Buzzer_Stop();
void PWM_Buzzer_Start();
void PWM_Buzzer_SetFrequency(uint32_t freq);