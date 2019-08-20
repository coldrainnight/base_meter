/*"****************************************************************************
Description:�Լ�����ݴ洢�Ͷ�ȡ�ӿ�
Function List:
    1:MSR_Init()                     �Լ���ʼ������
    2:MSR_Main()                    �Լ����������
    3:MSR_GetRecord()            �Լ���ѯ����
    4:MSR_ClrRecord()             �Լ�����㺯��
Author:YWJ
Date:2018-05-14
*****************************************************************************"*/

#ifndef _MSR_H_
#define _MSR_H_
//#include "..\\..\\lib\\macro.h"

//#include "LIB\sys_def.h"
#include "System\sys_def.h"
//#include "..\\..\\hal\\FS\\FS_DataType.h"
//#include "..\\..\\hal\\ST\\ST.h"
//#include "..\\..\\lib\\LIB_func.h"
//#include "..\\cm\\cm.h"
//#include "..\\dc\\dc_data.h"
//#include "..\\dc\\datatype.h"
//#include "..\\dc\\DC_Process.h"
#define Frame_LENGTH22        (22) //todo 
extern INT8U MsrSetTime(INT8U *time);
extern INT16U MsrGetRecord(INT8U *indata,INT8U *outdata,INT8U type);
extern void MsrClear(void);
extern void MsrInit(void);
extern void MsrMain(void);
extern void MsrSaveRecord(INT8U *data);
#endif




