#include "qi51arduino.h"
#include "lcd12864.h"
#include "iic.h"
#include "24C02.h"

uchar code IC_DAT[] = {
    "С��洢��ʵ��"};

uchar code IC_DAT2[] = {
    "��ȡ����Ϊ"};



void setup()
{
    int_lcd_12864();
    iic_init();
    AT24C02_writeByte(0x00, 2);
    lcd_12864_len_text(1, IC_DAT);
    while (pin1 == 1)
        ;
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, IC_DAT2);
}

void loop()
{


    delay(1000);
}