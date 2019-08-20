#ifndef EU_H
#define EU_H

#include "..\..\PlatForm.h"
#include "LRM_Config.h"
#include "App\message\MS.h"

//#define  EU_TEST

#define  MWS_NUM_1KWH               (3600000000UL)/*1kwh��Ӧ��mWS��*/
#define  MWS_NUM_01KWH              (360000000UL) /*0.1kwh��Ӧ��mWS��*/
#define  WS_NUM_1KWH                 (3600000UL)/*1kwh��Ӧ��WS��*/

/*"������С�ֱ��ʶ���"*/
#if defined (SINGLE_PHASE)
#define  MU_ENGY_MIN_POINT              (-5)   /*"36����0�ĸ���ȡ����"*/
#else
#define  MU_ENGY_MIN_POINT              (-8)
#endif

#define  MAX_READ_FILE_LOOP   3    /*"�������ļ�����ظ�����"*/

/*"�����ۼӽ�λ���ֵ���壬��С��λ���й�"*/
#if(ENGY_DECIMAL_POINT >= MU_ENGY_MIN_POINT)
#define  SCALE_THR             (36UL * (LIB_Pow10(ENGY_DECIMAL_POINT - MU_ENGY_MIN_POINT)))
#else
"err  define of "ENGY_DECIMAL_POINT"out of range"
#endif


typedef struct
{
    INT8U data[ENERGY_SIZE];
} ST_ENERGY_FORM;

/*"-------------------������������ �ṹ�嶨��-------------------"*/
typedef struct
{
#if (QUAD_ACTIVE_ENGY_EN == 1)
    ST_ENERGY_FORM P[4];        /*"�������й�����"*/
#endif
    ST_ENERGY_FORM ComP[2];        /*"���������й�����"*/
#if (REACTIVE_ENGY_EN == 1)
    ST_ENERGY_FORM Q[4];        /*"�������޹�����"*/
#ifdef REACTIVE_ENGY_COM
    ST_ENERGY_FORM ComQ[2];    /*"���������޹�����"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    ST_ENERGY_FORM ComS[2];        /*"�������ڵ���"*/
#endif
}ST_BASE_ENGY;

/*"----------RAM��ǰ�������� �ṹ�嶨��---------"*/
typedef struct
{
    INT16U ValidFlag;     /*"�Ϸ��Լ�������֣��̶�Ϊ5a5a"*/
    ST_BASE_ENGY  TotalEngy;    /*"��ǰ�ܵ�������"*/
#if (TARIFF_NUM >= 1)
    ST_BASE_ENGY  CurTEngy;     /*"��ǰ���ʵ�������"*/
#endif
#if (PHASE_ENGY_EN == 1)
    ST_BASE_ENGY  Phase[3];     /*"��ǰ�����ܵ�������"*/
#endif
#if (TARIFF_NUM >= 1)
    INT8U tariffNow;            /*"��ǰ���ʺ�"*/
#endif
    CRC_DATA_TP   Crc;          /*"CRCУ��"*/
} ST_RAM_ENGY_DATA;

/*"--------����(RAM + EEP1 + EEP2)�������ݽṹ�嶨��--------"*/
typedef struct
{
    INT16U ValidFlag;     /*"�Ϸ��Լ�������֣��̶�Ϊ5a5a"*/
    ST_BASE_ENGY  TariffEngy[TARIFF_NUM + 1];   /*"��ǰ�� + �ַ��ʵ�������"*/
#if (PHASE_ENGY_EN == 1)
    ST_BASE_ENGY  Phase[3];     /*"��ǰ�����ܵ�������"*/
#endif
#if (TARIFF_NUM >= 1)
    INT8U  tariffBak;       /*"���ݷַ���ʱ���ʺ�"*/
#endif
    CRC_DATA_TP Crc;
} ST_BAKUP_ENGY_DATA;

/*"-------------------��������β������ �ṹ�嶨��-------------------"*/
typedef struct
{
#if (QUAD_ACTIVE_ENGY_EN == 1)
    INT32U P[4];        /*"�������й�����"*/
#endif
    INT32U ComP[2];        /*"���������й�����"*/
#if (REACTIVE_ENGY_EN == 1)
    INT32U Q[4];        /*"�������޹�����"*/
#ifdef REACTIVE_ENGY_COM
    INT32U ComQ[2];    /*"���������޹�����"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    INT32U ComS[2];        /*"�������ڵ���"*/
#endif
}ST_BASE_ENGY_TAIL;

/*"-----------����β���ṹ�嶨��---------"*/
typedef struct
{
    INT16U ValidFlag;     /*"�Ϸ��Լ�������֣��̶�Ϊ5a5a"*/
    ST_BASE_ENGY_TAIL  TotalTail;   /*"�ܵ���β������"*/
    #if (PHASE_ENGY_EN == 1)
    ST_BASE_ENGY_TAIL  PhaseTail[3];     /*"�������β������"*/
    #endif
    CRC_DATA_TP Crc;
}ST_ENGY_TAIL;

/*"-----------����ģ���������ṹ�嶨��(��ǰ�ۼƵ���������β�����øýṹ�����)-----------"*/
typedef struct
{
    /*"-----------�����������---------"*/
    INT16U ValidFlag;     /*"�Ϸ��Լ�������֣��̶�Ϊ5a5a"*/
    ST_RAM_ENGY_DATA pdNowEngy;
    ST_ENGY_TAIL stEngyTail;
    INT8U  EngyAddrFlag;  /*"��ǰ�ڿ��еĵ������ݿ��ʶ"*/
    ST_6TIME_FORM pdTime;/*"����ʱ��"*/
    CRC_DATA_TP crc;
} ST_LRM_DATA;

#ifdef ENGY_REGISTER_ACTIVE_FUN
typedef enum
{
    E_PA_ENGY_T1_MASK,/*"�����й�����1����"*/
    E_PA_ENGY_T2_MASK,/*"�����й�����2����"*/
    E_PA_ENGY_T3_MASK,/*"�����й�����3����"*/
    E_PA_ENGY_T4_MASK,/*"�����й�����4����"*/
    E_NA_ENGY_T1_MASK,/*"�����й�����1����"*/
    E_NA_ENGY_T2_MASK,/*"�����й�����2����"*/
    E_NA_ENGY_T3_MASK,/*"�����й�����3����"*/
    E_NA_ENGY_T4_MASK,/*"�����й�����4����"*/
    E_PR_ENGY_T1_MASK,/*"�����޹�����1����"*/
    E_PR_ENGY_T2_MASK,/*"�����޹�����2����"*/
    E_PR_ENGY_T3_MASK,/*"�����޹�����3����"*/
    E_PR_ENGY_T4_MASK,/*"�����޹�����4����"*/
    E_NR_ENGY_T1_MASK,/*"�����޹�����1����"*/
    E_NR_ENGY_T2_MASK,/*"�����޹�����2����"*/
    E_NR_ENGY_T3_MASK,/*"�����޹�����3����"*/
    E_NR_ENGY_T4_MASK,/*"�����޹�����4����"*/
    E_R_ENGY_I_T1_MASK,/*"I�����޹�����1����"*/
    E_R_ENGY_I_T2_MASK,/*"I�����޹�����2����"*/
    E_R_ENGY_I_T3_MASK,/*"I�����޹�����3����"*/
    E_R_ENGY_I_T4_MASK,/*"I�����޹�����4����"*/
    E_R_ENGY_II_T1_MASK,/*"II�����޹�����1����"*/
    E_R_ENGY_II_T2_MASK,/*"II�����޹�����2����"*/
    E_R_ENGY_II_T3_MASK,/*"II�����޹�����3����"*/
    E_R_ENGY_II_T4_MASK,/*"II�����޹�����4����"*/
    E_R_ENGY_III_T1_MASK,/*"III�����޹�����1����"*/
    E_R_ENGY_III_T2_MASK,/*"III�����޹�����2����"*/
    E_R_ENGY_III_T3_MASK,/*"III�����޹�����3����"*/
    E_R_ENGY_III_T4_MASK,/*"III�����޹�����4����"*/
    E_R_ENGY_IV_T1_MASK,/*"IV�����޹�����1����"*/
    E_R_ENGY_IV_T2_MASK,/*"IV�����޹�����2����"*/
    E_R_ENGY_IV_T3_MASK,/*"IV�����޹�����3����"*/
    E_R_ENGY_IV_T4_MASK,/*"IV�����޹�����4����"*/
    E_ENGY_MASK_MAX
}EN_ENGY_REG_MASK;

#endif
extern ST_RAM_ENGY_DATA stNowEngy;/*" ��ǰ�����������"*/
extern ST_BAKUP_ENGY_DATA stNowEngyBak;/*" ��ǰ��������RAM����"*/
extern ST_LRM_DATA stPdSaveData;
#ifdef  AMP_HOUR_EN
extern INT32U AmpHour[MU_ELEMENT_NUM];/*"�ϵ����ۼư�ʱֵ����λ 0.01AH ���������"*/
#endif

extern void EU_Init(INT8U mode);
extern void EU_secTask(void);
extern INT8U EU_Clr(INT8U kind);
extern void EU_PwrDown(void);
#endif
