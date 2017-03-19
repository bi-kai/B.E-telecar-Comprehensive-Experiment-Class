/*******************************************************************/

// ���ö�ʱ�����Ʋ���ռ�ձȿɱ�� PWM ��
// ��K1��PWMֵ���ӣ���ռ�ձȼ�С,LED �ƽ�����
// ��K2��PWMֵ��С����ռ�ձ�����,LED �ƽ�����
// ��PWMֵ���ӵ����ֵ���С����Сֵʱ����������������
// Ϊ�˿�����������Ч��������ʹ�öŰ��߰ѷ��������������ӿ���
//��Ϊ��������С��L4����һP1.3�ڣ����԰�������ʱL4���������
//SW17���������Ӽ� SW16�����ȼ�С��

/*********************************************************/

#include < reg52.h >
#include < intrins.h >

sbit  K1=P3^0;                    //���Ӽ�
sbit  K2=P3^1;                    //���ټ�
sbit  LED1=P2^6;                
sbit  LED2=P2^7;

sbit  K3=P2^2;
sbit  K4=P2^3;
sbit  LED3=P2^4; 
sbit  LED4=P2^5; 

sbit out_1=P1^2;
sbit out_2=P3^4;

unsigned int PWM_1=0xFA99;   //���1����ֵ
unsigned int PWM_2=0xFA99;   //���2����ֵ

bit longorshorttimerflag=0;
bit longorshorttimerflag_2=0;
unsigned int timerbuf[2]={0};

void delayms(unsigned char ms);
void refree_allpins();
/********����Ϊֱ��������ư����Ķ���*****************/
sbit motor11=P0^0;//���1���ź�����
sbit motor12=P0^1;

sbit motor21=P0^2;//���2���ź�����
sbit motor22=P0^3;

sbit motorforward=P0^4;
sbit motorback=P0^5;
sbit motorleftcircle=P0^6;
sbit motorrightcircle=P0^7;
sbit motorleft=P2^0;
sbit motorright=P2^1;





/*********************************************************/
void main()
{  

/****************************************/

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
   while(1)
   {

motorforward=1;
motorback=1;
motorleftcircle=1;
motorrightcircle=1;
motorleft=1;
motorright=1;


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
            if(PWM_2>0xF91B){PWM_2--;delayms(2);}//ԭ����0xF93B
           else {LED4=0;} 
         };
		
			while(motorforward==0){
				motor11=1;
				motor12=0;
				motor21=1;
				motor22=0;
			
			}
			
			while(motorback==0){
				motor11=0;
				motor12=1;
				motor21=0;
				motor22=1;
			}

			while(motorleftcircle==0){
				motor11=0;
				motor12=1;
				motor21=1;
				motor22=0;
			}

			while(motorrightcircle==0){
				motor11=1;
				motor12=0;
				motor21=0;
				motor22=1;
			}

			while(motorleft==0){
				motor11=0;
				motor12=0;
				motor21=1;
				motor22=0;
			}

			while(motorright==0){
				motor11=1;
				motor12=0;
				motor21=0;
				motor22=0;
			}
	
			refree_allpins();


	  LED1=1;
	  LED2=1;
	  LED3=1;
	  LED4=1;
   }
}

/*********************************************************/
// ��ʱ��0�жϷ������.
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
// ��ʱ�ӳ���
/*******************************************************/
void delayms(unsigned char ms) 

{
    unsigned char i;
    while(ms--)
     {
        for(i=0;i<120;i++);
     }
}


void refree_allpins(){

         	motor11=0;
				motor12=0;
				motor21=0;
				motor22=0;

}
