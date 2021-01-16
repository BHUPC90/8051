#include "qi51arduino.h"
#include "lcd12864.h"
#include "DS1302.h"

uchar temp[3];

uchar READ_RTC_ADDR[7]= {0x81,0x83,0x85,0x87,0x89,0x8B,0x8D};
uchar WEITE_RTC_ADDR[7]= {0x80,0x82,0x84,0x86,0x88,0x8A,0x8C};
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