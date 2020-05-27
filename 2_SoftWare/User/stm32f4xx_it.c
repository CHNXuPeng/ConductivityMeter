/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    06-March-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_basic_tim.h"
#include "bsp_i2c_touch.h"
#include "bsp_key_exti.h"

/*======宏定义======*/
#define DEVICE_RUNNING (1)
#define DEVICE_STOPPED (0)


/*变量定义*/
extern unsigned long Running_Time;                //程序运行时间
extern unsigned int DeviceStatus;               //程序运行状态
extern unsigned int  StirSpeed;                   //搅拌速度
extern double        MachVoltage;                 //加工电压
extern float         MachGap;                     //加工间隙



extern void GTP_TouchProcess(void);
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */



/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
* @brief  This function handles TIM interrupt request.
* @param  None
* @retval None
*/

/*基本定时器中断处理函数*/
void  BASIC_TIM_IRQHandler (void)
{
    if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET )
    {
        if (DeviceStatus == DEVICE_RUNNING)
            Running_Time++;
        TIM_ClearITPendingBit(BASIC_TIM, TIM_IT_Update);
    }
}

/*触摸中断处理函数*/
void GTP_IRQHandler(void)
{
	if(EXTI_GetITStatus(GTP_INT_EXTI_LINE) != RESET) //确保是否产生了EXTI Line中断
	{
    GTP_TouchProcess();    
		EXTI_ClearITPendingBit(GTP_INT_EXTI_LINE);     //清除中断标志位
	}  
}


/*按键1（暂停/继续）中断处理函数*/
void KEY1_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {
        if (DeviceStatus == DEVICE_RUNNING)        //如果程序处于运行状态，则将其停止
        {
            DeviceStatus = DEVICE_STOPPED;
            LED_RED;
        }
        else if (DeviceStatus == DEVICE_STOPPED)   //如果程序处于停止状态，则设置为运行
        {
            DeviceStatus = DEVICE_RUNNING;
            LED_GREEN;
        }
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}




/*按键1（暂停/继续）中断处理函数*/
/*
void KEY1_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE) != RESET)
    {
       if(DeviceStatus == DEVICE_STOPPED)
       {
          if (StirSpeed <= 3500)
              StirSpeed += 500;  
       }
       else if (DeviceStatus == DEVICE_RUNNING)
       {
          if (MachVoltage <= 30.0)
              MachVoltage += 0.5;
       }
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);
    }
}
*/
/*按键2（开始/停止）中断处理函数*/
/*
void KEY2_IRQHandler(void)
{
    //确保是否产生了EXTI Line中断
    if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE) != RESET)
    {
        if(DeviceStatus == DEVICE_STOPPED)
       {
          if (StirSpeed >= 1)
              StirSpeed -= 500;  
       }
       else if (DeviceStatus == DEVICE_RUNNING)
       {
          if (MachVoltage >= 0.0)
              MachVoltage -= 0.5;
       }
        //清除中断标志位
        EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);
    }
}
*/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
