#ifndef __BSP_SD_WRITE_READ_H
#define __BSP_SD_WRITE_READ_H


/*=================ͷ�ļ�==========================*/
#include "stdio.h"
#include <string.h>
#include "led/bsp_led.h"
#include "usart/bsp_debug_usart.h"
#include "ff.h"


/*=================================�������======================================*/
#define DEVICE_RUNNING (1)
#define DEVICE_STOPPED (0)




/*==============================================================================*/

/*======================��������==============*/
/*��ʼ��BL8782 WiFiģ��ʹ�����ţ�������WiFiģ��*/
void BL8782_PDN_INIT(void);

/* �ļ�ϵͳ��ʼ������ */
void FatFs_Init(void);

/*�ļ�д����*/
void Record_Write(void);

/*�ļ�������*/
void Record_Read(void);

void FatFs_DeInit(void);

/*��SD������ʵ������*/
void SendDataToSD(void);






#endif /*__BSP_SD_WRITE_READ_H*/
