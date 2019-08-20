/*"*****************************************************************************"*/
/*"  FileName: Reprot.c"*/
/*"  Description:     ���ļ�ʵ���¼��ϱ���ؽӿں�������"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.12.01"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
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
//INT8U PrStateST;/*"����¼��ϱ���ʼ״̬"*/
/*"------------------------645�¼������ϱ�-----------------------"*/
typedef enum 
{
    REVT_ESAM2=0,   	 /*"ESAM��"*/
    REVT_BAT2,   	 /*"ʱ�ӵ�ص�ѹ��"*/
    REVT_EEP2,   	 /*"�洢�����ϻ���"*/
    REVT_LOWBAT2,   	 /*"ͣ�糭����Ƿѹ"*/
    REVT_CREGIT2,  	 /*"͸֧״̬"*/
    REVT_TRIP_SW2,  	 /*" ��բ�ɹ� "*/
    REVT_CLS_SW2,  	 /*" ��բ�ɹ�"*/

    /*"�¼����ϱ�"*/
    REVT_RLY,        /*"���ɿ�������"*/
    REVT_OPEN,       /*"�����"*/
    REVT_OCRR,       /*"����"*/
    REVT_CLR_MTR,    /*" ������� "*/
    
    REVT_RTC,        /*"ʱ�ӹ���"*/
    REVT_PD,         /*"����"*/
    REVT_PRG,        /*" ��� "*/
    REVT_CLR_EVT,    /*" �¼����� "*/
    REVT_JUST_TIME,  /*" Уʱ "*/

    REVT_PRG_DSLOT,  /*" ʱ�α��� "*/
    REVT_PRG_YSLOT,  /*" ʱ������ "*/
    REVT_PRG_WEEK,  /*" �����ձ�� "*/  
    REVT_PRG_BILL,  /*" �����ձ�� "*/
    REVT_PRG_AEMODE,  /*" �й���Ϸ�ʽ��� "*/
    REVT_PRG_TARIFF, /*" ���ʱ��� "*/
    REVT_PRG_LEVEL, /*" ���ݱ��� "*/
    REVT_KEY_UPDATE,  /*" ��Կ���� "*/
    
    MAX_REVENT2_ID
}EN_REPORT_EVENT2_ID;

const ST_645REPORTEVENTINFO mSI645EventInfo[]=
{
    {0x0000,0x00,0x02,0x00,0x02}, /*"ESAM����"*/ 
    {0x0000,0x00,0x08,0x00,0x08}, /*"ʱ�ӵ�ص�ѹ��"*/
    {0x0000,0x00,0x20,0x00,0x20}, /*"�洢�����ϻ���"*/
    {0x0000,0x01,0x01,0x01,0x01}, /*"ͣ�糭����Ƿѹ"*/
    {0x0000,0x01,0x02,0x01,0x02}, /*"͸֧״̬"*/
    {0x0000,0x01,0x40,0x01,0x40}, /*"��բ�ɹ�"*/
    {0x0000,0x01,0x80,0x01,0x80}, /*"��բ�ɹ�"*/   

    /*"�¼���"*/
    {(INT16U)E_RELAY_ERR_EVNT_RCRD,0x00,0x01,0x00,0x01}, /*"���ɿ�������"*/
    {(INT16U)E_MTR_COVER_OPEN_EVNT_RCRD,0x01,0x04,0x01,0x04}, /*"�����"*/
    //{(INT16U)E_OVER_CURR_LA_EVNT_RCRD,0x02,0x10,0x02,0x10}, /*"����"*/  
    {(INT16U)E_MTR_CLR_EVNT_RCRD,0x06,0x02,0x0A,0x02}, /*"�������"*/  
    
    {(INT16U)E_RTC_ERR_EVNT_RCRD,0x00,0x80,0x00,0x80}, /*"ʱ�ӹ���"*/
    {(INT16U)E_PWR_DOWN_EVNT_RCRD,0x04,0x20,0x08,0x20}, /*"����"*/  
   
    {(INT16U)E_EVNT_CLR_EVNT_RCRD,0x06,0x08,0x0A,0x08}, /*"�¼�����"*/  
    {(INT16U)E_JUST_TIME_EVNT_RCRD,0x06,0x10,0x0A,0x10}, /*"Уʱ"*/ 

               
    {(INT16U)0x0000,0x07,0x20,0x0B,0x20}, /*"���ʲ�������"*/     
    {(INT16U)0x0000,0x07,0x40,0x0B,0x40}, /*"���ݱ���"*/        
    {(INT16U)0x0000,0x07,0x80,0x0B,0x80}, /*"��Կ����"*/    
};


UN_645EVTRPT_STATUS ClrReportStatus;/*"��Ҫ����ı�־λ"*/
ST_645EVTRPT_BLK m645EvtPrtStatus;  /*"645Э���¼������ϱ�����"*/
INT8U OldStatusFlg;/*"645�ѷ����¼�״̬�����ı�"*/
INT8U NewStatusFlg;/*"645δ�����¼�״̬�����ı�"*/

INT8U NewCntLen; /*"������������"*/
INT8U NewCntBuff[MAX_REVENT2_ID]; /*"���������ݴ滺����"*/

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

 /*"���ָ���¼��Ƿ���Ҫ�����ϱ���ʵ�ֶԸ��¼���־��λ�ͼ�¼��������"*/	
void Report645_Chk(INT8U EentId,INT8U EventState)
{
    INT8U pModeTmp;
    INT8U *pStatueNew,*pStatueOld,*pStatueClr;
    INT8U StatueNewTmp,StatueOldTmp;
    INT8U byteTmp,bitTmp,RbitTmp;
	UN_645EVTRPT_MODE  evtRptMode;   

    GetSingle(E_645REPORT_MODE,(INT8U *)&evtRptMode);/*"�����ϱ�ģʽ��"*/
    
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
    else/*"ģʽ��δ����ʱ�����±������Ϊ��ǰ����"*/
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
    /*"�ϱ�״̬�ֲ�һ�£��ϱ�����1�������������"*/
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


/*"645�¼��ϱ�״̬"*/
void Report645Status_Get(void)
{
    INT8U i;
    INT8U EventFlgTmp2[MAX_REVENT2_ID];
    
    NewStatusFlg=0;
    OldStatusFlg=0;
    NewCntLen = 0;

    LIB_MemSet(0,EventFlgTmp2,MAX_REVENT2_ID);  
    /*"��־��"*/
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
#ifdef THREE_PHASE  /*"�������֧��ͣ�糭����Ƿѹ�ϱ�"*/
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

    /*"״̬�иı�ʱ��Ҫ���ڿ�"*/
    if (NewStatusFlg>0)
    {
        m645EvtPrtStatus.rptFlg = 1;
        Report_SetFlg();
        CMD_Eventout_On();  /*"��EVENTOUT"*/
    }
    if ((OldStatusFlg>0)||(NewStatusFlg>0))
    {
        Report_SetStatus(); 
    }  
    
}

/*"645�������ϱ�״̬"*/
INT8U SI_645Report(INT8U *pdata)
{
    INT8U i,lenTmp,mrptTimer;

    for(i=0;i<12;i++)
    {
        pdata[i] = m645EvtPrtStatus.un645NewRptStatus.byte[i]^m645EvtPrtStatus.un645OldRptStatus.byte[i];
    }
    pdata[12] = 0xAA;/*"��AA�ֿ�"*/
    LIB_MemCpy(NewCntBuff, &pdata[13], NewCntLen);
    pdata[13 + NewCntLen] = 0xAA;/*"��AA�ֿ�"*/
    lenTmp = (12 + NewCntLen + 2); 
    LIB_MemCpy(&m645EvtPrtStatus.un645NewRptStatus.byte[0], &m645EvtPrtStatus.ClrReportStatus.byte[0], sizeof(UN_645EVTRPT_STATUS)); 
    Report_SetStatus();
    m645EvtPrtStatus.rptStartFlg = 1;
    GetSingle(E_RPTRST_TIME,(INT8U *)&mrptTimer);
    m645EvtPrtStatus.rptTimer = (INT16U)(mrptTimer*60L);
    Report_SetFlg();
    return lenTmp;
}

/*"645��λ�����ϱ�״̬"*/
INT8U SI_645RstReport(INT8U *pdata)
{
    INT8U i,rsult;
    UN_645EVTRPT_STATUS comRptStatusTmp,clrRptStatusTmp;/*"��Ҫ����ı�־λ"*/    
    rsult = TRUE;
    if(*(pdata-13)==24)/*"645֡�е����ݳ���"*/
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

/*"��ָ��Ϊ0ʱ�������������ϱ�״̬��"*/
static void Report_645SetEventEnd(INT8U *ReportPr)
{
    INT8U i,valTmp;
    UN_645EVTRPT_STATUS *ReportStatus;
    INT8U byteTmp,bitTmp,RbitTmp;
	UN_645EVTRPT_MODE  evtRptMode;   
	
	GetSingle(E_645REPORT_MODE,(INT8U *)&evtRptMode);/*"�����ϱ�ģʽ��"*/

    ReportStatus = (UN_645EVTRPT_STATUS *)ReportPr;
    if (ReportPr != 0)
    {
        /*"Ϊ1������Ϊ0��״̬��"*/
        /*"���¼�������������"*/
        for(i=0;i<MAX_REVENT2_ID;i++)
        {
            byteTmp=mSI645EventInfo[i].eventStatue;
            bitTmp=mSI645EventInfo[i].eventStatueBit;
            RbitTmp = ~bitTmp;    
            if((ReportStatus->byte[byteTmp]&bitTmp)==0)/*"״̬����"*/
            {   
                if(i >= REVT_RLY)/*"���¼�����"*/
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
            else/*"״̬������"*/
            {
            }
        }

    }
    else
    {
        LIB_MemSet(0, &m645EvtPrtStatus.un645NewRptStatus.byte[0],2*sizeof(UN_645EVTRPT_STATUS)); 
        LIB_MemSet(0, (INT8U *)&m645EvtPrtStatus.rptEvtCnt[0], MAX_645EVENT_CNT); 
        if(ChkStatus(E_PRIVATE) == TRUE)/*"˽Կ״̬�£����������ϱ�"*/
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
        m645EvtPrtStatus.rptFlg=0; /*"�����ϱ��������������ϱ���־"*/
        CMD_Eventout_Off();
    }
    
    FS_WriteFile(E_FILE_UI_DATA,EVRP645_BAK_SHIFT,(INT8U *)&m645EvtPrtStatus.un645NewRptStatus,sizeof(ST_645EVTRPT_BLK));
    Report645Status_Get();
    
}


/*"�Զ���λ��ʱ���"*/
void Report_1Sec(void)
{
    INT8U i;
    UN_645EVTRPT_STATUS clrRptStatusTmp;/*"��Ҫ����ı�־λ"*/    
    INT8U *pClrRpt;
    pClrRpt=clrRptStatusTmp.byte;
    if(m645EvtPrtStatus.rptStartFlg>0)/*"�����ϱ���ʱ��λ���"*/
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
            /*"ÿһ���Ӵ��������ģ�����������һ���ӵĲ�ֵ"*/
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


/*"645�¼��ϱ���λ"*/
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
        GetSingle(E_645REPORT_MODE,(INT8U *)&evtRptMode);/*"�����ϱ�ģʽ��"*/
        pModeTmp = evtRptMode.byte[mSI645EventInfo[i].eventMode];

        if((pModeTmp & mSI645EventInfo[i].eventModeBit) > 0)
        {
            FS_WriteFile(E_FILE_UI_DATA,EVRP645_EVENTCNT_SHIFT,(INT8U *)&m645EvtPrtStatus.rptEvtCnt[0],MAX_645EVENT_CNT); 
        }
    }
}
#endif





/*"*****************************�¼������ϱ�*****************************"*/

/*"*****��Ϊ���ܱ�����ϱ�״̬��2015�������ϱ�����3320****"*/

ST_EVTRPT_STATUS mEvtPrtStatus;/*"�¼������ϱ�����"*/


/*"�ϱ�״̬�ִ���"*/
INT8U OldStatuschangeFlg[REPORTCOMNUM];/*"�ѷ����¼�״̬�����ı�"*/
INT8U NewStatuschangeFlg[REPORTCOMNUM];/*"δ�����¼�״̬�����ı�"*/

//static INT8S Check_EvtReport_Eable(UN_OI *evtOi, INT8U repMode);

typedef enum 
{
    REVT_ESAM=0,   	 /*"ESAM��"*/
    REVT_BAT,   	 /*"ʱ�ӵ�ص�ѹ��"*/
    REVT_EEP,   	 /*"�洢�����ϻ���"*/
    REVT_LOWBAT,   	 /*"ͣ�糭����Ƿѹ"*/
    REVT_CREGIT,  	 /*"͸֧״̬"*/
    REVT_TRIP_SW,  	 /*" ��բ�ɹ� "*/
    REVT_CLS_SW,  	 /*" ��բ�ɹ�"*/
    MAX_REVENT_ID
}EN_REPORT_EVENT_ID;

const ST_REPORTEVENTINFO mSIEventInfo[]=
{
    {0x00,0x02}, /*"ESAM����"*/ 
    {0x00,0x08}, /*"ʱ�ӵ�ص�ѹ��"*/
    {0x00,0x20}, /*"�洢�����ϻ���"*/
    {0x01,0x01}, /*"ͣ�糭����Ƿѹ"*/
    {0x01,0x02}, /*"͸֧״̬"*/
    {0x01,0x40}, /*"��բ�ɹ�"*/
    {0x01,0x80}, /*"��բ�ɹ�"*/      
};


/*"*****************************************************************************"*/
/*"  Function:       PLC_EventOut_Set"*/
/*"  Description:   EVENOUT��״̬��λ"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          NONE"*/
/*"  Output:        NONE "*/
/*"  Return:         NONE       "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void PLC_EventOut_Set(void)
{
	if((mEvtPrtStatus.OadNum[EVRP_ID_CM] != 0)||(mEvtPrtStatus.rptFlg[EVRP_ID_CM] == TRUE)) /*"�ز�ͨ����EVENTOUT"*/
	{
		CMD_Eventout_On();
	}
	else
	{
		CMD_Eventout_Off();
	}

    #if 0
    #ifdef REPORT645 
    if(1==m645EvtPrtStatus.rptFlg)	/*"��EVENTOUT"*/
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
/*"Description: �¼��ϱ�ģ���ϵ��ʼ��"*/
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
	if(ChkStatus(E_FAC) == FALSE)/*"Ϊ�˷�ֹ�����ϱ�������ȡ�����ϱ�ʱ��״̬"*/
	{
		if(mEvtPrtStatus.unNewRptStatus[0].bits.BatteryErr == TRUE)/*"ʱ�ӵ��Ƿѹ"*/
		{
			Push(E_CLK_BAT_LOSS,TRUE);
		}

		if(mEvtPrtStatus.unNewRptStatus[0].bits.LowBatErr == TRUE)/*"ͣ�糭����Ƿѹ"*/
		{
			Push(E_DIS_BAT_LOSS,TRUE);
		}
	}
}


/*"*****************************************************************************"*/
/*"  Function:       Report_GetStatus"*/
/*"  Description:   ���������ϱ�״̬��"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          uartID ͨ���� 0 485 1 �ز�"*/
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
/*"  Description:   ���������¼������������ϱ�"*/
/*"  Calls:          "*/
/*"  Called By:      ��������"*/
/*"  Input:          Obis: ��¼���� "*/
/*"  Input:        mode: ���ӻ�����ϱ���Ϣ"*/
/*"0����ϱ���1�¼������ϱ���2�¼������ϱ�"*/

/*"  Return:        ��"*/
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
	if((E_PRG_EVNT_RCRD == Evt_Obis)&&(mode > 0))/*"����¼�û�н�����Ӧ�ô�1�����⴦��"*/
	{
		mode = 1;
	}
	#endif

#if 0	
	if(ChkStatus(E_MTR_INIT) == TRUE)/*"�ϵ�û��ʼ����ɣ����ڿ�����Ϊ׼"*/
	{
		FS_ReadFile(E_FILE_UI_DATA,EVTRPTSTATUS_SHIFT,(INT8U *)&mEvtPrtStatus,sizeof(ST_EVTRPT_STATUS));
	}

	FS_ReadFile(E_FILE_UI_DATA,EVTREPTOI_SHIFT,(INT8U *)&OiLst,sizeof(ST_REPRORT_OI));

    PP_ObisToOad(Evt_Obis,&evtOad);	/*"PP_ObisToOad"*/

	if(mode >0)
	{
		for(i = 0; i < mEvtPrtStatus.OiNum; i++)
		{
			if(EQUAL == LIB_MemCmp((INT8U *)&evtOad.Oi, OiLst.Oi[i], sizeof(UN_OI)))/*"�¼���OI���ϱ��б�����"*/
			{
				if(Check_EvtReport_Eable(&evtOad.Oi,OiLst.rptCtrl[i]) == MS_SUCCESS)
				{
					if(mode&OiLst.rptCtrl[i])/*"�ϱ��Ƿ�����õ����"*/
					{
						Report_SetOrClrOad((INT8U *)&evtOad, mode);
					}
					else/*"��������ֱ�ӷ���"*/
					{
					}
				}
				else/*"��������ֱ�ӷ���"*/
				{
				}
				break;
			}
		}
        
        #ifdef REPORT645
        if(mode == 1)/*"645�ϱ�ֻ���Ƿ����ϱ�"*/
        {
            Report_645EventChk((INT8U *)&evtOad);
        }
        #endif
	}
	else
	{
		Report_SetOrClrOad((INT8U *)&evtOad, CLR_RPT);/*"����ϱ��б�"*/
	}
#else
    if(mode > 0)
    {
        Report_645EventChk(Evt_Obis);
    }
    else
    {
        //Report_SetOrClrOad((INT8U *)&evtOad, CLR_RPT);/*"����ϱ��б�"*/
    }
#endif
	return MS_SUCCESS; 
	
}

/*"*********************************************************"*/
/*"Function:   Report_Clr"*/
/*"Description:�帴λ״̬��clrtype 0��������㣬1�¼�������"*/
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
	
	/*"��ӵ�����������ϱ�"*/
	/*"����¼����������ϱ�"*/
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
        Report_645SetEventEnd(0);/*"�ȶ�����״̬���㣬Ȼ������λ"*/
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
Description:�̵�����բ��բ�ɹ���־����
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

