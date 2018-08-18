#include "includes.h"
#include "IE.h"

void IE_enable(void){
  EIE1=0xa0;//比较器0,1上升沿中断始能1010 xxxx
	ES0=1;//////UART0中断使能
	EIE2|=0x40;//ES1=1;//////UART1中断使能
	EIE1|=0x02;//IIC中断使能
	EX0=1;
	ET0=1;
	EIE2 |= 0x02;   
  EA=1;///////总中断使能
}

/*

	EIE1=0xa0;//比较器0,1上升沿中断始能1010 xxxx
	ES0=1;//////UART0中断使能
	EIE2|=0x40;//ES1=1;//////UART1中断使能
	EIE1|=0x02;//IIC中断使能
	EA=1;///////总中断使能

*/