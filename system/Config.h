#ifndef __CONFIG_H__
#define __CONFIG_H__

/****************************************************
            ---------���ٲ����������---------
****************************************************/
#define 				UP								0
#define 				DOWN						  1
/****************************************************
            ---------�����ͨ��ʹ�õĲ�����---------
****************************************************/
#define         Baute9600          0xa0;
#define         Baute57600         0x40;

/****************************************************
            ---------CPU ���Ŷ���---------
****************************************************/
sfr16 	ADC0     = 0xbe;            // ADC0 data

sbit    TXD0=P0^0;//����0����
sbit    RXD0=P0^1;//����0����
sbit    SCK=P0^2; //SPI����SCK���� 
sbit    MISO=P0^3;//SPI����MISO����
sbit    MOSI=P0^4;//SPI����MOSI����
sbit    SDA=P0^5; //IIC����SDA����
sbit    SCL=P0^6; //IIC����SCL����
sbit 		TXD1=P0^7;//����1����
sbit    RXD1=P1^0;//����1����
sbit    INT0=P1^1;//���ж�0����

sbit    CS1=P1^2; //AD7714Ƭѡ
sbit    CS2=P1^3; //AD7714Ƭѡ
sbit    CS3=P1^4; //AD7714Ƭѡ
sbit    CS4=P4^0; //AD7714Ƭѡ
sbit    CS5=P4^1; //AD7714Ƭѡ
//===============================================    
sbit    diguanheat							=	P2^7;   

sbit    DIR1										=	P1^5; //�������PULL
sbit   	shaoping_mor_PIN				= P1^5; //shaoping motor
sbit    DIR0										=	P1^6; //�������DIR
sbit    PULSE0									=	P1^7; //�������PULL
sbit   	BUZZ										=	P4^3; //������
sbit    TRACING0reset						= P3^7; //�����λ���
sbit    shaopingresetjudge_pin	= P3^6; //�����λ���
//===============================================
//*************************************************
//------------�˶����ڵ�·���ϱ귴-----------------

sbit 	LOOPmor										= P2^0;    //  
sbit 	liangtongheat							= P2^1;		//�̵���
sbit 	shaopingheat_PIN					= P2^2;		//�̵���
sbit 	COMPRESSOR								= P2^3; 	//ѹ����
sbit 	FAN0											= P2^4;		//�̵���     ����
sbit 	FURNACE0									= P2^5;		//�̵��� ¯��
sbit 	waterHEATER								= P2^6;		//���Ȱ� 


////////////////////////////////////////////////////
#define SUCCESS	0
#define FAILURE 1

///////////////////////////////////////////////	
#define IDLE 0x00
#define SZCC 0x52
#define SVCC 0x53
#define SSAC 0x58
#define SSTC 0x59
#define SSPC 0x81
#define SRST 0x5a
#define SFW1 0x41
#define SFW2 0x9c
#define SVSC 0xa1

//////////////////////////////////

#define CATM 0x90//
#define CRHT 0x96
#define CCLT 0x92
#define CVIF 0x94
#define CRVL 0x95
#define CFPT 0x81
#define CFPD 0x82
#define CLPT 0x87
#define CCTM 0x9a
#define CSGT 0x91
#define CFIF 0x93
#define CFPP 0x98
#define CNKT 0x97
#define CSTA 0x5c
#define CCNT 0xa0
#define CFW1 0x41
#define CFW2 0x9c
#define CBOF 0x3e
#define CSMW 0xa1
#define CRLV 0x59//�����ٶ� ml/min
#define CDLV 0x58
		
//------------�˶����ڵ�·���ϱ귴-----------------
//************************************************* 

// ����ÿһ��7714��������Ӧ���¶�
#define 				DEVICEP1STREAMTEMP		0
#define 				DEVICEDENSORTEMP			1
#define 				RETRIEVEROOMTTEMP 		2
#define 				NECKTEMP							3

//===============================================
// ���������Ĺ���״̬
#define 				READY											0
#define 				CABLIRATEZEROPOINT				1
#define 				CABLIRATEVOLUMN						2
#define 				FIRST300SECONDHEATING			3
#define 				F300SECOND2FIRSTPOINT			4
#define 				FIRSTPOINT2VOLUMN96PER		5
#define 				VOLUMN96PER2LASTPOINT			6
#define 				DEVICECOOLING							7
#define 				CRYOSTATING								8
#define 				ERROR											9
#endif