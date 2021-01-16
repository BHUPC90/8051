#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//���ģ����û�м����������裬ʹ��ʱ��ý���2
sbit RST = P0 ^ 1;  //RET,ʹ���������ţ�����дʱ���ø�λ
sbit DAT = P0 ^ 2;  //IO˫��ͨ�����ţ���д���ݶ���ͨ��������
sbit SCLK = P0 ^ 3; //SCLK��ʱ���ź�

struct date_struct
{
    uint year;
    uint mounth;
    uint date;
    uint hour;
    uint minute;
    uint second;
} date_type;

void DS1302_WriteByte(uchar byte)
{
    for (n = 0; n < 8; n++)
    { //������
        DAT = byte & 0x01;
        byte >>= 1;
        SCLK = 1;
        _nop_(); //������SCLK������ʱ��DS1302��д���ݣ�
                 //��SCLK�½���ʱ��DS1302�������ݵ�IO��
        SCLK = 0;
        _nop_();
    }
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