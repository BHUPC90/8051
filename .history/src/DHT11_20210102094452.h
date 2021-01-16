#ifndef _INC_DHT11
#define _INC_DHT11
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DHT11_pin = P2 ^ 3;

uint temperature = 0;
uint humidity = 0;
uchar rec_dat[15];

//10us 精确延时

void DHT11_delay_us(uchar t)
{
    while (--t)
        ;
}

void DHT11_delay_ms(uint t)
{
    uint i, j;
    for (i = t; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

//初始化 DHT11
void DHT11_int()
{
    DHT11_pin = 1;
    DHT11_delay_us(2);
    DHT11_pin = 0;
    DHT11_delay_ms(20); //延时18ms以上
    DHT11_pin = 1;
    DHT11_delay_us(30);
}

//读取数据8位
uchar DHT11_read()
{
    uchar i, dat = 0;
    for (i = 0; i < 8; i++) //从高到低依次接收8位数据
    {
        while (!DHT11_pin)
            ;
        DHT11_delay_us(8);  //延时60us，如果还为高则数据为1，否则为0
        dat <<= 1;          //移位使正确接收8位数据，数据为0时直接移位
        if (DHT11_pin == 1) //数据为1时，使dat加1来接收数据1
            dat += 1;
        while (DHT11_pin)
            ; //等待数据线拉低
    }
    return dat;
}

//读取温度
void DHT11_temp()
{
   
}