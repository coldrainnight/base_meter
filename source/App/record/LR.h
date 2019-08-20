/*"*******************************************************************"*/
/*"  File name:      lr.h"*/
/*"  Author:  ganjp     Version:   V1.0     Date: 2017.5.9"*/
/*"  Description:    ���Ӷ�����⺯���ӿ�����"*/
/*"                  ���Ӷ���ṹ������"*/

/*"  Others:         "*/
/*"  Function List:  "*/

/*"    1. Date:"*/
/*"       Author:"*/
/*"       Modification:"*/
/*"    2. ..."*/
/*"*********************************************************************"*/
#ifndef _LR_H_
#define _LR_H_

struct LR_RECORD
{
    INT8U time[4];
	INT8U VA[2];
	INT8U IA[3];
    INT8U Freq[2];

    INT8U PAll[3];
    INT8U RAll[3];

    INT8U Factor[2];

    INT8U PP[ENERGY_SIZE];
	INT8U NP[ENERGY_SIZE];

	INT8U Quad1[ENERGY_SIZE];
	INT8U Quad2[ENERGY_SIZE];
	INT8U Quad3[ENERGY_SIZE];
	INT8U Quad4[ENERGY_SIZE];    
};


struct RECORD1
{
    INT8U time[4];
	INT8U VA[2];
#if defined(THREE_PHASE)
	INT8U VB[2];
	INT8U VC[2];
#endif
	INT8U IA[3];
#if defined(THREE_PHASE)
	INT8U IB[3];
	INT8U IC[3];
#endif
    INT8U Freq[2];
};

struct RECORD2
{
    INT8U time[4];
    INT8U PAll[3];
#if defined(THREE_PHASE)
	INT8U PA[3];
	INT8U PB[3];
	INT8U PC[3];

    INT8U RAll[3];
	INT8U RA[3];
	INT8U RB[3];
	INT8U RC[3];
#endif
};

struct RECORD3
{
    INT8U time[4];
	INT8U Factor[2];
#if defined(THREE_PHASE)
	INT8U FactorA[2];
	INT8U FactorB[2];
	INT8U FactorC[2];
#endif
};

struct RECORD4
{
    INT8U time[4];
	INT8U PP[ENERGY_SIZE];
	INT8U NP[ENERGY_SIZE];
#if defined(THREE_PHASE)
    INT32U com1_r_eng;
    INT32U com2_r_eng;
#endif
};

struct RECORD5
{
    INT8U time[4];

	INT8U Quad1[ENERGY_SIZE];
	INT8U Quad2[ENERGY_SIZE];
	INT8U Quad3[ENERGY_SIZE];
	INT8U Quad4[ENERGY_SIZE];

};

struct RECORD6
{
    INT8U time[4];
	INT8U DemandActive[3];
	INT8U DemandReactive[3];
};



/*"�������Զ���"*/
typedef struct
{
    RECORD_ADDR_TYPE startAddr;/*"������ʼ�洢��ַ"*/
#ifdef LD_DATA_IN_FLASH
    INT16U sum_sector;          /*"��������ռ���������� "*/
#endif
    INT16U   priod;            /*"�������ݴ洢����"*/
    INT32U   deep;             /*"�������ݴ洢���"*/
    INT8U    frzObjIndex[MAX_OAD_MIN_FRZ];/*"�������������Ӧ���������б������� �������ܹ��������ĵڼ��� "*/
    INT8U    AssObjNum;        /*"��������������"*/
    INT8U    DotLen;           /*"����ÿ����¼�����ݳ��� "*/
}ST_SCHEME_ATTR_FORM;

typedef struct
{
    ST_SCHEME_ATTR_FORM  SchemeAttr[MIN_FRZ_CLASS_NUM];
    INT8U    SchemeNum;/*"��ǰ������"*/
    CRC_DATA_TP crcValue;
}ST_SCHEME_ATTR;

typedef struct
{
    INT16U proid;  /*"����"*/
    ST_OAD oad;    /*"������������������"*/
    INT16U deep;   /*"�洢���"*/
}ST_FRZ_OBJ_ATTR;

typedef struct
{
    ST_6TIME_FORM TimeOld;
    ST_6TIME_FORM TimeNew;
}ST_TIME_ADJUST_INPUT;

extern INT8U IndexRefFlg[MIN_FRZ_CLASS_NUM];

extern void LR_Init(INT8U mode);/*"���ɼ�¼��ʼ������"*/
extern void LR_Main(void);/*"���ɼ�¼��������"*/
extern INT8U LR_ClrRecord(void);/*"���ɼ�¼���㺯��"*/
extern void LR_TimeAdjust(INT8U *pdata);
extern void LR_CtrlInforPdSave(void);
extern void upd_all_ee_ctrl_info(INT8U channel);
extern INT8U poweron_init_ctrl_info(INT8U channel);
extern void save_ctrl_info(INT8U channel);
extern void LR_SetDefaultIdx(void);


#endif

