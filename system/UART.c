#include "includes.h"
#include <math.h>
#include "UART.h"
#include "DeviceStatus.h"
#include "miscellaneous.h"
#include "step.h"

void StartDevice();
////////////////////////////////////////////////////////////////
// UART0 �� ��ƽͨ�ţ�ʹ��Timer3�������Ϊ�䲨���ʡ�ĿǰΪ1200
// ����ʹ�ò�ѯ��ʽ������ʹ���жϷ�ʽ
void UART0Init(){
	unsigned char SFRPAGE_SAVE = SFRPAGE; 
	SFRPAGE   = UART0_PAGE;
	SCON0     = 0x50;  //REN0 = 1
	SSTA0     = 0x0A;  //T3 ������

	SFRPAGE = SFRPAGE_SAVE; 
}	


/**********************************************************
   ����0�ж��ӳ���
**********************************************************/
void sendcom0(unsigned char dat){
	unsigned char SFRPAGE_SAVE = SFRPAGE; //�򴮿�0����һ���ֽ�
	SFRPAGE=UART0_PAGE;
	SBUF0 = dat;
 	while(TI0 == 0);        //�ȴ��������
 	TI0 = 0;
	SFRPAGE = SFRPAGE_SAVE; 
}

void sendscom0computer(void *ptr,unsigned int n){
	unsigned char SFRPAGE_SAVE = SFRPAGE; //�򴮿�0���Ͷ���ֽ�(T3��Ϊ��ʱ��)
	unsigned char *p=(unsigned int *)ptr;
	while(n--){
		SFRPAGE = UART0_PAGE;
		SBUF0=*(p++);
		while(TI0 == 0);        //�ȴ��������
		TI0 = 0;
	}                //�������
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
		if(ch == 0x0D0A)//CR LF ��ʾ�յ�һ��ָ��Ľ�������
		{
			if(Com0ReceivePointer > 15)//ȡ15 Ϊ�յ�ָ��ĳ��ȣ��е�ǣǿ 
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
// UART1 ������ͨ�ţ�ʹ��T1�������Ϊ�䲨���ʡ�ĿǰΪ57600
// ����ʹ�ò�ѯ��ʽ������ʹ���жϷ�ʽ
void UART1Init()
{
	unsigned char SFRPAGE_SAVE = SFRPAGE; 
    SFRPAGE   = UART1_PAGE;
    SCON1     = 0x50;   //REN1=1  
    //SSTA1     = 0x0A;   ֻ��T1 ����������
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
						
				  case 0x81://��λ�����ݵĲ���
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
	unsigned char SFRPAGE_SAVE = SFRPAGE;//�򴮿�1����һ���ֽ�
	SFRPAGE = UART1_PAGE;
	SBUF1 = dat;
	while(TI1 == 0);        //�ȴ��������
	TI1= 0;
	SFRPAGE = SFRPAGE_SAVE;
}

void sendscom1computer(void *ptr,unsigned int n){
	unsigned char SFRPAGE_SAVE = SFRPAGE; //�򴮿�0���Ͷ���ֽ�(T3��Ϊ��ʱ��)	
	unsigned char *p = (unsigned char *)ptr;
	SFRPAGE = UART1_PAGE;
	while(n--){		
		SBUF1 = *p;
		while(TI1 == 0);        //�ȴ��������
		TI1= 0;
		p++;
	}                //�������
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
