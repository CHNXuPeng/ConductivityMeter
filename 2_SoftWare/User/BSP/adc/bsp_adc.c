/**
  ******************************************************************************
  * @file    bsp_bsp_adc.c
  * @author  XuPeng
  * @version V1.1
  * @date    2020-1-13
  * @brief   adc����

  ******************************************************************************
  */ 
/*====================ͷ�ļ����� =====================*/
#include "bsp_adc.h"
#include "bsp_ds18b20.h"

__IO uint16_t ADC_ConvertedValue[ELECONDUCTIVITY_ADC_NOFCHANEL]={0};

static void ELECONDUCTIVITY_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*=====================ͨ��1======================*/	
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(POOL_ADC_GPIO_CLK1,ENABLE);		
	// ���� IO
  GPIO_InitStructure.GPIO_Pin = POOL_ADC_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //������������	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(POOL_ADC_GPIO_PORT1, &GPIO_InitStructure);

	/*=====================ͨ��2======================*/
	// ʹ�� GPIO ʱ��
	RCC_AHB1PeriphClockCmd(RESISTOR_ADC_GPIO_CLK2,ENABLE);		
	// ���� IO
  GPIO_InitStructure.GPIO_Pin = RESISTOR_ADC_GPIO_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //������������	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(RESISTOR_ADC_GPIO_PORT2, &GPIO_InitStructure);	

}

static void ELECONDUCTIVITY_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ------------------DMA Init �ṹ����� ��ʼ��--------------------------
  // ADC1ʹ��DMA2��������0��ͨ��0��������ֲ�̶�����
  // ����DMAʱ��
  RCC_AHB1PeriphClockCmd(ELECONDUCTIVITY_ADC_DMA_CLK, ENABLE); 
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ELECONDUCTIVITY_ADC_DR_ADDR;	
  // �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  
  // ���ݴ��䷽��Ϊ���赽�洢��	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// ��������СΪ��ָһ�δ����������
	DMA_InitStructure.DMA_BufferSize = ELECONDUCTIVITY_ADC_NOFCHANEL;	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // �洢����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // �������ݴ�СΪ���֣��������ֽ� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	�洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // ��ֹDMA FIFO	��ʹ��ֱ��ģʽ
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO ��С��FIFOģʽ��ֹʱ�������������	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// ѡ�� DMA ͨ����ͨ������������
  DMA_InitStructure.DMA_Channel = ELECONDUCTIVITY_ADC_DMA_CHANNEL; 
  //��ʼ��DMA�������൱��һ����Ĺܵ����ܵ������кܶ�ͨ��
	DMA_Init(ELECONDUCTIVITY_ADC_DMA_STREAM, &DMA_InitStructure);
	// ʹ��DMA��
  DMA_Cmd(ELECONDUCTIVITY_ADC_DMA_STREAM, ENABLE);
	
	// ����ADCʱ��
	RCC_APB2PeriphClockCmd(ELECONDUCTIVITY_ADC_CLK , ENABLE);
  // -------------------ADC Common �ṹ�� ���� ��ʼ��------------------------
	// ����ADCģʽ
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // ʱ��Ϊfpclk x��Ƶ	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // ��ֹDMAֱ�ӷ���ģʽ	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // ����ʱ����	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init �ṹ�� ���� ��ʼ��--------------------------
	ADC_StructInit(&ADC_InitStructure);
  // ADC �ֱ���
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // ɨ��ģʽ����ͨ���ɼ���Ҫ	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // ����ת��	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //��ֹ�ⲿ���ش���
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //�ⲿ����ͨ����������ʹ�������������ֵ��㸳ֵ����
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //�����Ҷ���	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //ת��ͨ�� 1��
  ADC_InitStructure.ADC_NbrOfConversion = ELECONDUCTIVITY_ADC_NOFCHANEL;                                    
  ADC_Init(ELECONDUCTIVITY_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // ���� ADC ͨ��ת��˳��Ͳ���ʱ������
  ADC_RegularChannelConfig(ELECONDUCTIVITY_ADC, POOL_ADC_CHANNEL1, 1, 
	                         ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ELECONDUCTIVITY_ADC, RESISTOR_ADC_CHANNEL2, 2, 
	                         ADC_SampleTime_3Cycles); 

  // ʹ��DMA���� after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(ELECONDUCTIVITY_ADC, ENABLE);
  // ʹ��ADC DMA
  ADC_DMACmd(ELECONDUCTIVITY_ADC, ENABLE);
	
	// ʹ��ADC
  ADC_Cmd(ELECONDUCTIVITY_ADC, ENABLE);  
  //��ʼadcת�����������
  ADC_SoftwareStartConv(ELECONDUCTIVITY_ADC);
}



void ELECONDUCTIVITY_ADC_Init(void)
{
	ELECONDUCTIVITY_ADC_GPIO_Config();
	ELECONDUCTIVITY_ADC_Mode_Config();
}




/*

  25��絼�ʼ��㺯��
  
*/
/*=========================================================*/
float EleConductivity_Value(void)
{
  double R_Pool,S_RT;
  unsigned int Tempt,S25;
  float S_EleConductivity;
  
  /*��õ絼�صĵ�Ч����*/
  R_Pool = (ADC_ConvertedValue[0]*R_Resistor)/ADC_ConvertedValue[1];
  
  /*��õ絼�ص�ʵʱ�絼��,��λs/cm*/
  S_RT = K_EleCndct/R_Pool;
  
  /*��ȡ��ǰʵʱ�¶�ֵ*/
  Tempt = ds18b20_read();
  
  /*����25���µ�Ч�絼�ʣ���λus/cm*/
  if (Tempt>=1&&Tempt<=10)
    {S25 = S_RT/(0.0169*Tempt+0.5583)*1000000;}
    
  else if(Tempt>=11&&Tempt<=20)
    {S25 = S_RT/(0.0180*Tempt+0.5473)*1000000;}
    
  else if(Tempt>=11&&Tempt<=20)
    {S25 = S_RT/(0.0180*Tempt+0.5473)*1000000;}
    
  else if(Tempt>=21&&Tempt<=30)
    {S25 = S_RT/(0.0189*Tempt+0.5281)*1000000;}    
    
  else
    {S25 = S_RT/(0.02*Tempt+0.5)*1000000;}
    
    
  /*����25���µ�Ч�絼�ʣ���λms/cm*/ 
  
  S_EleConductivity = S25/1000;
    
    
  return S_EleConductivity;
    
     
}

































