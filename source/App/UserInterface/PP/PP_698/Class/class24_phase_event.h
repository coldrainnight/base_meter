/*"***********************************************************		
FileName: CLASS24_PHASE_EVENT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 分项事件类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS24_PHASE_EVENT_H_
#define _CLASS24_PHASE_EVENT_H_
/*"******************************************************"*/ 

#define PARAMENT_AID_CLASS24    5   /*24类参数AID*/
/*"-------------------------------------------------------"*/

typedef struct
{
   INT8U AID;             /*"属性"*/    
   INT8U RW;              /*"读写权限 0:只读，0x0f:可读写*/     
   INT8U Max_IID;         /*"最大IID"*/  
   /*INT8U res*/
   const INT8U *Pointer;  /*数据类型指针*/              
}ST_CLASS24_AID_INFO;


typedef struct
{
    INT8U  Aid;       /*AID*/      
    INT8U  FormatId;       /*"通信、显示格式ID"*/
    INT8U  FunctionId;     /*"读写方法ID"*/
    INT8U  ShowinfoId;     /*"显示配置表ID"*/
    
    INT16U ATT_OBIS_BASE;  /*属性OBIS块基值，对应第一个OBIS，后面的OBIS在上面进行增*/
}ST_CLASS24_OBIS_INFO;

/*class24 属性OBIS列表结构体*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.下面的数据中心事件属性编号，由于相同类的编号是一样的，所以如果一个属性有两个OBIS，则可以连续定义，然后在程序中偏移计算*/
    /*2.显示时间格式，则判断IID即可*/
    INT16U  OBIS_ATT2;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT3;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT4;      /*"数据中心事件属性编号"*/   
    INT16U  OBIS_ATT10;     /*"数据中心事件属性编号"*/       
    INT16U  OBIS_ATT11;     /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT12;     /*"数据中心事件属性编号"*/  
    
    #ifdef THREE_PHASE
    INT16U  OBIS_ATT13;     /*"数据中心事件属性编号"*/  
    #endif
    
    INT16U  OBIS_ATT14;     /*"数据中心事件属性编号"*/   
}ST_CLASS24_OI_AID_OBIS;

/*9类冻结记录表对应OBIS*/
typedef struct 
{
    INT16U  OI;              /*OI*/  
    INT16U  OBIS_1;          /*属性6 事件记录表1*/
    INT16U  OBIS_2;          /*属性7 事件记录表2*/
    INT16U  OBIS_3;          /*属性8 事件记录表3*/
    INT16U  OBIS_4;          /*属性9 事件记录表4*/
}ST_CLASS24_PHASE_EVENT_OI_OBIS;


/*class24 配置参数 AID = 5 ，读写权限看后面是否需要，为了和CLASS7一样，暂时保留*/
typedef struct
{   
   INT8U  MAX_IID;        /*MAX_IID,看是否有必要定义*/
   INT8U  Right_RW;       /*读写权限,0:只读，FF:可读写,这个看情况，后面是否删掉，因为有部份参数是不可写的*/
   //INT8U  Reser[2];
   const INT8U *Pointer;  /*类型指针*/   
}ST_Class24_PARAMENT_ObjectList;    /*属性元素*/


/*基础OBIS值*/
#ifdef THREE_PHASE
#define BASE_CLASS24_OBIS  E_PWR_DOWN_EVNT_RCRD//E_OVER_CURR_LA_EVNT_RCRD 改成和class7的一样，以便算偏移
#else
#define BASE_CLASS24_OBIS  E_PWR_DOWN_EVNT_RCRD//E_OVER_CURR_LA_EVNT_RCRD 改成和class7的一样，以便算偏移
#endif

/*当前记录数的OBIS个数*/
#ifdef THREE_PHASE
#define CLASS24_ATT3_OBIS_NUM_NORMAL  3
#else
#define CLASS24_ATT3_OBIS_NUM_NORMAL  1
#endif

#define CLASS24_ATT3_OBIS_NUM_ALL     4


/*-------------------------------------------------------------------*/
/*外部常量或变量声明*/

extern INT8U get_class24_object(SUB_OBJECT *pObject);
extern INT8U PP_Obis_Oad_class24(INT16U Obis,ST_OAD *pOad);
extern INT8U *Get_Class24_Att10(INT8U *pdata,ST_OAD Oad);
extern INT8U *Get_Class24_Att14(INT8U *pdata,ST_OAD Oad);
extern INT8U *Get_Class24_Att3(INT8U *pdata,ST_OAD Oad);
#ifdef THREE_PHASE
extern INT8U *Get_Class24_Att13(INT8U *pdata,ST_OAD Oad);
#endif
extern INT8U get_class24_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);

/*"******************************************************"*/
#endif
