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


void loop();
void setup();
void Serial_begin();

void delay(unsigned int wait_time);
void delaywhile(unsigned int wait_time); //1ms ??
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

void delaywhile(unsigned int wait_time) 
{
	while (wait_time--)
		;
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
