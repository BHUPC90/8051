#include "qi51arduino.h"
//串口通信
void setup()
{
    EA = 1;  //设置CPU开启中断
    ES = 1;  //打开串口中断
    REN = 1; //打开串口接收
    //////////////////////////////////////////////////////////////////////////////////
    TMOD = 0x20; //定时器1设置模式
    TR1 = 1;     //打开定时器1中断
    TH1 = 253;   //115200
    TL1 = 253;   //115200
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
}
void loop()
{
    SBUF = 0XAA;
    while (!TI)
        ;
    TI = 0;
    delay(1000);
}
