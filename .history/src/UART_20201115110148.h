#ifndef _INC_UART
#define _INC_UART
#define uchar unsigned char
#define uint unsigned int
#endif
void delay(unsigned int wait_time);
void Serial_begin(uint speed);
void Serial_print_byte(uchar print_date);
void Serial_print(uchar print_date);

int Serial_available = 0;
uchar re_data = NULL;
/*
���Դ���ͨ�ţ���ʱ��1��ͻ
�����ʣ�253 -- 9600 ������㣩
*/
void Serial_begin(uint speed)
{
    EA = 1;  //����CPU�����ж�
    ES = 1;  //�򿪴����ж�
    REN = 1; //�򿪴��ڽ���
    //////////////////////////////////////////////////////////////////////////////////
    TMOD = 0x20; //��ʱ��1����ģʽ
    TR1 = 1;     //�򿪶�ʱ��1�ж�
    TH1 = speed; //���ô��ڲ�����
    TL1 = speed;
    /*  //////////////////////////////////////////////////////////////////////////////
        SM0=0&SM1=0��ģʽ0����λ�Ĵ�������I/O�������ʹ̶�Ϊ����Ƶ��/12��
        SM0=0&SM1=1��ģʽ1��8λ�������ݴ��ͣ��������ɼ�ʱ��1�����ơ�
        SM0=1&SM1=0��ģʽ2��9λ�������ݴ��ͣ������ʿɷ�Ϊ2�֣�����Ƶ��/32����Ƶ��/64��
        SM0=1&SM1=1��ģʽ3��9λ�������ݴ��ͣ������ʿ��ü�ʱ��1���ơ�
    */
    SM0 = 0;
    SM1 = 1;
    /////////////////////////////////////////////////////////////////////////////////
    RI = 0; //��ս��ձ�ʶ
    TI = 0; //��շ��ͱ�ʶ
}

/*����һ���ַ�*/
void Serial_print_byte(uchar print_date)
{
    if (RI == 0)
    {
        SBUF = print_date; //�����ݷŵ�SBUF��
        while (TI == 0)
            ;   //�ȴ�����
        TI = 0; //������Ϻ�������ͱ�ʶ
    }
}

//��ӡ�ַ���
void Serial_print(uchar print_date[])
{
    int num = 0;
    while (1)
    {
        if (print_date[num] == '\0')
        {
            break;
        }
        Serial_print_byte(print_date[num]);
        num++;
        delay(2);
    }
}

void Serial_have_read() interrupt 4
{
    if (TI == 0)
    {
        ES = 0;               //��ʱ�رմ����ж�
        re_data = SBUF;       //��������
        Serial_available = 1; //���ô��ڱ�ʶ��������
        RI = 0;               //��������жϱ�־λ
        ES = 1;               //�򿪴����ж�
    }
}