#include "stm32f1xx_hal.h"
#include "MENU.h"
#include "OLED.h"
#include "KEY.h"
#include "PWM.h"
#include "W25Q64_Soft_SPI.h"
#include "MYFLASH.h"
#include "menu_items.h"

volatile uint8_t Current_Index=0;
Menu *Current_Menu=Menu_Main;
volatile uint8_t LED_Pulse=50;          //led占空比
volatile uint32_t BUZZER_Frequency=3000;//频率(2000~5000)
volatile uint8_t OLED_Contrast=127;     //对比度(0~255)
uint8_t LED_Data[3]={0};                //flash保存
uint8_t BUZZER_Data[3]={0};             //flash保存

void Led_Turn_On(){
    PWM_Led_Start();
}
void Led_Turn_Off(){
    PWM_Led_Stop();
}
void Led_Contrast_Add_Set(){
    PWM_SetCompare2(LED_Pulse+=10);
    Menu_Led_Brightness_Draw(Current_Menu);
}
void Led_Contrast_Dec_Set(){
    if(LED_Pulse>9){
        PWM_SetCompare2(LED_Pulse-=10);
    }
    else{ 
        LED_Pulse=0;
        PWM_SetCompare2(LED_Pulse);
    }
    Menu_Led_Brightness_Draw(Current_Menu);
}
void Led_Flash_Load(){
    Flash_Load(LED_Load_Address_Head,LED_Data,2);
}
void Menu_Led_Brightness_Draw(Menu*m){
    for(uint8_t i = 0; i < 10; i++) {
        OLED_ShowChar(1, 7 + i, ' ');
    }   
    for(uint8_t i = 0; i < (LED_Pulse / 10); i++) {
        OLED_ShowChar(1, 7 + i, '|');
    }
    OLED_ShowString(4,9,"V:");
    OLED_ShowNum(4,11,LED_Pulse,3);
}

void Buzzer_Turn_On(){
    PWM_Buzzer_Start();
}
void Buzzer_Turn_Off(){
    PWM_Led_Stop();
}
void Buzzer_Frequency_Add_Set(){
    if(BUZZER_Frequency<=4800)
        PWM_Buzzer_SetFrequency(BUZZER_Frequency+=200);
    else{
        BUZZER_Frequency=5000;
        PWM_Buzzer_SetFrequency(BUZZER_Frequency);
    }
    Menu_Buzzer_Frequency_Draw(Current_Menu);
}
void Buzzer_Frequency_Dec_Set(){
    if(BUZZER_Frequency<=2200){
        BUZZER_Frequency=2000;
        PWM_Buzzer_SetFrequency(BUZZER_Frequency);
    }
    else{
        PWM_Buzzer_SetFrequency(BUZZER_Frequency-=200);
    }   
    Menu_Buzzer_Frequency_Draw(Current_Menu); 
}
void Buzzer_Flash_Load(){
    Flash_Load(BUZZER_Load_Address_Head,LED_Data,2);
}
void Menu_Buzzer_Frequency_Draw(){
    OLED_ShowNum(1,8,BUZZER_Frequency,4);
    OLED_ShowString(1,12,"Hz");
}

void Oled_Contrast_Add_Set(){
    if(OLED_Contrast==255)
        OLED_SetContrast(OLED_Contrast);
    else
        OLED_SetContrast(OLED_Contrast+=16); 
    Menu_Oled_Contrast_Draw();   
}
void Oled_Contrast_Dec_Set(){
   if(OLED_Contrast==0)
        OLED_SetContrast(OLED_Contrast);
    else
        OLED_SetContrast(OLED_Contrast-=16);
    Menu_Oled_Contrast_Draw();    
}
void Menu_Oled_Contrast_Draw(){
    OLED_ShowNum(1,12,OLED_Contrast,3);
}

void Default_Set(){
    while(W25Q64_Soft_SPI_SectorErase(BUZZER_Load_Address_Head)!=0);
    while(W25Q64_Soft_SPI_SectorErase(LED_Load_Address_Head)!=0);
    LED_Pulse=50;
    BUZZER_Frequency=3000; 
    OLED_Contrast=127;
    PWM_SetCompare2(LED_Pulse);
    PWM_Buzzer_SetFrequency(BUZZER_Frequency);
    OLED_SetContrast(OLED_Contrast);
    __disable_irq(); // 关闭总中断
    HAL_NVIC_SystemReset(); // 立即复位
}
void Default_Failre_Set(){
    OLED_Clear();
    OLED_ShowString(1,1,"Save Failure!");
    HAL_Delay(500);
    __disable_irq(); // 关闭总中断
    HAL_NVIC_SystemReset(); // 立即复位
}
void Check_Default_Set(){
    if(W25Q64_Soft_SPI_ReadData(LED_Load_Address_Head,LED_Data,2)==0){
        OLED_Clear();
        if(LED_Data[0]==0x5A){
            LED_Pulse=LED_Data[1];    
            OLED_ShowString(1,1,"Led bri:");
            OLED_ShowNum(1,13,LED_Pulse,3);
        }
    }
    if(W25Q64_Soft_SPI_ReadData(BUZZER_Load_Address_Head,BUZZER_Data,2)==0){
        if(BUZZER_Data[0]==0x5A){
            BUZZER_Frequency=BUZZER_Data[1];
            OLED_ShowString(2,1,"Buzzer Fr:");
            OLED_ShowNum(2,12,BUZZER_Frequency,4);
        }
    }
    HAL_Delay(500);
    OLED_Clear();        
}

void Menu_Draw(Menu*m){//光标的指定以及菜单的变化
    OLED_Clear();
    if(m->draw){
        m->draw(m);
        return;
    }
    for(uint8_t i=0;i<m->item_count;i++){
        if(Current_Index==i){
            OLED_ShowChar(1+i,1,'>');
        }
        OLED_ShowString(i+1,3,m->items[i].text);    
    }
    
}
void Menu_Key_Handler(uint8_t key){//菜单选项控制
    switch (key)
    {
        case KEY_UP:{
            if(Current_Index>0)Current_Index--;
            Menu_Draw(Current_Menu);
            break;
        }
        case KEY_DOWM:{
            if(Current_Index<Current_Menu->item_count-1)Current_Index++;
            Menu_Draw(Current_Menu);
            break;
        }
        case KEY_ENTER:{
            Menu* Selected=&Current_Menu->items[Current_Index];
            if(Selected->Confirm)
                Selected->Confirm();
            if(Selected->items){
                Current_Menu=Selected;
                Current_Index=0;
                Menu_Draw(Current_Menu);
            }
            break;
        }
        case KEY_RETURN:{
            if(Current_Menu->parent){
                Current_Menu=Current_Menu->parent;
                Current_Index=0;
                Menu_Draw(Current_Menu);
            }    
            break;
        }
    }
}
void Flash_Load(uint32_t Address,uint8_t*Data,uint8_t Count){
    Data[0]=MAGIC_WORD;
    Data[1]=LED_Pulse;
    while(W25Q64_Soft_SPI_SectorErase(Address)!=0);
    while(W25Q64_Soft_SPI_PageProgram(Address,Data,Count)!=0);
    W25Q64_Soft_SPI_ReadData(Address,LED_Data,Count);
    OLED_Clear();
    OLED_ShowString(1,1,"Save OK!");
    HAL_Delay(500);
    OLED_Clear();
    Menu_Draw(Current_Menu);
}

