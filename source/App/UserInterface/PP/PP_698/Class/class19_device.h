/*"***********************************************************		
FileName: CLASS19_DEVICE.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 设备管理类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS19_DEVICE_H_
#define _CLASS19_DEVICE_H_
/*"******************************************************"*/
#include "class_comm.h"


/*class19 属性OBIS列表结构体*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.下面的数据中心事件属性编号，由于相同类的编号是一样的，所以如果一个属性有两个OBIS，则可以连续定义，然后在程序中偏移计算*/
    /*2.显示时间格式，则判断IID即可*/
    INT16U  OBIS_ATT2;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT3;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT4;      /*"数据中心事件属性编号"*/       
    INT16U  OBIS_ATT5;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT6;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT7;      /*"数据中心事件属性编号"*/       
    INT16U  OBIS_ATT8;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT9;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT10;     /*"数据中心事件属性编号"*/     
}ST_CLASS19_OI_AID_OBIS;

/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern const ST_SPECIAL_CLASS_AID_INFO CLASS19_AID_LIST[9];
extern const ST_CLASS19_OI_AID_OBIS CLASS19_OI_AID_OBIS[];


#define MAX_CLASS19_AID_LIST_NUM   ARRAY_SIZE(CLASS19_AID_LIST)


extern INT8U get_class19_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
