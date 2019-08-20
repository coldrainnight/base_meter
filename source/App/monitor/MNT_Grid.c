/*"************************************************************"*/
/*"FileName: MNT_data.c"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: 电能表状态监测模块对象列表定义"*/
/*" 总字节数:          "*/
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

#define EVT_START        (1)  /*"事件起始"*/
#define EVT_END          (0)  /*"事件结束"*/
#define PWRUP_PD_E_SEC   (1)  /*"低功耗上电后判断掉电结束延时 ，单位秒"*/
#define EVTENDNOW  (0x02)


typedef INT8U  (* PwrNetEvtChkFun)(INT8U evtId);

/*"电网事件起始结束判断函数"*/
typedef struct
{
    PwrNetEvtChkFun  EvtStartChk;
    PwrNetEvtChkFun  EvtEndChk;
}ST_EVT_CHK_FUN;

static INT8U mNetEvtSecAdd[E_MAX_GRID_EVNT];/*"电网事件发生或结束持续时间累计"*/
static INT8U mStNetEvtFlg[GRID_EVNT_NUM];/*"电网事件发生标志"*/
#ifdef THREE_PHASE
INT16U avgVoltMin[3];   /*"每分钟的平均电压"*/
INT32U avgVoltMinAcc[3];
INT32U avgVoltMinAccTimes;
INT8U   avgVoltFlag=0;
NO_INIT VOLTHEALTH_DATA voltHealthData; /*"电压合格率数据"*/
NO_INIT static VOLTHEALTH_DATA voltHealthDataBak; /*"电压合格率备份数据"*/
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
                {PwrDownEvtS, PwrDownEvtE},/*"掉电事件"*/
		   #ifdef THREE_PHASE
		  {LostVoltEvtS, LostVoltEvtE},    /*"A相失压"*/
		  {LostVoltEvtS, LostVoltEvtE},    /*"B相失压"*/
		  {LostVoltEvtS, LostVoltEvtE},    /*"C相失压"*/
		  {LostVoltAllEvtS, LostVoltAllEvtE},    /*"全失压"*/
		  {LostCurrEvtS, LostCurrEvtE},    /*"A相失流"*/
		  {LostCurrEvtS, LostCurrEvtE},    /*"B相失流"*/
		  {LostCurrEvtS, LostCurrEvtE},    /*"C相失流"*/
		  {BreakPhaseEvtS, BreakPhaseEvtE},    /*"A相断相"*/
		  {BreakPhaseEvtS, BreakPhaseEvtE},    /*"B相断相"*/
		  {BreakPhaseEvtS, BreakPhaseEvtE},    /*"C相断相"*/
		   #endif
                #ifdef OVER_CURR_CHK
                {OverCurrEvtS, OverCurrEvtE}, /*"过流事件"*/
                #endif
		   #ifdef THREE_PHASE
                #ifdef OVER_CURR_CHK
		  {OverCurrEvtS, OverCurrEvtE},             /*"B相过流"*/
                {OverCurrEvtS, OverCurrEvtE},             /*"C相过流"*/
                #endif
                {CutCurrEvtS, CutCurrEvtE},             /*"A相断流"*/
                {CutCurrEvtS, CutCurrEvtE},             /*"B相断流"*/
                {CutCurrEvtS, CutCurrEvtE},             /*"C相断流"*/
                {OverVoltEvtS, OverVoltEvtE},         /*"A相过压"*/
                {OverVoltEvtS, OverVoltEvtE},         /*"B相过压"*/
                {OverVoltEvtS, OverVoltEvtE},         /*"C相过压"*/
                {UnderVoltEvtS, UnderVoltEvtE},     /*"A相欠压"*/
                {UnderVoltEvtS, UnderVoltEvtE},     /*"B相欠压"*/
                {UnderVoltEvtS, UnderVoltEvtE},     /*"C相欠压"*/
                {PwrReverseAllEvtS, PwrReverseAllEvtE},     /*"潮流反向"*/
                {PwrReverseEvtS, PwrReverseEvtE},     /*"A相功率反向"*/
                {PwrReverseEvtS, PwrReverseEvtE},     /*"B相功率反向"*/
                {PwrReverseEvtS, PwrReverseEvtE},     /*"C相功率反向"*/
                {OverLoadEvtS, OverLoadEvtE},    /*"A相过载"*/
                {OverLoadEvtS, OverLoadEvtE},    /*"B相过载"*/
                {OverLoadEvtS, OverLoadEvtE},    /*"C相过载"*/
                {mstDemandEvtS, mstDemandEvtE},   /*"正向有功需量超限"*/
                {mstDemandEvtS, mstDemandEvtE},   /*"反向有功需量超限"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"I象限无功需量超限"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"II象限无功需量超限"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"III象限无功需量超限"*/
                {mstDemandEvtS, mstDemandEvtE},  /*"IV象限无功需量超限"*/
                {VoltImBalanceEvtS, VoltImBalanceEvtE},  /*"电压不平衡"*/
                {CurrImBalanceEvtS, CurrImBalanceEvtE},  /*"电流不平衡"*/
                {CurrImBadlyBalanceEvtS, CurrImBadlyBalanceEvtE},  /*"电流严重不平衡"*/
                {OverFactorEvtS, OverFactorEvtE},  /*"总功率因数超下限"*/
                {VoltReverseEvtS, VoltReverseEvtE},  /*"电压逆相序"*/
                {CurrReverseEvtS, CurrReverseEvtE},   /*"电流逆相序"*/
		   #endif
        };
const INT8U GridEvntStatusList[E_MAX_GRID_EVNT] = 
	{
	  E_PWR_DOWN, 
	  #ifdef THREE_PHASE
	  E_LOSS_VOLT_LA, /*"A相失压"*/
           E_LOSS_VOLT_LB, /*"B相失压"*/
           E_LOSS_VOLT_LC, /*"C相失压"*/
           E_LOSS_VOLT_ALL,/*"全失压"*/
           E_LOSS_CURR_LA, /*"A相失流"*/
           E_LOSS_CURR_LB, /*"B相失流"*/
           E_LOSS_CURR_LC, /*"C相失流"*/
           E_BRK_PHASE_LA,/*"A相断相"*/
           E_BRK_PHASE_LB,/*"B相断相"*/
           E_BRK_PHASE_LC,/*"C相断相"*/
	  #endif
         #ifdef OVER_CURR_CHK
	  E_OVER_CURR_LA, /*"A相过流"*/
	  #endif
	  #ifdef THREE_PHASE
           #ifdef OVER_CURR_CHK
	    E_OVER_CURR_LB, /*"B相过流"*/
           E_OVER_CURR_LC, /*"C相过流"*/
           #endif
           E_CUT_CURR_LA, /*"A相断流"*/
           E_CUT_CURR_LB, /*"B相断流"*/
           E_CUT_CURR_LC, /*"C相断流"*/
           E_OVER_VOLT_LA, /*"A相过压"*/
           E_OVER_VOLT_LB, /*"B相过压"*/
           E_OVER_VOLT_LC, /*"C相过压"*/
           E_UNDER_VOLT_LA, /*"A相欠压"*/
           E_UNDER_VOLT_LB, /*"B相欠压"*/
           E_UNDER_VOLT_LC, /*"C相欠压"*/
           E_PWR_REVERSE, /*"潮流反向"*/
           E_PWR_REVERSE_LA,/*"A相功率反向"*/
           E_PWR_REVERSE_LB,/*"B相功率反向"*/
           E_PWR_REVERSE_LC,/*"C相功率反向"*/
           E_OVER_LOAD_LA, /*"A相过载"*/
           E_OVER_LOAD_LB, /*"B相过载"*/
           E_OVER_LOAD_LC, /*"C相过载"*/
           E_OVER_DEMAND_PA,/*"正向有功需量超限"*/
           E_OVER_DEMAND_NA,/*"反向有功需量超限"*/
           E_OVER_DEMAND_I, /*"I象限无功需量超限"*/
           E_OVER_DEMAND_II,/*"II象限无功需量超限"*/
           E_OVER_DEMAND_III,/*"III象限无功需量超限"*/
           E_OVER_DEMAND_IV, /*"IV象限无功需量超限"*/
           E_VOLT_IMBAL, /*"电压不平衡"*/
           E_CURR_IMBAL, /*"电流不平衡"*/
           E_CURR_BADLY_IMBAL, /*"电流严重不平衡"*/
           E_OVER_FACTOR, /*"总功率因数超下限"*/
           E_VOLT_REVERSE, /*"电压逆相序"*/
           E_CURR_REVERSE, /*"电流逆相序"*/
	  #endif
	};
/*"************************************************"*/
/*"Function:    GE_Init"*/
/*"Description: 电网事件初始化"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
        /*"记录复位事件"*/
        SaveRcrd(E_SYS_RST_RCRD, E_EVTLST_SYS_RST_S);
        #if(MNT_VERSION == STATE_GRID)
        err_code_addr = 0;   /*清一下，以便下次用*/
        #endif       
        for(i = 0; i < E_MAX_GRID_EVNT; i++)
        {
            if((mStNetEvtFlg[i >> 3] & (0x01 << (i & 0x07))) != 0)/*"复位之前事件发生"*/
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
/*"Description: 电网事件主程序"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
/*"Description: 检测电网功率方向"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
    if(TRUE == ChkStatus(E_PWR_P_DIR))/*"总有功功率反向"*/
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
/*"Description: 上电时电网事件累计时间纠错处理"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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

    EvntTimeAddCrc  = CRC16((INT8U *)&EvntTimeAdd[0] , sizeof(EvntTimeAdd))+1; /*"RAM中数据丢失"*/
}

 typedef struct
{
    INT16U evtObis; /*"电网事件对象标识"*/
    INT16U evtStartLstObis; /*"事件发生派生对象标识"*/
    INT16U evtEndLstObis; /*"事件结束派生对象标识"*/
}ST_EVT_INFO_LST;

/*"事件信息列表，定义时须与EN_NET_EVT结构中定义的事件顺序保持一致"*/
const ST_EVT_INFO_LST mstNetEvtInfo[E_MAX_GRID_EVNT]=
        {
                {E_PWR_DOWN_EVNT_RCRD , E_EVTLST_PD_S , E_EVTLST_PD_E},     /*"掉电"*/
                #ifdef THREE_PHASE
		  {E_LOST_VOLT_LA_EVNT_RCRD , E_EVTLST_LOST_VOLT_S , E_EVTLST_LOST_VOLT_E},   /*"A相失压"*/
            {E_LOST_VOLT_LB_EVNT_RCRD , E_EVTLST_LOST_VOLT_S , E_EVTLST_LOST_VOLT_E},  /*"B相失压"*/
            {E_LOST_VOLT_LC_EVNT_RCRD , E_EVTLST_LOST_VOLT_S , E_EVTLST_LOST_VOLT_E},  /*"C相失压"*/
            {E_ALL_LOST_VOLT_EVNT_RCRD , E_EVTLST_ALL_LOST_VOLT_S , E_EVTLST_ALL_LOST_VOLT_E},  /*"全失压"*/
            {E_LOST_CURR_LA_EVNT_RCRD , E_EVTLST_LOST_CURR_S , E_EVTLST_LOST_CURR_E},   /*"A相失流"*/
            {E_LOST_CURR_LB_EVNT_RCRD , E_EVTLST_LOST_CURR_S , E_EVTLST_LOST_CURR_E},   /*"B相失流"*/
            {E_LOST_CURR_LC_EVNT_RCRD , E_EVTLST_LOST_CURR_S , E_EVTLST_LOST_CURR_E},   /*"C相失流"*/
            {E_BRK_PHASE_LA_EVNT_RCRD , E_EVTLST_BRK_PHASE_S , E_EVTLST_BRK_PHASE_E},  /*"A相断相"*/
            {E_BRK_PHASE_LB_EVNT_RCRD , E_EVTLST_BRK_PHASE_S , E_EVTLST_BRK_PHASE_E},  /*"B相断相"*/
            {E_BRK_PHASE_LC_EVNT_RCRD , E_EVTLST_BRK_PHASE_S , E_EVTLST_BRK_PHASE_E},  /*"C相断相"*/
		  #endif
                #ifdef OVER_CURR_CHK
                {E_OVER_CURR_LA_EVNT_RCRD , E_EVTLST_OVERCURR_S , E_EVTLST_OVERCURR_E},   /*"A相过流"*/
                #endif
                #ifdef THREE_PHASE
                #ifdef OVER_CURR_CHK
		  {E_OVER_CURR_LB_EVNT_RCRD , E_EVTLST_OVERCURR_S , E_EVTLST_OVERCURR_E},   /*"B相过流"*/
            {E_OVER_CURR_LC_EVNT_RCRD , E_EVTLST_OVERCURR_S , E_EVTLST_OVERCURR_E},   /*"C相过流"*/
            #endif
            {E_BRK_CURR_LA_EVNT_RCRD , E_EVTLST_BRK_CURR_S , E_EVTLST_BRK_CURR_E},  /*"A相断流"*/
            {E_BRK_CURR_LB_EVNT_RCRD , E_EVTLST_BRK_CURR_S , E_EVTLST_BRK_CURR_E},  /*"B相断流"*/
            {E_BRK_CURR_LC_EVNT_RCRD , E_EVTLST_BRK_CURR_S , E_EVTLST_BRK_CURR_E},  /*"C相断流"*/
            {E_OVER_VOLT_LA_EVNT_RCRD , E_EVTLST_OVER_VOLT_S , E_EVTLST_OVER_VOLT_E},  /*"A相过压"*/
            {E_OVER_VOLT_LB_EVNT_RCRD , E_EVTLST_OVER_VOLT_S , E_EVTLST_OVER_VOLT_E},  /*"B相过压"*/
            {E_OVER_VOLT_LC_EVNT_RCRD , E_EVTLST_OVER_VOLT_S , E_EVTLST_OVER_VOLT_E},   /*"C相过压"*/
            {E_LACK_VOLT_LA_EVNT_RCRD , E_EVTLST_LACK_VOLT_S , E_EVTLST_LACK_VOLT_E},  /*"A相欠压"*/
            {E_LACK_VOLT_LB_EVNT_RCRD , E_EVTLST_LACK_VOLT_S , E_EVTLST_LACK_VOLT_E},   /*"B相欠压"*/
            {E_LACK_VOLT_LC_EVNT_RCRD , E_EVTLST_LACK_VOLT_S , E_EVTLST_LACK_VOLT_E},  /*"C相欠压"*/
            {E_PWR_REVERSE_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"潮流反向"*/
            {E_PWR_REVERSE_LA_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"A相功率反向"*/
            {E_PWR_REVERSE_LB_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"B相功率反向"*/
            {E_PWR_REVERSE_LC_EVNT_RCRD , E_EVTLST_PWR_REVERSE_S , E_EVTLST_PWR_REVERSE_E},  /*"C相功率反向"*/
            {E_OVER_LOAD_LA_EVNT_RCRD , E_EVTLST_OVER_LOAD_S , E_EVTLST_OVER_LOAD_E},  /*"A相过载"*/
            {E_OVER_LOAD_LB_EVNT_RCRD , E_EVTLST_OVER_LOAD_S , E_EVTLST_OVER_LOAD_E},  /*"B相过载"*/
            {E_OVER_LOAD_LC_EVNT_RCRD , E_EVTLST_OVER_LOAD_S , E_EVTLST_OVER_LOAD_E},  /*"C相过载"*/
            {E_OVER_PA_DEMAND_EVNT_RCRD , E_EVTLST_OVER_PA_DEMAND_S , E_EVTLST_OVER_PA_DEMAND_E},  /*"正向有功需量超限"*/
            {E_OVER_NA_DEMAND_EVNT_RCRD , E_EVTLST_OVER_NA_DEMAND_S , E_EVTLST_OVER_NA_DEMAND_E},  /*"反向有功需量超限"*/
            {E_OVER_R_DEMAND_1_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_1_E},  /*"I象限无功需量超限"*/
            {E_OVER_R_DEMAND_2_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_2_E},  /*"II象限无功需量超限"*/
            {E_OVER_R_DEMAND_3_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_3_E},  /*"III象限无功需量超限"*/
            {E_OVER_R_DEMAND_4_EVNT_RCRD , E_EVTLST_OVER_R_DEMAND_S , E_EVTLST_OVER_R_DMD_4_E},  /*"IV象限无功需量超限"*/
            {E_VOLT_IMBALANCE_EVNT_RCRD , E_EVTLST_VOLT_IMBALANCE_S , E_EVTLST_VOLT_IMBALANCE_E},  /*"电压不平衡"*/
            {E_CURR_IMBALANCE_EVNT_RCRD , E_EVTLST_CURR_IMBALANCE_S , E_EVTLST_CURR_IMBALANCE_E},  /*"电流不平衡"*/
            {E_CURR_IMBALANCE_BADLY_EVNT_RCRD , E_EVTLST_CURR_IMBALANCE_BADLY_S , E_EVTLST_CURR_IMBALANCE_BADLY_E},  /*"电流严重不平衡"*/
            {E_FACTOR_LOW_EVNT_RCRD , E_EVTLST_FACTOR_LOW_S , E_EVTLST_FACTOR_LOW_E},  /*"总功率因数超下限"*/
            {E_VOLT_INVERSE_EVNT_RCRD , E_EVTLST_VOLT_INVERSE_S , E_EVTLST_VOLT_INVERSE_E},   /*"电压逆相序"*/
            {E_CURR_INVERSE_EVNT_RCRD , E_EVTLST_CURR_INVERSE_S , E_EVTLST_CURR_INVERSE_E},  /*"电流逆相序"*/
		  #endif
        };

/*"***********************************************************************"*/
/*"Function:    GE_SaveEvtSOrE"*/
/*"Description: 记录事件发生或结束，存储事件发生标志        "*/
/*"Input:  evtName:电网事件名称， 参见EN_NET_EVT定义                     "*/
/*"           timeObis:事件列表中时间从DC_DATA_BUF取值时使用，  "*/
/*"                         通常为PD_TIME 或SYS_TIME     "*/
/*"           stOrEnd: _EVT_START事件发生，_EVT_END 事件结束                 "*/
/*"Output: 无                                                                                                            "*/
/*"Return: 无                                                                                                            "*/
/*"**********************************************************************"*/
static void GE_SaveEvtSOrE( INT16U evtEnumId, INT16U timeObis, INT8U stOrEnd)
{
    INT16U evtLstObis;

    if(stOrEnd == EVT_START)/*"记录事件发生内容"*/
    {
        evtLstObis = mstNetEvtInfo[evtEnumId].evtStartLstObis;
    }
    else/*"记录事件结束内容"*/
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
           mStNetEvtFlg[evtEnumId >> 3] |= (INT8U)(0x01 << (evtEnumId & 0x07)); /*"事件标志置位"*/
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

    /*"有累计时间的事件在结束事件时存储累计时间到eeprom"*/
    if(stOrEnd == EVT_END)
    {
        FS_WriteFile(E_FILE_MNT, EVNT_TIME_ADD_SHIFT + evtEnumId * 4, (INT8U *)&EvntTimeAdd[evtEnumId], 4);
    }
}

/*"************************************************"*/
/*"Function:  NetEvtPwrUpInit"*/
/*"Description:电网事件上电处理函数"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
	     if((E_EVT_PWR_DOWN != evtId)&&(E_EVT_LOST_VOLT_ALL != evtId))/*"掉电、全失压事件已经发生，主循环中判断事件是否结束"*/
#else
	     if(E_EVT_PWR_DOWN != evtId)/*"掉电事件已经发生，主循环中判断事件是否结束"*/
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
                /*"记录掉电事件发生"*/
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
/*"Description: 掉电事件起始判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足起始条件     _FALSE 不满足起始条件"*/
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
/*"Description: 掉电事件结束判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足结束条件     _FALSE 不满足结束条件"*/
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
/*"Description: 过流事件起始判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足起始条件     _FALSE 不满足起始条件"*/
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
/*"Description: 过流事件结束判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足结束条件     _FALSE 不满足结束条件"*/
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
/*"Description: 掉电事件起始判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足起始条件     _FALSE 不满足起始条件"*/
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
/*"Description: 掉电事件结束判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足结束条件     _FALSE 不满足结束条件"*/
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
/*"Description: 过流事件起始判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足起始条件     _FALSE 不满足起始条件"*/
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
/*"Description: 过流事件结束判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足结束条件     _FALSE 不满足结束条件"*/
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
/*"Description: 失压事件起始判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足起始条件     _FALSE 不满足起始条件"*/
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
/*"Description: 失压事件结束判断条件"*/
/*"Input:evtId  电网事件编号"*/
/*"Output: 无"*/
/*"Return: _TRUE 满足结束条件     _FALSE 不满足结束条件"*/
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
#define DEMAND_CLR_DELAY (1)  /*抗干扰时间*/
#define MODE_CLR_CUR_DEMAND  0  /*默认延时清0模式，1或0*/

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
        GetSingle(E_EXT_MODE1, &DemandMode);  /*b4 潮流反向立即清0(0)/潮流反向延时清0(1)*/
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
    		         Action(E_CURRENT_DEMAND_CLR, NULL, NULL);  /*清当前需量*/
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
    		           Action(E_CURRENT_DEMAND_CLR, NULL, NULL);  /*清当前需量*/
                   }

    		       return TRUE;
    	       }
        	   else
        	   {
        		   pwrReverseTFlag=TPWRNEGTIVE;
        	   }
    	    }
    	}

        /*"为1时，方向改变时就清需量"*/
        if(MODE_CLR_CUR_DEMAND != DemandMode) 
        {
             if((DEMAND_CLR_DELAY==pwrReverseTACnt)||(DEMAND_CLR_DELAY==pwrReverseTNCnt))
             {
                 Action(E_CURRENT_DEMAND_CLR, NULL, NULL);  /*清当前需量*/ 
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
    if((MODE3P3W)&&((BBREAK)||(BLOSTVOLT)))  /*"三相三线B相断相、B相失压时不判断逆相序"*/
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
    if(MODE3P3W)  /*"三相三线不判断逆相序"*/
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
/*"Description:正常运行时电网事件判断主函数"*/
/*"Input:无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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

        if((mStNetEvtFlg[evtEnumId >> 3] & (0x01 << (evtEnumId & 0x07))) == 0)  /*"事件未发生"*/
        {
            if(TRUE == PwrNetEvtChkFun->EvtStartChk(evtEnumId))  /*"满足事件发生条件"*/
            {
                mNetEvtSecAdd[evtEnumId]++;
                if(TRUE == GE_JudgeEvtTime(evtEnumId, EVT_START))  /*"满足事件发生延时"*/
                {
                     GE_SaveEvtSOrE(evtEnumId, E_SYS_TIME, EVT_START);
                }
            }
            else /*"不满足事件发生条件"*/
            {
                 mNetEvtSecAdd[evtEnumId] = 0;
            }
        }
        else /*"事件已经发生"*/
        {
            res=PwrNetEvtChkFun->EvtEndChk(evtEnumId);
	     if((TRUE == res)||(EVTENDNOW == res))  /*"满足事件结束条件"*/
            {
                mNetEvtSecAdd[evtEnumId]++;
                if((TRUE == GE_JudgeEvtTime(evtEnumId, EVT_END))||(EVTENDNOW == res))  /*"满足事件结束延时"*/
                {
                     GE_SaveEvtSOrE(evtEnumId, E_SYS_TIME, EVT_END);
                }
            }
            else /*"不满足事件结束条件"*/
            {
                 mNetEvtSecAdd[evtEnumId] = 0;
            }
        }
    }
}

/*"各类事件的判断时延，可指定"*/
const INT32U DelayTimeList[E_MAX_GRID_EVNT] =
    {
        (INT32U)&MntPara.PwrDownDelayT,  /*"掉电"*/
	 #ifdef THREE_PHASE
	 (INT32U)&MntPara.LossVoltDelayT, /*"A相失压"*/
        (INT32U)&MntPara.LossVoltDelayT, /*"B相失压"*/
        (INT32U)&MntPara.LossVoltDelayT, /*"C相失压"*/
        60,                                                       /*"全失压"*/
        (INT32U)&MntPara.LossCurrDelayT, /*"A相失流"*/
        (INT32U)&MntPara.LossCurrDelayT, /*"B相失流"*/
        (INT32U)&MntPara.LossCurrDelayT, /*"C相失流"*/
        (INT32U)&MntPara.BrkPhaseDelayT,/*"A相断相"*/
        (INT32U)&MntPara.BrkPhaseDelayT,/*"B相断相"*/
        (INT32U)&MntPara.BrkPhaseDelayT,/*"C相断相"*/
	 #endif
        #ifdef OVER_CURR_CHK
        (INT32U)&MntPara.OverCurrDelayT,  /*"A相过流"*/
        #endif
        #ifdef THREE_PHASE
        #ifdef OVER_CURR_CHK
	 (INT32U)&MntPara.OverCurrDelayT,  /*"B相过流"*/
        (INT32U)&MntPara.OverCurrDelayT,  /*"C相过流"*/
        #endif
        (INT32U)&MntPara.CutCurrDelayT,  /*"A相断流"*/
        (INT32U)&MntPara.CutCurrDelayT,  /*"B相断流"*/
        (INT32U)&MntPara.CutCurrDelayT,  /*"C相断流"*/
        (INT32U)&MntPara.OverVoltDelayT, /*"A相过压"*/
        (INT32U)&MntPara.OverVoltDelayT, /*"B相过压"*/
        (INT32U)&MntPara.OverVoltDelayT, /*"C相过压"*/
        (INT32U)&MntPara.UnderVoltDelayT,   /*"A相欠压"*/
        (INT32U)&MntPara.UnderVoltDelayT,   /*"B相欠压"*/
        (INT32U)&MntPara.UnderVoltDelayT,   /*"C相欠压"*/
        1,                                                             /*"潮流反向"*/
        (INT32U)&MntPara.PwrReverseDelayT,/*"A相功率反向"*/
        (INT32U)&MntPara.PwrReverseDelayT,/*"B相功率反向"*/
        (INT32U)&MntPara.PwrReverseDelayT,/*"C相功率反向"*/
        (INT32U)&MntPara.OverLoadDelayT,    /*"A相过载"*/
        (INT32U)&MntPara.OverLoadDelayT,    /*"B相过载"*/
        (INT32U)&MntPara.OverLoadDelayT,    /*"C相过载"*/
        (INT32U)&MntPara.AAOverDemandDelayT,/*"正向有功需量超限"*/
        (INT32U)&MntPara.NAOverDemandDelayT,/*"反向有功需量超限"*/
        (INT32U)&MntPara.ROverDemandDelayT, /*"I象限无功需量超限"*/
        (INT32U)&MntPara.ROverDemandDelayT,/*"II象限无功需量超限"*/
        (INT32U)&MntPara.ROverDemandDelayT,/*"III象限无功需量超限"*/
        (INT32U)&MntPara.ROverDemandDelayT, /*"IV象限无功需量超限"*/
        (INT32U)&MntPara.VoltImbalDelayT,       /*"电压不平衡"*/
        (INT32U)&MntPara.CurrImbalDelayT,       /*"电流不平衡"*/
        (INT32U)&MntPara.CurrBadlyImbalDelayT, /*"电流严重不平衡"*/
        (INT32U)&MntPara.OverFactorDelayT,    /*"总功率因数超下限"*/
        (INT32U)&MntPara.VoltReverseDelayT,   /*"电压逆相序"*/
        (INT32U)&MntPara.CurrReverseDelayT,   /*"电流逆相序"*/
	 #endif
    };
/*"************************************************"*/
/*"Function:    GE_JudgeEvtTime"*/
/*"Description:判断是否达到事件延时要求"*/
/*"Input: enumId:电网事件枚举序号；startOrEnd:事件起始或结束"*/
/*"Output: 无"*/
/*"Return: 是否满足条件 _TRUE:满足，_FALSE:不满足"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U GE_JudgeEvtTime(INT8U enumId , INT8U startOrEnd)
{
    INT8U result;
    INT8U DelayTimePara;

    if(DelayTimeList[enumId] < 0x1000)/*"延时参数为常数数据"*/
    {
        DelayTimePara = (INT8U) DelayTimeList[enumId];
    }
    else /*"延时参数为变量指针"*/
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
/*"Description: 电网事件时间累加,每秒调用"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
/*"Description: 清电网事件时调用"*/
/*"Input: enumId 电网事件的枚举号(见EN_NET_EVT)，0xff为全清"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
    if(MCU_RUNMODE == mode) /*"上电时判断是否跨月或日"*/
    {
        GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
        GetSingle(E_PD_TIME, (INT8U *)&pdTime);
        if((nowTime.year != pdTime.year)||(nowTime.month != pdTime.month)) /*"跨月时清月电压合格率"*/
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
         if((voltMode&0x01) != 0)  /*"三相三线"*/
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


NO_INIT ST_PDCURR stPdCurr; /*"低功耗电流"*/

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
INT32S minActPower[4];        /*"每分钟平均有功功率 总ABC"*/
INT32S minReActPower[4];    /*"每分钟平均无功功率 总ABC"*/
static INT64S minActPowerAcc[4];  
static INT64S minReActPowerAcc[4];
static INT16U minPowerTimes;
#else 
INT32S minActPower[2];        /*"每分钟平均有功功率 总"*/
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
    //if((mStNetEvtFlg[E_EVT_PWR_DOWN >> 3] & (0x01 << (E_EVT_PWR_DOWN & 0x07))) == 0)  /*"事件未发生"*/
    //    {           
    //         GE_SaveEvtSOrE(E_EVT_PWR_DOWN, E_PD_TIME, EVT_START);
   //     }
   INT32U d[2];

    d[0]=stMmtData.volt[0];
    d[1]=stMmtData.curr[0];
    FS_WriteFile(E_FILE_MNT, PDCURRVOLT_SHIFT,  (INT8U *)&d[0], 8);
}


