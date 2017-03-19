#include<reg51.h>
//#include<math.h>
#define uchar unsigned char
#define uint unsigned int
uint a,c;
sbit p10=P1^2;

void timer0(void) interrupt 1 
{p10=!p10;

c=20000-c; 
TH0=-(c/256); TL0=-(c%256); 
if(c>=500&&c<=2500)c=a;
else c=20000-a; 
}

void delay(long j)      
{
for(j;j>0;j--);
}

void main(void)
{
TMOD=0x01; //16位定时器 工作方式1
p10=1;
//p11=1;
a=2500;//180
//c=a;
TH0=-(a/256); TL0=-(a%256);
EA=1;
ET0=1; 
TR0=1;
for(a=2500;a>=500;a--)
{
a=a-50;
c=a;   
delay(10000);
}
}
