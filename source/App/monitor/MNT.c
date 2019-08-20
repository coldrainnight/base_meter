/*"************************************************************"*/
/*"FileName: MNT.c"*/
/*"Author:    Version :          "*/
/*"Date: 2017-11-16 09:11:36         "*/
/*" Description: ���ܱ�״̬���ģ�����ӿں���"*/
/*" ���ֽ���:          "*/
/*" Function List:                     "*/
/*"                                    "*/
/*" History:                           "*/
/*" <author> <time> <version > <desc>  "*/
/*"************************************************************"*/
#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"
#include "App\monitor\MNT.h"

#define  _RAM_CHK_BYTE_NUM   10

extern void StatusInit(INT8U mode);
extern void StatusTaskMnt(void);
extern void StatusTask1s(void);
extern void StatusTask1Min(void);
extern void OutFac(void);
extern void StatusTask1sPD(void);
extern void GE_Init(void);
extern void GE_EvtTimePwrUp(void);
extern void GE_PdEvtTimeInit(void);
extern void GE_Tsk1s(void);
extern void StatusEvntClr(INT16U evntFileId);
extern void GE_NetEvtClr(INT8U enumId);
extern void ResetFacRemainTime(INT8U *Indata);
extern void MNT_PwrUpSEInit(void);
#ifdef THREE_PHASE
extern void  VoltHealthInit(INT8U mode);
extern void  VoltHealthSecTask(void);
extern void  VoltHealthMinTask(void);
extern void StPdCurrSecTask(void);
extern void StPdCurrPdInit(void);
#endif

NO_INIT INT32U  EvntTimeAdd[E_MAX_GRID_EVNT];/*"�¼��ۼ�ʱ��"*/
NO_INIT INT16U  EvntTimeAddCrc;/*"�¼��ۼ�ʱ��У���"*/
NO_INIT ST_MNT_PARA MntPara;
NO_INIT INT8U ramChkFlg[_RAM_CHK_BYTE_NUM];
INT8U  FacStatusFlg;/*"����״̬��־��   1 ����    0�ǹ���"*/
#ifdef THREE_PHASE
INT16U mVolt[3];
INT16U mVoltMax;
INT16U mVoltMin;
INT16U mVoltBase;
INT32U mCurr[3];
INT32U mCurrMax;
INT32U mCurrMin;
INT32U mCurrBase;
INT32S mActivePower[4];
INT16U imBVoltRate;
INT16U imBCurrRate;
INT16U uiState;
INT32U mntPdLastTime;
INT32U TotalLostVoltTime;     /*"����ʧѹ�ۼ�ʱ��"*/
NO_INIT INT16U maxImBVoltRate;
NO_INIT INT16U maxImBCurrRate;
NO_INIT INT16U maxBadlyImBCurrRate;
#endif
NO_INIT INT8U mucRstInfo;

MNT_UP_DATA mMntUpData;   /*"����о��������о������"*/
MNT_DOWN_DATA mMntDownData;   /*"����о��������о������"*/

static void MNT_Task(void);
static void ChkRamDataValid(void);
static void setDefaultMntPara(void);
#ifdef THREE_PHASE
INT32U GetMaxValue(INT32U X,INT32U Y,INT32U Z);
static INT32U GetMinValue(INT32U X,INT32U Y,INT32U Z);
#endif
extern void MNT_AlarmInit(MNT_ALARM_SCR id);
extern INT8U MNT_AlarmEvtChk(INT8U evtId);
extern void MNT_AlarmSecTask(void);

static void MNT_UpDataSecTask(void);

void pdSaveRecord(void);

/*"*****************************************************************************"*/
/*"  Function:       MNT_Init"*/
/*"  Description:    ���״̬���ģ���ϵ��ʼ���ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      main"*/
/*"  Input:          mode ��ʼ��ģʽ"*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void MNT_Init(INT8U mode)
{

    if(FIRST_PWR_ON == mode)
    {
        setDefaultMntPara();
        GE_NetEvtClr(0xff);
        StatusEvntClr(0xffff);
    }
    else
    {
        ChkMntParaValid();
    }
    ChkRamDataValid();
    StatusInit(mode);
#ifdef THREE_PHASE
    VoltHealthInit(mode);
#endif
    minPowerInit();
    if((FIRST_PWR_ON == mode) || (MCU_RUNMODE == mode))
    {
        Task_MainTaskRegister(MAINTASK_MNT, MNT_Task);
	 
        MNT_AlarmInit(EA_ALARM_SCR_MAX);
#ifdef THREE_PHASE
	 pwrRstMode=PWR_RST_NORMAL;
#endif
        #ifdef GLX_CHG_CHK
        GPIO_ConfigPin(COM_RQ,INPUT,NULL_IN);
        #endif
    }
    else
    {
         GE_PdEvtTimeInit();
#ifdef THREE_PHASE
         if(pwrRstMode != PWR_RST_PD)
         {
            pwrRstMode = PWR_RST_BATT;
         }
         StPdCurrPdInit();
#endif
    }
#ifdef THREE_PHASE
    mntPdLastTime=0;
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_Task"*/
/*"  Description:    ���״̬���ģ���ϵ�����ע��������"*/
/*"  Calls:          "*/
/*"  Called By:      task"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void MNT_Task(void)
{
    StatusTaskMnt();
    if(TRUE == Task_ChkTsk_1s (MAINTASK_MNT))
    {
        StatusTask1s();
        GE_Tsk1s();
#ifdef THREE_PHASE
	 VoltHealthSecTask();
#endif
        minPowerSecTask();
	 MNT_AlarmSecTask();
        MNT_UpDataSecTask();
    }
    if(TRUE == Task_ChkTsk_1min (MAINTASK_MNT))
    {
        LIB_MemSet(0x5a, (INT8U *)&ramChkFlg[0], _RAM_CHK_BYTE_NUM);
        StatusTask1Min();
	 #ifdef THREE_PHASE
	 VoltHealthMinTask();
	 #endif
        minPowerMinTask();
    }
}
/*"*****************************************************************************"*/
/*"  Function:       MNT_TaskPd"*/
/*"  Description:    ���״̬���ģ��͹����������ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      main"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void MNT_TaskPd(void)
{
     #ifdef THREE_PHASE
     mntPdLastTime++;
     StPdCurrSecTask();
     #endif
     StatusTask1sPD();
}
/*"*****************************************************************************"*/
/*"  Function:       MNT_PwrDown"*/
/*"  Description:    ���״̬���ģ����紦��������"*/
/*"  Calls:          "*/
/*"  Called By:      ms����"*/
/*"  Input:          pIn ������� ���ޣ�"*/
/*"  Output:         pOut ������ݣ��ޣ�"*/
/*"  Return:         DAR ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_PwrDown(void *pIn, void *pOut)
{
    INT32U  RstChkFlg;
    if(FALSE == ChkStatus(E_MTR_INIT))
    {
       pdSaveRecord();
       LIB_MemSet(0x5a, (INT8U *)&ramChkFlg[0], _RAM_CHK_BYTE_NUM);
       RstChkFlg = 0x5a5a5a5a;
       FS_WriteFile(E_FILE_MNT, RST_CHK_FLG_SHIFT, (INT8U *) &RstChkFlg, 4);
    }
#ifdef THREE_PHASE
    pwrRstMode=PWR_RST_PD;
#endif
    return MS_SUCCESS;
}
/*"*****************************************************************************"*/
/*"  Function:       ChkRamDataValid"*/
/*"  Description:    ����ϵ�RAM���ݺϷ��Լ��"*/
/*"  Calls:          "*/
/*"  Called By:      ms����"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ChkRamDataValid(void)
{
    /*"MCU�ϵ縴λ��RAM��־�������쳣������ΪRAM���ݲ���ȷ"*/
    if(/*"todo (TRUE == MCU_Por_State())
      || "*/(EQUAL != LIB_ChkValue((INT8U *) &ramChkFlg[0], _RAM_CHK_BYTE_NUM, 0X5A)))
    {
        Push(E_RAM_VALID, FALSE);
    }
    else
    {
        Push(E_RAM_VALID, TRUE);
    }
}

typedef struct
{
    INT16U  file;
    INT16U  idMin;
    INT16U  idMax;
}ST_GRID_EVNT_FILE_TO_ID;

const ST_GRID_EVNT_FILE_TO_ID GridEvntFileToId[E_MAX_GRID_EVNT] =
        {
                {E_PWR_DOWN_EVNT_RCRD, E_EVT_PWR_DOWN ,E_EVT_PWR_DOWN},
		   #ifdef THREE_PHASE
		  {E_LOST_VOLT_LA_EVNT_RCRD, E_EVT_LOST_VOLT_LA,E_EVT_LOST_VOLT_LC},
		  {E_LOST_VOLT_LB_EVNT_RCRD, E_EVT_LOST_VOLT_LA,E_EVT_LOST_VOLT_LC},
		  {E_LOST_VOLT_LC_EVNT_RCRD, E_EVT_LOST_VOLT_LA,E_EVT_LOST_VOLT_LC},
		  {E_ALL_LOST_VOLT_EVNT_RCRD, E_EVT_LOST_VOLT_ALL,E_EVT_LOST_VOLT_ALL},
		  {E_LOST_CURR_LA_EVNT_RCRD, E_EVT_LOST_CURR_LA,E_EVT_LOST_CURR_LC},
		  {E_LOST_CURR_LB_EVNT_RCRD, E_EVT_LOST_CURR_LA,E_EVT_LOST_CURR_LC},
		  {E_LOST_CURR_LC_EVNT_RCRD, E_EVT_LOST_CURR_LA,E_EVT_LOST_CURR_LC},
		  {E_BRK_PHASE_LA_EVNT_RCRD, E_EVT_BREAK_PHASE_LA,E_EVT_BREAK_PHASE_LC},
		  {E_BRK_PHASE_LB_EVNT_RCRD, E_EVT_BREAK_PHASE_LA,E_EVT_BREAK_PHASE_LC},
		  {E_BRK_PHASE_LC_EVNT_RCRD, E_EVT_BREAK_PHASE_LA,E_EVT_BREAK_PHASE_LC},
		  #ifdef OVER_CURR_CHK
		  {E_OVER_CURR_LA_EVNT_RCRD, E_EVT_OVER_CURR_LA,E_EVT_OVER_CURR_LC},
		  {E_OVER_CURR_LB_EVNT_RCRD, E_EVT_OVER_CURR_LA,E_EVT_OVER_CURR_LC},
		  {E_OVER_CURR_LC_EVNT_RCRD, E_EVT_OVER_CURR_LA,E_EVT_OVER_CURR_LC},
		  #endif
		  {E_BRK_CURR_LA_EVNT_RCRD, E_EVT_FAIL_CURR_LA,E_EVT_FAIL_CURR_LC},
		  {E_BRK_CURR_LB_EVNT_RCRD, E_EVT_FAIL_CURR_LA,E_EVT_FAIL_CURR_LC},
		  {E_BRK_CURR_LC_EVNT_RCRD, E_EVT_FAIL_CURR_LA,E_EVT_FAIL_CURR_LC},
		  {E_OVER_VOLT_LA_EVNT_RCRD, E_EVT_OVER_VOLT_LA,E_EVT_OVER_VOLT_LC},
		  {E_OVER_VOLT_LB_EVNT_RCRD, E_EVT_OVER_VOLT_LA,E_EVT_OVER_VOLT_LC},
		  {E_OVER_VOLT_LC_EVNT_RCRD, E_EVT_OVER_VOLT_LA,E_EVT_OVER_VOLT_LC},
		  {E_LACK_VOLT_LA_EVNT_RCRD, E_EVT_UNDER_VOLT_LA,E_EVT_UNDER_VOLT_LC},
		  {E_LACK_VOLT_LB_EVNT_RCRD, E_EVT_UNDER_VOLT_LA,E_EVT_UNDER_VOLT_LC},
		  {E_LACK_VOLT_LC_EVNT_RCRD, E_EVT_UNDER_VOLT_LA,E_EVT_UNDER_VOLT_LC},
		  {E_PWR_REVERSE_EVNT_RCRD, E_EVT_PWR_REVERSE,E_EVT_PWR_REVERSE_LC},
		  {E_PWR_REVERSE_LA_EVNT_RCRD, E_EVT_PWR_REVERSE,E_EVT_PWR_REVERSE_LC},
		  {E_PWR_REVERSE_LB_EVNT_RCRD, E_EVT_PWR_REVERSE,E_EVT_PWR_REVERSE_LC},
		  {E_PWR_REVERSE_LC_EVNT_RCRD, E_EVT_PWR_REVERSE,E_EVT_PWR_REVERSE_LC},
		  {E_OVER_LOAD_LA_EVNT_RCRD, E_EVT_OVER_LOAD_LA, E_EVT_OVER_LOAD_LC},
		  {E_OVER_LOAD_LB_EVNT_RCRD, E_EVT_OVER_LOAD_LA, E_EVT_OVER_LOAD_LC},
		  {E_OVER_LOAD_LC_EVNT_RCRD, E_EVT_OVER_LOAD_LA, E_EVT_OVER_LOAD_LC},
		  {E_OVER_PA_DEMAND_EVNT_RCRD, E_EVT_OVER_DEMAND_PA,E_EVT_OVER_DEMAND_PA},
		  {E_OVER_NA_DEMAND_EVNT_RCRD, E_EVT_OVER_DEMAND_NA,E_EVT_OVER_DEMAND_NA},
		  {E_OVER_R_DEMAND_1_EVNT_RCRD, E_EVT_OVER_DEMAND_I,E_EVT_OVER_DEMAND_IV},
		  {E_OVER_R_DEMAND_2_EVNT_RCRD, E_EVT_OVER_DEMAND_I,E_EVT_OVER_DEMAND_IV},
		  {E_OVER_R_DEMAND_3_EVNT_RCRD, E_EVT_OVER_DEMAND_I,E_EVT_OVER_DEMAND_IV},
                {E_OVER_R_DEMAND_4_EVNT_RCRD, E_EVT_OVER_DEMAND_I,E_EVT_OVER_DEMAND_IV},
                {E_VOLT_IMBALANCE_EVNT_RCRD, E_EVT_VOLT_IMB,E_EVT_VOLT_IMB},
                {E_CURR_IMBALANCE_EVNT_RCRD, E_EVT_CURR_IMB,E_EVT_CURR_IMB},
                {E_CURR_IMBALANCE_BADLY_EVNT_RCRD, E_EVT_CURR_BADLY_IMB,E_EVT_CURR_BADLY_IMB},
                {E_FACTOR_LOW_EVNT_RCRD, E_EVT_OVER_FACTOR,E_EVT_OVER_FACTOR},
                {E_VOLT_INVERSE_EVNT_RCRD, E_EVT_VOLT_REVERSE,E_EVT_VOLT_REVERSE},
                {E_CURR_INVERSE_EVNT_RCRD, E_EVT_CURR_REVERSE,E_EVT_CURR_REVERSE},
                #else
                #ifdef OVER_CURR_CHK
		  //{E_OVER_CURR_LA_EVNT_RCRD, E_EVT_OVER_CURR_LA,E_EVT_OVER_CURR_LA},
		  #endif
		  #endif
        };
/*"*****************************************************************************"*/
/*"  Function:       MNT_EvntClr"*/
/*"  Description:    �¼����㴦�����ӿ�"*/
/*"  Calls:          "*/
/*"  Called By:      ms����"*/
/*"  Input:          pIn ��¼�ļ���ʶ"*/
/*"  Output:         NULL"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_EvntClr(void *pIn, void *pOut)
{
    INT8U i;
    INT16U j;
    INT16U fileId;

    LIB_MemCpy((INT8U*)pIn, (INT8U *)&fileId, 2);
    for(i = 0; i < E_MAX_GRID_EVNT; i++)
    {
        if(GridEvntFileToId[i].file == fileId)
        {
           for(j=GridEvntFileToId[i].idMin; j <= GridEvntFileToId[i].idMax; j++)
           {
	       GE_NetEvtClr((INT8U)j);
           }
           break;
        }
        else
        {
            if(fileId >= 0xfffe)
            {
                GE_NetEvtClr(0xff);
                StatusEvntClr(fileId);
                break;
            }
        }
    }
    if(i >=  E_MAX_GRID_EVNT)
    {
        StatusEvntClr(fileId);
    }

    return MS_SUCCESS;
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_OutFacCmd"*/
/*"  Description:    �˹����������"*/
/*"  Calls:          "*/
/*"  Called By:      ms����"*/
/*"  Input:          NULL"*/
/*"  Output:         NULL"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_OutFacCmd(void *pIn, void *pOut)
{
    if(SID_TRUE == SID_PollRead(SID_FAC))
    {
        return MS_OTHER_ERR;
    }
    else
    {
        OutFac();
    }

    return MS_SUCCESS;
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_FirstPwrUpOver"*/
/*"  Description:    ��һ���ϵ��ʼ�����"*/
/*"  Calls:          "*/
/*"  Called By:      ms����"*/
/*"  Input:          NULL"*/
/*"  Output:         NULL"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_FirstPwrUpOver(void *pIn, void *pOut)
{
    INT32U InitFlg;

    InitFlg = NEW_EEP_FLG;
    FS_WriteFile(E_FILE_MNT, NEW_EEP_CHK_FLG_SHIFT, (INT8U *) &InitFlg, 4);
    Push(E_FIRST_PWR_ON, FALSE);

    return MS_SUCCESS;
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_ReSetFacRemainTime"*/
/*"  Description:    ���ù���״̬ʣ��ʱ�䷽��"*/
/*"  Calls:          "*/
/*"  Called By:      ms����"*/
/*"  Input:          NULL"*/
/*"  Output:         NULL"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_ReSetFacRemainTime(void *pIn, void *pOut)
{
	INT8U Indata;
	if(pIn == NULL)
	{
		Indata = 0;
	}
	else
	{
		Indata = 1;
	}
	ResetFacRemainTime(&Indata);
    return MS_SUCCESS;
}
/*"*****************************************************************************"*/
/*"  Function:       ChkMntParaValid"*/
/*"  Description:    ��鷨��ģ��������ݺϷ��ԣ����ӱ������ݽ��лָ�����"*/
/*"  Calls:          "*/
/*"  Called By:      ��ʼ����set"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         TRUE  �����Ϸ�  FALSE �����Ƿ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ChkMntParaValid(void)
{
    INT8U rsult;

    rsult = LIB_ChkDataValid((INT8U *)&MntPara, FIND(ST_MNT_PARA, crc), MntPara.crc);
    if(FALSE == rsult)
    {
        FS_ReadFile(E_FILE_MNT_PARA, 0, (INT8U *)&MntPara, sizeof(ST_MNT_PARA));
        rsult = LIB_ChkDataValid((INT8U *)&MntPara, FIND(ST_MNT_PARA, crc), MntPara.crc);
    }

    return (rsult);
}
/*"*****************************************************************************"*/
/*"  Function:       setDefaultMntPara"*/
/*"  Description:    ���״̬���ģ�������ʼ���ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      MNT_Init"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void setDefaultMntPara(void)
{
    INT16U uni;

    for(uni = 0; uni < sizeof(ST_MNT_PARA); uni++)/*"RAM�в����ȳ�ʼ��ΪĬ��ֵ"*/
    {
        *((INT8U *)&MntPara + uni) = *((INT8U FAR_POINTER*)&MntParaDefault + uni);
    }
    MntPara.crc = CRC16((INT8U *)&MntPara, FIND(ST_MNT_PARA, crc));
    FS_WriteFile(E_FILE_MNT_PARA, 0, (INT8U *)&MntPara, sizeof(ST_MNT_PARA));

}
#ifdef THREE_PHASE
void MNT_RefreshVC(void)
{
   INT8U   i;
   INT32S sCurr[3];
   INT32U tempVoltBase;

   GetSingle(E_MU_STATUS, (INT8U *)&uiState);

   GetSingle(E_VOLT_BLK, (INT8U *)&mVolt[0]);
   
   for(i=0;i<3;i++)
   {
      mVolt[i]=mVolt[i]/10;   /*"10-1V"*/
   }

   if(MODE3P3W)
   {
      mVolt[1]=0;
   }

   GetSingle(E_NOMINAL_VOLT, (INT8U *)&tempVoltBase);
   mVoltBase=tempVoltBase/1000;

   mVoltMax=GetMaxValue(mVolt[0], mVolt[1], mVolt[2]);

   if(MODE3P3W)
   {
      mVoltMin=GetMinValue(mVolt[0], mVolt[0], mVolt[2]);
   }
   else
   {
      mVoltMin=GetMinValue(mVolt[0], mVolt[1], mVolt[2]);
   }

   GetSingle(E_CURR_BLK, (INT8U *)&sCurr[0]);

    if(MODE3P3W)
   {
      sCurr[1]=0;
   }
    
   for(i=0;i<3;i++)
   {
      mCurr[i]=LIB_ABS(sCurr[i]);     /*"10-4A"*/
   }

   GetSingle(E_NOMINAL_CURR, (INT8U *)&mCurrBase);

   mCurrMax=GetMaxValue(mCurr[0], mCurr[1], mCurr[2]);

   if(MODE3P3W)
   {
      mCurrMin=GetMinValue(mCurr[0], mCurr[0], mCurr[2]);
   }
   else
   {
      mCurrMin=GetMinValue(mCurr[0], mCurr[1], mCurr[2]);
   }

   GetSingle(E_ACTIVE_PWR_BLK, (INT8U *)&mActivePower[0]);
   for(i=0;i<4;i++)
   {
      mActivePower[i]=mActivePower[i]/100;    /*"10-4kW"*/
   }

   imBVoltRate=0;
   if(mVoltMax > 0)
   {
      imBVoltRate=(mVoltMax-mVoltMin)*(INT32U)10000/mVoltMax;   /*"10-2"*/
   }

   imBCurrRate=0;
   if(mCurrMax > 0)
   {
      imBCurrRate=(mCurrMax-mCurrMin)*(INT64U)10000/mCurrMax; /*"10-2"*/
   }

   GetSingle(E_CURR_BLK, (INT8U *)&stPdCurr.pdCurr[0]);
}

INT32U GetMaxValue(INT32U X,INT32U Y,INT32U Z)
{
    INT32U tempvalue;

    tempvalue = X;
    if(Y>X)
        tempvalue = Y;

    if(tempvalue <Z)
        tempvalue = Z;

    return tempvalue;
}

static INT32U GetMinValue(INT32U X,INT32U Y,INT32U Z)
{
    INT32U tempvalue;

    tempvalue = X;
    if(X > Y)
        tempvalue = Y;

    if(tempvalue >  Z)
        tempvalue = Z;

    return tempvalue;
}
#endif


static INT32U mntEvtLastFlag[EA_ALARM_SCR_MAX];
static INT32U mntEvtNowFlag[EA_ALARM_SCR_MAX];
static INT32U mntAlarmFlag[EA_ALARM_SCR_MAX];

static void MNT_AlarmInit(MNT_ALARM_SCR id)
{
   BUILD_BUG_ON(EA_ALARM_FLAG_MAX >= (sizeof(INT32U)*8));

   if((id == EA_BGT) ||(id == EA_ALARM_SCR_MAX))
   {
      mntEvtLastFlag[EA_BGT]=0;
      mntEvtNowFlag[EA_BGT]=0;
      mntAlarmFlag[EA_BGT]=0;
   } 
#ifdef THREE_PHASE   
   if((id == EA_LCD) ||(id == EA_ALARM_SCR_MAX))
   {
      mntEvtLastFlag[EA_LCD]=0;
      mntEvtNowFlag[EA_LCD]=0;
      mntAlarmFlag[EA_LCD]=0;
      DIS_ClrFlashStauSeg(STA_ALARM);  /*"LCD"*/ 
   }

    if((id == EA_RLY) ||(id == EA_ALARM_SCR_MAX))
   {
      mntEvtLastFlag[EA_RLY]=0;
      mntEvtNowFlag[EA_RLY]=0;
      mntAlarmFlag[EA_RLY]=0;
      OC_RelayAct(E_RELAY_SC_EVENT, E_RELAY_RELAYALARM, E_RELAY_MODE_LEVEL, RELAY_ACT_CLOSE, 0);/*"�����̵���"*/
   }
#endif
}

static INT8U MNT_AlarmEvtChk(INT8U evtId)
{
    INT8U result=FALSE;
#ifdef THREE_PHASE
    switch(evtId)
    {
         case EA_OWEFEE:
	     if(ChkStatus(E_RELAY_ALARM) == TRUE)
            {
               result=TRUE;
            }
         break;
	  case EA_BATTLOSTVOLT:
	     if((ChkStatus(E_CLK_BAT_LOSS) == TRUE)||(ChkStatus(E_DIS_BAT_LOSS) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_ClOCKERR:
	     if(ChkStatus(E_CLK_ERR) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_EEPROMERR:	
	     if((ChkStatus(E_EEP_ERR) == TRUE)||(ChkStatus(E_FLASH_ERR) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_ESAMERR:
	     if(ChkStatus(E_ESAM_ERR) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_CTRCIRCLEERR:
	     if(ChkStatus(E_RELAY_ERR) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_COVEROPEN:    
	     if(ChkStatus(E_OPEN_COVER) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_TAILOPEN: 
	     if(ChkStatus(E_OPEN_TAIL) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_MAGNET:    
	     if(ChkStatus(E_MAGNET) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_OVERFACTOR:
	     if(ChkStatus(E_OVER_FACTOR) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_VOLTREVERSE:
	     if(ChkStatus(E_VOLT_REVERSE) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_CURRREVERSE:
	     if(ChkStatus(E_CURR_REVERSE) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_PWRREVERSE:
	     if((ChkStatus(E_PWR_REVERSE) == TRUE)||(ChkStatus(E_PWR_REVERSE_LA) == TRUE)
	     ||(ChkStatus(E_PWR_REVERSE_LB) == TRUE)||(ChkStatus(E_PWR_REVERSE_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_LOSTVOLTAGE: 
	     if((ChkStatus(E_LOSS_VOLT_LA) == TRUE)||(ChkStatus(E_LOSS_VOLT_LB) == TRUE)
	     ||(ChkStatus(E_LOSS_VOLT_LC) == TRUE)||(ChkStatus(E_LOSS_VOLT_ALL) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_BREAKPHASE: 
	     if((ChkStatus(E_BRK_PHASE_LA) == TRUE)||(ChkStatus(E_BRK_PHASE_LB) == TRUE)
	     ||(ChkStatus(E_BRK_PHASE_LC) == TRUE)||(ChkStatus(E_PWR_DOWN) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_OVERVOLTAGE: 
	     if((ChkStatus(E_OVER_VOLT_LA) == TRUE)||(ChkStatus(E_OVER_VOLT_LB) == TRUE)||(ChkStatus(E_OVER_VOLT_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_VOLTAGEDOWN: 
	     if((ChkStatus(E_UNDER_VOLT_LA) == TRUE)||(ChkStatus(E_UNDER_VOLT_LB) == TRUE)||(ChkStatus(E_UNDER_VOLT_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_VOLTIMBALANCE:
	     if(ChkStatus(E_VOLT_IMBAL) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_CURRIMBALANCE:
	     if(ChkStatus(E_CURR_IMBAL) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_CURRFARIMBALANCE:	      
	     if(ChkStatus(E_CURR_BADLY_IMBAL) == TRUE)
            {
               result=TRUE;
            }
         break;
         case EA_LOSTCURRENT:
	     if((ChkStatus(E_LOSS_CURR_LA) == TRUE)||(ChkStatus(E_LOSS_CURR_LB) == TRUE)||(ChkStatus(E_LOSS_CURR_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
         #ifdef OVER_CURR_CHK
         case EA_OVERCURRENT:
	     if((ChkStatus(E_OVER_CURR_LA) == TRUE)||(ChkStatus(E_OVER_CURR_LB) == TRUE)||(ChkStatus(E_OVER_CURR_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
         #endif
         case EA_FAILCURRENT:
	     if((ChkStatus(E_CUT_CURR_LA) == TRUE)||(ChkStatus(E_CUT_CURR_LB) == TRUE)||(ChkStatus(E_CUT_CURR_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
         case EA_UPLOAD:
	     if((ChkStatus(E_OVER_LOAD_LA) == TRUE)||(ChkStatus(E_OVER_LOAD_LB) == TRUE)||(ChkStatus(E_OVER_LOAD_LC) == TRUE))
            {
               result=TRUE;
            }
         break;
    }
#else
    
    switch(evtId)
    {
	  case EA_BATTLOSTVOLT:          
            if(ChkStatus(E_CLK_BAT_LOSS) == TRUE)            
            {
               result=TRUE;
            }
         break;
         
         case EA_PWRREVERSE:
	     if(ChkStatus(E_PWR_P_DIR) == TRUE)
            {
               result=TRUE;
            }
         break;

         case EA_GLXCOMM_ERR:
	     if(ChkStatus(E_MGMT_COMM_ERR) == TRUE)
            {
               result=TRUE;
            }
         break;
    }
#endif
    return result;
}

static void MNT_AlarmSecTask(void)
{
   INT8U id;
   INT8U evtId;
   INT32U d;
   INT8U modeId[EA_ALARM_SCR_MAX]={EA_LCD, EA_BGT, EA_LCD};
#ifdef THREE_PHASE
   for(id=0;id < EA_ALARM_SCR_MAX;id++)
   {
      for(evtId=0;evtId<EA_ALARM_FLAG_MAX;evtId++)
      {
          d=(0x00000001<<evtId);
		  
	   mntEvtLastFlag[id]=mntEvtNowFlag[id];
	   
	   if(MNT_AlarmEvtChk(evtId) == TRUE)
	   {
	      mntEvtNowFlag[id]=mntEvtNowFlag[id]|d;
	   }
	   else
	   {
	     mntEvtNowFlag[id]=mntEvtNowFlag[id]&(~d);
	   }
	   
	   if(((mntEvtLastFlag[id]&d) == 0)&&((mntEvtNowFlag[id]&d) != 0))
	   {
	      mntAlarmFlag[id]=mntAlarmFlag[id]|d;
	   }
	   else if(((mntEvtLastFlag[id]&d) != 0)&&((mntEvtNowFlag[id]&d) == 0))
	   {
	      mntAlarmFlag[id]=mntAlarmFlag[id]&(~d);
	   }
      }

      if((mntAlarmFlag[id]&MntPara.mntAlarmMode[modeId[id]])==0)
      {
         if(id == EA_LCD)
         {
            DIS_ClrFlashStauSeg(STA_ALARM);  /*"LCD"*/ 
         }
	  else if(id == EA_BGT)
	  {
	     OC_LedOff(E_ID_LED_BGD, E_LED_SC_EVENT);  /*"����"*/
	  }
	  else if(id == EA_RLY)
	  {
	     OC_RelayAct(E_RELAY_SC_EVENT, E_RELAY_RELAYALARM, E_RELAY_MODE_LEVEL, RELAY_ACT_CLOSE, 0);/*"�����̵���"*/
	  }
      }
      else
      {
         if(id == EA_LCD)
         {
            DIS_FlashStauSeg(STA_ALARM);  /*"LCD"*/ 
         }
	  else if(id == EA_BGT)
	  {
   	     OC_LedOnContin(E_ID_LED_BGD, E_LED_SC_EVENT);  /*"����"*/
	  }
	  else if(id == EA_RLY)
	  {
            if((ChkStatus(E_VOLT_RELAY_OK) == TRUE)||(TRUE == ChkStatus(E_FAC)))
            {
	        OC_RelayAct(E_RELAY_SC_EVENT, E_RELAY_RELAYALARM, E_RELAY_MODE_LEVEL, RELAY_ACT_TRIP, 0);/*"�����̵���"*/
            }
	     else
	     {
	        OC_RelayAct(E_RELAY_SC_EVENT, E_RELAY_RELAYALARM, E_RELAY_MODE_LEVEL, RELAY_ACT_CLOSE, 0);/*"�����̵���"*/
	     }
	  }
      }
   }
#else
   for(id=EA_BGT;id <= EA_BGT;id++)
   {
      for(evtId=0;evtId<EA_ALARM_FLAG_MAX;evtId++)
      {
          d=(0x00000001<<evtId);
		  
	   mntEvtLastFlag[id]=mntEvtNowFlag[id];
	   
	   if(MNT_AlarmEvtChk(evtId) == TRUE)
	   {
	      mntEvtNowFlag[id]=mntEvtNowFlag[id]|d;
	   }
	   else
	   {
	     mntEvtNowFlag[id]=mntEvtNowFlag[id]&(~d);
	   }
	   
	   if(((mntEvtLastFlag[id]&d) == 0)&&((mntEvtNowFlag[id]&d) != 0))
	   {
	      mntAlarmFlag[id]=mntAlarmFlag[id]|d;
	   }
	   else if(((mntEvtLastFlag[id]&d) != 0)&&((mntEvtNowFlag[id]&d) == 0))
	   {
	      mntAlarmFlag[id]=mntAlarmFlag[id]&(~d);
	   }
      }

      if((mntAlarmFlag[id]&MntPara.mntAlarmMode[modeId[id]])==0)
      {
	   OC_LedOff(E_ID_LED_BGD, E_LED_SC_EVENT);  /*"����"*/
      }
      else
      {
          if((TRUE == ChkStatus(E_VOLT_LCD_OK))||(TRUE == ChkStatus(E_FAC)))
          {
	      OC_LedOnContin(E_ID_LED_BGD, E_LED_SC_EVENT);  /*"����"*/
          }
	   else
	   {
	      OC_LedOff(E_ID_LED_BGD, E_LED_SC_EVENT);  /*"����"*/
	   }
      }
   }
#endif
}

INT8S MNT_AlarmKeyAction(void *pIn, void *pOut)
{
#ifdef THREE_PHASE
   mntAlarmFlag[EA_RLY]=0;
   OC_RelayAct(E_RELAY_SC_EVENT, E_RELAY_RELAYALARM, E_RELAY_MODE_LEVEL, RELAY_ACT_CLOSE, 0);/*"�����̵���"*/
#endif
   return TRUE;
}

INT8S MNT_PwrUpEvtInit(void *pIn, void *pOut)
{
   MNT_PwrUpSEInit();
   GE_Init();
   return TRUE;
}

static INT32U get_meter_Ib_Imax(INT8U *ptr)
{
    INT8U i, dot;
    INT32U Ib;

    Ib = 0;
    dot = 0;
    for(i=5; i>0; i--)
    {
        if(ptr[i] == 'A' || ptr[i] == 'a')
        {
            if(dot == 0)
            {
                Ib = Ib*10;
            }
            break;
        }
        if(ptr[i] == '.')
        {
            dot = 1;
            continue;
        }
        if(ptr[i] >= '0' && ptr[i] <= '9')
        {
            Ib = Ib*10 + (ptr[i] - '0');
            if(dot == 1)
            {/*" ֻ����1λС�� "*/
                break;
            }
        }
    }
    Ib = Ib*1000; /* 4λС��*/
    return Ib;
}

INT8S MNT_EvtParaCal(void *pIn, void *pOut)
{
   INT64U currMax;
   INT8U   iMaxAscii[6];
   
   GetSingle(E_MAX_CURR, (INT8U *)&iMaxAscii[0]);
   LIB_RvsSelf(&iMaxAscii[0],6);
   currMax=get_meter_Ib_Imax(&iMaxAscii[0]);
#ifdef THREE_PHASE
   INT8U i;   
   INT16U meter331;
   INT64U voltBase;
   INT64U currBase;
   INT64U pwrMax;
   INT64U pwrBase;
   INT64U tPwrMax;
   INT32U d;

   GetSingle(E_NOMINAL_VOLT, (INT8U *)&d);  /*"10 -4V"*/
   voltBase=d/1000;                                         /*"10 -1V"*/
   GetSingle(E_NOMINAL_CURR, (INT8U *)&d);  /*"10 -4A"*/
   currBase=d;
   pwrMax=(currMax*voltBase)/100;                                /*"10 -6 kW"*/
   pwrBase=(currBase*voltBase)/100;                             /*"10 -6 kW"*/ 
   tPwrMax=pwrMax*3;
   GetSingle(E_MU_STATUS, (INT8U *)&meter331);
   meter331=meter331&0x01;
   if(meter331)
   {
      tPwrMax=pwrMax*1732/1000;
      pwrMax=pwrMax*866/1000;
   }
   for(i=0;i<EA_ALARM_SCR_MAX;i++)
   {
      MntPara.mntAlarmMode[i]=((0x01<<EA_LOSTVOLTAGE)|(0x01<<EA_VOLTREVERSE)|(0x01<<EA_CURRREVERSE)|(0x01<<EA_UPLOAD)
                                                |(0x01<<EA_PWRREVERSE)|(0x01<<EA_BATTLOSTVOLT)|(0x01<<EA_OWEFEE));/*"����ģʽ��"*/
   }

    MntPara.LossCurrLimitIU=(currBase*5)/1000;  /*"ʧ���¼�������������"*/
    MntPara.LossCurrLimitIL=(currBase*5)/100;     /*"ʧ���¼�������������"*/
    MntPara.BrkPhaseLimitIU=(currBase*5)/1000; /*"�����¼�������������"*/
    MntPara.LossVoltLimitIL=(currBase*5)/1000; /*"ʧѹ�¼�������������"*/
    MntPara.FailCurrLimitIU=(currBase*5)/1000; /*"�����¼�������������"*/
#endif
    MntPara.OverCurrLimitIL=(currMax*12)/10; /*"�����¼�������������"*/
    MntPara.RlyTripCtrlCurr=300000; /*"�̵�����բ���Ƶ�������ֵ"*/
    MntPara.DisBatVoltThrL=481;  /*"�͹��ĵ��Ƿѹ��ֵ"*/
#ifdef THREE_PHASE
    MntPara.UpLoadLimitP=(pwrMax*12)/1000; /*"�����¼��й����ʴ�������"*/
    MntPara.PhasePwrReverseLimitP=(pwrBase*5)/100000; /*"���ʷ����¼��й����ʴ�������"*/
    if(MntPara.PhasePwrReverseLimitP == 0)
    {
       MntPara.PhasePwrReverseLimitP=1;
    }
    MntPara.AAOverDemandLimitP=(tPwrMax*12)/1000; /*"�����й����������¼�������������"*/
    MntPara.NAOverDemandLimitP=(tPwrMax*12)/1000; /*"�����й����������¼�������������"*/
    MntPara.OverDemandLimitQ=(tPwrMax*12)/1000; /*"�޹����������¼�������������"*/
    MntPara.VoltChkLimitU=(voltBase*12)/10; /*"��ѹ��������"*/
    MntPara.VoltChkLimitL=(voltBase*7)/10; /*"��ѹ��������"*/
    MntPara.VoltHealthLimitU=(voltBase*107)/100; /*"��ѹ�ϸ�����ֵ"*/
    MntPara.VoltHealthLimitL=(voltBase*93)/100; /*"��ѹ�ϸ�����ֵ"*/
    MntPara.OverVoltLimitVL=(voltBase*12)/10; /*"��ѹ�¼���ѹ��������"*/
    MntPara.UnderVoltLimitVU=(voltBase*78)/100; /*"Ƿѹ�¼���ѹ��������"*/
    MntPara.LossVoltLimitVU=(voltBase*78)/100; /*"ʧѹ�¼���ѹ��������"*/
    MntPara.LossVoltLimitVL=(voltBase*85)/100; /*"ʧѹ�¼���ѹ�ָ�����"*/
    MntPara.BrkPhaseLimitVU=(voltBase*60)/100; /*"�����¼���ѹ��������"*/
    MntPara.LossCurrLimitVL=(voltBase*70)/100; /*"ʧ���¼���ѹ��������"*/
    MntPara.CutCurrLimitVL=(voltBase*60)/100; /*"�����¼���ѹ��������"*/
    MntPara.ClkBatVoltThrL=261; /*"ʱ�ӵ��Ƿѹ��ֵ"*/
    MntPara.OverFactorThr=300; /*"�ܹ������������޷�ֵ"*/
    MntPara.CurrImbalLimitRatio=3000; /*"������ƽ������ֵ "*/
    MntPara.VoltImbalLimitRatio=3000; /*"��ѹ��ƽ������ֵ"*/
    MntPara.CurrBadlyImBalLimitRatio=9000; /*"�������ز�ƽ����ֵ"*/
#endif
    MntPara.CriticalVoltage=60; /*"�ٽ��ѹ"*/
#ifdef THREE_PHASE
    MntPara.LossVoltDelayT=60; /*"ʧѹ�¼��ж���ʱʱ��"*/
    MntPara.BrkPhaseDelayT=60; /*"�����¼��ж���ʱʱ�� "*/
    MntPara.LossCurrDelayT=60; /*"ʧ���¼��ж���ʱ��"*/
    MntPara.UnderVoltDelayT=60; /*"Ƿѹ�¼��ж���ʱʱ��"*/
    MntPara.OverVoltDelayT=60; /*"��ѹ�¼��ж���ʱʱ��"*/
    MntPara.VoltImbalDelayT=60; /*"��ѹ��ƽ�����ж���ʱʱ��"*/
    MntPara.CurrImbalDelayT=60; /*"������ƽ�����ж���ʱʱ��"*/
    MntPara.PwrReverseDelayT=60; /*"���ʷ����¼��ж���ʱʱ��"*/
    MntPara.OverLoadDelayT=60; /*"�����¼��ж���ʱʱ��"*/
    MntPara.OverFactorDelayT=60; /*"�ܹ��������������ж���ʱʱ��"*/
    MntPara.CurrBadlyImbalDelayT=60; /*"�������ز�ƽ�ⴥ����ʱʱ��"*/
    MntPara.CutCurrDelayT=60; /*"�����¼��ж���ʱʱ��"*/
    MntPara.AAOverDemandDelayT=60; /*"�����й����������¼��ж���ʱʱ��"*/
    MntPara.NAOverDemandDelayT=60; /*"�����й����������¼��ж���ʱʱ��"*/
    MntPara.ROverDemandDelayT=60;    /*"�޹����������¼��ж���ʱʱ��"*/
    MntPara.AuxPwrDownDelayT=60; /*"������Դ�����¼��ж�ʱ��,��ʱ����"*/
    MntPara.VoltReverseDelayT=60; /*"��ѹ�������¼��ж�ʱ��"*/
    MntPara.CurrReverseDelayT=60; /*"�����������¼��ж�ʱ��"*/
    MntPara.MagnetDelayT=60; /*"�㶨�ų������¼��ж�ʱ��"*/
#endif
    MntPara.ClkErrDelayT=5; /*"���ܱ�ʱ�ӹ����¼��ж�ʱ��"*/
    MntPara.MuErrDelayT=60; /*"���ܱ����оƬ�����¼��ж�ʱ��"*/
    MntPara.OverCurrDelayT=60; /*"�����¼��ж���ʱʱ��"*/
    MntPara.RelayErrChkDelayT=5; /*"���ɿ��������¼��ж�ʱ��"*/
    MntPara.PwrAbnormalDelayT=0; /*"��Դ�쳣�¼��ж�ʱ��"*/
    MntPara.PwrDownDelayT=60; /*"���ܱ�����¼��ж�ʱ��"*/
    MntPara.crc=CRC16((INT8U *)&MntPara, FIND(ST_MNT_PARA, crc));
    FS_WriteFile(E_FILE_MNT_PARA, 0, (INT8U *)&MntPara, sizeof(ST_MNT_PARA));
   return TRUE;
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_UpDataSet"*/
/*"  Description:   ����о�����ݴ��ݸ�����о"*/
/*"  Calls:          "*/
/*"  Called By:   "*/
/*"  Input:          pIn ������� ���ޣ�"*/
/*"  Output:         pOut ������ݣ�����о��������о�����ݣ�"*/
/*"  Return:         DAR ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_UpDataSet(void *pIn, void *pOut)
{
    LIB_MemCpy((INT8U *)&mMntUpData, (INT8U *)pOut, sizeof(MNT_UP_DATA));
    
    return MS_SUCCESS;
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_DownDataGet"*/
/*"  Description:   ����о�����ݴ��ݸ�����о"*/
/*"  Calls:          "*/
/*"  Called By:   "*/
/*"  Input:          pIn ������� ������о��������о�����ݣ�"*/
/*"  Output:         pOut ������ݣ��ޣ�"*/
/*"  Return:         DAR ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_DownDataGet(void *pIn, void *pOut)
{
    INT16U mCs=0;
    INT16U i;
    MNT_DOWN_DATA gDownData;

    LIB_MemCpy((INT8U *)pIn, (INT8U *)&gDownData, sizeof(MNT_DOWN_DATA));

    for(i=0; i< FIND(MNT_DOWN_DATA,Cs); i++)
    {
        mCs+= *((INT8U *)&gDownData+i);
    }
    if(mCs == gDownData.Cs)
    {
       LIB_MemCpy((INT8U *)&gDownData, (INT8U *)&mMntDownData, sizeof(MNT_DOWN_DATA));
    }
    
    return MS_SUCCESS;
}

/*"*****************************************************************************"*/
/*"  Function:       MNT_UpDataSecTask"*/
/*"  Description:   "*/
/*"  Calls:          "*/
/*"  Called By:   "*/
/*"  Input:          pIn ������� ���ޣ�"*/
/*"  Output:         pOut ������ݣ�����о��������о�����ݣ�"*/
/*"  Return:         DAR ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void MNT_UpDataSecTask(void)
{
    INT16U i;

    #ifdef TMNL_TMP_CHK
    LIB_MemCpy((INT8U *)&gTmnlTmp[0], (INT8U *)&mMntUpData.gTmnlTmp[0], E_TEMP_MAX*2UL);
    #endif

    #if(MNT_VERSION == STATE_GRID)
    GetSingle(E_AMP_ERR, (INT8U *)&mMntUpData.PwrAmpErr);

    GetSingle(E_PHS_ERR, (INT8U *)&mMntUpData.PwrPhsErr);
    #endif

    mMntUpData.Cs=0;
    for(i=0; i< FIND(MNT_UP_DATA,Cs); i++)
    {
        mMntUpData.Cs+= *((INT8U *)&mMntUpData+i);
    }
}

