#include "qi51arduino.h"



void setup()
{
    
}

void loop()
{
    Keypros();
}

void Keypros()
{
    if (k1 == 0)
    {
        delay(100); //��������
        if (k1 == 0)
        {
            num++;
            if (num == 10)
            {
                num = 0;
            }
            AT24C02_writeByte(1, num);      //�ڵ�ַ1��д������num
            delay(10);
            num = AT24C02_readData_Byte(1); //��ȡEEPROM��ַ1�ڵ����ݱ�����num��
        }
        while (!k1)
            ;
    }

    if (k2 == 0)
    {
        delay(100); //��������
        if (k2 == 0)
        {
            if (num == 0)
            {
                num = 10;
            }
            num--;
            AT24C02_writeByte(1, num);      //�ڵ�ַ1��д������num
            delay(10);
            num = AT24C02_readData_Byte(1); //��ȡEEPROM��ַ1�ڵ����ݱ�����num��
        }
        while (!k2)
            ;
    }
}