#ifndef _INC_SR04
#define _INC_SR04
#define uchar unsigned char
#define uint unsigned int
#endif
//���Ծ��� = ( �ߵ�ƽʱ�� * ����(340M/S) ) / 2;

sbit ECHO_port = P3 ^ 6;
sbit TRIG_port = P3 ^ 7;

void delay(unsigned int wait_time);
void delaywhile(unsigned int wait_time); //1ms ??

void StartSR04() //����ģ��
{
    TRIG_port = 0;
    delaywhile(1);
    TRIG_port = 1; //����һ��ģ��
    delaywhile(2);
    TRIG_port = 0;
}

void SR04_Init()
{
    TMOD |= 0x01;
    TH0 = 0;
    TL0 = 0;
}

uint SR04_GetDistance()
{
    unsigned int time = TH0 * 256 + TL0; //�õ���ʱ�䣬��λus
    uint distance = time * 0.017;
    TH0 = 0;
    TL0 = 0; //��ն�ʱ��
    return distance;
}

uint SR04_GetOnce() //�õ�һ�γ��������ģ��ľ���
{
    StartSR04(); //TRIG����һ��֪ͨ������ģ��
    while (!ECHO_port)
        ;    //��EchoΪ��ʱ���������ӵ���Ϣ����࿪ʼ���ȴ�
    TR0 = 1; //�����������򿪶�ʱ��0
    while (ECHO_port)
        ;                     //��EchoΪ1�������ȴ�
    TR0 = 0;                  //�رռ������رն�ʱ��0
    return SR04_GetDistance(); //����
}
