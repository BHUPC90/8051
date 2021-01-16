#ifndef _INC_iic
#define _INC_iic
#define uchar unsigned char
#define uint unsigned int
#endif

sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;

//最大255，一个机器周期为1us，最大延时255us。
void Delay10us()
{
    unsigned char a, b;
    for (b = 1; b > 0; b--)
        for (a = 2; a > 0; a--)
            ;
}

void iic_init() //总线初始化
{
    SCL = 1;
    SDA = 1;
    Delay10us();
}

void iic_start() //启动信号
{
    SDA = 1;
    Delay10us();
    SCL = 1;
    Delay10us(); //建立时间是SDA保持时间>4.7us
    SDA = 0;
    Delay10us(); //保持时间是>4us
    SCL = 0;
    Delay10us();
}

void iic_stop() //停止信号
{

    SDA = 0;
    Delay10us();
    SCL = 1;
    Delay10us(); //建立时间大于4.7us
    SDA = 1;
    Delay10us();
}

void iic_sendByte(uchar dat) //mcu发送一个字节
{
    uchar a = 0 ;
    for (a = 0; a < 8; a++) //要发送8位，从最高位开始
    {
        SDA = dat >> 7; //起始信号之后SCL=0，所以可以直接改变SDA信号
        dat = dat << 1;
        Delay10us();
        SCL = 1;
        Delay10us(); //建立时间>4.7us
        SCL = 0;
        Delay10us(); //时间大于4us
    }
    SDA = 1;
    Delay10us();
}

uchar iic_readByte() //读一个字节
{
    uchar a = 0, dat = 0;
    SDA = 1; //起始和发送一个字节之后SCL都是0
    Delay10us();
    for (a = 0; a < 8; a++) //接收8个字节
    {
        SCL = 1;
        Delay10us();
        dat <<= 1;
        dat |= SDA;
        Delay10us();
        SCL = 0;
        Delay10us();
    }
    return dat;
}

int iic_checkACK() //处理应答信号
{
    uchar b = 0 ;
    SDA = 1;
    SCL = 1;
    while (SDA) //等待应答，也就是等待从设备把SDA拉低
    {
        b++;
        if (b > 200) //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
        {
            SCL = 0;
            Delay10us();
            return 0; //发送失败
        }
    }
    SCL = 0;
    return 1; //发送成功
}