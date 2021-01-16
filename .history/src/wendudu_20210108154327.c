#include <reg52.h>
#include "lcd12864.h"

#define uchar unsigned char
#define uint unsigned int

sbit DQ = P2 ^ 2; //����DS18B20���������Ŷ�
uchar high, low;
char a, d2, d1, d0;
float f_temp;
uint i, j, temp;

uchar code line1[] = {"�繤���Ӵ���Э��"};
uchar code line2[] = "��ǰ�¶ȣ�";
uchar code line3[] = "00.0��";
//-------------------���뼶��ʱ����-------------------------
void delay_ms(uint z) //��ʱz����
{
  uint x, y;
  for (x = z; x > 0; x--)
    for (y = 115; y > 0; y--)
      ;
}

//------------------------�û�����������ʾ����-------------------------------
void dat_dis()
{
  lcd_12864_write_com(0x8c); //��ʾ�¶�
  wr_dat(0x30 + d2);
  wr_dat(0x30 + d1);
  wr_dat(0x2e); //д�롰.����
  wr_dat(0x30 + d0);
}

//-----------------------------��ʼ������------------------------------------
void init()
{
  delay_ms(20); //�ȴ�Һ�������Դ�ȶ�
  int_lcd_12864();
  lcd_12864_write_com(0x00);
  lcd_12864_len_text(1, line1);
  lcd_12864_len_text(2, line2);
  lcd_12864_len_text(3, line3);
}
//-----------------------���²��֣���ʼ������--------------------
void init_temp()
{
  DQ = 0; //�������ߣ�������λ�ź�
  i = 80;
  while (i > 0)
    i--;  //��ʱ480~960us
  DQ = 1; //��������
  i = 4;
  while (i > 0)
    i--; //��ʱ15~60us
  while (DQ)
    ; //�ȴ�����Ӧ������
  i = 70;
  while (i > 0)
    i--; //��ʱ����480us
}
//------------------���²��֣�д����-------------------------------
void wr_temp(uchar dat) //д1���ֽ�
{
  bit testb;
  for (j = 8; j > 0; j--)
  {
    testb = dat & 0x01;
    dat = dat >> 1;
    if (testb) //д1
    {
      DQ = 0; //�������ߣ�����дʱ��϶
      i++;    //��ʱ����1us
      DQ = 1; //��������
      i = 8;
      while (i > 0)
        i--; //��ʱ����60us����DS18B20����
    }
    else //д0
    {
      DQ = 0; //�������ߣ�����дʱ��϶
      i = 8;
      while (i > 0)
        i--;  //��������60us����DS18B20����
      DQ = 1; //��������
      i++;
      i++;
    }
  }
}
//------------------���²��֣�������-------------------------------
uchar rd_temp() //��1���ֽ�
{
  bit b; //�����Ž��յ���1���ֽ�
  uchar i_b;
  uchar rdbyte;
  for (j = 8; j > 0; j--)
  {
    DQ = 0; //�������ߣ�������ʱ϶
    i++;    //��ʱ����1us
    DQ = 1; //�ͷ�����
    i++;
    i++;    //��һ��ʱ���������ͷ�
    b = DQ; //��ȡ����
    i = 8;
    while (i > 0)
      i--; //��ʱ����60us
    i_b = b;
    rdbyte = (i_b << 7) | (rdbyte >> 1); //����ȡ����һλֵ����7λ����Ŷ�ȡ�����ݱ���rdbyte����1λ
  }
  return rdbyte;
}
//----------------------�¶Ȼ�ȡ����---------------------------
void get_temp()
{
  init_temp();      //��ʼ������λ����ȡӦ���ź�
  wr_temp(0xcc);    //����ROM
  wr_temp(0x44);    //��ʼ�¶�ת��
  delay_ms(1000);   //�ȴ��¶�ת�����
  init_temp();      //��DS18B20��λ
  wr_temp(0xcc);    //����ROM
  wr_temp(0xbe);    //���ݴ���
  low = rd_temp();  //��ȡ��8λ
  high = rd_temp(); //��ȡ��8λ
  temp = high;
  temp <<= 8;
  temp = temp | low;        //����ȡ�ĵ�8λ�͸�8λ�ϲ�
  f_temp = temp * 0.0625;   //�¶��ڼĴ�����Ϊ12λ �ֱ���λ0.0625��
  temp = f_temp * 10 + 0.5; //��ȷ��ʮ��λ����������
  d2 = temp / 100;          //��ʾ���ݣ�ʮλ
  d1 = temp % 100 / 10;     //��ʾ���ݣ���λ
  d0 = temp % 10;           //��ʾ���ݣ�ʮ��λ
}
//-----------------������------------------------
void main()
{
  init(); //lcd��ʼ��
  while (1)
  {
    get_temp(); //��ȡ�¶�
    dat_dis();  //��ʾ�¶�
  }
}