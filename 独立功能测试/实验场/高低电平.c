/******************************************************************************************\ 
============================================================================================
* T905.c
*
* This file is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTYT; 
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*
* NOTES:
* This program is for the nRF905 with 232 interface;
* $write date: 2004.4.7 ;time 11:00$
* $Revision: 1 $
*
/*******************************************************************************************/
#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint  unsigned int

#define WC		0x00			// Write configuration register command
#define RC		0x10 			// Read  configuration register command
#define WTP		0x20 			// Write TX Payload  command
#define RTP		0x21			// Read  TX Payload  command
#define WTA		0x22			// Write TX Address  command
#define RTA		0x23			// Read  TX Address  command
#define RRP		0x24			// Read  RX Payload  command
/*******************************************************************************************/
typedef struct RFConfig
{
	uchar n;
	uchar buf[10];
}RFConfig;

code RFConfig RxTxConf={
	10,
	0x01, 0x0c, 0x44, 0x20, 0x20, 0xcc, 0xcc, 0xcc,0xcc, 0x58	
};
// The content of this struct is nRF905's initialize data.��ʼ������
// CH_NO=1;433MHZ;Normal Opration,No Retrans;RX,TX Address is 4 Bytes
// RX TX Payload Width is 32 Bytes;Disable Extern Clock;Fosc=16MHZ
// 8 Bits CRC And enable
/*******************************************************************************************/
uchar data TxBuf[32];
uchar data RxBuf[32];
uchar data Textbuf[2];
uchar key;
uchar rsbrf;
uchar delayf;

/*******************************************************************************************/
uchar bdata DATA_BUF;
sbit	flag=DATA_BUF^7;
sbit	flag1=DATA_BUF^0;
/*******************************************************************************************/
sbit	TX_EN=P1^7;
sbit	TRX_CE=P1^6;
sbit	PWR_UP=P1^5;
sbit	MISO=P1^1;
sbit	MOSI=P1^0;
sbit	SCK=P3^7;
sbit	CSN=P3^3;

sbit	AM=P1^3;//��ַƥ��
sbit	DR=P3^2;//���ջ������
sbit	CD=P1^4;//�ز����

sbit duoji_1_up=P2^0;
sbit duoji_1_down=P2^1;
sbit duoji_2_up=P2^2;
sbit duoji_2_down=P2^3;

sbit motor_1_1=P2^4;
sbit motor_1_2=P2^5;
sbit motor_2_1=P2^6;
sbit motor_2_2=P2^7;

//sbit    LED3=P2^4;
//sbit    LED4=P2^5;
//sbit    LED1=P2^6;
//sbit    LED2=P2^7;
//sbit    KEY1=P3^0;
//sbit    KEY2=P3^1;

/*******************************************************************************************/
void InitIO(void);			// Initialize IO port
void Inituart(void);			// initialize 232 uart
void Config905(void);			// Config nRF905 module(�����ģ��)����NRF905ģʽ
void SetTxMode(void);			// Set nRF905 in Tx mode
void SetRxMode(void);			// Set nRF905 in Rx mode
void TxPacket(void);			// Send data by nRF905
void RxPacket(void);			// Recive data by nRF905
void SpiWrite(uchar);			// Write data to nRF905
uchar SpiRead(void);			// Read data to nRF905
void Delay(uchar n);			// Delay 100us
void Scankey(void);				// Scan key
void TxData (uchar x);			// Send key_value to CRT display
void comdebug(void);
void send(unsigned char str[]);
void delayms(unsigned char ms);
/*******************************************************************************************/
void Delay(uchar n)
{
	uint i;
	while(n--)
	for(i=0;i<80;i++);	
}

/*######################����Ķ�����򲿷�һ########################*
sbit out_1=P1^2;
sbit out_2=P3^4;
unsigned int PWM_1=0xFA99;   //���1����ֵ
unsigned int PWM_2=0xFA99;   //���2����ֵ
bit longorshorttimerflag=0;
bit longorshorttimerflag_2=0;
unsigned int timerbuf[2]={0};
/**********************************************************/
void delayms(unsigned char ms);
void refree_allpins();



/*######################������ֳ���һ�������####################*/

/*********************ֱ���������ĵ�һ����*********************************
sbit motor11=P2^0;//���1���ź�����
sbit motor12=P2^1;

sbit motor21=P2^2;//���2���ź�����
sbit motor22=P2^3;

/**********************ֱ���������ĵ�һ���ֽ���********************************/

void main()
{



    //================NRF905��ʼ��
	CSN=1;						// Spi 	disable						
	SCK=0;						// Spi clock line init high
	DR=1;						// Init DR for input
	AM=1;						// Init AM for input
	PWR_UP=1;					// nRF905 power on
	TRX_CE=0;					// Set nRF905 in standby mode
	TX_EN=0;					// set radio in Rx mode	

	//================
	Config905();				// Config nRF905 module
//	TxBuf[0]=1;
//	TxBuf[1]=1;
//	SetTxMode();				// Set Tx Mode
//	TxPacket();				// Transmit Tx buffer data
//	TxBuf[0] = 0xff;
//	TxBuf[1] = 0xff;
//LED1=0;
//LED2=0;
//Delay(150);
//LED1=1;
//LED2=1;

					// led close
//	SetRxMode();				// Set nRF905 in Rx mode


/******************���봮��********************************************
	SCON=0x50;//���ڷ�ʽ1���������
   TMOD=0x20;//��ʱ��1����ʱ��ʽ2
   TCON=0x40;//�趨ʱ��1��ʼ����
   TH1=0xE8;//11.0592MHz, 1200������
   TL1=0xE8;
   TI=1;
   TR1=1;
send("before while(1).\n");
******************���ڼ������********************************************/
/*#####################����Ķ�����򲿷ֶ�######################*
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


refree_allpins();
/*#####################������ֳ�����������#######################*/

while(1)
{refree_allpins();
/***********************************************************/
		  delayms(1);
		SetRxMode();
	   if (DR){			// If recive data ready... 
	   RxPacket();			// ... recive data
			}
			
/*****************************************************/
		/*##################����Ķ�����򲿷���################################*/
		if(RxBuf[0]==1){
            duoji_1_up=0;
				}
           else {
			duoji_1_up=1;
			  } 
         }//duoji_1_UP//duoji_1_DOWN

     if(RxBuf[1]==1){
          duoji_1_down=0;
			  }
           else {
			duoji_1_down=1;
			  } 
          }

		if(RxBuf[2]==1){
          duoji_2_up=0;
			  }
           else {
				duoji_2_up=0;
			  } 
          }

		 if(RxBuf[3]==1){
           duoji_2_down=0;
			  }
           else {
			duoji_2_down=1;
			  } 
          }

/*################������ֳ������������################################*/
/*******************************ֱ���������ĵڶ�����*************************/
			if(RxBuf[4]==1){
				motor_1_1=1;
				motor_1_2=0;
				motor_2_1=1;
				motor_2_2=0;
			}
			
			if(RxBuf[5]==1){
			motor_1_1=0;
				motor_1_2=1;
				motor_2_1=0;
				motor_2_2=1;
			}

			if(RxBuf[6]==1){
				motor_1_1=0;
				motor_1_2=1;
				motor_2_1=1;
				motor_2_2=0;
			}

			if(RxBuf[7]==1){
			motor_1_1=1;
				motor_1_2=0;
				motor_2_1=0;
				motor_2_2=1;
			}

			if(RxBuf[8]==1){
			motor_1_1=0;
				motor_1_2=0;
				motor_2_1=1;
				motor_2_2=0;
			}

			if(RxBuf[9]==1){
			motor_1_1=1;
				motor_1_2=0;
				motor_2_1=0;
				motor_2_2=0;
			}
	
        delayms(10);
/*******************************ֱ���������ĵڶ��������********************/
		
}

}
/*******************************************************************************************/
/*******************************************************************************************/
//function Config905();
/*******************************************************************************************/
void Config905(void)
{
	uchar i;					
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(WC);				// Write config commandд����������
	for (i=0;i<RxTxConf.n;i++)	// Write configration words  д��������
	{
		SpiWrite(RxTxConf.buf[i]);
	}
	CSN=1;						// Disable Spi
}
/*******************************************************************************************/
//function SpiWrite();
/*******************************************************************************************/
void SpiWrite(uchar  byte)
{
	uchar i;	
	DATA_BUF=byte;				// Put function's parameter into a bdata variable		
	for (i=0;i<8;i++)			// Setup byte circulation bits
	{	
		
		if (flag)				// Put DATA_BUF.7 on data line
			MOSI=1;
		else
			MOSI=0;
		SCK=1;					// Set clock line high
		DATA_BUF=DATA_BUF<<1;	// Shift DATA_BUF
		SCK=0;					// Set clock line low 
	}	
}
/*******************************************************************************************/
//function SpiRead();
/*******************************************************************************************/
uchar SpiRead(void)
{
	uchar i;	
	for (i=0;i<8;i++)			// Setup byte circulation bits
	{	
		DATA_BUF=DATA_BUF<<1;	// Right shift DATA_BUF
		SCK=1;					// Set clock line high
		if (MISO)
			flag1=1;			// Read data
		else
			flag1=0;
							
		
		SCK=0;					// Set clock line low
	}
	return DATA_BUF;			// Return function parameter
}
/*******************************************************************************************/
//function TxPacket();
/*******************************************************************************************
void TxPacket(void)
{
	uchar i;
	//Config905();				
	CSN=0;						// Spi enable for write a spi command	
	SpiWrite(WTP);				// Write payload command
	for (i=0;i<32;i++)
	{
		SpiWrite(TxBuf[i]);		// Write 32 bytes Tx data
	}
	CSN=1;						// Spi disable						
	Delay(1);
	CSN=0;						// Spi enable for write a spi command	
	SpiWrite(WTA);				// Write address command
	for (i=0;i<4;i++)			// Write 4 bytes address
	{
		SpiWrite(RxTxConf.buf[i+5]);
	}	
	CSN=1;						// Spi disable
	TRX_CE=1;					// Set TRX_CE high,start Tx data transmission
	Delay(1);					// while (DR!=1);
	TRX_CE=0;					// Set TRX_CE low
}
/*******************************************************************************************/
//function RxPacket();
/*******************************************************************************************/
void RxPacket(void)
{
	uchar i;	
	TRX_CE=0;					// Set nRF905 in standby mode	
	CSN=0;						// Spi enable for write a spi command
	SpiWrite(RRP);				// Read payload command	
	for (i=0;i<32;i++)
	{
		RxBuf[i]=SpiRead();		// Read data and save to buffer		
	}
	CSN=1;						// Disable spi
	while(DR||AM);	
	TRX_CE=1;
	 //=====================
	  if(RxBuf[0]==1){ 
	  				

							}
		else {
					

		}

	   if(RxBuf[1] == 1) {
				

		}
	  	else{ 
						
			
		}							
							
}

/*******************************************************************************************/
//function SetTxMode();
/*******************************************************************************************
void SetTxMode(void)				
{	
	TX_EN=1;
	TRX_CE=0;
	Delay(1); 					// delay for mode change(>=650us)
}				
/*******************************************************************************************/
//function SetRxMode();
/*******************************************************************************************/
void SetRxMode(void)
{
	TX_EN=0;
	TRX_CE=1;
	Delay(1); 					// delay for mode change(>=650us)				
}



/***************************************
void send(unsigned char str[])
{
	unsigned int i=0;
	while(str[i]!='\0')
	{
		SBUF=str[i];
		while(!TI);
		TI=0;
		i++;
		}
}
***************************************/
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
/*********************************************************
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
/*********************************************************
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

            motor_1_1=0;
		  		motor_1_2=0;
				motor_2_1=0;
				motor_2_2=0;

}
/****************************************************************/
