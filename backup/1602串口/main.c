#include "qi51arduino.h"
#include "UART.h"
#include "lcd.h"
//����ͨ��
void setup()
{
    Serial_begin(253);
    LCD_RW = 0;        //����lcd writeģʽ
    LCD_EN = 0;        //��ʼlcd��ʹ�ܵ͵�ƽ
    lcd_commode(0x38); //��ʼ��
    lcd_commode(0x0e); //1110 ��ʾ���
}
void loop()
{
    if (Serial_available)
    {
        lcd_data(re_data);
    }
    Serial_available = 0;
}
