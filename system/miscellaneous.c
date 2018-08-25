#include "includes.h"
#include "config.h"
#include <math.h>
#include "miscellaneous.h"
#include "SMBus.h"
#include "DeviceStatus.h"

void InitAllDatas(void);

void getPressData(void)                      //获取大气压值 
{//*
	TARGET = SLAVE_ADDR; // Target the CPS120
	
	if(CPS_Wake)
  {
     SMBusRead();
     if((SMB_DATA_IN[0]>>6)==0)
        {
				ulDeviceAtm=SMB_DATA_IN[0];
        ulDeviceAtm<<=8;
				ulDeviceAtm|=SMB_DATA_IN[1];
        ulDeviceAtm*=9000;
        ulDeviceAtm>>=14;
        ulDeviceAtm+=3000;
				ulDeviceAtm/=10;
				fDeviceAtm = ulDeviceAtm * 0.1f ;	
				///////////////////////////////////
				ulDeviceTempture = SMB_DATA_IN[2];	
				ulDeviceTempture <<= 6;
				ulDeviceTempture += (SMB_DATA_IN[3] >> 2);
				ulDeviceTempture *= 16500;
				ulDeviceTempture >>= 14;
				ulDeviceTempture-=4000;
				ulDeviceTempture /=10;
					
				buzzer();
					
				fDeviceTempture = ulDeviceTempture * 0.1f ;
       }  
  }
	else 
  {
   SMBusWrite ();
   delayms(50);//Delay_50ms();//可将此处延时加长，只有每次复位时才进入此程序。
  }
}


void delayms(unsigned int t)//延时约1ms
{ 
	unsigned int i,j;
	for(j=0;j<t;j++)
	{
		{
			for(i=0;i<1361;i++){}
		}
	}      
}

void wait(unsigned int n){//延时一个机器周期
   while(n)
		 n--;
}

void buzzer(void)
{      //板上蜂鸣器驱动
  unsigned char SFRPAGE_SAVE = SFRPAGE; //Save current SFR page   
  SFRPAGE = CONFIG_PAGE;   
  BUZZ=0;
  delayms(250);
  BUZZ=1  ;
  SFRPAGE = SFRPAGE_SAVE;
}

void ResetDevice(){
	InitAllDatas();
}


void updateDensor(){
		int Delta = iCryostatHigh - iCryostatLow;
		if(abs(Delta)<4){
			iCryostatMid = (iCryostatHigh + iCryostatLow) / 2;
			
			iCryostatLow = iCryostatMid - Delta / 2;
			iCryostatHigh = iCryostatMid + Delta / 2;
		}
		////////////////////////////////////////////////////	
		Delta = iCryostatHigh - iCryostatLow;
		iCryostatMid = (iCryostatHigh + iCryostatLow) / 2;
		iCryostat25per = iCryostatLow + Delta/4;
		iCryostat75per = iCryostatHigh - Delta/4;
}

 unsigned char generateCRC(unsigned char* ptr,int len){
	 unsigned char crc = 0x00;
	 int i;
	 for(i=0;i<len;i++,ptr++){
		 crc ^= (*ptr);
	 }
	 
	 return crc;
 }
 
 float byteArray2Float(unsigned char* pData,int start){
	float value = 0;
	unsigned char* ptr = (unsigned char*)&value;
	ptr += 3;
	pData += start;
	
	*ptr = *pData;ptr--;pData++;
	*ptr = *pData;ptr--;pData++;
	*ptr = *pData;ptr--;pData++;
	*ptr = *pData;
	return value;
}
 
double xx[4]={100.0,196.27,287.0,331.0};
double yy[4]={100.0,200.0,296.0,343.0};

void parseFunctionFitParameter(){
	//int base = 3;
	//int index = 0;
	double P[3]={0,1,0};
	/*
	xx[0]	= (float)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;	
	yy[0]	= (float)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	xx[1]	= (float)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	yy[1]	= (float)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	xx[2]	= (float)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	yy[2]	= (float)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);
	*/
			
	if(bAllowFunctionFit == 1){
		polyfit(4,xx,yy,2,P);
	}
	
	k0 = P[0];
	k1 = P[1];
	k2 = P[2];	
}