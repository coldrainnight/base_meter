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

#define MAX_GPRS_STATUS_DISP_TIME  120   /*GPRS状态显示时间*/


typedef struct
{
    INT8U name;/*"继电器描述符"*/
    INT8U status;/*"当前状态"*/
    INT8U prop;/*"开关属性"*/
    INT8U lineSta;/*"接线状态"*/
}ST_RLY_STA;


/*电能表运行状态字1~7定义*/


typedef union       /*"电表运行状态字1"*/
{
    INT16U word;          
    struct
    {
    	INT8U :1; 
    	INT8U :1;     
    	INT8U clockBat:1; /*"时钟电池            0 正常  1欠压"*/
    	INT8U lowPwrBat:1;  /*"停电抄表电池            0 正常  1欠压"*/
    	INT8U actPwrDir:1;/*"有功功率方向  0 正向  1反向"*/
    	INT8U :1;
    	INT8U :1;
    	INT8U :1;

    	INT8U ctrCircleErr:1; /*"控制回路错"*/
    	INT8U EsamErr:1; /*"ESAM错"*/
    	INT8U :1; 
    	INT8U :1;
    	INT8U codeErr:1; /*"内部程序错误"*/
    	INT8U RomErr:1; /*"存储器故障"*/
    	INT8U Overdraft:1; /*"透支状态"*/
    	INT8U ClockErr:1; /*"时钟故障"*/
    }bits;
}UN_RUNWORD1_FORM;


typedef union     /*"电表运行状态字2"*/
{	
    INT16U  word;                                          
    struct
    {
    	INT8U actPwrDirA:1; /*"A相有功功率方向 0 正向  1 反向"*/
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


typedef union       /*"电表运行状态字3"*/
{
    INT16U word;          
	struct
	{
		INT8U dailySlotSet :1;/*"当前运行时段 0 第一套 1 第二套"*/
		INT8U pwrModeL:1;        
		INT8U pwrModeH:1;       /*"供电方式(00主电源，01辅助电源，10电池供电)"*/
		INT8U programable:1;       /*"约外认证/编程状态"*/
		INT8U rlySta:1;        /*"继电器状态 0 通  1 断"*/
		INT8U yearSlotSet :1;     /*"当前运行时区 0 第一套 1 第二套"*/
		INT8U rlyCmdSta:1; /*"继电器命令状态 0 通 1断"*/
		INT8U rlyPreTripAlarm:1;   /*"预跳闸报警状态 0 无 1有"*/

		INT8U mtrType:2;         /*"电表类型  00 非预付费表 01电量型预付费表,10 电费型预付费表"*/
		INT8U tariff:1;    /*"当前运行费率电价 0 第一套 1 第二套"*/
		INT8U step:1;     /*"当前阶梯    0 第一套 1 第二套"*/
		INT8U keepPwrSta:1; /*" 保电状态 "*/
		INT8U identityFlg:1; /*"身份认证标志"*/
		INT8U localOpenAcnt:1;  /*" 本地开户 "*/
		INT8U farOpenAcnt:1;/*" 远程开户 "*/   
	}bits;
 }UN_RUNWORD3_FORM;

typedef union     /*"电表运行状态字4"*/
{	
    INT16U  word;                                          
    struct
    {
    	INT8U :1; 
    	INT8U :1;     
    	INT8U :1; 
    	INT8U :1; 
    	INT8U overCurrA:1;/*"A相过流0 正常  1 过流"*/
    	INT8U :1;
    	INT8U dirA:1;/*"A相方向 0 正向  1 反向"*/
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

typedef union       /*"电表运行状态字7"*/
{
    INT16U word;          
	struct
	{
            INT8U voltReverse:1; /*"电压逆相序"*/
            INT8U currReverse:1; /*"电流逆相序"*/
            INT8U voltImbal:1; /*"电压不平衡"*/
            INT8U currImbal:1; /*"电流不平衡"*/
            INT8U auxiliaryPwr:1; /*"辅助电源失电"*/
            INT8U pwrDown:1; /*"掉电"*/
            INT8U overDemand:1; /*"需量超限"*/       
            INT8U overFactor:1; /*"总功率因数超下限"*/

            INT8U CurrFarImbal:1; /*"电流严重不平衡"*/
            INT8U openCover:1; /*"开表盖"*/
            INT8U openBox:1; /*"开端钮盖"*/
            INT8U :1;
            INT8U :1;
            INT8U :1;
            INT8U :1;
            INT8U :1;	   
	}bits;
 }UN_RUNWORD7_FORM;


typedef struct       /*"电表运行状态字"*/
{
    UN_RUNWORD1_FORM  word1;
    UN_RUNWORD2_FORM  word2;
    UN_RUNWORD3_FORM  word3;
    UN_RUNWORD4_FORM  word4;
    UN_WORD_DEF word5;
    UN_WORD_DEF word6;
    UN_RUNWORD7_FORM  word7;
}ST_RUN_STATUS;


extern ST_RUN_STATUS   RunStatus;/*电能表运行状态字*/
extern UN_WORD_DEF    MeterSampleStatus;     /*计量芯系统状态字*/
extern UN_WORD_DEF    SampleStatus;          /*采样状态字*/

/******************通信参数文件部分结束*******************/







/******************通信数据文件部分*******************/



/******************通信数据文件部分结束*******************/



/*UI模块数据文件*/
typedef struct
{
    #ifdef REPORT645
    ST_645EVTRPT_BLK Event645_Rpt;          /*645协议主动上报文件'*/
    #endif
    INT32U LastBroadcastAdjTime;  /*上一次广播校时时间*/
    INT32U ClockBatWorkTimeMin;/*时钟电池工作时间(分)*/
    TCA_AuthSaveData TcaSaveData;	/*双芯认证掉电数据*/
}ST_UI_FILE_DATA;



extern INT8U UI_ShowFlash(void *pdin, void *pdout);
extern INT8S UI_GetRelayOutput(void *pdin, void *pdout);


extern void UI_Init(INT8U mode);
extern void UI_LoadDefaultUiPara(void);



/*RAM中全局变量*/
extern ST_UI_PARA_WITH_BACKUP StUIParaWithRamBackup;
#ifdef THREE_PHASE
extern INT16S SignalStrength;
extern INT8U  GPRS_Status_Time;
extern INT8U  GPRS_Status;
extern INT8U Change_GPRS_Value_Status(INT16S Value);
#endif

/*"显示参数文件偏移"*/
#define  DM_PARA_SHIFT     
#define  DISP_PARA_SHIFT     (FIND(ST_UI_PARA_WITH_BACKUP, StDmParaWithRamBackup) + FIND(ST_DM_PARA_WITH_RAM_BACKUP, DispPara))
#define  CYC_OBJ_LIST_SHIFT  (FIND(ST_UI_PARA_NO_BACKUP, StDmParaNoRamBackup) + FIND(ST_DM_PARA_NO_RAM_BACKUP, CycObjList))
#define  KEY_OBJ_LIST_SHIFT  (FIND(ST_UI_PARA_NO_BACKUP, StDmParaNoRamBackup) + FIND(ST_DM_PARA_NO_RAM_BACKUP, KeyObjList))


/*通信参数文件偏移*/
#define CM_PARA_WITH_RAM_BACKUP_SHIFT FIND(ST_UI_PARA_WITH_BACKUP, StCmParaWithRamBackup)
#define CM_PARA_NO_RAM_BACKUP_SHIFT FIND(ST_UI_PARA_NO_BACKUP, StCmParaNoRamBackup)

/*有RAM备份数据偏移*/
#define CM_PARA_WITH_RAM_BACKUP_1HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara1Hex))
#define CM_PARA_WITH_RAM_BACKUP_4HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara4Hex))
#define CM_PARA_WITH_RAM_BACKUP_6HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara6Hex))
#define CM_PARA_WITH_RAM_BACKUP_8HEX_SHIFT (CM_PARA_WITH_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_WITH_RAM_BACKUP,StCmPara8Hex))

/*无RAM备份数据偏移*/
#define CM_PARA_NO_RAM_BACKUP_1HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData1Hex) )
#define CM_PARA_NO_RAM_BACKUP_2HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData2Hex) )
#define CM_PARA_NO_RAM_BACKUP_4HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData4Hex) )
#define CM_PARA_NO_RAM_BACKUP_6HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData6Hex) )
#define CM_PARA_NO_RAM_BACKUP_7HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData7Hex) )
#define CM_PARA_NO_RAM_BACKUP_8HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData8Hex) )
#define CM_PARA_NO_RAM_BACKUP_16HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData16Hex) )
#define CM_PARA_NO_RAM_BACKUP_32HEX_SHIFT    (CM_PARA_NO_RAM_BACKUP_SHIFT + FIND(ST_CM_PARA_NO_RAM_BACKUP,StCmData32Hex) )

/*"事件主动上报文件偏移"*/
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

