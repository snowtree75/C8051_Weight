#ifndef __SMBUS_H__
#define __SMBUS_H__

 /*               ---SMBUS-----I2C----
************************************************/
#define  SMB_DATA_LEN   4              //定义读取数据长度

#define  WRITE          0x00           // WRITE direction bit
#define  READ           0x01           // READ direction bit
#define  SLAVE_ADDR     0x50           // Device address for slave target  (0x28>>1)

#define  SMB_BUS_ERROR  0x00           // (all modes) BUS ERROR
#define  SMB_START      0x08           // (MT & MR)起始条件已发出 将从地址//+R/W 装入到SMB0DAT 清0 STA
#define  SMB_RP_START   0x10           // (MT & MR)重复起始条件已发出 将从地
//址+R/W 装入到SMB0DAT 清0 STA
#define  SMB_MTADDACK   0x18           // (MT)从地址+W 已发出 收到ACK 将要//发送的数据装入到SMB0DAT
#define  SMB_MTADDNACK  0x20           // (MT)从地址+W 已发出 收到NACK 确//认查询重试 置位STO+STA
#define  SMB_MTDBACK    0x28           // (MT)数据字节已发出 收到ACK
#define  SMB_MTDBNACK   0x30           // (MT)数据字节已发出 收到NACK
#define  SMB_MTARBLOST  0x38           // (MT)竞争失败
#define  SMB_MRADDACK   0x40           // (MR)从地址+R 已发出 收到ACK
#define  SMB_MRADDNACK  0x48           // (MR)从地址+R 已发出 收到NACK
#define  SMB_MRDBACK    0x50           // (MR)数据字节收到 ACK已发出
#define  SMB_MRDBNACK   0x58           // (MR)数据字节收到 NACK已发出

extern bit CPS_Wake;              
extern unsigned char  	TARGET;
extern unsigned char 	SMB_DATA_IN[4]; 

void SMBusInit(void);
void SMBusRead(void);
void SMBusWrite(void);

#endif