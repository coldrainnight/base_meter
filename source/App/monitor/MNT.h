/*"************************************************************"*/
/*"FileName: MNT_data.h"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: 需量模块对象列表定义"*/
/*" 总字节数:          "*/
/*" Function List:                     "*/
/*"                                    "*/
/*" History:                           "*/
/*" <author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#ifndef MNT_H
#define MNT_H

#include "PlatForm.h"
#include "App\message\MS.h"
#include "MNTConfig.h"
#include "App\MtrParamDefault\MtrParamDef.h"
#if(MNT_VERSION == SOUTH_GRID)
#include  "App\LegallyRelevant\LRM.h"
#endif
/*"----------电网事件定义（需累计时间）---------"*/
typedef enum
{
    E_EVT_PWR_DOWN = 0,   /*"掉电"*/
#ifdef THREE_PHASE
    E_EVT_LOST_VOLT_LA, /*"A相失压"*/
    E_EVT_LOST_VOLT_LB, /*"B相失压"*/
    E_EVT_LOST_VOLT_LC, /*"C相失压"*/
    E_EVT_LOST_VOLT_ALL,/*"全失压"*/
    E_EVT_LOST_CURR_LA, /*"A相失流"*/
    E_EVT_LOST_CURR_LB, /*"B相失流"*/
    E_EVT_LOST_CURR_LC, /*"C相失流"*/
    E_EVT_BREAK_PHASE_LA,/*"A相断相"*/
    E_EVT_BREAK_PHASE_LB,/*"B相断相"*/
    E_EVT_BREAK_PHASE_LC,/*"C相断相"*/
#endif
#ifdef OVER_CURR_CHK
    E_EVT_OVER_CURR_LA,  /*"A相过流"*/
#endif
#ifdef THREE_PHASE
    #ifdef OVER_CURR_CHK
    E_EVT_OVER_CURR_LB,  /*"B相过流"*/
    E_EVT_OVER_CURR_LC,  /*"C相过流"*/
    #endif
    E_EVT_FAIL_CURR_LA,  /*"A相断流"*/
    E_EVT_FAIL_CURR_LB,  /*"B相断流"*/
    E_EVT_FAIL_CURR_LC,  /*"C相断流"*/
    E_EVT_OVER_VOLT_LA, /*"A相过压"*/
    E_EVT_OVER_VOLT_LB, /*"B相过压"*/
    E_EVT_OVER_VOLT_LC, /*"C相过压"*/
    E_EVT_UNDER_VOLT_LA,   /*"A相欠压"*/
    E_EVT_UNDER_VOLT_LB,   /*"B相欠压"*/
    E_EVT_UNDER_VOLT_LC,   /*"C相欠压"*/
    E_EVT_PWR_REVERSE,   /*"潮流反向"*/
    E_EVT_PWR_REVERSE_LA,/*"A相功率反向"*/
    E_EVT_PWR_REVERSE_LB,/*"B相功率反向"*/
    E_EVT_PWR_REVERSE_LC,/*"C相功率反向"*/
    E_EVT_OVER_LOAD_LA,    /*"A相过载"*/
    E_EVT_OVER_LOAD_LB,    /*"B相过载"*/
    E_EVT_OVER_LOAD_LC,    /*"C相过载"*/
    E_EVT_OVER_DEMAND_PA,/*"正向有功需量超限"*/
    E_EVT_OVER_DEMAND_NA,/*"反向有功需量超限"*/
    E_EVT_OVER_DEMAND_I, /*"I象限无功需量超限"*/
    E_EVT_OVER_DEMAND_II,/*"II象限无功需量超限"*/
    E_EVT_OVER_DEMAND_III,/*"III象限无功需量超限"*/
    E_EVT_OVER_DEMAND_IV, /*"IV象限无功需量超限"*/
    E_EVT_VOLT_IMB,       /*"电压不平衡"*/
    E_EVT_CURR_IMB,       /*"电流不平衡"*/
    E_EVT_CURR_BADLY_IMB, /*"电流严重不平衡"*/
    E_EVT_OVER_FACTOR,    /*"总功率因数超下限"*/
    E_EVT_VOLT_REVERSE,   /*"电压逆相序"*/
    E_EVT_CURR_REVERSE,   /*"电流逆相序"*/
#endif
    E_MAX_GRID_EVNT
}EN_GRID_EVNT;

#define GRID_EVNT_NUM    ((E_MAX_GRID_EVNT >> 3) + 1)

#ifdef THREE_PHASE
typedef struct
{
    INT32U ckTime;      /*"电压检测时间"*/
    INT16U okRate;      /*"合格率"*/
    INT16U bdRate;      /*"超限率"*/
    INT32U upTime;     /*"电压超上限时间"*/
    INT32U dnTime;     /*"电压超下限时间"*/
}VOLTHEALTH;
typedef struct
{
    VOLTHEALTH Day[3];
    VOLTHEALTH Mon[3];
    CRC_DATA_TP Crc;
}VOLTHEALTH_DATA; 
#endif

/*"电表状态监测模块存EEPROM文件数据定义"*/
typedef struct
{
    INT32U NewEepChkFlg;    /*"新内卡检查标识字"*/
    INT16U FacAbleTime;    /*"厂内状态时间定时器"*/
    INT8U RelayActionNow;  /*"继电器当前理论状态"*/
    INT8U RelayErrFlg;     /*"负荷开关误动作发生标志"*/
    INT8U OpenCoverFlg[COVER_NUM];/*"开盖事件发生标志"*/
    INT8U GridEvntFlg[GRID_EVNT_NUM];/*"电网事件发生标志"*/
    INT32U EvntTimeAdd[E_MAX_GRID_EVNT];/*"电网事件累计时间"*/
    INT32U RstChkFlg;/*"电表复位检测标志"*/
#ifdef THREE_PHASE
    VOLTHEALTH_DATA voltHealthData; /*"电压合格率数据"*/
    INT8U magnetFlag; /*"恒定磁场事件标志"*/
    INT32U overMaxDemand[6];   /*"需量超限事件最大需量"*/
    ST_5TIME_FORM overMaxDemandTime[6];  /*"需量超限事件最大需量发生时间"*/
    INT16U maxImBVoltRate;
    INT16U maxImBCurrRate;
    INT16U maxBadlyImBCurrRate;
#endif
    INT8U muErrFlag;
    INT8U clkErrFlag;
    INT8U oscErrFlag;
    #ifdef GLX_CHG_CHK
    INT8U GlxChgFlag;
    INT8U GlxChgESamID[8];
    #endif
    #ifdef GLX_COMM_CHK
    INT8U GlxCommFlag;
    #endif
    INT32U pdCurrVolt[2];
}ST_MNT_FILE_DATA;

#define  NEW_EEP_CHK_FLG_SHIFT     FIND(ST_MNT_FILE_DATA, NewEepChkFlg)
#define  FAC_REMAIN_TIME           FIND(ST_MNT_FILE_DATA, FacAbleTime)
#define  RELAY_ACTION_NOW          FIND(ST_MNT_FILE_DATA, RelayActionNow)
#define  RELAY_ERR_FLG             FIND(ST_MNT_FILE_DATA, RelayErrFlg)
#define  GRID_EVNT_FLG_SHIFT       FIND(ST_MNT_FILE_DATA, GridEvntFlg)
#define  OPEN_COVER_FLG_SHIFT      FIND(ST_MNT_FILE_DATA, OpenCoverFlg)
#define  EVNT_TIME_ADD_SHIFT       FIND(ST_MNT_FILE_DATA, EvntTimeAdd)
#define  RST_CHK_FLG_SHIFT         FIND(ST_MNT_FILE_DATA, RstChkFlg)
#ifdef THREE_PHASE
#define  VOLTHEALTHDATA_SHIFT   FIND(ST_MNT_FILE_DATA, voltHealthData)
#define  MAGNET_FLG_SHIFT   FIND(ST_MNT_FILE_DATA, magnetFlag)
#define  OVERMAXDEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand)
#define  OVERMAXDEMANDTIME_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime)
#define  OVMXPADEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand[0])
#define  OVMXPADEMANDTM_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime[0])
#define  OVMXNADEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand[1])
#define  OVMXNADEMANDTM_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime[1])
#define  OVMXR1DEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand[2])
#define  OVMXR1DEMANDTM_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime[2])
#define  OVMXR2DEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand[3])
#define  OVMXR2DEMANDTM_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime[3])
#define  OVMXR3DEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand[4])
#define  OVMXR3DEMANDTM_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime[4])
#define  OVMXR4DEMAND_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemand[5])
#define  OVMXR4DEMANDTM_SHIFT   FIND(ST_MNT_FILE_DATA, overMaxDemandTime[5])
#define  MAXIMBVOLTRATE_SHIFT   FIND(ST_MNT_FILE_DATA, maxImBVoltRate)
#define  MAXIMBCURRRATE_SHIFT   FIND(ST_MNT_FILE_DATA, maxImBCurrRate)
#define  MAXBADLYIMBCURRRATE_SHIFT   FIND(ST_MNT_FILE_DATA, maxBadlyImBCurrRate)
#endif
#define  MUERR_FLG_SHIFT         FIND(ST_MNT_FILE_DATA, muErrFlag)
#define  CLKERR_FLG_SHIFT         FIND(ST_MNT_FILE_DATA, clkErrFlag)
#define  OSCERR_FLG_SHIFT         FIND(ST_MNT_FILE_DATA, oscErrFlag)
#ifdef GLX_CHG_CHK
#define  GLXCHG_FLG_SHIFT         FIND(ST_MNT_FILE_DATA, GlxChgFlag)
#define  GLXCHG_ESAMID_SHIFT         FIND(ST_MNT_FILE_DATA, GlxChgESamID[0])
#endif
#ifdef GLX_COMM_CHK
#define  GLXCOMM_FLG_SHIFT         FIND(ST_MNT_FILE_DATA, GlxCommFlag)
#endif
#define  PDCURRVOLT_SHIFT         FIND(ST_MNT_FILE_DATA, pdCurrVolt[0])

extern ST_MNT_PARA MntPara;
extern INT32U  EvntTimeAdd[E_MAX_GRID_EVNT];/*"事件累计时间"*/
extern INT16U  EvntTimeAddCrc;
extern INT8U  FacStatusFlg;
extern INT16U BatVolt[BAT_NUM];   /*"电池电压"*/
extern INT8U RelayStatusNow;/*"负荷开关当前状态 0 通   1断"*/
#ifdef THREE_PHASE
extern VOLTHEALTH_DATA voltHealthData;

extern INT16U mVolt[3];
extern INT16U mVoltMax;
extern INT16U mVoltMin;
extern INT16U mVoltBase;
extern INT32U mCurr[3];
extern INT32U mCurrMax;
extern INT32U mCurrMin;
extern INT32U mCurrBase;
extern INT32S mActivePower[4];
extern INT16U imBVoltRate;
extern INT16U imBCurrRate;
extern INT16U maxImBVoltRate;
extern INT16U maxImBCurrRate;
extern INT16U maxBadlyImBCurrRate;
extern INT16U uiState;
#define MODE3P3W  (uiState&0x01)   /*"bit值为0表示3P4W，为1表示3P3W"*/
#define NOLOAD       (uiState&0x02)   /*"bit值为0表示起动，为1表示潜动"*/
#define VOLTSEQU   (uiState&0x04)   /*"bit值为0表示正相序，为1表示逆相序"*/
#define CURRSEQU   (uiState&0x08)   /*"bit值为0表示正相序，为1表示逆相序"*/
#define BBREAK       (uiState&0x10)    /*"331 B相断相，bit值为0表示B相正常，为1表示B相断相"*/
#define BLOSTVOLT  (uiState&0x20)    /*"331 B相失压，bit值为0表示B相正常，为1表示B相失压"*/

extern INT32U mntPdLastTime;

extern INT8U magnetFlag; /*"恒定磁场事件标志"*/


typedef struct
{
    INT16U keyWord;  /*"低功耗下的特征字，0xA5A5"*/
    INT32S pdCurr[3];    /*"三相电流"*/
    ST_7TIME_FORM pdTime; /*"开始时间"*/
    CRC_DATA_TP crc;
}ST_PDCURR;
extern ST_PDCURR stPdCurr; /*"低功耗电流"*/

extern INT32U overMaxDemand[6];
extern ST_5TIME_FORM overMaxDemandTime[6];
extern INT32U overMaxDemandRead;

extern INT32U TotalLostVoltTime;     /*"三相失压累计时间"*/

extern NO_INIT INT64U  pwrRstMode;  /*"电源复位模式"*/
#define  PWR_RST_NORMAL  (0x9999EEEE)
#define  PWR_RST_PD           (0xAAAA5555)
#define  PWR_RST_BATT       (0x1111CCCC)

#endif

#ifdef THREE_PHASE
extern INT32S minActPower[4];        /*"每分钟平均有功功率 总ABC"*/
extern INT32S minReActPower[4];    /*"每分钟平均无功功率 总ABC"*/
#else
extern INT32S minActPower[2];        /*"每分钟平均有功功率 总"*/
#endif

extern INT8U mucRstInfo;

typedef struct
{
    INT16S gTmnlTmp[E_TEMP_MAX];
    INT32S PwrAmpErr;
    INT32S PwrPhsErr;
    INT16U Cs;
}MNT_UP_DATA; 

typedef struct
{
    INT8U EsamID[8];
    INT16U Cs;
}MNT_DOWN_DATA; 
extern MNT_UP_DATA mMntUpData;   /*"计量芯传给管理芯的数据"*/
extern MNT_DOWN_DATA mMntDownData;   /*"管理芯传给计量芯的数据"*/

#ifdef TMNL_TMP_CHK
extern INT16S gTmnlTmp[E_TEMP_MAX];
extern INT16S gMtrTmp;    /*"表内温度"*/
extern INT16U gMtrHmdty;  /*"表内湿度"*/
#endif

#ifdef GLX_CHG_CHK
extern INT8U gGlxChgEsamID[8];  /*"管理芯ESAM ID"*/
#endif

extern void MNT_Init(INT8U mode);
extern void FirstPwrOnChk(void);
extern INT8S MNT_PwrDown(void *pIn, void *pOut);
extern INT8S MNT_EvntClr(void *pIn, void *pOut);
extern INT8S MNT_OutFacCmd(void *pIn, void *pOut);
extern INT8U ChkMntParaValid(void);
extern void MNT_TaskPd(void);
extern INT8S MNT_FirstPwrUpOver(void *pIn, void *pOut);
extern INT8S MNT_ReSetFacRemainTime(void *pIn, void *pOut);
#ifdef THREE_PHASE
extern void MNT_RefreshVC(void);
extern INT8S MonVoltHealthClear(void *pIn, void *pOut);
extern INT8S DayVoltHealthClear(void *pIn, void *pOut);
#endif
extern void minPowerInit(void);
extern void minPowerSecTask(void);
extern void minPowerMinTask(void);
extern INT8S MNT_AlarmKeyAction(void *pIn, void *pOut);
extern INT8S MNT_PwrUpEvtInit(void *pIn, void *pOut);
extern INT8S MNT_EvtParaCal(void *pIn, void *pOut);
#ifdef THREE_PHASE
extern INT8S MNT_PDEvtTimeAdd(void *pIn, void *pOut);
#endif
extern INT8S MNT_UpDataSet(void *pIn, void *pOut);
extern INT8S MNT_DownDataGet(void *pIn, void *pOut);
#endif
