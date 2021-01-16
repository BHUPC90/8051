#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

//���ģ����û�м����������裬ʹ��ʱ��ý���2
sbit CE = P2 ^ 5;
;                   //RET,ʹ���������ţ�����дʱ���ø�λ
sbit SCIO = P2 ^ 4; //IO     ��˫��ͨ�����ţ���д���ݶ���ͨ��������
sbit SCLK = P2 ^ 3; //SCLK��ʱ���ź�

void DS1302_WriteByte(uchar byte)
{
    uint i;
    uint t = 0x01;
    for (i = 0; i < 8; i++)
    {
        DSIO = byte & t;
        t <<= 1;
        DOWN(); //�½������һ��λ�Ĳ���
    }
    SCLK = 1; //ȷ���ͷ�io����
}

/*------------------------------------
   Read����
------------------------------------*/
uchar DS1302_ReadByte(uchar address)
{
}

void init_ds1302()
{
    uchar i;
    CE = 0; //��ʼ������
    SCLK = 0;
    i = ds1302_readdata(0x00); //��ȡ��Ĵ���,�������λ
    if ((i & 0x80 != 0))
    {
        ds1302_writedata(7, 0x00); //����д����������д������
        for (i = 0; i < 7; i++)
        {
            ds1302_writedata(i, init_time[i]);
        }
    }
}