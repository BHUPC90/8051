#include "qi51arduino.h"
#include "iic.h"
#include "24C02.h"

#define smg P0

uchar code smgduan[17]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
					0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};//œ‘ æ0~Fµƒ÷µ

void setup()
{
    uchar temp;

    iic_init();
    AT24C02_writeByte(0x00, 9);
    temp = AT24C02_readData_Byte(0x00);
    smg=~smgduan[temp];
}

void loop()
{
}