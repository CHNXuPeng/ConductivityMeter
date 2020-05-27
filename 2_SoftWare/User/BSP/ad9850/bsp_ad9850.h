//========================AD9850.h=============================//
/*--------------------------------------------
AD9850��������
----------------------------------------------*/
#ifndef __AD9850_H
#define __AD9850_H

#include "stm32f4xx.h"

/*ѡ������ͨ�ŷ�ʽ��ֻ��ѡ��һ��*/
#define MODE_SEIAL   //����ģʽ
//#define MODE_PARALLEL  //����ģʽ
#define AD9850_SYSTEM_COLCK     1000000
#define CRYSTAL_FREQ            125
/* AD9850 ���ƺ��������Ŷ��� */
#define AD9850_CONTROL_PORT     GPIOA
#define AD9850_FQUD             GPIO_Pin_2
#define AD9850_WCLK             GPIO_Pin_3
#define AD9850_RST              GPIO_Pin_4
#ifdef MODE_PARALLEL  //����ģʽ���ݿ�0~7
#define AD9850_DATA_PORT        GPIOC
#endif
#ifdef MODE_SEIAL   //����ģʽ���ݿ�   
#define AD9850_DATA             GPIO_Pin_5  
#define AD9850_DATA_Write_1     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_DATA,Bit_SET)
#define AD9850_DATA_Write_0     GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_DATA,Bit_RESET)
#endif 
#define AD9850_WCLK_SET         GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_WCLK,Bit_SET)
#define AD9850_WCLK_CLR         GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_WCLK,Bit_RESET)
#define AD9850_FQUD_SET         GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_FQUD,Bit_SET)
#define AD9850_FQUD_CLR         GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_FQUD,Bit_RESET)
#define AD9850_RST_SET          GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_RST,Bit_SET)
#define AD9850_RST_CLR          GPIO_WriteBit(AD9850_CONTROL_PORT,AD9850_RST,Bit_RESET)
/* ����ģʽ�������� */
#ifdef MODE_SEIAL    
extern void AD9850_Reset_Sreial(void) ;
extern void AD9850_Write_Serial(unsigned char W0,unsigned long freq) ;
#endif
/* ����ģʽ�������� */
#ifdef MODE_PARALLEL
extern void AD9850_Reset_Parallel(void) ;
extern void AD9850_Write_Parallel(unsigned char W0,unsigned long freq) ;
#endif
extern void Init_AD9850(void) ;
#endif /* AD8950_H */
//==================================================================//
