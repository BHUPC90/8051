#include "qi51arduino.h"
#include "lcd.h"
#include "DS18B20.h"

void setup()
{
    LCD_RW = 0;        //设置lcd write模式
    LCD_EN = 0;        //初始lcd化使能低电平
    lcd_commode(0x38); //初始化
    lcd_commode(0x0e); //1110 显示光标
    DS18B20_inttemp();
}


/* 整型数转换为字符串， str-字符串指针， dat-待转换数，返回值-字符串长度 */
unsigned char IntToString(unsigned char *str, int dat)
{
	signed char i = 0;
	unsigned char len = 0;
	unsigned char buf[6];
	if (dat < 0) //如果为负数，首先取绝对值，并在指针上添加负号
	{
		dat = -dat;
		*str++ = '-';
		len++;
	}
	do { //先转换为低位在前的十进制数组
			buf[i++] = dat % 10;
			dat /= 10;
	} while (dat > 0);
	len += i; //i 最后的值就是有效字符的个数
	while (i-- > 0) //将数组值转换为 ASCII 码反向拷贝到接收指针上
	{
		*str++ = buf[i] + '0';
		}
	*str = '\0'; //添加字符串结束符
	return len; //返回字符串长度
	}

void loop()
{
    bit res;
    int temp; //读取到的当前温度值
	int intT, decT; //温度值的整数和小数部分
	uchar len;
	uchar str[12];
    res = DS18B20_temp(&temp); //读取当前温度
    DS18B20_inttemp(); 
    if (res)                   //读取成功时，刷新当前温度显示
    {
        intT = temp >> 4;             //分离出温度值整数部分
        decT = temp & 0xF;            //分离出温度值小数部分
        len = IntToString(str, intT); //整数部分转换为字符串
        str[len++] = '.';             //添加小数点
        decT = (decT * 10) / 16;      //二进制的小数部分转换为 1 位十进制位
        str[len++] = decT + '0';      //十进制小数位再转换为 ASCII 字符
        while (len < 6)               //用空格补齐到 6 个字符长度
        {
            str[len++] = ' ';
        }
        str[len] = '\0';        //添加字符串结束符
        lcd_str( 1, str);  //显示到液晶屏上
    }
    else //读取失败时，提示错误信息
    {
        lcd_str( 0, "error!");
    }
    
}