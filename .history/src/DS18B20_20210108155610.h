#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 4;

void Delay_ds18b20(uint t) //专用超精确延时10us
{
    int i;
    while (t--)
    {
        for (i = 0; i < 12; i++)
            ;
    }
}

uint int_ds18b20()
{
    int i;
    DS18B20_pin = 0; //拉低总线，产生复位信号
    i = 80;
    while (i > 0)
        i--;         //延时480~960us
    DS18B20_pin = 1; //拉高总线
    i = 4;
    while (i > 0)
        i--; //延时15~60us
    while (DS18B20_pin)
        ; //等待产生应答脉冲
    i = 70;
    while (i > 0)
        i--; //延时至少480us
}

//向DS18B20写一个字节

void Write_DS18B20(uint dat)
{
    int i, j;
    bit testb;
    for (j = 8; j > 0; j--)
    {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb) //写1
        {
            DS18B20_pin = 0; //拉低总线，产生写时间隙
            i++;             //延时大于1us
            DS18B20_pin = 1; //拉高总线
            i = 8;
            while (i > 0)
                i--; //延时至少60us，供DS18B20采样
        }
        else //写0
        {
            DS18B20_pin = 0; //拉低总线，产生写时间隙
            i = 8;
            while (i > 0)
                i--;         //保持至少60us，供DS18B20采样
            DS18B20_pin = 1; //拉高总线
            i++;
            i++;
        }
    }
}

//从DS18B20读取一个字节

uchar Read_DS18B20()
{
    int i, j;
    bit b; //定义存放接收到的1个字节
    uchar i_b;
    uchar rdbyte;
    for (j = 8; j > 0; j--)
    {
        DS18B20_pin = 0; //拉低总线，产生读时隙
        i++;             //延时大于1us
        DS18B20_pin = 1; //释放总线
        i++;
        i++;             //给一定时间让总线释放
        b = DS18B20_pin; //读取数据
        i = 8;
        while (i > 0)
            i--; //延时至少60us
        i_b = b;
        rdbyte = (i_b << 7) | (rdbyte >> 1); //将读取到得一位值左移7位，存放读取的数据变量rdbyte右移1位
    }
    return rdbyte;
}

void DS18B20_Read_id()
{
}
//docker run -d -p 9000:9000 -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data -v /file/portainer_web:/public portainer/portainer:1.20.2

uint DS18B20_Read_tem()
{
    uchar low_data, high_data;
    uint tflag;
    uint temp;
    Int_DS18B20();              //初始化
    Write_DS18B20(0xCC);        //忽略读取64位ROM地址，直接向DS18B20发送温度变换命令，适用于一个从机工作
    Write_DS18B20(0x44);        //表示启动 ds18b20 温度转换 , 结果自行存入 9 字节的 RAM中
    Delay_ds18b20(200);         //2ms
    Int_DS18B20();              //初始化
    Write_DS18B20(0xCC);        //跳过设备选取
    Write_DS18B20(0xBE);        //读取温度数据
    low_data = Read_DS18B20();  //低八位
    high_data = Read_DS18B20(); //高八位
    temp = high_data << 4;      //整合成一个字节位移
    temp |= (low_data >> 4);    //合并

    temp = temp | low_data; //结果 与 low_data 进行按位或
    if (temp < 0x0fff)      // 00001111 11111111
    {                       //
        tflag = 0;          // 前五位为 0 时，读取的温度为正，标志位为 0，此时只要
                            // 将测得数值乘以 0.0625 即可得到实际温度
    }                       //
    else                    // 前五位为 1 时，读取的温度为负，标志位为 1，此时需要
    {                       // 将测得数值取反后再加 1，再乘以 0.0625 即可得到实际温度
        tflag = 1;          // 此时表示负温度
        temp = ~temp + 1;
    }
    temp = temp * (0.625); // 温度值扩大 10 倍，精确到 1 位小数
    return temp;
}
