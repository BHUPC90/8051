#include "qi51arduino.h"
#include "UART.h"
#include "lcd.h"
//串口通信
void setup()
{
    Serial_begin(253);
    LCD_RW = 0;        //设置lcd write模式
    LCD_EN = 0;        //初始lcd化使能低电平
    lcd_commode(0x38); //初始化
    lcd_commode(0x0e); //1110 显示光标
}
void loop()
{
    if (Serial_available)
    {
        lcd_data(re_data);
    }
    Serial_available = 0;
}
