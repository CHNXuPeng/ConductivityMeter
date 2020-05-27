#include "bsp_sd_write_read.h"

/*================================定义变量=========================================*/
int i;
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */

char ReadBuffer[1024]= {0};       /* 读缓冲区 */
char WriteBuffer[170];            /* 写缓冲区*/
char FileName[20];                //实验数据记录文件名

extern unsigned int   IsWriteToSD;
extern unsigned long  Running_Time;
extern unsigned int   DeviceStatus;      //程序运行状态
extern unsigned int   ExperNum;            //实验编号
extern char BugBugBug [];                  //记录程序运行过程中出现的问题
//extern  unsigned int IsCreateFile;
/*===============================================================================*/


/*初始化BL8782 WiFi模块使能引脚，并禁用WiFi模块*/
void BL8782_PDN_INIT(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOB,GPIO_Pin_13);  //禁用WiFi模块
}






/* 文件系统初始化函数 */
void FatFs_Init(void)
{

    /* 禁用WiFi模块 */
    BL8782_PDN_INIT();

    /* 初始化调试串口，一般为串口1 */
    Debug_USART_Config();


    printf("\r\n****** 电导率仪测量数据记录 ******\r\n");

    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
    res_sd = f_mount(&fs,"0:",1);

    /*----------------------- 格式化测试 ---------------------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if(res_sd == FR_NO_FILESYSTEM)
    {
        printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
        /* 格式化 */
        res_sd=f_mkfs("0:",0,0);

        if(res_sd == FR_OK)
        {
            printf("》SD卡已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res_sd = f_mount(NULL,"0:",1);
            /* 重新挂载	*/
            res_sd = f_mount(&fs,"0:",1);
        }
        else
        {
            LED_RED;
            printf("《《格式化失败。》》\r\n");
            while(1);
        }
    }
    else if(res_sd!=FR_OK)
    {
        printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
        printf("！！可能原因：SD卡初始化不成功。\r\n");
        //sprintf(BugBugBug, "SD卡挂载文件系统失败");     //记录BUG信息
        while(1);
    }
    else
    {
        printf("》文件系统挂载成功，可以进行读写测试\r\n\n");
    }

}




/*文件写操作*/
void Record_Write(void)
{
    static unsigned int IsCreateFile = 0;   //标志是否已创建文件
    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    //printf("\r\n****** 即将进行数据记录... ******\r\n");

    sprintf(FileName, "0:实验%d电导率数据记录.txt", ExperNum);   //赋值文件名

    /*创建记录文件*/
    //res_sd = f_open(&fnew, "0:电导率数据记录.txt",FA_CREATE_ALWAYS | FA_WRITE );
    if ((DeviceStatus == DEVICE_RUNNING) && IsCreateFile == 0)   //如果程序处于运行状态且还未创建文件，就创建文件
    {
        /*创建记录文件*/
        res_sd = f_open(&fnew, FileName, FA_CREATE_ALWAYS );
        IsCreateFile = 1;         //标记为已创建
        /* 不再读写，关闭文件 */
        f_close(&fnew);
    }

    if (IsCreateFile == 1)   //如果文件已创建
    {
        /*打开文件，设置模式*/
        res_sd = f_open(&fnew, FileName, FA_OPEN_EXISTING | FA_WRITE );
        //res_sd = f_open(&fnew, FileName,FA_OPEN_EXISTING | FA_WRITE );

        f_lseek (&fnew, sizeof(WriteBuffer) * (Running_Time / 60));     //文件指针偏移一个记录数组的量
    }

    if ( res_sd == FR_OK )
    {
        // printf("》打开/创建电导率数据记录.txt文件成功，开始记录数据。\r\n");
        /* 将指定存储区内容写入到文件内 */
        res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        if (res_sd==FR_OK)
        {
            printf("》数据写入成功，写入字节：%d\n",fnum);
            printf("》 \n%s\r\n\n\n",WriteBuffer);
        }
        else
        {
            printf("！！文件写入失败：(%d)\n",res_sd);
            sprintf(BugBugBug, "文件写入失败");     //记录BUG信息
        }
        /* 不再读写，关闭文件 */
        f_close(&fnew);
    }
    else
    {
        LED_RED;
        printf("！！打开/创建文件失败。\r\n");
        sprintf(BugBugBug, "实验%d打开/创建文件失败", ExperNum);       //记录BUG信息
    }
}



/*文件读操作*/
void Record_Read(void)
{
    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    //printf("****** 即将进行文件读取测试... ******\r\n");
    res_sd = f_open(&fnew, "0:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ);
    if(res_sd == FR_OK)
    {
        LED_GREEN;
        //printf("》打开文件成功。\r\n");
        res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum);
        if(res_sd==FR_OK)
        {
            printf("》数据读取成功,读到字节：%d\r\n",fnum);
            printf("》数据为：\r\n%s \r\n", ReadBuffer);
        }
        else
        {
            printf("！！文件读取失败：(%d)\n",res_sd);
        }
    }
    else
    {
        LED_RED;
        printf("！！打开文件失败。\r\n");
    }
}




void FatFs_DeInit(void)
{
    /* 不再读写，关闭文件 */
    f_close(&fnew);

    /* 不再使用文件系统，取消挂载文件系统 */
    f_mount(NULL,"0:",1);


}



/*向SD卡发送实验数据*/
void SendDataToSD(void)
{
    /*每分钟写入一次实验数据*/
    if ((Running_Time % 60 == 1) && IsWriteToSD == 0)
    {
        Record_Write();
        IsWriteToSD = 1;
        //Record_Read();
    }
    if (Running_Time % 60 == 2)
        IsWriteToSD = 0;
}

