#include < reg52.h >
#include < intrins.h >

#define uchar unsigned char
#define uint  unsigned int

/*******************************����ȫ�ֱ���********************************/
unsigned char dat=0x00 ;      //ADֵ

unsigned char channel;       //ͨ������
//ADC0832������
sbit AD1_CS =P3^6;  //ADC0832 chip seclect
sbit AD1_DI =P3^4;  //ADC0832 k in
sbit AD1_DO =P3^4;  //ADC0832 k out
sbit AD1_CLK=P3^5; //ADC0832 clock signal

unsigned char data  temp_data1_[2] = {0x00,0x00} ;
//unsigned char data  display1_[5] =   {0x00,0x00,0x00,0x00,0x00} ;
//unsigned char code  ditab[16] =    {0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09} ;/*С��λ����������*/


/**************����ĵ�һ����************************/
sbit ADC_0=P0^0;
sbit ADC_1=P0^1;
sbit ADC_2=P0^2;
sbit ADC_3=P0^3;
sbit ADC_4=P0^4;
sbit ADC_5=P0^5;
sbit ADC_6=P0^6;
sbit ADC_7=P0^7;

sbit ADC_work_LED_flag=P2^6;
uchar channel=2;
/**************����ĵ�һ���֣�����*******************/
/*******************************************************************
void delay1(int ms)//�ӳٺ���

{ unsigned char y ;
  while(ms--)
  {
  for(y = 0 ; y<250 ; y++)
  {
   _nop_() ;
   _nop_() ;
   _nop_() ;
   _nop_() ;
  }
  }
 }
/**********************************************************************/
 unsigned int Adc1_0832()     //ADת�������ؽ��
{

    uchar i=0;
    uchar j;
    uint dat=0;
    uchar ndat=0;
    AD1_DI=1;
    _nop_();
    _nop_();
    AD1_CS=0;//����CS��
    _nop_();
    _nop_();
    AD1_CLK=1;//����CLK��
    _nop_();
    _nop_();
    AD1_CLK=0;//����CLK��,�γ��½���1
    _nop_();
    _nop_();
    AD1_CLK=1;//����CLK��
    AD1_DI=channel&0x1;	//ͨ��0
    _nop_();
    _nop_();
    AD1_CLK=0;//����CLK��,�γ��½���2
    _nop_();
    _nop_();
    AD1_CLK=1;//����CLK��
    AD1_DI=(channel>>1)&0x1;//ѡͨ��	1
    _nop_();
    _nop_();
    AD1_CLK=0;//����CLK��,�γ��½���3
    AD1_DI=1;//����������� 
    _nop_();
    _nop_();
    dat=0;
    for(i=0;i<8;i++)
    {
        dat|=AD1_DO;//������
        AD1_CLK=1;
        _nop_();
        _nop_();
        AD1_CLK=0;//�γ�һ��ʱ������
        _nop_();
        _nop_();
        dat<<=1;
        if(i==7)dat|=AD1_DO;
    }  
    for(i=0;i<8;i++)
    {
        j=0;
        j=j|AD1_DO;//������
        AD1_CLK=1;
        _nop_();
        _nop_();
        AD1_CLK=0;//�γ�һ��ʱ������
        _nop_();
        _nop_();
        j=j<<7;
        ndat=ndat|j;
        if(i<7)ndat>>=1;
    }
    AD1_CS=1;//����CS��
    AD1_CLK=0;//����CLK��
    AD1_DO=1;//�������ݶ�,�ص���ʼ״̬
    dat<<=8;
    dat|=ndat;
    return(dat);            //return ad k
}

  void main()
  { 
	while(1) {  
		/******************************/
//		ADC_work_LED_flag=!ADC_work_LED_flag;	
		/*****************************/ 

		temp_data1_[0] =  Adc1_0832();   //��8λ

/**************����ĵڶ�����*****************************/
//		 ADC_0=temp_data1_[0]^7;// ����������Ƭ���˸����Ŵ����ݣ��˴�ֻ��P0=Adc1_0832();������
//		 ADC_1=temp_data1_[0]^6;
//		 ADC_2=temp_data1_[0]^5;
//		 ADC_3=temp_data1_[0]^4;
//		 ADC_4=temp_data1_[0]^3;
//		 ADC_5=temp_data1_[0]^2;
//		 ADC_6=temp_data1_[0]^1;
//		 ADC_7=temp_data1_[0]^0;
				 
//		P0=Adc1_0832();
		P0=temp_data1_[0];
		
/**************����ĵڶ����֣�����************************/
	 }
	}



