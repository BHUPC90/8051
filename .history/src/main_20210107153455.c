#include "qi51arduino.h"
#include "lcd12864.h"
#include "SR04.h"

uchar code IC_DAT[] = {
    "����С����Գ������ʵ��ȫ����ʾ������ʾ�����ţ���������"};
uchar code IC_DAT[] = {
    "��������࣬��λ������"};

void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
    SR04_Init();
}

void loop()
{
    uchar temp[9];
    uint longl;
    while (pin21 == 1)
        ;
    lcd_12864_write_com(0x00);
    longl = SR04_GetOnce();
    temp[4] = '0' + (longl / 1000) % 10;
    temp[5] = '0' + (longl / 100) % 10;
    temp[6] = '0' + (longl / 10) % 10;
    temp[7] = '0' + longl % 10;
    temp[8] = '\0';
    lcd_12864_len_text(1, temp);
}