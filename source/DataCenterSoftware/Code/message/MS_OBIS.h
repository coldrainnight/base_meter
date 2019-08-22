/*"*************************************************************"*/
/*"FileName: MS_OBIS.h"*/
/*"Author:    		Version :  "*/
/*"2019-08-21 11:03:30"*/
/*"Description: ���ܱ����ƽ̨���ݶ����ʶ����"*/
/*"���ֽ���:          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/


#include "PlatForm.h"
#ifndef MS_OBIS_H
#define MS_OBIS_H
#define  RAM      0x80000000


typedef struct
{
    INT16U obis; /*"�����ʶ"*/
    DATA_ADDR addr; /*"���ݴ洢��ʼ��ַ"*/
    INT16U len; /*"���ݳ���"*/
    INT8U ObjNum; /*"���ݿ��Ա��"*/
    INT8U format_id; /*"���ݸ�ʽ����"*/
}ST_BLK_DATA_ATTR;

typedef struct
{
    INT16U obis; /*"�����ʶ"*/
    DATA_ADDR addr; /*"���ݴ洢��ʼ��ַ"*/
    INT16U len; /*"���ݳ���"*/
    INT8U format_id; /*"���ݸ�ʽ����"*/
}ST_SINGLE_DATA_ATTR;

typedef struct
{
    INT16U obis; /*"���ݱ�ʶ"*/
    INT8U format_id; /*"��¼�洢���ݸ�ʽ"*/
}ST_FIXED_LIST_ATTR;

typedef struct

{
    INT16U obis; /*"�����������ݱ�ʶ"*/
    const ST_FIXED_LIST_ATTR *pdatalis1; /*"�̶���¼�����б�"*/
    const ST_FIXED_LIST_ATTR *pdatalis2; /*"���������б����ݶ���"*/
    INT8U data_num1; /*"�̶���¼�����б����ݶ������"*/
    INT8U data_num2; /*"���������б����ݶ������"*/
}ST_FELXOBJ_DATA_ATTR;

typedef struct
{
    INT16U obis; /*"��¼�����ʶ"*/
    INT16U file; /*"�ļ�ϵͳ�ж�Ӧ�������ļ���"*/
    INT16U timesAddr; /*"��¼�����ڿ��洢��ַ"*/
    INT16U fileSize;   /*"��¼�ļ���С"*/
    INT16U startFelxObj; /*"��ʼ��¼���������ʶ"*/
    INT16U endFelxObj; /*"������¼���������ʶ"*/
}ST_FILE_ATTR;

typedef INT8S (* ACTION)(void *pdin, void *pdout);
typedef struct
{
    INT16U obis; /*"�����ʶ"*/
    ACTION Fuction; /*"ִ�з���"*/    
}ST_ACTION_ATTR;



/*"��������ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_PA_ENGY_BLK = 0x0000,/*"�����й��������ݿ�"*/
      E_PA_ENGY,/*"�����й��ܵ���"*/
      E_PA_ENGY_T1,/*"�����й�����1����"*/
      E_PA_ENGY_T2,/*"�����й�����2����"*/
      E_PA_ENGY_T3,/*"�����й�����3����"*/
      E_PA_ENGY_T4,/*"�����й�����4����"*/
      E_PA_ENGY_T5,/*"�����й�����5����"*/
      E_PA_ENGY_T6,/*"�����й�����6����"*/
      E_PA_ENGY_T7,/*"�����й�����7����"*/
      E_PA_ENGY_T8,/*"�����й�����8����"*/
    E_NA_ENGY_BLK = 0x0010,/*"�����й��������ݿ�"*/
      E_NA_ENGY,/*"�����й��ܵ���"*/
      E_NA_ENGY_T1,/*"�����й�����1����"*/
      E_NA_ENGY_T2,/*"�����й�����2����"*/
      E_NA_ENGY_T3,/*"�����й�����3����"*/
      E_NA_ENGY_T4,/*"�����й�����4����"*/
      E_NA_ENGY_T5,/*"�����й�����5����"*/
      E_NA_ENGY_T6,/*"�����й�����6����"*/
      E_NA_ENGY_T7,/*"�����й�����7����"*/
      E_NA_ENGY_T8,/*"�����й�����8����"*/
    E_ACTIVE_PWR_BLK = 0x0020,/*"�й��������ݿ�"*/
      E_ACTIVE_PWR,/*"���й�����"*/
    E_VOLT_BLK = 0x0030,/*"��ѹ���ݿ�"*/
      E_VOLT_LA,/*"A���ѹ"*/
    E_CURR_BLK = 0x0040,/*"�������ݿ�"*/
      E_CURR_LA,/*"A�����"*/
    E_FACTOR_BLK = 0x0050,/*"�����������ݿ�"*/
      E_FACTOR,/*"�ܹ�������"*/
    E_PHASE_BLK = 0x0060,/*"��������������ݿ�"*/
      E_PHASE,/*"���"*/
    E_A_ENGY_COM_BLK = 0x0070,/*"����й��������ݿ�"*/
      E_A_ENGY_COM,/*"����й��ܵ���"*/
      E_A_ENGY_COM_T1,/*"����й�����1����"*/
      E_A_ENGY_COM_T2,/*"����й�����2����"*/
      E_A_ENGY_COM_T3,/*"����й�����3����"*/
      E_A_ENGY_COM_T4,/*"����й�����4����"*/
      E_A_ENGY_COM_T5,/*"����й�����5����"*/
      E_A_ENGY_COM_T6,/*"����й�����6����"*/
      E_A_ENGY_COM_T7,/*"����й�����7����"*/
      E_A_ENGY_COM_T8,/*"����й�����8����"*/
    E_R_ENGY_I_BLK = 0x0080,/*"I�����޹��������ݿ�"*/
      E_R_ENGY_I,/*"I�����޹��ܵ���"*/
      E_R_ENGY_I_T1,/*""*/
      E_R_ENGY_I_T2,/*""*/
      E_R_ENGY_I_T3,/*""*/
      E_R_ENGY_I_T4,/*""*/
      E_R_ENGY_I_T5,/*""*/
      E_R_ENGY_I_T6,/*""*/
      E_R_ENGY_I_T7,/*""*/
      E_R_ENGY_I_T8,/*""*/
    E_R_ENGY_II_BLK = 0x0090,/*"II�����޹��������ݿ�"*/
      E_R_ENGY_II,/*""*/
      E_R_ENGY_II_T1,/*""*/
      E_R_ENGY_II_T2,/*""*/
      E_R_ENGY_II_T3,/*""*/
      E_R_ENGY_II_T4,/*""*/
      E_R_ENGY_II_T5,/*""*/
      E_R_ENGY_II_T6,/*""*/
      E_R_ENGY_II_T7,/*""*/
      E_R_ENGY_II_T8,/*""*/
    E_R_ENGY_III_BLK = 0x00a0,/*"III�����޹��������ݿ�"*/
      E_R_ENGY_III,/*""*/
      E_R_ENGY_III_T1,/*""*/
      E_R_ENGY_III_T2,/*""*/
      E_R_ENGY_III_T3,/*""*/
      E_R_ENGY_III_T4,/*""*/
      E_R_ENGY_III_T5,/*""*/
      E_R_ENGY_III_T6,/*""*/
      E_R_ENGY_III_T7,/*""*/
      E_R_ENGY_III_T8,/*""*/
    E_R_ENGY_IV_BLK = 0x00b0,/*"IV�����޹��������ݿ�"*/
      E_R_ENGY_IV,/*""*/
      E_R_ENGY_IV_T1,/*""*/
      E_R_ENGY_IV_T2,/*""*/
      E_R_ENGY_IV_T3,/*""*/
      E_R_ENGY_IV_T4,/*""*/
      E_R_ENGY_IV_T5,/*""*/
      E_R_ENGY_IV_T6,/*""*/
      E_R_ENGY_IV_T7,/*""*/
      E_R_ENGY_IV_T8,/*""*/
    E_REACTIVE_PWR_BLK = 0x00c0,/*"�޹��������ݿ�"*/
      E_REACTIVE_PWR,/*"���޹�����"*/
    E_A_VOLT_CONT_BLK = 0x00d0,/*"A���ѹг��������(��=THD)"*/
      E_A_VOLT_CONT_TOTAL,/*"��"*/
      E_A_VOLT_CONT_1,/*"1��"*/
      E_A_VOLT_CONT_2,/*""*/
      E_A_VOLT_CONT_3,/*""*/
      E_A_VOLT_CONT_4,/*""*/
      E_A_VOLT_CONT_5,/*""*/
      E_A_VOLT_CONT_6,/*""*/
      E_A_VOLT_CONT_7,/*""*/
      E_A_VOLT_CONT_8,/*""*/
      E_A_VOLT_CONT_9,/*""*/
      E_A_VOLT_CONT_10,/*""*/
      E_A_VOLT_CONT_11,/*""*/
      E_A_VOLT_CONT_12,/*""*/
      E_A_VOLT_CONT_13,/*""*/
      E_A_VOLT_CONT_14,/*""*/
      E_A_VOLT_CONT_15,/*""*/
    E_A_CURR_CONT_BLK = 0x00e0,/*"A�����г��������(��=THD)"*/
      E_A_CURR_CONT_TOTAL,/*"��"*/
      E_A_CURR_CONT_1,/*"1��"*/
      E_A_CURR_CONT_2,/*""*/
      E_A_CURR_CONT_3,/*""*/
      E_A_CURR_CONT_4,/*""*/
      E_A_CURR_CONT_5,/*""*/
      E_A_CURR_CONT_6,/*""*/
      E_A_CURR_CONT_7,/*""*/
      E_A_CURR_CONT_8,/*""*/
      E_A_CURR_CONT_9,/*""*/
      E_A_CURR_CONT_10,/*""*/
      E_A_CURR_CONT_11,/*""*/
      E_A_CURR_CONT_12,/*""*/
      E_A_CURR_CONT_13,/*""*/
      E_A_CURR_CONT_14,/*""*/
      E_A_CURR_CONT_15,/*""*/
    E_FREQ = 0x0E00,/*"����Ƶ��"*/
    E_SYS_TIME,/*"ϵͳʱ��"*/
    E_PD_TIME,/*"����ʱ��"*/
    E_A_PULSE_CONST,/*"�й����峣��"*/
    E_CURR_LN,/*"���ߵ���"*/
    E_CURR_RESIDUAL,/*"ʣ�����"*/
    E_PULSE_WIDTH,/*"������"*/
    E_A_ENGY_COM_TYPE,/*"�й���Ϸ�ʽ������"*/
    E_PWR_DIR,/*"���ʷ���"*/
    E_LRM_TMP_LINE_IN,/*"���߽����¶�(��)"*/
    E_LRM_TMP_LINE_OUT,/*"���߳����¶�(��)"*/
    E_LRM_TMP_N_IN,/*"���߽����¶�(��)"*/
    E_LRM_TMP_N_OUT,/*"���߳����¶�(��)"*/
    E_AMP_ERR,/*"���ʷ�ֵ���"*/
    E_A_VOLT_AVAILAB,/*"��ѹ��г����Чֵ(����)"*/
    E_A_CURR_AVAILAB,/*"������г����Чֵ(����)"*/
    E_LRM_CLR = 0x0FE0,/*"����������������"*/
    E_LRM_JUSTMETER,/*"����У��"*/
    E_LRM_JUSTIME,/*"�޸�ʱ��"*/
    E_LRM_PWR_DOWN,/*"����ģ����紦��"*/
    E_LRM_PCTR_INIT,/*"������ƿ��ϵ��ʼ��"*/
 }EN_LRM_DATA_ID;


/*"��������ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_PA_MAX_DEMAND_BLK = 0x1000,/*"�����й�����������ݿ�"*/
      E_PA_MAX_DEMAND,/*"�����й��������������"*/
      E_PA_MAX_DEMAND_T1,/*"�����й�����1�����������"*/
      E_PA_MAX_DEMAND_T2,/*"�����й�����2�����������"*/
      E_PA_MAX_DEMAND_T3,/*"�����й�����3�����������"*/
      E_PA_MAX_DEMAND_T4,/*"�����й�����4�����������"*/
      E_PA_MAX_DEMAND_T5,/*"�����й�����5�����������"*/
      E_PA_MAX_DEMAND_T6,/*"�����й�����6�����������"*/
      E_PA_MAX_DEMAND_T7,/*"�����й�����7�����������"*/
      E_PA_MAX_DEMAND_T8,/*"�����й�����8�����������"*/
    E_NA_MAX_DEMAND_BLK = 0x1010,/*"�����й�����������ݿ�"*/
      E_NA_MAX_DEMAND,/*"�����й��������������"*/
      E_NA_MAX_DEMAND_T1,/*"�����й�����1�����������"*/
      E_NA_MAX_DEMAND_T2,/*"�����й�����2�����������"*/
      E_NA_MAX_DEMAND_T3,/*"�����й�����3�����������"*/
      E_NA_MAX_DEMAND_T4,/*"�����й�����4�����������"*/
      E_NA_MAX_DEMAND_T5,/*"�����й�����5�����������"*/
      E_NA_MAX_DEMAND_T6,/*"�����й�����6�����������"*/
      E_NA_MAX_DEMAND_T7,/*"�����й�����7�����������"*/
      E_NA_MAX_DEMAND_T8,/*"�����й�����8�����������"*/
    E_DEMAND_PERIOD = 0x1E00,/*"��������"*/
    E_DEMAND_SLID_PERIOD,/*"����ʱ��"*/
    E_DU_CLR = 0x1FE0,/*"��������ռ�����"*/
    E_MAX_DEMAND_CLR,/*"�����������"*/
 }EN_DU_DATA_ID;


/*"��ʱģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_ACTIVE_CALENDAR_DAY_TBL = 0x2000,/*"��ǰ����ʱ�α�"*/
    E_PASSIVE_CALENDAR_DAY_TBL = 0x2040,/*"��������ʱ�α�"*/
    E_ACTIVE_CALENDAR_SEASON_TBL = 0x2080,/*"��ǰ��ʱ����"*/
    E_PASSIVE_CALENDAR_SEASON_TBL = 0x20c0,/*"������ʱ����"*/
    E_SPECIAL_DAY_TBL = 0x2100,/*"�������ձ�"*/
    E_TARIFF_NOW = 0x2E00,/*"��ǰ���ʺ�"*/
    E_SEASON_TBL_ACTIVE_PASSIVE_SW_TIME,/*"����ʱ�����л�ʱ��"*/
    E_PROG_DAY_TBL,/*"���ǰ��ʱ�α�"*/
    E_PROG_SEASON_TBL,/*"���ǰ��ʱ����"*/
    E_DAY_TBL_ACTIVE_PASSIVE_SW_TIME,/*"������ʱ���л�ʱ��"*/
    E_SEASON_SLOT_NUM,/*"��ʱ����"*/
    E_DAY_TBL_NUM,/*"��ʱ�α���"*/
    E_DAY_SLOT_NUM,/*"��ʱ����(ÿ���л���)"*/
    E_TARIFF_NUM,/*"������"*/
    E_SPECIAL_DAY_NUM,/*"����������"*/
    E_PROG_HOLIDAY,/*"���ǰ�ڼ���"*/
    E_WEEKEND_TYPE,/*"������������"*/
    E_WEEKEND_DAY_TBL_ID,/*"��������õ���ʱ�α��"*/
    E_CHECK_SWITCH_ACTION = 0x2FE0,/*"�л����"*/
    E_FIND_NOW_TRIFF_ACTION,/*"��ѯ����"*/
    E_SYSTIME_UPDATE_ACTION,/*"ʱ��������²�ѯ����"*/
 }EN_TR_DATA_ID;


/*"Ԥ����ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    PM_NULL = 0x3E00,/*"PM_NULL"*/
 }EN_PM_DATA_ID;


/*"���״̬���ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_EVNT_DURATION_BLK = 0x4000,/*"�¼��ۼ�ʱ��"*/
      E_PWR_DOWN_DURATION,/*"�����ۼ�ʱ��"*/
      E_OVER_CURR_LA_DURATION,/*"�����¼��ۼ�ʱ��"*/
    E_MIN_ACTIVEPOWER_BLK = 0x4040,/*"ÿ����ƽ������"*/
      E_MIN_ACTIVEPOWER_T,/*"��"*/
      E_MIN_ACTIVEPOWER_A,/*"A��"*/
    E_OVERCURR_IL_THR = 0x4E00,/*"�����¼�������������"*/
    E_OVERCURR_TIME_THR,/*"�����¼��ж���ʱʱ��"*/
    E_MNT_EVNT_TIME_S,/*"��¼��ʼʱ��"*/
    E_MNT_EVNT_TIME_E,/*"��¼����ʱ��"*/
    E_RLY_TRIP_CTRL_CURR_THR,/*"�̵�����բ���Ƶ�������ֵ"*/
    E_FAC_STATUS,/*"����״̬"*/
    E_CLK_BAT_VOLT,/*"ʱ�ӵ�ص�ѹ"*/
    E_RLY_STATUS_NOW,/*"�̵��������ǰ״̬"*/
    E_EVT_LCDALARM_MODE,/*"LCD�ͼ̵����¼�����ģʽ��"*/
    E_EVT_BGTALARM_MODE,/*"�����¼�����ģʽ��"*/
    E_MUERR_TIME_THR,/*"���������ж�ʱ��"*/
    E_CLKERR_TIME_THR,/*"ʱ�ӹ����ж�ʱ��"*/
    E_RST_INFO,/*"��λԴ"*/
    E_RST_ADDR,/*"��λ��ַ"*/
    E_UPDATA,/*"����о���ݸ�����о������"*/
    E_TMP_LINE_IN,/*"���߽����¶�(��)"*/
    E_TMP_LINE_OUT,/*"���߳����¶�(��)"*/
    E_TMP_N_IN,/*"���߽����¶�(��)"*/
    E_TMP_N_OUT,/*"���߳����¶�(��)"*/
    E_MNT_ESAM_ID,/*"ESAM���"*/
    E_MNT_MTR_TMP,/*"�����¶�"*/
    E_MNT_MTR_HMDTY,/*"����ʪ��"*/
    E_MNT_AMP_ERR,/*"���ʷ�ֵ���"*/
    E_ACCRCY_RFRSHT,/*"����Լ�����"*/
    E_MNT_EVNT_CLR = 0x4FE0,/*"�¼�����"*/
    E_MNT_PWR_DOWN,/*"���紦��"*/
    E_OUT_FAC,/*"�˳�����"*/
    E_FIRST_PWR_UP_OVER,/*"��һ���ϵ��ʼ�����"*/
    E_RESET_FAC_REMAIN_TIME,/*"���ù���״̬ʣ��ʱ��"*/
    E_PWRUP_EVTINIT,/*"�ϵ粹�¼�"*/
    E_EVTPARACAL,/*"����Ĭ�ϲ���"*/
    E_UPDATA_SET,/*"����о�����ݴ��ݸ�����о"*/
    E_DOWNDATA_GET,/*"����о�����ݴ��ݸ�����о"*/
 }EN_MNT_DATA_ID;


/*"�û��ӿ�ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_SAFE_MODE_LIST = 0x5000,/*"��ȫģʽ����"*/
    E_NO_BACKUP_BEGIN_BLOCK = 0x5080,/*"�����Ƿ�RAM���ݷָ��"*/
    E_CYCLE_DIS_OBJ_LIST = 0x5100,/*"ѭ����ʾ�б�"*/
    E_KEY_DIS_OBJ_LIST = 0x5180,/*"������ʾ�б�"*/
    E_PWRUP_DIS_ALL_TIME = 0x5E00,/*"�ϵ�ȫ��ʱ��"*/
    E_BACKLIGHT_SHOW_TIME,/*"�������ʱ��"*/
    E_BACKLIGHT_SHOW_TIME_ON_VIEW,/*"��ʾ�鿴�������ʱ��"*/
    E_PD_NOKEY_RETURN_TIME,/*"�޵簴����Ļפ�����ʱ��"*/
    E_PWR_SHOW_POINT,/*"��ʾ���ʣ����������С��λ��"*/
    E_ENGY_SHOW_POINT,/*"��ʾ����С��λ��"*/
    E_DIS_SEG_MODE_CTRL_12,/*"Һ���٢���������"*/
    E_NOKEY_RETURN_TIME,/*"�ް�������ʱ��"*/
    E_CYCLE_SHOW_TIME,/*"ѭ�Լ��ʱ��"*/
    E_CYCLE_SHOW_NUM,/*"ѭ����Ŀ��"*/
    E_KEY_SHOW_NUM,/*"������Ŀ��"*/
    E_RS485_1_BAUDRATE,/*"��1·485������"*/
    E_RPTRST_TIME,/*"645�����ϱ�״̬���Ը�λ��ʱʱ��"*/
    E_MTR_RUN_MODE,/*"�������������"*/
    E_EXT_MODE1,/*"��չ����1"*/
    E_EXT_MODE2,/*"��չ����2"*/
    E_EXT_MODE3,/*"��չ����3"*/
    E_EXT_MODE4,/*"��չ����4"*/
    E_EXT_MODE5,/*"��չ����5"*/
    E_EXT_MODE6,/*"��չ����6"*/
    E_EXT_MODE7,/*"��չ����7"*/
    E_EXT_MODE8,/*"��չ����8"*/
    E_EXT_MODE9,/*"��չ����9"*/
    E_EXT_MODE10,/*"��չ����10"*/
    E_COMM_ADDR,/*"ͨ�ŵ�ַ"*/
    E_GROUP_ADDR,/*"���ַ"*/
    E_AUTHEN_PASSWORD,/*"��֤����"*/
    E_645REPORT_MODE,/*"645�����ϱ�ģʽ��"*/
    E_NO_BACKUP_BEGIN,/*"�����Ƿ�RAM���ݷָ��"*/
    E_REGU_VOLT,/*"���ѹ"*/
    E_REGU_CURR,/*"�����"*/
    E_MAX_CURR,/*"������"*/
    E_SOFT_VER_DATE,/*"����汾����"*/
    E_HARD_VER_DATE,/*"Ӳ���汾����"*/
    E_ACT_ACCU_GRADE,/*"�й�׼ȷ�ȵȼ�"*/
    E_FAC_NUM,/*"���Ҵ���"*/
    E_SOFTWARE_VER,/*"����汾��"*/
    E_HARDWARE_VER,/*"Ӳ���汾��"*/
    E_LONGITUDE,/*"����"*/
    E_LATITUDE,/*"γ��"*/
    E_HEIGHT,/*"�߶�"*/
    E_SOFT_RECORD_ID,/*"���������"*/
    E_MANUFACTOR_EXT_INFO,/*"������չ��Ϣ"*/
    E_SUP_PROFILE_LIST,/*"֧�ֹ�Լ�б�"*/
    E_PROTOCOL_VER,/*"Э��汾��"*/
    E_METER_TYPE,/*"����ͺ�"*/
    E_ASSET_NUM,/*"�ʲ��������"*/
    E_PRODUCT_DATE,/*"��������"*/
    E_PRO_CONSISTENCY,/*"Э��һ���Կ�"*/
    E_FUN_CONSISTENCY,/*"����һ���Կ�"*/
    E_PP_OAD,/*"ȫ��OAD"*/
    E_PP_Program_OAD,/*"���OAD"*/
    E_CUR_CHANNL,/*"��ǰͨ��ͨ��"*/
    E_RUN_STA_1,/*"�������״̬��1"*/
    E_RUN_STA_2,/*"�������״̬��2"*/
    E_RUN_STA_3,/*"�������״̬��3"*/
    E_RUN_STA_4,/*"�������״̬��4"*/
    E_RUN_STA_5,/*"�������״̬��5"*/
    E_RUN_STA_6,/*"�������״̬��6"*/
    E_RUN_STA_7,/*"�������״̬��7"*/
    E_METER_SAMPLE_STATUS,/*"����о����״̬��"*/
    E_SAMPLE_STATUS,/*"����״̬��"*/
    E_CLK_BAT_TIME,/*"ʱ�ӵ������ʱ��"*/
    E_RELAY_UNIT_POWER,/*"��բ�̵�����Ԫ(���⴦��)"*/
    E_SOFT_VER_32,/*"�ڲ�����汾32�ֽ�"*/
    E_UIDATA_CLR = 0x5FE0,/*"����û���������"*/
    E_SHOW_SCREEN_APPOINT,/*"��ʾָ����������ģ�飩"*/
    E_SHOW_FLASH,/*"��ʾ��ʱˢ��"*/
    E_SHOW_SCREEN_ALL,/*"ȫ����ʾ"*/
    E_SHOW_SCREEN_NEXT,/*"��ʾ�·�"*/
    E_SHOW_SCREEN_LAST,/*"��ʾ�Ϸ�"*/
    E_AUTHALLOW_KEEP,/*"˫о��֤��������"*/
    E_PRGEVENT_RCRD,/*"����¼���¼"*/
    E_SHOW_SCREEN_DLT698,/*"��ʾ�鿴��698Э�飩"*/
 }EN_UI_DATA_ID;


/*"��¼����ģ��(����)ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_RCRD_CNT_BLK = 0x6000,/*"��¼�ۼƴ������ݿ�"*/
      E_RCRD_CNT_BLK_PWR_DOWN,/*"�����¼�����"*/
    E_RCRD_CUR_CNT = 0x6080,/*"��ǰ��¼�����ݿ�"*/
      E_RCRD_CUR_CNT_PD,/*"�����¼���ǰ��¼"*/
    E_RCRD_MAX_CNT = 0x6100,/*"����¼�����ݿ�"*/
      E_RCRD_MAX_CNT_PD,/*"�����¼�����¼��"*/
    E_RCRD_CUR_VALUE = 0x6180,/*"��ǰֵ��¼�����ݿ�"*/
      E_RCRD_CUR_VALUE_PD,/*"�����¼���ǰֵ��¼��"*/
    E_RCRD_TIME_STATUS = 0x6200,/*"ʱ��״̬��¼�����ݿ�"*/
      E_RCRD_TIME_STATUS_PD,/*"�����¼�ʱ��״̬��¼��"*/
    E_MOUTH_BILLDAY = 0x6280,/*"�½����ղ�����"*/
      E_MOUTH_BILLDAY1,/*"��һ�½�����"*/
      E_MOUTH_BILLDAY2,/*"�ڶ��½�����"*/
      E_MOUTH_BILLDAY3,/*"�����½�����"*/
    PWR_DOWN_OBJ_LIST = 0x6E00,/*"�����¼���������"*/
    MTR_COVER_OPEN_OBJ_LIST,/*"������¼���������"*/
    PRG_EVT_OBJ_LIST,/*"����¼���������"*/
    CLRMTR_OBJ_LIST,/*"�������"*/
    EVNT_CLR_OBJ_LIST,/*"�¼�����"*/
    DAY_TBL_PRG_OBJ_LIST,/*"��ʱ�α����¼�"*/
    SEASON_TBL_PRG_OBJ_LIST,/*"��ʱ�������¼�"*/
    WEEKEND_PRG_OBJ_LIST,/*"�����ձ��"*/
    A_ENGY_COM_OBJ_LIST,/*"����й�ģʽ�ֱ���¼�"*/
    BILL_DAY_PRG_OBJ_LIST,/*"�����ձ���¼�"*/
    JUSTTIME_EVT_OBJ_LIST,/*"Уʱ�¼�"*/
    RELAY_ERR_OBJ_LIST,/*"�̵��������¼�"*/
    PWR_FAILURE_OBJ_LIST,/*"��Դ�쳣�¼�"*/
    HOLIDAY_PRG_OBJ_LIST,/*"�ڼ��ձ���¼�"*/
    OVERCURR_OBJ_LIST,/*"A������¼�"*/
    RTC_ERR_OBJ_LIST,/*"ʱ�ӹ��Ϲ�������"*/
    MS_ERR_OBJ_LIST,/*"����оƬ���Ϲ�������"*/
    MMT_FRZ_OBJ_LIST,/*"˲ʱ����"*/
    MIN_FRZ_OBJ_LIST,/*"���Ӷ���"*/
    E_LD_TYPE1_PERIOD,/*"��һ�ฺ�ɼ��"*/
    HOUR_FRZ_OBJ_LIST,/*"Сʱ����"*/
    E_LD_TYPE2_PERIOD,/*"�ڶ��ฺ�ɼ��"*/
    DAY_FRZ_OBJ_LIST,/*"�ն���"*/
    E_LD_TYPE3_PERIOD,/*"�����ฺ�ɼ��"*/
    MOUTH_BILL_FRZ_OBJ_LIST,/*"�½���"*/
    E_LD_TYPE4_PERIOD,/*"�����ฺ�ɼ��"*/
    MOUTH_FRZ_OBJ_LIST,/*"�¶���"*/
    E_LD_TYPE5_PERIOD,/*"�����ฺ�ɼ��"*/
    SEASON_TBL_SW_FRZ_OBJ_LIST,/*"ʱ�����л�����"*/
    E_LD_TYPE6_PERIOD,/*"�����ฺ�ɼ��"*/
    DAY_TBL_SW_FRZ_OBJ_LIST,/*"��ʱ�α��л�����"*/
    E_LD_ST_TIME,/*"���ɼ�¼��ʼʱ��"*/
    E_TIME_STAMP,/*"ʱ���"*/
    E_OPER_CODE,/*"�����ߴ���"*/
    E_FRZ_ST_TIME,/*"���ᷢ��ʱ��"*/
    E_RCRD_CLR = 0x6FE0,/*"��¼����"*/
    E_RCRD_ADD_ONE_ROAD,/*""*/
    E_RCRD_TRIGGER_RCRD,/*"����һ�ζ���"*/
    E_RCRD_ADD_MULT_ROAD,/*""*/
    E_RCRD_TIME_ADJUST,/*"��ʱ���¼��������"*/
    E_RCRD_DEL_ONE_ROAD,/*""*/
    E_RCRD_PD_ACTION,/*"��¼ģ����紦��"*/
    E_RCRD_DEL_ALL_ROAD,/*""*/
 }EN_RCRD_DATA_ID;


/*"��¼����ģ��(�ļ�)��׼������"*/
 typedef enum 
 {
    E_PWR_DOWN_EVNT_RCRD = 0x7000,  /*"�����¼�"*/
    E_MTR_COVER_OPEN_EVNT_RCRD,  /*"������¼�"*/
    E_MTR_CLR_EVNT_RCRD,  /*"�������"*/
    E_EVNT_CLR_EVNT_RCRD,  /*"�¼�����"*/
    E_JUST_TIME_EVNT_RCRD,  /*"Уʱ�¼�"*/
    E_RELAY_ERR_EVNT_RCRD,  /*"�̵��������¼�"*/
    E_PWR_FAILURE_EVNT_RCRD,  /*"��Դ�쳣�¼�"*/
    E_OSCILLATOR_ERR_RCRD,  /*"��������¼�"*/
    E_MGMT_COMM_ERR_RCRD,  /*"����Ԫͨ���쳣"*/
    E_SYS_RST_RCRD,  /*"��λ�����¼�"*/
    E_RTC_ERR_EVNT_RCRD,  /*"ʱ�ӹ����¼�"*/
    E_NONLEGAL_UPGRADE_RCRD,  /*"����Ԫ����"*/
    E_NONLEGAL_REPLACE_RCRD,  /*"����Ԫ����"*/
    E_BROADCAST_JUST_TIME_RCRD,  /*"�㲥Уʱ�¼�"*/
    E_MS_ERR_EVNT_RCRD,  /*"����оƬ�����¼�"*/
    E_EVT_END_RCRD,  /*"�¼���������"*/
    E_MMT_FRZ_RCRD,  /*"˲ʱ����"*/
    E_MIN_FRZ_RCRD,  /*"���Ӷ���"*/
    E_HOUR_FRZ_RCRD,  /*"Сʱ����"*/
    E_DAY_FRZ_RCRD,  /*"�ն���"*/
    E_MONTH_BILL_FRZ_RCRD,  /*"�½���"*/
    E_MONTH_FRZ_RCRD,  /*"�¶���"*/
    E_LR_TYPE1_RCRD,  /*"���ɼ�¼Type1��¼"*/
    E_LR_TYPE2_RCRD,  /*"���ɼ�¼Type2��¼"*/
    E_LR_TYPE3_RCRD,  /*"���ɼ�¼Type3��¼"*/
    E_LR_TYPE4_RCRD,  /*"���ɼ�¼Type4��¼"*/
    E_STEP_BILL_FRZ_RCRD,  /*"������¼"*/
}EN_RCRD_FILE_ID;


/*"�Զ�������ģ�����ݱ�׼������"*/ 
 typedef enum 
 {
    E_PRODUCT_NOUSED = 0x8000,/*"�Զ���Ԥ��������"*/
    E_PRODUCT_ID1 = 0x8E00,/*"���ܱ�Ψһ��ʶ1"*/
    E_PRODUCT_ID2,/*"���ܱ�Ψһ��ʶ2"*/
    E_PRODUCT_ID3,/*"���ܱ�Ψһ��ʶ3"*/
    E_AUTO_READEXTDATA = 0x8FE0,/*"�Զ���������������"*/
    E_AUTO_WRTEXTDATA,/*"�Զ���������������"*/
 }EN_SMT_DATA_ID;


/*"��¼����ģ������������ݱ�׼������"*/
 typedef enum 
 {
    E_EVT_LST_START_NULL,  /*"��¼��ʼ�����б�"*/
    E_EVTLST_CLRMTR,  /*"�������"*/
    E_EVTLST_PD_S,  /*"�����¼���ʼ"*/
    E_EVTLST_PD_E,  /*"�����¼�����"*/
    E_EVTLST_JUSTTIME_S,  /*"Уʱ�¼���ʼ"*/
    E_EVTLST_JUSTTIME_E,  /*"Уʱ�¼�����"*/
    E_EVTLST_MTR_COVER_OPEN_S,  /*"������¼���ʼ"*/
    E_EVTLST_MTR_COVER_OPEN_E,  /*"������¼�����"*/
    E_EVTLST_RELAY_ERR_S,  /*"�̵���������ʼ"*/
    E_EVTLST_RELAY_ERR_E,  /*"�̵������Ͻ���"*/
    E_EVTLST_SOSCILLATOR_ERR_S,  /*"������������б�"*/
    E_EVTLST_SYS_RST_S,  /*"��λ�����¼�����"*/
    E_EVTLST_MGMT_ERR_S,  /*"����Ԫͨ���쳣��ʼ"*/
    E_EVTLST_MGMT_ERR_E,  /*"����Ԫͨ���쳣����"*/
    E_EVTLST_UPGRADE_S,  /*"�Ƿ�����Ԫ������ʼ"*/
    E_EVTLST_UPGRADE_E,  /*"�Ƿ�����Ԫ��������"*/
    E_EVTLST_NOLEG_REPALCE_S,  /*"�Ƿ�����Ԫ������ʼ"*/
    E_EVTLST_NOLEG_REPALCE_E,  /*"�Ƿ�����Ԫ��������"*/
    EVTLST_END,  /*"�¼��������"*/
    E_EVTLST_PWR_FAILURE_S,  /*"��Դ�쳣��ʼ"*/
    E_EVTLST_PWR_FAILURE_E,  /*"��Դ�쳣����"*/
    E_EVTLST_EVNT_CLR,  /*"�¼�����"*/
    E_EVTLST_RTC_ERR_S,  /*"ʱ�ӹ����¼���ʼ"*/
    E_EVTLST_MS_ERR_S,  /*"����оƬ�����¼���ʼ"*/
    E_EVTLST_MS_ERR_E,  /*"����оƬ�����¼�����"*/
    E_FRZ_LST_MON_FRZ,  /*"˲ʱ����"*/
    E_FRZ_LST_MIN_FRZ,  /*"���Ӷ���"*/
    E_FRZ_LST_HOUR_FRZ,  /*"Сʱ����"*/
    E_FRZ_LST_DAY_FRZ,  /*"�ն���"*/
    E_FRZ_LST_MOUTH_BILL_FRZ,  /*"�½��㶳��"*/
    E_FRZ_LST_MOUTH_FRZ,  /*"�¶���"*/
    E_FRZ_LST_LR_TYPE1,  /*"���ɼ�¼Type1"*/
    E_FRZ_LST_LR_TYPE2,  /*"���ɼ�¼Type2"*/
    E_FRZ_LST_LR_TYPE3,  /*"���ɼ�¼Type3"*/
    E_FRZ_LST_LR_TYPE4,  /*"���ɼ�¼Type4"*/
    E_FRZ_LST_LR_TYPE5,  /*"���ɼ�¼Type5"*/
}EN_FELXOBJ_ID;

extern const ST_FMT_ATTR stFormatList[65];

#endif