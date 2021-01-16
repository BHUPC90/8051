#include "qi51arduino.h"

#define GPIO_KEY P0

k1 = P2 ^ 7;
k2 = P2 ^ 6;
k3 = P2 ^ 5;
k4 = P2 ^ 4;
k5 = P2 ^ 3;
k6 = P2 ^ 2;
k7 = P2 ^ 1;
k8 = P2 ^ 0;


void setup()
{
}

void loop()
{
    Keypros();
}

void Keypros()
{
    if (k1 == 0)
    {
        delay(10); //消抖处理
        if (k1 == 0)
        {
           
        }
        while (!k1)
            ;
    }

    if (k2 == 0)
    {
        delay(10); //消抖处理
        if (k2 == 0)
        {
            
        }
        while (!k2)
            ;
    }
}


void KeyDown(void)
{
	char a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)//读取按键是否按下
	{
		delay(1000);//延时10ms进行消抖
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
				delay(1000);
				a++;
			}
		}
	}
}