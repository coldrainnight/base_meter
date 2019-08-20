/*"************************************************************"*/
/*"FileName: SC.c"*/
/*"Author:   Version : 1.0       Date: 2014-03-24"*/
/*" Description: ϵͳʱ�Ӵ���"*/
/*" Function List:  ��Ҫ�������书��"*/
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

#define  SYS_TIME_BAK_PEROID  (15)   /*������������15����*/

/*"ϵͳĬ��ʱ��"*/
#if (APP_TIME_FRMT == HEX_FRMT)
const ST_7TIME_FORM Default_Time = {17, 11, 29, 0, 0, 0, 3}; /*"system time powerup err default time"*/
#else
const ST_7TIME_FORM Default_Time = {0x17, 0x11, 0x29, 0, 0, 0, 3}; /*"system time powerup err default time"*/
#endif
static INT8U mRtcErrCnt;   /*"RTCʱ�ӳ�����������ۼƣ�ÿ�����ж�һ��"*/
static INT8U mRtcErrTimes;   /*"RTCʱ�ӳ�������Сʱ��"*/
static INT8U mMinFlg;/*"���ӱ�־�������ķ�������ʱ��λ������������������㣬���ڴ����ִ�������"*/
static INT8U SystimeBakPeriodCnt;/*"ʱ�ӱ�������ʱ���ۼƱ�������λ��"*/

static void SC_Task1min(void);

/*"*****************************************************************************"*/
/*"  Function:       SC_SysClkInit"*/
/*"  Description:    ϵͳʱ���ϵ��ʼ�������ϵ�RTCʱ���쳣ʱ����ǰϵͳʱ�Ӳ��õ���ʱ��"*/
/*"  Calls:          ST_Init"*/
/*"  Called By:      �ϵ��ʼ��"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define  MAX_RTC_SOFT_TIME_DELTA    (30UL * 60UL) /*"RTCʱ������ʱ���������ʱ���RTC > SOFT"*/
#define  MAX_SOFT_RTC_TIME_DELTA    (30UL * 60UL) /*"RTCʱ������ʱ���������ʱ��� SOFT > RTC"*/
#define  MAX_RTC_PDTIME_DELTA       (3UL * 365UL * 24UL * 60UL * 60UL)/*"RTCʱ�������ʱ���������ʱ��� SOFT > RTC"*/
void SC_SysClkInit(INT8U Mode)
{
    ST_7TIME_FORM nowTime;        /*"��ǰʱ��"*/
    INT8U pwrUpRtcChk;    /*"�ϵ�RTCʱ��Ϸ��Ա�־"*/
#if 0
    ST_7TIME_FORM LastprdFrzTime;  /*"�ϴ����ڶ���ʱ��"*/
#endif
    INT32S deltaT;

    /*"ģ����ȫ�ֱ�����ʼ��"*/
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
        if(1 == evtRptStus.bits.RtcErr)/*"����ǰRTC�����Ѿ�������ֱ�ӱ���RTC����״̬�������¼�����ϵ���ظ��ϱ�"*/
        {
            unOutputCtrl.bits.clkErr = 1;
        }
#endif
    }
    if(RTC_OK == pwrUpRtcChk)/*"RTC�Ϸ�ʱ����ϵͳʱ�ӽ��бȽϣ����RTC��Ч��"*/
    {
        ST_SynStClkToSysClk((INT8U *)&nowTime);/*"ȡRTCʱ��"*/
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
            if(deltaT < 0)/*"��ǰRTCʱ���ڵ���ʱ��ǰ��RTCʱ���쳣"*/
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
            /*"��ǰʱ���Ϊ����ʱ��"*/
            LIB_MemCpy((INT8U *)&stPdSaveData.pdTime, (INT8U *)&stSysTime, sizeof(ST_6TIME_FORM));
        }
        else
        {
            INT8U i;
            INT8U ret;
            ST_6TIME_FORM SystimeBakTmp;

            for (i = 0; i < MAX_READ_FILE_LOOP; i++)
            {
                ret = FS_ReadFile(E_FILE_LRM_DATA, SYS_TIME_BAK, (INT8U *) &SystimeBakTmp, sizeof(ST_6TIME_FORM));/*����EEP����ʱ��*/
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
/*"  Description:    ϵͳʱ�����ӿں�������ȡ��ʱ��Ϊ��ǰϵͳʱ�ӣ�ÿ���ӽ���ʱ��ͬ�������RTC�Ƿ����"*/
/*"  Calls:          ST_GetSysClk��SC_Task1min"*/
/*"  Called By:      ��ѭ��"*/
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
/*"  Description:    ϵͳʱ��ģ�����������ӿں�������Ҫ�����ʱ����Ӳʱ��ͬ����RTC������"*/
/*"                  �Ѿ�����ʱ�����¼���¼"*/
/*"  Calls:          ST_SynSysClk"*/
/*"  Called By:      SC_main"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         ģ����������Ļ�ȡ�������ݣ�"*/
/*"                  OUTPUT_CTRL  RTC�����־                  "*/
/*"                  ģ��������ݵ������������ݣ�"*/
/*"                  OUTPUT_CTRL   RTC�����־      "*/
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
    if(mRtcErrCnt >= RTC_ERR_CNT)/*"����mRtcErrCntͬ������RTC�������³�ʼ������"*/
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
/*"  Description:    �͹���ϵͳʱ���������ӿں�����ÿ�뻽��ʱ����ϵͳʱ�Ӹ���"*/
/*"  Calls:          ST_GetSysClkPd"*/
/*"  Called By:      �͹�������������"*/
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
