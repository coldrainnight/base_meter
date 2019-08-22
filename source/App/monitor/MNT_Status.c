/*"************************************************************"*/
/*"FileName: MNT_Status.c"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: 电能表状态监测模块状态检查相关处理"*/
/*" 总字节数:          "*/
/*" Function List:                     "*/
/*"                                    "*/
/*" History:                           "*/
/*" <author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "Driver\MCU\GPIO.h"
#include "MNTConfig.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\monitor\MNT.h"
#include "App\message\MS.h"
#include "Driver\MCU\AdcDrv.h"
#include "Driver\device\HTSensor.h"

static ST_7TIME_FORM mMntNowTime;  /*当前时间*/

static INT16U mFacAbleTime;    /*"厂内状态时间定时器"*/

#ifdef BAT_VOLT_CHK
static INT8U BatErrNum[BAT_NUM];     /*"电池欠压判断延时"*/
#endif

#ifdef BAT_VOLT_CAL
INT16U BatVolt[BAT_NUM];   /*"电池电压"*/
#define BAT_UNKOWN (0xFF)
#endif

#define FAC_TIMER  14400   /*"厂内状态有效时间,10D,单位min"*/

#ifdef OPEN_COVER_CHK
static INT8U mCoverStatus[COVER_NUM];/*"盖状态  0合盖，1开盖"*/
#ifdef PD_OPEN_COVER_CHK
/*"----低功耗开盖数据定义------"*/
typedef struct
{
    INT16U keyWord;  /*"低功耗下的特征字，0xA5A5"*/
    INT8U lastState; /*"最后一次开合盖后状态 1 open , 0 close"*/
    INT8U times; /*"低功耗下开盖次数"*/
    ST_7TIME_FORM startTime; /*"开始时间"*/
    ST_7TIME_FORM endTime;/*"结束时间"*/
    CRC_DATA_TP crc;
}ST_OPENCVR_PD;
NO_INIT static ST_OPENCVR_PD stPdOpenCvrRcd[COVER_NUM];/*"低功耗下开盖记录数据"*/
#endif
#endif

#ifdef TMNL_TMP_CHK
INT16S gTmnlTmp[E_TEMP_MAX];
INT16S gMtrTmp;    /*"表内温度"*/
INT16U gMtrHmdty;  /*"表内湿度"*/
INT8U   gMtrTHCnt;  /*"表内温湿度测量计数器，0 开始测量，1 读取数值"*/
#endif

#ifdef GLX_CHG_CHK
static INT8U gGlxChgFlag;   /*"管理芯插拔事件标志"*/
static INT8U gGlxChgCnt;     /*"管理芯插拔事件累计时间"*/
INT8U gGlxChgEsamID[8];  /*"管理芯ESAM ID"*/
#define GLXCHGDELAY (5)   /*"管理芯插拔事件延时"*/
#if (MNT_VERSION == STATE_GRID)
#define GLX_CHG_FILENAME  E_MU_PLUGIN_RCRD
#define GLX_CHG_FILE_S       E_EVTLST_MU_PLUGIN_S
#define GLX_CHG_FILE_E       E_EVTLST_MU_PLUGIN_E
#else
#define GLX_CHG_FILENAME  E_FILE_NOLEG_REPALCE_EVNT
#define GLX_CHG_FILE_S       E_EVTLST_NOLEG_REPALCE_S
#define GLX_CHG_FILE_E       E_EVTLST_NOLEG_REPALCE_E
#endif
#endif

#ifdef GLX_COMM_CHK
static INT8U mGlxCommFlag;   /*"管理芯异常通信标志"*/
static INT8U mGlxCommCnt;     /*"管理芯异常通信计数器"*/
#define MAX_GLXCOMM_DELAY (60)   /*"管理芯异常通信计数器最大延时"*/
#endif

#ifdef ACCRCY_CHK
static INT8U mAccrcyRfrshCnt;    /*"误差自监测更新时间  单位  分"*/
INT32S mPwrAmpErr;
#endif

void OutFac(void);

static void RstChk(void);
static void FacStaChk(void);
static void FacRemainTimeDis(void);
#ifdef BAT_VOLT_CHK
static void BatErrChk(void);
static INT8U BatVoltLowChk(INT8U id);
#endif
#ifdef OPEN_COVER_CHK
static void OpenCoverInit(void);
static void OpenCoverMain(void);
#ifdef PD_OPEN_COVER_CHK
static void OpenCoverPDMain(void);
#endif
#endif

#ifdef RELAY_ERR_CHK
typedef union       /*"负荷开关误动作检测及上报状态定义"*/
{
    INT8U byte;          
    struct 
    {
        INT8U rlyOff: 1;   		/*"跳闸，继电器动作时置1，检测到成功跳闸后清零"*/
        INT8U rlyOn: 1;    		/*"合闸，继电器动作时置1，检测到成功合闸后清零"*/
        INT8U rlyOffRpt: 1;  	/*"继电器成功跳闸置此位，上报后清零"*/
        INT8U rlyOnRpt: 1;    	/*"继电器成功合闸置此位，上报后清零"*/
        INT8U saved: 3;    		/*"保留"*/
		INT8U rlyErrEvent: 1;   /*"负荷开关误动作事件记录"*/
    }bits;
}UN_RLY_FBACK;
static void RelayChkInit(void);
static void RlyErrChkTsk1s(void);
static void ChkRelayStatus(UN_BYTE_DEF RelayChk,UN_RLY_FBACK RelaySucc,INT8U relayTpye);
#endif
#ifdef PWR_ERR_CHK
static void PwrErrInit(void);
static void PDPwrErrInit(void);
static void PDPwrErr(INT8U Type);
static void PDPwrErrTsk1s(void);
#endif
static void VoltLevelChk(void);
static void VoltLevelInit(void);
static void CurrLevelChk(void);
#ifdef THREE_PHASE
static void MagnetEventSecTask(void);
static void MagnetEventInit(void);
#endif
#ifdef MU_ERR_CHK
static void MuErrSecTask(void);
static void MuErrInit(void);
#endif
static void ClkErrSecTask(void);
static void ClkErrInit(void);
static void OscErrSecTask(void);
static void OscErrInit(void);
#ifdef THREE_PHASE
NO_INIT INT64U  pwrRstMode;  /*"电源复位模式"*/
#endif
#ifdef TMNL_TMP_CHK
static void TmnlTmpInit(void);
static void TmnlTmpSecTask(void);
#endif
#ifdef GLX_CHG_CHK
static void GlxChgChkInit(void);
static void GlxChgChkSecTask(void);
#endif
#ifdef GLX_COMM_CHK
static void GlxCommChkInit(void);
static void GlxCommChkSecTask(void);
#endif
#ifdef ACCRCY_CHK
static void AccrcyChkInit(void);
static void AccrcyChkMinTask(void);
#endif
/*"*****************************************************************************"*/
/*"  Function:       StatusInit"*/
/*"  Description:    状态检查初始化主接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void StatusInit(INT8U mode)
{
#ifdef OPEN_COVER_CHK
    FS_ReadFile(E_FILE_MNT, OPEN_COVER_FLG_SHIFT, (INT8U *)&mCoverStatus[0], COVER_NUM);
#endif
    if(MCU_RUNMODE == mode)
    {
        FS_ReadFile(E_FILE_MNT, FAC_REMAIN_TIME, (INT8U *) &mFacAbleTime, 2);
        if(mFacAbleTime > 0)
        {
            Push(E_FAC, TRUE);
        }
#ifdef BAT_VOLT_CHK
        LIB_MemSet(5, (INT8U *) &BatErrNum[0], BAT_NUM);/*"上电确定电池不欠压也需监测5s"*/
#endif
        #ifdef TMNL_TMP_CHK
        TmnlTmpInit();
        #endif
        #ifdef GLX_CHG_CHK
        GlxChgChkInit();
        #endif
        #ifdef GLX_COMM_CHK
        GlxCommChkInit();
        #endif
        GetSingle(E_SYS_TIME, (INT8U *)&mMntNowTime);
    }
#ifdef PWR_ERR_CHK
    if(MCU_RUNMODE == mode)
    {
        
    }
    else
    {
        PDPwrErrInit();
    }
#endif
    VoltLevelInit();
}
/*"*****************************************************************************"*/
/*"  Function:       StatusTaskMnt"*/
/*"  Description:    状态检查瞬时任务接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void StatusTaskMnt(void)
{
    ST_7TIME_FORM nowTime;

    GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
    if((nowTime.year != mMntNowTime.year) ||(nowTime.month!= mMntNowTime.month) 
        ||(nowTime.day!= mMntNowTime.day))
    {
        OC_MultiTermnSw(E_MULTI_REACTPULSE);
    }
    mMntNowTime=nowTime;

    FacStaChk();
#ifdef OPEN_COVER_CHK
    OpenCoverMain();
#endif
    if(FALSE == EEP_GetErrSta())
    {
        Push(E_EEP_ERR, TRUE);
    }
    else
    {
        Push(E_EEP_ERR, FALSE);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       StatusTask1s"*/
/*"  Description:    状态检查秒任务接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void StatusTask1s(void)
{
#ifdef THREE_PHASE
    MNT_RefreshVC();
#endif 
   OscErrSecTask();  
#ifdef BAT_VOLT_CHK
    BatErrChk();
#endif
#ifdef RELAY_ERR_CHK
    RlyErrChkTsk1s();
#endif
#if(MNT_VERSION == STATE_GRID)
 //if(ChkStatus(E_MU_DATA_READY) == TRUE)
#endif
 {
#ifdef VOLT_STATUS_CHK
    VoltLevelChk();
#endif
#ifdef CURR_STATUS_CHK
    CurrLevelChk();
#endif
 }
#ifdef THREE_PHASE
    MagnetEventSecTask();
#endif 
#ifdef MU_ERR_CHK
    MuErrSecTask();
#endif
    ClkErrSecTask();
#ifdef TMNL_TMP_CHK
    TmnlTmpSecTask();
#endif
#ifdef GLX_CHG_CHK
    GlxChgChkSecTask();
#endif
#ifdef GLX_COMM_CHK
    GlxCommChkSecTask();
#endif
}

void StatusTask1sPD(void)
{
#ifdef PWR_ERR_CHK
    PDPwrErrTsk1s();
#endif
#ifdef PD_OPEN_COVER_CHK
    OpenCoverPDMain();
#endif
}
/*"*****************************************************************************"*/
/*"  Function:       StatusTask1Min"*/
/*"  Description:    状态检查分钟任务接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void StatusTask1Min(void)
{
    FacRemainTimeDis();
    #ifdef ACCRCY_CHK
    AccrcyChkMinTask();
    #endif
}
/*"*****************************************************************************"*/
/*"  Function:       RstChk"*/
/*"  Description:    电表复位检测"*/
/*"  Calls:          "*/
/*"  Called By:      FirstPwrOnChk"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RstChk(void)
{
    INT32U  RstChkFlg;
    FS_ReadFile(E_FILE_MNT, RST_CHK_FLG_SHIFT, (INT8U *) &RstChkFlg, 4);

    if(0x5a5a5a5a != RstChkFlg)
    {
        Push(E_MTR_RST, TRUE);
    }
    else
    {
        Push(E_MTR_RST, FALSE);
    }

    mucRstInfo=MCU_GetResetSrc();
}
/*"*****************************************************************************"*/
/*"  Function:       FirstPwrOnChk"*/
/*"  Description:    电表初次上电状态检查，条件工厂状态跳线短接、开盖、内卡标识未初始化"*/
/*"  Calls:          各功能模块第一次上电初始化处理方法"*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void FirstPwrOnChk(void)
{
    INT8U valTmp;
    INT32U InitFlg;
    INT32U InitFlgTmp;

    /*"先判断是否为新内卡，是否满足初始化条件"*/
    valTmp = FS_ReadFile(E_FILE_MNT, NEW_EEP_CHK_FLG_SHIFT, (INT8U *)&InitFlg, 4);
    FS_ReadFile(E_FILE_MNT, NEW_EEP_CHK_FLG_SHIFT, (INT8U *)&InitFlgTmp, 4);
    if((NEW_EEP_FLG != InitFlg) && (FALSE != valTmp) && (InitFlgTmp == InitFlg))
    {
		Push(E_FIRST_PWR_ON, TRUE);
	 if(SID_TRUE == SID_PollRead(SID_FAC))/*"处于短接状态"*/
        {
            Push(E_FAC, TRUE);
            mFacAbleTime = FAC_TIMER;
            FS_WriteFile(E_FILE_MNT, FAC_REMAIN_TIME, (INT8U *) &mFacAbleTime, 2);
        }
        else
        {
        }
    }
    else
    {
        Push(E_FIRST_PWR_ON, FALSE);
    }

    if(mFacAbleTime > 0)
    {
        #if(MNT_VERSION == SOUTH_GRID)
        //DIS_FlashStauSeg(STA_ING);
        #endif
        Push(E_FAC, TRUE);
    }
    else
    {
    }

    RstChk();
}
/*"*****************************************************************************"*/
/*"  Function:       FacStaChk"*/
/*"  Description:    工厂状态检查"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void FacStaChk(void)
{
    if(FALSE == ChkStatus(E_FAC))
    {
        if (SID_TRUE == SID_Read(SID_FAC))
        {
            Push(E_FAC, TRUE);
            mFacAbleTime = FAC_TIMER;
            FS_WriteFile(E_FILE_MNT, FAC_REMAIN_TIME, (INT8U *) &mFacAbleTime, 2);
            #if(MNT_VERSION == SOUTH_GRID)
            //DIS_FlashStauSeg(STA_ING);
            #endif
        }
        else
        {
        }
    }
    else
    {
        #if(MNT_VERSION == SOUTH_GRID)
        //DIS_FlashStauSeg(STA_ING);
        #endif
    }
}
/*"*****************************************************************************"*/
/*"  Function:       ResetFacRemainTime"*/
/*"  Description:    重置工厂状态剩余时间"*/
/*"  Calls:          "*/
/*"  Called By:      重置方法"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void ResetFacRemainTime(INT8U *Indata)
{
    if(TRUE == ChkStatus(E_FAC))
    {
		if(*Indata != 0)
		{
			mFacAbleTime = 30;
		}
		else
		{
			mFacAbleTime = FAC_TIMER;
		}
        FS_WriteFile(E_FILE_MNT, FAC_REMAIN_TIME, (INT8U *) &mFacAbleTime, 2);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       FacRemainTimeDis"*/
/*"  Description:    工厂状态剩余时间扣减"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void FacRemainTimeDis(void)
{
    if(mFacAbleTime > 0) /*"厂内状态时间扣减"*/
    {
        Push(E_FAC, TRUE);
        mFacAbleTime -= 1;
        if(0 == mFacAbleTime)
        {
            OutFac();
        }
        else if(0 == (mFacAbleTime & 0x000F))/*"剩余时长16分钟备份内卡"*/
        {
            FS_WriteFile(E_FILE_MNT, FAC_REMAIN_TIME, (INT8U *)&mFacAbleTime, 2);
        }
    }
    else
    {
        Push(E_FAC, FALSE);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       OutFac"*/
/*"  Description:    退出厂内状态"*/
/*"  Calls:          "*/
/*"  Called By:      FacRemainTimeDis,方法"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void OutFac(void)
{
    Push(E_FAC, FALSE);
    mFacAbleTime = 0;
    FS_WriteFile(E_FILE_MNT, FAC_REMAIN_TIME, (INT8U *) &mFacAbleTime, 2);
    #if(MNT_VERSION == SOUTH_GRID)
    //DIS_ClrFlashStauSeg(STA_ING);
    DIS_ClrFlashStauSeg(STA_KEY);
    #endif
    SID_Init(MCU_RUNMODE, SID_FAC);
}

#ifdef BAT_VOLT_CHK
/*"*****************************************************************************"*/
/*"  Function:       OutFac"*/
/*"  Description:    电池欠压检测，连续60次为欠压,在厂内状态时只检测一次"*/
/*"  Calls:          "*/
/*"  Called By:      StatusTask1s,StatusInit"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void BatErrChk(void)
{
    INT8U i;
    INT8U result;
#ifdef THREE_PHASE
    INT8U BatDisId[BAT_NUM] = {STA_BAT , STA_BAT2};
#else
    INT8U BatDisId[BAT_NUM] = {STA_BAT,STA_BAT};
#endif
    INT8U BatStatusId[BAT_NUM] = {E_CLK_BAT_LOSS,E_DIS_BAT_LOSS};/*"单相表时钟电池欠压时，停电抄表电压也要置位"*/

    for(i = 0; i < BAT_NUM; i++)
    {
        result=BatVoltLowChk(i);
	 if(TRUE==result)
        {
            DIS_SetStauSeg(BatDisId[i]);/*"显示状态为实时"*/
            if((BatErrNum[i] >= 60) || (TRUE == ChkStatus(E_FAC)))/*"事件上报状态为60s"*/
            {
                Push((EN_MTR_STATUS)BatStatusId[i], TRUE);
            }
            else
            {
                BatErrNum[i] += 1;
            }
        }
        else if(FALSE==result)
        {
            //DIS_ClrStauSeg(BatDisId[i]);/*"显示状态为实时"*/
            if((BatErrNum[i] > 0) && (FALSE == ChkStatus(E_FAC)))
            {
                BatErrNum[i] -= 1;
            }
            else
            {
                Push((EN_MTR_STATUS)BatStatusId[i], FALSE);
            }
        }
	else
	{
	}
    }
}
/*"*****************************************************************************"*/
/*"  Function:       BatVoltLowChk"*/
/*"  Description:    电池欠压瞬时状态判断"*/
/*"  Calls:          "*/
/*"  Called By:      BatErrChk"*/
/*"  Input:          id 电池编号"*/
/*"  Output:         "*/
/*"  Return:         TRUE 电池欠压    FALSE  电池正常"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U BatVoltLowChk(INT8U id)
{
#ifdef  BAT_VOLT_CAL
#ifdef THREE_PHASE
    INT16U BatVoltThrl[BAT_NUM]={MntPara.ClkBatVoltThrL, MntPara.DisBatVoltThrL};
    INT8U AdcId[BAT_NUM] = {AC_ADCID_RTC_BAT , AC_ADCID_BAT};
#else
    INT16U BatVoltThrl[BAT_NUM]={MntPara.DisBatVoltThrL, MntPara.DisBatVoltThrL};
    INT8U AdcId[BAT_NUM] = {AC_ADCID_BAT,  AC_ADCID_BAT};
#endif
    INT16U d;
    
    d = Cal_VolVal(AdcId[id]);
    if(d != 0xFFFF)
    {
        if((d >= 4500) || (d <= 100))
        {
          d=0;
        }
        BatVolt[id] = d;
        if(BatVolt[id] < BatVoltThrl[id])
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    return BAT_UNKOWN;
#else

#endif
}
#endif

/*"------------------------------开盖事件记录--------------------------------"*/
#ifdef OPEN_COVER_CHK
/*"*****************************************************************************"*/
/*"  Function:       OpenCoverInit"*/
/*"  Description:    开盖检测初始化"*/
/*"  Calls:          "*/
/*"  Called By:      StatusInit, 初始化方法"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void OpenCoverInit(void)
{
#ifdef PD_OPEN_COVER_CHK
    CRC_DATA_TP crcTmp;
    INT32U tmptimes;
    INT8U evtEndFlg;
    INT8U i;
#ifdef THREE_PHASE
    INT32U evtFileId[COVER_NUM]={E_MTR_COVER_OPEN_EVNT_RCRD , E_TAIL_COVER_OPEN_EVNT_RCRD};
    INT32U evtFileStartId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_S , E_EVTLST_TAIL_COVER_OPEN_S};
    INT32U evtFileEndId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_E , E_EVTLST_TAIL_COVER_OPEN_E};
    EN_MTR_STATUS StaId[COVER_NUM]={E_OPEN_COVER, E_OPEN_TAIL};
#else
    INT32U evtFileId[COVER_NUM]={E_MTR_COVER_OPEN_EVNT_RCRD};
    INT32U evtFileStartId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_S};
    INT32U evtFileEndId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_E};
    EN_MTR_STATUS StaId[COVER_NUM]={E_OPEN_COVER};
#endif


    for(i = 0; i < COVER_NUM; i++)
    {
        crcTmp = CRC16((INT8U *)&stPdOpenCvrRcd[i], FIND(ST_OPENCVR_PD, crc));
        if((stPdOpenCvrRcd[i].crc != crcTmp) || (stPdOpenCvrRcd[i].keyWord != 0xa5a5))
        {
        }
        else
        {
            evtEndFlg = 0;
            if(stPdOpenCvrRcd[i].times > 0)/*"有开盖"*/
            {
                LIB_MemCpy((INT8U *)&stPdOpenCvrRcd[i].startTime, &DCDataBuf[0], sizeof(ST_6TIME_FORM));
                SaveRcrd(evtFileId[i], evtFileStartId[i]);

                if(stPdOpenCvrRcd[i].lastState == 0)/*"完整开合盖"*/
                {
                    evtEndFlg = 1;
                }
                else
                {
                }
            }
            else /*"只有合盖"*/
            {
                GetSingle(E_RCRD_CNT_BLK + (evtFileId[i] & 0x00ff), (INT8U *)&tmptimes);
                if(tmptimes > 0)
                {
                    evtEndFlg = 1;
                }
            }
            if(1 == evtEndFlg)
            {
                /*"记录合盖"*/
                LIB_MemCpy((INT8U *)&stPdOpenCvrRcd[i].endTime, &DCDataBuf[6], sizeof(ST_6TIME_FORM));
                SaveRcrd(evtFileId[i], evtFileEndId[i]);
            }
            else
            {
            }
            mCoverStatus[i] = stPdOpenCvrRcd[i].lastState;

            FS_WriteFile(E_FILE_MNT, OPEN_COVER_FLG_SHIFT + i, (INT8U *)&mCoverStatus[i], 1);
        }
        stPdOpenCvrRcd[i].keyWord=0;

	 Push(StaId[i], mCoverStatus[i]);
    }
#endif
}
/*"*****************************************************************************"*/
/*"  Function:       OpenCoverMain"*/
/*"  Description:    上电检测开合盖主接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      OpenCoverInit, StatusTaskMnt"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void OpenCoverMain(void)
{
    INT8U OpenStatus;
    INT16U OpenTypeTmp;
    INT8U i;
#ifdef THREE_PHASE
    INT8U  CoverId[COVER_NUM] = {SID_MTRCOVER , SID_RAILCOVER};
    INT32U evtFileId[COVER_NUM]={E_MTR_COVER_OPEN_EVNT_RCRD , E_TAIL_COVER_OPEN_EVNT_RCRD};
    INT32U evtFileStartId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_S , E_EVTLST_TAIL_COVER_OPEN_S};
    INT32U evtFileEndId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_E , E_EVTLST_TAIL_COVER_OPEN_E};
    EN_MTR_STATUS StaId[COVER_NUM]={E_OPEN_COVER, E_OPEN_TAIL};
#else
    INT8U  CoverId[COVER_NUM] = {SID_MTRCOVER};
    INT32U evtFileId[COVER_NUM]={E_MTR_COVER_OPEN_EVNT_RCRD};
    INT32U evtFileStartId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_S};
    INT32U evtFileEndId[COVER_NUM]={E_EVTLST_MTR_COVER_OPEN_E};
    EN_MTR_STATUS StaId[COVER_NUM]={E_OPEN_COVER};
#endif

    for(i = 0; i < COVER_NUM; i++)
    {
        OpenStatus = SID_Read(CoverId[i]);
	 
        OpenTypeTmp = 0xffff;
        if(((OpenStatus == SID_TRUE) || (OpenStatus == SID_FALSE)) && (OpenStatus != mCoverStatus[i]))
        {
            if(SID_FALSE == mCoverStatus[i])/*"当前状态为合盖"*/
            {
                OpenTypeTmp = evtFileStartId[i]; /*"记录开盖"*/
                GetSingle(E_SYS_TIME, &DCDataBuf[0]);
            }
            else /*"之前状态为开盖"*/
            {
                OpenTypeTmp = evtFileEndId[i]; /*"记录合盖"*/
                GetSingle(E_SYS_TIME, &DCDataBuf[6]);
            }
        }

        if(0xffff != OpenTypeTmp)
        {
            SaveRcrd(evtFileId[i], OpenTypeTmp);
            mCoverStatus[i] = OpenStatus;
            FS_WriteFile(E_FILE_MNT, OPEN_COVER_FLG_SHIFT + i, (INT8U *) &mCoverStatus[i], 1);
            Push(StaId[i], mCoverStatus[i]);
        }
    }
}
#ifdef PD_OPEN_COVER_CHK
/*"*****************************************************************************"*/
/*"  Function:       OpenCoverPDMain"*/
/*"  Description:    低功耗检测开合盖主接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      OpenCoverInit, StatusTaskMnt"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#ifdef THREE_PHASE
static INT8U pdCoverStat[COVER_NUM][2]={{0xFF,0xFF},{0xFF,0xFF}};
#else
static INT8U pdCoverStat[COVER_NUM][2]={{0xFF,0xFF}};
#endif
static void OpenCoverPDMain(void)
{
    INT8U i;
#ifdef THREE_PHASE
    INT8U  CoverId[COVER_NUM] = {SID_MTRCOVER , SID_RAILCOVER};
#else
    INT8U  CoverId[COVER_NUM] = {SID_MTRCOVER};
#endif

    /*"开盖检测"*/
    for(i = 0; i < COVER_NUM; i++)
    {
        if((stPdOpenCvrRcd[i].keyWord == 0xA5A5) && (stPdOpenCvrRcd[i].lastState == SID_FALSE) 
	 && (stPdOpenCvrRcd[i].endTime.month > 0))
        {
           continue;
        }

	 pdCoverStat[i][1]=pdCoverStat[i][0];
	 pdCoverStat[i][0] = SID_PollRead(CoverId[i]);
        if(pdCoverStat[i][1] == pdCoverStat[i][0])
        {
            if((stPdOpenCvrRcd[i].keyWord != 0xA5A5))/*"ram未初始化"*/
            {
                LIB_MemSet(0, (INT8U *)&stPdOpenCvrRcd[i], sizeof(ST_OPENCVR_PD));
                LIB_MemCpy((INT8U *)&mCoverStatus[i], (INT8U *)&stPdOpenCvrRcd[i].lastState, 1);
                stPdOpenCvrRcd[i].keyWord = 0xA5A5;
            }
            else
            {
            }
            
            if(pdCoverStat[i][0] != stPdOpenCvrRcd[i].lastState)
            {
                if(pdCoverStat[i][0] == SID_TRUE) /*"记录开盖"*/
                {
                    stPdOpenCvrRcd[i].times += 1;
                    GetSingle(E_SYS_TIME, (INT8U *)&stPdOpenCvrRcd[i].startTime);
                }
                else if(pdCoverStat[i][0] == SID_FALSE) /*"记录合盖"*/
                {
                    GetSingle(E_SYS_TIME, (INT8U *)&stPdOpenCvrRcd[i].endTime);
                }
                else
                {
                }
                stPdOpenCvrRcd[i].lastState = pdCoverStat[i][0];
                stPdOpenCvrRcd[i].crc = CRC16((INT8U *)&stPdOpenCvrRcd[i], FIND(ST_OPENCVR_PD,crc));
            }           
        }
        else
        {
        }
    }
}
#endif
#endif

INT8U RelayStatusNow;/*"负荷开关当前状态 0 通   1断"*/
#ifdef RELAY_ERR_CHK
/*"--------------负荷开关误动作事件记录-------------------------"*/
static INT8U mRelayActionNow;/*"继电器当前理论状态"*/
static INT8U mRelayErrTimes;/*"负荷开关误动作判断延时累计次数"*/
static INT8U mRelayErrFlg; /*"负荷开关误动作发生标志，发生时置位，结束时清零"*/
static ENGY_TYPE_U  mRealyEnergyA;
static ENGY_TYPE_U  mRealyEnergyN;
#ifdef RELAY_ERR_TEST
static INT8U relayCtrl;
static INT8U relayStatus;
static INT8U PwrStatus;
#endif

/*"*****************************************************************************"*/
/*"  Function:       RelayChkInit"*/
/*"  Description:    继电器状态检测上电初始化"*/
/*"  Calls:          "*/
/*"  Called By:      StatusInit"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
/*""*/
static void RelayChkInit(void)
{
    mRelayErrTimes=0;
    FS_ReadFile(E_FILE_MNT, RELAY_ACTION_NOW, (INT8U *)&mRelayActionNow, 1);
    FS_ReadFile(E_FILE_MNT, RELAY_ERR_FLG, (INT8U *)&mRelayErrFlg, 1);
    if (mRelayErrFlg == TRUE)/*"上电(非复位）时检测上一次负荷开关误动作还未结束，结束上一次记录"*/
    {
        if(ChkStatus(E_MTR_RST) == FALSE)
        {
            GetSingle(E_PD_TIME, (INT8U *)&DCDataBuf[6]);
        }
        else
        {
            GetSingle(E_SYS_TIME, (INT8U *)&DCDataBuf[6]);
        }
        SaveRcrd(E_RELAY_ERR_EVNT_RCRD, E_EVTLST_RELAY_ERR_E);
        mRelayErrFlg = FALSE;
        FS_WriteFile(E_FILE_MNT, RELAY_ERR_FLG, (INT8U *)&mRelayErrFlg, 1);
    }
    GetSingle(E_PA_ENGY,(INT8U *)&mRealyEnergyA);
    GetSingle(E_NA_ENGY,(INT8U *)&mRealyEnergyN);
#ifdef RELAY_ERR_TEST
    relayCtrl = 1;
    relayStatus = 1;
    PwrStatus = 1;
#endif
}
/*"*****************************************************************************"*/
/*"  Function:       RlyErrChkTsk1s"*/
/*"  Description:    负荷开关误动作判断"*/
/*"***************************负荷开关误动作判断真值表***************************"*/
/*"**继电器理论值b0   当前功率状态b1  负荷开关反馈状态b2      继电器误动作状态"*/
/*"**     0                0               0                    成功                  "*/
/*"**     0                0               1                    失败                  "*/
/*"**     0                1               0                    失败                  "*/
/*"**     0                1               1                    失败                  "*/
/*"**     1                0               0                    失败                  "*/
/*"**     1                0               1                    成功                  "*/
/*"**     1                1               0                    成功                  "*/
/*"**     1                1               1                    成功                  "*/
/*"  Calls:          "*/
/*"  Called By:      StatusTask1s"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define  RELAY_ERR_CHK_TIME  4 /*"负荷开关误动作判断延时5秒"*/
static void RlyErrChkTsk1s(void)
{
    INT32S rPwr;
    INT8U RelayType,OutRlyChk;/*"继电器类型，内置or外置"*/
    INT8U ChkRelayErr;/*"是否需求检测继电器异常   TRUE  需要  FALSE  不需要"*/
    UN_BYTE_DEF RelayChk;/*"继电器误动作检测信息  b0  继电器理论状态 1合闸    0跳闸"*/
                         /*"                   b1   当前功率              1 大于启动功率     0小于启动功率"*/
                         /*"                   b2  继电器反馈状态  1 通       0 断"*/
	UN_RLY_FBACK RlySuccRpt;
	INT8U evtRptMode[4];	 
	ENGY_TYPE_U  tEnergyA;
       ENGY_TYPE_U  tEnergyN;
	INT16U tEnergyMax;

	tEnergyA=0;
	tEnergyN=0;
	tEnergyMax=0;
						 
	#ifdef THREE_PHASE
	INT8U mtrType;
	GetSingle(E_METER_MODE,&mtrType);
	if((mtrType& METERTPYE) == SMARTMETER)/*"智能表不需要进行负荷开关误动作检测"*/
	{
		return;
	}
	#endif
	
	GetSingle(E_RLYSUCC_RPT,&RlySuccRpt.byte);

    if(ChkStatus(E_MU_IC_ERR) == TRUE)/*"如果计量芯片读电压异常时，则认为掉电不判继电器状态，避免掉电后出现继电器误动作事件"*/
    {
        return;
    }

    ChkRelayErr = TRUE;

    RelayChk.byte = 0;

    if(RELAY_ACT_CLOSE == OC_GetRelayCtrlStatus())/*"获取继电器理论状态"*/
    {
        RelayChk.bit.b0 = 1;
    }
    else
    {
        RelayChk.bit.b0 = 0;
    }

    if(RelayChk.bit.b0 != mRelayActionNow)/*"继电器控制理论状态发生改变，需重新判断负荷开关误动作，先置负荷开关误动作未发生"*/
    {
        if(mRelayErrFlg == TRUE)/*"上一次负荷开关误动作还未结束，继电器控制状态发生改变，结束上一次记录"*/
        {
			GetSingle(E_SYS_TIME, (INT8U *)&DCDataBuf[6]);
			SaveRcrd(E_RELAY_ERR_EVNT_RCRD, E_EVTLST_RELAY_ERR_E);
			mRelayErrFlg = FALSE;
			FS_WriteFile(E_FILE_MNT, RELAY_ERR_FLG, (INT8U *)&mRelayErrFlg, 1);
        }
        Push(E_RELAY_ERR, FALSE);
        mRelayActionNow = RelayChk.bit.b0;
        FS_WriteFile(E_FILE_MNT, RELAY_ACTION_NOW, (INT8U *)&mRelayActionNow, 1);
    }
    /*"获取当前功率状态"*/
    GetSingle(E_ACTIVE_PWR, (INT8U *)&rPwr);
#ifdef THREE_PHASE
    if(LIB_ABS(rPwr) > 0)/*"功率大于启动功率"*///todo
#else
    if(LIB_ABS(rPwr) > 44)/*"功率大于启动功率"*///todo
#endif
    {
        RelayChk.bit.b1 = 1;
    }
    GetSingle(E_RELAY_TYPE, (INT8U *)&RelayType);
    RelayType = (RelayType&RLYTYPE);
#ifdef THREE_PHASE
    if(RLY_INSIDE == RelayType)
    {
        RelayChk.bit.b2 = SID_PollRead(SID_REALY);  /*"获取继电器反馈状态"*/
    }
    else
    {
        RelayChk.bit.b2 = 0;  /*"三相外置继电器负荷开关误动作不考虑反馈信号"*/
    }
#else
    RelayChk.bit.b2 = SID_PollRead(SID_REALY);  /*"获取继电器反馈状态"*/
#endif
    
    ChkRelayStatus(RelayChk,RlySuccRpt,RelayType);
    switch(RelayChk.byte & 0x07)
    {
    case 0x01:/*"负荷开关误动作"*/
    case 0x02:
    case 0x04:
    case 0x06:
        if((RelayChk.bit.b0 == 1)&&((RLY_OUTSIDE == RelayType)||(RLY_PULSEOUTSIDE == RelayType)))/*"当前为合闸状态,并且为外置继电器"*/
        {
			if(RlySuccRpt.bits.rlyOn == 1)/*"没检测成功合闸前，需重新检测"*/
        {
				OutRlyChk = TRUE;/*"可以重新检测，保证E_RELAY_ERR正常置位"*/
			}
			else
            {
				Push(E_RELAY_ERR, FALSE);/*"不需要检测，已经成功合闸"*/
                ChkRelayErr = FALSE;
				OutRlyChk = FALSE;
            }
        }
        else
        {
			OutRlyChk = FALSE;
		}

        if((ChkStatus(E_RELAY_ERR) == FALSE) && (TRUE == ChkRelayErr) && ((mRelayErrFlg == FALSE)||(OutRlyChk == TRUE)))/*"误动作未开始，需判断开始"*/
        {
            mRelayErrTimes++;
            if(mRelayErrTimes > RELAY_ERR_CHK_TIME)
            {
                Push(E_RELAY_ERR, TRUE);
				if(RlySuccRpt.bits.rlyErrEvent == 0)/*"动作执行一次，只记录一次异常事件记录"*/
				{
					if(OutRlyChk == TRUE)/*"外置继电器合闸状态下不记录负荷开关误动作"*/
					{
						;
					}
					else
					{
					GetSingle(E_SYS_TIME, (INT8U *)&DCDataBuf[0]);
					SaveRcrd(E_RELAY_ERR_EVNT_RCRD, E_EVTLST_RELAY_ERR_S);
					mRelayErrFlg = TRUE;
					FS_WriteFile(E_FILE_MNT, RELAY_ERR_FLG, (INT8U *)&mRelayErrFlg, 1);
					}

					RlySuccRpt.bits.rlyErrEvent = 1;
					Action(E_RLYSUCC_UPDATE,(INT8U *)&RlySuccRpt,NULL);
				}
            }
        }
        else
        {
            mRelayErrTimes = 0;
        }
        break;
    case 0x00:/*"负荷开动作成功"*/
    case 0x03:
    case 0x05:
    case 0x07:
        if(ChkStatus(E_RELAY_ERR) == TRUE)/*"误动作已开始，需判断结束"*/
        {
            mRelayErrTimes++;
            if(mRelayErrTimes > RELAY_ERR_CHK_TIME)
            {
                Push(E_RELAY_ERR, FALSE);
                if(mRelayErrFlg == TRUE)
                {
                    GetSingle(E_SYS_TIME, (INT8U *) &DCDataBuf[6]);
                    SaveRcrd(E_RELAY_ERR_EVNT_RCRD, E_EVTLST_RELAY_ERR_E);
                    mRelayErrFlg = FALSE;
                    FS_WriteFile(E_FILE_MNT, RELAY_ERR_FLG, (INT8U *) &mRelayErrFlg, 1);
                }
            }
        }
        else
        {
            mRelayErrTimes = 0;
        }

		if((RelayChk.byte & 0x07) == 0)/*"跳闸成功"*/
		{
			if(RlySuccRpt.bits.rlyOff == 1)/*"发生过跳闸"*/
			{
				GetSingle(E_FOLLOW_REPPORT_MODE,(INT8U *)&evtRptMode[0]);/*"主动上报模式字"*/
				RlySuccRpt.bits.rlyOff = 0;/*"检测到成功跳闸后清零"*/
				if(evtRptMode[1] & 0x40)/*"跳闸成功模式打开,才置位"*/
				{
					RlySuccRpt.bits.rlyOffRpt = 1;
				}
				else
				{
					RlySuccRpt.bits.rlyOffRpt = 0;
				}
				Action(E_RLYSUCC_UPDATE,(INT8U *)&RlySuccRpt,NULL);
			}
		}
		else/*"合闸成功"*/
		{
			if((RlySuccRpt.bits.rlyOn == 1)&&(RelayChk.bit.b0 == 1))/*"发生过合闸，并且此时为合闸状态"*/
			{
				GetSingle(E_FOLLOW_REPPORT_MODE,(INT8U *)&evtRptMode[0]);/*"主动上报模式字"*/
				RlySuccRpt.bits.rlyOn = 0; /*"检测到成功合闸后清零"*/
				if(evtRptMode[1] & 0x80)/*"合闸成功模式打开,才置位"*/
				{
					RlySuccRpt.bits.rlyOnRpt = 1;
				}
				else
				{
					RlySuccRpt.bits.rlyOnRpt = 0;
				}
				Action(E_RLYSUCC_UPDATE,(INT8U *)&RlySuccRpt,NULL);
			}
		}
        break;
    default:
        break;
    }

    if(RLY_PULSEOUTSIDE == RelayType)  /*"外置脉冲继电器"*/
    {
        GetSingle(E_PA_ENGY,(INT8U *)&tEnergyA);
        GetSingle(E_NA_ENGY,(INT8U *)&tEnergyN);
	 GetSingle(E_RELAY_ACT_ENGY,(INT8U *)&tEnergyMax);
	 if(RELAY_ACT_TRIP == OC_GetRelayCtrlStatus()) /*"跳闸"*/
        {
            if((tEnergyA+tEnergyN)<(mRealyEnergyA+mRealyEnergyN))
            {
                mRealyEnergyA=tEnergyA;
	         mRealyEnergyN=tEnergyN;
            }
	     else
	     {
	        if((tEnergyA+tEnergyN) >= (mRealyEnergyA+mRealyEnergyN+tEnergyMax*10))
	        {
	           mRealyEnergyA=tEnergyA;
	           mRealyEnergyN=tEnergyN;
		    OC_RelayActReDo();
	        }
	     }
        }
        else /*"合闸"*/
        {
            mRealyEnergyA=tEnergyA;
	     mRealyEnergyN=tEnergyN;
        }
    }
}
/*"*****************************************************************************"*/
/*"  Function:       ChkRelayStatus"*/
/*"  Description:    继电器真实状态"*/
/*"  Calls:          "*/
/*"  Called By:      RlyErrChkTsk1s"*/
/*"  Input:          RelayChk 继电器状态检测信息"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ChkRelayStatus(UN_BYTE_DEF RelayChk,UN_RLY_FBACK RelaySucc,INT8U relayTpye)
{
    if ((RelayChk.byte & 0x06) != 0) /*"实际为通(有功率或检测到反馈通)"*/
    {
        Push(E_RELAY_STATUS, FALSE);
        RelayStatusNow = 0;
    }
    else
    {
		if((RelayChk.bit.b0 == 1)&&((RLY_OUTSIDE == relayTpye)||(RLY_PULSEOUTSIDE == relayTpye))&&(RelaySucc.bits.rlyOn == 0))/*"合闸状态，外置继电器并且已经成功合闸"*/
		{
			Push(E_RELAY_STATUS, FALSE);/*"状态置位通"*/
			RelayStatusNow = 0;
		}
		else
		{
			Push(E_RELAY_STATUS, TRUE);
			RelayStatusNow = 1;
		}
    }
}
#endif

#ifdef PWR_ERR_CHK
/*"低功耗下电源异常事件结构"*/
typedef struct
{
    INT16U KeyWord; /*"低功耗下的特征字，0xA5A5"*/
    ST_7TIME_FORM BeginTime; /*"开始时间"*/
    ST_7TIME_FORM EndTime; /*"结束时间"*/
    CRC_DATA_TP crc;
}ST_PDPOWERERREVENT;

#ifdef SINGLE_PHASE
NO_INIT static INT8U mVoltageLowFlg; /*"电压低标识，电压低时直接掉电不判电源异常"*/
static INT8U mClsPowerErrNum;     /*"电源异常时间计数"*/
static INT8U mPwrErrFlg;  /*"电源异常标志"*/
#endif

NO_INIT static ST_PDPOWERERREVENT mstPDPowerErrEvent; /*"低功耗下电源异常记录数据"*/
/*"*****************************************************************************"*/
/*"  Function:       PwrErrInit"*/
/*"  Description:    正常上电初始化电源异常事件处理"*/
/*"  Calls:          "*/
/*"  Called By:      StatusInit"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void PwrErrInit(void)
{
    CRC_DATA_TP crcTmp;
    crcTmp = CRC16((INT8U *)&mstPDPowerErrEvent,FIND(ST_PDPOWERERREVENT, crc));
    if((mstPDPowerErrEvent.crc != crcTmp) || (mstPDPowerErrEvent.KeyWord != 0xA5A5))
    {
    }
    else
    {
        LIB_MemCpy((INT8U *)&mstPDPowerErrEvent.BeginTime, (INT8U *)&DCDataBuf[0], 6);
        GetSingle(E_SYS_TIME, (INT8U *)&DCDataBuf[6]);
        SaveRcrd(E_PWR_FAILURE_EVNT_RCRD, E_EVTLST_PWR_FAILURE_S);
        SaveRcrd(E_PWR_FAILURE_EVNT_RCRD, E_EVTLST_PWR_FAILURE_E);
    }
    LIB_MemSet(0,(INT8U *)&mstPDPowerErrEvent,sizeof(ST_PDPOWERERREVENT));
}
/*"*****************************************************************************"*/
/*"  Function:       PDPwrErrInit"*/
/*"  Description:    低功耗初始化电源异常事件处理"*/
/*"  Calls:          "*/
/*"  Called By:      StatusInit"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void PDPwrErrInit(void)
{
#ifdef SINGLE_PHASE
    INT8U RelayType;/*"继电器类型，内置or外置"*/
    INT8U RelayAction;/*"继电器控制理论状态"*/

    mClsPowerErrNum = 0;
    mPwrErrFlg = 0;
    GetSingle(E_RELAY_TYPE, (INT8U *)&RelayType);

    if(FALSE == ChkStatus(E_RAM_VALID))
    {
        mVoltageLowFlg = TRUE;
    }

    RelayAction = OC_GetRelayCtrlStatus();
    /*"内置继电器表需要处于合闸状态，或者外置表"*/
    if((((RelayType & RLYTYPE) == RLY_OUTSIDE) ||((RelayType & RLYTYPE) == RLY_PULSEOUTSIDE)|| (RelayAction == RELAY_ACT_CLOSE))
        && (mstPDPowerErrEvent.KeyWord != 0xa5a5) && (FALSE == mVoltageLowFlg))
    {
    }
    else
    {
        SID_Release(MCU_LPMODE_LIGHT, SID_REALY);
    }
#endif
}
/*"*****************************************************************************"*/
/*"  Function:       PDPwrErrPDMain"*/
/*"  Description:    掉电检测电源异常"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Type 1异常开始，0异常结束"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void PDPwrErr(INT8U Type)
{
    if(mstPDPowerErrEvent.KeyWord != 0xa5a5)
    {
        LIB_MemSet(0, (INT8U *) &mstPDPowerErrEvent, sizeof(ST_PDPOWERERREVENT));
        mstPDPowerErrEvent.KeyWord = 0xa5a5;
    }
    else
    {
    }
    if(Type == 1)
    {
        GetSingle(E_SYS_TIME, (INT8U *)&mstPDPowerErrEvent.BeginTime);
    }
    else
    {
    }
#ifdef SINGLE_PHASE
    SID_Release(MCU_LPMODE_LIGHT, SID_REALY);
#endif
    mstPDPowerErrEvent.crc = CRC16((INT8U *)&mstPDPowerErrEvent, FIND(ST_PDPOWERERREVENT, crc));
}
/*"*****************************************************************************"*/
/*"  Function:       PDPwrErrTsk1s"*/
/*"  Description:    掉电检测主任务"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#ifdef SINGLE_PHASE
static void PDPwrErrTsk1s(void)
{
    /*"电源异常检测"*/
    if(mClsPowerErrNum < 120)/*"120s后关闭检测"*/
    {
        mClsPowerErrNum += 1;
        if(SID_TRUE == SID_PollRead(SID_REALY))
        {
            mPwrErrFlg += 1;
            if(mPwrErrFlg > 3)/*"连续3s后检测后记事件"*/
            {
                PDPwrErr(1);
            }
            else
            {
            }
        }
        else
        {
            mPwrErrFlg = 0;
        }
    }
    else
    {
        SID_Release(MCU_LPMODE_LIGHT, SID_REALY);
    }
}
#else
static void PDPwrErrTsk1s(void)
{
   INT8U RelayType;/*"继电器类型，内置or外置"*/
   INT8U RelayAction;/*"继电器控制理论状态"*/
   INT32U tempVoltBase;
   INT16U tVolt[3];
   INT64U tMode=0;

   GetSingle(E_PWR_RST_MODE, (INT8U *)&tMode); 
   if(tMode == PWR_RST_BATT)
   {
      return;
   }

   if(mntPdLastTime == 5)
   {
      GetSingle(E_VOLT_BLK, (INT8U *)&tVolt[0]);    /*"10-2V"*/
      GetSingle(E_NOMINAL_VOLT, (INT8U *)&tempVoltBase);
      tempVoltBase=tempVoltBase/100;    /*"10-2V"*/
      GetSingle(E_RELAY_TYPE, (INT8U *)&RelayType);
      RelayAction = OC_GetRelayCtrlStatus();
      /*"内置继电器表需要处于合闸状态，或者外置表"*/
      if((!(((RelayType & RLYTYPE) == RLY_INSIDE)&&(RelayAction == RELAY_ACT_TRIP)))
           && (mstPDPowerErrEvent.KeyWord != 0xa5a5))
      {
          if((tVolt[0]*100)>=(tempVoltBase*80)||(tVolt[1]*100)>=(tempVoltBase*80)||(tVolt[2]*100)>=(tempVoltBase*80))
          {
             PDPwrErr(1);
          }
      }
   }
}
#endif
#endif

#define  VOLT_200    (200)
#define  VOLT_170    (170)
#define  VOLT_60     (60)
#define  VOLT_70     (70)
#define  VOLT_75     (75)
#define  VOLT_80     (80)
typedef union       /*"电表工作电压状态"*/
{
    INT8U byte;
    struct
    {
        INT8U OverNormal :1; /*"单相电压高于正常电压(临界电压)"*/
        INT8U OverRelayVolt :1; /*"单相电压高于继电器正常工作电压"*/
        INT8U OverCardVolt :1; /*"某相电压高于IC卡正常工作电压*/
        INT8U OverBLcdVolt :1; /*"某相电压高于LCD正常工作电压*/
        INT8U OverComVolt :1; /*"某相电压高于通信正常工作电压*/
        INT8U :3; /*"保留*/
    } bits;
}UN_VOLT_STATUS;
const  INT8U  VoltStatusList[5] = {E_VOLT_NORMAL, E_VOLT_RELAY_OK, E_VOLT_ICCARD_OK, E_VOLT_LCD_OK,E_VOLT_COM_OK};

static void VoltLevelInit(void)
{
   INT8U i;
   
   for(i = 0; i < 5; i++)
   {     
      Push((EN_MTR_STATUS)VoltStatusList[i], FALSE);   
   }

   Push(E_VOLT_ICCARD_OK,TRUE);/*"上电时，强制为插卡电压有效"*/

   Push(E_VOLT_COM_OK,TRUE);

   Push(E_VOLT_LCD_OK,TRUE);
}
#ifdef SINGLE_PHASE
#define  RATE_VOLT     (2200ul)/*"应用层电压改为了1位小数"*/
/*"*****************************************************************************"*/
/*"  Function:       VoltLevelChk"*/
/*"  Description:    工作电压等级检测"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void VoltLevelChk(void)
{
    INT32U rVolt;
    UN_VOLT_STATUS VoltStatus;
    INT8U i;
    INT8U mask;

    rVolt = 0;
    GetSingle(E_VOLT_LA, (INT8U *)&rVolt);

#ifdef PWR_ERR_CHK
            mVoltageLowFlg = TRUE;
#endif
            rVolt *= 100;
    if(rVolt <= (VOLT_170 * RATE_VOLT))
    {
        VoltStatus.byte = 0xff;

        if(rVolt < (VOLT_60 * RATE_VOLT))
        {
            VoltStatus.byte = 0x00;
#ifdef PWR_ERR_CHK
            if(rVolt == 0)
            {
                mVoltageLowFlg = FALSE;
            }
#endif
        }
        else if(rVolt < (VOLT_75 * RATE_VOLT))
        {
            VoltStatus.byte = 0x01; /*"OverCardVolt,OverRelayVolt,OverWaveVolt,OverBLcdVolt置0"*/
        }
        else if(rVolt < (VOLT_80 * RATE_VOLT))
        {
            VoltStatus.byte = 0x1B; /*"OverCardVolt置0"*/
        }
        else
        {
#ifdef PWR_ERR_CHK
            mVoltageLowFlg = FALSE;
#endif
        }
    }
    else/*"大于1.7倍,不执行继电器动作"*/
    {
        VoltStatus.bits.OverRelayVolt = 0;
    }
    mask = 0x01;
    for(i = 0; i < 5; i++)
    {
        if((VoltStatus.byte & ((INT8U)(mask << i))) == 0)
        {
            Push((EN_MTR_STATUS)VoltStatusList[i], FALSE);
        }
        else
        {
            Push((EN_MTR_STATUS)VoltStatusList[i], TRUE);
        }
    }
}
/*"*****************************************************************************"*/
/*"  Function:       CurrLevelChk"*/
/*"  Description:    电流等级检测"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void CurrLevelChk(void)
{
    INT32S currTmp;
    INT32U rCurr;

    GetSingle(E_CURR_LA, (INT8U *)&currTmp);
    rCurr = LIB_ABS(currTmp);

    if((rCurr > MntPara.RlyTripCtrlCurr) && (MntPara.RlyTripCtrlCurr > 0))
    {
        Push(E_CURR_PROTECT, TRUE);
    }
    else
    {
        Push(E_CURR_PROTECT, FALSE);
    }
}
#else /*"三相表电压、电流等级检测"*/
/*"*****************************************************************************"*/
/*"  Function:       VoltLevelChk"*/
/*"  Description:    工作电压等级检测"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define REALY_MAXVOLT_RATIO  (77)
#define REALY_VOLT_RATIO        (58)
#define LCD_MAXVOLT_RATIO     (77)
#define LCD_VOLT_RATIO            (58)
static void VoltLevelChk(void)
{
    INT8U i;
    INT32U maxVoltRate[5] = {60, REALY_MAXVOLT_RATIO, 77, LCD_MAXVOLT_RATIO,77};
    INT32U minVoltRate[5]  = {60, REALY_VOLT_RATIO,       77, LCD_VOLT_RATIO,      60};
    
    for(i = 0; i < 5; i++)
    {
        if((mVoltMax>=(mVoltBase*maxVoltRate[i]/100))||(mVoltMin>=(mVoltBase*minVoltRate[i]/100)))
        { /*"一相电压大于Un*maxVoltRate 或者 三相电压均大于Un*minVoltRate"*/
            Push((EN_MTR_STATUS)VoltStatusList[i],TRUE);
        }
        else
        {
            Push((EN_MTR_STATUS)VoltStatusList[i], FALSE);
        }
    }

    if(mVoltMax <= 100)  /*"辅助电源供电"*/
    {
        Push(E_AUX_PWR,TRUE);
        for(i = 1; i < 5; i++)
        {
            Push((EN_MTR_STATUS)VoltStatusList[i],TRUE);        
        }
    }
    else
    {
        Push(E_AUX_PWR,FALSE);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       CurrLevelChk"*/
/*"  Description:    电流等级检测"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void CurrLevelChk(void)
{

    if((mCurrMax > MntPara.RlyTripCtrlCurr) && (MntPara.RlyTripCtrlCurr > 0))
    {
        Push(E_CURR_PROTECT, TRUE);
    }
    else
    {
        Push(E_CURR_PROTECT, FALSE);
    }
}
#endif
#ifdef THREE_PHASE
INT8U magnetFlag;         /*"恒定磁场事件标志"*/
static INT8U magnetAccTimes;  /*"恒定磁场发生、结束累计时间"*/

static void MagnetEventSecTask(void)
{
   INT8U nowState;
   INT16U evtObis;
   INT8U d;

   nowState=SID_PollRead(SID_MAGNET);
   
   if(magnetFlag != nowState)
   {
      magnetAccTimes++;

      if(magnetAccTimes >= 5)
      {
          if(nowState == SID_TRUE)  /*"开始"*/
          {
             evtObis=E_EVTLST_CONSTANT_MGNT_DISTURB_S;
             GetSingle(E_SYS_TIME, &DCDataBuf[0]);
    	      d=TRUE;
          }
          else /*"结束"*/
          {
             evtObis=E_EVTLST_CONSTANT_MGNT_DISTURB_E;
    	      GetSingle(E_SYS_TIME, &DCDataBuf[6]);
    	      d=FALSE;
          }
          SaveRcrd(E_CONSTANT_MGNT_DISTURB_EVNT_RCRD, evtObis);
          magnetFlag=nowState;
          FS_WriteFile(E_FILE_MNT, MAGNET_FLG_SHIFT,  (INT8U *)&magnetFlag, 1);
          Push(E_MAGNET, d);
          magnetAccTimes=0;
      }
   }
   else
   {
       magnetAccTimes=0;
   }
}

static void MagnetEventInit(void)
{
   FS_ReadFile(E_FILE_MNT, MAGNET_FLG_SHIFT,  (INT8U *)&magnetFlag, 1);
   if(magnetFlag == SID_TRUE)
   {
      GetSingle(E_SYS_TIME, &DCDataBuf[6]);
      SaveRcrd(E_CONSTANT_MGNT_DISTURB_EVNT_RCRD, E_EVTLST_CONSTANT_MGNT_DISTURB_E);
      magnetFlag=SID_FALSE;
      FS_WriteFile(E_FILE_MNT, MAGNET_FLG_SHIFT,  (INT8U *)&magnetFlag, 1);
      Push(E_MAGNET, FALSE);
   }
   magnetAccTimes=0;
}
#endif

#ifdef MU_ERR_CHK
static INT8U muErrFlag;
static INT8U muErrAccTimes;

static void MuErrSecTask(void)
{
   INT8U nowState=0;
   INT16U evtObis=0;

   nowState=ChkStatus(E_MU_IC_ERR);

   if(muErrFlag != nowState)
   {
      muErrAccTimes++;

      if(muErrAccTimes >= MntPara.MuErrDelayT)
      {
          if(nowState == TRUE)  /*"开始"*/
          {
             evtObis=E_EVTLST_MS_ERR_S;
             GetSingle(E_SYS_TIME, &DCDataBuf[0]);
          }
          else /*"结束"*/
          {
             evtObis=E_EVTLST_MS_ERR_E;
    	      GetSingle(E_SYS_TIME, &DCDataBuf[6]);
          }
          SaveRcrd(E_MS_ERR_EVNT_RCRD, evtObis);
          muErrFlag=nowState;
          FS_WriteFile(E_FILE_MNT, MUERR_FLG_SHIFT,  (INT8U *)&muErrFlag, 1);
          muErrAccTimes=0;
      }
   }
   else
   {
       muErrAccTimes=0;
   }
}

static void MuErrInit(void)
{
   FS_ReadFile(E_FILE_MNT, MUERR_FLG_SHIFT,  (INT8U *)&muErrFlag, 1);
   muErrAccTimes=0;
}
#endif

static INT8U clkErrFlag;
static INT8U clkErrAccTimes;

static void ClkErrSecTask(void)
{
   INT8U nowState=0;
   INT16U evtObis=0;

   nowState=ChkStatus(E_CLK_ERR);

   if(clkErrFlag != nowState)
   {
      clkErrAccTimes++;

      if(clkErrAccTimes >= MntPara.ClkErrDelayT)
      {
          if(nowState == TRUE)  /*"开始"*/
          {
             evtObis=E_EVTLST_RTC_ERR_S;
             GetSingle(E_SYS_TIME, &DCDataBuf[0]);
	      SaveRcrd(E_RTC_ERR_EVNT_RCRD, evtObis);
          }
          else /*"结束"*/
          {
             
          }
          clkErrFlag=nowState;
          FS_WriteFile(E_FILE_MNT, CLKERR_FLG_SHIFT,  (INT8U *)&clkErrFlag, 1);
          clkErrAccTimes=0;
      }
   }
   else
   {
       clkErrAccTimes=0;
   }
}

static void ClkErrInit(void)
{
   FS_ReadFile(E_FILE_MNT, CLKERR_FLG_SHIFT,  (INT8U *)&clkErrFlag, 1);
   if(clkErrFlag == TRUE)
   {
       clkErrFlag=FALSE;
       FS_WriteFile(E_FILE_MNT, CLKERR_FLG_SHIFT,  (INT8U *)&clkErrFlag, 1);
   }
   clkErrAccTimes=0;
}


static INT8U oscErrFlag;
static INT8U oscErrAccTimes;
#define OSC_ERR_DELAY  (60)

static void OscErrSecTask(void)
{
   INT8U nowState=0;
   INT16U evtObis=0;

    if (FALSE == MCU_Maintain(MCU_RUNMODE))
    {
        Push(E_OSCILLATOR_ERR, TRUE);
    }
    else
    {
        Push(E_OSCILLATOR_ERR, FALSE);
    }

   nowState=ChkStatus(E_OSCILLATOR_ERR);

   if(oscErrFlag != nowState)
   {
      oscErrAccTimes++;

      if(oscErrAccTimes >= OSC_ERR_DELAY)
      {
          if(nowState == TRUE)  /*"开始"*/
          {
             evtObis=E_EVTLST_SOSCILLATOR_ERR_S;
             GetSingle(E_SYS_TIME, &DCDataBuf[0]);
	      SaveRcrd(E_OSCILLATOR_ERR_RCRD, evtObis);
          }
          else /*"结束"*/
          {
             
          }
          oscErrFlag=nowState;
          FS_WriteFile(E_FILE_MNT, OSCERR_FLG_SHIFT,  (INT8U *)&oscErrFlag, 1);
          oscErrAccTimes=0;
      }
   }
   else
   {
       oscErrAccTimes=0;
   }
}

static void OscErrInit(void)
{
   FS_ReadFile(E_FILE_MNT, OSCERR_FLG_SHIFT,  (INT8U *)&oscErrFlag, 1);
   oscErrAccTimes=0;
}

/*"************************************************"*/
/*"Function:   StatusEvntClr "*/
/*"Description: 清状态监测事件时调用"*/
/*"Input: enumId 状态监测事件的枚举号(见EN_STATUS_EVNT)，0xff为全清"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
typedef struct
{
    INT16U  file;
    INT8U   *pEvntFlg;
    INT16U  FileShift;
}ST_STATUS_EVNT_FILE_TO_FLG_ADDR;

const ST_STATUS_EVNT_FILE_TO_FLG_ADDR StatusEvntFileToFlgAddr[] =
        {
#ifdef OPEN_COVER_CHK
                {E_MTR_COVER_OPEN_EVNT_RCRD, &mCoverStatus[0], OPEN_COVER_FLG_SHIFT},
#endif
#ifdef THREE_PHASE
                {E_TAIL_COVER_OPEN_EVNT_RCRD, &mCoverStatus[1], (OPEN_COVER_FLG_SHIFT+1)},
#endif
#ifdef RELAY_ERR_CHK
                {E_RELAY_ERR_EVNT_RCRD, &mRelayErrFlg, RELAY_ERR_FLG},
#endif
#ifdef THREE_PHASE
                {E_CONSTANT_MGNT_DISTURB_EVNT_RCRD, &magnetFlag, MAGNET_FLG_SHIFT},
#endif
#ifdef MU_ERR_CHK
                {E_MS_ERR_EVNT_RCRD, &muErrFlag, MUERR_FLG_SHIFT},
#endif
                {E_RTC_ERR_EVNT_RCRD, &clkErrFlag, CLKERR_FLG_SHIFT},
                {E_OSCILLATOR_ERR_RCRD, &oscErrFlag, OSCERR_FLG_SHIFT},
#ifdef GLX_CHG_CHK
                {GLX_CHG_FILENAME, &gGlxChgFlag, GLXCHG_FLG_SHIFT},
#endif
        };

#define E_MAX_STATUS_EVNT  (sizeof(StatusEvntFileToFlgAddr)/sizeof(ST_STATUS_EVNT_FILE_TO_FLG_ADDR))
void StatusEvntClr(INT16U evntFileId)
{
    INT8U num;

    for(num = 0; num < E_MAX_STATUS_EVNT; num++)
    {
        if((StatusEvntFileToFlgAddr[num].file == evntFileId) || (evntFileId >= 0xfffe))
        {
            (*StatusEvntFileToFlgAddr[num].pEvntFlg) = 0;
#ifdef OPEN_COVER_CHK
            if((E_MTR_COVER_OPEN_EVNT_RCRD == evntFileId) || (evntFileId >= 0xfffe))
            {
                Push(E_OPEN_COVER, mCoverStatus[0]);
            }
#endif
#ifdef THREE_PHASE
	     if((E_TAIL_COVER_OPEN_EVNT_RCRD == evntFileId) || (evntFileId >= 0xfffe))
            {
                Push(E_OPEN_TAIL, mCoverStatus[1]);
            }
#endif
            FS_WriteFile(E_FILE_MNT, StatusEvntFileToFlgAddr[num].FileShift, 0, 1);
        }
        else
        {
        }
    }
}


void MNT_PwrUpSEInit(void)
{
#ifdef OPEN_COVER_CHK
   OpenCoverInit();
#endif
#ifdef RELAY_ERR_CHK
   RelayChkInit();
#endif
#ifdef THREE_PHASE
   MagnetEventInit();
#endif
#ifdef PWR_ERR_CHK
   PwrErrInit();
#endif
#ifdef MU_ERR_CHK
   MuErrInit();
#endif
   ClkErrInit();

   OscErrInit();
#ifdef ACCRCY_CHK
   AccrcyChkInit();
#endif
}

const EN_MTR_STATUS bm_status[] =
{
    E_FAC,/*"工厂状态"*/
    E_PRGEVENT_START,/*"编程状态"*/
    E_FIRST_PWR_ON,/*"初次上电"*/
    E_OPEN_COVER,/*"开表盖"*/
    E_OPEN_TAIL, /*"开端盖"*/
    E_CLK_ERR, /*"时钟故障"*/
    
    E_CLK_BAT_LOSS, /*"时钟电池欠压"*/
    E_DIS_BAT_LOSS, /*"低功耗电池欠压"*/
    //E_MAGNET, /*"磁场干扰"*/
    E_MTR_RST,/*"电表复位"*/
    E_LRM_PD_DATA_ERR,/*"法定掉电存数数据不合法"*/
    E_OSCILLATOR_ERR,/*"晶振故障"*/
    E_RAM_VALID,/*"RAM数据维持合法"*/
    E_MTR_INIT,/*"电表初始化"*/
    
    E_MU_JUST_MTR,/*"校表状态 0 未校表  1已校表"*/
    E_RTC_CALIB, /*"RTC补偿校准状态 0 未校准  1已校准"*/
    E_LOW_PWR,/*"低功耗状态"*/
    
    E_EEP_ERR,/*"EEPROM故障"*/
    E_FLASH_ERR,/*"外部FLASH存储故障"*/
    
    E_MU_IC_ERR,/*"计量芯片故障"*/
    E_MU_DATA_READY,/*"计量数据准备就绪状态  1就绪  0未就绪"*/
    
    E_AUX_PWR,  /*"辅助电源供电  1 辅助电源供电    0 市电供电"*/
    E_BAT_PWR, /*"电池供电  1 电池供电    0 主电源供电"*/
    //E_RELAY_CMD_STATUS,/*"继电器命令状态  1 跳闸    0 合闸"*/
    //E_RELAY_ALARM,/*"继电器预跳闸状态  1 报警    0 无报警"*/
    
    E_PWR_DOWN, /*"掉电"*/
    E_RELAY_ERR, /*"继电器误动作"*/

    //E_RELAY_STATUS,/*"继电器状态通过反馈信号和功率判断继电器通断  1 断 0 通"*/
};

/*" 目前只用了24个状态，预留为32个状态，总计4字节 "*/
INT8U get_bm_status(INT8U *buf)
{
    INT8U i;

    LIB_MemSet(0x00, buf, 4);
    for(i=0; i<ARRAY_SIZE(bm_status); i++)
    {
        if(ChkStatus(bm_status[i]) == TRUE)
        {
            buf[i >> 3] |= (1 << (i & 0x07));
        }
        else
        {
            buf[i >> 3] &= ~(1 << (i & 0x07));
        }
    }
    return 4;
}

#ifdef TMNL_TMP_CHK
/*"*****************************************************************************"*/
/*"  Function:       TmnlTmpInit"*/
/*"  Description:   端子排测温初始化"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void TmnlTmpInit(void)
{
    INT8U i;

   #if(MNT_VERSION == STATE_GRID)
   TSensor_Init();
   #endif
    for(i = 0; i< E_TEMP_MAX; i++)
    {
        gTmnlTmp[i]=0;
    }

    HTSensor_Init();
    gMtrTmp=0;
    gMtrHmdty=0;
    gMtrTHCnt=0;
}

/*"*****************************************************************************"*/
/*"  Function:       TmnlTmpSecTask"*/
/*"  Description:   端子排测温秒任务"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void TmnlTmpSecTask(void)
{
    #if(MNT_VERSION == STATE_GRID)
    INT8U i;
    
    for(i = 0; i< E_TEMP_MAX; i++)
    {
        gTmnlTmp[i]=TSensor_GetVal(i);
    }
    #elif(MNT_VERSION == SOUTH_GRID)
    INT32S d;
    ST_TH_VAL t;
    
    GetSingle(E_LRM_TMP_LINE_IN, (INT8U *)&d);
    gTmnlTmp[E_TEMP_LIN]=(INT16S)d;
    
    GetSingle(E_LRM_TMP_LINE_OUT, (INT8U *)&d);
    gTmnlTmp[E_TEMP_LOUT]=(INT16S)d;
    
    GetSingle(E_LRM_TMP_N_IN, (INT8U *)&d);
    gTmnlTmp[E_TEMP_NIN]=(INT16S)d;
    
    GetSingle(E_LRM_TMP_N_OUT, (INT8U *)&d);
    gTmnlTmp[E_TEMP_NOUT]=(INT16S)d;

    if(Get_HTSensorVal(gMtrTHCnt, &t) == TRUE)
    {
        if(gMtrTHCnt == 1)
        {
           gMtrTmp=t.Temperature/10;   //XXX.X
           gMtrHmdty=t.Humidity/10;     //XX.XX
        }
    }
    gMtrTHCnt = (gMtrTHCnt+1)%2;
    #endif
}

#endif

#ifdef GLX_CHG_CHK
/*"*****************************************************************************"*/
/*"  Function:       GlxChgChkInit"*/
/*"  Description:                       "*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void GlxChgChkInit(void)
{
    FS_ReadFile(E_FILE_MNT, GLXCHG_ESAMID_SHIFT,  (INT8U *)&gGlxChgEsamID[0], 8);
    FS_ReadFile(E_FILE_MNT, GLXCHG_FLG_SHIFT,  (INT8U *)&gGlxChgFlag, 1);
    
    gGlxChgCnt=0;
}
/*"*****************************************************************************"*/
/*"  Function:       GlxChgChkSecTask"*/
/*"  Description:                       "*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void GlxChgChkSecTask(void)
{

    if(gGlxChgFlag == TRUE)
    {
        if(GPIO_ReadPin(COM_RQ) == LOW)
        {
           gGlxChgCnt++;
           if(gGlxChgCnt >= GLXCHGDELAY)
           {
               gGlxChgFlag=FALSE;
               FS_WriteFile(E_FILE_MNT, GLXCHG_FLG_SHIFT,  (INT8U *)&gGlxChgFlag, 1);

               GetSingle(E_SYS_TIME, &DCDataBuf[6]);
	        SaveRcrd(GLX_CHG_FILENAME, GLX_CHG_FILE_E); 
           }
        }
        else
        {
           gGlxChgCnt=0;
        }       
    }
    else
    {
        if(GPIO_ReadPin(COM_RQ) == HIGH)
        {
           gGlxChgCnt++;
           if(gGlxChgCnt >= GLXCHGDELAY)
           {
               gGlxChgFlag=TRUE;
               FS_WriteFile(E_FILE_MNT, GLXCHG_FLG_SHIFT,  (INT8U *)&gGlxChgFlag, 1);

               GetSingle(E_SYS_TIME, &DCDataBuf[0]);
	        SaveRcrd(GLX_CHG_FILENAME, GLX_CHG_FILE_S); 

               Action(E_AUTHALLOW_KEEP, NULL, NULL);
           }
        }
        else
        {
           gGlxChgCnt=0;
        }
    }
}
#endif

#ifdef GLX_COMM_CHK
/*"*****************************************************************************"*/
/*"  Function:       GlxCommChkInit"*/
/*"  Description:                       "*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void GlxCommChkInit(void)
{
    FS_ReadFile(E_FILE_MNT, GLXCOMM_FLG_SHIFT,  (INT8U *)&mGlxCommFlag, 1);    
    mGlxCommCnt=0;
}
/*"*****************************************************************************"*/
/*"  Function:       GlxCommChkSecTask"*/
/*"  Description:                       "*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          StatusTask1s"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void GlxCommChkSecTask(void)
{

    if(mGlxCommFlag == TRUE)
    {
        if(ChkStatus(E_MGMT_COMM_ERR) == FALSE)
        {
           mGlxCommCnt++;
           if(mGlxCommCnt >= MAX_GLXCOMM_DELAY)
           {
               mGlxCommCnt=0;
               mGlxCommFlag=FALSE;
               FS_WriteFile(E_FILE_MNT, GLXCOMM_FLG_SHIFT,  (INT8U *)&mGlxCommFlag, 1);
               GetSingle(E_SYS_TIME, &DCDataBuf[6]);
	        SaveRcrd(E_FILE_MGMT_COMM_ERR_EVNT, E_EVTLST_MGMT_ERR_E); 
           }
        }
        else
        {
           mGlxCommCnt=0;
        }       
    }
    else
    {
        if(ChkStatus(E_MGMT_COMM_ERR) == TRUE)
        {
           mGlxCommCnt++;
           if(mGlxCommCnt >= MAX_GLXCOMM_DELAY)
           {
               mGlxCommCnt=0;
               mGlxCommFlag=TRUE;
               FS_WriteFile(E_FILE_MNT, GLXCOMM_FLG_SHIFT,  (INT8U *)&mGlxCommFlag, 1);
               GetSingle(E_SYS_TIME, &DCDataBuf[0]);
	        SaveRcrd(E_FILE_MGMT_COMM_ERR_EVNT, E_EVTLST_MGMT_ERR_S); 
           }
        }
        else
        {
           mGlxCommCnt=0;
        }
    }
}
#endif

#ifdef ACCRCY_CHK
/*"*****************************************************************************"*/
/*"  Function:       AccrcyChkInit"*/
/*"  Description:   "*/
/*"  Calls:          "*/
/*"  Called By:   "*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void AccrcyChkInit(void)
{
   mAccrcyRfrshCnt=0;
   mPwrAmpErr=0;
}
/*"*****************************************************************************"*/
/*"  Function:       AccrcyChkMinTask"*/
/*"  Description:   "*/
/*"  Calls:          "*/
/*"  Called By:   "*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
extern void Host_Commu_AccryChk_Push(void);
static void AccrcyChkMinTask(void)
{
   if(MntPara.AccrcyChkRfrshT == 0)
   {
       mAccrcyRfrshCnt=0;
       mPwrAmpErr=0;
       return;
   }
   
   mAccrcyRfrshCnt++;
   if(mAccrcyRfrshCnt >= MntPara.AccrcyChkRfrshT)
   {
       mAccrcyRfrshCnt=0;
       GetSingle(E_AMP_ERR, (INT8U *)&mPwrAmpErr);
       Host_Commu_AccryChk_Push();
   }
}
#endif

