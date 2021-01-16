#include "qi51arduino.h"
#include "lcd12864.h"
#include "SR04.h"

uchar code IC_DAT[] = {
    "����С����Գ������ʵ��ȫ����ʾ������ʾ�����ţ���������"};
uchar code IC_DAT_2[] = {
    "��������࣬��λ������"};

void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
    SR04_Init();
    while (pin22 == 1)
        ;
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, IC_DAT_2);
}

void loop()
{
    uchar temp[9];
    uint longl;
    longl = SR04_GetOnce();
    temp[0] = '0' + (longl / 1000) % 10;
    temp[1] = '0' + (longl / 100) % 10;
    temp[2] = '0' + (longl / 10) % 10;
    temp[3] = '0' + longl % 10;
    temp[4] = '\0';
    lcd_12864_len_text(2, temp);
    delay(1000);
}