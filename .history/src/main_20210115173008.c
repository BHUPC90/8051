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
uchar year_moon, month_moon, day_moon;

/*---------------------------------����ת������������ݱ�--------------------------------*/
uchar code year_code[] =
    {
        0x0C, 0x96, 0x45, //2000 297
        0x4d, 0x4A, 0xB8, //2001
        0x0d, 0x4A, 0x4C, //2002
        0x0d, 0xA5, 0x41, //2003
        0x25, 0xAA, 0xB6, //2004
        0x05, 0x6A, 0x49, //2005
        0x7A, 0xAd, 0xBd, //2006
        0x02, 0x5d, 0x52, //2007
        0x09, 0x2d, 0x47, //2008
        0x5C, 0x95, 0xBA, //2009
        0x0A, 0x95, 0x4e, //2010
        0x0B, 0x4A, 0x43, //2011
        0x4B, 0x55, 0x37, //2012
        0x0A, 0xd5, 0x4A, //2013
        0x95, 0x5A, 0xBf, //2014
        0x04, 0xBA, 0x53, //2015
        0x0A, 0x5B, 0x48, //2016
        0x65, 0x2B, 0xBC, //2017
        0x05, 0x2B, 0x50, //2018
        0x0A, 0x93, 0x45, //2019
        0x47, 0x4A, 0xB9, //2020
        0x06, 0xAA, 0x4C, //2021
        0x0A, 0xd5, 0x41, //2022
        0x24, 0xdA, 0xB6, //2023
        0x04, 0xB6, 0x4A, //2024
        0x69, 0x57, 0x3d, //2025
        0x0A, 0x4e, 0x51, //2026
        0x0d, 0x26, 0x46, //2027
        0x5e, 0x93, 0x3A, //2028
        0x0d, 0x53, 0x4d, //2029
        0x05, 0xAA, 0x43, //2030
        0x36, 0xB5, 0x37, //2031
        0x09, 0x6d, 0x4B, //2032
        0xB4, 0xAe, 0xBf, //2033
        0x04, 0xAd, 0x53, //2034
        0x0A, 0x4d, 0x48, //2035
        0x6d, 0x25, 0xBC, //2036
        0x0d, 0x25, 0x4f, //2037
        0x0d, 0x52, 0x44, //2038
        0x5d, 0xAA, 0x38, //2039
        0x0B, 0x5A, 0x4C, //2040
        0x05, 0x6d, 0x41, //2041
        0x24, 0xAd, 0xB6, //2042
        0x04, 0x9B, 0x4A, //2043
        0x7A, 0x4B, 0xBe, //2044
        0x0A, 0x4B, 0x51, //2045
        0x0A, 0xA5, 0x46, //2046
        0x5B, 0x52, 0xBA, //2047
        0x06, 0xd2, 0x4e, //2048
        0x0A, 0xdA, 0x42, //2049
        0x35, 0x5B, 0x37, //2050
        0x09, 0x37, 0x4B, //2051
        0x84, 0x97, 0xC1, //2052
        0x04, 0x97, 0x53, //2053
        0x06, 0x4B, 0x48, //2054
        0x66, 0xA5, 0x3C, //2055
        0x0e, 0xA5, 0x4f, //2056
        0x06, 0xB2, 0x44, //2057
        0x4A, 0xB6, 0x38, //2058
        0x0A, 0xAe, 0x4C, //2059
        0x09, 0x2e, 0x42, //2060
        0x3C, 0x97, 0x35, //2061
        0x0C, 0x96, 0x49, //2062
        0x7d, 0x4A, 0xBd, //2063
        0x0d, 0x4A, 0x51, //2064
        0x0d, 0xA5, 0x45, //2065
        0x55, 0xAA, 0xBA, //2066
        0x05, 0x6A, 0x4e, //2067
        0x0A, 0x6d, 0x43, //2068
        0x45, 0x2e, 0xB7, //2069
        0x05, 0x2d, 0x4B, //2070
        0x8A, 0x95, 0xBf, //2071
        0x0A, 0x95, 0x53, //2072
        0x0B, 0x4A, 0x47, //2073
        0x6B, 0x55, 0x3B, //2074
        0x0A, 0xd5, 0x4f, //2075
        0x05, 0x5A, 0x45, //2076
        0x4A, 0x5d, 0x38, //2077
        0x0A, 0x5B, 0x4C, //2078
        0x05, 0x2B, 0x42, //2079
        0x3A, 0x93, 0xB6, //2080
        0x06, 0x93, 0x49, //2081
        0x77, 0x29, 0xBd, //2082
        0x06, 0xAA, 0x51, //2083
        0x0A, 0xd5, 0x46, //2084
        0x54, 0xdA, 0xBA, //2085
        0x04, 0xB6, 0x4e, //2086
        0x0A, 0x57, 0x43, //2087
        0x45, 0x27, 0x38, //2088
        0x0d, 0x26, 0x4A, //2089
        0x8e, 0x93, 0x3e, //2090
        0x0d, 0x52, 0x52, //2091
        0x0d, 0xAA, 0x47, //2092
        0x66, 0xB5, 0x3B, //2093
        0x05, 0x6d, 0x4f, //2094
        0x04, 0xAe, 0x45, //2095
        0x4A, 0x4e, 0xB9, //2096
        0x0A, 0x4d, 0x4C, //2097
        0x0d, 0x15, 0x41, //2098
        0x2d, 0x92, 0xB5, //2099
};
/*-------------------------------------�·����ݱ�----------------------------------------*/
static unsigned char code day_code1[9] = {0x0, 0x1f, 0x3b, 0x5a, 0x78, 0x97, 0xb5, 0xd4, 0xf3};
static unsigned short code day_code2[3] = {0x111, 0x130, 0x14e};

/*�Ӻ���,���ڶ�ȡ���ݱ���ũ���µĴ��»�С��,�������Ϊ�󷵻�1,ΪС����0------------------*/
static unsigned char GetMoonDay(unsigned char month_p, unsigned short table_addr)
{
    switch (month_p)
    {
    case 1:
        if ((year_code[table_addr] & 0x08) == 0)
            return (0);
        else
            return (1);
    case 2:
        if ((year_code[table_addr] & 0x04) == 0)
            return (0);
        else
            return (1);
    case 3:
        if ((year_code[table_addr] & 0x02) == 0)
            return (0);
        else
            return (1);
    case 4:
        if ((year_code[table_addr] & 0x01) == 0)
            return (0);
        else
            return (1);
    case 5:
        if ((year_code[table_addr + 1] & 0x80) == 0)
            return (0);
        else
            return (1);
    case 6:
        if ((year_code[table_addr + 1] & 0x40) == 0)
            return (0);
        else
            return (1);
    case 7:
        if ((year_code[table_addr + 1] & 0x20) == 0)
            return (0);
        else
            return (1);
    case 8:
        if ((year_code[table_addr + 1] & 0x10) == 0)
            return (0);
        else
            return (1);
    case 9:
        if ((year_code[table_addr + 1] & 0x08) == 0)
            return (0);
        else
            return (1);
    case 10:
        if ((year_code[table_addr + 1] & 0x04) == 0)
            return (0);
        else
            return (1);
    case 11:
        if ((year_code[table_addr + 1] & 0x02) == 0)
            return (0);
        else
            return (1);
    case 12:
        if ((year_code[table_addr + 1] & 0x01) == 0)
            return (0);
        else
            return (1);
    case 13:
        if ((year_code[table_addr + 2] & 0x80) == 0)
            return (0);
        else
            return (1);
    }
    return (0);
}

/*--------------------------------����תũ������-----------------------------------------*/

/*--------------------------------����תũ������-----------------------------------------*/
//�����������10���Ʋ���
void Conversion(uchar year, uchar month, uchar day)
{
    uchar temp1, temp2, temp3, month_p;
    uint temp4, table_addr;
    bit flag2, flag_y;

    //��λ���ݱ��ַ
    table_addr = year * 0x3;

    //ȡ���괺�����ڵĹ����·�
    temp1 = year_code[table_addr + 2] & 0x60;
    temp1 = _cror_(temp1, 5);
    //ȡ���괺�����ڵĹ�����
    temp2 = year_code[table_addr + 2] & 0x1f;

    if (temp1 == 0x1)
    {
        temp3 = temp2 - 1;
    }
    else
    {
        temp3 = temp2 + 0x1f - 1;
    }

    if (month < 10)
    {
        temp4 = day_code1[month - 1] + day - 1;
    }
    else
    {
        temp4 = day_code2[month - 10] + day - 1;
    }

    if ((month > 0x2) && (year % 0x4 == 0))
        temp4 += 1; //��������´���2�²��Ҹ����2��Ϊ����,������1
    //���㹫�����뵱��Ԫ�����������

    //�жϹ������ڴ���ǰ���Ǵ��ں�
    if (temp4 >= temp3)
    {
        temp4 -= temp3;
        month = 0x1;
        month_p = 0x1;                           //month_pΪ�·�ָ��,�������ڴ���ǰ����Ǵ��ڵ���month_pָ������
        flag2 = GetMoonDay(month_p, table_addr); //����ũ����Ϊ��С����С��,���·���1,С�·���0
        flag_y = 0;
        if (flag2 == 0)
            temp1 = 0x1d; //С��29��
        else
            temp1 = 0x1e; //����30��
        temp2 = year_code[table_addr] & 0xf0;
        temp2 = _cror_(temp2, 4); //�����ݱ���ȡ����������·�,��Ϊ0�����������
        while (temp4 >= temp1)
        {
            temp4 -= temp1;
            month_p += 1;
            if (month == temp2)
            {
                flag_y = ~flag_y;
                if (flag_y == 0)
                    month += 1;
            }
            else
                month += 1;
            flag2 = GetMoonDay(month_p, table_addr);
            if (flag2 == 0)
                temp1 = 0x1d;
            else
                temp1 = 0x1e;
        }
        day = temp4 + 1;
    }
    else
    {
        temp3 -= temp4;

        table_addr -= 0x3;
        month = 0xc;
        temp2 = year_code[table_addr] & 0xf0;
        temp2 = _cror_(temp2, 4);
        if (temp2 == 0)
            month_p = 0xc;
        else
            month_p = 0xd;
        flag_y = 0;
        flag2 = GetMoonDay(month_p, table_addr);
        if (flag2 == 0)
            temp1 = 0x1d;
        else
            temp1 = 0x1e;
        while (temp3 > temp1)
        {
            temp3 -= temp1;
            month_p -= 1;
            if (flag_y == 0)
                month -= 1;
            if (month == temp2)
                flag_y = ~flag_y;
            flag2 = GetMoonDay(month_p, table_addr);
            if (flag2 == 0)
                temp1 = 0x1d;
            else
                temp1 = 0x1e;
        }
        day = temp1 - temp3 + 1;
    }
    year_moon = year;
    month_moon = month;
    day_moon = day;
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
    Ds1302readTime();
    Conversion((time[6] / 16) * 10 + (time[6] & 0x0f), (time[4] / 16) * 10 + (time[4] & 0x0f), (time[3] / 16) * 10 + (time[3] & 0x0f));
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
    lcd_12864_len_text(5, "    ũ��");
    temp[0] = ' ';
    temp[1] = ' ';
    temp[2] = '2';
    temp[3] = '0';
    temp[4] = '0' + year_moon / 10;
    temp[5] = '0' + year_moon % 10;
    temp[6] = '-';
    temp[7] = '0' + month_moon / 10;
    temp[8] = '0' + month_moon % 10;
    temp[9] = '-';
    temp[10] = '0' + day_moon / 10;
    temp[11] = '0' + day_moon % 10;
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