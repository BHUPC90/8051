#include "qi51arduino.h"
//�ж�ʵ��

int mod = 0;

void setup()
{
    pin1 = 1; //��ʼ��LED��
    pin2 = 1; //��ʼ��LED��
    pin3 = 1; //��ʼ��LED��
    pin4 = 1; //��ʼ��LED��

    EA = 1; //����CPU�����ж�

    EX0 = 1; //����EX0�����жϣ��ⲿ�ж�0,P3.2��
    IT0 = 1; // �����жϴ�����ʽ 0����ʱ�䴥����1��ƽ�任ʱ�������ʺϰ�ť

    EX1 = 1; //����EX1�����жϣ��ⲿ�ж�0,P3.2��
    IT1 = 1; // �����жϴ�����ʽ 0����ʱ�䴥����1��ƽ�任ʱ�������ʺϰ�ť
    
}

void loop()
{
    while (mod == 0)
    {
        pin1 = 1;
        pin2 = 1;
        pin3 = 1;
        pin4 = 0;
        delay(1000);
        if (mod)
            break;
        pin1 = 1;
        pin2 = 1;
        pin3 = 0;
        pin4 = 1;
        delay(1000);
        if (mod)
            break;
        pin1 = 1;
        pin2 = 0;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod)
            break;
        pin1 = 0;
        pin2 = 1;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod)
            break;
    }
    while (mod)
    {
        if (mod == 0)
            break;
        pin1 = 0;
        pin2 = 1;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod == 0)
            break;
        pin1 = 1;
        pin2 = 0;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod == 0)
            break;
        pin1 = 1;
        pin2 = 1;
        pin3 = 0;
        pin4 = 1;
        delay(1000);
        if (mod == 0)
            break;
        pin1 = 1;
        pin2 = 1;
        pin3 = 1;
        pin4 = 0;
        delay(1000);
    }
}

//�ⲿ�ж˳���ע���пӣ�0��EX0�ⲿ�ж�0��2��EX1�ⲿ�ж�һ
void zhongduan0() interrupt 0
{
    mod = 0;
}
void zhongduan1() interrupt 2
{
    mod = 1;
}