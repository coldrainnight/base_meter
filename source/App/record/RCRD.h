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
#define  INDEX_LIST_DEEP    (732)    /*"���Ӷ������������"*/
#define  MAX_DOT_NUM_PER_STAMP   (720)  /*"����ʱ����������¼��������"*/
#define LD_CRC_SZ       0 /*" sizeof(CRC_DATA_TP) "*/
#define  MIN_FRZ_CLASS_NUM    1    /*"���Ӷ�����󷽰���"*/
#else
#define  INDEX_LIST_DEEP    (360)    /*"���Ӷ������������ ÿ������64���� "*/
#define  MAX_DOT_NUM_PER_STAMP   (90)  /*"����ʱ����������¼��������"*/

//#define IDX_IN_FLASH    /*" ʱ������������flash�к궨��  "*/
#define LD_CRC_SZ       2 /*" sizeof(CRC_DATA_TP) "*/
#define  MIN_FRZ_CLASS_NUM    6    /*"���Ӷ�����󷽰���"*/
#endif

#define LD_DATA_IN_FLASH    /*" �����������flash�к궨��  "*/


#ifndef LD_DATA_IN_FLASH
typedef  INT16U   RECORD_ADDR_TYPE;/*"��¼���ݴ洢��ַ�������Ͷ���  eeprom��ַINT16U  FLASH��ַ  INT32U"*/
#else
typedef  INT32U   RECORD_ADDR_TYPE;/*"��¼���ݴ洢��ַ�������Ͷ���  eeprom��ַINT16U  FLASH��ַ  INT32U"*/
#endif



#define NUM_LD_CTRL_SPACE      6
#if(INDEX_LIST_DEEP < 256)
typedef  INT8U   INDEX_RANGE_TYPE;
#else
typedef  INT16U   INDEX_RANGE_TYPE;
#endif


/*"�����������Զ���"*/
typedef struct
{
    INT32U   TimeStamp; /*"���Ӷ��᷽������ʱ���  ���2000��1��1��0��0�ֵķ�����"*/
    INT32U   startDotId;/*"ʱ�����Ӧ��¼������ʼ��¼����� "*/
}ST_SCHEME_INDEX_ATTR;

/*"���Ӷ�����������"*/
typedef struct
{
    ST_SCHEME_INDEX_ATTR minFrzIndexOadList[MIN_FRZ_CLASS_NUM][INDEX_LIST_DEEP];
}ST_LOAD_RCD_IDX;


/*"���Ӷ��ᵱǰ��¼������Ϣ"*/
typedef struct
{
    INT32U dotCnt;  /*"��¼�����ۼ�ֵ"*/
    INT32U real_dotCnt;  /*"ʵ�ʼ�¼�����ۼ�ֵ"*/
    INDEX_RANGE_TYPE  startIndexId;  /*"������Χ��ʼ�������"*/
    INDEX_RANGE_TYPE  endIndexId;    /*"������Χ�����������"*/
    INT16U  NowStampDotCnt;/*"��ǰʱ����¶�������ۼ�ֵ"*/
    CRC_DATA_TP crcValue;
}ST_LR_CTR_DATA_FORM;

typedef struct
{
    ST_LR_CTR_DATA_FORM    LrCtrlData[MIN_FRZ_CLASS_NUM][NUM_LD_CTRL_SPACE];
}ST_LR_CTR_DATA;

#define  MAX_LINE_RSULT_NUM   96

/*"��¼��ѯ�����Ϣ�������"*/
typedef struct
{
    INT8U  kind;    /*"ɸѡ����  1/2/9 ����2�����ɸѡ ����Ϊ3"*/
    INT8U  Channel; /*"ɸѡͨ��"*/
    INT8U  stampNum;/*"ɸѡ����ֲ��ڶ��ٸ���������"*/
    INT8U num; /*" �ҵ��ļ�¼����  "*/
    INT32U  RcrdId; /*"��ʼ�����"*/
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
#define    MAX_DATA_BUFF_LEN  (1024L) /*"��¼����ռ��С����ǰ���ݱ���266 + Ԥ��50�ֽ�"*/
#define    MAX_LEN  200            /*"�������ݻ���ռ��С�����ڱ�������ת��ǰ����"*/
#define    MAX_RCRD_ROAD_NUM       (52) /*"�¼�������������б�����Ա��"*/
#else
#define    MAX_DATA_BUFF_LEN  (320L) /*"��¼����ռ��С����ǰ���ݱ���266 + Ԥ��50�ֽ�"*/
#define    MAX_LEN  200            /*"�������ݻ���ռ��С�����ڱ�������ת��ǰ����"*/
#define    MAX_RCRD_ROAD_NUM       (16) /*"�¼�������������б�����Ա��"*/
#endif

#define FLASH_SECTOR_SZ   4096L

/*" �¼������������������ "*/
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
#define MAX_OAD_RTC_ERR 2   /*" ʱ�ӹ��� "*/
#define MAX_OAD_MS_ERR 2    /*" ����оƬ���� "*/
#define MAX_OAD_MCU_RST 2    /*" MCU RST "*/

/* �����������������������*/
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
#define MAX_OAD_STEP_TBL_SW_FRZ   16  /*" �����л����� "*/
#define MAX_OAD_STEP_BILL_FRZ   4 /*" ���ݽ��㶳�� "*/
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
#define MAX_OAD_MS_ERR 2    /*" ����оƬ���� "*/
#define MAX_OAD_MCU_RST 2    /*" MCU RST "*/

/* �����������������������*/
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
#define MAX_OAD_STEP_TBL_SW_FRZ   5  /*" �����л����� "*/
#define MAX_OAD_STEP_BILL_FRZ   3 /*" ���ݽ��㶳�� "*/


#endif

#if defined(THREE_PHASE)
#define MAX_EVT_RCRD    75  /*" EEE ���ӻ�ɾ��һ���¼�Ҫ�޸� "*/
#else
#define MAX_EVT_RCRD    15  /*" EEE ���ӻ�ɾ��һ���¼�Ҫ�޸� "*/
#endif
#define MAX_FRZ_RCRD    11
#define MAX_ALL_RCRD    (MAX_EVT_RCRD + 1 + MAX_FRZ_RCRD)

#define    NUM_MMT_FRZ              (3)    /*" ˲ʱ���� "*/
#if defined(THREE_PHASE)
#define    NUM_MIN_FRZ              (28800)  /*���Ӷ������*/
#else
#define    NUM_MIN_FRZ              (288)  /*���Ӷ������*/
#endif
#define    NUM_HOUR_FRZ           (254)  /*" Сʱ���� "*/
#define    NUM_DAY_FRZ              (62)   /*" �ն��� "*/
#define    NUM_ENGY_BILL            (12)   /*" �½��� "*/
#define    NUM_MON_FRZ              (12)   /*" �¶��� "*/
#define    NUM_YSLOT_SW_FRZ         (2)    /*" ����ʱ�����л����� "*/
#define    NUM_DSLOT_SW_FRZ         (2)    /*" ������ʱ���л����� "*/
#define    NUM_TARIFF_SW_FRZ        (2)    /*" ���ʵ���л����� "*/
#define    NUM_STEP_SW_FRZ          (2)    /*" ���ݵ���л����� "*/
#define    NUM_YEAR_BILL            (4)   /*"���ݽ��㶳��"*/

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
#define MIN_FRZ_IDX_SZ  (sizeof(ST_LOAD_RCD_IDX))  /*" ���Ӷ���������С (���ڿ�)"*/
#define MIN_FRZ_SZ  (6350*FLASH_SECTOR_SZ)
#define HOUR_FRZ_SZ (2*FLASH_SECTOR_SZ)
#define DAY_FRZ_SZ  (2*FLASH_SECTOR_SZ)
#define MONTH_BILL_SZ   (2*FLASH_SECTOR_SZ)
#define MONTH_FRZ_SZ        (2*FLASH_SECTOR_SZ)
#define YEAR_TBL_SW_FRZ_SZ    (2*FLASH_SECTOR_SZ)
#define DAY_TBL_SW_FRZ_SZ       (2*FLASH_SECTOR_SZ)
#define TRIFF_SW_FRZ_SZ     (2*FLASH_SECTOR_SZ)
#define STEP_SW_FRZ_SZ      (2*FLASH_SECTOR_SZ) /*" �����л����� "*/
#define STEP_BILL_FRZ_SZ        (2*FLASH_SECTOR_SZ) /*" ���ݽ��㶳�� "*/

#define PD_PON_EVT_SZ           (2*FLASH_SECTOR_SZ)
#define MSR_DATAL_SIZE			(5*FLASH_SECTOR_SZ)
#define MSR_DATAN_SIZE			(5*FLASH_SECTOR_SZ)
/*" �����ļ����� "*/
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
#define PRG_SZ             (94*10)  /*" ��̼�¼ ���ڿ� "*/
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
/*" �����ļ� "*/


#define MMT_FRZ_SZ              (2*FLASH_SECTOR_SZ)
#define MIN_FRZ_IDX_SZ  (sizeof(ST_LOAD_RCD_IDX))  /*" ���Ӷ���������С (���ڿ�)"*/
#define MIN_FRZ_SZ              (1180*FLASH_SECTOR_SZ)
#define HOUR_FRZ_SZ                (7*FLASH_SECTOR_SZ)   /*" Сʱ���� ��flash "*/
#define DAY_FRZ_SZ              (15*FLASH_SECTOR_SZ)
#define MONTH_BILL_SZ          (12*1000L) /*" �½��㶳�� ���ڿ� "*/    
#define MONTH_FRZ_SZ            (4*FLASH_SECTOR_SZ)
#define YEAR_TBL_SW_FRZ_SZ    (2*FLASH_SECTOR_SZ)    
#define DAY_TBL_SW_FRZ_SZ       (2*FLASH_SECTOR_SZ)
#define TRIFF_SW_FRZ_SZ     (2*FLASH_SECTOR_SZ)
#define STEP_SW_FRZ_SZ      (2*FLASH_SECTOR_SZ) /*" �����л����� "*/
#define STEP_BILL_FRZ_SZ        (2*FLASH_SECTOR_SZ) /*" ���ݽ��㶳�� "*/

#endif

/*" �����ݽṹÿOIһ�� "*/
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

/*" ���ᷢ�������������ݽṹÿOADһ�� "*/
typedef struct
{
    INT32U frz_cnt[MAX_ALL_RCRD];
}ST_RCRD_CNTS;

/*" �¼����������������ݽṹÿOADһ�� "*/
typedef struct
{
    INT32U evt_cnt[MAX_EVT_RCRD];
}ST_EVT_CNTS;

/*" �¼��ۼ�ʱ�䣬�����ݽṹÿOADһ�� "*/
typedef struct
{
    INT32U evt_duration[MAX_EVT_RCRD];
}ST_EVT_DURATION;



typedef struct
{
    ST_RCRD_PARA_TABLE  para_tbl;        /*"�Զ�������"*/
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

#define    MMT_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mmt_frz_obj_list))    /*" ˲ʱ���� "*/
#define    MIN_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, min_frz_obj_list))  /*���Ӷ������*/
#define    HOUR_FRZ_ROAD_OFF             (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, hour_frz_obj_list))  /*" Сʱ���� "*/
#define    DAY_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, day_frz_obj_list))   /*" �ն��� "*/
#define    ENGY_BILL_ROAD_OFF            (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mouth_bill_frz_obj_list))   /*" �½��� "*/
#define    MON_FRZ_ROAD_OFF              (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, mouth_frz_obj_list))   /*" �¶��� "*/
#define    YSLOT_SW_FRZ_ROAD_OFF         (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, season_tbl_sw_obj_list))    /*" ����ʱ�����л����� "*/
#define    DSLOT_SW_FRZ_ROAD_OFF         (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, day_tbl_sw_obj_list))    /*" ������ʱ���л����� "*/
#define    TARIFF_SW_FRZ_ROAD_OFF        (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, triff_tbl_sw_obj_list))    /*" ���ʵ���л����� "*/
#define    STEP_SW_FRZ_ROAD_OFF          (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, step_tbl_sw_obj_list))    /*" ���ݵ���л����� "*/
#define    YEAR_BILL_ROAD_OFF            (RCRD_OAD_BASE + FIND(ST_EVT_FRZ_OBJ, step_bill_frz_obj_list))   /*"���ݽ��㶳��"*/

#if defined(THREE_PHASE)
typedef struct
{
    INT32U r_eng_com2_prg_tms;  /*" �޹���Ϸ�ʽ2��̴��� "*/
    INT8U r_eng_com_time[2][6]; /*" ���һ���޹���Ϸ�ʽ���ʱ�� "*/
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

    INT8U last_lost_volt_tm[2][6]; /*" ���һ��ʧѹ��������ʱ�� "*/
    
    INT32U EvtBeginAmpHour[36]; /*" ABC����ʧѹǷѹ��ѹ���෢��ʱABC���లʱֵ"*/

    INT32U pd_amp[3]; /*" ����ʱ��ʱֵ "*/
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

/*" ������¼����ݶ������ڿ��ļ��еĴ洢ƫ�� "*/
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

/*��������������Ա�*/
typedef struct/*����OAD�洢���ڿ��е���Ϣ�ṹ*/
{
    INT16U FrzPeriod;
    ST_OAD FrzRoad;
    INT16U FrzDeep;
}ST_ROAD_FRZ_INFO;

/*" ��¼�洢�������ݽṹ�� "*/
typedef struct
{
    INT16U rcrd_obis;
    INT16U duration_obis;
}ST_RCRD_DURATION;

extern NO_INIT ST_RCRD_PARA_TABLE rcrd_para;/*"���õ��½�����"*/
extern INT32U mMinCnt;
extern INT8U mSetROadLstFlg;
extern ST_RCRD_LINE_RSULT_DATA  RsultAttr;
extern INT8U is_show_data; /*" ����ʾ����ȡ�� "*/
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
