#include "bsp_sd_write_read.h"

/*================================�������=========================================*/
int i;
FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_sd;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */

char ReadBuffer[1024]= {0};       /* �������� */
char WriteBuffer[170];            /* д������*/
char FileName[20];                //ʵ�����ݼ�¼�ļ���

extern unsigned int   IsWriteToSD;
extern unsigned long  Running_Time;
extern unsigned int   DeviceStatus;      //��������״̬
extern unsigned int   ExperNum;            //ʵ����
extern char BugBugBug [];                  //��¼�������й����г��ֵ�����
//extern  unsigned int IsCreateFile;
/*===============================================================================*/


/*��ʼ��BL8782 WiFiģ��ʹ�����ţ�������WiFiģ��*/
void BL8782_PDN_INIT(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //����WiFiģ��
}






/* �ļ�ϵͳ��ʼ������ */
void FatFs_Init(void)
{

    /* ����WiFiģ�� */
    BL8782_PDN_INIT();

    /* ��ʼ�����Դ��ڣ�һ��Ϊ����1 */
    Debug_USART_Config();


    printf("\r\n****** �絼���ǲ������ݼ�¼ ******\r\n");

    //���ⲿSPI Flash�����ļ�ϵͳ���ļ�ϵͳ����ʱ���SPI�豸��ʼ��
    res_sd = f_mount(&fs,"0:",1);

    /*----------------------- ��ʽ������ ---------------------------*/
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if(res_sd == FR_NO_FILESYSTEM)
    {
        printf("��SD����û���ļ�ϵͳ���������и�ʽ��...\r\n");
        /* ��ʽ�� */
        res_sd=f_mkfs("0:",0,0);

        if(res_sd == FR_OK)
        {
            printf("��SD���ѳɹ���ʽ���ļ�ϵͳ��\r\n");
            /* ��ʽ������ȡ������ */
            res_sd = f_mount(NULL,"0:",1);
            /* ���¹���	*/
            res_sd = f_mount(&fs,"0:",1);
        }
        else
        {
            LED_RED;
            printf("������ʽ��ʧ�ܡ�����\r\n");
            while(1);
        }
    }
    else if(res_sd!=FR_OK)
    {
        printf("����SD�������ļ�ϵͳʧ�ܡ�(%d)\r\n",res_sd);
        printf("��������ԭ��SD����ʼ�����ɹ���\r\n");
        //sprintf(BugBugBug, "SD�������ļ�ϵͳʧ��");     //��¼BUG��Ϣ
        while(1);
    }
    else
    {
        printf("���ļ�ϵͳ���سɹ������Խ��ж�д����\r\n\n");
    }

}




/*�ļ�д����*/
void Record_Write(void)
{
    static unsigned int IsCreateFile = 0;   //��־�Ƿ��Ѵ����ļ�
    /*----------------------- �ļ�ϵͳ���ԣ�д���� -----------------------------*/
    /* ���ļ�������ļ��������򴴽��� */
    //printf("\r\n****** �����������ݼ�¼... ******\r\n");

    sprintf(FileName, "0:ʵ��%d�絼�����ݼ�¼.txt", ExperNum);   //��ֵ�ļ���

    /*������¼�ļ�*/
    //res_sd = f_open(&fnew, "0:�絼�����ݼ�¼.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ((DeviceStatus == DEVICE_RUNNING) && IsCreateFile == 0)   //�������������״̬�һ�δ�����ļ����ʹ����ļ�
    {
        /*������¼�ļ�*/
        res_sd = f_open(&fnew, FileName, FA_CREATE_ALWAYS );
        IsCreateFile = 1;         //���Ϊ�Ѵ���
        /* ���ٶ�д���ر��ļ� */
        f_close(&fnew);
    }

    if (IsCreateFile == 1)   //����ļ��Ѵ���
    {
        /*���ļ�������ģʽ*/
        res_sd = f_open(&fnew, FileName, FA_OPEN_EXISTING | FA_WRITE );
        //res_sd = f_open(&fnew, FileName,FA_OPEN_EXISTING | FA_WRITE );

        f_lseek (&fnew, sizeof(WriteBuffer) * (Running_Time / 60));     //�ļ�ָ��ƫ��һ����¼�������
    }

    if ( res_sd == FR_OK )
    {
        // printf("����/�����絼�����ݼ�¼.txt�ļ��ɹ�����ʼ��¼���ݡ�\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if (res_sd==FR_OK)
        {
            printf("������д��ɹ���д���ֽڣ�%d\n",fnum);
            printf("�� \n%s\r\n\n\n",WriteBuffer);
        }
        else
        {
            printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);
            sprintf(BugBugBug, "�ļ�д��ʧ��");     //��¼BUG��Ϣ
        }
        /* ���ٶ�д���ر��ļ� */
        f_close(&fnew);
    }
    else
    {
        LED_RED;
        printf("������/�����ļ�ʧ�ܡ�\r\n");
        sprintf(BugBugBug, "ʵ��%d��/�����ļ�ʧ��", ExperNum);       //��¼BUG��Ϣ
    }
}



/*�ļ�������*/
void Record_Read(void)
{
    /*------------------- �ļ�ϵͳ���ԣ������� ------------------------------------*/
    //printf("****** ���������ļ���ȡ����... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs��д�����ļ�.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        LED_GREEN;
        //printf("�����ļ��ɹ���\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            printf("�����ݶ�ȡ�ɹ�,�����ֽڣ�%d\r\n",fnum);
            printf("������Ϊ��\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            printf("�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);
        }
    }
    else
    {
        LED_RED;
        printf("�������ļ�ʧ�ܡ�\r\n");
    }
}




void FatFs_DeInit(void)
{
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);

    /* ����ʹ���ļ�ϵͳ��ȡ�������ļ�ϵͳ */
    f_mount(NULL,"0:",1);


}



/*��SD������ʵ������*/
void SendDataToSD(void)
{
    /*ÿ����д��һ��ʵ������*/
    if ((Running_Time % 60 == 1) && IsWriteToSD == 0)
    {
        Record_Write();
        IsWriteToSD = 1;
        //Record_Read();
    }
    if (Running_Time % 60 == 2)
        IsWriteToSD = 0;
}

