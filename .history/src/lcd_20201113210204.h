#ifndef _INC_LCD
#define _INC_LCD
#define uchar unsigned char
#define uint unsigned int
#endif

sbit LCD_RS = P2 ^ 7; //
sbit LCD_RW = P2 ^ 6; //��ȡ/д��
sbit LCD_EN = P2 ^ 5; //

//lcd����ָ��
void lcd_commode (){
LCD_RS = 0;
}