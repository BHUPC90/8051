#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

sbit DSIO = P2 ^ 4;
sbit RST = P2 ^ 5;
sbit SCLK = P2 ^ 3;

void DS1302_WriteByte(uchar address, uchar byte)
{
    uchar i;
    address = address & 0xfe; //R/W控制位低电位写
    DS_RST_SET;
    for (i = 0; i < 8; i++) //发送地址,找寄存器
    {
        if (address & 0x01)
            DS_IO_SET;
        else
            DS_IO_CLR;
        address >>= 1;
        DS_SCL_SET;
        DS_SCL_CLR;
    }
    for (i = 0; i < 8; i++) //发送数据到寻找到的寄存器
    {
        if (byte & 0x01)
            DS_IO_SET;
        else
            DS_IO_CLR;
        byte >>= 1;
        DS_SCL_SET;
        DS_SCL_CLR;
    }
    DS_RST_CLR;
}

/*------------------------------------
   Read函数
------------------------------------*/
uchar DS1302_ReadByte(uchar address)
{
    UINT8 i, byte;
    address = address | 0x01;
    DS_RST_SET;
    for (i = 0; i < 8; i++) //发送地址,找到寄存器
    {
        if (address & 0x01)
            DS_IO_SET;
        else
            DS_IO_CLR;
        address >>= 1;
        DS_SCL_SET;
        DS_SCL_CLR;
    }
    for (i = 0; i < 8; i++) //从找到的寄存器中读取数据
    {
        byte >>= 1;
        if (DS_IO)
            byte |= 0x80;
        DS_SCL_SET;
        DS_SCL_CLR;
    }
    DS_RST_CLR;
    return byte;
}