/*"***********************************************************		
FileName: CLASS17_DISPLAY.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 显示类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS17_DISPLAY_H_
#define _CLASS17_DISPLAY_H_
/*"******************************************************"*/
#include "class_comm.h"


/*class17 属性OBIS列表结构体*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.下面的数据中心事件属性编号，由于相同类的编号是一样的，所以如果一个属性有两个OBIS，则可以连续定义，然后在程序中偏移计算*/
    /*2.显示时间格式，则判断IID即可*/
    INT16U  OBIS_ATT2;      /*"显示对象列表"*/  
    INT16U  OBIS_ATT3;      /*"显示时间"*/  
    INT16U  OBIS_ATT4;      /*"显示参数"*/  
}ST_CLASS17_OI_AID_OBIS;


/*外部常量或变量声明*/
extern const ST_SPECIAL_CLASS_AID_INFO CLASS17_AID_LIST[];
extern const ST_CLASS17_OI_AID_OBIS CLASS17_OI_AID_OBIS[];

/*"******************************************************"*/
extern INT8U get_special_class_object(SUB_OBJECT *pObject);
extern INT8U set_class17_disp_data(ST_OAD Oad,INT8U *pdata);
extern INT8U *get_class17_disp_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet);
extern INT8U get_class17_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
