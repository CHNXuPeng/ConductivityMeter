/**
  ******************************************************************************
  * @file    bsp_bsp_adc.c
  * @author  XuPeng
  * @version V1.1
  * @date    2020-1-13
  * @brief   adc驱动

  ******************************************************************************
  */ 
/*====================头文件声明 =====================*/
#include "bsp_adc.h"
#include "bsp_ds18b20.h"

__IO uint16_t ADC_ConvertedValue[ELECONDUCTIVITY_ADC_NOFCHANEL]={0};

static void ELECONDUCTIVITY_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	/*=====================通道1======================*/	
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(POOL_ADC_GPIO_CLK1,ENABLE);		
	// 配置 IO
  GPIO_InitStructure.GPIO_Pin = POOL_ADC_GPIO_PIN1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //不上拉不下拉	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(POOL_ADC_GPIO_PORT1, &GPIO_InitStructure);

	/*=====================通道2======================*/
	// 使能 GPIO 时钟
	RCC_AHB1PeriphClockCmd(RESISTOR_ADC_GPIO_CLK2,ENABLE);		
	// 配置 IO
  GPIO_InitStructure.GPIO_Pin = RESISTOR_ADC_GPIO_PIN2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //不上拉不下拉	
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(RESISTOR_ADC_GPIO_PORT2, &GPIO_InitStructure);	

}

static void ELECONDUCTIVITY_ADC_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
  // ------------------DMA Init 结构体参数 初始化--------------------------
  // ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(ELECONDUCTIVITY_ADC_DMA_CLK, ENABLE); 
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ELECONDUCTIVITY_ADC_DR_ADDR;	
  // 存储器地址，实际上就是一个内部SRAM的变量	
	DMA_InitStructure.DMA_Memory0BaseAddr = (u32)ADC_ConvertedValue;  
  // 数据传输方向为外设到存储器	
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	
	// 缓冲区大小为，指一次传输的数据量
	DMA_InitStructure.DMA_BufferSize = ELECONDUCTIVITY_ADC_NOFCHANEL;	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  // 存储器地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
  // // 外设数据大小为半字，即两个字节 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; 
  //	存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;  
  // FIFO 大小，FIFO模式禁止时，这个不用配置	
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
	// 选择 DMA 通道，通道存在于流中
  DMA_InitStructure.DMA_Channel = ELECONDUCTIVITY_ADC_DMA_CHANNEL; 
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
	DMA_Init(ELECONDUCTIVITY_ADC_DMA_STREAM, &DMA_InitStructure);
	// 使能DMA流
  DMA_Cmd(ELECONDUCTIVITY_ADC_DMA_STREAM, ENABLE);
	
	// 开启ADC时钟
	RCC_APB2PeriphClockCmd(ELECONDUCTIVITY_ADC_CLK , ENABLE);
  // -------------------ADC Common 结构体 参数 初始化------------------------
	// 独立ADC模式
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  // 时钟为fpclk x分频	
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
  // 禁止DMA直接访问模式	
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  // 采样时间间隔	
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;  
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  // -------------------ADC Init 结构体 参数 初始化--------------------------
	ADC_StructInit(&ADC_InitStructure);
  // ADC 分辨率
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  // 扫描模式，多通道采集需要	
  ADC_InitStructure.ADC_ScanConvMode = ENABLE; 
  // 连续转换	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
  //禁止外部边沿触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  //外部触发通道，本例子使用软件触发，此值随便赋值即可
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  //数据右对齐	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  //转换通道 1个
  ADC_InitStructure.ADC_NbrOfConversion = ELECONDUCTIVITY_ADC_NOFCHANEL;                                    
  ADC_Init(ELECONDUCTIVITY_ADC, &ADC_InitStructure);
  //---------------------------------------------------------------------------
	
  // 配置 ADC 通道转换顺序和采样时间周期
  ADC_RegularChannelConfig(ELECONDUCTIVITY_ADC, POOL_ADC_CHANNEL1, 1, 
	                         ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ELECONDUCTIVITY_ADC, RESISTOR_ADC_CHANNEL2, 2, 
	                         ADC_SampleTime_3Cycles); 

  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(ELECONDUCTIVITY_ADC, ENABLE);
  // 使能ADC DMA
  ADC_DMACmd(ELECONDUCTIVITY_ADC, ENABLE);
	
	// 使能ADC
  ADC_Cmd(ELECONDUCTIVITY_ADC, ENABLE);  
  //开始adc转换，软件触发
  ADC_SoftwareStartConv(ELECONDUCTIVITY_ADC);
}



void ELECONDUCTIVITY_ADC_Init(void)
{
	ELECONDUCTIVITY_ADC_GPIO_Config();
	ELECONDUCTIVITY_ADC_Mode_Config();
}




/*

  25℃电导率计算函数
  
*/
/*=========================================================*/
float EleConductivity_Value(void)
{
  double R_Pool,S_RT;
  unsigned int Tempt,S25;
  float S_EleConductivity;
  
  /*求得电导池的等效电阻*/
  R_Pool = (ADC_ConvertedValue[0]*R_Resistor)/ADC_ConvertedValue[1];
  
  /*求得电导池的实时电导率,单位s/cm*/
  S_RT = K_EleCndct/R_Pool;
  
  /*读取当前实时温度值*/
  Tempt = ds18b20_read();
  
  /*计算25℃下等效电导率，单位us/cm*/
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
    
    
  /*计算25℃下等效电导率，单位ms/cm*/ 
  
  S_EleConductivity = S25/1000;
    
    
  return S_EleConductivity;
    
     
}

































