/*"***********************************************************		
FileName: pp_bois.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 协议处理对象类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/
#ifndef _PP_PROCESS_H
#define _PP_PROCESS_H
/********************************************************/
//#include "App\UserInterface\PP\PP_698\Class\class_comm.h"
#include "Lib\AXDR\AXDR.h"
#include "App\UserInterface\PP\PP_698\Class\class8_parament.h"
#include "app\UserInterface\display\dm.h"

/*后续帧处理结构体*/
/*这里还要定义多个RECORDLIST。。。*/
typedef struct
{
   INT16U TotalLine;        /*要读取的总行数(记录条数) =0 时，底层直接返回记录条数*/
   INT16U CurrentLine;      /*要读取的当前行数(记录条数)*/      

   INT16U Obis_event_freeze;/*事件或冻结的OBIS*/
   INT8U  SelectNumber;     /*筛选方法*/
   INT8U  OadListNumber;    /*筛选的列OAD个数*/
      
   ST_OAD Oad_Selector;     /*筛选的OAD,如开始时间、结束时间、冻结时间*/
   
   ST_OAD Oad;              /*具体事件或冻结OAD，如3000,3002,5000等*/

   ST_OAD oadlist[MAXCSD];  /*筛选的列OAD,这里看要不要支持ROAD*/
   
   INT8U  DdataFilter[LEN_SELECT2_DATA];    /*筛选的数据条件,方法2的数据最长，所以用方法2的长度代替*/
   INT8U  DataRersev;                       /*预留一个字节，构成四字节对齐*/
}ST_Next_Frame_Record_Data;/*63*4=252 Byte*/

#define LEN_NEXT_FRAM_RECORD_DATA    sizeof(ST_Next_Frame_Record_Data)

typedef struct
{
   INT16U TotalLine;        /*非0:要读取的总行数(记录条数), =0:底层直接返回记录条数*/
   INT16U CurrentLine;      /*要读取的当前行数(记录条数) =0:需要打包RCSD*/
      
   INT16U Obis_event_freeze;/*事件或冻结的OBIS*/
   INT8U  SelectNumber;    /*筛选方法*/
   INT8U  OadListNumber;   /*筛选的列OAD个数*/
      
   ST_OAD Oad_Selector;    /*筛选的OAD*/
   
   ST_OAD *poadlist;       /*筛选的列OAD,这里看要不要支持ROAD*/

   INT8U  *pDdataFilter;   /*方法9，第2字节非0，表示是显示*/
}ST_Get_Record_Data;


/*获取数据返回结构体*/
typedef struct 
{
  INT8U *pLeng;     /*指向对应的OAD返回数据长度*/
  INT8U *pdata;     /*指向数据*/
}ST_RECROD_RET;

typedef struct
{
    INT8U *pLenList;
    INT8U *pBuff;
}ST_GetRcrdOut;

/*固定事件记录单元*/
typedef struct
{
    INT16U Oi;
    INT8U Fixed_RCSD_Num;
    const ST_OAD *Pointer;  /*固定对象指针*/
}ST_Fixed_RCSD;    

typedef struct
{
   INT8U TotalLine;        /*要读取的OAD总个数*/
   INT8U CurrentLine;      /*要读取的当前OAD个数 =0:表示没有要读取的数据了，即没有后续帧了*/
   INT8U reser[2];         /*字节对齐，备用*/
   
   /*这里后面用一个宏扩起来，如果要读后续帧的话*/
   ST_OAD oadlist[MAXGETLISTNUMBER];  /*筛选的OAD*/
}ST_Next_Frame_Normal_Data;


typedef struct
{
    INT8U ServiceType;/*服务类型 GET-Request、GET-Response、  */
    INT8U Choice;     /*选择号  服务类型的数据类型，如GetRequestNormal\GetRequestNormalList等  */
	INT8U PIID;       /*优先级  */
	INT8U ListNumber; /*"set list number"*/
}ST_SERVICE_DATA;    

/*设置数据，不支持后续帧，也不需要用后续帧*/
typedef struct{
	INT8U ListNumber; /*"set list number"*/
    INT8U Rerse[3];   /*字节对齐，备用*/
    
	ST_OAD SetOad[MAXSETTHENGETLISTNUMBER];
    ST_OAD GetOad[MAXSETTHENGETLISTNUMBER];
	INT8U  *pointer[MAXSETTHENGETLISTNUMBER];   /*"具体数据内容指针，解码后数据"*/
}ST_SET_THEN_GET_SERVICE;

typedef struct{
    INT8U ListNumber; /*"set list number"*/
    INT8U Rerse[3];   /*字节对齐，备用*/
    
	ST_OAD SetOad[MAXSETLISTNUMBER];
	INT8U  *pointer[MAXSETLISTNUMBER];   /*"具体数据内容指针，解码后数据"*/
}ST_SET_SERVICE;

/*方法类操作，不支持后续帧，也不需要用后续帧*/
typedef struct
{
    INT8U ListNumber; /*"action list number"*/
    INT8U Rerse[3];   /*字节对齐，备用*/
    
	_ST_OMD  ActionOmd[MAXACTIONTHENLISTNUMBER];
    
	ST_OAD   GetOad[MAXACTIONTHENLISTNUMBER];
    
	INT8U    *Pointer[MAXACTIONTHENLISTNUMBER];  /*方法具体数据指针*/
}ST_ACTION_THEN_GET_SERVICE;

typedef struct
{
    INT8U ListNumber; /*"action list number"*/
    INT8U Rerse[3];   /*字节对齐，备用*/
    
	_ST_OMD  ActionOmd[MAXACTIONLISTNUMBER];
    
	INT8U    *Pointer[MAXACTIONLISTNUMBER];  /*方法具体数据指针*/
}ST_ACTION_SERVICE; 


typedef struct
{
    ObjectMethod ObjMeth;    /*方法ID*/
    INT16U resr;             /*字节对齐，备用*/
    const INT8U * Pointer;        /*数据类型(编码指针) */
}ACTION_OBJECT;

/*传给底层的方法数据结构体*/
typedef struct
{
  INT16U Obis;        /*Obis*/   
  INT16U len;         /*数据长度*/
  INT8U  *pdata;    
}ST_ACTION_DATa ;

/*传给底层的方法数据结构体*/
typedef struct
{
  ST_OAD Oad;        /*Obis*/   
  INT8U  *pdata;    
  INT16U len;        /*数据长度*/
}ST_ACTION_REMOTE_DATa ;

/*分屏序号对应的格式*/
typedef struct 
{
   INT8U format_1;
   INT8U format_2;
   //INT8U format_3;
   //INT8U format_4;
}ST_DISP_FORTMAT_SRCNO;

typedef struct
{
    UN_OI Oi;
    INT8U ShowinfoId;    /*显示配置ID*/
}ST_ROAD_SHOW_INFO;

typedef struct
{
    INT16U obis;
    INT8U min;
    INT8U max;
}ST_DATA_RANGE;

typedef struct
{ 
	ST_OAD Oad;
	const INT8U *pointer;//数据类型地址指针
}_Event_ObjectList;


/*用方法9的第2字节表示通信或显示*/
typedef enum
{
    E_SELELTOR9_COMM   = 0,   /*通信*/
    E_SELELTOR9_DISP   = 1,   /*显示*/
}EN_SELELTOR9_COM_DISP;    

extern INT8U gServiceChoice;       /*CHOICE格式*/

extern INT8U *pEndEncodeAddr;   /*应用层给的最终编码长度*/

/*后续帧暂存数据*/
extern INT8U gBak_data[MAXGETRECORDLISTNUMBER][LEN_NEXT_FRAM_RECORD_DATA];

/*分别用于暂存当前的recordlist和recordlistnum*/   
extern INT8U Record_Num_List;     

extern INT8U ComPort;   /*通信端口号*/
extern INT8U LenSingleData;
extern INT8U Fix_Len_Varible;

/*看后面这个变量是否要做成RECORDLISTNUM个数*/
extern INT8U gDarDecode;  /*解析出错，如格式错误，或LIST NUM越界 解码出错，优先级最高，打包时优先以这个为准*/


extern INT8U PP_GetOadToObisList(INT8U IID,INT8U num_obis,INT16U *pObis,
                                ST_PARAMENT_OAD_OBIS_INFO *pObis_Info,ST_FMT_ATTR *pFormat);

extern INT8U * EncodeAtt1LogicName(ST_OAD*pOad,INT8U *pdata);
extern INT8U *Encode_Scale_Unit(ST_OAD*pOad,INT8U *pdata,INT8U element);
extern INT8U *EnCodeOAD(ST_OAD *poad,INT8U *pdata);
extern INT8U *EncodeAcionDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdst);
extern INT8U *EncodeSetDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdst);
extern INT8U *EncodeGetDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdst);
extern INT8U *get_record_process(INT8U *pdata);
extern INT8U *encode_record_data(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,
                               ST_GetRcrdOut *pGetRcrdOut, INT8U *pdata,INT8U *pRet);

extern INT8U *Process_698(INT8U servicetype,INT8U *pdata);
extern INT16S LenGet_PP_698(ENUM_DATALEN_ID Id);
extern INT8U check_time_tag(INT8U *p_time_tag_src);

extern INT8U PP_ShowGetData (ST_DISP_OBJ *pdata_in,
                           ST_SHOWDATA_OUTPUT *pdata_out );

extern INT8U PP_RecordColumnFilterDisp(ST_OAD *pRoad, ST_OAD *pOad, INT8U *pbuf);

extern INT8S PrgEvent_Rcrd(void *pdin, void *pdout);

extern INT8U BeforeFunSetSystemTime(INT8U *pdata);

#ifdef THREE_PHASE
extern INT8U BeforeFunSetDuPara(INT8U *pdata);
#endif

extern INT8U get_oad_object(SUB_OBJECT *pObject);
extern INT8U Check_Object(INT8U ServiceType,SUB_OBJECT *pObject,INT8U *pData);
extern void PP_PrgEvent(ST_OAD Oad);
extern INT8U PP_OadToObis_Info(SUB_OBJECT *pobject, ST_PARAMENT_OAD_OBIS_INFO *pObis_Info);
extern INT16U Get_OAD_Data_len(ST_OAD Oad,SUB_OBJECT *pobject,INT8U  *pret);
extern INT8U TransYear(INT8U * InYear);
extern INT16U Cal_EnCode_Data_Len(ST_ENCODE_INFO * pInfo,INT8U IID);
extern INT8U Check_Relation_Oad_Ok(INT8U *pdata,ST_OAD Oad,INT16U len);
extern INT8U SetSingle(INT16U OBIS,INT8U *pData,INT8U len);
/********************************************************/
#endif
