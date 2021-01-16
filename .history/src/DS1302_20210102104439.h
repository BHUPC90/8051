#ifndef DS1302_H
#define DS1302_H
#define uchar unsigned char
#define uint unsigned int
#endif

sbit DSIO = P2^4;
sbit RST = P3^5;
sbit SCLK = P2^3;