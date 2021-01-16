#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 4;

uint init_ds18b20(void)
{
    uint reback = 0;      //
    DS18B20_pin = 1;      //首先将数据线置高
    Delay_OneWire(12);    //短暂延时
    DS18B20_pin = 0;      //将数据线拉到低电平
    Delay_OneWire(80);    //延时在480~960US之间
    DS18B20_pin = 1;      //数据线拉回高电平
    Delay_OneWire(10);    // 延时等待
    reback = DS18B20_pin; // initflag等于1初始化失败
    Delay_OneWire(5);     //最后一个延时
    return reback;
}

//向DS18B20写一个字节

void Write_DS18B20(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++) //重复直到发送完整个字节
    {
        DS18B20_pin = 0; //数据线首先拉低
        DS18B20_pin = dat & 0x01;
        Delay_OneWire(5); //延时45us
        DS18B20_pin = 1;           //将数据线拉回高电平1
        dat >>= 1;
    }
    Delay_OneWire(5);
}

//从DS18B20读取一个字节

unsigned char Read_DS18B20(void)
{
    unsigned char i;
    unsigned char dat;
    for (i = 0; i < 8; i++) //重复接收直到读完整个字节
    {
        DS18B20_pin = 0; //数据线拉低
        dat >>= 1;
        DS18B20_pin = 1; //数据线拉高
        if (DS18B20_pin)
        {
            dat |= 0x80;
        }
        Delay_OneWire(5);
    }
    return dat;
}