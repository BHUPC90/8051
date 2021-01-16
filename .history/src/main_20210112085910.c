#include "qi51arduino.h"
#include "iic.h"
#include "24C02.h"
#include "UART.h"

void setup()
{
    uchar temp;

    Serial_begin(253);
    iic_init();
    AT24C02_writeByte(0x00, 9);
    
}

void loop()
{

    if (Serial_available)
    {
        Serial_print_byte(re_data);
    }
    Serial_available = 0;
}