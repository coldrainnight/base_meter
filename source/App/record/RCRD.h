#ifndef _H_RCRD
#define _H_RCRD

#include "PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "app\userinterface\PP\PP_698\Process\Pp_macro.h"
#include "App\MtrParamDefault\MtrParamDef.h"

#define     RCRD_BUF_SIZE  64
//extern INT8U RCRDDataBuf[RCRD_BUF_SIZE];



#if defined (SINGLE_PHASE)
#define  INDEX_LIST_DEEP    (732)    /*"分钟冻结索引表深度"*/
#define  MAX_DOT_NUM_PER_STAMP   (720)  /*"单个时间戳下允许记录的最大点数"*/
#define LD_CRC_SZ       0 /*" sizeof(CRC_DATA_TP) "*/
#define  MIN_FRZ_CLASS_NUM    1    /*"分钟冻结最大方案数"*/
#else
#define  INDEX_LIST_DEEP    (360)    /*"分钟冻结索引表深度 每个索引64个点 "*/
#define  MAX_DOT_NUM_PER_STAMP   (90)  /*"单个时间戳下允许记录的最大点数"*/

//#define IDX_IN_FLASH    /*" 时间戳索引存放在flash中宏定义  "*/
#define LD_CRC_SZ       2 /*" sizeof(CRC_DATA_TP) "*/
#define  MIN_FRZ_CLASS_NUM    6    /*"分钟冻结最大方案数"*/
#endif

#define LD_DATA_IN_FLASH    /*" 数据区存放在flash中宏定义  "*/


#ifndef LD_DATA_IN_FLASH
typedef  INT16U   RECORD_ADDR_TYPE;/*"记录数据存储地址数据类型定义  eeprom地址INT16U  FLASH地址  INT32U"*/
#else
typedef  INT32U   RECORD_ADDR_TYPE;/*"记录数据存储地址数据类型定义  eeprom地址INT16U  FLASH地址  INT32U"*/
#endif



#define NUM_LD_CTRL_SPACE      6
#if(INDEX_LIST_DEEP < 256)
typedef  INT8U   INDEX_RANGE_TYPE;
#else
typedef  INT16U   INDEX_RANGE_TYPE;
#endif


/*"方案索引属性定义"*/
typedef struct
{
    INT32U   TimeStamp; /*"分钟冻结方案索引时间戳  相对2000年1月1日0点0分的分钟数"*/
    INT32U   startDotId;/*"时间戳对应记录数据起始记录点序号 "*/
}ST_SCHEME_INDEX_ATTR;

/*"分钟冻结索引表定义"*/
typedef struct
{
    ST_SCHEME_INDEX_ATTR minFrzIndexOadList[MIN_FRZ_CLASS_NUM][INDEX_LIST_DEEP];
}ST_LOAD_RCD_IDX;


/*"分钟冻结当前记录控制信息"*/
typedef struct
{
    INT32U dotCnt;  /*"记录点数累计值"*/
    INT32U real_dotCnt;  /*"实际记录点数累计值"*/
    INDEX_RANGE_TYPE  startIndexId;  /*"检索范围起始索引编号"*/
    INDEX_RANGE_TYPE  endIndexId;    /*"检索范围结束索引编号"*/
    INT16U  NowStampDotCnt;/*"当前时间戳下冻结点数累计值"*/
    CRC_DATA_TP crcValue;
}ST_LR_CTR_DATA_FORM;

typedef struct
{
    ST_LR_CTR_DATA_FORM    LrCtrlData[MIN_FRZ_CLASS_NUM][NUM_LD_CTRL_SPACE];
}ST_LR_CTR_DATA;

#define  MAX_LINE_RSULT_NUM   96

/*"记录查询结果信息结果定义"*/
typedef struct
{
    INT8U  kind;    /*"筛选方法  1/2/9 方法2按间隔筛选 定义为3"*/
    INT8U  Channel; /*"筛选通道"*/
    INT8U  stampNum;/*"筛选结果分布在多少个索引点内"*/
    INT8U num; /*" 找到的记录条数  "*/
    INT32U  RcrdId; /*"起始点序号"*/
    INT32U  seq_no[MAX_LINE_RSULT_NUM];
}ST_RCRD_LINE_RSULT_DATA;



typedef enum
{
    INVL_SEC = 0,
    INVL_MIN,
    INVL_HOUR,
    INVL_DAY,
    INVL_MONTH,
    INVL_YEAR,
    INVL_NULL
}EN_INTERVAL;

#ifdef THREE_PHASE
#define    MAX_DATA_BUFF_LEN  (1024L) /*"记录缓存空间大小，当前阶梯表编程266 + 预留50字节"*/
#define    MAX_LEN  200            /*"单项数据缓存空间大小，用于保存数据转换前数据"*/
#define    MAX_RCRD_ROAD_NUM       (52) /*"事件冻结关联对象列表最大成员数"*/
#else
#define    MAX_DATA_BUFF_LEN  (320L) /*"记录缓存空间大小，当前阶梯表编程266 + 预留50字节"*/
#define    MAX_LEN  200            /*"单项数据缓存空间大小，用于保存数据转换前数据"*/
#define    MAX_RCRD_ROAD_NUM       (16) /*"事件冻结关联对象列表最大成员数"*/
#endif

#define FLASH_SECTOR_SZ   4096L

/*" 事件最大关联对象个数配置 "*/
#if defined (THREE_PHASE)
#define MAX_OAD_LOST_VOLT 50
#define MAX_OAD_LACK_VOLT 50
#define MAX_OAD_OVER_VOLT 50
#define MAX_OAD_BRK_PHASE 50

#define MAX_OAD_LOST_CURR 49
#define MAX_OAD_OVER_CURR 49
#define MAX_OAD_BRK_CURR 49

#define MAX_OAD_PWR_REVERSE 42
#define MAX_OAD_OVER_LOAD 42

#define MAX_OAD_OVER_PA_DEMAND 6
#define MAX_OAD_OVER_NA_DEMAND 6
#define MAX_OAD_OVER_R_DEMAND 6

#define MAX_OAD_FACTOR_LOW 13
#define MAX_OAD_ALL_LOST_VOLT 6
//#define MAX_OAD_AUX_PWR_DOWN 6

#define MAX_OAD_VOLT_INVERSE 42
#define MAX_OAD_CURR_INVERSE 42

#define MAX_OAD_PWR_DOWN 6
#define MAX_OAD_PRG_EVNT 6
#define MAX_OAD_MTR_CLR 32
#define MAX_OAD_DEMAND_CLR 29
#define MAX_OAD_EVNT_CLR 6

#define MAX_OAD_JUST_TIME 7
#define MAX_OAD_DAY_TBL_PRG 7
#define MAX_OAD_SEASON_TBL_PRG 7

#define MAX_OAD_WEEKEND_PRG_EVNT 6
#define MAX_OAD_BILL_DAY_PRG 6

#define MAX_OAD_MTR_COVER_OPEN 16
#define MAX_OAD_TAIL_COVER_OPEN 16

#define MAX_OAD_VOLT_IMBALANCE 43
#define MAX_OAD_CURR_IMBALANCE 43

#define MAX_OAD_RELAY_TRIP 16
#define MAX_OAD_RELAY_CLOSE 16

#define MAX_OAD_HOLIDAY_PRG 6
#define MAX_OAD_A_ENGY_COM 6
#define MAX_OAD_R_ENGY_COM 7
#define MAX_OAD_TRIFF_PRG 7
#define MAX_OAD_STEP_PRG 7
#define MAX_OAD_KEY_UPDATE 5
#define MAX_OAD_CARD_IN_ERR 6
#define MAX_OAD_BUY_ENGY 10
#define MAX_OAD_REFUND_EVNT 6
#define MAX_OAD_CONSTANT_MGNT_DISTURB 9
#define MAX_OAD_RELAY_ERR 10
#define MAX_OAD_PWR_FAILURE 7
#define MAX_OAD_CURR_IMBALANCE_BADLY 43
#define MAX_OAD_ENG_ERR 7
#define MAX_OAD_RTC_ERR 2   /*" 时钟故障 "*/
#define MAX_OAD_MS_ERR 2    /*" 计量芯片故障 "*/
#define MAX_OAD_MCU_RST 2    /*" MCU RST "*/

/* 冻结类最大关联对象个数配置*/
#define MAX_RELE_OO     30


#define MAX_OAD_MMT_FRZ   18
#define MAX_OAD_MIN_FRZ   26
#define MAX_OAD_HOUR_FRZ   5
#define MAX_OAD_DAY_FRZ   24
#define MAX_OAD_MOUTH_BILL_FRZ   18
#define MAX_OAD_MOUTH_FRZ   30
#define MAX_OAD_SEASON_TBL_SW_FRZ   16
#define MAX_OAD_DAY_TBL_SW_FRZ   16
#define MAX_OAD_TRIFF_TBL_SW_FRZ   16
#define MAX_OAD_STEP_TBL_SW_FRZ   16  /*" 阶梯切换冻结 "*/
#define MAX_OAD_STEP_BILL_FRZ   4 /*" 阶梯结算冻结 "*/
#elif defined (SINGLE_PHASE)
//#define MAX_OAD_LOST_VOLT 50
//#define MAX_OAD_LACK_VOLT 50
//#define MAX_OAD_OVER_VOLT 50
//#define MAX_OAD_BRK_PHASE 50

//#define MAX_OAD_LOST_CURR 49
#define MAX_OAD_OVER_CURR 10
//#define MAX_OAD_BRK_CURR 49

//#define MAX_OAD_PWR_REVERSE 42
//#define MAX_OAD_OVER_LOAD 42

#define MAX_OAD_OVER_PA_DEMAND 6
#define MAX_OAD_OVER_NA_DEMAND 6
#define MAX_OAD_OVER_R_DEMAND 6

//#define MAX_OAD_FACTOR_LOW 13
#define MAX_OAD_ALL_LOST_VOLT 6
//#define MAX_OAD_AUX_PWR_DOWN 6

//#define MAX_OAD_VOLT_INVERSE 42
//#define MAX_OAD_CURR_INVERSE 42

#define MAX_OAD_PWR_DOWN 2
#define MAX_OAD_PRG_EVNT 2
#define MAX_OAD_MTR_CLR 4
//#define MAX_OAD_DEMAND_CLR 29
#define MAX_OAD_EVNT_CLR 2

#define MAX_OAD_JUST_TIME 4
#define MAX_OAD_DAY_TBL_PRG 4
#define MAX_OAD_SEASON_TBL_PRG 3

#define MAX_OAD_WEEKEND_PRG_EVNT 3
#define MAX_OAD_BILL_DAY_PRG 3

#define MAX_OAD_MTR_COVER_OPEN 6
//#define MAX_OAD_TAIL_COVER_OPEN 16

//#define MAX_OAD_VOLT_IMBALANCE 43
//#define MAX_OAD_CURR_IMBALANCE 43

#define MAX_OAD_RELAY_TRIP 4
#define MAX_OAD_RELAY_CLOSE 4

#define MAX_OAD_HOLIDAY_PRG 2
#define MAX_OAD_A_ENGY_COM 3
#define MAX_OAD_R_ENGY_COM 7
#define MAX_OAD_TRIFF_PRG 4
#define MAX_OAD_STEP_PRG 4
#define MAX_OAD_KEY_UPDATE 3
#define MAX_OAD_CARD_IN_ERR 2
#define MAX_OAD_BUY_ENGY 8
#define MAX_OAD_REFUND_EVNT 2
#define MAX_OAD_CONSTANT_MGNT_DISTURB 9
#define MAX_OAD_RELAY_ERR 7
#define MAX_OAD_PWR_FAILURE 4
//#define MAX_OAD_CURR_IMBALANCE_BADLY 43
#define MAX_OAD_RTC_ERR 2
#define MAX_OAD_MS_ERR 2    /*" 计量芯片故障 "*/
#define MAX_OAD_MCU_RST 2    /*" MCU RST "*/

/* 冻结类最大关联对象个数配置*/
#define MAX_RELE_OO     30


#define MAX_OAD_MMT_FRZ   5
#define MAX_OAD_MIN_FRZ   8
#define MAX_OAD_HOUR_FRZ   3
#define MAX_OAD_DAY_FRZ   7
#define MAX_OAD_MOUTH_BILL_FRZ   6
#define MAX_OAD_MOUTH_FRZ   5
#define MAX_OAD_SEASON_TBL_SW_FRZ   5
#define MAX_OAD_DAY_TBL_SW_FRZ   5
#define MAX_OAD_TRIFF_TBL_SW_FRZ   5
#define MAX_OAD_STEP_TBL_SW_FRZ   5  /*" 阶梯切换冻结 "*/
#define MAX_OAD_STEP_BILL_FRZ   3 /*" 阶梯结算冻结 "*/


#endif

#if defined(THREE_PHASE)
#define MAX_EVT_RCRD    75  /*" EEE 增加或删除一个事件要修改 "*/
#else
#define MAX_EVT_RCRD    15  /*" EEE 增加或删除一个事件要修改 "*/
#endif
#define MAX_FRZ_RCRD    11
#define MAX_ALL_RCRD    (MAX_EVT_RCRD + 1 + MAX_FRZ_RCRD)

#define    NUM_MMT_FRZ              (3)    /*" 瞬时冻结 "*/
#if defined(THREE_PHASE)
#define    NUM_MIN_FRZ              (28800)  /*分钟冻结次数*/
#else
#define    NUM_MIN_FRZ              (288)  /*分钟冻结次数*/
#endif
#define    NUM_HOUR_FRZ           (254)  /*" 小时冻结 "*/
#define    NUM_DAY_FRZ              (62)   /*" 日冻结 "*/
#define    NUM_ENGY_BILL            (12)   /*" 月结算 "*/
#define    NUM_MON_FRZ              (12)   /*" 月冻结 "*/
#define    NUM_YSLOT_SW_FRZ         (2)    /*" 两套时区表切换冻结 "*/
#define    NUM_DSLOT_SW_FRZ         (2)    /*" 两套日时段切换冻结 "*/
#define    NUM_TARIFF_SW_FRZ        (2)    /*" 费率电价切换冻结 "*/
#define    NUM_STEP_SW_FRZ          (2)    /*" 阶梯电价切换冻结 "*/
#define    NUM_YEAR_BILL            (4)   /*"阶梯结算冻结"*/

#if(MAX_OAD_MIN_FRZ < 9)
typedef  INT8U   RECORD_MASK_TYPE;
#else
typedef  INT32U   RECORD_MASK_TYPE;
#endif

#if defined (SINGLE_PHASE)

#define OVER_CURR_LA_SZ    (2*FLASH_SECTOR_SZ)
#define PWR_DOWN_SZ     (2*FLASH_SECTOR_SZ)   
#define PRG_SZ          (2*FLASH_SECTOR_SZ)
#define MTR_CLR_SZ      (2*FLASH_SECTOR_SZ)   

#define EVNT_CLR_SZ     (2*FLASH_SECTOR_SZ)   
#define JUST_TIME_SZ    (2*FLASH_SECTOR_SZ)
#define BROADCAST_JUST_TIME_SZ  (3*FLASH_SECTOR_SZ)
#define DAY_TBL_PRG_SZ     (2*FLASH_SECTOR_SZ)
#define YEAR_TBL_PRG_SZ  (2*FLASH_SECTOR_SZ)
#define WEEKEND_PRG_SZ     (2*FLASH_SECTOR_SZ)
#define BILL_DAY_PRG_SZ    (2*FLASH_SECTOR_SZ)

#define MTR_COVER_OPEN_SZ  (2*FLASH_SECTOR_SZ)

#define RELAY_TRIP_SZ      (2*FLASH_SECTOR_SZ)
#define RELAY_CLOSE_SZ     (2*FLASH_SECTOR_SZ)
#define HOLIDAY_PRG_SZ     (2*FLASH_SECTOR_SZ)
#define OSCILLATOR_ERR_EVT_SZ     (2*FLASH_SECTOR_SZ)
#define SYS_RST_EVNT_SZ     (2*FLASH_SECTOR_SZ)
#define A_ENGY_COM_PRG_SZ  (2*FLASH_SECTOR_SZ)

#define TRIFF_PRG_SZ       (2*FLASH_SECTOR_SZ)
#define STEP_PRG_SZ        (2*FLASH_SECTOR_SZ)
#define KEY_UPDATE_SZ      (2*FLASH_SECTOR_SZ)
#define CARD_PLUG_IN_ERR_SZ    (2*FLASH_SECTOR_SZ)
#define BUY_ENGY_SZ        (2*FLASH_SECTOR_SZ)
#define REFUND_SZ          (2*FLASH_SECTOR_SZ)

#define RELAY_ERR_SZ       (2*FLASH_SECTOR_SZ)
#define PWR_FAILURE_SZ     (2*FLASH_SECTOR_SZ)
#define RTC_ERR_SZ         (2*FLASH_SECTOR_SZ)
#define MS_ERR_SZ          (2*FLASH_SECTOR_SZ)
#define MGMT_COMM_ERR_SZ		   (2*FLASH_SECTOR_SZ)
#define UPGRADE_SZ		   (2*FLASH_SECTOR_SZ)
#define NOLEG_REPALCE_SZ   (2*FLASH_SECTOR_SZ)

#define MMT_FRZ_SZ  (2*FLASH_SECTOR_SZ)
#define MIN_FRZ_IDX_SZ  (sizeof(ST_LOAD_RCD_IDX))  /*" 分钟冻结索引大小 (存内卡)"*/
#define MIN_FRZ_SZ  (6350*FLASH_SECTOR_SZ)
#define HOUR_FRZ_SZ (2*FLASH_SECTOR_SZ)
#define DAY_FRZ_SZ  (2*FLASH_SECTOR_SZ)
#define MONTH_BILL_SZ   (2*FLASH_SECTOR_SZ)
#define MONTH_FRZ_SZ        (2*FLASH_SECTOR_SZ)
#define YEAR_TBL_SW_FRZ_SZ    (2*FLASH_SECTOR_SZ)
#define DAY_TBL_SW_FRZ_SZ       (2*FLASH_SECTOR_SZ)
#define TRIFF_SW_FRZ_SZ     (2*FLASH_SECTOR_SZ)
#define STEP_SW_FRZ_SZ      (2*FLASH_SECTOR_SZ) /*" 阶梯切换冻结 "*/
#define STEP_BILL_FRZ_SZ        (2*FLASH_SECTOR_SZ) /*" 阶梯结算冻结 "*/

#define PD_PON_EVT_SZ           (2*FLASH_SECTOR_SZ)
#define MSR_DATAL_SIZE			(5*FLASH_SECTOR_SZ)
#define MSR_DATAN_SIZE			(5*FLASH_SECTOR_SZ)
/*" 数据文件定义 "*/
#elif defined (THREE_PHASE) 
#define PD_PON_EVT_SZ           (2*FLASH_SECTOR_SZ)
#define PARA_ERR_EVT_SZ           (2*FLASH_SECTOR_SZ)
#define ENG_ERR_EVT_SZ           (3*FLASH_SECTOR_SZ)

#define LOST_VOLT_LA_SZ      (3*FLASH_SECTOR_SZ)
#define LOST_VOLT_LB_SZ      (3*FLASH_SECTOR_SZ)
#define LOST_VOLT_LC_SZ      (3*FLASH_SECTOR_SZ)
    
#define LACK_VOLT_LA_SZ      (3*FLASH_SECTOR_SZ)
#define LACK_VOLT_LB_SZ      (3*FLASH_SECTOR_SZ)
#define LACK_VOLT_LC_SZ      (3*FLASH_SECTOR_SZ)
    
#define OVER_VOLT_LA_SZ    (3*FLASH_SECTOR_SZ)
#define OVER_VOLT_LB_SZ    (3*FLASH_SECTOR_SZ)
#define OVER_VOLT_LC_SZ    (3*FLASH_SECTOR_SZ)
    
#define BRK_PHASE_LA_SZ    (3*FLASH_SECTOR_SZ)
#define BRK_PHASE_LB_SZ    (3*FLASH_SECTOR_SZ)
#define BRK_PHASE_LC_SZ    (3*FLASH_SECTOR_SZ)

#define LOST_CURR_LA_SZ    (3*FLASH_SECTOR_SZ)
#define LOST_CURR_LB_SZ    (3*FLASH_SECTOR_SZ)
#define LOST_CURR_LC_SZ    (3*FLASH_SECTOR_SZ)
    
#define OVER_CURR_LA_SZ    (3*FLASH_SECTOR_SZ)
#define OVER_CURR_LB_SZ    (3*FLASH_SECTOR_SZ)
#define OVER_CURR_LC_SZ    (3*FLASH_SECTOR_SZ)
    
#define BRK_CURR_LA_SZ     (3*FLASH_SECTOR_SZ)
#define BRK_CURR_LB_SZ     (3*FLASH_SECTOR_SZ)
#define BRK_CURR_LC_SZ     (3*FLASH_SECTOR_SZ)

#define PWR_REVERSE_SZ         (3*FLASH_SECTOR_SZ)
#define PWR_REVERSE_LA_SZ      (3*FLASH_SECTOR_SZ)
#define PWR_REVERSE_LB_SZ      (3*FLASH_SECTOR_SZ)
#define PWR_REVERSE_LC_SZ      (3*FLASH_SECTOR_SZ)

#define OVER_LOAD_LA_SZ        (3*FLASH_SECTOR_SZ)
#define OVER_LOAD_LB_SZ        (3*FLASH_SECTOR_SZ)
#define OVER_LOAD_LC_SZ        (3*FLASH_SECTOR_SZ)

#define OVER_PA_DEMAND_SZ      (3*FLASH_SECTOR_SZ)
#define OVER_NA_DEMAND_SZ      (3*FLASH_SECTOR_SZ)
    
#define OVER_R_DEMAND_I_SZ     (3*FLASH_SECTOR_SZ)
#define OVER_R_DEMAND_II_SZ    (3*FLASH_SECTOR_SZ)
#define OVER_R_DEMAND_III_SZ   (3*FLASH_SECTOR_SZ)
#define OVER_R_DEMAND_IV_SZ    (3*FLASH_SECTOR_SZ)
    
#define FACTOR_LOW_SZ      (3*FLASH_SECTOR_SZ)
#define ALL_LOST_VOLT_SZ   (3*FLASH_SECTOR_SZ)
#define AUX_PWR_DOWN_SZ    (3*FLASH_SECTOR_SZ) 
#define VOLT_INVERSE_SZ    (3*FLASH_SECTOR_SZ)
#define CURR_INVERSE_SZ    (3*FLASH_SECTOR_SZ)

#define PWR_DOWN_SZ        (2*FLASH_SECTOR_SZ)
#define PRG_SZ             (94*10)  /*" 编程记录 存内卡 "*/
#define MTR_CLR_SZ         (2*FLASH_SECTOR_SZ)

#define DEMAND_CLR_SZ      (2*FLASH_SECTOR_SZ)
#define EVNT_CLR_SZ        (2*FLASH_SECTOR_SZ)
#define JUST_TIME_SZ       (2*FLASH_SECTOR_SZ)
#define DAY_TBL_PRG_SZ     (2*FLASH_SECTOR_SZ)
#define YEAR_TBL_PRG_SZ  (2*FLASH_SECTOR_SZ)
#define WEEKEND_PRG_SZ     (2*FLASH_SECTOR_SZ)
#define BILL_DAY_PRG_SZ    (2*FLASH_SECTOR_SZ)

#define MTR_COVER_OPEN_SZ  (2*FLASH_SECTOR_SZ)

#define TAIL_COVER_OPEN_SZ (2*FLASH_SECTOR_SZ)

#define VOLT_IMBALANCE_SZ  (2*FLASH_SECTOR_SZ)
#define CURR_IMBALANCE_SZ  (2*FLASH_SECTOR_SZ)

#define RELAY_TRIP_SZ      (2*FLASH_SECTOR_SZ)
#define RELAY_CLOSE_SZ     (2*FLASH_SECTOR_SZ)
#define HOLIDAY_PRG_SZ     (2*FLASH_SECTOR_SZ)
#define OSCILLATOR_ERR_EVT_SZ     (7*10)
#define SYS_RST_EVNT_SZ     (12*10)
#define A_ENGY_COM_PRG_SZ  (2*FLASH_SECTOR_SZ)
    
#define R_ENGY_COM_PRG_SZ  (2*FLASH_SECTOR_SZ)

#define TRIFF_PRG_SZ               (2*FLASH_SECTOR_SZ)
#define STEP_PRG_SZ                (2*FLASH_SECTOR_SZ)
#define KEY_UPDATE_SZ              (2*FLASH_SECTOR_SZ)
#define CARD_PLUG_IN_ERR_SZ        (2*FLASH_SECTOR_SZ)
#define BUY_ENGY_SZ                (2*FLASH_SECTOR_SZ)
#define REFUND_SZ                  (2*FLASH_SECTOR_SZ)

#define CONSTANT_MGNT_DISTURB_SZ   (2*FLASH_SECTOR_SZ)

#define RELAY_ERR_SZ               (2*FLASH_SECTOR_SZ)
#define PWR_FAILURE_SZ             (2*FLASH_SECTOR_SZ)

#define CURR_IMBALANCE_BADLY_SZ    (2*FLASH_SECTOR_SZ)
#define RTC_ERR_SZ                 (2*FLASH_SECTOR_SZ)
#define MS_ERR_SZ                  (2*FLASH_SECTOR_SZ)
/*" 冻结文件 "*/


#define MMT_FRZ_SZ              (2*FLASH_SECTOR_SZ)
#define MIN_FRZ_IDX_SZ  (sizeof(ST_LOAD_RCD_IDX))  /*" 分钟冻结索引大小 (存内卡)"*/
#define MIN_FRZ_SZ              (1180*FLASH_SECTOR_SZ)
#define HOUR_FRZ_SZ                (7*FLASH_SECTOR_SZ)   /*" 小时冻结 存flash "*/
#define DAY_FRZ_SZ              (15*FLASH_SECTOR_SZ)
#define MONTH_BILL_SZ          (12*1000L) /*" 月结算冻结 存内卡 "*/    
#define MONTH_FRZ_SZ            (4*FLASH_SECTOR_SZ)
#define YEAR_TBL_SW_FRZ_SZ    (2*FLASH_SECTOR_SZ)    
#define DAY_TBL_SW_FRZ_SZ       (2*FLASH_SECTOR_SZ)
#define TRIFF_SW_FRZ_SZ     (2*FLASH_SECTOR_SZ)
#define STEP_SW_FRZ_SZ      (2*FLASH_SECTOR_SZ) /*" 阶梯切换冻结 "*/
#define STEP_BILL_FRZ_SZ        (2*FLASH_SECTOR_SZ) /*" 阶梯结算冻结 "*/

#endif

/*" 此数据结构每OI一个 "*/
typedef struct
{
#if defined (THREE_PHASE)
    INT8U lost_volt_obj_list[MAX_OAD_LOST_VOLT][4];
    INT8U lack_volt_obj_list[MAX_OAD_LACK_VOLT][4];
    INT8U over_volt_obj_list[MAX_OAD_OVER_VOLT][4];
    INT8U brk_phase_obj_list[MAX_OAD_BRK_PHASE][4];
    INT8U lost_curr_obj_list[MAX_OAD_LOST_CURR][4];
    INT8U over_curr_obj_list[MAX_OAD_OVER_CURR][4];
    INT8U brk_curr_obj_list[MAX_OAD_BRK_CURR][4];
    INT8U pwr_reverse_obj_list[MAX_OAD_PWR_REVERSE][4];
    INT8U over_load_obj_list[MAX_OAD_OVER_LOAD][4];
    INT8U over_pa_dmd_obj_list[MAX_OAD_OVER_PA_DEMAND][4];
    INT8U over_na_dmd_obj_list[MAX_OAD_OVER_NA_DEMAND][4];
    INT8U over_r_dmd_obj_list[MAX_OAD_OVER_R_DEMAND][4];
    INT8U factor_low_obj_list[MAX_OAD_FACTOR_LOW][4];
    INT8U all_lost_volt_obj_list[MAX_OAD_ALL_LOST_VOLT][4];
    //INT8U aux_pwr_down_obj_list[MAX_OAD_AUX_PWR_DOWN][4];
    INT8U volt_inverse_obj_list[MAX_OAD_VOLT_INVERSE][4];
    INT8U curr_inverse_obj_list[MAX_OAD_CURR_INVERSE][4];

    INT8U pwr_down_obj_list[MAX_OAD_PWR_DOWN][4];
    INT8U prg_evt_obj_list[MAX_OAD_PRG_EVNT][4];
    INT8U meter_clr_obj_list[MAX_OAD_MTR_CLR][4];

    INT8U dmd_clr_obj_list[MAX_OAD_DEMAND_CLR][4];
    
    INT8U evt_clr_obj_list[MAX_OAD_EVNT_CLR][4];
    INT8U just_tm_obj_list[MAX_OAD_JUST_TIME][4];
    INT8U day_tbl_prg_obj_list[MAX_OAD_DAY_TBL_PRG][4];
    INT8U season_tbl_prg_obj_list[MAX_OAD_SEASON_TBL_PRG][4];
    INT8U weekend_prg_obj_list[MAX_OAD_WEEKEND_PRG_EVNT][4];
    INT8U bill_day_prg_obj_list[MAX_OAD_BILL_DAY_PRG][4];
    INT8U meter_cover_open_obj_list[MAX_OAD_MTR_COVER_OPEN][4];

    INT8U tail_cover_open_obj_list[MAX_OAD_TAIL_COVER_OPEN][4];

    INT8U volt_imbalance_obj_list[MAX_OAD_VOLT_IMBALANCE][4];
    INT8U curr_imbalance_obj_list[MAX_OAD_CURR_IMBALANCE][4];

    INT8U rly_trip_obj_list[MAX_OAD_RELAY_TRIP][4];
    INT8U rly_close_obj_list[MAX_OAD_RELAY_CLOSE][4];
    INT8U holiday_prg_obj_list[MAX_OAD_HOLIDAY_PRG][4];
    INT8U a_eng_prg_obj_list[MAX_OAD_A_ENGY_COM][4];

    INT8U r_eng_prg_obj_list[MAX_OAD_R_ENGY_COM][4];

    INT8U triff_prg_obj_list[MAX_OAD_TRIFF_PRG][4];
    INT8U step_prg_obj_list[MAX_OAD_STEP_PRG][4];
    INT8U key_upd_obj_list[MAX_OAD_KEY_UPDATE][4];
    INT8U card_in_err_obj_list[MAX_OAD_CARD_IN_ERR][4];
    INT8U buy_evt_obj_list[MAX_OAD_BUY_ENGY][4];
    INT8U refund_evt_obj_list[MAX_OAD_REFUND_EVNT][4];

    INT8U constant_mgmt_obj_list[MAX_OAD_CONSTANT_MGNT_DISTURB][4];

    INT8U rly_err_obj_list[MAX_OAD_RELAY_ERR][4];
    INT8U pwr_fail_obj_list[MAX_OAD_PWR_FAILURE][4];

    INT8U curr_imbalance_badly_obj_list[MAX_OAD_CURR_IMBALANCE_BADLY][4];
    INT8U eng_err_obj_list[MAX_OAD_ENG_ERR][4];
    INT8U rtc_err_obj_list[MAX_OAD_RTC_ERR][4];
    INT8U ms_err_obj_list[MAX_OAD_MS_ERR][4];
#else
    INT8U over_curr_obj_list[MAX_OAD_OVER_CURR][4];

    INT8U pwr_down_obj_list[MAX_OAD_PWR_DOWN][4];
    INT8U prg_evt_obj_list[MAX_OAD_PRG_EVNT][4];
    INT8U meter_clr_obj_list[MAX_OAD_MTR_CLR][4];

    
    INT8U evt_clr_obj_list[MAX_OAD_EVNT_CLR][4];
    INT8U just_tm_obj_list[MAX_OAD_JUST_TIME][4];
    INT8U day_tbl_prg_obj_list[MAX_OAD_DAY_TBL_PRG][4];
    INT8U season_tbl_prg_obj_list[MAX_OAD_SEASON_TBL_PRG][4];
    INT8U weekend_prg_obj_list[MAX_OAD_WEEKEND_PRG_EVNT][4];
    INT8U bill_day_prg_obj_list[MAX_OAD_BILL_DAY_PRG][4];
    INT8U meter_cover_open_obj_list[MAX_OAD_MTR_COVER_OPEN][4];

    INT8U rly_trip_obj_list[MAX_OAD_RELAY_TRIP][4];
    INT8U rly_close_obj_list[MAX_OAD_RELAY_CLOSE][4];
    INT8U holiday_prg_obj_list[MAX_OAD_HOLIDAY_PRG][4];
    INT8U a_eng_prg_obj_list[MAX_OAD_A_ENGY_COM][4];


    INT8U triff_prg_obj_list[MAX_OAD_TRIFF_PRG][4];
    INT8U step_prg_obj_list[MAX_OAD_STEP_PRG][4];
    INT8U key_upd_obj_list[MAX_OAD_KEY_UPDATE][4];
    INT8U card_in_err_obj_list[MAX_OAD_CARD_IN_ERR][4];
    INT8U buy_evt_obj_list[MAX_OAD_BUY_ENGY][4];
    INT8U refund_evt_obj_list[MAX_OAD_REFUND_EVNT][4];


    INT8U rly_err_obj_list[MAX_OAD_RELAY_ERR][4];
    INT8U pwr_fail_obj_list[MAX_OAD_PWR_FAILURE][4];
    INT8U rtc_err_obj_list[MAX_OAD_RTC_ERR][4];
    INT8U ms_err_obj_list[MAX_OAD_MS_ERR][4];
#endif
    INT8U mcu_rst_obj_list[MAX_OAD_MCU_RST][4];

    INT8U mmt_frz_obj_list[MAX_OAD_MMT_FRZ][8];
    INT8U min_frz_obj_list[MAX_OAD_MIN_FRZ][8];
    INT8U hour_frz_obj_list[MAX_OAD_HOUR_FRZ][8];
    INT8U day_frz_obj_list[MAX_OAD_DAY_FRZ][8];
    INT8U mouth_bill_frz_obj_list[MAX_OAD_MOUTH_BILL_FRZ][8];
    INT8U mouth_frz_obj_list[MAX_OAD_MOUTH_FRZ][8];
    INT8U season_tbl_sw_obj_list[MAX_OAD_SEASON_TBL_SW_FRZ][8];
    INT8U day_tbl_sw_obj_list[MAX_OAD_DAY_TBL_SW_FRZ][8];
    INT8U triff_tbl_sw_obj_list[MAX_OAD_TRIFF_TBL_SW_FRZ][8];
    INT8U step_tbl_sw_obj_list[MAX_OAD_STEP_TBL_SW_FRZ][8];
    INT8U step_bill_frz_obj_list[MAX_OAD_STEP_BILL_FRZ][8];
}ST_EVT_FRZ_OBJ;

typedef struct
{
    INT8U day_frz_flg;
    ST_6TIME_FORM lst_day_frz;
    INT8U mouth_frz_flg;
    ST_6TIME_FORM lst_mouth_frz;
    INT8U step_frz_flg;
    ST_6TIME_FORM lst_step_frz;
}ST_PowerUP_Frz_Proc;

/*" 冻结发生次数，此数据结构每OAD一个 "*/
typedef struct
{
    INT32U frz_cnt[MAX_ALL_RCRD];
}ST_RCRD_CNTS;

/*" 事件发生次数，此数据结构每OAD一个 "*/
typedef struct
{
    INT32U evt_cnt[MAX_EVT_RCRD];
}ST_EVT_CNTS;

/*" 事件累计时间，此数据结构每OAD一个 "*/
typedef struct
{
    INT32U evt_duration[MAX_EVT_RCRD];
}ST_EVT_DURATION;



typedef struct
{
    ST_RCRD_PARA_TABLE  para_tbl;        /*"自动结算日"*/
    ST_EVT_FRZ_OBJ evt_frz_obj;
}ST_RCRD_PARA_FILE;

#define RCRD_PARA_TBL_OFF  FIND(ST_RCRD_PARA_FILE, para_tbl)

#define MOUTH_BILL_DATE_OFF (RCRD_PARA_TBL_OFF + FIND(ST_RCRD_PARA_TABLE,monBillDay))

#define RCRD_PARA_TBL_CRC_OFF  (RCRD_PARA_TBL_OFF + FIND(ST_RCRD_PARA_TABLE,Crc))
#define RCRD_PARA_LD_OFF        (RCRD_PARA_TBL_OFF + FIND(ST_RCRD_PARA_TABLE,LrPara))
#define RCRD_PARA_LD_PRD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordPeriod))
#define RCRD_PARA_LD_T2_PRD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordPeriod[1]))
#define RCRD_PARA_LD_T3_PRD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordPeriod[2]))
#define RCRD_PARA_LD_T4_PRD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordPeriod[3]))
#define RCRD_PARA_LD_T5_PRD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordPeriod[4]))
#define RCRD_PARA_LD_T6_PRD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordPeriod[5]))
#define RCRD_PARA_LD_MODE_WD_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecordMode))
#define RCRD_PARA_LD_ST_TM_OFF        (RCRD_PARA_LD_OFF + FIND(ST_LR645_Para,LoadRecoedBeginTime))

#define RCRD_PARA_FRZ_OFF        (RCRD_PARA_TBL_OFF + FIND(ST_RCRD_PARA_TABLE,frzPara))
#define PRD_FRZ_TM_ITV_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,prdFrzIntval))
#define TIME_FRZ_MD_WD_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,timeFrzMode))
#define MMT_FRZ_MD_WD_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,mmtFrzMode))
#define APT_FRZ_MD_WD_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,aptFrzMode))
#define PRD_FRZ_MD_WD_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,prdFrzMode))
#define DAY_FRZ_MD_WD_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,dayFrzMode))

#define DAY_FRZ_TIME_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,dayFrzTm))
#define TIME_FRZ_TIME_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,timeFrzTm))
#define PRD_FRZ_ST_TIME_OFF      (RCRD_PARA_FRZ_OFF + FIND(ST_FRZ_PARA,prdFrzStartTime))


#define RCRD_OAD_BASE   FIND(ST_RCRD_PARA_FILE, evt_frz_obj)
#if defined (THREE_PHASE)
#define LOST_VOLT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, lost_volt_obj_list))
#define LACK_VOLT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, lack_volt_obj_list))
#define OVER_VOLT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_volt_obj_list))
#define BRK_PHASE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, brk_phase_obj_list))
#define LOST_CURR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, lost_curr_obj_list))
#define OVER_CURR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_curr_obj_list))
#define BRK_CURR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, brk_curr_obj_list))
#define PWR_REVERSE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, pwr_reverse_obj_list))
#define OVER_LOAD_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_load_obj_list))
#define OVER_PA_DEMAND_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_pa_dmd_obj_list))
#define OVER_NA_DEMAND_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_na_dmd_obj_list))
#define OVER_R_DEMAND_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_r_dmd_obj_list))
#define FACTOR_LOW_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, factor_low_obj_list))
#define ALL_LOST_VOLT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, all_lost_volt_obj_list))
#define VOLT_INVERSE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, volt_inverse_obj_list))
#define CURR_INVERSE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, curr_inverse_obj_list))

#define PWR_DOWN_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, pwr_down_obj_list))
#define PRG_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, prg_evt_obj_list))
#define MTR_CLR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, meter_clr_obj_list))

#define DEMAND_CLR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, dmd_clr_obj_list))

#define EVNT_CLR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, evt_clr_obj_list))
#define JUST_TIME_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, just_tm_obj_list))
#define DAY_TBL_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, day_tbl_prg_obj_list))
#define SEASON_TBL_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, season_tbl_prg_obj_list))
#define WEEKEND_PRG_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, weekend_prg_obj_list))
#define BILL_DAY_PRG_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, bill_day_prg_obj_list))
#define MTR_COVER_OPEN_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, meter_cover_open_obj_list))
#define TAIL_COVER_OPEN_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, tail_cover_open_obj_list))

#define VOLT_IMBALANCE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, volt_imbalance_obj_list))
#define CURR_IMBALANCE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, curr_imbalance_obj_list))

#define RELAY_TRIP_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rly_trip_obj_list))
#define RELAY_CLOSE_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rly_close_obj_list))
#define HOLIDAY_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, holiday_prg_obj_list))
#define A_ENGY_COM_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, a_eng_prg_obj_list))

#define R_ENGY_COM_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, r_eng_prg_obj_list))
#define TRIFF_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, triff_prg_obj_list))
#define STEP_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, step_prg_obj_list))
#define KEY_UPDATE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, key_upd_obj_list))
#define CARD_PLUG_IN_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, card_in_err_obj_list))
#define BUY_ENGY_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, buy_evt_obj_list))
#define REFUND_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, refund_evt_obj_list))

#define CONSTANT_MGNT_DISTURB_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, constant_mgmt_obj_list))

#define RELAY_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rly_err_obj_list))
#define PWR_FAILURE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, pwr_fail_obj_list))

#define CURR_IMBALANCE_BADLY_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, curr_imbalance_badly_obj_list))
#define ENG_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, eng_err_obj_list))

#define RTC_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rtc_err_obj_list))
#define MS_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, ms_err_obj_list))

#elif defined (SINGLE_PHASE)
#define OVER_CURR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, over_curr_obj_list))

#define PWR_DOWN_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, pwr_down_obj_list))
#define PRG_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, prg_evt_obj_list))
#define MTR_CLR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, meter_clr_obj_list))

#define EVNT_CLR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, evt_clr_obj_list))
#define JUST_TIME_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, just_tm_obj_list))
#define DAY_TBL_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, day_tbl_prg_obj_list))
#define SEASON_TBL_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, season_tbl_prg_obj_list))
#define WEEKEND_PRG_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, weekend_prg_obj_list))
#define BILL_DAY_PRG_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, bill_day_prg_obj_list))
#define MTR_COVER_OPEN_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, meter_cover_open_obj_list))
#define TAIL_COVER_OPEN_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, tail_cover_open_obj_list))

#define RELAY_TRIP_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rly_trip_obj_list))
#define RELAY_CLOSE_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rly_close_obj_list))
#define HOLIDAY_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, holiday_prg_obj_list))
#define A_ENGY_COM_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, a_eng_prg_obj_list))

#define R_ENGY_COM_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, r_eng_prg_obj_list))
#define TRIFF_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, triff_prg_obj_list))
#define STEP_PRG_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, step_prg_obj_list))
#define KEY_UPDATE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, key_upd_obj_list))
#define CARD_PLUG_IN_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, card_in_err_obj_list))
#define BUY_ENGY_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, buy_evt_obj_list))
#define REFUND_EVNT_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, refund_evt_obj_list))


#define RELAY_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rly_err_obj_list))
#define PWR_FAILURE_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, pwr_fail_obj_list))

#define RTC_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, rtc_err_obj_list))
#define MS_ERR_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, ms_err_obj_list))

#endif
#define MCU_RST_ROAD_OFF (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mcu_rst_obj_list))

#define    MMT_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mmt_frz_obj_list))    /*" 瞬时冻结 "*/
#define    MIN_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, min_frz_obj_list))  /*分钟冻结次数*/
#define    HOUR_FRZ_ROAD_OFF             (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, hour_frz_obj_list))  /*" 小时冻结 "*/
#define    DAY_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, day_frz_obj_list))   /*" 日冻结 "*/
#define    ENGY_BILL_ROAD_OFF            (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mouth_bill_frz_obj_list))   /*" 月结算 "*/
#define    MON_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mouth_frz_obj_list))   /*" 月冻结 "*/
#define    YSLOT_SW_FRZ_ROAD_OFF         (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, season_tbl_sw_obj_list))    /*" 两套时区表切换冻结 "*/
#define    DSLOT_SW_FRZ_ROAD_OFF         (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, day_tbl_sw_obj_list))    /*" 两套日时段切换冻结 "*/
#define    TARIFF_SW_FRZ_ROAD_OFF        (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, triff_tbl_sw_obj_list))    /*" 费率电价切换冻结 "*/
#define    STEP_SW_FRZ_ROAD_OFF          (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, step_tbl_sw_obj_list))    /*" 阶梯电价切换冻结 "*/
#define    YEAR_BILL_ROAD_OFF            (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, step_bill_frz_obj_list))   /*"阶梯结算冻结"*/

#if defined(THREE_PHASE)
typedef struct
{
    INT32U r_eng_com2_prg_tms;  /*" 无功组合方式2编程次数 "*/
    INT8U r_eng_com_time[2][6]; /*" 最近一次无功组合方式编程时间 "*/
}ST_REngComPrgData;
#endif

typedef struct
{
    ST_RCRD_CNTS rcrd_cnts;
    
    INT32U hour_frz_num;
    INT32U day_frz_num;
    INT32U mouth_frz_num;
    
    ST_PowerUP_Frz_Proc frz_proc;
#if defined (THREE_PHASE)
    ST_REngComPrgData REngComPrg;

    INT8U last_lost_volt_tm[2][6]; /*" 最近一次失压发生结束时间 "*/
    
    INT32U EvtBeginAmpHour[36]; /*" ABC三相失压欠压过压断相发生时ABC三相安时值"*/

    INT32U pd_amp[3]; /*" 掉电时安时值 "*/
#endif
    
    ST_LR_CTR_DATA ld_info;
}ST_RCRD_DATA_FILE;
#define RCRD_CNT_BLK_OFF    FIND(ST_RCRD_DATA_FILE, rcrd_cnts)

#define RCRD_HOUR_FRZ_NUM_OFF    FIND(ST_RCRD_DATA_FILE, hour_frz_num)
#define RCRD_DAY_FRZ_NUM_OFF    FIND(ST_RCRD_DATA_FILE, day_frz_num)
#define RCRD_MOUTH_FRZ_NUM_OFF    FIND(ST_RCRD_DATA_FILE, mouth_frz_num)

#define POWERUP_FRZ_PROC_BASE   FIND(ST_RCRD_DATA_FILE, frz_proc)

#if defined(THREE_PHASE)
#define R_ENG_COM_PRG_DATA_BASE   FIND(ST_RCRD_DATA_FILE, REngComPrg)

#define LAST_LOST_VOLT_ST_TM_OFF    FIND(ST_RCRD_DATA_FILE, last_lost_volt_tm[0])
#define LAST_LOST_VOLT_END_TM_OFF    FIND(ST_RCRD_DATA_FILE, last_lost_volt_tm[1])

#define EVT_BEGIN_AMP_HOUR_BASE   FIND(ST_RCRD_DATA_FILE, EvtBeginAmpHour[0])

#define PD_AMP_HOUR_BASE   FIND(ST_RCRD_DATA_FILE, pd_amp[0])
#endif

#define E2_LR_CTR_DATA      FIND(ST_RCRD_DATA_FILE, ld_info)

/*" 计算各事件数据对象在内卡文件中的存储偏移 "*/
#define RCRD_CNT_OFF(id) (RCRD_CNT_BLK_OFF + (id - E_PWR_DOWN_EVNT_RCRD)*4)

#define DAY_FRZ_PROC_FLG    (POWERUP_FRZ_PROC_BASE + FIND(ST_PowerUP_Frz_Proc, day_frz_flg))
#define DAY_FRZ_LST_FRZ_DATE    (POWERUP_FRZ_PROC_BASE + FIND(ST_PowerUP_Frz_Proc, lst_day_frz))

#define MOUTH_BILL_FRZ_PROC_FLG    (POWERUP_FRZ_PROC_BASE + FIND(ST_PowerUP_Frz_Proc, mouth_frz_flg))
#define MOUTH_BILL_FRZ_LST_FRZ_DATE    (POWERUP_FRZ_PROC_BASE + FIND(ST_PowerUP_Frz_Proc, lst_mouth_frz))

#define STEP_FRZ_PROC_FLG    (POWERUP_FRZ_PROC_BASE + FIND(ST_PowerUP_Frz_Proc, step_frz_flg))
#define STEP_FRZ_LST_FRZ_DATE    (POWERUP_FRZ_PROC_BASE + FIND(ST_PowerUP_Frz_Proc, lst_step_frz))

#if defined(THREE_PHASE)
#define R_ENG_COM2_PRG_TMS_OFF   (R_ENG_COM_PRG_DATA_BASE + FIND(ST_REngComPrgData, r_eng_com2_prg_tms))
#define R_ENG_COM1_PRG_TIME_OFF  (R_ENG_COM_PRG_DATA_BASE + FIND(ST_REngComPrgData, r_eng_com_time[0]))
#define R_ENG_COM2_PRG_TIME_OFF  (R_ENG_COM_PRG_DATA_BASE + FIND(ST_REngComPrgData, r_eng_com_time[1]))
#endif

/*冻结关联对象属性表*/
typedef struct/*关联OAD存储在内卡中的信息结构*/
{
    INT16U FrzPeriod;
    ST_OAD FrzRoad;
    INT16U FrzDeep;
}ST_ROAD_FRZ_INFO;

/*" 记录存储操作数据结构体 "*/
typedef struct
{
    INT16U rcrd_obis;
    INT16U duration_obis;
}ST_RCRD_DURATION;

extern NO_INIT ST_RCRD_PARA_TABLE rcrd_para;/*"设置的月结算日"*/
extern INT32U mMinCnt;
extern INT8U mSetROadLstFlg;
extern ST_RCRD_LINE_RSULT_DATA  RsultAttr;
extern INT8U is_show_data; /*" 是显示抄读取数 "*/
extern ST_6TIME_FORM mstNowTime;
#if defined(THREE_PHASE)
extern INT8U is_2_3_mouth_bill;
#endif

extern INT8U AidType_is_evt_start(INT8U aid);
extern INT8U AidType_is_evt_end(INT8U aid);
extern INT8U get_single_obis_file_off_len(INT16U obis, ENUM_FS_ID *file, INT16U *off, INT16U *len);
extern INT32U get_data_addr_off(INT16U num_per_sec, INT16U sum_sector, INT16U per_sz, INT32U seq_no);
extern INT16U GetRcrdRoadListNum(EN_RCRD_FILE_ID rcrd_obis, INT8U *buf);
extern INT8U GetRcrdRoad(EN_RCRD_FILE_ID rcrd_obis, INT8U *buf, INT16U *sum_len);
extern void SetDefaultRoad(void);
extern INT8U set_default_other_para(void);
extern INT8S setBillDay(INT16U obis, INT8U *pdata, INT8U len);
extern void RCRD_Init(INT8U mode);
extern void LR_Init(INT8U mode);
extern INT8S GetBillDate(INT16U id, INT8U *pBuff);
extern INT8U GetMeterType(void);

extern INT8U RCRD_SaveData(EN_RCRD_FILE_ID file, EN_FELXOBJ_ID obis);
extern INT8S RCRD_AddOneRoad(void *pdin, void *pdout);
extern INT8S RCRD_AddMultRoad(void *pdin, void *pdout);
extern INT8S RCRD_DelOneRoad(void *pdin, void *pdout);
extern INT8S RCRD_DelAllRoad(void *pdin, void *pdout);
extern INT8S RCRD_ClrRecord(void *pdin, void *pdout);
extern INT8S RCRD_Trigger_Rcrd(void *pdin, void *pdout);
extern INT8S RCRD_Time_Adjust(void *pdin, void *pdout);
extern INT8S RCRD_Pd_Action(void *pdin, void *pdout);

#endif
