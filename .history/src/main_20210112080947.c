#include "qi51arduino.h"
#include "lcd12864.h"
#include "iic.h"
#include "UART.h"

uchar code IC_DAT[] = {
    "小奇存储器实验"};

uchar code IC_DAT2[] = {
    "读取"};

void setup()
{

    iic_init();
    AT24C02_writeByte(0x00,9);
}

void loop()
{
    uchar temp[9];
    temp[0] = AT24C02_readData_Byte(0x00);
    temp[1] = '\0';
    lcd_12864_len_text(2, temp);

    delay(1000);
}