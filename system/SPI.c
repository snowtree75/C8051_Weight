#include "includes.h"
#include "config.h"
#include "miscellaneous.h"
#include "SPI.h"


void SPIInit()
{   
    unsigned char  SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
    SFRPAGE   = SPI0_PAGE;
    SPI0CFG   = 0x70;
    SPI0CN    = 0x01;
    SPI0CKR   = 0x15;
    SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page
}

/**********************************************************
  SPI向AD7714写一个字节程序
**********************************************************/
void spiwrite(unsigned char ch,unsigned char channel){
	unsigned char  saveSFRPAGE = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	switch(channel){
	case  0:CS1=0;break;
	case  1:CS2=0;break;
	case  2:CS3=0;break;

	case  3:CS4=0;break;     
	case  4:CS5=0;break;

	default:;}
 
  SFRPAGE=SPI0_PAGE;
	SPI0CFG=0x70;
	SPIF=0;
	WCOL=0;
	MODF=0;
	RXOVRN=0;
	SPI0DAT=ch;
	while(!SPIF);
	SPIF=0;
	SFRPAGE = CONFIG_PAGE;
    CS1=1;
	CS2=1;
	CS3=1;
CS4=1;
CS5=1; 
SFRPAGE = saveSFRPAGE;
}
/**********************************************************
  SPI从AD7714读一个字节程序
**********************************************************/
unsigned char spiread(unsigned char channel){
	unsigned char ch;
	unsigned char saveSFRPAGE = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;
	switch(channel){
		case  0:CS1=0;break;
		case  1:CS2=0;break;
		case  2:CS3=0;break;
		case  3:CS4=0;break;     
		case  4:CS5=0;break; 

		default:;
	}
	
	SFRPAGE = SPI0_PAGE;
	SPI0CFG = 0x50;
	SPIF = 0;
	WCOL = 0;
	MODF = 0;
	RXOVRN = 0;
	SPI0DAT = 0xff;	
	while(!SPIF);
	
	ch=SPI0DAT;
	SPIF=0;
	SFRPAGE = CONFIG_PAGE;
	CS1=1;
	CS2=1;
	CS3=1;
	CS4=1;
	CS5=1; 
	SFRPAGE = saveSFRPAGE;
	return (ch);
}
