#ifndef _INC_DHT11
#define _INC_DHT11
#define uchar unsigned char
#define uint unsigned int
#endif
sbit DHT11_pin = P2 ^ 3;

uint temperature = 0;
uint humidity = 0;
uchar rec_dat[14];

//10us 精确延时
void DHT11_delay(uint n)
{
    while (--n)
        ;
}

//初始化 DHT11
void DHT11_int()
{
    DHT11_pin = 1;
    DHT11_delay(2);
    DHT11_pin = 0;
    delay(20);
    DHT11_pin = 1;
    delay(30);
}

//读取数据8位
uchar DHT11_read()
{
    uint i;
    uchar data1 = 0;
    for (i = 0; i < 8; i++) //从高到低接收8
    {                       //
        while (!DHT11_pin)  //
            ;               //
        DHT11_delay(8);     //延时80us，如果还为高则数据为1，否则为0
        data1 <<= 1;        //移位使正确接收8位数据，数据为0时直接移位
        if (DHT11_pin == 1) //数据为1时，使dat加1来接收数据1
            data1 += 1;     //
        while (DHT11_pin)   //
            ;               //等待数据线拉低
    }
}

//读取温度
void DHT11_temp()
{
    uchar R_H, R_L, T_H, T_L, RH,  TH, revise;
    DHT11_int();

    if (DHT11_pin == 0)
    {
lcd_12864_text(IC_DAT2);
        while (DHT11_pin == 0)
            ;

        DHT11_delay(40);    //拉高后延时80us
        R_H = DHT11_read();    //接收湿度高八位
        R_L = DHT11_read();    //接收湿度低八位
        T_H = DHT11_read();    //接收温度高八位
        T_L = DHT11_read();    //接收温度低八位
        revise = DHT11_read(); //接收校正位

        DHT11_delay(25); //结束

        if ((R_H + R_L + T_H + T_L) == revise) //校正
        {
            temperature = T_H;
            RH = R_H;
            humidity = R_H;
            TH = T_H;
        }

        /*数据处理，方便显示*/
        rec_dat[0] = '0' + (RH / 10);
        rec_dat[1] = '0' + (RH % 10);
        rec_dat[2] = ' ';
        rec_dat[3] = 'R';
        rec_dat[4] = 'H';
        rec_dat[5] = ' ';
        rec_dat[6] = ' ';
        rec_dat[7] = ' ';
        rec_dat[8] = ' ';
        rec_dat[9] = '0' + (TH / 10);
        rec_dat[10] = '0' + (TH % 10);
        rec_dat[11] = ' ';
        rec_dat[12] = 'C';
        rec_dat[13] = ' ';
        rec_dat[14] = ' ';
    }
}