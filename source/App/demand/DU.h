/*******************************************************************************
*Copyright   : 2017  Wasion Group Ltd.
*FileName    : DU.h
*Description : ��������ģ��ͷ�ļ�
Modify records:
Author       Date               Version           Description
HeZhuli      2017.2.7           v1.0              Created
*******************************************************************************/
#ifndef  _DU_H
#define  _DU_H
/*"*******************************************************************************"*/
#include "system\Sys_def.h" 
#include "App\LegallyRelevant\LRM_Config.h"
#include "App\MtrParamDefault\MtrParamDef.h"
/*"*******************************************************************************"*/

/*�޹����������Ĭ��ֵ*/
#define DEFAULT_NA_MODE_1_VALUE  0x41
#define DEFAULT_NA_MODE_2_VALUE  0x14

#define TYPE_DEMAND_CAL  INT64U    //INT32U
#define MULTIPLY_POWER   1         /*�����5λС��������10*/

#define MULTIPLY_POWER_POINTER 100 /*-6ת����-4�����Գ���100*/

//#define DEMAND_DATE_TIME     DateTimeType

#define DEMAND_DATE_TIME     ST_5TIME_FORM

#define DU_CUR_AVE_DM   0x01  /* ��ǰƽ������ */
#define DU_MAX_DM      	0x02  /* ������� */
#define DU_TOTALMAX_DM  0x04  /* �ۼ�������� */

#define DU_LAST_DM      0x08  /* �����ۼƻ�������� */

#define DU_R_DEMAND_DM  0x10  /*�޹�����*/

#define MAX_POWER       (380UL*120*3*1000)  /*�����Ĺ�����380V��120A��*/

#if 0
#define Ap_TOTAL      0  /* ���й����� */
#define Qp_TOTAL      1  /* ���޹����� */
#define Sp_TOTAL      2  /* ���������� */
#define Ap_TOU    	   3  /* �й���ʱ���� */
#define Qp_TOU    	   4  /* �޹���ʱ���� */
#define Sp_TOU   	   5  /* ���ڷ�ʱ���� */
#endif

/*��������ö�ٶ���*/
typedef enum
{
    Ap_P     = 0,  /* �� �����й� */
    Ap_N     = 1,  /* �� �����й� */ 
    Qp_I     = 2,  /* I�����޹� */
    Qp_II    = 3,  /* II�����޹� */
    Qp_III   = 4,  /* III�����޹� */
    Qp_IV    = 5,  /* IV�����޹� */
    /*��������*/
    #if 0
    Qp_P = 2,  /* �� �����޹� */
    Qp_N = 3,  /* �� �����޹� */
    Sp_P = 4,  /* �� �������� */
    Sp_N = 5,  /* �� ��������*/
    #endif
    E_MAC_DEMAND_TYPE,
}ENUM_DEMAND_TYPE;


/*������������*/
#define INX_DEMANDPERIOD 			0/*��������*/
#define INX_NUMOFPERIOD				1/*����������"*/

//#define Qp_I      6  /* QI �޹�     */
//#define Qp_II     7  /* QII �޹�    */
//#define Qp_III    8  /* QIII �޹�   */
//#define Qp_IV     9  /* QIV �޹�    */

#define DM_POSITIVE  0   /*����*/
#define DM_REVERSE   1   /*����*/

#define ERR_RELOADTIMES 2   /*�����ݴ���������*/

#define DU_FALSE 		0
#define DU_TRUE 		1

#define DU_THROWN       	0   /*�޸�ʱ����ϵ��һ���ӣ�����һ���Ӷ���"*/

/*������������һ��������ʶ*/
#define DU_FIRST_DM    	   (g_DU_Ram.start_flag.bit.b0)   

/*�ϵ�����ֱ�ʶ*/
#define DU_FIRST1MIN       (g_DU_Ram.start_flag.bit.b1)   

/*"������������"*/
#define MAX_MD_PERIOD          (60)    /*"����:��������.             ����:����ʱ��  ��"*/
#define MAX_SLIP_NUM           60      /*"����:����������:��󻬲��� ����:��������/����ʱ�� "*/
#define MAX_MD_NUM             6       /*"��������Ĵ�����: �����й����������޹�, ����޹�1��2�ڳ���ʱ���⴦�����"*/

/*"ƽ�������ṹ��"*/
typedef struct    
{
  INT32U TempValue;            /*"��ʱƽ��ֵ"*/
  INT32U CurrValue;            /*"��ǰƽ��ֵ"*/
  INT32U LastValue;            /*"����ƽ��ֵ"*/
  DEMAND_DATE_TIME CapTime;    /*"����ƽ��ֵ����ʱ��: ������ʱ����."*/
  //DEMAND_DATE_TIME StarTime;   /*"����ƽ��ֵ��ʼʱ��: ������ʱ����."*/
} ST_AVDM_DEF;

/*"��������ṹ��"*/
typedef struct      
{
  INT32U           Value;          /*"�������ֵ: ����/������.  ��32λ����Ҳ�Ƚϴ󣬿��Բ��ÿ������"*/
  DEMAND_DATE_TIME Time;           /*"�����������ʱ��: ������ʱ����."*/
} ST_MD_DEF;  /*4+5->���ֽڶ���12*/

/*"���������������"*/
typedef struct    
{
    ST_MD_DEF   Data[MAX_MD_NUM];        /*"�����������."*/
    CRC_DATA_TP Crc;                     /*"CRCУ��"*/
} ST_MD_FROM; /*12*8+4=100*/

/*"��ǰ�����������"*/
typedef struct    
{
    ST_MD_FROM  Total;      /*"�������������"*/
    ST_MD_FROM  CurData;    /*"��ǰ������������"*/
    CRC_DATA_TP Crc;  /*"CRCУ��"*/
} ST_MD_DATA;

/*"���������������: ��+1~8��ʱ��������"*/
typedef struct    
{
    ST_MD_FROM  Data[TARIFF_NUM+1];      /*"�����������"*/
    
    CRC_DATA_TP Crc;  /*"CRCУ��"*/
} ST_MD_DATA_ALL;

/*"�ۼ������������"*/
typedef struct    
{
    INT32U Value[MAX_MD_NUM];      /*"�ۼ��������"*/
    
    CRC_DATA_TP Crc;               /*"CRCУ��"*/
} ST_TLMD_DATA;


/*���������ۼ�buff*/
typedef struct
{
	TYPE_DEMAND_CAL SkipTime_Power;		    /*����������������ۼƹ���  64λ 10��-6kw*/
	TYPE_DEMAND_CAL power_buf[MAX_SLIP_NUM];/*����ƽ��ֵ*/
} st_DU_RamType;

/*�����ۼ�״̬�ṹ��*/
typedef struct
{
    UN_BYTE_DEF   start_flag;           /*bit0:������������һ��������ʶ bit1:�ϵ�����ֱ�ʶ*/
    INT8U   Periods_Count;              /*��������������*/
	INT8U   Resr;//Cur_TariffNo;               /*��ʱ������ǰ���ʺ�,�÷�������*/
	INT8U   UpdatePermit;               /*��ʱ�л�����0��ʱ�ȴ�һ��ȫ���ں��ٳ��µ�����*/

    INT16U  SkipTime_Count;            /*��������ʱ���ۼ�(��)*/
	INT8U	last_min;                  /*��һ�μ���������ʱ��֣��뵱ǰʱ��Ƚϣ���ͬʱ��������µ�*/
    INT8U   UpdatePermit_R;            /*���޹���������ʱ���޹��л�ʱ���ȴ�һ��ȫ���ں��ٳ�������*/ 
    
    st_DU_RamType Data[MAX_MD_NUM];    /*���������ۼ�buff*/
} st_DU_Ram;/*����210���ֽ�*/

/*
ʱ��ͬ����������ʱ����ϵͳʱ���йء���ϵͳʱ��ķ����ܱ���������������ʼ��������
����ͬ������������ʱ����ϵͳʱ���޹أ����ϵ���ۼ�ʱ�䵽�������ڿ�ʼ��������
���磺������������Ϊ15���ӣ�����Ϊ3���ӣ�09ʱ02��30���ϵ磬�������ʱ��ͬ����������09ʱ15��0�뿪ʼ��������������շ���ͬ����������09ʱ17��0�뿪ʼ������.
*/

/*���������ṹ��*/
typedef struct
{
  FTR_DATA_TP validFlg; 
  INT8U  NumOfPeriod;   /*"����:����������:��󻬲��� ����:�������ڳ�����ʱ�� "*/
  INT8U  DemandTime;    /*"����:��������:��λ:��      ֮�������������Ҫ��Ϊ�˺ü���ʹ洢��ͬʱҲ��Ϊ���ֽڶ���"*/

  INT8U  DemandPeriod;  /*"����:��������.             ����:����ʱ��  ��  ���ĳɷ�����"*/
  INT8U  Resr;          /*���ã�4�ֽڶ��룬Ԥ��*/
  INT8U  DemandComMode1;/*����޹�����ֵ1*/
  INT8U  DemandComMode2;/*����޹�����ֵ1*/
  //-----
  CRC_DATA_TP Crc;       
}ST_DU_PARA;   

/*��������ƫ��*/
#define DU_PARA_NUM_PERIOD_SHIFT     FIND(ST_DU_PARA, validFlg)
#define DU_PARA_DEMAND_TIME_SHIFT    FIND(ST_DU_PARA, DemandTime)
#define DU_PARA_DEMAND_PERIOD_SHIFT  FIND(ST_DU_PARA, DemandPeriod)
#define DU_PARA_DEMAND_MODE_SHIFT    FIND(ST_DU_PARA, DemandMode)
#define DU_PARA_CRC_SHIFT            FIND(ST_DU_PARA, Crc)


/*����ģ����ļ�����*/
typedef struct
{
   //ST_MD_DATA     FILE_MDData;          /*"������������� ������ʱ�ģ�һֱ�仯�ģ��¹̶���(�����ڽ���ģ���ֱ�Ӵ��ڿ���)"*/
   ST_MD_DATA_ALL FILE_MD_ALL_Data;     /*"����ʷ����������ݣ��ѽ���洢��"*/
   ST_TLMD_DATA   FILE_TotalDm;         /*"��ʷ�ۼ������������"*/
}ST_DU_FILE_DATA;    

#if 0
/*���������������ƫ��*/
#define DU_MD_DATA_SHIFT   	    FIND(ST_DU_FILE_DATA, FILE_MDData)
#define DU_MD_TOTAL_DATA_SHIFT  (DU_MD_DATA_SHIFT+FIND(ST_MD_DATA, Total))
#define DU_MD_CUR_DATA_SHIFT    (DU_MD_DATA_SHIFT+FIND(ST_MD_DATA, CurData))
#define DU_MD_DATA_CRC_SHIFT    (DU_MD_DATA_SHIFT+FIND(ST_MD_DATA, Crc))
#endif

/*�ܼ����� ��������ƫ��*/
#define DU_MD_DATA_ALL_SHIFT               FIND(ST_DU_FILE_DATA, FILE_MD_ALL_Data)
#define DU_MD_DATA_ALL_DATA_SUM_SHIFT      (DU_MD_DATA_ALL_SHIFT+FIND(ST_MD_DATA_ALL, Data))
#define DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(i) (DU_MD_DATA_ALL_SHIFT+i*sizeof(ST_MD_FROM))
#define DU_MD_DATA_ALL_CRC_SHIFT           (DU_MD_DATA_ALL_SHIFT+FIND(ST_MD_DATA_ALL, Crc))


/*��ʷ�ۼ������������*/
#define DU_TOTAL_DM_SHIFT   	FIND(ST_DU_FILE_DATA, FILE_TotalDm)
#define DU_TOTAL_DM_VALUE_SHIFT (DU_TOTAL_DM_SHIFT+FIND(ST_TLMD_DATA, Value))
#define DU_TOTAL_DM_CRC_SHIFT   (DU_TOTAL_DM_SHIFT+FIND(ST_TLMD_DATA, Crc))

/*"*******************************************************************************"*/
extern st_DU_Ram  g_DU_Ram;
extern ST_DU_PARA g_DU_Para;   /*��������*/
extern INT32S     Current_Demand[2];
extern NO_INIT ST_AVDM_DEF  stAvDmData[MAX_MD_NUM];
/******************************************************************************/
extern void DU_Init(INT8U mode);
extern void DU_Main(void);
extern void DU_CurAveValue(INT8U flag);

extern INT8S DU_Clr(void *pdin, void *pdout);
extern INT8S DU_MaxDemandClr(void *pdin, void *pdout);
extern INT8S DU_CurrentDemandClr(void *pdin, void *pdout);
extern INT8S DU_R_DemandClr(void *pdin, void *pdout);
extern INT8S DU_Update_R_com_type(void *pdin, void *pdout);
extern INT8S DU_Tariff_Switch(void *pdin, void *pdout);

extern void  DU_Change_Date_Time_Com(INT8U *pSrcDateTime,INT8U *pDstDateTime);
/*"*******************************************************************************"*/

#endif

