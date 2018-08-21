#ifndef __CONFIG_H__
#define __CONFIG_H__

/****************************************************
            ---------跟踪步进电机方向---------
****************************************************/
#define 				UP								0
#define 				DOWN						  1
/****************************************************
            ---------与电脑通信使用的波特率---------
****************************************************/
#define         Baute9600          0xa0;
#define         Baute57600         0x40;

/****************************************************
            ---------CPU 引脚定义---------
****************************************************/
sfr16 	ADC0     = 0xbe;            // ADC0 data

sbit    TXD0=P0^0;//串口0发送
sbit    RXD0=P0^1;//串口0接收
sbit    SCK=P0^2; //SPI总线SCK引脚 
sbit    MISO=P0^3;//SPI总线MISO引脚
sbit    MOSI=P0^4;//SPI总线MOSI引脚
sbit    SDA=P0^5; //IIC总线SDA引脚
sbit    SCL=P0^6; //IIC总线SCL引脚
sbit 		TXD1=P0^7;//串口1发送
sbit    RXD1=P1^0;//串口1接收
sbit    INT0=P1^1;//外中断0引脚

sbit    CS1=P1^2; //AD7714片选
sbit    CS2=P1^3; //AD7714片选
sbit    CS3=P1^4; //AD7714片选
sbit    CS4=P4^0; //AD7714片选
sbit    CS5=P4^1; //AD7714片选
//===============================================    
sbit    diguanheat							=	P2^7;   

sbit    DIR1										=	P1^5; //步进电机PULL
sbit   	shaoping_mor_PIN				= P1^5; //shaoping motor
sbit    DIR0										=	P1^6; //步进电机DIR
sbit    PULSE0									=	P1^7; //步进电机PULL
sbit   	BUZZ										=	P4^3; //蜂鸣器
sbit    TRACING0reset						= P3^7; //电机复位检测
sbit    shaopingresetjudge_pin	= P3^6; //电机复位检测
//===============================================
//*************************************************
//------------此二脚在电路板上标反-----------------

sbit 	LOOPmor										= P2^0;    //  
sbit 	liangtongheat							= P2^1;		//继电器
sbit 	shaopingheat_PIN					= P2^2;		//继电器
sbit 	COMPRESSOR								= P2^3; 	//压缩机
sbit 	FAN0											= P2^4;		//继电器     风扇
sbit 	FURNACE0									= P2^5;		//继电器 炉子
sbit 	waterHEATER								= P2^6;		//电热棒 


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
#define CRLV 0x59//回收速度 ml/min
#define CDLV 0x58
		
//------------此二脚在电路板上标反-----------------
//************************************************* 

// 定义每一个7714传感器对应的温度
#define 				DEVICEP1STREAMTEMP		0
#define 				DEVICEDENSORTEMP			1
#define 				RETRIEVEROOMTTEMP 		2
#define 				NECKTEMP							3

//===============================================
// 定义仪器的工作状态
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