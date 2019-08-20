#ifndef UI_H
#define UI_H

#include "system\Sys_def.h"
#include "App\UserInterface\display\dm.h"
#include "App\UserInterface\ProtocolStack\ProtocolStack.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
#include "App\UserInterface\EventReport\Report.h"
#include "Driver\DriverComm.h"
#include "Lib\LIB_func.h"
#include "Driver\HAL\FS\FS_EepConfig.h"
#include "App\UserInterface\ProtocolStack\PS_698\AL_698.h"
#include "App\userinterface\PP\PP_698\Process\Pp_process.h"
#include "App\MtrParamDefault\MtrParamDef.h"
#include "App\UserInterface\Security\tca.h"

/*-------------------------------------------------------------------------------------*/

#define MAX_GPRS_STATUS_DISP_TIME  120   /*GPRS״̬��ʾʱ��*/


typedef struct
{
    INT8U name;/*"�̵���������"*/
    INT8U status;/*"��ǰ״̬"*/
    INT8U prop;/*"��������"*/
    INT8U lineSta;/*"����״̬"*/
}ST_RLY_STA;


/*���ܱ�����״̬��1~7����*/


typedef union       /*"�������״̬��1"*/
{
    INT16U word;          
    struct
    {
    	INT8U :1; 
    	INT8U :1;     
    	INT8U clockBat:1; /*"ʱ�ӵ��            0 ����  1Ƿѹ"*/
    	INT8U lowPwrBat:1;  /*"ͣ�糭����            0 ����  1Ƿѹ"*/
    	INT8U actPwrDir:1;/*"�й����ʷ���  0 ����  1����"*/
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;

    	INT8U ctrCircleErr:1; /*"���ƻ�·��"*/
    	INT8U EsamErr:1; /*"ESAM��"*/
    	INT8U :1; 
    	INT8U :1;
    	INT8U codeErr:1; /*"�ڲ��������"*/
    	INT8U RomErr:1; /*"�洢������"*/
    	INT8U Overdraft:1; /*"͸֧״̬"*/
    	INT8U ClockErr:1; /*"ʱ�ӹ���"*/
    }bits;
}UN_RUNWORD1_FORM;


typedef union     /*"�������״̬��2"*/
{	
    INT16U  word;                                          
    struct
    {
    	INT8U actPwrDirA:1; /*"A���й����ʷ��� 0 ����  1 ����"*/
    	INT8U :1;     
    	INT8U :1; 
    	INT8U :1; 
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;

    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    }bits;
}UN_RUNWORD2_FORM ; 


typedef union       /*"�������״̬��3"*/
{
    INT16U word;          
	struct
	{
		INT8U dailySlotSet :1;/*"��ǰ����ʱ�� 0 ��һ�� 1 �ڶ���"*/
		INT8U pwrModeL:1;        
		INT8U pwrModeH:1;       /*"���緽ʽ(00����Դ��01������Դ��10��ع���)"*/
		INT8U programable:1;       /*"Լ����֤/���״̬"*/
		INT8U rlySta:1;        /*"�̵���״̬ 0 ͨ  1 ��"*/
		INT8U yearSlotSet :1;     /*"��ǰ����ʱ�� 0 ��һ�� 1 �ڶ���"*/
		INT8U rlyCmdSta:1; /*"�̵�������״̬ 0 ͨ 1��"*/
		INT8U rlyPreTripAlarm:1;   /*"Ԥ��բ����״̬ 0 �� 1��"*/

		INT8U mtrType:2;         /*"�������  00 ��Ԥ���ѱ� 01������Ԥ���ѱ�,10 �����Ԥ���ѱ�"*/
		INT8U tariff:1;    /*"��ǰ���з��ʵ�� 0 ��һ�� 1 �ڶ���"*/
		INT8U step:1;     /*"��ǰ����    0 ��һ�� 1 �ڶ���"*/
		INT8U keepPwrSta:1; /*" ����״̬ "*/
		INT8U identityFlg:1; /*"�����֤��־"*/
		INT8U localOpenAcnt:1;  /*" ���ؿ��� "*/
		INT8U farOpenAcnt:1;/*" Զ�̿��� "*/   
	}bits;
 }UN_RUNWORD3_FORM;

typedef union     /*"�������״̬��4"*/
{	
    INT16U  word;                                          
    struct
    {
    	INT8U :1; 
    	INT8U :1;     
    	INT8U :1; 
    	INT8U :1; 
    	INT8U overCurrA:1;/*"A�����0 ����  1 ����"*/
    	INT8U :1;
    	INT8U dirA:1;/*"A�෽�� 0 ����  1 ����"*/
    	INT8U :1;

    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;
    }bits;
}UN_RUNWORD4_FORM ; 

typedef union       /*"�������״̬��7"*/
{
    INT16U word;          
	struct
	{
            INT8U voltReverse:1; /*"��ѹ������"*/
            INT8U currReverse:1; /*"����������"*/
            INT8U voltImbal:1; /*"��ѹ��ƽ��"*/
            INT8U currImbal:1; /*"������ƽ��"*/
            INT8U auxiliaryPwr:1; /*"������Դʧ��"*/
            INT8U pwrDown:1; /*"����"*/
            INT8U overDemand:1; /*"��������"*/       
            INT8U overFactor:1; /*"�ܹ�������������"*/

            INT8U CurrFarImbal:1; /*"�������ز�ƽ��"*/
            INT8U openCover:1; /*"�����"*/
            INT8U openBox:1; /*"����ť��"*/
            INT8U :1;
            INT8U :1;
            INT8U :1;
            INT8U :1;
            INT8U :1;	   
	}bits;
 }UN_RUNWORD7_FORM;


typedef struct       /*"�������״̬��"*/
{
    UN_RUNWORD1_FORM  word1;
    UN_RUNWORD2_FORM  word2;
    UN_RUNWORD3_FORM  word3;
    UN_RUNWORD4_FORM  word4;
    UN_WORD_DEF word5;
    UN_WORD_DEF word6;
    UN_RUNWORD7_FORM  word7;
}ST_RUN_STATUS;


extern ST_RUN_STATUS   RunStatus;/*���ܱ�����״̬��*/
extern UN_WORD_DEF    MeterSampleStatus;     /*����оϵͳ״̬��*/
extern UN_WORD_DEF    SampleStatus;          /*����״̬��*/

/******************ͨ�Ų����ļ����ֽ���*******************/







/******************ͨ�������ļ�����*******************/



/******************ͨ�������ļ����ֽ���*******************/



/*UIģ�������ļ�*/
typedef struct
{
    #ifdef REPORT645
    ST_645EVTRPT_BLK Event645_Rpt;          /*645Э�������ϱ��ļ�'*/
    #endif
    INT32U LastBroadcastAdjTime;  /*��һ�ι㲥Уʱʱ��*/
    INT32U ClockBatWorkTimeMin;/*ʱ�ӵ�ع���ʱ��(��)*/
    TCA_AuthSaveData TcaSaveData;	/*˫о��֤��������*/
}ST_UI_FILE_DATA;



extern INT8U UI_ShowFlash(void *pdin, void *pdout);
extern INT8S UI_GetRelayOutput(void *pdin, void *pdout);


extern void UI_Init(INT8U mode);
extern void UI_LoadDefaultUiPara(void);



/*RAM��ȫ�ֱ���*/
extern ST_UI_PARA_WITH_BACKUP StUIParaWithRamBackup;
#ifdef THREE_PHASE
extern INT16S SignalStrength;
extern INT8U  GPRS_Status_Time;
extern INT8U  GPRS_Status;
extern INT8U Change_GPRS_Value_Status(INT16S Value);
#endif

/*"��ʾ�����ļ�ƫ��"*/
#define  DM_PARA_SHIFT     
#define  DISP_PARA_SHIFT     (FIND(ST_UI_PARA_WITH_BACKUP, StDmParaWithRamBackup) + FIND(ST_DM_PARA_WITH_RAM_BACKUP, DispPara))
#define  CYC_OBJ_LIST_SHIFT  (FIND(ST_UI_PARA_NO_BACKUP, StDmParaNoRamBackup) + FIND(ST_DM_PARA_NO_RAM_BACKUP, CycObjList))
#define  KEY_OBJ_LIST_SHIFT  (FIND(ST_UI_PARA_NO_BACKUP, StDmParaNoRamBackup) + FIND(ST_DM_PARA_NO_RAM_BACKUP, KeyObjList))


/*ͨ�Ų����ļ�ƫ��*/
#define CM_PARA_WITH_RAM_BACKUP_SHIFT FIND(ST_UI_PARA_WITH_BACKUP, StCmParaWithRamBackup)
#define CM_PARA_NO_RAM_BACKUP_SHIFT FIND(ST_UI_PARA_NO_BACKUP, StCmParaNoRamBackup)

/*��RAM��������ƫ��*/
#define CM_PARA_WITH_RAM_BACKUP_1HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara1Hex))
#define CM_PARA_WITH_RAM_BACKUP_4HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara4Hex))
#define CM_PARA_WITH_RAM_BACKUP_6HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara6Hex))
#define CM_PARA_WITH_RAM_BACKUP_8HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara8Hex))

/*��RAM��������ƫ��*/
#define CM_PARA_NO_RAM_BACKUP_1HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData1Hex) )
#define CM_PARA_NO_RAM_BACKUP_2HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData2Hex) )
#define CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData4Hex) )
#define CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData6Hex) )
#define CM_PARA_NO_RAM_BACKUP_7HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData7Hex) )
#define CM_PARA_NO_RAM_BACKUP_8HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData8Hex) )
#define CM_PARA_NO_RAM_BACKUP_16HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData16Hex) )
#define CM_PARA_NO_RAM_BACKUP_32HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData32Hex) )

/*"�¼������ϱ��ļ�ƫ��"*/
#ifdef REPORT645
#define EVRP645_BAK_SHIFT           FIND(ST_UI_FILE_DATA, Event645_Rpt)
#define EVRP645_STATUSFLG_SHIFT     EVRP645_BAK_SHIFT+FIND(ST_645EVTRPT_BLK, rptTimer)
#define EVRP645_EVENTCNT_SHIFT      EVRP645_BAK_SHIFT+FIND(ST_645EVTRPT_BLK, rptEvtCnt[0])
#endif

extern void UI_Task(void);
extern INT8S UI_DataClr(void *pdin, void *pdout);
extern INT8U UI_GetRunStatues(INT8U *pdata);
extern INT8U UI_ReadRlyStatus(INT8U *pdata);

#endif

