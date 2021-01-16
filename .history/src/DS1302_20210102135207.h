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
    address = address & 0xfe; //R/W����λ�͵�λд
    DS_RST_SET;
    for (i = 0; i < 8; i++) //���͵�ַ,�ҼĴ���
    {
        if (address & 0x01)
            DS_IO_SET;
        else
            DS_IO_CLR;
        address >>= 1;
        DS_SCL_SET;
        DS_SCL_CLR;
    }
    for (i = 0; i < 8; i++) //�������ݵ�Ѱ�ҵ��ļĴ���
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
   Read����
------------------------------------*/
uchar DS1302_ReadByte(uchar address)
{
    UINT8 i, byte;
    address = address | 0x01;
    DS_RST_SET;
    for (i = 0; i < 8; i++) //���͵�ַ,�ҵ��Ĵ���
    {
        if (address & 0x01)
            DS_IO_SET;
        else
            DS_IO_CLR;
        address >>= 1;
        DS_SCL_SET;
        DS_SCL_CLR;
    }
    for (i = 0; i < 8; i++) //���ҵ��ļĴ����ж�ȡ����
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