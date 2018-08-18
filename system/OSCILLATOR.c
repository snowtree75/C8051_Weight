#include "includes.h"
#include "OSCILLATOR.h"

/**********************************************************
  时钟配置程序  外部晶振22.1184M
**********************************************************/
void OscillatorInit()
{
	INT16U i = 0;
	INT8U SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = CONFIG_PAGE;     // set SFR page
	OSCXCN    = 0x67;
	for (i = 0; i < 3000; i++);//Wait 1ms for initialization
	while ((OSCXCN & 0x80) == 0);
	CLKSEL    = 0x01;
	WDTCN     = 0xDE;            //Disable watchdog timer
	WDTCN     = 0xAD;
	SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page   
}