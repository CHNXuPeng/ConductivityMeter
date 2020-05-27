/*
只要调用一次ds18b20_start()初始化DS18B20，
然后每次读温度时直接调用 ds18b20_read()就可以了。
如：
ds18b20_start()； 
 while(1)
 { 
    for(i=1000000;i>0;i--);
    val = ds18b20_read();
 }
  
  *****测温范围-55℃~+125℃，在-10~+85℃时精度为±0.5℃
  
  ***精度配置寄存器 1f=9位; 3f=10位; 5f=11位; 7f=12位;
*/


/*========================头文件声明==========================*/
#include "bsp_ds18b20.h"

/*========================函数=========================*/
/*
static unsigned char TempX_TAB[16]= {0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,
                                     0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09
                                    };
*/

void Delay_us(u32 Nus)
{
    SysTick->LOAD=Nus*9;          //时间加载
    SysTick->CTRL|=0x01;             //开始倒数
    while(!(SysTick->CTRL&(1<<16))); //等待时间到达
    SysTick->CTRL=0X00000000;        //关闭计数器
    SysTick->VAL=0X00000000;         //清空计数器
}



unsigned char ResetDS18B20(void)
{
    unsigned char resport;
    SetDQ();
    Delay_us(50);

    ResetDQ();
    Delay_us(500);  //500us （该时间的时间范围可以从480到960微秒）
    SetDQ();
    Delay_us(40);  //40us
//resport = GetDQ();
    while(GetDQ());
    Delay_us(500);  //500us
    SetDQ();
    return resport;
}

void DS18B20WriteByte(unsigned char Dat)
{
    unsigned char i;
    for(i=8; i>0; i--)
    {
        ResetDQ();     //在15u内送数到数据线上，DS18B20在15-60u读数
        Delay_us(5);    //5us
        if(Dat & 0x01)
            SetDQ();
        else
            ResetDQ();
        Delay_us(65);    //65us
        SetDQ();
        Delay_us(2);    //连续两位间应大于1us
        Dat >>= 1;
    }
}


unsigned char DS18B20ReadByte(void)
{
    unsigned char i,Dat;
    SetDQ();
    Delay_us(5);
    for(i=8; i>0; i--)
    {
        Dat >>= 1;
        ResetDQ();     //从读时序开始到采样信号线必须在15u内，且采样尽量安排在15u的最后
        Delay_us(5);   //5us
        SetDQ();
        Delay_us(5);   //5us
        if(GetDQ())
            Dat|=0x80;
        else
            Dat&=0x7f;
        Delay_us(65);   //65us
        SetDQ();
    }
    return Dat;
}


void ReadRom(unsigned char *Read_Addr)
{
    unsigned char i;

    DS18B20WriteByte(ReadROM);

    for(i=8; i>0; i--)
    {
        *Read_Addr=DS18B20ReadByte();
        Read_Addr++;
    }
}


void DS18B20Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)
{
    DisableINT();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(WriteScratchpad);
    DS18B20WriteByte(AlarmTL);
    DS18B20WriteByte(AlarmTH);
    DS18B20WriteByte(Precision);

    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(CopyScratchpad);
    EnableINT();

    while(!GetDQ());  //等待复制完成 ///////////
}


void DS18B20StartConvert(void)
{
    DisableINT();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(StartConvert);
    EnableINT();
}

void DS18B20_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(DS_RCC_PORT, ENABLE);

    GPIO_InitStructure.GPIO_Pin      = DS_DQIO;
    GPIO_InitStructure.GPIO_Mode     = GPIO_Mode_OUT;    //输出模式
    GPIO_InitStructure.GPIO_OType    = GPIO_OType_OD;    //开漏输出
    GPIO_InitStructure.GPIO_Speed    = GPIO_Speed_50MHz; //2M时钟速度
    GPIO_Init(DS_PORT, &GPIO_InitStructure);
}


void ds18b20_start(void)
{
    DS18B20_Configuration();
    DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);
    DS18B20StartConvert();
}


/*此处读取的温度没有经过分辨率转换*/
double ds18b20_read(void)
{
    unsigned char TemperatureL,TemperatureH;
    unsigned int  Temperature;
    double Tempt;
    

    DisableINT();
    ResetDS18B20();
    DS18B20WriteByte(SkipROM);
    DS18B20WriteByte(ReadScratchpad);
    TemperatureL=DS18B20ReadByte();
    TemperatureH=DS18B20ReadByte();
    ResetDS18B20();
    EnableINT();
/*
    if(TemperatureH & 0x80)
    {
        TemperatureH=(~TemperatureH) | 0x08;
        TemperatureL=~TemperatureL+1;
        if(TemperatureL==0)
            TemperatureH+=1;
    }

    TemperatureH=(TemperatureH<<4)+((TemperatureL&0xf0)>>4);
    TemperatureL=TempX_TAB[TemperatureL&0x0f];

//bit0-bit7为小数位，bit8-bit14为整数位，bit15为正负位
    Temperature=TemperatureH;
    Temperature=(Temperature<<8) | TemperatureL;

    
*/
    Temperature = TemperatureH;
    Temperature <<= 8;
    Temperature |= TemperatureL;
    
    DS18B20StartConvert();
    
    Tempt = (double)Temperature * 0.0625;
    
    return  Tempt;
}


