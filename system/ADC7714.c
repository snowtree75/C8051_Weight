#include "includes.h"
#include "miscellaneous.h"
#include "SPI.h"
#include "DeviceStatus.h"


/**********************************************************
  AD7714初始化程序  
**********************************************************/
void AD7714Init(void){
     spiwrite(0xff,0);//************************************/
     spiwrite(0xff,0);//向AD7714写入32位1 软件复位AD7714
     spiwrite(0xff,0);//
     spiwrite(0xff,0);////**********************************/
     delayms(100);      //延时100ms
     spiwrite(0x24,0);//24 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0 
     spiwrite(0x4F,0);//41 B/U    WL  BST ZER0 FS11 FS10 FS9 FS8
     spiwrite(0x34,0);//34 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0xA0,0);//80 FS7    FS6 FS5 FS4  FS3  FS2  FS1 FS0
     spiwrite(0x14,0);//14 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0x20,0);//20 MD2    MD1 MD0 G2   G1   G0   BO  FSYNC
	 delayms(20);
	 spiwrite(0xff,1);//************************************/
     spiwrite(0xff,1);//向AD7714写入32位1 软件复位AD7714
     spiwrite(0xff,1);//
     spiwrite(0xff,1);////**********************************/
     delayms(100);    //延时100ms
     spiwrite(0x24,1);//24 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0 
     spiwrite(0x4F,1);//41 B/U    WL  BST ZER0 FS11 FS10 FS9 FS8
     spiwrite(0x34,1);//34 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0xA0,1);//80 FS7    FS6 FS5 FS4  FS3  FS2  FS1 FS0
     spiwrite(0x14,1);//14 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0x20,1);//20 MD2    MD1 MD0 G2   G1   G0   BO  FSYNC
	 delayms(20);
     spiwrite(0xff,2);//************************************/
     spiwrite(0xff,2);//向AD7714写入32位1 软件复位AD7714
     spiwrite(0xff,2);//
     spiwrite(0xff,2);////**********************************/
     delayms(100);    //延时100ms
     spiwrite(0x24,2);//24 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0 
     spiwrite(0x4F,2);//41 B/U    WL  BST ZER0 FS11 FS10 FS9 FS8
     spiwrite(0x34,2);//34 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0xA0,2);//80 FS7    FS6 FS5 FS4  FS3  FS2  FS1 FS0
     spiwrite(0x14,2);//14 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0x20,2);//20 MD2    MD1 MD0 G2   G1   G0   BO  FSYNC
	 delayms(20);
//*
     spiwrite(0xff,3);
     spiwrite(0xff,3);//向AD7714写入32位1 软件复位AD7714
     spiwrite(0xff,3);//
     spiwrite(0xff,3);// 
     delayms(100);    //延时100ms
     spiwrite(0x24,3);//24 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0 
     spiwrite(0x4F,3);//41 B/U    WL  BST ZER0 FS11 FS10 FS9 FS8
     spiwrite(0x34,3);//34 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0xA0,3);//80 FS7    FS6 FS5 FS4  FS3  FS2  FS1 FS0
     spiwrite(0x14,3);//14 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0x20,3);//20 MD2    MD1 MD0 G2   G1   G0   BO  FSYNC
	 delayms(20);

	 spiwrite(0xff,4); 
     spiwrite(0xff,4);//向AD7714写入32位1 软件复位AD7714
     spiwrite(0xff,4);//
     spiwrite(0xff,4);// 
     delayms(100);    //延时100ms
     spiwrite(0x24,4);//24 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0 
     spiwrite(0x4F,4);//41 B/U    WL  BST ZER0 FS11 FS10 FS9 FS8
     spiwrite(0x34,4);//34 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0xA0,4);//80 FS7    FS6 FS5 FS4  FS3  FS2  FS1 FS0
     spiwrite(0x14,4);//14 0/DRDY RS2 RS1 RS0  R/W  CH2  CH1 CH0
     spiwrite(0x20,4);//20 MD2    MD1 MD0 G2   G1   G0   BO  FSYNC
	 delayms(20);
}

float x,y;
void read7714tempratuer(unsigned char channel)
{
	unsigned long resu;
	unsigned long temperature;
	unsigned char flag;
	unsigned char temp;

	spiwrite(0x0C,channel);
	temp=spiread(channel);

	if(!(temp&0x80))
	{
		spiwrite(0x5C,channel);
		temperature=spiread(channel); 
		temperature<<=8;
		temperature|=spiread(channel);
		temperature<<=8;
		temperature|=spiread(channel);
		resu=temperature;
		if(resu>=0x800000)
		{
			temperature = (resu-0x800000);
			flag = 0;
		}//温度为正
		else 
		{
			temperature = (0x800000-resu);
			flag = 1;
		}//温度为负
		temperature = temperature/10+5;
		
		//////////////////////////////////////////////////
		temperature_value[channel] 	= temperature;
		temperature_flag[channel]		= flag;
		
		fDeviceTemperature[channel] = (flag==1)?temperature_value[channel]*(-1.0):temperature_value[channel];
		fDeviceTemperature[channel] *= 0.01f;
		
		if(channel == DEVICEP1STREAMTEMP){
			if((bAllowFunctionFit == 1) &&  (fDeviceTemperature[channel] > 100)){//允许温度调整；在温度小于100摄氏度，不用调整
				x = fDeviceTemperature[channel];
				y = k2 * x * x + k1 * x + k0;
				
				fDeviceTemperature[channel] = y;
			}
			
			fDeviceTemperature[channel] += fStreamCorrect;			
		}
	}			
} 