/*********************************************************************************************
程序名： 　　 DHT11-1602
编写人： 　　 fby
编写时间：　　2019年3月21日
硬件支持：　　STC89C52RC  DHT11  LCD1602
接口说明：　　DHT11：VCC DATA NC GND
*********************************************************************************************
说明：1、单片机是STC89C52RC，晶振频率11.0592MHz，实验成功。
          2、好东西要共享，大家随意折腾~~
*********************************************************************************************/
#include<reg51.h>
#include<intrins.h>

#define uchar unsigned char        //无符号字符型类型数据
#define uint unsigned int //无符号整型数据
#define IODATA  P0

uchar dat_r[4];          //用于存放从DHT11读取到的数值

sbit DATA=P2^2;   //定义数据线
sbit RS=P1^0; //定义rs端口
sbit RW=P1^1; //定义rw端口
sbit E=P2^5; //定义en端口


/*********************************************************************************************
函数名：延时函数
调  用：Delay30us();/Delay20ms();/Delay1s();
参  数：无
返回值：无
结  果：延时相应时间
备  注：振荡晶体为11.0592MHz(所有延时函数Copy自STC-ISP(v6.85).exe)
**********************************************************************************************/
void Delay30us()                //@11.0592MHz-40
{
        unsigned char i;

        i = 11;
        while (--i);
}


void Delay20ms()                //@11.0592MHz
{
        unsigned char i, j;

        i = 36;
        j = 217;
        do
        {
                while (--j);
        } while (--i);
}


void Delay1s()                //@11.0592MHz
{
        unsigned char i, j, k;

        _nop_();
        i = 8;
        j = 1;
        k = 243;
        do
        {
                do
                {
                        while (--k);
                } while (--j);
        } while (--i);
}


/*********************************************************************************************
函数名：DHT11启动函数
调  用：无
参  数：无
返回值：无
结  果：DHT11开始工作
备  注：
**********************************************************************************************/
void DHT11_start()                 // 主机控制DHT11开始工作
{
   DATA=1;                                 // 主线空闲状态
   _nop_();
   _nop_();
   DATA=0;                                 // 主机发送开始工作信号
   Delay20ms();   //延时18ms以上
   DATA=1;                                 // 拉高并延时等待DHT11响应
   Delay30us();
}
/*********************************************************************************************
函数名：DHT11读数据函数
调  用：? = DHT11_rec_byte();
参  数：无
返回值：
结  果：读DHT11数据并保存到dat
备  注：
**********************************************************************************************/

uchar DHT11_rec_byte()      //接收一个字节
{
   uchar i,dat=0;
  for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {         
      while(!DATA);   ////等待50us低电平过去
      Delay30us();
          Delay30us();     //延时60us，如果还为高则数据为1，否则为0
      dat<<=1;           //左移位使正确接收8位数据，数据为0时直接移位
      if(DATA==1)    //数据为1时，使dat加1来接收数据1
         dat+=1;
      while(DATA);  //等待数据线拉低   
    }  
    return dat;
}

/*********************************************************************************************
函数名：接收DHT11的40位的数据并校验
调  用：? = DHT11_receive();
参  数：无
返回值：无
结  果：结果保存到dat_r[i]中
备  注：
**********************************************************************************************/
void DHT11_receive()      //接收40位的数据
{
    uchar R_H,R_L,T_H,T_L,RH,RL,TH,TL,revise;
    DHT11_start();                 // 主机控制DHT11开始工作
    if(DATA==0)
    {
        while(DATA==0);   //等待拉高     
        Delay30us();
                Delay30us();
                Delay30us();  //拉高后延时80us
        R_H=DHT11_rec_byte();    //接收湿度高八位  
        R_L=DHT11_rec_byte();    //接收湿度低八位  
        T_H=DHT11_rec_byte();    //接收温度高八位  
        T_L=DHT11_rec_byte();    //接收温度低八位
        revise=DHT11_rec_byte(); //接收校正位

        Delay30us();    //结束

        if((R_H+R_L+T_H+T_L)==revise)      //校正
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        }
        /*数据处理，方便显示*/
        dat_r[0]='0'+(RH/10);
        dat_r[1]='0'+(RH%10);
        dat_r[2]='0'+(TH/10);
        dat_r[3]='0'+(TH%10);
    }
}

/********************************************************************
* 名称 : bit Busy(void)
* 功能 : 这个是一个读状态函数，读出函数是否处在忙状态
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/
void Busy(void)
{
bit busy_flag = 1;
IODATA  = 0xff;
RS = 0;          //指令关
Delay30us();
RW = 1;          //读状态
Delay30us();
E = 1;        //使能高电平
while(1)
{
busy_flag = (bit)(IODATA  & 0x80); //强制取最高位
if(busy_flag == 0)
{
break;
}
}
E = 0;           //E 1->0，rs=0，rw=1，
           //将数据寄存器内的数据读到DB0~DB7,分别将状态标志BF和地址计数器（AC）内容读到DB7和DB6~DB0。

}

/********************************************************************
* 名称 : lcd1602_write_cmd(uchar del)
* 功能 : 1602命令函数
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/
void lcd1602_write_cmd(uchar del)
{
RS = 0;
Delay30us();
RW = 0;
Delay30us();
E = 0;
Delay30us();
IODATA  = del;
Delay30us();
E = 1;
Delay30us();
E = 0;                        //E 1->0，rs=0，rw=0, 将指令代码写入指令寄存器中
}

/********************************************************************
* 名称 : lcd1602_write_data(uchar del)
* 功能 : 1602写数据函数
* 输入 : 需要写入1602的数据
* 输出 : 无
***********************************************************************/

void lcd1602_write_data(uchar del)
{
Busy();      
Delay30us();
RS = 1;
Delay30us();
RW = 0;
Delay30us();
E = 0;
Delay30us();
IODATA  = del;
Delay30us();
E = 1;
Delay30us();
E = 0;                          //E 1->0，rs=1，rw=0, 数据写入数据寄存器中
}

/********************************************************************
* 名称 : lcd1602_init()
* 功能 : 1602初始化，请参考1602的资料
* 输入 : 无
* 输出 : 无
***********************************************************************/
void lcd1602_init(void)
{
Delay20ms();
lcd1602_write_cmd(0x38);         //8位数据，双列，5*7字形
Delay20ms();
lcd1602_write_cmd(0x38);
Delay20ms();
lcd1602_write_cmd(0x38);
Busy();
lcd1602_write_cmd(0x08);// 只开显示
Busy();   
lcd1602_write_cmd(0x01);        //清屏
Busy();
lcd1602_write_cmd(0x06);//显示地址递增，即写一个数据后，显示位置右移一位
Busy();
lcd1602_write_cmd(0x0c);        //开启显示屏，关光标，光标不闪烁
}

/********************************************************************
* 名称 : lcd1602_char(uchar hang,uchar lie,char sign)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符显示"b" ，调用该函数如下
L1602_char(1,5,'b')
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void lcd1602_char(uchar hang,uchar lie,char sign)
{
uchar a;
if(hang == 1)
{
a = 0x80;
}
if(hang == 2)
{
a = 0xc0;
}
a = a + lie - 1;
Busy();
lcd1602_write_cmd(a);  //置数据存贮器地址
Busy();
lcd1602_write_data(sign);
}

/********************************************************************
* 名称 : lcd1602_string(uchar hang,uchar lie,uchar *p)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符开始显示"ab cd ef" ，调用该函数如下
L1602_string(1,5,"ab cd ef;")
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void lcd1602_string(uchar hang,uchar lie,uchar *p)
{
uchar a;
if(hang == 1)
{
a = 0x80;
}
if(hang == 2)
{
a = 0xc0;
}
a = a + lie - 1;
while(1)
{
Busy();
lcd1602_write_cmd(a);
Busy();
lcd1602_write_data(*p);
a++;
p++;   
if((*p == '\0')||(a==0x90)||(a==0xd0)) //字符结束或每行最后一个字符
{
break;
}
}
}

/*********************************************************************************************
函数名：主函数
调  用：无
参  数：无
返回值：无
结  果：读DHT11数据并送到1602显示
备  注：
**********************************************************************************************/
void main (void)
{
        
        Delay20ms();
        lcd1602_init();                //LCD初始化
        while(1)
        {
                Delay1s();                //经测试，两次连读要至少延时80ms        
                DHT11_receive();//接受数据
                lcd1602_string(1,1,"SHIDU:");
                lcd1602_string(2,1,"WENDU:");
                lcd1602_char(1,7,dat_r[0]);
                lcd1602_char(1,8,dat_r[1]);
                lcd1602_char(2,7,dat_r[2]);
                lcd1602_char(2,8,dat_r[3]);
                lcd1602_string(1,9,"%");
                lcd1602_char(2,9,0XDF);
                lcd1602_string(2,10,"C");
               
        }
}