#include "includes.h"
#include "config.h"
#include "TIMER.h"
#include "miscellaneous.h"

/**********************************************************
  ģ�飺ȫ����ʱ����ʼ������
	���ߣ���ѩ��
	���ڣ�July 21 2015
	������void
	����ֵ��void
	���ܣ�����һ��������ʼ�������ж�ʱ��
**********************************************************/
void TimersInit(){
	Timer01Init();
	Timer2Init();
	Timer3Init();
	Timer4Init();
}
/**********************************************************
  ģ�飺��ʱ��0��ʼ������
	���ߣ���ѩ��
	���ڣ�July 21 2015
	������void
	����ֵ��void
	���ܣ����Ķ�ʱ��������uCOS-II����ϵͳ�Ļ���ʱ��TICK 
				��ϵͳʱ��Ϊ22.1184MHz����£�TH0=70 TL0=00��Ӧ��
				TickΪ50��/��(��0.02��/��)����OS_CPU_A.ASM��
				����OS_TICKS_PER_SECΪ50
**********************************************************/
void Timer01Init()
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE = TIMER01_PAGE;      //set SFR page
	CKCON = 0x10;   // ʱ�ӿ��ƼĴ�����CKCON��SCA1~0 ��ʱ��01Ԥ��Ƶλ��00ʱΪ12��Ƶ
									// T0M λΪ0ʱ����ʱ��ʹ��Ԥ��Ƶλ�����ʱ��
									// T1M λΪ1ʱ����ʱ��ʹ��ϵͳʱ��
	
	TMOD = 0x21;    // ��ʱ��0�����ڷ�ʽ1�£���16Ϊ�ֹ�װ�ʽ��
									// ��ʱ��1�����ڷ�ʽ2�£���8λ�Զ�װ�ʽ��ΪUART0�ṩ������
	TCON = 0x05;    // ��ʱ��0��1��û���������ڵ�һ������������ 
	ET0  = 1;
	
	TH0 = 0x70;     // Timer 0 High Byte
	TL0 = 0x00;     // Timer 0 Low Byte

	TH1 = Baute57600;
	TL1 = Baute57600;
	
	TR1	= 1; 
	
	SFRPAGE = SFRPAGE_SAVE;     
} 
/**********************************************************
  ģ�飺��ʱ��3��ʼ������
	���ߣ���ѩ��
	���ڣ�July 21 2015
	������void
	����ֵ��void
	���ܣ�
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
  ģ�飺��ʱ��3��ʼ������
	���ߣ���ѩ��
	���ڣ�July 21 2015
	������void
	����ֵ��void
	���ܣ�ΪUART0�ṩ�����ʣ�[TMR3H:TMR3L]=0xFB80����Ӧ�Ĳ�����Ϊ1200bps
**********************************************************/
void Timer3Init() //UART0_T3 ������
{
	BYTE SFRPAGE_SAVE = SFRPAGE; //Save current SFR page
	SFRPAGE   = TMR3_PAGE;       // set SFR page
	TMR3CN    = 0x00;
	TMR3CF    = 0x08; // ��ʱ��ʱ��ѡ��λ TnM1��TnM0 = 01 ����ʹ��ϵͳʱ��
	
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
	// ��ѩ�ɸã�FB80 ��Ӧ�Ĳ�����λ 1200������ƽͨ��  (0x0480)
	////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////
	/*RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  */
	// ��ѩ�ɸã�FDC0 ��Ӧ�Ĳ�����λ 2400������ƽͨ��  (0x0240)
	////////////////////////////////////////////////////////////
	
		///////////////////////////////////////////////////////////
	/*RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  */
	// ��ѩ�ɸã�FEE0 ��Ӧ�Ĳ�����λ 4800������ƽͨ��  (0x0120)
	////////////////////////////////////////////////////////////
	
		///////////////////////////////////////////////////////////
	/*RCAP3L    = 0x80;
	RCAP3H    = 0xFB;
	TMR3L     = 0x80;
	TMR3H     = 0xFB;  */
	// ��ѩ�ɸã�FF70 ��Ӧ�Ĳ�����λ 9600������ƽͨ��  (0x0090)
	////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////
	/*RCAP3L    = 0xB8;
	RCAP3H    = 0xFF;
	TMR3L     = 0xB8;
	TMR3H     = 0xFF;*/  
	// ��ѩ�ɸã�FFB8 ��Ӧ�Ĳ�����λ 19200������ƽͨ��  (0x0048)
	////////////////////////////////////////////////////////////
	
	TR3       = 1;
	SFRPAGE = SFRPAGE_SAVE;     // Restore SFR page
}
/**********************************************************
  ģ�飺��ʱ��3��ʼ������
	���ߣ���ѩ��
	���ڣ�July 21 2015
	������void
	����ֵ��void
	���ܣ�
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
		
		// �����ٶȿ�ֵ��һ���ӵ���һ��
		
		if(ucCurDeviceStatus >= FIRST300SECONDHEATING && ucCurDeviceStatus <= FIRSTPOINT2VOLUMN96PER){
				if(fCurPureWeight > 5.0){//��ǰ���ճ���5ml����ʼ��������
						ucPowerChangePeriod++;
						if(ucPowerChangePeriod >= 5){//���ʵĵ�������Ϊ 5s����һ��
							ucPowerChangePeriod = 0;//
							
							//1��������ٶ�
							fCurVelocity = (fCurPureWeight - fPrePureWeight) * 12;			//��ǰ�ٶ�						
							fCurError = (fCurVelocity - fRetrieveVolecity);//�ٶ����,Ĭ�ϱ�׼����4.5g/min
							
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
		second++;//һ��
		if(tianping_status && (second==5)){//5�����һ��
			tianping_status = 0;
			second = 0;			
		}
		
	}
	
	SFRPAGE = saveSFRPAGE;
}
