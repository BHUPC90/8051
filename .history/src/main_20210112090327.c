#include "qi51arduino.h"
#include "iic.h"
#include "24C02.h"

#define smg  P0

void setup()
{
    uchar temp;

    iic_init();
    AT24C02_writeByte(0x00, 9);
    temp = AT24C02_readData_Byte(0x00);
    Serial_print_byte(temp);
}

void loop()
{
}