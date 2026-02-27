#include "stm32f1xx_hal.h"
#include "OLED.h"
#include "KEY.h"
#include "MENU.h"
#include "PWM.h"
#include "W25Q64_Soft_SPI.h"
#include "menu_items.h"
// 前置函数声明
   void SystemClock_Config(void);


int main(void)
{
    // 标准初始化顺序，100%兼容HAL库
    HAL_Init();
    SystemClock_Config(); // 配置内部HSI 8MHz，SysTick精准计时
    KEY_Init();
    OLED_Init();
    PWM_Init();
    W25Q64_Soft_SPI_Init();
    Current_Menu = Menu_Main; // 主菜单指针
    Current_Index = 0;        // 初始选中第0项
    Check_Default_Set();
    Menu_Draw(Current_Menu);
    
    while(1){
      uint8_t key=KEY_GetNum();
      Menu_Key_Handler(key);
    }
}
// 使用片上硬件外设 + 调用 HAL 库HAL_XXX_xxx驱动函数→必须用句柄
// 例：定时器 TIM2（HAL_TIM_xxx）、硬件 SPI1（HAL_SPI_xxx）、
// 硬件 I2C1（HAL_I2C_xxx）、串口 USART1（HAL_UART_xxx）、ADC1（HAL_ADC_xxx）；

// 【核心】稳定的内部HSI时钟配置，8MHz系统主频，自动配置SysTick
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  //定义振荡器配置结构体，初始化为 0，
  //用来配置 STM32 的时钟源（内部 HSI、外部 HSE、PLL 锁相环等）。
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  //定义总线时钟配置结构体，初始化为 0，用来配置 CPU 主频（SYSCLK）
  //和各总线（AHB、APB1、APB2）的分频系数。

  // 仅开启内部8MHz HSI.
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    while(1); // 时钟配置失败卡死，方便定位
  }

  // 系统时钟直接用HSI 8MHz，所有总线不分频
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    // 作用：指定要一次性配置的 4 个核心时钟：
    // SYSCLK：系统时钟，CPU 的核心运行时钟；
    // HCLK：AHB 总线时钟，给内存、DMA、GPIO 等外设用；
    // PCLK1：APB1 低速总线时钟，给串口 2、定时器 2 等低速外设用；
    // PCLK2：APB2 高速总线时钟，给 GPIO、串口 1 等高速外设用。

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    while(1);
  }

  // 自动配置SysTick：8MHz主频，1ms中断一次，HAL_Delay正常工作
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
//  HAL_RCC_GetHCLKFreq()会返回 HCLK 的频率（这里是 8000000Hz），
//   除以 1000 得到 8000，意思是 SysTick 每计数 8000 个时钟周期，
//   就触发一次中断，8MHz 时钟下正好是 1ms。

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// 【必须补全】SysTick中断服务函数，HAL库计时的核心
void SysTick_Handler(void)
{
  HAL_IncTick(); // 累加计时变量，HAL_Delay依赖这个
}

// 补全核心异常中断，避免程序跑飞
void HardFault_Handler(void)// 硬件故障中断
{
  while(1);
}

void NMI_Handler(void)// 非屏蔽中断
{
  while(1);
}