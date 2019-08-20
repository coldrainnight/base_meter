#ifndef FS_H
#define FS_H


#include "system\Sys_def.h"
#include "driver\device\EEP.h"
#include "FS_Def.h"

#if defined (SINGLE_PHASE) 
#define FS_EEP_EN
#define FS_FLASH_EN
#elif defined (THREE_PHASE)   
#define FS_EEP_EN
#define FS_FLASH_EN
#endif


typedef struct 
{
    ENUM_FS_ID         FileId;/*"�ļ���"*/
    INT16U         Size;/*"�ļ���С"*/
    EEP_ADDR_TYPE  Addr;/*"��ʼ��ַ"*/ 
}ST_EEP_FILE_INFO;



typedef struct 
{
    ENUM_FS_ID     FileId; /*"�ļ���"*/
    INT32U         Size; /*"�ļ���СSection"*/
    INT32U         Addr; /*"��ʼ��ַSection"*/ 
}ST_FLASH_FILE_INFO;

extern INT8U FS_Init(INT8U Mode);
extern INT8U FS_WriteFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
extern INT8U FS_ReadFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
extern INT8U FS_DelFile(ENUM_FS_ID FileName,INT32U StartOffset, INT32U Len);



#endif
