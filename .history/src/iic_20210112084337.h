#ifndef _INC_iic
#define _INC_iic
#define uchar unsigned char
#define uint unsigned int
#endif

sbit SCL=P2^1;
sbit SDA=P2^0;

void Delay10us()
{
	unsigned char a,b;
	for(b=1;b>0;b--)
		for(a=2;a>0;a--);

}

void iic_init() //总线初始化
{
    SCL = 1;
    SDA = 1;
    Delay10us();
}

void iic_start() //启动信号
{
    SDA=1;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间是SDA保持时间>4.7us
	SDA=0;
	Delay10us();//保持时间是>4us
	SCL=0;			
	Delay10us();
}

void iic_stop() //停止信号
{

    SDA=0;
	Delay10us();
	SCL=1;
	Delay10us();//建立时间大于4.7us
	SDA=1;
	Delay10us();
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