#ifndef _INC_LCD12864
#define _INC_LCD12864
#define uchar unsigned char
#define uint unsigned int
#endif

//PSB 拉低启并口口通信
sbit lcd_12864_RS = P2 ^ 0;  //片选
sbit lcd_12864_WRD = P2 ^ 1; //数据
sbit lcd_12864_EN = P2 ^ 2;  //时钟

void delay(unsigned int wait_time);
void lcd_12864_commode(uchar data1);

uint qi_take_length(char *input_data_char)
{
    uint i = 0;
    while (input_data_char[i] != '\0')
    {
        i++;
    };
    return i;
}

void int_lcd_12864()
{
    delay(10);               //延时10
    lcd_12864_commode(0x30); //1100 地址归为
    delay(100);              //延时100
    lcd_12864_commode(0x08); //1000 清除显示
    delay(100);              //延时100
    lcd_12864_commode(0x10); //设置光标
    delay(100);              //延时100
    lcd_12864_commode(0x0C); //1100 打开显示
    delay(100);              //延时100
    lcd_12864_commode(0x01); //1000 清除显示
    delay(10);               //延时100
    lcd_12864_commode(0x06); //光标右移
    delay(100);              //延时100
}

void lcd_12864_commode(uchar data1)
{
    lcd_12864_WRD = 0;
    lcd_12864_RS = 0;
    delay(1);
    P0 = data1;
    lcd_12864_EN = 1;
    delay(5);
    lcd_12864_EN = 0;
}
void lcd_12864_data(uchar data1)
{
    lcd_12864_WRD = 0;
    lcd_12864_RS = 1;
    delay(1);
    P0 = data1;
    lcd_12864_EN = 1;
    delay(5);
    lcd_12864_EN = 0;
}

void lcd_12864_text(uchar *data1)
{
    uint i;
    uint strlong = qi_take_length(data1);
    lcd_12864_commode(0x80); //第1行
    for (i = 0; i < strlong; i++)
    {
        if (i == 16)
            lcd_12864_commode(0x90); //第2行
        if (i == 32)
            lcd_12864_commode(0x88); //第3行
        if (i == 48)
            lcd_12864_commode(0x98); //第4行

        lcd_12864_data(*data1);
        data1++;
    }
}