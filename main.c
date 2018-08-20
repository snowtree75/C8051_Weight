#include "system/includes.h"


void obtainMeasurement(void){
	getPressData();
	//acquisition4DW();
	read7714tempratuer(DEVICEP1STREAMTEMP);
	read7714tempratuer(DEVICEDENSORTEMP);
	read7714tempratuer(NECKTEMP);
}

int zeroCorrect(){
	sendcom0(0x54);//����ƽ����'T',ȥƤ
	sendcom0(0x0D);//CR
	sendcom0(0x0A);//LF
	
	return SUCCESS;
}

int volumnCorrect(){
	fTotalWeight = fCurWeight;
	return SUCCESS;
}

/********************************************
	���ƣ�TaskOneSecond
	���ܣ���Ҫ�����û��趨�ļ���¯���ʣ����Ƽ���¯
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
				//uiCurPower = uiPower3; //���ʵ�������
				uiCurPower = uiTempPower;//���ʵ���
			break;
			
			case VOLUMN96PER2LASTPOINT:
				uiCurPower = uiPower4;
			break;			
		}
		
		
		uiPowerCnt++;
		uiPowerCnt %= uiPowerPeriod;
		//uiPowerCnt = (uiPowerCnt++) % uiPowerPeriod;//uiPowerPeriod = 100

		if(uiPowerCnt <= uiCurPower){
			FURNACE0 = 0;//�򿪼���¯��
		}else{
			FURNACE0 = 1;//�رռ���¯��
		}
	}else{
		uiCurPower = 0;
		FURNACE0 = 1;//�رռ���¯��
	}
}

/********************************************
	���ƣ�TaskControlCryostat
	���ܣ���ҪĿ����Ϊ������ԡ��ʹ����ԡ����Ҫ��
*********************************************/
void TaskControlCryostat()
{
	if(cryostatWorking){

		read7714tempratuer(DEVICEDENSORTEMP);
		if(		 (ucCurDeviceStatus == READY || ucCurDeviceStatus == CABLIRATEZEROPOINT || ucCurDeviceStatus == CABLIRATEVOLUMN)//�����������Ҫ�������ȣ���ԡ����������Ҫ��Ļ���ֱ�ӿ�ʼ����
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
					if(fDeviceTemperature[DEVICEDENSORTEMP] > iCryostat75per ){ //���þ��Է�Χ
							COMPRESSOR = 0;
							waterHEATER = 1;
							if(ucCurDeviceStatus == READY || ucCurDeviceStatus == CABLIRATEZEROPOINT || ucCurDeviceStatus == CABLIRATEVOLUMN){
								ucCurDeviceStatus = CRYOSTATING;
								ulCountTime = 0;
							}
					}
					
					if(fDeviceTemperature[DEVICEDENSORTEMP] < iCryostat25per ){
						waterHEATER = 0;
						COMPRESSOR = 1;
						if(ucCurDeviceStatus == READY || ucCurDeviceStatus == CABLIRATEZEROPOINT || ucCurDeviceStatus == CABLIRATEVOLUMN){
							ucCurDeviceStatus = CRYOSTATING;
							ulCountTime = 0;
						}		
					}
					
			}
		} else{
			COMPRESSOR = 1;
			waterHEATER = 1;
		}
}
/********************************************
	���ƣ�neckControl
	���ܣ���ҪĿ����Ϊ����
*********************************************/
void TaskControlNeck(void)
{ 
	// shaopingresetjudge_pin = 1 ��ʾ��Ƭû���ڵ�
	// shaoping_mor_PIN = 0 ��ʾ����
	
	/*if(ucCurDeviceStatus >= FIRST300SECONDHEATING && ucCurDeviceStatus <= FIRSTPOINT2VOLUMN96PER)
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

	if(ucCurDeviceStatus == READY){
		if(shaopingresetjudge_pin){
			SHAOPING_OPEN = 1;//open shaoping jia
			shaopingheat_PIN = 1;// stop heat		
			shaoping_mor_PIN = 0;//open motor
		}
		else	 //end
		{
			shaoping_mor_PIN = 1;// close motor
			SHAOPING_OPEN = 0; // 
		}
	}
	*/
	
	if(fDeviceTemperature[NECKTEMP] > fJingbuTemp){
		SHAOPING_OPEN = 0;//���ô��ź�
	}
	
	if(!SHAOPING_OPEN){//û�д򿪵Ļ���
		
		if(shaopingresetjudge_pin){
			shaoping_mor_PIN = 0;//�����
		}else{
			shaoping_mor_PIN = 1;// �ر����
			SHAOPING_OPEN = 1;//����Ϊ��
			shaopingheat_PIN = 1;//�ر���ƿ��������
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
						buzzer();
						sendcom1computer_float(CFPT,fFirstPontTemp); //���ͳ����
						sendcom1computer_float(CFPD,ulCountTime); //���ͳ����delay	
						bSendFirstPoint = 1;						
					}
				}else{
					if(ulCountTime >= iFirst300Delay){
						ucCurDeviceStatus = F300SECOND2FIRSTPOINT;
						buzzer();
					}
				}
				break;
			
			case F300SECOND2FIRSTPOINT:
				if(bFirstPoint){
					ucCurDeviceStatus = FIRSTPOINT2VOLUMN96PER;
					buzzer();
					bFirstPoint = 0;
					fFirstPontTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					iFirstPointDelay = ulCountTime;
				}
				break;
				
			case FIRSTPOINT2VOLUMN96PER:
				if(fCurWeightPer > iVolumnThresholdForLastpoint){//iVolumnThresholdForLastpoint
					ucCurDeviceStatus = VOLUMN96PER2LASTPOINT;
					buzzer();
					ulCountTime = 0;
					fMaxTemp = -1;
				}else{
					if(fDeviceTemperature[DEVICEP1STREAMTEMP] >= iMaxStreamTemperature){			// to max stream,test stop			
						currentCommand = SSTC;
						buzzer();
						ulCountTime = 0;
						ucCurDeviceStatus = DEVICECOOLING;//
					}
					
					if(fMaxTemp < fDeviceTemperature[DEVICEP1STREAMTEMP]){
						fMaxTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					}
					
					if(fMaxTemp > fDeviceTemperature[DEVICEP1STREAMTEMP] + iLastPointFindThreshold){// down 20C, test stop
							bLastPoint = 1;
						
							fLastPointTemp = fMaxTemp;
							if(!bSendLastPoint){
								sendcom1computer_float(CLPT,fLastPointTemp); //����zhong���
								bSendLastPoint = 1;						
							}
							furnanceWorking = 0;
							cryostatWorking = 0;
							ulCountTime = 0;
							ucCurDeviceStatus = DEVICECOOLING;
							buzzer();
					}
				}
				break;
				
			case VOLUMN96PER2LASTPOINT:
				if(!bLastPoint){
					if(fMaxTemp < fDeviceTemperature[DEVICEP1STREAMTEMP]){
						fMaxTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					}
													
					if(	ulCountTime >= iLastPointDelayThreashold 
						|| fDeviceTemperature[DEVICEP1STREAMTEMP] >= iMaxStreamTemperature){
						bLastPoint = 1;
						
						fLastPointTemp = fMaxTemp;
						if(!bSendLastPoint){
							sendcom1computer_float(CLPT,fLastPointTemp); //����zhong���
							bSendLastPoint = 1;						
						}
						furnanceWorking = 0;
						cryostatWorking = 0;
						ulCountTime = 0;
						ucCurDeviceStatus = DEVICECOOLING;
						buzzer();
					}
				}
				break;
				
			case DEVICECOOLING:
				if(ulCountTime >= iCoolingDelay){
						InitDeviceStatus();
						buzzer();
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

			if(fDeviceTemperature[DEVICEP1STREAMTEMP] <= 300){//300���϶ȣ����ձ�
				fCurPurePervious100Weight = fCurWeight - fFlask1Weight;		
				fCurPurePervious100Weight = fCurPurePervious100Weight <0 ? 0 : fCurPurePervious100Weight;		
				fCurPureWeight = fCurPurePervious100Weight;
				fCurWeightPer = 100 * fCurPurePervious100Weight / fTotalWeight ;
			}else{	
				fCurPureNext100Weight = fCurWeight - fFlask2Weight;	
				fCurPureNext100Weight = fCurPureNext100Weight <0 ? 0 : fCurPureNext100Weight;	
				fCurPureWeight = fCurPurePervious100Weight + fCurPureNext100Weight;//����180ǰ��
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
	uiPower1							= (unsigned int)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	uiPower2							= (unsigned int)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	uiPower3 							= (unsigned int)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	uiPower4 							= (unsigned int)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iCryostatLow 					= (unsigned int)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iCryostatHigh 				= (unsigned int)(int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fStreamCorrect 				= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fAtmCorrect 					= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fJingbuTemp						= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	fTotalWeight				  = byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	
	iVolumnThresholdForLastpoint 						= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iLastPointFindThreshold 								= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iLastPointDelayThreashold								= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iMaxStreamTemperature				  					= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iDryPointDelay 													= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	iVolumnDelay 														= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	
	fRetrieveVolecity												= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);
	
	// �����ʵ���ʹ��
	uiTempPower						= uiPower3;
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
				currentCommand = IDLE;
				ucCurDeviceStatus = VOLUMN96PER2LASTPOINT;
				ulCountTime = iLastPointDelayThreashold ;
			break;
		
		case SRST : // reset device
				buzzer();
				WDTCN = 0xA5;
			break;
		
		case SFW1 : // flask1 weight				
				fFlask1Weight = fCurWeight;
				fFlask2Weight = fFlask1Weight;
				buzzer();
				currentCommand = IDLE;
			break;
		
		case SFW2 : // flask2 weight				
				fFlask2Weight = fCurWeight;
				currentCommand = IDLE;
			break;
	
		case SVSC : // volumn step count			
				currentCommand = IDLE;
				buzzer();
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
	InitDeviceStatus();
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