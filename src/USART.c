#include "stm32f1xx_hal.h"
#include "USART.h"
#include <string.h>
// 中断接收核心变量：加volatile禁止编译器优化
static volatile uint8_t  g_Rx_Byte;   // 接收单字节缓冲区
static volatile uint8_t  g_Rx_Flag = 0;// 接收完成标志位（中断仅置位，主循环处理）
#define RX_BUFFER_SIZE 64
static volatile uint8_t Rx_head=0;
static volatile uint8_t Rx_tail=0;
static volatile uint8_t Rx_Buffer[RX_BUFFER_SIZE];//接收缓冲区
// 串口1句柄（全局/静态，供整个工程使用）
UART_HandleTypeDef huart1;
// HAL 库：定义串口句柄结构体，里面包含
// 「外设实例（USART1）+ 初始化参数 + 状态标志」，全局定义让工程其他文件能调用串口 1

void USART1_UART_Init(){
    //配置参数，STM32F1 的 USART1 默认对应PA9（TX）、PA10（RX）
    huart1.Instance=USART1;
    huart1.Init.BaudRate=9600;//波特率
    huart1.Init.HwFlowCtl=UART_HWCONTROL_NONE;//无硬件流控制
    huart1.Init.Mode=UART_MODE_TX_RX;//收发模式
    huart1.Init.OverSampling=UART_OVERSAMPLING_16;//过采样16倍
    huart1.Init.Parity=UART_PARITY_NONE;//无奇偶校验
    huart1.Init.StopBits=UART_STOPBITS_1;//1位停止位
    huart1.Init.WordLength=UART_WORDLENGTH_8B;//8位数据位
    //初始化
    HAL_UART_Init(&huart1);

    // 启动串口1接收中断：接收到1个字节就触发中断（制造中断的核心步骤）
    HAL_UART_Receive_IT(&huart1,&g_Rx_Byte,1);
}

// 不需要在 main 函数中手动调用，它是由 HAL 库底层自动触发调用的，
// 而非用户显式调用。

// HAL_UART_Init：负责串口通用参数初始化（波特率、校验位等），是 HAL 库通用逻辑；
// HAL_UART_MspInit：负责串口底层硬件初始化（GPIO、时钟、中断），
// 是和 STM32F1 芯片强相关的逻辑，由用户重写实现
void HAL_UART_MspInit(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_struct={0};

        HAL_NVIC_SetPriority(USART1_IRQn,2,0);//配置优先级
        HAL_NVIC_EnableIRQ(USART1_IRQn);//开启全局中断


        GPIO_struct.Mode=GPIO_MODE_AF_PP;
        GPIO_struct.Pin=GPIO_PIN_9;
        GPIO_struct.Pull=GPIO_PULLUP;
        GPIO_struct.Speed=GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA,&GPIO_struct);

        GPIO_struct.Mode=GPIO_MODE_INPUT;
        GPIO_struct.Pin=GPIO_PIN_10;
        GPIO_struct.Pull=GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA,&GPIO_struct);
    }
}
void Serial_SendByte(uint8_t Byte){
    HAL_UART_Transmit(&huart1,&Byte,1,100);
}
void Serial_SendString(char *str){
    // HAL_UART_Transmit(&huart1, (uint8_t*)str, strlen(str), 1000);
    while(*str!='\0'){
        Serial_SendByte((uint8_t)*str++);
    }
}
void Serial_SendNumber(uint32_t Num){
    if(Num==0){
        Serial_SendByte('0');
        Serial_SendByte('\r');
        Serial_SendByte('\n');
        return ;
    }
    uint8_t buf[10]={0};
    int length=0;
    while(Num>0){
        buf[length++]=(Num%10);
        Num/=10;
    }
    for(int i=length-1;i>=0;i--){
        Serial_SendByte(buf[i]+'0');      
    }
    Serial_SendByte('\r');
    Serial_SendByte('\n');  
}
uint8_t Serial_ReadData(uint8_t *ch){
    //head写指针，tail读指针，head==tail表示缓冲区空
    if(Rx_head==Rx_tail){
        return 0;//缓冲区空
    }
    *ch=Rx_Buffer[Rx_tail++];
    if(Rx_tail>=RX_BUFFER_SIZE){
        Rx_tail=0;//环形缓冲区
    }
    return 1;//成功读取一个字节
}
// 串口1中断服务函数：后续开启接收中断必须保留
void USART1_IRQHandler(void)
{
  // 调用HAL库通用串口中断处理函数，和你之前的逻辑完全一致
  HAL_UART_IRQHandler(&huart1);
}
//接收完成中断
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(huart->Instance==USART1){
        if(HAL_UART_GetState(&huart1)==HAL_UART_STATE_BUSY_RX){
            uint8_t next_head=Rx_head+1;
            if(next_head>=RX_BUFFER_SIZE){
                next_head=0;//环形缓冲区
            }
            if(next_head!=Rx_tail){//缓冲区未满
                Rx_Buffer[Rx_head]=g_Rx_Byte;//存储接收到的字节
                Rx_head=next_head;//更新写指针
            }
        }
        // 继续启动下一次接收中断，形成循环
        HAL_UART_Receive_IT(&huart1,&g_Rx_Byte,1);
    }
 }
// 【核心修正】接收完成中断回调：仅置标志位，不做任何阻塞操作，彻底避免死锁

// 接收完成中断回调
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
// {
//     if(huart->Instance == USART1)
//     {
//         g_Rx_Flag = 1; // 仅置位标志
//         // 【修正】统一使用 g_Rx_Byte
//         HAL_UART_Receive_IT(&huart1, (uint8_t*)&g_Rx_Byte, 1);
//     }
// }

// // 【核心修正】STM32F1 专用错误回调：通过读寄存器清除标志
// void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
// {
//     if(huart->Instance == USART1)
//     {
//         // STM32F1 清除错误标志的正确方式：
//         // 1. 读 SR 寄存器
//         // 2. 读 DR 寄存器
//         // 这样可以自动清除 ORE/FE/NE/PE 标志
//         volatile uint32_t tmp_reg = 0x00U;
//         tmp_reg = huart->Instance->SR;
//         tmp_reg = huart->Instance->DR;
//         (void)tmp_reg; // 避免编译器警告未使用变量

//         // 重新启动接收中断
//         HAL_UART_Receive_IT(huart, (uint8_t*)&g_Rx_Byte, 1);
//     }
// }


// // 【新增】获取接收标志和数据，供主循环调用
// uint8_t USART_Get_Rx_Flag(void)
// {
//     return g_Rx_Flag;
// }

// uint8_t USART_Get_Rx_Data(void)
// {
//     g_Rx_Flag = 0; // 读取数据后自动清除标志位
//     return g_Rx_Byte;
// }