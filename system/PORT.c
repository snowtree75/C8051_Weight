#include "includes.h"
#include "PORT.h"

void PortIOInit() 
{
	INT8U SFRPAGE_SAVE = SFRPAGE; //Save current SFR page

	SFRPAGE   = CONFIG_PAGE;
	XBR0      = 0x07;
	XBR1      = 0x04;
	XBR2      = 0x44;

	P0MDOUT   = 0x14;
	P1MDOUT   = 0xFC;
	P2MDOUT   = 0xFF;
	P3MDOUT   = 0x3F;
	P4MDOUT   = 0x13; 
	P7MDOUT   = 0x7e; 
	P6MDOUT   = 0x7c;     
	EX0=1;
	SFRPAGE = SFRPAGE_SAVE;    // Restore SFR page
}