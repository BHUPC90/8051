#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "huatu.h"


sbit IRIN = P3 ^ 2;


uchar code IC_DAT[] = {
    ""};

void IrInit()
{
    IT0 = 1;  //下降沿触发
    EX0 = 1;  //打开中断0允许
    EA = 1;   //打开总中断
    IRIN = 1; //初始化端口
}

void setup()
{
    IrInit();
    int_lcd_12864();
    lcd_12864_write_com(0x00);
    lcd_12864_picture(firstpage);
    Ds1302readTime();
    delay(500);
    lcd_12864_write_com(0x00);
}

void loop()
{
}
