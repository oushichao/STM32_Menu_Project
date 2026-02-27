#include "stm32f1xx_hal.h"
#include "KEY.h"
void KEY_Init(void){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE(); 
    GPIO_InitStruct.Pin = KEY_1 | KEY_2|KEY_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = KEY_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; 
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);     
}
uint8_t KEY_GetNum(void){
    if(HAL_GPIO_ReadPin(GPIOA, KEY_1) == GPIO_PIN_RESET){
        HAL_Delay(20);
        while(HAL_GPIO_ReadPin(GPIOA, KEY_1) == GPIO_PIN_RESET);
        HAL_Delay(20);
        return 4;
    }
    if(HAL_GPIO_ReadPin(GPIOA, KEY_2) == GPIO_PIN_RESET){
        HAL_Delay(20);
        while(HAL_GPIO_ReadPin(GPIOA, KEY_2) == GPIO_PIN_RESET);
        HAL_Delay(20);
        return 3;
    }
    if(HAL_GPIO_ReadPin(GPIOA, KEY_3) == GPIO_PIN_RESET){
        HAL_Delay(20);
        while(HAL_GPIO_ReadPin(GPIOA, KEY_3) == GPIO_PIN_RESET);
        HAL_Delay(20);
        return 2;
    }
    if(HAL_GPIO_ReadPin(GPIOB, KEY_4) == GPIO_PIN_RESET){
        HAL_Delay(20);
        while(HAL_GPIO_ReadPin(GPIOB, KEY_4) == GPIO_PIN_RESET);
        HAL_Delay(20);
        return 1;
    }    
    else{
        return 0;
    }
}
