#include "qi51arduino.h"
#include "lcd12864.h"

unsigned char code IC_DAT[]={
"这是小奇测试程序可以实现全屏显示可以显示标点符号，。！（）" 
};
void setup()
{
int_lcd_12864();
lcd_12864_text(IC_DAT);
}

void loop()
{
    
}