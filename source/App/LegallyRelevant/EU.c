/*"*****************************************************************************"*/
/*"  FileName: EU.c"*/
/*"  Description:     ���ļ���������ģ����������ݴ�����ع���"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
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
#define  ENGY_ADDR1      (0X01)/*���������ڿ���ַ1*/
#define  ENGY_ADDR2      (0X02)/*���������ڿ���ַ2*/
#define  ENGY_BAK_ADDR_FLG     stPdSaveData.EngyAddrFlag   /*"��ǰ�ڿ��еĵ������ݿ�"*/
#define  ENGY_BAK_PEROID  (30 * 60)   /*�����������ڣ��룩  30����*/

/*"*********************����ģ���������**************************"*/
NO_INIT ST_RAM_ENGY_DATA stNowEngy;/*"��ǰ����������+��ǰ����"*/
NO_INIT ST_LRM_DATA stPdSaveData;/*" ��ǰ����ģ�����ݣ�������ǰ�����ۼ�ֵ������β��������������ݣ�"*/
NO_INIT ST_BAKUP_ENGY_DATA stNowEngyBak;/*" ��ǰ��������RAM����"*/
#ifdef  AMP_HOUR_EN
INT32U AmpHour[MU_ELEMENT_NUM];/*"�ϵ����ۼư�ʱֵ����λ 0.01AH ���������"*/
static INT32U AmpHourTail[MU_ELEMENT_NUM];/*"�ۼư�ʱֵβ������λ 0.0001A.s ���������"*/
#define ANHOUR_VALUE_01ANH   360000 /*"��0.0001An.sת����0.01An.h"*/
#endif
static INT8U mPdDateCrcBrkFlg; /*"������CRC�ƻ���ʶ"*/
static INT16U EngyBakPeriodCnt;/*"������������ʱ���ۼƱ�������λ��"*/
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
    E_PA_ENGY_T1,/*"�����й�����1����"*/
    E_PA_ENGY_T2,/*"�����й�����2����"*/
    E_PA_ENGY_T3,/*"�����й�����3����"*/
    E_PA_ENGY_T4,/*"�����й�����4����"*/
    E_NA_ENGY_T1,/*"�����й�����1����"*/
    E_NA_ENGY_T2,/*"�����й�����2����"*/
    E_NA_ENGY_T3,/*"�����й�����3����"*/
    E_NA_ENGY_T4,/*"�����й�����4����"*/
    E_PR_ENGY_T1,/*"�����޹�����1����"*/
    E_PR_ENGY_T2,/*"�����޹�����2����"*/
    E_PR_ENGY_T3,/*"�����޹�����3����"*/
    E_PR_ENGY_T4,/*"�����޹�����4����"*/
    E_NR_ENGY_T1,/*"�����޹�����1����"*/
    E_NR_ENGY_T2,/*"�����޹�����2����"*/
    E_NR_ENGY_T3,/*"�����޹�����3����"*/
    E_NR_ENGY_T4,/*"�����޹�����4����"*/
    E_R_ENGY_I_T1,/*"I�����޹�����1����"*/
    E_R_ENGY_I_T2,/*"I�����޹�����2����"*/
    E_R_ENGY_I_T3,/*"I�����޹�����3����"*/
    E_R_ENGY_I_T4,/*"I�����޹�����4����"*/
    E_R_ENGY_II_T1,/*"II�����޹�����1����"*/
    E_R_ENGY_II_T2,/*"II�����޹�����2����"*/
    E_R_ENGY_II_T3,/*"II�����޹�����3����"*/
    E_R_ENGY_II_T4,/*"II�����޹�����4����"*/
    E_R_ENGY_III_T1,/*"III�����޹�����1����"*/
    E_R_ENGY_III_T2,/*"III�����޹�����2����"*/
    E_R_ENGY_III_T3,/*"III�����޹�����3����"*/
    E_R_ENGY_III_T4,/*"III�����޹�����4����"*/
    E_R_ENGY_IV_T1,/*"IV�����޹�����1����"*/
    E_R_ENGY_IV_T2,/*"IV�����޹�����2����"*/
    E_R_ENGY_IV_T3,/*"IV�����޹�����3����"*/
    E_R_ENGY_IV_T4,/*"IV�����޹�����4����"*/
};
#endif
/*"*****************************************************************************"*/
/*"  Function:       EU_Init"*/
/*"  Description:    �����ۼ�ģ���ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      LRM��ʼ�����ӿں���"*/
/*"  Input:          mode      ��ʼ��ģʽ��  "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void EU_Init(INT8U mode)
{
    mPdDateCrcBrkFlg = 0;
    EngyBakPeriodCnt = 0;

    /*����β���Ϸ��Լ��*/
    EngyTailChkPwrUp();
    /*��ʼ�����غϷ��ĵ�������*/
    LoadEngyData(TRUE, mode);
#ifdef  AMP_HOUR_EN
    LIB_MemSet(0, (INT8U *)&AmpHourTail[0], (MU_ELEMENT_NUM << 2));
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       EU_secTask"*/
/*"  Description:    �����ۼ�ģ��������ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      LRM_main��������"*/
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

    PdDateCrcBrkFlg();/*"�ƻ���������Ϸ��ԣ������ϵ��ظ�����"*/
    EngyBakPeriodCnt++;

    /*"���ݱ�־�ݴ���"*/
    if((ENGY_BAK_ADDR_FLG & 0x03) == 0)
    {
        ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
    }
    /*"���й����е������ݺϷ���У��"*/
    LoadEngyData(FALSE, MCU_RUNMODE);
    /*δ����1kwh��������ʱ����RAM������*/
#if (TARIFF_NUM >= 1)
    GetSingle(E_TARIFF_NOW, &TariffNow);
    if((TariffNow == 0) || (TariffNow > TARIFF_NUM))/*"���ʺ��ݴ���"*/
    {
        TariffNow = 1;
    }

    if(TariffNow != stNowEngy.tariffNow)/*"�����л������л���ǰ���ʵ�������"*/
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
    /*"ÿS��õ��������ݵ�λ��ws��"*/
    EngyTailAdd();
    Engyadd();
}
/*"*****************************************************************************"*/
/*"  Function:       EU_Clr"*/
/*"  Description:    ������������"*/
/*"  Calls:          "*/
/*"  Called By:      ACTION�б�"*/
/*"  Input:          kind  ���� �������"*/
/*"  Output:         "*/
/*"  Return:         TRUE ����ɹ�   FALSE  ����ʧ��"*/
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
    /*��ʱ���ݵ������*/
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
/*"  Description:    �������ݵ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ACTION�б�"*/
/*"  Input:          kind  ���� �������"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void EU_PwrDown(void)
{
    /*��ȡmWS���ݲ�ת�����������洢*/
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
/*"  Description:    ���·�ʱ�����ṹ��"*/
/*"  Calls:          "*/
/*"  Called By:      �������ݱ����ݴ�"*/
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
/*"  Description:    �������ݱ��ݲ����ڿ�"*/
/*"  Calls:          "*/
/*"  Called By:      EU_main"*/
/*"  Input:          flag  �������ͱ�־"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void EngySave(INT8U flag)
{
    /*�رյ����ж�*/
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
/*"  Description:    ��ǰ����������Ч�Լ��"*/
/*"  Calls:          "*/
/*"  Called By:      ��������ǰ"*/
/*"  Input:          pdata  �����������ָ��"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U NowEngyDataChk(ST_RAM_ENGY_DATA *pdata)
{
#if (TARIFF_NUM >= 1)
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_RAM_ENGY_DATA,Crc), pdata->Crc))/*��ʶ�ֱȶ�,CRCУ��*/
    && ((pdata->tariffNow <= TARIFF_NUM) && (pdata->tariffNow != 0)))/*���ʺ�����*/
#else
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_RAM_ENGY_DATA,Crc), pdata->Crc))/*��ʶ�ֱȶ�,CRCУ��*/
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
/*"  Description:    ���ݵ���������Ч�Լ��"*/
/*"  Calls:          "*/
/*"  Called By:      �����ݴ���"*/
/*"  Input:          �����������ָ��"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U BakEngyDataChk(ST_BAKUP_ENGY_DATA *pdata)
{
#if (TARIFF_NUM >= 1)
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_BAKUP_ENGY_DATA,Crc), pdata->Crc))/*��ʶ�ֱȶ�,CRCУ��*/
    && ((pdata->tariffBak <= TARIFF_NUM) && (pdata->tariffBak != 0))/*���ʺ�����*/
    && (TRUE == EngSumChk(pdata)))/*�ܺͷַ��ʺͱȽ�*/
#else
    if((TRUE == LIB_ChkDataValid((INT8U*)pdata, FIND(ST_BAKUP_ENGY_DATA,Crc), pdata->Crc))/*��ʶ�ֱȶ�,CRCУ��*/
     /*���ʺ�����*/
    /*&& (TRUE == EngSumChk(pdata))*/) /*�ܺͷַ��ʺͱȽ�*/
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
/*"  Description:    ���ڿ�EE1��EE2��ȡ��Ч�ĵ�������"*/
/*"  Calls:          "*/
/*"  Called By:      �����ݴ�����ʼ������������ǰ��"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         0 EE1/EE2���ݶ��쳣  1��ǰ���ݵ�����������  2��һ���ڱ��ݵ�����������   3 EE1/EE2���ݵ������ݶ�����"*/
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
        ret = FS_ReadFile(E_FILE_LRM_DATA, ENGY_BAK1_SHIFT, (INT8U *)&EngyBak1, sizeof(ST_BAKUP_ENGY_DATA));/*����EE1�洢��*/
        if ((ret == TRUE) && (TRUE == BakEngyDataChk(&EngyBak1)))
        {
            EngyValidFlg.bit.b0 = TRUE;
            break;
        }
        MCU_DelayMs(10);
    }
    for (i = 0; i < MAX_READ_FILE_LOOP; i++)
    {
        ret = FS_ReadFile(E_FILE_LRM_DATA, ENGY_BAK2_SHIFT, (INT8U *) &EngyBak2, sizeof(ST_BAKUP_ENGY_DATA));/*����EE2�洢��*/
        if ((ret == TRUE) && (TRUE == BakEngyDataChk(&EngyBak2)))
        {
            EngyValidFlg.bit.b1 = TRUE;
            break;
        }
        MCU_DelayMs(10);
    }

    if((0x03 == (EngyValidFlg.byte & 0X03)))/*ee1,ee2��*/
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
    else if(0x01 == (EngyValidFlg.byte & 0X03))/*EE1�洢����*/
    {
        ENGY_BAK_ADDR_FLG = ENGY_ADDR2;
        pScr = (INT8U *) &EngyBak1;
    }
    else if(0x02 == (EngyValidFlg.byte & 0X03))/*EE2�洢����*/
    {
        ENGY_BAK_ADDR_FLG = ENGY_ADDR1;
        pScr = (INT8U *) &EngyBak2;
    }
    else/*ȫ��*/
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
/*"Description: ���ֵ���֮�����ܵ���֮�͵Ĳ�ֵ�Ƿ�Ϸ�"*/
/*"Input:       pData ����������ָ��"*/
/*"Output:      ��    "*/
/*"Return:      valFlg  TRUE �ֵ���֮�����ܵ���֮�͵Ĳ�ֵ�Ϸ�  FALSE �ֵ���֮�����ܵ���֮�͵Ĳ�ֵ����Χ"*/
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
/*"  Description:    װ����ȷ�ĵ������ݡ�����RAM�����ڿ��е����ݱȶ�"*/
/*"  Calls:          "*/
/*"  Called By:      �����ݴ�����ʼ������������ǰ��"*/
/*"  Input:          flag: 1 �ϵ����������װ�أ�0 ���ݹ�����������ȷ���ж�"*/
/*"                  mode =MCU_RUNMODE����ģʽ����͹���ģʽ"*/
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
            if(TRUE == flag)/*���ϵ�ʱ�õ������ڿ������ݣ��������̵�����У�鲻ʹ�õ������ڿ�����*/
            {
                INT8U i;

                for(i = 0; i < MAX_READ_FILE_LOOP; i++)
                {
                    ret = FS_ReadFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT + FIND(ST_LRM_DATA, pdNowEngy),
                                       (INT8U *)&stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));/*"�����洢��"*/
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
            /*"����û�з����ı䣬���ܺͷַ��ʵ�����Ϸ�"*/
            if((stNowEngy.tariffNow == stNowEngyBak.tariffBak) && (TRUE == TariffEngyDivOk()))
            {
                EngyRenewFlg = 0x5a;
            }
            else/*"�����иı���ַܷ��ʵ�����ޣ����ܵ����õ�ǰ�������ݵ��ַ��ʵ������п�������ܺͷַ��ʵ�����һ�£�ֻ�ָܻ�����ʷ����"*/
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
            RenewTariffData();/*�ϵ������RAM�Ϸ�ʱ�����µ�ǰ��������*/
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
/*"  Description:    ��ǰ�ܵ�������ǰ���ʵ����뱸���ܵ������ַ��ʵ�����ֵ�Ϸ��Լ��"*/
/*"  Calls:          "*/
/*"  Called By:      �����ݴ�����ʼ������������ǰ��"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         TRUE �Ϸ�  FALSE �Ƿ� "*/
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
/*"  Description:    ����β����Ч�Լ��"*/
/*"  Calls:          "*/
/*"  Called By:      �ϵ��ʼ��"*/
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
                                      (INT8U *)&stPdSaveData.stEngyTail, sizeof(ST_ENGY_TAIL));/*"�����洢��"*/
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
/*"  Description:    RAM�е���β����Ч�Լ��"*/
/*"  Calls:          "*/
/*"  Called By:      �ϵ��ʼ��������β���ۼ�ǰ"*/
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
    ENGY_TYPE_U P[4]; /*"�������й�����"*/
#endif
    ENGY_TYPE_U ComP[2]; /*"���������й�����"*/
#if (REACTIVE_ENGY_EN == 1)
    ENGY_TYPE_U Q[4]; /*"�������޹�����"*/
#ifdef REACTIVE_ENGY_COM
    ENGY_TYPE_U ComQ[2]; /*"���������޹�����"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    ENGY_TYPE_U ComS[2]; /*"�������ڵ���"*/
#endif
} ST_BASE_CAL_ENGY;

//extern void Lrm_test(void);
extern INT8U get_bm_status(INT8U *buf);




/*"*****************************************************************************"*/
/*"  Function:       Engyadd"*/
/*"  Description:    ����β����λ�������ۼ�"*/
/*"  Calls:          "*/
/*"  Called By:      ��ѭ��"*/
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
    ST_BASE_CAL_ENGY tmpTotalEngy;/*"�ܵ����ۼ���ʱ�����������ֽ�������ΪINT32U��INT64U���������"*/
#if (TARIFF_NUM >= 1)
    ST_BASE_CAL_ENGY tmpTariffEngy;/*"��ǰ���ʵ����ۼ���ʱ�����������ֽ�������ΪINT32U��INT64U���������"*/
#endif
#if (PHASE_ENGY_EN == 1)
    ST_BASE_CAL_ENGY tmpPhaseEngy[3];/*"��������ۼ���ʱ�����������ֽ�������ΪINT32U��INT64U���������"*/
#endif
    INT8U EnergyRenewFlg;/*"��������ˢ�±�־��������ˢ�£������¼���У���"*/
    INT8U TariffRenewFlg;/*"��ǰ�����б仯���豸�ݷַ��ʵ������ݣ����¼���У���"*/
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
    /*"��ǰ���ʺźϷ��Լ��"*/
    if((stNowEngy.tariffNow == 0) || (stNowEngy.tariffNow > TARIFF_NUM))
    {
        stNowEngy.tariffNow = 1;
        TariffRenewFlg = 0x5a;
    }
#endif

    /*"��stNowEngy�е�����������ʱ���������У����е����ۼ�����"*/
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
        /*"�������й������ۼ�"*/
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
        /*"�����й������ۼ�"*/
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
    for(i = 0; i < 2; i++)/*"�����������ڵ����ۼ�"*/
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

    /*"�����ۼ�������ɺ�,���������и��£�����ʱ���������п�����stNowEngy������"*/
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
        /*"���ݵ�ǰ���������������RAM��"*/
        LIB_MemCpy((INT8U *)&stNowEngy, (INT8U *)&stPdSaveData.pdNowEngy, sizeof(ST_RAM_ENGY_DATA));

        stPdSaveData.stEngyTail.Crc = CRC16((INT8U *)&stPdSaveData.stEngyTail, FIND(ST_ENGY_TAIL,Crc));
    /*"��ǰ��������ͬ�������ݷ�ʱ���������� stNowEngy -> stNowEngyBak"*/
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
/*"ÿ��������ݻ���ṹ��"*/
typedef struct
{
    INT32U EngyP;        /*"���й�����"*/
#if (PHASE_ENGY_EN == 1)
    INT32U PhaseEngyP[3];/*"�����й�����"*/
#endif
#if (REACTIVE_ENGY_EN == 1)
    INT32U EngyQ;        /*"���޹�����"*/
#if (PHASE_ENGY_EN == 1)
    INT32U PhaseEngyQ[3];/*"�����޹�����"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    INT32U EngyS;        /*"�����ڵ���"*/
#if (PHASE_ENGY_EN == 1)
    INT32U PhaseEngyS[3];/*"�������ڵ���"*/
#endif
#endif
#if (PHASE_ENGY_EN == 1)
    INT8U PhaseQuad[3];/*"��ǰ��������ֵ"*/
#endif
} ST_SECOND_ENGY;

/*"��ȡÿ����������ʶ�б��б�˳������ST_SECOND_ENGY�ṹ�嶨��˳�򱣳�һ��"*/
/*"-------�������ݲ����б��ж���----------"*/
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
/*"  Description:    ����β���ۼ�"*/
/*"  Calls:          "*/
/*"  Called By:      ��ѭ��������"*/
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
    if ((stMmtData.PowerDir & 0x80) == 0)/*"�й������ۼӵ������й�������Ԫ"*/
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
    if ((stMmtData.PowerDir & 0x08) == 0)/*"�޹������ۼӵ������޹�������Ԫ"*/
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
    if((stMmtData.PowerDir & 0x80) == 0)/*"���й�����Ϊ�������ڵ����ۼӵ��������ڵ�����Ԫ"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[0] += EngergyTemp.EngyS;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[1] += EngergyTemp.EngyS;
    }
#endif

/*"--------�������β�������ۼ�--------------"*/
#if (PHASE_ENGY_EN == 1)
    for(i = 0; i < 3; i++)
    {
#if (QUAD_ACTIVE_ENGY_EN == 1)
        stPdSaveData.stEngyTail.PhaseTail[i].P[stMmtData.Quad[i + 1]] += EngergyTemp.PhaseEngyP[i];
#else
        if((stMmtData.Quad[i + 1] == 0) || (stMmtData.Quad[i + 1] == 3))/*"1/4���ޣ��й������ۼӵ������й�������Ԫ"*/
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
        if((stMmtData.Quad[i + 1] == 0) || (stMmtData.Quad[i + 1] == 1))/*"1/2���ޣ��޹������ۼӵ������޹�������Ԫ"*/
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
        if((stMmtData.Quad[i + 1] == 0) || (stMmtData.Quad[i + 1] == 3))/*"1/4���ޣ����ڵ����ۼӵ��������ڵ�����Ԫ"*/
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
/*"  Description:    ������CRC�ƻ�"*/
/*"  Calls:          "*/
/*"  Called By:      ��ѭ��or�ϵ������ʼ���������"*/
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
        crc = stPdSaveData.pdNowEngy.Crc + 0x1234; /*"д�ҵ�������CRC"*/
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

        crc = stPdSaveData.stEngyTail.Crc + 0x1234; /*"д�ҵ���������β����CRC"*/
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
    stNowEngy.TotalEngy.P[0] = data; /*�����ۼ�*/
    stNowEngy.TotalEngy.S[0] = data; /*�����ۼ�*/
#if (TARIFF_NUM >= 1)
    stNowEngy.CurTEngy.P[0] = data; /*��ʱ�ܵ����ۼ�*/
    stNowEngy.CurTEngy.S[0] = data; /*��ʱ�ܵ����ۼ�*/
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
    if (( stNowEngyMny.Total.T.P[0] +stNowEngyMny.Total.T.P[1]+stNowEngyMny.Total.T.P[2]+stNowEngyMny.Total.T.P[3]>999999999)) //ÿһ�ȵ�洢����
    {
        stNowEngyMny.Total.T.P[0]= 0; /*�����ۼ�*/
        stNowEngyMny.CurData.T.P[0] = 0; /*��ʱ�ܵ����ۼ�*/
        stNowEngyMny.Total.T.P[1]= 0; /*�����ۼ�*/
        stNowEngyMny.CurData.T.P[1] = 0; /*��ʱ�ܵ����ۼ�*/
        stNowEngyMny.Total.T.P[2]= 0; /*�����ۼ�*/
        stNowEngyMny.CurData.T.P[2] = 0; /*��ʱ�ܵ����ۼ�*/
        stNowEngyMny.Total.T.P[3]= 0; /*�����ۼ�*/
        stNowEngyMny.CurData.T.P[3] = 0; /*��ʱ�ܵ����ۼ�*/

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
        Capture(ENERGY_OVER_EVENT_LOG,0,&DateTime);/*"�Ƿ�ת��¼"*/
    }
#endif
}
#endif
/*"*****************************************************************************"*/
/*"  Function:       EngyCarryChk"*/
/*"  Description:    ����β����λ���㴦��"*/
/*"  Calls:          "*/
/*"  Called By:      ��ѭ��or�ϵ������ʼ���������"*/
/*"  Input:          pEngyTail  ��������β������ָ��"*/
/*"                  pDestEngyTotal �ܵ����ۼӱ�������ָ��"*/
/*"                  pDestEngyTariff ��ǰ���ʵ����ۼӱ�������ָ��"*/
/*"  Return:         TRUE  �����н�λ    FALSE  �����޽�λ"*/
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
        (*pDestEngyTotal) += 1; /*�����ۼ�*/
    #if (TARIFF_NUM >= 1)
        if(NULL != pDestEngyTariff)
        {
            (*pDestEngyTariff) += 1; /*��ʱ�ܵ����ۼ�*/
        }
    #endif
    }
    return CarryFlg;
}

#if (TARIFF_NUM >= 1)
/*"*****************************************************************************"*/
/*"  Function:       ChkEngyRegActive"*/
/*"  Description:    ����ƶ�������ǰ���ʼĴ����Ƿ񼤻�"*/
/*"  Calls:          "*/
/*"  Called By:      Engyadd"*/
/*"  Input:          obis  ���������Ĵ���������"*/
/*"  Output          "*/
/*"  Return:         TRUE  �Ĵ�������    FALSE  �Ĵ���������"*/
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
    if(0 != (stLrmPara.EngyRegMask[ActiveMaskId].byte[maskByteNum] & mask))/*"��Ӧ�Ĵ�����maskλΪ1"*/
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
    INT32U ExEngyP;        /*"���й�����"*/
#if (PHASE_ENGY_EN == 1)
    INT32U ExPhaseEngyP[3];/*"�����й�����"*/
#endif
#if (REACTIVE_ENGY_EN == 1)
    INT32U ExEngyQ;        /*"���޹�����"*/
#if (PHASE_ENGY_EN == 1)
    INT32U ExPhaseEngyQ[3];/*"�����޹�����"*/
#endif
#endif
#if (APPARENT_ENGY_EN == 1)
    INT32U ExEngyS;        /*"�����ڵ���"*/
#if (PHASE_ENGY_EN == 1)
    INT32U ExPhaseEngyS[3];/*"�������ڵ���"*/
#endif
#endif
} ST_EX_ENGY;
/*"*****************************************************************************"*/
/*"  Function:       ExEngyAccumulation"*/
/*"  Description:    ���紦����չ�Ĵ����������ݵ�����β����Ԫ��ֻ���7038��������������֤�����������ʧ"*/
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
    for(i = 0;i < EX_ENGY_NUM; i++)/*"�ۼӿ��ٵ����Ĵ����еĵ�����β��"*/
    {
        MU_EngyGet(ExEngyList[i], pEngy);
        pEngy++;
    }
#if (QUAD_ACTIVE_ENGY_EN == 1)
    stPdSaveData.stEngyTail.TotalTail.P[quadrant[0]] += EngergyTemp.ExEngyP;
#else
    if ((quadrant[0] == 0) || (quadrant[0] == 3))/*"�й������ۼӵ������й�������Ԫ"*/
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
    if ((quadrant[0] == 0) || (quadrant[0] == 1))/*"�޹������ۼӵ������޹�������Ԫ"*/
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
    if((quadrant[0] == 0) || (quadrant[0] == 1))/*"���й�����Ϊ�������ڵ����ۼӵ��������ڵ�����Ԫ"*/
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[0] += EngergyTemp.ExEngyS;
    }
    else
    {
        stPdSaveData.stEngyTail.TotalTail.ComS[1] += EngergyTemp.ExEngyS;
    }
#endif

/*"--------�������β�������ۼ�--------------"*/
#if (PHASE_ENGY_EN == 1)
    for(i = 0; i < 3; i++)
    {
#if (QUAD_ACTIVE_ENGY_EN == 1)
        stPdSaveData.stEngyTail.PhaseTail[i].P[quadrant[i + 1]] += EngergyTemp.ExPhaseEngyP[i];
#else
        if((quadrant[i + 1] == 0) || (quadrant[i + 1] == 3))/*"1/4���ޣ��й������ۼӵ������й�������Ԫ"*/
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
        if((quadrant[i + 1] == 0) || (quadrant[i + 1] == 1))/*"1/2���ޣ��޹������ۼӵ������޹�������Ԫ"*/
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
        if((quadrant[i + 1] == 0) || (quadrant[i + 1] == 3))/*"1/4���ޣ����ڵ����ۼӵ��������ڵ�����Ԫ"*/
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
