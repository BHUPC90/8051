#ifndef _INC_24C02
#define _INC_24C02
#define uchar unsigned char
#define uint unsigned int
#endif

uchar fm_address = 0xA0; //Ӳ����ַ
//���пƼ�������: 0xA0

/*
AT24C02�ڲ�����һ��8λ���ƼĴ�������ÿһλ�ĺ������£�
Bit7 Bit6 Bit5 Bit4 Bit3 Bit2 Bit1 Bit0
1      0     1    0    A2   A1   A0  R/W
A2/A1/A0����ѡ�������ϴ����ʵ�I2C������R/W=1��������R/W=0д������
I2C��������������չ8Ƭͬ����2K����EEPROM�洢��,
������4Ƭ4Kb��EEPROM,
������2Ƭ����Ϊ8Kb��EEPROM�洢����
������1Ƭ����Ϊ16Kb��EEPROM�洢��
��ʱӲ���͹̶���,��ΪA2/A1/A0�Ѿ���P2P1P0ռ��
*/

void AT24C02_writeByte(uchar address, uchar dataByte) //��24c02дһ�ֽ�����
{
    iic_start();
	iic_sendByte(fm_address);//����д������ַ
    iic_checkACK();
	iic_sendByte(address);//����Ҫд���ڴ��ַ
    iic_checkACK();
	iic_sendByte(dataByte);	//��������
    iic_checkACK();
	iic_stop();
    //���ֽ�д��ʱ,24c02�ڽ��յ�ֹͣ�źź����ݲ�д���ڲ�,����Ҫʱ��
    //���������ʱ���ڲ�����Ӧ�����ϵ��κ�����,����mcu��2�������ϵĵȴ�
}

void AT24C02_writeData(uchar address, uchar numBytes, uchar *buf) //д�����ⳤ������
{
    while (numBytes--)
    {
        AT24C02_writeByte(address++, *buf++);
    }
}

void AT24C02_readData(uchar beginAddr, uchar dataSize, uchar *buf) //��ȡ���ⳤ���ֽ�
{    
    iic_start();              //��ʼ�ź�
    iic_sendByte(fm_address); //������,д
    iic_checkACK();           //����Ӧ���ź�
    iic_sendByte(beginAddr);  //���͵�ַ
    iic_checkACK();           //����Ӧ���ź�
    iic_start();              //������ʼ�ź�
    iic_sendByte(0xa1);       //������,��
    iic_checkACK();           //����Ӧ���ź�
    while (dataSize--)        //��ȡdataSize���ֽ�
    {
        *buf++ = iic_readByte(); //��ȡһ�����ֽڲ����浽������buf��
    }
    iic_stop(); //����ֹͣ�ź�
}

uchar AT24C02_readData_Byte(uchar addr)
{
	uchar num;
	iic_start();
	iic_sendByte(fm_address); //����д������ַ
    iic_checkACK();
	iic_sendByte(addr); //����Ҫ��ȡ�ĵ�ַ
    iic_checkACK();
	iic_start();
	iic_sendByte(0xa1); //���Ͷ�������ַ
    iic_checkACK();
	num=iic_readByte(); //��ȡ����
	iic_stop();
	return num;		
}