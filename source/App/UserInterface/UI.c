/*"*****************************************************************************"*/
/*"  FileName: UI.c"*/
/*"  Description:     ���ļ�ʵ���û��ӿ�ģ�����ӿں�������"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.12.01"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/12/01     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "UI.h"
#include "App\UserInterface\PP\PP_698\Process\Pp_process.h"
#include "Lib\AXDR\AXDR.h"

#define READ_MAX_CNT    (3)

NO_INIT ST_UI_PARA_WITH_BACKUP StUIParaWithRamBackup;

ST_RUN_STATUS   RunStatus;/*���ܱ�����״̬��*/
UN_WORD_DEF    MeterSampleStatus;     /*����оϵͳ״̬��*/
UN_WORD_DEF    SampleStatus;          /*����״̬��*/


#ifdef THREE_PHASE
INT16S SignalStrength   = 0;
INT8U  GPRS_Status_Time = 0;
INT8U  GPRS_Status      = 0;
#endif

static void UI_RamParaChk(void);
static void UI_DefaultInit(void);

/*"************************************************"*/
/*"Function:    UI_DefaultInit"*/
/*"Description: Ĭ�ϲ�����ʼ��"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
static void UI_DefaultInit(void)
{
    /*��RAM���ݲ��� �ָ�Ĭ��ֵ*/
    UI_LoadDefaultUiPara();
    FS_WriteFile(E_FILE_UI_PARA_WITH_BACKUP,0,(INT8U *)&StUIParaWithRamBackup,sizeof(ST_UI_PARA_WITH_BACKUP));

    /*��RAM���ݲ����ָ�Ĭ��ֵ*/
    FS_WriteFile(E_FILE_UI_PARA_NO_BACKUP, 0, (INT8U *)&DefaultUiParaNoRamBackup, sizeof(ST_UI_PARA_NO_BACKUP));
}

static void UI_RamParaChk(void)
{
    INT8U i;
    INT8U Ret;
    CRC_DATA_TP crc;
        
    Ret = LIB_ChkDataValid((INT8U *)&StUIParaWithRamBackup,FIND(ST_UI_PARA_WITH_BACKUP, CrcValue),StUIParaWithRamBackup.CrcValue);
    if(Ret != TRUE)
    {
        for(i = 0; i < READ_MAX_CNT; i++)
        {
            FS_ReadFile(E_FILE_UI_PARA_WITH_BACKUP, 0, (INT8U *)&StUIParaWithRamBackup, sizeof(ST_UI_PARA_WITH_BACKUP));

            crc = CRC16((INT8U *)&StUIParaWithRamBackup, FIND(ST_UI_PARA_WITH_BACKUP, CrcValue));
            if(crc == StUIParaWithRamBackup.CrcValue)
            {
                break;
            }
            MCU_DelayMs(5);
        }

        if(i >= READ_MAX_CNT) /*"ȡ�ڿ�����ʧ��ʱ����ȱʡ����"*/
        {
            UI_LoadDefaultUiPara();
        }
    }
}

/*"************************************************"*/
/*"Function:    UI_Init"*/
/*"Description: �û��ӿڳ�ʼ��"*/
/*"Input: mode �ϵ�or ��������ģʽ"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/

void UI_Init(INT8U mode)
{

    ST_7TIME_FORM SysTime;
    ST_6TIME_FORM PdTime;
    INT32S pdSec;
    INT32U TmpClockBatWorkTimeMin;
    
    if(FIRST_PWR_ON == mode)
    {
        UI_DefaultInit();
    }
    
    if(MCU_RUNMODE == mode)/*����������ʱ��*/
    {
        GetSingle(E_PD_TIME, (INT8U *)&PdTime);/*��ȡ��һ�ε���ʱ��*/
        GetSingle(E_SYS_TIME, (INT8U *)&SysTime);/*��ȡ��ǰʱ��*/
        pdSec =  LIB_CmpTime((ST_6TIME_FORM*)&SysTime, &PdTime);
        if((pdSec>=60) && (FALSE == ChkStatus(E_MTR_RST))
           #ifdef SINGLE_PHASE /*������ض������磬RAM��ʧʱ�ӵ�ؿ��ܴ���*/
            &&(0x5a5a == StUIParaWithRamBackup.validFlg)
           #endif
           )/*"�Է���Ϊ��λ�ۼ�"*/
        {
            pdSec /= 60;
            FS_ReadFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin), (INT8U *)&TmpClockBatWorkTimeMin, 4);
            TmpClockBatWorkTimeMin += pdSec;
            FS_WriteFile(E_FILE_UI_DATA, FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin), (INT8U *)&TmpClockBatWorkTimeMin, 4);
        }
    }

    UI_RamParaChk();/*"UI��RAM���ݲ�����ʼ��"*/  

    DM_Init(mode);/*"��ʼ����ʾ����ģ��"*/  
    TCA_Init(mode); /*˫о��֤��ʼ��*/
    if((MCU_RUNMODE == mode) || (FIRST_PWR_ON == mode))
    {
        Report_Init();
        AXDR_DataLenGetInit(LenGet_PP_698,0);
    }

   
}

#ifdef THREE_PHASE
/*****************************************************************************
 �� �� ��  : Change_GPRS_Value_Status
 ��������  : ��698��ֵת�����źŸ�,����ŵ���ǵ��źŸ��������ֱ��Ӧ5��4��3��2��1
��1
 �������  : SINT16 Value  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��22��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Change_GPRS_Value_Status(INT16S Value)
{
    INT8U Status_Value;

    if(Value > -85)
    {
       Status_Value = 0x80+4;
    }    
    else if((Value>-90)&&(Value<=-85))
    {
       Status_Value = 0x80+3; 
    }  
    else if((Value>-95)&&(Value<=-90))
    {
       Status_Value = 0x80+2; 
    }
    else if((Value>-100)&&(Value<=-95))
    {
       Status_Value = 0x80+1; 
    }
    else if((Value>=-105)&&(Value<=-100))
    {
       Status_Value = 0x80+0;
    }   
    else
    {
       return 0;
    }    

    return Status_Value;
    
}   

/*****************************************************************************
 �� �� ��  : UI_GprsStatusTime
 ��������  : GPRS��ʱ����ˢ��״̬
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��6��22��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void UI_GprsStatusTime(void)
{    
    if(GPRS_Status_Time)
    {        
        GPRS_Status_Time--;    
        if(0 == GPRS_Status_Time)
        {
            GPRS_Status    = 0; 
            SignalStrength = 0;
        }    
    }

    if((GPRS_Status&0x80) != 0)
        DIS_SetStauSeg(STA_STATION);
    else        
        DIS_ClrStauSeg(STA_STATION);
    
    switch((GPRS_Status&0x07))
    {
        #if 0  /*��defaultһ��*/
        case 0:    
            DIS_ClrStauSeg(STA_SIGNAL_1);
            DIS_ClrStauSeg(STA_SIGNAL_2);
            DIS_ClrStauSeg(STA_SIGNAL_3);
            DIS_ClrStauSeg(STA_SIGNAL_4);
            break;
        #endif    
        case 1:
            DIS_SetStauSeg(STA_SIGNAL_1);
            DIS_ClrStauSeg(STA_SIGNAL_2);
            DIS_ClrStauSeg(STA_SIGNAL_3);
            DIS_ClrStauSeg(STA_SIGNAL_4);
            break;
        case 2:
            DIS_SetStauSeg(STA_SIGNAL_1);
            DIS_SetStauSeg(STA_SIGNAL_2);
            DIS_ClrStauSeg(STA_SIGNAL_3);
            DIS_ClrStauSeg(STA_SIGNAL_4);
            break;
        case 3:
            DIS_SetStauSeg(STA_SIGNAL_1);
            DIS_SetStauSeg(STA_SIGNAL_2);
            DIS_SetStauSeg(STA_SIGNAL_3);
            DIS_ClrStauSeg(STA_SIGNAL_4);            
            break;
        case 4:
            DIS_SetStauSeg(STA_SIGNAL_1);
            DIS_SetStauSeg(STA_SIGNAL_2);
            DIS_SetStauSeg(STA_SIGNAL_3);
            DIS_SetStauSeg(STA_SIGNAL_4);
            break;
        default: 
            DIS_ClrStauSeg(STA_SIGNAL_1);
            DIS_ClrStauSeg(STA_SIGNAL_2);
            DIS_ClrStauSeg(STA_SIGNAL_3);
            DIS_ClrStauSeg(STA_SIGNAL_4);
            break;
   }
}    
#endif

/*"************************************************"*/
/*"Function:    UI_Task"*/
/*"Description: �û��ӿ�����"*/
/*"Input: ��"*/
/*"Output: ��"*/
/*"Return: ��"*/
/*"Others: "*/
/*"************************************************"*/
void UI_Task(void)
{
    if(TRUE == Task_ChkTsk_1s(MAINTASK_UI))/*"������"*/
    {
		OC_TaskSec();
        #ifdef THREE_PHASE
        UI_GprsStatusTime();
        #endif
        #ifdef REPORT645 
        Report645Status_Get();
        Report_1Sec();
		
        #endif

    }
    else
    {
    }

    if(TRUE == Task_ChkTsk_1min(MAINTASK_UI))/*"��������"*/
	{
	    UI_RamParaChk();
	    TCA_OnEvyMin();
	}
	else
	{
	}


}


void UI_LoadDefaultUiPara(void)
{
    LIB_ConstMemCpy((INT8U *)&DefaultUiParaWithRamBackup, (INT8U *)&StUIParaWithRamBackup,sizeof(ST_UI_PARA_WITH_BACKUP));
    StUIParaWithRamBackup.CrcValue = CRC16((INT8U *)&StUIParaWithRamBackup, FIND(ST_UI_PARA_WITH_BACKUP, CrcValue));
}

/*����״̬�����*/


const EN_MTR_STATUS  RunStatus1Tab[]=
{
    /*����״̬��1��Ӧ��*/
    E_STA_NO_DEF,/*����*/
    E_DEMAND_MODE,
    E_CLK_BAT_LOSS,
    E_DIS_BAT_LOSS,
    E_PWR_P_DIR,
    E_PWR_Q_DIR,
    E_STA_NO_DEF,/*����*/
    E_STA_NO_DEF,/*����*/
    
    E_RELAY_ERR,/*���ƻ�·�������⴦��*/
    E_ESAM_ERR,
    E_STA_NO_DEF,/*����*/
    E_STA_NO_DEF,/*����*/
    E_STA_NO_DEF,/*�ڲ��������δ��*/
    E_STA_NO_DEF,/*�洢������ �����⴦��*/
    E_OVERDRAFT,
    E_CLK_ERR,
    
};


const EN_MTR_STATUS  RunStatus2Tab[]=
{
    /*����״̬��2��Ӧ��*/
    #if defined (SINGLE_PHASE)
    E_PWR_P_DIR,/*�����ֻ���������״̬*/
    #else
    E_PWR_P_LA_DIR,
    #endif
    E_PWR_P_LB_DIR,
    E_PWR_P_LC_DIR,
    E_STA_NO_DEF,
    E_PWR_Q_LA_DIR,
    E_PWR_Q_LB_DIR,
    E_PWR_Q_LC_DIR,    
};


const EN_MTR_STATUS  RunStatus3Tab[]=
{
    /*����״̬��3��Ӧ��*/
    E_STA_NO_DEF,/*����*/
    E_AUX_PWR,
    E_BAT_PWR,
    E_IRAUTH_STATUS,/*���*/
    E_RELAY_STATUS,
    E_STA_NO_DEF,/*����*/
    E_RELAY_CMD_STATUS,
    E_RELAY_ALARM,
    
    E_PREPAY_ENGY,
    E_PREPAY_MONEY,
    E_STA_NO_DEF,/*����*/
    E_STA_NO_DEF,/*����*/
    E_RELAY_PROTECT,
    E_AUTH_STATUS,
    E_LOCAL_CREDIT,
    E_REMOTE_CREDIT,

};

/*A�����״̬*/
const EN_MTR_STATUS  RunStatus4Tab[]=
{
    /*����״̬��4��Ӧ��*/
    E_LOSS_VOLT_LA,
    E_UNDER_VOLT_LA,
    E_OVER_VOLT_LA,
    E_LOSS_CURR_LA,
    E_OVER_CURR_LA,
    E_OVER_LOAD_LA,
    #if defined (SINGLE_PHASE)
    E_PWR_P_DIR,/*�����ֻ���������״̬*/
    #else
    E_PWR_REVERSE_LA,    
    #endif
    E_BRK_PHASE_LA,
    
    E_CUT_CURR_LA,
};


/*B�����״̬*/
const EN_MTR_STATUS  RunStatus5Tab[]=
{
    /*����״̬��5��Ӧ��*/
    E_LOSS_VOLT_LB,
    E_UNDER_VOLT_LB,
    E_OVER_VOLT_LB,
    E_LOSS_CURR_LB,
    E_OVER_CURR_LB,
    E_OVER_LOAD_LB,
    E_PWR_REVERSE_LB,    
    E_BRK_PHASE_LB,
    
    E_CUT_CURR_LB,
};


/*C�����״̬*/
const EN_MTR_STATUS  RunStatus6Tab[]=
{
    /*����״̬��6��Ӧ��*/
    E_LOSS_VOLT_LC,
    E_UNDER_VOLT_LC,
    E_OVER_VOLT_LC,
    E_LOSS_CURR_LC,
    E_OVER_CURR_LC,
    E_OVER_LOAD_LC,
    E_PWR_REVERSE_LC,    
    E_BRK_PHASE_LC,
    
    E_CUT_CURR_LC,
};


/*�������״̬*/
const EN_MTR_STATUS  RunStatus7Tab[]=
{
    E_VOLT_REVERSE,
    E_CURR_REVERSE,
    E_VOLT_IMBAL,
    E_CURR_IMBAL,
    E_STA_NO_DEF,/*������Դʧ�粻����*/
    E_PWR_DOWN,
    E_OVER_DEMAND,           
    E_OVER_FACTOR,

    E_CURR_BADLY_IMBAL,
    E_OPEN_COVER,
    E_OPEN_TAIL,
};

/*����о����״̬�� */
const EN_MTR_STATUS  MeterSampleStatusTab[]=
{
    E_OPEN_COVER,   /*bit0*/
    E_PERMISS_LIMIT,   /*bit1*/
    E_STA_NO_DEF,
    E_STA_NO_DEF,
    E_STA_NO_DEF,//E_FLASH_ERR,    /*�ڲ�������� ��ô����*/ 
    E_EEP_ERR,      /*�洢�����ϻ���*/
    E_STA_NO_DEF,           
    E_CLK_ERR,      /*ʱ�ӹ���*/
};

/*����״̬��*/
const EN_MTR_STATUS  SampleStatusTab[]=
{
    E_PWR_P_DIR,        /*bit0  ���й����ʷ���*/
    E_PWR_P_LA_DIR,     /*bit1  A���й����ʷ���*/
    E_PWR_P_LB_DIR,     /*B���й����ʷ���*/
    E_PWR_P_LC_DIR,     /*C���й����ʷ���*/
    E_PWR_Q_DIR,        /*���޹����ʷ���*/
    E_PWR_Q_LA_DIR,     /*A���޹����ʷ���*/
    E_PWR_Q_LB_DIR,     /*B���޹����ʷ���*/    
    E_PWR_Q_LC_DIR,     /*C���޹����ʷ���*/

    E_CURR_BADLY_IMBAL, /*����*/
    E_CURR_BADLY_IMBAL, /*A��Ǳ��*/
    E_CURR_BADLY_IMBAL, /*B��Ǳ��*/
    E_CURR_BADLY_IMBAL,  /*C��Ǳ��*/
    E_VOLT_REVERSE,     /*��ѹ������*/
    E_CURR_REVERSE,     /*����������*/
};


INT8U UI_GetRunStatues(INT8U *pdata)
{
    INT8U i;
    
    BUILD_BUG_ON(MAX_MTR_STA>=0xFF);

    /*������״̬��*/
    LIB_MemSet(0,(INT8U *)&RunStatus,sizeof(RunStatus));  
#ifdef PM_MODLE            
	Action(E_PMRUNSTA_FRESH,NULL,NULL);/*"ˢ��Ԥ��������״̬��"*/
#endif
    /*ˢ��״̬��1*/
    for(i=0;i<sizeof(RunStatus1Tab);i++)
    {
        if(RunStatus1Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus1Tab[i]))
            {
                RunStatus.word1.word |= (0x0001<<i);
            }
        }
    }
    #if defined (SINGLE_PHASE) 
    /*�������Ƿѹʱ��ͣ�糭����Ƿѹ��ʱ�ӵ��ǷѹҲӦ��λ*/
    RunStatus.word1.bits.lowPwrBat = RunStatus.word1.bits.clockBat;
    #endif

    /*״̬��1�� �洢������*/
    if((TRUE == ChkStatus(E_EEP_ERR)) ||(TRUE == ChkStatus(E_FLASH_ERR) ) )
    {
        RunStatus.word1.bits.RomErr = 1;
    }

    /*ˢ��״̬��2*/
    for(i=0;i<sizeof(RunStatus2Tab);i++)
    {
        if(RunStatus2Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus2Tab[i]))
            {
                RunStatus.word2.word |= (0x0001<<i);
            }
        }
    }

    /*ˢ��״̬��3*/
    for(i=0;i<sizeof(RunStatus3Tab);i++)
    {
        if(RunStatus3Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus3Tab[i]))
            {
                RunStatus.word3.word |= (0x0001<<i);
            }
        }
    }
	
    /*ˢ��״̬��4*/
    for(i=0;i<sizeof(RunStatus4Tab);i++)
    {
        if(RunStatus4Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus4Tab[i]))
            {
                RunStatus.word4.word |= (0x0001<<i);
            }
        }
    }

    #ifdef THREE_PHASE
    /*ˢ��״̬��5*/
    for(i=0;i<sizeof(RunStatus5Tab);i++)
    {
        if(RunStatus5Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus5Tab[i]))
            {
                RunStatus.word5.word |= (0x0001<<i);
            }
        }
    }

    /*ˢ��״̬��6*/
    for(i=0;i<sizeof(RunStatus6Tab);i++)
    {
        if(RunStatus6Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus6Tab[i]))
            {
                RunStatus.word6.word |= (0x0001<<i);
            }
        }
    }
    #endif

    /*ˢ��״̬��7*/
    for(i=0;i<sizeof(RunStatus7Tab);i++)
    {
        if(RunStatus7Tab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(RunStatus7Tab[i]))
            {
                RunStatus.word7.word |= (0x0001<<i);
            }
        }
    }

    /*ˢ�� ����о����״̬��*/
    MeterSampleStatus.word = 0;
    for(i=0;i<sizeof(MeterSampleStatusTab);i++)
    {
        if(MeterSampleStatusTab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(MeterSampleStatusTab[i]))
            {
                MeterSampleStatus.word |= (0x0001<<i);
            }
        }
    }

    if(TRUE == ChkStatus(E_FLASH_ERR))
    {
        MeterSampleStatus.word |= (0x0001<<5);   /*�ⲿFLASHҲ��洢�����ϻ���*/
    }        
    /*ˢ�� ����о����״̬�� end*/    
    /*--------------------------------------------*/
    /*ˢ�� ����״̬��*/
    SampleStatus.word = 0;
    for(i=0;i<sizeof(SampleStatusTab);i++)
    {
        if(SampleStatusTab[i]!= E_STA_NO_DEF)
        {
            if(TRUE == ChkStatus(SampleStatusTab[i]))
            {
                SampleStatus.word |= (0x0001<<i);
            }
        }
    }
    
    return TRUE;
}

/*"*****************************************************************************"*/
/*"  Function:       UI_ReadRlyStatus"*/
/*"  Description:  ��ȡ�̵��������״̬"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata ���״̬����"*/
/*"  Output          "*/
/*"  Return:         DAR_READ_FUN_DIRECT_DATA"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U UI_ReadRlyStatus(INT8U *pdata)
{
    INT8U result;
	ST_OAD mstOad;

	GetSingle(E_PP_OAD,(INT8U *)&mstOad);
    result = DAR_READ_FUN_DIRECT_DATA;
	
    *pdata = 0x00;
#ifdef PM_MODLE            
	Action(E_PMRUNSTA_FRESH,NULL,NULL);/*"ˢ��Ԥ��������״̬��"*/
#endif
    if(0x8000 == mstOad.Oi.asInt)
    {
        switch(mstOad.AId)
        {
        case 4:/*"�澯״̬"*/
			if(ChkStatus(E_RELAY_ALARM) == TRUE)
            {
                *pdata = 0x01;
            }
            break;
        case 5:/*"�̵�������״̬"*/
            if(ChkStatus(E_RELAY_CMD_STATUS) == TRUE)
            {
                *pdata = 0x01;
            }
            break;
        default:
            result = FALSE;
            break;
        }
    }
    else if(0x8001 == mstOad.Oi.asInt)
    {
        switch(mstOad.AId)
        {
        case 2:/*"����״̬"*/
			if(ChkStatus(E_RELAY_PROTECT) == TRUE)
            {
                *pdata = 0x01;
            }
            break;
        default:
            result = FALSE;
            break;
        }
    }
    else
    {
        result = FALSE;
    }

    return result;
	
}

/*"************************************************			
Function: 			
Description: 	F205_�̵������			
Input: pdin: OAD IID =1 ��ʾ��բ�̵��� IID=2��ʾ�澯�̵��� 	
Output: ����
Return: ���ػ�ȡ���ݳ���	
Others: 
************************************************"*/	
INT8S UI_GetRelayOutput(void *pdin, void *pdout)
{
    ST_RLY_STA tempRlySta;
    INT8U RelayType=0;
    INT8S RetLen;

            
    /*������*/
    tempRlySta.name =  '0';/*��������OADԪ��������Ӧ*/ 

    /*��ǰ״̬ ��բ0 ��բ1*/
    if(TRUE == ChkStatus(E_RELAY_STATUS))/*"�̵������״̬"*/
    {
        tempRlySta.status = 1;
    }
    else
    {
        tempRlySta.status = 0;
    }
    /*�������� ����ʽ 0 ����ʽ 1*/
#ifdef PM_MODLE            
    GetSingle(E_RELAY_TYPE, &RelayType);
#else
    RelayType = RLY_OUTSIDE;
#endif 
	RelayType &= RLYTYPE;
    if((RLY_INSIDE == RelayType)||(RLY_PULSEOUTSIDE == RelayType))
    {
        tempRlySta.prop = 0;/*"���û���������ʽ"*/
    }
    else
    {
        tempRlySta.prop = 1;/*"���õ�ƽ"*/
    }
    /*����״̬ 0 ���� 1 δ����*/
    tempRlySta.lineSta = 0;/*����*/
    LIB_MemCpy((INT8U *)&tempRlySta, pdout, sizeof(ST_RLY_STA));
    RetLen = sizeof(ST_RLY_STA);

    return RetLen;

    
}


/*"*****************************************************************************"*/
/*"  Function:       UI_DataClr"*/
/*"  Description:    �û��ӿ�����������"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdin: �ޣ�pdout:  ��"*/
/*"  Output          "*/
/*"  Return:         DAR"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8S UI_DataClr(void *pdin, void *pdout)
{
    INT32U DayStdNow;

	DayStdNow = 0;
	FS_WriteFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,LastBroadcastAdjTime), (INT8U *)&DayStdNow, 4);/*"��Уʱʱ��'*/
    FS_WriteFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin), (INT8U *)&DayStdNow, 4);/*��ʱ�ӵ�ع���ʱ��*/

    return MS_SUCCESS;

}

