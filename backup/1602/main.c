#include "qi51arduino.h"
#include "lcd.h"

void setup()
{
    LCD_RW = 0;        //����lcd writeģʽ
    LCD_EN = 0;        //��ʼlcd��ʹ�ܵ͵�ƽ
    lcd_commode(0x38); //��ʼ��
    lcd_commode(0x0e); //1110 ��ʾ���
    lcd_str(1, "Made By 666SQi");
    while (pin21 == 1)
        ;
    lcd_clean();
}

void loop()
{
    int i = 0;
    lcd_str(1, "I LOVE YOU");
    lcd_str(2, "China");
    for (i = 0; i < 16; i++)
    {
        lcd_commode(0x1c);
        delay(200);
    }
}