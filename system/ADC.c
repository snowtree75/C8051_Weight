#include "includes.h"
#include "config.h"
#include "ADC.h"
#include "DeviceStatus.h"
#include "miscellaneous.h"

void ADC0Init(void){
	AMX0CF    = 0x00;     //全为单端入
	ADC0CF    = 0xf8;     //时钟32分频,增益为1
	ADC0CN    = 0x90;     //BIT7=1 开始ADC0 B4=AD0BUSY=1 启动转换
	AMX0SL		=	2;
}

void ADC0ISR (void) interrupt 15
{
	static unsigned int_dec=INT_DEC;    // integrate/decimate counter
	static long accumulator0=0L; 
	static long accumulator1=0L;
	static long accumulator2=0L;
	static long accumulator3=0L;
	unsigned char  saveSFRPAGE = SFRPAGE;
	SFRPAGE = ADC0_PAGE;
	
	AD0INT = 0;						   // clear ADC conversion complete
	//AD0BUSY=1 ;                                // indicator
	if(AMX0SL==0){accumulator0 += ADC0;}
	if(AMX0SL==1){accumulator1 += ADC0;} 
	if(AMX0SL==2){accumulator2 += ADC0;}
	if(AMX0SL==3){accumulator3 += ADC0;} 

	int_dec--;                          // update decimation counter
	if (int_dec == 0) 
	{ 
		int_dec = INT_DEC; 
		switch (AMX0SL)
		{
			case   0: 
				AMX0SL=1;
				uiDeviceP2FirstVolumnDianWei=accumulator0>>2;

				accumulator0 = 0L;  
				break;

			case   1: 
				AMX0SL=2;
				uiDeviceP1FirstVolumnDianWei=accumulator1>>2;	                     

				accumulator1 = 0L;  
				break;

			case   2: 
				AMX0SL=3;
				uiDeviceP2FirstPointDianWei=accumulator2>>2; 

				accumulator2 = 0L;  
				break;

			case   3: 
				AMX0SL=0;
				uiDeviceP1FirstPointDianWei=accumulator3>>2;                       
				accumulator3 = 0L;  
				break;
		}       
	}
	AD0BUSY = 1;  
	SFRPAGE = saveSFRPAGE;
}