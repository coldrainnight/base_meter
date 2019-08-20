/*"************************************************************"*/
/*"FileName: TR.h                                              "*/
/*"Author: 尹凌                                                "*/
/*"Version : 1.0       Date: 2017.11.10                        "*/
/*"Description: 分时模块                                       "*/
/*"Function List:                                              "*/
/*"<FunctionName>                                              "*/
/*"<author> <time> <version > <desc>                           "*/
/*"************************************************************"*/

#ifndef _TR_H_
#define _TR_H_

#include "system\Sys_def.h"
#include "Driver\HAL\FS\FS_EepConfig.h"
#include "App\MtrParamDefault\MtrParamDef.h"


#ifdef TR_IEC62056
#define WEEK_TBL_NUM        (8) /*"周表数"*/

#define CUR_EE_0          (0x55AA)
#define CUR_EE_1          (0xAA55)
#endif

#ifdef RELIABILITY 
#define SYNC_TO_ALL        (0)
#define SYNC_TO_BAK       (1)
#endif

#define TR_INIT_MODE      (0x5A)
#define TR_RUN_MODE       (0)

#define SEASON_TBL_SIZE    sizeof(ST_SEASON_TBL)
#define DAY_TBL_SIZE          sizeof(ST_DAY_TBL)
#define SPECDAY_TBL_SIZE  sizeof(ST_SPECDAY_TBL) /*"注: 用于数据缓存，不能超过20个元素 size<=82"*/
#ifdef TR_IEC62056
#define WEEK_TBL_SIZE       sizeof(ST_WEEK_TBL)
#endif

#define SPECDAY_FORM_SIZE sizeof(ST_SPECDAY_FORM)
#define SEASON_SLOT_FORM_SIZE sizeof(ST_SEASON_SLOT_FORM)
#define DAY_SLOT_FORM_SIZE sizeof(ST_DAY_SLOT_FORM)


typedef struct /*"当前费率时段信息"*/
{
    INT8U seasonSlotId;/*"当前年时区号"*/
#ifdef TR_IEC62056
    INT8U weekTblId;/*"当前周表号"*/
#endif
    INT8U dayTblId;/*"当前时段表号"*/
    INT8U daySlotId;/*"当前运行时段号"*/
    INT8U tariffId;    /*"当前费率号"*/

}ST_TR_RUN_INFO;


typedef struct /*"公共假日单元 / 特殊日单元"*/
{
#if TR_IEC62056
    INT8U  index[2];    /*"日表索引,标准为long"*/
#endif
    INT8U year;/*"特殊日: 年月日"*/
    INT8U month;
    INT8U day;
    INT8U dayTblNo;/*"日表号"*/
}ST_SPECDAY_FORM;

typedef struct /*"公共假日表/ 特殊日表"*/
{
#if TR_IEC62056
    INT8U num;    /*" 特殊日天数 "*/
#endif
    ST_SPECDAY_FORM  table[SPEC_DAY_NUM];/*"特殊日表内容"*/
#ifdef RELIABILITY 
    CRC_DATA_TP crc; /*"CRC校验"*/
#endif
}ST_SPECDAY_TBL;

typedef struct
{
#ifdef TR_IEC62056
    INT8U num;/*"日表数"*/	
#endif
    ST_DAY_TBL  table[DAY_TBL_NUM];/*"日表数据内容"*/
}ST_DAY_TBL_LIST;

#ifdef TR_IEC62056

typedef struct    /*" 周表单元 "*/
{
    INT8U  weekName;
    INT8U  dayID[7]; /*"日表号"*/
} ST_WEEK_FORM;

typedef struct  
{
    INT8U  weekSum;/*"周表总数"*/
    ST_WEEK_FORM table[WEEK_TBL_NUM];/*"周表内容"*/
#ifdef RELIABILITY 
    CRC_DATA_TP crc; 
#endif
} ST_WEEK_TBL;

typedef struct   /*"活动日历表定义 "*/
{
    INT8U  name[2];/*"日历名"*/
    ST_SEASON_TBL season;/*"季表"*/
    ST_WEEK_TBL week;/*"周表"*/
    ST_DAY_TBL_LIST day;/*"日表"*/
} ST_CALANDER;

#endif

typedef enum
{
    NORMAL = 0,
    SEASON_TBL1,
    SEASON_TBL2,
    DAY_TBL1,
    DAY_TBL2,
    SPECDAY_TBL,

    MAX_TR_TYPE
}EN_PARA_TYPE;

typedef struct
{
    INT16U fileId;
    INT16U evtObis;
}ST_TR_SW_EVT;


extern void TR_Init(INT8U mode);
#ifdef TR_IEC62056
extern INT8S TR_ActivePassive(void *pdin, void *pdout);
#endif


extern void SyncToEeprom(INT8U *pData, INT8U type, INT8U mode, INT8U index);


extern ST_TR_PARA stTrPara;
extern ST_TR_RUN_INFO  stTrRunInfo; 


/*"分时模块存EEPROM文件数据定义"*/
typedef struct
{
    ST_TR_PARA         TrPara;

    ST_SPECDAY_TBL  SpecDayTbl;
    ST_SEASON_TBL   SeasonTbl1;
    ST_SEASON_TBL   SeasonTbl2;
#ifdef TR_IEC62056
    ST_WEEK_TBL WeekTbl1;
    ST_WEEK_TBL WeekTbl2;
#endif
    ST_DAY_TBL_LIST  DayTbl1List;
    ST_DAY_TBL_LIST  DayTbl2List;

#ifdef RELIABILITY
    ST_TR_PARA         TrParaBak;

    ST_SEASON_TBL   SeasonTbl1Bak;
    ST_SEASON_TBL   SeasonTbl2Bak;
    #ifdef TR_IEC62056
    ST_WEEK_TBL WeekTbl1Bak;
    ST_WEEK_TBL WeekTbl2Bak;
    #endif
    ST_DAY_TBL_LIST  DayTbl1ListBak;
    ST_DAY_TBL_LIST  DayTbl2ListBak;

#endif

}ST_TR_FILE_PARA;


/*"分时参数文件偏移"*/
#define  TR_PARA_SHIFT           FIND(ST_TR_FILE_PARA, TrPara)
#define  SPECDAY_TBL_SHIFT   FIND(ST_TR_FILE_PARA, SpecDayTbl)
#define  SEASON_TBL1_SHIFT    FIND(ST_TR_FILE_PARA, SeasonTbl1)
#define  SEASON_TBL2_SHIFT    FIND(ST_TR_FILE_PARA, SeasonTbl2)
#ifdef TR_IEC62056
#define  WEEK_TBL1_SHIFT        FIND(ST_TR_FILE_PARA, WeekTbl1)
#define  WEEK_TBL2_SHIFT        FIND(ST_TR_FILE_PARA, WeekTbl2)
#endif
#define  DAY_TBL1_LIST_SHIFT  FIND(ST_TR_FILE_PARA, DayTbl1List)
#define  DAY_TBL2_LIST_SHIFT  FIND(ST_TR_FILE_PARA, DayTbl2List)

#ifdef RELIABILITY
#define  TR_PARA_BAK_SHIFT            FIND(ST_TR_FILE_PARA, TrParaBak)
#define  SEASON_TBL1_BAK_SHIFT    FIND(ST_TR_FILE_PARA, SeasonTbl1Bak)
#define  SEASON_TBL2_BAK_SHIFT    FIND(ST_TR_FILE_PARA, SeasonTbl2Bak)
    #ifdef TR_IEC62056
#define  WEEK_TBL1_BAK_SHIFT        FIND(ST_TR_FILE_PARA, WeekTbl1Bak)
#define  WEEK_TBL2_BAK_SHIFT        FIND(ST_TR_FILE_PARA, WeekTbl2Bak)
    #endif
#define  DAY_TBL1_LIST_BAK_SHIFT  FIND(ST_TR_FILE_PARA, DayTbl1ListBak)
#define  DAY_TBL2_LIST_BAK_SHIFT  FIND(ST_TR_FILE_PARA, DayTbl2ListBak)

#endif

#endif


