#include "qi51arduino.h"

int mod = 0;

uint wait_timer;
void cshtimer() //1000是1毫秒（ms）最大65.536毫秒
{
    TMOD = 0x01; //设置模式1，16位定时 ， 8高8低
    //11111111 11111111 转10进制 65,535 加1得到 65536 , 256 = 2的八次幂
    TH0 = (65536 - wait_timer) / 256;
    TL0 = (65536 - wait_timer) % 256;
    EA = 1;  //开中断
    ET0 = 1; //打开定时器0中断
    TR0 = 1; //进入定时器函数
}

void setup()
{
    wait_timer = 50000;
    cshtimer();
}

void loop()
{
    pin1 = mod;
}

void timervoid() interrupt 1
{
    TH0 = (65536 - wait_timer) / 256;
    TL0 = (65536 - wait_timer) % 256;
    mod = !mod;
}