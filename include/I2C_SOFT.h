#pragma once
#define I2C_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_4
#define I2C_SDA_Pin GPIO_PIN_5

void I2C_SOFT_W_SDA(uint8_t BitValue);
void I2C_SOFT_W_SCL(uint8_t BitValue);
uint8_t I2C_SOFT_R_SDA(void);
void I2C_SOFT_Init();
void I2C_Start();
void I2C_Stop();
void I2C_SendByte(uint8_t Byte);
uint8_t I2C_ReceiveByte();
void I2C_SendAck(uint8_t Ack);
uint8_t I2C_ReceiveAck();
