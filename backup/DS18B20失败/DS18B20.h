#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 2;

int DS18B20_delay(uint wait_time) //10΢��
{
    do
    {
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    } while (--wait_time);
}

/*����DS18B20�����ֳ���*/
bit DS18B20_int()
{
    bit a;
    DS18B20_pin = 0; //׼����λ
    DS18B20_delay(50);
    DS18B20_pin = 1;
    DS18B20_delay(6); //��ʱ 60us
    a = DS18B20_pin;  //����Ƿ�����
    while (!DS18B20_pin)
        ; //�ȴ��ظ�
    return a;
}

/* �� DS18B20 д��һ���ֽ�*/
void DS18B20_write(uchar data1)
{
    uchar data2;
    for (data2 = 0x01; data2 != 0; data2 <<= 1) //��λ������� 8 λ
    {
        DS18B20_pin = 0; //���� 2us �͵�ƽ����
        _nop_();
        _nop_();
        if ((data2 & data1) == 0) //��� : & ������  1&1 = 1 1&0 = 0
            DS18B20_pin = 0;
        else
            DS18B20_pin = 1;
        DS18B20_delay(6); //��ʱ 60
        DS18B20_pin = 1;  //����ͨ������
    }
}

/* �� DS18B20 ��ȡһ���ֽ� */
uchar DS18B20_read()
{
    uchar data2;
    uchar mask;
    for (mask = 0x01; mask != 0; mask <<= 1) //��λ���ȣ����βɼ� 8 �� bit
    {
        DS18B20_pin = 0; //���� 2us �͵�ƽ����
        _nop_();
        _nop_();
        DS18B20_pin = 1; //�����͵�ƽ���壬�ȴ� 18B20 �������
        _nop_();         //��ʱ 2us
        _nop_();
        if (!DS18B20_pin) //��ȡͨ�������ϵ�ֵ
            data2 &= ~mask;
        else
            data2 |= mask;
        DS18B20_delay(6); //����ʱ 60us
    }
    return data2;
}

/* ��ȡ 18B20 �¶�ת�� */
bit DS18B20_inttemp()
{
    bit ack;                 //
    ack = DS18B20_int();     //ִ�����߸�λ������ȡ 18B20 Ӧ��
    if (ack == 0)            //�� 18B20 ��ȷӦ��������һ��ת��
    {                        //
        DS18B20_write(0xCC); //���� ROM ����
        DS18B20_write(0x44); //�����¶�ת��
    }                        //
    return ~ack;             //ack==0 ��ʾ�����ɹ������Է���ֵ����ȡ��
}

/* ��ȡ DS18B20 ת�����¶�ֵ*/
bit DS18B20_temp(int *temp)
{                                      //
    bit ack;                           //
    uchar LSB, MSB;                    //16bit �¶�ֵ�ĵ��ֽں͸��ֽ�
    ack = DS18B20_int();               //ִ�����߸�λ������ȡ 18B20 Ӧ��
    if (ack == 0)                      //�� 18B20 ��ȷӦ�����ȡ�¶�ֵ
    {                                  //
        DS18B20_write(0xCC);           //���� ROM ����
        DS18B20_write(0xBE);           //���Ͷ�����
        LSB = DS18B20_read();          //���¶�ֵ�ĵ��ֽ�
        MSB = DS18B20_read();          //���¶�ֵ�ĸ��ֽ�
        *temp = ((int)MSB << 8) + LSB; //�ϳ�Ϊ 16bit ������
    }                                  //
    return ~ack;                       //ack==0 ��ʾ����Ӧ�����Է���ֵΪ��ȡ��ֵ
}