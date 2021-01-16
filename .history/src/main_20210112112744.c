#include "qi51arduino.h"

#define GPIO_KEY P1
#define GPIO_DIG P0

uint KeyValue; //用来存放读取到的键值

void Keypros();
void KeyDown();

uchar code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//显示0~F的值

void UsartInit()
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;				//计数器初始值设置，注意波特率是4800的
	TL1=0XF3;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}

void setup()
{
    UsartInit();  //	串口初始化
}

void loop()
{
    KeyDown();
    GPIO_DIG=~smgduan[KeyValue];	  //
    Serial_print_byte(KeyValue);
    
}

void Keypros()
{
    if (pin28 == 0)
    {
        delay(10); //消抖处理
        if (pin28 == 0)
        {
        }
        while (!pin28)
            ;
    }

    if (pin27 == 0)
    {
        delay(10); //消抖处理
        if (pin27 == 0)
        {
        }
        while (!pin27)
            ;
    }
}

void KeyDown()
{
    char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//读取按键是否按下
	{
		delaywhile(1000);//延时10ms进行消抖
		if(GPIO_KEY!=0x0f)//再次检测键盘是否按下
		{	
			//测试列
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//测试行
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
            
			while((a<50)&&(GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				delaywhile(1000);
				a++;
			}
		}
	}
}