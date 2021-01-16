#include "qi51arduino.h"
#include "lcd12864.h"
#include "SR04.h"
#include "UART.h"

uchar code IC_DAT[] = {
    "这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）"};

void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
    Serial_begin(253);
    SR04_Init();
}

void loop()
{
    uint longl ;
    while (pin21 == 1)
        ;
    lcd_12864_write_com(0x00);
    longl = SR04_GetOnce();
    //lcd_12864_len_text(1, rec_dat);
}