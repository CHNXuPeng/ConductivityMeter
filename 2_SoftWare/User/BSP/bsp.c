//#include "stm32f4xx.h"
#include  "bsp.h"


void BL8782_PDN_INIT(void);

/*===============�԰弫��������г�ʼ������=================*/
void  BSP_Init (void)
{
    /* ��ʼ��LED*/
    LED_GPIO_Config();
    /* ʹ��PLLI2S��ΪMCO2���Դ��ʼ���������趨���Ƶ��Ϊ100MHZ==*/
    //MCO2_PLLI2S_OUTPUT();

    /* AD9850��ʼ������*/
    Init_AD9850();

    /* DS18B20��ʼ��*/
    ds18b20_start();

    /* ADC��ʼ��*/
    ELECONDUCTIVITY_ADC_Init();
    
    /* LCD��ʼ��*/
    LCD_ALL_INIT();
    
    /* ��ʼ��������ʱ����ʱ��1s����һ���ж� */
    TIMx_Configuration();
    
    /* �����жϺ�����ʼ��*/
    EXTI_Key_Config();
    
    /* SD����д��ʼ������ */
    FatFs_Init();
    
    /* ��������ʼ��*/
    GTP_Init_Panel();
    
    
    
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
