#pragma once
#include "stm32f1xx_hal.h"
#define KEY_1 GPIO_PIN_2
#define KEY_2 GPIO_PIN_4
#define KEY_3 GPIO_PIN_6
#define KEY_4 GPIO_PIN_0
void KEY_Init(void);
uint8_t KEY_GetNum(void);
