#include "qi51arduino.h"
#include "lcd12864.h"
#include "DHT11.h"

uchar temp[2];

unsigned char code IC_DAT[] = {
    "这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）"};

unsigned char code IC_DAT2[] = {
    "湿度"};
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
        lcd_12864_write_com(0x80); //第1行
        uint strlong = qi_take_length(IC_DAT2);
        lcd_12864_write_com(0x80); //第1行
        for (i = 0; i < strlong; i++)
        {
                lcd_12864_write_data(*IC_DAT2);
                IC_DAT2++;
        }
        temp[0] = '0' + (humidity / 10);
        temp[1] = '0' + (humidity % 10);
        lcd_12864_write_data(temp);
}