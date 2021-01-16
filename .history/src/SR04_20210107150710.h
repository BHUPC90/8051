#ifndef _INC_SR04
#define _INC_SR04
#define uchar unsigned char
#define uint unsigned int
#endif
//���Ծ��� = ( �ߵ�ƽʱ�� * ����(340M/S) ) / 2;

sbit ECHO_port = P2 ^ 4;
sbit TRIG_port = P2 ^ 3;

void delay(unsigned int wait_time);
void delaywhile(unsigned int wait_time); //1ms ??

void StartModule() //����ģ��
{
    TRIG_port = 1; //����һ��ģ��
    delaywhile(2);
    TRIG_port = 0;
}

void initDate9600()
{
    SCON = 0x50; //0101 0000 
    TMOD = 0x20; //0010 0000
    TH1 = 0xFD;  //
    TL1 = 0xFD;  //
    TR1 = 1;     //
    ES = 1;      //
    EA = 1;      //
}