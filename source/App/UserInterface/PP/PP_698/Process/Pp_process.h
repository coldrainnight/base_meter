/*"***********************************************************		
FileName: pp_bois.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: Э�鴦�������ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/
#ifndef _PP_PROCESS_H
#define _PP_PROCESS_H
/********************************************************/
//#include "App\UserInterface\PP\PP_698\Class\class_comm.h"
#include "Lib\AXDR\AXDR.h"
#include "App\UserInterface\PP\PP_698\Class\class8_parament.h"
#include "app\UserInterface\display\dm.h"

/*����֡����ṹ��*/
/*���ﻹҪ������RECORDLIST������*/
typedef struct
{
   INT16U TotalLine;        /*Ҫ��ȡ��������(��¼����) =0 ʱ���ײ�ֱ�ӷ��ؼ�¼����*/
   INT16U CurrentLine;      /*Ҫ��ȡ�ĵ�ǰ����(��¼����)*/      

   INT16U Obis_event_freeze;/*�¼��򶳽��OBIS*/
   INT8U  SelectNumber;     /*ɸѡ����*/
   INT8U  OadListNumber;    /*ɸѡ����OAD����*/
      
   ST_OAD Oad_Selector;     /*ɸѡ��OAD,�翪ʼʱ�䡢����ʱ�䡢����ʱ��*/
   
   ST_OAD Oad;              /*�����¼��򶳽�OAD����3000,3002,5000��*/

   ST_OAD oadlist[MAXCSD];  /*ɸѡ����OAD,���￴Ҫ��Ҫ֧��ROAD*/
   
   INT8U  DdataFilter[LEN_SELECT2_DATA];    /*ɸѡ����������,����2��������������÷���2�ĳ��ȴ���*/
   INT8U  DataRersev;                       /*Ԥ��һ���ֽڣ��������ֽڶ���*/
}ST_Next_Frame_Record_Data;/*63*4=252 Byte*/

#define LEN_NEXT_FRAM_RECORD_DATA    sizeof(ST_Next_Frame_Record_Data)

typedef struct
{
   INT16U TotalLine;        /*��0:Ҫ��ȡ��������(��¼����), =0:�ײ�ֱ�ӷ��ؼ�¼����*/
   INT16U CurrentLine;      /*Ҫ��ȡ�ĵ�ǰ����(��¼����) =0:��Ҫ���RCSD*/
      
   INT16U Obis_event_freeze;/*�¼��򶳽��OBIS*/
   INT8U  SelectNumber;    /*ɸѡ����*/
   INT8U  OadListNumber;   /*ɸѡ����OAD����*/
      
   ST_OAD Oad_Selector;    /*ɸѡ��OAD*/
   
   ST_OAD *poadlist;       /*ɸѡ����OAD,���￴Ҫ��Ҫ֧��ROAD*/

   INT8U  *pDdataFilter;   /*����9����2�ֽڷ�0����ʾ����ʾ*/
}ST_Get_Record_Data;


/*��ȡ���ݷ��ؽṹ��*/
typedef struct 
{
  INT8U *pLeng;     /*ָ���Ӧ��OAD�������ݳ���*/
  INT8U *pdata;     /*ָ������*/
}ST_RECROD_RET;

typedef struct
{
    INT8U *pLenList;
    INT8U *pBuff;
}ST_GetRcrdOut;

/*�̶��¼���¼��Ԫ*/
typedef struct
{
    INT16U Oi;
    INT8U Fixed_RCSD_Num;
    const ST_OAD *Pointer;  /*�̶�����ָ��*/
}ST_Fixed_RCSD;    

typedef struct
{
   INT8U TotalLine;        /*Ҫ��ȡ��OAD�ܸ���*/
   INT8U CurrentLine;      /*Ҫ��ȡ�ĵ�ǰOAD���� =0:��ʾû��Ҫ��ȡ�������ˣ���û�к���֡��*/
   INT8U reser[2];         /*�ֽڶ��룬����*/
   
   /*���������һ���������������Ҫ������֡�Ļ�*/
   ST_OAD oadlist[MAXGETLISTNUMBER];  /*ɸѡ��OAD*/
}ST_Next_Frame_Normal_Data;


typedef struct
{
    INT8U ServiceType;/*�������� GET-Request��GET-Response��  */
    INT8U Choice;     /*ѡ���  �������͵��������ͣ���GetRequestNormal\GetRequestNormalList��  */
	INT8U PIID;       /*���ȼ�  */
	INT8U ListNumber; /*"set list number"*/
}ST_SERVICE_DATA;    

/*�������ݣ���֧�ֺ���֡��Ҳ����Ҫ�ú���֡*/
typedef struct{
	INT8U ListNumber; /*"set list number"*/
    INT8U Rerse[3];   /*�ֽڶ��룬����*/
    
	ST_OAD SetOad[MAXSETTHENGETLISTNUMBER];
    ST_OAD GetOad[MAXSETTHENGETLISTNUMBER];
	INT8U  *pointer[MAXSETTHENGETLISTNUMBER];   /*"������������ָ�룬���������"*/
}ST_SET_THEN_GET_SERVICE;

typedef struct{
    INT8U ListNumber; /*"set list number"*/
    INT8U Rerse[3];   /*�ֽڶ��룬����*/
    
	ST_OAD SetOad[MAXSETLISTNUMBER];
	INT8U  *pointer[MAXSETLISTNUMBER];   /*"������������ָ�룬���������"*/
}ST_SET_SERVICE;

/*�������������֧�ֺ���֡��Ҳ����Ҫ�ú���֡*/
typedef struct
{
    INT8U ListNumber; /*"action list number"*/
    INT8U Rerse[3];   /*�ֽڶ��룬����*/
    
	_ST_OMD  ActionOmd[MAXACTIONTHENLISTNUMBER];
    
	ST_OAD   GetOad[MAXACTIONTHENLISTNUMBER];
    
	INT8U    *Pointer[MAXACTIONTHENLISTNUMBER];  /*������������ָ��*/
}ST_ACTION_THEN_GET_SERVICE;

typedef struct
{
    INT8U ListNumber; /*"action list number"*/
    INT8U Rerse[3];   /*�ֽڶ��룬����*/
    
	_ST_OMD  ActionOmd[MAXACTIONLISTNUMBER];
    
	INT8U    *Pointer[MAXACTIONLISTNUMBER];  /*������������ָ��*/
}ST_ACTION_SERVICE; 


typedef struct
{
    ObjectMethod ObjMeth;    /*����ID*/
    INT16U resr;             /*�ֽڶ��룬����*/
    const INT8U * Pointer;        /*��������(����ָ��) */
}ACTION_OBJECT;

/*�����ײ�ķ������ݽṹ��*/
typedef struct
{
  INT16U Obis;        /*Obis*/   
  INT16U len;         /*���ݳ���*/
  INT8U  *pdata;    
}ST_ACTION_DATa ;

/*�����ײ�ķ������ݽṹ��*/
typedef struct
{
  ST_OAD Oad;        /*Obis*/   
  INT8U  *pdata;    
  INT16U len;        /*���ݳ���*/
}ST_ACTION_REMOTE_DATa ;

/*������Ŷ�Ӧ�ĸ�ʽ*/
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
    INT8U ShowinfoId;    /*��ʾ����ID*/
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
	const INT8U *pointer;//�������͵�ַָ��
}_Event_ObjectList;


/*�÷���9�ĵ�2�ֽڱ�ʾͨ�Ż���ʾ*/
typedef enum
{
    E_SELELTOR9_COMM   = 0,   /*ͨ��*/
    E_SELELTOR9_DISP   = 1,   /*��ʾ*/
}EN_SELELTOR9_COM_DISP;    

extern INT8U gServiceChoice;       /*CHOICE��ʽ*/

extern INT8U *pEndEncodeAddr;   /*Ӧ�ò�������ձ��볤��*/

/*����֡�ݴ�����*/
extern INT8U gBak_data[MAXGETRECORDLISTNUMBER][LEN_NEXT_FRAM_RECORD_DATA];

/*�ֱ������ݴ浱ǰ��recordlist��recordlistnum*/   
extern INT8U Record_Num_List;     

extern INT8U ComPort;   /*ͨ�Ŷ˿ں�*/
extern INT8U LenSingleData;
extern INT8U Fix_Len_Varible;

/*��������������Ƿ�Ҫ����RECORDLISTNUM����*/
extern INT8U gDarDecode;  /*�����������ʽ���󣬻�LIST NUMԽ�� ����������ȼ���ߣ����ʱ���������Ϊ׼*/


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
