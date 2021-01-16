#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 4;

uint init_ds18b20(void)
{
    uint reback = 0;      //
    DS18B20_pin = 1;      //���Ƚ��������ø�
    Delay_OneWire(12);    //������ʱ
    DS18B20_pin = 0;      //�������������͵�ƽ
    Delay_OneWire(80);    //��ʱ��480~960US֮��
    DS18B20_pin = 1;      //���������ظߵ�ƽ
    Delay_OneWire(10);    // ��ʱ�ȴ�
    reback = DS18B20_pin; // initflag����1��ʼ��ʧ��
    Delay_OneWire(5);     //���һ����ʱ
    return reback;
}

//��DS18B20дһ���ֽ�

void Write_DS18B20(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++) //�ظ�ֱ�������������ֽ�
    {
        DS18B20_pin = 0; //��������������
        DS18B20_pin = dat & 0x01;
        Delay_OneWire(5); //��ʱ45us
        DS18B20_pin = 1;           //�����������ظߵ�ƽ1
        dat >>= 1;
    }
    Delay_OneWire(5);
}

//��DS18B20��ȡһ���ֽ�

unsigned char Read_DS18B20(void)
{
    unsigned char i;
    unsigned char dat;
    for (i = 0; i < 8; i++) //�ظ�����ֱ�����������ֽ�
    {
        DS18B20_pin = 0; //����������
        dat >>= 1;
        DS18B20_pin = 1; //����������
        if (DS18B20_pin)
        {
            dat |= 0x80;
        }
        Delay_OneWire(5);
    }
    return dat;
}