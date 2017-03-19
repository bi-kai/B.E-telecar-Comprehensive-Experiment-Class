/*******************************************************************/

// 利用定时器控制产生占空比可变的 PWM 波
// 按K1，PWM值增加，则占空比减小,LED 灯渐暗。
// 按K2，PWM值减小，则占空比增加,LED 灯渐亮。
// 当PWM值增加到最大值或减小到最小值时，蜂鸣器将报警。
// 为了看到蜂鸣器的效果，可以使用杜邦线把蜂鸣器引到其它接口上
//因为蜂鸣器和小灯L4共用一P1.3口，所以按键按下时L4会比其它亮
//SW17是亮度增加键 SW16是亮度减小键

/*********************************************************/

#include < reg52.h >
#include < intrins.h >

sbit  K1=P3^0;                    //增加键
sbit  K2=P3^1;                    //减少键
sbit  LED1=P2^6;                
sbit  LED2=P2^7;

sbit  K3=P2^2;
sbit  K4=P2^3;
sbit  LED3=P2^4; 
sbit  LED4=P2^5; 

sbit out_1=P1^2;
sbit out_2=P3^4;

unsigned int PWM_1=0xFA99;   //舵机1赋初值
unsigned int PWM_2=0xFA99;   //舵机2赋初值

bit longorshorttimerflag=0;
bit longorshorttimerflag_2=0;
unsigned int timerbuf[2]={0};

void delayms(unsigned char ms);

/********以下为直流电机控制按键的定义*****************/
sbit motor11=P1^0;//电机1的信号引脚
sbit motor12=P1^1;

sbit motor21=P1^2;//电机2的信号引脚
sbit motor33=P1^3;

sbit motorforward=P1^4;
sbit motorback=P1^5;
sbit motorleftcircle=P1^6;
sbit motorrightcircle=P1^7;
sbit motorleft=P2^0;
sbit motorright=P2^1;





/*********************************************************/
void main()
{  

/****************************************/

     out_1=0;
 	  out_2=0;
    TMOD=0x11;
    TH0=0xB8;             //1ms延时常数 12M
    TL0=0x00;            //频率调节

    TH1=0xB8;            //脉宽调节//就差这一点了
    TL1=0x00;

     EA=1;
     ET0=1;
     ET1=1;    
     TR0=1;
	  TR1=1;
   while(1)
   {
     if(K1==0){
            if(PWM_1<0xFBF7){PWM_1++;delayms(2);}
           else {LED1=0;} 
         };

     if(K2==0){
           if(PWM_1>0xF93B){PWM_1--;delayms(2);}
           else {LED2=0;} 
          };

			  if(K3==0){
            if(PWM_2<0xFF91){PWM_2++;delayms(2);}
				else {LED3=0;}
         };

			 if(K4==0){
            if(PWM_2>0xF91B){PWM_2--;delayms(2);}//原来是0xF93B
           else {LED4=0;} 
         };
		

	  LED1=1;
	  LED2=1;
	  LED3=1;
	  LED4=1;
   }
}

/*********************************************************/
// 定时器0中断服务程序.
/*********************************************************/
void timer0() interrupt 1 
{  
	if(longorshorttimerflag==0){
    		TR0=0;
	 		TH0=(PWM_1/256);            //脉宽调节
    		TL0=(PWM_1%256);
   		 out_1=1;      //启动输出
			 longorshorttimerflag=!longorshorttimerflag;
			 TR0=1;
	 }
	else {
		
		TR0=0;
		timerbuf[0]=-0xB800+PWM_1;
		TH0=timerbuf[0]/256;
		TL0=timerbuf[0]%256;
		out_1=0;      //关闭输出
		longorshorttimerflag=!longorshorttimerflag;
		TR0=1;
	}
}

/*********************************************************/
// 定时器1中断服务程序
/*********************************************************/
void timer1() interrupt 3 
{ 
  	if(longorshorttimerflag_2==0){
    		TR1=0;
	 		TH1=(PWM_2/256);            //脉宽调节
    		TL1=(PWM_2%256);
   		 out_2=1;      //启动输出
			 longorshorttimerflag_2=!longorshorttimerflag_2;
			 TR1=1;
	 }
	else {
		
		TR1=0;
		timerbuf[1]=-0xB800+PWM_2;
		TH0=timerbuf[1]/256;
		TL0=timerbuf[1]%256;
		out_2=0;      //关闭输出
		longorshorttimerflag_2=!longorshorttimerflag_2;
		TR1=1;
	}
}
/*********************************************************/
// 延时子程序
/*******************************************************/
void delayms(unsigned char ms) 

{
    unsigned char i;
    while(ms--)
     {
        for(i=0;i<120;i++);
     }
}


