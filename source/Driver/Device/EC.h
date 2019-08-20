/*"**************************************************************************"*/
/*" EC_DRIVER.c���ͷ�ļ�                                                   "*/
/*" �����ļ��ɵ���                                                           "*/
/*"**************************************************************************"*/
#ifndef _EC_H_
#define _EC_H_

#include "Driver\MCU\EC_Config.h"
#include "Driver\MCU\EC_driver.h"


#define _EC_ID_ESAM  0             /*"ESAMͨ�����"*/
#define _EC_ID_CPU  1              /*"CPU��ͨ�����"*/
#define _EC_ID_OTHER 2

#define _PAY_ERR   0xFF           /*" ��0	����Ϊ���� "*/
#define _PAY_OK	   0x00           /*" 0����Ϊ��ȷ "*/

typedef struct
{
	UN_DWORD_DEF RemnantReg;  
	UN_DWORD_DEF BuyTimes; 
	INT32U Timer;                    /*"��ѯ��ʱ�� "*/ 
	UN_WORD_DEF Sw;
    INT8U Rnd[4];                   /*"�ϴ�ȡ�õ�������� "*/ 
    INT8U Rnd2[4];                  /*"�ϴ�ȡ�õ�������� "*/ 
	INT8U DisperFactor[8];             /*"��ɢ���� "*/

	INT8U ApduCmd[5];			/*"������������ͷ"*/
	INT8U APRnd[16];			/*"16�ֽ������"*/
	INT8U mCardStatReg;			/*"���ش���״̬��"*/
}ST_ESAMINFO;

extern ST_ESAMINFO mEsamInfo;
extern INT8U mECCurChn; /*"��ǰͨ��"*/
extern INT8U mECBuff[_ECBUFFCAP];        /*"���ݻ�����"*/
extern volatile INT16U mCurRevLen;                 /*"��ǰ���ճ���"*/

extern void EC_DataToBuf(INT8U data);
extern void EC_Tx1Byte(INT8U T_data);
extern INT8U EC_WaitRx(INT8U len);
extern void EC_EnRx(void);
extern void EC_EnTx(void);
extern INT8U EC_ATR(INT8U ECID, INT8U *pBuf);
extern INT8U EC_Cmd_Send(INT8U *Order_Head,           /*"����ͷ"*/  
									INT8U W_Len,     /*"д���ݳ���"*/
									INT8U *W_Data,   /*"д������"*/
									INT8U R_Len,     /*"�������ݳ���"*/
									INT8U ECID,      /*"ͨ��"*/
									INT8U R_Spec_Len); /*"����ĺ�������֡����"*/

extern INT8U SPIESAM_Cmd_Send(INT8U *Order_Head,           /*"����ͷ"*/  
									INT16U  W_Len,     /*"д���ݳ���"*/
									INT8U  *W_Data,   /*"д������"*/
									INT16U *R_Len);     /*"�����ݳ��ȷ���ֵ"*/

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


