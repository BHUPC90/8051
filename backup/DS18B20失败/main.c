#include "qi51arduino.h"
#include "lcd.h"
#include "DS18B20.h"

void setup()
{
    LCD_RW = 0;        //����lcd writeģʽ
    LCD_EN = 0;        //��ʼlcd��ʹ�ܵ͵�ƽ
    lcd_commode(0x38); //��ʼ��
    lcd_commode(0x0e); //1110 ��ʾ���
    DS18B20_inttemp();
}


/* ������ת��Ϊ�ַ����� str-�ַ���ָ�룬 dat-��ת����������ֵ-�ַ������� */
unsigned char IntToString(unsigned char *str, int dat)
{
	signed char i = 0;
	unsigned char len = 0;
	unsigned char buf[6];
	if (dat < 0) //���Ϊ����������ȡ����ֵ������ָ������Ӹ���
	{
		dat = -dat;
		*str++ = '-';
		len++;
	}
	do { //��ת��Ϊ��λ��ǰ��ʮ��������
			buf[i++] = dat % 10;
			dat /= 10;
	} while (dat > 0);
	len += i; //i ����ֵ������Ч�ַ��ĸ���
	while (i-- > 0) //������ֵת��Ϊ ASCII �뷴�򿽱�������ָ����
	{
		*str++ = buf[i] + '0';
		}
	*str = '\0'; //����ַ���������
	return len; //�����ַ�������
	}

void loop()
{
    bit res;
    int temp; //��ȡ���ĵ�ǰ�¶�ֵ
	int intT, decT; //�¶�ֵ��������С������
	uchar len;
	uchar str[12];
    res = DS18B20_temp(&temp); //��ȡ��ǰ�¶�
    DS18B20_inttemp(); 
    if (res)                   //��ȡ�ɹ�ʱ��ˢ�µ�ǰ�¶���ʾ
    {
        intT = temp >> 4;             //������¶�ֵ��������
        decT = temp & 0xF;            //������¶�ֵС������
        len = IntToString(str, intT); //��������ת��Ϊ�ַ���
        str[len++] = '.';             //���С����
        decT = (decT * 10) / 16;      //�����Ƶ�С������ת��Ϊ 1 λʮ����λ
        str[len++] = decT + '0';      //ʮ����С��λ��ת��Ϊ ASCII �ַ�
        while (len < 6)               //�ÿո��뵽 6 ���ַ�����
        {
            str[len++] = ' ';
        }
        str[len] = '\0';        //����ַ���������
        lcd_str( 1, str);  //��ʾ��Һ������
    }
    else //��ȡʧ��ʱ����ʾ������Ϣ
    {
        lcd_str( 0, "error!");
    }
    
}