#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif

sbit sda = P2 ^ 3;
sbit scl = P2 ^ 3;

void iic_start()  //�����ź�
{
        sda=1;
        scl=1;
        delayus();                //sda��sclͬΪ�ߵ�ƽ����4.7us����
        _nop_();                        //1.085us,��5.78us,����sda=0���½���,���ܼ�������ʱʱ����
        sda=0;                         //�½���
        delayus();                //sda�͵�ƽ����4us����        ,������4.34us����Ҫ��
}