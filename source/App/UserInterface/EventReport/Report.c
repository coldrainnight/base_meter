/*"*****************************************************************************"*/
/*"  FileName: Reprot.c"*/
/*"  Description:     该文件实现事件上报相关接口函数定义"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.12.01"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/12/01     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "..\UI.h"
#include "Report.h"
#include "app\userinterface\PP\PP_698\Process\Pp_macro.h"

#ifdef REPORT645
//INT8U PrStateST;/*"编程事件上报开始状态"*/
/*"------------------------645事件主动上报-----------------------"*/
typedef enum 
{
    REVT_ESAM2=0,   	 /*"ESAM错"*/
    REVT_BAT2,   	 /*"时钟电池电压低"*/
    REVT_EEP2,   	 /*"存储器故障或损坏"*/
    REVT_LOWBAT2,   	 /*"停电抄表电池欠压"*/
    REVT_CREGIT2,  	 /*"透支状态"*/
    REVT_TRIP_SW2,  	 /*" 跳闸成功 "*/
    REVT_CLS_SW2,  	 /*" 合闸成功"*/

    /*"事件类上报"*/
    REVT_RLY,        /*"负荷开关误动作"*/
    REVT_OPEN,       /*"开表盖"*/
    REVT_OCRR,       /*"过流"*/
    REVT_CLR_MTR,    /*" 电表清零 "*/
    
    REVT_RTC,        /*"时钟故障"*/
    REVT_PD,         /*"掉电"*/
    REVT_PRG,        /*" 编程 "*/
    REVT_CLR_EVT,    /*" 事件清零 "*/
    REVT_JUST_TIME,  /*" 校时 "*/

    REVT_PRG_DSLOT,  /*" 时段表编程 "*/
    REVT_PRG_YSLOT,  /*" 时区表编程 "*/
    REVT_PRG_WEEK,  /*" 周休日编程 "*/  
    REVT_PRG_BILL,  /*" 结算日编程 "*/
    REVT_PRG_AEMODE,  /*" 有功组合方式编程 "*/
    REVT_PRG_TARIFF, /*" 费率表编程 "*/
    REVT_PRG_LEVEL, /*" 阶梯表编程 "*/
    REVT_KEY_UPDATE,  /*" 密钥更新 "*/
    
    MAX_REVENT2_ID
}EN_REPORT_EVENT2_ID;

const ST_645REPORTEVENTINFO mSI645EventInfo[]=
{
    {0x0000,0x00,0x02,0x00,0x02}, /*"ESAM错误"*/ 
    {0x0000,0x00,0x08,0x00,0x08}, /*"时钟电池电压低"*/
    {0x0000,0x00,0x20,0x00,0x20}, /*"存储器故障或损坏"*/
    {0x0000,0x01,0x01,0x01,0x01}, /*"停电抄表电池欠压"*/
    {0x0000,0x01,0x02,0x01,0x02}, /*"透支状态"*/
    {0x0000,0x01,0x40,0x01,0x40}, /*"跳闸成功"*/
    {0x0000,0x01,0x80,0x01,0x80}, /*"合闸成功"*/   

    /*"事件类"*/
    {(INT16U)E_RELAY_ERR_EVNT_RCRD,0x00,0x01,0x00,0x01}, /*"负荷开关误动作"*/
    {(INT16U)E_MTR_COVER_OPEN_EVNT_RCRD,0x01,0x04,0x01,0x04}, /*"开表盖"*/
    //{(INT16U)E_OVER_CURR_LA_EVNT_RCRD,0x02,0x10,0x02,0x10}, /*"过流"*/  
    {(INT16U)E_MTR_CLR_EVNT_RCRD,0x06,0x02,0x0A,0x02}, /*"电表清零"*/  
    
    {(INT16U)E_RTC_ERR_EVNT_RCRD,0x00,0x80,0x00,0x80}, /*"时钟故障"*/
    {(INT16U)E_PWR_DOWN_EVNT_RCRD,0x04,0x20,0x08,0x20}, /*"掉电"*/  
   
    {(INT16U)E_EVNT_CLR_EVNT_RCRD,0x06,0x08,0x0A,0x08}, /*"事件清零"*/  
    {(INT16U)E_JUST_TIME_EVNT_RCRD,0x06,0x10,0x0A,0x10}, /*"校时"*/ 

               
    {(INT16U)0x0000,0x07,0x20,0x0B,0x20}, /*"费率参数表编程"*/     
    {(INT16U)0x0000,0x07,0x40,0x0B,0x40}, /*"阶梯表编程"*/        
    {(INT16U)0x0000,0x07,0x80,0x0B,0x80}, /*"密钥更新"*/    
};


UN_645EVTRPT_STATUS ClrReportStatus;/*"需要清除的标志位"*/
ST_645EVTRPT_BLK m645EvtPrtStatus;  /*"645协议事件主动上报变量"*/
INT8U OldStatusFlg;/*"645已发生事件状态发生改变"*/
INT8U NewStatusFlg;/*"645未发生事件状态发生改变"*/

INT8U NewCntLen; /*"新增次数长度"*/
INT8U NewCntBuff[MAX_REVENT2_ID]; /*"新增次数暂存缓冲区"*/

static void Report_645SetEventEnd(INT8U *ReportPr);
static void Report_645EventChk(INT16U rcrd_obis);

void Report_SetStatus(void)
{
    FS_WriteFile(E_FILE_UI_DATA,EVRP645_BAK_SHIFT,(INT8U *)&m645EvtPrtStatus.un645NewRptStatus,3*sizeof(UN_645EVTRPT_STATUS));
}
void Report_SetFlg(void)
{
    FS_WriteFile(E_FILE_UI_DATA,EVRP645_STATUSFLG_SHIFT,(INT8U *)&m645EvtPrtStatus.rptTimer,4);
}

 /*"检测指定事件是否需要主动上报，实现对该事件标志置位和记录次数功能"*/	
void Report645_Chk(INT8U EentId,INT8U EventState)
{
    INT8U pModeTmp;
    INT8U *pStatueNew,*pStatueOld,*pStatueClr;
    INT8U StatueNewTmp,StatueOldTmp;
    INT8U byteTmp,bitTmp,RbitTmp;
	UN_645EVTRPT_MODE  evtRptMode;   

    GetSingle(E_645REPORT_MODE,(INT8U *)&evtRptMode);/*"主动上报模式字"*/
    
    byteTmp=mSI645EventInfo[EentId].eventStatue;
    bitTmp=mSI645EventInfo[EentId].eventStatueBit;
    RbitTmp = ~bitTmp;
    
    pModeTmp =evtRptMode.byte[mSI645EventInfo[EentId].eventMode];
    pStatueNew=(INT8U *)&m645EvtPrtStatus.un645NewRptStatus.byte[byteTmp];
    pStatueOld=(INT8U *)&m645EvtPrtStatus.un645OldRptStatus.byte[byteTmp];
    pStatueClr=(INT8U *)&ClrReportStatus.byte[byteTmp];
    StatueNewTmp = *pStatueNew&bitTmp;
    StatueOldTmp=*pStatueOld&bitTmp;
  
    if((pModeTmp&mSI645EventInfo[EentId].eventModeBit)>0)
    {
        if ((1==EventState) && (0 == StatueNewTmp))
        {
            NewStatusFlg = 1;
            *pStatueNew |=bitTmp;
            *pStatueClr |=bitTmp;
        }
        else if ((0 ==EventState) && (StatueOldTmp>0))
        {
            OldStatusFlg = 1;
            *pStatueOld &=RbitTmp;
            *pStatueNew &= RbitTmp;
        } 
        else
        {
        }	
        StatueNewTmp = *pStatueNew&bitTmp;
        StatueOldTmp=*pStatueOld&bitTmp;
    } 
    else/*"模式字未开启时，更新保存次数为当前次数"*/
    {
        if(EentId >= REVT_RLY)
        {
            if((1==EventState)&& (0 == StatueNewTmp)&&(m645EvtPrtStatus.rptEvtCnt[EentId-REVT_RLY]>0))
            {
                m645EvtPrtStatus.rptEvtCnt[EentId-REVT_RLY]=0;
                FS_WriteFile(E_FILE_UI_DATA,EVRP645_EVENTCNT_SHIFT,(INT8U *)&m645EvtPrtStatus.rptEvtCnt[0],MAX_645EVENT_CNT);
            }
            else
            {
            }
        }
    }
    /*"上报状态字不一致，上报是置1，则跟新增次数"*/
    if (StatueNewTmp != StatueOldTmp)
    {
        if(EentId<REVT_RLY)
        {
            NewCntBuff[NewCntLen]=0xff;
            NewCntLen += 1;
        }
        else
        {
            if(m645EvtPrtStatus.rptEvtCnt[EentId- REVT_RLY] >0)
            {
                 NewCntBuff[NewCntLen]=m645EvtPrtStatus.rptEvtCnt[EentId- REVT_RLY];
                 NewCntLen += 1;
            }
        }

    }
}


/*"645事件上报状态"*/
void Report645Status_Get(void)
{
    INT8U i;
    INT8U EventFlgTmp2[MAX_REVENT2_ID];
    
    NewStatusFlg=0;
    OldStatusFlg=0;
    NewCntLen = 0;

    LIB_MemSet(0,EventFlgTmp2,MAX_REVENT2_ID);  
    /*"标志类"*/
    /*
    if(ChkStatus(E_ESAM_ERR) == TRUE)
    {
        EventFlgTmp2[REVT_ESAM2]=1;
    }
    else
    {
    }
    */
    if(ChkStatus(E_CLK_BAT_LOSS) == TRUE)
    {
        EventFlgTmp2[REVT_BAT2]=1;
    }
    else
    {
    }
    if(ChkStatus(E_EEP_ERR) == TRUE)
    {
        EventFlgTmp2[REVT_EEP2]=1;
    }
    else
    {
    }
#ifdef THREE_PHASE  /*"单相表不需支持停电抄表电池欠压上报"*/
    if(ChkStatus(E_DIS_BAT_LOSS) == TRUE)
    {
        EventFlgTmp2[REVT_LOWBAT2]=1;
    }
    else
    {
    }
#endif
/*
    if(ChkStatus(E_OVERDRAFT) == TRUE)
    {
        EventFlgTmp2[REVT_CREGIT2]=1;
    }
    else
    {
    }
*/
    if(mEvtPrtStatus.rlyChkStatus.bits.rlyOffRpt == TRUE)
    {
        EventFlgTmp2[REVT_TRIP_SW2]=1;
    }
    else
    {
    }
    if(mEvtPrtStatus.rlyChkStatus.bits.rlyOnRpt == TRUE)
    {
        EventFlgTmp2[REVT_CLS_SW2]=1;
    }
    else
    {
    }

    for(i=0;i<MAX_REVENT2_ID;i++)
    {
        if(i>= REVT_RLY)
        {
            if(m645EvtPrtStatus.rptEvtCnt[i-REVT_RLY] > 0)
            {
                 EventFlgTmp2[i]=1;
            }
        }
        Report645_Chk(i,EventFlgTmp2[i]);
    }

    /*"状态有改变时需要存内卡"*/
    if (NewStatusFlg>0)
    {
        m645EvtPrtStatus.rptFlg = 1;
        Report_SetFlg();
        CMD_Eventout_On();  /*"置EVENTOUT"*/
    }
    if ((OldStatusFlg>0)||(NewStatusFlg>0))
    {
        Report_SetStatus(); 
    }  
    
}

/*"645读主动上报状态"*/
INT8U SI_645Report(INT8U *pdata)
{
    INT8U i,lenTmp,mrptTimer;

    for(i=0;i<12;i++)
    {
        pdata[i] = m645EvtPrtStatus.un645NewRptStatus.byte[i]^m645EvtPrtStatus.un645OldRptStatus.byte[i];
    }
    pdata[12] = 0xAA;/*"加AA分开"*/
    LIB_MemCpy(NewCntBuff, &pdata[13], NewCntLen);
    pdata[13 + NewCntLen] = 0xAA;/*"加AA分开"*/
    lenTmp = (12 + NewCntLen + 2); 
    LIB_MemCpy(&m645EvtPrtStatus.un645NewRptStatus.byte[0], &m645EvtPrtStatus.ClrReportStatus.byte[0], sizeof(UN_645EVTRPT_STATUS)); 
    Report_SetStatus();
    m645EvtPrtStatus.rptStartFlg = 1;
    GetSingle(E_RPTRST_TIME,(INT8U *)&mrptTimer);
    m645EvtPrtStatus.rptTimer = (INT16U)(mrptTimer*60L);
    Report_SetFlg();
    return lenTmp;
}

/*"645复位主动上报状态"*/
INT8U SI_645RstReport(INT8U *pdata)
{
    INT8U i,rsult;
    UN_645EVTRPT_STATUS comRptStatusTmp,clrRptStatusTmp;/*"需要清除的标志位"*/    
    rsult = TRUE;
    if(*(pdata-13)==24)/*"645帧中的数据长度"*/
    {
        if(1==m645EvtPrtStatus.rptStartFlg)
        {
            LIB_MemCpy(pdata, &comRptStatusTmp.byte[0], sizeof(UN_645EVTRPT_STATUS)); 
            for(i=0;i<sizeof(UN_645EVTRPT_STATUS);i++)
            {
                clrRptStatusTmp.byte[i]=m645EvtPrtStatus.un645NewRptStatus.byte[i]^m645EvtPrtStatus.ClrReportStatus.byte[i];
                if((clrRptStatusTmp.byte[i]&(~comRptStatusTmp.byte[i]))>0)
                {
                    rsult = FALSE;
                    break;
                }
            }
            if(TRUE==rsult)
            {
                Report_645SetEventEnd(pdata);
                m645EvtPrtStatus.rptStartFlg = 0;
                Report_SetFlg();
            }
            else
            {
            } 
        }
        else
        {
            rsult = FALSE;
        }
    }
    else
    {
        rsult = FALSE;
    }    
    return rsult;           
}

/*"当指针为0时，主动清主动上报状态字"*/
static void Report_645SetEventEnd(INT8U *ReportPr)
{
    INT8U i,valTmp;
    UN_645EVTRPT_STATUS *ReportStatus;
    INT8U byteTmp,bitTmp,RbitTmp;
	UN_645EVTRPT_MODE  evtRptMode;   
	
	GetSingle(E_645REPORT_MODE,(INT8U *)&evtRptMode);/*"主动上报模式字"*/

    ReportStatus = (UN_645EVTRPT_STATUS *)ReportPr;
    if (ReportPr != 0)
    {
        /*"为1不动，为0清状态字"*/
        /*"各事件新增次数清零"*/
        for(i=0;i<MAX_REVENT2_ID;i++)
        {
            byteTmp=mSI645EventInfo[i].eventStatue;
            bitTmp=mSI645EventInfo[i].eventStatueBit;
            RbitTmp = ~bitTmp;    
            if((ReportStatus->byte[byteTmp]&bitTmp)==0)/*"状态清零"*/
            {   
                if(i >= REVT_RLY)/*"清事件次数"*/
                {
                    m645EvtPrtStatus.rptEvtCnt[i-REVT_RLY]=0;
                }
                if((evtRptMode.byte[mSI645EventInfo[i].eventMode]&mSI645EventInfo[i].eventModeBit)>0)
                {
                    m645EvtPrtStatus.un645OldRptStatus.byte[byteTmp] |= (m645EvtPrtStatus.un645NewRptStatus.byte[byteTmp]&bitTmp);
                }
                else
                {
                    m645EvtPrtStatus.un645OldRptStatus.byte[byteTmp] &=RbitTmp;
                    m645EvtPrtStatus.un645NewRptStatus.byte[byteTmp] &=RbitTmp;
                }  
            }
            else/*"状态不清零"*/
            {
            }
        }

    }
    else
    {
        LIB_MemSet(0, &m645EvtPrtStatus.un645NewRptStatus.byte[0],2*sizeof(UN_645EVTRPT_STATUS)); 
        LIB_MemSet(0, (INT8U *)&m645EvtPrtStatus.rptEvtCnt[0], MAX_645EVENT_CNT); 
        if(ChkStatus(E_PRIVATE) == TRUE)/*"私钥状态下，总清零需上报"*/
        {
            m645EvtPrtStatus.rptEvtCnt[REVT_CLR_MTR - REVT_RLY]= 1;
        }
    }
    
    valTmp = 0;
    for(i=0;i<sizeof(UN_645EVTRPT_STATUS);i++)
    {
		valTmp = (m645EvtPrtStatus.un645NewRptStatus.byte[i]^m645EvtPrtStatus.un645OldRptStatus.byte[i]);
		if(valTmp != 0)
		{
			break;
		}
    }
    if(valTmp == 0)
    {
        m645EvtPrtStatus.rptFlg=0; /*"主动上报后立即清主动上报标志"*/
        CMD_Eventout_Off();
    }
    
    FS_WriteFile(E_FILE_UI_DATA,EVRP645_BAK_SHIFT,(INT8U *)&m645EvtPrtStatus.un645NewRptStatus,sizeof(ST_645EVTRPT_BLK));
    Report645Status_Get();
    
}


/*"自动复位延时检测"*/
void Report_1Sec(void)
{
    INT8U i;
    UN_645EVTRPT_STATUS clrRptStatusTmp;/*"需要清除的标志位"*/    
    INT8U *pClrRpt;
    pClrRpt=clrRptStatusTmp.byte;
    if(m645EvtPrtStatus.rptStartFlg>0)/*"主动上报定时复位检测"*/
    {
        if(m645EvtPrtStatus.rptTimer>0)
        {
            m645EvtPrtStatus.rptTimer-=1;
            if(0==m645EvtPrtStatus.rptTimer)
            {  
                m645EvtPrtStatus.rptStartFlg=0;
                for(i=0;i<sizeof(UN_645EVTRPT_STATUS);i++)
                {
                    *(pClrRpt+i)=(m645EvtPrtStatus.un645NewRptStatus.byte[i]^m645EvtPrtStatus.ClrReportStatus.byte[i]);
                }
                Report_645SetEventEnd(pClrRpt);  
            }
            else
            {
            }
			if((m645EvtPrtStatus.rptTimer%60) == 0)
            /*"每一分钟存数据中心，掉电最大存在一分钟的差值"*/
			{
				Report_SetFlg();
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


/*"645事件上报置位"*/
static void Report_645EventChk(INT16U rcrd_obis)
{
    INT8U i,wrflg;
    INT8U pModeTmp;
	UN_645EVTRPT_MODE  evtRptMode;   

    wrflg = FALSE;
    for(i=REVT_RLY;i<MAX_REVENT2_ID;i++)
    {
        if(rcrd_obis == mSI645EventInfo[i].rcrd_obis)
        {
            m645EvtPrtStatus.rptEvtCnt[i-REVT_RLY]+=1;
            wrflg = TRUE;
            break;
        }
    }
    if(wrflg == TRUE)
    {
        GetSingle(E_645REPORT_MODE,(INT8U *)&evtRptMode);/*"主动上报模式字"*/
        pModeTmp = evtRptMode.byte[mSI645EventInfo[i].eventMode];

        if((pModeTmp & mSI645EventInfo[i].eventModeBit) > 0)
        {
            FS_WriteFile(E_FILE_UI_DATA,EVRP645_EVENTCNT_SHIFT,(INT8U *)&m645EvtPrtStatus.rptEvtCnt[0],MAX_645EVENT_CNT); 
        }
    }
}
#endif





/*"*****************************事件主动上报*****************************"*/

/*"*****分为电能表跟随上报状态字2015和主动上报对象3320****"*/

ST_EVTRPT_STATUS mEvtPrtStatus;/*"事件主动上报变量"*/


/*"上报状态字处理"*/
INT8U OldStatuschangeFlg[REPORTCOMNUM];/*"已发生事件状态发生改变"*/
INT8U NewStatuschangeFlg[REPORTCOMNUM];/*"未发生事件状态发生改变"*/

//static INT8S Check_EvtReport_Eable(UN_OI *evtOi, INT8U repMode);

typedef enum 
{
    REVT_ESAM=0,   	 /*"ESAM错"*/
    REVT_BAT,   	 /*"时钟电池电压低"*/
    REVT_EEP,   	 /*"存储器故障或损坏"*/
    REVT_LOWBAT,   	 /*"停电抄表电池欠压"*/
    REVT_CREGIT,  	 /*"透支状态"*/
    REVT_TRIP_SW,  	 /*" 跳闸成功 "*/
    REVT_CLS_SW,  	 /*" 合闸成功"*/
    MAX_REVENT_ID
}EN_REPORT_EVENT_ID;

const ST_REPORTEVENTINFO mSIEventInfo[]=
{
    {0x00,0x02}, /*"ESAM错误"*/ 
    {0x00,0x08}, /*"时钟电池电压低"*/
    {0x00,0x20}, /*"存储器故障或损坏"*/
    {0x01,0x01}, /*"停电抄表电池欠压"*/
    {0x01,0x02}, /*"透支状态"*/
    {0x01,0x40}, /*"跳闸成功"*/
    {0x01,0x80}, /*"合闸成功"*/      
};


/*"*****************************************************************************"*/
/*"  Function:       PLC_EventOut_Set"*/
/*"  Description:   EVENOUT脚状态置位"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          NONE"*/
/*"  Output:        NONE "*/
/*"  Return:         NONE       "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void PLC_EventOut_Set(void)
{
	if((mEvtPrtStatus.OadNum[EVRP_ID_CM] != 0)||(mEvtPrtStatus.rptFlg[EVRP_ID_CM] == TRUE)) /*"载波通道置EVENTOUT"*/
	{
		CMD_Eventout_On();
	}
	else
	{
		CMD_Eventout_Off();
	}

    #if 0
    #ifdef REPORT645 
    if(1==m645EvtPrtStatus.rptFlg)	/*"置EVENTOUT"*/
    {
        CMD_Eventout_On();
    }
    else
    {
		CMD_Eventout_Off();
    }
    #endif
    #endif
}

/*"*********************************************************"*/
/*"Function:   Report_Init"*/
/*"Description: 事件上报模块上电初始化"*/
/*"Input:    NONE"*/
/*"Output:"*/ 
/*"Return: "*/
/*"*********************************************************"*/
void Report_Init(void)
{   
    #ifdef REPORT645 
    //PrStateST = 0;
    FS_ReadFile(E_FILE_UI_DATA,EVRP645_BAK_SHIFT,(INT8U *)&m645EvtPrtStatus,sizeof(ST_645EVTRPT_BLK));
    #endif
	//PLC_EventOut_Set();
	if(ChkStatus(E_FAC) == FALSE)/*"为了防止反复上报，厂外取主动上报时的状态"*/
	{
		if(mEvtPrtStatus.unNewRptStatus[0].bits.BatteryErr == TRUE)/*"时钟电池欠压"*/
		{
			Push(E_CLK_BAT_LOSS,TRUE);
		}

		if(mEvtPrtStatus.unNewRptStatus[0].bits.LowBatErr == TRUE)/*"停电抄表电池欠压"*/
		{
			Push(E_DIS_BAT_LOSS,TRUE);
		}
	}
}


/*"*****************************************************************************"*/
/*"  Function:       Report_GetStatus"*/
/*"  Description:   抄读主动上报状态字"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          uartID 通道号 0 485 1 载波"*/
/*"  Output:        NONE "*/
/*"  Return:         NONE       "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Report_GetStatus(INT8U *pdout)
{
    INT8U i,uartID;
	
	uartID = Report_Channal();

    for(i=0;i<4;i++)
    {
        pdout[i] = mEvtPrtStatus.unNewRptStatus[uartID].byte[i]^mEvtPrtStatus.unOldRptStatus[uartID].byte[i];
    }
	
	mEvtPrtStatus.rptStartFlg[uartID] = 1;

	return DAR_READ_FUN_DIRECT_DATA;
}

/*"*****************************************************************************"*/
/*"  Function:       PP_EvtReport"*/
/*"  Description:   发生新增事件，处理主动上报"*/
/*"  Calls:          "*/
/*"  Called By:      数据中心"*/
/*"  Input:          Obis: 记录对象 "*/
/*"  Input:        mode: 增加或清除上报信息"*/
/*"0清除上报，1事件发生上报，2事件结束上报"*/

/*"  Return:        无"*/
/*"*****************************************************************************"*/
INT8S PP_EvtReport(void *pdin, void *pdout)
{
    //INT8U i;
    INT8U mode,*pData;
    //ST_REPRORT_OI OiLst;
    //ST_OAD evtOad;
	INT16U Evt_Obis;

	pData = (INT8U *)(pdin);
	LIB_MemCpy(pData,(INT8U *)&Evt_Obis,2);
    mode = *(pData+2);
	#if 0
	if((E_PRG_EVNT_RCRD == Evt_Obis)&&(mode > 0))/*"编程事件没有结束，应该传1，特殊处理"*/
	{
		mode = 1;
	}
	#endif

#if 0	
	if(ChkStatus(E_MTR_INIT) == TRUE)/*"上电没初始化完成，以内卡数据为准"*/
	{
		FS_ReadFile(E_FILE_UI_DATA,EVTRPTSTATUS_SHIFT,(INT8U *)&mEvtPrtStatus,sizeof(ST_EVTRPT_STATUS));
	}

	FS_ReadFile(E_FILE_UI_DATA,EVTREPTOI_SHIFT,(INT8U *)&OiLst,sizeof(ST_REPRORT_OI));

    PP_ObisToOad(Evt_Obis,&evtOad);	/*"PP_ObisToOad"*/

	if(mode >0)
	{
		for(i = 0; i < mEvtPrtStatus.OiNum; i++)
		{
			if(EQUAL == LIB_MemCmp((INT8U *)&evtOad.Oi, OiLst.Oi[i], sizeof(UN_OI)))/*"事件的OI在上报列表里面"*/
			{
				if(Check_EvtReport_Eable(&evtOad.Oi,OiLst.rptCtrl[i]) == MS_SUCCESS)
				{
					if(mode&OiLst.rptCtrl[i])/*"上报是否和设置的相符"*/
					{
						Report_SetOrClrOad((INT8U *)&evtOad, mode);
					}
					else/*"不做处理，直接返回"*/
					{
					}
				}
				else/*"不做处理，直接返回"*/
				{
				}
				break;
			}
		}
        
        #ifdef REPORT645
        if(mode == 1)/*"645上报只考虑发生上报"*/
        {
            Report_645EventChk((INT8U *)&evtOad);
        }
        #endif
	}
	else
	{
		Report_SetOrClrOad((INT8U *)&evtOad, CLR_RPT);/*"清除上报列表"*/
	}
#else
    if(mode > 0)
    {
        Report_645EventChk(Evt_Obis);
    }
    else
    {
        //Report_SetOrClrOad((INT8U *)&evtOad, CLR_RPT);/*"清除上报列表"*/
    }
#endif
	return MS_SUCCESS; 
	
}

/*"*********************************************************"*/
/*"Function:   Report_Clr"*/
/*"Description:清复位状态，clrtype 0电表总清零，1事件总清零"*/
/*"Input:    NONE"*/
/*"Output:"*/ 
/*"Return: "*/
/*"*********************************************************"*/
void Report_Clr(INT8U clrtype)
{
#if 0
	INT8U OAD[4],i;
	ST_REPRORT_OI OiLst;
	
	Report_SetEventEnd(0,0);
	OAD[1] = 0x30;
	OAD[2] = 0x02;
	OAD[3] = 0x00;
	if(clrtype == 0)
	{
		OAD[0] = 0x13;
	}
	else
	{
		OAD[0] = 0x15;
	}
	
	/*"添加电表清零主动上报"*/
	/*"添加事件清零主动上报"*/
	FS_ReadFile(E_FILE_UI_DATA,EVTREPTOI_SHIFT,(INT8U *)&OiLst,sizeof(ST_REPRORT_OI));

	for(i = 0; i < mEvtPrtStatus.OiNum; i++)
	{
		if(EQUAL == LIB_MemCmp(OAD, OiLst.Oi[i], 2))
		{
			Report_SetOrClrOad(OAD,SET);
		}
	}
#endif

    #ifdef REPORT645 
    if(0==clrtype)
    {
        Report_645SetEventEnd(0);/*"先对清零状态清零，然后再置位"*/
    }
    else
    {
        LIB_MemSet(0, &ClrReportStatus.byte[0],sizeof(UN_645EVTRPT_STATUS)); 
        ClrReportStatus.bits.MeterClr=1;
        ClrReportStatus.bits.EventClr=1;
        Report_645SetEventEnd(&ClrReportStatus.byte[0]);
    }
    #endif

}

/*"************************************************         
Function:UPdate_RlySucc
Description:继电器跳闸合闸成功标志更新
Input:None  
Output:None
Return: None
Others: None
************************************************"*/
INT8S UPdate_RlySucc(void *pdin, void *pdout)
{
	//INT8U *pdata;
	//pdata = (INT8U *)(pdin);

	//mEvtPrtStatus.rlyChkStatus.byte = *pdata;
	//FS_WriteFile(E_FILE_UI_DATA,RLYCTRSTA_SHIFT,(INT8U *)&mEvtPrtStatus.rlyChkStatus,1);

	return _PAY_OK;
}

