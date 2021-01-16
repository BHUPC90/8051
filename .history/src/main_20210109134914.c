#include "qi51arduino.h"
#include "lcd12864.h"
#include "UART.h"
#include "DS18B20.h"

uchar code IC_DAT[] = {
    "程建龙"};
uchar code IC_DAT22[] = {
    "  程建龙"};
uchar code IC_DAT33[] = {
    "    程建龙"};
uchar code IC_DAT44[] = {
    "      程建龙"};

void setup()
{
    int_lcd_12864();
    lcd_12864_len_text(1, IC_DAT);
    lcd_12864_len_text(2, IC_DAT22);
    lcd_12864_len_text(3, IC_DAT33);
    lcd_12864_len_text(4, IC_DAT44);

}

void loop()
{

}