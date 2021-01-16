#ifndef _INC_DS18B20
#define _INC_DS18B20
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DS18B20_pin = P2 ^ 4;

void init_ds18b20(void)
{
    bit initflag = 0;
    DS18B20_pin = 1;            //���Ƚ��������ø�
    Delay_OneWire(12); //������ʱ
    DS18B20_pin = 0;            //�������������͵�ƽ
    Delay_OneWire(80); //��ʱ��480~960US֮��
    DS18B20_pin = 1;            //���������ظߵ�ƽ
    Delay_OneWire(10); // ��ʱ�ȴ�
    initflag = DS18B20_pin;     // initflag����1��ʼ��ʧ��
    Delay_OneWire(5);  //���һ����ʱ
}