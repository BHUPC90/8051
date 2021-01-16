#include "qi51arduino.h"
#include "DS1302.h"
#include "lcd12864.h"
#include "huatu.h"
#include "DHT11.h"

sbit IRIN = P3 ^ 2;
uint IrTime;
uchar IrValue[6];
uint the_key_id;

int maohao = 0;
int page = 0;
int mode = 0;
int set_time_mode = 0;

uint state = 0;

uchar temp[30];

void IrInit()
{
    IT0 = 1;  //下降沿触发
    EX0 = 1;  //打开中断0允许
    EA = 1;   //打开总中断
    IRIN = 1; //初始化端口
}

void menu()
{
    int_lcd_12864();
    lcd_12864_write_com(0x00);
    lcd_12864_len_text(1, "    设置时间    ");
    lcd_12864_len_text(2, "    设置闹钟    ");
    lcd_12864_len_text(3, "    关于我们    ");
    lcd_12864_len_text(4, "    返回时钟    ");
}

void readtime()
{
    DHT11_temp();
    Ds1302readTime();
    lcd_12864_len_text(1, "    日期");
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
    temp[12] = '\0';
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
    lcd_12864_len_text(5, "    闹钟");
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

void setup()
{
    IrInit();
    int_lcd_12864();
    lcd_12864_write_com(0x00);
    //////////////////////////////
    lcd_12864_picture(firstpage);
    //////////////////////////////
    read_alarm();
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
    if (time[0] == alarm_time[0] && time[1] == alarm_time[1] && time[2] == alarm_time[3])
    {
        state = 4;
        lcd_12864_write_com(0x00);
        lcd_12864_len_text(1, "到点了！");
    }
    while (state == 4)
    {
        beep = ~beep;
        delaywhile(10);
    }

    delay(500);
}

void changemenu_wive(int menuid)
{
    switch (menuid)
    {
    case 0:
        lcd_12864_len_text(1, "  →");
        lcd_12864_len_text(2, "    ");
        lcd_12864_len_text(3, "    ");
        lcd_12864_len_text(4, "    ");
        break;
    case 1:
        lcd_12864_len_text(2, "  →");
        lcd_12864_len_text(1, "    ");
        lcd_12864_len_text(3, "    ");
        lcd_12864_len_text(4, "    ");
        break;
    case 2:
        lcd_12864_len_text(3, "  →");
        lcd_12864_len_text(2, "    ");
        lcd_12864_len_text(1, "    ");
        lcd_12864_len_text(4, "    ");
        break;
    case 3:
        lcd_12864_len_text(4, "  →");
        lcd_12864_len_text(2, "    ");
        lcd_12864_len_text(3, "    ");
        lcd_12864_len_text(1, "    ");
        break;
    default:
        break;
    }
}

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

void change_time_view(int set_id)
{

    lcd_12864_len_text(1, "设置日期");
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
    temp[10] = '\0';
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
    case 5:
        lcd_12864_write_com(0x90);
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

void change_time(int mmod)
{

    if (set_time_mode == 5)
    {
        set_time_mode = 6;
    }

    if (mmod)
    {
        time[set_time_mode] = time[set_time_mode] + 0x01;
    }
    else
    {
        time[set_time_mode] = time[set_time_mode] - 0x01;
    }

    if (set_time_mode == 6)
    {
        set_time_mode = 5;
    }
    change_time_view();
}

void Irdo()
{
    if (the_key_id == 14)
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
            menu();
            changemenu_wive(0);
            state = 1;
        }
    }

    if (the_key_id == 10)
    {
        if (state == 0)
        {
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
    if (the_key_id == 2)
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

    if (the_key_id == 1)
    {
        if (state == 2)
        {
            set_time_mode++;
            if (set_time_mode > 5)
                set_time_mode = 0;
            change_time_view(set_time_mode);
        }
        if (state == 3)
        {
            set_time_mode++;
            if (set_time_mode > 2)
                set_time_mode = 0;
            change_time_alm(set_time_mode);
        }
    }
}

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
            for (k = 0; k < 4; k++) //共有4组数据
            {
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
                    IrValue[k] >>= 1; //k表示第几组数据
                    if (IrTime >= 8)  //如果高电平出现大于565us，那么是1
                    {
                        IrValue[k] |= 0x80;
                    }
                    IrTime = 0; //用完时间要重新赋值
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