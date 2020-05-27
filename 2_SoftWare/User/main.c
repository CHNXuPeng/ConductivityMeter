/*  =========================Including Files================  */
#include "stm32f4xx.h"
#include "bsp.h"
/*====================用户自定义=======================*/

/*=========================================================*/
static void Delay(__IO uint32_t nCount);
/*======宏定义======*/
#define DEVICE_RUNNING (1)
#define DEVICE_STOPPED (0)



/*变量定义*/
unsigned long Running_Time    =   0;                  //程序运行时间
unsigned int  DeviceStatus    =   DEVICE_STOPPED;     //程序运行状态
unsigned int  IsWriteToSD     =   0;                  //是否已写入
unsigned int  StirSpeed       =   0;                  //搅拌速度
double        MachVoltage     =   0.0;                //加工电压
float         MachGap         =   0.0;                /*加工间隙*/
unsigned int  IsTouch         =   0;                  //屏幕是否被按下
unsigned int  ExperNum        =   0;                  //实验编号
//unsigned int IsCreateFile     =   0;                  //是否将数据写入SD文件

char BugBugBug[50];                                   //记录程序运行过程中出现的问题


/*====================================MAIN函数====================================*/
int main(void)
{


    /*BSP初始化*/
    BSP_Init();

    /*设置状态灯*/
    LED_RED;

    while (1)
    {

      /*实时显示函数*/
      LCD_Display();

      /*向SD卡发送实验数据*/
      SendDataToSD();


      Delay(0xffffff);
    }



}

/*********************************************END OF FILE**********************/

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
    for(; nCount != 0; nCount--);
}

