#include "qi51arduino.h"
#include "UART.h"
//���ڿ������ʰ�ť���ƺ�����ˮ��
void openled(int led, int state);
int mod = 0;
uint set_pwm = 0;
uint pwm;
uint led_switch = 1;

void setup()
{

    TMOD = 0x01; //����ģʽ1��16λ��ʱ �� 8��8��
    EA = 1;      //����CPU�����ж�
    TH0 = 255;   //(65536 - 10) /256 0.1ms    ����ȡ��������� http://www.ab126.com/shuxue/2892.html
    TL0 = 246;   //(65536 - 10) %256 0.1ms
    ET0 = 1;     //�򿪶�ʱ��0�ж�
    TR0 = 1;     //���붨ʱ������

    EX0 = 1; //����EX0�����жϣ��ⲿ�ж�0,P3.2��
    IT0 = 1; // �����жϴ�����ʽ 0����ʱ�䴥����1��ƽ�任ʱ�������ʺϰ�ť

    pin1 = 1;
    pin2 = 1;
    pin3 = 1;
    pin4 = 1;
    Serial_begin(253);
}
void loop()
{
    while (set_pwm < 100)
    {
        set_pwm += speed;
        delay(1);
    }
    if (mod)
    {
        led_switch++;
        if (led_switch >= 5)
        {
            led_switch = 1;
        }
    }
    else
    {
        led_switch--;
        if (led_switch <= 0)
        {
            led_switch = 5;
        }
    }
    Serial_print_byte(led_switch + 64);
    Serial_available = 0;

    while (set_pwm > 0)
    {
        set_pwm -= speed;
        delay(1);
    }
}
void timervoid() interrupt 1
{
    TH0 = 255;
    TL0 = 246;
    if (pwm >= 100)
    {
        pwm = 0;
    }
    if (pwm < set_pwm)
    {
        openled(led_switch, 1);
    }
    else
    {
        openled(led_switch, 0);
    }
    pwm++;
}
//�ⲿ�ж˳���ע���пӣ�0��EX0�ⲿ�ж�0��2��EX1�ⲿ�ж�һ
void zhongduan0() interrupt 0
{
    mod = !mod;
}

void openled(int led, int state)
{
    switch (led)
    {
    case 1:
        pin1 = state;
        break;
    case 2:
        pin2 = state;
        break;
    case 3:
        pin3 = state;
        break;
    case 4:
        pin4 = state;
        break;
    }
}