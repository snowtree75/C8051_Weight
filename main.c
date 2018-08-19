#include "system/includes.h"

unsigned int uiTempPower;//临时功率变量，调整功率用

void obtainMeasurement(void){
	getPressData();
	//acquisition4DW();
	read7714tempratuer(DEVICEP1STREAMTEMP);
	read7714tempratuer(DEVICEDENSORTEMP);
	read7714tempratuer(NECKTEMP);
}

int zeroCorrect(){
	sendcom0(0x54);//给天平发送'T',去皮
	sendcom0(0x0D);//CR
	sendcom0(0x0A);//LF
	
	return SUCCESS;
}

int volumnCorrect(){
	fTotalWeight = fCurWeight;
	return SUCCESS;
}

/********************************************
	名称：TaskOneSecond
	功能：主要根据用户设定的加热炉功率，控制加热炉
*********************************************/
void TaskControlFurnace()
{
	if(furnanceWorking){
		
		switch(ucCurDeviceStatus){
			case FIRST300SECONDHEATING:
				uiCurPower = uiPower1;
			break;
			
			case F300SECOND2FIRSTPOINT:
				uiCurPower = uiPower2;
			break;
			
			case FIRSTPOINT2VOLUMN96PER:
				uiCurPower = uiPower3; //功率调整前
				//uiCurPower = uiTempPower;//功率调整后
			break;
			
			case VOLUMN96PER2LASTPOINT:
				uiCurPower = uiPower4;
			break;			
		}
		
		
		uiPowerCnt++;
		uiPowerCnt %= uiPowerPeriod;
		//uiPowerCnt = (uiPowerCnt++) % uiPowerPeriod;//uiPowerPeriod = 100

		if(uiPowerCnt <= uiCurPower){
			FURNACE0 = 0;//打开加热炉子
		}else{
			FURNACE0 = 1;//关闭加热炉子
		}
	}    
}

/********************************************
	名称：TaskControlCryostat
	功能：主要目的是为控制冷浴，使得冷浴符合要求
*********************************************/
void TaskControlCryostat()
{
	if(cryostatWorking){

		read7714tempratuer(DEVICEDENSORTEMP);
		if(		 ucCurDeviceStatus == READY //如果仪器不需要制冷或加热，冷浴正常就满足要求的话，直接开始工作
				&& fDeviceTemperature[DEVICEDENSORTEMP] < iCryostat75per 
				&& fDeviceTemperature[DEVICEDENSORTEMP] > iCryostat25per){
				ucCurDeviceStatus = FIRST300SECONDHEATING;
				furnanceWorking = 1;
				ulCountTime = 0;
		}
		
		if(fDeviceTemperature[DEVICEDENSORTEMP] > iCryostat25per 
				&& fDeviceTemperature[DEVICEDENSORTEMP] < iCryostat75per){
						COMPRESSOR = 1;
						waterHEATER = 1;
						if(ucCurDeviceStatus == CRYOSTATING){
							ucCurDeviceStatus = FIRST300SECONDHEATING;
							furnanceWorking = 1;
							ulCountTime = 0;
						}				
				}else{
					if(fDeviceTemperature[DEVICEDENSORTEMP] > iCryostat75per ){ //利用绝对范围
							COMPRESSOR = 0;
							if(ucCurDeviceStatus == READY){
								ucCurDeviceStatus = CRYOSTATING;
							}
					}
					
					if(fDeviceTemperature[DEVICEDENSORTEMP] < iCryostat25per ){
						waterHEATER = 0;
						if(ucCurDeviceStatus == READY){
							ucCurDeviceStatus = CRYOSTATING;
						}		
					}
					
			}
	}    
}
/********************************************
	名称：neckControl
	功能：主要目的是为控制
*********************************************/
void TaskControlNeck(void)
{ 
	if(ucCurDeviceStatus >= FIRST300SECONDHEATING && ucCurDeviceStatus <= FIRSTPOINT2VOLUMN96PER)
	{	
	
		if(!shaopingresetjudge_pin)		 //end
		{
			shaoping_mor_PIN = 1;// close motor
			SHAOPING_OPEN = 0; // 
		}
		
		
		if(shaopingresetjudge_pin&&!SHAOPING_OPEN){// 
				if(fDeviceTemperature[NECKTEMP] < fJingbuTemp) {
					shaopingheat_PIN = 0;
				}else{
					SHAOPING_OPEN = 1;//open shaoping jia
					shaopingheat_PIN = 1;// stop heat		
					shaoping_mor_PIN = 0;//open motor					
				}
		} 
		else{
			shaopingheat_PIN = 1 ;
		}
	}	   	   	   
}


void idleTask(void){
		switch(ucCurDeviceStatus){
			case FIRST300SECONDHEATING:
				if(bFirstPoint){
					ucCurDeviceStatus = FIRSTPOINT2VOLUMN96PER;
					bFirstPoint = 0;
					fFirstPontTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					iFirstPointDelay = ulCountTime;
					if(!bSendFirstPoint){
						sendcom1computer_float(CFPT,fFirstPontTemp); //发送初馏点
						sendcom1computer_float(CFPD,ulCountTime); //发送初馏点delay	
						bSendFirstPoint = 1;						
					}
				}else{
					if(ulCountTime >= iFirst300Delay){
						ucCurDeviceStatus = F300SECOND2FIRSTPOINT;
					}
				}
				break;
			
			case F300SECOND2FIRSTPOINT:
				if(bFirstPoint){
					ucCurDeviceStatus = FIRSTPOINT2VOLUMN96PER;
					bFirstPoint = 0;
					fFirstPontTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					iFirstPointDelay = ulCountTime;
				}
				break;
				
			case FIRSTPOINT2VOLUMN96PER:
				if(fCurWeightPer > volumnThresholdForLastpoint){
					ucCurDeviceStatus = VOLUMN96PER2LASTPOINT;
					ulCountTime = 0;
				}
				break;
				
			case VOLUMN96PER2LASTPOINT:
				if(!bLastPoint){
					if(fMaxTemp < fDeviceTemperature[DEVICEP1STREAMTEMP]){
						fMaxTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					}
					if(ulCountTime >= iLastPointDelayThreashold){
						ucCurDeviceStatus = DEVICECOOLING;
						fLastPointTemp = fMaxTemp;
						if(!bSendLastPoint){
							sendcom1computer_float(CLPT,fLastPointTemp); //发送zhong馏点
							bSendLastPoint = 1;						
						}
						furnanceWorking = 0;
						cryostatWorking = 0;
					}
				}
				break;
				
			case DEVICECOOLING:
				if(ulCountTime >= iCoolingDelay){
						ucCurDeviceStatus = READY;
						ulCountTime = 0;
				}
				break;
			
			default:
				break;
		}
		
		if(ucCurDeviceStatus >= FIRST300SECONDHEATING
		&& ucCurDeviceStatus <= DEVICECOOLING) 
		{
			if(fCurWeight < 0)
				fCurWeight = 0;

			if(fTotalWeight <= 0)
				fTotalWeight = 1;

			if(fDeviceTemperature[DEVICEP1STREAMTEMP] <= 300){//300摄氏度，换烧杯
				fCurPurePervious100Weight = fCurWeight - fFlask1Weight;		
				fCurPurePervious100Weight = fCurPurePervious100Weight <0 ? 0 : fCurPurePervious100Weight;		
				fCurPureWeight = fCurPurePervious100Weight;
				fCurWeightPer = 100 * fCurPurePervious100Weight / fTotalWeight ;
			}else{	
				fCurPureNext100Weight = fCurWeight - fFlask2Weight;	
				fCurPureNext100Weight = fCurPureNext100Weight <0 ? 0 : fCurPureNext100Weight;	
				fCurPureWeight = fCurPurePervious100Weight + fCurPureNext100Weight;//加上180前的
				fCurWeightPer = 100 * fCurPurePervious100Weight / fTotalWeight ;
			}
		}
}
float byteArray2Float(unsigned char* pData,int start){
	float value = 0;
	unsigned char* ptr = (unsigned char*)&value;
	ptr += 3;
	pData += start;
	
	*ptr = *pData;ptr--;pData++;
	*ptr = *pData;ptr--;pData++;
	*ptr = *pData;ptr--;pData++;
	*ptr = *pData;
	return value;
}

void parseParameter(){

	int base = 3;
	int index = 0;
	
	ucMethod 							= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	ucP1 									= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	ucP2 									= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	ucP3 									= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	ucP4 									= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	ucLow 								= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	ucHigh 								= (unsigned char)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fStreamCorrect 				= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fAtmCorrect 					= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fJingbuTemp						= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fTotalWeight				  = byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	
	iCryostatHigh 				= ucHigh;
	iCryostatLow 					= ucLow;
	uiPower1 							= ucP1;
	uiPower2 							= ucP2;
	uiPower3 							= ucP3;
	uiPower4 							= ucP4;
	updateDensor();	
}

void mainControl(void){
	
	
	switch(currentCommand){
		case SZCC : // zero correcting
				buzzer();
				if(SUCCESS == zeroCorrect()){
					currentCommand = IDLE;
					ucCurDeviceStatus = CABLIRATEZEROPOINT;
				}
			break;

		case SVCC : // volumn correcting 
				buzzer();
				if(SUCCESS == volumnCorrect()){
					currentCommand = IDLE;
					ucCurDeviceStatus = CABLIRATEVOLUMN;
				}
			break;
		
		case SSPC : // send parameter		
				buzzer();
				parseParameter();
				currentCommand = IDLE;
			break;
		
		case SSAC : // start test
				buzzer();
				cryostatWorking = 1;// start Cryostat
				currentCommand = IDLE;
			break;
		
		case SSTC : // stop test		
				buzzer();
				cryostatWorking = 0;
				furnanceWorking = 0;		
				currentCommand = IDLE;
			break;
		
		case SRST : // reset device
				buzzer();
				WDTCN = 0xA5;
				cryostatWorking = 0;
				furnanceWorking = 0;
				currentCommand = IDLE;
			break;
		
		case SFW1 : // flask1 weight
				buzzer();
				fFlask1Weight = fCurWeight;
				fFlask2Weight = fFlask1Weight;
				currentCommand = IDLE;
			break;
		
		case SFW2 : // flask2 weight
				buzzer();
				fFlask2Weight = fCurWeight;
				currentCommand = IDLE;
			break;
	
		case SVSC : // volumn step count			
				currentCommand = IDLE;
			break;
		
		case IDLE:
				idleTask();
			break;
			
		default:
			break;
	}
	
}

void sendMeasurement(void){
	sendcom1computer_float(CCTM,(int)ulCountTime);// send one second as counter
	
	sendcom1computer_float(CATM,fDeviceAtm); 
	sendcom1computer_float(CSGT,fDeviceTemperature[DEVICEP1STREAMTEMP]); 

	//***************************************************************************************
	sendcom1computer_float(CCLT,fDeviceTemperature[DEVICEDENSORTEMP]);
	sendcom1computer_float(CNKT,fDeviceTemperature[NECKTEMP]); 			

	sendcom1computer_float(CFW1,fFlask1Weight);
	sendcom1computer_float(CFW2,fFlask2Weight);

	sendcom1computer_float(CFIF,uiDeviceP1FirstPointDianWei);  

	if(ucCurDeviceStatus >= FIRST300SECONDHEATING
		&& ucCurDeviceStatus <= DEVICECOOLING ){				
		sendcom1computer_float(CRVL,fCurWeightPer);
	}else{			
		sendcom1computer_float(CRVL,fCurWeight); 
	}
	
	sendcom1computer_float(CSMW,fTotalWeight);
	sendcom1computer_float(CFPP,uiCurPower);
	//sendcom1computer_float(CDLV,vari.COMP0drops);
	sendcom1computer_float(CBOF,tianping_status);
 
	sendcom1computer_float(CSTA,ucCurDeviceStatus);// send device status
	
}

void main(void){
	Init_Device(); 
	IE_enable(); 
	InitAllDatas();
	
	for(;;){
		obtainMeasurement();
		sendMeasurement();
		
		mainControl();
		
		TaskControlNeck();
		TaskControlCryostat();
		TaskControlFurnace();
	}
	
}