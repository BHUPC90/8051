#ifndef _INC_iic
#define _INC_iic
#define uchar unsigned char
#define uint unsigned int
#endif

sbit sda = P2 ^ 0;
sbit scl = P2 ^ 1;

void iic_init() //���߳�ʼ��
{
    scl = 1;
    sda = 1;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

void iic_start() //�����ź�
{
    scl = 1;
    _nop_(); //׼��״̬
    sda = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda��sclͬΪ�ߵ�ƽ����4.7us����

    _nop_(); //1.085us,��5.78us,����sda=0���½���,���ܼ�������ʱʱ����
    sda = 0; //�½���

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda�͵�ƽ����4us����        ,������4.34us����Ҫ��
}

void iic_stop() //ֹͣ�ź�
{

    scl = 1;
    _nop_(); //׼��״̬
    sda = 0;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //��״̬�ȶ�ʱ��Ҫ�󱣳�4us����

    sda = 1; //scl�ߵ�ƽ�ڼ�,sda��һ��������

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda����4.7us����,4.34���Ϻ�������ʱ�����4.7us
             //ע:��ʱscl��sda��Ϊ1
}

void iic_sendByte(uchar byteData) //mcu����һ���ֽ�
{
    uchar i;
    uchar temp = byteData;
    for (i = 0; i < 8; i++)
    {
        temp = temp << 1; //�ƶ������λ����PSW�Ĵ�����CYλ��,����Ĵ���
        scl = 0;          //׼��
        _nop_();          //�ȶ�һ��
        sda = CY;         //�������͵�����һλλ�ķŵ�sda��
        _nop_();
        scl = 1; //ÿһ���ߵ�ƽ�ڼ�,ic�������Ὣ����ȡ��
        _nop_();
    }

    scl = 0; //���д��scl=1;sda=1����ֹͣ�ź�,������ôд
    _nop_();
    sda = 1; //�ͷ�����,�������߲���ʱҪ�ͷ�
    _nop_();
}

uchar iic_readByte() //��һ���ֽ�
{
    uchar i, temp;
    scl = 0; //׼��������
    _nop_();
    sda = 1; //�ͷ�����
    _nop_();

    for (i = 0; i < 8; i++)
    {
        scl = 1; //mcu��ʼȡ����

        _nop_();
        _nop_();
        _nop_();
        _nop_();                  //sclΪ�ߵ�ƽ��,ic�����ͻὫ1λ�����͵�sda��
                                  //�ܹ���ʱ�������4.34us��,Ȼ��Ϳ�����mcu��sda��
        temp = (temp << 1) | sda; //��һλ���浽temp��
        scl = 0;

        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
    return temp;
}

int iic_checkACK() //����Ӧ���ź�
{
    uchar errCounts = 255; //���峬ʱ��Ϊ255��
    scl = 1;
    _nop_();

    while (sda)
    { //��һ��ʱ���ڼ�⵽sda=0�Ļ���Ϊ��Ӧ���ź�
        if (0 == errCounts)
        {
            scl = 0; //ǯס����
            _nop_();
            return 0; //û��Ӧ���ź�
        }
        errCounts--;
    }

    scl = 0; //ǯס����,Ϊ��1��ͨ����׼��
    _nop_();
    return 1; //�ɹ�����Ӧ���ź�
}

void iic_sendACK(int b_ACK) //����Ӧ����Ӧ���ź�
{
    scl = 0; //׼��
    _nop_();

    if (b_ACK) //ACK
    {
        sda = 0;
    }
    else //unACK
    {
        sda = 1;
    }

    _nop_();
    scl = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //����4us����ʱ

    scl = 0; //ǯסscl,�Ա������������
    _nop_();
}