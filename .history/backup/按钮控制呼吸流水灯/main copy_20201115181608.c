#include "qi51arduino.h"
#include "UART.h"
//串口控制速率按钮控制呼吸流水灯
void openled(int led, int state);
int mod = 0;
uint set_pwm = 0;
uint pwm;
uint led_switch = 1;

void setup()
{

    TMOD = 0x01; //设置模式1，16位定时 ， 8高8低
    EA = 1;      //设置CPU开启中断
    TH0 = 255;   //(65536 - 10) /256 0.1ms    在线取余计算器： http://www.ab126.com/shuxue/2892.html
    TL0 = 246;   //(65536 - 10) %256 0.1ms
    ET0 = 1;     //打开定时器0中断
    TR0 = 1;     //进入定时器函数

    EX0 = 1; //设置EX0开启中断（外部中断0,P3.2）
    IT0 = 1; // 设置中断触发方式 0，长时间触发，1电平变换时触发，适合按钮

    pin1 = 1;
    pin2 = 1;
    pin3 = 1;
    pin4 = 1;
    Serial_begin(253);
}
void loop()
{
    while (set_pwm < 100)
    {
        set_pwm += speed;
        delay(1);
    }
    if (mod)
    {
        led_switch++;
        if (led_switch >= 5)
        {
            led_switch = 1;
        }
    }
    else
    {
        led_switch--;
        if (led_switch <= 0)
        {
            led_switch = 5;
        }
    }
    Serial_print_byte(led_switch + 64);
    Serial_available = 0;

    while (set_pwm > 0)
    {
        set_pwm -= speed;
        delay(1);
    }
}
void timervoid() interrupt 1
{
    TH0 = 255;
    TL0 = 246;
    if (pwm >= 100)
    {
        pwm = 0;
    }
    if (pwm < set_pwm)
    {
        openled(led_switch, 1);
    }
    else
    {
        openled(led_switch, 0);
    }
    pwm++;
}
//外部中端程序，注意有坑！0是EX0外部中断0，2是EX1外部中断一
void zhongduan0() interrupt 0
{
    mod = !mod;
}

void openled(int led, int state)
{
    switch (led)
    {
    case 1:
        pin1 = state;
        break;
    case 2:
        pin2 = state;
        break;
    case 3:
        pin3 = state;
        break;
    case 4:
        pin4 = state;
        break;
    }
}