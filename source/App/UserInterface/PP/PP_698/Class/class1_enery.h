/*"***********************************************************		
FileName: CLASS1_ENERY.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 电能量类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS1_ENERY_H_
#define _CLASS1_ENERY_H_
/*"******************************************************"*/
//#include "class_comm.h"

typedef struct
{ 
    //INT8U Offset_List;/*这里，可能还要定义个在对象表中的位置字节,用于与OAD列表对应*/
    INT16U Obis;      /*"与num有关，num小于0x80,则是连续的OBIS基础，否则是OBIS块"*/
    INT8U  num;       /*"OAD对应OBIS中元素个数 ,如果小于0x80,则表示是连续的OBIS，否则则是块OBIS"*/
    INT8U  Offset;    /*"数据中心数据的偏移 都固定为0？先保留，后面看情况，再决定是否去掉"*/

    INT8U  FormatId;  /*"通信、显示格式ID"*/
    INT8U  FunctionId;/*"读写方法ID"*/
    INT8U  ShowinfoId;/*"显示配置表ID"*/
    /*仿真看了下，实际这里空了一个字节*/
}ST_OAD_OBIS_INFO;
    

typedef struct
{
   INT16U OI;             /*OI内容*/   
   INT8U  MAX_IID;        /*MAX_IID,看是否有必要定义*/
   INT8U  element;        /*对应ST_OBJECT_ELC的元素位置*/
}ST_Class1_6_Object;    /*属性元素*/
/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern const ST_Class1_6_Object CLASS1OBISLIST[];
extern const ST_OAD_OBIS_INFO stCls1ObisInfo[];
 
/*class1 属性4列表 高精度电能*/
extern const ST_Class1_6_Object CLASS1_AID_4_OBISLIST[];
extern const ST_OAD_OBIS_INFO stCls1_AID_4_ObisInfo[]; 

extern INT8U Get_Variable_Object(SUB_OBJECT *pObject);
extern INT8U get_class1_6_object(SUB_OBJECT *pObject,INT8U class_num);
extern void get_class1_6_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObis_Info);

/*"******************************************************"*/
#endif
