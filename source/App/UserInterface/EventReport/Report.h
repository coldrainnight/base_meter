#ifndef _REPORT_H_
#define _REPORT_H_

#include "system\Sys_def.h"


#if defined (SINGLE_PHASE)
#define  RPT_MAX_CNT    30/*"上报事件新增对象列表"*/
#else
#define  RPT_MAX_CNT    76/*"上报事件新增对象列表"*/
#endif

#if defined (SINGLE_PHASE)
#define  REPORTCOMNUM	3		/*"上报通道 0，485，1，载波(或第二路485)，2红外但是红外不跟随上报"*/
#else
#define  REPORTCOMNUM   4		/*"增加第二路485  3"*/
#endif

#define  EVRP_ID_485  	0 /*"RS485"*/
#define  EVRP_ID_CM  	1 /*"通信模块"*/
#define  EVRP_ID_IR  	2 /*"红外"*/
#define  EVRP_ID_4852  	3 /*"第二路485"*/

#define  CLR_RPT  		(0x00)
#define  START_RPT  	(0x01)	/*"发生上报"*/
#define  END_RPT        (0x02)	/*"恢复上报"*/
#define  STARTOREND_RPT (0x03)	/*"发生和恢复都上报"*/

#define GET   	0
#define SET   	1


typedef union       /*"主动上报状态字"*/
{
     INT8U byte[4];          
    struct 
    {
/*"Byte0"*/
        INT8U bit0 : 1;    		  /*"保留"*/
        INT8U EsamErr : 1;        /*"ESAM错误"*/ 
        INT8U bit3 : 1;  		  /*"保留"*/
        INT8U BatteryErr  : 1;    /*"时钟电池电压低"*/
        INT8U bit4 : 1;           /*"保留"*/
        INT8U EepromErr : 1;      /*"存储器故障或损坏"*/
        INT8U bit6 : 1;           /*"保留"*/
        INT8U bit7 : 1;           /*"保留"*/
/*"Byte1"*/
        INT8U LowBatErr : 1;      /*"停电抄表电池欠压"*/
        INT8U Credit : 1;         /*"透支状态"*/ 
        INT8U bit10 : 1;      	  /*"保留"*/
        INT8U bit11 : 1;          /*"保留"*/
        INT8U bit12 : 1;          /*"保留"*/
        INT8U bit13 : 1;       	  /*"保留"*/
        INT8U RelayRelease : 1;   /*"跳闸成功"*/
        INT8U RelayHoldSuc : 1;   /*"合闸成功"*/
/*"Byte2"*/
        INT8U bit16 : 1;          /*"保留"*/
        INT8U bit17 : 1;          /*"保留"*/
        INT8U bit18 : 1;          /*"保留"*/
        INT8U bit19 : 1;          /*"保留"*/
        INT8U bit20 : 1;    	  /*"保留"*/
        INT8U bit21 : 1;          /*"保留"*/
        INT8U bit22 : 1;          /*"保留"*/
        INT8U bit23 : 1;          /*"保留"*/
/*"Byte3"*/
        INT8U bit24 : 1;          /*"保留"*/
        INT8U bit25 : 1;          /*"保留"*/
        INT8U bit26 : 1;          /*"保留"*/
        INT8U bit27 : 1;          /*"保留"*/
        INT8U bit28 : 1;          /*"保留"*/
        INT8U bit29 : 1;          /*"保留"*/
        INT8U bit30 : 1;          /*"保留"*/
        INT8U bit31 : 1;          /*"保留"*/	
     }bits;
}UN_EVTRPT_STATUS;


/*"上报事件信息"*/
typedef struct 
{
    INT8U eventMode;    /*"上报事件在mode中的byte位置"*/
    INT8U eventModeBit; /*"上报事件在该byte中的bit位置"*/
}ST_REPORTEVENTINFO;

/*"上报事件信息"*/
typedef struct 
{
    INT16U rcrd_obis;     /*"事件OI"*/
    INT8U eventMode;    /*"上报事件在mode中的byte位置"*/
    INT8U eventModeBit; /*"上报事件在该byte中的bit位置"*/
    INT8U eventStatue;  /*"上报事件在statue中的byte位置"*/
    INT8U eventStatueBit;   /*"上报事件在该statue中的bit位置"*/
}ST_645REPORTEVENTINFO;

typedef struct       /*"电表主动上报对象OI列表"*/
{
	INT8U  Oi[RPT_MAX_CNT][2];  /*"主动上报对象OI列表，固定30个"*/
    /*"上报标识0_不上报，1_事件发生上报，2_事件恢复上报，3_事件发生恢复均上报"*/
    INT8U rptCtrl[RPT_MAX_CNT];
}ST_REPRORT_OI;

typedef struct       /*"电表新增主动上报对象OAD列表"*/
{
	INT8U  Oad[RPT_MAX_CNT][4];  /*"新增主动上报事件列表，固定30个"*/ 
}ST_REPRORT_OAD;


typedef struct       /*"各事件上报列表"*/
{
	UN_OI rptOI;  /*"事件OI"*/
    /*"上报标识0_不上报，1_事件发生上报，2_事件恢复上报，3_事件发生恢复均上报"*/
    INT8U rptCtrl;
}ST_EVTRPT_LIST;

typedef union       /*"跳合闸命令执行状态"*/
{
    INT8U byte;          
    struct 
    {
        INT8U rlyOff: 1;   /*"跳闸，继电器动作时置1，检测到成功跳闸后清零"*/
        INT8U rlyOn: 1;    /*"合闸，继电器动作时置1，检测到成功合闸后清零"*/
        INT8U rlyOffRpt: 1;  /*"继电器成功跳闸置此位，上报后清零"*/
        INT8U rlyOnRpt: 1;    /*"继电器成功合闸置此位，上报后清零"*/
        INT8U saved: 4;    /*"保留"*/
    }bits;
}UN_RLY_STATUS;

/*"主动上报2015相关状态存储"*/
typedef struct
{
    UN_EVTRPT_STATUS unNewRptStatus[REPORTCOMNUM];/*"当前状态"*/
    UN_EVTRPT_STATUS unOldRptStatus[REPORTCOMNUM];/*"过去状态"*/

	INT8U  rptFlg[REPORTCOMNUM]; /*"需主动上报标志"*/
    INT8U  rptStartFlg[REPORTCOMNUM];/*"主动上报开始标志"*/
	
	INT8U  OadNum[REPORTCOMNUM]; 	/*"新增事件主动上报对象个数"*/
	INT8U  OiNum;  /*"主动上报事件列表"*/

	UN_RLY_STATUS rlyChkStatus;	/*"检测跳闸合闸状态字"*/
}ST_EVTRPT_STATUS;


typedef struct
{  
	ST_EVTRPT_STATUS stEvtPrtStatus;
	ST_REPRORT_OAD stReportOad[REPORTCOMNUM];  /*" 0 485/ 1 载波新增主动上报事件列表，固定30个"*/
	ST_REPRORT_OI  stReportOi;	 /*"主动上报对象OI列表，固定30个不区分通道"*/
}ST_EVTRPT_BLK;


/*"*****************************主动上报状态字645*******************************"*/

#ifdef REPORT645
#define MAX_645EVENT_CNT     18

typedef union       /*"主动上报状态字645"*/
{
    INT8U byte[12];          
    struct 
    {
  /*"Byte0"*/
        INT8U RelayMisAct : 1;    /*负荷开关误动作*/
        INT8U EsamErr : 1;        /*"ESAM错误"*/ 
        INT8U EepromInitErr : 1;  /*"内卡初始化错误"*/
        INT8U BatteryErr  : 1;    /*"时钟电池电压低"*/
        INT8U bit4 : 1;           /*"保留"*/
        INT8U EepromErr : 1;      /*"存储器故障或损坏"*/
        INT8U bit6 : 1;           /*"保留"*/
        INT8U RtcErr : 1;         /*"时钟故障"*/
  /*"Byte1"*/
        INT8U bit8 : 1;           /*"停电抄表电池欠压"*/
        INT8U Credit : 1;         /*"透支状态"*/ 
        INT8U OpenState : 1;      /*"开表盖"*/
        INT8U bit11 : 1;          /*"保留"*/
        INT8U bit12 : 1;          /*"保留"*/
        INT8U PowerErr : 1;       /*"电源异常"*/
        INT8U RelayRelease : 1;   /*"跳闸成功"*/
        INT8U RelayHoldSuc : 1;   /*"合闸成功"*/
  /*"Byte2"*/
        INT8U bit16 : 1;          /*"保留"*/
        INT8U bit17 : 1;          /*"保留"*/
        INT8U bit18 : 1;          /*"保留"*/
        INT8U bit19 : 1;          /*"保留"*/
        INT8U LoadCurrent : 1;    /*"A相过流"*/ 
        INT8U bit21 : 1;          /*"保留"*/
        INT8U bit22 : 1;          /*"保留"*/
        INT8U bit23 : 1;          /*"保留"*/
  /*"Byte3"*/
        INT8U bit24 : 1;          /*"保留"*/
        INT8U bit25 : 1;          /*"保留"*/
        INT8U bit26 : 1;          /*"保留"*/
        INT8U bit27 : 1;          /*"保留"*/
        INT8U bit28 : 1;          /*"保留"*/
        INT8U bit29 : 1;          /*"保留"*/
        INT8U bit30 : 1;          /*"保留"*/
        INT8U bit31 : 1;          /*"保留"*/	
  /*"Byte4"*/
        INT8U bit32 : 1;          /*"保留"*/
        INT8U bit33 : 1;          /*"保留"*/
        INT8U bit34 : 1;          /*"保留"*/
        INT8U bit35 : 1;          /*"保留"*/
        INT8U bit36 : 1;          /*"保留"*/
        INT8U bit37 : 1;          /*"保留"*/
        INT8U bit38 : 1;          /*"保留"*/
        INT8U bit39 : 1;          /*"保留"*/
  /*"Byte5"*/
        INT8U bit40 : 1;          /*"保留"*/
        INT8U bit41 : 1;          /*"保留"*/
        INT8U bit42 : 1;          /*"保留"*/
        INT8U bit43 : 1;          /*"保留"*/
        INT8U bit44 : 1;          /*"保留"*/
        INT8U bit45 : 1;          /*"保留"*/
        INT8U bit46 : 1;          /*"保留"*/
        INT8U bit47 : 1;          /*"保留"*/
   /*"Byte6"*/
        INT8U bit48 : 1;          /*"保留"*/
        INT8U bit49 : 1;          /*"保留"*/
        INT8U bit50 : 1;          /*"保留"*/
        INT8U bit51 : 1;          /*"保留"*/
        INT8U bit52 : 1;          /*"保留"*/
        INT8U bit53 : 1;          /*"保留"*/
        INT8U bit54 : 1;          /*"保留"*/
        INT8U bit55 : 1;          /*"保留"*/
   /*"Byte7"*/
        INT8U bit56 : 1;          /*"保留"*/
        INT8U bit57 : 1;          /*"保留"*/
        INT8U bit58 : 1;          /*"保留"*/
        INT8U bit59 : 1;          /*"保留"*/
        INT8U bit60 : 1;          /*"保留"*/
        INT8U bit61 : 1;          /*"保留"*/
        INT8U bit62 : 1;          /*"保留"*/
        INT8U bit63 : 1;          /*"保留"*/
   /*"Byte8"*/
        INT8U bit64 : 1;          /*"保留"*/
        INT8U bit65 : 1;          /*"保留"*/
        INT8U bit66 : 1;          /*"保留"*/
        INT8U bit67 : 1;          /*"保留"*/
        INT8U bit68 : 1;          /*"保留"*/
        INT8U PowerOff : 1;       /*"掉电"*/
        INT8U bit70 : 1;          /*"保留"*/
        INT8U bit71 : 1;          /*"保留"*/
   /*"Byte9"*/
        INT8U bit72 : 1;          /*"保留"*/
        INT8U bit73 : 1;          /*"保留"*/
        INT8U bit74 : 1;          /*"保留"*/
        INT8U bit75 : 1;          /*"保留"*/
        INT8U bit76 : 1;          /*"保留"*/
        INT8U bit77 : 1;          /*"保留"*/
        INT8U bit78 : 1;          /*"保留"*/
        INT8U bit79 : 1;          /*"保留"*/
   /*"Byte10"*/
        INT8U ProgramEvent : 1;	  /*"编程"*/
        INT8U MeterClr : 1;		  /*"电表清零"*/
        INT8U bit82 : 1;		  /*"保留"*/
        INT8U EventClr : 1;		  /*"事件清零"*/
        INT8U adiustTime : 1;	  /*"校时"*/
        INT8U DayTabPrg : 1;	  /*"日时段表编程"*/
        INT8U YearTabPrg : 1;	  /*"时区表编程"*/
        INT8U WeakTabPrg : 1;	  /*"周休日编程"*/
    /*"Byte11"*/
        INT8U bit88 : 1;		  /*"保留"*/
        INT8U EnergyMode : 1;	  /*"有功组合方式编程"*/
        INT8U bit90 : 1;		  /*"保留"*/
        INT8U bit91 : 1;		  /*"保留"*/
        INT8U BillSetPrg : 1; 	  /*"结算日编程"*/
        INT8U TriffSetPrg : 1;	  /*"费率参数表编程"*/
        INT8U LevelSetPrg : 1;	  /*"阶梯表编程"*/
        INT8U updateKey : 1;	  /*"密钥更新"*/
    }bits;
    
}UN_645EVTRPT_STATUS;

typedef union/*"主动上报模式字"*/
{
    INT8U byte[8];
    struct
    {
        INT8U RelayMisAct : 1;   /*"负荷开关误动作"*/
        INT8U EsamErr : 1;       /*"ESAM错误"*/ 
        INT8U EepromInitErr : 1; /*"内卡初始化错误"*/
        INT8U BatteryErr  : 1;   /*"时钟电池电压低"*/
        INT8U bit4 : 1;          /*"保留"*/
        INT8U EepromErr : 1;     /*"存储器故障或损坏"*/
        INT8U bit6 : 1;          /*"保留"*/
        INT8U RtcErr : 1;        /*"时钟故障"*/

        INT8U bit8 : 1;          /*"保留"*/
        INT8U Cregit : 1;        /*"透支状态"*/ 
        INT8U OpenState : 1;     /*"开表盖"*/
        INT8U bit11 : 1;         /*"保留"*/
        INT8U bit12 : 1;         /*"保留"*/
        INT8U PowerErr : 1;      /*"电源异常"*/
        INT8U RelayRelease : 1;  /*"跳闸成功"*/
        INT8U RelayHoldSuc : 1;  /*"合闸成功"*/

        INT8U bit16 : 1;         /*"保留"*/
        INT8U bit17 : 1;         /*"保留"*/
        INT8U bit18 : 1;         /*"保留"*/
        INT8U bit19 : 1;         /*"保留"*/
        INT8U LoadCurrent : 1;   /*"过流"*/
        INT8U bit21 : 1;         /*"保留"*/
        INT8U bit22 : 1;         /*"保留"*/
        INT8U bit23 : 1;         /*"保留"*/

        INT8U bit24 : 1;         /*"保留"*/
        INT8U bit25 : 1;         /*"保留"*/
        INT8U bit26 : 1;         /*"保留"*/
        INT8U bit27 : 1;         /*"保留"*/
        INT8U bit28 : 1;         /*"保留"*/
        INT8U bit29 : 1;         /*"保留"*/
        INT8U bit30 : 1;         /*"保留"*/
        INT8U bit31 : 1;         /*"保留"*/	

        INT8U bit32 : 1;         /*"保留"*/
        INT8U bit33 : 1;         /*"保留"*/
        INT8U bit34 : 1;         /*"保留"*/
        INT8U bit35 : 1;         /*"保留"*/
        INT8U bit36 : 1;         /*"保留"*/
        INT8U PowerOff : 1;      /*"掉电"*/
        INT8U bit38 : 1;         /*"保留"*/
        INT8U bit39 : 1;         /*"保留"*/

        INT8U bit40 : 1;         /*"保留"*/
        INT8U bit41 : 1;         /*"保留"*/
        INT8U bit42 : 1;         /*"保留"*/
        INT8U bit43 : 1;         /*"保留"*/
        INT8U bit44 : 1;         /*"保留"*/
        INT8U bit45 : 1;         /*"保留"*/
        INT8U bit46 : 1;         /*"保留"*/
        INT8U bit47 : 1;         /*"保留"*/
	
        INT8U ProgramEvent : 1;	 /*"编程"*/
        INT8U MeterClr : 1;		 /*"电表清零"*/
        INT8U bit50 : 1;		 /*"保留"*/
        INT8U EventClr : 1;		 /*"事件清零"*/
        INT8U adiustTime : 1;	 /*"校时"*/
        INT8U DayTabPrg : 1; 	 /*"时段表编程"*/
        INT8U YearTabPrg : 1;	 /*"时区表编程"*/
        INT8U WeakTabPrg : 1;	 /*"周休日编程"*/

        INT8U bit56 : 1;		 /*"保留"*/
        INT8U EnergyMode : 1;	 /*"有功组合方式编程"*/
        INT8U bit58 : 1;		 /*"保留"*/
        INT8U bit59 : 1;		 /*"保留"*/
        INT8U BillSetPrg : 1;	 /*"结算日编程"*/
        INT8U TriffSetPrg : 1;	 /*"费率参数表编程"*/
        INT8U LevelSetPrg : 1;	 /*"阶梯表编程"*/
        INT8U updateKey : 1; 	 /*"密钥更新"*/
     }bits;
    
}UN_645EVTRPT_MODE;


typedef struct
{
    UN_645EVTRPT_STATUS un645NewRptStatus;/*"当前状态"*/
    UN_645EVTRPT_STATUS un645OldRptStatus;/*"过去状态"*/
    UN_645EVTRPT_STATUS ClrReportStatus;/*"需要清除的标志位"*/  
    INT16U rptTimer;/*"主动上报等待剩余时间 秒为单位"*/
    INT8U  rptFlg; /*"需主动上报标志"*/
    INT8U  rptStartFlg;/*"主动上报开始标志"*/
    INT8U  rptEvtCnt[MAX_645EVENT_CNT];   /*"主动上报新增的事件次数"*/ 
}ST_645EVTRPT_BLK;
#endif

extern ST_EVTRPT_STATUS mEvtPrtStatus;/*"事件主动上报变量"*/
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
//extern INT8U PrStateST;/*"编程事件上报开始状态"*/
extern INT8U SI_645Report(INT8U *pdata);
extern INT8U SI_645RstReport(INT8U *pdata);
extern void Report645Status_Get(void);
extern void Report_1Sec(void);
extern INT8U SI_645RstReport(INT8U *pdata);
#endif

#endif

