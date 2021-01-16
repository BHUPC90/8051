#ifndef _INC_LCD
#define _INC_LCD
#define uchar unsigned char
#define uint unsigned int
#endif

sbit LCD_RS = P2 ^ 7; //命令/数据
sbit LCD_RW = P2 ^ 6; //读取/写入
sbit LCD_EN = P2 ^ 5; //使能

//lcd发送指令
void lcd_commode()
{
    LCD_EN = 0; //初始化使能低电平
    LCD_RS = 0; //发送命令
}