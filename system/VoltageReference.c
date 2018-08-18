#include "includes.h"
#include "VoltageReference.h"

void VoltageReferenceInit()
{
	unsigned char SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = ADC0_PAGE;       // set SFR page
	REF0CN    = 0x03;     //�ڲ�2.43V�ο�
	SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page
}