#include "system/includes.h"
/********************************************
	名称：obtainMeasurement
	功能：利用设备中的传感器获取测量数据
*********************************************/
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
				//uiCurPower = uiPower3; //功率调不调整
				uiCurPower = uiTempPower;//功率调整
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
	}else{
		uiCurPower = 0;
		FURNACE0 = 1;//关闭加热炉子
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
		if(		 (ucCurDeviceStatus == READY || ucCurDeviceStatus == CABLIRATEZEROPOINT || ucCurDeviceStatus == CABLIRATEVOLUMN)//如果仪器不需要制冷或加热，冷浴正常就满足要求的话，直接开始工作
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
	名称：neckControl
	功能：主要目的是为控制
*********************************************/
void TaskControlNeck(void)
{ 
	// shaopingresetjudge_pin = 1 表示挡片没有遮挡
	// shaoping_mor_PIN = 0 表示开启
#ifndef FIRSTPOINTCONTROLBYNECKTEMPERATURE
	allowFirstPoint = 1;//控制初馏点，初馏点的流出是否受颈部温度的控制 1 松原没有控制，始终为 1
#endif
	
	diguanheat = 0;//滴管持续加热
	
	if(shaopingresetjudge_pin&&(fDeviceTemperature[DEVICEP1STREAMTEMP] > fJingbuTemp + 20)){//颈部温度打开的阈值为颈部设定温度在增加20度
		//并且夹子是夹上的
		SHAOPING_OPEN = 0;//设置打开信号
		shaopingheat_PIN = 1;//关闭加热
		allowFirstPoint = 1;
	}
	
	
	//控制颈部温度加热
	if(ucCurDeviceStatus >= FIRST300SECONDHEATING && ucCurDeviceStatus <= FIRSTPOINT2VOLUMN96PER){//表示实验开始后才加温
		if(shaopingresetjudge_pin){//没有打开的话，开始加热
			if((fDeviceTemperature[NECKTEMP] < fJingbuTemp - 2) && ( shaopingheat_PIN == 1)){//低于颈部温度-2，且没有加热
				shaopingheat_PIN = 0;//开始加热
			}
			
			if((fDeviceTemperature[NECKTEMP] > fJingbuTemp - 2) && ( shaopingheat_PIN == 0)){//高于颈部温度+2，且加热
				shaopingheat_PIN = 1;//停止加热
			}
		}
	}
		
		
	if(!SHAOPING_OPEN){//没有打开的话，
			
		//控制颈部加热夹马达
		if(shaopingresetjudge_pin){
			shaoping_mor_PIN = 0;//打开马达
			shaopingheat_PIN = 1;//关闭烧瓶颈部加热
		}else{
			shaoping_mor_PIN = 1;// 关闭马达
			SHAOPING_OPEN = 1;//设置为已打开
			shaopingheat_PIN = 1;//关闭烧瓶颈部加热
		}		
	}
}

/********************************************
	名称：idleTask
	功能：在没有收到上位机指令的时候完成其他业务
*********************************************/
void idleTask(void){
		switch(ucCurDeviceStatus){
			case FIRST300SECONDHEATING:
				if(bFirstPoint){
					
					bFirstPoint = 0;
					fFirstPontTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					iFirstPointDelay = ulCountTime;					
					
					if(!bSendFirstPoint){//没有发送,则发送一次
						//bRecvFirstPointACK	= 0;
						delayms(100);
						sendcom1computer_float(CFPT,fFirstPontTemp); //发送初馏点
						delayms(100);
						sendcom1computer_float(CFPD,ulCountTime); //发送初馏点delay	
						//uiSendTimeOut = 0;//初始化
						bSendFirstPoint = 1;
						buzzer();
					}
					/*else if(!bRecvFirstPointACK && (uiSendTimeOut > 600)){//没有收到且超时，则重发
						bRecvFirstPointACK	= 0;
						sendcom1computer_float(CFPT,fFirstPontTemp); //发送初馏点
						sendcom1computer_float(CFPD,ulCountTime); //发送初馏点delay	
						uiSendTimeOut = 0;//初始化
					}
					*/
					
					ucCurDeviceStatus = FIRSTPOINT2VOLUMN96PER;
				}else{
					if(ulCountTime >= iFirst300Delay){
						ucCurDeviceStatus = F300SECOND2FIRSTPOINT;
						buzzer();
					}
				}
				break;
			
			case F300SECOND2FIRSTPOINT:
				if(bFirstPoint){					
					bFirstPoint = 0;
					fFirstPontTemp = fDeviceTemperature[DEVICEP1STREAMTEMP];
					iFirstPointDelay = ulCountTime;
					
					if(!bSendFirstPoint){//没有发送,则发送一次
						//bRecvFirstPointACK	= 0;
						delayms(100);
						sendcom1computer_float(CFPT,fFirstPontTemp); //发送初馏点
						delayms(100);
						sendcom1computer_float(CFPD,ulCountTime); //发送初馏点delay	
						//uiSendTimeOut = 0;//初始化
						bSendFirstPoint = 1;
						buzzer();
					}
					
					/*else if(!bRecvFirstPointACK && (uiSendTimeOut > 600)){//没有收到且超时，则重发
						bRecvFirstPointACK	= 0;
						sendcom1computer_float(CFPT,fFirstPontTemp); //发送初馏点
						sendcom1computer_float(CFPD,ulCountTime); //发送初馏点delay	
						uiSendTimeOut = 0;//初始化
					}
					*/
					ucCurDeviceStatus = FIRSTPOINT2VOLUMN96PER;
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
								sendcom1computer_float(CLPT,fLastPointTemp); //发送zhong馏点
								//uiSendTimeOut = 0;//初始化	
								bSendLastPoint = 1;
							}
							/*else if(!bRecvLastPointACK && (uiSendTimeOut > 600)){//没有收到且超时，则重发
								bRecvLastPointACK = 0;
								sendcom1computer_float(CLPT,fLastPointTemp); //发送zhong馏点
								uiSendTimeOut = 0;//初始化	
							}
							*/
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
								sendcom1computer_float(CLPT,fLastPointTemp); //发送zhong馏点
							//	uiSendTimeOut = 0;//初始化	
								bSendLastPoint = 1;
							}
						/*else if(!bRecvLastPointACK && (uiSendTimeOut > 600)){//没有收到且超时，则重发
								bRecvLastPointACK = 0;
								sendcom1computer_float(CLPT,fLastPointTemp); //发送zhong馏点
								uiSendTimeOut = 0;//初始化	
							}*/
							
							
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
		
		//计算机回收数量
		if(ucCurDeviceStatus >= FIRST300SECONDHEATING
				&& ucCurDeviceStatus <= DEVICECOOLING) 
		{
			if(fCurWeight < 0)
				fCurWeight = 0;

			if(fTotalWeight <= 0)
				fTotalWeight = 1;

			/*
			if(fDeviceTemperature[DEVICEP1STREAMTEMP] <= 300){//300摄氏度，换烧杯
				fCurPurePervious100Weight = fCurWeight - fFlask1Weight;		
				fCurPurePervious100Weight = (fCurPurePervious100Weight < 0) ? 0 : fCurPurePervious100Weight;		
				fCurPureWeight = fCurPurePervious100Weight;				
			}else{	
				fCurPureNext100Weight = fCurWeight - fFlask2Weight;	
				fCurPureNext100Weight = (fCurPureNext100Weight < 0) ? 0 : fCurPureNext100Weight;	
				fCurPureWeight = fCurPurePervious100Weight + fCurPureNext100Weight;//加上180前的
			}
			
			fCurWeightPer = 100 * fCurPureWeight / fTotalWeight ;
			*/
			
			fCurPurePervious100Weight = fCurWeight - fFlask1Weight;		
			fCurPurePervious100Weight = (fCurPurePervious100Weight < 0) ? 0 : fCurPurePervious100Weight;		
			fCurPureWeight = fCurPurePervious100Weight;		
			fCurWeightPer = 100 * fCurPureWeight / fTotalWeight ;			
		}
}

/********************************************
	名称：parseParameter
	功能：在收到上位机发送的参数后，分析各个参数
*********************************************/
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
	iFirst300Delay													= (int)byteArray2Float(ucCom1ReceiveByte,index * 4 + base);index++;
	
	fRetrieveVolecity												= byteArray2Float(ucCom1ReceiveByte,index * 4 + base);
	if(fRetrieveVolecity < 2){
		fRetrieveVolecity = 2;
	}else if(fRetrieveVolecity > 9){
		fRetrieveVolecity = 9;
	}
		
	// 给功率调整使用
	uiTempPower						= uiPower3;
	fCurPower							= uiPower3;
	updateDensor();	
}

void mainControl(void){
	
	switch(currentCommand){
		case SZCC : // zero correcting
				buzzer();
				if(SUCCESS == zeroCorrect()){					
					ucCurDeviceStatus = CABLIRATEZEROPOINT;
					currentCommand = IDLE;
				}
			break;

		case SVCC : // volumn correcting 
				buzzer();
				if(SUCCESS == volumnCorrect()){					
					ucCurDeviceStatus = CABLIRATEVOLUMN;
					currentCommand = IDLE;
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
				ucCurDeviceStatus = VOLUMN96PER2LASTPOINT;
				ulCountTime = iLastPointDelayThreashold ;
				currentCommand = IDLE;
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
		
		case SFFP://分析温度函数拟合
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
	
	sendcom1computer_float(CATM,fDeviceAtm + fAtmCorrect); //不太合适
	sendcom1computer_float(CSGT,fDeviceTemperature[DEVICEP1STREAMTEMP]); 

	//***************************************************************************************
	sendcom1computer_float(CCLT,fDeviceTemperature[DEVICEDENSORTEMP]);
	sendcom1computer_float(CNKT,fDeviceTemperature[NECKTEMP]); 			

	sendcom1computer_float(CFW1,fFlask1Weight);
	sendcom1computer_float(CFW2,fFlask2Weight);

	sendcom1computer_float(CFIF,uiDeviceP1FirstPointDianWei);  
	sendcom1computer_float(CRLV,fCurVelocity);  	

	if(ucCurDeviceStatus >= FIRST300SECONDHEATING
		&& ucCurDeviceStatus <= DEVICECOOLING ){	
		delayms(100);			
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
	
	bAllowFunctionFit = 1;
	parseFunctionFitParameter();//计算函数拟合的系数
		
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