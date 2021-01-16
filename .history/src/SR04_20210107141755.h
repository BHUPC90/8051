#ifndef _INC_SR04
#define _INC_SR04
#define uchar unsigned char
#define uint unsigned int
#endif
//测试距离 = ( 高电平时间 * 声速(340M/S) ) / 2;

sbit ECHO_port = P2 ^ 4;
sbit TRIG_port = P2 ^ 3;

void delay(unsigned int wait_time);

void  StartModule() 		         //启动模块
{
	  TX=1;			                     //启动一次模块
	  delaywhile(2);
	  TX=0;
}