#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif



void iic_start() //�����ź�
void iic_init() //���߳�ʼ��
void iic_stop() //ֹͣ�ź�
void iic_sendByte(uchar byteData) //mcu����һ���ֽ�
uchar iic_readByte() //��һ���ֽ�
int iic_checkACK() //����Ӧ���ź�
