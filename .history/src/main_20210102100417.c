#include "qi51arduino.h"
#include "lcd12864.h"
#include "DHT11.h"

uchar temp[2];

unsigned char code IC_DAT[] = {
    "这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）"};

unsigned char code IC_DATA_hi[] = {
    "湿度"};
unsigned char code IC_DATA_te[] = {
    "温度"};

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
        DHT11_temp();
        
        lcd_12864_len_text(1,IC_DATA_hi);
        lcd_12864_len_text(2,IC_DATA_te);
        temp[0] = '0' + (temperature / 10);
        temp[1] = '0' + (temperature % 10);
        lcd_12864_len_text(0,temp);
}