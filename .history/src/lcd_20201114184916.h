#ifndef _INC_LCD
#define _INC_LCD
#define uchar unsigned char
#define uint unsigned int
#endif

sbit LCD_RS = P2 ^ 7; //����/����
sbit LCD_RW = P2 ^ 6; //��ȡ/д��
sbit LCD_EN = P2 ^ 5; //ʹ��

void lcd_commode(uchar commode);
void lcd_data(uchar commode);
void delay(unsigned int wait_time);

/*
lcd����ָ��
��������
00111000 (0x38) ����1602
0|0|0|0|1|D|C|B D=������ʾ C=������� B=�����˸
0|0|0|0|0|1|N|S N=ִ�ж�д���1/0��1 S=����Ļ����
��������ָ��λ��  0x80 + 
*/
void lcd_commode(uchar commode)
{
    LCD_RS = 0;   //����ģʽΪ����������
    P0 = commode; //׼�����ݰ�
    delay(5);     //��΢��ʱһ��
    LCD_EN = 0;   //�������ݣ�
    delay(5);     //��΢��ʱһ��
    LCD_EN = 1;   //������ɣ��������ģʽ
}
//lcd��������
void lcd_data(uchar commode)
{
    LCD_RS = 1;   //����ģʽΪ����������
    P0 = commode; //׼�����ݰ�
    delay(5);     //��΢��ʱһ��
    LCD_EN = 0;   //�������ݣ�
    delay(5);     //��΢��ʱһ��
    LCD_EN = 1;   //������ɣ��������ģʽ
}