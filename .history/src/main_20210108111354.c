#include "qi51arduino.h"
#include "lcd12864.h"
#include "DS18B20.h"

uchar code IC_DAT[] = {
    "这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）"};
uchar code IC_DAT_2[] = {
    "温度"};
uchar code IC_DAT_3[] = {
    "单位：摄氏度"};
void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
    
    while (pin22 == 1)
        ;
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, IC_DAT_2);
    lcd_12864_len_text(3, IC_DAT_3);
}

void loop()
{
    uchar temp[9];
    uint longl;
    temp[0] = '0' + (longl / 1000) % 10;
    temp[1] = '0' + (longl / 100) % 10;
    temp[2] = '0' + (longl / 10) % 10;
    temp[3] = '0' + longl % 10;
    temp[4] = '\0';
    lcd_12864_len_text(2, temp);
    delay(1000);
}