/*"***********************************************************		
FileName: CLASS7_COMM_EVENT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: ͨ���¼���ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS7_COMM_EVENT_H_
#define _CLASS7_COMM_EVENT_H_
/*"******************************************************"*/ 
#define PARAMENT_AID_CLASS7    6   /*7�����AID*/
/*"-------------------------------------------------------"*/
typedef struct
{
    INT8U  Aid;       /*AID*/    
    INT8U  FormatId;  /*"ͨ�š���ʾ��ʽID"*/
    INT8U  FunctionId;/*"��д����ID"*/
    INT8U  ShowinfoId;/*"��ʾ���ñ�ID"*/
    
    INT16U ATT_OBIS_BASE;  /*����OBIS���ֵ����Ӧ��һ��OBIS�������OBIS�����������*/
}ST_CLASS7_OBIS_INFO;

/*class7 ����OBIS�б�ṹ��*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.��������������¼����Ա�ţ�������ͬ��ı����һ���ģ��������һ������������OBIS��������������壬Ȼ���ڳ�����ƫ�Ƽ���*/
    /*2.��ʾʱ���ʽ�����ж�IID����*/
    INT16U  OBIS_ATT3;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT4;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT5;      /*"���������¼����Ա��"*/   
    INT16U  OBIS_ATT7;      /*"���������¼����Ա��"*/       
    INT16U  OBIS_ATT8;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT9;      /*"���������¼����Ա��"*/      
    INT16U  OBIS_ATT10;     /*"���������¼����Ա��"*/   
}ST_CLASS7_OI_AID_OBIS;


/*7���¼���¼���ӦOBIS*/
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

/*class7���ò��� AID = 6 ����дȨ�޿������Ƿ���Ҫ��Ϊ�˺�CLASS7һ������ʱ����*/
typedef struct
{   
   INT16U OI;              /*OI*/
   INT8U  Max_IID;        /*MAX_IID,���Ƿ��б�Ҫ����*/
   INT8U  Right_RW;       /*��дȨ��,0:ֻ����FF:�ɶ�д,���������������Ƿ�ɾ������Ϊ�в��ݲ����ǲ���д��*/
   const INT8U *Pointer;  /*����ָ��*/   
}ST_Class7_PARAMENT_ObjectList;    /*����Ԫ��*/

#if 0
typedef struct
{ 
    INT8U  Seq_Num;   /*�б����к� ��CLASS7_OI_AID_OBIS�е�һ��,֮���Զ��������������Ҫ��Ϊ�˼���һ�����������α�����Ӧ */
    INT8U  num;       /*"OAD��ӦOBIS��Ԫ�ظ���"*/
    INT16U Obis;      /*"�������Ķ���  <3000,����ֱ�Ӷ�ȡ����������OBIS�б�"*/

    INT8U  Offset;    /*"�����������ݵ�ƫ�� ���̶�Ϊ0���ȱ��������濴������پ����Ƿ�ȥ��"*/
    INT8U  FormatId;  /*"ͨ�š���ʾ��ʽID"*/
    INT8U  FunctionId;/*"��д����ID"*/
    INT8U  ShowinfoId;/*"��ʾ���ñ�ID"*/
}ST_Class7_OAD_OBIS_INFO;
#endif


#ifdef THREE_PHASE
#define BASE_CLASS7_OBIS  E_PWR_DOWN_EVNT_RCRD
#else
#define BASE_CLASS7_OBIS  E_PWR_DOWN_EVNT_RCRD
#endif
/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/
extern INT8U get_class7_object(SUB_OBJECT *pObject);
extern INT8U PP_Obis_Oad_class7(INT16U Obis,ST_OAD *pOad);
extern INT8U *Get_Class7_Att7(INT8U *pdata,ST_OAD Oad);
extern INT8U *Get_Class7_Att10(INT8U *pdata,ST_OAD Oad);
extern INT8U get_class7_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);
extern INT16U Get_Duration_OBIS(INT16U Record_Obis);

/*"******************************************************"*/
#endif
