/*"***********************************************************		
FileName: CLASS21_ESAM.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: ESAM接口类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS21_ESAM_H_
#define _CLASS21_ESAM_H_
/*"******************************************************"*/  

/*class19 属性OBIS列表结构体*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.下面的数据中心事件属性编号，由于相同类的编号是一样的，所以如果一个属性有两个OBIS，则可以连续定义，然后在程序中偏移计算*/
    /*2.显示时间格式，则判断IID即可*/
    INT16U  OBIS_ATT2;      /*"1.数据中心事件属性编号"*/  
    INT16U  OBIS_ATT3;      /*"2.数据中心事件属性编号"*/  
    INT16U  OBIS_ATT4;      /*"3.数据中心事件属性编号"*/       
    INT16U  OBIS_ATT5;      /*"4.数据中心事件属性编号"*/  
    INT16U  OBIS_ATT6;      /*"5.数据中心事件属性编号"*/  
    INT16U  OBIS_ATT7;      /*"6.数据中心事件属性编号"*/       
    INT16U  OBIS_ATT14;      /*"7.数据中心事件属性编号"*/  
    INT16U  OBIS_ATT15;      /*"8.数据中心事件属性编号"*/  
}ST_CLASS21_OI_AID_OBIS;


/*外部常量或变量声明*/
extern const ST_SPECIAL_CLASS_AID_INFO CLASS21_AID_LIST[8];
extern const ST_CLASS21_OI_AID_OBIS CLASS21_OI_AID_OBIS[1];

extern INT8U get_class21_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
