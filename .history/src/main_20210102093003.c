#include "qi51arduino.h"
#include "lcd12864.h"
#include "DHT11.h"

unsigned char code IC_DAT[]={
"����С����Գ������ʵ��ȫ����ʾ������ʾ�����ţ���������" 
};

unsigned char code IC_DAT2[]={
"����2" 
};
void setup()
{
int_lcd_12864();

lcd_12864_text(IC_DAT);

while (pin21 == 1)
        ;
lcd_12864_write_com(0x00);

DHT11_temp();
lcd_12864_text(IC_DAT2);
}

void loop()
{
    
}