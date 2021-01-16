#ifndef _INC_UART
#define _INC_UART
#define uchar unsigned char
#define uint unsigned int
#endif
void delay(unsigned int wait_time);
void Serial_begin(uint speed);
void Serial_print_byte(uchar print_date);
void Serial_print(uchar print_date);
int Serial_available = 0;
uchar re_data = ' ';
/*
测试串口通信，和时钟1冲突
波特率：253 -- 9600 （请计算）
*/
void Serial_begin(uint speed)
{
    EA = 1;  //设置CPU开启中断
    ES = 1;  //打开串口中断
    REN = 1; //打开串口接收
    //////////////////////////////////////////////////////////////////////////////////
    TMOD = 0x20; //定时器1设置模式
    TR1 = 1;     //打开定时器1中断
    TH1 = speed; //设置串口波特率
    TL1 = speed;
    /*  //////////////////////////////////////////////////////////////////////////////
        SM0=0&SM1=0：模式0，移位寄存器控制I/O，波特率固定为工作频率/12。
        SM0=0&SM1=1：模式1，8位串行数据传送，波特率由计时器1来控制。
        SM0=1&SM1=0：模式2，9位串行数据传送，波特率可分为2种，工作频率/32或工作频率/64。
        SM0=1&SM1=1：模式3，9位串行数据传送，波特率可用计时器1控制。
    */
    SM0 = 0;
    SM1 = 1;
    /////////////////////////////////////////////////////////////////////////////////
    RI = 0; //清空接收标识
    TI = 0; //清空发送标识
}

/*发送一个字符*/
void Serial_print_byte(uchar print_date)
{
    SBUF = print_date; //把数据放到SBUF中
    while (TI == 0)
        ;   //等待发送
    TI = 0; //发送完毕后，清除发送标识
}

//打印字符串
void Serial_print(uchar print_date[])
{
    int num = 0;
    while (1)
    {
        if (print_date[num] == '\0')
        {
            break;
        }
        Serial_print_byte(print_date[num]);
        num++;
        delay(2);
    }
}

void Serial_have_read() interrupt 4
{
    re_data = SBUF; //出去,接,收到的数据
    RI = 0;         //清除接收中断标志位
    Serial_available = 1;
}