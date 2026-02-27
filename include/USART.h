#pragma once 
// 关键补充：声明外部串口句柄，让main.c能访问USART.c中的huart1
extern UART_HandleTypeDef huart1;

// 原有函数声明保留
void USART1_UART_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendString(char *str);
void Serial_SendNumber(uint32_t Number);
uint8_t USART_Get_Rx_Flag(void);
uint8_t USART_Get_Rx_Data(void);
void Error_Handler(void);


