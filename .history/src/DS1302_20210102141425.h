#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//这个模块内没有集成上拉电阻，使用时最好接上2
sbit RST = P0 ^ 1;   //RET,使能输入引脚，当读写时，置高位
sbit DATIO = P0 ^ 2; //IO双向通信引脚，读写数据都是通过这个完成
sbit SCLK = P0 ^ 3;  //SCLK，时钟信号

struct date_struct
{
    uint year;
    uint mounth;
    uint date;
    uint hour;
    uint minute;
    uint second;
} date_type;

//发送一个8位二进制字符，调用前请先写时序
void DS1302_WriteByte(uchar byte)
{
    for (int n = 0; n < 8; n++)
    {
        DATIO = byte & 0x01;
        byte >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
}

//向DS1302命令和数据（地址+数据）
void Ds1302Write(uchar addr, uchar dat)
{
    uchar n;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    DS1302_WriteByte(addr);
    DS1302_WriteByte(dat);
    RST = 0;
    _nop_();
}

//读取一个个字符 8位二进制
uchar DS1302_Read(uchar address)
{
    uchar dat1, dat;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    DS1302_WriteByte(address);
    _nop_();
    for (uint r = 0; r < 8; r++)
    {
        dat1 = DATIO;
        dat = (dat >> 1) | (dat1 << 7);
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    RST = 0;
    _nop_();
    SCLK = 1;
    _nop_();
    DATIO = 0;
    _nop_();
    DATIO = 1;
    _nop_();
    return dat;
}

void int_ds1302()
{
}