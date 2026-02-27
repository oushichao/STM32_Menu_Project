#pragma once
#include "stm32f1xx_hal.h"
//------------------读取----------------------
uint32_t MYFlash_ReadWord(uint32_t Address);
uint16_t MYFlash_ReadHalfWord(uint32_t Address);
uint8_t MYFlash_ReadByte(uint32_t Address);
//----------------擦除-------------------
void MYFLASH_Erase(uint32_t Address,uint8_t Number);
//--------------------写入-----------------
void MYFLASH_WriteWord(uint32_t Address,uint32_t Word);

