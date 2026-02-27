#include "stm32f1xx_hal.h"
#include "PWM.h"
//PSC 0~65535
//ARR 0~65535
//在公式中为 1~65536，在频率精准的前提下，尽量让 ARR 越大越好，PSC 越小越好
// 定时器句柄（全局唯一，HAL库所有操作都基于这个句柄）
TIM_HandleTypeDef htim2;//---管理LED
TIM_HandleTypeDef htim3;//---管理蜂鸣器
void PWM_Init(){
    // 1. 开启外设时钟（必须先开时钟，再配置外设）
    __HAL_RCC_TIM2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 2. GPIO初始化（PA0=TIM2_CH1、PA1=TIM2_CH2，复用推挽输出）
    GPIO_InitTypeDef GPIOInitStructure;
    GPIOInitStructure.Mode = GPIO_MODE_AF_PP;
    GPIOInitStructure.Pin = GPIO_PIN_1 | GPIO_PIN_0;
    GPIOInitStructure.Pull = GPIO_NOPULL;
    GPIOInitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIOInitStructure);

    // 3. 定时器时基核心配置（关键修复：直接赋值给句柄的Init成员，配置真正生效）
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 720 - 1;                 // 预分频：72MHz系统时钟 → 100KHz计数频率
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;    // 向上计数模式
    htim2.Init.Period = 100 - 1;                    // 自动重装值：100KHz/100 = 1KHz PWM频率
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim2); // 时基初始化，真正加载上面的配置到硬件

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 720 - 1;                 // 预分频：72MHz系统时钟 → 100KHz计数频率
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;    // 向上计数模式
    htim3.Init.Period = 100 - 1;                    // 自动重装值：100KHz/100 = 1KHz PWM频率
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_Base_Init(&htim3); // 时基初始化，真正加载上面的配置到硬件    

    // 4. PWM通道配置（修复：用PWM专用配置函数，分通道独立赋值，规避HAL库坑点）
    TIM_OC_InitTypeDef TIM_OC_InitStucture;
    // 通用配置（PWM1模式：计数值<CCR时输出高电平，符合LED正极接引脚的接线逻辑）
    TIM_OC_InitStucture.OCMode = TIM_OCMODE_PWM1;
    TIM_OC_InitStucture.OCPolarity = TIM_OCPOLARITY_LOW;
    TIM_OC_InitStucture.OCFastMode = TIM_OCFAST_DISABLE;

    // 配置CH1（PA0）：单独赋值初始占空比
    TIM_OC_InitStucture.Pulse = 50;
    HAL_TIM_PWM_ConfigChannel(&htim3, &TIM_OC_InitStucture, TIM_CHANNEL_1);

    // 配置CH2（PA1）：重新赋值初始占空比，避免结构体被库函数修改导致配置失效
    TIM_OC_InitStucture.Pulse = 50;
    HAL_TIM_PWM_ConfigChannel(&htim2, &TIM_OC_InitStucture, TIM_CHANNEL_2);

    // 5. 启动定时器+PWM输出（先启定时器，再启PWM，保证时序正确）
    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start(&htim3);

    PWM_Led_Stop();
    PWM_Buzzer_Stop();
    
}

// 设置PA0(TIM2_CH1)占空比（Compare范围0-99，对应0%-100%占空比）
void PWM_SetCompare1(uint32_t Compare){
    if(Compare > 99) Compare = 99;
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, Compare);
}

// 设置PA1(TIM2_CH2)占空比（Compare范围0-99，对应0%-100%占空比）
void PWM_SetCompare2(uint32_t Compare){
    if(Compare > 99) Compare = 99;
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Compare);
}

void PWM_Led_Stop(){
    // 1. 停止TIM2_CH2的PWM输出
    HAL_TIM_PWM_Stop(&htim2,TIM_CHANNEL_2);

    // 2. 将PA1切回普通GPIO模式，由CPU直接控制电平
    GPIO_InitTypeDef GPIOInitStructure;
    GPIOInitStructure.Pin = GPIO_PIN_1;
    GPIOInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOInitStructure.Pull = GPIO_NOPULL;
    GPIOInitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIOInitStructure);

    // 3. 输出高电平（共阳接法：PA1=高，LED灭
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
}

void PWM_Led_Start(){
    GPIO_InitTypeDef GPIOInitStructure;
    GPIOInitStructure.Pin = GPIO_PIN_1;
    GPIOInitStructure.Mode = GPIO_MODE_AF_PP;
    GPIOInitStructure.Pull = GPIO_NOPULL;
    GPIOInitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIOInitStructure);

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
}

void PWM_Buzzer_Stop(){
    // 1. 停止TIM2_CH1的PWM输出
    HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_1);
    // 2. PA0切回普通GPIO模式，由CPU直接控制电平
    GPIO_InitTypeDef GPIOInitStructure;
    GPIOInitStructure.Pin = GPIO_PIN_0;
    GPIOInitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIOInitStructure.Pull = GPIO_NOPULL;
    GPIOInitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIOInitStructure);
    // 3. 输出高电平（低电平触发：高电平=无效，蜂鸣器彻底停止）
    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
}

void PWM_Buzzer_Start(){
    // 1. PA0切回复用推挽模式，交还控制权给定时器，PWM才能正常输出
    GPIO_InitTypeDef GPIOInitStructure;
    GPIOInitStructure.Pin = GPIO_PIN_0;
    GPIOInitStructure.Mode = GPIO_MODE_AF_PP;
    GPIOInitStructure.Pull = GPIO_NOPULL;
    GPIOInitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIOInitStructure);
    // 2. 启动TIM2_CH1的PWM输出
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
}

void PWM_Buzzer_SetFrequency(uint32_t freq){
    uint32_t timer_clk = 72000000;
    // 保存当前占空比
    uint8_t duty = (uint8_t)((__HAL_TIM_GET_COMPARE(&htim3, TIM_CHANNEL_1) * 100) / (__HAL_TIM_GET_AUTORELOAD(&htim3) + 1));
    
    // 只停止蜂鸣器通道，不影响LED
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
    if(freq>5000)
        freq=5000;
    if(freq<2000)
        freq=2000;
    // 计算新参数
    uint32_t psc = 0;
    uint32_t arr = 0;
    for(psc = 0; psc <= 65535; psc++)
    {
        arr = (timer_clk / (psc + 1)) / freq - 1;
        if(arr <= 65535) break;
    }

    // 更新配置
    htim3.Init.Prescaler = psc;
    htim3.Init.Period = arr;
    HAL_TIM_Base_Init(&htim3);

    // 恢复占空比
    PWM_SetCompare1(duty);

    // 重新启动
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void PWM_Buzzer_Toggle(void)
{
    static uint8_t buzzer_state = 0;

    if(buzzer_state == 0)
    {
        PWM_Buzzer_Start();
        buzzer_state = 1;
    }
    else
    {
        PWM_Buzzer_Stop();
        buzzer_state = 0;
    }
}