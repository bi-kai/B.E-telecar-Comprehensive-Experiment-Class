/*******************************************************************/

// 利用定时器控制产生占空比可变的 PWM 波
// 按K1，PWM值增加，则占空比减小,LED 灯渐暗。
// 按K2，PWM值减小，则占空比增加,LED 灯渐亮。
// 当PWM值增加到最大值或减小到最小值时，蜂鸣器将报警。
// 为了看到蜂鸣器的效果，可以使用杜邦线把蜂鸣器引到其它接口上
//因为蜂鸣器和小灯L4共用一P1.3口，所以按键按下时L4会比其它亮
//SW17是亮度增加键 SW16是亮度减小键

/*********************************************************/

#include < reg51.h >
#include < intrins.h >

sbit  K1 =P3^2 ;                    //增加键
sbit  K2 =P3^3 ;                    //减少键
sbit  BEEP =P1^3 ;                //蜂鸣器
unsigned char PWM=0x7f ;   //赋初值

void Beep();
void delayms(unsigned char ms);
void delay(unsigned char t);

/*********************************************************/
void main()
{  

     P1=0xff; 
    TMOD=0x21 ;
    TH0=0xfc ;             //1ms延时常数 12M
    TL0=0x18 ;            //频率调节

    TH1=PWM ;            //脉宽调节
    TL1=0 ;

     EA=1;
     ET0=1;
     ET1=1;
    
     TR0=1 ;

   while(1)
   {
     do{
            if(PWM!=0xff)
           {PWM++ ;delayms(10);}
           else Beep() ; 
         }
     while(K1==0);

     do{
           if(PWM!=0x02)
          {PWM-- ;delayms(10);}
           else Beep() ; 
          }
     while(K2==0);
   }
}

/*********************************************************/
// 定时器0中断服务程序.
/*********************************************************/
void timer0() interrupt 1 
{  
    TR1=0 ;
    TH0=0xfc ;
    TL0=0x66 ;
    TH1=PWM ;
    TR1=1 ;
    P1=0x00 ;      //启动输出
}

/*********************************************************/
// 定时器1中断服务程序
/*********************************************************/
void timer1() interrupt 3 
{ 
    TR1=0 ;
    P1=0xff ;     //结束输出
}

/*********************************************************/
//蜂鸣器子程序
/*********************************************************/

void Beep()     
  {
      unsigned char i  ;
      for (i=0  ;i<100  ;i++)
        {
          delay(100)  ;
          BEEP=!BEEP  ;                //Beep取反
        } 
     BEEP=1  ;                            //关闭蜂鸣器
     delayms(100);
  } 

/*********************************************************/
// 延时子程序
/*********************************************************/  
void delay(unsigned char t)
 { 
    while(t--)   ;
 }

/*********************************************************/
// 延时子程序
/*********************************************************/
void delayms(unsigned char ms) 

{
    unsigned char i ;
    while(ms--)
     {
        for(i = 0 ; i < 120 ; i++) ;
     }
}

/*********************************************************/
