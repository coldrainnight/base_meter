/*"*****************************************************************************"*/
/*"  FileName: EU.c"*/
/*"  Description:     该文件法定计量模块电能量数据处理相关功能"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/10/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include  "EU.h"
#include  "MU_lib.h"
#include  "LRM.h"
#include  "SC.h"
#include "Driver\device\MU_IC.h"

#define  VALIDFLAG       (0x5A)
#define  ENGY_ADDR1      (0X01)/*电量备份内卡地址1*/
#define  ENGY_ADDR2      (0X02)/*电量备份内卡地址2*/
#define  ENGY_BAK_ADDR_FLG     stPdSaveData.EngyAddrFlag   /*"当前内卡中的电量备份块"*/
#define  ENGY_BAK_PEROID  (30 * 60)   /*电量备份周期（秒）  30分钟*/

/*"*********************计量模块输出数据**************************"*/
NO_INIT ST_RAM_ENGY_DATA stNowEngy;/*"当前电量数据总+当前费率"*/
NO_INIT ST_LRM_DATA stPdSaveData;/*" 当前法定模块数据（包括当前电量累计值、电量尾数、掉电存数数据）"*/
NO_INIT ST_BAKUP_ENGY_DATA stNowEngyBak;/*" 当前电量数据RAM备份"*/
#ifdef  AMP_HOUR_EN
INT32U AmpHour[MU_ELEMENT_NUM];/*"上电后的累计安时值，单位 0.01AH 掉电后清零"*/
static INT32U AmpHourTail[MU_ELEMENT_NUM];/*"累计安时值尾数，单位 0.0001A.s 掉电后清零"*/
#define ANHOUR_VALUE_01ANH   360000 /*"将0.0001An.s转换成0.01An.h"*/
#endif
static INT8U mPdDateCrcBrkFlg; /*"掉电区CRC破坏标识"*/
static INT16U EngyBakPeriodCnt;/*"电量备份周期时间累计变量，单位秒"*/
/*"-----public fuction-------"*/
void EU_Init(INT8U mode);
void EU_secTask(void);
INT8U EU_Clr(INT8U kind);
void EU_PwrDown(void);

/*"-----private fuction------"*/
static void RenewTariffData(void);
static void EngySave(INT8U flag);
static INT8U NowEngyDataChk(ST_RAM_ENGY_DATA *pdata);
static INT8U BakEngyDataChk(ST_BAKUP_ENGY_DATA *pdata);
static INT8U GetEepEngy(void);

static void LoadEngyData(INT8U flag, INT8U mode);

static void EngyTailChkPwrUp(void);
static void RamEngyTailChk(void);
static void Engyadd(void);
static void EngyTailAdd(void);
static void PdDateCrcBrkFlg(void); 
static INT8U EngyCarryChk(INT32U *pEngyTail, ENGY_TYPE_U *pDestEngyTotal, ENGY_TYPE_U *pDestEngyTariff);
#if (TARIFF_NUM >= 1)
static INT8U EngSumChk(ST_BAKUP_ENGY_DATA *pData);
static INT8U TariffEngyDivOk(void);
static INT8U  ChkEngyRegActive(INT16U obis);
#endif

#ifdef WS_METER_HT7038_EXENERGY
static void ExEngyAccumulation(void);
#endif

#ifdef ENGY_REGISTER_ACTIVE_FUN
const  EN_LRM_DATA_ID AssignObj[] =
{
    E_PA_ENGY_T1,/*"正向有功费率1电量"*/
    E_PA_ENGY_T2,/*"正向有功费率2电量"*/
    E_PA_ENGY_T3,/*"正向有功费率3电量"*/
    E_PA_ENGY_T4,/*"正向有功费率4电量"*/
    E_NA_ENGY_T1,/*"反向有功费率1电量"*/
    E_NA_ENGY_T2,/*"反向有功费率2电量"*/
    E_NA_ENGY_T3,/*"反向有功费率3电量"*/
    E_NA_ENGY_T4,/*"反向有功费率4电量"*/
    E_PR_ENGY_T1,/*"正向无功费率1电量"*/
    E_PR_ENGY_T2,/*"正向无功费率2电量"*/
    E_PR_ENGY_T3,/*"正向无功费率3电量"*/
    E_PR_ENGY_T4,/*"正向无功费率4电量"*/
    E_NR_ENGY_T1,/*"反向无功费率1电量"*/
    E_NR_ENGY_T2,/*"反向无功费率2电量"*/
    E_NR_ENGY_T3,/*"反向无功费率3电量"*/
    E_NR_ENGY_T4,/*"反向无功费率4电量"*/
    E_R_ENGY_I_T1,/*"I象限无功费率1电量"*/
    E_R_ENGY_I_T2,/*"I象限无功费率2电量"*/
    E_R_ENGY_I_T3,/*"I象限无功费率3电量"*/
    E_R_ENGY_I_T4,/*"I象限无功费率4电量"*/
    E_R_ENGY_II_T1,/*"II象限无功费率1电量"*/
    E_R_ENGY_II_T2,/*"II象限无功费率2电量"*/
    E_R_ENGY_II_T3,/*"II象限无功费率3电量"*/
    E_R_ENGY_II_T4,/*"II象限无功费率4电量"*/
    E_R_ENGY_III_T1,/*"III象限无功费率1电量"*/
    E_R_ENGY_III_T2,/*"III象限无功费率2电量"*/
    E_R_ENGY_III_T3,/*"III象限无功费率3电量"*/
    E_R_ENGY_III_T4,/*"III象限无功费率4电量"*/
    E_R_ENGY_IV_T1,/*"IV象限无功费率1电量"*/
    E_R_ENGY_IV_T2,/*"IV象限无功费率2电量"*/
    E_R_ENGY_IV_T3,/*"IV象限无功费率3电量"*/
    E_R_ENGY_IV_T4,/*"IV象限无功费率4电量"*/
};
#endif
/*"*****************************************************************************"*/
/*"  Function:       EU_Init"*/
/*"  Description:    电量累加模块初始化"*/
/*"  Calls:          "*/
/*"  Called By:      LRM初始化主接口函数"*/
/*"  Input:          mode      初始化模式字  "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void EU_Init(INT8U mode)
{
    mPdDateCrcBrkFlg = 0;
    EngyBakPeriodCnt = 0;

    /*计量尾数合法性检查*/
    EngyTailChkPwrUp();
    /*初始化加载合法的电量数据*/
    LoadEngyData(TRUE, mode);
#ifdef  AMP_HOUR_EN
    LIB_MemSet(0, (INT8U *)&AmpHourTail[0], (MU_ELEMENT_NUM << 2));
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       EU_secTask"*/
/*"  Description:    电量累加模块秒任务接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      LRM_main秒任务处理"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void EU_secTask(void)
{
#if (TARIFF_NUM >= 1)
    INT8U TariffNow;
#endif

    PdDateCrcBrkFlg();/*"破坏掉电存数合法性，避免上电重复处理"*/
    EngyBakPeriodCnt++;

    /*"备份标志容错处理"*/
    if((ENGY_BAK_ADDR_FLG & 0x03) == 0)
    {
        ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
    }
    /*"运行过程中电量数据合法性校验"*/
    LoadEngyData(FALSE, MCU_RUNMODE);
    /*未进行1kwh电量备份时进行RAM区备份*/
#if (TARIFF_NUM >= 1)
    GetSingle(E_TARIFF_NOW, &TariffNow);
    if((TariffNow == 0) || (TariffNow > TARIFF_NUM))/*"费率号容错处理"*/
    {
        TariffNow = 1;
    }

    if(TariffNow != stNowEngy.tariffNow)/*"费率切换，需切换当前费率电量数据"*/
    {
        MCU_PwrChkCloseINT();
        stNowEngy.tariffNow = TariffNow;
        LIB_MemCpy((INT8U *)&stNowEngyBak.TariffEngy[stNowEngy.tariffNow], (INT8U *)&stNowEngy.CurTEngy, sizeof(ST_BASE_ENGY));
        stNowEngy.ValidFlag = 0x5a5a;
        stNowEngy.Crc = CRC16((INT8U *)&stNowEngy, FIND(ST_RAM_ENGY_DATA,Crc));
        MCU_PwrChkOpenINT();
    }

    if((stNowEngyBak.tariffBak != stNowEngy.tariffNow) || (EngyBakPeriodCnt > ENGY_BAK_PEROID))
    {
        if (stNowEngyBak.tariffBak != stNowEngy.tariffNow)
        {
            EngySave(ENGY_ADDR1 | ENGY_ADDR2);
        }
        else
        {
            EngySave(ENGY_BAK_ADDR_FLG);
        }
        EngyBakPeriodCnt = 0;
    }
    else
    {
    }
#else
    if(EngyBakPeriodCnt > ENGY_BAK_PEROID)
    {
        EngySave(ENGY_BAK_ADDR_FLG);
        EngyBakPeriodCnt = 0;
    }
#endif
    /*"每S获得电能量数据单位（ws）"*/
    EngyTailAdd();
    Engyadd();
}
/*"*****************************************************************************"*/
/*"  Function:       EU_Clr"*/
/*"  Description:    电量数据清零"*/
/*"  Calls:          "*/
/*"  Called By:      ACTION列表"*/
/*"  Input:          kind  保留 清除类型"*/
/*"  Output:         "*/
/*"  Return:         TRUE 清零成功   FALSE  清零失败"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U EU_Clr(INT8U kind)
{
    INT8U   rsult;

    rsult = TRUE;
    LIB_MemSet(0, (INT8U *) &stNowEngy, sizeof(ST_RAM_ENGY_DATA));
    LIB_MemSet(0, (INT8U *) &stPdSaveData.stEngyTail, sizeof(ST_ENGY_TAIL));
    LIB_MemSet(0, (INT8U *) &stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));

    stNowEngy.ValidFlag = 0x5A5A;
    stPdSaveData.stEngyTail.ValidFlag = 0x5A5A;
    stPdSaveData.pdNowEngy.ValidFlag = 0x5A5A;
#if (TARIFF_NUM >= 1)
    stNowEngy.tariffNow = 1;
    stPdSaveData.pdNowEngy.tariffNow = 1;
#endif
    stNowEngy.Crc = CRC16((INT8U *)&stNowEngy, FIND(ST_RAM_ENGY_DATA,Crc));
    stPdSaveData.pdNowEngy.Crc = CRC16((INT8U *)&stPdSaveData.pdNowEngy, FIND(ST_RAM_ENGY_DATA,Crc));
    stPdSaveData.stEngyTail.Crc = CRC16((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
    /*分时备份电量清除*/
    LIB_MemSet(0, (INT8U *)&stNowEngyBak, sizeof(ST_BAKUP_ENGY_DATA));
#if (TARIFF_NUM >= 1)
    stNowEngyBak.tariffBak = 1;
#endif
    stNowEngyBak.ValidFlag = 0x5A5A;
    stNowEngyBak.Crc = CRC16((INT8U *)&stNowEngyBak, FIND(ST_BAKUP_ENGY_DATA,Crc));
    EngySave(ENGY_ADDR1 | ENGY_ADDR2);

    FS_WriteFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT, (INT8U *)&stPdSaveData, sizeof(ST_LRM_DATA));
    ENGY_BAK_ADDR_FLG = ENGY_ADDR1;

    return  (rsult);
}

/*"*****************************************************************************"*/
/*"  Function:       EU_PwrDown"*/
/*"  Description:    电量数据掉电存数处理"*/
/*"  Calls:          "*/
/*"  Called By:      ACTION列表"*/
/*"  Input:          kind  保留 清除类型"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void EU_PwrDown(void)
{
    /*获取mWS数据并转换成脉冲数存储*/
    if(FALSE == ChkStatus(E_MTR_INIT))
    {
#ifdef WS_METER_HT7038_EXENERGY
        ExEngyAccumulation();
#endif
        MU_IC_PD_Close();
        LIB_MemCpy((INT8U *) &stNowEngy, (INT8U *) &stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));
        //stPdSaveData.stEngyTail.Crc = CRC16((INT8U *) &stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
        LIB_MemCpy((INT8U *) &stSysTime, (INT8U *) &stPdSaveData.pdTime, sizeof(ST_6TIME_FORM));
        //LIB_MemSet(VALIDFLAG, (INT8U *)stPdSaveData.stEngyTail.ValidFlag, sizeof(stPdSaveData.stEngyTail.ValidFlag));
        //LIB_MemSet(VALIDFLAG, (INT8U *)stPdSaveData.pdNowEngy.ValidFlag, sizeof(stPdSaveData.pdNowEngy.ValidFlag));
        stPdSaveData.ValidFlag = 0x5A5A;
        stPdSaveData.crc = CRC16((INT8U *) &stPdSaveData, FIND(ST_LRM_DATA,crc));
        FS_WriteFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT, (INT8U *)&stPdSaveData, sizeof(ST_LRM_DATA));
    }

}
/*"*****************************************************************************"*/
/*"  Function:       RenewTariffData"*/
/*"  Description:    更新分时电量结构体"*/
/*"  Calls:          "*/
/*"  Called By:      电量数据备份容错"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RenewTariffData(void)
{
    MCU_PwrChkCloseINT();
    LIB_MemCpy((INT8U *) &stNowEngy.TotalEngy, (INT8U *) &stNowEngyBak.TariffEngy[0], sizeof(ST_BASE_ENGY));
#if (TARIFF_NUM >= 1)
    LIB_MemCpy((INT8U *) &stNowEngy.CurTEngy, (INT8U *) &stNowEngyBak.TariffEngy[stNowEngy.tariffNow], sizeof(ST_BASE_ENGY));
#endif
#if (PHASE_ENGY_EN == 1)
    LIB_MemCpy((INT8U *) &stNowEngy.Phase[0], (INT8U *) &stNowEngyBak.Phase, 3 * sizeof(ST_BASE_ENGY));
#endif
#if (TARIFF_NUM >= 1)
    stNowEngyBak.tariffBak = stNowEngy.tariffNow;
#endif
    stNowEngyBak.Crc = CRC16((INT8U *) &stNowEngyBak,  FIND(ST_BAKUP_ENGY_DATA,Crc));
    MCU_PwrChkOpenINT();
}
/*"*****************************************************************************"*/
/*"  Function:       EngySave"*/
/*"  Description:    电量数据备份并存内卡"*/
/*"  Calls:          "*/
/*"  Called By:      EU_main"*/
/*"  Input:          flag  备份类型标志"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void EngySave(INT8U flag)
{
    /*关闭掉电中断*/
    MCU_PwrChkCloseINT();
    RenewTariffData();
    if(flag & ENGY_ADDR1)
    {
        FS_WriteFile(E_FILE_LRM_DATA, ENGY_BAK2_SHIFT, (INT8U *) &stNowEngyBak, sizeof(ST_BAKUP_ENGY_DATA));
        ENGY_BAK_ADDR_FLG = ENGY_ADDR2;
    }

    if(flag & ENGY_ADDR2)
    {
        FS_WriteFile(E_FILE_LRM_DATA, ENGY_BAK1_SHIFT, (INT8U *) &stNowEngyBak, sizeof(ST_BAKUP_ENGY_DATA));
        ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
    }
    MCU_PwrChkOpenINT();
}
/*"*****************************************************************************"*/
/*"  Function:       NowEngyDataChk"*/
/*"  Description:    当前电量数据有效性检测"*/
/*"  Calls:          "*/
/*"  Called By:      电量更新前"*/
/*"  Input:          pdata  输入电量数据指针"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U NowEngyDataChk(ST_RAM_ENGY_DATA *pdata)
{
#if (TARIFF_NUM >= 1)
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_RAM_ENGY_DATA,Crc), pdata->Crc))/*标识字比对,CRC校验*/
    && ((pdata->tariffNow <= TARIFF_NUM) && (pdata->tariffNow != 0)))/*费率号正常*/
#else
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_RAM_ENGY_DATA,Crc), pdata->Crc))/*标识字比对,CRC校验*/
    )
#endif
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*"*****************************************************************************"*/
/*"  Function:       BakEngyDataChk"*/
/*"  Description:    备份电量数据有效性检测"*/
/*"  Calls:          "*/
/*"  Called By:      电量容错处理"*/
/*"  Input:          输入电量数据指针"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U BakEngyDataChk(ST_BAKUP_ENGY_DATA *pdata)
{
#if (TARIFF_NUM >= 1)
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_BAKUP_ENGY_DATA,Crc), pdata->Crc))/*标识字比对,CRC校验*/
    && ((pdata->tariffBak <= TARIFF_NUM) && (pdata->tariffBak != 0))/*费率号正常*/
    && (TRUE == EngSumChk(pdata)))/*总和分费率和比较*/
#else
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_BAKUP_ENGY_DATA,Crc), pdata->Crc))/*标识字比对,CRC校验*/
     /*费率号正常*/
    /*&& (TRUE == EngSumChk(pdata))*/) /*总和分费率和比较*/
#endif
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*"*****************************************************************************"*/
/*"  Function:       GetEepEngy"*/
/*"  Description:    从内卡EE1、EE2获取有效的电量数据"*/
/*"  Calls:          "*/
/*"  Called By:      电量容错处理（初始化、电量更新前）"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         0 EE1/EE2数据都异常  1当前备份电量数据正常  2上一周期备份电量数据正常   3 EE1/EE2备份电量数据都正常"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U GetEepEngy(void)
{
    INT8U ret;
    UN_BYTE_DEF EngyValidFlg;/*"0 valid  1 unvalid  b0  backup1  b1 backup2"*/
    ST_BAKUP_ENGY_DATA EngyBak1, EngyBak2;
    INT8U *pScr;
    INT8U i;

    EngyValidFlg.byte = 0;

    for(i = 0; i < MAX_READ_FILE_LOOP; i++)
    {
        ret = FS_ReadFile(E_FILE_LRM_DATA, ENGY_BAK1_SHIFT, (INT8U *)&EngyBak1, sizeof(ST_BAKUP_ENGY_DATA));/*读出EE1存储区*/
        if ((ret == TRUE) && (TRUE == BakEngyDataChk(&EngyBak1)))
        {
            EngyValidFlg.bit.b0 = TRUE;
            break;
        }
        MCU_DelayMs(10);
    }
    for (i = 0; i < MAX_READ_FILE_LOOP; i++)
    {
        ret = FS_ReadFile(E_FILE_LRM_DATA, ENGY_BAK2_SHIFT, (INT8U *) &EngyBak2, sizeof(ST_BAKUP_ENGY_DATA));/*读出EE2存储区*/
        if ((ret == TRUE) && (TRUE == BakEngyDataChk(&EngyBak2)))
        {
            EngyValidFlg.bit.b1 = TRUE;
            break;
        }
        MCU_DelayMs(10);
    }

    if((0x03 == (EngyValidFlg.byte & 0X03)))/*ee1,ee2对*/
    {
        INT8U BakEngyComp;

        BakEngyComp = LIB_RvsMemCmp((INT8U *) &EngyBak2.TariffEngy, (INT8U *) &EngyBak1.TariffEngy, sizeof(ST_BASE_ENGY));
        /*ee2>ee1*/
        if(BIG == BakEngyComp)
        {
            ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
            pScr = (INT8U *) &EngyBak2;
        }
        else if(EQUAL == BakEngyComp)
        {
#if (TARIFF_NUM >= 1)
            if(EngyBak2.tariffBak == stNowEngy.tariffNow)
            {
                ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
                pScr = (INT8U *) &EngyBak2;
            }
            else
            {
                ENGY_BAK_ADDR_FLG = ENGY_ADDR2;
                pScr = (INT8U *) &EngyBak1;
            }
#else
            ENGY_BAK_ADDR_FLG = ENGY_ADDR2;
            pScr = (INT8U *) &EngyBak1;
#endif
        }
        else
        {
            ENGY_BAK_ADDR_FLG = ENGY_ADDR2;
            pScr = (INT8U *) &EngyBak1;
        }
    }
    else if(0x01 == (EngyValidFlg.byte & 0X03))/*EE1存储区对*/
    {
        ENGY_BAK_ADDR_FLG = ENGY_ADDR2;
        pScr = (INT8U *) &EngyBak1;
    }
    else if(0x02 == (EngyValidFlg.byte & 0X03))/*EE2存储区对*/
    {
        ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
        pScr = (INT8U *) &EngyBak2;
    }
    else/*全错*/
    {

    }

    if(0 != (EngyValidFlg.byte & 0x03))
    {
        LIB_MemCpy(pScr, (INT8U *) &stNowEngyBak, sizeof(ST_BAKUP_ENGY_DATA));
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

#if (TARIFF_NUM >= 1)
/*"************************************************"*/
/*"Function:    EU_EngSumChk        "*/
/*"Description: 检查分电量之和与总电量之和的差值是否合法"*/
/*"Input:       pData 检查电量数据指针"*/
/*"Output:      无    "*/
/*"Return:      valFlg  TRUE 分电量之和与总电量之和的差值合法  FALSE 分电量之和与总电量之和的差值超范围"*/
/*"Others:        "*/
/*"************************************************"*/
#define  ENGY_DIFF_LMT     6000l
static INT8U EngSumChk(ST_BAKUP_ENGY_DATA *pData)
{
#ifndef  ENGY_REGISTER_ACTIVE_FUN
    INT8U valFlg;
    INT8U uni, unj;
    INT32S EngyDiv;
#if (QUAD_ACTIVE_ENGY_EN == 1)
    ENGY_TYPE_U tempEngyP[4];
    ENGY_TYPE_U TariffSumP[4];
    ENGY_TYPE_U TotalP[4];
#endif
    ENGY_TYPE_U tempEngyComP[2];
    ENGY_TYPE_U TariffSumComP[2];
    ENGY_TYPE_U TotalComP[2];
#if (REACTIVE_ENGY_EN == 1)
    ENGY_TYPE_U tempEngyQ[4];
    ENGY_TYPE_U TariffSumQ[4];
    ENGY_TYPE_U TotalQ[4];
#ifdef REACTIVE_ENGY_COM
    ENGY_TYPE_U tempEngyComQ[2];
    ENGY_TYPE_U TariffSumComQ[2];
    ENGY_TYPE_U TotalComQ[2];
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    ENGY_TYPE_U tempEngyComS[2];
    ENGY_TYPE_U TariffSumComS[2];
    ENGY_TYPE_U TotalComS[2];
#endif

    valFlg = TRUE;
#if (QUAD_ACTIVE_ENGY_EN == 1)
    LIB_MemSet(0, (INT8U *) &TariffSumP, sizeof(TariffSumP));
    LIB_MemSet(0, (INT8U *) &TotalP, sizeof(TotalP));
#endif
    LIB_MemSet(0, (INT8U *) &TariffSumComP, sizeof(TariffSumComP));
    LIB_MemSet(0, (INT8U *) &TotalComP, sizeof(TotalComP));
#if (REACTIVE_ENGY_EN == 1)
    LIB_MemSet(0, (INT8U *) &TariffSumQ, sizeof(TariffSumQ));
    LIB_MemSet(0, (INT8U *) &TotalQ, sizeof(TotalQ));
#ifdef REACTIVE_ENGY_COM
    LIB_MemSet(0, (INT8U *) &TariffSumComQ, sizeof(TariffSumComQ));
    LIB_MemSet(0, (INT8U *) &TotalComQ, sizeof(TotalComQ));
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    LIB_MemSet(0, (INT8U *) &TariffSumComS, sizeof(TariffSumComS));
    LIB_MemSet(0, (INT8U *) &TotalComS, sizeof(TotalComS));
#endif
    for(uni = 0; uni < TARIFF_NUM; uni++)
    {
#if (QUAD_ACTIVE_ENGY_EN == 1)
        for(unj = 0; unj < 4; unj++)
        {
            tempEngyP[unj] = 0;
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[uni + 1].P[unj]), (INT8U *) &tempEngyP[unj], sizeof(ST_ENERGY_FORM));
            TariffSumP[unj] += tempEngyP[unj];
        }
#endif
        for(unj = 0; unj < 2; unj++)
        {
            tempEngyComP[unj] = 0;
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[uni + 1].ComP[unj]), (INT8U *) &tempEngyComP[unj], sizeof(ST_ENERGY_FORM));
            TariffSumComP[unj] += tempEngyComP[unj];
        }
#if (REACTIVE_ENGY_EN == 1)
        for(unj = 0; unj < 4; unj++)
        {
            tempEngyQ[unj] = 0;
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[uni + 1].Q[unj]), (INT8U *) &tempEngyQ[unj], sizeof(ST_ENERGY_FORM));
            TariffSumQ[unj] += tempEngyQ[unj];
        }
#ifdef REACTIVE_ENGY_COM
        for(unj = 0; unj < 2; unj++)
        {
            tempEngyComQ[unj] = 0;
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[uni + 1].ComQ[unj]), (INT8U *) &tempEngyComQ[unj], sizeof(ST_ENERGY_FORM));
            TariffSumComQ[unj] += tempEngyComQ[unj];
        }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
        for(unj = 0; unj < 2; unj++)
        {
            tempEngyComS[unj] = 0;
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[uni + 1].ComS[unj]), (INT8U *) &tempEngyComS[unj], sizeof(ST_ENERGY_FORM));
            TariffSumComS[unj] += tempEngyComS[unj];
        }
#endif
    }
#if (QUAD_ACTIVE_ENGY_EN == 1)
    for(uni = 0; uni < 4; uni++)
    {
        LIB_MemCpy((INT8U *) &(pData->TariffEngy[0].P[uni]), (INT8U *) &TotalP[uni], sizeof(ST_ENERGY_FORM));
        EngyDiv = TotalP[uni] - TariffSumP[uni];
        if(LIB_ABS(EngyDiv) > ENGY_DIFF_LMT)
        {
            valFlg = FALSE;
            break;
        }
    }
#endif
    for(uni = 0; uni < 2; uni++)
    {
        LIB_MemCpy((INT8U *) &(pData->TariffEngy[0].ComP[uni]), (INT8U *) &TotalComP[uni], sizeof(ST_ENERGY_FORM));
        EngyDiv = TotalComP[uni] - TariffSumComP[uni];
        if(LIB_ABS(EngyDiv) > ENGY_DIFF_LMT)
        {
            valFlg = FALSE;
            break;
        }
    }
#if (REACTIVE_ENGY_EN == 1)
    if(TRUE == valFlg)
    {
        for(uni = 0; uni < 4; uni++)
        {
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[0].Q[uni]), (INT8U *) &TotalQ[uni], sizeof(ST_ENERGY_FORM));
            EngyDiv = TotalQ[uni] - TariffSumQ[uni];
            if(LIB_ABS(EngyDiv) > ENGY_DIFF_LMT)
            {
                valFlg = FALSE;
                break;
            }
        }
#ifdef REACTIVE_ENGY_COM
        if(TRUE == valFlg)
        {
            for(uni = 0; uni < 2; uni++)
            {
                LIB_MemCpy((INT8U *) &(pData->TariffEngy[0].ComQ[uni]), (INT8U *) &TotalComQ[uni], sizeof(ST_ENERGY_FORM));
                EngyDiv = TotalComQ[uni] - TariffSumComQ[uni];
                if(LIB_ABS(EngyDiv) > ENGY_DIFF_LMT)
                {
                    valFlg = FALSE;
                    break;
                }
            }
        }
#endif

    }
#endif
#if (APPARENT_ENGY_EN == 1)
    if(TRUE == valFlg)
    {
        for(uni = 0; uni < 2; uni++)
        {
            LIB_MemCpy((INT8U *) &(pData->TariffEngy[0].ComS[uni]), (INT8U *) &TotalComS[uni], sizeof(ST_ENERGY_FORM));
            EngyDiv = TotalComS[uni] - TariffSumComS[uni];
            if(LIB_ABS(EngyDiv) > ENGY_DIFF_LMT)
            {
                valFlg = FALSE;
                break;
            }
        }
    }
#endif
    return valFlg;
#else
    return TRUE;
#endif
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       LoadEngyData"*/
/*"  Description:    装载正确的电量数据、包括RAM区和内卡中的数据比对"*/
/*"  Calls:          "*/
/*"  Called By:      电量容错处理（初始化、电量更新前）"*/
/*"  Input:          flag: 1 上电过程中数据装载，0 备份过程中数据正确性判断"*/
/*"                  mode =MCU_RUNMODE运行模式，或低功耗模式"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void LoadEngyData(INT8U flag, INT8U mode)
{
    INT8U ret;
    UN_BYTE_DEF RamEngyValidFlg;/*"0 valid  1 unvalid  b0  stNowEngy  b1 stNowEngyBak"*/
    INT8U EepEngyValidFlg;
    INT8U EngyRenewFlg; /*"5a  stNowEngy to stNowEngyBak  a5  stNowEngyBak to stNowEngy"*/

    RamEngyValidFlg.byte = 0;
    EngyRenewFlg = 0;

    if(TRUE == NowEngyDataChk(&stNowEngy))
    {
        RamEngyValidFlg.bit.b0 = TRUE;
    }
    else
    {
        if(TRUE == NowEngyDataChk(&stPdSaveData.pdNowEngy))
        {
            LIB_MemCpy((INT8U *) &stPdSaveData.pdNowEngy, (INT8U *) &stNowEngy, sizeof(ST_RAM_ENGY_DATA));
            RamEngyValidFlg.bit.b0 = TRUE;
        }
        else
        {
            if(TRUE == flag)/*仅上电时用掉电区内卡的数据，操作过程的数据校验不使用掉电区内卡数据*/
            {
                INT8U i;

                for(i = 0; i < MAX_READ_FILE_LOOP; i++)
                {
                    ret = FS_ReadFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT + FIND(ST_LRM_DATA, pdNowEngy),
                                       (INT8U *)&stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));/*"读出存储区"*/
                    if ((ret == TRUE) && (TRUE == NowEngyDataChk(&stPdSaveData.pdNowEngy)))
                    {
                        LIB_MemCpy((INT8U *) &stPdSaveData.pdNowEngy, (INT8U *) &stNowEngy, sizeof(ST_RAM_ENGY_DATA));
                        RamEngyValidFlg.bit.b0 = TRUE;
                        break;
                    }
                    MCU_DelayMs(10);
                }
            }
        }
    }
    if(BakEngyDataChk(&stNowEngyBak))
    {
        RamEngyValidFlg.bit.b1 = TRUE;
    }
    else
    {
        EepEngyValidFlg = GetEepEngy();
    }
    switch(RamEngyValidFlg.byte & 0x03)
    {
    case 0x03:
        EngyRenewFlg = 0x5a;
        break;
    case 0x02:
        EngyRenewFlg = 0xa5;
        break;
    case 0x01:
        if(FALSE != EepEngyValidFlg)
        {
#if (TARIFF_NUM >= 1)
            /*"费率没有发生改变，且总和分费率电量差合法"*/
            if((stNowEngy.tariffNow == stNowEngyBak.tariffBak) && (TRUE == TariffEngyDivOk()))
            {
                EngyRenewFlg = 0x5a;
            }
            else/*"费率有改变或总分费率电量差超限，不能单纯用当前电量备份到分费率电量，有可能造成总和分费率电量不一致，只能恢复到历史电量"*/
            {
                EngyRenewFlg = 0xa5;
            }
#else
            EngyRenewFlg = 0x5a;
#endif
        }
        else /*"stNowEngy ok but all stNowEngyBak err ,the tariff energy can't recover"*/
        {
            //todo log energy err
        }
        break;
    default:
        if(FALSE != EepEngyValidFlg)
        {
            EngyRenewFlg = 0xa5;
        }
        else
        {
            //todo log energy err
        }
        break;
    }

    if(0x5a == EngyRenewFlg)
    {        
        if(TRUE == flag)
        {
            RenewTariffData();/*上电过程在RAM合法时，更新当前费率数据*/
            if(MCU_RUNMODE == mode)
            {
                EngySave(ENGY_ADDR1 | ENGY_ADDR2);
            }
        }
    }
    else if(0xa5 == EngyRenewFlg)
    {
        MCU_PwrChkCloseINT();
        LIB_MemCpy((INT8U *) &stNowEngyBak.TariffEngy[0], (INT8U *) &stNowEngy.TotalEngy, sizeof(ST_BASE_ENGY));
#if (TARIFF_NUM >= 1)
        LIB_MemCpy((INT8U *) &stNowEngyBak.TariffEngy[stNowEngyBak.tariffBak], (INT8U *) &stNowEngy.CurTEngy, sizeof(ST_BASE_ENGY));
        stNowEngy.tariffNow = stNowEngyBak.tariffBak;
#endif
        stNowEngy.ValidFlag = 0x5a5a;
#if (PHASE_ENGY_EN == 1)
        LIB_MemCpy((INT8U *) &stNowEngyBak.Phase, (INT8U *) &stNowEngy.Phase, 3 * sizeof(ST_BASE_ENGY));
#endif
        stNowEngy.Crc = CRC16((INT8U *)&stNowEngy, FIND(ST_RAM_ENGY_DATA,Crc));
        LIB_MemCpy((INT8U *) &stNowEngy, (INT8U *) &stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));
        MCU_PwrChkOpenINT();
    }
}

#if (TARIFF_NUM >= 1)
/*"*****************************************************************************"*/
/*"  Function:       TariffEngyDivOk"*/
/*"  Description:    当前总电量、当前费率电量与备份总电量、分费率电量差值合法性检查"*/
/*"  Calls:          "*/
/*"  Called By:      电量容错处理（初始化、电量更新前）"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         TRUE 合法  FALSE 非法 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define   TARIFF_DIV_MAX_LIMIT   (10)
static INT8U TariffEngyDivOk(void)
{
    INT8U rsult;
    INT32S EngyDiv[2];
    INT8U uni;

    ENGY_TYPE_U NowEngy[2];/*"now & nowtariff"*/
    ENGY_TYPE_U BakEngy[2];/*"now & nowtariff"*/

    rsult = TRUE;
    LIB_MemSet(0, (INT8U *)&NowEngy, sizeof(NowEngy));
    LIB_MemSet(0, (INT8U *)&BakEngy, sizeof(BakEngy));
    for(uni = 0; uni < 2; uni++)
    {
        LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.ComP[uni]), (INT8U *) &NowEngy[0], sizeof(ST_ENERGY_FORM));
        LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.ComP[uni]), (INT8U *) &NowEngy[1], sizeof(ST_ENERGY_FORM));
        LIB_MemCpy((INT8U *)&(stNowEngyBak.TariffEngy[0].ComP[uni]), (INT8U *) &BakEngy[0], sizeof(ST_ENERGY_FORM));
        LIB_MemCpy( (INT8U *)&(stNowEngyBak.TariffEngy[stNowEngy.tariffNow].ComP[uni]), (INT8U *) &BakEngy[1], sizeof(ST_ENERGY_FORM));
        EngyDiv[0] = (NowEngy[0] - BakEngy[0]);
        EngyDiv[1] = (NowEngy[1] - BakEngy[1]);
        if((LIB_ABS(EngyDiv[0] - EngyDiv[1]) > TARIFF_DIV_MAX_LIMIT))
        {
            rsult = FALSE;
            break;
        }
    }
#if (QUAD_ACTIVE_ENGY_EN == 1)
    if(TRUE == rsult)
    {
        LIB_MemSet(0, (INT8U *) &NowEngy, sizeof(NowEngy));
        LIB_MemSet(0, (INT8U *) &BakEngy, sizeof(BakEngy));
        for(uni = 0; uni < 4; uni++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.P[uni]), (INT8U *)&NowEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.P[uni]), (INT8U *)&NowEngy[1], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngyBak.TariffEngy[0].P[uni]), (INT8U *)&BakEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy( (INT8U *)&(stNowEngyBak.TariffEngy[stNowEngy.tariffNow].P[uni]), (INT8U *)&BakEngy[1], sizeof(ST_ENERGY_FORM));
            EngyDiv[0] = (NowEngy[0] - BakEngy[0]);
            EngyDiv[1] = (NowEngy[1] - BakEngy[1]);
            if((LIB_ABS(EngyDiv[0] - EngyDiv[1]) > TARIFF_DIV_MAX_LIMIT))
            {
                rsult = FALSE;
                break;
            }
        }
    }
#endif
#if (REACTIVE_ENGY_EN == 1)
    if(TRUE == rsult)
    {
        LIB_MemSet(0, (INT8U *) &NowEngy, sizeof(NowEngy));
        LIB_MemSet(0, (INT8U *) &BakEngy, sizeof(BakEngy));
        for(uni = 0; uni < 4; uni++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.Q[uni]), (INT8U *)&NowEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.Q[uni]), (INT8U *)&NowEngy[1], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngyBak.TariffEngy[0].Q[uni]), (INT8U *)&BakEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy( (INT8U *)&(stNowEngyBak.TariffEngy[stNowEngy.tariffNow].Q[uni]), (INT8U *)&BakEngy[1], sizeof(ST_ENERGY_FORM));
            EngyDiv[0] = (NowEngy[0] - BakEngy[0]);
            EngyDiv[1] = (NowEngy[1] - BakEngy[1]);
            if((LIB_ABS(EngyDiv[0] - EngyDiv[1]) > TARIFF_DIV_MAX_LIMIT))
            {
                rsult = FALSE;
                break;
            }
        }
    }
#ifdef  REACTIVE_ENGY_COM
    if(TRUE == rsult)
    {
        for(uni = 0; uni < 2; uni++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.ComQ[uni]), (INT8U *)&NowEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.ComQ[uni]), (INT8U *)&NowEngy[1], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngyBak.TariffEngy[0].ComQ[uni]), (INT8U *)&BakEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy( (INT8U *)&(stNowEngyBak.TariffEngy[stNowEngy.tariffNow].ComQ[uni]), (INT8U *)&BakEngy[1], sizeof(ST_ENERGY_FORM));
            EngyDiv[0] = (NowEngy[0] - BakEngy[0]);
            EngyDiv[1] = (NowEngy[1] - BakEngy[1]);
            if((LIB_ABS(EngyDiv[0] - EngyDiv[1]) > TARIFF_DIV_MAX_LIMIT))
            {
                rsult = FALSE;
                break;
            }
        }
    }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    if(TRUE == rsult)
    {
        LIB_MemSet(0, (INT8U *) &NowEngy, sizeof(NowEngy));
        LIB_MemSet(0, (INT8U *) &BakEngy, sizeof(BakEngy));
        for(uni = 0; uni < 2; uni++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.ComS[uni]), (INT8U *)&NowEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.ComS[uni]), (INT8U *)&NowEngy[1], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngyBak.TariffEngy[0].ComS[uni]), (INT8U *)&BakEngy[0], sizeof(ST_ENERGY_FORM));
            LIB_MemCpy((INT8U *)&(stNowEngyBak.TariffEngy[stNowEngy.tariffNow].ComS[uni]), (INT8U *)&BakEngy[1], sizeof(ST_ENERGY_FORM));
            EngyDiv[0] = (NowEngy[0] - BakEngy[0]);
            EngyDiv[1] = (NowEngy[1] - BakEngy[1]);
            if((LIB_ABS(EngyDiv[0] - EngyDiv[1]) > TARIFF_DIV_MAX_LIMIT))
            {
                rsult = FALSE;
                break;
            }
        }
    }
#endif
    return (rsult);
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       EngyTailChk"*/
/*"  Description:    电量尾数有效性检测"*/
/*"  Calls:          "*/
/*"  Called By:      上电初始化"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define  ENGY_TAIL_LIMIT   MWS_NUM_01KWH
static void EngyTailChkPwrUp(void)
{
    INT8U ret;
    INT8U rsult;
    INT8U i;

    if(FALSE == LIB_ChkDataValid((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL, Crc), stPdSaveData.stEngyTail.Crc))
    {
        rsult = FALSE;
        for (i = 0; i < MAX_READ_FILE_LOOP; i++)
        {
            ret = FS_ReadFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT + FIND(ST_LRM_DATA, stEngyTail),
                                      (INT8U *)&stPdSaveData.stEngyTail, sizeof(ST_ENGY_TAIL));/*"读出存储区"*/
            if((TRUE == ret)
               && (TRUE == LIB_ChkDataValid((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL, Crc), stPdSaveData.stEngyTail.Crc)))
            {
                rsult = TRUE;
                break;
            }
            MCU_DelayMs(10);
        }
    }
    else
    {
        rsult = TRUE;
    }
    if(TRUE == rsult)//todo check every energy over limit
    {
        INT8U num;
        INT8U i;
        INT32U *pTailEngytmp;

        num = (FIND(ST_ENGY_TAIL, Crc) - FIND(ST_ENGY_TAIL, TotalTail)) >> 2;
        pTailEngytmp = (INT32U *)&stPdSaveData.stEngyTail.TotalTail;
        for(i = 0; i < num; i++)
        {
            if(*pTailEngytmp > ENGY_TAIL_LIMIT)
            {
                rsult = FALSE;
                break;
            }
            pTailEngytmp++;
        }
    }

    if(FALSE == rsult)
    {
        LIB_MemSet(0, (INT8U *) &stPdSaveData.stEngyTail, sizeof(ST_ENGY_TAIL));
        stPdSaveData.stEngyTail.ValidFlag = 0x5A5A;
        stPdSaveData.stEngyTail.Crc = CRC16((INT8U *) &stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
    }
}
/*"*****************************************************************************"*/
/*"  Function:       RamEngyTailChk"*/
/*"  Description:    RAM中电量尾数有效性检测"*/
/*"  Calls:          "*/
/*"  Called By:      上电初始化，电量尾数累加前"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RamEngyTailChk(void)
{
    if(FALSE == LIB_ChkDataValid((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL, Crc), stPdSaveData.stEngyTail.Crc))
    {
        LIB_MemSet(0, (INT8U *)&stPdSaveData.stEngyTail, sizeof(ST_ENGY_TAIL));
        stPdSaveData.stEngyTail.ValidFlag = 0x5A5A;
        stPdSaveData.stEngyTail.Crc = CRC16((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
    }
    else
    {
    }
}
typedef struct
{
#if (QUAD_ACTIVE_ENGY_EN == 1)
    ENGY_TYPE_U P[4]; /*"四象限有功电量"*/
#endif
    ENGY_TYPE_U ComP[2]; /*"正、反向有功电量"*/
#if (REACTIVE_ENGY_EN == 1)
    ENGY_TYPE_U Q[4]; /*"四象限无功电量"*/
#ifdef REACTIVE_ENGY_COM
    ENGY_TYPE_U ComQ[2]; /*"正、反向无功电量"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    ENGY_TYPE_U ComS[2]; /*"正反视在电量"*/
#endif
} ST_BASE_CAL_ENGY;

//extern void Lrm_test(void);
extern INT8U get_bm_status(INT8U *buf);




/*"*****************************************************************************"*/
/*"  Function:       Engyadd"*/
/*"  Description:    电量尾数进位，电量累加"*/
/*"  Calls:          "*/
/*"  Called By:      主循环"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:          "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void Engyadd(void)
{
    INT8U i;
#if (PHASE_ENGY_EN == 1)
    INT8U j;
#endif
    ST_BASE_CAL_ENGY tmpTotalEngy;/*"总电量累加临时变量，根据字节数调整为INT32U或INT64U，方便计算"*/
#if (TARIFF_NUM >= 1)
    ST_BASE_CAL_ENGY tmpTariffEngy;/*"当前费率电量累加临时变量，根据字节数调整为INT32U或INT64U，方便计算"*/
#endif
#if (PHASE_ENGY_EN == 1)
    ST_BASE_CAL_ENGY tmpPhaseEngy[3];/*"分相电量累加临时变量，根据字节数调整为INT32U或INT64U，方便计算"*/
#endif
    INT8U EnergyRenewFlg;/*"电量数据刷新标志，电量有刷新，需重新计算校验和"*/
    INT8U TariffRenewFlg;/*"当前费率有变化，需备份分费率电量数据，重新计算校验和"*/
    ENGY_TYPE_U *pTariffEngy;

    pTariffEngy = NULL;
    RamEngyTailChk();
    EnergyRenewFlg = 0;
    TariffRenewFlg = 0;
    LIB_MemSet(0, (INT8U *) &tmpTotalEngy, sizeof(ST_BASE_CAL_ENGY));
#if (TARIFF_NUM >= 1)
    LIB_MemSet(0, (INT8U *) &tmpTariffEngy, sizeof(ST_BASE_CAL_ENGY));
#endif
#if (PHASE_ENGY_EN == 1)
    LIB_MemSet(0, (INT8U *) &tmpPhaseEngy, sizeof(ST_BASE_CAL_ENGY));
#endif

#if (TARIFF_NUM >= 1)
    /*"当前费率号合法性检查"*/
    if((stNowEngy.tariffNow == 0) || (stNowEngy.tariffNow > TARIFF_NUM))
    {
        stNowEngy.tariffNow = 1;
        TariffRenewFlg = 0x5a;
    }
#endif

    /*"将stNowEngy中电量拷贝到临时电量变量中，进行电量累加运算"*/
    for(i = 0; i < 2; i++)
    {
        LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.ComP[i]), (INT8U *)&tmpTotalEngy.ComP[i], sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
        LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.ComP[i]), (INT8U *)&tmpTariffEngy.ComP[i], sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.Phase[j].ComP[i].data), (INT8U *)&tmpPhaseEngy[j].ComP[i], sizeof(ST_ENERGY_FORM));
        }
#endif
    }
#if (QUAD_ACTIVE_ENGY_EN == 1)
    for(i = 0; i < 4; i++)
    {
        LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.P[i]), (INT8U *)&tmpTotalEngy.P[i], sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
        LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.P[i]), (INT8U *)&tmpTariffEngy.P[i], sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.Phase[j].P[i].data), (INT8U *)&tmpPhaseEngy[j].P[i], sizeof(ST_ENERGY_FORM));
        }
#endif
    }
#endif
#if (REACTIVE_ENGY_EN == 1)
    for(i = 0; i < 4; i++)
    {
        LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.Q[i]), (INT8U *)&tmpTotalEngy.Q[i], sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
        LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.Q[i]), (INT8U *)&tmpTariffEngy.Q[i], sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.Phase[j].Q[i]), (INT8U *)&tmpPhaseEngy[j].Q[i], sizeof(ST_ENERGY_FORM));
        }
#endif
    }
#ifdef REACTIVE_ENGY_COM
    for(i = 0; i < 2; i++)
    {
        LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.ComQ[i]), (INT8U *)&tmpTotalEngy.ComQ[i], sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
        LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.ComQ[i]), (INT8U *)&tmpTariffEngy.ComQ[i], sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.Phase[j].ComQ[i]), (INT8U *)&tmpPhaseEngy[j].ComQ[i], sizeof(ST_ENERGY_FORM));
        }
#endif
    }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    for(i = 0; i < 2; i++)
    {
        LIB_MemCpy((INT8U *)&(stNowEngy.TotalEngy.ComS[i]), (INT8U *)&tmpTotalEngy.ComS[i], sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
        LIB_MemCpy((INT8U *)&(stNowEngy.CurTEngy.ComS[i]), (INT8U *)&tmpTariffEngy.ComS[i], sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            LIB_MemCpy((INT8U *)&(stNowEngy.Phase[j].ComS[i]), (INT8U *)&tmpPhaseEngy[j].ComS[i], sizeof(ST_ENERGY_FORM));
        }
#endif
    }
#endif

    MCU_PwrChkCloseINT();
#if (QUAD_ACTIVE_ENGY_EN == 1)
    for(i = 0; i < 4; i++)
    {
        /*"四象限有功电量累加"*/
        if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.TotalTail.P[i], &tmpTotalEngy.P[i], &tmpTariffEngy.P[i]))
        {
            EnergyRenewFlg = 0x5a;
        }

#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.PhaseTail[j].P[i], &tmpPhaseEngy[j].P[i], NULL))
            {
                EnergyRenewFlg = 0x5a;
            }
        }
#endif/*" end #if (PHASE_ENGY_EN == 1)"*/
    }
#endif
    for(i = 0; i < 2; i++)
    {
#if (TARIFF_NUM >= 1)
        /*"正反有功电量累加"*/
        if(TRUE == ChkEngyRegActive(E_PA_ENGY_T1 + (TARIFF_NUM * i) + (stNowEngy.tariffNow - 1)))
        {
            pTariffEngy = &tmpTariffEngy.ComP[i];
        }
        else
        {
            pTariffEngy = NULL;
        }
#else
        pTariffEngy = NULL;
#endif
        if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.TotalTail.ComP[i], &tmpTotalEngy.ComP[i], pTariffEngy))
        {
            EnergyRenewFlg = 0x5a;
        }

#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.PhaseTail[j].ComP[i], &tmpPhaseEngy[j].ComP[i], NULL))
            {
                EnergyRenewFlg = 0x5a;
            }
        }
#endif/*" end #if (PHASE_ENGY_EN == 1)"*/
    }

#if (REACTIVE_ENGY_EN == 1)
    for(i = 0; i < 4; i++)
    {
#if (TARIFF_NUM >= 1)
        if(TRUE == ChkEngyRegActive(E_R_ENGY_I_T1 + (TARIFF_NUM * i) + (stNowEngy.tariffNow - 1)))
        {
            pTariffEngy = &tmpTariffEngy.Q[i];
        }
        else
        {
            pTariffEngy = NULL;
        }
#else
        pTariffEngy = NULL;
#endif
        if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.TotalTail.Q[i], &tmpTotalEngy.Q[i], pTariffEngy))
        {
            EnergyRenewFlg = 0x5a;
        }
#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.PhaseTail[j].Q[i], &tmpPhaseEngy[j].Q[i], NULL))
            {
                EnergyRenewFlg = 0x5a;
            }
        }
#endif/*" end #if (PHASE_ENGY_EN == 1)"*/
    }
#ifdef REACTIVE_ENGY_COM
    for(i = 0; i < 2; i++)
    {
#if (TARIFF_NUM >= 1)        
        if(TRUE == ChkEngyRegActive(E_PR_ENGY_T1 + (TARIFF_NUM * i) + (stNowEngy.tariffNow - 1)))
        {
            pTariffEngy = &tmpTariffEngy.ComQ[i];
        }
        else
        {
            pTariffEngy = NULL;
        }
#else
        pTariffEngy = NULL;
#endif
        if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.TotalTail.ComQ[i], &tmpTotalEngy.ComQ[i], pTariffEngy))
        {
            EnergyRenewFlg = 0x5a;
        }
    #if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.PhaseTail[j].ComQ[i], &tmpPhaseEngy[j].ComQ[i], NULL))
            {
                EnergyRenewFlg = 0x5a;
            }
        }
    #endif/*" end #if (PHASE_ENGY_EN == 1)"*/
    }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    for(i = 0; i < 2; i++)/*"正、反向视在电量累计"*/
    {
        if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.TotalTail.ComS[i], &tmpTotalEngy.ComS[i], &tmpTariffEngy.ComS[i]))
        {
            EnergyRenewFlg = 0x5a;
        }

#if (PHASE_ENGY_EN == 1)
        for(j = 0; j < 3; j++)
        {
            if(TRUE == EngyCarryChk(&stPdSaveData.stEngyTail.PhaseTail[j].ComS[i], &tmpPhaseEngy[j].ComS[i], NULL))
            {
                EnergyRenewFlg = 0x5a;
            }
        }
#endif/*" end #if (PHASE_ENGY_EN == 1)"*/
    }
#endif

    /*"电量累加运算完成后,电量数据有更新，将临时电量变量中拷贝到stNowEngy电量中"*/
    if(0x5a == EnergyRenewFlg)
    {
#if (QUAD_ACTIVE_ENGY_EN == 1)
        for(i = 0; i < 4; i++)
        {
            LIB_MemCpy((INT8U *) &tmpTotalEngy.P[i], (INT8U *) &(stNowEngy.TotalEngy.P[i]), sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
            LIB_MemCpy((INT8U *) &tmpTariffEngy.P[i], (INT8U *) &(stNowEngy.CurTEngy.P[i]), sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
            for(j = 0; j < 3; j++)
            {
                LIB_MemCpy((INT8U *) &tmpPhaseEngy[j].P[i], (INT8U *) &(stNowEngy.Phase[j].P[i]), sizeof(ST_ENERGY_FORM));
            }
#endif
        }
#endif
        for(i = 0; i < 2; i++)
        {
            LIB_MemCpy((INT8U *) &tmpTotalEngy.ComP[i], (INT8U *) &(stNowEngy.TotalEngy.ComP[i]), sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
            LIB_MemCpy((INT8U *) &tmpTariffEngy.ComP[i], (INT8U *) &(stNowEngy.CurTEngy.ComP[i]), sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
            for(j = 0; j < 3; j++)
            {
                LIB_MemCpy((INT8U *) &tmpPhaseEngy[j].ComP[i], (INT8U *) &(stNowEngy.Phase[j].ComP[i]), sizeof(ST_ENERGY_FORM));
            }
#endif
        }
#if (REACTIVE_ENGY_EN == 1)
        for(i = 0; i < 4; i++)
        {
            LIB_MemCpy((INT8U *) &tmpTotalEngy.Q[i], (INT8U *) &(stNowEngy.TotalEngy.Q[i]), sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
            LIB_MemCpy((INT8U *) &tmpTariffEngy.Q[i], (INT8U *) &(stNowEngy.CurTEngy.Q[i]), sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
            for(j = 0; j < 3; j++)
            {
                LIB_MemCpy((INT8U *) &tmpPhaseEngy[j].Q[i], (INT8U *) &(stNowEngy.Phase[j].Q[i]), sizeof(ST_ENERGY_FORM));
            }
#endif
        }
#ifdef REACTIVE_ENGY_COM
        for(i = 0; i < 2; i++)
        {
            LIB_MemCpy((INT8U *) &tmpTotalEngy.ComQ[i], (INT8U *) &(stNowEngy.TotalEngy.ComQ[i]), sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
            LIB_MemCpy((INT8U *) &tmpTariffEngy.ComQ[i], (INT8U *) &(stNowEngy.CurTEngy.ComQ[i]), sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
            for(j = 0; j < 3; j++)
            {
                LIB_MemCpy((INT8U *) &tmpPhaseEngy[j].ComQ[i], (INT8U *) &(stNowEngy.Phase[j].ComQ[i]), sizeof(ST_ENERGY_FORM));
            }
#endif
        }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
        for(i = 0; i < 2; i++)
        {
            LIB_MemCpy((INT8U *) &tmpTotalEngy.ComS[i], (INT8U *) &(stNowEngy.TotalEngy.ComS[i]), sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM >= 1)
            LIB_MemCpy((INT8U *) &tmpTariffEngy.ComS[i], (INT8U *) &(stNowEngy.CurTEngy.ComS[i]), sizeof(ST_ENERGY_FORM));
#endif
#if (PHASE_ENGY_EN == 1)
            for(j = 0; j < 3; j++)
            {
                LIB_MemCpy((INT8U *) &tmpPhaseEngy[j].ComS[i], (INT8U *) &(stNowEngy.Phase[j].ComS[i]), sizeof(ST_ENERGY_FORM));
            }
#endif
        }
#endif
        stNowEngy.Crc = CRC16((INT8U *)&stNowEngy, FIND(ST_RAM_ENGY_DATA, Crc));
        /*"备份当前电量掉掉电存数区RAM区"*/
        LIB_MemCpy((INT8U *)&stNowEngy, (INT8U *)&stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));

        stPdSaveData.stEngyTail.Crc = CRC16((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
    /*"当前电量数据同步到备份分时电量数据中 stNowEngy -> stNowEngyBak"*/
        RenewTariffData();
    }
    if(TariffRenewFlg == 0x5a)
    {
        stNowEngy.Crc = CRC16((INT8U *)&stNowEngy, FIND(ST_RAM_ENGY_DATA, Crc));
    }
    MCU_PwrChkOpenINT();

#ifdef  AMP_HOUR_EN
    for(i = 0; i < MU_ELEMENT_NUM; i++)
    {
        while(AmpHourTail[i] >= ANHOUR_VALUE_01ANH)
        {
            AmpHourTail[i] -= ANHOUR_VALUE_01ANH;
            AmpHour[i]++;
        }
    }
#endif
}
/*"每秒电量数据缓存结构体"*/
typedef struct
{
    INT32U EngyP;        /*"总有功电量"*/
#if (PHASE_ENGY_EN == 1)
    INT32U PhaseEngyP[3];/*"分相有功电量"*/
#endif
#if (REACTIVE_ENGY_EN == 1)
    INT32U EngyQ;        /*"总无功电量"*/
#if (PHASE_ENGY_EN == 1)
    INT32U PhaseEngyQ[3];/*"分相无功电量"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    INT32U EngyS;        /*"总视在电量"*/
#if (PHASE_ENGY_EN == 1)
    INT32U PhaseEngyS[3];/*"分相视在电量"*/
#endif
#endif
#if (PHASE_ENGY_EN == 1)
    INT8U PhaseQuad[3];/*"当前分相象限值"*/
#endif
} ST_SECOND_ENGY;

/*"获取每秒电量对象标识列表，列表顺序需与ST_SECOND_ENGY结构体定义顺序保持一致"*/
/*"-------象限数据不在列表中定义----------"*/
const INT16U  SecEngyList[] =
        {
#ifdef SINGLE_PHASE
                L_ENGY_P,
#else
                T_ENGY_P,
#endif
#if (PHASE_ENGY_EN == 1)
                A_ENGY_P,
                B_ENGY_P,
                C_ENGY_P,
#endif
#if (REACTIVE_ENGY_EN == 1)
#ifdef SINGLE_PHASE
                L_ENGY_Q,
#else
                T_ENGY_Q,
#endif
#if (PHASE_ENGY_EN == 1)
                A_ENGY_Q,
                B_ENGY_Q,
                C_ENGY_Q,
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
                T_ENGY_S,
#if (PHASE_ENGY_EN == 1)
                A_ENGY_S,
                B_ENGY_S,
                C_ENGY_S,
#endif
#endif
        };
#define  ENGY_NUM   (sizeof(SecEngyList)/2)

/*"*****************************************************************************"*/
/*"  Function:       EngyTailAdd"*/
/*"  Description:    电量尾数累加"*/
/*"  Calls:          "*/
/*"  Called By:      主循环秒任务"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:          "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void EngyTailAdd(void)
{
    ST_SECOND_ENGY EngergyTemp;
#ifndef EU_TEST
    INT8U i;
    INT32U *pEngy;
#endif

    RamEngyTailChk();
    MCU_PwrChkCloseINT();
#ifdef EU_TEST
    EngergyTemp.EngyP = 36000;
    EngergyTemp.EngyQ = 36000;
    if(0 == stMmtData.Quad[0])
    {
        stMmtData.Quad[0] = 1;
    }
    else
    {
        stMmtData.Quad[0] = 0;
    }
#else
    pEngy = (INT32U *)&EngergyTemp;
    for(i = 0; i < ENGY_NUM; i++)
    {
        MU_EngyGet(SecEngyList[i], pEngy);
        pEngy++;
    }
#endif
#if (QUAD_ACTIVE_ENGY_EN == 1)
    stPdSaveData.stEngyTail.TotalTail.P[stMmtData.Quad[0]] += EngergyTemp.EngyP;
#else
    if ((stMmtData.PowerDir & 0x80) == 0)/*"有功电量累加到正向有功电量单元"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComP[0] += EngergyTemp.EngyP;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComP[1] += EngergyTemp.EngyP;
    }
#endif
#if (REACTIVE_ENGY_EN == 1)
    stPdSaveData.stEngyTail.TotalTail.Q[stMmtData.Quad[0]] += EngergyTemp.EngyQ;
#ifdef REACTIVE_ENGY_COM
    if ((stMmtData.PowerDir & 0x08) == 0)/*"无功电量累加到正向无功电量单元"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComQ[0] += EngergyTemp.EngyQ;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComQ[1] += EngergyTemp.EngyQ;
    }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    if((stMmtData.PowerDir & 0x80) == 0)/*"总有功方向为正，视在电量累加到正向视在电量单元"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[0] += EngergyTemp.EngyS;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[1] += EngergyTemp.EngyS;
    }
#endif

/*"--------分相电量尾数数据累加--------------"*/
#if (PHASE_ENGY_EN == 1)
    for(i = 0; i < 3; i++)
    {
#if (QUAD_ACTIVE_ENGY_EN == 1)
        stPdSaveData.stEngyTail.PhaseTail[i].P[stMmtData.Quad[i + 1]] += EngergyTemp.PhaseEngyP[i];
#else
        if((stMmtData.Quad[i + 1] == 0) || (stMmtData.Quad[i + 1] == 3))/*"1/4象限，有功电量累加到正向有功电量单元"*/
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComP[0] += EngergyTemp.PhaseEngyP[i];
        }
        else
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComP[1] += EngergyTemp.PhaseEngyP[i];
        }
#endif
#if (REACTIVE_ENGY_EN == 1)
        stPdSaveData.stEngyTail.PhaseTail[i].Q[stMmtData.Quad[i + 1]] += EngergyTemp.PhaseEngyQ[i];
#ifdef REACTIVE_ENGY_COM
        if((stMmtData.Quad[i + 1] == 0) || (stMmtData.Quad[i + 1] == 1))/*"1/2象限，无功电量累加到正向无功电量单元"*/
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComQ[0] += EngergyTemp.PhaseEngyQ[i];
        }
        else
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComQ[1] += EngergyTemp.PhaseEngyQ[i];
        }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
        if((stMmtData.Quad[i + 1] == 0) || (stMmtData.Quad[i + 1] == 3))/*"1/4象限，视在电量累加到正向视在电量单元"*/
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComS[0] += EngergyTemp.PhaseEngyS[i];
        }
        else
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComS[1] += EngergyTemp.PhaseEngyS[i];
        }
#endif
    }
#endif
    stPdSaveData.stEngyTail.Crc = CRC16((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
    MCU_PwrChkOpenINT();

#ifdef  AMP_HOUR_EN
    for(i = 0; i < MU_ELEMENT_NUM; i++)
    {
        AmpHourTail[i] += LIB_ABS(stMmtData.curr[i]);
    }
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       PdDateCrcBrkFlg"*/
/*"  Description:    掉电区CRC破坏"*/
/*"  Calls:          "*/
/*"  Called By:      主循环or上电电量初始化处理完成"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:          "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void PdDateCrcBrkFlg(void)
{
    CRC_DATA_TP crc;
    INT8U i;
    CRC_DATA_TP crcTmp;

    crcTmp = 0;
    if (mPdDateCrcBrkFlg != VALIDFLAG)
    {
        mPdDateCrcBrkFlg = VALIDFLAG;
        crc = stPdSaveData.pdNowEngy.Crc + 0x1234; /*"写乱掉电区的CRC"*/
        for (i = 0; i < MAX_READ_FILE_LOOP; i++)
        {
            FS_WriteFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT + FIND(ST_LRM_DATA, pdNowEngy)
                         + FIND(ST_RAM_ENGY_DATA, Crc), (INT8U *) &crc, sizeof(CRC_DATA_TP));
            FS_ReadFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT + FIND(ST_LRM_DATA, pdNowEngy)
                         + FIND(ST_RAM_ENGY_DATA, Crc), (INT8U *) &crcTmp, sizeof(CRC_DATA_TP));
            if (crcTmp == crc)
            {
                break;
            }
            else
            {
                MCU_DelayMs(10);
            }
        }

        crc = stPdSaveData.stEngyTail.Crc + 0x1234; /*"写乱掉电区电量尾数的CRC"*/
        FS_WriteFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT + FIND(ST_LRM_DATA, stEngyTail)
                                 + FIND(ST_ENGY_TAIL, Crc), (INT8U *) &crc, sizeof(CRC_DATA_TP));
    }
}

//#define ENERGY_ROLL_OVER 0
#if defined(ENERGY_ROLL_OVER)
void EU_UseEnergySet(INT32U data)
{
    MCU_PwrChkCloseINT();
    LIB_MemSet(0, (INT8U *)&stNowEngy.TotalEngy, sizeof(ST_BASE_ENGY));
#if (TARIFF_NUM >= 1)
    LIB_MemSet(0, (INT8U *)&stNowEngy.CurTEngy, sizeof(ST_BASE_ENGY));
#endif
    stNowEngy.TotalEngy.P[0] = data; /*电量累计*/
    stNowEngy.TotalEngy.S[0] = data; /*电量累计*/
#if (TARIFF_NUM >= 1)
    stNowEngy.CurTEngy.P[0] = data; /*分时总电量累计*/
    stNowEngy.CurTEngy.S[0] = data; /*分时总电量累计*/
#endif
#if (PHASE_ENGY_EN == 1)
    LIB_MemCpy((INT8U *)&stNowEngy.TotalEngy, (INT8U *)&stNowEngy.Phase[0], sizeof(ST_BASE_ENGY));
    LIB_MemSet(0, (INT8U *)&stNowEngy.Phase[1], sizeof(ST_BASE_ENGY));
    LIB_MemSet(0, (INT8U *)&stNowEngy.Phase[2], sizeof(ST_BASE_ENGY));
#endif
    LIB_MemSet(0, (INT8U *) &stPdSaveData.stEngyTail.TotalTail, sizeof(ST_BASE_ENGY_Tail));
    LIB_MemSet(0, (INT8U *) &stPdSaveData.stEngyTail.PhaseTail, sizeof(ST_BASE_ENGY_Tail));
    LIB_MemSet(0, (INT8U *) &stNowEngyBak, sizeof(ST_BAKUP_ENGY_DATA));
    LIB_MemCpy((INT8U *) &stNowEngy.tariffNow, (INT8U *) &stNowEngyBak.TariffEngy[1], sizeof(ST_BASE_ENGY));
    LIB_MemCpy((INT8U *) &stNowEngy.TotalEngy, (INT8U *) &stNowEngyBak.TariffEngy[0], sizeof(ST_BASE_ENGY));
    LIB_MemSet(VALIDFLAG, (INT8U *) stNowEngyBak.ValidFlag,sizeof(stNowEngyBak.ValidFlag));
    stNowEngyBak.Crc = CRC16((INT8U *) &stNowEngyBak, FIND(ST_BAKUP_ENGY_DATA,Crc));

    EngySave(ENGY_ADDR1 | ENGY_ADDR2);
    MCU_PwrChkOpenINT();
}

void Energy_Roll_Over(void)
{
#if 0
    if (( stNowEngyMny.Total.T.P[0] +stNowEngyMny.Total.T.P[1]+stNowEngyMny.Total.T.P[2]+stNowEngyMny.Total.T.P[3]>999999999)) //每一度电存储电量
    {
        stNowEngyMny.Total.T.P[0]= 0; /*电量累计*/
        stNowEngyMny.CurData.T.P[0] = 0; /*分时总电量累计*/
        stNowEngyMny.Total.T.P[1]= 0; /*电量累计*/
        stNowEngyMny.CurData.T.P[1] = 0; /*分时总电量累计*/
        stNowEngyMny.Total.T.P[2]= 0; /*电量累计*/
        stNowEngyMny.CurData.T.P[2] = 0; /*分时总电量累计*/
        stNowEngyMny.Total.T.P[3]= 0; /*电量累计*/
        stNowEngyMny.CurData.T.P[3] = 0; /*分时总电量累计*/

        LIB_MemSet(0, (INT8U *)&stTariffEngyMny, sizeof(stTariffEngyMny));
        LIB_MemCpy((INT8U *)&stNowEngyMny.CurData, (INT8U *)&stTariffEngyMny.Data[stNowEngyMny.CurTariffNo-1], sizeof(ST_ENGY_MNY_FORM));
        LIB_MemSet(VALIDFLAG, (INT8U *)stTariffEngyMny.ValidFlag, sizeof(stTariffEngyMny.ValidFlag));
        stTariffEngyMny.CRC = CRC16((INT8U *)&stTariffEngyMny, FIND(ST_ENGYMNY_TARIFF,CRC) );
        //LIB_MemCpy((INT8U *)&stNowEngyMny.Total.T,(INT8U *)&stNowEngyMny.ABC.A,sizeof(ST_PHASE_ENGY));

        stNowEngyMny.PmData.MonthMoney=0;
        stNowEngyMny.PmData.MonthEngy =0;
        stRunData.PmData.NowMonthUseDays =0;
        stRunData.PmData.NowMonthUseEngy =0;
        stRunData.PmData.CRC= CRC16((INT8U *)&stRunData.PmData,FIND(ST_PMRUN_DATA,CRC) );
        EEP_Write(PMRUNDATA_ADDR, (INT8U *)&stRunData, sizeof(stRunData));
        EngySave(ENGY_ADDR1|ENGY_ADDR2);
        Capture(ENERGY_OVER_EVENT_LOG,0,&DateTime);/*"记翻转记录"*/
    }
#endif
}
#endif
/*"*****************************************************************************"*/
/*"  Function:       EngyCarryChk"*/
/*"  Description:    电量尾数进位运算处理"*/
/*"  Calls:          "*/
/*"  Called By:      主循环or上电电量初始化处理完成"*/
/*"  Input:          pEngyTail  给定电量尾数数据指针"*/
/*"                  pDestEngyTotal 总电量累加变量数据指针"*/
/*"                  pDestEngyTariff 当前费率电量累加变量数据指针"*/
/*"  Return:         TRUE  电量有进位    FALSE  电量无进位"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U EngyCarryChk(INT32U *pEngyTail, ENGY_TYPE_U *pDestEngyTotal, ENGY_TYPE_U *pDestEngyTariff)
{
    INT8U CarryFlg;

    CarryFlg = FALSE;

    while((*pEngyTail) >= SCALE_THR)
    {
        CarryFlg = TRUE;
        (*pEngyTail) -= SCALE_THR;
        (*pDestEngyTotal) += 1; /*电量累计*/
    #if (TARIFF_NUM >= 1)
        if(NULL != pDestEngyTariff)
        {
            (*pDestEngyTariff) += 1; /*分时总电量累计*/
        }
    #endif
    }
    return CarryFlg;
}

#if (TARIFF_NUM >= 1)
/*"*****************************************************************************"*/
/*"  Function:       ChkEngyRegActive"*/
/*"  Description:    检查制定电量当前费率寄存器是否激活"*/
/*"  Calls:          "*/
/*"  Called By:      Engyadd"*/
/*"  Input:          obis  给定电量寄存器对象名"*/
/*"  Output          "*/
/*"  Return:         TRUE  寄存器激活    FALSE  寄存器不激活"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U  ChkEngyRegActive(INT16U obis)
{
#ifndef ENGY_REGISTER_ACTIVE_FUN
    return TRUE;
#else
    INT8U maskBitNum;
    INT8U maskByteNum;
    INT8U mask;
    INT8U ret;

    ret = FALSE;
    maskBitNum = (INT8U)((obis - E_PA_ENGY_T1) & 0x07);
    maskByteNum = (INT8U)((obis - E_PA_ENGY_T1) >> 3);
    mask = (INT8U)(0x01 << maskBitNum);
    if(0 != (stLrmPara.EngyRegMask[ActiveMaskId].byte[maskByteNum] & mask))/*"对应寄存器的mask位为1"*/
    {
        ret = TRUE;
    }

    return ret;
#endif
}
#endif

#ifdef WS_METER_HT7038_EXENERGY
const INT16U  ExEngyList[] =
{
        T_EXENGY_P,
        A_EXENGY_P,
        B_EXENGY_P,
        C_EXENGY_P,
        T_EXENGY_Q,
        A_EXENGY_Q,
        B_EXENGY_Q,
        C_EXENGY_Q,
};
#define  EX_ENGY_NUM   (sizeof(ExEngyList)/2)

typedef struct
{
    INT32U ExEngyP;        /*"总有功电量"*/
#if (PHASE_ENGY_EN == 1)
    INT32U ExPhaseEngyP[3];/*"分相有功电量"*/
#endif
#if (REACTIVE_ENGY_EN == 1)
    INT32U ExEngyQ;        /*"总无功电量"*/
#if (PHASE_ENGY_EN == 1)
    INT32U ExPhaseEngyQ[3];/*"分相无功电量"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    INT32U ExEngyS;        /*"总视在电量"*/
#if (PHASE_ENGY_EN == 1)
    INT32U ExPhaseEngyS[3];/*"分相视在电量"*/
#endif
#endif
} ST_EX_ENGY;
/*"*****************************************************************************"*/
/*"  Function:       ExEngyAccumulation"*/
/*"  Description:    掉电处理扩展寄存器电量数据到电量尾数单元（只针对7038计量方案），保证掉电电量不丢失"*/
/*"  Calls:          "*/
/*"  Called By:      EU_PwrDown"*/
/*"  Input:          "*/
/*"  Output          "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ExEngyAccumulation(void)
{
    INT8U i;
    INT16U quadrant[4];
    ST_EX_ENGY EngergyTemp;
    INT32U *pEngy;

    MU_GetExEnergyQuad(quadrant);
    pEngy = (INT32U *)&EngergyTemp;
    for(i = 0;i < EX_ENGY_NUM; i++)/*"累加快速电量寄存器中的电量到尾数"*/
    {
        MU_EngyGet(ExEngyList[i], pEngy);
        pEngy++;
    }
#if (QUAD_ACTIVE_ENGY_EN == 1)
    stPdSaveData.stEngyTail.TotalTail.P[quadrant[0]] += EngergyTemp.ExEngyP;
#else
    if ((quadrant[0] == 0) || (quadrant[0] == 3))/*"有功电量累加到正向有功电量单元"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComP[0] += EngergyTemp.ExEngyP;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComP[1] += EngergyTemp.ExEngyP;
    }
#endif
#if (REACTIVE_ENGY_EN == 1)
    stPdSaveData.stEngyTail.TotalTail.Q[quadrant[0]] += EngergyTemp.ExEngyQ;
#ifdef REACTIVE_ENGY_COM
    if ((quadrant[0] == 0) || (quadrant[0] == 1))/*"无功电量累加到正向无功电量单元"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComQ[0] += EngergyTemp.ExEngyQ;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComQ[1] += EngergyTemp.ExEngyQ;
    }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    if((quadrant[0] == 0) || (quadrant[0] == 1))/*"总有功方向为正，视在电量累加到正向视在电量单元"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[0] += EngergyTemp.ExEngyS;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[1] += EngergyTemp.ExEngyS;
    }
#endif

/*"--------分相电量尾数数据累加--------------"*/
#if (PHASE_ENGY_EN == 1)
    for(i = 0; i < 3; i++)
    {
#if (QUAD_ACTIVE_ENGY_EN == 1)
        stPdSaveData.stEngyTail.PhaseTail[i].P[quadrant[i + 1]] += EngergyTemp.ExPhaseEngyP[i];
#else
        if((quadrant[i + 1] == 0) || (quadrant[i + 1] == 3))/*"1/4象限，有功电量累加到正向有功电量单元"*/
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComP[0] += EngergyTemp.ExPhaseEngyP[i];
        }
        else
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComP[1] += EngergyTemp.ExPhaseEngyP[i];
        }
#endif
#if (REACTIVE_ENGY_EN == 1)
        stPdSaveData.stEngyTail.PhaseTail[i].Q[quadrant[i + 1]] += EngergyTemp.ExPhaseEngyQ[i];
#ifdef REACTIVE_ENGY_COM
        if((quadrant[i + 1] == 0) || (quadrant[i + 1] == 1))/*"1/2象限，无功电量累加到正向无功电量单元"*/
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComQ[0] += EngergyTemp.ExPhaseEngyQ[i];
        }
        else
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComQ[1] += EngergyTemp.ExPhaseEngyQ[i];
        }
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
        if((quadrant[i + 1] == 0) || (quadrant[i + 1] == 3))/*"1/4象限，视在电量累加到正向视在电量单元"*/
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComS[0] += EngergyTemp.ExPhaseEngyS[i];
        }
        else
        {
            stPdSaveData.stEngyTail.PhaseTail[i].ComS[1] += EngergyTemp.ExPhaseEngyS[i];
        }
#endif
    }
#endif
    stPdSaveData.stEngyTail.Crc = CRC16((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
}
#endif
