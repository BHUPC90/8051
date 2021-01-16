#include "qi51arduino.h"

#define GPIO_KEY P0

k1 = P2 ^ 7;
k2 = P2 ^ 6;
k3 = P2 ^ 5;
k4 = P2 ^ 4;
k5 = P2 ^ 3;
k6 = P2 ^ 2;
k7 = P2 ^ 1;
k8 = P2 ^ 0;


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
        delay(10); //消抖处理
        if (k1 == 0)
        {
           
        }
        while (!k1)
            ;
    }

    if (k2 == 0)
    {
        delay(10); //消抖处理
        if (k2 == 0)
        {
            
        }
        while (!k2)
            ;
    }
}