/*"**************************************************************************"*/
/*" EC_DRIVER.c输出头文件                                                   "*/
/*" 其他文件可调用                                                           "*/
/*"**************************************************************************"*/
#ifndef _EC_H_
#define _EC_H_

#include "Driver\MCU\EC_Config.h"
#include "Driver\MCU\EC_driver.h"


#define _EC_ID_ESAM  0             /*"ESAM通道编号"*/
#define _EC_ID_CPU  1              /*"CPU卡通道编号"*/
#define _EC_ID_OTHER 2

#define _PAY_ERR   0xFF           /*" 非0	定义为错误 "*/
#define _PAY_OK	   0x00           /*" 0定义为正确 "*/

typedef struct
{
	UN_DWORD_DEF RemnantReg;  
	UN_DWORD_DEF BuyTimes; 
	INT32U Timer;                    /*"查询定时器 "*/ 
	UN_WORD_DEF Sw;
    INT8U Rnd[4];                   /*"上次取得的随机数据 "*/ 
    INT8U Rnd2[4];                  /*"上次取得的随机数据 "*/ 
	INT8U DisperFactor[8];             /*"分散因子 "*/

	INT8U ApduCmd[5];			/*"最后操作的命令头"*/
	INT8U APRnd[16];			/*"16字节随机数"*/
	INT8U mCardStatReg;			/*"返回错误状态字"*/
}ST_ESAMINFO;

extern ST_ESAMINFO mEsamInfo;
extern INT8U mECCurChn; /*"当前通道"*/
extern INT8U mECBuff[_ECBUFFCAP];        /*"数据缓冲区"*/
extern volatile INT16U mCurRevLen;                 /*"当前接收长度"*/

extern void EC_DataToBuf(INT8U data);
extern void EC_Tx1Byte(INT8U T_data);
extern INT8U EC_WaitRx(INT8U len);
extern void EC_EnRx(void);
extern void EC_EnTx(void);
extern INT8U EC_ATR(INT8U ECID, INT8U *pBuf);
extern INT8U EC_Cmd_Send(INT8U *Order_Head,           /*"命令头"*/  
									INT8U W_Len,     /*"写数据长度"*/
									INT8U *W_Data,   /*"写的数据"*/
									INT8U R_Len,     /*"读的数据长度"*/
									INT8U ECID,      /*"通道"*/
									INT8U R_Spec_Len); /*"特殊的后续数据帧长度"*/

extern INT8U SPIESAM_Cmd_Send(INT8U *Order_Head,           /*"命令头"*/  
									INT16U  W_Len,     /*"写数据长度"*/
									INT8U  *W_Data,   /*"写的数据"*/
									INT16U *R_Len);     /*"读数据长度返回值"*/

extern INT8U EC_SelectFile(INT8U ECID, INT16U fileName);
extern INT8U EC_GetRnd(INT8U ECID, INT8U len,INT8U *output);
extern INT8U EC_Read(INT8U ECID, INT8U file, INT16U offset, INT8U len, INT8U * output);
extern INT8U EC_EsamRead(INT8U file, INT16U offset, INT8U len, INT8U * output);
extern INT8U EC_Write(INT8U ECID, INT8U file, INT16U offset, INT8U len, INT8U *input);

extern INT8U EC_ReadMnyBag(INT8U *data);
extern INT8U EC_MnyBagInit(INT8U *data);
extern INT8U EC_MnyBagAdd(INT8U fileName,INT8U offset,INT8U *addr);
extern INT8U EC_MnyBagDec(INT32U RmnMny);
extern INT8U EC_InterAuth(INT8U *CpuNum);
extern INT8U EC_IrAuthChk(INT8U *disperse,INT8U *Rnd,INT8U *output);
extern INT8U EC_IrAuth(INT8U *Rnd);

extern INT8U EC_ExterAuth(INT8U *CpuNum);

extern INT8U EC_EsamUpdate(INT8U CpuFileName,
                        INT8U EsamFileName,
                        INT8U CpuAddr,
                        INT8U EsamAddr,
                        INT8U len,
                        INT8U *output);
extern INT8U EC_CpuUpdate(INT8U cpuFileName,
                        INT8U EsamFileName,
                        INT8U CpuAddr,
                        INT8U EsamAddr,
                        INT8U len,
                        INT8U *CpuNum);

#endif


