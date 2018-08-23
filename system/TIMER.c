#include "includes.h"
#include "config.h"
#include "TIMER.h"
#include "miscellaneous.h"

/**********************************************************
  模块：全部定时器初始化程序
	作者：金雪松
	日期：July 21 2015
	参数：void
	返回值：void
	功能：利用一个函数初始化化所有定时器
**********************************************************/
void TimersInit(){
	Timer01Init();
	Timer2Init();
	Timer3Init();
	Timer4Init();
}
/**********************************************************
  模块：定时器0初始化程序
	作者：金雪松
	日期：July 21 2015
	参数：void
	返回值：void
	功能：核心定时器，用作uCOS-II操作系统的基础时钟TICK 
				在系统时钟为22.1184MHz情况下，TH0=70 TL0=00对应的
				Tick为50次/秒(即0.02秒/次)，在OS_CPU_A.ASM中
				定义OS_TICKS_PER_SEC为50
**********************************************************/
void Timer01Init()
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE = TIMER01_PAGE;      //set SFR page
	CKCON = 0x10;   // 时钟控制寄存器，CKCON，SCA1~0 定时器01预分频位。00时为12分频
									// T0M 位为0时，定时器使用预分频位定义的时钟
									// T1M 位为1时，定时器使用系统时钟
	
	TMOD = 0x21;    // 定时器0工作在方式1下，即16为手工装填方式；
									// 定时器1工作在方式2下，即8位自动装填方式，为UART0提供波特率
	TCON = 0x05;    // 定时器0和1都没有启动，在第一个任务中启动 
	ET0  = 1;
	
	TH0 = 0x70;     // Timer 0 High Byte
	TL0 = 0x00;     // Timer 0 Low Byte

	TH1 = Baute57600;
	TL1 = Baute57600;
	
	TR1	= 1; 
	
	SFRPAGE = SFRPAGE_SAVE;     
} 
/**********************************************************
  模块：定时器3初始化程序
	作者：金雪松
	日期：July 21 2015
	参数：void
	返回值：void
	功能：
**********************************************************/
void Timer2Init()
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = TMR2_PAGE;       // set SFR page    
	TMR2CN    = 0x00;
	TMR2CF    = 0x00;
	//RCAP2H=0xFD;  // 
	RCAP2H=0xB8;          
	RCAP2L=0x00;  
	ET2=1;
	TR2=1;
	SFRPAGE = SFRPAGE_SAVE;      // Restore SFR page
}

/**********************************************************
  模块：定时器3初始化程序
	作者：金雪松
	日期：July 21 2015
	参数：void
	返回值：void
	功能：为UART0提供波特率，[TMR3H:TMR3L]=0xFB80，对应的波特率为1200bps
**********************************************************/
void Timer3Init() //UART0_T3 波特率
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = TMR3_PAGE;       // set SFR page
	TMR3CN    = 0x00;
	TMR3CF    = 0x08; // 定时器时钟选择位 TnM1、TnM0 = 01 代表使用系统时钟
	
	/*RCAP3L    = 0xE8;
	RCAP3H    = 0xFF;
	TMR3L     = 0xE8;
	TMR3H     = 0xFF;  //57600bps  (0x0018)
	*/

	///////////////////////////////////////////////////////////
	RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  
	// 金雪松该，FB80 对应的波特率位 1200，与天平通信  (0x0480)
	////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////
	/*RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  */
	// 金雪松该，FDC0 对应的波特率位 2400，与天平通信  (0x0240)
	////////////////////////////////////////////////////////////
	
		///////////////////////////////////////////////////////////
	/*RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  */
	// 金雪松该，FEE0 对应的波特率位 4800，与天平通信  (0x0120)
	////////////////////////////////////////////////////////////
	
		///////////////////////////////////////////////////////////
	/*RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  */
	// 金雪松该，FF70 对应的波特率位 9600，与天平通信  (0x0090)
	////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////
	/*RCAP3L    = 0xB8;
	RCAP3H    = 0xFF;
	TMR3L     = 0xB8;
	TMR3H     = 0xFF;*/  
	// 金雪松该，FFB8 对应的波特率位 19200，与天平通信  (0x0048)
	////////////////////////////////////////////////////////////
	
	TR3       = 1;
	SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page
}
/**********************************************************
  模块：定时器3初始化程序
	作者：金雪松
	日期：July 21 2015
	参数：void
	返回值：void
	功能：
**********************************************************/
void Timer4Init()
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = TMR4_PAGE;       // set SFR page
	TMR4CN    = 0x00;
	TMR4CF    = 0x00;

	RCAP4H    = 0xFD;
	RCAP4L    = 0x00; 
	EIE2 			|= 0x04;       //ET4 = 1;
	TR4       = 1;	

	SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page
}

int iTempPower;
void TMR4_ISR(void) interrupt  16
{   
	BYTE  saveSFRPAGE=SFRPAGE;
	SFRPAGE = TMR4_PAGE;
	TF4 = 0;
	//uiSendTimeOut++;
	
	if(count++ == 2671){
		count = 0;
		ulCountTime++;
		//buzzer();
		
		// 回收速度空值，一秒钟调整一次
		
		if(ucCurDeviceStatus >= FIRST300SECONDHEATING && ucCurDeviceStatus <= FIRSTPOINT2VOLUMN96PER){
				if(fCurPureWeight > 5.0){//当前回收超过5ml，开始调整功率
						ucPowerChangePeriod++;
						if(ucPowerChangePeriod >= 5){//功率的调整周期为 5s调整一次
							ucPowerChangePeriod = 0;//
							
							//1计算回收速度
							fCurVelocity = (fCurPureWeight - fPrePureWeight) * 12;			//当前速度						
							fCurError = (fCurVelocity - fRetrieveVolecity);//速度误差,默认标准回收4.5g/min
							
							fCurPower += (Fuzzy_Control(fCurError,(fCurError - fPreError)));		
							if(fCurPower < 0){
								iTempPower = 0;
								fCurPower = 0;
							}else if(fCurPower > 100){
								iTempPower = 100;
								fCurPower = 100;
							}else{
								iTempPower = (int)floor(fCurPower);
							}
							
							uiTempPower = iTempPower;												
							fPrePureWeight = fCurPureWeight;
							fPreError = fCurError;
						}
				}
				
			}
		
		// for balance on & off
		second++;//一秒
		if(tianping_status && (second==5)){//5秒进入一次
			tianping_status = 0;
			second = 0;			
		}
		
	}
	
	SFRPAGE = saveSFRPAGE;
}
