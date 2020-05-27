#ifndef __BSP_SD_WRITE_READ_H
#define __BSP_SD_WRITE_READ_H


/*=================头文件==========================*/
#include "stdio.h"
#include <string.h>
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"
#include "ff.h"


/*=================================定义变量======================================*/
#define DEVICE_RUNNING (1)
#define DEVICE_STOPPED (0)




/*==============================================================================*/

/*======================函数声明==============*/
/*初始化BL8782 WiFi模块使能引脚，并禁用WiFi模块*/
void BL8782_PDN_INIT(void);

/* 文件系统初始化函数 */
void FatFs_Init(void);

/*文件写操作*/
void Record_Write(void);

/*文件读操作*/
void Record_Read(void);

void FatFs_DeInit(void);

/*向SD卡发送实验数据*/
void SendDataToSD(void);






#endif /*__BSP_SD_WRITE_READ_H*/
