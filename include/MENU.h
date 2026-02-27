#pragma once
#include "stm32f1xx_hal.h"
typedef struct  Menu{
    char* text;                 //菜单显示文本
    void (*Confirm)(void);      //确认键执行的函数
    void(*draw)(struct Menu*);  //自定义绘制函数
    struct Menu *parent;        //父菜单
    struct Menu *items;         //子菜单数组
    uint8_t item_count;         //子菜单数量
}Menu;

extern volatile uint8_t LED_Pulse;
extern volatile uint32_t BUZZER_Frequency;
extern volatile uint8_t OLED_Contrast;
extern uint8_t LED_Data[];
extern  uint8_t BUZZER_Data[];
#define LED_Load_Address_Head 0x000000
#define BUZZER_Load_Address_Head 0x001000
#define MAGIC_WORD 0x5A
#define KEY_UP  1
#define KEY_DOWM    2
#define KEY_ENTER   3
#define KEY_RETURN  4
extern volatile uint8_t Current_Index;
extern Menu *Current_Menu;

//-------------------------------------------------
void Led_Turn_On();
void Led_Turn_Off();
void Led_Contrast_Add_Set();
void Led_Contrast_Dec_Set();
void Led_Flash_Load();
void Menu_Led_Brightness_Draw(Menu*m);

void Buzzer_Turn_On();
void Buzzer_Turn_Off();
void Buzzer_Frequency_Add_Set();
void Buzzer_Frequency_Dec_Set();
void Buzzer_Flash_Load();
void Menu_Buzzer_Frequency_Draw();

void Oled_Contrast_Add_Set();
void Oled_Contrast_Dec_Set();
void Menu_Oled_Contrast_Draw();

void Default_Set();
void Default_Failre_Set();
void Check_Default_Set();

void Menu_Draw(Menu*m);
void Menu_Key_Handler(uint8_t key);
void Flash_Load(uint32_t Address, uint8_t *Data, uint8_t Count);


