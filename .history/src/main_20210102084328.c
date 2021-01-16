#include "qi51arduino.h"
#include "lcd.h"
#include "lcd.h"

void setup()
{
    LCD_RW = 0;        //设置lcd write模式
    LCD_EN = 0;        //初始lcd化使能低电平
    lcd_commode(0x38); //初始化
    lcd_commode(0x0e); //1110 显示光标
    lcd_str(1, "Made By 666SQi");
    while (pin21 == 1)
        ;
    lcd_clean();
}

void loop()
{
}