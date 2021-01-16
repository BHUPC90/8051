#include "qi51arduino.h"
#include "iic.h"
#include "24C02.h"

#define smg P0

uchar code smgduan[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
                          0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71}; //显示0~F的值
uint num = 0;
sbit k1 = P2 ^ 7;
sbit k2 = P2 ^ 6;

void Keypros();

void setup()
{
    iic_init();
    num = AT24C02_readData_Byte(1);
}

void loop()
{
    Keypros();
    smg = ~smgduan[num / 1000];
}

void Keypros()
{
    if (k1 == 0)
    {
        delay(10); //消抖处理
        if (k1 == 0)
        {
            if (num <= 9)
            {
                num++;
            }
            smg = ~smgduan[num / 1000];
            AT24C02_readData(1, num);           //在地址1内写入数据num
            //num = AT24C02_readData_Byte(1); //读取EEPROM地址1内的数据保存在num中
        }
        while (!k1)
            ;
    }
    if (k2 == 0)
    {
        delay(10); //消抖处理
        if (k2 == 0)
        {
            if (num >= 0)
            {
                num--;
            }
            AT24C02_readData(1, num);           //在地址1内写入数据num
            //num = AT24C02_readData_Byte(1); //读取EEPROM地址1内的数据保存在num中
        }
        while (!k2)
            ;
    }
}