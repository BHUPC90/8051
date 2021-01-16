#include "qi51arduino.h"
#include "lcd12864.h"
#include "iic.h"
#include "24C02.h"

uchar code IC_DAT[] = {
    "Ð¡Ææ´æ´¢Æ÷ÊµÑé"};

void setup()
{
    int_lcd_12864();
    lcd_12864_len_text(1, IC_DAT);
    while (pin17 == 1)
        ;
    lcd_12864_write_com(0x00);
    iic_init(); 
}

void loop()
{
    uchar temp[9];
    lcd_12864_len_text(2, temp);
    delay(1000);
}