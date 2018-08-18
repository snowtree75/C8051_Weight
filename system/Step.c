#include "includes.h"
#include "miscellaneous.h"
#include "Step.h"
#include "Config.h"

void MeasureMotorMove(unsigned char direction,unsigned int step){
	DIR0 = (direction==0)? UP : DOWN;
	do{	     
		wait(800);
		PULSE0=!PULSE0;
	}while(step--);		 	
}

void ResetMotor(){
	if(!TRACING0reset){
		do{
			MeasureMotorMove(UP,1);			
		}while(!TRACING0reset);	
	}else{
		do{
			MeasureMotorMove(DOWN,1);			
		}while(TRACING0reset);		
	}	
}