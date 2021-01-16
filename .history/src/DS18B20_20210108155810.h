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
    uchar high, low;

    int_ds18b20();      //初始化，复位并获取应答信号
    wr_temp(0xcc);    //跳过ROM
    wr_temp(0x44);    //开始温度转换
    delay_ms(1000);   //等待温度转换完成

    int_ds18b20();      //将DS18B20复位
    wr_temp(0xcc);    //跳过ROM
    wr_temp(0xbe);    //读暂存器
    
    low = rd_temp();  //读取低8位
    high = rd_temp(); //读取高8位
    temp = high;
    temp <<= 8;
    temp = temp | low;        //将读取的低8位和高8位合并
    f_temp = temp * 0.0625;   //温度在寄存器中为12位 分辨率位0.0625°
    temp = f_temp * 10 + 0.5; //精确到十分位，四舍五入
    d2 = temp / 100;          //显示数据：十位
    d1 = temp % 100 / 10;     //显示数据：个位
    d0 = temp % 10;           //显示数据：十分位
}
