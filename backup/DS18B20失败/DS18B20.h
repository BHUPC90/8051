#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 2;

int DS18B20_delay(uint wait_time) //10微妙
{
    do
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--wait_time);
}

/*启动DS18B20，握手程序*/
bit DS18B20_int()
{
    bit a;
    DS18B20_pin = 0; //准备复位
    DS18B20_delay(50);
    DS18B20_pin = 1;
    DS18B20_delay(6); //延时 60us
    a = DS18B20_pin;  //检测是否连接
    while (!DS18B20_pin)
        ; //等待回复
    return a;
}

/* 向 DS18B20 写入一个字节*/
void DS18B20_write(uchar data1)
{
    uchar data2;
    for (data2 = 0x01; data2 != 0; data2 <<= 1) //低位依次输出 8 位
    {
        DS18B20_pin = 0; //产生 2us 低电平脉冲
        _nop_();
        _nop_();
        if ((data2 & data1) == 0) //输出 : & 与运算  1&1 = 1 1&0 = 0
            DS18B20_pin = 0;
        else
            DS18B20_pin = 1;
        DS18B20_delay(6); //延时 60
        DS18B20_pin = 1;  //拉高通信引脚
    }
}

/* 从 DS18B20 读取一个字节 */
uchar DS18B20_read()
{
    uchar data2;
    uchar mask;
    for (mask = 0x01; mask != 0; mask <<= 1) //低位在先，依次采集 8 个 bit
    {
        DS18B20_pin = 0; //产生 2us 低电平脉冲
        _nop_();
        _nop_();
        DS18B20_pin = 1; //结束低电平脉冲，等待 18B20 输出数据
        _nop_();         //延时 2us
        _nop_();
        if (!DS18B20_pin) //读取通信引脚上的值
            data2 &= ~mask;
        else
            data2 |= mask;
        DS18B20_delay(6); //再延时 60us
    }
    return data2;
}

/* 读取 18B20 温度转换 */
bit DS18B20_inttemp()
{
    bit ack;                 //
    ack = DS18B20_int();     //执行总线复位，并获取 18B20 应答
    if (ack == 0)            //如 18B20 正确应答，则启动一次转换
    {                        //
        DS18B20_write(0xCC); //跳过 ROM 操作
        DS18B20_write(0x44); //启动温度转换
    }                        //
    return ~ack;             //ack==0 表示操作成功，所以返回值对其取反
}

/* 读取 DS18B20 转换的温度值*/
bit DS18B20_temp(int *temp)
{                                      //
    bit ack;                           //
    uchar LSB, MSB;                    //16bit 温度值的低字节和高字节
    ack = DS18B20_int();               //执行总线复位，并获取 18B20 应答
    if (ack == 0)                      //如 18B20 正确应答，则读取温度值
    {                                  //
        DS18B20_write(0xCC);           //跳过 ROM 操作
        DS18B20_write(0xBE);           //发送读命令
        LSB = DS18B20_read();          //读温度值的低字节
        MSB = DS18B20_read();          //读温度值的高字节
        *temp = ((int)MSB << 8) + LSB; //合成为 16bit 整型数
    }                                  //
    return ~ack;                       //ack==0 表示操作应答，所以返回值为其取反值
}