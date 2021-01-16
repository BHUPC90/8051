#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif

uchar fm_address = 0xA0; //硬件地址
//普中科技开发板: 0xA0

/*
AT24C02内部设有一个8位控制寄存器，其每一位的含义如下：
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
1      0     1    0    A2   A1   A0  R/W
A2/A1/A0用于选择总线上待访问的I2C器件，R/W=1读操作，R/W=0写操作；
I2C总线上最多可以扩展8片同样的2K容量EEPROM存储器,
或者是4片4Kb的EEPROM,
或者是2片容量为8Kb的EEPROM存储器。
或者是1片容量为16Kb的EEPROM存储器
此时硬件就固定了,因为A2/A1/A0已经被P2P1P0占用
*/

void AT24C02_writeByte(uchar address, uchar dataByte) //向24c02写一字节数据
{
    iic_start();
	iic_sendByte(fm_address);//发送写器件地址
    iic_checkACK();
	iic_sendByte(address);//发送要写入内存地址
    iic_checkACK();
	iic_sendByte(dataByte);	//发送数据
    iic_checkACK();
	iic_stop();
    //按字节写入时,24c02在接收到停止信号后将数据擦写到内部,这需要时间
    //并且在这段时间内不会响应总线上的任何请求,故让mcu有2毫秒以上的等待
}

void AT24C02_writeData(uchar address, uchar numBytes, uchar *buf) //写入任意长度数据
{
    while (numBytes--)
    {
        AT24C02_writeByte(address++, *buf++);
    }
}

void AT24C02_readData(uchar beginAddr, uchar dataSize, uchar *buf) //读取任意长度字节
{    
    iic_start();              //起始信号
    iic_sendByte(fm_address); //控制字,写
    iic_checkACK();           //处理应答信号
    iic_sendByte(beginAddr);  //发送地址
    iic_checkACK();           //处理应答信号
    iic_start();              //发送起始信号
    iic_sendByte(0xa1);       //控制字,读
    iic_checkACK();           //处理应答信号
    while (dataSize--)        //读取dataSize个字节
    {
        *buf++ = iic_readByte(); //读取一个个字节并保存到缓冲区buf中
    }
    iic_stop(); //发送停止信号
}

uchar AT24C02_readData_Byte(uchar addr)
{
	uchar num;
	iic_start();
	iic_sendByte(fm_address); //发送写器件地址
    iic_checkACK();
	iic_sendByte(addr); //发送要读取的地址
    iic_checkACK();
	iic_start();
	iic_sendByte(0xa1); //发送读器件地址
    iic_checkACK();
	num=iic_readByte(); //读取数据
	iic_stop();
	return num;		
}