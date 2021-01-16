#ifndef _INC_iic
#define _INC_iic
#define uchar unsigned char
#define uint unsigned int
#endif

sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;

//���255��һ����������Ϊ1us�������ʱ255us��
void Delay10us()
{
    unsigned char a, b;
    for (b = 1; b > 0; b--)
        for (a = 2; a > 0; a--)
            ;
}

void iic_init() //���߳�ʼ��
{
    SCL = 1;
    SDA = 1;
    Delay10us();
}

void iic_start() //�����ź�
{
    SDA = 1;
    Delay10us();
    SCL = 1;
    Delay10us(); //����ʱ����SDA����ʱ��>4.7us
    SDA = 0;
    Delay10us(); //����ʱ����>4us
    SCL = 0;
    Delay10us();
}

void iic_stop() //ֹͣ�ź�
{

    SDA = 0;
    Delay10us();
    SCL = 1;
    Delay10us(); //����ʱ�����4.7us
    SDA = 1;
    Delay10us();
}

void iic_sendByte(uchar dat) //mcu����һ���ֽ�
{
    uchar a = 0 ;
    for (a = 0; a < 8; a++) //Ҫ����8λ�������λ��ʼ
    {
        SDA = dat >> 7; //��ʼ�ź�֮��SCL=0�����Կ���ֱ�Ӹı�SDA�ź�
        dat = dat << 1;
        Delay10us();
        SCL = 1;
        Delay10us(); //����ʱ��>4.7us
        SCL = 0;
        Delay10us(); //ʱ�����4us
    }
    SDA = 1;
    Delay10us();
}

uchar iic_readByte() //��һ���ֽ�
{
    uchar a = 0, dat = 0;
    SDA = 1; //��ʼ�ͷ���һ���ֽ�֮��SCL����0
    Delay10us();
    for (a = 0; a < 8; a++) //����8���ֽ�
    {
        SCL = 1;
        Delay10us();
        dat <<= 1;
        dat |= SDA;
        Delay10us();
        SCL = 0;
        Delay10us();
    }
    return dat;
}

int iic_checkACK() //����Ӧ���ź�
{
    uchar b = 0 ;
    SDA = 1;
    SCL = 1;
    while (SDA) //�ȴ�Ӧ��Ҳ���ǵȴ����豸��SDA����
    {
        b++;
        if (b > 200) //�������2000usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
        {
            SCL = 0;
            Delay10us();
            return 0; //����ʧ��
        }
    }
    SCL = 0;
    return 1; //���ͳɹ�
}