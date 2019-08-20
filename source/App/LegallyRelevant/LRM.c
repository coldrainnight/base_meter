/*"*****************************************************************************"*/
/*"  FileName: LRM.c"*/
/*"  Description:     ���ļ�ʵ�ַ�������ģ�����ӿں�������"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/10/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "LRM.h"
#include "MU_LIB.h"
#include "EU.h"
#include "SC.h"
#include "App\message\MS.h"
#include "Driver\device\MU_IC.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\monitor\MNT.h"


NO_INIT ST_LRM_PARA  stLrmPara;

static void setDefaultLrmPara(void);
static void LrmPdDataInit(void);

/*"*****************************************************************************"*/
/*"  Function:       LRM_Init"*/
/*"  Description:    ����ģ���ϵ��ʼ���ӿ�"*/
/*"  Calls:          "*/
/*"  Called By:      main"*/
/*"  Input:          mode      ��ʼ��ģʽ"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void LRM_Init(INT8U mode)
{
    if(FIRST_PWR_ON == mode)
    {
        EU_Clr(0);
        setDefaultLrmPara();
    }
    else
    {
        ChkLrmParaValid();
        LrmPdDataInit();
        EU_Init(mode);
    }
    if(ChkStatus(E_MTR_RST) == TRUE)
    {
        SC_SysClkInit(mode);
        //MCU_DelayMs(300);/*" ���¹���� "*/
    }
    if((FIRST_PWR_ON == mode) || (MCU_RUNMODE == mode))
    {
        MU_Init(MCU_RUNMODE);
    }
    RTC_CaliInit(mode);
}



/*"*****************************************************************************"*/
/*"  Function:       LRM_Main"*/
/*"  Description:    ����ģ���ϵ�����������ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void LRM_Main(void)
{
    SC_main();
    if(TRUE == Task_ChkTsk_1s (MAINTASK_EU))
    {
        MU_Main();
        EU_secTask();
        RTC_CaliMain(MCU_RUNMODE);
        //ChkLrmParaValid();
    }

}
/*"*****************************************************************************"*/
/*"  Function:       LRM_LPMain"*/
/*"  Description:    ����ģ��͹�������������ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void LRM_LPMain(INT8U mode)
{
   
   RTC_CaliMain(MCU_LPMODE_DEEP);
#ifdef THREE_PHASE
    INT64U tMode=0;

    GetSingle(E_PWR_RST_MODE, (INT8U *)&tMode); 
    if(tMode != PWR_RST_BATT)
    {
      MU_PD_SecTask();
     }
#endif
    if(MCU_LPMODE_MID == mode)
    {
        SC_Task1SecPD();
    }
}

/*"*****************************************************************************"*/
/*"  Function:       LRM_Clr"*/
/*"  Description:    ����ģ�����㷽���ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      ms"*/
/*"  Input:          pdin �������"*/
/*"  Output:         pdout �������"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S LRM_Clr(void *pdin, void *pdout)
{
    INT8U rsult;

    EU_Clr(0);
    rsult = MS_SUCCESS;

    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       LRM_JustMtr"*/
/*"  Description:    ����ģ��У�����ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      ms"*/
/*"  Input:          pdin �������"*/
/*"  Output:         pdout �������"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S LRM_JustMtr(void *pdin, void *pdout)
{
    INT8U rsult;

    rsult = MU_JustMtr(pdin);

    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       LRM_PwrDown"*/
/*"  Description:    ����ģ����紦�����ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      ms"*/
/*"  Input:          pdin �������"*/
/*"  Output:         pdout �������"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S LRM_PwrDown(void *pdin, void *pdout)
{

    EU_PwrDown();

    return TRUE;
}


#ifdef SINGLE_PHASE
/*"*****************************************************************************"*/
/*"  Function:       LRM_PulseCtrInit"*/
/*"  Description:    ������ƿ��ϵ��ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      ms"*/
/*"  Input:          pdin �������"*/
/*"  Output:         pdout �������"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S LRM_PulseCtrInit(void *pdin, void *pdout)
{
	MU_OpenPulse();
    return TRUE;
}
#endif


/*"*****************************************************************************"*/
/*"  Function:       setDefaultLrmPara"*/
/*"  Description:    ����ģ�������ʼ���ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      LRM_FirstInit"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void setDefaultLrmPara(void)
{
    INT16U uni;

    for(uni = 0; uni < sizeof(ST_LRM_PARA); uni++)/*"RAM�в����ȳ�ʼ��ΪĬ��ֵ"*/
    {
        *((INT8U *)&stLrmPara + uni) = *((INT8U FAR_POINTER*)&LrmParaDefault + uni);
    }
    stLrmPara.Crc = CRC16((INT8U *)&stLrmPara, FIND(ST_LRM_PARA, Crc));
    FS_WriteFile(E_FILE_LRM,LRM_PARA_BAK1_SHIFT,(INT8U *)&stLrmPara,sizeof(ST_LRM_PARA));
    FS_WriteFile(E_FILE_LRM,LRM_PARA_BAK2_SHIFT,(INT8U *)&stLrmPara,sizeof(ST_LRM_PARA));
}
/*"*****************************************************************************"*/
/*"  Function:       ChkLrmParaValid"*/
/*"  Description:    ��鷨��ģ��������ݺϷ��ԣ����ӱ������ݽ��лָ�����"*/
/*"  Calls:          "*/
/*"  Called By:      ��ʼ����set"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         TRUE  �����Ϸ�  FALSE �����Ƿ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ChkLrmParaValid(void)
{
    INT8U rsult;
    INT8U i;

    rsult = LIB_ChkDataValid((INT8U *)&stLrmPara, FIND(ST_LRM_PARA, Crc), stLrmPara.Crc);
    if(FALSE == rsult)
    {
        for(i = 0; i < MAX_READ_FILE_LOOP; i++)
        {
            rsult = FS_ReadFile(E_FILE_LRM, LRM_PARA_BAK1_SHIFT, (INT8U *)&stLrmPara, sizeof(ST_LRM_PARA));
            if((TRUE == rsult) && (TRUE == LIB_ChkDataValid((INT8U *)&stLrmPara, FIND(ST_LRM_PARA, Crc), stLrmPara.Crc)))
            {
                rsult = TRUE;
                break;
            }
            else
            {
                rsult = FALSE;
                MCU_DelayMs(10);
            }
        }
        if(FALSE == rsult)
        {
            for (i = 0; i < MAX_READ_FILE_LOOP; i++)
            {
                rsult = FS_ReadFile(E_FILE_LRM, LRM_PARA_BAK2_SHIFT, (INT8U *)&stLrmPara, sizeof(ST_LRM_PARA));
                if ((TRUE == rsult)
                    && (TRUE == LIB_ChkDataValid((INT8U *) &stLrmPara, FIND(ST_LRM_PARA, Crc), stLrmPara.Crc)))
                {
                    rsult = TRUE;
                    break;
                }
                else
                {
                    rsult = FALSE;
                    MCU_DelayMs(10);
                }
            }
            if(FALSE == rsult)
            {
                INT16U uni;
                for (uni = 0; uni < sizeof(ST_LRM_PARA); uni++)/*"RAM�в����ȳ�ʼ��ΪĬ��ֵ"*/
                {
                    *((INT8U *) &stLrmPara + uni) = *((INT8U FAR_POINTER*) &LrmParaDefault + uni);
                }
                //todo save record
            }
        }
    }

    return (rsult);
}
/*"*****************************************************************************"*/
/*"  Function:       LrmPdDataInit"*/
/*"  Description:    ��������ģ����������ϵ��ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      ��ʼ��"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:        "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void LrmPdDataInit(void)
{
    INT8U rsult;
    INT8U i;
    rsult = LIB_ChkDataValid((INT8U *)&stPdSaveData, FIND(ST_LRM_DATA, crc), stPdSaveData.crc);
    if(FALSE == rsult)
    {
        for (i = 0; i < MAX_READ_FILE_LOOP; i++)
        {
            rsult = FS_ReadFile(E_FILE_LRM_DATA, LRM_PD_SAVE_DATA_SHIFT, (INT8U *)&stPdSaveData, sizeof(ST_LRM_DATA));
            if((TRUE == rsult) && (TRUE == LIB_ChkDataValid((INT8U *)&stPdSaveData, FIND(ST_LRM_DATA, crc), stPdSaveData.crc)))
            {
                rsult = TRUE;
                break;
            }
            else
            {
                rsult = FALSE;
                MCU_DelayMs(10);
            }
        }
        if(FALSE == rsult)
        {
            Push(E_LRM_PD_DATA_ERR, TRUE);
            //todo debug
        }
    }
    stPdSaveData.ValidFlag = 0;/*"�ƻ����������RAM�����ݺϷ���"*/
}

#if 1
void Lrm_test(void)
{
    INT8U i;

    INT64U sum;
    #if defined (THREE_PHASE)
    INT64U phase_eng;
    #endif

#if (TARIFF_NUM > 0)
    INT64U data;
    sum = 0;
    for(i=0; i<TARIFF_NUM; i++)
    {
        data = 100000 + i*10000;
        sum += data;
        LIB_MemCpy((INT8U *)&data, (INT8U *)&stNowEngyBak.TariffEngy[i+1].ComP[0],
            sizeof(ST_ENERGY_FORM));
    }
#endif
    sum = 101000;
    LIB_MemCpy((INT8U *)&sum, (INT8U *)&stNowEngyBak.TariffEngy[0].ComP[0],sizeof(ST_ENERGY_FORM));
#if (TARIFF_NUM > 0)
    sum = 0;
    for(i=0; i<TARIFF_NUM; i++)
    {
        data = 200000 + i*10000;
        sum += data;
        LIB_MemCpy((INT8U *)&data, (INT8U *)&stNowEngyBak.TariffEngy[i+1].ComP[1],
            sizeof(ST_ENERGY_FORM));
    }
#endif
    for(i=0; i<4; i++)
    {
        sum = 10000 + i*1000;
        LIB_MemCpy((INT8U *)&sum, (INT8U *)&stNowEngyBak.TariffEngy[0].Q[i],sizeof(ST_ENERGY_FORM));
    }
    sum = 201000;
    LIB_MemCpy((INT8U *)&sum, (INT8U *)&stNowEngyBak.TariffEngy[0].ComP[1],sizeof(ST_ENERGY_FORM));
#if defined (THREE_PHASE)
    phase_eng = sum/3;
#endif
    for(i=0; i<MU_ELEMENT_NUM; i++)
    {
        #if defined (THREE_PHASE)
        LIB_MemCpy((INT8U *)&phase_eng, (INT8U *)&stNowEngyBak.Phase[i].ComP[0],
        sizeof(ST_ENERGY_FORM));
        #endif
        stMmtData.curr[0] = 49000 +i*1000;
    }
    stMmtData.volt[0] = 2200;
    stMmtData.actPwr[0] = 50000;
    stMmtData.freq = 500000;
    stMmtData.factor[0] = 10000;
}


#endif

