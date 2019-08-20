/*"***********************************************************		
FileName: CLASS3_PHASE.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 分相变量类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS3_PHASE_H_
#define _CLASS3_PHASE_H_
/*"******************************************************"*/ 
/*"-------------------------------------------------------"*/

/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern const ST_Class1_6_Object CLASS3OBISLIST[MAX_CLASS3_LIST_NUM];
extern const ST_OAD_OBIS_INFO stCls3ObisInfo[MAX_CLASS3_LIST_NUM];
extern const ST_Class1_6_Object CLASS3_AID_4_OBISLIST[];
extern const ST_OAD_OBIS_INFO stCls3_AID_4_ObisInfo[];


extern INT8U get_special_aid_object(SUB_OBJECT *pObject);
/*"******************************************************"*/
#endif
