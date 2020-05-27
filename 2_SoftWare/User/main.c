/*  =========================Including Files================  */
#include "stm32f4xx.h"
#include "bsp.h"
/*====================�û��Զ���=======================*/

/*=========================================================*/
static void Delay(__IO uint32_t nCount);
/*======�궨��======*/
#define DEVICE_RUNNING (1)
#define DEVICE_STOPPED (0)



/*��������*/
unsigned long Running_Time    =   0;                  //��������ʱ��
unsigned int  DeviceStatus    =   DEVICE_STOPPED;     //��������״̬
unsigned int  IsWriteToSD     =   0;                  //�Ƿ���д��
unsigned int  StirSpeed       =   0;                  //�����ٶ�
double        MachVoltage     =   0.0;                //�ӹ���ѹ
float         MachGap         =   0.0;                /*�ӹ���϶*/
unsigned int  IsTouch         =   0;                  //��Ļ�Ƿ񱻰���
unsigned int  ExperNum        =   0;                  //ʵ����
//unsigned int IsCreateFile     =   0;                  //�Ƿ�����д��SD�ļ�

char BugBugBug[50];                                   //��¼�������й����г��ֵ�����


/*====================================MAIN����====================================*/
int main(void)
{


    /*BSP��ʼ��*/
    BSP_Init();

    /*����״̬��*/
    LED_RED;

    while (1)
    {

      /*ʵʱ��ʾ����*/
      LCD_Display();

      /*��SD������ʵ������*/
      SendDataToSD();


      Delay(0xffffff);
    }



}

/*********************************************END OF FILE**********************/

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
    for(; nCount != 0; nCount--);
}

