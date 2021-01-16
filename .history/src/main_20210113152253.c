#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "huatu.h"

sbit IRIN = P3 ^ 2;
uint IrTime;
uchar IrValue[6];
uint the_key_id;

uint state = 0;

uchar temp[30];

void IrInit()
{
    IT0 = 1;  //�½��ش���
    EX0 = 1;  //���ж�0����
    EA = 1;   //�����ж�
    IRIN = 1; //��ʼ���˿�
}

void show_menu()
{
    lcd_12864_len_text(1, "    ��    ��    ");
    lcd_12864_len_text(2, "    ����ʱ��    ");
    lcd_12864_len_text(3, "  ������������   ");
    lcd_12864_len_text(3, "      ����       ");
}

void readtime()
{
    Ds1302readTime();
    temp[0] = ' ';
    temp[1] = '0' + the_key_id;
    temp[2] = '\0';
    lcd_12864_len_text(2, temp);
}

void setup()
{
    IrInit();
    //////////////////////////////
    lcd_12864_picture(firstpage);
    //////////////////////////////

    /////////////////////////////
    delay(500);
    int_lcd_12864();
}

void loop()
{
    if (state == 0)
        readtime();
    if (state == 1)
        ;
    delay(1000);
}

void Irdo()
{
    if (the_key_id == 14)
    {
        show_menu();
        state = 1;
    }
}

void ReadIr() interrupt 0
{
    uchar j, k;
    uint err;
    IrTime = 0;
    delaywhile(700); //7ms
    if (IRIN == 0)   //ȷ���Ƿ���Ľ��յ���ȷ���ź�
    {

        err = 1000; //1000*10us=10ms,����˵�����յ�������ź�
        /*������������Ϊ����ѭ���������һ������Ϊ�ٵ�ʱ������ѭ������ó�������ʱ
		�������������*/
        while ((IRIN == 0) && (err > 0)) //�ȴ�ǰ��9ms�ĵ͵�ƽ��ȥ
        {
            delaywhile(1);
            err--;
        }
        if (IRIN == 1) //�����ȷ�ȵ�9ms�͵�ƽ
        {
            err = 500;
            while ((IRIN == 1) && (err > 0)) //�ȴ�4.5ms����ʼ�ߵ�ƽ��ȥ
            {
                delaywhile(1);
                err--;
            }
            for (k = 0; k < 4; k++) //����4������
            {
                for (j = 0; j < 8; j++) //����һ������
                {

                    err = 60;
                    while ((IRIN == 0) && (err > 0)) //�ȴ��ź�ǰ���560us�͵�ƽ��ȥ
                    {
                        delaywhile(1);
                        err--;
                    }
                    err = 500;
                    while ((IRIN == 1) && (err > 0)) //����ߵ�ƽ��ʱ�䳤�ȡ�
                    {
                        delaywhile(10); //0.1ms
                        IrTime++;
                        err--;
                        if (IrTime > 30)
                        {
                            return;
                        }
                    }
                    IrValue[k] >>= 1; //k��ʾ�ڼ�������
                    if (IrTime >= 8)  //����ߵ�ƽ���ִ���565us����ô��1
                    {
                        IrValue[k] |= 0x80;
                    }
                    IrTime = 0; //����ʱ��Ҫ���¸�ֵ
                }
            }
        }
        if (IrValue[2] != ~IrValue[3])
        {
            return;
        }
        the_key_id = IrValue[2] % 16;
        Irdo();
    }
}