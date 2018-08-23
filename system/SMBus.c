#include "includes.h"
#include "SMBus.h"

#include "UART.h"

//===================================================
unsigned char 	SMB_DATA_IN[4] = {0,0,0,0};             // 存储气压和温度数组                                      
unsigned char  	SMB_DATA_NUM = 0;                   //IIC读取字节的个数                                       
unsigned char  	TARGET;                              // 目标IIC从器件地址


bit SMB_BUSY=0;                          // 软件IIC总线忙标志
bit SMB_RW=0;                            // 读写方向位 0 W  1 R
//===================================================
bit CPS_Wake=0;                           //传感器唤醒标志


//**********************************************
/**************************************************
  端口配置程序
***************************************************/
void SMBusWrite (void)
{
   unsigned char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = SMB0_PAGE;
   while (SMB_BUSY);                    // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   STA = 1;                              // Start transfer
   SFRPAGE = SFRPAGE_SAVE;           // Restore SFR page detector
}
//--------------------------------------------------------------
void SMBusRead (void)                       //读准备函数
{
   unsigned char SFRPAGE_SAVE = SFRPAGE;         // Save Current SFR page
   SFRPAGE = SMB0_PAGE;
	 
   while (SMB_BUSY);                     // Wait for bus to be free.
   SMB_BUSY = 1;                        // Claim SMBus (set to busy)
   SMB_RW = 1;                          // Mark this transfer as a READ
   STA = 1;                               // Start transfer
   while (SMB_BUSY);                     // Wait for transfer to complete
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page detector
}

//--------------硬件SMBUS配置程序//兼容IIC总线-------------------------------------
void SMBusInit()
{
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = SMB0_PAGE;
	SMB0CN    = 0x40;                  //IIC使能
	SMB0CR    = 0xE9;                  //IIC时钟频率107462Hz
	SI = 0;                               //清除中断标志
	SFRPAGE = SFRPAGE_SAVE;
}

//=========================================================================================	
void SMBusISR(void) interrupt  7{//
   bit FAIL = 0;      
   unsigned char  saveSFRPAGE=SFRPAGE; 
   SFRPAGE = SMB0_PAGE;
	 //sendcom0(SMB0STA);
   switch (SMB0STA)
   { 
      case SMB_START:                       // (MT & MR)起始条件已发出
           SMB0DAT = TARGET;              // Load address of the slave.
           SMB0DAT &= 0xFE;              
           SMB0DAT |= SMB_RW; 
					 SMB_DATA_NUM=0;              
           STA = 0;                           // Manually clear STA bit
           break;            
      case SMB_RP_START:                   // (MT & MR)重复起始条件已发出 
           SMB0DAT = TARGET;              // Load address of the slave.
           SMB0DAT &= 0xFE;              
           SMB0DAT |= SMB_RW; 
					 SMB_DATA_NUM=0;              
           STA = 0;                           // Manually clear STA bit
           break;
			case SMB_MTADDACK:    //Slave address + WRITE transmitted. ACK received.
           STO = 1;                        // Set STO to terminte transfer
           SMB_BUSY = 0;                 // Clear software busy flag
					 CPS_Wake=1;
         break;
      
      case SMB_MTADDNACK:    //Slave address + WRITE transmitted. NACK received.
         STA = 1;                // Restart transfer
         break;
      case SMB_MRADDACK:                 // (MR)从地址+R 已发出 收到ACK                 
           if (SMB_DATA_NUM==SMB_DATA_LEN){AA = 0;}
           else{AA = 1;}
           break;
      case SMB_MRADDNACK:                // (MR)从地址+R 已发出 收到NACK
           STA = 1;                           // Restart transfer
           break;
      case SMB_MRDBACK:                   // (MR)数据字节收到 ACK已发出
	       if ( SMB_DATA_NUM < SMB_DATA_LEN )   // Is there any data remaining?
             {
               SMB_DATA_IN[SMB_DATA_NUM]=SMB0DAT; 
               SMB_DATA_NUM++;                    
               AA = 1;}

          if (SMB_DATA_NUM==SMB_DATA_LEN)    // This is the last byte
          {
            SMB_BUSY = 0;                         // Free SMBus interface
            AA = 0;                  
            STO = 1;
			SMB_DATA_NUM=0;                   
          }
           
	       break; 

      case SMB_MRDBNACK:                // (MR)数据字节收到 NACK已发出
           SMB_DATA_IN[SMB_DATA_NUM]=SMB0DAT; 
           STO = 1;
           SMB_BUSY = 0;
           AA = 1;
		   SMB_DATA_NUM=0;           
           break;
      case SMB_MTARBLOST:                   //(MT)竞争失败
           FAIL = 1;                    
           break;
      // All other status codes invalid.  Reset communication.
      default:
         FAIL = 1;
         break;
   }
   if (FAIL)                                    // If the transfer failed,
   {
      SMB0CN &= ~0x40;                       // Reset communication
      SMB0CN |= 0x40;
      STA = 0;
      STO = 0;
      AA = 0;
	  SMB_BUSY = 0; 
	  SMB_DATA_NUM=0;                      // Free SMBus
      FAIL = 0;
   }
   SI = 0;                                      // Clear interrupt flag 
   SFRPAGE=saveSFRPAGE;
}