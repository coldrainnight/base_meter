#ifndef EU_H
#define EU_H

#include "..\..\PlatForm.h"
#include "LRM_Config.h"
#include "App\message\MS.h"

//#define  EU_TEST

#define  MWS_NUM_1KWH               (3600000000UL)/*1kwh对应的mWS数*/
#define  MWS_NUM_01KWH              (360000000UL) /*0.1kwh对应的mWS数*/
#define  WS_NUM_1KWH                 (3600000UL)/*1kwh对应的WS数*/

/*"计量最小分辨率定义"*/
#if defined (SINGLE_PHASE)
#define  MU_ENGY_MIN_POINT              (-5)   /*"36后面0的个数取负数"*/
#else
#define  MU_ENGY_MIN_POINT              (-8)
#endif

#define  MAX_READ_FILE_LOOP   3    /*"读数据文件最大重复次数"*/

/*"电量累加进位额度值定义，与小数位数有关"*/
#if(ENGY_DECIMAL_POINT >= MU_ENGY_MIN_POINT)
#define  SCALE_THR             (36UL * (LIB_Pow10(ENGY_DECIMAL_POINT - MU_ENGY_MIN_POINT)))
#else
"err  define of "ENGY_DECIMAL_POINT"out of range"
#endif


typedef struct
{
    INT8U data[ENERGY_SIZE];
} ST_ENERGY_FORM;

/*"-------------------基本电量数据 结构体定义-------------------"*/
typedef struct
{
#if (QUAD_ACTIVE_ENGY_EN == 1)
    ST_ENERGY_FORM P[4];        /*"四象限有功电量"*/
#endif
    ST_ENERGY_FORM ComP[2];        /*"正、反向有功电量"*/
#if (REACTIVE_ENGY_EN == 1)
    ST_ENERGY_FORM Q[4];        /*"四象限无功电量"*/
#ifdef REACTIVE_ENGY_COM
    ST_ENERGY_FORM ComQ[2];    /*"正、反向无功电量"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    ST_ENERGY_FORM ComS[2];        /*"正反视在电量"*/
#endif
}ST_BASE_ENGY;

/*"----------RAM当前电量数据 结构体定义---------"*/
typedef struct
{
    INT16U ValidFlag;     /*"合法性检测特征字，固定为5a5a"*/
    ST_BASE_ENGY  TotalEngy;    /*"当前总电量数据"*/
#if (TARIFF_NUM >= 1)
    ST_BASE_ENGY  CurTEngy;     /*"当前费率电量数据"*/
#endif
#if (PHASE_ENGY_EN == 1)
    ST_BASE_ENGY  Phase[3];     /*"当前分相总电量数据"*/
#endif
#if (TARIFF_NUM >= 1)
    INT8U tariffNow;            /*"当前费率号"*/
#endif
    CRC_DATA_TP   Crc;          /*"CRC校验"*/
} ST_RAM_ENGY_DATA;

/*"--------备份(RAM + EEP1 + EEP2)电量数据结构体定义--------"*/
typedef struct
{
    INT16U ValidFlag;     /*"合法性检测特征字，固定为5a5a"*/
    ST_BASE_ENGY  TariffEngy[TARIFF_NUM + 1];   /*"当前总 + 分费率电量数据"*/
#if (PHASE_ENGY_EN == 1)
    ST_BASE_ENGY  Phase[3];     /*"当前分相总电量数据"*/
#endif
#if (TARIFF_NUM >= 1)
    INT8U  tariffBak;       /*"备份分费率时费率号"*/
#endif
    CRC_DATA_TP Crc;
} ST_BAKUP_ENGY_DATA;

/*"-------------------基本电量尾数数据 结构体定义-------------------"*/
typedef struct
{
#if (QUAD_ACTIVE_ENGY_EN == 1)
    INT32U P[4];        /*"四象限有功电量"*/
#endif
    INT32U ComP[2];        /*"正、反向有功电量"*/
#if (REACTIVE_ENGY_EN == 1)
    INT32U Q[4];        /*"四象限无功电量"*/
#ifdef REACTIVE_ENGY_COM
    INT32U ComQ[2];    /*"正、反向无功电量"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    INT32U ComS[2];        /*"正反视在电量"*/
#endif
}ST_BASE_ENGY_TAIL;

/*"-----------电量尾数结构体定义---------"*/
typedef struct
{
    INT16U ValidFlag;     /*"合法性检测特征字，固定为5a5a"*/
    ST_BASE_ENGY_TAIL  TotalTail;   /*"总电量尾数数据"*/
    #if (PHASE_ENGY_EN == 1)
    ST_BASE_ENGY_TAIL  PhaseTail[3];     /*"分相电量尾数数据"*/
    #endif
    CRC_DATA_TP Crc;
}ST_ENGY_TAIL;

/*"-----------法定模块掉电存数结构体定义(当前累计电量、电量尾数共用该结构体变量)-----------"*/
typedef struct
{
    /*"-----------电量掉电存数---------"*/
    INT16U ValidFlag;     /*"合法性检测特征字，固定为5a5a"*/
    ST_RAM_ENGY_DATA pdNowEngy;
    ST_ENGY_TAIL stEngyTail;
    INT8U  EngyAddrFlag;  /*"当前内卡中的电量备份块标识"*/
    ST_6TIME_FORM pdTime;/*"掉电时刻"*/
    CRC_DATA_TP crc;
} ST_LRM_DATA;

#ifdef ENGY_REGISTER_ACTIVE_FUN
typedef enum
{
    E_PA_ENGY_T1_MASK,/*"正向有功费率1电量"*/
    E_PA_ENGY_T2_MASK,/*"正向有功费率2电量"*/
    E_PA_ENGY_T3_MASK,/*"正向有功费率3电量"*/
    E_PA_ENGY_T4_MASK,/*"正向有功费率4电量"*/
    E_NA_ENGY_T1_MASK,/*"反向有功费率1电量"*/
    E_NA_ENGY_T2_MASK,/*"反向有功费率2电量"*/
    E_NA_ENGY_T3_MASK,/*"反向有功费率3电量"*/
    E_NA_ENGY_T4_MASK,/*"反向有功费率4电量"*/
    E_PR_ENGY_T1_MASK,/*"正向无功费率1电量"*/
    E_PR_ENGY_T2_MASK,/*"正向无功费率2电量"*/
    E_PR_ENGY_T3_MASK,/*"正向无功费率3电量"*/
    E_PR_ENGY_T4_MASK,/*"正向无功费率4电量"*/
    E_NR_ENGY_T1_MASK,/*"反向无功费率1电量"*/
    E_NR_ENGY_T2_MASK,/*"反向无功费率2电量"*/
    E_NR_ENGY_T3_MASK,/*"反向无功费率3电量"*/
    E_NR_ENGY_T4_MASK,/*"反向无功费率4电量"*/
    E_R_ENGY_I_T1_MASK,/*"I象限无功费率1电量"*/
    E_R_ENGY_I_T2_MASK,/*"I象限无功费率2电量"*/
    E_R_ENGY_I_T3_MASK,/*"I象限无功费率3电量"*/
    E_R_ENGY_I_T4_MASK,/*"I象限无功费率4电量"*/
    E_R_ENGY_II_T1_MASK,/*"II象限无功费率1电量"*/
    E_R_ENGY_II_T2_MASK,/*"II象限无功费率2电量"*/
    E_R_ENGY_II_T3_MASK,/*"II象限无功费率3电量"*/
    E_R_ENGY_II_T4_MASK,/*"II象限无功费率4电量"*/
    E_R_ENGY_III_T1_MASK,/*"III象限无功费率1电量"*/
    E_R_ENGY_III_T2_MASK,/*"III象限无功费率2电量"*/
    E_R_ENGY_III_T3_MASK,/*"III象限无功费率3电量"*/
    E_R_ENGY_III_T4_MASK,/*"III象限无功费率4电量"*/
    E_R_ENGY_IV_T1_MASK,/*"IV象限无功费率1电量"*/
    E_R_ENGY_IV_T2_MASK,/*"IV象限无功费率2电量"*/
    E_R_ENGY_IV_T3_MASK,/*"IV象限无功费率3电量"*/
    E_R_ENGY_IV_T4_MASK,/*"IV象限无功费率4电量"*/
    E_ENGY_MASK_MAX
}EN_ENGY_REG_MASK;

#endif
extern ST_RAM_ENGY_DATA stNowEngy;/*" 当前电量电费数据"*/
extern ST_BAKUP_ENGY_DATA stNowEngyBak;/*" 当前电量数据RAM备份"*/
extern ST_LRM_DATA stPdSaveData;
#ifdef  AMP_HOUR_EN
extern INT32U AmpHour[MU_ELEMENT_NUM];/*"上电后的累计安时值，单位 0.01AH 掉电后清零"*/
#endif

extern void EU_Init(INT8U mode);
extern void EU_secTask(void);
extern INT8U EU_Clr(INT8U kind);
extern void EU_PwrDown(void);
#endif
