/*"*****************************************************************************"*/
/*"  FileName: class8_parament.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  �α������ļ�   "*/
/*"  Version:         "*/
/*"  V1.0       ��ʼ�汾"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h"
#include "app\userinterface\UI.h"

/*"*******************************************************************************"*/
/*�����õģ��������Ϊ0xffffffff�����ʾû��*/

INT8U Crypt_Type = 0;     /*���ܼ��� ע��Ҫ�����ת��*/  //0�����ģ�1������+MAC��2�����ģ�3������+MAC
/*"*******************************************************************************"*/
/*CLASS8����2֧���б� ֻ��*/
const ST_Parament_Object CLASS8OBISLIST_RD[]=
{
    {0x2015,0x01,E_SEQ_FLOWER_REPORT_STATUS,  TYPE_BITS_32},  /*���ܱ�����ϱ�״̬��*/
	{0x201e,0x01,E_SEQ_EVENT_START_TIME,      TYPE_DATES},    /*�¼�����ʱ��*/	
	{0x2020,0x01,E_SEQ_EVENT_END_TIME,        TYPE_DATES},    /*�¼�����ʱ��*/
	{0x2021,0x01,E_SEQ_FREEZE_TIME,           TYPE_DATES},    /*���ݶ���ʱ��*/
	{0x2022,0x01,E_SEQ_EVENT_RECORD_NUM,      TYPE_U32},      /*�¼���¼���*/
	{0x2023,0x01,E_SEQ_FREEZE_RECORD_NUM,     TYPE_U32},      /*�����¼���  */
	{0x2024,0x01,E_SEQ_EVENT_SRC_NUM,         TYPE_ENUM},     /*�¼�����Դ  */
	{0x2025,0x02,E_SEQ_EVENT_CUR_NUM,         ST_2_U32},      /*�¼���ǰֵ �¼������������¼��ۼ�ʱ�� */
	
	{0x3300,0x03,E_SEQ_EVENT_REPORT_STATUS,   TYPE_NULL},     /*�¼��ϱ�״̬ ͳһ��NULL��*/	
	{0x3320,0xff,E_SEQ_ADD_EVENT_REPORT_LIST, AR_OAD},        /*�����¼��б� MAX_IID�Ǹ��仯��*/ 
	
	{0x4014,0x0e,E_SEQ_CUR_YEAR_SLOT_TABLE,   AR_ST_3_YEARLY_TABLE},    /*��ǰ��ʱ����   ʱ������<=14 */
	{0x4016,0x08,E_SEQ_CUR_DAILY_SLOT_TABLE,  AR_ST_3_DAILY_SLOT_TABLE},/*��ǰ����ʱ�α� ��ʱ�α�����q��8�� */
	{0x4018,0x3f,E_SEQ_CUR_TRIFF_PRICE_TABLE, AR_TRIFF_U32},       /*��ǰ�׷��ʵ�� ��������k��63��*/
	{0x401a,0x06,E_SEQ_CUR_STEP_PRICE_TABLE,  TYPE_STEP_PRICE},    /*��ǰ�׽��ݵ�� ������<=6 */
	
	{0x4104,0x01,E_SEQ_REGU_VOLT,            TYPE_VIS_6}, /*���ѹ  */
	{0x4105,0x01,E_SEQ_REGU_CURRENT,         TYPE_VIS_6}, /*�������������  */
	{0x4106,0x01,E_SEQ_MAX_CURRENT,          TYPE_VIS_6}, /*������  */
	{0x4107,0x01,E_SEQ_ACT_ACCU_GRADE,       TYPE_VIS_4}, /*�й�׼ȷ�ȵȼ�  */
	{0x4109,0x01,E_SEQ_ACT_CONST,            TYPE_U32},   /*���ܱ��й�����  */
	{0x410b,0x01,E_SEQ_METER_TYPE,           TYPE_VIS_32},/*���ܱ��ͺ�  */
	{0x4111,0x01,E_SEQ_SOFT_RECORD_ID,       TYPE_VIS_16},/*���������  */
	{0x8001,0x01,E_SEQ_HOLD_POWER,           TYPE_ENUM},  /*���� */	
#ifdef THREE_PHASE			
	{0x4108,0x01,E_SEQ_R_ACT_ACCU_GRADE,  TYPE_VIS_4},  /*�޹�׼ȷ�ȵȼ�  */
	{0x410a,0x01,E_SEQ_R_ACT_CONST,       TYPE_U32},    /*���ܱ��޹�����  */
    {0xFF80,0x01,E_SEQ_DEMAND_TEMP_TIME,  ST_2_U16_U8},/*������ʱ���ݣ���ǰ��������������*/
    {0xFF81,0x01,E_SEQ_DEMAND_DATA ,      AR_U64},     /*������ʱ���ݣ���ǰƽ������ֵ����*/
    {0xFF82,0x01,E_SEQ_DEMAND_TEMP_VALUE, AR_6_U32},    /*������ʱ���ݣ���ʱ����*/
#endif	
    {0xF101,0x01,E_SEQ_SAFE_MODE_PARA,TYPE_ENUM},       /*��ȫģʽ����*/
    
    /*�Զ��������*/
    {0xFF20,0x01,E_SEQ_DISP_VERSION,        TYPE_U32},     /*�����ʾ�汾��  �ĳ�BCD */    
    {0xFF26,0x01,E_SEQ_CURRENT_TRIFF_NUM,   TYPE_U8},      /*��ǰ���ʺ�*/
    {0xFF27,0x01,E_SEQ_CURRENT_STEP_NUM,    TYPE_U8},      /*��ǰ���ݺ�*/
	{0xFF61,0x04,E_SEQ_PARAMENT_CRC,        AR_4_U32},     /*����CRCֵ */

	{0xFF60,0x04,E_SEQ_DL_LENGTH,         ST_3_U16},        /*��·�㳤��      */	
    
    {0xFF01,0x01,E_SEQ_METER_MODE,        TYPE_U8},         /*����ģʽ��*/
    {0xFF03,0x01,E_SEQ_RELAY_TYPE,        TYPE_U8},         /*�̵�������*/

    {0xFF04,0x01,E_SEQ_ALM_RELAY_OUT_MODE,TYPE_U32},       /*�����̵������ģʽ�� */
	{0xFF05,0x01,E_SEQ_ALM_BL_MODE,       TYPE_U32},        /*���ⱨ��ģʽ�� */

    {0xFF06,0x01,E_SEQ_WAI_RELAY_TIME,    TYPE_U16},      /*���ü̵������巽ʽ��բά��ʱ�䣨��λ:ms��*/
    {0xFF07,0x01,E_SEQ_WAI_RELAY_AGAIN,   TYPE_U16},      /*���ü̵������巽ʽ�ٴ���բ��������λ:Wh��*/

    {0xFF11,0x01,E_SEQ_EXTEND_MODE_2,     TYPE_U8},      /*������չģʽ��2*/
    {0xFF12,0x01,E_SEQ_EXTEND_MODE_3,     TYPE_U8},      /*������չģʽ��3*/
    {0xFF13,0x01,E_SEQ_EXTEND_MODE_4,     TYPE_U8},      /*������չģʽ��4*/
    {0xFF14,0x01,E_SEQ_EXTEND_MODE_5,     TYPE_U8},      /*������չģʽ��5*/
    {0xFF15,0x01,E_SEQ_EXTEND_MODE_6,     TYPE_U8},      /*������չģʽ��6*/
    {0xFF23,0x01,E_SEQ_PRODUCT_NUMBER_1,  TYPE_VIS_32},  /*����ʶ��Ψһ��1*/
    {0xFF24,0x01,E_SEQ_PRODUCT_NUMBER_2,  TYPE_VIS_32},  /*����ʶ��Ψһ��2*/
    {0xFF25,0x01,E_SEQ_PRODUCT_NUMBER_3,  TYPE_VIS_32},  /*����ʶ��Ψһ��3*/
    
	{0xFF40,0x01,E_SEQ_STATUS_FACTOR,     TYPE_U16},      /*����״̬  */
    {0xFF41,0x01,E_SEQ_TEST_RELAY,        TYPE_U8},       /*�̵�����������*/
    {0xFF42,0x01,E_SEQ_TEST_MULTI,        TYPE_U8},       /*�๦�ܶ���Ͷ�в���*/
    {0xFF43,0x01,E_SEQ_TEST_CHECK,        TYPE_U8},       /*�Լ����*/

    {0xFF62,0x01,E_SEQ_STATUS_FACTOR,     TYPE_BOOL},      /*����״̬  */
    {0xFF67,0x01,E_SEQ_SET_DEFAULT,       TYPE_BOOL},      /*�ָ�Ĭ�ϲ��� */

};

/*CLASS8����2֧���б� �ɶ�д*/
const ST_Parament_Object CLASS8OBISLIST_WR[]=
{	
	{0x202c,0x02,E_SEQ_CUR_PAY_FILE,      ST_2_U32},      /*(��ǰ��Ǯ���ļ� */
	{0x4000,0x01,E_SEQ_DATE_TIME,         TYPE_DATES},    /*����ʱ��  */
	{0x4001,0x01,E_SEQ_ADDR,              TYPE_OCTS_6},   /*ͨ�ŵ�ַ  */
	{0x4002,0x01,E_SEQ_METER_ID,          TYPE_OCTS_6},   /*���*/
	{0x4003,0x01,E_SEQ_USERID,            TYPE_OCTS_6},   /*�ͻ����*/
	{0x4004,0x03,E_SEQ_DEVICE_ADDR,       TYPE_LOCATION}, /*�豸����λ��*/
	{0x4005,0x01,E_SEQ_GROUP_ADDR,        AR_OCTS},       /*���ַ  */
	{0x4006,0x02,E_SEQ_CLK_SRC,           ST_2_EN},       /*ʱ��Դ  */
	{0x4007,0x07,E_SEQ_LCD_PARA,          TYPE_LCD_PARAMENT},/*LCD����  */
	{0x4008,0x01,E_SEQ_YEAR_SLOT_SWITCH_TIME,TYPE_DATES}, /*����ʱ�����л�ʱ��  */
	{0x4009,0x01,E_SEQ_DAILY_SLOT_SWITCH_TIME,TYPE_DATES},/*������ʱ���л�ʱ��  */
	{0x400a,0x01,E_SEQ_TRIFF_SWITCH_TIME, TYPE_DATES},    /*���׷�ʱ�����л�ʱ��  */
	{0x400b,0x01,E_SEQ_STEP_SWITCH_TIME, TYPE_DATES},     /*���׽��ݵ���л�ʱ��  */
	{0x400c,0x05,E_SEQ_SLOT_NUM,         ST_5_U8},        /*ʱ��ʱ����  */
	{0x400d,0x01,E_SEQ_STEP_NUM,         TYPE_U8},        /*������  */
	//{0x400f,0x01,E_SEQ_PASS_NUM,         TYPE_U8},        /*��Կ������  ������ں���ֻ���� */
	{0x4010,0x01,E_SEQ_METER_NUM,        TYPE_U8},        /*����Ԫ����  */
	{0x4011,0xFE,E_SEQ_HOLIDAY_NUM,      TYPE_HOLIDAY_TABLE},/*�������� ����������<=254 */
	{0x4012,0x01,E_SEQ_WEEK_TYPE,        TYPE_BITS_8},    /*������������  */
	{0x4013,0x01,E_SEQ_WEEK_SLOT_NUM,    TYPE_U8},        /*��������õ���ʱ�α��  */
	
	{0x4015,0x0e,E_SEQ_BAK_YEAR_SLOT_TABLE,AR_ST_3_YEARLY_TABLE},    /*������ʱ����  ʱ������<=14*/
	{0x4017,0x08,E_SEQ_BAK_DAILY_SLOT_TABLE,AR_ST_3_DAILY_SLOT_TABLE},/*��������ʱ�α� ��ʱ�α�����q��8)*/
	{0x4019,0x3f,E_SEQ_BAK_TRIFF_PRICE_TABLE,AR_TRIFF_U32},                  /*�����׷��ʵ��  ��������k��63��*/
	{0x401b,0x06,E_SEQ_BAK_STEP_PRICE_TABLE,TYPE_STEP_PRICE}, /*�����׽��ݵ��  ������<=6  */
	{0x401c,0x01,E_SEQ_CT,               TYPE_U32},        /*�������������*/
	{0x401d,0x01,E_SEQ_PT,               TYPE_U32},        /*��ѹ���������*/ 
	{0x401e,0x02,E_SEQ_ALARM_MONEY_LIMIT,ST_2_U32},        /*���������ֵ  */
	{0x401f,0x03,E_SEQ_OTHER_MONEY_LIMIT,ST_3_U32},        /*���������ֵ  */

	{0x4102,0x01,E_SEQ_PULSE_WIDTH,      TYPE_U8},         /*У��������  */
	{0x4103,0x01,E_SEQ_MANAGE_CODE,      TYPE_VIS_32},     /*�ʲ��������  */
	{0x4112,0x01,E_SEQ_ACTIVE_TYPE,      TYPE_BITS_8},     /*�й���Ϸ�ʽ������  */
	{0x4116,0x03,E_SEQ_BILL_DAY,         TYPE_BILL_DAY},   /*������       */
	{0x4401,0x01,E_SEQ_PASS,             TYPE_VIS_8},      /*��֤����  �Ƚ����⣬����ֻ��д�����ܶ�     */
    {0x8000,0x02,E_SEQ_REMOTE,           ST_2_U32_U16},    /*Զ�̿��Ʋ���        */
#ifdef THREE_PHASE	
	{0x4030,0x04,E_SEQ_VOLT_RATE_PARA,     ST_4_U16},               /*��ѹ�ϸ��ʲ���*/
	{0x4100,0x01,E_SEQ_MAX_DEMAND_TIME,    TYPE_U8},                /*�����������*/
	{0x4101,0x01,E_SEQ_SKIP_TIME,          TYPE_U8},                /*���ʻ���ʱ��  */
	{0x4113,0x01,E_SEQ_R1_ACTION_TYPE,     TYPE_BITS_8},            /*�޹���Ϸ�ʽ1������  */
	{0x4114,0x01,E_SEQ_R2_ACTION_TYPE,     TYPE_BITS_8},            /*�޹���Ϸ�ʽ2������  */
	{0x4117,0x01,E_SEQ_INTERVAL_DEMAND_TIME,TYPE_TI},               /*�ڼ�������������*/
	{0x4500,0x01,E_SEQ_SIGNLE,             TYPE_S16},               /*�ź�ǿ��      */
#endif
    {0xFF08,0x01,E_SEQ_SOFT_BAK_NUMBER,   TYPE_VIS_16},   /*���������(�Զ���ı���)*/
    {0xFF10,0x01,E_SEQ_EXTEND_MODE_1,     TYPE_U8},      /*������չģʽ��1*/
    {0xFF16,0x01,E_SEQ_EXTEND_MODE_7,     TYPE_U8},      /*������չģʽ��7*/
    {0xFF17,0x01,E_SEQ_EXTEND_MODE_8,     TYPE_U8},      /*������չģʽ��8*/
    {0xFF18,0x01,E_SEQ_EXTEND_MODE_3,     TYPE_U8},      /*������չģʽ��9*/
    {0xFF19,0x01,E_SEQ_EXTEND_MODE_4,     TYPE_U8},      /*������չģʽ��10*/
    {0xFF22,0x01,E_SEQ_SOFTWARE_VER,      TYPE_VIS_32},  /*����汾��(�Զ���)*/  
};  


/*CLASS8��������֧���б� �������£��Ƿ���ֻ���ģ����Ƕ����Զ�д�ģ�Ȼ�������½�ֻ���б�*/
const ST_Parament_SEPECIAL_Object CLASS8_OBISLIST_SPECIAL[]=
{
    {0x2015,0x04,E_SEQ_FLOW_REPORT_MODE,   0x01,READ_WRITE,TYPE_BITS_32},      /*���ܱ�����ϱ�ģʽ��*/
    {0x3320,0x03,E_SEQ_EVENT_NEED_REPORT_LIST,0x01,READ_ONLY,AR_N_OI},             /*���ϱ��¼������б�ֻ��*/
    {0x4000,0x03,E_SEQ_JUST_TIME_MODE,     0x01,READ_WRITE,TYPE_ENUM},         /*4000 Уʱģʽ*/
    {0x8000,0x04,E_SEQ_ALARM_STATUS,       0x01,READ_ONLY,TYPE_BITS_8},       /*�澯״̬��ֻ��*/
    {0x8000,0x05,E_SEQ_CMD_STATUS,         0x01,READ_ONLY,TYPE_BITS_8},       /*����״̬��ֻ��*/
    //{0x8001,0x03,E_SEQ_MAX_NO_COMM_TIME,   0x01,READ_WRITE,TYPE_U16},          /*��������վ�����ͨ��ʱ�������ӣ���0��ʾ���Զ�����*/
    //{0x8001,0x04,E_SEQ_AUTO_HOLD_TIME,     0x01,READ_WRITE,TYPE_U16},          /*�ϵ��Զ�����ʱ�������ӣ���0��ʾ�ϵ粻�Զ�����*/
    {0xf101,0x03,E_SEQ_SAFE_MODE_PARA,     0x01,READ_WRITE,AR_ST_2_OI_U16},    /*��ʽ��ȫģʽ����  ,��Ȼ���⴦����Ҳ����ɾ�����������ʾδ���塣*/
    {0x4500,0x09,E_SEQ_SIGNLE,             0x01,READ_WRITE,TYPE_S16},          /*�ź�ǿ��      */
};

/*"************************************************"*/
/*class8 ����8 OBIS�б�*/
const ST_PARAMENT_OAD_OBIS_INFO stCls8ObisInfo[] =
{
    {E_SEQ_FLOWER_REPORT_STATUS,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_EVTRPTSTATUS, E_INFO_NULL},   /*���ܱ�����ϱ�״̬��*/
	{E_SEQ_EVENT_START_TIME   ,0x01,E_SYS_TIME/*ֻ��������ʾ*/, 0, FORMAT_DATETIME, FUN_NULL, E_INFO_TIME},    /*�¼�����ʱ��*/	
	{E_SEQ_EVENT_END_TIME     ,0x01,E_SYS_TIME/*ֻ��������ʾ*/, 0, FORMAT_DATETIME, FUN_NULL, E_INFO_TIME},    /*�¼�����ʱ��*/
	{E_SEQ_FREEZE_TIME        ,0x01,E_SYS_TIME/*ֻ��������ʾ*/, 0, FORMAT_DATETIME, FUN_NULL, E_INFO_TIME},    /*���ݶ���ʱ��*/
#ifdef PM_MODLE  
	{E_SEQ_EVENT_RECORD_NUM   ,0x01,E_CT_RATIO/*ֻ��������ʾ*/, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},   /*�¼���¼��� ֻ��������ʾ*/
	{E_SEQ_FREEZE_RECORD_NUM  ,0x01,E_CT_RATIO/*ֻ��������ʾ*/, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},   /*�����¼��� ֻ��������ʾ */

	{E_SEQ_EVENT_CUR_NUM      ,0x01,E_CT_RATIO/*ֻ��������ʾ*/, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},   /*�¼���ǰֵ  ֻ��������ʾ*/
#endif
    {E_SEQ_ADD_EVENT_REPORT_LIST,E_RELATION_OBJECT_ID,E_OBIS_NULL,0,MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},/*�����¼��б� MAX_IID�Ǹ��仯��*/ 
	
#ifdef PM_MODLE  
	{E_SEQ_CUR_TRIFF_PRICE_TABLE,E_TRIFF_ID_PHASE,E_TARIFF_TABLE1, 0, FORMAT_PRICE, FUN_NULL, E_INFO_NOW_TRIFF_PRICE},        	/*��ǰ�׷��ʵ�� ��������k��63��*/

	{E_SEQ_CUR_STEP_PRICE_TABLE, 0x01,E_SETPVAULE_CURBLK, 0, FORMAT_STEPENGY, FUN_NULL, E_INFO_NOW_STEP_PRICE_MONEY}, /*��ǰ�׽��ݵ��� <=6 */
	{E_SEQ_CUR_STEP_PRICE_TABLE, 0x01,E_SETPPRICE_CURBLK, 0, FORMAT_PRICE, FUN_NULL, E_INFO_NOW_STEP_PRICE},  		  /*��ǰ�׽��ݵ�� <=7 */
	{E_SEQ_CUR_STEP_PRICE_TABLE, 0x01,E_STEP_YEARBILL_CURBLK, 0, FORMAT_YEARBILL, FUN_NULL, E_INFO_NOW_STEP},  	      /*��ǰ�׽��ݽ�����<=4 */
#endif    
	{E_SEQ_REGU_VOLT,       0x01,E_REGU_VOLT, 0, FORMAT_ASCII6, FUN_NULL, E_INFO_NULL}, /*���ѹ  */
	{E_SEQ_REGU_CURRENT,    0x01,E_REGU_CURR, 0, FORMAT_ASCII6, FUN_NULL, E_INFO_NULL}, /*�������������  */
	{E_SEQ_MAX_CURRENT,     0x01,E_MAX_CURR,  0, FORMAT_ASCII6, FUN_NULL, E_INFO_NULL}, /*������  */
	{E_SEQ_ACT_ACCU_GRADE,  0x01,E_ACT_ACCU_GRADE, 0, FORMAT_ASCII4, FUN_NULL, E_INFO_NULL}, /*�й�׼ȷ�ȵȼ�  */
	{E_SEQ_ACT_CONST,       0x01,E_A_PULSE_CONST, 0, FORMAT_CONST, FUN_NULL, E_INFO_CONST},   /*���ܱ��й�����  */
	{E_SEQ_METER_TYPE,      0x01,E_METER_TYPE, 0, FORMAT_ASCII32, FUN_NULL, E_INFO_NULL}, /*���ܱ��ͺ�  */
	{E_SEQ_SOFT_RECORD_ID,  0x01,E_SOFT_RECORD_ID, 0, FORMAT_ASCII16, FUN_NULL, E_INFO_NULL},     /*���������  */
	{E_SEQ_HOLD_POWER,      0x01,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*���� */	

#ifdef THREE_PHASE			
	{E_SEQ_R_ACT_ACCU_GRADE,0x01,E_NE_ACCU_GRADE, 0, FORMAT_ASCII4, FUN_NULL, E_INFO_NULL},      /*�޹�׼ȷ�ȵȼ�  */
	{E_SEQ_R_ACT_CONST,     0x01,E_R_PULSE_CONST, 0, FORMAT_CONST, FUN_NULL, E_INFO_R_CONST},    /*���ܱ��޹�����  */
#endif	

#ifdef PM_MODLE  
    {E_SEQ_SAFE_MODE_PARA,0x01,E_SAFE_MODE,0,MAX_FORMAT_ID,FUN_NULL,E_INFO_NULL},
#endif

/*---------------������ֻ���Ĳ���--------------------*/
//----------------------------------------------------------------
/*---------------�����ǿɶ�д�Ĳ���--------------------*/
#ifdef PM_MODLE  
	{E_SEQ_CUR_PAY_FILE,0x01,E_REMAIN_MONEY, 0, FORMAT_MONEY, FUN_NULL, E_INFO_REMAIN_MONEY},      /*(��ǰ��Ǯ���ļ�  ��ǰʣ����*/
	{E_SEQ_CUR_PAY_FILE,0x01,E_BUY_ENGY_CNT, 0, FORMAT_BUYTIMES, FUN_NULL, E_INFO_NULL},      /*(��ǰ��Ǯ���ļ�  �������*/
#endif
    {E_SEQ_DATE_TIME,0x01,E_SYS_TIME, 0, FORMAT_DATETIME, FUN_SET_TIME, E_INFO_NOW_TIME},    /*����ʱ��  */
    //{E_SEQ_DATE_TIME,0x01,E_SYS_TIME, 0, FORMAT_TIME, FUN_NULL, 0x13},        /*����ʱ��  */
	{E_SEQ_ADDR,0x01,E_COMM_ADDR, 0, FORMAT_ADDR_H, FUN_NULL, E_INFO_NULL},          /*ͨ�ŵ�ַ  */
	//{E_SEQ_ADDR,0x01,E_COMM_ADDR, 0, FORMAT_ADDR_L, FUN_NULL, 0x0B},          /*ͨ�ŵ�ַ  */
#ifdef PM_MODLE  
	{E_SEQ_METER_ID,0x01,E_METERID, 0, FORMAT_MTRNO_H, FUN_NULL, E_INFO_METER_NUM},       /*���*/
	{E_SEQ_METER_ID,0x01,E_METERID, 0, FORMAT_MTRNO_L, FUN_NULL, E_INFO_METER_NUM},       /*���*/
	{E_SEQ_USERID,0x01,E_USERID, 0, FORMAT_ADDR_H, FUN_NULL, E_INFO_CUSTOM_NUM},           /*�ͻ����*/	
#endif
	//{E_SEQ_USERID,0x01,E_USERID, 0, FORMAT_ADDR_L, FUN_NULL, 0x17},           /*�ͻ����*/
	{E_SEQ_DEVICE_ADDR,0x03,E_LONGITUDE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},   /*�豸����λ��*/
	{E_SEQ_GROUP_ADDR,0x01,E_GROUP_ADDR, 0, FORMAT_ADDR_H, FUN_NULL, E_INFO_NULL},   /*���ַ  */
	{E_SEQ_CLK_SRC,0x02,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, /*ʱ��Դ  */

    #if DEBUG_PARAMENT_SINGLE_OBIS
    {E_SEQ_LCD_PARA,0x01,E_PWRUP_DIS_ALL_TIME, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},         /*LCD����1 �ϵ�ȫ��ʱ�� */
	{E_SEQ_LCD_PARA,0x01,E_BACKLIGHT_SHOW_TIME, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},        /*LCD����2 �������ʱ�� */
	{E_SEQ_LCD_PARA,0x01,E_BACKLIGHT_SHOW_TIME_ON_VIEW, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},/*LCD����3 ��ʾ�鿴�������ʱ�� */
	{E_SEQ_LCD_PARA,0x01,E_PD_NOKEY_RETURN_TIME, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},       /*LCD����4 �޵簴����Ļפ�����ʱ�� */
	{E_SEQ_LCD_PARA,0x01,E_ENGY_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},             /*LCD����5 ��ʾ����С��λ�� */
	{E_SEQ_LCD_PARA,0x01,E_PWR_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},            /*LCD����5 ��ʾ���ʣ����������С��λ�� */
	{E_SEQ_LCD_PARA,0x01,E_DIS_SEG_MODE_CTRL_12, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},       /*LCD����7 Һ���٢���������  */
    #else
    {E_SEQ_LCD_PARA,0x01,E_PWRUP_DIS_ALL_TIME, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},         /*LCD����1 �ϵ�ȫ��ʱ�� */
	{E_SEQ_LCD_PARA,0x03,E_BACKLIGHT_SHOW_TIME, 0, FORMAT_WORD, FUN_LCDPARA, E_INFO_NULL},      /*LCD����2 �������ʱ�� */
	{E_SEQ_LCD_PARA,0x01,E_ENGY_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},             /*LCD����5 ��ʾ���ʣ����������С��λ�� */
	{E_SEQ_LCD_PARA,0x01,E_PWR_SHOW_POINT, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},             /*LCD����5 ��ʾ���ʣ����������С��λ�� */
	{E_SEQ_LCD_PARA,0x01,E_DIS_SEG_MODE_CTRL_12, 0, FORMAT_CTRLPARA, FUN_LCDPARA, E_INFO_NULL},       /*LCD����7 Һ���٢���������  */
    #endif
    
#ifdef PM_MODLE  
	{E_SEQ_TRIFF_SWITCH_TIME,0x01,E_TRSWITCH_TIME, 0, FORMAT_SWTIME, FUN_NULL, E_INFO_TIME},    /*���׷�ʱ�����л�ʱ��  */
	{E_SEQ_STEP_SWITCH_TIME,0x01,E_STEPSWITCH_TIME, 0, FORMAT_SWTIME, FUN_NULL, E_INFO_TIME},    /*���׽��ݵ���л�ʱ��  */
#endif
#ifdef PM_MODLE  
	{E_SEQ_STEP_NUM,0x01,E_STEP_NUM, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},    /*������  */
#endif
	{E_SEQ_METER_NUM,0x01,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*����Ԫ����  */
#ifdef PM_MODLE  
	{E_SEQ_BAK_TRIFF_PRICE_TABLE,E_TRIFF_ID_PHASE,E_TARIFF_TABLE2, 0, FORMAT_PRICE, FUN_NULL, E_INFO_TRIFF_PRICE},    /*�����׷��ʵ��  ��������k��63��*/
	{E_SEQ_BAK_STEP_PRICE_TABLE, 0x01,E_SETPVAULE_PASBLK, 0, FORMAT_STEPENGY, FUN_NULL, E_INFO_STEP_PRICE_MONEY},    /*�����׽��ݵ���  ������<=6  */
	{E_SEQ_BAK_STEP_PRICE_TABLE, 0x01,E_SETPPRICE_PASBLK, 0, FORMAT_PRICE, FUN_NULL, E_INFO_STEP_PRICE},       /*�����׽��ݵ��  <=7  */
	{E_SEQ_BAK_STEP_PRICE_TABLE, 0x01,E_STEP_YEARBILL_PASBLK, 0, FORMAT_YEARBILL, FUN_NULL, E_INFO_STEP},/*�����׽��ݽ�����  <=4 */
	{E_SEQ_CT,                   0x01,E_CT_RATIO, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},    /*�������������*/
	{E_SEQ_PT,                   0x01,E_PT_RATIO, 0, FORMAT_U4_UBCD4, FUN_NULL, E_INFO_NULL},    /*��ѹ���������*/ 
	{E_SEQ_ALARM_MONEY_LIMIT,0x01,E_ALRM_MONEY_THR1, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*���������ֵ �����������ֵ1*/
	{E_SEQ_ALARM_MONEY_LIMIT,0x01,E_ALRM_MONEY_THR2, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*���������ֵ �����������ֵ2 */
	{E_SEQ_OTHER_MONEY_LIMIT,0x01,E_OVERDRAW_MONEY_THR, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*���������ֵ ͸֧�����ֵ*/
	{E_SEQ_OTHER_MONEY_LIMIT,0x01,E_CORNER_MONEY_THR, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*���������ֵ �ڻ������ֵ */
	{E_SEQ_OTHER_MONEY_LIMIT,0x01,E_RELAY_CLOSE_MONEY_THR, 0, FORMAT_MONEY, FUN_NULL, E_INFO_NULL},    /*���������ֵ ��բ��������ֵ  */
#endif
	{E_SEQ_PULSE_WIDTH,0x01,E_PULSE_WIDTH, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},    /*У��������  */
	{E_SEQ_MANAGE_CODE,0x01,E_ASSET_NUM, 0, FORMAT_ASCII32, FUN_NULL, E_INFO_NULL},    /*�ʲ��������  */
	{E_SEQ_ACTIVE_TYPE,0x01,E_A_ENGY_COM_TYPE, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},    /*�й���Ϸ�ʽ������  */
	{E_SEQ_BILL_DAY,E_BILL_DATE_ID,E_MOUTH_BILLDAY, 0, FORMAT_BILLDATE, FUN_NULL, E_INFO_NULL},    /*������       */
	{E_SEQ_PASS,0x01,E_AUTHEN_PASSWORD, 0, FORMAT_ASCII8, FUN_NULL, E_INFO_NULL},    /*��֤����  �Ƚ����⣬����ֻ��д�����ܶ�     */
#ifdef PM_MODLE  
    {E_SEQ_REMOTE,0x01,E_RLY_TRIP_CTRL_CURR_THR, 0, FORMAT_P4, FUN_NULL, E_INFO_NULL},  /*�̵�����բ��������ֵ        */
    {E_SEQ_REMOTE,0x01,E_TRIPCURR_TIME, 0, FORMAT_WORD, FUN_NULL, E_INFO_NULL},         /*���������ޱ�����ʱʱ��        */
#endif

#ifdef THREE_PHASE
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTCHECK_UU_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*��ѹ�ϸ��ʲ���*/
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTCHECK_UL_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*��ѹ�ϸ��ʲ���*/
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTHEALTH_UU_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*��ѹ�ϸ��ʲ���*/
	{E_SEQ_VOLT_RATE_PARA,       0x01,E_VOLTHEALTH_UL_THR, 0, FORMAT_V, FUN_NULL, E_INFO_NULL},    /*��ѹ�ϸ��ʲ���*/

    {E_SEQ_MAX_DEMAND_TIME,      0x01,E_DEMAND_PERIOD, 0, FORMAT_WORD, FUN_SET_DU_PARA, E_INFO_NULL},                /*�����������*/
	{E_SEQ_SKIP_TIME,            0x01,E_DEMAND_SLID_PERIOD, 0, FORMAT_WORD, FUN_SET_DU_PARA, E_INFO_NULL},           /*��������ʱ��  */
	{E_SEQ_R1_ACTION_TYPE,       0x01,E_R_ENGY_COM_TYPE1, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},            /*�޹���Ϸ�ʽ1������  */
	{E_SEQ_R2_ACTION_TYPE,       0x01,E_R_ENGY_COM_TYPE2, 0, FORMAT_CTRLPARA, FUN_NULL, E_INFO_NULL},            /*�޹���Ϸ�ʽ2������  */
	{E_SEQ_INTERVAL_DEMAND_TIME, 0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*�ڼ�������������*/
	{E_SEQ_SIGNLE,               0x01,E_SIGNAL_STRENGTH, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},               /*�ź�ǿ��      */
    //--
    {E_SEQ_DEMAND_TEMP_TIME,     0x02,E_DEMAND_TEMP_SKIPTIMES_COUNT, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*������ʱ���ݣ���ǰ��������������*/
    {E_SEQ_DEMAND_DATA,          0x01,E_DEMAND_AV_POWER_BLK, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������ʱ���ݣ���ǰƽ������ֵ����*/
    {E_SEQ_DEMAND_TEMP_VALUE,    0x06,E_CURRENT_PA_DEMAND_TEMP, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},         /*������ʱ���� */
#endif    

    /*��������(AID)OAD OBIS�б�*/ 
    {E_SEQ_FLOW_REPORT_MODE, 0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*���ܱ�����ϱ�ģʽ��*/
    {E_SEQ_EVENT_NEED_REPORT_LIST,  0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, /*���ϱ��¼��б�*/ 
    {E_SEQ_JUST_TIME_MODE,   0x01,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*4000 Уʱģʽ*/
    {E_SEQ_ALARM_STATUS,     0x01,E_OBIS_NULL/*��ǰ����*/, 0, MAX_FORMAT_ID, FUN_RLYSTATUS, E_INFO_NULL},    /*�澯״̬��ֻ��*/
    {E_SEQ_CMD_STATUS,       0x01,E_OBIS_NULL/*��ǰ����*/, 0, MAX_FORMAT_ID, FUN_RLYSTATUS, E_INFO_NULL},    /*����״̬��ֻ��*/
    //{E_SEQ_MAX_NO_COMM_TIME, 0x01,E_FREQ/*�ݲ�֧��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*��������վ�����ͨ��ʱ�������ӣ���0��ʾ���Զ�����*/
    //{E_SEQ_AUTO_HOLD_TIME,   0x01,E_FREQ/*�ݲ�֧��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*�ϵ��Զ�����ʱ�������ӣ���0��ʾ�ϵ粻�Զ�����*/
    {E_SEQ_PRIO_SAFE_MODE_PARA,   0x01,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL}, /*��ʽ��ȫģʽ����*/

    /*�Զ���Э��OBIS�б�*/
	{E_SEQ_DL_LENGTH,   0x04,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},    /*��·�㳤��      */

    //{E_SEQ_METER_MODE,  0x01,E_METER_MODE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*����ģʽ��*/
    //{E_SEQ_RELAY_TYPE,  0x01,E_RELAY_TYPE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*�̵�������*/

    {E_SEQ_ALM_RELAY_OUT_MODE, 0x01,E_EVT_LCDALARM_MODE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},      /*�����̵������ģʽ��*/
    {E_SEQ_ALM_BL_MODE,  0x01,E_EVT_BGTALARM_MODE, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*���ⱨ��ģʽ��*/  

    //{E_SEQ_WAI_RELAY_TIME, 0x01,E_RELAY_ACT_TIME, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},          /*���ü̵������巽ʽ��բά��ʱ�䣨��λ:ms��*/ 
    //{E_SEQ_WAI_RELAY_AGAIN,0x01,E_RELAY_ACT_ENGY, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},          /*���ü̵������巽ʽ�ٴ���բ��������λ:Wh��*/ 
    {E_SEQ_SOFT_BAK_NUMBER,0x01,E_SOFT_RECORD_ID, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},          /*���������(�Զ���ı���)*/ 
    {E_SEQ_EXTEND_MODE_1,0x01,E_EXT_MODE1, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��1*/ 
    {E_SEQ_EXTEND_MODE_2,0x01,E_EXT_MODE2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��2*/ 
    {E_SEQ_EXTEND_MODE_3,0x01,E_EXT_MODE3, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��3*/ 
    {E_SEQ_EXTEND_MODE_4,0x01,E_EXT_MODE4, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��4*/ 
    {E_SEQ_EXTEND_MODE_5,0x01,E_EXT_MODE5, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��5*/ 
    {E_SEQ_EXTEND_MODE_6,0x01,E_EXT_MODE6, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��7*/ 
    {E_SEQ_EXTEND_MODE_7,0x01,E_EXT_MODE7, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��7*/ 
    {E_SEQ_EXTEND_MODE_8,0x01,E_EXT_MODE8, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��8*/ 
    {E_SEQ_EXTEND_MODE_9,0x01,E_EXT_MODE9, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*������չģʽ��9*/ 
    {E_SEQ_EXTEND_MODE_10,0x01,E_EXT_MODE10, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},           /*������չģʽ��10*/ 
    {E_SEQ_DISP_VERSION,0x01,E_OBIS_NULL/*�����⴦��*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},/*�����ʾ�汾��*/          
    {E_SEQ_SOFTWARE_VER,0x01,E_SOFT_VER_32, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},           /*�Զ�������汾��*/          
    {E_SEQ_PRODUCT_NUMBER_1,0x01,E_PRODUCT_ID1, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*����ʶ��Ψһ��1*/ 
    {E_SEQ_PRODUCT_NUMBER_2,0x01,E_PRODUCT_ID2, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*����ʶ��Ψһ��2*/ 
    {E_SEQ_PRODUCT_NUMBER_3,0x01,E_PRODUCT_ID3, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*����ʶ��Ψһ��3*/ 
    //{E_SEQ_CURRENT_STEP_NUM,0x01,E_CUR_STEPNUM, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*��ǰ���ݺ�*/ 
    {E_SEQ_STATUS_FACTOR,0x01,E_FAC_STATUS, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},            /*���ܱ���״̬*/ 
    {E_SEQ_TEST_RELAY,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*�̵�����������*/
    {E_SEQ_TEST_MULTI,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*�๦�ܶ���Ͷ�в���*/
    {E_SEQ_TEST_CHECK,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},                /*�Լ����*/
    {E_SEQ_PARAMENT_CRC,0x04,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},              /*����CRCֵ */
    
    {E_SEQ_SET_DEFAULT, 0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},              /*�ָ�Ĭ�ϲ��� */
    /*ע:���������ò�Ҫȥ�����Ա����к�����һ����ͬ���ڳ�����ʱ���쳣*/
    {E_MAX_SEQ_PARAMENT,0x01,E_OBIS_NULL, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},              /*�����һ�������ڵ����кţ���ֹ��������쳣��� */
};
/*"************************************************"*/

const ST_Parament_Object* const CLASS8OBISLIST[] =
{
    CLASS8OBISLIST_RD,
    CLASS8OBISLIST_WR,
};

const INT8U CLASS8_OI_NUM[]=
{
   ARRAY_SIZE(CLASS8OBISLIST_RD),
   ARRAY_SIZE(CLASS8OBISLIST_WR),
};    



/*****************************************************************************
 �� �� ��  : get_class_parament_Obis
 ��������  : ������OBIS��ȡ����
 �������  : INT8U seq_num                                   
             ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak        
             const ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_ser  
             INT8U list_num    :ȡһ���OBIS���ֵ����Ҫ��Ϊ�˼򻯹���                             
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 17
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U get_class_parament_Obis(INT8U seq_num, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak,
                                    const ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_ser,INT8U list_num)
{
    INT8U i,j,num_obis = 0;
    ST_PARAMENT_OAD_OBIS_INFO *pObisInfo = pObisInfo_bak;
    INT16U temp_obis = 0xffff;
        
    for(i=0;i<list_num;i++)
    {
        if(seq_num == pObisInfo_ser[i].Seq_Num)
        {
           /*list_num-i������ʣ����ô���*/
           for(j=0;j<list_num-i;j++)
           {       
               /*�������ݴ��������Ż�����OBIS�б�����ȥ��*/
               if(temp_obis!=pObisInfo_ser[i+j].Obis)
               { 
                   LIB_MemCpy((INT8U*)&pObisInfo_ser[i+j], (INT8U*)&pObisInfo[num_obis], sizeof(ST_PARAMENT_OAD_OBIS_INFO));
                   temp_obis = pObisInfo_ser[i+j].Obis;
                   num_obis++;
               }   

               /*2018-01-19:���´���ֹԽ��*/
               if(num_obis>=MAX_OBIS_NUM_ONE_OAD)
               {
                   return 0; 
               } 

               /*�����Ƚ���һ�������Ƿ��ж��*/
               if(seq_num != pObisInfo_ser[i+j+1].Seq_Num)  
               {
                   return num_obis;
                   //break;
               } 
           }

           /*ʵ�ʿ��Բ��ã�����ֻ��Ϊ�˿ɿ��� ��Ϊ������������һ���ģ�����Ҫ�����*/
           return num_obis;  
        }   
    }    

    
    return 0;

}


/*****************************************************************************
 �� �� ��  : get_class8_Obis
 ��������  : ��ȡ8���OBIS
 �������  : SUB_OBJECT *pObject  
             INT16U *pObis        
             ST_OBIS_ID *pObisId  
 �������  : ��
 �� �� ֵ  : j:OBISINFO�б����
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 11
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U get_class8_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
    INT8U num;
    
    num = get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                                stCls8ObisInfo,ARRAY_SIZE(stCls8ObisInfo));

    
    return num;
}    


/*8���¼�Ԫ�ز���*/
INT8U get_class8_object(SUB_OBJECT *pObject)
{
    INT8U  i,j,AID,IID;//,OI_NUM;
    INT16U OI;
    const ST_Parament_Object *pCLASS8_OBISLIST;

    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    /*1.�Ȳ��������OAD*/
    if(2<AID) /*����OAD��ֻ��AID��������2��*/
    {    
        for(i=0;i<ARRAY_SIZE(CLASS8_OBISLIST_SPECIAL);i++)
        {
            if((OI==CLASS8_OBISLIST_SPECIAL[i].OI)&&(AID==CLASS8_OBISLIST_SPECIAL[i].AID))
            {
                pObject->Class   = 8;
                if(IID>CLASS8_OBISLIST_SPECIAL[i].MAX_IID)
                {
                    return DAR_8_OVERFLOW;
                }    
                pObject->Right_RW= CLASS8_OBISLIST_SPECIAL[i].Right_RW;
                pObject->seq_num = CLASS8_OBISLIST_SPECIAL[i].Seq_Num;
                pObject->Pointer = CLASS8_OBISLIST_SPECIAL[i].Pointer;
                return DAR_0_OK;
            }    
        } 

        return DAR_4_OANODEF;
    }
    
    /*-----��������AID=2��1��OI����----*/
    
    /*2.���ҿɶ�д��OI*/ /*3.����ֻ����OI*/ /*����һ��*/
    for(j=0;j<ARRAY_SIZE(CLASS8OBISLIST);j++)
    {    
        pCLASS8_OBISLIST = CLASS8OBISLIST[j];
        
        for(i=0;i<CLASS8_OI_NUM[j];i++)
        {
            if(OI==pCLASS8_OBISLIST[i].OI)
            {
                pObject->Class   = 8;
                if(IID>pCLASS8_OBISLIST[i].MAX_IID)
                {
                    /*��ʾʱ��������ʱ�α�������������*/
                    if((OI_CURRENT_SLOT_TABLE == OI)||(OI_BAK_SLOT_TABLE == OI))
                    {
                        return DAR_17_YEARZONE;
                    } 
                    else if((OI_CURRENT_DAILY_SLOT_TABLE == OI)||(OI_BAK_DAILY_SLOT_TABLE == OI))
                    {
                        return DAR_18_DAILYSLOT;                        
                    }
                    else if((OI_CURRENT_TRIFF_PRICE == OI)||(OI_BAK_TRIFF_PRICE == OI))
                    {
                        return DAR_19_TRIFF;         
                    }
                    else
                    {    
                        return DAR_8_OVERFLOW;
                    }
                }  
                
                if(j==0)
                {
                    pObject->Right_RW= READ_ONLY;
                }
                else
                {     
                    pObject->Right_RW= READ_WRITE;
                }

                if(OI_CONNECT_PASS == OI)
                {
                    pObject->Right_RW= WRITE_ONLY;
                }  
                else if((OI_EVENT_START_TIME <= OI)&&(OI_EVENT_CURRENT_VALUE>=OI))
                {
                    pObject->Right_RW= NO_READ_WRITE;
                } 
                
                
                pObject->seq_num = pCLASS8_OBISLIST[i].Seq_Num;
                pObject->Pointer = pCLASS8_OBISLIST[i].Pointer;
                
                return DAR_0_OK;
            }    
        } 
    }

    return DAR_4_OANODEF;
}

/***************************�����ǰ�ȫģʽ����******************************/
/*"***************************************************************************************"*/ 
/*-------------------------------------------------------------------*/
/*Ĭ�ϰ�ȫģʽ����,����OI*/
const ST_F101_SAFE_MODE Default_OI_Safe_Mode[]=
{
   {0x202C,0x80,0x04},   /*����ǰ��Ǯ���ļ�*/
   {0x202F,0x41,0x80},   /*���ܱ������ϱ�*/
   {0x4000,0x81,0x84},   /*����ʱ��*/
   {0x4001,0x81,0x04},   /*ͨ�ŵ�ַ*/
   {0x4002,0x84,0x04},   /*���*/
   {0x4003,0x44,0x00},   /*�ͻ����*/
   {0x400A,0x41,0x00},   /*���׷�ʱ�����л�ʱ��*/
   {0x400B,0x41,0x00},   /*���׽����л�ʱ��*/
   {0x401C,0x41,0x00},   /*�������������*/
   {0x401D,0x41,0x00},   /*��ѹ���������*/
   {0x401E,0x41,0x00},   /*�����ֵ*/
   {0x4018,0x40,0x00},   /*��ǰ�׷��ʵ��*/
   {0x4019,0x41,0x00},   /*�����׷��ʵ��*/
   {0x401A,0x40,0x00},   /*��ǰ�׽��ݲ���*/
   {0x401B,0x41,0x00},   /*�����׽��ݲ���*/
   {0x4111,0x81,0x04},   /*������*/
   {0x5000,0x41,0x40},   /*˲ʱ����*/
   {0xF100,0x84,0x14},   /*ESAM*/
   {0xF101,0x81,0x24},   /*��ȫģʽ����*/
};


const ST_F101_SAFE_MODE Default_TYPE_Safe_Mode[]=
{
   {0x0fff,0x80,0x04},   /*��ǰ����0x0ZZZ 1��*/
   {0x1fff,0x40,0x00},   /*�������0x1ZZZ 2��*/
   {0x2fff,0x40,0x40},   /*����0x2ZZZ */
   {0x3fff,0x41,0x10},   /*�¼�0x3ZZZ */
   {0x4FFF,0x41,0x10},   /*�α���4ZZZ*/
   {0x50FF,0x41,0x10},   /*����50ZZ*/
   {0x80FF,0x41,0x10},   /*����80ZZ*/
   {0xF2FF,0x88,0x8f},   /*��������ӿ��豸F2ZZ*/
};

/*****************************************************************************
 �� �� ��  : add_oi_right
 ��������  : ���OI�İ�ȫģʽ
 �������  : ST_F101_SAFE_MODE *p_oi_right
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : ����һ 2017��4��17��
    ��    ��   : xyt
    �޸�����   : �����ɺ���
return:0���ɹ���-1:ʧ��
*****************************************************************************/
INT8S add_oi_right(ST_F101_SAFE_MODE *p_oi_right)
{
    INT8U  i;
    INT16U OI,OBIS;   
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];

    GetSingle(E_SAFE_MODE_LIST, (INT8U *) Array_Safe_Mode);

    OI=p_oi_right->Oi.asInt;
        
    // 1.�Ȳ����Ƿ��������OI,����У���ֱ�Ӵ���
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        if(Array_Safe_Mode[i].Oi.asInt== OI)
        {
           /*���ﱾ������Ҫ��һ��,��Ϊ��ʹ�ã��Լ���ʱ���÷����������ﲻ���ˣ��õ�ʱ��һ��*/
           Array_Safe_Mode[i].Rigth2 = p_oi_right->Rigth2;
           Array_Safe_Mode[i].Rigth3 = p_oi_right->Rigth3;
           //len = sizeof(ST_F101_SAFE_MODE);
           OBIS = E_SAFE_MODE_LIST+i+1;
           
           SetSingle(OBIS,(INT8U *)&Array_Safe_Mode[i],sizeof(ST_F101_SAFE_MODE));
            
           return 0;
        }
    }

    // 2.���û�У����ҵ�һ��ΪOXFFFF��
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        if(0xffff == Array_Safe_Mode[i].Oi.asInt)
        {
           Array_Safe_Mode[i].Oi.asInt= OI;	          
           Array_Safe_Mode[i].Rigth2 = p_oi_right->Rigth2;
           Array_Safe_Mode[i].Rigth3 = p_oi_right->Rigth3;
           
           OBIS = E_SAFE_MODE_LIST+i+1;
           SetSingle(OBIS,(INT8U *)&Array_Safe_Mode[i],sizeof(ST_F101_SAFE_MODE));
           
           return 0;
        }
    }

    // 3.�����Ӳ�������ʧ��
    return (-1);
}
/*****************************************************************************
 �� �� ��  : del_oi_right
 ��������  : ɾ��OI�İ�ȫģʽ
 �������  : ST_F101_SAFE_MODE *p_oi_right
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : ����һ 2017��4��17��
    ��    ��   : xyt
    �޸�����   : �����ɺ���
return:0���ɹ���-1:ʧ��
*****************************************************************************/
INT8S del_oi_right(ST_F101_SAFE_MODE *p_oi_right)
{
    INT8U  i;
    INT16U OI,OBIS; 
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];

    GetSingle(E_SAFE_MODE_LIST, (INT8U *) Array_Safe_Mode);

    OI=p_oi_right->Oi.asInt; 

    // 1.�Ȳ����Ƿ��������OI,����У���ɾ��
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        if(Array_Safe_Mode[i].Oi.asInt== OI)
        {
           Array_Safe_Mode[i].Oi.asInt= 0xffff; 
	       Array_Safe_Mode[i].Rigth2 = 0xff;
           Array_Safe_Mode[i].Rigth3 = 0xff;	
              
           OBIS = E_SAFE_MODE_LIST+i+1;
           
           SetSingle(OBIS,(INT8U *)&Array_Safe_Mode[i],sizeof(ST_F101_SAFE_MODE));
           
           return 0;
        }
    }

    return (-1);
}
/*****************************************************************************
 �� �� ��  : check_right_oi
 ��������  : ��������ģʽ�������Ƿ��д�Ȩ��
 �������  : INT8U ServiceType ��������
             INT8U Rigth2  ��ȡ����Ȩ�� �ο� 698Э�� ��¼��F.2����ȫģʽ����
             INT8U Rigth3  ��������Ȩ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : ����һ 2017��4��17��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

return:0,����ִ�У���0������ִ��
*****************************************************************************/
INT8S check_right_oi(INT8U ServiceType,INT8U Rigth2,INT8U Rigth3)
{
    INT8U temp_crypt_type;
    
    INT8U right_value;    

    if(EN_GETREQUEST ==ServiceType)        //��ȡ
    {
        right_value = Rigth2>>4;
    }
    else if(EN_SETREQUEST ==ServiceType)   //����
    {
        right_value = Rigth2&0x0f;
    }
    else if(EN_ACTIONREQUEST ==ServiceType)//����
    {
        right_value = Rigth3>>4;
    }
    else
    {

    }

    if(right_value&0x08)
    {
	  temp_crypt_type = 0;
    }
    else if(right_value&0x04)
    {
	  temp_crypt_type = 1;
    }
    else if(right_value&0x02)
    {
	  temp_crypt_type = 2;
    }	
    else if(right_value&0x01)
    {
	  temp_crypt_type = 3;
    }		
	else
	{
		temp_crypt_type = 4;  //0�������д
	}
	

    if(Crypt_Type>=temp_crypt_type)
    {
        return 0;
    }
    else
    {
        return (-1);
    }
}
/**********************************************************************************/
const INT16U NO_DIRECT_SET_OI[]={0x4002,0x4003,0x400a,0x400b,0x4019,0x401b,0x401c,0x401d,0x401e};

/*�������ֱ�Ӷ�������*/           /*"Ǯ��,  ����ʱ��, �¶��õ���,ͨ�ŵ�ַ, ���, ������"*/
const INT16U DIRECT_READ_OI_INFRARED[]={0x202C, 0x2031, 0x2032, 0x4000,0x4001, 0x4002, 0x4111};
/*****************************************************************************
 �� �� ��  : get_oi_right_type
 ��������  : ��ȡOI������Ȩ��
 �������  : ��
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : ����һ 2017��4��17��
    ��    ��   : xyt
    �޸�����   : �����ɺ���
return:0,����ִ�У���0������ִ��
*****************************************************************************/
INT8S get_oi_right_type(INT8U ServiceType, SUB_OBJECT *pObject)
{
    INT8U i;
    INT8U tmp;//,relayMode;

    INT8U OI_0;
    INT16U OI;

    INT8U Rigth2;
    INT8U Rigth3;
    INT16U OBIS;
 
    ST_F101_SAFE_MODE TmpSafe_Mode;
	    
    OI = pObject->Oad.Oi.asInt;    

    OI_0 = pObject->Oad.Oi.asByte[OIH]; //(OI&0xff00)>>8; //

#ifdef PM_MODLE  
	GetSingle(E_RELAY_TYPE,(INT8U *)&relayMode);        /*"������������բ����"*/
	if(relayMode & ENMWRELAY)                          /*"��������բģʽ�ִ�"*/
	{
		if((OI_REMOTE==OI)||(OI_REMAIN==OI))
		{
			return 0;	
		}
	}

    
    /*      ���ⰲȫ��֤ûͨ������������������뷽��      */
    if((EN_SETREQUEST == ServiceType)||(EN_ACTIONREQUEST == ServiceType))
    {    

    }
#endif    
	    
    /*�ж��Ƿ���һ�������һ���������ֱ�����ã�������ACTION*/
    if(EN_SETREQUEST == ServiceType)
    {    
        /*2017-08-02:���ⲻ׼�谲ȫģʽ*/		
        /*
		if(((OI_SAFE_MODE==OI)&&(2==AID))&&(FALSE == ChkStatus(E_FAC)))
		{
			 return (-1); 
		}	*/	
	
        for(i=0;i<sizeof(NO_DIRECT_SET_OI)/sizeof(INT16U);i++)
        {
            if(OI == NO_DIRECT_SET_OI[i])
            {
                return (-1);
            }    
        }    
    }
	else if(EN_GETREQUEST == ServiceType)
	{
		
	}

	/*���ܼ���Ϊ3����ûʲô���е��ˣ�ֱ�ӷ��سɹ�*/
	if(Crypt_Type>=3)
	{
		return 0;
	}

    // 1.�Ȳ�����ʾ��ȫģʽ
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        OBIS = E_SAFE_MODE_LIST+i+1;
        GetSingle(OBIS, (INT8U *)&TmpSafe_Mode);
        
        /*LIB_MemCpy((INT8U *)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara4Hex.SafeMode[i], 
                    (INT8U *) &TmpSafe_Mode, sizeof(ST_F101_SAFE_MODE));*/

        if(TmpSafe_Mode.Oi.asInt == OI)
        {
           /*����Ҫ��һ�£��Ա�ʹ����������ݶ�Ӧ*/
           Rigth2 = TmpSafe_Mode.Rigth3;
           Rigth3 = TmpSafe_Mode.Rigth2;
           
           return check_right_oi(ServiceType,Rigth2,Rigth3);
        }
    }

    // 2.����Ĭ��OI��ȫģʽ
     for(i=0;i<sizeof(Default_OI_Safe_Mode)/sizeof(ST_F101_SAFE_MODE);i++)
    {
        if(Default_OI_Safe_Mode[i].Oi.asInt == OI)
        {
           /*ע:���ﲻ�÷�*/
           Rigth2 = Default_OI_Safe_Mode[i].Rigth2;
           Rigth3 = Default_OI_Safe_Mode[i].Rigth3;
           
           return check_right_oi(ServiceType,Rigth2,Rigth3);
        }
    }

    // 3.����Ĭ���లȫģʽ
    //ת������OI��ǰ����ȡ�߰��ֽڣ����漸��ȡ���ֽ�
    tmp=((OI_0&0xf0)>>4);
    if(tmp<=4)
    {
        OI_0 = OI_0|0x0f;
    }

    for(i=0;i<sizeof(Default_TYPE_Safe_Mode)/sizeof(ST_F101_SAFE_MODE);i++)
    {			
        if(Default_TYPE_Safe_Mode[i].Oi.asByte[OIH] == OI_0)
        {
           /*ע:���ﲻ�÷�*/
           Rigth2 = Default_TYPE_Safe_Mode[i].Rigth2;
           Rigth3 = Default_TYPE_Safe_Mode[i].Rigth3;
           
           return check_right_oi(ServiceType,Rigth2,Rigth3);
        }
    }

    return 0;

}

/*****************************************************************************
 �� �� ��  : get_num_oi_safe_mode
 ��������  : ��ȡ��ʾ��ȫģʽ����
 �������  : INT8U *pdata  ���ݻ����ַ
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 05 23
    ��    ��   : x
    �޸�����   : �����ɺ���
    
�ݲ�֧��IID!=0�����Σ���Ϊ�����ж�IID��Ӧ�ĸ�(����м�ɾ��һ���Ļ�)
*****************************************************************************/
INT8U get_num_oi_safe_mode(INT8U *pdata)
{
    INT8U i;
    INT8U len = 0;
    
    ST_F101_SAFE_MODE TmpSafe_Mode;
    
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];

    GetSingle(E_SAFE_MODE_LIST, (INT8U *) Array_Safe_Mode);
    
    for(i=0;i<MAX_OI_NUM_SAFE_MODE;i++)
    {
        //GetSingle(E_SAFE_MODE_LIST+i, (INT8U *) &TmpSafe_Mode);
        //LIB_MemCpy((INT8U *)&StUIParaWithRamBackup.StCmParaWithRamBackup.StCmPara4Hex.SafeMode[i], 
        //           (INT8U *) &TmpSafe_Mode, sizeof(ST_F101_SAFE_MODE));
        LIB_MemCpy((INT8U *) &Array_Safe_Mode[i],(INT8U *) &TmpSafe_Mode, sizeof(ST_F101_SAFE_MODE));
        
        if(TmpSafe_Mode.Oi.asInt != 0xffff)
        {
           LIB_MemCpy((INT8U*)&TmpSafe_Mode,pdata,sizeof(ST_F101_SAFE_MODE));
           len += sizeof(ST_F101_SAFE_MODE);
           pdata += sizeof(ST_F101_SAFE_MODE);
        }
    }    

    return len;
}


/*****************************************************************************
 �� �� ��  : Reset_safe_mode
 ��������  : ��λ��ȫģʽ����
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2017 11 07
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
static void Reset_safe_mode(void)
{
    ST_F101_SAFE_MODE Array_Safe_Mode[MAX_OI_NUM_SAFE_MODE];
   
    LIB_MemSet(0xff, (INT8U*)Array_Safe_Mode,  MAX_OI_NUM_SAFE_MODE*sizeof(ST_F101_SAFE_MODE));  
 
    SetSingle(E_SAFE_MODE_LIST, (INT8U*)Array_Safe_Mode, MAX_OI_NUM_SAFE_MODE*sizeof(ST_F101_SAFE_MODE));    
}    


//return:0,ok,not 0:not ok
INT8U Action_safe_mode(INT8U AId,INT8U *Ioaddr,INT8U Iooffset)
{
    INT8U i;
	INT8U ret= DAR_0_OK;      

    if(1==AId)
    {
        /*
        ����1����λ��������
        ������=integer��0��
        ��λʱ���������3��
        */
        Reset_safe_mode();
    }
    else if(128==AId)
    {
        /*
        ����128��ɾ����ʽ��ȫģʽ�����������ʶ��
        �����ʶ��=OI
        */         

        if(0!=del_oi_right((ST_F101_SAFE_MODE *)Ioaddr))
        {
            ret = DAR_0_OK;
        }
    }
    else if((129==AId)||(127==AId)||(3==AId))
    {
        /*
        ����129������������ʽ��ȫģʽ������array ��ȫģʽ������
        ��ȫģʽ������=structure
        {
        �����ʶ    OI��
        ��ȫģʽ    long-unsigned
        }
        */           

        /*����������ã�����������а�ȫģʽ*/
        if(3 == AId)
        {
            Reset_safe_mode();
        }    
        /*����������ã�����������а�ȫģʽ END*/
        
        for(i=0;i<DIV_4(Iooffset);i++)
        {               
            if(0!=add_oi_right((ST_F101_SAFE_MODE *)(Ioaddr+MULTI_4(i))))
            {
               ret = DAR_FF_OTHERERR;
               break;
            }
        }
    }
    else
    {
        ret = DAR_5_OANOADAPT;
    }  
    
	return (ret);
   
}


/*****************************************************************************
 �� �� ��  : Get_safe_mode
 ��������  : ��ȡ��ʾ��ȫģʽ���ݣ���֧����IID����Ϊ���ɾ��˳��������ģ���
             ��IID��Ӧ�����ݲ���һһ��Ӧ��
 �������  : ST_OAD Oad    
             INT8U *pdata  
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 19
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *Get_safe_mode(ST_OAD Oad, INT8U *pdata)
{
    //INT8U ServiceType;
    //INT8U AId;
    INT8U i,len;
    INT8U array_num;
    ST_F101_SAFE_MODE tmp_L[MAX_OI_NUM_SAFE_MODE];
    INT8U *pdst = pdata;

    /*
    ����3����ʽ��ȫģʽ��������=array ��ȫģʽ����
    ��ȫģʽ������=structure
    {
      �����ʶ   OI��
      ��ȫģʽ   long-unsigned
    }
    */
           
    pdst   = EnCodeOAD(&Oad,pdst);//OAD
    *pdst++=0x01;	 /*��ʾ���ص�������data������DAR*/
    len = get_num_oi_safe_mode((INT8U*)&tmp_L);
    
    *pdst++ =array;
    if(0== len)
    {
        *pdst++=0;
    }
    else
    {
        /*����*/		        
        array_num = DIV_4(len);
        
        *pdst++ = array_num;

        for(i=0;i<array_num;i++)
        {
            //EncodeComplexData((INT8U *)C8_ATT3_F101,p);
            pdst += EnCode_Data((ST_ENCODE_INFO *)ST_2_OI_U16,(INT8U *)&tmp_L[i],pdst,0); 
        }
    }
    
    return pdst;
    
} 

#if DEBUG_PP_698_CODE
/*������������Ϊ��ǰ������дǰ��������*/
INT8U GetLcdPara(INT8U *pdata)
{
    if(CurOad.IId == 1)
    {    
        *pdata=60;
    } 

    return DAR_0_OK;
    
} 

INT8U setLcdPara(INT8U *pdata)
{
    ST_OAD Oad;
    
    GetSingle(E_PP_OAD,(INT8U *)&Oad);  
    
    if(Oad.IId == 1)
    {
        if(*pdata>=60)
        {
            return DAR_8_OVERFLOW;
        }    
    } 

    return DAR_0_OK;
    
} 
#endif

/*"************************************************"*/


