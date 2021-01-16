#include "qi51arduino.h"
#include "UART.h"

#define GPIO_KEY P0

uint KeyValue; //������Ŷ�ȡ���ļ�ֵ

void Keypros();
void KeyDown();


void setup()
{
    Serial_begin(253);
}

void loop()
{
    KeyDown();
    
}

void Keypros()
{
    if (pin28 == 0)
    {
        delay(10); //��������
        if (pin28 == 0)
        {
        }
        while (!pin28)
            ;
    }

    if (pin27 == 0)
    {
        delay(10); //��������
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
	if(GPIO_KEY!=0x0f)//��ȡ�����Ƿ���
	{
		delaywhile(1000);//��ʱ10ms��������
		if(GPIO_KEY!=0x0f)//�ٴμ������Ƿ���
		{	
			//������
			GPIO_KEY=0X0F;
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
			}
			//������
			GPIO_KEY=0XF0;
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
			}
			while((a<50)&&(GPIO_KEY!=0xf0))	 //��ⰴ�����ּ��
			{
				delaywhile(1000);
				a++;
			}
		}
	}
}