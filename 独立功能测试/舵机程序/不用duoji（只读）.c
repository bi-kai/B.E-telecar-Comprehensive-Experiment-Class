#include<reg52.h>
#define uchar  unsigned char
#define uint unsigned int


sbit key1=P3^0;
sbit key2=P3^1;
sbit key3=P1^2;
sbit key4=P1^3;
sbit key5=P1^4;
sbit key6=P1^5;

//PWM的输出端口
sbit PWM_OUT0=P1^2;
sbit PWM_OUT1=P0^1;
sbit PWM_OUT2=P0^2;

//PWM的数据值
uint PWM_Value[8]={1500,1000,1500,1000,1750,2000,2500,2000};

uchar order1;  //定时器扫描序列
/*=========================定时器T0的中断服务程序一个循环20MS=8*2.5ms=============*/
void timer0(void) interrupt 1 using 1
{
switch(order1)
{
case 1:   PWM_OUT0=1;
          TH0=-PWM_Value[0]/256;
          TL0=-PWM_Value[0]%256;
          break;
case 2:  PWM_OUT0=0;       
          TH0=-(2700-PWM_Value[0])/256;
          TL0=-(2700-PWM_Value[0])%256;
          break;
case 3:  PWM_OUT1=1;
          TH0=-PWM_Value[1]/256;
          TL0=-PWM_Value[1]%256;
          break;
case 4:  PWM_OUT1=0;
          TH0=-(2700-PWM_Value[1])/256;
          TL0=-(2700-PWM_Value[1])%256;
          break;
case 5:  PWM_OUT2=1;
          TH0=-PWM_Value[2]/256;
          TL0=-PWM_Value[2]%256;
          break;
case 6:  PWM_OUT2=0 ;
          TH0=-(2700-PWM_Value[2])/256;
          TL0=-(2700-PWM_Value[2])%256;
          break;
          default : order1=0;
    }
    order1++;
}

/*============================初始化中断=========================*/
void InitPWM(void)
{
   order1=1;

   TMOD |=0x11;
   TH0=-1500/256;
   TL0=-1500%256;
   EA=1;
   EX0=0;
   ET0=1; 
	TR0=1;
	PT0=1;
	PX0=0;
}


void delay(void)
{
  uint i=100;

  while(i--);

}


void main(void)
{

InitPWM();
    while(1)
    {
  
  if(key1==0)
  {
    if(PWM_Value[0]<2500)
       PWM_Value[0]++;
   }

     if(key2==0)
  {
    if(PWM_Value[0]>500)
       PWM_Value[0]--;
  }

if(key3==0)
  {
    if(PWM_Value[1]<2500)
       PWM_Value[1]++;
   }

     if(key4==0)
  {
    if(PWM_Value[2]>500)
       PWM_Value[2]--;
  }

if(key5==0)
  {
    if(PWM_Value[2]<2500)
       PWM_Value[2]++;
   }

     if(key6==0)
  {
    if(PWM_Value[3]>500)
       PWM_Value[3]--;
  }


     delay();
    }
}