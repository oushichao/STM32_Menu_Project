#include "W25Q64_Soft_SPI.h"
#include "MYSPI.h"
#include "OLED.h"
// 补全W25Q64容量宏（8MB=0x800000），用于地址校验
#define W25Q64_MAX_ADDRESS    0x7FFFFF
#define W25Q64_PAGE_SIZE      256     // 单页最大字节数
#define W25Q64_BUSY_MAX_CNT   500000  
// BUSY等待最大计数（根据主频调整，72MHz下约500ms）

//执行 W25Q64 的任何操作（读 / 写 / 擦除 / 查状态 / 读 ID 等）
//都必须先发送对应的指令码

//页编程，擦除等，每次进行修改内容时必须先进行写使能
void W25Q64_Soft_SPI_Init(){
    MYSPI_Init();
}
void W25Q64_Soft_SPI_WriteEnable(){
    MYSPI_Start();
    MYSPI_Swap_Byte(W25Q64_WRITE_ENABLE);
    MYSPI_Stop();
}
void W25Q64_Soft_SPI_Read_ID(uint8_t* MID,uint16_t* DID){
    if(MID==NULL||DID==NULL)
    return;
    MYSPI_Start();
    MYSPI_Swap_Byte(W25Q64_JEDEC_ID);
    *MID=MYSPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    *DID=MYSPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    *DID<<=8;
    *DID|=MYSPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    MYSPI_Stop();
}
uint8_t W25Q64_Soft_SPI_WaitForBusyState(){
    MYSPI_Start();
    uint8_t Status=0x01;
    uint32_t Busy_Cnt=W25Q64_BUSY_MAX_CNT;
    MYSPI_Swap_Byte(W25Q64_READ_STATUS_REGISTER_1);
    while((Status & 0x01) && Busy_Cnt-- > 0){
       Status=MYSPI_Swap_Byte(W25Q64_DUMMY_BYTE); 
    }
    MYSPI_Stop();
    if(Status==0x00)
        return 0;   //成功
    return 1;       //超时
}

//页编程---一次性最多写入256byte
uint8_t W25Q64_Soft_SPI_PageProgram(uint32_t Address,uint8_t* DataArray,uint16_t Count){
    if(Address>W25Q64_MAX_ADDRESS||DataArray==NULL||Count==0||Count>W25Q64_PAGE_SIZE )
        return 1;   //参数错误
    W25Q64_Soft_SPI_WriteEnable();
    MYSPI_Start();
    MYSPI_Swap_Byte(W25Q64_PAGE_PROGRAM);
    for(int i=2;i>=0;--i){
        MYSPI_Swap_Byte((Address>>8*i)&0xFF);
    }
    for(uint8_t i=0;i<Count;i++){
        MYSPI_Swap_Byte(DataArray[i]);
    }

    MYSPI_Stop();
    if(W25Q64_Soft_SPI_WaitForBusyState()==1)
        return 2;   //busy位超时等待
   return 0;        //成功
}

//扇区擦除
uint8_t W25Q64_Soft_SPI_SectorErase(uint32_t Address){
    if(Address>W25Q64_MAX_ADDRESS)
        return 1;//参数错误
    W25Q64_Soft_SPI_WriteEnable();
    MYSPI_Start();
    MYSPI_Swap_Byte(W25Q64_SECTOR_ERASE_4KB);
    MYSPI_Swap_Byte((Address >> 16) & 0xFF); // 地址高8位（bit23~16）
    MYSPI_Swap_Byte((Address >> 8) & 0xFF);  // 地址中8位（bit15~8）
    MYSPI_Swap_Byte(Address & 0xFF);         // 地址低8位（bit7~0）
    MYSPI_Stop();
    if(W25Q64_Soft_SPI_WaitForBusyState()==0)
        return 0;//成功
    return 2;    //超时
}

uint8_t W25Q64_Soft_SPI_ReadData(uint32_t Address,uint8_t* DataArray,uint16_t Count){
    if(Address>W25Q64_MAX_ADDRESS||DataArray==NULL||Count==0)
        return 1;   //参数错误
    MYSPI_Start();
    MYSPI_Swap_Byte(W25Q64_READ_DATA);
    for(int i=2;i>=0;--i){
        MYSPI_Swap_Byte((Address>>8*i)&0xFF);
    }
    for(uint8_t i=0;i<Count;i++){
        DataArray[i]=MYSPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    }
    MYSPI_Stop();
    return 0;//成功
}

// 1. W25Q64_Soft_SPI_WaitForBusyState
// return 0：BUSY 位成功置 0（芯片空闲），等待成功；
// return 1：BUSY 位未置 0（或因Busy_Cnt耗尽超时退出），等待失败；
// 2. W25Q64_Soft_SPI_PageProgram
// return 1：参数非法（地址越界 / 空指针 / 计数 0 / 超 256）；
// return 2：BUSY 等待失败（超时 / 芯片未空闲）；
// return 0：页编程执行成功，芯片已空闲。