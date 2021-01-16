#include "qi51arduino.h"

#define GPIO_KEY P0
#define GPIO_KEY_2 P2

uint KeyValue; //������Ŷ�ȡ���ļ�ֵ

void Keypros();
void KeyDown();


void UsartInit()
{
    SCON = 0X50; //����Ϊ������ʽ1
    TMOD = 0X20; //���ü�����������ʽ2
    PCON = 0X80; //�����ʼӱ�
    TH1 = 0XF3;  //��������ʼֵ���ã�ע�Ⲩ������9600��
    TL1 = 0XF3;
    ES = 1;  //�򿪽����ж�
    EA = 1;  //�����ж�
    TR1 = 1; //�򿪼�����
}

void setup()
{
    UsartInit(); //	���ڳ�ʼ��
}

void loop()
{
    KeyDown1();
    KeyDown2();
}

void Keypros()
{
    if (pin28 == 0)
    {
        delay(10); //��������
        if (pin28 == 0)
        {
            SBUF = '1';
            while (TI == 0)
                ;                                  //�ȴ�����
            TI = 0;
        }
        while (!pin28)
            ;
    }

    if (pin27 == 0)
    {
        delay(10); //��������
        if (pin27 == 0)
        {
            SBUF = '2';
            while (TI == 0)
                ;                                  //�ȴ�����
            TI = 0;
        }
        while (!pin27)
            ;
    }

}

void KeyDown1()
{
    char a = 0;
    GPIO_KEY = 0x0f;
    if (GPIO_KEY != 0x0f) //��ȡ�����Ƿ���
    {
        delaywhile(1000);     //��ʱ10ms��������
        if (GPIO_KEY != 0x0f) //�ٴμ������Ƿ���
        {
            //������
            GPIO_KEY = 0X0F;
            switch (GPIO_KEY)
            {
            case (0X07):
                KeyValue = 0;
                break;
            case (0X0b):
                KeyValue = 1;
                break;
            case (0X0d):
                KeyValue = 2;
                break;
            case (0X0e):
                KeyValue = 3;
                break;
            }
            //������
            GPIO_KEY = 0XF0;
            switch (GPIO_KEY)
            {
            case (0X70):
                KeyValue = KeyValue;
                break;
            case (0Xb0):
                KeyValue = KeyValue + 4;
                break;
            case (0Xd0):
                KeyValue = KeyValue + 8;
                break;
            case (0Xe0):
                KeyValue = KeyValue + 12;
                break;
            }
            SBUF = KeyValue;
            while (TI == 0)
                ;                                  //�ȴ�����
            TI = 0;                                //������Ϻ�������ͱ�ʶ
            while ((a < 50) && (GPIO_KEY != 0xf0)) //��ⰴ�����ּ��
            {
                delaywhile(1000);
                a++;
            }
        }
    }
}

void KeyDown1()
{
    char a = 0;
    GPIO_KEY_2 = 0x0f;
    if (GPIO_KEY_2 != 0x0f) //��ȡ�����Ƿ���
    {
        delaywhile(1000);     //��ʱ10ms��������
        if (GPIO_KEY_2 != 0x0f) //�ٴμ������Ƿ���
        {
            //������
            GPIO_KEY_2 = 0X0F;
            switch (GPIO_KEY_2)
            {
            case (0X07):
                KeyValue = 0;
                break;
            case (0X0b):
                KeyValue = 1;
                break;
            case (0X0d):
                KeyValue = 2;
                break;
            case (0X0e):
                KeyValue = 3;
                break;
            }
            //������
            GPIO_KEY_2 = 0XF0;
            switch (GPIO_KEY_2)
            {
            case (0X70):
                KeyValue = KeyValue;
                break;
            case (0Xb0):
                KeyValue = KeyValue + 4;
                break;
            case (0Xd0):
                KeyValue = KeyValue + 8;
                break;
            case (0Xe0):
                KeyValue = KeyValue + 12;
                break;
            }
            SBUF = '0' + KeyValue;
            while (TI == 0)
                ;                                  //�ȴ�����
            TI = 0;                                //������Ϻ�������ͱ�ʶ
            while ((a < 50) && (GPIO_KEY_2 != 0xf0)) //��ⰴ�����ּ��
            {
                delaywhile(1000);
                a++;
            }
        }
    }
}