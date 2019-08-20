#ifndef _REPORT_H_
#define _REPORT_H_

#include "system\Sys_def.h"


#if defined (SINGLE_PHASE)
#define  RPT_MAX_CNT    30/*"�ϱ��¼����������б�"*/
#else
#define  RPT_MAX_CNT    76/*"�ϱ��¼����������б�"*/
#endif

#if defined (SINGLE_PHASE)
#define  REPORTCOMNUM	3		/*"�ϱ�ͨ�� 0��485��1���ز�(��ڶ�·485)��2���⵫�Ǻ��ⲻ�����ϱ�"*/
#else
#define  REPORTCOMNUM   4		/*"���ӵڶ�·485  3"*/
#endif

#define  EVRP_ID_485  	0 /*"RS485"*/
#define  EVRP_ID_CM  	1 /*"ͨ��ģ��"*/
#define  EVRP_ID_IR  	2 /*"����"*/
#define  EVRP_ID_4852  	3 /*"�ڶ�·485"*/

#define  CLR_RPT  		(0x00)
#define  START_RPT  	(0x01)	/*"�����ϱ�"*/
#define  END_RPT        (0x02)	/*"�ָ��ϱ�"*/
#define  STARTOREND_RPT (0x03)	/*"�����ͻָ����ϱ�"*/

#define GET   	0
#define SET   	1


typedef union       /*"�����ϱ�״̬��"*/
{
     INT8U byte[4];          
    struct 
    {
/*"Byte0"*/
        INT8U bit0 : 1;    		  /*"����"*/
        INT8U EsamErr : 1;        /*"ESAM����"*/ 
        INT8U bit3 : 1;  		  /*"����"*/
        INT8U BatteryErr  : 1;    /*"ʱ�ӵ�ص�ѹ��"*/
        INT8U bit4 : 1;           /*"����"*/
        INT8U EepromErr : 1;      /*"�洢�����ϻ���"*/
        INT8U bit6 : 1;           /*"����"*/
        INT8U bit7 : 1;           /*"����"*/
/*"Byte1"*/
        INT8U LowBatErr : 1;      /*"ͣ�糭����Ƿѹ"*/
        INT8U Credit : 1;         /*"͸֧״̬"*/ 
        INT8U bit10 : 1;      	  /*"����"*/
        INT8U bit11 : 1;          /*"����"*/
        INT8U bit12 : 1;          /*"����"*/
        INT8U bit13 : 1;       	  /*"����"*/
        INT8U RelayRelease : 1;   /*"��բ�ɹ�"*/
        INT8U RelayHoldSuc : 1;   /*"��բ�ɹ�"*/
/*"Byte2"*/
        INT8U bit16 : 1;          /*"����"*/
        INT8U bit17 : 1;          /*"����"*/
        INT8U bit18 : 1;          /*"����"*/
        INT8U bit19 : 1;          /*"����"*/
        INT8U bit20 : 1;    	  /*"����"*/
        INT8U bit21 : 1;          /*"����"*/
        INT8U bit22 : 1;          /*"����"*/
        INT8U bit23 : 1;          /*"����"*/
/*"Byte3"*/
        INT8U bit24 : 1;          /*"����"*/
        INT8U bit25 : 1;          /*"����"*/
        INT8U bit26 : 1;          /*"����"*/
        INT8U bit27 : 1;          /*"����"*/
        INT8U bit28 : 1;          /*"����"*/
        INT8U bit29 : 1;          /*"����"*/
        INT8U bit30 : 1;          /*"����"*/
        INT8U bit31 : 1;          /*"����"*/	
     }bits;
}UN_EVTRPT_STATUS;


/*"�ϱ��¼���Ϣ"*/
typedef struct 
{
    INT8U eventMode;    /*"�ϱ��¼���mode�е�byteλ��"*/
    INT8U eventModeBit; /*"�ϱ��¼��ڸ�byte�е�bitλ��"*/
}ST_REPORTEVENTINFO;

/*"�ϱ��¼���Ϣ"*/
typedef struct 
{
    INT16U rcrd_obis;     /*"�¼�OI"*/
    INT8U eventMode;    /*"�ϱ��¼���mode�е�byteλ��"*/
    INT8U eventModeBit; /*"�ϱ��¼��ڸ�byte�е�bitλ��"*/
    INT8U eventStatue;  /*"�ϱ��¼���statue�е�byteλ��"*/
    INT8U eventStatueBit;   /*"�ϱ��¼��ڸ�statue�е�bitλ��"*/
}ST_645REPORTEVENTINFO;

typedef struct       /*"��������ϱ�����OI�б�"*/
{
	INT8U  Oi[RPT_MAX_CNT][2];  /*"�����ϱ�����OI�б��̶�30��"*/
    /*"�ϱ���ʶ0_���ϱ���1_�¼������ϱ���2_�¼��ָ��ϱ���3_�¼������ָ����ϱ�"*/
    INT8U rptCtrl[RPT_MAX_CNT];
}ST_REPRORT_OI;

typedef struct       /*"������������ϱ�����OAD�б�"*/
{
	INT8U  Oad[RPT_MAX_CNT][4];  /*"���������ϱ��¼��б��̶�30��"*/ 
}ST_REPRORT_OAD;


typedef struct       /*"���¼��ϱ��б�"*/
{
	UN_OI rptOI;  /*"�¼�OI"*/
    /*"�ϱ���ʶ0_���ϱ���1_�¼������ϱ���2_�¼��ָ��ϱ���3_�¼������ָ����ϱ�"*/
    INT8U rptCtrl;
}ST_EVTRPT_LIST;

typedef union       /*"����բ����ִ��״̬"*/
{
    INT8U byte;          
    struct 
    {
        INT8U rlyOff: 1;   /*"��բ���̵�������ʱ��1����⵽�ɹ���բ������"*/
        INT8U rlyOn: 1;    /*"��բ���̵�������ʱ��1����⵽�ɹ���բ������"*/
        INT8U rlyOffRpt: 1;  /*"�̵����ɹ���բ�ô�λ���ϱ�������"*/
        INT8U rlyOnRpt: 1;    /*"�̵����ɹ���բ�ô�λ���ϱ�������"*/
        INT8U saved: 4;    /*"����"*/
    }bits;
}UN_RLY_STATUS;

/*"�����ϱ�2015���״̬�洢"*/
typedef struct
{
    UN_EVTRPT_STATUS unNewRptStatus[REPORTCOMNUM];/*"��ǰ״̬"*/
    UN_EVTRPT_STATUS unOldRptStatus[REPORTCOMNUM];/*"��ȥ״̬"*/

	INT8U  rptFlg[REPORTCOMNUM]; /*"�������ϱ���־"*/
    INT8U  rptStartFlg[REPORTCOMNUM];/*"�����ϱ���ʼ��־"*/
	
	INT8U  OadNum[REPORTCOMNUM]; 	/*"�����¼������ϱ��������"*/
	INT8U  OiNum;  /*"�����ϱ��¼��б�"*/

	UN_RLY_STATUS rlyChkStatus;	/*"�����բ��բ״̬��"*/
}ST_EVTRPT_STATUS;


typedef struct
{  
	ST_EVTRPT_STATUS stEvtPrtStatus;
	ST_REPRORT_OAD stReportOad[REPORTCOMNUM];  /*" 0 485/ 1 �ز����������ϱ��¼��б��̶�30��"*/
	ST_REPRORT_OI  stReportOi;	 /*"�����ϱ�����OI�б��̶�30��������ͨ��"*/
}ST_EVTRPT_BLK;


/*"*****************************�����ϱ�״̬��645*******************************"*/

#ifdef REPORT645
#define MAX_645EVENT_CNT     18

typedef union       /*"�����ϱ�״̬��645"*/
{
    INT8U byte[12];          
    struct 
    {
  /*"Byte0"*/
        INT8U RelayMisAct : 1;    /*���ɿ�������*/
        INT8U EsamErr : 1;        /*"ESAM����"*/ 
        INT8U EepromInitErr : 1;  /*"�ڿ���ʼ������"*/
        INT8U BatteryErr  : 1;    /*"ʱ�ӵ�ص�ѹ��"*/
        INT8U bit4 : 1;           /*"����"*/
        INT8U EepromErr : 1;      /*"�洢�����ϻ���"*/
        INT8U bit6 : 1;           /*"����"*/
        INT8U RtcErr : 1;         /*"ʱ�ӹ���"*/
  /*"Byte1"*/
        INT8U bit8 : 1;           /*"ͣ�糭����Ƿѹ"*/
        INT8U Credit : 1;         /*"͸֧״̬"*/ 
        INT8U OpenState : 1;      /*"�����"*/
        INT8U bit11 : 1;          /*"����"*/
        INT8U bit12 : 1;          /*"����"*/
        INT8U PowerErr : 1;       /*"��Դ�쳣"*/
        INT8U RelayRelease : 1;   /*"��բ�ɹ�"*/
        INT8U RelayHoldSuc : 1;   /*"��բ�ɹ�"*/
  /*"Byte2"*/
        INT8U bit16 : 1;          /*"����"*/
        INT8U bit17 : 1;          /*"����"*/
        INT8U bit18 : 1;          /*"����"*/
        INT8U bit19 : 1;          /*"����"*/
        INT8U LoadCurrent : 1;    /*"A�����"*/ 
        INT8U bit21 : 1;          /*"����"*/
        INT8U bit22 : 1;          /*"����"*/
        INT8U bit23 : 1;          /*"����"*/
  /*"Byte3"*/
        INT8U bit24 : 1;          /*"����"*/
        INT8U bit25 : 1;          /*"����"*/
        INT8U bit26 : 1;          /*"����"*/
        INT8U bit27 : 1;          /*"����"*/
        INT8U bit28 : 1;          /*"����"*/
        INT8U bit29 : 1;          /*"����"*/
        INT8U bit30 : 1;          /*"����"*/
        INT8U bit31 : 1;          /*"����"*/	
  /*"Byte4"*/
        INT8U bit32 : 1;          /*"����"*/
        INT8U bit33 : 1;          /*"����"*/
        INT8U bit34 : 1;          /*"����"*/
        INT8U bit35 : 1;          /*"����"*/
        INT8U bit36 : 1;          /*"����"*/
        INT8U bit37 : 1;          /*"����"*/
        INT8U bit38 : 1;          /*"����"*/
        INT8U bit39 : 1;          /*"����"*/
  /*"Byte5"*/
        INT8U bit40 : 1;          /*"����"*/
        INT8U bit41 : 1;          /*"����"*/
        INT8U bit42 : 1;          /*"����"*/
        INT8U bit43 : 1;          /*"����"*/
        INT8U bit44 : 1;          /*"����"*/
        INT8U bit45 : 1;          /*"����"*/
        INT8U bit46 : 1;          /*"����"*/
        INT8U bit47 : 1;          /*"����"*/
   /*"Byte6"*/
        INT8U bit48 : 1;          /*"����"*/
        INT8U bit49 : 1;          /*"����"*/
        INT8U bit50 : 1;          /*"����"*/
        INT8U bit51 : 1;          /*"����"*/
        INT8U bit52 : 1;          /*"����"*/
        INT8U bit53 : 1;          /*"����"*/
        INT8U bit54 : 1;          /*"����"*/
        INT8U bit55 : 1;          /*"����"*/
   /*"Byte7"*/
        INT8U bit56 : 1;          /*"����"*/
        INT8U bit57 : 1;          /*"����"*/
        INT8U bit58 : 1;          /*"����"*/
        INT8U bit59 : 1;          /*"����"*/
        INT8U bit60 : 1;          /*"����"*/
        INT8U bit61 : 1;          /*"����"*/
        INT8U bit62 : 1;          /*"����"*/
        INT8U bit63 : 1;          /*"����"*/
   /*"Byte8"*/
        INT8U bit64 : 1;          /*"����"*/
        INT8U bit65 : 1;          /*"����"*/
        INT8U bit66 : 1;          /*"����"*/
        INT8U bit67 : 1;          /*"����"*/
        INT8U bit68 : 1;          /*"����"*/
        INT8U PowerOff : 1;       /*"����"*/
        INT8U bit70 : 1;          /*"����"*/
        INT8U bit71 : 1;          /*"����"*/
   /*"Byte9"*/
        INT8U bit72 : 1;          /*"����"*/
        INT8U bit73 : 1;          /*"����"*/
        INT8U bit74 : 1;          /*"����"*/
        INT8U bit75 : 1;          /*"����"*/
        INT8U bit76 : 1;          /*"����"*/
        INT8U bit77 : 1;          /*"����"*/
        INT8U bit78 : 1;          /*"����"*/
        INT8U bit79 : 1;          /*"����"*/
   /*"Byte10"*/
        INT8U ProgramEvent : 1;	  /*"���"*/
        INT8U MeterClr : 1;		  /*"�������"*/
        INT8U bit82 : 1;		  /*"����"*/
        INT8U EventClr : 1;		  /*"�¼�����"*/
        INT8U adiustTime : 1;	  /*"Уʱ"*/
        INT8U DayTabPrg : 1;	  /*"��ʱ�α���"*/
        INT8U YearTabPrg : 1;	  /*"ʱ������"*/
        INT8U WeakTabPrg : 1;	  /*"�����ձ��"*/
    /*"Byte11"*/
        INT8U bit88 : 1;		  /*"����"*/
        INT8U EnergyMode : 1;	  /*"�й���Ϸ�ʽ���"*/
        INT8U bit90 : 1;		  /*"����"*/
        INT8U bit91 : 1;		  /*"����"*/
        INT8U BillSetPrg : 1; 	  /*"�����ձ��"*/
        INT8U TriffSetPrg : 1;	  /*"���ʲ�������"*/
        INT8U LevelSetPrg : 1;	  /*"���ݱ���"*/
        INT8U updateKey : 1;	  /*"��Կ����"*/
    }bits;
    
}UN_645EVTRPT_STATUS;

typedef union/*"�����ϱ�ģʽ��"*/
{
    INT8U byte[8];
    struct
    {
        INT8U RelayMisAct : 1;   /*"���ɿ�������"*/
        INT8U EsamErr : 1;       /*"ESAM����"*/ 
        INT8U EepromInitErr : 1; /*"�ڿ���ʼ������"*/
        INT8U BatteryErr  : 1;   /*"ʱ�ӵ�ص�ѹ��"*/
        INT8U bit4 : 1;          /*"����"*/
        INT8U EepromErr : 1;     /*"�洢�����ϻ���"*/
        INT8U bit6 : 1;          /*"����"*/
        INT8U RtcErr : 1;        /*"ʱ�ӹ���"*/

        INT8U bit8 : 1;          /*"����"*/
        INT8U Cregit : 1;        /*"͸֧״̬"*/ 
        INT8U OpenState : 1;     /*"�����"*/
        INT8U bit11 : 1;         /*"����"*/
        INT8U bit12 : 1;         /*"����"*/
        INT8U PowerErr : 1;      /*"��Դ�쳣"*/
        INT8U RelayRelease : 1;  /*"��բ�ɹ�"*/
        INT8U RelayHoldSuc : 1;  /*"��բ�ɹ�"*/

        INT8U bit16 : 1;         /*"����"*/
        INT8U bit17 : 1;         /*"����"*/
        INT8U bit18 : 1;         /*"����"*/
        INT8U bit19 : 1;         /*"����"*/
        INT8U LoadCurrent : 1;   /*"����"*/
        INT8U bit21 : 1;         /*"����"*/
        INT8U bit22 : 1;         /*"����"*/
        INT8U bit23 : 1;         /*"����"*/

        INT8U bit24 : 1;         /*"����"*/
        INT8U bit25 : 1;         /*"����"*/
        INT8U bit26 : 1;         /*"����"*/
        INT8U bit27 : 1;         /*"����"*/
        INT8U bit28 : 1;         /*"����"*/
        INT8U bit29 : 1;         /*"����"*/
        INT8U bit30 : 1;         /*"����"*/
        INT8U bit31 : 1;         /*"����"*/	

        INT8U bit32 : 1;         /*"����"*/
        INT8U bit33 : 1;         /*"����"*/
        INT8U bit34 : 1;         /*"����"*/
        INT8U bit35 : 1;         /*"����"*/
        INT8U bit36 : 1;         /*"����"*/
        INT8U PowerOff : 1;      /*"����"*/
        INT8U bit38 : 1;         /*"����"*/
        INT8U bit39 : 1;         /*"����"*/

        INT8U bit40 : 1;         /*"����"*/
        INT8U bit41 : 1;         /*"����"*/
        INT8U bit42 : 1;         /*"����"*/
        INT8U bit43 : 1;         /*"����"*/
        INT8U bit44 : 1;         /*"����"*/
        INT8U bit45 : 1;         /*"����"*/
        INT8U bit46 : 1;         /*"����"*/
        INT8U bit47 : 1;         /*"����"*/
	
        INT8U ProgramEvent : 1;	 /*"���"*/
        INT8U MeterClr : 1;		 /*"�������"*/
        INT8U bit50 : 1;		 /*"����"*/
        INT8U EventClr : 1;		 /*"�¼�����"*/
        INT8U adiustTime : 1;	 /*"Уʱ"*/
        INT8U DayTabPrg : 1; 	 /*"ʱ�α���"*/
        INT8U YearTabPrg : 1;	 /*"ʱ������"*/
        INT8U WeakTabPrg : 1;	 /*"�����ձ��"*/

        INT8U bit56 : 1;		 /*"����"*/
        INT8U EnergyMode : 1;	 /*"�й���Ϸ�ʽ���"*/
        INT8U bit58 : 1;		 /*"����"*/
        INT8U bit59 : 1;		 /*"����"*/
        INT8U BillSetPrg : 1;	 /*"�����ձ��"*/
        INT8U TriffSetPrg : 1;	 /*"���ʲ�������"*/
        INT8U LevelSetPrg : 1;	 /*"���ݱ���"*/
        INT8U updateKey : 1; 	 /*"��Կ����"*/
     }bits;
    
}UN_645EVTRPT_MODE;


typedef struct
{
    UN_645EVTRPT_STATUS un645NewRptStatus;/*"��ǰ״̬"*/
    UN_645EVTRPT_STATUS un645OldRptStatus;/*"��ȥ״̬"*/
    UN_645EVTRPT_STATUS ClrReportStatus;/*"��Ҫ����ı�־λ"*/  
    INT16U rptTimer;/*"�����ϱ��ȴ�ʣ��ʱ�� ��Ϊ��λ"*/
    INT8U  rptFlg; /*"�������ϱ���־"*/
    INT8U  rptStartFlg;/*"�����ϱ���ʼ��־"*/
    INT8U  rptEvtCnt[MAX_645EVENT_CNT];   /*"�����ϱ��������¼�����"*/ 
}ST_645EVTRPT_BLK;
#endif

extern ST_EVTRPT_STATUS mEvtPrtStatus;/*"�¼������ϱ�����"*/
extern ST_645EVTRPT_BLK m645EvtPrtStatus;

extern void Report_Clr(INT8U clrtype);
extern void Report_1min(void);
extern void Report_Init(void);
extern void Report_SetEventEnd(INT8U *ReportPr,INT8U chanl);
extern INT8U Report_GetStatus(INT8U *pdout);
extern void Get_ReportFlag(INT8U *pOi,INT8U *pdout);
extern INT8U Set_ReportFlag(INT8U *pOi,INT8U *pdout);
extern INT8U Report_RstStatus(INT8U *pdata);
extern INT8S PP_EvtReport(void *pdin, void *pdout);
extern INT8S UPdate_RlySucc(void *pdin, void *pdout);
extern void GetReportStatus_Fun(void);
extern INT8U Get_SetReport_Chanl(INT8U *pdata,INT8U Mode);
extern INT8U Report_Channal(void);
extern INT8U Check_RptChanl_Enable(void);
extern INT16U Get_EvtReportOAD(INT16U RprOBIS);


#ifdef REPORT645
//extern INT8U PrStateST;/*"����¼��ϱ���ʼ״̬"*/
extern INT8U SI_645Report(INT8U *pdata);
extern INT8U SI_645RstReport(INT8U *pdata);
extern void Report645Status_Get(void);
extern void Report_1Sec(void);
extern INT8U SI_645RstReport(INT8U *pdata);
#endif

#endif

