#include "qi51arduino.h"
#include "lcd12864.h"
#include "UART.h"
#include "DS18B20.h"

uchar code IC_DAT[] = {
    "����С����Գ������ʵ��ȫ����ʾ������ʾ�����ţ���������"};
uchar code IC_DAT_2[] = {
    "�¶�"};
uchar code IC_DAT_3[] = {
    "��λ�����϶�"};
void setup()
{
    int_lcd_12864();
    lcd_12864_text(IC_DAT);
    Serial_begin(253);
    while (pin22 == 1)
        ;
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, IC_DAT_2);
    lcd_12864_len_text(3, IC_DAT_3);
    DS18B20_Read_id();
    lcd_12864_len_text(4, ds_id);
}

void loop()
{
    uchar temp[9];
    uint longl;

    longl = DS18B20_Read_tem();
    temp[0] = '0' + longl / 100;
    temp[1] = '0' + longl % 100 / 10;
    temp[2] = '.';
    temp[3] = '0' + longl % 10;
    temp[4] = '\0';
    lcd_12864_len_text(2, temp);
    delay(1000);
}