/*"************************************************************"*/
/*"FileName: Main.c"*/
/*"Author:       Version : 1.0       Date: 2009-01-20"*/
/*" Description: 主运行框架"*/
/*" Function List:  主要函数及其功能"*/
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
 Description: 主函数
 Global: 无
 Calls:  Init_LowPowerMode、Init_Hardware、Init_Software、
 NormalMode

 Input:  无
 Output: 无
 Return: 无
 Others: 无
 *************************************************/
void PD_Check(void)
{
    MC_ClosePulse();/*关闭脉冲输出*/
    LCD_Close(); /*"关LCD电源"*/
    OC_Init();/*关闭背光等*/
    //EC_EPwrOff();
    Action(E_LRM_PWR_DOWN, NULL, NULL);
    Action(E_RCRD_PD_ACTION, NULL, NULL);
    Action(E_MNT_PWR_DOWN, NULL, NULL);

    MCU_Reset();/*"软复位"*/
}



void main(void)
{  
    MCU_IntDisableAll();
    MCU_RunModeSet(MCU_RUNMODE);    /*"MCU系统时钟初始化"*/
    WDT_Init(MCU_RUNMODE);    /*"WDT初始化"*/
    INT_Init();    /*中断向量初始化*/
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
 Global: 无
 Calls:  SystemReset、KeyScan_Run、
 Clock_Run、Watch_Dog

 Input:  无
 Output: 无
 Return: 无
 Others: 无
 *************************************************/
void HardwareInit(void)
{
    MCU_RunModeSet(MCU_RUNMODE);

    GPIO_Init(MCU_RUNMODE);    /*"上电端口的初始化"*/    
    MCU_PwrChkInit(PD_Check);
    WDT_Init(MCU_RUNMODE);    /*"WDT初始化"*/
    EEP_Init();    /*"EEP初始化"*/
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
    INT_ENABLE_ALL;/*"防意外导致开不了中断"*/
}

/*"************************************************************"*/
/*"Function:    PowerChkLow                                  *"*/
/*"Description: 上电状态掉电检测                                *"*/
/*"Input:       无                                            *"*/
/*"Output:      无                                            *"*/
/*"Return:      无                                            *"*/
/*"Others:                                                    *"*/
/*"************************************************************"*/
static void PowerChkLow(void)
{
    if(PWRMODE_DOWN == MCU_GetPwrState())/*"上电状态进掉电中断"*/
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
 Global: 无
 Calls:  SystemReset、KeyScan_Run、
 Clock_Run、Watch_Dog

 Input:  无
 Output: 无
 Return: 无
 Others: 无
 *************************************************/
void SoftwareInit(void)
{
    ST_TMRCONFIG Timercfg;
    
    Task_Init();
    /*"上电时基配置"*/
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
        
    /*"系统任务初始化及任务注册"*/    

    Task_MainTaskRegister(MAINTASK_EU, LRM_Main);
    Task_MainTaskRegister(MAINTASK_DM, DM_Task);
    Task_MainTaskRegister(MAINTASK_CM, ProtocolStackTask);
    Task_MainTaskRegister(MAINTASK_UI, UI_Task);
    #ifdef DU_MODLE
    Task_MainTaskRegister(MAINTASK_DU, DU_Main);
    #endif
    Task_MainTaskRegister(MAINTASK_STM, AUTO_Main);

    /*"定时任务注册"*/
   
    Task_TimerTaskRegister(TIMERTASK_TICK,MCU_AddTick);
    Task_TimerTaskRegister(TIMERTASK_OC,OC_Int);
#ifdef SINGLE_PHASE
    Task_TimerTaskRegister(TIMERTASK_PLUSE,MU_IC_PluseAffirm);
    Action(E_LRM_PCTR_INIT,NULL,NULL);/*"脉冲控制口上电初始化"*/
#else
    Task_TimerTaskRegister(TIMERTASK_CM,COM_Monitor_10Ms);
#endif

    //Task_TimerTaskRegister(TIMERTASK_HOST_COMM, spi_run_check);
    Push(E_MTR_INIT, FALSE);
    Host_Commu_Bm_Data_Sync(1);
}

/*************************************************
 Function: NormalMode
 Description: 正常运行模式下的主循环
 Global: 无
 Calls:  SystemReset、KeyScan_Run、
 Clock_Run、Watch_Dog

 Input:  无
 Output: 无
 Return: 无
 Others: 无
 *************************************************/


void NormalMode(void)
{    
    for (;;)
    {
        Push(E_MTR_INIT, FALSE);
		Push(E_LOW_PWR, FALSE);
        WDT_FeedWdg();
        MCU_PD_Deal();
        MCU_IntEnableAll(); /*"防意外导致开不了中断"*/
        Task_Task();
    }
}

void PU_Check(void)
{
    if((PWRMODE_UP == MCU_GetPwrState())/* || (HIGH == GPIO_ReadPin(FAC_JUMP))*/)/*"检测到上电"*/
    {
        MCU_DelayMs(10);
        if((PWRMODE_UP == MCU_GetPwrState()) /*|| (HIGH == GPIO_ReadPin(FAC_JUMP))*/)/*"检测到上电"*/
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
    INT_Init();    /*中断向量初始化*/
    GPIO_Init(MCU_LPMODE_DEEP);
    MCU_PwrChkInit(NULL);
    WDT_Init(MCU_LPMODE_DEEP);  
    KEY_Init(MCU_LPMODE_DEEP);
    INRTC_Init(MCU_LPMODE_DEEP);
    EEP_Init();
    SID_Init(MCU_LPMODE_DEEP, SID_MAX);
    LCD_Init(MCU_LPMODE_DEEP);/*先初始化后关闭，避免静电问题*/
    LCD_Close();/*先将LCD gpio关闭，防止静电*/
}


/*低功耗下软件初始化接口*/
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
    ST_DM_PD_DATA PdShowList[PD_DISP_DATA_NUM];/*"低功耗显示数据缓存区"*/
        
    PD_HardwareInit();  /*"硬件初始化"*/
    PU_Check();/*上电检测*/
    PD_SoftwareInit();  /*软件初始化*/
    PU_Check();/*上电检测*/
    MCU_IntEnableAll(); /*"开总中断"*/
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
    LIB_MemSet(0xFF, (INT8U *)&PdShowList[0], sizeof(PdShowList));/*"清低功耗显示数据缓存区"*/
    for(;;)
    {
        MCU_PwrChkOpen();
        PU_Check();/*上电检测*/

        if(BatChangeable == TRUE)/*电池可更换模式*/
        {        
            /*检测是否有电池*/
            AC_CompInit(MCU_LPMODE_DEEP,AC_COMPID_BAT);
            TmpRet = AC_CompRet(AC_COMPID_BAT);
            AC_CompDisable(AC_COMPID_BAT);

            if(TRUE == TmpRet)
            {
                if(PdMode == PD_MODE_CAP)/*前状态为cap则初始化液晶*/
                {
                    LRM_Init(MCU_LPMODE_DEEP);
                    UI_Init(MCU_LPMODE_DEEP);
                }
                PdMode= PD_MODE_BAT;
            }
            else
            {
                if(PdMode == PD_MODE_BAT)/*前状态为BAT则关闭液晶*/
                {
                    LCD_Close();
                }
                PdMode= PD_MODE_CAP;
            }
        }
        else/*电池不可更换模式*/
        {
            PdMode= PD_MODE_BAT;/*固定为电池模式*/
        }

        PU_Check();/*上电检测*/
        if(PdMode == PD_MODE_CAP)/*电池模式开中断，电容模式不开中断*/
        {
            MCU_PwrChkClose();
        }
        else
        {
        }
        
        /*每秒处理*/
        if(TRUE == gSecFlag)/*"低功耗下秒中断标志"*/
        {/*"秒刷新"*/
            gSecFlag = FALSE; /*"秒中断标志清零"*/



            WDT_FeedWdg(); /*"喂狗"*/

            if( PD_MODE_BAT == PdMode)/*电池模式*/
            {
                LRM_LPMain(MCU_LPMODE_MID);
                DM_PD1s();
                MNT_TaskPd();/*电源异常处理、开盖检测*/
            }
            else
            {
                LRM_LPMain(MCU_LPMODE_DEEP);
            }
        }
        
        if( PD_MODE_BAT == PdMode)/*电池模式*/
        {
            DM_PDMain(&PdShowList[0]);/*显示处理*/
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


