/*"*******************************************************************"*/
/*"  File name:      lr.h"*/
/*"  Author:  ganjp     Version:   V1.0     Date: 2017.5.9"*/
/*"  Description:    分钟冻结对外函数接口申明"*/
/*"                  分钟冻结结构体属性"*/

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



/*"方案属性定义"*/
typedef struct
{
    RECORD_ADDR_TYPE startAddr;/*"方案起始存储地址"*/
#ifdef LD_DATA_IN_FLASH
    INT16U sum_sector;          /*"方案数据占用扇区个数 "*/
#endif
    INT16U   priod;            /*"方案数据存储周期"*/
    INT32U   deep;             /*"方案数据存储深度"*/
    INT8U    frzObjIndex[MAX_OAD_MIN_FRZ];/*"方案关联对象对应关联对象列表索引号 就是在总关联对象表的第几个 "*/
    INT8U    AssObjNum;        /*"方案关联对象数"*/
    INT8U    DotLen;           /*"方案每个记录点数据长度 "*/
}ST_SCHEME_ATTR_FORM;

typedef struct
{
    ST_SCHEME_ATTR_FORM  SchemeAttr[MIN_FRZ_CLASS_NUM];
    INT8U    SchemeNum;/*"当前方案数"*/
    CRC_DATA_TP crcValue;
}ST_SCHEME_ATTR;

typedef struct
{
    INT16U proid;  /*"周期"*/
    ST_OAD oad;    /*"关联对象属性描述符"*/
    INT16U deep;   /*"存储深度"*/
}ST_FRZ_OBJ_ATTR;

typedef struct
{
    ST_6TIME_FORM TimeOld;
    ST_6TIME_FORM TimeNew;
}ST_TIME_ADJUST_INPUT;

extern INT8U IndexRefFlg[MIN_FRZ_CLASS_NUM];

extern void LR_Init(INT8U mode);/*"负荷记录初始化函数"*/
extern void LR_Main(void);/*"负荷记录主处理函数"*/
extern INT8U LR_ClrRecord(void);/*"负荷记录清零函数"*/
extern void LR_TimeAdjust(INT8U *pdata);
extern void LR_CtrlInforPdSave(void);
extern void upd_all_ee_ctrl_info(INT8U channel);
extern INT8U poweron_init_ctrl_info(INT8U channel);
extern void save_ctrl_info(INT8U channel);
extern void LR_SetDefaultIdx(void);


#endif

