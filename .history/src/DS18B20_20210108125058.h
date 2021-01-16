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

uint int_ds18b20(void)
{
    uint reback = 0;      //验证是否正常
    DS18B20_pin = 1;      //首先将数据线置高
    Delay_ds18b20(12);    //短暂延时
    DS18B20_pin = 0;      //将数据线拉到低电平
    Delay_ds18b20(80);    //延时在480~960US之间
    DS18B20_pin = 1;      //数据线拉回高电平
    Delay_ds18b20(10);    // 延时等待
    reback = DS18B20_pin; // initflag等于1初始化失败
    Delay_ds18b20(5);     //最后一个延时
    return reback;
}

//向DS18B20写一个字节

void Write_DS18B20(uint dat)
{
    uint i;
    for (i = 0; i < 8; i++) //重复直到发送完整个字节
    {
        DS18B20_pin = 0; //数据线首先拉低
        DS18B20_pin = dat & 0x01;
        Delay_ds18b20(5); //延时45us
        DS18B20_pin = 1;  //将数据线拉回高电平1
        dat >>= 1;
    }
    Delay_ds18b20(5);
}

//从DS18B20读取一个字节

uchar Read_DS18B20(void)
{
    uint i;
    uchar dat;
    for (i = 0; i < 8; i++) //重复接收直到读完整个字节
    {
        DS18B20_pin = 0; //数据线拉低
        dat >>= 1;       //下一位
        DS18B20_pin = 1; //数据线拉高
        if (DS18B20_pin)
            dat |= 0x80; //1000 0000
        Delay_ds18b20(5);
    }
    return dat;
}

void DS18B20_Read_id(void)
{
}
//docker run -d -p 9000:9000 -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data -v /file/portainer_web:/public portainer/portainer:1.20.2
char DS18B20_Read_tem(void)
{
    unsigned char low_data, high_data;
    char temp;
    Int_DS18B20();              //初始化
    Write_DS18B20(0xCC);        //忽略64位ROM地址，直接向DS18B20发送温度变换命令，适用于一个从机工作
    Write_DS18B20(0x44);        //温度转换
    Delay_ds18b20(200);         //2ms
    Int_DS18B20();              //初始化
    Write_DS18B20(0xCC);        //跳过设备选取
    Write_DS18B20(0xBE);        //读取温度数据
    low_data = Read_DS18B20();  //低八位
    high_data = Read_DS18B20(); //高八位
    temp = high_data << 4;      //整合成一个字节位移
    temp |= (low_data >> 4);    //合并
    return temp;
}