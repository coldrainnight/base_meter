/*"***********************************************************		
FileName: CLASS7_COMM_EVENT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 通用事件类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS7_COMM_EVENT_H_
#define _CLASS7_COMM_EVENT_H_
/*"******************************************************"*/ 
#define PARAMENT_AID_CLASS7    6   /*7类参数AID*/
/*"-------------------------------------------------------"*/
typedef struct
{
    INT8U  Aid;       /*AID*/    
    INT8U  FormatId;  /*"通信、显示格式ID"*/
    INT8U  FunctionId;/*"读写方法ID"*/
    INT8U  ShowinfoId;/*"显示配置表ID"*/
    
    INT16U ATT_OBIS_BASE;  /*属性OBIS块基值，对应第一个OBIS，后面的OBIS在上面进行增*/
}ST_CLASS7_OBIS_INFO;

/*class7 属性OBIS列表结构体*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.下面的数据中心事件属性编号，由于相同类的编号是一样的，所以如果一个属性有两个OBIS，则可以连续定义，然后在程序中偏移计算*/
    /*2.显示时间格式，则判断IID即可*/
    INT16U  OBIS_ATT3;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT4;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT5;      /*"数据中心事件属性编号"*/   
    INT16U  OBIS_ATT7;      /*"数据中心事件属性编号"*/       
    INT16U  OBIS_ATT8;      /*"数据中心事件属性编号"*/  
    INT16U  OBIS_ATT9;      /*"数据中心事件属性编号"*/      
    INT16U  OBIS_ATT10;     /*"数据中心事件属性编号"*/   
}ST_CLASS7_OI_AID_OBIS;


/*7类事件记录表对应OBIS*/
typedef struct 
{
    INT16U  OI;              /*OI*/  
    INT16U  OBIS;            /*OI*/
}ST_CLASS7_COMM_EVENT_OI_OBIS;

typedef struct 
{
    INT16U  OBIS_1;            
    INT16U  OBIS_2;        
}ST_OBIS_2;

/*class7配置参数 AID = 6 ，读写权限看后面是否需要，为了和CLASS7一样，暂时保留*/
typedef struct
{   
   INT16U OI;              /*OI*/
   INT8U  Max_IID;        /*MAX_IID,看是否有必要定义*/
   INT8U  Right_RW;       /*读写权限,0:只读，FF:可读写,这个看情况，后面是否删掉，因为有部份参数是不可写的*/
   const INT8U *Pointer;  /*类型指针*/   
}ST_Class7_PARAMENT_ObjectList;    /*属性元素*/

#if 0
typedef struct
{ 
    INT8U  Seq_Num;   /*列表序列号 与CLASS7_OI_AID_OBIS中的一样,之所以定义这个变量，主要是为了兼容一个参数与多个参变量对应 */
    INT8U  num;       /*"OAD对应OBIS中元素个数"*/
    INT16U Obis;      /*"数据中心对象  <3000,则是直接读取，否则，则是OBIS列表"*/

    INT8U  Offset;    /*"数据中心数据的偏移 都固定为0？先保留，后面看情况，再决定是否去掉"*/
    INT8U  FormatId;  /*"通信、显示格式ID"*/
    INT8U  FunctionId;/*"读写方法ID"*/
    INT8U  ShowinfoId;/*"显示配置表ID"*/
}ST_Class7_OAD_OBIS_INFO;
#endif


#ifdef THREE_PHASE
#define BASE_CLASS7_OBIS  E_PWR_DOWN_EVNT_RCRD
#else
#define BASE_CLASS7_OBIS  E_PWR_DOWN_EVNT_RCRD
#endif
/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern INT8U get_class7_object(SUB_OBJECT *pObject);
extern INT8U PP_Obis_Oad_class7(INT16U Obis,ST_OAD *pOad);
extern INT8U *Get_Class7_Att7(INT8U *pdata,ST_OAD Oad);
extern INT8U *Get_Class7_Att10(INT8U *pdata,ST_OAD Oad);
extern INT8U get_class7_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);
extern INT16U Get_Duration_OBIS(INT16U Record_Obis);

/*"******************************************************"*/
#endif
