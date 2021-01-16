#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

sbit DSIO = P2 ^ 4;
sbit RST = P2 ^ 5;
sbit SCLK = P2 ^ 3;

void Ds1302Write(uchar addr, uchar dat) //��DS1302�����ַ+���ݣ�
{
    uchar n;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    for (n = 0; n < 8; n++)
    { //��ַ����
        DSIO = addr & 0x01;
        addr >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    for (n = 0; n < 8; n++)
    { //������
        DSIO = dat & 0x01;
        dat >>= 1;
        SCLK = 1;
        _nop_(); //������SCLK������ʱ��DS1302��д���ݣ�
                 //��SCLK�½���ʱ��DS1302�������ݵ�IO��
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
        _nop_(); //������SCLK������ʱ��DS1302��д���ݣ�
                 //��SCLK�½���ʱ��DS1302�������ݵ�IO��
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