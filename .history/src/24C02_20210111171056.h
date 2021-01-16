#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif



void iic_start() //启动信号
void iic_init() //总线初始化
void iic_stop() //停止信号
void iic_sendByte(uchar byteData) //mcu发送一个字节
uchar iic_readByte() //读一个字节
int iic_checkACK() //处理应答信号
