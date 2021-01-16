#include "qi51arduino.h"
//中断实验

int mod = 0;

void setup()
{
    pin1 = 1; //初始化LED灯
    pin2 = 1; //初始化LED灯
    pin3 = 1; //初始化LED灯
    pin4 = 1; //初始化LED灯

    EA = 1; //设置CPU开启中断

    EX0 = 1; //设置EX0开启中断（外部中断0,P3.2）
    IT0 = 1; // 设置中断触发方式 0，长时间触发，1电平变换时触发，适合按钮

    EX1 = 1; //设置EX1开启中断（外部中断0,P3.2）
    IT1 = 1; // 设置中断触发方式 0，长时间触发，1电平变换时触发，适合按钮
    
}

void loop()
{
    while (mod == 0)
    {
        pin1 = 1;
        pin2 = 1;
        pin3 = 1;
        pin4 = 0;
        delay(1000);
        if (mod)
            break;
        pin1 = 1;
        pin2 = 1;
        pin3 = 0;
        pin4 = 1;
        delay(1000);
        if (mod)
            break;
        pin1 = 1;
        pin2 = 0;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod)
            break;
        pin1 = 0;
        pin2 = 1;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod)
            break;
    }
    while (mod)
    {
        if (mod == 0)
            break;
        pin1 = 0;
        pin2 = 1;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod == 0)
            break;
        pin1 = 1;
        pin2 = 0;
        pin3 = 1;
        pin4 = 1;
        delay(1000);
        if (mod == 0)
            break;
        pin1 = 1;
        pin2 = 1;
        pin3 = 0;
        pin4 = 1;
        delay(1000);
        if (mod == 0)
            break;
        pin1 = 1;
        pin2 = 1;
        pin3 = 1;
        pin4 = 0;
        delay(1000);
    }
}

//外部中端程序，注意有坑！0是EX0外部中断0，2是EX1外部中断一
void zhongduan0() interrupt 0
{
    mod = 0;
}
void zhongduan1() interrupt 2
{
    mod = 1;
}