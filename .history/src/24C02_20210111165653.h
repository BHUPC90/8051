#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif

sbit sda = P2 ^ 3;
sbit scl = P2 ^ 3;

void iic_start()  //启动信号
{
        sda=1;
        scl=1;
        delayus();                //sda和scl同为高电平保持4.7us以上
        _nop_();                        //1.085us,共5.78us,下面sda=0是下降沿,不能计算在延时时间中
        sda=0;                         //下降沿
        delayus();                //sda低电平保持4us以上        ,这里是4.34us满足要求
}