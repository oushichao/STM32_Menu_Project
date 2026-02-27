#include "stm32f1xx_hal.h"
#include "menu_items.h"
#include "MENU.h"
// 所有菜单项的定义（数据）

Menu Menu_Main[] = {
    {.text = "Main Menu", .Confirm = NULL, .draw = NULL, .parent = NULL, 
     .items = Menu_Main_Items, .item_count = 4},
};

Menu Menu_Main_Items[]={//----------主菜单------------
    {"Led Set",NULL,NULL,&Menu_Main[0],&Menu_Led_Items[0],4},
    {"Buzzer Set",NULL,NULL,&Menu_Main[0],&Menu_Buzzer_Items[0],4},
    {"Oled Set",NULL,NULL,&Menu_Main[0],&Menu_Oled_Items[0],4},
    {"Default Set",NULL,NULL,&Menu_Main[0],&Menu_Default[0],4}
};

Menu Menu_Led_Items[]={//---------led子菜单---------
    {"Turn On",Led_Turn_On,NULL,&Menu_Main_Items[0],NULL,0},
    {"Turn Off",Led_Turn_Off,NULL,&Menu_Main_Items[0],NULL,0},
    {"Brightness Set",NULL,NULL,&Menu_Main_Items[0],&Menu_Led_Brightness_Items[0],4},
    {"--------",NULL,NULL,&Menu_Main_Items[0],NULL,0}
};

Menu Menu_Led_Brightness_Items[]={//------设置led亮度的子菜单-------
    {"SET",NULL,NULL,&Menu_Led_Items[0],NULL,0},
    {"Bn+",Led_Contrast_Add_Set,NULL,&Menu_Led_Items[0],NULL,0},
    {"Bn-",Led_Contrast_Dec_Set,NULL,&Menu_Led_Items[0],NULL,0},
    {"Save",Led_Flash_Load,NULL,&Menu_Led_Items[0],NULL,0},
};

Menu Menu_Buzzer_Items[]={//---------buzzer子菜单---------
    {"Turn On",Buzzer_Turn_On,NULL,&Menu_Main_Items[1],NULL,0},
    {"Turn Off",Buzzer_Turn_Off,NULL,&Menu_Main_Items[1],NULL,0},
    {"Fre Set",NULL,NULL,&Menu_Main_Items[1],&Menu_Buzzer_Frequency_Items[0],4},
    {"--------",NULL,NULL,&Menu_Main_Items[1],NULL,0}
};

Menu Menu_Buzzer_Frequency_Items[]={//------设置buzzer频率的子菜单-------
    {"SET",NULL,NULL,&Menu_Buzzer_Items[0],NULL,0},
    {"Fr+",Buzzer_Frequency_Add_Set,NULL,&Menu_Buzzer_Items[0],NULL,0},
    {"Fr-",Buzzer_Frequency_Dec_Set,NULL,&Menu_Buzzer_Items[0],NULL,0},
    {"Save",Buzzer_Flash_Load,NULL,&Menu_Buzzer_Items[0],NULL,0},
};

Menu Menu_Oled_Items[]={//----------设置oled的子菜单---------
    {"Contrast:",NULL,NULL,&Menu_Main_Items[2],NULL,0},
    {"Brightness+16",Oled_Contrast_Add_Set,NULL,&Menu_Main_Items[2],NULL,0},
    {"Brightness-16",Oled_Contrast_Dec_Set,NULL,&Menu_Main_Items[2],NULL,0},
    {"---------",NULL,NULL,&Menu_Main_Items[2],NULL,0}
};

Menu Menu_Default[]={
    {"Are You Sure?",NULL,NULL,&Menu_Main_Items[0],NULL,0},
    {"Yes",Default_Set,NULL,&Menu_Main_Items[0],NULL,0},
    {"No",Default_Failre_Set,NULL,&Menu_Main_Items[0],NULL,0},
    {"--------",NULL,NULL,&Menu_Main_Items[0],NULL,0}
};
