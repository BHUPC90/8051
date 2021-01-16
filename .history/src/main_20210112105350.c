#include "qi51arduino.h"

#define GPIO_KEY P0

uint KeyValue; //������Ŷ�ȡ���ļ�ֵ

void Keypros();
void KeyDown();

void setup()
{
}

void loop()
{
    Keypros();
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
    char a = 0;
    GPIO_KEY = 0x0f;
    if (GPIO_KEY != 0x0f) //��ȡ�����Ƿ���
    {
        delay(1000);          //��ʱ10ms��������
        if (GPIO_KEY != 0x0f) //�ٴμ������Ƿ��� 0000 1111
        {
            //������
            GPIO_KEY = 0X0F; // 0000 1111
            switch (GPIO_KEY)
            {
            case (0X07):  //0000 0111
                KeyValue = 0;
                break;
            case (0X0b):  //0000 1011
                KeyValue = 1;
                break;
            case (0X0d):  //0000 1101
                KeyValue = 2;
                break;
            case (0X0e):  //0000 1110
                KeyValue = 3;
                break;
            }
            //������
            GPIO_KEY = 0XF0;  // 1111 0000
            switch (GPIO_KEY)
            {
            case (0X70):  //0111 0000
                KeyValue = KeyValue;
                break;
            case (0Xb0):  //1011 0000
                KeyValue = KeyValue + 4;
                break;
            case (0Xd0):  //1101 0000
                KeyValue = KeyValue + 8;
                break;
            case (0Xe0):  //1110 0000
                KeyValue = KeyValue + 12;
                break;
            }
            while ((a < 50) && (GPIO_KEY != 0xf0)) //��ⰴ�����ּ��
            {
                delay(1);
                a++;
            }
        }
    }
}