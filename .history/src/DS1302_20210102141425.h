#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//���ģ����û�м����������裬ʹ��ʱ��ý���2
sbit RST = P0 ^ 1;   //RET,ʹ���������ţ�����дʱ���ø�λ
sbit DATIO = P0 ^ 2; //IO˫��ͨ�����ţ���д���ݶ���ͨ��������
sbit SCLK = P0 ^ 3;  //SCLK��ʱ���ź�

struct date_struct
{
    uint year;
    uint mounth;
    uint date;
    uint hour;
    uint minute;
    uint second;
} date_type;

//����һ��8λ�������ַ�������ǰ����дʱ��
void DS1302_WriteByte(uchar byte)
{
    for (int n = 0; n < 8; n++)
    {
        DATIO = byte & 0x01;
        byte >>= 1;
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
}

//��DS1302��������ݣ���ַ+���ݣ�
void Ds1302Write(uchar addr, uchar dat)
{
    uchar n;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    DS1302_WriteByte(addr);
    DS1302_WriteByte(dat);
    RST = 0;
    _nop_();
}

//��ȡһ�����ַ� 8λ������
uchar DS1302_Read(uchar address)
{
    uchar dat1, dat;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    DS1302_WriteByte(address);
    _nop_();
    for (uint r = 0; r < 8; r++)
    {
        dat1 = DATIO;
        dat = (dat >> 1) | (dat1 << 7);
        SCLK = 1;
        _nop_();
        SCLK = 0;
        _nop_();
    }
    RST = 0;
    _nop_();
    SCLK = 1;
    _nop_();
    DATIO = 0;
    _nop_();
    DATIO = 1;
    _nop_();
    return dat;
}

void int_ds1302()
{
}