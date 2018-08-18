#include "system/includes.h"

void obtainMeasurement(void){
	//get_pressdata();
	//acquisition4DW();
}

void mainControl(void){
	/*
	switch(currentCommand){
		case SZCC : // zero corr
				
				currentCommand = IDLE;
			break;

		case SVCC : // volumn corr
			
				currentCommand = IDLE;
			break;
		
		case SSAC : // start test
				currentCommand = IDLE;
			break;
		
		case SSTC : // stop test
			
				currentCommand = IDLE;
			break;
		
		case SSPC : // send parameter
				
				currentCommand = IDLE;
			break;
		
		case SRST : // reset device
			
				currentCommand = IDLE;
			break;
		
		case SFW1 : // flask1 weight
			
				currentCommand = IDLE;
			break;
		
		case SFW2 : // flask2 weight
			
				currentCommand = IDLE;
			break;
		
		case SVSC : // volumn step count
			
				currentCommand = IDLE;
			break;
		
		default:
			break;
	}
	*/
}

void sendMeasurement(void){
	sendcom1computer_float(CCTM,ulDeviceCount);
}

void main(void){
	Init_Device(); 
	IE_enable(); 
	
	for(;;){
		obtainMeasurement();
		sendMeasurement();
		mainControl();
	}
	
}