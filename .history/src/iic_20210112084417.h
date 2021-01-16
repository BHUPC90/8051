#ifndef _INC_iic
#define _INC_iic
#define uchar unsigned char
#define uint unsigned int
#endif

sbit SCL = P2 ^ 1;
sbit SDA = P2 ^ 0;

//最大255，一个机器周期为1us，最大延时255us。
void Delay10us()
{
    unsigned char a, b;
    for (b = 1; b > 0; b--)
        for (a = 2; a > 0; a--)
            ;
}

void iic_init() //总线初始化
{
    SCL = 1;
    SDA = 1;
    Delay10us();
}

void iic_start() //启动信号
{
    SDA = 1;
    Delay10us();
    SCL = 1;
    Delay10us(); //建立时间是SDA保持时间>4.7us
    SDA = 0;
    Delay10us(); //保持时间是>4us
    SCL = 0;
    Delay10us();
}

void iic_stop() //停止信号
{

    SDA = 0;
    Delay10us();
    SCL = 1;
    Delay10us(); //建立时间大于4.7us
    SDA = 1;
    Delay10us();
}

void iic_sendByte(uchar byteData) //mcu发送一个字节
{
    unsigned char a = 0, b = 0; 
    for (a = 0; a < 8; a++)     //要发送8位，从最高位开始
    {
        SDA = dat >> 7; //起始信号之后SCL=0，所以可以直接改变SDA信号
        dat = dat << 1;
        Delay10us();
        SCL = 1;
        Delay10us(); //建立时间>4.7us
        SCL = 0;
        Delay10us(); //时间大于4us
    }
    SDA = 1;
    Delay10us();
    SCL = 1;
    while (SDA) //等待应答，也就是等待从设备把SDA拉低
    {
        b++;
        if (b > 200) //如果超过2000us没有应答发送失败，或者为非应答，表示接收结束
        {
            SCL = 0;
            Delay10us();
            return 0;
        }
    }
    SCL = 0;
    Delay10us();
    return 1;
}

uchar iic_readByte() //读一个字节
{
    uchar i, temp;
    scl = 0; //准备读数据
    _nop_();
    sda = 1; //释放总线
    _nop_();

    for (i = 0; i < 8; i++)
    {
        scl = 1; //mcu开始取数据

        _nop_();
        _nop_();
        _nop_();
        _nop_();                  //scl为高电平后,ic器件就会将1位数据送到sda上
                                  //总共用时不会大于4.34us的,然后就可以让mcu读sda了
        temp = (temp << 1) | sda; //读一位保存到temp中
        scl = 0;

        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
    return temp;
}

int iic_checkACK() //处理应答信号
{
    uchar errCounts = 255; //定义超时量为255次
    scl = 1;
    _nop_();

    while (sda)
    { //在一段时间内检测到sda=0的话认为是应答信号
        if (0 == errCounts)
        {
            scl = 0; //钳住总线
            _nop_();
            return 0; //没有应答信号
        }
        errCounts--;
    }

    scl = 0; //钳住总线,为下1次通信做准备
    _nop_();
    return 1; //成功处理应答信号
}

void iic_sendACK(int b_ACK) //发送应答或非应答信号
{
    scl = 0; //准备
    _nop_();

    if (b_ACK) //ACK
    {
        sda = 0;
    }
    else //unACK
    {
        sda = 1;
    }

    _nop_();
    scl = 1;

    _nop_();
    _nop_();
    _nop_();
    _nop_(); //大于4us的延时

    scl = 0; //钳住scl,以便继续接收数据
    _nop_();
}