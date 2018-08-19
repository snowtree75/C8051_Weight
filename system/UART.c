#include "includes.h"
#include <math.h>
#include "UART.h"
#include "DeviceStatus.h"
#include "miscellaneous.h"
#include "step.h"

void StartDevice();
////////////////////////////////////////////////////////////////
// UART0 与 天平通信，使用Timer3的溢出率为其波特率。目前为1200
// 发送使用查询方式。接受使用中断方式
void UART0Init(){
	unsigned char SFRPAGE_SAVE = SFRPAGE; 
	SFRPAGE   = UART0_PAGE;
	SCON0     = 0x50;  //REN0 = 1
	SSTA0     = 0x0A;  //T3 波特率

	SFRPAGE = SFRPAGE_SAVE; 
}	


/**********************************************************
   串口0中断子程序
**********************************************************/
void sendcom0(unsigned char dat){
	unsigned char SFRPAGE_SAVE = SFRPAGE; //向串口0发送一个字节
	SFRPAGE=UART0_PAGE;
	SBUF0 = dat;
 	while(TI0 == 0);        //等待发送完毕
 	TI0 = 0;
	SFRPAGE = SFRPAGE_SAVE; 
}

void sendscom0computer(void *ptr,unsigned int n){
	unsigned char SFRPAGE_SAVE = SFRPAGE; //向串口0发送多个字节(T3，为定时器)
	unsigned char *p=(unsigned int *)ptr;
	while(n--){
		SFRPAGE = UART0_PAGE;
		SBUF0=*(p++);
		while(TI0 == 0);        //等待发送完毕
		TI0 = 0;
	}                //软件清零
	SFRPAGE = SFRPAGE_SAVE; 
}


/*
void UART0ISR(void) interrupt  4
{
	unsigned char  saveSFRPAGE=SFRPAGE;
	SFRPAGE = UART0_PAGE;
	if(RI0){
		RI0 = 0;
		buzzer();

	//	sendcom0(0xf1);		

		/*ch<<=8;
		ch |= SBUF0;
		if(ch == 0x0D0A)//CR LF 表示收到一个指令的结束符号
		{
			if(Com0ReceivePointer > 15)//取15 为收到指令的长度，有点牵强 
			{
				Com0ReceiveByte[11]='\0';
				//CurWeight = atof(Com0ReceiveByte);		
			}			
			Com0ReceivePointer = 0;
		}else{
			Com0ReceiveByte[Com0ReceivePointer++] = SBUF0;
		}

	}
	SFRPAGE = saveSFRPAGE;
}*/
////////////////////////////////////////////////////////////////
// UART1 与计算机通信，使用T1的溢出率为其波特率。目前为57600
// 发送使用查询方式。接受使用中断方式
void UART1Init()
{
	unsigned char SFRPAGE_SAVE = SFRPAGE; 
    SFRPAGE   = UART1_PAGE;
    SCON1     = 0x50;   //REN1=1  
    //SSTA1     = 0x0A;   只有T1 可做波特率
	SFRPAGE = SFRPAGE_SAVE; 
}
unsigned int s;
void UART1ISR(void) interrupt  20
{
	unsigned char  saveSFRPAGE = SFRPAGE;
	SFRPAGE = UART1_PAGE;
	if(RI1){
		RI1=0;
		ucCom1ReceiveByte[ucCom1ReceivePointer] = SBUF1;
		if(ucCom1ReceiveByte[0] == 0xAA)
		{
			ucCom1ReceivePointer++; 
			if(ucCom1ReceiveByte[ucCom1ReceivePointer-1] == 0xFF && ucCom1ReceiveByte[0] == 0xAA)		
			{
				switch(ucCom1ReceiveByte[1]){
				
					case 0x80:
						currentCommand = ucCom1ReceiveByte[2];
						break;
						
				  case 0x81://上位机传递的参数
						currentCommand = ucCom1ReceiveByte[2];
						buzzer();
							
							/*InitAllDatas();
				 
							ucMethod = ucCom1ReceiveByte[2];
							ucP1 = ucCom1ReceiveByte[3];
							ucP2 = ucCom1ReceiveByte[4];
							ucP3 = ucCom1ReceiveByte[5];
							ucP4 = ucCom1ReceiveByte[6];
							ucLow = ucCom1ReceiveByte[7];
							ucHigh = ucCom1ReceiveByte[8];
							uiStreamCorrect1 = ucCom1ReceiveByte[9];
							uiStreamCorrect1 <<= 8;
							uiStreamCorrect1 += ucCom1ReceiveByte[10];
							uiStreamCorrect2 = ucCom1ReceiveByte[11];
							uiStreamCorrect2 <<= 8;
							uiStreamCorrect2 += ucCom1ReceiveByte[12];
							uiCondensorCorrect = ucCom1ReceiveByte[13];
							uiCondensorCorrect <<= 8;
							uiCondensorCorrect += ucCom1ReceiveByte[14];
							uiAtmCorrect = ucCom1ReceiveByte[15];
							uiAtmCorrect <<= 8;
							uiAtmCorrect += ucCom1ReceiveByte[16];
							ucAuxtemp = ucCom1ReceiveByte[17];
							ucJingbuTemp = ucCom1ReceiveByte[18];
							
							/////////////////////////////////////////
							iCryostatHigh = ucHigh;
							iCryostatLow = ucLow;
							uiPower1 = ucP1;
							uiPower2 = ucP2;
							uiPower3 = ucP3;
							uiPower4 = ucP4;
							fTotalWeight = ucJingbuTemp;
							updateDensor();*/
						break;
					
				}
				ucCom1ReceivePointer = 0;
			}	
		}
	}
	SFRPAGE=saveSFRPAGE;
}

void sendcom1computer_byte(unsigned char dat)
{
	unsigned char SFRPAGE_SAVE = SFRPAGE;//向串口1发送一个字节
	SFRPAGE = UART1_PAGE;
	SBUF1 = dat;
	while(TI1 == 0);        //等待发送完毕
	TI1= 0;
	SFRPAGE = SFRPAGE_SAVE;
}

void sendscom1computer(void *ptr,unsigned int n){
	unsigned char SFRPAGE_SAVE = SFRPAGE; //向串口0发送多个字节(T3，为定时器)	
	unsigned char *p = (unsigned char *)ptr;
	SFRPAGE = UART1_PAGE;
	while(n--){		
		SBUF1 = *p;
		while(TI1 == 0);        //等待发送完毕
		TI1= 0;
		p++;
	}                //软件清零
	SFRPAGE = SFRPAGE_SAVE; 
}

void sendcom1computer_float(unsigned int mod,float dat)
{ 
	unsigned char a,b,c,d;
	unsigned char* ptr = (unsigned char*)&dat;
	a = *ptr;ptr++;
	b = *ptr;ptr++;
	c = *ptr;ptr++;
	d = *ptr;
	
	sendcom1computer_byte(0xaa);   
	sendcom1computer_byte(mod); 
	//sendscom1computer(&dat,sizeof(float));
	sendcom1computer_byte(a); 
	sendcom1computer_byte(b); 
	sendcom1computer_byte(c); 
	sendcom1computer_byte(d); 
	sendcom1computer_byte(0xcc);  
	sendcom1computer_byte(0x33);  
	sendcom1computer_byte(0xc3);  
	sendcom1computer_byte(0x3c);    
} 
