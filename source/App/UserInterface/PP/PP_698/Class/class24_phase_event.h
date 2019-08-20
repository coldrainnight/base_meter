/*"***********************************************************		
FileName: CLASS24_PHASE_EVENT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: �����¼���ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS24_PHASE_EVENT_H_
#define _CLASS24_PHASE_EVENT_H_
/*"******************************************************"*/ 

#define PARAMENT_AID_CLASS24    5   /*24�����AID*/
/*"-------------------------------------------------------"*/

typedef struct
{
   INT8U AID;             /*"����"*/    
   INT8U RW;              /*"��дȨ�� 0:ֻ����0x0f:�ɶ�д*/     
   INT8U Max_IID;         /*"���IID"*/  
   /*INT8U res*/
   const INT8U *Pointer;  /*��������ָ��*/              
}ST_CLASS24_AID_INFO;


typedef struct
{
    INT8U  Aid;       /*AID*/      
    INT8U  FormatId;       /*"ͨ�š���ʾ��ʽID"*/
    INT8U  FunctionId;     /*"��д����ID"*/
    INT8U  ShowinfoId;     /*"��ʾ���ñ�ID"*/
    
    INT16U ATT_OBIS_BASE;  /*����OBIS���ֵ����Ӧ��һ��OBIS�������OBIS�����������*/
}ST_CLASS24_OBIS_INFO;

/*class24 ����OBIS�б�ṹ��*/
typedef  struct
{
    INT16U  OI;              /*OI*/
    
    /*1.��������������¼����Ա�ţ�������ͬ��ı����һ���ģ��������һ������������OBIS��������������壬Ȼ���ڳ�����ƫ�Ƽ���*/
    /*2.��ʾʱ���ʽ�����ж�IID����*/
    INT16U  OBIS_ATT2;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT3;      /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT4;      /*"���������¼����Ա��"*/   
    INT16U  OBIS_ATT10;     /*"���������¼����Ա��"*/       
    INT16U  OBIS_ATT11;     /*"���������¼����Ա��"*/  
    INT16U  OBIS_ATT12;     /*"���������¼����Ա��"*/  
    
    #ifdef THREE_PHASE
    INT16U  OBIS_ATT13;     /*"���������¼����Ա��"*/  
    #endif
    
    INT16U  OBIS_ATT14;     /*"���������¼����Ա��"*/   
}ST_CLASS24_OI_AID_OBIS;

/*9�ඳ���¼���ӦOBIS*/
typedef struct 
{
    INT16U  OI;              /*OI*/  
    INT16U  OBIS_1;          /*����6 �¼���¼��1*/
    INT16U  OBIS_2;          /*����7 �¼���¼��2*/
    INT16U  OBIS_3;          /*����8 �¼���¼��3*/
    INT16U  OBIS_4;          /*����9 �¼���¼��4*/
}ST_CLASS24_PHASE_EVENT_OI_OBIS;


/*class24 ���ò��� AID = 5 ����дȨ�޿������Ƿ���Ҫ��Ϊ�˺�CLASS7һ������ʱ����*/
typedef struct
{   
   INT8U  MAX_IID;        /*MAX_IID,���Ƿ��б�Ҫ����*/
   INT8U  Right_RW;       /*��дȨ��,0:ֻ����FF:�ɶ�д,���������������Ƿ�ɾ������Ϊ�в��ݲ����ǲ���д��*/
   //INT8U  Reser[2];
   const INT8U *Pointer;  /*����ָ��*/   
}ST_Class24_PARAMENT_ObjectList;    /*����Ԫ��*/


/*����OBISֵ*/
#ifdef THREE_PHASE
#define BASE_CLASS24_OBIS  E_PWR_DOWN_EVNT_RCRD//E_OVER_CURR_LA_EVNT_RCRD �ĳɺ�class7��һ�����Ա���ƫ��
#else
#define BASE_CLASS24_OBIS  E_PWR_DOWN_EVNT_RCRD//E_OVER_CURR_LA_EVNT_RCRD �ĳɺ�class7��һ�����Ա���ƫ��
#endif

/*��ǰ��¼����OBIS����*/
#ifdef THREE_PHASE
#define CLASS24_ATT3_OBIS_NUM_NORMAL  3
#else
#define CLASS24_ATT3_OBIS_NUM_NORMAL  1
#endif

#define CLASS24_ATT3_OBIS_NUM_ALL     4


/*-------------------------------------------------------------------*/
/*�ⲿ�������������*/

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
