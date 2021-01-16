#include "qi51arduino.h"
#include "24C02.h"
#include "iic.h"
#include "UART.h"

void setup()
{
    uchar temp;

    Serial_begin(253);
    //iic_init();
    //AT24C02_writeByte(0x00, 9);

    temp = '7';
    Serial_print_byte(temp);
}

void loop()
{

    delay(1000);
}