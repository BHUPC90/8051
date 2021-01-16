#ifndef _INC_LCD
#define _INC_LCD
#define uchar unsigned char
#define uint unsigned int
#endif

sbit LCD_RS = P2 ^ 7; //命令/数据
sbit LCD_RW = P2 ^ 6; //读取/写入
sbit LCD_EN = P2 ^ 5; //使能

void lcd_commode(uchar commode);
void delay(unsigned int wait_time);
//lcd发送指令
void lcd_commode(uchar commode)
{
    LCD_RS = 0;   //设置模式为：发送命令
    P0 = commode; //准备数据包
    delay(5);     //稍微延时一下
    LCD_EN = 0;   //发送数据！
    delay(5);     //稍微延时一下
    LCD_EN = 0;   //发送数据！
}