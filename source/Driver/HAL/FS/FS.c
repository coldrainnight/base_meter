
#include "FS.h"
#include "Driver\Hal\DIS\DIS.h"
#include "FS_Def.h"
#include  "App\record\RCRD.h"
#ifdef THREE_PHASE
#include "App\demand\DU.h"
#endif


#if defined (FS_EEP_EN)
#include "FS_EepConfig.h"
#endif

#if defined (FS_FLASH_EN)
#include "FS_FlashConfig.h"
#endif


INT8U FS_Init(INT8U Mode);
INT8U FS_WriteFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
INT8U FS_ReadFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
INT8U FS_DelFile(ENUM_FS_ID FileName,INT32U StartOffset, INT32U Len);


#ifdef FS_EEP_EN

/**EEP����***/
static INT8U FS_EepInit(INT8U Mode);
static INT8U FS_WriteEepFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
static INT8U FS_ReadEepFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
static INT8U FS_DelEepFile(ENUM_FS_ID FileName,INT32U StartOffset, INT32U Len);

#endif

#ifdef FS_FLASH_EN

/**FLASH����***/
static INT8U FS_FlashInit(INT8U Mode);
static INT8U FS_WriteFlashFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
static INT8U FS_ReadFlashFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len);
static INT8U FS_DelFlashFile(ENUM_FS_ID FileName,INT32U StartOffset, INT32U Len);
#endif

const INT8U DM_ClrCode[8]=
{
        DIS_Space,DIS_C,DIS_L,DIS_r,DIS_C,DIS_L,DIS_r,DIS_Space
};
INT8U FS_Init(INT8U Mode)
{
    ST_SHOW_DATA showData;
	INT32U InitFlg;

    if(Mode == FIRST_PWR_ON)
    {
        LIB_MemSet(0, (INT8U *) &showData, sizeof(showData));
        showData.Protc.stShowTable.stShowFormat.format = FMT_STRING; /*"��ʾ��ʽΪ �ַ���"*/
        LIB_ConstMemCpy(DM_ClrCode, showData.Protc.showDataBuff, 8);
        DIS_PrintLcdLine1(&showData, 3, LEVEL_3);/*"��ʾˢ��"*/
    	InitFlg = 0;/*"��ʼ��ǰ���ƻ��ڿ���ʼ����־"*/
    	FS_WriteFile(E_FILE_MNT, NEW_EEP_CHK_FLG_SHIFT, (INT8U *) &InitFlg, 4);
    }

    #ifdef FS_EEP_EN
    
    if( FALSE == FS_EepInit(Mode))
    {
        return FALSE;
    }
    
    #endif
    
    #ifdef FS_FLASH_EN
    
    return FS_FlashInit(Mode);
    
    #else
    return TRUE;
    #endif
}


INT8U FS_WriteFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len)
{
    INT8U Ret;
    
    if(FileName>=E_FILE_FLASH_START)/*Flash���ļ�*/
    {
        #ifdef FS_FLASH_EN
        Ret = FS_WriteFlashFile(FileName, Offset, pDataBuff, Len); 
        #else
        Ret =FALSE;
        #endif
    }
    else/*EEP ���ļ�*/
    {
        Ret = FS_WriteEepFile(FileName, Offset, pDataBuff, Len);

    }
    return Ret;
}


INT8U FS_ReadFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len)
{
    INT8U Ret;

    if(FileName>=E_FILE_FLASH_START)/*Flash���ļ�*/
    {
        #ifdef FS_FLASH_EN
        Ret = FS_ReadFlashFile(FileName, Offset, pDataBuff, Len); 
        #else
        Ret =FALSE;
        #endif
    }
    else/*EEP ���ļ�*/
    {
        Ret = FS_ReadEepFile(FileName, Offset, pDataBuff, Len);
    }
    return Ret;
}

INT8U FS_DelFile(ENUM_FS_ID FileName,INT32U StartOffset, INT32U Len)
{
    
    INT8U Ret;

    if(FileName>=E_FILE_FLASH_START)/*Flash���ļ�*/
    {
        #ifdef FS_FLASH_EN
        Ret = FS_DelFlashFile(FileName, StartOffset, Len); 
        #else
        Ret =FALSE;
        #endif
    }
    else/*EEP ���ļ�*/
    {
        Ret = FS_DelEepFile(FileName, StartOffset, Len);
    }
    return Ret;

}


#ifdef FS_EEP_EN
/**EEP����***/


const ST_EEP_FILE_INFO EepFileInfo[] = 
{
    /*����*/
    {E_FILE_LRM,         LRM_FILE_SIZE,           LRM_EEP_ADDR},
    {E_FILE_UI_PARA_WITH_BACKUP,     UI_PARA_NO_BACKUP_SIZE,            UI_PARA_WITH_BACKUP_EEP_ADDR},
    {E_FILE_UI_PARA_NO_BACKUP,     UI_PARA_NO_BACKUP_SIZE,            UI_PARA_NO_BACKUP_EEP_ADDR},
    {E_FILE_PM_PARA,     PM_PARA_SIZE,            PM_PARA_EEP_ADDR},
    {E_FILE_TR_PARA,     TR_PARA_SIZE,            TR_PARA_EEP_ADDR},
    {E_FILE_RCRD_PARA,   RCRD_PARA_SIZE,          RCRD_PARA_EEP_ADDR},
    {E_FILE_MNT_PARA,    MNT_PARA_SIZE,           MNT_PARA_EEP_ADDR},
    
    #if defined (THREE_PHASE)
    {E_FILE_DU_PARA,     DU_PARA_SIZE,            DU_PARA_EEP_ADDR},
    #endif
    
    /*����*/
    {E_FILE_LRM_DATA,    LRM_DATA_FILE_SIZE,      LRM_DATA_EEP_ADDR},
    {E_FILE_PD_DATA,     PD_SAVE_SIZE,           PD_SAVE_EEP_ADDR},

    {E_FILE_UI_DATA,     UI_DATA_SIZE,            UI_DATA_EEP_ADDR},
    {E_FILE_MNT,         MNT_FILE_SIZE,           MNT_EEP_ADDR},
    
    #if defined (THREE_PHASE)
    {E_FILE_DU_DATA,     DU_DATA_SIZE,            DU_DATA_EEP_ADDR},
    #endif
    
    {E_FILE_PM_DATA,     PM_DATA_SIZE,            PM_DATA_EEP_ADDR},
    {E_FILE_RCRD_DATA,   RCRD_DATA_SIZE,          RCRD_DATA_EEP_ADDR },
    {E_FILE_AUTO_DATA,   AUTO_DATA_SIZE,          AUTO_DATA_EEP_ADDR },


    /*��¼���մ洢�ļ�����*/
    #if defined (SINGLE_PHASE)
    {E_FILE_MIN_FRZ_IDX,MIN_FRZ_IDX_SZ,MIN_FRZ_IDX_ADDR},/*���Ӷ�������*/


    #elif defined (THREE_PHASE)
    {E_FILE_PRG_EVNT,PRG_SZ,PRG_ADDR},/*����¼���¼*/
    {E_FILE_OSCILLATOR_ERR_EVNT,OSCILLATOR_ERR_EVT_SZ,OSCILLATOR_ERR_EVT_ADDR},/*" ��������¼� "*/
    {E_FILE_SYS_RST_EVNT,SYS_RST_EVNT_SZ,SYS_RST_EVT_ADDR},/*��λ�����¼�*/
    {E_FILE_MONTH_BILL_FRZ,MONTH_BILL_SZ,MONTH_BILL_ADDR},/*�½��㶳��*/
    {E_FILE_MIN_FRZ_IDX,MIN_FRZ_IDX_SZ,MIN_FRZ_IDX_ADDR},/*���Ӷ�������*/

    #endif
};


static INT8U FS_EepInit(INT8U Mode)
{
    INT16U i;
    INT16U j;
    INT8U Ret;

    Ret = TRUE;
    
    EEP_Init();/*��EEP*/

    if(Mode == FIRST_PWR_ON)
    {
        if(FALSE ==  EEPSize_Compara())
        {
            Ret = FALSE;
        }
    }
    
    
    /*�����������Ƿ����ص�*/
    for(i = 0 ;i < ARRAY_SIZE(EepFileInfo); i++)
    {
        for(j= (i+1);j < ARRAY_SIZE(EepFileInfo);j++)
        {
            if((EepFileInfo[i].Addr >= EepFileInfo[j].Addr) &&
                (EepFileInfo[i].Addr < (EepFileInfo[j].Addr + EepFileInfo[j].Size))
                )
            {
               Ret = FALSE;
               break;
            }

        }

    }

    /*���EepFileInfo�Ƿ��� ENUM_FS_ID˳����д,������Ҳ�����Ӧ�ļ���*/
    for(i=0;i < ARRAY_SIZE(EepFileInfo); i++)
    {
        if((EepFileInfo[i].FileId != i) || (EepFileInfo[i].FileId >= E_FILE_FLASH_START))
        {
            Ret = FALSE;
            break;
        }
    }
    
    /*�ļ������Ƿ񳬹����EEP�ɷ�������*/
    for(i=0;i<ARRAY_SIZE(EepFileInfo);i++)
    {
        if((EepFileInfo[i].Addr + EepFileInfo[i].Size) > EEP_DEV_MAX_ADDR)
        { 
            Ret = FALSE;
            break;
        }
    }
    /*�ڿ�����ַ����*/
    BUILD_BUG_ON(DATA_EEP_ADDR_END>EEP_DEV_MAX_ADDR);

    /*�ļ��������岻һ��*/
    BUILD_BUG_ON(ARRAY_SIZE(EepFileInfo) != E_FILE_EEP_END);

    /*ȷ���������Ϊ��ҳ��ʼ*/
    BUILD_BUG_ON((PD_SAVE_EEP_ADDR%128) != 0);

    return Ret;
}


static INT8U FS_WriteEepFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len)
{
    EEP_ADDR_TYPE Tmp_Addr;
    INT8U Ret = FALSE;

    if(EepFileInfo[FileName].FileId == FileName)
    {
        Tmp_Addr = EepFileInfo[FileName].Addr;

        if((Offset + Len) <= EepFileInfo[FileName].Size)/*"ֻ�ڱ��ļ��ڽ��в���"*/
        {
            Ret = EEP_Write(Tmp_Addr + Offset, pDataBuff,Len);
        }
        else
        {
            #ifdef DEBUG_FS
            Debug.printf("Warning!!! Writefile over the boundary\n");
            #endif 
        }           
    }
    else
    {
    }
    return Ret;

}

static INT8U FS_ReadEepFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len)
{
    EEP_ADDR_TYPE Tmp_Addr;
    INT8U Ret = FALSE;

    if(EepFileInfo[FileName].FileId == FileName)
    {
        Tmp_Addr = EepFileInfo[FileName].Addr;

        if((Offset + Len) <= EepFileInfo[FileName].Size)/*"ֻ�ڱ��ļ��ڽ��в���"*/
        {
            Ret = EEP_Read(Tmp_Addr + Offset, pDataBuff,Len);
        }
        else
        {
            #ifdef DEBUG_FS
            Debug.printf("Warning!!! Writefile over the boundary\n");
            #endif 
        }           
    }
    else
    {

    }
    return Ret;


}
static INT8U FS_DelEepFile(ENUM_FS_ID FileName,INT32U Offset, INT32U Len)
{
    EEP_ADDR_TYPE Tmp_Addr;
    INT8U Ret = FALSE;

    if(EepFileInfo[FileName].FileId == FileName)
    {
        Tmp_Addr = EepFileInfo[FileName].Addr;

        if((Offset + Len) <= EepFileInfo[FileName].Size)/*"ֻ�ڱ��ļ��ڽ��в���"*/
        {
            Ret = EEP_Write(Tmp_Addr + Offset, NULL,Len);
        }
        else
        {
            #ifdef DEBUG_FS
            Debug.printf("Warning!!! Writefile over the boundary\n");
            #endif 
        }           
    }
    else
    {
    }
    
    return Ret;

}

#endif










#ifdef FS_FLASH_EN

const ST_FLASH_FILE_INFO FlashFileInfo[] = 
{
    #if defined (SINGLE_PHASE)
    {E_FILE_OVER_CURR_LA_EVNT ,OVER_CURR_LA_SZ,OVER_CURR_LA_ADDR},/*����A*/
    {E_FILE_PWR_DOWN_EVNT,PWR_DOWN_SZ,PWR_DOWN_ADDR}, /*����*/  
    {E_FILE_MTR_COVER_OPEN_EVNT,MTR_COVER_OPEN_SZ,MTR_COVER_OPEN_ADDR},/*�����¼�*/
    {E_FILE_RELAY_TRIP_EVNT,RELAY_TRIP_SZ,RELAY_TRIP_ADDR},/*��բ�¼��¼�*/
    {E_FILE_RELAY_CLOSE_EVNT,RELAY_CLOSE_SZ,RELAY_CLOSE_ADDR},/*��բ�¼��¼�*/
    
    {E_FILE_PRG_EVNT,PRG_SZ,PRG_ADDR},/*���*/
    {E_FILE_MTR_CLR_EVNT,MTR_CLR_SZ,MTR_CLR_ADDR}, /*�������*/
    {E_FILE_EVNT_CLR_EVNT,EVNT_CLR_SZ,EVNT_CLR_ADDR},/*�¼�����*/
    {E_FILE_JUST_TIME_EVNT,JUST_TIME_SZ,JUST_TIME_ADDR},/*Уʱ�¼�*/
    {E_FILE_BROADCAST_JUST_TIME_EVNT,BROADCAST_JUST_TIME_SZ,BROADCAST_JUST_TIME_ADDR},/*�㲥Уʱ�¼�*/
    {E_FILE_DAY_TBL_PRG_EVNT,DAY_TBL_PRG_SZ,DAY_TBL_PRG_ADDR},/*��ʱ�α���*/
    {E_FILE_YEAR_TBL_PRG_EVNT,YEAR_TBL_PRG_SZ,YEAR_TBL_PRG_ADDR},/*��ʱ������*/
    {E_FILE_WEEKEND_PRG_EVNT,WEEKEND_PRG_SZ,WEEKEND_PRG_ADDR},/*�����ձ���¼�*/
    {E_FILE_HOLIDAY_PRG_EVNT,HOLIDAY_PRG_SZ,HOLIDAY_PRG_ADDR},/*�ڼ��ձ��*/
    {E_FILE_OSCILLATOR_ERR_EVNT,OSCILLATOR_ERR_EVT_SZ,OSCILLATOR_ERR_EVT_ADDR},/*��������¼�*/
    {E_FILE_SYS_RST_EVNT,SYS_RST_EVNT_SZ,SYS_RST_EVT_ADDR},/*��λ�����¼�*/
    {E_FILE_BILL_DAY_PRG_EVNT,BILL_DAY_PRG_SZ,BILL_DAY_PRG_ADDR},/*�����ձ���¼�*/
    {E_FILE_A_ENGY_COM_PRG_EVNT,A_ENGY_COM_PRG_SZ,A_ENGY_COM_PRG_ADDR},/*�й����ģʽ�ֱ���¼�*/
    {E_FILE_TRIFF_PRG_EVNT,TRIFF_PRG_SZ,TRIFF_PRG_ADDR},/*���ʱ����¼�*/
    {E_FILE_STEP_PRG_EVNT,STEP_PRG_SZ,STEP_PRG_ADDR},/*���ݱ����¼�*/
    {E_FILE_KEY_UPDATE_EVNT,KEY_UPDATE_SZ,KEY_UPDATE_ADDR},/*��Կ�����¼�*/
    {E_FILE_CARD_PLUG_IN_ERR_EVNT,CARD_PLUG_IN_ERR_SZ,CARD_PLUG_IN_ERR_ADDR},/*�忨�쳣�¼�*/
    {E_FILE_BUY_ENGY_EVNT,BUY_ENGY_SZ,BUY_ENGY_ADDR},/*�����¼�*/
    {E_FILE_REFUND_EVNT,REFUND_SZ,REFUND_ADDR},/*�˷��¼�*/
    {E_FILE_RELAY_ERR_EVNT,RELAY_ERR_SZ,RELAY_ERR_ADDR},/*�̵�������*/
    {E_FILE_PWR_FAILURE_EVNT,PWR_FAILURE_SZ,PWR_FAILURE_ADDR},/*��Դ�쳣�¼�*/
    {E_FILE_RTC_ERR_EVNT,RTC_ERR_SZ,RTC_ERR_ADDR},
    {E_FILE_MS_ERR_EVNT,MS_ERR_SZ,MS_ERR_ADDR},
    {E_FILE_MGMT_COMM_ERR_EVNT,MGMT_COMM_ERR_SZ,MGMT_COMM_ERR_ADDR},
    {E_FILE_UPGRADE_EVNT,UPGRADE_SZ,UPGRADE_ADDR},
    {E_FILE_NOLEG_REPALCE_EVNT,NOLEG_REPALCE_SZ,NOLEG_REPALCE_ADDR},

    {E_FILE_MMT_FRZ,MMT_FRZ_SZ,MMT_FRZ_ADDR},/*˲ʱ����*/
    
    {E_FILE_MIN_FRZ,MIN_FRZ_SZ,MIN_FRZ_ADDR},/*���Ӷ���*/
    {E_FILE_HOUR_FRZ,HOUR_FRZ_SZ,HOUR_FRZ_ADDR},/*Сʱ����*/
    {E_FILE_DAY_FRZ,DAY_FRZ_SZ,DAY_FRZ_ADDR},/*�ն���*/
    {E_FILE_MONTH_BILL_FRZ,MONTH_BILL_SZ,MONTH_BILL_ADDR},/*�½��㶳��*/
    {E_FILE_MONTH_FRZ, MONTH_FRZ_SZ,MONTH_FRZ_ADDR},/*�¶���*/
    {E_FILE_DAY_TBL_SW_FRZ,DAY_TBL_SW_FRZ_SZ,DAY_TBL_SW_FRZ_ADDR},/*��ʱ�α��л�����*/
    {E_FILE_YEAR_TBL_SW_FRZ,YEAR_TBL_SW_FRZ_SZ,YEAR_TBL_SW_FRZ_ADDR},/*���������л�*/
    {E_FILE_TRIFF_SW_FRZ,TRIFF_SW_FRZ_SZ,TRIFF_SW_FRZ_ADDR},/*���ʵ���л�*/
    {E_FILE_STEP_SW_FRZ,STEP_SW_FRZ_SZ,STEP_SW_FRZ_ADDR}, /*" �����л����� "*/
    {E_FILE_STEP_BILL_FRZ,STEP_BILL_FRZ_SZ,STEP_SW_BILL_ADDR}, /*" ���ݽ��㶳�� "*/
        

    {E_FILE_PD_PON_EVNT, PD_PON_EVT_SZ,PD_PON_EVT_ADDR},
    {E_FILE_MSRL_EVNT, MSR_DATAL_SIZE,MSR_DATAL_ADDR},
    {E_FILE_MSRN_EVNT, MSR_DATAN_SIZE,MSR_DATAN_ADDR},
    #elif defined (THREE_PHASE)
    {E_FILE_PD_PON_EVNT,PD_PON_EVT_SZ,PD_PON_EVT_ADDR}, /*" ���ϵ��¼ "*/
    {E_FILE_PARA_ERR_EVNT,PARA_ERR_EVT_SZ,PARA_ERR_EVT_ADDR}, /*" ���������¼ "*/
    {E_FILE_ENG_ERR_EVNT,ENG_ERR_EVT_SZ,ENG_ERR_EVT_ADDR}, /*" ���������¼ "*/
                
    {E_FILE_LOST_VOLT_LA_EVNT,LOST_VOLT_LA_SZ,LOST_VOLT_LA_ADDR},/*A��ʧѹ*/
    {E_FILE_LOST_VOLT_LB_EVNT,LOST_VOLT_LB_SZ,LOST_VOLT_LB_ADDR},/*B��ʧѹ*/
    {E_FILE_LOST_VOLT_LC_EVNT,LOST_VOLT_LB_SZ,LOST_VOLT_LC_ADDR},/*C��ʧѹ*/

    {E_FILE_LACK_VOLT_LA_EVNT, LACK_VOLT_LA_SZ,LACK_VOLT_LA_ADDR},/*A��Ƿѹ*/
    {E_FILE_LACK_VOLT_LB_EVNT, LACK_VOLT_LB_SZ,LACK_VOLT_LB_ADDR},/*B��Ƿѹ*/
    {E_FILE_LACK_VOLT_LC_EVNT, LACK_VOLT_LC_SZ,LACK_VOLT_LC_ADDR},/*C��Ƿѹ*/

    {E_FILE_OVER_VOLT_LA_EVNT, OVER_VOLT_LA_SZ ,OVER_VOLT_LA_ADDR},/*A���ѹ*/
    {E_FILE_OVER_VOLT_LB_EVNT, OVER_VOLT_LB_SZ ,OVER_VOLT_LB_ADDR,},/*B���ѹ*/
    {E_FILE_OVER_VOLT_LC_EVNT, OVER_VOLT_LC_SZ ,OVER_VOLT_LC_ADDR,},/*C���ѹ*/

    {E_FILE_BRK_PHASE_LA_EVNT  ,BRK_PHASE_LA_SZ,BRK_PHASE_LA_ADDR},/*A�����*/
    {E_FILE_BRK_PHASE_LB_EVNT  ,BRK_PHASE_LB_SZ,BRK_PHASE_LB_ADDR},/*B�����*/
    {E_FILE_BRK_PHASE_LC_EVNT  ,BRK_PHASE_LC_SZ,BRK_PHASE_LC_ADDR},/*C�����*/

    {E_FILE_LOST_CURR_LA_EVNT  ,LOST_CURR_LA_SZ,LOST_CURR_LA_ADDR},/*A��ʧ��*/
    {E_FILE_LOST_CURR_LB_EVNT  ,LOST_CURR_LB_SZ,LOST_CURR_LB_ADDR},/*B��ʧ��*/
    {E_FILE_LOST_CURR_LC_EVNT  ,LOST_CURR_LC_SZ,LOST_CURR_LC_ADDR},/*C��ʧ��*/

    {E_FILE_OVER_CURR_LA_EVNT ,OVER_CURR_LA_SZ,OVER_CURR_LA_ADDR},/*����A*/
    {E_FILE_OVER_CURR_LB_EVNT ,OVER_CURR_LB_SZ,OVER_CURR_LB_ADDR},/*B�����*/
    {E_FILE_OVER_CURR_LC_EVNT ,OVER_CURR_LC_SZ,OVER_CURR_LC_ADDR},/*C�����*/

    {E_FILE_BRK_CURR_LA_EVNT  ,BRK_CURR_LA_SZ,BRK_CURR_LA_ADDR},/*A�����*/
    {E_FILE_BRK_CURR_LB_EVNT  ,BRK_CURR_LB_SZ,BRK_CURR_LB_ADDR},/*B�����*/
    {E_FILE_BRK_CURR_LC_EVNT  ,BRK_CURR_LC_SZ,BRK_CURR_LC_ADDR},/*C�����*/

    {E_FILE_PWR_REVERSE_EVNT  , PWR_REVERSE_SZ,    PWR_REVERSE_ADDR        },/*���ʷ���*/
    {E_FILE_PWR_REVERSE_LA_EVNT,PWR_REVERSE_LA_SZ, PWR_REVERSE_LA_ADDR     },/*A���ʷ���*/
    {E_FILE_PWR_REVERSE_LB_EVNT,PWR_REVERSE_LB_SZ, PWR_REVERSE_LB_ADDR     },/*B���ʷ���*/
    {E_FILE_PWR_REVERSE_LC_EVNT,PWR_REVERSE_LC_SZ, PWR_REVERSE_LC_ADDR     },/*C���ʷ���*/

    {E_FILE_OVER_LOAD_LA_EVNT ,OVER_LOAD_LA_SZ,OVER_LOAD_LA_ADDR},
    {E_FILE_OVER_LOAD_LB_EVNT ,OVER_LOAD_LB_SZ,OVER_LOAD_LB_ADDR},
    {E_FILE_OVER_LOAD_LC_EVNT ,OVER_LOAD_LC_SZ,OVER_LOAD_LC_ADDR},

    {E_FILE_OVER_PA_DEMAND_EVNT ,OVER_PA_DEMAND_SZ,OVER_PA_DEMAND_ADDR},
    {E_FILE_OVER_NA_DEMAND_EVNT ,OVER_NA_DEMAND_SZ,OVER_NA_DEMAND_ADDR},

    {E_FILE_OVER_R_DEMAND_I_EVNT  ,OVER_R_DEMAND_I_SZ,OVER_R_DEMAND_I_ADDR},
    {E_FILE_OVER_R_DEMAND_II_EVNT  ,OVER_R_DEMAND_II_SZ,OVER_R_DEMAND_II_ADDR},
    {E_FILE_OVER_R_DEMAND_III_EVNT ,OVER_R_DEMAND_III_SZ,OVER_R_DEMAND_III_ADDR},
    {E_FILE_OVER_R_DEMAND_IV_EVNT  ,OVER_R_DEMAND_IV_SZ,OVER_R_DEMAND_IV_ADDR},

    {E_FILE_FACTOR_LOW_EVNT  ,FACTOR_LOW_SZ,FACTOR_LOW_ADDR},
    {E_FILE_ALL_LOST_VOLT_EVNT,ALL_LOST_VOLT_SZ,ALL_LOST_VOLT_ADDR},
    {E_FILE_AUX_PWR_DOWN_EVNT,AUX_PWR_DOWN_SZ,AUX_PWR_DOWN_ADDR},
    {E_FILE_VOLT_INVERSE_EVNT,VOLT_INVERSE_SZ,VOLT_INVERSE_ADDR},
    {E_FILE_CURR_INVERSE_EVNT,CURR_INVERSE_SZ,CURR_INVERSE_ADDR},

    {E_FILE_PWR_DOWN_EVNT,PWR_DOWN_SZ,PWR_DOWN_ADDR}, /*����*/  
    {E_FILE_MTR_CLR_EVNT,MTR_CLR_SZ,MTR_CLR_ADDR}, /*�������*/
    {E_FILE_DEMAND_CLR_EVNT,DEMAND_CLR_SZ,DEMAND_CLR_ADDR},
    {E_FILE_EVNT_CLR_EVNT,EVNT_CLR_SZ,EVNT_CLR_ADDR},/*�¼�����*/
    {E_FILE_JUST_TIME_EVNT,JUST_TIME_SZ,JUST_TIME_ADDR},/*Уʱ�¼�*/
    {E_FILE_DAY_TBL_PRG_EVNT,DAY_TBL_PRG_SZ,DAY_TBL_PRG_ADDR},/*��ʱ�α���*/
    {E_FILE_YEAR_TBL_PRG_EVNT,YEAR_TBL_PRG_SZ,YEAR_TBL_PRG_ADDR},/*��ʱ������*/
    {E_FILE_WEEKEND_PRG_EVNT,WEEKEND_PRG_SZ,WEEKEND_PRG_ADDR},/*�����ձ���¼�*/
    {E_FILE_BILL_DAY_PRG_EVNT,BILL_DAY_PRG_SZ,BILL_DAY_PRG_ADDR},/*�����ձ���¼�*/
    {E_FILE_MTR_COVER_OPEN_EVNT,MTR_COVER_OPEN_SZ,MTR_COVER_OPEN_ADDR},/*�����¼�*/
    {E_FILE_TAIL_COVER_OPEN_EVNT,TAIL_COVER_OPEN_SZ,TAIL_COVER_OPEN_ADDR},
    {E_FILE_VOLT_IMBALANCE_EVNT,VOLT_IMBALANCE_SZ,VOLT_IMBALANCE_ADDR},
    {E_FILE_CURR_IMBALANCE_EVNT,CURR_IMBALANCE_SZ,CURR_IMBALANCE_ADDR},

    {E_FILE_RELAY_TRIP_EVNT,RELAY_TRIP_SZ,RELAY_TRIP_ADDR},/*��բ�¼��¼�*/
    {E_FILE_RELAY_CLOSE_EVNT,RELAY_CLOSE_SZ,RELAY_CLOSE_ADDR},/*��բ�¼��¼�*/
    {E_FILE_HOLIDAY_PRG_EVNT,HOLIDAY_PRG_SZ,HOLIDAY_PRG_ADDR},/*�ڼ��ձ��*/
    {E_FILE_A_ENGY_COM_PRG_EVNT,A_ENGY_COM_PRG_SZ,A_ENGY_COM_PRG_ADDR},/*�й����ģʽ�ֱ���¼�*/
    {E_FILE_R_ENGY_COM_PRG_EVNT,R_ENGY_COM_PRG_SZ,R_ENGY_COM_PRG_ADDR},
    {E_FILE_TRIFF_PRG_EVNT,TRIFF_PRG_SZ,TRIFF_PRG_ADDR},/*���ʱ����¼�*/
    {E_FILE_STEP_PRG_EVNT,STEP_PRG_SZ,STEP_PRG_ADDR},/*���ݱ����¼�*/
    {E_FILE_KEY_UPDATE_EVNT,KEY_UPDATE_SZ,KEY_UPDATE_ADDR},/*��Կ�����¼�*/
    {E_FILE_CARD_PLUG_IN_ERR_EVNT,CARD_PLUG_IN_ERR_SZ,CARD_PLUG_IN_ERR_ADDR},/*�忨�쳣�¼�*/
    {E_FILE_BUY_ENGY_EVNT,BUY_ENGY_SZ,BUY_ENGY_ADDR},/*�����¼�*/
    {E_FILE_REFUND_EVNT,REFUND_SZ,REFUND_ADDR},/*�˷��¼�*/
    {E_FILE_CONSTANT_MGNT_DISTURB_EVNT,CONSTANT_MGNT_DISTURB_SZ,CONSTANT_MGNT_DISTURB_ADDR},
    {E_FILE_RELAY_ERR_EVNT,RELAY_ERR_SZ,RELAY_ERR_ADDR},/*�̵�������*/
    {E_FILE_PWR_FAILURE_EVNT,PWR_FAILURE_SZ,PWR_FAILURE_ADDR},/*��Դ�쳣�¼�*/
    {E_FILE_CURR_IMBALANCE_BADLY_EVNT,CURR_IMBALANCE_BADLY_SZ,CURR_IMBALANCE_BADLY_ADDR},
    {E_FILE_RTC_ERR_EVNT,RTC_ERR_SZ,RTC_ERR_ADDR},
    {E_FILE_MS_ERR_EVNT,MS_ERR_SZ,MS_ERR_ADDR},

    {E_FILE_MMT_FRZ,MMT_FRZ_SZ,MMT_FRZ_ADDR},/*˲ʱ����*/
    {E_FILE_MIN_FRZ,MIN_FRZ_SZ,MIN_FRZ_ADDR},/*���Ӷ���*/
    {E_FILE_HOUR_FRZ,HOUR_FRZ_SZ,HOUR_FRZ_ADDR},/*Сʱ����*/
    {E_FILE_DAY_FRZ,DAY_FRZ_SZ,DAY_FRZ_ADDR},/*�ն���*/
    {E_FILE_MONTH_FRZ, MONTH_FRZ_SZ,MONTH_FRZ_ADDR},/*�¶���*/
    {E_FILE_YEAR_TBL_SW_FRZ,YEAR_TBL_SW_FRZ_SZ,YEAR_TBL_SW_FRZ_ADDR},/*���������л�*/
    {E_FILE_DAY_TBL_SW_FRZ,DAY_TBL_SW_FRZ_SZ,DAY_TBL_SW_FRZ_ADDR},/*��ʱ�α��л�����*/
    {E_FILE_TRIFF_SW_FRZ,TRIFF_SW_FRZ_SZ,TRIFF_SW_FRZ_ADDR},/*���ʵ���л�*/
    {E_FILE_STEP_SW_FRZ,STEP_SW_FRZ_SZ,STEP_SW_FRZ_ADDR}, /*" �����л����� "*/
    {E_FILE_STEP_BILL_FRZ,STEP_BILL_FRZ_SZ,STEP_SW_BILL_ADDR}, /*" ���ݽ��㶳�� "*/

    #endif
};
INT8U ibuf[6]={0x01,0x02,0x03,0x04,0x05,0x06};
INT8U obuf[6]={0x00,0x00,0x00,0x00,0x00,0x00};

/**FLASH����***/
static INT8U FS_FlashInit(INT8U Mode)
{
    INT8U Ret;
    INT8U i;
    INT8U j;

    Ret= Flash_Init(Mode);   

    /*�����������Ƿ����ص�*/
    for(i = 0 ;i < ARRAY_SIZE(FlashFileInfo); i++)
    {
        for(j= (i+1);j < ARRAY_SIZE(FlashFileInfo);j++)
        {
            if((FlashFileInfo[i].Addr >= FlashFileInfo[j].Addr) &&
                (FlashFileInfo[i].Addr < (FlashFileInfo[j].Addr + FlashFileInfo[j].Size))
                )
            {
               Ret = FALSE;
               break;
            }
        }
    }


    /*���EepFileInfo�Ƿ��� ENUM_FS_ID˳����д,������Ҳ�����Ӧ�ļ���*/
    for(i=0;i < ARRAY_SIZE(FlashFileInfo); i++)
    {
        if((FlashFileInfo[i].FileId != (i+E_FILE_FLASH_START+1)) || /*�ļ�ID���䲻ƥ��*/
            (FlashFileInfo[i].FileId <= E_FILE_FLASH_START)      || /*�ļ�ID����flash��*/
            ((FlashFileInfo[i].Addr%FLASH_SECTOR_SIZE ) != 0)       /*�ļ���ʼ���ǿ���ʼ*/
            )
        {
            Ret = FALSE;
            break;
        }
    }
    
    /*�ļ������Ƿ񳬹����FALSH�ɷ�������*/
    for(i=0;i<ARRAY_SIZE(FlashFileInfo);i++)
    {
        if((FlashFileInfo[i].Addr+ FlashFileInfo[i].Size) > FLASH_SIZE)
        { 
            Ret = FALSE;
            break;
        }
    }
	
	FS_WriteFile(E_FILE_PD_PON_EVNT,0,ibuf,6);
	FS_ReadFile(E_FILE_PD_PON_EVNT,0,obuf,6);
        
    /*�ڿ�����ַ����*/
    BUILD_BUG_ON(DATA_FLASH_ADDR_END>FLASH_SIZE);

    /*�ļ��������岻һ��*/
    BUILD_BUG_ON(ARRAY_SIZE(FlashFileInfo) != (E_FILE_FLASH_END-E_FILE_FLASH_START-1));

    return Ret;
}


static INT8U FS_WriteFlashFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len)
{
    INT32U  Tmp_Addr;
    INT8U Ret = FALSE;
    INT16U FileInfoId;

    FileInfoId = FileName-E_FILE_FLASH_START-1;
    if(FlashFileInfo[FileInfoId].FileId == FileName)
    {
        Tmp_Addr = FlashFileInfo[FileInfoId].Addr;

        if((Offset + Len) <= FlashFileInfo[FileInfoId].Size)/*"ֻ�ڱ��ļ��ڽ��в���"*/
        {
            Ret = Flash_Write(Tmp_Addr + Offset, pDataBuff,Len);
        }
        else
        {
            #ifdef DEBUG_FS
            Debug.printf("Warning!!! Writefile over the boundary\n");
            #endif
        }           
    }
    else
    {
    }
    return Ret;

}

static INT8U FS_ReadFlashFile(ENUM_FS_ID FileName,INT32U Offset, INT8U *pDataBuff, INT16U Len)
{
    INT32U  Tmp_Addr;
    INT8U Ret = FALSE;
    INT16U FileInfoId;

    FileInfoId = FileName-E_FILE_FLASH_START-1;
    if(FlashFileInfo[FileInfoId].FileId == FileName)
    {
        Tmp_Addr = FlashFileInfo[FileInfoId].Addr;

        if((Offset + Len) <= FlashFileInfo[FileInfoId].Size)/*"ֻ�ڱ��ļ��ڽ��в���"*/
        {
            Ret = Flash_Read(Tmp_Addr + Offset, pDataBuff,Len);
        }
        else
        {
            #ifdef DEBUG_FS
            Debug.printf("Warning!!! Writefile over the boundary\n");
            #endif
        }           
    }
    else
    {
    }
    return Ret;


}



static INT8U FS_DelFlashFile(ENUM_FS_ID FileName,INT32U StartOffset, INT32U Len)
{
    INT32U  Tmp_Addr;
    INT32U  StartSecLen;
    INT32U  StartSecAddr;/*ʵ�ʲ�����ʼλ��*/
    INT32U  Offset2;
    INT8U Ret; 
    INT16U FileInfoId;
    INT8U DelBuf[FLASH_SECTOR_SIZE];/*���ڻ��治��Ҫɾ�����ļ�*/


    Ret = FALSE;
    FileInfoId    = FileName-E_FILE_FLASH_START-1;
    /*��startoffset�����������ʼλ�úͳ���*/
    StartSecAddr  = (StartOffset/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;
    StartSecLen   = (StartOffset % FLASH_SECTOR_SIZE);
        
    if(FlashFileInfo[FileInfoId].FileId == FileName)
    {
        if((StartSecLen+Len) <= FLASH_SECTOR_SIZE)/*���������ܿ�Խ����*/
        {
            Tmp_Addr = FlashFileInfo[FileInfoId].Addr;
            if((StartSecLen+Len) == FLASH_SECTOR_SIZE)/*�󲿷�ȫ�������ˣ�����Ҫ�������Ч��*/
            {
                if(StartSecLen > 0)
                {
                    Ret = Flash_Read(Tmp_Addr+StartSecAddr, &DelBuf[0], StartSecLen);
                }
                else
                {
                    Ret = TRUE;
                }
            }
            else
            {
                Ret = Flash_Read(Tmp_Addr+StartSecAddr, &DelBuf[0],FLASH_SECTOR_SIZE);/*һ�ζ�ȡ��������*/
            }
            
            if(Ret == TRUE)
            {
                Ret = Flash_SectorErase(Tmp_Addr+StartSecAddr);/*������������*/
                if(Ret == TRUE)
                {
                    /*д�����������ڲ���Ҫɾ��������*/
                    if(StartSecLen > 0)
                    {
                        Ret = Flash_Write(Tmp_Addr+StartSecAddr, &DelBuf[0], StartSecLen);/*д��ǰ������*/
                    }
                    else
                    {

                    }
                    if(Ret == TRUE)
                    {
                        if((StartSecLen+Len) == FLASH_SECTOR_SIZE)/*ȫ�������Ѿ�д��*/
                        {
                        }  
                        else/*��벿�ֻ�������*/
                        {
                            Offset2 = StartSecAddr+Len;
                            Ret = Flash_Write(Tmp_Addr+Offset2, &DelBuf[Offset2], FLASH_SECTOR_SIZE-(Offset2));/*д��������*/
                        }
                    }
                }
            }
        }
        else
        {
            Ret = FALSE;
        }
    }
    else
    {
        Ret = FALSE;
    }
    return Ret;
}
#endif
