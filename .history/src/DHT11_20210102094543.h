#ifndef _INC_DHT11
#define _INC_DHT11
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DHT11_pin = P2 ^ 3;

uint temperature = 0;
uint humidity = 0;
uchar rec_dat[15];

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

        DHT11_delay_us(40);        //���ߺ���ʱ80us
        R_H = DHT11_rec_byte();    //����ʪ�ȸ߰�λ
        R_L = DHT11_rec_byte();    //����ʪ�ȵͰ�λ
        T_H = DHT11_rec_byte();    //�����¶ȸ߰�λ
        T_L = DHT11_rec_byte();    //�����¶ȵͰ�λ
        revise = DHT11_rec_byte(); //����У��λ

        DHT11_delay_us(25); //����

        if ((R_H + R_L + T_H + T_L) == revise) //У��
        {
            RH = R_H;
            RL = R_L;
            TH = T_H;
            TL = T_L;
        }
        /*���ݴ���������ʾ*/
        rec_dat[0] = '0' + (RH / 10);
        rec_dat[1] = '0' + (RH % 10);
        rec_dat[2] = ' ';
        rec_dat[3] = 'R';
        rec_dat[4] = 'H';
        rec_dat[5] = ' ';
        rec_dat[6] = ' ';
        rec_dat[7] = ' ';
        rec_dat[8] = ' ';
        rec_dat[9] = '0' + (TH / 10);
        rec_dat[10] = '0' + (TH % 10);
        rec_dat[11] = ' ';
        rec_dat[12] = 'C';
        rec_dat[13] = ' ';
        rec_dat[14] = ' ';
    }
}