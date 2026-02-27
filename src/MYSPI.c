#include "MYSPI.h"

void MYSPI_W_SS(uint8_t pin_state){
    HAL_GPIO_WritePin(MYSPI_GPIO_PORT,MYSPI_SS_PIN,pin_state);
}
void MYSPI_W_SCK(uint8_t pin_state){
    HAL_GPIO_WritePin(MYSPI_GPIO_PORT,MYSPI_SCK_PIN ,pin_state);
}
void MYSPI_W_MOSI(uint8_t pin_state){
    HAL_GPIO_WritePin(MYSPI_GPIO_PORT,MYSPI_MOSI_PIN,pin_state);
}
uint8_t MYSPI_R_MISO(){
   return HAL_GPIO_ReadPin(MYSPI_GPIO_PORT,MYSPI_MISO_PIN);
}
void MYSPI_Start(){
    MYSPI_W_SS(0);
}
void MYSPI_Stop(){
    MYSPI_W_SS(1);
}
void MYSPI_Init(){
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pin=MYSPI_SS_PIN |MYSPI_MOSI_PIN;
    GPIO_InitStructure.Pull=GPIO_PULLUP;
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(MYSPI_GPIO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.Pin=MYSPI_SCK_PIN;
    GPIO_InitStructure.Pull=GPIO_PULLDOWN;
    HAL_GPIO_Init(MYSPI_GPIO_PORT,&GPIO_InitStructure);

    GPIO_InitStructure.Pin=MYSPI_MISO_PIN;
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT ;
    GPIO_InitStructure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(MYSPI_GPIO_PORT,&GPIO_InitStructure);

    MYSPI_W_SS(1);
    MYSPI_W_SCK(0);
    MYSPI_W_MOSI(1);
}
uint8_t MYSPI_Swap_Byte(uint8_t Byte){
    uint8_t ReceiveData=0x00;
    for(int i=0;i<8;++i){
        MYSPI_W_MOSI(Byte&(0x80>>i));
        MYSPI_W_SCK(1);
        if(MYSPI_R_MISO()==1)ReceiveData|=(0x80>>i);
        MYSPI_W_SCK(0);
    }
    return ReceiveData;
}