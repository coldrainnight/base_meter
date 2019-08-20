/*"***********************************************************		
FileName: EEP_Device.h		
Author: 邹杨       
Version : 1.0       Date: 2016.12.27	
Description: EEP设备层接口
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef EEP_H
#define EEP_H
#include "system\Sys_def.h"

typedef  INT16U  EEP_ADDR_TYPE;

/*物理常量定义*/
#define EEP_32KB_MAX_ADDR  (0x7FFF)
#define EEP_64KB_MAX_ADDR  (0xFFFF)


#define EEP_DEV_NUM_1    /*  有1块内卡*/
//#define EEP_DEV_NUM_2    /*  有2块内卡*/

#if defined (EEP_DEV_NUM_1) 
    #if defined (EEP_DEV_NUM_2)
        "err only one of "EEP_DEV_NUM_1" or "EEP_DEV_NUM_2" shoulud be selected"
    #endif
#endif

#ifndef EEP_DEV_NUM_1
    #ifndef EEP_DEV_NUM_2
        "err one of "EEP_DEV_NUM_1" or "EEP_DEV_NUM_2" be selected"
    #endif
#endif

#if defined (EEP_DEV_NUM_1)  /*有一块内卡*/
    #define  EEP_SLAVE_ADDR_1  (0xA2)       /*"EEP设备从设备地址"*/
    
    #define  EEP_DEV_MAX_ADDR (EEP_64KB_MAX_ADDR)
    
#elif defined (EEP_DEV_NUM_2) /*存在2块内卡*/
    #define EEP_DEV_1_MAX_ADDR  (EEP_64KB_MAX_ADDR)
    #define EEP_DEV_2_MAX_ADDR  (EEP_32KB_MAX_ADDR)

    /*一般无需修改部分*/
    #define  EEP_DEV_MAX_ADDR (EEP_DEV_1_MAX_ADDR+EEP_DEV_2_MAX_ADDR +1)

#else
    #error "DevNum should be defined ,Max support 2"
#endif



typedef struct
{
  INT8U  SlaveAddr;/*从设备地址*/
  INT16U  DevMaxAddr; /*设备最大地址*/
  INT8U  PageSize;/*EEP设备最大页写限制*/
}ST_EEP_DRV;


extern void  EEP_Init(void);
extern INT8U EEP_Read(EEP_ADDR_TYPE Addr, INT8U *pBuf, INT16U Len);
extern INT8U EEP_Write(EEP_ADDR_TYPE Addr, INT8U *pBuf, INT16U Len);
extern INT8U EEP_GetErrSta(void);
extern void EEP_Close(void);
extern INT8U  EEPSize_Compara(void);

#endif  

