#ifndef _INC_SR04
#define _INC_SR04
#define uchar unsigned char
#define uint unsigned int
#endif
//测试距离 = ( 高电平时间 * 声速(340M/S) ) / 2;

sbit ECHO_port = P2 ^ 4;
sbit TRIG_port = P2 ^ 3;

void delay(unsigned int wait_time);
void delaywhile(unsigned int wait_time); //1ms ??

void StartSR04() //启动模块
{
    TRIG_port = 0;
    delaywhile(1);
    TRIG_port = 1; //启动一次模块
    delaywhile(2);
    TRIG_port = 0;
}

void SR04_Init()
{
	TMOD|=0x01;
	TH0=0;
	TL0=0;
}



float CSB_GetOnce()//得到一次超声波测距模块的距离
{
	CSB_Rstart();
	while(!ECHO_port);//当Echo为零时等待
	starttime();//开启计数
	while(ECHO_port);//当Echo为1计数并等待
	TR0=0;//关闭计数
	return CSB_GetDistance();//计算
}


