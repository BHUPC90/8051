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

uint int_ds18b20(void)
{
    uint reback = 0;      //��֤�Ƿ�����
    DS18B20_pin = 1;      //���Ƚ��������ø�
    Delay_ds18b20(12);    //������ʱ
    DS18B20_pin = 0;      //�������������͵�ƽ
    Delay_ds18b20(80);    //��ʱ��480~960US֮��
    DS18B20_pin = 1;      //���������ظߵ�ƽ
    Delay_ds18b20(10);    // ��ʱ�ȴ�
    reback = DS18B20_pin; // initflag����1��ʼ��ʧ��
    Delay_ds18b20(5);     //���һ����ʱ
    return reback;
}

//��DS18B20дһ���ֽ�

void Write_DS18B20(uint dat)
{
    uint i;
    for (i = 0; i < 8; i++) //�ظ�ֱ�������������ֽ�
    {
        DS18B20_pin = 0; //��������������
        DS18B20_pin = dat & 0x01;
        Delay_ds18b20(5); //��ʱ45us
        DS18B20_pin = 1;  //�����������ظߵ�ƽ1
        dat >>= 1;
    }
    Delay_ds18b20(5);
}

//��DS18B20��ȡһ���ֽ�

uchar Read_DS18B20(void)
{
    uint i;
    uchar dat;
    for (i = 0; i < 8; i++) //�ظ�����ֱ�����������ֽ�
    {
        DS18B20_pin = 0; //����������
        dat >>= 1;       //��һλ
        DS18B20_pin = 1; //����������
        if (DS18B20_pin)
            dat |= 0x80; //1000 0000
        Delay_ds18b20(5);
    }
    return dat;
}

void DS18B20_Read_id(void)
{
}
//docker run -d -p 9000:9000 -v /var/run/docker.sock:/var/run/docker.sock -v portainer_data:/data -v /public:/public portainer/portainer
char DS18B20_Read_tem(void)
{
    unsigned char low_data, high_data;
    char temp;
    Int_DS18B20();              //��ʼ��
    Write_DS18B20(0xCC);        //����64λROM��ַ��ֱ����DS18B20�����¶ȱ任���������һ���ӻ�����
    Write_DS18B20(0x44);        //�¶�ת��
    Delay_ds18b20(200);         //2ms
    Int_DS18B20();              //��ʼ��
    Write_DS18B20(0xCC);        //�����豸ѡȡ
    Write_DS18B20(0xBE);        //��ȡ�¶�����
    low_data = Read_DS18B20();  //�Ͱ�λ
    high_data = Read_DS18B20(); //�߰�λ
    temp = high_data << 4;      //���ϳ�һ���ֽ�λ��
    temp |= (low_data >> 4);    //�ϲ�
    return temp;
}