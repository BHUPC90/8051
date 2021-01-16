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
    Serial_begin(253);
    iic_init();
    AT24C02_writeByte(0x00, 9);
}

void loop()
{
    uchar temp;
    temp = AT24C02_readData_Byte(0x00);
    Serial_print_byte(temp);

    delay(1000);
}