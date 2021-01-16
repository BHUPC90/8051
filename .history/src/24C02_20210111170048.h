#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif

sbit sda = P2 ^ 0;
sbit scl = P2 ^ 1;

void iic_start() //启动信号
{
    sda = 1;
    scl = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda和scl同为高电平保持4.7us以上

    _nop_(); //1.085us,共5.78us,下面sda=0是下降沿,不能计算在延时时间中
    sda = 0; //下降沿

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda低电平保持4us以上        ,这里是4.34us满足要求
}

void iic_stop() //停止信号
{
    sda = 0;
    _nop_(); //准备状态
    scl = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //该状态稳定时间要求保持4us以上

    sda = 1; //scl高电平期间,sda来一个上升沿

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //sda保持4.7us以上,4.34加上函数返回时间大于4.7us
             //注:此时scl和sda都为1
}

void iic_sendByte(uchar byteData) //mcu发送一个字节
{
    uchar i;
    uchar temp = byteData;
    for (i = 0; i < 8; i++)
    {
        temp = temp << 1; //移动后最高位到了PSW寄存器的CY位中,溢出寄存器
        scl = 0;          //准备
        _nop_();          //稳定一下
        sda = CY;         //将待发送的数据一位位的放到sda上
        _nop_();
        scl = 1; //每一个高电平期间,ic器件都会将数据取走
        _nop_();
    }

    scl = 0; //如果写成scl=1;sda=1就是停止信号,不能这么写
    _nop_();
    sda = 1; //释放总线,数据总线不用时要释放
    _nop_();
}