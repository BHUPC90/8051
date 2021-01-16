#include "qi51arduino.h"
#include "lcd12864.h"

sbit IRIN = P3 ^ 2;

uchar code gImage_huatu[1030] = { 0X00,0X01,0X80,0X00,0X40,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X04,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X03,0X80,0X0C,0X06,0X00,0X00,0X00,0X70,0X07,0XFF,0XFC,0X00,0X00,
0X00,0X00,0X0C,0X03,0X00,0X0F,0XBF,0X7F,0X00,0X80,0X70,0X0F,0XFF,0XFC,0X00,0X00,
0X00,0X00,0X0F,0X87,0XFF,0X1F,0XBE,0XFF,0X01,0XDF,0XFF,0X0F,0XEE,0X0C,0X00,0X00,
0X00,0X00,0X07,0XE7,0XFF,0X01,0X30,0XE3,0X03,0X9F,0XFF,0X0E,0X0E,0X0C,0X00,0X00,
0X00,0X00,0X01,0XE6,0X03,0X00,0X30,0XE7,0X07,0X00,0X60,0X0E,0X0C,0X0C,0X00,0X00,
0X00,0X00,0X00,0XEE,0X07,0X1E,0X30,0XE7,0X0F,0X8F,0XF8,0X0E,0X0C,0X0C,0X00,0X00,
0X00,0X00,0X00,0XCC,0X67,0X1F,0X30,0XE7,0X1D,0X9F,0XFF,0X0E,0XFF,0XEC,0X00,0X00,
0X00,0X00,0X19,0XCC,0X76,0X07,0XB0,0XE7,0X09,0X9D,0XFF,0X0E,0XFF,0XEC,0X00,0X00,
0X00,0X00,0X1D,0XDC,0XE6,0X03,0XB0,0XE6,0X01,0X9C,0X67,0X0E,0X0C,0X2C,0X00,0X00,
0X00,0X00,0X0F,0X80,0XE6,0X03,0XB8,0XE6,0X01,0X8C,0X66,0X0E,0X0C,0X0C,0X00,0X00,
0X00,0X00,0X07,0X81,0XCE,0X03,0X3E,0XE6,0X01,0X8F,0XFE,0X0E,0X0C,0X0C,0X00,0X00,
0X00,0X00,0X07,0X01,0XC2,0X03,0X3F,0XEE,0X01,0X8F,0XFC,0X0E,0X0D,0XEC,0X00,0X00,
0X00,0X00,0X07,0X83,0X80,0X03,0X3E,0XEE,0X01,0X80,0X60,0X0F,0XFF,0XEC,0X00,0X00,
0X00,0X00,0X07,0X83,0XC0,0X03,0X1C,0XFE,0X03,0X9C,0XE0,0X0F,0XFF,0XCC,0X00,0X00,
0X00,0X00,0X07,0XC7,0XE0,0X03,0X08,0X60,0X03,0X9F,0XC0,0X0F,0X8C,0X0C,0X00,0X00,
0X00,0X00,0X0E,0XE6,0X78,0X03,0XE0,0X00,0X03,0X8F,0XE0,0X0E,0X0C,0X0E,0X00,0X00,
0X00,0X00,0X0C,0X6E,0X3C,0X07,0XFF,0X80,0X03,0X8F,0XF8,0X0E,0X0C,0X0E,0X00,0X00,
0X00,0X00,0X1C,0X4C,0X1E,0X0E,0X7F,0XFE,0X03,0X9E,0X7F,0X0C,0X0C,0X1E,0X00,0X00,
0X00,0X00,0X18,0X1C,0X0F,0X1C,0X01,0XFC,0X03,0XBC,0X0F,0X1C,0X0C,0X3C,0X00,0X00,
0X00,0X00,0X18,0X18,0X07,0X08,0X00,0X0C,0X03,0X90,0X02,0X38,0X04,0X18,0X00,0X00,
0X00,0X00,0X00,0X00,0X02,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X08,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X60,0X00,0XC0,0X18,0X00,0X10,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X80,0X0D,0XFE,0X18,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0X1F,0XFE,0X08,0X02,0X10,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0X00,0X80,0X00,0X82,0X3D,0XFF,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X06,0X4C,0X03,0XF0,0X20,0X02,0X41,0X11,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X04,0X44,0X1C,0X1C,0X2F,0XFA,0X41,0X11,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X04,0X44,0X00,0X01,0X20,0X82,0X5D,0X11,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0X42,0X3F,0XFE,0X20,0X82,0X11,0X11,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X08,0X42,0X00,0X09,0X27,0XF2,0X11,0X11,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X10,0X41,0X0F,0XC8,0X24,0X92,0X7D,0XFF,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X30,0X41,0X88,0X48,0X24,0X92,0X11,0X11,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0X08,0X48,0X24,0X92,0X14,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X40,0X0F,0XC8,0X24,0XA2,0X14,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X01,0XC0,0X08,0X08,0X20,0X82,0X18,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X80,0X00,0X38,0X20,0X8C,0X30,0X10,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X10,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
};


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
    lcd_12864_picture(gImage_huatu);
}

void loop()
{
}
