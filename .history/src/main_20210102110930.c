#include "qi51arduino.h"
#include "lcd12864.h"
#include "DS1302.h"

uchar temp[3];


uchar code IC_DAT[] = {
    "这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）"};

void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
}

void loop()
{
    while (pin21 == 1)
        ;
    lcd_12864_write_com(0x00);
    Ds1302Init();
    Ds1302readTime();
    lcd_12864_len_text(1, TIME);
}