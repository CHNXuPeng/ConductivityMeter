#ifndef __BSP_ADC_H
#define	__BSP_ADC_H

#include "stm32f4xx.h"

#define ELECONDUCTIVITY_ADC_NOFCHANEL      2

///*=====================通道1 IO======================*/
//// PC3 电导池电压
//// ADC IO宏定义
//#define POOL_ADC_GPIO_PORT1    GPIOC
//#define POOL_ADC_GPIO_PIN1     GPIO_Pin_3
//#define POOL_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOC
//#define POOL_ADC_CHANNEL1      ADC_Channel_13
///*=====================通道2 IO ======================*/
/*=====================通道1 IO======================*/
// PA6 电导池电压
// ADC IO宏定义
#define POOL_ADC_GPIO_PORT1    GPIOA
#define POOL_ADC_GPIO_PIN1     GPIO_Pin_6
#define POOL_ADC_GPIO_CLK1     RCC_AHB1Periph_GPIOA
#define POOL_ADC_CHANNEL1      ADC_Channel_6
/*=====================通道2 IO ======================*/
// PA4 参考电阻电压
// ADC IO宏定义
#define RESISTOR_ADC_GPIO_PORT2    GPIOA
#define RESISTOR_ADC_GPIO_PIN2     GPIO_Pin_4
#define RESISTOR_ADC_GPIO_CLK2     RCC_AHB1Periph_GPIOA
#define RESISTOR_ADC_CHANNEL2      ADC_Channel_4


// ADC 序号宏定义
#define ELECONDUCTIVITY_ADC              ADC1
#define ELECONDUCTIVITY_ADC_CLK          RCC_APB2Periph_ADC1
// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define ELECONDUCTIVITY_ADC_DR_ADDR    ((u32)ADC1+0x4c)


// ADC DMA 通道宏定义，这里我们使用DMA传输
#define ELECONDUCTIVITY_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define ELECONDUCTIVITY_ADC_DMA_CHANNEL  DMA_Channel_0
#define ELECONDUCTIVITY_ADC_DMA_STREAM   DMA2_Stream0



/*==============宏定义===========*/
#define         R_Resistor         100
#define         K_EleCndct         9.236










/*=========函数声明===========*/
void ELECONDUCTIVITY_ADC_Init(void);
float EleConductivity_Value(void);


#endif /* __BSP_ADC_H */



