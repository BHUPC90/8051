#include "qi51arduino.h"

int mod = 0;

uint wait_timer;
void cshtimer() //1000��1���루ms�����65.536����
{
    TMOD = 0x01; //����ģʽ1��16λ��ʱ �� 8��8��
    //11111111 11111111 ת10���� 65,535 ��1�õ� 65536 , 256 = 2�İ˴���
    TH0 = (65536 - wait_timer) / 256;
    TL0 = (65536 - wait_timer) % 256;
    EA = 1;  //���ж�
    ET0 = 1; //�򿪶�ʱ��0�ж�
    TR0 = 1; //���붨ʱ������
}

void setup()
{
    wait_timer = 50000;
    cshtimer();
}

void loop()
{
    pin1 = mod;
}

void timervoid() interrupt 1
{
    TH0 = (65536 - wait_timer) / 256;
    TL0 = (65536 - wait_timer) % 256;
    mod = !mod;
}