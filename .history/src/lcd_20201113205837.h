#ifndef _INC_LCD
#define _INC_LCD
#define uchar unsigned char
#define uint unsigned int
#endif

sbit LCD_RS = P2 ^ 7;
sbit LCD_RW = P2 ^ 6;
sbit LCD_EN = P2 ^ 5;
