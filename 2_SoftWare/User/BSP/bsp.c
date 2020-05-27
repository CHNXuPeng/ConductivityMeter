//#include "stm32f4xx.h"
#include  "bsp.h"


void BL8782_PDN_INIT(void);

/*===============对板极各外设进行初始化配置=================*/
void  BSP_Init (void)
{
    /* 初始化LED*/
    LED_GPIO_Config();
    /* 使用PLLI2S作为MCO2输出源初始化函数，设定输出频率为100MHZ==*/
    //MCO2_PLLI2S_OUTPUT();

    /* AD9850初始化函数*/
    Init_AD9850();

    /* DS18B20初始化*/
    ds18b20_start();

    /* ADC初始化*/
    ELECONDUCTIVITY_ADC_Init();
    
    /* LCD初始化*/
    LCD_ALL_INIT();
    
    /* 初始化基本定时器定时，1s产生一次中断 */
    TIMx_Configuration();
    
    /* 按键中断函数初始化*/
    EXTI_Key_Config();
    
    /* SD卡读写初始化函数 */
    FatFs_Init();
    
    /* 触摸屏初始化*/
    GTP_Init_Panel();
    
    
    
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
