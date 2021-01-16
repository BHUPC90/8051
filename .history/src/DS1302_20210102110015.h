#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

sbit DSIO = P2 ^ 4;
sbit RST = P2 ^ 5;
sbit SCLK = P2 ^ 3;

uchar code READ_RTC_ADDR[7]=
        {0x81,0x83,0x85,0x87,0x89,0x8B,0x8D};
uchar code WEITE_RTC_ADDR[7]=
        {0x80,0x82,0x84,0x86,0x88,0x8A,0x8C};
uchar TIME[7]={0,0,0x23,0x013,0x05,0x06,0x18};

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

uchar Ds1302Read(uchar addr)
{
    uchar r, dat1, dat;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    for (r = 0; r < 8; r++)
    {
        DSIO = addr & 0x01;
        addr >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    _nop_();
    for (r = 0; r < 8; r++)
    {
        dat1 = DSIO;
        dat = (dat >> 1) | (dat1 << 7);
        SCLK = 1;
        _nop_(); //数据在SCLK上升沿时，DS1302读写数据，
                 //在SCLK下降沿时，DS1302放置数据到IO上
        SCLK = 0;
        _nop_();
    }
    RST = 0;
    _nop_();
    SCLK = 1;
    _nop_();
    DSIO = 0;
    _nop_();
    DSIO = 1;
    _nop_();
    return dat;
}

void Ds1302Init()
{
    uchar n;
    Ds1302Write(0x8e,0x00);//禁止写保护，开始写入
     for(n=0;n<8;n++){
          Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);
    }
    Ds1302Write(0x8e,0x80);//打开写保护，关闭写入
}

void Ds1302readTime()//读取时钟信息
{
    uchar n;
     for(n=0;n<8;n++){
         TIME[n]=Ds1302Read(READ_RTC_ADDR[n]);
    }
}