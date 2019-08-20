/*"************************************************************"*/
/*"FileName: MNT_data.c"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: ���ܱ�״̬���ģ������б���"*/
/*" ���ֽ���:          "*/
/*" Function List:                     "*/
/*"                                    "*/
/*" History:                           "*/
/*" <author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\monitor\MNT.h"
#include "APP\LegallyRelevant\MU_LIB.h"

#define EVT_START        (1)  /*"�¼���ʼ"*/
#define EVT_END          (0)  /*"�¼�����"*/
#define PWRUP_PD_E_SEC   (1)  /*"�͹����ϵ���жϵ��������ʱ ����λ��"*/
#define EVTENDNOW  (0x02)


typedef INT8U  (* PwrNetEvtChkFun)(INT8U evtId);

/*"�����¼���ʼ�����жϺ���"*/
typedef struct
{
    PwrNetEvtChkFun  EvtStartChk;
    PwrNetEvtChkFun  EvtEndChk;
}ST_EVT_CHK_FUN;

static INT8U mNetEvtSecAdd[E_MAX_GRID_EVNT];/*"�����¼��������������ʱ���ۼ�"*/
static INT8U mStNetEvtFlg[GRID_EVNT_NUM];/*"�����¼�������־"*/
#ifdef THREE_PHASE
INT16U avgVoltMin[3];   /*"ÿ���ӵ�ƽ����ѹ"*/
INT32U avgVoltMinAcc[3];
INT32U avgVoltMinAccTimes;
INT8U   avgVoltFlag=0;
NO_INIT VOLTHEALTH_DATA voltHealthData; /*"��ѹ�ϸ�������"*/
NO_INIT static VOLTHEALTH_DATA voltHealthDataBak; /*"��ѹ�ϸ��ʱ�������"*/
INT32U overMaxDemand[6];
ST_5TIME_FORM overMaxDemandTime[6];
INT32U overMaxDemandRead;
#endif

static void ChkPwrDir(void);
static void GE_NetEvtJudge(void);
static void NetEvtPwrUpInit(void);
static INT8U GE_JudgeEvtTime(INT8U enumId, INT8U startOrEnd);
void GE_EvtTimePwrUp(void);
void GE_PdEvtTimeInit(void);
static INT8U PwrDownEvtS(INT8U evtId);
static INT8U PwrDownEvtE(INT8U evtId);
#ifdef OVER_CURR_CHK
static INT8U OverCurrEvtS(INT8U evtId);
static INT8U OverCurrEvtE(INT8U evtId);
#endif
#ifdef THREE_PHASE
static INT8U LostVoltEvtS(INT8U evtId);
static INT8U LostVoltEvtE(INT8U evtId);
static INT8U LostVoltAllEvtS(INT8U evtId);
static INT8U LostVoltAllEvtE(INT8U evtId);
static INT8U LostCurrEvtS(INT8U evtId);
static INT8U LostCurrEvtE(INT8U evtId);
static INT8U BreakPhaseEvtS(INT8U evtId);
static INT8U BreakPhaseEvtE(INT8U evtId);
static INT8U CutCurrEvtS(INT8U evtId);
static INT8U CutCurrEvtE(INT8U evtId);
static INT8U OverVoltEvtS(INT8U evtId);
static INT8U OverVoltEvtE(INT8U evtId);
static INT8U UnderVoltEvtS(INT8U evtId);
static INT8U UnderVoltEvtE(INT8U evtId);
static INT8U PwrReverseAllEvtS(INT8U evtId);
static INT8U PwrReverseAllEvtE(INT8U evtId);
static INT8U PwrReverseEvtS(INT8U evtId);
static INT8U PwrReverseEvtE(INT8U evtId);
static INT8U OverLoadEvtS(INT8U evtId);
static INT8U OverLoadEvtE(INT8U evtId);
static INT8U mstDemandEvtS(INT8U evtId);
static INT8U mstDemandEvtE(INT8U evtId);
static INT8U VoltImBalanceEvtS(INT8U evtId);
static INT8U VoltImBalanceEvtE(INT8U evtId);
static INT8U CurrImBalanceEvtS(INT8U evtId);
static INT8U CurrImBalanceEvtE(INT8U evtId);
static INT8U CurrImBadlyBalanceEvtS(INT8U evtId);
static INT8U CurrImBadlyBalanceEvtE(INT8U evtId);
static INT8U OverFactorEvtS(INT8U evtId);
static INT8U OverFactorEvtE(INT8U evtId);
static INT8U VoltReverseEvtS(INT8U evtId);
static INT8U VoltReverseEvtE(INT8U evtId);
static INT8U CurrReverseEvtS(INT8U evtId);
static INT8U CurrReverseEvtE(INT8U evtId);
#endif
static void GE_NetEvtTimeAdd(void);
static void GE_SaveEvtSOrE( INT16U evtName, INT16U timeObis, INT8U stOrEnd);
#ifdef THREE_PHASE
void GE_RefreshLcd(void);
void  VoltHealthClear(void);
static void StPdCurrClear(void);
static INT8U StPdCurrChk(void);
void StPdCurrPdInit(void);
void StPdCurrSecTask(void);
extern INT32U GetMaxValue(INT32U X,INT32U Y,INT32U Z);
#endif

const ST_EVT_CHK_FUN  stEvtChkFun[E_MAX_GRID_EVNT] =
        {
                {PwrDownEvtS, PwrDownEvtE},/*"�����¼�"*/
		   #ifdef THREE_PHASE
		  {LostVoltEvtS, LostVoltEvtE},    /*"A��ʧѹ"*/
		  {LostVoltEvtS, LostVoltEvtE},    /*"B��ʧѹ"*/
		  {LostVoltEvtS, LostVoltEvtE},    /*"C��ʧѹ"*/
		  {LostVoltAllEvtS, LostVoltAllEvtE},    /*"ȫʧѹ"*/
		  {LostCurrEvtS, LostCurrEvtE},    /*"A��ʧ��"*/
		  {LostCurrEvtS, LostCurrEvtE},    /*"B��ʧ��"*/
		  {LostCurrEvtS, LostCurrEvtE},    /*"C��ʧ��"*/
		  {BreakPhaseEvtS, BreakPhaseEvtE},    /*"A�����"*/
		  {BreakPhaseEvtS, BreakPhaseEvtE},    /*"B�����"*/
		  {BreakPhaseEvtS, BreakPhaseEvtE},    /*"C�����"*/
		   #endif
                #ifdef OVER_CURR_CHK
                {OverCurrEvtS, OverCurrEvtE}, /*"�����¼�"*/
                #endif
		   #ifdef THREE_PHASE
                #ifdef OVER_CURR_CHK
		  {OverCurrEvtS, OverCurrEvtE},             /*"B�����"*/
                {OverCurrEvtS, OverCurrEvtE},             /*"C�����"*/
                #endif
                {CutCurrEvtS, CutCurrEvtE},             /*"A�����"*/
                {CutCurrEvtS, CutCurrEvtE},             /*"B�����"*/
                {CutCurrEvtS, CutCurrEvtE},             /*"C�����"*/
                {OverVoltEvtS, OverVoltEvtE},         /*"A���ѹ"*/
                {OverVoltEvtS, OverVoltEvtE},         /*"B���ѹ"*/
                {OverVoltEvtS, OverVoltEvtE},         /*"C���ѹ"*/
                {UnderVoltEvtS, UnderVoltEvtE},     /*"A��Ƿѹ"*/
                {UnderVoltEvtS, UnderVoltEvtE},     /*"B��Ƿѹ"*/
                {UnderVoltEvtS, UnderVoltEvtE},     /*"C��Ƿѹ"*/
                {PwrReverseAllEvtS, PwrReverseAllEvtE},     /*"��������"*/
                {PwrReverseEvtS, PwrReverseEvtE},     /*"A�๦�ʷ���"*/
                {PwrReverseEvtS, PwrReverseEvtE},     /*"B�๦�ʷ���"*/
                {PwrReverseEvtS, PwrReverseEvtE},     /*"C�๦�ʷ���"*/
                {OverLoadEvtS, OverLoadEvtE},    /*"A�����"*/
                {OverLoadEvtS, OverLoadEvtE},    /*"B�����"*/
                {OverLoadEvtS, OverLoadEvtE},    /*"C�����"*/
                {mstDemandEvtS, mstDemandEvtE},   /*"�����й���������"*/
                {mstDemandEvtS, mstDemandEvtE},   /*"�����й���������"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"I�����޹���������"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"II�����޹���������"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"III�����޹���������"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"IV�����޹���������"*/
                {VoltImBalanceEvtS, VoltImBalanceEvtE},  /*"��ѹ��ƽ��"*/
                {CurrImBalanceEvtS, CurrImBalanceEvtE},  /*"������ƽ��"*/
                {CurrImBadlyBalanceEvtS, CurrImBadlyBalanceEvtE},  /*"�������ز�ƽ��"*/
                {OverFactorEvtS, OverFactorEvtE},  /*"�ܹ�������������"*/
                {VoltReverseEvtS, VoltReverseEvtE},  /*"��ѹ������"*/
                {CurrReverseEvtS, CurrReverseEvtE},   /*"����������"*/
		   #endif
        };
const INT8U GridEvntStatusList[E_MAX_GRID_EVNT] = 
	{
	  E_PWR_DOWN, 
	  #ifdef THREE_PHASE
	  E_LOSS_VOLT_LA, /*"A��ʧѹ"*/
           E_LOSS_VOLT_LB, /*"B��ʧѹ"*/
           E_LOSS_VOLT_LC, /*"C��ʧѹ"*/
           E_LOSS_VOLT_ALL,/*"ȫʧѹ"*/
           E_LOSS_CURR_LA, /*"A��ʧ��"*/
           E_LOSS_CURR_LB, /*"B��ʧ��"*/
           E_LOSS_CURR_LC, /*"C��ʧ��"*/
           E_BRK_PHASE_LA,/*"A�����"*/
           E_BRK_PHASE_LB,/*"B�����"*/
           E_BRK_PHASE_LC,/*"C�����"*/
	  #endif
         #ifdef OVER_CURR_CHK
	  E_OVER_CURR_LA, /*"A�����"*/
	  #endif
	  #ifdef THREE_PHASE
           #ifdef OVER_CURR_CHK
	    E_OVER_CURR_LB, /*"B�����"*/
           E_OVER_CURR_LC, /*"C�����"*/
           #endif
           E_CUT_CURR_LA, /*"A�����"*/
           E_CUT_CURR_LB, /*"B�����"*/
           E_CUT_CURR_LC, /*"C�����"*/
           E_OVER_VOLT_LA, /*"A���ѹ"*/
           E_OVER_VOLT_LB, /*"B���ѹ"*/
           E_OVER_VOLT_LC, /*"C���ѹ"*/
           E_UNDER_VOLT_LA, /*"A��Ƿѹ"*/
           E_UNDER_VOLT_LB, /*"B��Ƿѹ"*/
           E_UNDER_VOLT_LC, /*"C��Ƿѹ"*/
           E_PWR_REVERSE, /*"��������"*/
           E_PWR_REVERSE_LA,/*"A�๦�ʷ���"*/
           E_PWR_REVERSE_LB,/*"B�๦�ʷ���"*/
           E_PWR_REVERSE_LC,/*"C�๦�ʷ���"*/
           E_OVER_LOAD_LA, /*"A�����"*/
           E_OVER_LOAD_LB, /*"B�����"*/
           E_OVER_LOAD_LC, /*"C�����"*/
           E_OVER_DEMAND_PA,/*"�����й���������"*/
           E_OVER_DEMAND_NA,/*"�����й���������"*/
           E_OVER_DEMAND_I, /*"I�����޹���������"*/
           E_OVER_DEMAND_II,/*"II�����޹���������"*/
           E_OVER_DEMAND_III,/*"III�����޹���������"*/
           E_OVER_DEMAND_IV, /*"IV�����޹���������"*/
           E_VOLT_IMBAL, /*"��ѹ��ƽ��"*/
           E_CURR_IMBAL, /*"������ƽ��"*/
           E_CURR_BADLY_IMBAL, /*"�������ز�ƽ��"*/
           E_OVER_FACTOR, /*"�ܹ�������������"*/
           E_VOLT_REVERSE, /*"��ѹ������"*/
           E_CURR_REVERSE, /*"����������"*/
	  #endif
	};
/*"************************************************"*/
/*"Function:    GE_Init"*/
/*"Description: �����¼���ʼ��"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
void GE_Init(void)
{
    INT32U RstChkFlg;
    LIB_MemSet(0 , (INT8U *)&mNetEvtSecAdd[0], E_MAX_GRID_EVNT);
    FS_ReadFile(E_FILE_MNT, GRID_EVNT_FLG_SHIFT, (INT8U *)&mStNetEvtFlg, GRID_EVNT_NUM);

    GE_EvtTimePwrUp();

    if(ChkStatus(E_MTR_RST) == TRUE)
    {
        INT8U i;
        /*"��¼��λ�¼�"*/
        SaveRcrd(E_SYS_RST_RCRD, E_EVTLST_SYS_RST_S);
        #if(MNT_VERSION == STATE_GRID)
        err_code_addr = 0;   /*��һ�£��Ա��´���*/
        #endif       
        for(i = 0; i < E_MAX_GRID_EVNT; i++)
        {
            if((mStNetEvtFlg[i >> 3] & (0x01 << (i & 0x07))) != 0)/*"��λ֮ǰ�¼�����"*/
            {
                Push((EN_MTR_STATUS)GridEvntStatusList[i], EVT_START);
            }
            else
            {
            }
        }
    }
    else
    {
        NetEvtPwrUpInit();
//#ifdef THREE_PHASE
        GetSingle(E_PD_TIME, &DCDataBuf[0]);
        GetSingle(E_SYS_TIME, &DCDataBuf[6]);
        #if(MNT_VERSION == STATE_GRID)
        SaveRcrd(E_PD_PON_RCRD, E_EVTLST_PD_PON_S);
        SaveRcrd(E_PD_PON_RCRD, E_EVTLST_PD_PON_E);
        #endif
//#endif
    }
    #ifdef THREE_PHASE
    StPdCurrClear();
    Push(E_PWR_REVERSE, EVT_END);
    #endif
    RstChkFlg = 0;
    FS_WriteFile(E_FILE_MNT, RST_CHK_FLG_SHIFT, (INT8U *) &RstChkFlg, 4);
}
/*"************************************************"*/
/*"Function:    GE_Main"*/
/*"Description: �����¼�������"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
void GE_Tsk1s(void)
{
    #ifdef THREE_PHASE
     MNT_RefreshVC();
    #endif
    GE_NetEvtJudge();
    #ifdef THREE_PHASE
     GE_RefreshLcd();
    #endif
    GE_NetEvtTimeAdd();
    ChkPwrDir();
}

/*"************************************************"*/
/*"Function:    ChkPwrDir"*/
/*"Description: ���������ʷ���"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
static void ChkPwrDir(void)
{
    INT8U PwrDir;
    INT8U i;
    INT8U mask;

    GetSingle(E_PWR_DIR, &PwrDir);
    mask = 0x01;

    for(i = 0; i < 8; i++)
    {
        if((PwrDir & mask) == 0)
        {
            Push((EN_MTR_STATUS)(E_PWR_Q_LA_DIR + i), 0);
        }
        else
        {
            Push((EN_MTR_STATUS)(E_PWR_Q_LA_DIR + i), 1);
        }
        mask <<= 1;
    }
    #ifdef SINGLE_PHASE
    #if(MNT_VERSION == SOUTH_GRID)
    if(TRUE == ChkStatus(E_PWR_P_DIR))/*"���й����ʷ���"*/
    {
        DIS_SetStauSeg(STA_REVERSE);
    }
    else
    {
        DIS_ClrStauSeg(STA_REVERSE);
    }
    #endif
    #endif
}
/*"************************************************"*/
/*"Function:   GE_EvtTimePwrUp "*/
/*"Description: �ϵ�ʱ�����¼��ۼ�ʱ�������"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
void GE_EvtTimePwrUp(void)
{
    INT8U i;
    INT8U j;
    INT8U ret1;
    INT8U ret2;
    INT32U  d[E_MAX_GRID_EVNT];

    if(EvntTimeAddCrc  == CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd)))
    {
       return;
    }
    
    for(i=0;i<3;i++)
    {
      ret1=FS_ReadFile(E_FILE_MNT, EVNT_TIME_ADD_SHIFT, (INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd));
      ret2=FS_ReadFile(E_FILE_MNT, EVNT_TIME_ADD_SHIFT, (INT8U *)&d[0] , sizeof(EvntTimeAdd));
      if((FALSE == ret1)||(FALSE == ret2))
      {
         continue;
      }
      for(j=0;j<E_MAX_GRID_EVNT;j++)
      {
          if(d[j] != EvntTimeAdd[j])
          {
             break;
          }
      }
      if(j >= E_MAX_GRID_EVNT)
      {
         break;
      }
    }

    EvntTimeAddCrc  = CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd))+1; /*"RAM�����ݶ�ʧ"*/
}

 typedef struct
{
    INT16U evtObis; /*"�����¼������ʶ"*/
    INT16U evtStartLstObis; /*"�¼��������������ʶ"*/
    INT16U evtEndLstObis; /*"�¼��������������ʶ"*/
}ST_EVT_INFO_LST;

/*"�¼���Ϣ�б�����ʱ����EN_NET_EVT�ṹ�ж�����¼�˳�򱣳�һ��"*/
const ST_EVT_INFO_LST mstNetEvtInfo[E_MAX_GRID_EVNT]=
        {
                {E_PWR_DOWN_EVNT_RCRD , E_EVTLST_PD_S , E_EVTLST_PD_E},     /*"����"*/
                #ifdef THREE_PHASE
		  {E_LOST_VOLT_LA_EVNT_RCRD , E_EVTLST_LOST_VOLT_S , E_EVTLST_LOST_VOLT_E},   /*"A��ʧѹ"*/
            {E_LOST_VOLT_LB_EVNT_RCRD , E_EVTLST_LOST_VOLT_S , E_EVTLST_LOST_VOLT_E},  /*"B��ʧѹ"*/
            {E_LOST_VOLT_LC_EVNT_RCRD , E_EVTLST_LOST_VOLT_S , E_EVTLST_LOST_VOLT_E},  /*"C��ʧѹ"*/
            {E_ALL_LOST_VOLT_EVNT_RCRD , E_EVTLST_ALL_LOST_VOLT_S , E_EVTLST_ALL_LOST_VOLT_E},  /*"ȫʧѹ"*/
            {E_LOST_CURR_LA_EVNT_RCRD , E_EVTLST_LOST_CURR_S , E_EVTLST_LOST_CURR_E},   /*"A��ʧ��"*/
            {E_LOST_CURR_LB_EVNT_RCRD , E_EVTLST_LOST_CURR_S , E_EVTLST_LOST_CURR_E},   /*"B��ʧ��"*/
            {E_LOST_CURR_LC_EVNT_RCRD , E_EVTLST_LOST_CURR_S , E_EVTLST_LOST_CURR_E},   /*"C��ʧ��"*/
            {E_BRK_PHASE_LA_EVNT_RCRD , E_EVTLST_BRK_PHASE_S , E_EVTLST_BRK_PHASE_E},  /*"A�����"*/
            {E_BRK_PHASE_LB_EVNT_RCRD , E_EVTLST_BRK_PHASE_S , E_EVTLST_BRK_PHASE_E},  /*"B�����"*/
            {E_BRK_PHASE_LC_EVNT_RCRD , E_EVTLST_BRK_PHASE_S , E_EVTLST_BRK_PHASE_E},  /*"C�����"*/
		  #endif
                #ifdef OVER_CURR_CHK
                {E_OVER_CURR_LA_EVNT_RCRD , E_EVTLST_OVERCURR_S , E_EVTLST_OVERCURR_E},   /*"A�����"*/
                #endif
                #ifdef THREE_PHASE
                #ifdef OVER_CURR_CHK
		  {E_OVER_CURR_LB_EVNT_RCRD , E_EVTLST_OVERCURR_S , E_EVTLST_OVERCURR_E},   /*"B�����"*/
            {E_OVER_CURR_LC_EVNT_RCRD , E_EVTLST_OVERCURR_S , E_EVTLST_OVERCURR_E},   /*"C�����"*/
            #endif
            {E_BRK_CURR_LA_EVNT_RCRD , E_EVTLST_BRK_CURR_S , E_EVTLST_BRK_CURR_E},  /*"A�����"*/
            {E_BRK_CURR_LB_EVNT_RCRD , E_EVTLST_BRK_CURR_S , E_EVTLST_BRK_CURR_E},  /*"B�����"*/
            {E_BRK_CURR_LC_EVNT_RCRD , E_EVTLST_BRK_CURR_S , E_EVTLST_BRK_CURR_E},  /*"C�����"*/
            {E_OVER_VOLT_LA_EVNT_RCRD , E_EVTLST_OVER_VOLT_S , E_EVTLST_OVER_VOLT_E},  /*"A���ѹ"*/
            {E_OVER_VOLT_LB_EVNT_RCRD , E_EVTLST_OVER_VOLT_S , E_EVTLST_OVER_VOLT_E},  /*"B���ѹ"*/
            {E_OVER_VOLT_LC_EVNT_RCRD , E_EVTLST_OVER_VOLT_S , E_EVTLST_OVER_VOLT_E},   /*"C���ѹ"*/
            {E_LACK_VOLT_LA_EVNT_RCRD , E_EVTLST_LACK_VOLT_S , E_EVTLST_LACK_VOLT_E},  /*"A��Ƿѹ"*/
            {E_LACK_VOLT_LB_EVNT_RCRD , E_EVTLST_LACK_VOLT_S , E_EVTLST_LACK_VOLT_E},   /*"B��Ƿѹ"*/
            {E_LACK_VOLT_LC_EVNT_RCRD , E_EVTLST_LACK_VOLT_S , E_EVTLST_LACK_VOLT_E},  /*"C��Ƿѹ"*/
            {E_PWR_REVERSE_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"��������"*/
            {E_PWR_REVERSE_LA_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"A�๦�ʷ���"*/
            {E_PWR_REVERSE_LB_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"B�๦�ʷ���"*/
            {E_PWR_REVERSE_LC_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"C�๦�ʷ���"*/
            {E_OVER_LOAD_LA_EVNT_RCRD , E_EVTLST_OVER_LOAD_S , E_EVTLST_OVER_LOAD_E},  /*"A�����"*/
            {E_OVER_LOAD_LB_EVNT_RCRD , E_EVTLST_OVER_LOAD_S , E_EVTLST_OVER_LOAD_E},  /*"B�����"*/
            {E_OVER_LOAD_LC_EVNT_RCRD , E_EVTLST_OVER_LOAD_S , E_EVTLST_OVER_LOAD_E},  /*"C�����"*/
            {E_OVER_PA_DEMAND_EVNT_RCRD , E_EVTLST_OVER_PA_DEMAND_S , E_EVTLST_OVER_PA_DEMAND_E},  /*"�����й���������"*/
            {E_OVER_NA_DEMAND_EVNT_RCRD , E_EVTLST_OVER_NA_DEMAND_S , E_EVTLST_OVER_NA_DEMAND_E},  /*"�����й���������"*/
            {E_OVER_R_DEMAND_1_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_1_E},  /*"I�����޹���������"*/
            {E_OVER_R_DEMAND_2_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_2_E},  /*"II�����޹���������"*/
            {E_OVER_R_DEMAND_3_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_3_E},  /*"III�����޹���������"*/
            {E_OVER_R_DEMAND_4_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_4_E},  /*"IV�����޹���������"*/
            {E_VOLT_IMBALANCE_EVNT_RCRD , E_EVTLST_VOLT_IMBALANCE_S , E_EVTLST_VOLT_IMBALANCE_E},  /*"��ѹ��ƽ��"*/
            {E_CURR_IMBALANCE_EVNT_RCRD , E_EVTLST_CURR_IMBALANCE_S , E_EVTLST_CURR_IMBALANCE_E},  /*"������ƽ��"*/
            {E_CURR_IMBALANCE_BADLY_EVNT_RCRD , E_EVTLST_CURR_IMBALANCE_BADLY_S , E_EVTLST_CURR_IMBALANCE_BADLY_E},  /*"�������ز�ƽ��"*/
            {E_FACTOR_LOW_EVNT_RCRD , E_EVTLST_FACTOR_LOW_S , E_EVTLST_FACTOR_LOW_E},  /*"�ܹ�������������"*/
            {E_VOLT_INVERSE_EVNT_RCRD , E_EVTLST_VOLT_INVERSE_S , E_EVTLST_VOLT_INVERSE_E},   /*"��ѹ������"*/
            {E_CURR_INVERSE_EVNT_RCRD , E_EVTLST_CURR_INVERSE_S , E_EVTLST_CURR_INVERSE_E},  /*"����������"*/
		  #endif
        };

/*"***********************************************************************"*/
/*"Function:    GE_SaveEvtSOrE"*/
/*"Description: ��¼�¼�������������洢�¼�������־        "*/
/*"Input:  evtName:�����¼����ƣ� �μ�EN_NET_EVT����                     "*/
/*"           timeObis:�¼��б���ʱ���DC_DATA_BUFȡֵʱʹ�ã�  "*/
/*"                         ͨ��ΪPD_TIME ��SYS_TIME     "*/
/*"           stOrEnd: _EVT_START�¼�������_EVT_END �¼�����                 "*/
/*"Output: ��                                                                                                            "*/
/*"Return: ��                                                                                                            "*/
/*"**********************************************************************"*/
static void GE_SaveEvtSOrE( INT16U evtEnumId, INT16U timeObis, INT8U stOrEnd)
{
    INT16U evtLstObis;

    if(stOrEnd == EVT_START)/*"��¼�¼���������"*/
    {
        evtLstObis = mstNetEvtInfo[evtEnumId].evtStartLstObis;
    }
    else/*"��¼�¼���������"*/
    {
        evtLstObis = mstNetEvtInfo[evtEnumId].evtEndLstObis;
    }

    if(EVT_START == stOrEnd)
    {
        GetSingle(timeObis, &DCDataBuf[0]);
    }
    else
    {
        GetSingle(timeObis, &DCDataBuf[6]);
    }

#ifdef THREE_PHASE
    if(evtEnumId != E_EVT_PWR_REVERSE)
#endif
    {
       if(EVT_START == stOrEnd)
       {
           mStNetEvtFlg[evtEnumId >> 3] |= (INT8U)(0x01 << (evtEnumId & 0x07)); /*"�¼���־��λ"*/
           Push((EN_MTR_STATUS)GridEvntStatusList[evtEnumId], EVT_START);
       }
       else
       {
           mStNetEvtFlg[evtEnumId >> 3] &= ~(INT8U)(0x01 << (evtEnumId & 0x07));
           Push((EN_MTR_STATUS)GridEvntStatusList[evtEnumId], EVT_END);
       }
	FS_WriteFile(E_FILE_MNT, GRID_EVNT_FLG_SHIFT + (evtEnumId >> 3), (INT8U *)&mStNetEvtFlg[evtEnumId >> 3], 1);
    }

    SaveRcrd(mstNetEvtInfo[evtEnumId].evtObis, evtLstObis);
    
    mNetEvtSecAdd[evtEnumId] = 0;

    /*"���ۼ�ʱ����¼��ڽ����¼�ʱ�洢�ۼ�ʱ�䵽eeprom"*/
    if(stOrEnd == EVT_END)
    {
        FS_WriteFile(E_FILE_MNT, EVNT_TIME_ADD_SHIFT + evtEnumId * 4, (INT8U *)&EvntTimeAdd[evtEnumId], 4);
    }
}

/*"************************************************"*/
/*"Function:  NetEvtPwrUpInit"*/
/*"Description:�����¼��ϵ紦����"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
static void NetEvtPwrUpInit(void)
{
    INT8U evtId;
    INT32S pdSec;
    ST_7TIME_FORM pdTime,nowTime;
    ST_7TIME_FORM evtStartTime[2];
#ifdef THREE_PHASE
    INT32U tmpCurrBase;
    GetSingle(E_NOMINAL_CURR, (INT8U *)&tmpCurrBase);
#endif
    INT8U evtTimeFlag;

    evtTimeFlag=FALSE;
    if(EvntTimeAddCrc  == CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd)))
    {
       evtTimeFlag=TRUE;
    }

    for(evtId = 0; evtId < E_MAX_GRID_EVNT; evtId++)
    {
        if(0 != (mStNetEvtFlg[evtId >> 3] & (0x01 << (evtId & 0x07))))
        {
#ifdef THREE_PHASE
	     if((E_EVT_PWR_DOWN != evtId)&&(E_EVT_LOST_VOLT_ALL != evtId))/*"���硢ȫʧѹ�¼��Ѿ���������ѭ�����ж��¼��Ƿ����"*/
#else
	     if(E_EVT_PWR_DOWN != evtId)/*"�����¼��Ѿ���������ѭ�����ж��¼��Ƿ����"*/
#endif
            {
                 if(evtTimeFlag == FALSE)
                 {
                        GetSingle((mstNetEvtInfo[evtId].evtObis- E_PWR_DOWN_EVNT_RCRD + E_RCRD_TIME_STATUS + 1), (INT8U *)&evtStartTime[0]);
                        GetSingle(E_PD_TIME, (INT8U *)&nowTime);
                        pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&evtStartTime[0]);
          	          if(pdSec > 0)
          	          {
                            EvntTimeAdd[evtId] += pdSec;
          	          }
                 }
		   GE_SaveEvtSOrE(evtId, E_PD_TIME, EVT_END);
            }
	     else
	     {
                 FS_ReadFile(E_FILE_MNT, EVNT_TIME_ADD_SHIFT+(evtId*sizeof(INT32U)), (INT8U *)&EvntTimeAdd[evtId] , sizeof(INT32U));
                 GetSingle((mstNetEvtInfo[evtId].evtObis- E_PWR_DOWN_EVNT_RCRD + E_RCRD_TIME_STATUS + 1), (INT8U *)&evtStartTime[0]);
                 GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
                 pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&evtStartTime[0]);
   	          if(pdSec > 0)
   	          {
                     EvntTimeAdd[evtId] += pdSec;
   	          }
	         Push((EN_MTR_STATUS)GridEvntStatusList[evtId], EVT_START);
	     }
        }
        else
        {
#ifdef THREE_PHASE
	      if(((E_EVT_PWR_DOWN == evtId) || (E_EVT_LOST_VOLT_ALL == evtId))
             &&(0 == (mStNetEvtFlg[E_EVT_PWR_DOWN >> 3] & (0x01 << (E_EVT_PWR_DOWN & 0x07))))
             &&(0 == (mStNetEvtFlg[E_EVT_LOST_VOLT_ALL >> 3] & (0x01 << (E_EVT_LOST_VOLT_ALL & 0x07)))))
             {
 		 if(StPdCurrChk() == TRUE)
 		 {
 		     if((LIB_ABS(stPdCurr.pdCurr[0])*20 > tmpCurrBase)||(LIB_ABS(stPdCurr.pdCurr[1])*20 > tmpCurrBase)
 		        ||LIB_ABS(stPdCurr.pdCurr[2])*20 > tmpCurrBase)
 		     {
 		        if( E_EVT_LOST_VOLT_ALL == evtId)
 		        {
                           GE_SaveEvtSOrE(evtId, E_TLV_TIME, EVT_START);
 			      GetSingle(E_TLV_TIME, (INT8U *)&pdTime);
                           GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
                           pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&pdTime);
			      if(pdSec > 0)
			      {
                              EvntTimeAdd[evtId] += pdSec;
			      }
 		        }
 		     }
 		     else
 		     {
 		        if( E_EVT_PWR_DOWN == evtId)
 		        {
 		            GE_SaveEvtSOrE(evtId, E_TLV_TIME, EVT_START);
 			     GetSingle(E_TLV_TIME, (INT8U *)&pdTime);
                           GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
                           pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&pdTime);
			      if(pdSec > 0)
			      {
                              EvntTimeAdd[evtId] += pdSec;
			      }
 		        }
 		     }
 		 }
		 else
		 {
		     if(E_EVT_PWR_DOWN == evtId)
		     {
		        GetSingle(E_PD_TIME, (INT8U *)&pdTime);
			 GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
                      pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&pdTime);
			 if(pdSec >= 60)
			 {
			   GE_SaveEvtSOrE(evtId, E_PD_TIME, EVT_START);
			   EvntTimeAdd[evtId] += pdSec;
			 }
		     }
		 }
             }
#else
	     if(E_EVT_PWR_DOWN == evtId)
            {
                /*"��¼�����¼�����"*/
		 GetSingle(E_PD_TIME, (INT8U *)&pdTime);
		 GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
               pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&pdTime);
		 if(pdSec > 0)
 	        {
                   EvntTimeAdd[evtId] += pdSec;
 	        }
               INT32U d[2];
               INT32U t[2];

              t[0]=stMmtData.volt[0];
              t[1]=stMmtData.curr[0];
              FS_ReadFile(E_FILE_MNT, PDCURRVOLT_SHIFT,  (INT8U *)&d[0], 8);
              stMmtData.volt[0]=d[0];
              stMmtData.curr[0]=d[1];
              GE_SaveEvtSOrE(E_EVT_PWR_DOWN,  E_PD_TIME,  EVT_START);
              stMmtData.volt[0]=t[0];
              stMmtData.curr[0]=t[1];
            }
#endif
        }
    }

    EvntTimeAddCrc  = CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd));
}

void GE_PdEvtTimeInit(void)
{
    FS_ReadFile(E_FILE_MNT, GRID_EVNT_FLG_SHIFT, (INT8U *)&mStNetEvtFlg, GRID_EVNT_NUM);
    GE_EvtTimePwrUp();
}

#ifdef SINGLE_PHASE
/*"************************************************"*/
/*"Function:    PwrDownEvtS"*/
/*"Description: �����¼���ʼ�ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE ������ʼ����     _FALSE ��������ʼ����"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U PwrDownEvtS(INT8U evtId)
{
    if(FALSE == ChkStatus(E_VOLT_NORMAL))
    {
        return TRUE;
    }

    return  FALSE;
}

/*"************************************************"*/
/*"Function:    PwrDownEvtE"*/
/*"Description: �����¼������ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE �����������     _FALSE �������������"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U PwrDownEvtE(INT8U evtId)
{
    if(TRUE == ChkStatus(E_VOLT_NORMAL))
    {
        return TRUE;
    }
    return  FALSE;
}

#ifdef OVER_CURR_CHK
/*"************************************************"*/
/*"Function:    OverCurrEvtS"*/
/*"Description: �����¼���ʼ�ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE ������ʼ����     _FALSE ��������ʼ����"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U OverCurrEvtS(INT8U evtId)
{
    INT32S lineCurr;
    INT16U CurrObis;
    INT32U CurrABS;

    CurrObis = E_CURR_LA + (evtId - E_EVT_OVER_CURR_LA);
    GetSingle(CurrObis, (INT8U *)&lineCurr);
    CurrABS = LIB_ABS(lineCurr);

    if((0 != MntPara.OverCurrLimitIL) && (CurrABS > MntPara.OverCurrLimitIL ))
    {
        return TRUE;
    }

    return  FALSE;
}

/*"************************************************"*/
/*"Function:    OverCurrEvtE"*/
/*"Description: �����¼������ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE �����������     _FALSE �������������"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U OverCurrEvtE(INT8U evtId)
{
    if(MntPara.OverCurrLimitIL == 0)
    {
       return EVTENDNOW;
    }
    
    if(FALSE == OverCurrEvtS(evtId))
    {
        return TRUE;
    }
    return  FALSE;
}
#endif
#else
/*"************************************************"*/
/*"Function:    PwrDownEvtS"*/
/*"Description: �����¼���ʼ�ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE ������ʼ����     _FALSE ��������ʼ����"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U PwrDownEvtS(INT8U evtId)
{
    if(ChkStatus(E_LOSS_VOLT_ALL) == TRUE)
    {
       return FALSE;
    }
    
    if((mVoltMax*((INT32U)100)<mVoltBase*((INT32U)60))&&(mCurrMax*((INT32U)100)<mCurrBase*((INT32U)5)))
    {
        return TRUE;
    }

    return  FALSE;
}

/*"************************************************"*/
/*"Function:    PwrDownEvtE"*/
/*"Description: �����¼������ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE �����������     _FALSE �������������"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U PwrDownEvtE(INT8U evtId)
{
    if(TRUE == PwrDownEvtS(evtId))
    {
        return FALSE;
    }
    return  TRUE;
}
#ifdef OVER_CURR_CHK
/*"************************************************"*/
/*"Function:    OverCurrEvtS"*/
/*"Description: �����¼���ʼ�ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE ������ʼ����     _FALSE ��������ʼ����"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U OverCurrEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_OVER_CURR_LB))
    {
       return FALSE;
    }
     
    evtId=evtId-E_EVT_OVER_CURR_LA;
    
    if((0 != MntPara.OverCurrLimitIL) && (mCurr[evtId] > MntPara.OverCurrLimitIL ))
    {
        return TRUE;
    }

    return  FALSE;
}

/*"************************************************"*/
/*"Function:    OverCurrEvtE"*/
/*"Description: �����¼������ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE �����������     _FALSE �������������"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U OverCurrEvtE(INT8U evtId)
{
    if(MntPara.OverCurrLimitIL == 0)
    {
       return EVTENDNOW;
    }
	
    if(FALSE == OverCurrEvtS(evtId))
    {
        return TRUE;
    }
    return  FALSE;
}
#endif
/*"************************************************"*/
/*"Function:    LostVoltEvtS"*/
/*"Description: ʧѹ�¼���ʼ�ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE ������ʼ����     _FALSE ��������ʼ����"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U LostVoltEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_LOST_VOLT_LB))
    {
       return FALSE;
    }
    evtId=evtId-E_EVT_LOST_VOLT_LA;

    if((ChkStatus(E_LOSS_VOLT_ALL) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
    {
       return FALSE;
    }

    if((0 != MntPara.LossVoltLimitVU) && (mVolt[evtId] < MntPara.LossVoltLimitVU) && (mCurr[evtId] > MntPara.LossVoltLimitIL ))
    {
        return TRUE;
    }

    return  FALSE;
}

/*"************************************************"*/
/*"Function:    LostVoltEvtE"*/
/*"Description: ʧѹ�¼������ж�����"*/
/*"Input:evtId  �����¼����"*/
/*"Output: ��"*/
/*"Return: _TRUE �����������     _FALSE �������������"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U LostVoltEvtE(INT8U evtId)
{
    evtId=evtId-E_EVT_LOST_VOLT_LA;

    if((MntPara.LossVoltLimitVU == 0)||(ChkStatus(E_LOSS_VOLT_ALL) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
    {
       return EVTENDNOW;
    }

    if( (mVolt[evtId] > MntPara.LossVoltLimitVL) || (mCurr[evtId] < MntPara.LossVoltLimitIL ))
    {
        return TRUE;
    }

    return  FALSE;
}
static INT8U LostVoltAllEvtS(INT8U evtId)
{
    if(ChkStatus(E_PWR_DOWN) == TRUE)
    {
       return FALSE;
    }

    if((mVoltMax*((INT32U)100)<mVoltBase*((INT32U)60))&&(mCurrMax*((INT32U)100) >= mCurrBase*((INT32U)5)))
    {
        return TRUE;
    }

    return  FALSE;
}
static INT8U LostVoltAllEvtE(INT8U evtId)
{
    if(LostVoltAllEvtS(evtId) == FALSE)
    {
        return TRUE;
    }
	
    return  FALSE;
}
static INT8U LostCurrEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_LOST_CURR_LB))
    {
       return FALSE;
    }
  
    evtId=evtId-E_EVT_LOST_CURR_LA;
	
    if((0 != MntPara.LossCurrLimitIU) && (mCurr[evtId] < MntPara.LossCurrLimitIU)
	&& (mCurrMax > MntPara.LossCurrLimitIL )&& (mVolt[evtId] > MntPara.LossCurrLimitVL ))
    {
        return TRUE;
    }
	
    return  FALSE;
}
static INT8U LostCurrEvtE(INT8U evtId)
{
    if(MntPara.LossCurrLimitIU == 0)
    {
       return EVTENDNOW;
    }
	
    if(LostCurrEvtS(evtId) == FALSE)
    {
       return  TRUE;
    }
	
    return  FALSE;
}
static INT8U BreakPhaseEvtS(INT8U evtId)
{
   if((MODE3P3W)&&(evtId == E_EVT_BREAK_PHASE_LB))
    {
       return FALSE;
    }

   evtId=evtId-E_EVT_BREAK_PHASE_LA;

    if((ChkStatus(E_LOSS_VOLT_ALL) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
    {
       return FALSE;
    }
	
    if((0 != MntPara.BrkPhaseLimitVU) && (mVolt[evtId] < MntPara.BrkPhaseLimitVU) && (mCurr[evtId] < MntPara.BrkPhaseLimitIU ))
    {
        return TRUE;
    }
	
    return  FALSE;
}
static INT8U BreakPhaseEvtE(INT8U evtId)
{
    if((MntPara.BrkPhaseLimitVU == 0)||(ChkStatus(E_LOSS_VOLT_ALL) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
    {
       return EVTENDNOW;
    }
	
    if(BreakPhaseEvtS(evtId) == FALSE)
    {
       return  TRUE;
    }
	
    return  FALSE;
}
static INT8U CutCurrEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_FAIL_CURR_LB))
    {
       return FALSE;
    }
	
    evtId=evtId-E_EVT_FAIL_CURR_LA;
	
    if((0 != MntPara.FailCurrLimitIU) && (mVolt[evtId] > MntPara.CutCurrLimitVL) && (mCurr[evtId] < MntPara.FailCurrLimitIU ))
    {
        return TRUE;
    }
	
    return  FALSE;
}
static INT8U CutCurrEvtE(INT8U evtId)
{
    if(MntPara.FailCurrLimitIU == 0)
    {
       return EVTENDNOW;
    }
    
    if(CutCurrEvtS(evtId) == FALSE)
    {
       return  TRUE;
    }
	
    return  FALSE;
}
static INT8U OverVoltEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_OVER_VOLT_LB))
    {
       return FALSE;
    }
	
    evtId=evtId-E_EVT_OVER_VOLT_LA;
	
    if((0 != MntPara.OverVoltLimitVL) && (mVolt[evtId] > MntPara.OverVoltLimitVL))
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U OverVoltEvtE(INT8U evtId)
{
   if(MntPara.OverVoltLimitVL == 0)
   {
       return EVTENDNOW;
   }
   
   if(OverVoltEvtS(evtId) == FALSE)
   {
       return  TRUE;
   }
   
    return  FALSE;
}
static INT8U UnderVoltEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_UNDER_VOLT_LB))
    {
       return FALSE;
    }
    
    evtId=evtId-E_EVT_UNDER_VOLT_LA;
	
    if((0 != MntPara.UnderVoltLimitVU) && (mVolt[evtId] < MntPara.UnderVoltLimitVU))
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U UnderVoltEvtE(INT8U evtId)
{
   if(MntPara.UnderVoltLimitVU == 0)
   {
       return EVTENDNOW;
   }
   
   if(UnderVoltEvtS(evtId) == FALSE)
   {
       return  TRUE;
   }
    return  FALSE;
}

#define TPWRUNKOWN   (0)
#define TPWRACTIVE     (1)
#define TPWRNEGTIVE   (2)
#define DEMAND_CLR_DELAY (1)  /*������ʱ��*/
#define MODE_CLR_CUR_DEMAND  0  /*Ĭ����ʱ��0ģʽ��1��0*/

static INT8U pwrReverseTFlag=TPWRUNKOWN;
static INT8U pwrReverseTACnt=0;
static INT8U pwrReverseTNCnt=0;
static INT8U PwrReverseAllEvtS(INT8U evtId)
{
    INT32U activePowerABS;
    INT8U DemandMode;    
    
    activePowerABS=LIB_ABS(mActivePower[0]);

    if((MntPara.PhasePwrReverseLimitP > 0)&&(activePowerABS > MntPara.PhasePwrReverseLimitP))
    {
        GetSingle(E_EXT_MODE1, &DemandMode);  /*b4 ��������������0(0)/����������ʱ��0(1)*/
        DemandMode = DemandMode&0x10;

        if(mActivePower[0] > 0)
        {
            pwrReverseTACnt++;
	        pwrReverseTNCnt=0;
	        if(pwrReverseTACnt >= MntPara.PwrReverseDelayT)
	        {
               pwrReverseTACnt=MntPara.PwrReverseDelayT;
			  
		       if(pwrReverseTFlag == TPWRNEGTIVE)
    	       {
    	          pwrReverseTFlag=TPWRACTIVE;

    		      Push(E_PWR_REVERSE, EVT_END);

                  if((MODE_CLR_CUR_DEMAND == DemandMode) ||(MntPara.PwrReverseDelayT <= 1))
                  {  
    		         Action(E_CURRENT_DEMAND_CLR, NULL, NULL);  /*�嵱ǰ����*/
                  }

    		      return TRUE;
    	       }
        	   else
        	   {
        		   pwrReverseTFlag=TPWRACTIVE;
        	   }
	        }
        }
	    else
    	{
    	   pwrReverseTACnt=0;
    	   pwrReverseTNCnt++;
    	   if(pwrReverseTNCnt >= MntPara.PwrReverseDelayT)
    	    {
                pwrReverseTNCnt=MntPara.PwrReverseDelayT;
    			  
    		    if(pwrReverseTFlag == TPWRACTIVE)
    	        {
    	           pwrReverseTFlag=TPWRNEGTIVE;

    		       Push(E_PWR_REVERSE, EVT_START);
                   
                   if((MODE_CLR_CUR_DEMAND == DemandMode) ||(MntPara.PwrReverseDelayT <= 1)) 
                   { 
    		           Action(E_CURRENT_DEMAND_CLR, NULL, NULL);  /*�嵱ǰ����*/
                   }

    		       return TRUE;
    	       }
        	   else
        	   {
        		   pwrReverseTFlag=TPWRNEGTIVE;
        	   }
    	    }
    	}

        /*"Ϊ1ʱ������ı�ʱ��������"*/
        if(MODE_CLR_CUR_DEMAND != DemandMode) 
        {
             if((DEMAND_CLR_DELAY==pwrReverseTACnt)||(DEMAND_CLR_DELAY==pwrReverseTNCnt))
             {
                 Action(E_CURRENT_DEMAND_CLR, NULL, NULL);  /*�嵱ǰ����*/ 
             }
        }    
    }
    else
    {
        pwrReverseTACnt=0;
	    pwrReverseTNCnt=0;
    }
    
    return  FALSE;
}
static INT8U PwrReverseAllEvtE(INT8U evtId)
{
    return  FALSE;
}
static INT8U PwrReverseEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_PWR_REVERSE_LB))
    {
       return FALSE;
    }
    
    evtId=evtId-E_EVT_PWR_REVERSE_LA+1;
	
    if((0 != MntPara.PhasePwrReverseLimitP)  && (mActivePower[evtId] <0)   && (mVolt[evtId-1] >100)
	&& (LIB_ABS(mActivePower[evtId]) > MntPara.PhasePwrReverseLimitP))
    {
        return TRUE;
    }
	
    return  FALSE;
}
static INT8U PwrReverseEvtE(INT8U evtId)
{
    if(MntPara.PhasePwrReverseLimitP == 0)
    {
        return EVTENDNOW;
    }
	
    evtId=evtId-E_EVT_PWR_REVERSE_LA+1;
	
    if( (mActivePower[evtId] >= 0) && (mVolt[evtId-1] >100) )
    {
        return TRUE;
    }
	
    return  FALSE;
}
static INT8U OverLoadEvtS(INT8U evtId)
{
    if((MODE3P3W)&&(evtId == E_EVT_OVER_LOAD_LB))
    {
       return FALSE;
    }
    
    evtId=evtId-E_EVT_OVER_LOAD_LA+1;
	
    if((0 != MntPara.UpLoadLimitP) && (LIB_ABS(mActivePower[evtId]) > MntPara.UpLoadLimitP))
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U OverLoadEvtE(INT8U evtId)
{
   if(MntPara.UpLoadLimitP == 0)
   {
        return EVTENDNOW;
   }
   
   if(OverLoadEvtS(evtId) == FALSE)
   {
       return  TRUE;
   }
   return  FALSE;
}
static INT8U mstDemandEvtS(INT8U evtId)
{
     INT32U nowDemand;
     INT32U nowDemandThr;
     INT32U *maxDemand;
     ST_5TIME_FORM *maxDemandTime;
     ST_7TIME_FORM tempTime;

     switch(evtId)
     {
         case E_EVT_OVER_DEMAND_PA:
	    GetSingle(E_CURRENT_PA_DEMAND, (INT8U *)&nowDemand);
	    nowDemandThr=MntPara.AAOverDemandLimitP;
	    maxDemand=&overMaxDemand[0];
	    maxDemandTime=&overMaxDemandTime[0];
	  break;

	  case E_EVT_OVER_DEMAND_NA:
	    GetSingle(E_CURRENT_NA_DEMAND, (INT8U *)&nowDemand);
	    nowDemandThr=MntPara.NAOverDemandLimitP;
	    maxDemand=&overMaxDemand[1];
	    maxDemandTime=&overMaxDemandTime[1];
	  break;

	  case E_EVT_OVER_DEMAND_I:
	    GetSingle(E_CURRENT_I_R_DEMAND, (INT8U *)&nowDemand);
	    nowDemandThr=MntPara.OverDemandLimitQ;
	    maxDemand=&overMaxDemand[2];
	    maxDemandTime=&overMaxDemandTime[2];
	  break;

	  case E_EVT_OVER_DEMAND_II:
	    GetSingle(E_CURRENT_II_R_DEMAND, (INT8U *)&nowDemand);
	    nowDemandThr=MntPara.OverDemandLimitQ;
	    maxDemand=&overMaxDemand[3];
	    maxDemandTime=&overMaxDemandTime[3];
	  break;

	  case E_EVT_OVER_DEMAND_III:
	    GetSingle(E_CURRENT_IIII_R_DEMAND, (INT8U *)&nowDemand);
	    nowDemandThr=MntPara.OverDemandLimitQ;
	    maxDemand=&overMaxDemand[4];
	    maxDemandTime=&overMaxDemandTime[4];
	  break;

	  case E_EVT_OVER_DEMAND_IV:
	    GetSingle(E_CURRENT_IV_R_DEMAND, (INT8U *)&nowDemand);
	    nowDemandThr=MntPara.OverDemandLimitQ;
	    maxDemand=&overMaxDemand[5];
	    maxDemandTime=&overMaxDemandTime[5];
	  break;

	  default:
	    return  FALSE;
	  break;
     }

     nowDemandThr=nowDemandThr*100;

     if((nowDemandThr > 0)&& (nowDemand >= nowDemandThr))
     {
        *maxDemand=nowDemand;
	 GetSingle(E_SYS_TIME, (INT8U *)&tempTime);
	 LIB_MemCpy((INT8U *)&tempTime, (INT8U *)maxDemandTime, 5);
	 FS_WriteFile(E_FILE_MNT, OVERMAXDEMAND_SHIFT+(evtId-E_EVT_OVER_DEMAND_PA)*4, (INT8U *)maxDemand, 4);
	 FS_WriteFile(E_FILE_MNT, OVERMAXDEMANDTIME_SHIFT+(evtId-E_EVT_OVER_DEMAND_PA)*sizeof(ST_5TIME_FORM), 
	 	         (INT8U *)maxDemandTime, sizeof(ST_5TIME_FORM));
	 return TRUE;
     }

     return  FALSE;
}
static INT8U mstDemandEvtE(INT8U evtId)
{
    INT32U nowDemand;
    INT32U nowDemandThr;
    INT32U *maxDemand;
    ST_5TIME_FORM *maxDemandTime;
    ST_7TIME_FORM tempTime;
   
    switch(evtId)
    {
      case E_EVT_OVER_DEMAND_PA:
        GetSingle(E_CURRENT_PA_DEMAND, (INT8U *)&nowDemand);
        nowDemandThr=MntPara.AAOverDemandLimitP;
        maxDemand=&overMaxDemand[0];
        maxDemandTime=&overMaxDemandTime[0];
     break;
   
     case E_EVT_OVER_DEMAND_NA:
       GetSingle(E_CURRENT_NA_DEMAND, (INT8U *)&nowDemand);
       nowDemandThr=MntPara.NAOverDemandLimitP;
       maxDemand=&overMaxDemand[1];
       maxDemandTime=&overMaxDemandTime[1];
     break;
   
     case E_EVT_OVER_DEMAND_I:
       GetSingle(E_CURRENT_I_R_DEMAND, (INT8U *)&nowDemand);
       nowDemandThr=MntPara.OverDemandLimitQ;
       maxDemand=&overMaxDemand[2];
       maxDemandTime=&overMaxDemandTime[2];
     break;
   
     case E_EVT_OVER_DEMAND_II:
       GetSingle(E_CURRENT_II_R_DEMAND, (INT8U *)&nowDemand);
       nowDemandThr=MntPara.OverDemandLimitQ;
       maxDemand=&overMaxDemand[3];
       maxDemandTime=&overMaxDemandTime[3];
     break;
   
     case E_EVT_OVER_DEMAND_III:
       GetSingle(E_CURRENT_IIII_R_DEMAND, (INT8U *)&nowDemand);
       nowDemandThr=MntPara.OverDemandLimitQ;
       maxDemand=&overMaxDemand[4];
       maxDemandTime=&overMaxDemandTime[4];
     break;
   
     case E_EVT_OVER_DEMAND_IV:
       GetSingle(E_CURRENT_IV_R_DEMAND, (INT8U *)&nowDemand);
       nowDemandThr=MntPara.OverDemandLimitQ;
       maxDemand=&overMaxDemand[5];
       maxDemandTime=&overMaxDemandTime[5];
     break;
   
     default:
       return  FALSE;
     break;
    }

    nowDemandThr=nowDemandThr*100;

    if(nowDemandThr == 0)
    {
       return EVTENDNOW;
    }
    if(nowDemand >= nowDemandThr)
    {
        if(nowDemand > *maxDemand)
        {
            *maxDemand=nowDemand;
            GetSingle(E_SYS_TIME, (INT8U *)&tempTime);
            LIB_MemCpy((INT8U *)&tempTime, (INT8U *)maxDemandTime, 5);
            FS_WriteFile(E_FILE_MNT, OVERMAXDEMAND_SHIFT+(evtId-E_EVT_OVER_DEMAND_PA)*4, (INT8U *)maxDemand, 4);
            FS_WriteFile(E_FILE_MNT, OVERMAXDEMANDTIME_SHIFT+(evtId-E_EVT_OVER_DEMAND_PA)*sizeof(ST_5TIME_FORM), 
            (INT8U *)maxDemandTime, sizeof(ST_5TIME_FORM));
        }
	 return FALSE;
    }
   
    return  TRUE;
}
static INT8U VoltImBalanceEvtS(INT8U evtId)
{
    if((0 != MntPara.VoltImbalLimitRatio) && (mVoltMax*((INT32U)100) > mVoltBase*((INT32U)60))
	&& (imBVoltRate> MntPara.VoltImbalLimitRatio))
    {
        maxImBVoltRate=imBVoltRate;
	 FS_WriteFile(E_FILE_MNT, MAXIMBVOLTRATE_SHIFT, (INT8U *)&maxImBVoltRate, sizeof(maxImBVoltRate));
	 return TRUE;
    }
    return  FALSE;
}
static INT8U VoltImBalanceEvtE(INT8U evtId)
{
   ST_7TIME_FORM tempTime;
   INT16U d;
   
   if(MntPara.VoltImbalLimitRatio == 0)
   {
        return EVTENDNOW;
   }
    if((mVoltMax*((INT32U)100) > mVoltBase*((INT32U)60)) && (imBVoltRate> MntPara.VoltImbalLimitRatio))
    {
        if(imBVoltRate > maxImBVoltRate)
        {
           maxImBVoltRate=imBVoltRate;
        }
	 GetSingle(E_SYS_TIME, (INT8U *)&tempTime);
	 if(tempTime.sec == 0)
	 {
	    FS_ReadFile(E_FILE_MNT, MAXIMBVOLTRATE_SHIFT, (INT8U *)&d, sizeof(d));
	    if(d < maxImBVoltRate)
	    {
	       FS_WriteFile(E_FILE_MNT, MAXIMBVOLTRATE_SHIFT, (INT8U *)&maxImBVoltRate, sizeof(maxImBVoltRate));
	    }
	 }
	 return  FALSE;
    }
    else
    {
        return TRUE;
    }
   
}
static INT8U CurrImBalanceEvtS(INT8U evtId)
{
    if((0 != MntPara.CurrImbalLimitRatio) && (mCurrMax*((INT32U)100) > mCurrBase*((INT32U)5))
      && (imBCurrRate> MntPara.CurrImbalLimitRatio))
    {
        maxImBCurrRate=imBCurrRate;
	 FS_WriteFile(E_FILE_MNT, MAXIMBCURRRATE_SHIFT, (INT8U *)&maxImBCurrRate, sizeof(maxImBCurrRate));
	 return TRUE;
    }
    return  FALSE;
}
static INT8U CurrImBalanceEvtE(INT8U evtId)
{
   ST_7TIME_FORM tempTime;
   INT16U d;
   
   if(MntPara.CurrImbalLimitRatio == 0)
   {
        return EVTENDNOW;
   }
   if((mCurrMax*((INT32U)100) > mCurrBase*((INT32U)5)) && (imBCurrRate> MntPara.CurrImbalLimitRatio))
   {
       if(imBCurrRate > maxImBCurrRate)
        {
           maxImBCurrRate=imBCurrRate;
        }
	 GetSingle(E_SYS_TIME, (INT8U *)&tempTime);
	 if(tempTime.sec == 0)
	 {
	    FS_ReadFile(E_FILE_MNT, MAXIMBCURRRATE_SHIFT, (INT8U *)&d, sizeof(d));
	    if(d < maxImBCurrRate)
	    {
	       FS_WriteFile(E_FILE_MNT, MAXIMBCURRRATE_SHIFT, (INT8U *)&maxImBCurrRate, sizeof(maxImBCurrRate));
	    }
	 }
       return  FALSE;
   }
    return  TRUE;
}
static INT8U CurrImBadlyBalanceEvtS(INT8U evtId)
{
    
    if((0 != MntPara.CurrBadlyImBalLimitRatio) && (mCurrMax*((INT32U)100) > mCurrBase*((INT32U)5))
	&& (imBCurrRate> MntPara.CurrBadlyImBalLimitRatio))
    {
        maxBadlyImBCurrRate=imBCurrRate;
	 FS_WriteFile(E_FILE_MNT, MAXBADLYIMBCURRRATE_SHIFT, (INT8U *)&maxBadlyImBCurrRate, sizeof(maxBadlyImBCurrRate));
	 return TRUE;
    }
    return  FALSE;
}
static INT8U CurrImBadlyBalanceEvtE(INT8U evtId)
{
   ST_7TIME_FORM tempTime;
   INT16U d;
   
   if(MntPara.CurrImbalLimitRatio == 0)
   {
        return EVTENDNOW;
   }
   if((mCurrMax*((INT32U)100) > mCurrBase*((INT32U)5)) && (imBCurrRate> MntPara.CurrBadlyImBalLimitRatio))
   {
        if(imBCurrRate > maxBadlyImBCurrRate)
        {
           maxBadlyImBCurrRate=imBCurrRate;
        }
	 GetSingle(E_SYS_TIME, (INT8U *)&tempTime);
	 if(tempTime.sec == 0)
	 {
	    FS_ReadFile(E_FILE_MNT, MAXBADLYIMBCURRRATE_SHIFT, (INT8U *)&d, sizeof(d));
	    if(d < maxBadlyImBCurrRate)
	    {
	       FS_WriteFile(E_FILE_MNT, MAXBADLYIMBCURRRATE_SHIFT, (INT8U *)&maxBadlyImBCurrRate, sizeof(maxBadlyImBCurrRate));
	    }
	 }
	 return  FALSE;
   }
   else
   {
       return  TRUE;
   }
}

static INT8U OverFactorEvtS(INT8U evtId)
{
    INT32S mFactor;

    GetSingle(E_FACTOR, (INT8U *)&mFactor);

    mFactor=mFactor/1000;   /*"10-3"*/
    
     if((0 != MntPara.OverFactorThr) && (LIB_ABS(mFactor)< MntPara.OverFactorThr)
       && (mCurrMax*((INT32U)100) > mCurrBase*((INT32U)5)))
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U OverFactorEvtE(INT8U evtId)
{
      if(MntPara.OverFactorThr == 0)
     {
          return EVTENDNOW;
     }
     if(OverFactorEvtS(evtId) == FALSE)
     {
         return  TRUE;
     }
     return  FALSE;
}
static INT8U VoltReverseEvtS(INT8U evtId)
{
    if((MODE3P3W)&&((BBREAK)||(BLOSTVOLT)))  /*"��������B����ࡢB��ʧѹʱ���ж�������"*/
    {
        return FALSE;
    }
    if((VOLTSEQU)&&(mVoltMin*((INT32U)100)>mVoltBase*((INT32U)60)))
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U VoltReverseEvtE(INT8U evtId)
{
    if(VoltReverseEvtS(evtId) == FALSE)
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U CurrReverseEvtS(INT8U evtId)
{
    if(MODE3P3W)  /*"�������߲��ж�������"*/
    {
        return FALSE;
    }
    if((CURRSEQU)&&(mVoltMin*((INT32U)100)>mVoltBase*((INT32U)60))&&(mCurrMin*((INT32U)100)>mCurrBase*((INT32U)5)))
    {
        return TRUE;
    }
    return  FALSE;
}
static INT8U CurrReverseEvtE(INT8U evtId)
{
    if(CurrReverseEvtS(evtId) == FALSE)
    {
        return TRUE;
    }
    return  FALSE;
}
#endif
/*"************************************************"*/
/*"Function:    PDEvent"*/
/*"Description:��������ʱ�����¼��ж�������"*/
/*"Input:��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
static void GE_NetEvtJudge(void)
{
    INT8U evtEnumId;
    const ST_EVT_CHK_FUN FAR_POINTER *PwrNetEvtChkFun;
    INT8U res;

    for(evtEnumId = 0; evtEnumId < E_MAX_GRID_EVNT ; evtEnumId ++)
    {
        PwrNetEvtChkFun = (const ST_EVT_CHK_FUN FAR_POINTER *)&stEvtChkFun[evtEnumId];

        if((mStNetEvtFlg[evtEnumId >> 3] & (0x01 << (evtEnumId & 0x07))) == 0)  /*"�¼�δ����"*/
        {
            if(TRUE == PwrNetEvtChkFun->EvtStartChk(evtEnumId))  /*"�����¼���������"*/
            {
                mNetEvtSecAdd[evtEnumId]++;
                if(TRUE == GE_JudgeEvtTime(evtEnumId, EVT_START))  /*"�����¼�������ʱ"*/
                {
                     GE_SaveEvtSOrE(evtEnumId, E_SYS_TIME, EVT_START);
                }
            }
            else /*"�������¼���������"*/
            {
                 mNetEvtSecAdd[evtEnumId] = 0;
            }
        }
        else /*"�¼��Ѿ�����"*/
        {
            res=PwrNetEvtChkFun->EvtEndChk(evtEnumId);
	     if((TRUE == res)||(EVTENDNOW == res))  /*"�����¼���������"*/
            {
                mNetEvtSecAdd[evtEnumId]++;
                if((TRUE == GE_JudgeEvtTime(evtEnumId, EVT_END))||(EVTENDNOW == res))  /*"�����¼�������ʱ"*/
                {
                     GE_SaveEvtSOrE(evtEnumId, E_SYS_TIME, EVT_END);
                }
            }
            else /*"�������¼���������"*/
            {
                 mNetEvtSecAdd[evtEnumId] = 0;
            }
        }
    }
}

/*"�����¼����ж�ʱ�ӣ���ָ��"*/
const INT32U DelayTimeList[E_MAX_GRID_EVNT] =
    {
        (INT32U)&MntPara.PwrDownDelayT,  /*"����"*/
	 #ifdef THREE_PHASE
	 (INT32U)&MntPara.LossVoltDelayT, /*"A��ʧѹ"*/
        (INT32U)&MntPara.LossVoltDelayT, /*"B��ʧѹ"*/
        (INT32U)&MntPara.LossVoltDelayT, /*"C��ʧѹ"*/
        60,                                                       /*"ȫʧѹ"*/
        (INT32U)&MntPara.LossCurrDelayT, /*"A��ʧ��"*/
        (INT32U)&MntPara.LossCurrDelayT, /*"B��ʧ��"*/
        (INT32U)&MntPara.LossCurrDelayT, /*"C��ʧ��"*/
        (INT32U)&MntPara.BrkPhaseDelayT,/*"A�����"*/
        (INT32U)&MntPara.BrkPhaseDelayT,/*"B�����"*/
        (INT32U)&MntPara.BrkPhaseDelayT,/*"C�����"*/
	 #endif
        #ifdef OVER_CURR_CHK
        (INT32U)&MntPara.OverCurrDelayT,  /*"A�����"*/
        #endif
        #ifdef THREE_PHASE
        #ifdef OVER_CURR_CHK
	 (INT32U)&MntPara.OverCurrDelayT,  /*"B�����"*/
        (INT32U)&MntPara.OverCurrDelayT,  /*"C�����"*/
        #endif
        (INT32U)&MntPara.CutCurrDelayT,  /*"A�����"*/
        (INT32U)&MntPara.CutCurrDelayT,  /*"B�����"*/
        (INT32U)&MntPara.CutCurrDelayT,  /*"C�����"*/
        (INT32U)&MntPara.OverVoltDelayT, /*"A���ѹ"*/
        (INT32U)&MntPara.OverVoltDelayT, /*"B���ѹ"*/
        (INT32U)&MntPara.OverVoltDelayT, /*"C���ѹ"*/
        (INT32U)&MntPara.UnderVoltDelayT,   /*"A��Ƿѹ"*/
        (INT32U)&MntPara.UnderVoltDelayT,   /*"B��Ƿѹ"*/
        (INT32U)&MntPara.UnderVoltDelayT,   /*"C��Ƿѹ"*/
        1,                                                             /*"��������"*/
        (INT32U)&MntPara.PwrReverseDelayT,/*"A�๦�ʷ���"*/
        (INT32U)&MntPara.PwrReverseDelayT,/*"B�๦�ʷ���"*/
        (INT32U)&MntPara.PwrReverseDelayT,/*"C�๦�ʷ���"*/
        (INT32U)&MntPara.OverLoadDelayT,    /*"A�����"*/
        (INT32U)&MntPara.OverLoadDelayT,    /*"B�����"*/
        (INT32U)&MntPara.OverLoadDelayT,    /*"C�����"*/
        (INT32U)&MntPara.AAOverDemandDelayT,/*"�����й���������"*/
        (INT32U)&MntPara.NAOverDemandDelayT,/*"�����й���������"*/
        (INT32U)&MntPara.ROverDemandDelayT, /*"I�����޹���������"*/
        (INT32U)&MntPara.ROverDemandDelayT,/*"II�����޹���������"*/
        (INT32U)&MntPara.ROverDemandDelayT,/*"III�����޹���������"*/
        (INT32U)&MntPara.ROverDemandDelayT, /*"IV�����޹���������"*/
        (INT32U)&MntPara.VoltImbalDelayT,       /*"��ѹ��ƽ��"*/
        (INT32U)&MntPara.CurrImbalDelayT,       /*"������ƽ��"*/
        (INT32U)&MntPara.CurrBadlyImbalDelayT, /*"�������ز�ƽ��"*/
        (INT32U)&MntPara.OverFactorDelayT,    /*"�ܹ�������������"*/
        (INT32U)&MntPara.VoltReverseDelayT,   /*"��ѹ������"*/
        (INT32U)&MntPara.CurrReverseDelayT,   /*"����������"*/
	 #endif
    };
/*"************************************************"*/
/*"Function:    GE_JudgeEvtTime"*/
/*"Description:�ж��Ƿ�ﵽ�¼���ʱҪ��"*/
/*"Input: enumId:�����¼�ö����ţ�startOrEnd:�¼���ʼ�����"*/
/*"Output: ��"*/
/*"Return: �Ƿ��������� _TRUE:���㣬_FALSE:������"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U GE_JudgeEvtTime(INT8U enumId , INT8U startOrEnd)
{
    INT8U result;
    INT8U DelayTimePara;

    if(DelayTimeList[enumId] < 0x1000)/*"��ʱ����Ϊ��������"*/
    {
        DelayTimePara = (INT8U) DelayTimeList[enumId];
    }
    else /*"��ʱ����Ϊ����ָ��"*/
    {
        DelayTimePara = *((INT8U *)(DelayTimeList[enumId]));
    }

    if(startOrEnd == EVT_END)
    {
       #ifdef THREE_PHASE
	if(enumId == E_EVT_PWR_DOWN)
       {
          DelayTimePara=MntPara.BrkPhaseDelayT;
       }
	else if(enumId == E_EVT_LOST_VOLT_ALL)
	{
	   DelayTimePara=MntPara.LossVoltDelayT;
	}
	#else
	 if(enumId == E_EVT_PWR_DOWN)
        {
           DelayTimePara=PWRUP_PD_E_SEC;
        }
	#endif
    }

    result = FALSE;

    if(mNetEvtSecAdd[enumId] >= DelayTimePara)
    {
        result = TRUE;
    }

    return result;
}


/*"************************************************"*/
/*"Function:   GE_NetEvtTimeAdd "*/
/*"Description: �����¼�ʱ���ۼ�,ÿ�����"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
static void GE_NetEvtTimeAdd(void)
{
    INT8U evtId;
    
    MCU_PwrChkCloseINT();
    
    for(evtId = 0; evtId < E_MAX_GRID_EVNT; evtId++)
    {
        if((mStNetEvtFlg[evtId >> 3] & (0x01 << (evtId & 0x07))) != 0)
        {
            EvntTimeAdd[evtId]++;
        }
    }

    EvntTimeAddCrc  = CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd));

    MCU_PwrChkOpenINT();
}

/*"************************************************"*/
/*"Function:   GE_NetEvtClr "*/
/*"Description: ������¼�ʱ����"*/
/*"Input: enumId �����¼���ö�ٺ�(��EN_NET_EVT)��0xffΪȫ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
void GE_NetEvtClr(INT8U enumId)
{
    INT8U num;

    for(num = 0; num < E_MAX_GRID_EVNT; num++)
    {
        if((enumId == 0xff) || (num == enumId))
        {
            mStNetEvtFlg[num >> 3] &= ~(INT8U)(0x01 << (num & 0x07));
            mNetEvtSecAdd[num] = 0;
            EvntTimeAdd[num] = 0;
            Push((EN_MTR_STATUS)GridEvntStatusList[num], EVT_END);
        }
    }
    FS_WriteFile(E_FILE_MNT, EVNT_TIME_ADD_SHIFT,(INT8U *)&EvntTimeAdd[0], sizeof(EvntTimeAdd));
    FS_WriteFile(E_FILE_MNT, GRID_EVNT_FLG_SHIFT,(INT8U *)&mStNetEvtFlg[0], sizeof(mStNetEvtFlg));
}

#ifdef THREE_PHASE
void GE_RefreshLcd(void)
{

   DIS_SetStauSeg(STA_UA);
   
   if((ChkStatus(E_LOSS_VOLT_LA) == TRUE)||(ChkStatus(E_LOSS_VOLT_ALL) == TRUE))
   {
      DIS_FlashStauSeg(STA_UA);
   }
   else
   {
      DIS_ClrFlashStauSeg(STA_UA);
   }

   if((ChkStatus(E_BRK_PHASE_LA) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
   {
      DIS_ClrStauSeg(STA_UA);
   }

   if(MODE3P3W == 0)
   {
       DIS_SetStauSeg(STA_UB);
	  
	if((ChkStatus(E_LOSS_VOLT_LB) == TRUE)||(ChkStatus(E_LOSS_VOLT_ALL) == TRUE))
       {
          DIS_FlashStauSeg(STA_UB);
       }
       else
       {
          DIS_ClrFlashStauSeg(STA_UB);
       }

	if((ChkStatus(E_BRK_PHASE_LB) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
       {
          DIS_ClrStauSeg(STA_UB);
       }
   }

   DIS_SetStauSeg(STA_UC);
   
   if((ChkStatus(E_LOSS_VOLT_LC) == TRUE)||(ChkStatus(E_LOSS_VOLT_ALL) == TRUE))
   {
      DIS_FlashStauSeg(STA_UC);
   }
   else
   {
      DIS_ClrFlashStauSeg(STA_UC);
   }

   if((ChkStatus(E_BRK_PHASE_LC) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
   {
      DIS_ClrStauSeg(STA_UC);
   }

   if((ChkStatus(E_VOLT_REVERSE) == TRUE)||(ChkStatus(E_CURR_REVERSE) == TRUE))
   {
      DIS_FlashStauSeg(STA_ATHWART);
   }
   else
   {
      DIS_ClrFlashStauSeg(STA_ATHWART);
   }

   DIS_SetStauSeg(STA_IA);
   
   if(ChkStatus(E_LOSS_CURR_LA) == TRUE)
   {
      DIS_FlashStauSeg(STA_IA);
   }
   else
   {
      DIS_ClrFlashStauSeg(STA_IA);
   }

   if((ChkStatus(E_BRK_PHASE_LA) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE)||(ChkStatus(E_CUT_CURR_LA) == TRUE))
   {
      DIS_ClrStauSeg(STA_IA);
   }

   if(MODE3P3W == 0)
   {
      DIS_SetStauSeg(STA_IB);
   
      if(ChkStatus(E_LOSS_CURR_LB) == TRUE)
      {
         DIS_FlashStauSeg(STA_IB);
      }
      else
      {
         DIS_ClrFlashStauSeg(STA_IB);
      }
   
      if((ChkStatus(E_BRK_PHASE_LB) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE)||(ChkStatus(E_CUT_CURR_LB) == TRUE))
      {
         DIS_ClrStauSeg(STA_IB);
      }
   }

   DIS_SetStauSeg(STA_IC);
   
   if(ChkStatus(E_LOSS_CURR_LC) == TRUE)
   {
      DIS_FlashStauSeg(STA_IC);
   }
   else
   {
      DIS_ClrFlashStauSeg(STA_IC);
   }

   if((ChkStatus(E_BRK_PHASE_LC) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE)||(ChkStatus(E_CUT_CURR_LC) == TRUE))
   {
      DIS_ClrStauSeg(STA_IC);
   }

   if(ChkStatus(E_PWR_REVERSE_LA) == TRUE)
   {
      DIS_SetStauSeg(STA_MINUS_IA);
   }
   else
   {
      DIS_ClrStauSeg(STA_MINUS_IA);
   }

   if(MODE3P3W == 0)
   {
       if(ChkStatus(E_PWR_REVERSE_LB) == TRUE)
       {
          DIS_SetStauSeg(STA_MINUS_IB);
       }
       else
       {
          DIS_ClrStauSeg(STA_MINUS_IB);
       }
   }

   if(ChkStatus(E_PWR_REVERSE_LC) == TRUE)
   {
      DIS_SetStauSeg(STA_MINUS_IC);
   }
   else
   {
      DIS_ClrStauSeg(STA_MINUS_IC);
   }

   if((ChkStatus(E_OVER_DEMAND_PA) == TRUE)||(ChkStatus(E_OVER_DEMAND_NA) == TRUE)
   ||(ChkStatus(E_OVER_DEMAND_I) == TRUE)||(ChkStatus(E_OVER_DEMAND_II) == TRUE)
   ||(ChkStatus(E_OVER_DEMAND_III) == TRUE)||(ChkStatus(E_OVER_DEMAND_IV) == TRUE))
   {
      Push(E_OVER_DEMAND,TRUE);
   }
   else
   {
      Push(E_OVER_DEMAND,FALSE);
   }
}

void  VoltHealthInit(INT8U mode)
{
    ST_7TIME_FORM pdTime,nowTime;
    
    if(FIRST_PWR_ON == mode)
    {
        VoltHealthClear();
    }
    else
    {
       LIB_MemSet(0, (INT8U *)&avgVoltMin[0], sizeof(avgVoltMin));
       LIB_MemSet(0, (INT8U *)&avgVoltMinAcc[0], sizeof(avgVoltMinAcc));
       avgVoltMinAccTimes=0;

	if(voltHealthData.Crc != CRC16((INT8U *)&voltHealthData,FIND(VOLTHEALTH_DATA,Crc)))
	{
          if(voltHealthDataBak.Crc != CRC16((INT8U *)&voltHealthDataBak,FIND(VOLTHEALTH_DATA,Crc)))
          {
	      FS_ReadFile(E_FILE_MNT, VOLTHEALTHDATA_SHIFT, (INT8U *) &voltHealthData, sizeof(voltHealthData));
          }
	   else
	   {
	      voltHealthData=voltHealthDataBak;
	   }
	}
    }
    voltHealthDataBak=voltHealthData;
    if(MCU_RUNMODE == mode) /*"�ϵ�ʱ�ж��Ƿ���»���"*/
    {
        GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
        GetSingle(E_PD_TIME, (INT8U *)&pdTime);
        if((nowTime.year != pdTime.year)||(nowTime.month != pdTime.month)) /*"����ʱ���µ�ѹ�ϸ���"*/
        {
            MonVoltHealthClear(NULL,NULL);
        }
    }
}

void  VoltHealthSecTask(void)
{
   INT8U i;
   INT16U rVolt[3];

   if(avgVoltFlag == 0)
   {
      avgVoltFlag=1;
      return;
   }

   GetSingle(E_VOLT_BLK, (INT8U *)&rVolt[0]);

   for(i=0;i<3;i++)
   {
      avgVoltMinAcc[i]=avgVoltMinAcc[i]+rVolt[i]/10;
   }
   avgVoltMinAccTimes++;
}

void  VoltHealthMinTask(void)
{
    INT8U i;
    ST_7TIME_FORM nowTime;
    INT16U voltMode;

   if(avgVoltFlag == 0)
   {
      return;
   }

   GetSingle(E_MU_STATUS, (INT8U *)&voltMode);

   voltHealthDataBak=voltHealthData;
   
   for(i=0;i<3;i++)
   {
      if(i==1)
      {
         if((voltMode&0x01) != 0)  /*"��������"*/
         {
            continue;
         }
      }
	 
      if(avgVoltMinAccTimes>0)
      {
         avgVoltMin[i]=avgVoltMinAcc[i]/avgVoltMinAccTimes;
      }
      else
      {
         avgVoltMin[i]=0;
      }

       if((avgVoltMin[i] >= MntPara.VoltChkLimitL) && (avgVoltMin[i] <= MntPara.VoltChkLimitU))
       {
          voltHealthData.Day[i].ckTime++;
	   voltHealthData.Mon[i].ckTime++;
	   if((avgVoltMin[i] >= MntPara.VoltHealthLimitU) && (avgVoltMin[i] <= MntPara.VoltChkLimitU))
	   {
	      voltHealthData.Day[i].upTime++;
	      voltHealthData.Mon[i].upTime++;
	   }
	   else if((avgVoltMin[i] >= MntPara.VoltChkLimitL) && (avgVoltMin[i] <= MntPara.VoltHealthLimitL))
	   {
	      voltHealthData.Day[i].dnTime++;
	      voltHealthData.Mon[i].dnTime++;
	   }

	   voltHealthData.Day[i].bdRate=(voltHealthData.Day[i].dnTime+voltHealthData.Day[i].upTime)*10000/voltHealthData.Day[i].ckTime;
	   voltHealthData.Mon[i].bdRate=(voltHealthData.Mon[i].dnTime+voltHealthData.Mon[i].upTime)*10000/voltHealthData.Mon[i].ckTime;
	   voltHealthData.Day[i].okRate=10000-voltHealthData.Day[i].bdRate;
	   voltHealthData.Mon[i].okRate=10000-voltHealthData.Mon[i].bdRate;
       }
   }
   voltHealthData.Crc=CRC16((INT8U *)&voltHealthData,FIND(VOLTHEALTH_DATA,Crc));
   
   LIB_MemSet(0, (INT8U *)&avgVoltMinAcc[0], sizeof(avgVoltMinAcc));
   avgVoltMinAccTimes=0;

   GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
   if(nowTime.min%15 == 0)
   {
      FS_WriteFile(E_FILE_MNT, VOLTHEALTHDATA_SHIFT, (INT8U *) &voltHealthData, sizeof(voltHealthData));
   }
}

void  VoltHealthClear(void)
{
   LIB_MemSet(0, (INT8U *)&avgVoltMin[0], sizeof(avgVoltMin));
   LIB_MemSet(0, (INT8U *)&avgVoltMinAcc[0], sizeof(avgVoltMinAcc));
   avgVoltMinAccTimes=0;
   
   LIB_MemSet(0, (INT8U *)&voltHealthData, sizeof(voltHealthData));
   voltHealthData.Crc=CRC16((INT8U *)&voltHealthData,FIND(VOLTHEALTH_DATA,Crc));
   FS_WriteFile(E_FILE_MNT, VOLTHEALTHDATA_SHIFT, (INT8U *) &voltHealthData, sizeof(voltHealthData));

   voltHealthDataBak=voltHealthData;
}

INT8S DayVoltHealthClear(void *pIn, void *pOut)
{
   LIB_MemSet(0, (INT8U *)&voltHealthData.Day[0], 3*sizeof(voltHealthData.Day[0]));
   voltHealthData.Crc=CRC16((INT8U *)&voltHealthData,FIND(VOLTHEALTH_DATA,Crc));
   FS_WriteFile(E_FILE_MNT, VOLTHEALTHDATA_SHIFT, (INT8U *) &voltHealthData, sizeof(voltHealthData));

   voltHealthDataBak=voltHealthData;

   return MS_SUCCESS;
}

INT8S MonVoltHealthClear(void *pIn, void *pOut)
{
   LIB_MemSet(0, (INT8U *)&voltHealthData.Mon[0], 3*sizeof(voltHealthData.Mon[0]));
   voltHealthData.Crc=CRC16((INT8U *)&voltHealthData,FIND(VOLTHEALTH_DATA,Crc));
   FS_WriteFile(E_FILE_MNT, VOLTHEALTHDATA_SHIFT, (INT8U *) &voltHealthData, sizeof(voltHealthData));

   voltHealthDataBak=voltHealthData;

   return MS_SUCCESS;
}


NO_INIT ST_PDCURR stPdCurr; /*"�͹��ĵ���"*/

static void StPdCurrClear(void)
{
    LIB_MemSet(0 , (INT8U *)&stPdCurr, sizeof(stPdCurr));
}

static INT8U StPdCurrChk(void)
{
    CRC_DATA_TP crcTmp;
    
    crcTmp = CRC16((INT8U *)&stPdCurr, FIND(ST_PDCURR, crc));
     
    if((stPdCurr.crc != crcTmp) || (stPdCurr.keyWord != 0xa5a5))
    {
       return FALSE;
    }

    return TRUE;
}

void StPdCurrPdInit(void)
{
    if(StPdCurrChk()==FALSE)
    {
       StPdCurrClear();
    }
}

void StPdCurrSecTask(void)
{
   INT64U tMode=0;
   
   GetSingle(E_PWR_RST_MODE, (INT8U *)&tMode); 
   if(tMode == PWR_RST_BATT)
   {
      return;
   }

   if(mntPdLastTime == 60)
   {
       stPdCurr.keyWord=0xa5a5;	
	GetSingle(E_CURR_BLK, (INT8U *)&stPdCurr.pdCurr[0]);    	
	GetSingle(E_SYS_TIME, (INT8U *)&stPdCurr.pdTime);
	stPdCurr.crc = CRC16((INT8U *)&stPdCurr, FIND(ST_PDCURR, crc));     
   }
}

#endif

#ifdef THREE_PHASE
INT32S minActPower[4];        /*"ÿ����ƽ���й����� ��ABC"*/
INT32S minReActPower[4];    /*"ÿ����ƽ���޹����� ��ABC"*/
static INT64S minActPowerAcc[4];  
static INT64S minReActPowerAcc[4];
static INT16U minPowerTimes;
#else 
INT32S minActPower[2];        /*"ÿ����ƽ���й����� ��"*/
static INT64S minActPowerAcc[2];  
static INT16U minPowerTimes;
#endif

void minPowerInit(void)
{
#ifdef THREE_PHASE
   INT8U i;

   for(i=0;i<4;i++)
   {
      minActPower[i]=0;
      minReActPower[i]=0;
      minActPowerAcc[i]=0;
      minReActPowerAcc[i]=0;
   }
   minPowerTimes=0;
#else
    INT8U i;

   for(i=0;i<2;i++)
   {
      minActPower[i]=0;
      minActPowerAcc[i]=0;
   }
   minPowerTimes=0;
#endif
}

void minPowerSecTask(void)
{
#ifdef THREE_PHASE
   INT8U i;
   INT32S d[4];

   GetSingle(E_ACTIVE_PWR_BLK, (INT8U *)&d[0]);

   for(i=0;i<4;i++)
   {
      minActPowerAcc[i] += d[i];
   }

   GetSingle(E_REACTIVE_PWR_BLK, (INT8U *)&d[0]);

   for(i=0;i<4;i++)
   {
      minReActPowerAcc[i] += d[i];
   }
   
   minPowerTimes++;
#else
    INT8U i;
    INT32S d[2];

   GetSingle(E_ACTIVE_PWR_BLK, (INT8U *)&d[0]);

   for(i=0;i<2;i++)
   {
      minActPowerAcc[i] += d[i];
   }
   
   minPowerTimes++;
#endif
}

void minPowerMinTask(void)
{
#ifdef THREE_PHASE
   INT8U i;

   if(minPowerTimes > 0)
   {
      for(i=0;i<4;i++)
      {
          minActPower[i]=(minActPowerAcc[i]/minPowerTimes)/100;
          minReActPower[i]=(minReActPowerAcc[i]/minPowerTimes)/100;
      }
   }
   for(i=0;i<4;i++)
   {
       minActPowerAcc[i]=0;
       minReActPowerAcc[i]=0;
   }
   minPowerTimes=0;
#else
  INT8U i;

   if(minPowerTimes > 0)
   {
      for(i=0;i<2;i++)
      {
          minActPower[i]=(minActPowerAcc[i]/minPowerTimes);
      }
   }
   for(i=0;i<2;i++)
   {
       minActPowerAcc[i]=0;
   }
   minPowerTimes=0;
#endif
}

#ifdef THREE_PHASE
static INT8U pdEventTimeAddFlag=0;
INT8S MNT_PDEvtTimeAdd(void *pIn, void *pOut)
{
    INT8U evtId;
    INT32S pdSec;
    ST_7TIME_FORM nowTime;
    ST_7TIME_FORM evtStartTime[2];

    if( pdEventTimeAddFlag != 0)
    {
       return TRUE;
    }
    pdEventTimeAddFlag=0x01;
    
    if(EvntTimeAddCrc  == CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd)))
    {
      return TRUE;
    }
    
    for(evtId = 0; evtId < E_MAX_GRID_EVNT; evtId++)
    {
        if(0 != (mStNetEvtFlg[evtId >> 3] & (0x01 << (evtId & 0x07))))
        {
	     if((E_EVT_LOST_VOLT_LA == evtId)||(E_EVT_LOST_VOLT_LB == evtId)||(E_EVT_LOST_VOLT_LC == evtId)
             ||(E_EVT_LOST_CURR_LA == evtId)||(E_EVT_LOST_CURR_LB == evtId)||(E_EVT_LOST_CURR_LC == evtId))
            {
                 GetSingle((mstNetEvtInfo[evtId].evtObis- E_PWR_DOWN_EVNT_RCRD + E_RCRD_TIME_STATUS + 1), (INT8U *)&evtStartTime[0]);
                 GetSingle(E_PD_TIME, (INT8U *)&nowTime);
                 pdSec = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, (ST_6TIME_FORM *)&evtStartTime[0]);
   	          if(pdSec > 0)
   	          {
                     EvntTimeAdd[evtId] += pdSec;
   	          }
            }
        }
    }

    EvntTimeAddCrc  = CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd))+1;

    return TRUE;
}
#endif

void pdSaveRecord(void)
{
    //if((mStNetEvtFlg[E_EVT_PWR_DOWN >> 3] & (0x01 << (E_EVT_PWR_DOWN & 0x07))) == 0)  /*"�¼�δ����"*/
    //    {           
    //         GE_SaveEvtSOrE(E_EVT_PWR_DOWN, E_PD_TIME, EVT_START);
   //     }
   INT32U d[2];

    d[0]=stMmtData.volt[0];
    d[1]=stMmtData.curr[0];
    FS_WriteFile(E_FILE_MNT, PDCURRVOLT_SHIFT,  (INT8U *)&d[0], 8);
}


