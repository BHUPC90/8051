#ifndef _INC_LCD12864
#define _INC_LCD12864
#define uchar unsigned char
#define uint unsigned int
#endif

//PSB ���������ڿ�ͨ��

sbit LCD12864_CS = P2 ^ 6;   //Ƭѡ
sbit LCD12864_SID = P2 ^ 5;  //����
sbit LCD12864_SCLK = P2 ^ 7; //ʱ��


uint qi_take_length(char *input_data_char)
{
    uint i = 0;
    while (input_data_char[i] != '\0')
    {
        i++;
    };
    return i;
}

/*�����ֽ�*/
void lcd_12864_sendbyte(uchar data1)
{
    uint i;
    for (i = 0; i < 8; i++)
    {
        if ((data1 << i) & 0x80) //10000000
        {
            LCD12864_SID = 1;
        }
        else
        {
            LCD12864_SID = 0;
        }
        LCD12864_SCLK = 0;
        LCD12864_SCLK = 1;
    }
}
/*д����ָ��*/
void lcd_12864_write_com(unsigned char data1)
{
    LCD12864_CS = 1;
    lcd_12864_sendbyte(0xf8);                //11111000
    lcd_12864_sendbyte(data1 & 0xf0);        //11110000
    lcd_12864_sendbyte((data1 << 4) & 0xf0); //11110000
}
/*д��������*/
void lcd_12864_write_data(unsigned char data1)
{
    LCD12864_CS = 1;
    lcd_12864_sendbyte(0xfa);
    lcd_12864_sendbyte(data1 & 0xf0);
    lcd_12864_sendbyte((data1 << 4) & 0xf0);
}
/*��ʼ������*/
void int_lcd_12864()
{
    
    lcd_12864_write_com(0x30); //00110000 ����ָ�
    delay(50);
    lcd_12864_write_com(0x0c); //1100 ������ʾ��,�α�ر�
    delay(50);
}

void lcd_12864_text(uchar *data1)
{
    uint i;
    uint strlong = qi_take_length(data1);
    lcd_12864_write_com(0x80); //��1��
    for (i = 0; i < strlong; i++)
    {
        if (i == 16)
            lcd_12864_write_com(0x90); //��2��
        if (i == 32)
            lcd_12864_write_com(0x88); //��3��
        if (i == 48)
            lcd_12864_write_com(0x98); //��4��

        lcd_12864_write_data(*data1);
        data1++;
    }
}

void lcd_12864_len_text(int len,uchar *data1)
{
    uint i;
    uint strlong = qi_take_length(data1);
    
    switch (len)
    {
    case 1:
        lcd_12864_write_com(0x80); //��1��
        break;
    case 2:
        lcd_12864_write_com(0x90); //��2��
        break;
    case 3:
        lcd_12864_write_com(0x88); //��3��
        break;
    case 4:
        lcd_12864_write_com(0x98); //��4��
        break;
    default:
        break;
    }
    for (i = 0; i < strlong; i++)
    {
        lcd_12864_write_data(*data1);
        data1++;
    }
}

void LCD_fulldisplay_picture_2(const uchar *pic) 
{
    unsigned int x = 0;
    unsigned char i, j;
    lcd_12864_write_com(0x34); 
    lcd_12864_write_com(0x36); 

    for (i = 0; i < 32; i++) 
    {
        lcd_12864_write_com(0x80 | i); //1000 0000
        lcd_12864_write_com(0x80); 
        for (j = 0; j < 16; j++) 
        {  
            lcd_12864_write_data(*pic);
            pic++;
        }
    }

    for (i = 0; i < 32; i++)
    {
        lcd_12864_write_com(0x80 | i);
        lcd_12864_write_com(0x88); 
        for (j = 0; j < 16; j++) //256/8=32 byte
        {
            lcd_12864_write_data(*pic);
            pic++;
        }
    }
    lcd_12864_write_com(0x30);
}

void lcd_12864_picture(const uchar *pic) 
{
    unsigned int x = 0;
    unsigned char i, j;
    lcd_12864_write_com(0x34); 
    lcd_12864_write_com(0x36); 

    for (i = 0; i < 32; i++) 
    {
        lcd_12864_write_com(0x80 | i); //1000 0000
        lcd_12864_write_com(0x80); 
        for (j = 0; j < 16; j++) 
        {  
            lcd_12864_write_data(*pic);
            pic++;
        }
    }

    for (i = 0; i < 32; i++)
    {
        lcd_12864_write_com(0x80 | i);
        lcd_12864_write_com(0x88); 
        for (j = 0; j < 16; j++) //256/8=32 byte
        {
            lcd_12864_write_data(*pic);
            pic++;
        }
    }
    lcd_12864_write_com(0x30);
}
