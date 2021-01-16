//����ʾ���ֵ�ʱ��Ҫע�ⲻҪ��ż����λ��

//��ż����λ��������

//ϣ����Ҷཻ��

#include <reg52.h>

#include <intrins.h>

#define uchar unsigned char

#define uint unsigned int

#define LCD_data P1 //���ݿ�

sbit LCD_RS = P2 ^ 0; //�Ĵ���ѡ������

sbit LCD_RW = P2 ^ 1; //Һ����/д����

sbit LCD_EN = P2 ^ 2; //Һ��ʹ�ܿ���

#define delayNOP()
{
    _nop_();
    _nop_();
    _nop_();
    _nop_();
};

uchar code DIS1[] = {" 123 "};

uchar code DIS2[] = {" ���� "};

uchar code DIS3[] = {" ���� "};

uchar code DIS4[] = {" �ܶ����ܳɹ� "};

void delay(int ms)

{

    uchar i;

    while (ms--)

    {

        for (i = 0; i < 250; i++)

            delayNOP();
    }
}

bit lcd_busy() //�ж��Ƿ�æ

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

void lcd_wcmd(uchar cmd) //д����

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

void lcd_wdat(uchar dat) //д����

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

void lcd_init() //Һ����ʼ��

{

    delay(40);

    lcd_wcmd(0x30); //����ָ�����

    delay(5);

    lcd_wcmd(0x30); //����ָ�����

    delay(1);

    lcd_wcmd(0x0C); //��ʾ�����ع��

    delay(5);

    lcd_wcmd(0x01); //���LCD����ʾ����

    delay(5);

    lcd_wcmd(0x06); //�α��Զ����ƣ����廭�治�ƶ�
}

void lcd_pos(uchar X, uchar Y) //XΪ�У�YΪ��

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
    lcd_wcmd(Y); //��ʾ��ַ
}

void clr_screen()

{
    lcd_wcmd(0x01); //����
    delay(5);
}

void display() //Ϊ������׼����Ŀ������ͼ����ʾʱ��ֹ�����

{
    uchar i, j;
    lcd_wcmd(0x34); //д����ʱ,�ر�ͼ����ʾ
    for (i = 0; i < 32; i++)
    {
        lcd_wcmd(0x80 + i); //��д��ˮƽ����ֵ
        lcd_wcmd(0x80); //д�봹ֱ����ֵ
        for (j = 0; j < 16; j++) //��д������8λԪ������
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
    lcd_wcmd(0x36); //д������,��ͼ����ʾ
}

void photodisplay(uchar x, uchar y, uchar width) //���ף�XֵΪ0��7��YֵΪ0��3��widthΪ�з��׸���
{

    uchar i, j, flag = 0x00;
    display();
    if (y > 1)
    {
        flag = 0x08;
        y= y - 2;
    }
    lcd_wcmd(0x34); //д����ʱ,�ر�ͼ����ʾ,�Ҵ���չָ�
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

    lcd_init(); //��ʼ��LCD

    clr_screen(); //����

    while (1)

    {

        clr_screen();

        lcd_pos(1, 0); //������ʾλ��Ϊ��һ��

        lcd_wstr(DIS1);

        delay(30);

        lcd_pos(2, 0); //������ʾλ��Ϊ�ڶ���

        lcd_wstr(DIS2);

        lcd_pos(3, 0); //������ʾλ��Ϊ������

        lcd_wstr(DIS3);

        lcd_pos(4, 0); //������ʾλ��Ϊ������

        lcd_wstr(DIS4);

        delay(1000);

        delay(1000);

        photodisplay(3, 2, 3);

        delay(1000);

        delay(1000);

        lcd_wcmd(0x30); //����ָ�

        delay(1000);

        delay(1000);
    }
}
