#include "qi51arduino.h"
#include "lcd12864.h"
#include "DHT11.h"

unsigned char code IC_DAT[]={
"����С����Գ������ʵ��ȫ����ʾ������ʾ�����ţ���������" 
};
void setup()
{
int_lcd_12864();


while (pin21 == 1)
        ;
DHT11_temp();
lcd_12864_text(IC_DAT);
}

void loop()
{
    
}