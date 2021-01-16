/**************************************************************************************
*		                 小奇红外控制电子钟												  *
企划时间：	2021年1月12日
完工时间：	2021年1月13日 23:58
制作人：	北华大学嵌入式实验室-刘佳奇		
QQ：        1792498113			
许可：      请遵守 Mozila MPL 开源许可
使用指南：  按“上(0xCA)”、“下(0x02)”键翻页，按“确认(0xCE)”键进入菜单,“右(0xC1)”来调节设置选中		
备注：      参考文献：https://wenku.baidu.com/view/c1968ce8b90d6c85ed3ac664.html
                     https://wenku.baidu.com/view/9882582aaaea998fcc220e63.html
                     https://wenku.baidu.com/view/375ec764cc22bcd127ff0c9a.html
                     https://wenku.baidu.com/view/5ff32b422f60ddccdb38a001.html
                     https://wenku.baidu.com/view/7d0d5994daef5ef7bb0d3c00.html
                     https://wenku.baidu.com/view/dcdc9efa700abb68a982fbe0.html
                     https://wenku.baidu.com/view/56ff1f1ffad6195f312ba608.html
                     https://www.it610.com/article/1249672601303896064.html
鸣谢：      感谢各位给予帮助和支持的学长和同学！
***************************************************************************************/

#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "DHT11.h"

/*
引脚图：
P3 ^ 6 <————> SCLK //DS1302
P3 ^ 5 <————> RST //DS1302
P3 ^ 4 <————> DSIO //DS1302
P3 ^ 2 <————> 红外中断
P2 ^ 7 <————>   LCD12864_SCLK //时钟
P2 ^ 6 <————>    LCD12864_CS //片选
P2 ^ 5 <————>   LCD12864_SID //数据
P2 ^ 3 <————> DHT11_pin
P2 ^ 2 <————> 蜂鸣器
*/

sbit IRIN = P3 ^ 2;      //红外中断
sbit beep = P2 ^ 2;      //蜂鸣器
uchar IrTime;            //红外接收计时
uchar the_key_id;        //红外转换成无符整数
uchar maohao = 0;        //bool型，是否显示:
char page = 0;           //主屏翻页
uchar mode = 0;          //一级菜单，选中的当前菜单条目 0-3
uchar set_time_mode = 0; //当前设置时间是什么
uchar state = 0;         //判断菜单状态
uchar temp[30];          //用于屏幕显示
uchar year_moon, month_moon, day_moon;

/*---------------------------------公历转换阴历年份数据表--------------------------------*/
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
/*-------------------------------------月份数据表----------------------------------------*/
static unsigned char code day_code1[9] = {0x0, 0x1f, 0x3b, 0x5a, 0x78, 0x97, 0xb5, 0xd4, 0xf3};
static unsigned short code day_code2[3] = {0x111, 0x130, 0x14e};

/*子函数,用于读取数据表中农历月的大月或小月,如果该月为大返回1,为小返回0------------------*/
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

/*--------------------------------公历转农历函数-----------------------------------------*/

/*--------------------------------公历转农历函数-----------------------------------------*/
//输入输出都是10进制参数
void Conversion(uchar year, uchar month, uchar day)
{
    uchar temp1, temp2, temp3, month_p;
    uint temp4, table_addr;
    bit flag2, flag_y;

    //定位数据表地址
    table_addr = year * 0x3;

    //取当年春节所在的公历月份
    temp1 = year_code[table_addr + 2] & 0x60;
    temp1 = _cror_(temp1, 5);
    //取当年春节所在的公历日
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
        temp4 += 1; //如果公历月大于2月并且该年的2月为闰月,天数加1
    //计算公历日离当年元旦的天数完成

    //判断公历日在春节前还是春节后
    if (temp4 >= temp3)
    {
        temp4 -= temp3;
        month = 0x1;
        month_p = 0x1;                           //month_p为月份指向,公历日在春节前或就是春节当日month_p指向首月
        flag2 = GetMoonDay(month_p, table_addr); //检查该农历月为大小还是小月,大月返回1,小月返回0
        flag_y = 0;
        if (flag2 == 0)
            temp1 = 0x1d; //小月29天
        else
            temp1 = 0x1e; //大月30天
        temp2 = year_code[table_addr] & 0xf0;
        temp2 = _cror_(temp2, 4); //从数据表中取该年的闰月月份,如为0则该年无闰月
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
* 函数名         : IrInit()
* 函数功能		 : 初始化红外线接收
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void IrInit()
{
    IT0 = 1;  //下降沿触发
    EX0 = 1;  //打开中断0允许
    EA = 1;   //打开总中断
    IRIN = 1; //初始化端口
}

/*******************************************************************************
* 函数名         : setup()
* 函数功能		 : 启动函数：初始化屏幕，显示欢迎页面，读取闹钟，读取时钟
* 输入           : 无
* 输出         	 : 无
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
* 函数名         : readtime()
* 函数功能		 : 主页面控制12864循环显示时间
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void readtime()
{
    DHT11_temp();
    Ds1302readTime();
    lcd_12864_len_text(1, "  日期        周");
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
    lcd_12864_len_text(3, "    时间");
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
    lcd_12864_len_text(5, "    农历");
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
    lcd_12864_len_text(7, "温度    湿度");
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
* 函数名         : loop()
* 函数功能		 : 主循环函数：控制屏幕点闪烁，刷新时间，检测闹铃
* 输入           : 无
* 输出         	 : 无
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
        //lcd_12864_len_text(1, "到点了！");
    }
    while (state == 4)
    {
        beep = ~beep;
        delaywhile(10);
    }

    delay(500);
}

/*******************************************************************************
* 函数名         : changemenu_wive()
* 函数功能		 : 主菜单选择函数，用于显示选中菜单
* 输入           : 选中菜单条目，整数型 0-3
* 输出         	 : 无
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
        lcd_12864_len_text(1, "  →");
        break;
    case 1:
        lcd_12864_len_text(2, "  →");
        break;
    case 2:
        lcd_12864_len_text(3, "  →");
        break;
    case 3:
        lcd_12864_len_text(4, "  →");
        break;
    default:
        break;
    }
}

/*******************************************************************************
* 函数名         : change_time_alm()
* 函数功能		 : 屏幕显示设置闹钟，并在对应位置闪烁光标
* 输入           : 设置光标位置：秒、分、时，整数型 0-2
* 输出         	 : 无
*******************************************************************************/
void change_time_alm(int set_id)
{
    lcd_12864_len_text(3, "设置闹钟");
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
    //移动对应位置，光标为两个像素，所以为偶数
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
    lcd_12864_write_com(0x0f); //显示光标
}

/*******************************************************************************
* 函数名         : change_alm()
* 函数功能		 : 设置闹钟时间函数
* 输入           : Ture 为当前项加1，Flase 为当前项减1 alarm_time ：秒、分、时
* 输出         	 : 无
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
* 函数名         : change_time_view()
* 函数功能		 : 屏幕显示设置时间，并在对应位置闪烁光标
* 输入           : 设置光标位置：秒、分、时、日、月、周、年 0-6
* 输出         	 : 无
*******************************************************************************/
void change_time_view(int set_id)
{

    lcd_12864_len_text(1, "设置日期    周");
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
    lcd_12864_len_text(3, "设置时间");
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
    lcd_12864_write_com(0x0f); //显示光标
}

/*******************************************************************************
* 函数名         : change_time()
* 函数功能		 : 设置时间函数
* 输入           : Ture 为当前项加1，Flase 为当前项减1 time：秒\分\时\日\月\周\年
* 输出         	 : 无
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
* 函数名         : Irdo()
* 函数功能		 : 红外接收处理函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Irdo()
{
    if (the_key_id == 14) //按钮确定
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
                //卡屏刷新////
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
            lcd_12864_len_text(1, "    设置时间    ");
            lcd_12864_len_text(2, "    设置闹钟    ");
            lcd_12864_len_text(3, "    关于我们    ");
            lcd_12864_len_text(4, "    返回时钟    ");
            changemenu_wive(0);
            state = 1;
        }
    }
    if (the_key_id == 10) //按钮上
    {
        if (state == 0)
        {
            //翻页程序
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
    if (the_key_id == 2) //按钮下
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
    if (the_key_id == 1) //按钮右
    {
        if (state == 2) //设置时间
        {
            set_time_mode++;
            if (set_time_mode > 6)
                set_time_mode = 0;
            change_time_view(set_time_mode);
        }
        if (state == 3) //设置闹钟
        {
            set_time_mode++;
            if (set_time_mode > 2)
                set_time_mode = 0;
            change_time_alm(set_time_mode);
        }
    }
}

/*******************************************************************************
* 函数名         : ReadIr()
* 函数功能		 : 读取红外数值的中断函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void ReadIr() interrupt 0
{
    uchar j, k;
    uint err;
    IrTime = 0;
    delaywhile(700); //7ms
    if (IRIN == 0)   //确认是否真的接收到正确的信号
    {

        err = 1000; //1000*10us=10ms,超过说明接收到错误的信号
        /*当两个条件都为真是循环，如果有一个条件为假的时候跳出循环，免得程序出错的时
		侯，程序死在这里*/
        while ((IRIN == 0) && (err > 0)) //等待前面9ms的低电平过去
        {
            delaywhile(1);
            err--;
        }
        if (IRIN == 1) //如果正确等到9ms低电平
        {
            err = 500;
            while ((IRIN == 1) && (err > 0)) //等待4.5ms的起始高电平过去
            {
                delaywhile(1);
                err--;
            }

            for (j = 0; j < 16; j++) //接收一组数据
            {
                err = 60;
                while ((IRIN == 0) && (err > 0)) //等待信号前面的560us低电平过去
                {
                    delaywhile(1);
                    err--;
                }
                err = 500;
                while ((IRIN == 1) && (err > 0)) //计算高电平的时间长度。
                {
                    delaywhile(10); //0.1ms
                    IrTime++;
                    err--;
                }
                IrTime = 0; //用完时间要重新赋值
            }

            for (j = 0; j < 8; j++) //接收一组数据
            {

                err = 60;
                while ((IRIN == 0) && (err > 0)) //等待信号前面的560us低电平过去
                {
                    delaywhile(1);
                    err--;
                }
                err = 500;
                while ((IRIN == 1) && (err > 0)) //计算高电平的时间长度。
                {
                    delaywhile(10); //0.1ms
                    IrTime++;
                    err--;
                    if (IrTime > 30)
                    {
                        return;
                    }
                }
                the_key_id >>= 1; //k表示第几组数据
                if (IrTime >= 8)  //如果高电平出现大于565us，那么是1
                {
                    the_key_id |= 0x80; //1000 0000
                }
                IrTime = 0; //用完时间要重新赋值
            }
        }
        the_key_id = the_key_id % 16;
        Irdo();
    }
}
//共计1,190行代码