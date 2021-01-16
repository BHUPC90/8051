#include "qi51arduino.h"
#include "UART.h"
//´®¿ÚÍ¨ÐÅ
void setup()
{
    Serial_begin(253);
}
void loop()
{
    if (Serial_available)
    {
        Serial_print_byte(re_data);
    }
    Serial_available = 0;
}
