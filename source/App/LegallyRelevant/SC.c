/*"************************************************************"*/
/*"FileName: SC.c"*/
/*"Author:   Version : 1.0       Date: 2014-03-24"*/
/*" Description: 系统时钟处理"*/
/*" Function List:  主要函数及其功能"*/
/*" 1. void main(void)"*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/
#include "SC.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "App\message\FRMT_def.h"
#include "EU.h"
#include  "LRM.h"
#include "App\UserInterface\ProtocolStack\host_commu\host_commu.h"

NO_INIT ST_7TIME_FORM stSysTime;

#define  SYS_TIME_BAK_PEROID  (15)   /*电量备份周期15分钟*/

/*"系统默认时钟"*/
#if (APP_TIME_FRMT == HEX_FRMT)
const ST_7TIME_FORM Default_Time = {17, 11, 29, 0, 0, 0, 3}; /*"system time powerup err default time"*/
#else
const ST_7TIME_FORM Default_Time = {0x17, 0x11, 0x29, 0, 0, 0, 3}; /*"system time powerup err default time"*/
#endif
static INT8U mRtcErrCnt;   /*"RTC时钟持续错误次数累计，每分钟判断一次"*/
static INT8U mRtcErrTimes;   /*"RTC时钟持续错误小时数"*/
static INT8U mMinFlg;/*"分钟标志，当检测的分钟任务到时置位，分钟任务处理完后清零，用于错开正分处理任务"*/
static INT8U SystimeBakPeriodCnt;/*"时钟备份周期时间累计变量，单位分"*/

static void SC_Task1min(void);

/*"*****************************************************************************"*/
/*"  Function:       SC_SysClkInit"*/
/*"  Description:    系统时钟上电初始化处理，上电RTC时钟异常时，当前系统时钟采用掉电时间"*/
/*"  Calls:          ST_Init"*/
/*"  Called By:      上电初始化"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define  MAX_RTC_SOFT_TIME_DELTA    (30UL * 60UL) /*"RTC时钟与软时钟最大允许时间差RTC > SOFT"*/
#define  MAX_SOFT_RTC_TIME_DELTA    (30UL * 60UL) /*"RTC时钟与软时钟最大允许时间差 SOFT > RTC"*/
#define  MAX_RTC_PDTIME_DELTA       (3UL * 365UL * 24UL * 60UL * 60UL)/*"RTC时钟与掉电时间最大允许时间差 SOFT > RTC"*/
void SC_SysClkInit(INT8U Mode)
{
    ST_7TIME_FORM nowTime;        /*"当前时间"*/
    INT8U pwrUpRtcChk;    /*"上电RTC时间合法性标志"*/
#if 0
    ST_7TIME_FORM LastprdFrzTime;  /*"上次周期冻结时间"*/
#endif
    INT32S deltaT;

    /*"模块内全局变量初始化"*/
    mRtcErrCnt = 0;
    mRtcErrTimes = 0;
    mMinFlg = 0;
    SystimeBakPeriodCnt = 0;

    pwrUpRtcChk = ST_Init(Mode, E_IN_RTC);//E_IN_RTC
    if((FIRST_PWR_ON == Mode) || (MCU_RUNMODE == Mode))
    {
        Task_TimerTaskRegister(TIMERTASK_ST, ST_secPulseChk);
    }
    if(RTC_OK != pwrUpRtcChk)/*"powerup rtc err"*/
    {
#if 0
        DC_GetData(E2_EVTRPT_BLK, (INT8U *)&evtRptStus, FIND(ST_EVTRPT_BLK,unNewRptStatus),
                   sizeof(UN_EVTRPT_STATUS));
        if(1 == evtRptStus.bits.RtcErr)/*"掉电前RTC故障已经发生，直接保持RTC故障状态，避免事件因掉上电而重复上报"*/
        {
            unOutputCtrl.bits.clkErr = 1;
        }
#endif
    }
    if(RTC_OK == pwrUpRtcChk)/*"RTC合法时，与系统时钟进行比较，检查RTC有效性"*/
    {
        ST_SynStClkToSysClk((INT8U *)&nowTime);/*"取RTC时钟"*/
#if 0
        if(TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&stSysTime))
        {
            deltaT = LIB_CmpTime((INT8U *)&nowTime, (INT8U *)&stSysTime);
            if(deltaT > 0)/*"RTC > SOFT"*/
            {
                if(deltaT > _MAX_RTC_SOFT_TIME_DELTA)
                {
                    pwrUpRtcChk = FALSE;
                }
            }
            else/*"SOFT >= RTC"*/
            {
                deltaT = 0 - deltaT;
                if(deltaT > _MAX_SOFT_RTC_TIME_DELTA)
                {
                    pwrUpRtcChk = FALSE;
                }
            }
        }
        else
        {
            pwrUpRtcChk = FALSE;
        }
#else
        if((FALSE == ChkStatus(E_LRM_PD_DATA_ERR)) && (TRUE == LIB_ChkDateTime((ST_6TIME_FORM *) &stPdSaveData.pdTime)))
        {
            deltaT = LIB_CmpTime((ST_6TIME_FORM *)&nowTime, &stPdSaveData.pdTime);
            if(deltaT < 0)/*"当前RTC时间在掉电时间前，RTC时钟异常"*/
            {
                pwrUpRtcChk = FALSE;
            }
            else
            {
                if(deltaT > MAX_RTC_PDTIME_DELTA)
                {
                    pwrUpRtcChk = FALSE;
                }
            }
        }
        else
        {
        }
#endif
    }

    if((RTC_OK != pwrUpRtcChk))
    {
        if(TRUE == LIB_ChkDateTime((ST_6TIME_FORM *) &stPdSaveData.pdTime) && (FALSE == ChkStatus(E_LRM_PD_DATA_ERR)))
        {
            /*"当前时间改为掉电时间"*/
            LIB_MemCpy((INT8U *)&stPdSaveData.pdTime, (INT8U *)&stSysTime, sizeof(ST_6TIME_FORM));
        }
        else
        {
            INT8U i;
            INT8U ret;
            ST_6TIME_FORM SystimeBakTmp;

            for (i = 0; i < MAX_READ_FILE_LOOP; i++)
            {
                ret = FS_ReadFile(E_FILE_LRM_DATA, SYS_TIME_BAK, (INT8U *) &SystimeBakTmp, sizeof(ST_6TIME_FORM));/*读出EEP备份时钟*/
                if ((ret == TRUE) && (TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&SystimeBakTmp)))
                {
                    break;
                }
                MCU_DelayMs(5);
            }
            if(i < MAX_READ_FILE_LOOP)
            {
                LIB_MemCpy((INT8U *)&SystimeBakTmp, (INT8U *)&stSysTime, sizeof(ST_6TIME_FORM));
            }
            else
            {
                stSysTime = Default_Time;
            }
        }

        ST_RefSysClk((INT8U *)&stSysTime, 0);
        Push(E_CLK_ERR, TRUE);
    }

    ST_SynStClkToSysClk((INT8U *)&stSysTime);
}
/*"*****************************************************************************"*/
/*"  Function:       SC_main"*/
/*"  Description:    系统时钟主接口函数，获取软时钟为当前系统时钟，每分钟进行时钟同步并检测RTC是否故障"*/
/*"  Calls:          ST_GetSysClk、SC_Task1min"*/
/*"  Called By:      主循环"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void SC_main (void)
{
    ST_GetSysClk((INT8U *)&stSysTime);

    if(TRUE == Task_ChkTsk_1min(MAINTASK_SC))
    {
        mMinFlg = 0x5a;
        SystimeBakPeriodCnt++;
        if(SystimeBakPeriodCnt >= SYS_TIME_BAK_PEROID)
        {
            FS_WriteFile(E_FILE_LRM_DATA, SYS_TIME_BAK, (INT8U *) &stSysTime, sizeof(ST_6TIME_FORM));
            SystimeBakPeriodCnt = 0;
        }
    }
    if(TRUE == Task_ChkTsk_1s(MAINTASK_SC))
    {
        if(0x5a == mMinFlg)
        {
#if(APP_TIME_FRMT == HEX_FRMT)
            if((stSysTime.sec > 30) && (stSysTime.sec < 35))
#else
            if((stSysTime.sec > 0x30) && (stSysTime.sec < 0x35))
#endif
            {
                SC_Task1min();
                Host_Commu_Bm_Data_Sync(3);
                mMinFlg = 0;
            }
        }
    }
}

#define RTC_ERR_CNT  10
#define RTC_ERR_TIMES  2
/*"*****************************************************************************"*/
/*"  Function:       SC_Task1min"*/
/*"  Description:    系统时钟模块分钟任务处理接口函数，主要完成软时钟与硬时钟同步，RTC错误检测"*/
/*"                  已经运行时钟乱事件记录"*/
/*"  Calls:          ST_SynSysClk"*/
/*"  Called By:      SC_main"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         模块从数据中心获取数据内容："*/
/*"                  OUTPUT_CTRL  RTC错误标志                  "*/
/*"                  模块输出数据到数据中心内容："*/
/*"                  OUTPUT_CTRL   RTC错误标志      "*/
/*"*****************************************************************************"*/
static void SC_Task1min(void)
{
    if(ST_SynSysClk() != RTC_OK)/*"RTC CHECK ERR"*/
    {
        mRtcErrCnt++;
    }
    else
    {
        mRtcErrCnt = 0;
        Push(E_CLK_ERR, FALSE);
        mRtcErrTimes = 0;
    }
    if(mRtcErrCnt >= RTC_ERR_CNT)/*"连续mRtcErrCnt同步错误，RTC进行重新初始化处理"*/
    {
        mRtcErrTimes++;
        mRtcErrCnt = 0;
        ST_Init(MCU_RUNMODE, E_IN_RTC);
        ST_SynStClkToSysClk((INT8U *)&stSysTime);
        ST_RefSysClk((INT8U *)&stSysTime, 0);
        if(mRtcErrTimes >= RTC_ERR_TIMES)
        {
            Push(E_CLK_ERR, TRUE);
        }
    }
}
/*"*****************************************************************************"*/
/*"  Function:       SC_Task1SecPD"*/
/*"  Description:    低功耗系统时钟运行主接口函数，每秒唤醒时进行系统时钟更新"*/
/*"  Calls:          ST_GetSysClkPd"*/
/*"  Called By:      低功耗下秒任务处理"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void SC_Task1SecPD(void)
{
    ST_GetSysClkPd((INT8U *)&stSysTime);
}

INT8S LRM_JustTime(void *pdin, void *pdout)
{
    INT8S result;

    result = MS_SUCCESS;
    
    if(TRUE == ST_RefSysClk((INT8U *)pdin, 0))
    {
        ST_SynStClkToSysClk((INT8U *)&stSysTime);
        Push(E_CLK_ERR, FALSE);
    }
    else
    {
        result = MS_OTHER_ERR;
    }

    return result;

} 
