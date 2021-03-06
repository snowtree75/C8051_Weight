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




/**********************************************************
   串口0中断子程序
**********************************************************/
unsigned int 	w;
char					pos = 1;
unsigned char ch;
void UART0_ISR(void) interrupt  4
{//串口0中断
	unsigned char  saveSFRPAGE=SFRPAGE;
	SFRPAGE = UART0_PAGE;
	if(RI0){
		RI0 = 0;
		ch = SBUF0;
		w <<= 8;
		w |= ch;
		//buzzer();
		if(w == 0x0D0A)//CR LF 表示收到一个指令的结束符号
		{
			//if(ucCom0ReceivePointer > 18)//取15 为收到指令的长度，有点牵强 
			{
				ucCom0ReceiveByte[pos - 1]='\0';
				fCurWeight = atof(ucCom0ReceiveByte);		
			}	
			ucCom0ReceivePointer = 0;
			
			// for balance on & off
			tianping_status = 1;
			second = 0;
		}else{
			if(ch == 0x67){//'g'
				pos = ucCom0ReceivePointer;
			}
			ucCom0ReceiveByte[ucCom0ReceivePointer++] = ch;
		}

	}
	SFRPAGE = saveSFRPAGE;
}

/**********************************************************
   串口1中断子程序
**********************************************************/
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
			if(  ucCom1ReceiveByte[ucCom1ReceivePointer - 1] == 0x3c 
				&& ucCom1ReceiveByte[ucCom1ReceivePointer - 2] == 0xc3
				&& ucCom1ReceiveByte[ucCom1ReceivePointer - 3] == 0x33
			  && ucCom1ReceiveByte[ucCom1ReceivePointer - 4] == 0xcc
			  && ucCom1ReceiveByte[0] == 0xAA)		
			{
				currentCommand = ucCom1ReceiveByte[2];
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

void sendcom1computer_float(unsigned char mod,float dat)
{ 
	unsigned char a,b,c,d;
	unsigned char* ptr = (unsigned char*)&dat;
	unsigned char context[5] = {0};
	a = *ptr;ptr++;
	b = *ptr;ptr++;
	c = *ptr;ptr++;
	d = *ptr;
	
	context[0] = mod;
	context[1] = a;
	context[2] = b;
	context[3] = c;
	context[4] = d;
	
	sendcom1computer_byte(0xaa);   
	sendcom1computer_byte(mod); 
	sendcom1computer_byte(a); 
	sendcom1computer_byte(b); 
	sendcom1computer_byte(c); 
	sendcom1computer_byte(d); 
	sendcom1computer_byte(generateCRC(context,5)); 
	sendcom1computer_byte(0xcc);  
	sendcom1computer_byte(0x33);  
	sendcom1computer_byte(0xc3);  
	sendcom1computer_byte(0x3c);    
} 
