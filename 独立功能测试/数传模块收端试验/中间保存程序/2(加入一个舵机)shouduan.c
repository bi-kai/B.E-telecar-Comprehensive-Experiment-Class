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
sbit    KEY1=P3^4;
sbit    KEY2=P3^5;

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

/*######################加入的舵机程序部分一########################*/
bit duoji_1_UP=0;
bit duoji_1_DOWN=0;
sbit out_1=P1^2;
unsigned int PWM_1=0xFA99;   //舵机1赋初值
bit longorshorttimerflag=0;
unsigned int timerbuf[2]={0};

void delayms(unsigned char ms);
void refree_allpins();



/*######################舵机部分程序一加入完毕####################*/

void main()
{
/*#####################加入的舵机程序部分二######################*/
	 out_1=0;
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



/*#####################舵机部分程序二加入完毕#######################*/

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
Delay(150);
LED1=1;
LED2=1;


/******************加入串口(用于调试)********************************************
	SCON=0x50;//串口方式1，允许接收
   TMOD=0x20;//定时器1，定时方式2
   TCON=0x40;//设定时器1开始计数
   TH1=0xE8;//11.0592MHz, 1200波特率
   TL1=0xE8;
   TI=1;
   TR1=1;
//send("before while(1).\n");
******************串口加入结束********************************************/

while(1)
{
//		  Delay(1);
		SetRxMode();
	   if (DR){			// If recive data ready... 
	   RxPacket();			// ... recive data

/*##################加入的舵机程序部分三################################*
if(duoji_1_UP==1){
            if(PWM_1<0xFBF7){PWM_1=PWM_1+10;delayms(2);}
           else {LED1=0;} 
         };

     if(duoji_1_DOWN==1){
           if(PWM_1>0xF93B){PWM_1=PWM_1-10;delayms(2);}
           else {LED2=0;} 
          };

/*################舵机部分程序三加入完毕################################*/


		}
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
	TRX_CE=1;
	 //===========以下为收到数据后的处理==========
	   if(RxBuf[0] == 1){ LED1 = 0 ;duoji_1_UP=1;}
		else {LED1 = 1;duoji_1_UP=0;}
	   if(RxBuf[1] == 1) {LED2 = 0 ;duoji_1_DOWN=1;}
	  	else{ LED2 = 1 ;duoji_1_DOWN=0;}							
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
*/

/*********************************************************/
// 定时器0中断服务程序.(用于舵机1)
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
void delayms(unsigned char ms) 

{
    unsigned char i;
    while(ms--)
     {
        for(i=0;i<120;i++);
     }
}

