#ifndef _INC_DHT11
#define _INC_DHT11
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DHT11_pin = P2 ^ 3;

uint temperature = 0;
uint humidity = 0;

//10us ��ȷ��ʱ

void DHT11_delay_us(uchar t)
{
    while (--t)
        ;
}

void DHT11_delay_ms(uint t)
{
    uint i, j;
    for (i = t; i > 0; i--)
        for (j = 110; j > 0; j--)
            ;
}

//��ʼ�� DHT11
void DHT11_int()
{
    DHT11_pin = 1;
    DHT11_delay_us(2);
    DHT11_pin = 0;
    DHT11_delay_ms(20); //��ʱ18ms����
    DHT11_pin = 1;
    DHT11_delay_us(30);
}

//��ȡ����8λ
uchar DHT11_read()
{
    uchar i, dat = 0;
    for (i = 0; i < 8; i++) //�Ӹߵ������ν���8λ����
    {
        while (!DHT11_pin)
            ;
        DHT11_delay_us(8);  //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0
        dat <<= 1;          //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
        if (DHT11_pin == 1) //����Ϊ1ʱ��ʹdat��1����������1
            dat += 1;
        while (DHT11_pin)
            ; //�ȴ�����������
    }
    return dat;
}

//��ȡ�¶�
void DHT11_temp()
{
    uchar R_H, R_L, T_H, T_L, RH, RL, TH, TL, revise;
    DHT11_int();

    if (DHT11_pin == 0)
    {

        while (DHT11_pin == 0)
            ; //�ȴ�����

        DHT11_delay_us(40);    //���ߺ���ʱ80us
        R_H = DHT11_read();
        R_L = DHT11_read(); 
        T_H = DHT11_read();
        T_L = DHT11_read();  
        revise = DHT11_read(); //У��λ

        DHT11_delay_us(25); //����

        if ((R_H + R_L + T_H + T_L) == revise) //У��
        {
            RH = R_H;
            RL = R_L;
            TH = T_H;
            TL = T_L;
            temperature = TH;
            humidity = RH;
        }

    }
}