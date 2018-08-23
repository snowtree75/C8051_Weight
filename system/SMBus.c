#include "includes.h"
#include "SMBus.h"

#include "UART.h"

//===================================================
unsigned char 	SMB_DATA_IN[4] = {0,0,0,0};             // �洢��ѹ���¶�����                                      
unsigned char  	SMB_DATA_NUM = 0;                   //IIC��ȡ�ֽڵĸ���                                       
unsigned char  	TARGET;                              // Ŀ��IIC��������ַ


bit SMB_BUSY=0;                          // ���IIC����æ��־
bit SMB_RW=0;                            // ��д����λ 0 W  1 R
//===================================================
bit CPS_Wake=0;                           //���������ѱ�־


//**********************************************
/**************************************************
  �˿����ó���
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
void SMBusRead (void)                       //��׼������
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

//--------------Ӳ��SMBUS���ó���//����IIC����-------------------------------------
void SMBusInit()
{
	unsigned char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE   = SMB0_PAGE;
	SMB0CN    = 0x40;                  //IICʹ��
	SMB0CR    = 0xE9;                  //IICʱ��Ƶ��107462Hz
	SI = 0;                               //����жϱ�־
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
      case SMB_START:                       // (MT & MR)��ʼ�����ѷ���
           SMB0DAT = TARGET;              // Load address of the slave.
           SMB0DAT &= 0xFE;              
           SMB0DAT |= SMB_RW; 
					 SMB_DATA_NUM=0;              
           STA = 0;                           // Manually clear STA bit
           break;            
      case SMB_RP_START:                   // (MT & MR)�ظ���ʼ�����ѷ��� 
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
      case SMB_MRADDACK:                 // (MR)�ӵ�ַ+R �ѷ��� �յ�ACK                 
           if (SMB_DATA_NUM==SMB_DATA_LEN){AA = 0;}
           else{AA = 1;}
           break;
      case SMB_MRADDNACK:                // (MR)�ӵ�ַ+R �ѷ��� �յ�NACK
           STA = 1;                           // Restart transfer
           break;
      case SMB_MRDBACK:                   // (MR)�����ֽ��յ� ACK�ѷ���
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

      case SMB_MRDBNACK:                // (MR)�����ֽ��յ� NACK�ѷ���
           SMB_DATA_IN[SMB_DATA_NUM]=SMB0DAT; 
           STO = 1;
           SMB_BUSY = 0;
           AA = 1;
		   SMB_DATA_NUM=0;           
           break;
      case SMB_MTARBLOST:                   //(MT)����ʧ��
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