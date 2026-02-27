#include "stm32f1xx_hal.h"
#include "MYFLASH.h"
//------------------读取----------------------
uint32_t MYFlash_ReadWord(uint32_t Address){
    return *(__IO uint32_t*)Address;
}
uint16_t MYFlash_ReadHalfWord(uint32_t Address){
    return *(__IO uint16_t*)Address;
}
uint8_t MYFlash_ReadByte(uint32_t Address){
    return *(__IO uint8_t*)Address;
}

//----------------擦除-------------------
void MYFLASH_Erase(uint32_t Address,uint8_t Number){
    uint32_t PageError=0;//接收擦除错误码
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInstructure;
    EraseInstructure.Banks=0;//多Bank芯片有效
    EraseInstructure.NbPages=Number;//擦除的页数
    EraseInstructure.PageAddress=Address;//擦除页的首地址
    EraseInstructure.TypeErase=FLASH_TYPEERASE_PAGES;//擦除类型--只能按页擦除
    if(HAL_FLASHEx_Erase(&EraseInstructure,&PageError)!=HAL_OK)
    while(1);
    HAL_FLASH_Lock();
}

//--------------------写入-----------------
void MYFLASH_WriteWord(uint32_t Address,uint32_t Word){
    HAL_FLASH_Unlock();
    if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,Address,Word)!=HAL_OK)
    while(1);
    HAL_FLASH_Lock();
}