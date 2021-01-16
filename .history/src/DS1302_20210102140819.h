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

void DS1302_WriteByte(uchar byte)
{
    for (n = 0; n < 8; n++)
    { //读数据
        DAT = byte & 0x01;
        byte >>= 1;
        SCLK = 1;
        _nop_(); //数据在SCLK上升沿时，DS1302读写数据，
                 //在SCLK下降沿时，DS1302放置数据到IO上
        SCLK = 0;
        _nop_();
    }
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