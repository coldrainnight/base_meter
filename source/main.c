/*"************************************************************"*/
/*"FileName: Main.c"*/
/*"Author:       Version : 1.0       Date: 2009-01-20"*/
/*" Description: �����п��"*/
/*" Function List:  ��Ҫ�������书��"*/
/*" 1. void main(void)"*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "System\sys_def.h"
#include "Driver\DriverComm.h"
#include "System\Task\task.h"
#include "app\LegallyRelevant\LRM.h"
#include "APP\UserInterface\UI.h"
#include "app\monitor\MNT.h"
#ifdef DU_MODLE
#include "App\demand\DU.h"
#endif
#include "App\UserInterface\ProtocolStack\host_commu\host_commu.h"

#include "Driver\Device\MU_IC.h"
#include "Driver\Device\chip_commu_slave.h"

void HardwareInit(void);
void SoftwareInit(void);
void NormalMode(void);
void PD_Main(void);

extern void SC_SysClkInit(INT8U Mode);
extern void Slave_Spi_Init(void);
extern void spi_run_check(void);
extern void poweron_set_bm_info(void);


/*************************************************
 Function: main
 Description: ������
 Global: ��
 Calls:  Init_LowPowerMode��Init_Hardware��Init_Software��
 NormalMode

 Input:  ��
 Output: ��
 Return: ��
 Others: ��
 *************************************************/
void PD_Check(void)
{
    MC_ClosePulse();/*�ر��������*/
    LCD_Close(); /*"��LCD��Դ"*/
    OC_Init();/*�رձ����*/
    //EC_EPwrOff();
    Action(E_LRM_PWR_DOWN, NULL, NULL);
    Action(E_RCRD_PD_ACTION, NULL, NULL);
    Action(E_MNT_PWR_DOWN, NULL, NULL);

    MCU_Reset();/*"��λ"*/
}



void main(void)
{  
    MCU_IntDisableAll();
    MCU_RunModeSet(MCU_RUNMODE);    /*"MCUϵͳʱ�ӳ�ʼ��"*/
    WDT_Init(MCU_RUNMODE);    /*"WDT��ʼ��"*/
    INT_Init();    /*�ж�������ʼ��*/
    MCU_PwrChkInit(PD_Check);
    MCU_DelayMs(50);

    if(PWRMODE_DOWN==MCU_GetPwrState())
    {
        MCU_DelayMs(50);
        if(PWRMODE_DOWN == MCU_GetPwrState())
        {
            Push(E_LOW_PWR, TRUE);
            PD_Main();
        }
        else
        {
            MCU_Reset();
        }
    }
    else
    {
        MCU_DelayMs(50);
        if(PWRMODE_DOWN == MCU_GetPwrState())
        {
            MCU_Reset();
        }
        else
        {
        }
        Push(E_LOW_PWR, FALSE);
        Push(E_MTR_INIT, TRUE);
        HardwareInit();
        SoftwareInit(); 
        
        NormalMode();

    }
}

/*************************************************
 Function: 
 Description: 
 Global: ��
 Calls:  SystemReset��KeyScan_Run��
 Clock_Run��Watch_Dog

 Input:  ��
 Output: ��
 Return: ��
 Others: ��
 *************************************************/
void HardwareInit(void)
{
    MCU_RunModeSet(MCU_RUNMODE);

    GPIO_Init(MCU_RUNMODE);    /*"�ϵ�˿ڵĳ�ʼ��"*/    
    MCU_PwrChkInit(PD_Check);
    WDT_Init(MCU_RUNMODE);    /*"WDT��ʼ��"*/
    EEP_Init();    /*"EEP��ʼ��"*/
    //EXRTC_Init(MCU_RUNMODE);
    //INRTC_Init(MCU_RUNMODE);
    KEY_Init(MCU_RUNMODE);
    SID_Init(MCU_RUNMODE, SID_MAX);
    //RTC_CaliInit(MCU_RUNMODE);
    //RTC_OpenSec();
    CMD_Init(PLC_MODE);
    CMD_Init(RF_MODE);
    EC_HardwareInit(MCU_RUNMODE);
    
    Slave_Commu_Init(1);
    
    MCU_IntEnableAll();
    LCD_Init(MCU_RUNMODE);
    Dis_ShowAll();
    INT_ENABLE_ALL;/*"�����⵼�¿������ж�"*/
}

/*"************************************************************"*/
/*"Function:    PowerChkLow                                  *"*/
/*"Description: �ϵ�״̬������                                *"*/
/*"Input:       ��                                            *"*/
/*"Output:      ��                                            *"*/
/*"Return:      ��                                            *"*/
/*"Others:                                                    *"*/
/*"************************************************************"*/
static void PowerChkLow(void)
{
    if(PWRMODE_DOWN == MCU_GetPwrState())/*"�ϵ�״̬�������ж�"*/
    {
        MCU_DelayMs(20);
        /*"affirm the power down"*/
        if(PWRMODE_DOWN == MCU_GetPwrState())
        {
            MCU_DelayMs(20);
            /*"affirm the power down"*/
            if(PWRMODE_DOWN == MCU_GetPwrState())
            {
				MCU_DelayMs(20);
				/*"affirm the power down"*/
				if(PWRMODE_DOWN == MCU_GetPwrState())
				{
					LCD_Close();
					MCU_Reset();
				}
			}
        }
    }
}

INT8U init_flg;
/*************************************************
 Function: 
 Description: 
 Global: ��
 Calls:  SystemReset��KeyScan_Run��
 Clock_Run��Watch_Dog

 Input:  ��
 Output: ��
 Return: ��
 Others: ��
 *************************************************/
void SoftwareInit(void)
{
    ST_TMRCONFIG Timercfg;
    
    Task_Init();
    /*"�ϵ�ʱ������"*/
    Timercfg.Mode=TMR_TIMING;
    Timercfg.Count=SYS_TICK_MS;
    TMR_Init(E_TMR_ID_SYS_TICK,&Timercfg);
    INT_VectSet(E_INT_ID_ET1, Task_TimerTaskListRun);
    INT_IntPrioSet(E_INT_ID_ET1,E_INT_PRIO_LEVEL_3);
    INT_En(E_INT_ID_ET1);
    TMR_Enable(E_TMR_ID_SYS_TICK);  
    PowerChkLow();
    OC_Init();
    FirstPwrOnChk();
    if(ChkStatus(E_MTR_RST) != TRUE)
    {
        //Slave_Spi_Init();
        SC_SysClkInit(MCU_RUNMODE);
        poweron_set_bm_info();
        //MCU_DelayMs(300);
        INT_Dis(E_INT_ID_SPI1);
    }
	init_flg = 0xab;//comment   for debug 
    if(((TRUE == ChkStatus(E_FIRST_PWR_ON)) && (TRUE == ChkStatus(E_FAC))
      &&(SID_TRUE == SID_PollRead(SID_FAC))  ||( (init_flg == 0xab)&&(SID_TRUE == SID_PollRead(SID_FAC))))) 
    {
        if(TRUE != FS_Init(FIRST_PWR_ON))
        {
            MCU_Reset();
        }
        LRM_Init(FIRST_PWR_ON);
        MNT_Init(FIRST_PWR_ON);
        UI_Init(FIRST_PWR_ON);
       #ifdef DU_MODLE
       DU_Init(FIRST_PWR_ON);
       #endif
        RCRD_Init(FIRST_PWR_ON);
        Action(E_FIRST_PWR_UP_OVER, NULL, NULL);
        Action(E_RESET_FAC_REMAIN_TIME, NULL, NULL);
        CLR_MTR();
        LCD_Close();
        MCU_Reset();
    }
    else
    {
        if(TRUE != FS_Init(MCU_RUNMODE))
        {
            if(TRUE == ChkStatus(E_FAC))
            {
                MCU_Reset();
            }
            else
            {

            }
        }
        LRM_Init(MCU_RUNMODE);
        MNT_Init(MCU_RUNMODE);
        UI_Init(MCU_RUNMODE);
        #ifdef DU_MODLE
        DU_Init(MCU_RUNMODE);
        #endif
        RCRD_Init(MCU_RUNMODE);
        Action(E_PWRUP_EVTINIT, NULL, NULL);
    }
    PowerChkLow();
    ProtocolStackInit();
        
    /*"ϵͳ�����ʼ��������ע��"*/    

    Task_MainTaskRegister(MAINTASK_EU, LRM_Main);
    Task_MainTaskRegister(MAINTASK_DM, DM_Task);
    Task_MainTaskRegister(MAINTASK_CM, ProtocolStackTask);
    Task_MainTaskRegister(MAINTASK_UI, UI_Task);
    #ifdef DU_MODLE
    Task_MainTaskRegister(MAINTASK_DU, DU_Main);
    #endif
    Task_MainTaskRegister(MAINTASK_STM, AUTO_Main);

    /*"��ʱ����ע��"*/
   
    Task_TimerTaskRegister(TIMERTASK_TICK,MCU_AddTick);
    Task_TimerTaskRegister(TIMERTASK_OC,OC_Int);
#ifdef SINGLE_PHASE
    Task_TimerTaskRegister(TIMERTASK_PLUSE,MU_IC_PluseAffirm);
    Action(E_LRM_PCTR_INIT,NULL,NULL);/*"������ƿ��ϵ��ʼ��"*/
#else
    Task_TimerTaskRegister(TIMERTASK_CM,COM_Monitor_10Ms);
#endif

    //Task_TimerTaskRegister(TIMERTASK_HOST_COMM, spi_run_check);
    Push(E_MTR_INIT, FALSE);
    Host_Commu_Bm_Data_Sync(1);
}

/*************************************************
 Function: NormalMode
 Description: ��������ģʽ�µ���ѭ��
 Global: ��
 Calls:  SystemReset��KeyScan_Run��
 Clock_Run��Watch_Dog

 Input:  ��
 Output: ��
 Return: ��
 Others: ��
 *************************************************/


void NormalMode(void)
{    
    for (;;)
    {
        Push(E_MTR_INIT, FALSE);
		Push(E_LOW_PWR, FALSE);
        WDT_FeedWdg();
        MCU_PD_Deal();
        MCU_IntEnableAll(); /*"�����⵼�¿������ж�"*/
        Task_Task();
    }
}

void PU_Check(void)
{
    if((PWRMODE_UP == MCU_GetPwrState())/* || (HIGH == GPIO_ReadPin(FAC_JUMP))*/)/*"��⵽�ϵ�"*/
    {
        MCU_DelayMs(10);
        if((PWRMODE_UP == MCU_GetPwrState()) /*|| (HIGH == GPIO_ReadPin(FAC_JUMP))*/)/*"��⵽�ϵ�"*/
        {
            MCU_DelayMs(10);
            if(PWRMODE_UP == MCU_GetPwrState())
            {
                MCU_Reset();
            }
        }
        else
        {
        }
    }
    else
    {
    }
}

#define PD_MODE_BAT  (0)
#define PD_MODE_CAP  (1)
INT8U PdMode = PD_MODE_CAP;

void PD_HardwareInit()
{
    MCU_RunModeSet(MCU_LPMODE_DEEP);
    INT_Init();    /*�ж�������ʼ��*/
    GPIO_Init(MCU_LPMODE_DEEP);
    MCU_PwrChkInit(NULL);
    WDT_Init(MCU_LPMODE_DEEP);  
    KEY_Init(MCU_LPMODE_DEEP);
    INRTC_Init(MCU_LPMODE_DEEP);
    EEP_Init();
    SID_Init(MCU_LPMODE_DEEP, SID_MAX);
    LCD_Init(MCU_LPMODE_DEEP);/*�ȳ�ʼ����رգ����⾲������*/
    LCD_Close();/*�Ƚ�LCD gpio�رգ���ֹ����*/
}


/*�͹����������ʼ���ӿ�*/
void PD_SoftwareInit(void)
{
    LRM_Init(MCU_LPMODE_DEEP);
    UI_Init(MCU_LPMODE_DEEP);
    MNT_Init(MCU_LPMODE_DEEP);
    #ifdef DU_MODLE
    DU_Init(MCU_LPMODE_DEEP);
    #endif
    RCRD_Init(MCU_LPMODE_DEEP);
}

void PD_Main(void)
{
    INT8U TmpRet;
    INT8U BatChangeable;
    ST_DM_PD_DATA PdShowList[PD_DISP_DATA_NUM];/*"�͹�����ʾ���ݻ�����"*/
        
    PD_HardwareInit();  /*"Ӳ����ʼ��"*/
    PU_Check();/*�ϵ���*/
    PD_SoftwareInit();  /*�����ʼ��*/
    PU_Check();/*�ϵ���*/
    MCU_IntEnableAll(); /*"�����ж�"*/
#ifdef PM_MODLE    
    GetSingle(E_METER_MODE, &MeterMode);
    if(MeterMode&0x80)
    {
        BatChangeable = TRUE; 
    }
    else
    {
        BatChangeable = FALSE; 
    }
#endif    
    LIB_MemSet(0xFF, (INT8U *)&PdShowList[0], sizeof(PdShowList));/*"��͹�����ʾ���ݻ�����"*/
    for(;;)
    {
        MCU_PwrChkOpen();
        PU_Check();/*�ϵ���*/

        if(BatChangeable == TRUE)/*��ؿɸ���ģʽ*/
        {        
            /*����Ƿ��е��*/
            AC_CompInit(MCU_LPMODE_DEEP,AC_COMPID_BAT);
            TmpRet = AC_CompRet(AC_COMPID_BAT);
            AC_CompDisable(AC_COMPID_BAT);

            if(TRUE == TmpRet)
            {
                if(PdMode == PD_MODE_CAP)/*ǰ״̬Ϊcap���ʼ��Һ��*/
                {
                    LRM_Init(MCU_LPMODE_DEEP);
                    UI_Init(MCU_LPMODE_DEEP);
                }
                PdMode= PD_MODE_BAT;
            }
            else
            {
                if(PdMode == PD_MODE_BAT)/*ǰ״̬ΪBAT��ر�Һ��*/
                {
                    LCD_Close();
                }
                PdMode= PD_MODE_CAP;
            }
        }
        else/*��ز��ɸ���ģʽ*/
        {
            PdMode= PD_MODE_BAT;/*�̶�Ϊ���ģʽ*/
        }

        PU_Check();/*�ϵ���*/
        if(PdMode == PD_MODE_CAP)/*���ģʽ���жϣ�����ģʽ�����ж�*/
        {
            MCU_PwrChkClose();
        }
        else
        {
        }
        
        /*ÿ�봦��*/
        if(TRUE == gSecFlag)/*"�͹��������жϱ�־"*/
        {/*"��ˢ��"*/
            gSecFlag = FALSE; /*"���жϱ�־����"*/



            WDT_FeedWdg(); /*"ι��"*/

            if( PD_MODE_BAT == PdMode)/*���ģʽ*/
            {
                LRM_LPMain(MCU_LPMODE_MID);
                DM_PD1s();
                MNT_TaskPd();/*��Դ�쳣�������Ǽ��*/
            }
            else
            {
                LRM_LPMain(MCU_LPMODE_DEEP);
            }
        }
        
        if( PD_MODE_BAT == PdMode)/*���ģʽ*/
        {
            DM_PDMain(&PdShowList[0]);/*��ʾ����*/
        }
     
        if(PdMode == PD_MODE_BAT)
        {
            MCU_EnterSleep(SLEEP);
        }
        else
        {
            MCU_EnterSleep(DEEP_SLEEP);
        }

        MCU_OutSleep();
    }
}


