#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//���ģ����û�м����������裬ʹ��ʱ��ý���2
sbit CE = P2 ^ 6;   //RET,ʹ���������ţ�����дʱ���ø�λ
sbit SCIO = P2 ^ 5; //IO˫��ͨ�����ţ���д���ݶ���ͨ��������
sbit SCLK = P2 ^ 4; //SCLK��ʱ���ź�

struct date_struct
{
    uint year;
    uint mounth;
    uint date;
    uint hour;
    uint minute;
    uint second;
}
date_type;

void DS1302_WriteByte(uchar byte)
{
}

/*------------------------------------
   Read����
------------------------------------*/
uchar DS1302_ReadByte(uchar address)
{
}

void int_ds1302()
{
}