#include <reg52.h>
#include "lcd12864.h"

#define uchar unsigned char
#define uint unsigned int

sbit DQ = P2 ^ 2; //定义DS18B20数据线引脚端
uchar high, low;
char a, d2, d1, d0;
float f_temp;
uint i, j, temp;

uchar code line1[] = {"电工电子创新协会"};
uchar code line2[] = "当前温度：";
uchar code line3[] = "00.0度";
//-------------------毫秒级延时函数-------------------------
void delay_ms(uint z) //延时z毫秒
{
  uint x, y;
  for (x = z; x > 0; x--)
    for (y = 115; y > 0; y--)
      ;
}

//------------------------用户界面数据显示函数-------------------------------
void dat_dis()
{
  lcd_12864_write_com(0x8c); //显示温度
  wr_dat(0x30 + d2);
  wr_dat(0x30 + d1);
  wr_dat(0x2e); //写入“.”号
  wr_dat(0x30 + d0);
}

//-----------------------------初始化函数------------------------------------
void init()
{
  delay_ms(20); //等待液晶输入电源稳定
  int_lcd_12864();
  lcd_12864_write_com(0x00);
  lcd_12864_len_text(1, line1);
  lcd_12864_len_text(2, line2);
  lcd_12864_len_text(3, line3);
}
//-----------------------获温部分：初始化函数--------------------
void init_temp()
{
  DQ = 0; //拉低总线，产生复位信号
  i = 80;
  while (i > 0)
    i--;  //延时480~960us
  DQ = 1; //拉高总线
  i = 4;
  while (i > 0)
    i--; //延时15~60us
  while (DQ)
    ; //等待产生应答脉冲
  i = 70;
  while (i > 0)
    i--; //延时至少480us
}
//------------------获温部分：写函数-------------------------------
void wr_temp(uchar dat) //写1个字节
{
  bit testb;
  for (j = 8; j > 0; j--)
  {
    testb = dat & 0x01;
    dat = dat >> 1;
    if (testb) //写1
    {
      DQ = 0; //拉低总线，产生写时间隙
      i++;    //延时大于1us
      DQ = 1; //拉高总线
      i = 8;
      while (i > 0)
        i--; //延时至少60us，供DS18B20采样
    }
    else //写0
    {
      DQ = 0; //拉低总线，产生写时间隙
      i = 8;
      while (i > 0)
        i--;  //保持至少60us，供DS18B20采样
      DQ = 1; //拉高总线
      i++;
      i++;
    }
  }
}
//------------------获温部分：读函数-------------------------------
uchar rd_temp() //读1个字节
{
  bit b; //定义存放接收到的1个字节
  uchar i_b;
  uchar rdbyte;
  for (j = 8; j > 0; j--)
  {
    DQ = 0; //拉低总线，产生读时隙
    i++;    //延时大于1us
    DQ = 1; //释放总线
    i++;
    i++;    //给一定时间让总线释放
    b = DQ; //读取数据
    i = 8;
    while (i > 0)
      i--; //延时至少60us
    i_b = b;
    rdbyte = (i_b << 7) | (rdbyte >> 1); //将读取到得一位值左移7位，存放读取的数据变量rdbyte右移1位
  }
  return rdbyte;
}
//----------------------温度获取函数---------------------------
void get_temp()
{
  init_temp();      //初始化，复位并获取应答信号
  wr_temp(0xcc);    //跳过ROM
  wr_temp(0x44);    //开始温度转换
  delay_ms(1000);   //等待温度转换完成
  init_temp();      //将DS18B20复位
  wr_temp(0xcc);    //跳过ROM
  wr_temp(0xbe);    //读暂存器
  low = rd_temp();  //读取低8位
  high = rd_temp(); //读取高8位
  temp = high;
  temp <<= 8;
  temp = temp | low;        //将读取的低8位和高8位合并
  f_temp = temp * 0.0625;   //温度在寄存器中为12位 分辨率位0.0625°
  temp = f_temp * 10 + 0.5; //精确到十分位，四舍五入
  d2 = temp / 100;          //显示数据：十位
  d1 = temp % 100 / 10;     //显示数据：个位
  d0 = temp % 10;           //显示数据：十分位
}
//-----------------主函数------------------------
void main()
{
  init(); //lcd初始化
  while (1)
  {
    get_temp(); //获取温度
    dat_dis();  //显示温度
  }
}