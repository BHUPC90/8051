#include "qi51arduino.h"
#include "lcd12864.h"
#include "huatu.h"
#include "lcd12864.h"

sbit IRIN = P3 ^ 2;


uchar code IC_DAT[] = {
    ""};

void IrInit()
{
    IT0 = 1;  //下降沿触发
    EX0 = 1;  //打开中断0允许
    EA = 1;   //打开总中断
    IRIN = 1; //初始化端口
}

void setup()
{
    IrInit();
    lcd_12864_picture(firstpage);
}

void loop()
{
}
