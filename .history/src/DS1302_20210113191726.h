#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

uint read[] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};  //�������ʱ����������Ĵ���
uint write[] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c}; //�������ʱ��������д�Ĵ���
uchar time[] = {0,0,0,0,0,0,0};                                        //��ų�ʼʱ��
uchar nao_time[] = {0,0};  

sbit RST = P3 ^ 5;
sbit DSIO = P3 ^ 4;
sbit SCLK = P3 ^ 6;

//��DS1302����һ���ֽ� 8λ������
void Ds1302WriteByte(uchar byte)
{
    uint n;
    for (n = 0; n < 8; n++)
    { //λ�Ʒ�������
        DSIO = byte & 0x01;
        byte >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
}

//��DS1302��ȡһ���ֽ� 8λ������
uchar Ds1302ReadByte()
{
    uint r;
    uchar dat1, dat;
    for (r = 0; r < 8; r++)
    {
        dat1 = DSIO;
        dat = (dat >> 1) | (dat1 << 7);
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    return dat;
}

//��DS1302���� 2�ֽڣ���ַ+���ݣ�
void Ds1302Write(uchar addr, uchar dat)
{
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    Ds1302WriteByte(addr);
    Ds1302WriteByte(dat);
    RST = 0;
}

//��ȡDS1302��Ϣ����ַ��
uchar Ds1302Read(uchar addr)
{
    uchar r, dat;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    Ds1302WriteByte(addr);
    _nop_();
    dat = Ds1302ReadByte();
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

void Ds1302readTime() //��ȡʱ����Ϣ
{
    uchar n;
    for (n = 0; n < 8; n++)
    {
        time[n] = Ds1302Read(read[n]);
    }
}

void ds1302init() //�ڼĴ�����д���ʼʱ��
{
    uchar i;
    Ds1302Write(0x8e, 0x00); //�ر�д�����Ĵ���
    for (i = 0; i < 7; i++)
    {
        Ds1302Write(write[i], time[i]);
    }
    Ds1302Write(0x8e, 0x80); //��д�����Ĵ���
}