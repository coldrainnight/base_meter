#ifndef  SID_H
#define  SID_H

#include "System\sys_def.h"

/*"状态输入类型"*/
#ifdef THREE_PHASE
#define SID_FAC               (0)    /*"工厂状态"*/
#define SID_MTRCOVER     (1)   /*"上盖"*/
#define SID_RAILCOVER    (2)   /*"翻盖"*/
#define SID_REALY            (3)   /*"继电器状态检测"*/
#define SID_CARD             (4)    /*卡到位"*/
#define SID_MAGNET        (5)    /*"恒定磁场干扰"*/
#define SID_MAX               (6)   /*"状态输入数"*/
#else
#define SID_FAC          (0)  /*"工厂状态"*/
#define SID_MTRCOVER     (1)  /*"开盖检测"*/
#define SID_REALY        (2)  /*"继电器状态检测"*/
#define SID_CARD         (4)  /*"卡检测"*/
#define SID_MAX          (3)  /*"状态输入数"*/
#endif

/*"状态输入的查询结果"*/
#define SID_UNDEF    0x02
#define SID_TRUE       0x01   /*" SID_TRUE 的含义 "*/
                              /*"厂内状态"*/
                              /*"上盖开盖"*/
                              /*"翻盖开盖"*/
                              /*"继电器合闸"*/
                              /*"有卡"*/
                              /*"有磁场干扰"*/                        
#define SID_FALSE      0x00


extern void SID_Init(INT8U mode,INT8U sel);  /*"状态输入的初始化"*/


extern void SID_Release(INT8U mode,INT8U sel);  /*"状态输入的释放"*/


extern INT8U SID_PollRead(INT8U sel);  /*"状态输入的IO口读函数"*/


extern INT8U SID_Read(INT8U sel);  /*"状态输入的读函数"*/

extern INT8U SID_IntVectSet(INT8U sel, ISR_FNCT_VOID intfunc);

extern void SID_NullIntFunc(void); /*"状态输入的空的中断函数"*/

#endif