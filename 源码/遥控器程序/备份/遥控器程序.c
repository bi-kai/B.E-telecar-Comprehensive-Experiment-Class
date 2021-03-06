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
// The content of this struct is nRF905's initialize data.初始化数据
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
//uchar tf;
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

sbit	AM=P1^3;//地址匹配
sbit	DR=P3^2;//接收或发射完成
sbit	CD=P1^4;//载波检测

sbit    LED1=P2^6;
sbit    LED2=P2^7;
sbit    KEY1=P3^0;
sbit    KEY2=P3^1;
sbit    LED3=P2^4;
sbit    LED4=P2^5;

sbit    KEY3=P2^2;
sbit    KEY4=P2^3;

/************以下为控制直流电机的按键的引脚********************/
sbit motorforward=P0^4;
sbit motorback=P0^5;
sbit motorleftcircle=P0^6;
sbit motorrightcircle=P0^7;
sbit motorleft=P2^0;
sbit motorright=P2^1;
/*******************************************************************************************/
void InitIO(void);			// Initialize IO port
void Inituart(void);			// initialize 232 uart
void Config905(void);			// Config nRF905 module(组件、模块)配置NRF905模式
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
/*******************************************************************************************/
void Delay(uchar n)
{
	uint i;
	while(n--)
	for(i=0;i<80;i++);	
}

void main()
{
    //================NRF905初始化
	CSN=1;						// Spi 	disable						
	SCK=0;						// Spi clock line init high
	DR=1;						// Init DR for input
	AM=1;						// Init AM for input
	PWR_UP=1;					// nRF905 power on
	TRX_CE=0;					// Set nRF905 in standby mode
	TX_EN=0;					// set radio in Rx mode	

	//================
	Config905();				// Config nRF905 module
LED1=0;
LED2=0;
LED3=0;
LED4=0;
Delay(150);
LED1=1;
LED2=1;
LED3=1;
LED4=1;

/******************加入串口********************************************/
	SCON=0x50;//串口方式1，允许接收
   TMOD=0x20;//定时器1，定时方式2
   TCON=0x40;//设定时器1开始计数
   TH1=0xE8;//11.0592MHz, 1200波特率
   TL1=0xE8;
   TI=1;
   TR1=1;
//send("before while(1).\n");
/******************串口加入结束********************************************/

	TxBuf[0]=0xff;
	TxBuf[1]=0xff;
	TxBuf[2]=0xff;
	TxBuf[3]=0xff;
while(1)
{

motorforward=1;
motorback=1;
motorleftcircle=1;
motorrightcircle=1;
motorleft=1;
motorright=1;


	if(KEY1==0){LED1=0;
				   TxBuf[0]=1;
					while(KEY1==0){
						 Delay(1);
						 TxBuf[0]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[0]=0xff;
			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}


	if(KEY2==0){LED2=0;
               TxBuf[1]=1;
					while(KEY2==0){
						 Delay(1);
						 TxBuf[1]=1;
						 SetTxMode();
						 TxPacket();
						  send("In KEY2 while.");
						 }
			LED2=1;
			Delay(1);
		  	TxBuf[0]=0xff;
			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY2 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}

	if(KEY3==0){LED3=0;
               TxBuf[2]=1;
					while(KEY3==0){
						 Delay(1);
						 TxBuf[2]=1;
						 SetTxMode();
						 TxPacket();
						  send("In KEY3 while.");
						 }
			LED3=1;
			Delay(1);
//		  	TxBuf[0]=0xff;
//			TxBuf[1]=0xff;
			TxBuf[2]=0xff;
			TxBuf[3]=0xff;
			send("out of KEY3 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}


	if(KEY4==0){LED4=0;
               TxBuf[3]=1;
					while(KEY4==0){
						 Delay(1);
						 TxBuf[3]=1;
						 SetTxMode();
						 TxPacket();
						  send("In KEY4 while.");
						 }
			LED4=1;
			Delay(1);
//		  	TxBuf[0]=0xff;
//			TxBuf[1]=0xff;
			TxBuf[2]=0xff;
			TxBuf[3]=0xff;
			send("out of KEY4 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}


/******************控制直流电机的按键的判断，加入的第二部分***************************/
if(motorforward==0){LED1=0;
				   TxBuf[4]=1;
					while(motorforward==0){
						 Delay(1);
						 TxBuf[4]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[4]=0xff;
//			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}

if(motorback==0){LED1=0;
				   TxBuf[5]=1;
					while(motorback==0){
						 Delay(1);
						 TxBuf[5]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[5]=0xff;
//			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}


if(motorleftcircle==0){LED1=0;
				   TxBuf[6]=1;
					while(motorleftcircle==0){
						 Delay(1);
						 TxBuf[6]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[6]=0xff;
//			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}

if(motorrightcircle==0){LED1=0;
				   TxBuf[7]=1;
					while(motorrightcircle==0){
						 Delay(1);
						 TxBuf[7]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[7]=0xff;
//			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}

if(motorleft==0){LED1=0;
				   TxBuf[8]=1;
					while(motorleft==0){
						 Delay(1);
						 TxBuf[8]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[8]=0xff;
//			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}


if(motorright==0){LED1=0;
				   TxBuf[9]=1;
					while(motorright==0){
						 Delay(1);
						 TxBuf[9]=1;
						 SetTxMode();
						 TxPacket();
						 send("In KEY1 while.");
						 }
			LED1=1;
			Delay(1);
			TxBuf[9]=0xff;
//			TxBuf[1]=0xff;
//			TxBuf[2]=0xff;
//			TxBuf[3]=0xff;
			send("out of KEY1 while.");
			SetTxMode();				// Set Tx Mode
		   TxPacket();	        // Transmit Tx buffer data
					}

/**********************控制直流电机的按键的判断，加入的第二部分结束**********************/

		 Delay(1);
		SetRxMode();			// Set nRF905 in Rx mode
	   if (DR){			// If recive data ready... 
	   RxPacket();			// ... recive data
		send("waiting for receive......\n");}
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
	SpiWrite(WC);				// Write config command写放配置命令
	for (i=0;i<RxTxConf.n;i++)	// Write configration words  写放配置字
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
/*******************************************************************************************/
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
//	P0=	RxBuf[0];				// Buffer[0] output from P0 port...
	TRX_CE=1;
	//xx=(RxBuf[0]>>4)&0x0f;
	//TxData(xx);
	 //=====================
	   if(RxBuf[0] == 1) LED1 = 0 ;
	   if(RxBuf[1] == 1) LED2 = 0 ;
	  
	Delay(500);					// ...light led
//	P0=0xff;					// Close led
      LED1 = 1 ;
	  LED2 = 1 ;								
}
/*******************************************************************************************/
//function SetTxMode();
/*******************************************************************************************/
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


