#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

sbit DSIO = P2 ^ 4;
sbit RST = P2 ^ 5;
sbit SCLK = P2 ^ 3;

void DS1302_WriteByte(uchar byte)
{
    uint i;
    uint t = 0x01;
    for(i=0;i<8;i++){
        DSIO = byte & t;        
        t<<=1;
        DOWN();       //下降沿完成一个位的操作
    }
    SCLK = 1;//确保释放io引脚
}

/*------------------------------------
   Read函数
------------------------------------*/
uchar DS1302_ReadByte(uchar address)
{
    
}