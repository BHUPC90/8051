#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "huatu.h"

sbit IRIN = P3 ^ 2;
uint IrTime;
uchar IrValue[6];
uint the_key_id;

int maohao = 0;
int page = 0;

uint state = 0;

uchar temp[30];

void IrInit()
{
    IT0 = 1;  //�½��ش���
    EX0 = 1;  //���ж�0����
    EA = 1;   //�����ж�
    IRIN = 1; //��ʼ���˿�
}

void set_time()
{
    int_lcd_12864();
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, "    ����ʱ��    ");
    lcd_12864_len_text(2, "    ��������    ");
    lcd_12864_len_text(3, "    ��������    ");
    lcd_12864_len_text(4, "    ����ʱ��    ");
}

void readtime()
{
    lcd_12864_len_text(1, "    ����");
    Ds1302readTime();
    temp[0] = ' ';
    temp[1] = ' ';
    temp[2] = ' ';
    temp[3] = ' ';
    temp[4] = '0' + time[6] / 16;
    temp[5] = '0' + (time[6] & 0x0f);
    temp[6] = '-';
    temp[7] = '0' + time[4] / 16;
    temp[8] = '0' + (time[4] & 0x0f);
    temp[9] = '-';
    temp[10] = '0' + time[3] / 16;
    temp[11] = '0' + (time[3] & 0x0f);
    temp[12] = ' ';
    temp[13] = ' ';
    temp[14] = ' ';
    temp[15] = '\0';
    lcd_12864_len_text(2, temp);
    lcd_12864_len_text(3, "    ʱ��");
    temp[0] = ' ';
    temp[1] = ' ';
    temp[2] = ' ';
    temp[3] = ' ';
    temp[4] = '0' + time[2] / 16;
    temp[5] = '0' + (time[2] & 0x0f);
    if (maohao)
        temp[6] = ':';
    else
        temp[6] = ' ';
    temp[7] = '0' + time[1] / 16;
    temp[8] = '0' + (time[1] & 0x0f);
    if (maohao)
        temp[9] = ':';
    else
        temp[9] = ' ';
    temp[10] = '0' + time[0] / 16;
    temp[11] = '0' + (time[0] & 0x0f);
    temp[12] = '\0';
    lcd_12864_len_text(4, temp);
    lcd_12864_len_text(4, "    ���·�ҳ");
}

void setup()
{
    IrInit();
    int_lcd_12864();
    lcd_12864_write_com(0x00);
    //////////////////////////////
    lcd_12864_picture(firstpage);
    //////////////////////////////

    /////////////////////////////
    delay(500);
    int_lcd_12864();
    lcd_12864_write_com(0x00);
}

void loop()
{
    if (state == 0)
        readtime();
    maohao = !maohao;
    delay(500);
}

void Irdo()
{
    if (the_key_id == 14)
    {
        set_time();
        state = 1;
    }
    if (the_key_id == 10)
    {
        if (state == 0 && page >= 0)
        {
            page -= 16;
            lcd_12864_write_com(0x34);
            lcd_12864_write_com(0x03);
            lcd_12864_write_com(0x40 + page);
            int_lcd_12864();
        }
    }
    if (the_key_id == 2)
    {
        if (state == 0 && page <= 64)
        {
            page += 16;
            lcd_12864_write_com(0x34);
            lcd_12864_write_com(0x03);
            lcd_12864_write_com(0x40 + page);
            int_lcd_12864();
        }
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