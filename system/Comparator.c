#include "includes.h"
#include "config.h"
#include "Comparator.h"
#include "DeviceStatus.h"
#include "miscellaneous.h"
#include "uart.h"
#include "ADC7714.h"

/**********************************************************
  硬件比较器配置程序
**********************************************************/
void Comparator01Init()
{
	unsigned int i = 0;
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = CPT0_PAGE;
	CPT0CN    = 0x80;
	for (i = 0; i < 60; i++);  // Wait 20us for initialization
	CPT0CN    &= ~0x30;
	CPT0MD    = 0x22;
	
	SFRPAGE   = CPT1_PAGE;
	CPT1CN    = 0x80;
	for (i = 0; i < 60; i++);  // Wait 20us for initialization
	CPT1CN    &= ~0x30;
	CPT1MD    = 0x22;
	SFRPAGE = SFRPAGE_SAVE; 
}

/**********************************************************
   比较器0中断子程序 
**********************************************************/
void COMP0ISR(void) interrupt  11{//比较器0上升沿中断
	unsigned char  saveSFRPAGE=SFRPAGE;
	SFRPAGE = CPT0_PAGE;
	if(CP0FIF){
		CP0RIF=0;
		CP0FIF=0;
		//buzzer();			 

	}
	SFRPAGE=saveSFRPAGE;
 }

int i;
 void COMP1ISR(void) interrupt  13{
	 unsigned char  saveSFRPAGE = SFRPAGE;
	SFRPAGE = CPT1_PAGE;
	if(CP1FIF){
		CP1RIF=0;
		CP1FIF=0;
		buzzer();
		if((ucCurDeviceStatus == FIRST300SECONDHEATING || ucCurDeviceStatus == F300SECOND2FIRSTPOINT) && allowFirstPoint){
			bFirstPoint = 1;
		}
	}
	SFRPAGE = saveSFRPAGE;
 }
 
void COMP1ISR1(void){//比较器1上升沿中断 P1管 初始点红外检测
	unsigned char  saveSFRPAGE=SFRPAGE;
	SFRPAGE = CPT1_PAGE;
	if(CP1FIF){

		CP1RIF=0;
		CP1FIF=0;

	}
	SFRPAGE=saveSFRPAGE;
 }