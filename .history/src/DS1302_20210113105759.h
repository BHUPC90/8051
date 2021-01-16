#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//���ģ����û�м����������裬ʹ��ʱ��ý���2
sbit RST = P0 ^ 0;   //RET,ʹ���������ţ�����дʱ���ø�λ
sbit DATIO = P0 ^ 1; //IO˫��ͨ�����ţ���д���ݶ���ͨ��������
sbit SCLK = P3 ^ 6;  //SCLK��ʱ���ź�

uchar code READ_RTC_ADDR[7]=
        {0x81,0x83,0x85,0x87,0x89,0x8B,0x8D};
uchar code WEITE_RTC_ADDR[7]=
        {0x80,0x82,0x84,0x86,0x88,0x8A,0x8C};
/*
DS1302ʱ�ӳ�ʼ��2018��5��13��������23��00��00�롣
�洢˳�������ʱ��������,�洢��ʽ����BCD��
*/
uchar TIME[7]={0,0,0x23,0x013,0x05,0x06,0x18};

uchar rec_dat[8];

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
    uint n = 0;
    for (n = 0; n < 8; n++)
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
    uint r = 0;
    RST = 0;
    _nop_();
    SCLK = 0;
    _nop_();
    RST = 1;
    _nop_();
    DS1302_WriteByte(address);
    _nop_();
    for (r = 0; r < 8; r++)
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

void Ds1302Init()
{
    uchar n;
     Ds1302Write(0x8e,0x00);//��ֹд����
     for(n=0;n<8;n++){
          Ds1302Write(WEITE_RTC_ADDR[n],TIME[n]);
    }
          Ds1302Write(0x8e,0x80);//��д����
}

void Ds1302readTime() //��ȡʱ����Ϣ
{
    uchar n;
    for (n = 0; n < 8; n++)
    {
        TIME[n] = DS1302_Read(READ_RTC_ADDR[n]);
    }

    date_type.hour = (TIME[2] / 16) * 10 + 0x34 % TIME[2];
    date_type.minute = (TIME[1] / 16) * 10 + 0x34 % TIME[1];
    date_type.second = (TIME[0] / 16) * 10 + 0x34 % TIME[0];

    rec_dat[0] = '0' + TIME[2] / 16;
    rec_dat[1] = '0' + date_type.hour % 10;
    rec_dat[2] = ':';
    rec_dat[3] = '0' + TIME[1] / 16;
    rec_dat[4] = '0' + date_type.minute % 10;
    rec_dat[5] = ':';
    rec_dat[6] = '0' + TIME[0] / 16;
    rec_dat[7] = '0' + date_type.second % 10;
}