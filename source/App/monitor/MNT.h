/*"************************************************************"*/
/*"FileName: MNT_data.h"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: ����ģ������б���"*/
/*" ���ֽ���:          "*/
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
/*"----------�����¼����壨���ۼ�ʱ�䣩---------"*/
typedef enum
{
    E_EVT_PWR_DOWN = 0,   /*"����"*/
#ifdef THREE_PHASE
    E_EVT_LOST_VOLT_LA, /*"A��ʧѹ"*/
    E_EVT_LOST_VOLT_LB, /*"B��ʧѹ"*/
    E_EVT_LOST_VOLT_LC, /*"C��ʧѹ"*/
    E_EVT_LOST_VOLT_ALL,/*"ȫʧѹ"*/
    E_EVT_LOST_CURR_LA, /*"A��ʧ��"*/
    E_EVT_LOST_CURR_LB, /*"B��ʧ��"*/
    E_EVT_LOST_CURR_LC, /*"C��ʧ��"*/
    E_EVT_BREAK_PHASE_LA,/*"A�����"*/
    E_EVT_BREAK_PHASE_LB,/*"B�����"*/
    E_EVT_BREAK_PHASE_LC,/*"C�����"*/
#endif
#ifdef OVER_CURR_CHK
    E_EVT_OVER_CURR_LA,  /*"A�����"*/
#endif
#ifdef THREE_PHASE
    #ifdef OVER_CURR_CHK
    E_EVT_OVER_CURR_LB,  /*"B�����"*/
    E_EVT_OVER_CURR_LC,  /*"C�����"*/
    #endif
    E_EVT_FAIL_CURR_LA,  /*"A�����"*/
    E_EVT_FAIL_CURR_LB,  /*"B�����"*/
    E_EVT_FAIL_CURR_LC,  /*"C�����"*/
    E_EVT_OVER_VOLT_LA, /*"A���ѹ"*/
    E_EVT_OVER_VOLT_LB, /*"B���ѹ"*/
    E_EVT_OVER_VOLT_LC, /*"C���ѹ"*/
    E_EVT_UNDER_VOLT_LA,   /*"A��Ƿѹ"*/
    E_EVT_UNDER_VOLT_LB,   /*"B��Ƿѹ"*/
    E_EVT_UNDER_VOLT_LC,   /*"C��Ƿѹ"*/
    E_EVT_PWR_REVERSE,   /*"��������"*/
    E_EVT_PWR_REVERSE_LA,/*"A�๦�ʷ���"*/
    E_EVT_PWR_REVERSE_LB,/*"B�๦�ʷ���"*/
    E_EVT_PWR_REVERSE_LC,/*"C�๦�ʷ���"*/
    E_EVT_OVER_LOAD_LA,    /*"A�����"*/
    E_EVT_OVER_LOAD_LB,    /*"B�����"*/
    E_EVT_OVER_LOAD_LC,    /*"C�����"*/
    E_EVT_OVER_DEMAND_PA,/*"�����й���������"*/
    E_EVT_OVER_DEMAND_NA,/*"�����й���������"*/
    E_EVT_OVER_DEMAND_I, /*"I�����޹���������"*/
    E_EVT_OVER_DEMAND_II,/*"II�����޹���������"*/
    E_EVT_OVER_DEMAND_III,/*"III�����޹���������"*/
    E_EVT_OVER_DEMAND_IV, /*"IV�����޹���������"*/
    E_EVT_VOLT_IMB,       /*"��ѹ��ƽ��"*/
    E_EVT_CURR_IMB,       /*"������ƽ��"*/
    E_EVT_CURR_BADLY_IMB, /*"�������ز�ƽ��"*/
    E_EVT_OVER_FACTOR,    /*"�ܹ�������������"*/
    E_EVT_VOLT_REVERSE,   /*"��ѹ������"*/
    E_EVT_CURR_REVERSE,   /*"����������"*/
#endif
    E_MAX_GRID_EVNT
}EN_GRID_EVNT;

#define GRID_EVNT_NUM    ((E_MAX_GRID_EVNT >> 3) + 1)

#ifdef THREE_PHASE
typedef struct
{
    INT32U ckTime;      /*"��ѹ���ʱ��"*/
    INT16U okRate;      /*"�ϸ���"*/
    INT16U bdRate;      /*"������"*/
    INT32U upTime;     /*"��ѹ������ʱ��"*/
    INT32U dnTime;     /*"��ѹ������ʱ��"*/
}VOLTHEALTH;
typedef struct
{
    VOLTHEALTH Day[3];
    VOLTHEALTH Mon[3];
    CRC_DATA_TP Crc;
}VOLTHEALTH_DATA; 
#endif

/*"���״̬���ģ���EEPROM�ļ����ݶ���"*/
typedef struct
{
    INT32U NewEepChkFlg;    /*"���ڿ�����ʶ��"*/
    INT16U FacAbleTime;    /*"����״̬ʱ�䶨ʱ��"*/
    INT8U RelayActionNow;  /*"�̵�����ǰ����״̬"*/
    INT8U RelayErrFlg;     /*"���ɿ�������������־"*/
    INT8U OpenCoverFlg[COVER_NUM];/*"�����¼�������־"*/
    INT8U GridEvntFlg[GRID_EVNT_NUM];/*"�����¼�������־"*/
    INT32U EvntTimeAdd[E_MAX_GRID_EVNT];/*"�����¼��ۼ�ʱ��"*/
    INT32U RstChkFlg;/*"���λ����־"*/
#ifdef THREE_PHASE
    VOLTHEALTH_DATA voltHealthData; /*"��ѹ�ϸ�������"*/
    INT8U magnetFlag; /*"�㶨�ų��¼���־"*/
    INT32U overMaxDemand[6];   /*"���������¼��������"*/
    ST_5TIME_FORM overMaxDemandTime[6];  /*"���������¼������������ʱ��"*/
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
extern INT32U  EvntTimeAdd[E_MAX_GRID_EVNT];/*"�¼��ۼ�ʱ��"*/
extern INT16U  EvntTimeAddCrc;
extern INT8U  FacStatusFlg;
extern INT16U BatVolt[BAT_NUM];   /*"��ص�ѹ"*/
extern INT8U RelayStatusNow;/*"���ɿ��ص�ǰ״̬ 0 ͨ   1��"*/
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
#define MODE3P3W  (uiState&0x01)   /*"bitֵΪ0��ʾ3P4W��Ϊ1��ʾ3P3W"*/
#define NOLOAD       (uiState&0x02)   /*"bitֵΪ0��ʾ�𶯣�Ϊ1��ʾǱ��"*/
#define VOLTSEQU   (uiState&0x04)   /*"bitֵΪ0��ʾ������Ϊ1��ʾ������"*/
#define CURRSEQU   (uiState&0x08)   /*"bitֵΪ0��ʾ������Ϊ1��ʾ������"*/
#define BBREAK       (uiState&0x10)    /*"331 B����࣬bitֵΪ0��ʾB��������Ϊ1��ʾB�����"*/
#define BLOSTVOLT  (uiState&0x20)    /*"331 B��ʧѹ��bitֵΪ0��ʾB��������Ϊ1��ʾB��ʧѹ"*/

extern INT32U mntPdLastTime;

extern INT8U magnetFlag; /*"�㶨�ų��¼���־"*/


typedef struct
{
    INT16U keyWord;  /*"�͹����µ������֣�0xA5A5"*/
    INT32S pdCurr[3];    /*"�������"*/
    ST_7TIME_FORM pdTime; /*"��ʼʱ��"*/
    CRC_DATA_TP crc;
}ST_PDCURR;
extern ST_PDCURR stPdCurr; /*"�͹��ĵ���"*/

extern INT32U overMaxDemand[6];
extern ST_5TIME_FORM overMaxDemandTime[6];
extern INT32U overMaxDemandRead;

extern INT32U TotalLostVoltTime;     /*"����ʧѹ�ۼ�ʱ��"*/

extern NO_INIT INT64U  pwrRstMode;  /*"��Դ��λģʽ"*/
#define  PWR_RST_NORMAL  (0x9999EEEE)
#define  PWR_RST_PD           (0xAAAA5555)
#define  PWR_RST_BATT       (0x1111CCCC)

#endif

#ifdef THREE_PHASE
extern INT32S minActPower[4];        /*"ÿ����ƽ���й����� ��ABC"*/
extern INT32S minReActPower[4];    /*"ÿ����ƽ���޹����� ��ABC"*/
#else
extern INT32S minActPower[2];        /*"ÿ����ƽ���й����� ��"*/
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
extern MNT_UP_DATA mMntUpData;   /*"����о��������о������"*/
extern MNT_DOWN_DATA mMntDownData;   /*"����о��������о������"*/

#ifdef TMNL_TMP_CHK
extern INT16S gTmnlTmp[E_TEMP_MAX];
extern INT16S gMtrTmp;    /*"�����¶�"*/
extern INT16U gMtrHmdty;  /*"����ʪ��"*/
#endif

#ifdef GLX_CHG_CHK
extern INT8U gGlxChgEsamID[8];  /*"����оESAM ID"*/
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
