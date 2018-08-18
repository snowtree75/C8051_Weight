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
				fDeviceAtm = ulDeviceAtm * 0.1f;	
				///////////////////////////////////
				ulDeviceTempture = SMB_DATA_IN[2];	
				ulDeviceTempture <<= 6;
				ulDeviceTempture += (SMB_DATA_IN[3] >> 2);
				ulDeviceTempture *= 16500;
				ulDeviceTempture >>= 14;
				ulDeviceTempture-=4000;
				ulDeviceTempture /=10;
				fDeviceTempture = ulDeviceTempture * 0.1f;
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
