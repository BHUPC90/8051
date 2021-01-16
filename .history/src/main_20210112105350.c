#include "qi51arduino.h"

#define GPIO_KEY P0

uint KeyValue; //用来存放读取到的键值

void Keypros();
void KeyDown();

void setup()
{
}

void loop()
{
    Keypros();
}

void Keypros()
{
    if (pin28 == 0)
    {
        delay(10); //消抖处理
        if (pin28 == 0)
        {
        }
        while (!pin28)
            ;
    }

    if (pin27 == 0)
    {
        delay(10); //消抖处理
        if (pin27 == 0)
        {
        }
        while (!pin27)
            ;
    }
}

void KeyDown()
{
    char a = 0;
    GPIO_KEY = 0x0f;
    if (GPIO_KEY != 0x0f) //读取按键是否按下
    {
        delay(1000);          //延时10ms进行消抖
        if (GPIO_KEY != 0x0f) //再次检测键盘是否按下 0000 1111
        {
            //测试列
            GPIO_KEY = 0X0F; // 0000 1111
            switch (GPIO_KEY)
            {
            case (0X07):  //0000 0111
                KeyValue = 0;
                break;
            case (0X0b):  //0000 1011
                KeyValue = 1;
                break;
            case (0X0d):  //0000 1101
                KeyValue = 2;
                break;
            case (0X0e):  //0000 1110
                KeyValue = 3;
                break;
            }
            //测试行
            GPIO_KEY = 0XF0;  // 1111 0000
            switch (GPIO_KEY)
            {
            case (0X70):  //0111 0000
                KeyValue = KeyValue;
                break;
            case (0Xb0):  //1011 0000
                KeyValue = KeyValue + 4;
                break;
            case (0Xd0):  //1101 0000
                KeyValue = KeyValue + 8;
                break;
            case (0Xe0):  //1110 0000
                KeyValue = KeyValue + 12;
                break;
            }
            while ((a < 50) && (GPIO_KEY != 0xf0)) //检测按键松手检测
            {
                delay(1);
                a++;
            }
        }
    }
}