/*"***********************************************************		
FileName: EEP_Device.h		
Author: ����       
Version : 1.0       Date: 2016.12.27	
Description: EEP�豸��ӿ�
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef EEP_H
#define EEP_H
#include "system\Sys_def.h"

typedef  INT16U  EEP_ADDR_TYPE;

/*����������*/
#define EEP_32KB_MAX_ADDR  (0x7FFF)
#define EEP_64KB_MAX_ADDR  (0xFFFF)


#define EEP_DEV_NUM_1    /*  ��1���ڿ�*/
//#define EEP_DEV_NUM_2    /*  ��2���ڿ�*/

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

#if defined (EEP_DEV_NUM_1)  /*��һ���ڿ�*/
    #define  EEP_SLAVE_ADDR_1  (0xA2)       /*"EEP�豸���豸��ַ"*/
    
    #define  EEP_DEV_MAX_ADDR (EEP_64KB_MAX_ADDR)
    
#elif defined (EEP_DEV_NUM_2) /*����2���ڿ�*/
    #define EEP_DEV_1_MAX_ADDR  (EEP_64KB_MAX_ADDR)
    #define EEP_DEV_2_MAX_ADDR  (EEP_32KB_MAX_ADDR)

    /*һ�������޸Ĳ���*/
    #define  EEP_DEV_MAX_ADDR (EEP_DEV_1_MAX_ADDR+EEP_DEV_2_MAX_ADDR +1)

#else
    #error "DevNum should be defined ,Max support 2"
#endif



typedef struct
{
  INT8U  SlaveAddr;/*���豸��ַ*/
  INT16U  DevMaxAddr; /*�豸����ַ*/
  INT8U  PageSize;/*EEP�豸���ҳд����*/
}ST_EEP_DRV;


extern void  EEP_Init(void);
extern INT8U EEP_Read(EEP_ADDR_TYPE Addr, INT8U *pBuf, INT16U Len);
extern INT8U EEP_Write(EEP_ADDR_TYPE Addr, INT8U *pBuf, INT16U Len);
extern INT8U EEP_GetErrSta(void);
extern void EEP_Close(void);
extern INT8U  EEPSize_Compara(void);

#endif  

