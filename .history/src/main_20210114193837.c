/**************************************************************************************
*		                 С�������Ƶ�����												  *
��ʱ�䣺	2021��1��12��
�깤ʱ�䣺	2021��1��13�� 23:58
�����ˣ�	������ѧǶ��ʽʵ����-������		
QQ��        1792498113			
��ɣ�      ������ Mozila MPL ��Դ���
ʹ��ָ�ϣ�  ������(0xCA)��������(0x02)������ҳ������ȷ��(0xCE)��������˵�,����(0xC1)������������ѡ��		
��ע��      �ο����ף�https://wenku.baidu.com/view/c1968ce8b90d6c85ed3ac664.html
                     https://wenku.baidu.com/view/9882582aaaea998fcc220e63.html
                     https://wenku.baidu.com/view/375ec764cc22bcd127ff0c9a.html
                     https://wenku.baidu.com/view/5ff32b422f60ddccdb38a001.html
                     https://wenku.baidu.com/view/7d0d5994daef5ef7bb0d3c00.html
                     https://wenku.baidu.com/view/dcdc9efa700abb68a982fbe0.html
                     https://wenku.baidu.com/view/56ff1f1ffad6195f312ba608.html
                     https://www.it610.com/article/1249672601303896064.html
��л��      ��л��λ���������֧�ֵ�ѧ����ͬѧ��
***************************************************************************************/

#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "DHT11.h"

/*
����ͼ��
P3 ^ 6 <��������> SCLK //DS1302
P3 ^ 5 <��������> RST //DS1302
P3 ^ 4 <��������> DSIO //DS1302
P3 ^ 2 <��������> �����ж�
P2 ^ 7 <��������>   LCD12864_SCLK //ʱ��
P2 ^ 6 <��������>    LCD12864_CS //Ƭѡ
P2 ^ 5 <��������>   LCD12864_SID //����
P2 ^ 3 <��������> DHT11_pin
P2 ^ 2 <��������> ������
*/

sbit IRIN = P3 ^ 2;      //�����ж�
sbit beep = P2 ^ 2;      //������
uchar IrTime;            //������ռ�ʱ
uchar the_key_id;        //����ת�����޷�����
uchar maohao = 0;        //bool�ͣ��Ƿ���ʾ:
char page = 0;           //������ҳ
uchar mode = 0;          //һ���˵���ѡ�еĵ�ǰ�˵���Ŀ 0-3
uchar set_time_mode = 0; //��ǰ����ʱ����ʲô
uchar state = 0;         //�жϲ˵�״̬
uchar temp[30];          //������Ļ��ʾ


char YearFlag(uchar cYear) //�ж�ƽ�꣬����
{
   if( (!(cYear%4) && (cYear%100)) || !(cYear%400) ) return 1; else return 0;
}



/*******************************************************************************
* ������         : IrInit()
* ��������		 : ��ʼ�������߽���
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void IrInit()
{
    IT0 = 1;  //�½��ش���
    EX0 = 1;  //���ж�0����
    EA = 1;   //�����ж�
    IRIN = 1; //��ʼ���˿�
}

/*******************************************************************************
* ������         : setup()
* ��������		 : ������������ʼ����Ļ����ʾ��ӭҳ�棬��ȡ���ӣ���ȡʱ��
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void setup()
{
    IrInit();
    int_lcd_12864();
    lcd_12864_write_com(0x00);
    //////////////////////////////
    read_alarm();
    /////////////////////////////
    int_lcd_12864();
    lcd_12864_write_com(0x00);
}

/*******************************************************************************
* ������         : readtime()
* ��������		 : ��ҳ�����12864ѭ����ʾʱ��
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void readtime()
{
    DHT11_temp();
    Ds1302readTime();
    lcd_12864_len_text(1, "  ����        ��");
    temp[0] = ' ';
    temp[1] = ' ';
    temp[2] = '2';
    temp[3] = '0';
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
    temp[15] = '0' + (time[5] & 0x0f);
    temp[16] = '\0';
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
    lcd_12864_len_text(5, "    ����");
    temp[0] = ' ';
    temp[1] = ' ';
    temp[2] = ' ';
    temp[3] = ' ';
    temp[4] = '0' + alarm_time[2] / 16;
    temp[5] = '0' + (alarm_time[2] & 0x0f);
    temp[6] = ':';
    temp[7] = '0' + alarm_time[1] / 16;
    temp[8] = '0' + (alarm_time[1] & 0x0f);
    temp[9] = ':';
    temp[10] = '0' + alarm_time[0] / 16;
    temp[11] = '0' + (alarm_time[0] & 0x0f);
    temp[12] = '\0';
    lcd_12864_len_text(6, temp);
    lcd_12864_len_text(7, "�¶�    ʪ��");
    temp[0] = '0' + (humidity / 10);
    temp[1] = '0' + (humidity % 10);
    temp[2] = ' ';
    temp[3] = ' ';
    temp[4] = ' ';
    temp[5] = ' ';
    temp[6] = ' ';
    temp[7] = ' ';
    temp[8] = '0' + (temperature / 10);
    temp[9] = '0' + (temperature % 10);
    temp[10] = '\0';
    lcd_12864_len_text(8, temp);
}

/*******************************************************************************
* ������         : loop()
* ��������		 : ��ѭ��������������Ļ����˸��ˢ��ʱ�䣬�������
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void loop()
{
    if (state == 0)
    {
        readtime();
    }
    maohao = !maohao;
    if (time[0] == alarm_time[0] && time[1] == alarm_time[1] && time[2] == alarm_time[3])
    {
        state = 4;
        lcd_12864_write_com(0x00);
        //lcd_12864_len_text(1, "�����ˣ�");
    }
    while (state == 4)
    {
        beep = ~beep;
        delaywhile(10);
    }

    delay(500);
}

/*******************************************************************************
* ������         : changemenu_wive()
* ��������		 : ���˵�ѡ������������ʾѡ�в˵�
* ����           : ѡ�в˵���Ŀ�������� 0-3
* ���         	 : ��
*******************************************************************************/
void changemenu_wive(int menuid)
{
    lcd_12864_len_text(1, "    ");
    lcd_12864_len_text(2, "    ");
    lcd_12864_len_text(3, "    ");
    lcd_12864_len_text(4, "    ");
    switch (menuid)
    {
    case 0:
        lcd_12864_len_text(1, "  ��");
        break;
    case 1:
        lcd_12864_len_text(2, "  ��");
        break;
    case 2:
        lcd_12864_len_text(3, "  ��");
        break;
    case 3:
        lcd_12864_len_text(4, "  ��");
        break;
    default:
        break;
    }
}

/*******************************************************************************
* ������         : change_time_alm()
* ��������		 : ��Ļ��ʾ�������ӣ����ڶ�Ӧλ����˸���
* ����           : ���ù��λ�ã��롢�֡�ʱ�������� 0-2
* ���         	 : ��
*******************************************************************************/
void change_time_alm(int set_id)
{
    lcd_12864_len_text(3, "��������");
    temp[0] = '0' + alarm_time[2] / 16;
    temp[1] = '0' + (alarm_time[2] & 0x0f);
    temp[2] = ':';
    temp[3] = ':';
    temp[4] = '0' + alarm_time[1] / 16;
    temp[5] = '0' + (alarm_time[1] & 0x0f);
    temp[6] = ':';
    temp[7] = ':';
    temp[8] = '0' + alarm_time[0] / 16;
    temp[9] = '0' + (alarm_time[0] & 0x0f);
    temp[10] = '\0';
    lcd_12864_len_text(4, temp);
    //�ƶ���Ӧλ�ã����Ϊ�������أ�����Ϊż��
    //https://zhidao.baidu.com/question/617361091139224292.html
    switch (set_time_mode)
    {
    case 2:
        lcd_12864_write_com(0x98);
        break;
    case 1:
        lcd_12864_write_com(0x9A);
        break;
    case 0:
        lcd_12864_write_com(0x9C);
        break;
    default:
        break;
    }
    lcd_12864_write_com(0x0f); //��ʾ���
}

/*******************************************************************************
* ������         : change_alm()
* ��������		 : ��������ʱ�亯��
* ����           : Ture Ϊ��ǰ���1��Flase Ϊ��ǰ���1 alarm_time ���롢�֡�ʱ
* ���         	 : ��
*******************************************************************************/
void change_alm(int mmod)
{

    if (mmod)
    {
        alarm_time[set_time_mode] = alarm_time[set_time_mode] + 0x01;
    }
    else
    {
        alarm_time[set_time_mode] = alarm_time[set_time_mode] - 0x01;
    }

    change_time_alm();
}

/*******************************************************************************
* ������         : change_time_view()
* ��������		 : ��Ļ��ʾ����ʱ�䣬���ڶ�Ӧλ����˸���
* ����           : ���ù��λ�ã��롢�֡�ʱ���ա��¡��ܡ��� 0-6
* ���         	 : ��
*******************************************************************************/
void change_time_view(int set_id)
{

    lcd_12864_len_text(1, "��������    ��");
    temp[0] = '0' + time[6] / 16;
    temp[1] = '0' + (time[6] & 0x0f);
    temp[2] = '-';
    temp[3] = '-';
    temp[4] = '0' + time[4] / 16;
    temp[5] = '0' + (time[4] & 0x0f);
    temp[6] = '-';
    temp[7] = '-';
    temp[8] = '0' + time[3] / 16;
    temp[9] = '0' + (time[3] & 0x0f);
    temp[10] = ' ';
    temp[11] = ' ';
    temp[12] = '0' + time[5] / 16;
    temp[13] = '0' + (time[5] & 0x0f);
    temp[14] = '\0';
    lcd_12864_len_text(2, temp);
    lcd_12864_len_text(3, "����ʱ��");
    temp[0] = '0' + time[2] / 16;
    temp[1] = '0' + (time[2] & 0x0f);
    temp[2] = ':';
    temp[3] = ':';
    temp[4] = '0' + time[1] / 16;
    temp[5] = '0' + (time[1] & 0x0f);
    temp[6] = ':';
    temp[7] = ':';
    temp[8] = '0' + time[0] / 16;
    temp[9] = '0' + (time[0] & 0x0f);
    temp[10] = '\0';
    lcd_12864_len_text(4, temp);
    switch (set_time_mode)
    {
    case 6:
        lcd_12864_write_com(0x90);
        break;
    case 5:
        lcd_12864_write_com(0x96);
        break;
    case 4:
        lcd_12864_write_com(0x92);
        break;
    case 3:
        lcd_12864_write_com(0x94);
        break;
    case 2:
        lcd_12864_write_com(0x98);
        break;
    case 1:
        lcd_12864_write_com(0x9A);
        break;
    case 0:
        lcd_12864_write_com(0x9C);
        break;
    default:
        break;
    }
    lcd_12864_write_com(0x0f); //��ʾ���
}

/*******************************************************************************
* ������         : change_time()
* ��������		 : ����ʱ�亯��
* ����           : Ture Ϊ��ǰ���1��Flase Ϊ��ǰ���1 time����\��\ʱ\��\��\��\��
* ���         	 : ��
*******************************************************************************/
void change_time(int mmod)
{
    if (mmod)
    {
        time[set_time_mode] = time[set_time_mode] + 0x01;
    }
    else
    {
        time[set_time_mode] = time[set_time_mode] - 0x01;
    }
    change_time_view();
}

/*******************************************************************************
* ������         : Irdo()
* ��������		 : ������մ�����
* ����           : ��
* ���         	 : ��
*******************************************************************************/
void Irdo()
{
    if (the_key_id == 14) //��ťȷ��
    {
        if (state == 1)
        {
            lcd_12864_write_com(0x00);
            int_lcd_12864();
            switch (mode)
            {
            case 0:
                state = 2;
                Ds1302readTime();
                change_time_view(5);
                return;
            case 1:
                state = 3;
                read_alarm();
                change_time_alm(5);
                return;
            case 2:
                state = 0;
                return;
            case 3:
                //����ˢ��////
                read_alarm();
                lcd_12864_write_com(0x00);
                int_lcd_12864();
                /////////////
                state = 0;
                return;
            }
        }
        if (state == 2)
        {
            ds1302init();
            lcd_12864_write_com(0x00);
            int_lcd_12864();
            state = 0;
            mode = 0;
            return;
        }
        if (state == 3)
        {
            set_alarm();
            lcd_12864_write_com(0x00);
            int_lcd_12864();
            state = 0;
            mode = 0;
            return;
        }
        if (state == 4)
        {
            state = 0;
            return;
        }
        if (state == 0)
        {
            int_lcd_12864();
            lcd_12864_write_com(0x00);
            lcd_12864_len_text(1, "    ����ʱ��    ");
            lcd_12864_len_text(2, "    ��������    ");
            lcd_12864_len_text(3, "    ��������    ");
            lcd_12864_len_text(4, "    ����ʱ��    ");
            changemenu_wive(0);
            state = 1;
        }
    }
    if (the_key_id == 10) //��ť��
    {
        if (state == 0)
        {
            //��ҳ����
            page -= 32;
            if (page < 0)
                page = 64;
            lcd_12864_write_com(0x34);
            lcd_12864_write_com(0x03);
            lcd_12864_write_com(0x40 + page);
            int_lcd_12864();
        }
        if (state == 1)
        {
            mode--;
            if (mode < 0)
                mode = 3;
            changemenu_wive(mode);
        }
        if (state == 2)
        {
            change_time(1);
        }
        if (state == 3)
        {
            change_alm(1);
        }
    }
    if (the_key_id == 2) //��ť��
    {
        if (state == 0)
        {
            page += 32;
            if (page > 64)
                page = 0;
            lcd_12864_write_com(0x34);
            lcd_12864_write_com(0x03);
            lcd_12864_write_com(0x40 + page);
            int_lcd_12864();
        }
        if (state == 1)
        {
            mode++;
            if (mode > 3)
                mode = 0;
            changemenu_wive(mode);
        }
        if (state == 2)
        {
            change_time(0);
        }
        if (state == 3)
        {
            change_alm(0);
        }
    }
    if (the_key_id == 1) //��ť��
    {
        if (state == 2) //����ʱ��
        {
            set_time_mode++;
            if (set_time_mode > 6)
                set_time_mode = 0;
            change_time_view(set_time_mode);
        }
        if (state == 3) //��������
        {
            set_time_mode++;
            if (set_time_mode > 2)
                set_time_mode = 0;
            change_time_alm(set_time_mode);
        }
    }
}

/*******************************************************************************
* ������         : ReadIr()
* ��������		 : ��ȡ������ֵ���жϺ���
* ����           : ��
* ���         	 : ��
*******************************************************************************/
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

            for (j = 0; j < 16; j++) //����һ������
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
                }
                IrTime = 0; //����ʱ��Ҫ���¸�ֵ
            }

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
                the_key_id >>= 1; //k��ʾ�ڼ�������
                if (IrTime >= 8)  //����ߵ�ƽ���ִ���565us����ô��1
                {
                    the_key_id |= 0x80; //1000 0000
                }
                IrTime = 0; //����ʱ��Ҫ���¸�ֵ
            }
        }
        the_key_id = the_key_id % 16;
        Irdo();
    }
}
//����1,190�д���