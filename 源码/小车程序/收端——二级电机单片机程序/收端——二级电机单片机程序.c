#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint  unsigned int

uchar data Textbuf[2];

sbit LED3=P2^4;
sbit LED4=P2^5;
/*######################����Ķ�����򲿷�һ########################*/
sbit out_1=P3^1;
sbit out_2=P3^0;
unsigned int PWM_1=0xFA99;   //���1����ֵ
unsigned int PWM_2=0xFA99;   //���2����ֵ
bit longorshorttimerflag=0;
bit longorshorttimerflag_2=0;
unsigned int timerbuf[2]={0};

void delayms(unsigned char ms);
void refree_allpins();

sbit duoji_1_1=P1^0;
sbit duoji_1_2=P1^1;
sbit duoji_2_1=P1^2;
sbit duoji_2_2=P1^3;

sbit allowchack_flag=P2^2;
/*######################������ֳ���һ�������####################*/

/*********************ֱ���������ĵ�һ����*********************************/
sbit motor_1_1=P1^4;//���1���ź�����
sbit motor_1_2=P1^5;
sbit motor_2_1=P1^6;//���2���ź�����
sbit motor_2_2=P1^7;

sbit	motor11=P3^2;
sbit	motor12=P3^3;
sbit	motor21=P3^4;
sbit	motor22=P3^5;
				
/**********************ֱ���������ĵ�һ���ֽ���********************************/

void main()
{
/*#####################����Ķ�����򲿷ֶ�######################*/
    out_1=0;
    out_2=0;
    TMOD=0x11;
    TH0=0xB8;             //1ms��ʱ���� 12M
    TL0=0x00;            //Ƶ�ʵ���

    TH1=0xB8;            //�������//�Ͳ���һ����
    TL1=0x00;

     EA=1;
     ET0=1;
     ET1=1;    
     TR0=1;
	  TR1=1;


/*#####################������ֳ�����������#######################*/

while(1)
{

while(allowchack_flag==0){
/*##################����Ķ�����򲿷���################################*/
if((duoji_1_1==0)||(duoji_1_2==0)||(duoji_2_1==0)||(duoji_2_2==0)){

				if(duoji_2_1==0){
           if(PWM_2<0xFD94){//ԭ��FBF7
			    PWM_2=PWM_2+30; 
			  LED3=1;
			  delayms(2);
			  }
           else {
			  LED3=0;
			  } 
          }


		 if(duoji_2_2==0){
           if(PWM_2>0xF93B){
			    PWM_2=PWM_2-30; 
			  LED4=1;
			  delayms(2);
			  }
           else {
			  LED4=0;
			  } 
          }

		if(duoji_1_1==0){
            if(PWM_1<0xFBF7){
			   	PWM_1=PWM_1+30; 
				LED3=1;
				delayms(2);
				}
           else {
			  LED3=0;
			  } 
         }//duoji_1_UP//duoji_1_DOWN


     if(duoji_1_2==0){
           if(PWM_1>0xF93B){
			    PWM_1=PWM_1-30; 
			  LED4=1;
			  delayms(2);
			  }
           else {
			  LED4=0;
			  } 
          }




}
if(allowchack_flag==1)break;

/*################������ֳ������������################################*/
/*******************************ֱ���������ĵڶ�����*************************/
if((motor_1_1==1)||(motor_1_2==1)||(motor_2_1==1)||(motor_2_2==1)){

//if(allowchack_flag==1)break;

			if((motor_1_1==1)&&(motor_1_2==0)&&(motor_2_1==1)&&(motor_2_2==0)){
				motor11=1;
				motor12=0;
				motor21=1;
				motor22=0;
			
			}
if(allowchack_flag==1)break;	
		
			if((motor_1_1==0)&&(motor_1_2==1)&&(motor_2_1==0)&&(motor_2_2==1)){
				motor11=0;
				motor12=1;
				motor21=0;
				motor22=1;
			}
if(allowchack_flag==1)break;

			if((motor_1_1==0)&&(motor_1_2==1)&&(motor_2_1==1)&&(motor_2_2==0)){
				motor11=0;
				motor12=1;
				motor21=1;
				motor22=0;
			}
if(allowchack_flag==1)break;

			if((motor_1_1==1)&&(motor_1_2==0)&&(motor_2_1==0)&&(motor_2_2==1)){
				motor11=1;
				motor12=0;
				motor21=0;
				motor22=1;
			}
if(allowchack_flag==1)break;

			if((motor_1_1==0)&&(motor_1_2==0)&&(motor_2_1==1)&&(motor_2_2==0)){
				motor11=0;
				motor12=0;
				motor21=1;
				motor22=0;
			}
if(allowchack_flag==1)break;

			if((motor_1_1==1)&&(motor_1_2==0)&&(motor_2_1==0)&&(motor_2_2==0)){
				motor11=1;
				motor12=0;
				motor21=0;
				motor22=0;
			}
	
}//end of if(motor)

  
}//while(allowchack_flag==0)
/*******************************ֱ���������ĵڶ��������********************/
while(allowchack_flag==1){	
refree_allpins();	
}//while(allowchack_flag==1)

}//while(1)
}

/***************************************/
void delayms(unsigned char ms) 

{
    unsigned char i;
    while(ms--)
     {
        for(i=0;i<120;i++);
     }
}

/*********************************************************/
// ��ʱ��0�жϷ������.(���ڶ��1)
/*********************************************************/
void timer0() interrupt 1 
{  
	if(longorshorttimerflag==0){
    		TR0=0;
	 		TH0=(PWM_1/256);            //�������
    		TL0=(PWM_1%256);
   		 out_1=1;      //�������
			 longorshorttimerflag=!longorshorttimerflag;
			 TR0=1;
	 }
	else {
		
		TR0=0;
		timerbuf[0]=-0xB800+PWM_1;
		TH0=timerbuf[0]/256;
		TL0=timerbuf[0]%256;
		out_1=0;      //�ر����
		longorshorttimerflag=!longorshorttimerflag;
		TR0=1;
	}
}

/*********************************************************/
// ��ʱ��1�жϷ������
/*********************************************************/
void timer1() interrupt 3 
{ 
  	if(longorshorttimerflag_2==0){
    		TR1=0;
	 		TH1=(PWM_2/256);            //�������
    		TL1=(PWM_2%256);
   		 out_2=1;      //�������
			 longorshorttimerflag_2=!longorshorttimerflag_2;
			 TR1=1;
	 }
	else {
		
		TR1=0;
		timerbuf[1]=-0xB800+PWM_2;
		TH0=timerbuf[1]/256;
		TL0=timerbuf[1]%256;
		out_2=0;      //�ر����
		longorshorttimerflag_2=!longorshorttimerflag_2;
		TR1=1;
	}
}
/*********************************************************/
void refree_allpins(){

         	    motor11=0;
		  		motor12=0;
				motor21=0;
				motor22=0;

				motor_1_1=1;
				motor_1_2=1;
				motor_2_1=1;
				motor_2_2=1;

				duoji_1_1=1;
				duoji_1_2=1;
				duoji_2_1=1;
				duoji_2_2=1;

}
