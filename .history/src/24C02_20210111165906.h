#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif

sbit sda = P2 ^ 0;
sbit scl = P2 ^ 1;

void iic_start() //�����ź�
{
    sda = 1;
    scl = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_();//sda��sclͬΪ�ߵ�ƽ����4.7us����
    
    _nop_(); //1.085us,��5.78us,����sda=0���½���,���ܼ�������ʱʱ����
    sda = 0; //�½���

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda�͵�ƽ����4us����        ,������4.34us����Ҫ��
}

void iic_stop() //ֹͣ�ź�
{
    sda = 0;
    _nop_(); //׼��״̬
    scl = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //��״̬�ȶ�ʱ��Ҫ�󱣳�4us����

    sda = 1; //scl�ߵ�ƽ�ڼ�,sda��һ��������

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda����4.7us����,4.34���Ϻ�������ʱ�����4.7us
             //ע:��ʱscl��sda��Ϊ1
}