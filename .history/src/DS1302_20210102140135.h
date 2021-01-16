#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//这个模块内没有集成上拉电阻，使用时最好接上2
sbit CE = P2 ^ 5;   //RET,使能输入引脚，当读写时，置高位
sbit SCIO = P2 ^ 4; //IO双向通信引脚，读写数据都是通过这个完成
sbit SCLK = P2 ^ 3; //SCLK，时钟信号

typedef date_struct
{
    uint year;
    uint mounth;
    uint date;
    uint hour;
    uint minute;
    uint second;
}
date_int;

date_int

void DS1302_WriteByte(uchar byte)
{
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