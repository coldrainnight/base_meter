/*"***********************************************************		
FileName: ISP.h		
Author: 邹杨       
Version : 1.0       Date: 2016.01.10	
Description: ISP对外接口
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef ISP_H
#define ISP_H
#include "system\Sys_def.h"


// 定义应用程序起始地址
#define APPLICATION_START           0x2000u         // APP 程序起始地址
#define FLASH_BLOCK_SIZE            1024            // RL78 I1C flash 一个块大小为1024bytes
#define FLASH_PAGE_SIZE             256             // RL78 I1C flash lib 一次最大写入字节数








extern INT8U  ISP_WriteFlash (INT32U  Addr, INT8U *Buff, INT16U Len);
extern INT8U  ISP_ReadFlash (INT32U Addr, INT8U *Buff, INT16U Len);


#define FUNID_LEGAL   (0)  /*法定部分*/
#define FUNID_ILLEGAL (1)  /*非法定部分*/


extern INT8U ISP_EraseFlash(INT32U Addr, INT16U Len);

#endif
