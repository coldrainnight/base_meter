/*"*****************************************************************************"*/
/*"  FileName: UI.c"*/
/*"  Description:     该文件实现用户接口模块主接口函数定义"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.12.01"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/12/01     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "UI.h"
#include "App\UserInterface\PP\PP_698\Process\Pp_process.h"
#include "Lib\AXDR\AXDR.h"

#define READ_MAX_CNT    (3)

NO_INIT ST_UI_PARA_WITH_BACKUP StUIParaWithRamBackup;

ST_RUN_STATUS   RunStatus;/*电能表运行状态字*/
UN_WORD_DEF    MeterSampleStatus;     /*计量芯系统状态字*/
UN_WORD_DEF    SampleStatus;          /*采样状态字*/


#ifdef THREE_PHASE
INT16S SignalStrength   = 0;
INT8U  GPRS_Status_Time = 0;
INT8U  GPRS_Status      = 0;
#endif

static void UI_RamParaChk(void);
static void UI_DefaultInit(void);

/*"************************************************"*/
/*"Function:    UI_DefaultInit"*/
/*"Description: 默认参数初始化"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void UI_DefaultInit(void)
{
    /*有RAM备份参数 恢复默认值*/
    UI_LoadDefaultUiPara();
    FS_WriteFile(E_FILE_UI_PARA_WITH_BACKUP,0,(INT8U *)&StUIParaWithRamBackup,sizeof(ST_UI_PARA_WITH_BACKUP));

    /*无RAM备份参数恢复默认值*/
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

        if(i >= READ_MAX_CNT) /*"取内卡参数失败时调用缺省参数"*/
        {
            UI_LoadDefaultUiPara();
        }
    }
}

/*"************************************************"*/
/*"Function:    UI_Init"*/
/*"Description: 用户接口初始化"*/
/*"Input: mode 上电or 掉电运行模式"*/
/*"Output: 无"*/
/*"Return: 无"*/
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
    
    if(MCU_RUNMODE == mode)/*计算电池运行时间*/
    {
        GetSingle(E_PD_TIME, (INT8U *)&PdTime);/*获取上一次掉电时间*/
        GetSingle(E_SYS_TIME, (INT8U *)&SysTime);/*获取当前时间*/
        pdSec =  LIB_CmpTime((ST_6TIME_FORM*)&SysTime, &PdTime);
        if((pdSec>=60) && (FALSE == ChkStatus(E_MTR_RST))
           #ifdef SINGLE_PHASE /*三相表电池独立供电，RAM丢失时钟电池可能存在*/
            &&(0x5a5a == StUIParaWithRamBackup.validFlg)
           #endif
           )/*"以分钟为单位累加"*/
        {
            pdSec /= 60;
            FS_ReadFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin), (INT8U *)&TmpClockBatWorkTimeMin, 4);
            TmpClockBatWorkTimeMin += pdSec;
            FS_WriteFile(E_FILE_UI_DATA, FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin), (INT8U *)&TmpClockBatWorkTimeMin, 4);
        }
    }

    UI_RamParaChk();/*"UI有RAM备份参数初始化"*/  

    DM_Init(mode);/*"初始化显示按键模块"*/  
    TCA_Init(mode); /*双芯认证初始化*/
    if((MCU_RUNMODE == mode) || (FIRST_PWR_ON == mode))
    {
        Report_Init();
        AXDR_DataLenGetInit(LenGet_PP_698,0);
    }

   
}

#ifdef THREE_PHASE
/*****************************************************************************
 函 数 名  : Change_GPRS_Value_Status
 功能描述  : 将698的值转换成信号格,根据诺基亚的信号格数来，分别对应5，4，3，2，1
，1
 输入参数  : SINT16 Value  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年6月22日
    作    者   : xyt
    修改内容   : 新生成函数

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
 函 数 名  : UI_GprsStatusTime
 功能描述  : GPRS定时处理，刷新状态
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年6月22日
    作    者   : xyt
    修改内容   : 新生成函数

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
        #if 0  /*与default一起*/
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
/*"Description: 用户接口任务"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
void UI_Task(void)
{
    if(TRUE == Task_ChkTsk_1s(MAINTASK_UI))/*"秒任务"*/
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

    if(TRUE == Task_ChkTsk_1min(MAINTASK_UI))/*"分钟任务"*/
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

/*运行状态字相关*/


const EN_MTR_STATUS  RunStatus1Tab[]=
{
    /*运行状态字1对应表*/
    E_STA_NO_DEF,/*保留*/
    E_DEMAND_MODE,
    E_CLK_BAT_LOSS,
    E_DIS_BAT_LOSS,
    E_PWR_P_DIR,
    E_PWR_Q_DIR,
    E_STA_NO_DEF,/*保留*/
    E_STA_NO_DEF,/*保留*/
    
    E_RELAY_ERR,/*控制回路错需特殊处理*/
    E_ESAM_ERR,
    E_STA_NO_DEF,/*保留*/
    E_STA_NO_DEF,/*保留*/
    E_STA_NO_DEF,/*内部程序错误未做*/
    E_STA_NO_DEF,/*存储器故障 需特殊处理*/
    E_OVERDRAFT,
    E_CLK_ERR,
    
};


const EN_MTR_STATUS  RunStatus2Tab[]=
{
    /*运行状态字2对应表*/
    #if defined (SINGLE_PHASE)
    E_PWR_P_DIR,/*单相表只给出了这个状态*/
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
    /*运行状态字3对应表*/
    E_STA_NO_DEF,/*保留*/
    E_AUX_PWR,
    E_BAT_PWR,
    E_IRAUTH_STATUS,/*编程*/
    E_RELAY_STATUS,
    E_STA_NO_DEF,/*保留*/
    E_RELAY_CMD_STATUS,
    E_RELAY_ALARM,
    
    E_PREPAY_ENGY,
    E_PREPAY_MONEY,
    E_STA_NO_DEF,/*保留*/
    E_STA_NO_DEF,/*保留*/
    E_RELAY_PROTECT,
    E_AUTH_STATUS,
    E_LOCAL_CREDIT,
    E_REMOTE_CREDIT,

};

/*A相故障状态*/
const EN_MTR_STATUS  RunStatus4Tab[]=
{
    /*运行状态字4对应表*/
    E_LOSS_VOLT_LA,
    E_UNDER_VOLT_LA,
    E_OVER_VOLT_LA,
    E_LOSS_CURR_LA,
    E_OVER_CURR_LA,
    E_OVER_LOAD_LA,
    #if defined (SINGLE_PHASE)
    E_PWR_P_DIR,/*单相表只给出了这个状态*/
    #else
    E_PWR_REVERSE_LA,    
    #endif
    E_BRK_PHASE_LA,
    
    E_CUT_CURR_LA,
};


/*B相故障状态*/
const EN_MTR_STATUS  RunStatus5Tab[]=
{
    /*运行状态字5对应表*/
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


/*C相故障状态*/
const EN_MTR_STATUS  RunStatus6Tab[]=
{
    /*运行状态字6对应表*/
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


/*合相故障状态*/
const EN_MTR_STATUS  RunStatus7Tab[]=
{
    E_VOLT_REVERSE,
    E_CURR_REVERSE,
    E_VOLT_IMBAL,
    E_CURR_IMBAL,
    E_STA_NO_DEF,/*辅助电源失电不处理*/
    E_PWR_DOWN,
    E_OVER_DEMAND,           
    E_OVER_FACTOR,

    E_CURR_BADLY_IMBAL,
    E_OPEN_COVER,
    E_OPEN_TAIL,
};

/*计量芯采样状态字 */
const EN_MTR_STATUS  MeterSampleStatusTab[]=
{
    E_OPEN_COVER,   /*bit0*/
    E_PERMISS_LIMIT,   /*bit1*/
    E_STA_NO_DEF,
    E_STA_NO_DEF,
    E_STA_NO_DEF,//E_FLASH_ERR,    /*内部程序错误 怎么定义*/ 
    E_EEP_ERR,      /*存储器故障或损坏*/
    E_STA_NO_DEF,           
    E_CLK_ERR,      /*时钟故障*/
};

/*采样状态字*/
const EN_MTR_STATUS  SampleStatusTab[]=
{
    E_PWR_P_DIR,        /*bit0  总有功功率方向*/
    E_PWR_P_LA_DIR,     /*bit1  A相有功功率方向*/
    E_PWR_P_LB_DIR,     /*B相有功功率方向*/
    E_PWR_P_LC_DIR,     /*C相有功功率方向*/
    E_PWR_Q_DIR,        /*总无功功率方向*/
    E_PWR_Q_LA_DIR,     /*A相无功功率方向*/
    E_PWR_Q_LB_DIR,     /*B相无功功率方向*/    
    E_PWR_Q_LC_DIR,     /*C相无功功率方向*/

    E_CURR_BADLY_IMBAL, /*保留*/
    E_CURR_BADLY_IMBAL, /*A相潜动*/
    E_CURR_BADLY_IMBAL, /*B相潜动*/
    E_CURR_BADLY_IMBAL,  /*C相潜动*/
    E_VOLT_REVERSE,     /*电压逆相序*/
    E_CURR_REVERSE,     /*电流逆相序*/
};


INT8U UI_GetRunStatues(INT8U *pdata)
{
    INT8U i;
    
    BUILD_BUG_ON(MAX_MTR_STA>=0xFF);

    /*先清零状态字*/
    LIB_MemSet(0,(INT8U *)&RunStatus,sizeof(RunStatus));  
#ifdef PM_MODLE            
	Action(E_PMRUNSTA_FRESH,NULL,NULL);/*"刷新预付费运行状态字"*/
#endif
    /*刷新状态字1*/
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
    /*单相表电池欠压时，停电抄表电池欠压，时钟电池欠压也应置位*/
    RunStatus.word1.bits.lowPwrBat = RunStatus.word1.bits.clockBat;
    #endif

    /*状态字1中 存储器故障*/
    if((TRUE == ChkStatus(E_EEP_ERR)) ||(TRUE == ChkStatus(E_FLASH_ERR) ) )
    {
        RunStatus.word1.bits.RomErr = 1;
    }

    /*刷新状态字2*/
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

    /*刷新状态字3*/
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
	
    /*刷新状态字4*/
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
    /*刷新状态字5*/
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

    /*刷新状态字6*/
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

    /*刷新状态字7*/
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

    /*刷新 计量芯采样状态字*/
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
        MeterSampleStatus.word |= (0x0001<<5);   /*外部FLASH也算存储器故障或损坏*/
    }        
    /*刷新 计量芯采样状态字 end*/    
    /*--------------------------------------------*/
    /*刷新 采样状态字*/
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
/*"  Description:  获取继电器的相关状态"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata 输出状态数据"*/
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
	Action(E_PMRUNSTA_FRESH,NULL,NULL);/*"刷新预付费运行状态字"*/
#endif
    if(0x8000 == mstOad.Oi.asInt)
    {
        switch(mstOad.AId)
        {
        case 4:/*"告警状态"*/
			if(ChkStatus(E_RELAY_ALARM) == TRUE)
            {
                *pdata = 0x01;
            }
            break;
        case 5:/*"继电器命令状态"*/
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
        case 2:/*"保电状态"*/
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
Description: 	F205_继电器输出			
Input: pdin: OAD IID =1 表示跳闸继电器 IID=2表示告警继电器 	
Output: 数据
Return: 返回获取数据长度	
Others: 
************************************************"*/	
INT8S UI_GetRelayOutput(void *pdin, void *pdout)
{
    ST_RLY_STA tempRlySta;
    INT8U RelayType=0;
    INT8S RetLen;

            
    /*描述符*/
    tempRlySta.name =  '0';/*描述符与OAD元素索引对应*/ 

    /*当前状态 合闸0 跳闸1*/
    if(TRUE == ChkStatus(E_RELAY_STATUS))/*"继电器输出状态"*/
    {
        tempRlySta.status = 1;
    }
    else
    {
        tempRlySta.status = 0;
    }
    /*开关属性 脉冲式 0 保持式 1*/
#ifdef PM_MODLE            
    GetSingle(E_RELAY_TYPE, &RelayType);
#else
    RelayType = RLY_OUTSIDE;
#endif 
	RelayType &= RLYTYPE;
    if((RLY_INSIDE == RelayType)||(RLY_PULSEOUTSIDE == RelayType))
    {
        tempRlySta.prop = 0;/*"内置或外置脉冲式"*/
    }
    else
    {
        tempRlySta.prop = 1;/*"外置电平"*/
    }
    /*接线状态 0 接入 1 未接入*/
    tempRlySta.lineSta = 0;/*接入*/
    LIB_MemCpy((INT8U *)&tempRlySta, pdout, sizeof(ST_RLY_STA));
    RetLen = sizeof(ST_RLY_STA);

    return RetLen;

    
}


/*"*****************************************************************************"*/
/*"  Function:       UI_DataClr"*/
/*"  Description:    用户接口类数据清零"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdin: 无，pdout:  无"*/
/*"  Output          "*/
/*"  Return:         DAR"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8S UI_DataClr(void *pdin, void *pdout)
{
    INT32U DayStdNow;

	DayStdNow = 0;
	FS_WriteFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,LastBroadcastAdjTime), (INT8U *)&DayStdNow, 4);/*"清校时时间'*/
    FS_WriteFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,ClockBatWorkTimeMin), (INT8U *)&DayStdNow, 4);/*清时钟电池工作时间*/

    return MS_SUCCESS;

}

