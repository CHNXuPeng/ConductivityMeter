/*======================ͷ�ļ�����==========================*/
#include "bsp_ad9850.h"
/********************************************
�������ƣ�AD9850_Delay
��    �ܣ�AD9850��ʱ����
��    ����z - ��ʱ����
����ֵ  ����
*********************************************/
void AD9850_Delay(unsigned int z)
{
    for(;z>0;z--)
    {;}
}
/*--------------------����ģʽ-----------------------*/
#ifdef MODE_PARALLEL   
/********************************************
�������ƣ�AD9850_Reset_Parallel
��    �ܣ��ڲ���ģʽ�¸�λAD9850
��    ������
����ֵ  ����
*********************************************/
void AD9850_Reset_Parallel(void)
{
    AD9850_WCLK_CLR ;
    AD9850_FQUD_CLR ;
    //RST�ź�
    AD9850_RST_CLR ;
    AD9850_RST_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_RST_CLR ;
}
/********************************************
�������ƣ�AD9850_Write_Parallel
��    �ܣ��ڲ���ģʽ��дAD9850�Ĵ���
��    ����W0 - W0�Ĵ�����ֵ
          freq - Ƶ��ֵ
����ֵ  ����
*********************************************/
void AD9850_Write_Parallel(unsigned char W0,unsigned long freq)
{
    unsigned char wdata ;
    unsigned long  y=(unsigned long)(4294967295/AD9850_SYSTEM_COLCK / CRYSTAL_FREQ) ;
    y *= freq ;
    wdata=W0 ;  //дw0
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>24 ;  //дw1
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>16 ;  //дw2
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>8 ;  //дw3
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    wdata=y>>0 ;  //дw4
    GPIO_Write(AD9850_DATA_PORT ,wdata) ;
    AD9850_WCLK_SET ;
    AD9850_WCLK_CLR ;
    
    AD9850_FQUD_SET ;  //����ʹ��
    AD9850_Delay(0xFFFF) ;
    AD9850_FQUD_CLR ;
}
#endif /* MODE_PARALLEL */
/*------------------------����ģʽ-------------------------*/
/*---------------------------------------------------------*/
/*------------------------����ģʽ-------------------------*/
#ifdef MODE_SEIAL
/********************************************
�������ƣ�AD9850_Reset_Sreial
��    �ܣ��ڴ���ģʽ�¸�λAD9850
��    ������
����ֵ  ����
*********************************************/
void AD9850_Reset_Sreial(void)
{
    AD9850_WCLK_CLR ;
    AD9850_FQUD_CLR ;
    //RST�ź�
    AD9850_RST_CLR ;
    AD9850_RST_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_RST_CLR ;
    //WCLK�ź�
    AD9850_WCLK_CLR ;
    AD9850_WCLK_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_WCLK_CLR ;
    //FQUD�ź�
    AD9850_FQUD_CLR ;
    AD9850_FQUD_SET ;
    AD9850_Delay(0xFFFF) ;
    AD9850_FQUD_CLR ;
}
/********************************************
�������ƣ�AD9850_Write_Serial
��    �ܣ��ڴ���ģʽ��дAD9850�Ĵ���
��    ����W0 - W0�Ĵ�����ֵ
          freq - Ƶ��ֵ
����ֵ  ����
*********************************************/
void AD9850_Write_Serial(unsigned char W0,unsigned long freq)
{
    unsigned char i,wdata ;
    unsigned long  y ;
    y = (unsigned long )268435456.0/AD9850_SYSTEM_COLCK*freq ;  
    
    wdata = y>>0 ;   //дw4
    for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = y>>8 ;  //дw3
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = y>>16 ;  //дw2
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = y>>24 ;  //дw1
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    wdata = W0 ;  //дw0
     for(i=0 ;i<8 ;i++)
    {
        if(wdata & 0x01)
          AD9850_DATA_Write_1 ;
        else
          AD9850_DATA_Write_0 ;
        AD9850_WCLK_SET ;
        wdata >>= 1 ;
        AD9850_WCLK_CLR ;
    }
    
    AD9850_FQUD_SET ;  //����ʹ��
    AD9850_Delay(0xFFFF) ;
    AD9850_FQUD_CLR ;
}
#endif /* MODE_SERIAL */
/*------------------------����ģʽ-------------------------*/
/*---------------------------------------------------------*/
/********************************************
�������ƣ�Init_AD9834
��    �ܣ���ʼ��AD9834
��    ������
����ֵ  ����
*********************************************/
void Init_AD9850(void)
{
#ifdef MODE_SEIAL   //����ģʽ
    GPIO_InitTypeDef GPIO_InitStructure ;
    GPIO_InitStructure.GPIO_Pin = AD9850_WCLK | AD9850_FQUD | AD9850_RST | AD9850_DATA ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_Init(AD9850_CONTROL_PORT ,&GPIO_InitStructure) ;
    
    AD9850_Reset_Sreial() ;
#endif
    
#ifdef MODE_PARALLEL //����ģʽ
    GPIO_InitTypeDef GPIO_InitStructure ;
    GPIO_InitStructure.GPIO_Pin     = AD9850_WCLK | AD9850_FQUD | AD9850_RST ;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz ;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType   = GPIO_OType_PP;
    GPIO_Init(AD9850_CONTROL_PORT ,&GPIO_InitStructure) ;
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_All ;
    GPIO_Init(AD9850_DATA_PORT ,&GPIO_InitStructure) ;
    
    AD9850_Reset_Parallel() ;
#endif
    
}
//================================================================//
