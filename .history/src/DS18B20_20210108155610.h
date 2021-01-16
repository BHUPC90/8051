#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 4;

void Delay_ds18b20(uint t) //ר�ó���ȷ��ʱ10us
{
    int i;
    while (t--)
    {
        for (i = 0; i < 12; i++)
            ;
    }
}

uint int_ds18b20()
{
    int i;
    DS18B20_pin = 0; //�������ߣ�������λ�ź�
    i = 80;
    while (i > 0)
        i--;         //��ʱ480~960us
    DS18B20_pin = 1; //��������
    i = 4;
    while (i > 0)
        i--; //��ʱ15~60us
    while (DS18B20_pin)
        ; //�ȴ�����Ӧ������
    i = 70;
    while (i > 0)
        i--; //��ʱ����480us
}

//��DS18B20дһ���ֽ�

void Write_DS18B20(uint dat)
{
    int i, j;
    bit testb;
    for (j = 8; j > 0; j--)
    {
        testb = dat & 0x01;
        dat = dat >> 1;
        if (testb) //д1
        {
            DS18B20_pin = 0; //�������ߣ�����дʱ��϶
            i++;             //��ʱ����1us
            DS18B20_pin = 1; //��������
            i = 8;
            while (i > 0)
                i--; //��ʱ����60us����DS18B20����
        }
        else //д0
        {
            DS18B20_pin = 0; //�������ߣ�����дʱ��϶
            i = 8;
            while (i > 0)
                i--;         //��������60us����DS18B20����
            DS18B20_pin = 1; //��������
            i++;
            i++;
        }
    }
}

//��DS18B20��ȡһ���ֽ�

uchar Read_DS18B20()
{
    int i, j;
    bit b; //�����Ž��յ���1���ֽ�
    uchar i_b;
    uchar rdbyte;
    for (j = 8; j > 0; j--)
    {
        DS18B20_pin = 0; //�������ߣ�������ʱ϶
        i++;             //��ʱ����1us
        DS18B20_pin = 1; //�ͷ�����
        i++;
        i++;             //��һ��ʱ���������ͷ�
        b = DS18B20_pin; //��ȡ����
        i = 8;
        while (i > 0)
            i--; //��ʱ����60us
        i_b = b;
        rdbyte = (i_b << 7) | (rdbyte >> 1); //����ȡ����һλֵ����7λ����Ŷ�ȡ�����ݱ���rdbyte����1λ
    }
    return rdbyte;
}

void DS18B20_Read_id()
{
}
//docker run -d -p 9000:9000 -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data -v /file/portainer_web:/public portainer/portainer:1.20.2

uint DS18B20_Read_tem()
{
    uchar low_data, high_data;
    uint tflag;
    uint temp;
    Int_DS18B20();              //��ʼ��
    Write_DS18B20(0xCC);        //���Զ�ȡ64λROM��ַ��ֱ����DS18B20�����¶ȱ任���������һ���ӻ�����
    Write_DS18B20(0x44);        //��ʾ���� ds18b20 �¶�ת�� , ������д��� 9 �ֽڵ� RAM��
    Delay_ds18b20(200);         //2ms
    Int_DS18B20();              //��ʼ��
    Write_DS18B20(0xCC);        //�����豸ѡȡ
    Write_DS18B20(0xBE);        //��ȡ�¶�����
    low_data = Read_DS18B20();  //�Ͱ�λ
    high_data = Read_DS18B20(); //�߰�λ
    temp = high_data << 4;      //���ϳ�һ���ֽ�λ��
    temp |= (low_data >> 4);    //�ϲ�

    temp = temp | low_data; //��� �� low_data ���а�λ��
    if (temp < 0x0fff)      // 00001111 11111111
    {                       //
        tflag = 0;          // ǰ��λΪ 0 ʱ����ȡ���¶�Ϊ������־λΪ 0����ʱֻҪ
                            // �������ֵ���� 0.0625 ���ɵõ�ʵ���¶�
    }                       //
    else                    // ǰ��λΪ 1 ʱ����ȡ���¶�Ϊ������־λΪ 1����ʱ��Ҫ
    {                       // �������ֵȡ�����ټ� 1���ٳ��� 0.0625 ���ɵõ�ʵ���¶�
        tflag = 1;          // ��ʱ��ʾ���¶�
        temp = ~temp + 1;
    }
    temp = temp * (0.625); // �¶�ֵ���� 10 ������ȷ�� 1 λС��
    return temp;
}
