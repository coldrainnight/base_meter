/*"***********************************************************		
FileName: CLASS9_FREEZE.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 参变量类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS9_FREEZE_H_
#define _CLASS9_FREEZE_H_
/*"******************************************************"*/ 
#include "class_comm.h"
/*"-------------------------------------------------------"*/

#define MAX_CLASS9_FROZE_OI_NUM    11

/*9类冻结记录表对应OBIS*/
typedef struct 
{
    INT16U  OI;              /*OI*/  
    INT16U  OBIS;            /*OI*/
}ST_CLASS9_FREEZE_OI_OBIS;

/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/

extern INT8U get_class9_freeze_object(SUB_OBJECT *pObject);
extern INT8U get_class9_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
