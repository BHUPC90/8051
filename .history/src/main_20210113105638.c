#include "qi51arduino.h"
#include "lcd12864.h"
#include "huatu.h"
#include "lcd12864.h"

sbit IRIN = P3 ^ 2;


uchar code IC_DAT[] = {
    ""};

void IrInit()
{
    IT0 = 1;  //�½��ش���
    EX0 = 1;  //���ж�0����
    EA = 1;   //�����ж�
    IRIN = 1; //��ʼ���˿�
}

void setup()
{
    IrInit();
    lcd_12864_picture(firstpage);
}

void loop()
{
}
