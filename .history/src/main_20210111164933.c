#include "qi51arduino.h"
#include "lcd12864.h"
#include "UART.h"
#include "DS18B20.h"

uchar code IC_DAT[] = {
    "Ð¡Ææ´æ´¢Æ÷ÊµÑé"};

void setup()
{
    int_lcd_12864();
    lcd_12864_len_text(1, IC_DAT);
    Serial_begin(253);
    while (pin22 == 1)
        ;
    lcd_12864_write_com(0x00);
    DS18B20_Read_id();
    Serial_print(ds_id);
    lcd_12864_len_text(4, ds_id);
}

void loop()
{
    uchar temp[9];
    uint longl;

    longl = DS18B20_Read_tem();
    temp[0] = '0' + longl / 100;
    temp[1] = '0' + longl % 100 / 10;
    temp[2] = '.';
    temp[3] = '0' + longl % 10;
    temp[4] = '\0';
    lcd_12864_len_text(2, temp);
    delay(1000);
}