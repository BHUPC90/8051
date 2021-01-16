//在显示汉字的时候要注意不要在偶数的位置

//在偶数的位置是乱码

//希望大家多交流

#include <reg52.h>

#include <intrins.h>

#define uchar unsigned char

#define uint unsigned int

#define LCD_data P1 //数据口

sbit LCD_RS = P2 ^ 0; //寄存器选择输入

sbit LCD_RW = P2 ^ 1; //液晶读/写控制

sbit LCD_EN = P2 ^ 2; //液晶使能控制

#define delayNOP()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
};

uchar code DIS1[] = {" 123 "};

uchar code DIS2[] = {" 电子 "};

uchar code DIS3[] = {" 加油 "};

uchar code DIS4[] = {" 奋斗就能成功 "};

void delay(int ms)

{

    uchar i;

    while (ms--)

    {

        for (i = 0; i < 250; i++)

            delayNOP();
    }
}

bit lcd_busy() //判断是否忙

{

    bit result;

    LCD_RS = 0;

    LCD_RW = 1;

    LCD_EN = 1;

    delayNOP();

    result = (bit)(LCD_data & 0x80);

    LCD_EN = 0;

    return (result);
}

void lcd_wcmd(uchar cmd) //写命令

{

    while (lcd_busy())
        ;

    LCD_RS = 0;

    LCD_RW = 0;

    LCD_data = cmd;

    LCD_EN = 1;

    delayNOP();

    LCD_EN = 0;
}

void lcd_wdat(uchar dat) //写数据

{

    while (lcd_busy())
        ;

    LCD_RS = 1;

    LCD_RW = 0;

    LCD_data = dat;

    LCD_EN = 1;

    delayNOP();

    LCD_EN = 0;
}

void lcd_wstr(uchar *p)

{

    while (*p)

        lcd_wdat(*p++);
}

void lcd_init() //液晶初始化

{

    delay(40);

    lcd_wcmd(0x30); //基本指令操作

    delay(5);

    lcd_wcmd(0x30); //基本指令操作

    delay(1);

    lcd_wcmd(0x0C); //显示开，关光标

    delay(5);

    lcd_wcmd(0x01); //清除LCD的显示内容

    delay(5);

    lcd_wcmd(0x06); //游标自动右移，整体画面不移动
}

void lcd_pos(uchar X, uchar Y) //X为行，Y为列

{
    switch (X)
    {
    case 1:
        Y |= 0x80;
        break;
    case 2:
        Y |= 0x90;
        break;
    case 3:
        Y |= 0x88;
        break;
    case 4:
        Y |= 0x98;
        break;
    default:
        break;
    }
    lcd_wcmd(Y); //显示地址
}

void clr_screen()

{
    lcd_wcmd(0x01); //清屏
    delay(5);
}

void display() //为反白做准备，目地是在图形显示时防止有噪点

{
    uchar i, j;
    lcd_wcmd(0x34); //写数据时,关闭图形显示
    for (i = 0; i < 32; i++)
    {
        lcd_wcmd(0x80 + i); //先写入水平坐标值
        lcd_wcmd(0x80); //写入垂直坐标值
        for (j = 0; j < 16; j++) //再写入两个8位元的数据
            lcd_wdat(0x00);
        delay(1);
    }
    for (i = 0; i < 32; i++)
    {
        lcd_wcmd(0x80 + i);
        lcd_wcmd(0x88);
        for (j = 0; j < 16; j++)
            lcd_wdat(0x00);
        delay(1);
    }
    lcd_wcmd(0x36); //写完数据,开图形显示
}

void photodisplay(uchar x, uchar y, uchar width) //反白，X值为0－7，Y值为0－3，width为行反白格数
{

    uchar i, j, flag = 0x00;
    display();
    if (y > 1)
    {
        flag = 0x08;
        y= y - 2;
    }
    lcd_wcmd(0x34); //写数据时,关闭图形显示,且打开扩展指令集
    for (i = 0; i < 16; i++)
    {
        lcd_wcmd(0x80 + (y << 4) + i);
        lcd_wcmd(0x80 + flag + x);
        for (j = 0; j < width; j++)
        {
            lcd_wdat(0xff);
            lcd_wdat(0xff);
        }
        delay(1);
    }
}

void main()

{

    lcd_init(); //初始化LCD

    clr_screen(); //清屏

    while (1)

    {

        clr_screen();

        lcd_pos(1, 0); //设置显示位置为第一行

        lcd_wstr(DIS1);

        delay(30);

        lcd_pos(2, 0); //设置显示位置为第二行

        lcd_wstr(DIS2);

        lcd_pos(3, 0); //设置显示位置为第三行

        lcd_wstr(DIS3);

        lcd_pos(4, 0); //设置显示位置为第四行

        lcd_wstr(DIS4);

        delay(1000);

        delay(1000);

        photodisplay(3, 2, 3);

        delay(1000);

        delay(1000);

        lcd_wcmd(0x30); //基本指令集

        delay(1000);

        delay(1000);
    }
}
