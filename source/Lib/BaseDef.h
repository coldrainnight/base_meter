/*******************************************************************************
File name 	 : BaseDef.h
Description	 : Base type define header file
Revision History :
DD.MM.YYYY      Description        Modefied By
08.09.2010      First Release      Yuhao.cui
18.02.2016      μC/OS III         Arnold.Jiang
*******************************************************************************/
#ifndef BaseDef_H
#define BaseDef_H
#include "System\sys_def.h"




//平台选用宏    //杨旺 2017.3.22
#define AMETER350_PLATFORM			0
#define	DLMS_I1C_PLATFORM			1

#define PROFILE_OBISIDEX_ENABLE 1   /*"曲线捕获对象是否采用obis index的方式，1表示开启，0不是不启用"*/

//------------------------------------------------------------
#define EQU    1      /* 时间比较返回值定义 */
#define EARLY  0
#define LATE   2


#ifndef true
    #define true           TRUE
#endif
#ifndef false
    #define false          FALSE
#endif

#ifndef Null
  #define Null           NULL
#endif

#define Busy       (0xFF)
#define Free       (0x00)

#define Locked       (1)
#define Unlock       (0)

#define TariffNumber     (8)
#define MAXTariffNumber     (8)
#define DISCONNECTOR_NUM	(4)
#define MBUS_DISCONNECTOR_NUM	(16)

#define NewI2CSign         (0x3434)

#define TimeChangeBill       0
#define MaxEncodeBuffLenth  (320)

#define NEEDSMS         1 
#define CRC_ERR_REDO_TIMES	(3)
#define NEW_EEPROM_ERR_REDO_TIMES	(10)
#define BILLING_PERIOD_NUM  (20)




#define EVENT_INDEX_MAX_NUM              20
#define  DLS_BEGIN     2
#define  DLS_END        1

/************************************************************************* 
              IEC62056  数据类型 定义     
*************************************************************************/
#define	 null_data    (0)
#define	 T_Array      (1) 
#define	 T_Structure  (2)
#define	 T_BOOL       (3)   // bool
#define	 T_BitStr     (4)   //
#define	 T_INT32      (5)   // Tdouble_long
#define	 T_UN32       (6)   // Tdouble_long_unsigned
#define	 T_OctStr     (9)   // octet-string
#define	 T_VisStr     (10)
#define	 T_BCD        (13)
#define	 T_INT8       (15)  // integer8
#define	 T_INT16      (16)  // integer16
#define	 T_UN8        (17)  // unsigned8
#define	 T_UN16       (18)  // unsigned16
#define	 T_LONG64     (20)  // integer64
#define	 T_UN64       (21)  // unsigned64
#define	 T_Enum       (22)  // enum
#define	 T_FLOAT32    (23)  // float32
#define	 T_FLOAT64    (24)  // float64
#define	 T_DateTime   (25)  // date time
#define	 T_Date       (26)  // date
#define	 T_Time       (27)  // time

#define  FIRST_BLOCK          1 
#define  NEXT_BLOCK           2 
#define  LAST_BLOCK           3 
#define  SINGLE_BLOCK         4  

/*********************************************************************
                     COSEM 消息控制字 枚举
*********************************************************************/

#define	 NoneType     (0)
#define	 DataNotification	(0x0F)
#define  AARQ      (0x60)   //96
#define  AARE      (0x61)   //97
#define  RLRQ      (0x62)   //98
#define  RLRE      (0x63)   //99
#define  AbortReq  (0x64)
#define	 ReadReq   (0xC0)   // 192  read require
#define  WriteReq  (0xC1)   // 193  0x32
#define  ActionReq (0xC3)   // 195 
#define  ReadCnf   (0xC4)   // 196
#define  WriteCnf  (0xC5)   // 197
#define  ActionCnf (0xC7)   // 199
#define  ErrorType (0xC8)   /* 响应error时， Pdata 直接执行错误类型代码 */
#define  Done      (0xC9)   // 
#define  ReadNext  (0xCA)
#define  ReadNextCnf (0xCB)
#define	 AutoCapReq	(0xD1)	//modified by yangwang 外部自动捕获曲线计时
#define	 AutoCapCnf	(0xD5)	//modified by yangwang 外部自动捕获曲线计时回复
/*********************************************************************
                     COSEM class 内部消息控制字 枚举
*********************************************************************/
#define	 CLASS01     (1)
#define	 CLASS03     (3)
#define	 CLASS04     (4)
#define	 CLASS05     (5)
#define  CLASS06 	 (6)
#define	 CLASS07     (7)
#define	 CLASS08     (8)
#define  CLASS11 	 (11)
//#define  CLASS20 	 (20)
#define  CLASS22 	 (22)
#define  CLASSTOU	 (20)


/************************************************************************* 
              IEC62056 访问结果枚举    
*************************************************************************/
#define  success                     (0)
#define  hardware_fault              (1)
#define  temporary_failure           (2)
#define  read_write_denied           (3)
#define  object_undefined            (4)
#define  object_class_inconsistent   (9)
#define  object_unavailable          (11)
#define  type_unmatched              (12)
#define  scope_of_access_violated    (13)
#define  data_block_unavailable      (14)
#define  long_get_aborted            (15)
#define  no_long_get_in_progress     (16)
#define  long_set_aborted            (17)
#define  no_long_set_in_progress     (18)
#define  other_reason                (250)

#define  wait_response                (240) /*"等待应答"*/
#define  delay_response                (249) /*"等待超时"*/

/************************************************************************* 
              IEC62056 属性权限描述定义     
*************************************************************************/
#define	NO      0   
#define YES     1 
#define RO      1
#define WO      2
#define WR      3
#define Exc     1

/* ------- 属性权限索引定义 ------ NO  LLS1 LLS2 LLS3 FAC(FC PRCC MC FT) --- */
#define ATTR_RWID_FAC      (0)    /*"NO, NO, NO, NO, WR"*/
#define ATTR_RWID_01       (1)    /*"NO, NO, NO, RO, WR"*/
#define ATTR_RWID_02       (2)    /*"NO, NO, NO, WR, WR"*/
#define ATTR_RWID_03       (3)    /*"NO, NO, RO, RO, WR"*/
#define ATTR_RWID_04       (4)    /*"NO, NO, RO, WR, WR"*/
#define ATTR_RWID_05       (5)    /*"NO, NO, WR, WR, WR"*/
#define ATTR_RWID_06       (6)    /*"NO, RO, RO, RO, WR"*/
#define ATTR_RWID_07       (7)    /*"NO, RO, RO, WR, WR"*/
#define ATTR_RWID_08       (8)    /*"NO, RO, WR, WR, WR"*/
#define ATTR_RWID_09       (9)    /*"NO, WR, WR, WR, WR"*/
#define ATTR_RWID_10       (10)   /*"RO, RO, RO, RO, WR"*/
#define ATTR_RWID_11       (11)   /*"RO, RO, RO, WR, WR"*/
#define ATTR_RWID_12       (12)   /*"NO, WO, WO, WO, WR"*/
#define ATTR_RWID_13       (13)   /*"NO, NO, WO, WO, WR"*/
#define ATTR_RWID_14       (14)   /*"NO, NO, NO, WO, WR"*/
#define ATTR_RWID_15       (15)   /*"RO, RO, WR, WR, WR"*/
#define ATTR_RWID_16       (16)   /*"RO, NO, NO, RO, WR"*/
#define ATTR_RWID_17       (17)   /*"NO, WO, NO, WO, WR"*/
#define ATTR_RWID_18       (18)   /*"RO, NO, NO, RO, WR"*/
#define ATTR_RWID_19       (19)   /*"NO, RO, NO, RO, WR"*/
#define ATTR_RWID_20       (20)   /*"NO, WO, NO, NO, WR"*/
#define ATTR_RWID_21       (21)   /*"RO, NO, RO, WR, WR"*/
#define ATTR_RWID_22       (22)   /*"NO, WR, NO, WR, WR"*/
#define ATTR_RWID_23       (23)   /*"RO, NO, NO, WR, WR"*/

/* ------- 方法权限索引定义 ------ NO  LLS1 LLS2 LLS3 FAC(FC PRCC MC FT) --- */
#define METH_EXCID_FAC     (0)    /*"NO, NO, NO, NO, YES"*/
#define METH_EXCID_01      (1)    /*"NO, NO, NO, YES, YES"*/
#define METH_EXCID_02      (2)    /*"NO, NO, YES, YES, YES"*/
#define METH_EXCID_03      (3)    /*"NO, YES, YES, YES, YES"*/
#define METH_EXCID_04      (4)    /*"YES, YES, YES, YES, YES"*/
#define METH_EXCID_05      (5)    /*"NO, NO, YES, NO, YES"*/
#define METH_EXCID_06      (6)    /*"NO, YES, YES, NO, YES"*/
#define METH_EXCID_07      (7)    /*"YES, YES, YES, NO, YES"*/


#define  TimeToHMS          0
#define  HMSToTime          1
#define  YMDToDate          2
#define  DateToYMD          3
#define  DateTimeToYMDHMS   4
#define  YMDHMSToDateTime   5

/************************************************************************* 
              IEC62056 单位、量程结构定义及相关函数     
*************************************************************************/
/* ----------- 单位枚举 -------- */
#define  None_Unit    (0xFF)    /* 无单位   modified by shaodonghai 2011-07-19  */
#define  U_Day        (4)    /* 时间 天 */
#define  U_h          (5)    /* 时间 小时 */
#define  U_Min        (6)    /* 时间 分钟 */
#define  U_S          (7)    /* 时间 秒   */
#define  U_Degree     (8)    /* 角度  度  */
#define  U_Temper     (9)    /* 温度 摄氏度 */
#define  U_Currency   (10)    /*货币  */
#define  U_M3         (13)   /* 体积 立方米 */
#define  U_M3h        (15)   /* 流量 立方米每小时 */
#define  U_M3d        (16)   /* 流量 立方米每天   */  
#define  U_J          (25)   /* 热量 焦耳         */
#define  U_Jh         (26)   /* 热流量 焦耳每小时 */
#define  U_W          (27)   /* 有功功率  瓦      */
#define  U_VA         (28)   /* 视在功率  伏安    */
#define  U_Var        (29)   /* 无功功率  乏      */
#define  U_Wh         (30)   /* 有功电能  瓦时    */
#define  U_VAh        (31)   /* 视在电能  伏安时  */
#define  U_Varh       (32)   /* 无功电能  乏时    */
#define  U_A          (33)   /* 电流      安培    */
#define  U_V          (35)   /* 电压      伏特    */
#define  U_Hz         (44)   /* 频率     赫兹     */
#define  U_Per_Wh     (45)   /*  1/wh  */
#define  U_Per_Varh   (46)   /*  1/varh  */
#define  U_Per_VAh    (47)   /*  1/vah  */
#define  U_V2h        (48)   /* 伏方时            */
#define  U_A2h        (49)   /* 安方时            */
#define  U_Percentage (56)   /* % */
#define  U_Size_KByte  (57)
#define  U_Other      (254)  /* 其他              */
#define  U_Count      (255)  /* 计数用            */


/* ------- 定义缺省的 单位索引 --------------------- */

#define Default_DM_Ap_Unit      (6)
#define Default_DM_Qp_Unit      (15)
#define Default_DM_Sp_Unit      (24)
#define Default_Ap_Unit         (6)    /* 0  缺省 有功功率 单位  0.1 W     */
#define Default_Qp_Unit         (15)   /* 1  缺省 无功功率 单位  0.1 Var   */
#define Default_Sp_Unit         (24)   /* 2  缺省 视在功率 单位  0.1 VA    */
#define Default_ApEnery_Unit    (31)   /* 3  缺省 有功电能 单位  10 Wh    */
#define Default_QpEnery_Unit    (40)   /* 4  缺省 无功电能 单位  10 Varh  */

#define Default_SpEnery_Unit    (49)   /* 5  缺省 视在电能 单位  10 VAh   */
#define Default_Currency_Unit   (64)   /* 6  缺省 货币     单位  0.01    */
#define Default_Voltage_Unit    (74)   /* 7  缺省 电压     单位  0.001V    */
#define Default_Current_Unit    (82)   /* 8  缺省 电流     单位  0.001A  */
#define Default_PwrFactr_Unit   (92)   /* 9  缺省 功率因数 单位  0.0001  */
#define Default_Angle_Unit      (99)   /* 10 缺省 角度     单位  0.01度  */
#define Default_U_Angle_Unit    (100)  /* 10 缺省 电压角度 单位  0.001度  */
#define Default_Frequently_Unit (106)  /* 11 缺省 频率     单位  0.01Hz */
#define Default_TmSec_Unit      (112)  /* 13 缺省 时间 秒  单位  1 s     */
#define Default_TmMin_Unit      (119)  /* 14 缺省 时间 分  单位  1 m     */
#define Default_TmHour_Unit     (124)  /* 15 缺省 时间 时  单位  1 h     */
#define Default_Modulus_Unit    (62)   /* 6  缺省 系数     单位  1       */
#define Default_Temprature_Unit (129)  /* 16  缺省 温度    单位  1 度    */
#define Default_A2h_Unit        (138)  /* 17  缺省 安方时  单位  0.01A2h */
#define Default_V2h_Unit        (147)  /* 18  缺省 伏方时  单位  0.01V2h */
#define Default_Cubage_Unit     (156)  /* 19  缺省 体积    单位  0.01M3  */
#define Default_Flux_Unit       (162)  /* 20 缺省 流量     单位  0.1M3h  */
#define Default_Heat_Unit       (173)  /* 21 缺省 热量     单位  0.01 J  */
#define Default_Count_Unit      (183)  /* 22 缺省 计数     单位  1       */
#define Default_Percentage_Unit (188)   //%
#define Default_Per_Wh_Unit     (189)   //%  /*  1/wh  */
#define Default_Per_VAh_Unit    (190)   //%  /*  1/vah  */
#define Default_MemorySize_Unit (191)   //%  /*  kByte  */
#define Default_Days_Unit       (195)  /* 缺省 时间 天     单位 0.1 day  */
#define Default_Days_Unit1      (194)  /* 缺省 时间 天     单位 1 day  */
#define NoUnit                  (0xFF) /* 23  无单位                     */
//-----------------------------------
#define Default_Tarrif_Unit     (88)   /*当前激活的费率号03*/
#define Default_Temper_Unit     (130)  /*  缺省 温度单位  0.1 ℃     */
#define Default_CurrRate_Unit   (66)   /*  缺省 货币  单位  0.0001    */


/* ------- 原始数据的小数点位置 - 需根据具体计量芯片做修改--- */
#define Urms_SSclr       (-4)
#define Irms_SSclr       (-4)
#define Actpower_SSclr   (-3)   // 0.001 W
#define ReActpower_SSclr (-3)   // 0.001 Var
#define Aprnpower_SSclr  (-3)   // 0.001 VA
#define PF_SSclr         (-6)   // 
#define HZ_SSclr         (-6)
#define ActEngy          (-3)   // 0.001Wh
#define ReactEngy        (-3)   // 0.001Varh
#define AprnEngy         (-3)   // 0.001VAh

/************************************************************************* 
             各个接口类任务ID列表
*************************************************************************/
#define Data_ClassID        (1)      // define my class id number
#define Reg_ClassID         (3)      // define my class id number
#define ExReg_ClassID         (4)      // define my class id number
#define Demand_ClassID	    (5)
#define RegAction_ClassID  (6)      // define my class id number
#define Profile_ClassID     (7)      // define my class id number
#define Clock_ClassID      (8)      // define my class id number
#define Script_ClassID      (9)      // define my class id number
#define SpecialDay_ClassID  (11)     // define my class id number
#define LN_ClassID          (15)     // define my class id number
#define Calendar_ClassID    (20)     // define my class id number
#define Monitor_ClassID     (21)     // define my class id number
#define SingleAct_ClassID   (22)     // define my class id number
#define HDLC_ClassID        (23)     // define my class id number
#define LOCALPORT_ClassID   (19)
#define DisControl_ClassID  (70)	 // added 5.24
#define Sap_ClassID         (17)	//  added 8.15
#define Push_ClassID      	(40)    //add  2016.8.2 by li qiang
#define ImageTransfer_ClassID (18)
#define TcpUdpSetup_ClassID    (41)     // define my class id number
#define GPRSModemSetup_ClassID		(45)
#define SecuritySetup_ClassID (64)
#define Limiter_ClassID (71)

///***********************************************************************   modifly by yy 2015,12,4


//通用任务消息属性
#define TASK_MESSAGE_NONE		0   // invaild this message
#define TASK_MESSAGE_NORMAL		1   //normal message
#define	TASH_MESSAGE_NO_RES		2
//liqiang add 2016.4.9
//COSEM Classes inner message. 
//Example for application:Class08 send msg to AppTaskLoopSecondFun 
//setting time at immediately.
//#define TASK_MESSAGE_ACTION		2
//#define TASK_MSG_CAPTURE    	3
//#define	TASK_MSG_FILESCAN		4
//#define	TASK_MSG_REPLENISH      5/*load profile  replenish record*/


typedef struct _OBIS_Type     
{
	INT8U  A;
	INT8U  B;
	INT8U  C;
	INT8U  D;
	INT8U  E;
    INT8U  F;
}OBIS_Type;



/************************************************************************* 
              IEC62056 时间类型 定义及转换函数     
*************************************************************************/
/* define time data type */
typedef union _TimeType
{
    INT8U time_byte[4];
	struct
	{
		INT8U Hour;        /* range 0 .. 23, 0xFF,   0xFF = not specified */
	    INT8U Min;         /* range 0 .. 59, 0xFF,   0xFF = not specified */
	    INT8U Sec;         /* range 0 .. 59, 0xFF,   0xFF = not specified */
	    INT8U HunredSecS;  /* range 0 .. 99, 0xFF,   0xFF = not specified */
	}Time;
}TimeType;

/* define date data type */
typedef union _DateType
{
    INT8U date_byte[5];
	struct
	{
		INT8U Year_H;     /* range 0 .. big,  0xFFFF = not specified */    
		INT8U Year_L;
		INT8U Month;      /* range 0 .. 12, 0xFF, FE, FD;  0xFF = not specified */
		INT8U Day;        /* range 0 .. 31, 0xFF, FE, FD;  0xFF = not specified */
		INT8U Week;       /* range 1 .. 7, 1 = moday       0xFF = not specified */
	}Date;
}DateType;

/* define date_time data type */
typedef union _DateTimeType
{
	INT8U datetime_byte[12];
	struct
	{
		DateType  Date;
		TimeType  Time;
		INT8U Dev_H;          /* in minutes of local time to GMT,  0x8000 = not specified */
		INT8U Dev_L;          /* -720 .. 720 */
		INT8U Clock_status;
	}DateTime;
	
}DateTimeType;

typedef struct {  
	INT8U Year;  
	INT8U Month; 
	INT8U Day;   
	INT8U Week;  
	INT8U Hour;  
	INT8U Minute;
	INT8U Second;   
}s_DateTime;//hex type


/*********************************************************************
                  COSEM\DLMS间信息传递结构类型定义
*********************************************************************/
typedef struct
{
    INT8U Attribute;
    INT8U STaskID;
    INT8U DTaskID;
    INT8U Code;
    INT8U *pData;	
}s_Message;


/*********************************************************************
                  COSEM 内部 信息传递结构类型定义
*********************************************************************/
typedef struct     
{
    INT8U       LockEn;         /* 消息锁           */
    INT8U       ServerID;       /* 用于识别服务id号 */
    INT16U       ClassID;        /* 类标识           */ 
    INT8U       OBIS[6];        /* 类对象标识       */
    INT8U       AttriID;        /* 标识属性号(执行命令时为方法号), 0代表所有属性           */
    INT8U       ResultType;     /* 用于表示访问结果 */
    INT8U       AccessRight;	//访问权限，yangwang 2016.10.29
    INT8U       Offset;
    INT8U       AANum;
    INT8U       NextBlock;      /* 是否为 最后一块 */
    INT16U      EncodeLenth;    /* 编码长度   协议栈发时为允许编码的长度，各类数据返回时为编码好的长度：modified by cyh 4.12 */
    INT8U       *PData;         /* 读取时指针为空或选择访问参数， 写和执行方式时执行 参数，*/
                                /* 用于响应时执行访问的结果(读响应时可强制转换成)对应类实  */
                                /* 例的接口类型. 在捕获命令时，指向OBIS列表，并且返回时执行数据， 列表和数据均采用a-xdr */
                                //Add by slx 2013-6-20
    INT8U       *pOutData;      //响应数据编码的buff，由协议栈分配，给COSEM对象用，避免多个类共同使用EncodeBuff    
    #if AMETER350_PLATFORM
    INT16U      MacroID;        /* Add by JIANG 2016-1-29 */ 
    #endif
    #if PROFILE_OBISIDEX_ENABLE
    INT16U      ObjectIdex;  /*"曲线的捕获对象在该类中的对象列表中的index"*/
    #endif
}COSEM_MessageType;



/* define one attribute right type */
typedef struct _Right_Type
{
    INT8U Level_0:2;
    INT8U Level_1:2;
    INT8U Level_2:2; 
    INT8U Level_3:2;
	INT8U Level_4:2;
}Right_Type;

typedef void (*TypeChange)(INT8U *, INT8U *);

typedef INT16U (*EncodeFunType)(INT8U *, INT8U *, INT16U);

/* Unit- Scaler  type define */
typedef struct _UnitScaler_Type
{
   INT8U  Unit; 
   INT8S Scaler;
}UnitScaler_Type;
/************************************************************************* 
           计量相关数据接口 
*************************************************************************/
typedef struct
{
    INT32U P;        // 0.01W 
    INT32U Q;        // 0.01Var 
    INT32U S;        // 0.01VA 
    
    INT32U Urms;     // 0.01V
    INT32U Irms;     // 0.01A
    INT32U Freq;     // 0.01Hz
    INT32U PF;       // 0.01
    
    INT32U EP;       // 1 Wh
    INT32U EQ;       // 1 Varh
    INT32U ES;       // 1 VAh
    INT8U  Limit;    // 0~3  : 1~4 /* 象限 */
} PhaseDataDef;

typedef struct 
{
    BOOL  Updated;
    INT16U  temperature; /* 0.001 摄氏度 */
    
    PhaseDataDef T;
    PhaseDataDef A;
    PhaseDataDef B;
    PhaseDataDef C;

    INT32U  CoLine_Irms_M; /* 中性线电流 测量的 */
    INT32U  CoLine_Urms;   /* 中性线电压        */
    INT32U  CoLine_Irms_C; /* 中性线电流 计算出 */

    INT32U  E_SMeterA; /* 0.000   */
    INT32U  E_SMeterB; /* 0.000   */
    INT32U  E_SMeterC; /* 0.000   */
    INT32U  P_SMeterA; /* 0.000   */
    INT32U  P_SMeterB; /* 0.000   */
    INT32U  P_SMeterC; /* 0.000   */
        
} MeteringPortType;


extern const EncodeFunType Encode[];       /* declare encode function list */
//extern INT8U  EncodeBuffer[];  /* 用于存放编解码后的数据        */
//extern INT8U TariffNum;
//extern const INT32U Exp_10[8];

//extern MeteringPortType MeterPort;
extern const Right_Type  AttriRighList[];    /* 使用全局变量作为标志 */
extern const Right_Type  MethodRighList[];    /* 使用全局变量作为标志 */


/* declare time type covert funciont */
extern void  TimeTypeToHMS(INT8U * Time , INT8U* HMS);
extern void  HMSToTimeType (INT8U* HMS , INT8U * Time );
extern void  YMDToDateType(INT8U* YMD , INT8U * Date);
extern void  DateTypeToYMD (INT8U* Date , INT8U * YMD );
extern void  DateTimeTypeToYMDHMS (INT8U* DateTime, INT8U * YMDHMS );
extern void  YMDHMSToDateTimeType (INT8U* YMDHMS, INT8U * DateTime);
//extern INT32U TimeDelta(DateTimeType DateTimeA, DateTimeType DateTimeB);
extern const TypeChange TimeTypeChg[];   /* declare function list */
/* decaler the list of unitscaler.  Notice: those list must be stored in I2C */
//extern INT8U UnitCnt; /* 当前有效量程单位 记录 */
extern const UnitScaler_Type  UnitSList[];
/* set unit scaler, return the new index in the unitscaler list, if retrun oxFF, means set fault */
INT8U Set_UnitScaler(UnitScaler_Type * NewUnitScaler);
/* read the unitscaler */
BOOL Get_UnitScaler(INT8U Index, UnitScaler_Type * ResultUnitScaler);

/************************************************************************* 
        IEC62056 基本数据类型的编解码函数    统一返回编码的报文长度 
*************************************************************************/
BOOL DeCodeLength(INT8U *Offset, INT16U *Length, INT8U *lpSrc);
INT16U EnArray   (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth); 
INT16U EnStruct  (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnBOOL    (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth); 
INT16U EnINT     (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnUnsgned (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnOctStr  (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnBCD     (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnEnum    (INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnDateTime(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EncodeLenth(INT8U * EnStartAddr,  INT16U DataLenth);
INT16U EnBitStr(INT8U * EnStartAddr, INT8U *Data, INT16U BitStringLenth);
INT8U  EnDateTime2(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnDate(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnTime(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnFloat32(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnConstOctStr( INT8U * EnStartAddr, const INT8U * Data, INT16U DataLenth);
INT16U EnVisibleStr(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth);
INT16U EnConstNull(INT8U * EnStartAddr,  INT16U DataLenth);
INT16U EnConstINT(INT8U * EnStartAddr, const INT8U *Data, INT16U DataLenth);
INT16U EnConstEnum(INT8U * EnStartAddr, const INT8U *Data, INT16U DataLenth);
INT16U EnSelectConstNull(INT8U * EnStartAddr,  INT16U DataLenth);

INT8U Calc_BCC(INT8U *lpSrc, unsigned int Length);  //lpSrc已经剔除了SOH和STX

extern INT8U SysDateCmpr(DateType *Fdate, DateType *Ldate);
extern INT8U SysTimeCmpr(TimeType *Ftime,  TimeType *Ltime);
extern INT8U SysDtTmCmpr(DateTimeType *Fdate,DateTimeType *Ldate);
extern INT8U SysDtTmCmprNoSec( DateTimeType *Fdate,  DateTimeType *Ldate);
extern INT8U BasetimeCmpr(s_DateTime * Systime, DateTimeType *SeasonTime); 

/************************************************************************* 
                             其他的基本函数
*************************************************************************/
//extern Bool  StrCmp(INT8U * Src, INT8U * Dest, INT16U Lenth );   /* 字符串比较 */
extern BOOL  StrConstCmp(const INT8U * Src, const INT8U * Dest, INT16U Lenth );   /* 字符串比较 */

//extern void  StrCopy(INT8U * Src, INT8U * Dest, INT16U Lenth );  /* 字符串复制 */
extern void  ConstStrCopy(const INT8U * Src, INT8U * Dest, INT16U Lenth );

//extern void  StrClean( INT8U * Dest, INT16U Lenth );           /* 字符串清空 */
extern INT32U  Power10(INT8U Exp);                             /* 求10的N次方*/
extern INT32U  CheckScaler(INT8S S_Scaler, INT8S D_Scaler, INT32U Data); /* 小数点平移 */

void CopyCosemMsg(COSEM_MessageType *Source, COSEM_MessageType *Dest );


void HexToDec(INT8U *Data, INT8U Lenth);
void DecToHex(INT8U *Data, INT8U Lenth);

//extern BOOL LoadStructData(INT8U *StructAddr, INT16U EpprommAddr, INT16U Size);/* 从Epprom 中读出某一个结构体数据 */

extern INT8U CalculateWeek(INT16U y, INT8U m, INT8U d);
extern INT8U TimeComp(INT8U *pr, INT8U *ps, INT8U Size);

//extern INT16U List_Sequence;

extern INT16U bswap16(INT16U x);

extern INT32U bswap32(INT32U x);

extern INT16U chSwap16(INT8U *temp);
extern INT32U chSwap32(INT8U *temp);

/* 时间调整函数，时间前调 shift time 单位为 秒 */
extern void DateTimeShiftBack(DateTimeType * CurDateTime, INT32U ShiftTime);
extern void DateTimeShiftForward(DateTimeType * CurDateTime, INT32U ShiftTime);

/* 获取原始计量数据的小数点位数 added by cyh 5.8 */
extern INT8S GetBaseDataScaler(INT8U C_Code);

extern BOOL CheckZero(INT8U *Addr, INT16U Lenth);  /* 检测0数据 若全为0 则返回true */
extern BOOL CheckConstZero(const INT8U *Addr, INT16U Lenth);
extern BOOL CheckWildcards(INT8U *Addr, INT16U Lenth);
extern INT16U EnConstUnsgned(INT8U * EnStartAddr, const INT8U *Data, INT16U DataLenth);

extern INT8U Timer_Valid(INT8U *p, INT8U type);
extern void TimeTransfer(s_DateTime *Time,s_DateTime *TempTime);
extern INT8U DecodeSetData(INT8U * EnStartAddr,INT8U *Data);
extern unsigned long TIME_IDEX(unsigned char  *p);
extern char leap(int year);

//--------add by slx
BOOL FindIdleMsgBuff(INT8U *MsgBuffIndex, COSEM_MessageType *lpMsg, INT8U MsgBuffCount);

unsigned char EncodeDateTime(unsigned char *buff, unsigned char *dest);
unsigned char U8ToBcd2(unsigned char *buff, unsigned char *dest);
//--------------------------------------------------------------------------------
extern INT8U  Calendar_Calculate(DateTimeType * S_DateTime, DateTimeType *D_DateTime,INT8U Year,INT8U flag );
extern void Time_Convert_WildCard(INT8U *S_DateTime,INT8U *D_DateTime);
extern void StrToHex(INT8U *Str, INT8U *Hex, INT16U StrLen);
extern void HexToStr(INT8U *Str, INT8U *Hex, INT16U HexLen);
extern INT8U ConstCompareString(const INT8U *ps,INT8U *pr,INT8S len);
#endif

