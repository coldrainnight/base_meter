/*"***********************************************************		
FileName: ISP.h		
Author: ����       
Version : 1.0       Date: 2016.01.10	
Description: ISP����ӿ�
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef ISP_H
#define ISP_H
#include "system\Sys_def.h"


// ����Ӧ�ó�����ʼ��ַ
#define APPLICATION_START           0x2000u         // APP ������ʼ��ַ
#define FLASH_BLOCK_SIZE            1024            // RL78 I1C flash һ�����СΪ1024bytes
#define FLASH_PAGE_SIZE             256             // RL78 I1C flash lib һ�����д���ֽ���








extern INT8U  ISP_WriteFlash (INT32U  Addr, INT8U *Buff, INT16U Len);
extern INT8U  ISP_ReadFlash (INT32U Addr, INT8U *Buff, INT16U Len);


#define FUNID_LEGAL   (0)  /*��������*/
#define FUNID_ILLEGAL (1)  /*�Ƿ�������*/


extern INT8U ISP_EraseFlash(INT32U Addr, INT16U Len);

#endif
