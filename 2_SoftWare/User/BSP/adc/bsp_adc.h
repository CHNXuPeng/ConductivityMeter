#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

#define ELECONDUCTIVITY_ADC_NOFCHANEL      2

///*=====================ͨ��1 IO======================*/
//// PC3 �絼�ص�ѹ
//// ADC IO�궨��
//#define POOL_ADC_GPIO_PORT1    GPIOC
//#define POOL_ADC_GPIO_PIN1     GPIO_Pin_3
//#define POOL_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOC
//#define POOL_ADC_CHANNEL1      ADC_Channel_13
///*=====================ͨ��2 IO ======================*/
/*=====================ͨ��1 IO======================*/
// PA6 �絼�ص�ѹ
// ADC IO�궨��
#define POOL_ADC_GPIO_PORT1    GPIOA
#define POOL_ADC_GPIO_PIN1     GPIO_Pin_6
#define POOL_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOA
#define POOL_ADC_CHANNEL1      ADC_Channel_6
/*=====================ͨ��2 IO ======================*/
// PA4 �ο������ѹ
// ADC IO�궨��
#define RESISTOR_ADC_GPIO_PORT2    GPIOA
#define RESISTOR_ADC_GPIO_PIN2     GPIO_Pin_4
#define RESISTOR_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOA
#define RESISTOR_ADC_CHANNEL2      ADC_Channel_4


// ADC ��ź궨��
#define ELECONDUCTIVITY_ADC              ADC1
#define ELECONDUCTIVITY_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR�Ĵ����궨�壬ADCת���������ֵ����������
#define ELECONDUCTIVITY_ADC_DR_ADDR    ((u32)ADC1+0x4c)


// ADC DMA ͨ���궨�壬��������ʹ��DMA����
#define ELECONDUCTIVITY_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define ELECONDUCTIVITY_ADC_DMA_CHANNEL  DMA_Channel_0
#define ELECONDUCTIVITY_ADC_DMA_STREAM   DMA2_Stream0



/*==============�궨��===========*/
#define         R_Resistor         100
#define         K_EleCndct         9.236










/*=========��������===========*/
void ELECONDUCTIVITY_ADC_Init(void);
float EleConductivity_Value(void);


#endif /* __BSP_ADC_H */



