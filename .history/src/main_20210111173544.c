#include "qi51arduino.h"
#include "lcd12864.h"
#include "iic.h"
#include "24C02.h"

uchar code IC_DAT[] = {
    "Ð¡Ææ´æ´¢Æ÷ÊµÑé"};

void setup()
{
    int_lcd_12864();
    iic_init(); 
    AT24C02_writeByte(0x00,0x37);
    lcd_12864_len_text(1, IC_DAT);
    while (pin17 == 1)
        ;
    lcd_12864_write_com(0x00);
    
}

void loop()
{
    uchar temp[9];
    temp[0] = AT24C02_readData_Byte(0x20,sizeof(temp[0]),);
    temp[1] = '\0'
    lcd_12864_len_text(2, temp);
    delay(1000);
}