/*******************************************************************************
*Copyright   : 2017  Wasion Group Ltd.
*FileName    : DU.h
*Description : 需量计量模块头文件
Modify records:
Author       Date               Version           Description
HeZhuli      2017.2.7           v1.0              Created
*******************************************************************************/
#ifndef  _DU_H
#define  _DU_H
/*"*******************************************************************************"*/
#include "system\Sys_def.h" 
#include "App\LegallyRelevant\LRM_Config.h"
#include "App\MtrParamDefault\MtrParamDef.h"
/*"*******************************************************************************"*/

/*无功组合特征字默认值*/
#define DEFAULT_NA_MODE_1_VALUE  0x41
#define DEFAULT_NA_MODE_2_VALUE  0x14

#define TYPE_DEMAND_CAL  INT64U    //INT32U
#define MULTIPLY_POWER   1         /*如果用5位小数，就用10*/

#define MULTIPLY_POWER_POINTER 100 /*-6转换成-4，所以除以100*/

//#define DEMAND_DATE_TIME     DateTimeType

#define DEMAND_DATE_TIME     ST_5TIME_FORM

#define DU_CUR_AVE_DM   0x01  /* 当前平均需量 */
#define DU_MAX_DM      	0x02  /* 最大需量 */
#define DU_TOTALMAX_DM  0x04  /* 累计最大需量 */

#define DU_LAST_DM      0x08  /* 上月累计或最大需量 */

#define DU_R_DEMAND_DM  0x10  /*无功需量*/

#define MAX_POWER       (380UL*120*3*1000)  /*最大规格的功率以380V，120A算*/

#if 0
#define Ap_TOTAL      0  /* 总有功需量 */
#define Qp_TOTAL      1  /* 总无功需量 */
#define Sp_TOTAL      2  /* 总视在需量 */
#define Ap_TOU    	   3  /* 有功分时需量 */
#define Qp_TOU    	   4  /* 无功分时需量 */
#define Sp_TOU   	   5  /* 视在分时需量 */
#endif

/*需量类型枚举定义*/
typedef enum
{
    Ap_P     = 0,  /* 总 正向有功 */
    Ap_N     = 1,  /* 总 反向有功 */ 
    Qp_I     = 2,  /* I象限无功 */
    Qp_II    = 3,  /* II象限无功 */
    Qp_III   = 4,  /* III象限无功 */
    Qp_IV    = 5,  /* IV象限无功 */
    /*海外需求*/
    #if 0
    Qp_P = 2,  /* 总 正向无功 */
    Qp_N = 3,  /* 总 反向无功 */
    Sp_P = 4,  /* 总 正向视在 */
    Sp_N = 5,  /* 总 反向视在*/
    #endif
    E_MAC_DEMAND_TYPE,
}ENUM_DEMAND_TYPE;


/*参数设置索引*/
#define INX_DEMANDPERIOD 			0/*需量周期*/
#define INX_NUMOFPERIOD				1/*需量周期数"*/

//#define Qp_I      6  /* QI 无功     */
//#define Qp_II     7  /* QII 无功    */
//#define Qp_III    8  /* QIII 无功   */
//#define Qp_IV     9  /* QIV 无功    */

#define DM_POSITIVE  0   /*正向*/
#define DM_REVERSE   1   /*反向*/

#define ERR_RELOADTIMES 2   /*读数据错误最多次数*/

#define DU_FALSE 		0
#define DU_TRUE 		1

#define DU_THROWN       	0   /*修改时间或上电第一分钟，不足一分钟丢弃"*/

/*满周期数出第一个需量标识*/
#define DU_FIRST_DM    	   (g_DU_Ram.start_flag.bit.b0)   

/*上电非整分标识*/
#define DU_FIRST1MIN       (g_DU_Ram.start_flag.bit.b1)   

/*"需量常量定义"*/
#define MAX_MD_PERIOD          (60)    /*"海外:需量周期.             国内:滑差时间  秒"*/
#define MAX_SLIP_NUM           60      /*"海外:需量周期数:最大滑差数 国内:需量周期/滑差时间 "*/
#define MAX_MD_NUM             6       /*"最大需量寄存器数: 正反有功，各象限无功, 组合无功1和2在抄读时特殊处理计算"*/

/*"平均需量结构体"*/
typedef struct    
{
  INT32U TempValue;            /*"临时平均值"*/
  INT32U CurrValue;            /*"当前平均值"*/
  INT32U LastValue;            /*"最终平均值"*/
  DEMAND_DATE_TIME CapTime;    /*"最终平均值捕获时间: 年月日时分秒."*/
  //DEMAND_DATE_TIME StarTime;   /*"最终平均值起始时间: 年月日时分秒."*/
} ST_AVDM_DEF;

/*"最大需量结构体"*/
typedef struct      
{
  INT32U           Value;          /*"最大需量值: 功率/电流等.  而32位数据也比较大，可以不用考虑溢出"*/
  DEMAND_DATE_TIME Time;           /*"最大需量捕获时间: 年月日时分秒."*/
} ST_MD_DEF;  /*4+5->四字节对齐12*/

/*"单个最大需量数据"*/
typedef struct    
{
    ST_MD_DEF   Data[MAX_MD_NUM];        /*"当月最大需量."*/
    CRC_DATA_TP Crc;                     /*"CRC校验"*/
} ST_MD_FROM; /*12*8+4=100*/

/*"当前最大需量数据"*/
typedef struct    
{
    ST_MD_FROM  Total;      /*"总最大需量数据"*/
    ST_MD_FROM  CurData;    /*"当前费率需量数据"*/
    CRC_DATA_TP Crc;  /*"CRC校验"*/
} ST_MD_DATA;

/*"所有最大需量数据: 总+1~8分时费率需量"*/
typedef struct    
{
    ST_MD_FROM  Data[TARIFF_NUM+1];      /*"最大需量数据"*/
    
    CRC_DATA_TP Crc;  /*"CRC校验"*/
} ST_MD_DATA_ALL;

/*"累计最大需量数据"*/
typedef struct    
{
    INT32U Value[MAX_MD_NUM];      /*"累计最大需量"*/
    
    CRC_DATA_TP Crc;               /*"CRC校验"*/
} ST_TLMD_DATA;


/*需量数据累计buff*/
typedef struct
{
	TYPE_DEMAND_CAL SkipTime_Power;		    /*单个滑差周期类的累计功率  64位 10的-6kw*/
	TYPE_DEMAND_CAL power_buf[MAX_SLIP_NUM];/*功率平均值*/
} st_DU_RamType;

/*需量累计状态结构体*/
typedef struct
{
    UN_BYTE_DEF   start_flag;           /*bit0:满周期数出第一个需量标识 bit1:上电非整分标识*/
    INT8U   Periods_Count;              /*滑差周期数计数*/
	INT8U   Resr;//Cur_TariffNo;               /*分时需量当前费率号,用方法代替*/
	INT8U   UpdatePermit;               /*分时切换或清0后时等待一个全周期后再出新的需量*/

    INT16U  SkipTime_Count;            /*滑差周期时间累计(秒)*/
	INT8U	last_min;                  /*上一次计算需量的时间分，与当前时间比较，不同时，则计算新的*/
    INT8U   UpdatePermit_R;            /*出无功需量倒计时，无功切换时，等待一个全周期后再出新需量*/ 
    
    st_DU_RamType Data[MAX_MD_NUM];    /*需量数据累计buff*/
} st_DU_Ram;/*共计210个字节*/

/*
时钟同步：出需量时间与系统时钟有关。当系统时间的分钟能被需量周期整除开始出需量。
分钟同步：出需量的时间与系统时间无关，在上电后累加时间到需量周期开始出需量。
比如：设置需量周期为15分钟，滑差为3分钟，09时02分30秒上电，如果按照时钟同步，则是在09时15分0秒开始出需量；如果按照分钟同步，则是在09时17分0秒开始出需量.
*/

/*需量参数结构体*/
typedef struct
{
  FTR_DATA_TP validFlg; 
  INT8U  NumOfPeriod;   /*"海外:需量周期数:最大滑差数 国内:需量周期除滑差时间 "*/
  INT8U  DemandTime;    /*"国内:需量周期:单位:分      之所以用这个，主要是为了好计算和存储，同时也是为了字节对齐"*/

  INT8U  DemandPeriod;  /*"海外:需量周期.             国内:滑差时间  秒  都改成分算了"*/
  INT8U  Resr;          /*备用，4字节对齐，预留*/
  INT8U  DemandComMode1;/*组合无功特征值1*/
  INT8U  DemandComMode2;/*组合无功特征值1*/
  //-----
  CRC_DATA_TP Crc;       
}ST_DU_PARA;   

/*需量参数偏移*/
#define DU_PARA_NUM_PERIOD_SHIFT     FIND(ST_DU_PARA, validFlg)
#define DU_PARA_DEMAND_TIME_SHIFT    FIND(ST_DU_PARA, DemandTime)
#define DU_PARA_DEMAND_PERIOD_SHIFT  FIND(ST_DU_PARA, DemandPeriod)
#define DU_PARA_DEMAND_MODE_SHIFT    FIND(ST_DU_PARA, DemandMode)
#define DU_PARA_CRC_SHIFT            FIND(ST_DU_PARA, Crc)


/*需量模块的文件定义*/
typedef struct
{
   //ST_MD_DATA     FILE_MDData;          /*"月最大需量数据 当月临时的，一直变化的，月固定的(已用于结算的，是直接存内卡的)"*/
   ST_MD_DATA_ALL FILE_MD_ALL_Data;     /*"月历史最大需量数据，已结算存储的"*/
   ST_TLMD_DATA   FILE_TotalDm;         /*"历史累计最大需量数据"*/
}ST_DU_FILE_DATA;    

#if 0
/*当月最大需量数据偏移*/
#define DU_MD_DATA_SHIFT   	    FIND(ST_DU_FILE_DATA, FILE_MDData)
#define DU_MD_TOTAL_DATA_SHIFT  (DU_MD_DATA_SHIFT+FIND(ST_MD_DATA, Total))
#define DU_MD_CUR_DATA_SHIFT    (DU_MD_DATA_SHIFT+FIND(ST_MD_DATA, CurData))
#define DU_MD_DATA_CRC_SHIFT    (DU_MD_DATA_SHIFT+FIND(ST_MD_DATA, Crc))
#endif

/*总及费率 需量数据偏移*/
#define DU_MD_DATA_ALL_SHIFT               FIND(ST_DU_FILE_DATA, FILE_MD_ALL_Data)
#define DU_MD_DATA_ALL_DATA_SUM_SHIFT      (DU_MD_DATA_ALL_SHIFT+FIND(ST_MD_DATA_ALL, Data))
#define DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(i) (DU_MD_DATA_ALL_SHIFT+i*sizeof(ST_MD_FROM))
#define DU_MD_DATA_ALL_CRC_SHIFT           (DU_MD_DATA_ALL_SHIFT+FIND(ST_MD_DATA_ALL, Crc))


/*历史累计最大需量数据*/
#define DU_TOTAL_DM_SHIFT   	FIND(ST_DU_FILE_DATA, FILE_TotalDm)
#define DU_TOTAL_DM_VALUE_SHIFT (DU_TOTAL_DM_SHIFT+FIND(ST_TLMD_DATA, Value))
#define DU_TOTAL_DM_CRC_SHIFT   (DU_TOTAL_DM_SHIFT+FIND(ST_TLMD_DATA, Crc))

/*"*******************************************************************************"*/
extern st_DU_Ram  g_DU_Ram;
extern ST_DU_PARA g_DU_Para;   /*需量参数*/
extern INT32S     Current_Demand[2];
extern NO_INIT ST_AVDM_DEF  stAvDmData[MAX_MD_NUM];
/******************************************************************************/
extern void DU_Init(INT8U mode);
extern void DU_Main(void);
extern void DU_CurAveValue(INT8U flag);

extern INT8S DU_Clr(void *pdin, void *pdout);
extern INT8S DU_MaxDemandClr(void *pdin, void *pdout);
extern INT8S DU_CurrentDemandClr(void *pdin, void *pdout);
extern INT8S DU_R_DemandClr(void *pdin, void *pdout);
extern INT8S DU_Update_R_com_type(void *pdin, void *pdout);
extern INT8S DU_Tariff_Switch(void *pdin, void *pdout);

extern void  DU_Change_Date_Time_Com(INT8U *pSrcDateTime,INT8U *pDstDateTime);
/*"*******************************************************************************"*/

#endif

