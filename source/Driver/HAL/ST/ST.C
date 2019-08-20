/*"************************************************************"*/
/*"FileName: ST.c"*/
/*"Author:     Version : 1.0       Date: "*/
/*" Description: 软时钟实现，系统时钟抗干扰处理，模块分时任务判断"*/
/*" Function List:  "*/
/*" 1. void main(void)"*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/
#include "Driver\Device\RTC_8025T.h"
#include "Driver\MCU\RTC.h"
#include "Driver\HAL\ST\ST.h"
#include "Lib\LIB_func.h"
#include "Driver\MCU\mcu.h"

#define  RTC_CHK_TIMES    15          /*"RTC操作异常重复次数"*/
#define  RTC_ABSENT_CHK_TIMES   10   /*"RTC连续访问异常累计次数，判断RTC设备不存在"*/

const ST_RTC_INTERFACE_LIST mstRtcInterfList[E_RTC_SCR_NUM] =
        {
                {EXRTC_Init, EXRTC_ReadTime, INRTC_WriteTime,
#if (SEC_PULSE_IN == TRUE)
                        NULL, NULL,&exSecCnt
#endif
                },
                {INRTC_Init, INRTC_ReadTime, INRTC_WriteTime,
#if (SEC_PULSE_IN == TRUE)
                        NULL, NULL, &gsecCnt
#endif
                },
        };

static ST_7TIME_FORM mSystemTime;/*"系统软时钟（MCU定时器产生时钟）"*/
static ST_7TIME_FORM mSystemTimeBak;/*"系统软时钟备份"*/
#if (SEC_PULSE_IN == TRUE)
static INT8U  mLastPulseCnt;/*"保存上次读取的秒脉冲累计数，用于算秒数差"*/
static volatile INT8U  mPwrUpSecFlg;/*"上电第一个秒脉冲输出标志"*/
#endif
static volatile INT16U mTimePulseMsAdd;/*"秒中断脉冲故障时用定时器补救"*/
static volatile INT8U mSecCnt;/*"容错处理后的秒累计"*/
static INT8U mLastSecCnt;/*"保存上次读取的容错处理后的秒累计，用于计算秒数差"*/
ST_TIME_SLICE_FLG mTimeSliceFlg;/*"时间片标志"*/
static EN_RTC_SCR_ID  mRtcHdId;/*"硬件RTC选择ID号"*/
static INT8U  mRtcHdErrcnt;

/*"--------------public function---------------"*/
INT8U ST_Init(INT8U Mode,EN_RTC_SCR_ID ClkScr);
INT8U ST_RefSysClk(INT8U *pTime, INT8U mode);
void ST_GetSysClk(INT8U *pTime);
INT8U ST_SynSysClk(void);

/*"--------------private function-------------------"*/
static INT8U ReadAndChkRTC(void);
static INT8U SoftTimeAdd(void);
static void SoftTimeChk(void);
static INT8U SysTimeAdd(void);
static INT8U Calendar_Add(INT8U *date);

/*"*****************************************************************************"*/
/*"  Function:       ST_Init"*/
/*"  Description:    初始化软时钟，根据应用层指定时钟源，上电检测RTC合法性，"*/
/*"                  如果RTC合法，上电软时钟与RTC时钟同步"*/
/*"  Calls:          "*/
/*"  Called By:      硬件初始化主接口函数中"*/
/*"  Input:          Mode 运行模式（正常上电，低功耗）"*/
/*"                  ClkScr指定硬件时钟源 E_EX_RTC 时钟源选择外置RTC， E_IN_RTC 时钟源选择内置RTC"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         RTC_OK  RTC合法  RTC_ERR  RTC不合法   RTC_ABSENT RTC不存在"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ST_Init(INT8U Mode,EN_RTC_SCR_ID ClkScr)
{
    INT8U ret;
    INT8U InitMode;

    ret = RTC_OK;

    /*"初始化模块内部全局变量"*/
    mTimeSliceFlg.byte = 0;

#if (SEC_PULSE_IN == TRUE)/*"有秒脉冲输入中断作为软时钟时钟源，初始化秒中断容错处理变量"*/
    mLastPulseCnt = 0;
    if(Mode == MCU_RUNMODE)
    {
        mPwrUpSecFlg = 0x55;
    }
    else
    {
        mPwrUpSecFlg = 0;
    }
#endif
    mTimePulseMsAdd = 0;
    mSecCnt = 0;
    mLastSecCnt = 0;
    mRtcHdErrcnt = 0;

    mRtcHdId = ClkScr;

#if (SEC_PULSE_IN == TRUE)
    *(mstRtcInterfList[mRtcHdId].pSecAddCnt) = 0;
#endif
    if((NULL != mstRtcInterfList[mRtcHdId].init) && (mRtcHdId < E_RTC_SCR_NUM))
    {
        if(mRtcHdId == E_EX_RTC)
        {
            InitMode = INIT_MODE1_8025T;
        }
        else
        {
            InitMode = Mode;
        }
        ret = mstRtcInterfList[mRtcHdId].init(InitMode);
        if(TRUE == ret)
        {
            ret = ReadAndChkRTC();
        }
        else
        {
            ret = RTC_ABSENT;
        }
    }
    else
    {
        ret = RTC_ABSENT;
    }

    return ret;
}
/*"*****************************************************************************"*/
/*"  Function:       ST_RefSysClk"*/
/*"  Description:    应用层与硬件层接口函数，将应用层时间格式转换为底层时钟格式后写RTC"*/
/*"  Calls:          "*/
/*"  Called By:      协议处理"*/
/*"  Input:          pTime  应用层输出时钟数据指针，要求时间格式顺序为年月日时分秒周次"*/
/*"                         时间格式为HEX"*/
/*"                  mode  0 更新RTC和软时钟     1 只更新软时钟，不更新RTC时钟"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ST_RefSysClk(INT8U *pTime, INT8U mode)
{
    INT8U ret;
    INT8U i;
    ST_7TIME_FORM TimeTmp;

#if (APP_TIME_FRMT == HEX_FRMT)/*""*/
        for(i = 0; i < 6; i++)
        {
            *((INT8U *)&TimeTmp + i) = LIB_CharToBcd(*(pTime + i));
        }
#else
        LIB_MemCpy((INT8U *)pTime, (INT8U *)&TimeTmp, sizeof(ST_7TIME_FORM));
#endif

        TimeTmp.week = LIB_CalWeekBCD((ST_6TIME_FORM *)&TimeTmp);

    if(0 == mode)
    {
//        DB_Printf("WRITE RTC MODE \n");
        if((NULL != mstRtcInterfList[mRtcHdId].writeTime) && (mRtcHdId < E_RTC_SCR_NUM))
        {
            ret = mstRtcInterfList[mRtcHdId].writeTime((INT8U *)&TimeTmp);
        }
    }
    else
    {
        ret = TRUE;
    }

    if(TRUE == ret)
    {
        /*"备份时钟同步"*/
        LIB_MemCpy((INT8U *)&TimeTmp, (INT8U *)&mSystemTimeBak, sizeof(ST_7TIME_FORM));
        LIB_MemCpy((INT8U *)&TimeTmp, (INT8U *)&mSystemTime, sizeof(ST_7TIME_FORM));
    }

    return ret;
}


/*"*****************************************************************************"*/
/*"  Function:       ReadAndChkRTC"*/
/*"  Description:    上电检查RTC的有效性，判断方法为，连续两次读RTC时钟值一致则认为"*/
/*"                  RTC有效，最多重复RTC_CHK_TIMES次，如果重复"*/
/*"                  RTC_CHK_TIMES次都没有两次RTC时钟值一致,则认为RTC故障"*/
/*"  Calls:          "*/
/*"  Called By:      PowerUp_Init_RTC"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         RTC_OK  RTC合法  RTC_ERR  RTC不合法   RTC_ABSENT RTC不存在"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ReadAndChkRTC(void)
{
    ST_7TIME_FORM time_temp;
    INT8U result;
    INT8U i;

    i = 0;
    result = RTC_OK;
    for(i = 0; i < RTC_CHK_TIMES; i++)
    {
        if((NULL != mstRtcInterfList[mRtcHdId].readTime) && (mRtcHdId < E_RTC_SCR_NUM))
        {
            result = mstRtcInterfList[mRtcHdId].readTime((INT8U *)&time_temp);
        }
        else
        {
            result = FALSE;
        }
        if(result == TRUE)
        {
            mRtcHdErrcnt = 0;
            result = LIB_ChkDateTimeBCD((ST_6TIME_FORM *)&time_temp);
        }
        else/*"读RTC数据不成功，连续N次则判断该RTC设备不存在"*/
        {
            mRtcHdErrcnt++;
        }

        if(result == TRUE)
        {
            break;
        }
        else
        {
            MCU_DelayMs(1);
        }
    }

    if(TRUE == result)
    {
        LIB_MemCpy((INT8U *)&time_temp, (INT8U *)&mSystemTimeBak, sizeof(ST_7TIME_FORM));
        LIB_MemCpy((INT8U *)&time_temp, (INT8U *)&mSystemTime, sizeof(ST_7TIME_FORM));
    }
    else
    {
        if(mRtcHdErrcnt >= RTC_ABSENT_CHK_TIMES)
        {
            result = RTC_ABSENT;
        }
        else
        {
            result = RTC_ERR;
        }
    }


    return result;
}

/*"*****************************************************************************"*/
/*"  Function:       ST_GetSysClk"*/
/*"  Description:    将合法有效的当前时间返回应用层作为当前系统时钟"*/
/*"  Calls:          "*/
/*"  Called By:      应用层时钟模块主函数实时调用"*/
/*"  Input:          "*/
/*"  Output:         time 当前系统时钟输出数据缓存"*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
/*"system time run err default time"*/
const ST_7TIME_FORM RunErrTime = {0x18, 0x1, 0x23, 0x00, 0x00, 0x00, 0x02};
void ST_GetSysClk(INT8U *ptime)
{
    if(mRtcHdId >= E_RTC_SCR_NUM)/*"对时钟源选择变量进行容错处理"*/
    {
        INT8U i;
        for(i = 0; i < E_RTC_SCR_NUM; i++)
        {
            if(NULL !=mstRtcInterfList[i].init)
            {
                mRtcHdId = (EN_RTC_SCR_ID)i;
                break;
            }
        }
    }
    if(TRUE == SoftTimeAdd())
    {
        ST_SynStClkToSysClk(ptime);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       ST_SynStClkToSysClk"*/
/*"  Description:    同步软时钟到系统时钟"*/
/*"  Calls:          "*/
/*"  Called By:      SC"*/
/*"  Input:          "*/
/*"  Output:         ptime 当前系统时钟输出数据缓存"*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void ST_SynStClkToSysClk(INT8U *ptime)
{
    MCU_PwrChkCloseINT();
    LIB_MemCpy((INT8U *) &mSystemTime, (INT8U *) ptime, sizeof(ST_7TIME_FORM));
#if (APP_TIME_FRMT == HEX_FRMT)/*""*/
    {
        INT8U i;

        for (i = 0; i < sizeof(ST_7TIME_FORM); i++)
        {
            *(ptime + i) = LIB_BcdToChar(*(ptime + i));
        }
    }
#endif
    MCU_PwrChkOpenINT();
}
/*"*****************************************************************************"*/
/*"  Function:       SoftTimeChk"*/
/*"  Description:    软时钟合法性检查及异常处理"*/
/*"  Calls:          "*/
/*"  Called By:      SoftTimeAdd"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:        无"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void SoftTimeChk(void)
{
    INT8U SoftTimeErrFlg;
    INT8U ret;
    ST_7TIME_FORM  RtcTime;

    SoftTimeErrFlg = 0;
    ret = TRUE;

    if(FALSE == LIB_ChkDateTimeBCD((ST_6TIME_FORM *)&mSystemTime))/*"软时钟数据异常"*/
    {
        if(FALSE == LIB_ChkDateTimeBCD((ST_6TIME_FORM *)&mSystemTimeBak))/*"软时钟备份数据异常"*/
        {
            SoftTimeErrFlg = 0xff;
        }
        else
        {
            LIB_MemCpy((INT8U *)&mSystemTimeBak, (INT8U *)&mSystemTime, sizeof(ST_7TIME_FORM));
        }
    }
    else
    {
    }

    if(0 != SoftTimeErrFlg)/*"软时钟异常，读RTC时钟恢复软时钟"*/
    {
        if((NULL != mstRtcInterfList[mRtcHdId].readTime) && (mRtcHdId < E_RTC_SCR_NUM))
        {
            ret = mstRtcInterfList[mRtcHdId].readTime((INT8U *)&RtcTime);
        }
        else
        {
            ret = FALSE;
        }
        if(TRUE == ret)
        {
            ret = LIB_ChkDateTimeBCD((ST_6TIME_FORM *)&RtcTime);
        }
        if(TRUE == ret)
        {
            LIB_MemCpy((INT8U *)&RtcTime, (INT8U *)&mSystemTime,sizeof(ST_7TIME_FORM));
        }
        else/*"运行过程中软时钟，硬时钟都故障，软时钟先采用默认时钟"*/
        {
            mSystemTime = RunErrTime;
        }
    }
}
/*"*****************************************************************************"*/
/*"  Function:       ST_SynSysClk"*/
/*"  Description:    定时器时钟，秒脉冲时钟与RTC时钟同步，每分钟调用"*/
/*"                  同步的前提条件是RTC时钟数据合法，且与合法的定时器时钟或秒脉冲时钟误差在设定的范围内"*/
/*"  Calls:          "*/
/*"  Called By:      应用层时钟模块主函数"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ST_SynSysClk(void)
{
    INT8U result;

    result = ReadAndChkRTC();

    return result;
}
#define INTERVAL_MAX  (10)        /*"定时器允许主循环占用最大时间，单位秒"*/
/*"*****************************************************************************"*/
/*"  Function:       SoftTimeAdd"*/
/*"  Description:    软时钟时钟累计"*/
/*"  Calls:          "*/
/*"  Called By:      GetSystemTime"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE  TIME RENEW   FALSE  TIME REMAIN "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U SoftTimeAdd(void)
{
    INT8U NowSec;
    INT8U i;
    INT8U Dec_s;
    INT8U rsult;

    NowSec = mSecCnt;
    Dec_s = (INT8U)(NowSec - mLastSecCnt);

    rsult = FALSE;
    if(Dec_s > INTERVAL_MAX)/*"主循环运行最大时间差，秒数异常检测"*/
    {
        mLastSecCnt = NowSec;
        return  rsult;
    }

    if(Dec_s > 0)
    {
        mTimeSliceFlg.bit.flg_1s += 1;
        SoftTimeChk();
        for(i = 0; i < Dec_s; i++)
        {
            if(SysTimeAdd())/*"秒脉冲时钟累计，整分到时，置整分时间标志"*/
            {
                mTimeSliceFlg.bit.flg_1min = 1;
                if(mSystemTime.day != mSystemTimeBak.day)/*"软时钟跨天时计算软时钟周次"*/
                {
                    mSystemTime.week = LIB_CalWeekBCD((ST_6TIME_FORM *)&mSystemTime);
                }
            }
        }
        LIB_MemCpy((INT8U *) &mSystemTime, (INT8U *) &mSystemTimeBak, sizeof(ST_7TIME_FORM));
        mLastSecCnt = NowSec;
        rsult = TRUE;
    }
    return  rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       SysTimeTmrAdd"*/
/*"  Description:    定时器时钟累计，当定时器毫秒数大于1000ms时调用"*/
/*"  Calls:          "*/
/*"  Called By:      TimeAdd"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         整分到标志，用于分钟任务时间片标志置位处理"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U SysTimeAdd(void)
{
    INT8U minFlg;

    minFlg = Calendar_Add((INT8U *) &mSystemTime);

    return minFlg;
}
/*"*****************************************************************************"*/
/*"  Function:       Calendar_Add"*/
/*"  Description:    按日历格式进行定时器时钟和秒脉冲时钟时钟日历累加"*/
/*"  Calls:          "*/
/*"  Called By:      SysTimeTmrAdd，PulseTimeAdd"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         整分到标志"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Calendar_Add(INT8U *data)
{
    INT8U Minute_Flg = 0;

    if(data[SEC] < 0x59)
    {
        data[SEC] = LIB_BcdByteAdd(data[SEC]);
    }
    else /*"到整分"*/
    {
        Minute_Flg = 1;
        data[SEC] = 0;
        if(data[MINU] < 0x59)
        {
            data[MINU] = LIB_BcdByteAdd(data[MINU]);
        }
        else /*"到整小时"*/
        {
            data[MINU] = 0;
            if(data[HOUR] < 0x23)
            {
                data[HOUR] = LIB_BcdByteAdd(data[HOUR]);
            }
            else /*"到0点"*/
            {
                data[HOUR] = 0;
                if(LIB_BcdToChar(data[DAY]) < LIB_CalDayBCD(data[YEAR], data[MON]))
                {
                    data[DAY] = LIB_BcdByteAdd(data[DAY]);
                }
                else /*"到月末"*/
                {
                    data[DAY] = 1;
                    if(data[MON] < 0x12)
                    {
                        data[MON] = LIB_BcdByteAdd(data[MON]);
                    }
                    else
                    {
                        data[MON] = 1;
                        data[YEAR] = LIB_BcdByteAdd(data[YEAR]);
                    }
                }
            }
        }
    }
    return (Minute_Flg);
}

/*"*****************************************************************************"*/
/*"  Function:       ST_GetSysClkPd"*/
/*"  Description:    低功耗软时钟运行接口函数，需在秒任务中调用，否则软时钟异常"*/
/*"  Calls:          SysTimeAdd"*/
/*"  Called By:      低功耗秒任务接口函数中"*/
/*"  Input:          "*/
/*"  Output:         ptime 软时钟输出缓存指针"*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void ST_GetSysClkPd(INT8U *ptime)
{
    SysTimeAdd();
    LIB_MemCpy((INT8U *)&mSystemTime, (INT8U *)ptime, sizeof(ST_7TIME_FORM));
#if (APP_TIME_FRMT == HEX_FRMT)/*""*/
    {
        INT8U i;

        for(i = 0; i < sizeof(ST_7TIME_FORM); i++)
        {
            *(ptime + i) = LIB_BcdToChar(*(ptime+i));
        }
    }
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       ST_secPulseChk"*/
/*"  Description:    秒脉冲容错处理，在10ms中断函数中调用，采用时基定时器累计毫秒数进行比较容错"*/
/*"  Calls:          "*/
/*"  Called By:      Task10msInt"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define   ERR_LIMIT      (200)
#define   ERR_LIMIT_UP   (1000 + ERR_LIMIT)
#define   ERR_LIMIT_DOWN (1000 - ERR_LIMIT)

void ST_secPulseChk(void)
{
#if (SEC_PULSE_IN == TRUE)
    INT8U Pulse_Count_Now;
#endif
    INT8U secDec;

#if (SEC_PULSE_IN == TRUE)
    Pulse_Count_Now = *(mstRtcInterfList[mRtcHdId].pSecAddCnt);
    secDec = (INT8U)(Pulse_Count_Now - mLastPulseCnt);
#else
    secDec = 0;
#endif

    mTimePulseMsAdd += SYS_TICK_MS;       /*"定时器为10ms"*/

    if(1 == secDec)/*"秒数有增加1"*/
    {
#if (SEC_PULSE_IN == TRUE)
        if((mTimePulseMsAdd < ERR_LIMIT_DOWN) && (0x55 != mPwrUpSecFlg))/*"second pulse is not valid"*/
        {

        }
        else /*"second pulse is valid"*/
        {
            mSecCnt++;
            mTimePulseMsAdd = 0;
            mPwrUpSecFlg = 0xaa;
        }
        mLastPulseCnt = Pulse_Count_Now;
#else
        mSecCnt = 0;
#endif
    }
    else if(0 == secDec)/*"秒数没有增加"*/
    {
        if(mTimePulseMsAdd > ERR_LIMIT_UP)
        {
            mSecCnt++;
            mTimePulseMsAdd -= 1000;
        }
    }
    else/*"秒数增加超过1，秒脉冲有问题，秒数不累计"*/
    {
#if (SEC_PULSE_IN == TRUE)
        mLastPulseCnt = Pulse_Count_Now;
#endif
    }
}
