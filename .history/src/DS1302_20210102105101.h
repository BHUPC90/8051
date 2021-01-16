#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

sbit DSIO = P2 ^ 4;
sbit RST = P2 ^ 5;
sbit SCLK = P2 ^ 3;

void Ds1302Write(uchar addr, uchar dat) //向DS1302命令（地址+数据）
{
    uchar n;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    for (n = 0; n < 8; n++)
    { //地址传送
        DSIO = addr & 0x01;
        addr >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    for (n = 0; n < 8; n++)
    { //读数据
        DSIO = dat & 0x01;
        dat >>= 1;
        SCLK = 1;
        _nop_(); //数据在SCLK上升沿时，DS1302读写数据，
                 //在SCLK下降沿时，DS1302放置数据到IO上
        SCLK = 0;
        _nop_();
    }
    RST = 0;
    _nop_();
}
