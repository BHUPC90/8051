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


uint MonthAdd[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
void chek_table(uint num)
{
    switch (num - 2000)
    {
    case 0:
        llsu = 0x0C9645;
        break; //2000
    case 1:
        llsu = 0x4D4AB8;
        break; //2001
    case 2:
        llsu = 0x0D4A4C;
        break; //2002
    case 3:
        llsu = 0x0DA541;
        break; //2003
    case 4:
        llsu = 0x25AAB6;
        break; //2004
    case 5:
        llsu = 0x056A49;
        break; //2005
    case 6:
        llsu = 0x7AADBD;
        break; //2006
    case 7:
        llsu = 0x025D52;
        break; //2007
    case 8:
        llsu = 0x092D47;
        break; //2008
    case 9:
        llsu = 0x5C95BA;
        break; //2009
    case 10:
        llsu = 0x0A954E;
        break; //2010
    case 11:
        llsu = 0x0B4A43;
        break; //2011
    case 12:
        llsu = 0x4B5537;
        break; //2012
    case 13:
        llsu = 0x0AD54A;
        break; //2013
    case 14:
        llsu = 0x955ABF;
        break; //2014
    case 15:
        llsu = 0x04BA53;
        break; //2015
    case 16:
        llsu = 0x0A5B48;
        break; //2016
    case 17:
        llsu = 0x652BBC;
        break; //2017
    case 18:
        llsu = 0x052B50;
        break; //2018
    case 19:
        llsu = 0x0A9345;
        break; //2019
    case 20:
        llsu = 0x474AB9;
        break; //2020
    case 21:
        llsu = 0x06AA4C;
        break; //2021
    case 22:
        llsu = 0x0AD541;
        break; //2022
    case 23:
        llsu = 0x24DAB6;
        break; //2023
    case 24:
        llsu = 0x04B64A;
        break; //2024
    case 25:
        llsu = 0x69573D;
        break; //2025
    case 26:
        llsu = 0x0A4E51;
        break; //2026
    case 27:
        llsu = 0x0D2646;
        break; //2027
    case 28:
        llsu = 0x5E933A;
        break; //2028
    case 29:
        llsu = 0x0D534D;
        break; //2029
    case 30:
        llsu = 0x05AA43;
        break; //2030
    case 31:
        llsu = 0x36B537;
        break; //2031
    case 32:
        llsu = 0x096D4B;
        break; //2032
    case 33:
        llsu = 0xB4AEBF;
        break; //2033
    case 34:
        llsu = 0x04AD53;
        break; //2034
    case 35:
        llsu = 0x0A4D48;
        break; //2035
    case 36:
        llsu = 0x6D25BC;
        break; //2036
    case 37:
        llsu = 0x0D254F;
        break; //2037
    case 38:
        llsu = 0x0D5244;
        break; //2038
    case 39:
        llsu = 0x5DAA38;
        break; //2039
    case 40:
        llsu = 0x0B5A4C;
        break; //2040
    case 41:
        llsu = 0x056D41;
        break; //2041
    case 42:
        llsu = 0x24ADB6;
        break; //2042
    case 43:
        llsu = 0x049B4A;
        break; //2043
    case 44:
        llsu = 0x7A4BBE;
        break; //2044
    case 45:
        llsu = 0x0A4B51;
        break; //2045
    case 46:
        llsu = 0x0AA546;
        break; //2046
    case 47:
        llsu = 0x5B52BA;
        break; //2047
    case 48:
        llsu = 0x06D24E;
        break; //2048
    case 49:
        llsu = 0x0ADA42;
        break; //2049
    }
} /**/

/*--------------------------------公历转农历函数-----------------------------------------*/
//输入输出都是10进制参数
void Conversion(uchar year, uchar month, uchar day)
{
    uchar i;
    uint Spring_NY, Sun_NY, StaticDayCount; //Spring_NY 记录春节离当年元旦的天数
    uint index, flag;                       //Sun_NY 记录阳历日离当年元旦的天数。
    chek_table(nian);
    if (((llsu & 0x0060) >> 5) == 1)
        Spring_NY = (llsu & 0x001F) - 1;
    else
        Spring_NY = (llsu & 0x001F) - 1 + 31;
    Sun_NY = MonthAdd[yue - 1] + ri - 1;
    if ((!(nian % 4)) && (yue > 2))
        Sun_NY++;
    //StaticDayCount记录大小月的天数 29 或30
    //index 记录从哪个月开始来计算。
    //flag 是用来对闰月的特殊处理。
    //判断阳历日在春节前还是春节后
    if (Sun_NY >= Spring_NY) //阳历日在春节后（含春节那天）
    {
        Sun_NY -= Spring_NY;
        yue = 1;
        index = 1;
        flag = 0;
        if ((llsu & (0x80000 >> (index - 1))) == 0)
            StaticDayCount = 29;
        else
            StaticDayCount = 30;
        while (Sun_NY >= StaticDayCount)
        {
            Sun_NY -= StaticDayCount;
            index++;
            if (yue == ((llsu & 0xF00000) >> 20))
            {
                flag = ~flag;
                if (flag == 0)
                    yue++;
            }
            else
                yue++;
            if ((llsu & (0x80000 >> (index - 1))) == 0)
                StaticDayCount = 29;
            else
                StaticDayCount = 30;
        }
        ri = Sun_NY + 1;
    }
    else //阳历日在春节前
    {
        Spring_NY -= Sun_NY;
        nian--;
        yue = 12;
        if (((llsu & 0xF00000) >> 20) == 0)
            index = 12;
        else
            index = 13;
        flag = 0;
        if ((llsu & (0x80000 >> (index - 1))) == 0)
            StaticDayCount = 29;
        else
            StaticDayCount = 30;
        while (Spring_NY > StaticDayCount)
        {
            Spring_NY -= StaticDayCount;
            index--;
            if (flag == 0)
                yue--;
            if (yue == ((llsu & 0xF00000) >> 20))
                flag = ~flag;
            if ((llsu & (0x80000 >> (index - 1))) == 0)
                StaticDayCount = 29;
            else
                StaticDayCount = 30;
        }
        ri = StaticDayCount - Spring_NY + 1;
    }

    write_cmd(0x98);
    for (i = 0; i < 2; i++)
    {
        write_data(read(0xa2, 0x00 + ((nian - 2000) % 10) * 2 + i));
    }
    for (i = 0; i < 2; i++)
    {
        write_data(read(0xa2, 0x00 + 20 + ((nian - 2000) % 12) * 2 + i));
    }
    for (i = 0; i < 2; i++)
    {
        write_data(read(0xa0, 0x80 + i));
    }
    if (yue == 1)
    {
        for (i = 0; i < 2; i++)
        {
            write_data(read(0xa0, 0x8e + i));
        }
    }
    if (yue >= 10)
    {
        for (i = 0; i < 2; i++)
        {
            write_data(read(0xa2, 0x70 + 20 + i));
        }
    }
    if (yue != 10 && yue != 1)
        for (i = 0; i < 2; i++)
        {
            write_data(read单片机公历转农历程序(0xa2, 0x70 + (yue % 10) * 2 + i));
        }
    for (i = 0; i < 2; i++)
    {
        write_data(read(0xa0, 0x84 + i));
    }
    if (ri / 10 == 3)
        for (i = 0; i < 2; i++)
            write_data(read(0xa2, 0x70 + 6 + i));
    if (ri / 10 == 2)
        for (i = 0; i < 2; i++)
            write_data(read(0xa2, 0x70 + 22 + i));
    if (ri / 10 == 1)
        for (i = 0; i < 2; i++)
            write_data(read(0xa2, 0x70 + 20 + i));
    if (ri / 10 == 0)
        for (i = 0; i < 2; i++)
            write_data(read(0xa2, 0x70 + i));
    for (i = 0; i < 2; i++)
    {
        write_data(read(0xa2, 0x70 + (ri % 10) * 2 + i));
    }
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