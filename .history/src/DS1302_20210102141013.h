#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//这个模块内没有集成上拉电阻，使用时最好接上2
sbit RST = P0 ^ 1;  //RET,使能输入引脚，当读写时，置高位
sbit DAT = P0 ^ 2;  //IO双向通信引脚，读写数据都是通过这个完成
sbit SCLK = P0 ^ 3; //SCLK，时钟信号

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
    for (n = 0; n < 8; n++)
    { 
        DAT = byte & 0x01;
        byte >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
}

//向DS1302命令和数据（地址+数据）
void Ds1302Write(uchar addr,uchar dat)
{
    uchar n;
    RST=0;_nop_();
    SCLK=0;_nop_();
    RST=1;_nop_();
    DS1302_WriteByte(addr);
    DS1302_WriteByte(dat);
    RST=0;_nop_();
}

/*------------------------------------
   Read函数
------------------------------------*/
uchar DS1302_ReadByte(uchar address)
{
}

void int_ds1302()
{
}