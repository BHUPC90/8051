#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "huatu.h"

sbit IRIN = P3 ^ 2;

uchar temp[64];

void IrInit()
{
    IT0 = 1;  //下降沿触发
    EX0 = 1;  //打开中断0允许
    EA = 1;   //打开总中断
    IRIN = 1; //初始化端口
}


void readtime()
{
    Ds1302readTime();
    temp[0] = ' ';
    temp[1] = ' ' ;
    temp[2] = ' ' ;
    temp[3] = ' ';
    temp[4] = '0' + time[6] / 16;
    temp[5] = '0' + (time[6] & 0x0f);
    temp[6] = '-';
    temp[7] = '0' + time[4] / 16;
    temp[8] = '0' + (time[4] & 0x0f);
    temp[9] = '-';
    temp[10] = '0' + time[3] / 16;
    temp[11] = '0' + (time[3] & 0x0f);
    temp[12] = '\0';
}

void setup()
{
    IrInit();
    //////////////////////////////
    int_lcd_12864();
    lcd_12864_picture(firstpage);
    //////////////////////////////
    Ds1302readTime();
    /////////////////////////////
    delay(500);
    lcd_12864_write_com(0x00);

    readtime();
    
}

void loop()
{

}
