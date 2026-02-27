#pragma once
#include "stm32f1xx_hal.h"
#define MYSPI_GPIO_PORT    GPIOB
#define MYSPI_SS_PIN          GPIO_PIN_12  // 片选
#define MYSPI_SCK_PIN         GPIO_PIN_13   // 时钟
#define MYSPI_MISO_PIN        GPIO_PIN_14   // 主机输入
#define MYSPI_MOSI_PIN        GPIO_PIN_15   // 主机输出
void MYSPI_Init();
uint8_t MYSPI_Swap_Byte(uint8_t Byte);
void MYSPI_Start();
void MYSPI_Stop();
