#ifndef _INC_LCD
#define _INC_LCD
#define uchar unsigned char
#define uint unsigned int
#endif

sbit LCD_RS = P2 ^ 7; //
sbit LCD_RW = P2 ^ 6; //读取/写入
sbit LCD_EN = P2 ^ 5; //

//lcd发送指令
void lcd_commode (){
LCD_RS = 0;
}