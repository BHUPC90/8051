#ifndef _INC_QI51ARDUINO
#define _INC_QI51ARDUINO
#define uchar unsigned char
#define uint unsigned int
#endif
#include <reg52.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <intrins.h>
#include <string.h>
uchar Serial_char;
uchar Last_Serial_char;
sbit pin1 = P1 ^ 0;
sbit pin2 = P1 ^ 1;
sbit pin3 = P1 ^ 2;
sbit pin4 = P1 ^ 3;
sbit pin5 = P1 ^ 4;
sbit pin6 = P1 ^ 5;
sbit pin7 = P1 ^ 6;
sbit pin8 = P1 ^ 7;

sbit pin11 = P3 ^ 0;
sbit pin12 = P3 ^ 1;
sbit pin13 = P3 ^ 2;
sbit pin14 = P3 ^ 3;
sbit pin15 = P3 ^ 4;
sbit pin16 = P3 ^ 5;
sbit pin17 = P3 ^ 6;

sbit pin21 = P2 ^ 0;
sbit pin22 = P2 ^ 1;
sbit pin23 = P2 ^ 2;

void loop();
void setup();
void Serial_begin();

void delay(unsigned int wait_time);
void main(void);

void delay(unsigned int wait_time) //1ms ??
{
	unsigned int i, j;
	for (j = wait_time; j > 0; j--)
		for (i = 112; i > 0; i--)
			_nop_;
}
void delaymicroseconds(unsigned int wait_time) //1ms ??
{
	if (wait_time == 0)
		return;
	wait_time = wait_time / 10;
	while (wait_time--)
		;
}
void Serial_begin()
{
	//设置SCON寄存器
	//设置工作方式1 ： 10位异步接收发送
	SM0 = 0;
	SM1 = 1;
	SM2 = 0; //用于方式2,3 RI

	REN = 1; //启用接收
	TB8 = 0; //发送：是否启用奇偶校验，0,1模式不启用
	RB8 = 0; //接收：是否启用奇偶校验，0,1模式不启用
	TI = 0;	 //TI 发送中断
	RI = 0;	 //RI 接收中断
	//11059200/(12*(256-T1))*1/32
	ET1 = 1;   //打开定时器0中断方式1
	TH1 = 253; //波特率9600
	TR0 = 253; //波特率9600
	TR1 = 1;   //进入定时器
}

void Serial_read() interrupt 4
{
	RI = 0;				//清除接收标志
	Serial_char = SBUF; //读取串口并写入
	P1 = Serial_char;
	if (Serial_char != Last_Serial_char)
	Last_Serial_char = Serial_char ;
}

/*没事别瞎动主函数*/
void main(void)
{
	setup();
	while (1)
	{
		loop();
	}
}
