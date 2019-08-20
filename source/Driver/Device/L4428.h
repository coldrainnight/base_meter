#ifndef _H_L4428_
#define _H_L4428_
#include "system\Sys_def.h"


extern INT8U L4428_Read(INT16U Addr, INT8U *pBuff, INT16U Len);
extern INT8U L4428_Write(INT16U Addr, INT8U *pBuff, INT16U Len);
extern INT8U L4428_Password_Check(INT8U *PcsKey);
extern INT8U L4428_ATR(void);
extern void L4428_PowOff(void);
extern void L4428_Hardware_Init(INT8U Mode);

#endif
