#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 4;

void init_ds18b20(void)
{
    bit initflag = 0;
    DS18B20_pin = 1;            //首先将数据线置高
    Delay_OneWire(12); //短暂延时
    DS18B20_pin = 0;            //将数据线拉到低电平
    Delay_OneWire(80); //延时在480~960US之间
    DS18B20_pin = 1;            //数据线拉回高电平
    Delay_OneWire(10); // 延时等待
    initflag = DS18B20_pin;     // initflag等于1初始化失败
    Delay_OneWire(5);  //最后一个延时
}