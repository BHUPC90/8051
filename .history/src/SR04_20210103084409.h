#ifndef _INC_SR04
#define _INC_SR04
#define uchar unsigned char
#define uint unsigned int
#endif
//���Ծ��� = ( �ߵ�ƽʱ�� * ����(340M/S) ) / 2;

sbit ECHO_port = P2 ^ 4;
sbit TRIG_port = P2 ^ 3;

void delay(unsigned int wait_time);

