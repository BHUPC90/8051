#ifndef _INC_DHT11
#define _INC_DHT11
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DHT11_pin = P2 ^ 3;

uint temperature = 0;
uint humidity = 0;
uchar rec_dat[14];

//10us ��ȷ��ʱ
void DHT11_delay(uint n)
{
    while (--n)
        ;
}

//��ʼ�� DHT11
void DHT11_int()
{
    DHT11_pin = 1;
    DHT11_delay(2);
    DHT11_pin = 0;
    delay(20);
    DHT11_pin = 1;
    delay(30);
}

//��ȡ����8λ
uchar DHT11_read()
{
    uint i;
    uchar data1 = 0;
    for (i = 0; i < 8; i++) //�Ӹߵ��ͽ���8
    {                       //
        while (!DHT11_pin)  //
            ;               //
        DHT11_delay(8);     //��ʱ80us�������Ϊ��������Ϊ1������Ϊ0
        data1 <<= 1;        //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
        if (DHT11_pin == 1) //����Ϊ1ʱ��ʹdat��1����������1
            data1 += 1;     //
        while (DHT11_pin)   //
            ;               //�ȴ�����������
    }
}

//��ȡ�¶�
void DHT11_temp()
{
    uchar R_H, R_L, T_H, T_L, RH,  TH, revise;
    DHT11_int();

    if (DHT11_pin == 0)
    {
lcd_12864_text(IC_DAT2);
        while (DHT11_pin == 0)
            ;

        DHT11_delay(40);    //���ߺ���ʱ80us
        R_H = DHT11_read();    //����ʪ�ȸ߰�λ
        R_L = DHT11_read();    //����ʪ�ȵͰ�λ
        T_H = DHT11_read();    //�����¶ȸ߰�λ
        T_L = DHT11_read();    //�����¶ȵͰ�λ
        revise = DHT11_read(); //����У��λ

        DHT11_delay(25); //����

        if ((R_H + R_L + T_H + T_L) == revise) //У��
        {
            temperature = T_H;
            RH = R_H;
            humidity = R_H;
            TH = T_H;
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