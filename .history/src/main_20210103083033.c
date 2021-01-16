#include "qi51arduino.h"
#include "lcd12864.h"


uchar code IC_DAT[] = {
    "这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）"};

void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
    Ds1302Init();
}

void loop()
{
    while (pin21 == 1)
        ;
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, rec_dat);
}