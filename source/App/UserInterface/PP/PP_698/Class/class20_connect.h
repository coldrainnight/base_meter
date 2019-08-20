/*"***********************************************************		
FileName: CLASS20_CONNECT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 应用连接类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS20_CONNECT_H_
#define _CLASS20_CONNECT_H_
/*"******************************************************"*/ 

/*"-------------------------------------------------------"*/

/*class20 属性OBIS列表结构体*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.下面的数据中心事件属性编号，由于相同类的编号是一样的，所以如果一个属性有两个OBIS，则可以连续定义，然后在程序中偏移计算*/
    /*2.显示时间格式，则判断IID即可*/
    INT16U  OBIS_ATT3;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT4;      /*"数据中心事件属性编号"*/       
    INT16U  OBIS_ATT5;      /*"数据中心事件属性编号"*/  
}ST_CLASS20_OI_AID_OBIS;

/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern const INT8U C20_Att3_FactoryVersion[];
extern const ST_SPECIAL_CLASS_AID_INFO CLASS20_AID_LIST[3];
extern const ST_CLASS20_OI_AID_OBIS CLASS20_OI_AID_OBIS[1];


extern INT8U *get_class20_connect_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet);
extern INT8U get_class20_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
