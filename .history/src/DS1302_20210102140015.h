#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//���ģ����û�м����������裬ʹ��ʱ��ý���2
sbit CE = P2 ^ 5;   //RET,ʹ���������ţ�����дʱ���ø�λ
sbit SCIO = P2 ^ 4; //IO˫��ͨ�����ţ���д���ݶ���ͨ��������
sbit SCLK = P2 ^ 3; //SCLK��ʱ���ź�

typedef struct
{
    int a;
    char b;
    double c; 
} Simple2;

uchar date_year;
uchar mounth;
uchar date;
uchar hour;
uchar minute;
uchar second;

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