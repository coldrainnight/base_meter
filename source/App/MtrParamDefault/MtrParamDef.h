/*"*****************************************************************************"*/
/*"  FileName: MtrParamDef.h"*/
/*"  Description:     该文件定义电能表默认参数数据结构体"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.11.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"*******************************************************************************"*/
#ifndef _MTRPARAMDEF_H_
#define _MTRPARAMDEF_H_

#include "PlatForm.h"
#include "APP\LegallyRelevant\LRM_Config.h"
#include "APP\LegallyRelevant\EU.h"
#include "App\UserInterface\EventReport\Report.h"

/*"法定计量模块运行参数定义"*/
typedef struct
{
    FTR_DATA_TP validFlg; /*"合法性检测特征字，固定为5a5a"*/
#ifdef  DLT698
    INT8U AEngyComType; /*"有功组合模式字"*/
#endif
    INT8U PulseWidth; /*"脉冲宽度"*/
#ifdef THREE_PHASE
#ifdef  DLT698
    INT8U REngyComType1; /*"无功组合模式字1"*/
    INT8U REngyComType2; /*"无功组合模式字2"*/
#endif
#endif
#ifdef ENGY_REGISTER_ACTIVE_FUN
    INT8U EngyRegMask[TARIFF_NUM][(E_ENGY_MASK_MAX >> 3) + 1];/*"分时电量寄存器激活mask参数，位控制定义顺序同EN_ENGY_REG_MASK"*/
#endif
    CRC_DATA_TP Crc; /*"CRC校验"*/
}ST_LRM_PARA;

/*"电网事件判断参数结构体定义"*/
typedef enum
{
      EA_OWEFEE=0,          /*"继电器预跳闸状态"*/
      EA_BATTLOSTVOLT,      /*"时钟电池欠压"*/
      EA_ClOCKERR,          /*"时钟故障"*/
      EA_EEPROMERR,	        /*"EEPROM故障"*/
      EA_ESAMERR,           /*"ESAM故障"*/
      EA_CTRCIRCLEERR,      /*"继电器误动作"*/
      EA_COVEROPEN,         /*"开表盖"*/
      EA_TAILOPEN,          /*"开端盖"*/
      
      EA_MAGNET,            /*"磁场干扰"*/
      EA_OVERFACTOR,        /*"总功率因数超下限"*/
      EA_VOLTREVERSE,       /*"电压逆相序"*/
      EA_CURRREVERSE,       /*"电流逆相序"*/
      EA_PWRREVERSE,        /*"潮流反向"*/
      EA_LOSTVOLTAGE,       /*"失压"*/  
      EA_BREAKPHASE,        /*"断相"*/
      EA_OVERVOLTAGE,       /*"过压"*/  
      
      EA_VOLTAGEDOWN,       /*"欠压"*/  
      EA_VOLTIMBALANCE,     /*"电压不平衡"*/  
      EA_CURRIMBALANCE,     /*"电流不平衡"*/ 
      EA_CURRFARIMBALANCE,	/*"电流严重不平衡"*/      
      EA_LOSTCURRENT,       /*"失流"*/
      EA_OVERCURRENT,       /*"过流"*/
      EA_FAILCURRENT,       /*"断流"*/
      EA_UPLOAD,            /*"过载"*/
      EA_GLXCOMM_ERR,            /*"管理芯通信异常"*/
      
      EA_ALARM_FLAG_MAX,  
}MNT_ALARM_FLAG;

typedef enum
{
      EA_LCD=0,
      EA_BGT,
      EA_RLY,
      EA_ALARM_SCR_MAX,  
}MNT_ALARM_SCR;

typedef struct
{
    FTR_DATA_TP validFlg;       /*"数据合法性检查特征字，固定为0x5a5a"*/
    INT32U mntAlarmMode[EA_ALARM_SCR_MAX];   /*"报警模式字"*/
#ifdef THREE_PHASE
    INT32U LossCurrLimitIU; /*"失流事件电流触发上限"*/
    INT32U LossCurrLimitIL; /*"失流事件电流触发下限"*/
    INT32U BrkPhaseLimitIU; /*"断相事件电流触发上限"*/
    INT32U LossVoltLimitIL; /*"失压事件电流触发下限"*/
    INT32U FailCurrLimitIU; /*"断流事件电流触发上限"*/
#endif
    INT32U OverCurrLimitIL; /*"过流事件电流触发下限"*/
    INT32U RlyTripCtrlCurr; /*"继电器拉闸控制电流门限值"*/
    INT16U DisBatVoltThrL;  /*"低功耗电池欠压阀值"*/
#ifdef THREE_PHASE
    INT32U UpLoadLimitP; /*"过载事件有功功率触发下限"*/
    INT32U PhasePwrReverseLimitP; /*"功率反向事件有功功率触发下限"*/
    INT32U AAOverDemandLimitP; /*"正向有功需量超限事件需量触发下限"*/
    INT32U NAOverDemandLimitP; /*"反向有功需量超限事件需量触发下限"*/
    INT32U OverDemandLimitQ; /*"无功需量超限事件需量触发下限"*/

    INT16U VoltChkLimitU; /*"电压考核上限"*/
    INT16U VoltChkLimitL; /*"电压考核下限"*/
    INT16U VoltHealthLimitU; /*"电压合格上限值"*/
    INT16U VoltHealthLimitL; /*"电压合格下限值"*/
    INT16U OverVoltLimitVL; /*"过压事件电压触发下限"*/
    INT16U UnderVoltLimitVU; /*"欠压事件电压触发上限"*/
    INT16U LossVoltLimitVU; /*"失压事件电压触发上限"*/
    INT16U LossVoltLimitVL; /*"失压事件电压恢复下限"*/
    INT16U BrkPhaseLimitVU; /*"断相事件电压触发上限"*/
    INT16U LossCurrLimitVL; /*"失流事件电压触发下限"*/
    INT16U CutCurrLimitVL; /*"断流事件电压触发下限"*/

    INT16U ClkBatVoltThrL; /*"时钟电池欠压阀值"*/

    INT16U OverFactorThr; /*"总功率因数超下限阀值"*/
    INT16U CurrImbalLimitRatio; /*"电流不平衡率限值 "*/
    INT16U VoltImbalLimitRatio; /*"电压不平衡率限值"*/
    INT16U CurrBadlyImBalLimitRatio; /*"电流严重不平衡限值"*/
#endif
    INT8U CriticalVoltage; /*"临界电压"*/
#ifdef THREE_PHASE
    INT8U LossVoltDelayT; /*"失压事件判定延时时间"*/
    INT8U BrkPhaseDelayT; /*"断相事件判定延时时间 "*/
    INT8U LossCurrDelayT; /*"失流事件判定延时间"*/
    INT8U UnderVoltDelayT; /*"欠压事件判定延时时间"*/
    INT8U OverVoltDelayT; /*"过压事件判定延时时间"*/
    INT8U VoltImbalDelayT; /*"电压不平衡率判定延时时间"*/
    INT8U CurrImbalDelayT; /*"电流不平衡率判定延时时间"*/
    INT8U PwrReverseDelayT; /*"功率反向事件判定延时时间"*/
    INT8U OverLoadDelayT; /*"过载事件判定延时时间"*/
    INT8U OverFactorDelayT; /*"总功率因数超下限判定延时时间"*/
    INT8U CurrBadlyImbalDelayT; /*"电流严重不平衡触发延时时间"*/
    INT8U CutCurrDelayT; /*"断流事件判定延时时间"*/
    INT8U AAOverDemandDelayT; /*"正向有功需量超限事件判定延时时间"*/
    INT8U NAOverDemandDelayT; /*"反向有功需量超限事件判定延时时间"*/
    INT8U ROverDemandDelayT;    /*"无功需量超限事件判定延时时间"*/
    INT8U AuxPwrDownDelayT; /*"辅助电源掉电事件判断时间,暂时不用"*/
    INT8U VoltReverseDelayT; /*"电压逆相序事件判定时间"*/
    INT8U CurrReverseDelayT; /*"电流逆相序事件判定时间"*/
    INT8U MagnetDelayT; /*"恒定磁场干扰事件判定时间"*/
#endif
    INT8U ClkErrDelayT; /*"电能表时钟故障事件判定时间"*/
    INT8U MuErrDelayT; /*"电能表计量芯片故障事件判定时间"*/
    INT8U OverCurrDelayT; /*"过流事件判定延时时间"*/
    INT8U RelayErrChkDelayT; /*"负荷开关误动作事件判定时间"*/
    INT8U PwrAbnormalDelayT; /*"电源异常事件判定时间"*/
    INT8U PwrDownDelayT; /*"电能表掉电事件判定时间"*/
    CRC_DATA_TP crc;
}ST_MNT_PARA;

/*"*****************************	预付费参数 *******************************"*/
typedef struct
{
    INT8U stepNum;		/*"梯度数"*/
    INT8U SecuityFlg; 	/*"是否启用安全传输1启用 0不启用"*/
	INT8U mtrTypeMode;  /*"表型模式字"*/
	INT8U relayMode;    /*"继电器模式字0内置、1外置电平、2外置脉冲"*/
	UN_BYTE_DEF PayUserFlg[4];/*"预付费用户扩展模式字"*/
}ST_PM_PARA_1HEX;

typedef struct
{
    INT16U rlyTripTime;		/*"跳闸延时,单位:分钟（远程用) "*/
	INT16U identAuthTime;	/*"远程身份认证时长参数单位分钟"*/
	INT16U overTripCurrTime;   /*"超拉闸控制电流门限保护延时时间,单位:分钟"*/
	INT16U rlyActTime;		/*"外置继电器脉冲方式跳闸维持时间,单位:mS"*/
	INT16U rlyActEngy;		/*"外置继电器脉冲方式再次跳闸电量,单位:Wh"*/
}ST_PM_PARA_2HEX;


typedef struct
{
    INT32U alarmLimit1;/*"报警门限值1"*/
    INT32U alarmLimit2; /*"报警门限值2"*/
    INT32U creditLimit;/*"透支门限值"*/
    INT32U hoardLimit;/*"囤积门限值"*/
    INT32U clsRlyMinMoney;/*"合闸允许最小金额"*/
	INT32U ct;/*"电流变比"*/
    INT32U pt;/*"电压变比 "*/
	INT32U IrAuthTime;/*"红外认证时长参数"*/
}ST_PM_PARA_4HEX;

typedef struct
{
	INT8U userID[6];       /*" 用户号 ，客户编号高字节在前"*/
	INT8U meterID[6];      /*" 表号高字节在前"*/
	INT8U cardSerialNum[8];/*"卡序列号，唯一标识，复位后8字节"*/
}ST_PM_PARA_XHEX;

/*"切换时间"*/
typedef struct     
{
    ST_5TIME_FORM tariffSwTime;	/*"两套费率电价切换时间"*/
    ST_5TIME_FORM stepSwTime;	/*"两套梯度切换时间"*/
}ST_PM_SWITCH_TIME;

typedef struct
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
	ST_PM_PARA_4HEX pmPara4Hex;
	ST_PM_PARA_2HEX pmPara2Hex;
	ST_PM_PARA_1HEX pmPara1Hex;
    ST_PM_PARA_XHEX pmParaXHex;
	ST_PM_SWITCH_TIME pmSwTime;
    CRC_DATA_TP   Crc;          /*"CRC校验"*/
}ST_PM_PARA;

typedef struct   /*"费率电价"*/
{
	INT32U price[_TARIFF_PRC_NUM];
}ST_TARIFF_PRICE;


typedef struct   /*"费率电价表"*/
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
	ST_TARIFF_PRICE stTriff_price;
	CRC_DATA_TP   Crc;          /*"CRC校验"*/
}ST_TARIFF_TABLE;

typedef struct /*"阶梯电价"*/
{
    INT32U stepEngy[_STEP_NUM]; /*"阶梯值0.01kwh"*/
    INT32U stepPrice[_STEP_PRC_NUM]; /*"阶梯电价"*/
    ST_3TIME_FORM yearBillDay[4];/*"年结算日"*/
}ST_STEP_PRICE;

typedef struct /*"阶梯电价表"*/
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
	ST_STEP_PRICE stStep_price;
	CRC_DATA_TP   Crc;          /*"CRC校验"*/
}ST_STEP_TABLE;


typedef struct LR645_Para{
    INT16U         LoadRecordPeriod[6];
    UN_BYTE_DEF    LoadRecordMode;
    ST_6TIME_FORM  LoadRecoedBeginTime;
}ST_LR645_Para;

typedef struct
{
    INT8U  prdFrzIntval;        /*"周期冻结时间间隔（分钟）"*/
/*"以下5个参数协议树中有压缩，顺序请勿随意改动"*/
    UN_BYTE_DEF  timeFrzMode;      /*"定时冻结模式字"*/
    UN_BYTE_DEF  mmtFrzMode;        /*"瞬时冻结模式字"*/
    UN_BYTE_DEF  aptFrzMode;        /*"约定冻结模式字"*/  
    UN_BYTE_DEF  prdFrzMode;        /*"周期冻结模式字"*/  
    UN_BYTE_DEF  dayFrzMode;        /*"日冻结模式字"*/  
/*"----------------------------------------------------------------------"*/

    ST_2TIME_FORM  dayFrzTm;        /*"日冻结时间"*/
    ST_4TIME_FORM  timeFrzTm;        /*"定时冻结时间"*/
    ST_5TIME_FORM  prdFrzStartTime;        /*"周期冻结起始时间"*/

    //ST_FRZPARA_1HEX   frzPara1Hex;
    //ST_FRZPARA_2TIME  frzPara2Time;
    //ST_FRZPARA_4TIME  frzPara4Time;
    //ST_FRZPARA_5TIME  frzPara5Time;
}ST_FRZ_PARA;

/*"RCRD模块 参数"*/
typedef struct       /*"3个月结算日"*/
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
	ST_DAY_HOUR  monBillDay[MON_BILL_DATE_NUM];       /*"结算日"*/
    ST_LR645_Para LrPara; /*" 负荷记录参数 "*/
    ST_FRZ_PARA frzPara;
	CRC_DATA_TP   Crc;          /*"CRC校验"*/
}ST_RCRD_PARA_TABLE;


/*"分时模块 参数"*/
typedef struct 
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/

#if defined(DLT698)
    ST_5TIME_FORM  SeasonTblSwTime; /*"备用套时区表切换时间"*/
    ST_5TIME_FORM  dayTblSwTime; /*"备用套时段表切换时间"*/

    INT8U  seasonSlotNum; /*"年时区数"*/
    INT8U  dayTblNum; /*"日时段表数"*/
    INT8U  daySlotNum; /*"日时段数"*/
    INT8U  tariffNum; /*"费率数"*/
    INT8U  specDayNum; /*"公共节假日数"*/	
    INT8U  weekendType; /*"周休日特征字"*/ 
    INT8U  weekendDayTblId; /*"周休日时段表号"*/ 

#elif defined(TR_IEC62056)
    s_DateTime  touSwTime; /*"备用活动日历激活时间"*/
    INT16U touEESign; /*"当前活动日历索引标识"*/

#else
#endif
    CRC_DATA_TP crc; /*"CRC校验"*/
}ST_TR_PARA;

typedef struct /*"年时区表单元/ 季表单元"*/    
{
#ifdef TR_IEC62056
    INT8U  seasonName;
#endif
    INT8U month;/*"季表开始日期: 月日"*/
    INT8U day;
    INT8U tblNo;/*"日时段表号/ 周表名"*/
}ST_SEASON_SLOT_FORM;

typedef struct /*"年时区表/ 季表"*/    
{
#ifdef TR_IEC62056
    INT8U  seasonSum;  /*"季表个数"*/
#endif
    ST_SEASON_SLOT_FORM table[SEASON_SLOT_NUM];  /*"季表内容"*/
#ifdef RELIABILITY 
    CRC_DATA_TP crc; /*"CRC校验"*/
#endif
}ST_SEASON_TBL;

typedef struct
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
    ST_SEASON_TBL table[DEFAULT_SEASON_TBL_NUM];/*"默认当前、备用两个时区表"*/
    CRC_DATA_TP crc; /*"CRC校验"*/
}ST_DEFAULT_SEASON_TBL;


typedef struct /*"日时段表单元"*/        
{
    INT8U hour;/*" 费率切换时间: 时分 "*/
    INT8U minute;
    INT8U tariffId;/*" 费率号/ 费率索引ID "*/
}ST_DAY_SLOT_FORM;

typedef struct /*"日时段表"*/
{
#ifdef TR_IEC62056
    INT8U dayId;    /*"日表表号"*/	
    INT8U num;/*"有效时段数 "*/
#endif
    ST_DAY_SLOT_FORM  table[DAY_SLOT_NUM];/*"日表时段内容"*/
#ifdef RELIABILITY 
    CRC_DATA_TP crc; /*"CRC校验"*/
#endif
}ST_DAY_TBL;

typedef struct
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
    ST_DAY_TBL table[DEFAULT_DAY_TBL_NUM];
    CRC_DATA_TP crc; /*"CRC校验"*/
}ST_DEFAULT_DAY_TBL;


/******************通信参数文件部分*******************/

/*RAM 和 文件都存储部分参数*/
/*通信1字节参数文件*/
typedef struct
{
    INT8U BaudRate[E_MAX_COM_ID];/*波特率*/
    INT8U evtRptRstTime; /*"主动上报状态字自复位延时时间"*/
    INT8U  mtrRunMode;         /*"电表运行特征字"*/
    INT8U ExMode[10];
}ST_CM_PARA_1HEX;

/*通信4字节参数文件*/
typedef struct
{
    INT32U SafeMode[MAX_OI_NUM_SAFE_MODE];/*安全模式字定义*/
}ST_CM_PARA_4HEX;

/*通信6字节参数文件*/
typedef struct
{
    INT8U  CommAddr[6];        /*"通信地址"*/     
    INT8U  GroupAddr[6];       /*"组地址"*/
}ST_CM_PARA_6HEX;

/*通信8字节参数文件*/
typedef struct
{
    INT8U AuthenPassword[8];/*认证密码*/
    INT8U Evt645RptMode[8];        /*"645主动上报模式字"*/
}ST_CM_PARA_8HEX;


typedef struct
{
    ST_CM_PARA_1HEX StCmPara1Hex;
    ST_CM_PARA_4HEX StCmPara4Hex;
    ST_CM_PARA_6HEX StCmPara6Hex;
    ST_CM_PARA_8HEX StCmPara8Hex;
}ST_CM_PARA_WITH_RAM_BACKUP;



/*只存文件部分参数*/
typedef struct
{
    INT16U  ProtocolVers;        /*"协议版本号"*/    
}ST_CM_PARA_NO_RAM_BACKUP_2HEX;


typedef struct
{
    INT8U  ActAccuGrade[4];        /*"有功准确度等级"*/     

    INT8U  FacNum[4];              /*"厂商代码"*/   	    
    INT8U  SoftwareVers[4];        /*"厂家软件版本号"*/
    INT8U  HardwareVers[4];        /*"厂家硬件版本号"*/
    INT8U  longitude[4];           /*"经度"*/
    INT8U  latitude[4];            /*"纬度"*/
    INT8U  height[4];              /*"高度"*/
    #if defined (THREE_PHASE)
    INT8U  NeAccuGrade[4];        /*"无功准确度等级"*/     
    #endif
}ST_CM_PARA_NO_RAM_BACKUP_4HEX;


typedef struct
{
    INT8U  VoltRegu[6];           /*"额定电压"*/     
    INT8U  CurrRegu[6];           /*"额定电流"*/
    INT8U  MaxCurr[6];            /*"最大电流"*/ 	    
    INT8U  SoftVerDate[6];        /*"软件版本日期"*/
    INT8U  hardVerDate[6];        /*"硬件版本日期"*/
}ST_CM_PARA_NO_RAM_BACKUP_6HEX;

typedef struct
{
    
    INT8U  ProductDate[7];        /*"生产日期"*/  

}ST_CM_PARA_NO_RAM_BACKUP_7HEX;

typedef struct
{
    INT8U  ManufactorExtInfo[8];   /*厂家扩展信息*/
}ST_CM_PARA_NO_RAM_BACKUP_8HEX;

typedef struct
{
    INT8U  SoftRecordID[16];        /*"软件备案号"*/    

}ST_CM_PARA_NO_RAM_BACKUP_16HEX;



typedef struct
{
    INT8U  AssetNum[32];              /*"资产管理编码"*/    
    INT8U  MeterType[32];             /*"电能表型号"*/ 
    INT8U  SupProfileList[32];        /*支持规约列表*/
    INT8U  SoftVer32[32];             /*扩展软件版本号*/
}ST_CM_PARA_NO_RAM_BACKUP_32HEX;

typedef struct
{
    ST_CM_PARA_NO_RAM_BACKUP_2HEX   StCmData2Hex;
    ST_CM_PARA_NO_RAM_BACKUP_4HEX   StCmData4Hex;
    ST_CM_PARA_NO_RAM_BACKUP_6HEX   StCmData6Hex;
    ST_CM_PARA_NO_RAM_BACKUP_7HEX   StCmData7Hex;
    ST_CM_PARA_NO_RAM_BACKUP_8HEX   StCmData8Hex;
    ST_CM_PARA_NO_RAM_BACKUP_16HEX  StCmData16Hex;
    ST_CM_PARA_NO_RAM_BACKUP_32HEX  StCmData32Hex;
}ST_CM_PARA_NO_RAM_BACKUP;

/*"显示项"*/
#if defined(DM_DLT698)/*"698协议显示对象相关定义"*/
typedef union
{
    INT16U asInt;
    INT8U  asByte[2];
}UN_OIS;

typedef struct
{
    ST_OAD Oad; /*"CSD: 基本OAD"*/
    ST_OAD releOad; /*CSD: 记录型ROAD"*/
    INT8U scrNo; /*"屏序号"*/
}ST_DISP_OBJ;/*"698协议显示对象定义"*/

#elif defined(DM_DLT645)/*"645协议显示对象相关定义"*/

typedef struct 
{
    UN_ID645 unId645;     /*"645_2007协议数据标识"*/
    INT8U leafNum;  /*"显示数据叶子号"*/
}ST_DISP_OBJ; /*"645协议显示对象定义"*/

#else
#endif

typedef struct
{
    ST_DISP_OBJ   cyc[CYC_DISP_MAX]; /*"循显对象列表"*/
 }ST_CYC_DISP_OBJ_LIST;

typedef struct
{
    ST_DISP_OBJ   key[KEY_DISP_MAX]; /*"按显对象列表"*/
}ST_KEY_DISP_OBJ_LIST;


typedef struct
{
    ST_CYC_DISP_OBJ_LIST   CycObjList;
    ST_KEY_DISP_OBJ_LIST   KeyObjList;
}ST_DM_PARA_NO_RAM_BACKUP;

/*DU模块有RAM备份部分参数*/
typedef struct
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
    INT8U  DemandTime;    /*"国内:需量周期:单位:分      之所以用这个，主要是为了好计算和存储，同时也是为了字节对齐"*/
    INT8U  DemandPeriod;  /*"海外:需量周期.             国内:滑差时间  秒  都改成分算了"*/
    CRC_DATA_TP     CrcValue;
}ST_DU_PARA_WITH_BACKUP;

/*UI模块未有备份部分参数*/
typedef struct
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
    ST_CM_PARA_NO_RAM_BACKUP StCmParaNoRamBackup;
    ST_DM_PARA_NO_RAM_BACKUP StDmParaNoRamBackup;
    CRC_DATA_TP     CrcValue;
}ST_UI_PARA_NO_BACKUP;



typedef struct 
{
    INT16U  BacklightTime; /*"按键时背光点亮时间"*/
    INT16U  DisChecklightTime; /*"显示查看背光点亮时间"*/ 
    INT16U  LPKeyDisTime; /*"无电按键屏幕驻留最大时间"*/
    INT16U  keyRetTime; /*"无按键返回时间"*/        
    INT16U  cycleShowTime; /*"循显时间"*/
    INT8U  pwrUpFullShowT; /*"上电全显时间"*/
    INT8U  engyPnt; /*"电能显示小数位"*/        
    INT8U  pwrDmdPnt; /*"功率&需量显示小数位数"*/	   
    INT8U  Lcd12Func; /*"液晶①②字样意义"*/
    INT8U  cycleShowNum; /*"循环显示项目数"*/
    INT8U  keyShowNum; /*"按键显示项目数"*/
}ST_DISP_PARA;

/*"用户接口模块存EEPROM文件数据定义"*/
typedef struct
{
/*"显示参数文件"*/
    ST_DISP_PARA         DispPara;
}ST_DM_PARA_WITH_RAM_BACKUP;

/*UI模块有RAM备份部分参数*/
typedef struct
{
    FTR_DATA_TP validFlg;     /*"合法性检测特征字，固定为5a5a"*/
    ST_CM_PARA_WITH_RAM_BACKUP StCmParaWithRamBackup;
    ST_DM_PARA_WITH_RAM_BACKUP StDmParaWithRamBackup;
    CRC_DATA_TP     CrcValue;
}ST_UI_PARA_WITH_BACKUP;




extern const ST_LRM_PARA LrmParaDefault;
extern const ST_MNT_PARA MntParaDefault;
extern const ST_PM_PARA PMParaDefault;
extern const ST_TARIFF_TABLE PMTriffPticeDefault;
extern const ST_STEP_TABLE PMStepPriceDefault;
extern const ST_RCRD_PARA_TABLE DefMouthBillDay;

#ifdef DU_MODLE
extern const ST_DU_PARA_WITH_BACKUP mstDefaultDuPara;
#endif
extern const ST_UI_PARA_NO_BACKUP DefaultUiParaNoRamBackup;
extern const ST_UI_PARA_WITH_BACKUP DefaultUiParaWithRamBackup;
extern const ST_TR_PARA mstDefaultTrPara;
extern const ST_DEFAULT_SEASON_TBL mstDefaultSeasonTbl;
extern const ST_DEFAULT_DAY_TBL mstDefaultDayTbl;
extern const INT8U SOFT_VERSION_WS[32];/*"软件版本号"*/
extern const INT8U ShowVersion[8];	/*"显示版本号"*/
extern const INT32U NEW_EEP_FLG;/*"内卡初始化标识'*/


#endif
