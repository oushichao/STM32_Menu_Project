#include "stm32f1xx_hal.h"
#include "I2C_SOFT.h"
void I2C_SOFT_W_SDA(uint8_t BitValue){
    HAL_GPIO_WritePin(I2C_GPIO_Port, I2C_SDA_Pin, BitValue);
}
void I2C_SOFT_W_SCL(uint8_t BitValue){
    HAL_GPIO_WritePin(I2C_GPIO_Port, I2C_SCL_Pin, BitValue);
}
uint8_t I2C_SOFT_R_SDA(){
    return HAL_GPIO_ReadPin(I2C_GPIO_Port, I2C_SDA_Pin);
}
void I2C_SOFT_Init(){
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = I2C_SCL_Pin | I2C_SDA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(I2C_GPIO_Port, &GPIO_InitStruct);
}
void I2C_Start(){
    I2C_SOFT_W_SDA(1);
    I2C_SOFT_W_SCL(1);
    I2C_SOFT_W_SDA(0);
    I2C_SOFT_W_SCL(0);
}
void I2C_Stop(){
    I2C_SOFT_W_SDA(0);
    I2C_SOFT_W_SCL(1);
    I2C_SOFT_W_SDA(1);
}
void I2C_SendByte(uint8_t Byte){
    for(int i=0; i<8; i++){
        I2C_SOFT_W_SDA((Byte & 0x80) >> i);
        I2C_SOFT_W_SCL(1);
        I2C_SOFT_W_SCL(0);
    }
}
uint8_t I2C_ReceiveByte(){
    uint8_t Byte = 0x00;
    for(int i=0; i<8; i++){
        I2C_SOFT_W_SCL(1);
        if(I2C_SOFT_R_SDA()==1){
            Byte |= (0x80 >> i);
        }
        I2C_SOFT_W_SCL(0);
    }
    return Byte;
}
void I2C_SendAck(uint8_t Ack){
    I2C_SOFT_W_SDA(Ack);
    I2C_SOFT_W_SCL(1);
    I2C_SOFT_W_SCL(0);
}
uint8_t I2C_ReceiveAck(){
    I2C_SOFT_W_SDA(1);
    I2C_SOFT_W_SCL(1);
    uint8_t Ack = I2C_SOFT_R_SDA();
    I2C_SOFT_W_SCL(0);
    return Ack;
}