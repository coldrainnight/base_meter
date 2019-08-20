/*"***********************************************************		
FileName: pp_bois.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: Э�鴦�������ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/
#ifndef _PP_OBIS_H
#define _PP_OBIS_H
/********************************************************/
/*OI����*/
typedef enum
{    
    /*����������ʶ*/
    OI_ENGY_COM                   = 0x0000,       /*����й�����*/
    OI_PA_ENGY                    = 0x0010,       /*�����й�����*/
    OI_PA_ENGY_LA                 = 0x0011,       /*A�������й�����*/
    OI_PA_ENGY_LB                 = 0x0012,       /*B�������й�����*/
    OI_PA_ENGY_LC                 = 0x0013,       /*C�������й�����*/
    OI_NA_ENGY                    = 0x0020,       /*�����й�����*/
    OI_NA_ENGY_LA                 = 0x0021,       /*A�෴���й�����*/
    OI_NA_ENGY_LB                 = 0x0022,       /*B�෴���й�����*/
    OI_NA_ENGY_LC                 = 0x0023,       /*C�෴���й�����*/
    OI_R_ENGY_COM1                = 0x0030,       /*����޹�1����*/
    OI_R_ENGY_LA_COM1             = 0x0031,       /*A������޹�1����*/
    OI_R_ENGY_LB_COM1             = 0x0032,       /*B������޹�1����*/
    OI_R_ENGY_LC_COM1             = 0x0033,       /*C������޹�1����*/
    OI_R_ENGY_COM2                = 0x0040,       /*����޹�2����*/
    OI_R_ENGY_LA_COM2             = 0x0041,       /*A������޹�2����*/
    OI_R_ENGY_LB_COM2             = 0x0042,       /*B������޹�2����*/
    OI_R_ENGY_LC_COM2             = 0x0043,       /*C������޹�2����*/
    OI_R_ENGY_I                   = 0x0050,       /*��һ�����޹�����*/
    OI_R_ENGY_I_LA                = 0x0051,       /*A���һ�����޹�����*/
    OI_R_ENGY_I_LB                = 0x0052,       /*B���һ�����޹�����*/
    OI_R_ENGY_I_LC                = 0x0053,       /*C���һ�����޹�����*/
    OI_R_ENGY_II                  = 0x0060,       /*�ڶ������޹�����*/
    OI_R_ENGY_II_LA               = 0x0061,       /*A��ڶ������޹�����*/
    OI_R_ENGY_II_LB               = 0x0062,       /*B��ڶ������޹�����*/
    OI_R_ENGY_II_LC               = 0x0063,       /*C��ڶ������޹�����*/
    OI_R_ENGY_III                 = 0x0070,       /*���������޹�����*/
    OI_R_ENGY_III_LA              = 0x0071,       /*A����������޹�����*/
    OI_R_ENGY_III_LB              = 0x0072,       /*B����������޹�����*/
    OI_R_ENGY_III_LC              = 0x0073,       /*C����������޹�����*/
    OI_R_ENGY_IV                  = 0x0080,       /*���������޹�����*/
    OI_R_ENGY_IV_LA               = 0x0081,       /*A����������޹�����*/
    OI_R_ENGY_IV_LB               = 0x0082,       /*B����������޹�����*/
    OI_R_ENGY_IV_LC               = 0x0083,       /*C����������޹�����*/
    OI_PAP_ENGY                   = 0x0090,       /*�������ڵ���*/
    OI_PAP_ENGY_LA                = 0x0091,       /*A���������ڵ���*/
    OI_PAP_ENGY_LB                = 0x0092,       /*B���������ڵ���*/
    OI_PAP_ENGY_LC                = 0x0093,       /*C���������ڵ���*/
    OI_NAP_ENGY                   = 0x00A0,       /*�������ڵ���*/
    OI_NAP_ENGY_LA                = 0x00A1,       /*A�෴�����ڵ���*/
    OI_NAP_ENGY_LB                = 0x00A2,       /*B�෴�����ڵ���*/
    OI_NAP_ENGY_LC                = 0x00A3,       /*C�෴�����ڵ���*/
    /*����������ʶ END*/

    /*�������������ʶ*/
    OI_PA_MAX_DEMAND              = 0x1010,       /*�����й��������*/
    OI_PA_MAX_DEMAND_LA           = 0x1011,       /*A�������й��������*/
    OI_PA_MAX_DEMAND_LB           = 0x1012,       /*B�������й��������*/
    OI_PA_MAX_DEMAND_LC           = 0x1013,       /*C�������й��������*/
    OI_NA_MAX_DEMAND              = 0x1020,       /*�����й��������*/
    OI_NA_MAX_DEMAND_LA           = 0x1021,       /*A�෴���й��������*/
    OI_NA_MAX_DEMAND_LB           = 0x1022,       /*B�෴���й��������*/
    OI_NA_MAX_DEMAND_LC           = 0x1023,       /*C�෴���й��������*/
    OI_R_MAX_DEMAND_COM1          = 0x1030,       /*����޹�1�������*/
    OI_R_MAX_DEMAND_LA_COM1       = 0x1031,       /*A������޹�1�������*/
    OI_R_MAX_DEMAND_LB_COM1       = 0x1032,       /*B������޹�1�������*/
    OI_R_MAX_DEMAND_LC_COM1       = 0x1033,       /*C������޹�1�������*/
    OI_R_MAX_DEMAND_COM2          = 0x1040,       /*����޹�2�������*/
    OI_R_MAX_DEMAND_LA_COM2       = 0x1041,       /*A������޹�2�������*/
    OI_R_MAX_DEMAND_LB_COM2       = 0x1042,       /*B������޹�2�������*/
    OI_R_MAX_DEMAND_LC_COM2       = 0x1043,       /*C������޹�2�������*/
    OI_R_MAX_DEMAND_I             = 0x1050,       /*��һ�����������*/
    OI_R_MAX_DEMAND_I_LA          = 0x1051,       /*A���һ�����������*/
    OI_R_MAX_DEMAND_I_LB          = 0x1052,       /*B���һ�����������*/
    OI_R_MAX_DEMAND_I_LC          = 0x1053,       /*C���һ�����������*/
    OI_R_MAX_DEMAND_II            = 0x1060,       /*�ڶ������������*/
    OI_R_MAX_DEMAND_II_LA         = 0x1061,       /*A��ڶ������������*/
    OI_R_MAX_DEMAND_II_LB         = 0x1062,       /*B��ڶ������������*/
    OI_R_MAX_DEMAND_II_LC         = 0x1063,       /*C��ڶ������������*/
    OI_R_MAX_DEMAND_III           = 0x1070,       /*���������������*/
    OI_R_MAX_DEMAND_III_LA        = 0x1071,       /*A����������������*/
    OI_R_MAX_DEMAND_III_LB        = 0x1072,       /*B����������������*/
    OI_R_MAX_DEMAND_III_LC        = 0x1073,       /*C����������������*/
    OI_R_MAX_DEMAND_IV            = 0x1080,       /*���������������*/
    OI_R_MAX_DEMAND_IV_LA         = 0x1081,       /*A����������������*/
    OI_R_MAX_DEMAND_IV_LB         = 0x1082,       /*B����������������*/
    OI_R_MAX_DEMAND_IV_LC         = 0x1083,       /*C����������������*/
    OI_PAP_MAX_DEMAND             = 0x1090,       /*���������������*/
    OI_PAP_MAX_DEMAND_LA          = 0x1091,       /*A�����������������*/
    OI_PAP_MAX_DEMAND_LB          = 0x1092,       /*B�����������������*/
    OI_PAP_MAX_DEMAND_LC          = 0x1093,       /*C�����������������*/
    OI_NAP_MAX_DEMAND             = 0x10A0,       /*���������������*/
    OI_NAP_MAX_DEMAND_LA          = 0x10A1,       /*A�෴�������������*/
    OI_NAP_MAX_DEMAND_LB          = 0x10A2,       /*B�෴�������������*/
    OI_NAP_MAX_DEMAND_LC          = 0x10A3,       /*C�෴�������������*/
    /*�������������ʶEND*/
    
    /*�����ڼ��������������ʶ*/
    OI_FREEZE_PA_MAX_DEMAND              = 0x1110,       /*���������������й��������*/
    OI_FREEZE_PA_MAX_DEMAND_LA           = 0x1111,       /*A�ඳ�������������й��������*/
    OI_FREEZE_PA_MAX_DEMAND_LB           = 0x1112,       /*B�ඳ�������������й��������*/
    OI_FREEZE_PA_MAX_DEMAND_LC           = 0x1113,       /*C�ඳ�������������й��������*/
    OI_FREEZE_NA_MAX_DEMAND              = 0x1120,       /*���������ڷ����й��������*/
    OI_FREEZE_NA_MAX_DEMAND_LA           = 0x1121,       /*A�ඳ�������ڷ����й��������*/
    OI_FREEZE_NA_MAX_DEMAND_LB           = 0x1122,       /*B�ඳ�������ڷ����й��������*/
    OI_FREEZE_NA_MAX_DEMAND_LC           = 0x1123,       /*C�ඳ�������ڷ����й��������*/
    OI_FREEZE_R_MAX_DEMAND_COM1          = 0x1130,       /*������������޹�1�������*/
    OI_FREEZE_R_MAX_DEMAND_LA_COM1       = 0x1131,       /*��������A������޹�1�������*/
    OI_FREEZE_R_MAX_DEMAND_LB_COM1       = 0x1132,       /*��������B������޹�1�������*/
    OI_FREEZE_R_MAX_DEMAND_LC_COM1       = 0x1133,       /*��������C������޹�1�������*/
    OI_FREEZE_R_MAX_DEMAND_COM2          = 0x1140,       /*������������޹�2�������*/
    OI_FREEZE_R_MAX_DEMAND_LA_COM2       = 0x1141,       /*��������A������޹�2�������*/
    OI_FREEZE_R_MAX_DEMAND_LB_COM2       = 0x1142,       /*��������B������޹�2�������*/
    OI_FREEZE_R_MAX_DEMAND_LC_COM2       = 0x1143,       /*��������C������޹�2�������*/
    OI_FREEZE_R_MAX_DEMAND_I             = 0x1150,       /*�������ڵ�һ�����������*/
    OI_FREEZE_R_MAX_DEMAND_I_LA          = 0x1151,       /*��������A���һ�����������*/
    OI_FREEZE_R_MAX_DEMAND_I_LB          = 0x1152,       /*��������B���һ�����������*/
    OI_FREEZE_R_MAX_DEMAND_I_LC          = 0x1153,       /*��������C���һ�����������*/
    OI_FREEZE_R_MAX_DEMAND_II            = 0x1160,       /*�������ڵڶ������������*/
    OI_FREEZE_R_MAX_DEMAND_II_LA         = 0x1161,       /*��������A��ڶ������������*/
    OI_FREEZE_R_MAX_DEMAND_II_LB         = 0x1162,       /*��������B��ڶ������������*/
    OI_FREEZE_R_MAX_DEMAND_II_LC         = 0x1163,       /*��������C��ڶ������������*/
    OI_FREEZE_R_MAX_DEMAND_III           = 0x1170,       /*�������ڵ��������������*/
    OI_FREEZE_R_MAX_DEMAND_III_LA        = 0x1171,       /*��������A����������������*/
    OI_FREEZE_R_MAX_DEMAND_III_LB        = 0x1172,       /*��������B����������������*/
    OI_FREEZE_R_MAX_DEMAND_III_LC        = 0x1173,       /*��������C����������������*/
    OI_FREEZE_R_MAX_DEMAND_IV            = 0x1180,       /*�������ڵ��������������*/
    OI_FREEZE_R_MAX_DEMAND_IV_LA         = 0x1181,       /*��������A����������������*/
    OI_FREEZE_R_MAX_DEMAND_IV_LB         = 0x1182,       /*��������B����������������*/
    OI_FREEZE_R_MAX_DEMAND_IV_LC         = 0x1183,       /*��������C����������������*/
    OI_FREEZE_PAP_MAX_DEMAND             = 0x1190,       /*�����������������������*/
    OI_FREEZE_PAP_MAX_DEMAND_LA          = 0x1191,       /*��������A�����������������*/
    OI_FREEZE_PAP_MAX_DEMAND_LB          = 0x1192,       /*��������B�����������������*/
    OI_FREEZE_PAP_MAX_DEMAND_LC          = 0x1193,       /*��������C�����������������*/
    OI_FREEZE_NAP_MAX_DEMAND             = 0x11A0,       /*�������ڷ��������������*/
    OI_FREEZE_NAP_MAX_DEMAND_LA          = 0x11A1,       /*��������A�෴�������������*/
    OI_FREEZE_NAP_MAX_DEMAND_LB          = 0x11A2,       /*��������B�෴�������������*/
    OI_FREEZE_NAP_MAX_DEMAND_LC          = 0x11A3,       /*��������C�෴�������������*/   
    /*�������������ʶEND*/

    /*����������ʶ*/
    OI_VOLTAGE                    = 0x2000,       /*��ѹ*/
    OI_CURRENT                    = 0x2001,       /*����*/
    OI_VOLT_ANGLE                 = 0x2002,       /*��ѹ���*/
    OI_CURR_ANGLE                 = 0x2003,       /*�������*/
    OI_ACTIVE_POWER               = 0x2004,       /*�й�����*/
    OI_REACTIVE_POWER             = 0x2005,       /*�޹�����*/
    OI_VISUAL_POWER               = 0x2006,       /*���ڹ���*/
    OI_AVERAGE_ACTIVE_POWER_MIN   = 0x2007,       /*һ����ƽ���й�����*/        
    OI_AVERAGE_REACTIVE_POWER_MIN = 0x2008,       /*һ����ƽ���޹�����*/
    OI_AP_PWR_MIN_AVER            = 0x2009,       /*һ����ƽ�����ڹ���*/ 
    OI_FACTOR                     = 0x200A,       /*��������*/
    OI_POWER_FREQ                 = 0x200F,       /*����Ƶ��*/
    OI_TEMPERATURE                = 0x2010,       /*�����¶�*/
    OI_RTC_POWER_VOL              = 0x2011,       /*ʱ�ӵ�ص�ѹ*/
    OI_LOW_POWER_VOL              = 0x2012,       /*ͣ�糭���ص�ѹ*/
    OI_RTC_POWER_TIME             = 0x2013,       /*ʱ�ӵ�ع���ʱ��*/
    OI_STATUS_METER               = 0x2014,       /*�������״̬��*/
    OI_REPORT_METER               = 0x2015,       /*���ܱ������ϱ�*/ 
    OI_CURRENT_P_DEMAND           = 0x2017,       /*��ǰ�й�����*/
    OI_CURRENT_R_DEMAND           = 0x2018,       /*��ǰ�޹�����*/
    OI_CURRENT_VISUAL_DEMAND      = 0x2019,       /*��ǰ��������*/ 
    OI_PRICE_NOW                  = 0x201A,       /*��ǰ���*/ 
    OI_TARIFF_PRICE_NOW           = 0x201B,       /*��ǰ���ʵ��*/ 
    OI_STEP_PRICE_NOW             = 0x201C,       /*��ǰ���ݵ��*/ 
    OI_EVENT_START_TIME           = 0x201E,       /*�¼�����ʱ��*/ 
    OI_EVENT_END_TIME             = 0x2020,       /*�¼�����ʱ��*/ 
    OI_DATA_FREEZE_TIME           = 0x2021,       /*���ݶ���ʱ��*/ 
    OI_EVENT_RECORD_NUM           = 0x2022,       /*�¼���¼���*/ 
    OI_FREEZE_RECORD_NUM          = 0x2023,       /*�����¼���*/ 
    OI_EVENT_SOURCE               = 0x2024,       /*�¼�����Դ*/ 
    OI_EVENT_CURRENT_VALUE        = 0x2025,       /*�¼���ǰֵ:�¼���������,�¼��ۼ�ʱ��*/ 
    OI_VOLT_IMBALANCE_RATE        = 0x2026,       /*��ѹ��ƽ����*/ 
    OI_CURR_IMBALANCE_RATE        = 0x2027,       /*������ƽ����*/ 
    OI_REROAD_RATE                = 0x2028,       /*������*/ 
    OI_VA                         = 0x2028,       /*��ʱֵ*/ 
    OI_CURRENT_MONEY_PAG          = 0x202C,       /*����ǰ��Ǯ���ļ�:ʣ����������*/ 
    OI_REMAIN_MONEY_NEGA          = 0x202D,       /*����ǰ��͸֧���*/
    OI_TOTAL_BUY_MONEY            = 0x202E,       /*�ۼƹ�����*/
    OI_ENGY_CONSUMPTION_MONTH     = 0x2031,       /*�¶��õ���*/
    OI_ENGY_CONSUMPTION_STEP_BILL = 0x2032,       /*���ݽ����õ���*/    
    OI_STATUS_CTR_CMD             = 0x2040,       /*��������ִ��״̬��*/
    OI_ERR_CTR_CMD                = 0x2041,       /*�����������״̬��*/
    OI_STATUS_CTR_CMD_NEW         = 0x2042,       /*�ϲ���Ŀ�������״̬��*/
    //OI_VOLT_STAT_RT               = 0x2130,       /*�ܵ�ѹ�ϸ��� ----��û��*/
    OI_VOLT_STAT_A_RT             = 0x2131,       /*A���ѹ�ϸ���*/
    OI_VOLT_STAT_B_RT             = 0x2132,       /*B���ѹ�ϸ���*/
    OI_VOLT_STAT_C_RT             = 0x2133,       /*C���ѹ�ϸ���*/
    OI_DAY_MAX_DEMAND_TIME        = 0x2140,       /*������й����ʼ�����ʱ��*/
    OI_MONTH_MAX_DEMAND_TIME      = 0x2141,       /*������й����ʼ�����ʱ��*/
    /*����������ʶEND*/

    /*�¼�������ʶ*/
    OI_LOST_VOLT_EVENT            = 0x3000,       /*���ܱ�ʧѹ�¼�*/
    OI_DOWN_VOLT_EVENT            = 0x3001,       /*���ܱ�Ƿѹ�¼�*/
    OI_VOER_VOLT_EVENT            = 0x3002,       /*���ܱ��ѹ�¼�*/
    OI_BREAKE_PHASE_EVENT         = 0x3003,       /*���ܱ�����¼�*/
    OI_LOST_CURR_EVENT            = 0x3004,       /*���ܱ�ʧ���¼�*/
    OI_OVER_CURR_EVENT            = 0x3005,       /*���ܱ�����¼�*/
    OI_BREAKE_CURR_EVENT          = 0x3006,       /*���ܱ�����¼�*/
    OI_PWR_REVERSE_EVENT          = 0x3007,       /*���ܱ��ʷ����¼�*/
    OI_OVER_PWR_EVENT             = 0x3008,       /*���ܱ�����¼�*/    
    OI_OVER_PA_DEMAND_EVENT       = 0x3009,       /*���ܱ������й����������¼�*/
    OI_OVER_NA_DEMAND_EVENT       = 0x300A,       /*���ܱ����й����������¼�*/
    OI_OVER_R_DEMAND_EVENT        = 0x300B,       /*���ܱ��޹����������¼�*/
    OI_FACTOR_LOW_EVENT           = 0x300C,       /*���ܱ��������������¼�*/
    OI_ALL_LOST_VOL_EVENT         = 0x300D,       /*ȫʧѹ�¼�*/    
    OI_AUX_POWER_DOWN_EVENT       = 0x300E,       /*���ܱ�����Դ�����¼�*/
    OI_VOLT_REVERSE_EVENT         = 0x300F,       /*���ܱ��ѹ�������¼�*/
    OI_CURRENT_REVERSE_EVENT      = 0x3010,       /*���ܱ�����������¼�*/
    OI_METER_POWER_DOWN_EVENT     = 0x3011,       /*���ܱ�����¼�*/     
    OI_PRG_EVENT                  = 0x3012,       /*���ܱ����¼�*/ 
    OI_MTR_CLR_EVENT              = 0x3013,       /*���ܱ������¼�*/ 
    OI_DEMAND_CLR_EVENT           = 0x3014,       /*���ܱ����������¼�*/ 
    OI_EVENT_CLR_EVENT            = 0x3015,       /*���ܱ��¼������¼�*/ 
    OI_JUST_TIME_EVENT            = 0x3016,       /*���ܱ�Уʱ�¼�*/ 
    OI_DAY_TBL_PRG_EVENT          = 0x3017,       /*���ܱ�ʱ�α����¼�*/ 
    OI_SEASON_TBL_PRG_EVENT       = 0x3018,       /*���ܱ�ʱ�������¼�*/ 
    OI_WEEKEND_PRG_EVENT          = 0x3019,       /*���ܱ������ձ���¼�*/    
    OI_BILLING_TIME_PRG_EVENT     = 0x301A,       /*���ܱ�����ձ���¼�*/ 
    OI_MTR_COVER_OPEN_EVENT       = 0x301B,       /*���ܱ����¼�*/ 
    OI_TAIL_COVER_OPEN_EVENT      = 0x301C,       /*���ܱ���ť���¼�*/ 
    OI_VOLT_IMBALANCE_EVENT       = 0x301D,       /*���ܱ��ѹ��ƽ���¼�*/ 
    OI_CURR_IMBALANCE_EVENT       = 0x301E,       /*���ܱ������ƽ���¼�*/
    OI_RELAY_TRIP_EVENT           = 0x301F,       /*���ܱ���բ�¼�*/ 
    OI_RELAY_CLOSE_EVENT          = 0x3020,       /*���ܱ��բ�¼�*/ 
    OI_HOLIDAY_PRG_EVENT          = 0x3021,       /*���ܱ�ڼ��ձ���¼�*/ 
    OI_A_ENGY_COM_PRG_EVENT       = 0x3022,       /*���ܱ��й���Ϸ�ʽ����¼�*/ 
    OI_R_ENGY_COM_PRG_EVENT       = 0x3023,       /*���ܱ��޹���Ϸ�ʽ����¼�*/
    OI_TARIFF_PRG_EVENT           = 0x3024,       /*���ܱ���ʲ��������¼�*/ 
    OI_STEP_PRG_EVENT             = 0x3025,       /*���ܱ���ݱ����¼�*/ 
    OI_KEY_UPDATE_EVENT           = 0x3026,       /*���ܱ���Կ�����¼�*/ 
    OI_CARD_PLUG_IN_ERR_EVENT     = 0x3027,       /*���ܱ��쳣�忨�¼�*/ 
    OI_BUY_ENGY_EVENT             = 0x3028,       /*���ܱ����¼*/ 
    OI_REFUND_EVENT               = 0x3029,       /*���ܱ��˷Ѽ�¼*/ 
    OI_CONSTANT_MAGNET_EVENT      = 0x302A,       /*���ܱ�㶨�ų������¼�*/
    OI_RELAY_ERR_EVENT            = 0x302B,       /*���ܱ��ɿ��������¼�*/
    OI_PWR_FAILURE_EVENT          = 0x302C,       /*���ܱ��Դ�쳣�¼�*/ 
    OI_CURR_IMBALANCE_BADLY_EVENT = 0x302D,       /*���ܱ�������ز�ƽ���¼�*/ 
    OI_RTC_ERR_EVENT              = 0x302E,       /*���ܱ�ʱ�ӹ����¼�*/
    OI_MEASURE_CHIP_ERR_EVENT     = 0x302F,       /*���ܱ����оƬ�����¼�*/
    /*�¼�������ʶ END*/

    OI_EVENT_REPORT_STATUS        = 0x3300,       /*�¼��ϱ�״̬*/
    /*�¼���¼��Ԫ*/
    OI_STARDARD_EVENT_RECORD_UINT = 0x3301,       /*��׼�¼���¼��Ԫ*/
    OI_PRG_RECORD_EVENT_UINT      = 0x3302,       /*��̼�¼�¼���Ԫ*/
    OI_OVER_DEMAND_EVENT_UINT     = 0x3308,       /*���ܱ����������¼���Ԫ*/
    OI_POWER_ON_OFF_EVENT_UINT    = 0x3309,       /*ͣ/�ϵ��¼���¼��Ԫ*/
    OI_EVENT_CLR_EVENT_UINT       = 0x330C,       /*�¼������¼���¼��Ԫ*/
    OI_CARD_PLUG_IN_ERR_EVENT_UINT= 0x3310,       /*�쳣�忨�¼���¼��Ԫ*/
    OI_REFUND_EVENT_UINT          = 0x3311,       /*�˷��¼���¼��Ԫ*/
    OI_CHANGE_MODE_EVENT_UINT     = 0x3312,       /*ͨ��ģ�����¼���Ԫ*/    
    OI_DAY_TBL_PRG_EVENT_UINT     = 0x3314,       /*���ܱ�ʱ�α����¼���¼��Ԫ*/
    OI_HOLIDAY_PRG_EVENT_UINT     = 0x3315,       /*���ܱ�ڼ��ձ���¼���¼��Ԫ*/
    /*�¼���¼��ԪEND*/
    OI_NEW_NEED_RPT_EVT_LIST      = 0x3320,       /*�����¼��б�*/  
    
    OI_DATE_TIME                  = 0x4000,       /*����ʱ��*/
    OI_METER_ADDR                 = 0x4001,       /*ͨ�ŵ�ַ*/
    OI_METER_ID                	  = 0x4002,       /*���*/
    OI_USE_ID                	  = 0x4003,       /*�û���*/
    OI_POSITION                	  = 0x4004,       /*�豸��ַλ��*/
    OI_GROUP_ADDR                 = 0x4005,       /*���ַ*/
    OI_CLOCK_SOURCE               = 0x4006,       /*ʱ��Դ*/
    OI_LCD_PARAMENT               = 0x4007,       /*LCD����*/
    OI_SLOT_SWITCH_TIME           = 0x4008,       /*����ʱ�����л�ʱ��*/
    OI_DAILY_SLOT_SWITCH_DATE     = 0x4009,       /*������ʱ���л�ʱ��*/
    OI_TRIFF_SWITCH_TIME          = 0x400A,       /*���׷�ʱ�����л�ʱ��*/
    OI_STEP_PRICE_SWITCH_TIME     = 0x400B,       /*���׽��ݵ���л�ʱ��*/
    OI_SLOT_PARAMENT              = 0x400c,       /*ʱ��ʱ����*/
    OI_STEP_PRICE_NUM             = 0x400D,       /*������*/   
    OI_METER_NUMBER               = 0x4010,       /*"����Ԫ����"*/
    OI_PUBLIC_HOLIDAY             = 0x4011,       /*�������ձ�*/
    OI_WEEKED_STATUS              = 0x4012,       /*������������*/
    OI_CURRENT_SLOT_TABLE         = 0x4014,       /*��ǰ��ʱ����*/
    OI_BAK_SLOT_TABLE             = 0x4015,       /*������ʱ����*/
    OI_CURRENT_DAILY_SLOT_TABLE   = 0x4016,       /*��ǰ����ʱ�α�*/
    OI_BAK_DAILY_SLOT_TABLE       = 0x4017,       /*��������ʱ�α�*/
    OI_CURRENT_TRIFF_PRICE        = 0x4018,       /*��ǰ�׷��ʵ��*/
    OI_BAK_TRIFF_PRICE            = 0x4019,       /*�����׷��ʵ��*/
    OI_CURRNET_STEP_PRICE         = 0x401A,       /*��ǰ�׽��ݵ��*/
    OI_BAK_STEP_PRICE             = 0x401B,       /*�����׽��ݵ��*/
    OI_SET_PT                     = 0x401C,       /*�������������*/
    OI_SET_CT                     = 0x401D,       /*��ѹ���������*/
    OI_ALARM_MONEY_LIMIT          = 0x401E,       /*�����������*/
    OI_OTHER_MONEY_LIMIT          = 0x401F,       /*�����������*/
    OI_CARD_PLUG_IN_STATUS        = 0x4022,       /*�忨״̬��*/
    
    OI_VOLT_RATE_STATUS           = 0x4030,       /*��ѹ�ϸ��ʲ���*/
    
    OI_DEMAND_PERIOD              = 0x4100,       /*��������*/
    OI_DEMAND_SKIP_PERIOD         = 0x4101,       /*����ʱ��*/
    OI_JUST_METER_PULSE_WIDTH     = 0x4102,       /*У��������*/
    OI_MANAGE_COD                 = 0x4103,       /*�ʲ��������*/
    OI_RATED_VOLT                 = 0x4104,       /*���ѹ*/
    OI_RATED_CURR                 = 0x4105,       /*���������������*/
    OI_MAX_CURR                   = 0x4106,       /*�����*/
    OI_A_PRECISE                  = 0x4107,       /*�й�׼ȷ�ȵȼ�*/
    OI_R_PRECISE                  = 0x4108,       /*�޹�׼ȷ�ȵȼ�*/    
    OI_ACTIVE_PULSE_CONSTANT      = 0x4109,       /*���ܱ��й�����*/
    OI_REACTIVE_PULSE_CONSTANT    = 0x410A,       /*���ܱ��޹�����*/
    OI_METER_TYPE                 = 0x410B,       /*���ܱ��ͺ�*/
    
    OI_SOFT_NUMBER_REGISTER       = 0x4111,       /*���������*/
    OI_A_ENGY_COM_TYPE            = 0x4112,       /*�й���Ϸ�ʽ������*/
    OI_R_ENGY_COM1_TYPE           = 0x4113,       /*�޹���Ϸ�ʽ1������*/
    OI_R_ENGY_COM2_TYPE           = 0x4114,       /*�޹���Ϸ�ʽ2������*/
    
    OI_AUTO_BILL_DAY_PROGRAM      = 0x4116,       /*�����ձ��*/
    OI_INTERVAL_DEMAND_FREEZE     = 0x4117,       /*�ڼ�������������*/

    OI_ELECTRI_DEVICE             = 0x4300,       /*"�����豸*/

    OI_CONNECT                    = 0x4400,       /*"Ӧ������"*/
    OI_CONNECT_PASS               = 0x4401,       /*"Ӧ��������֤����"*/
    
    OI_SIGNAL_GPRS                = 0x4500,       /*�������ź�ǿ��*/

    /*��������ʶ*/
    OI_MMT_FRZ                    = 0x5000,       /*˲ʱ����*/
    //OI_SEC_FRZ                    = 0x5001,       /*�붳��*/
    OI_MIN_FRZ                    = 0x5002,       /*���Ӷ���*/
    OI_HOUR_FRZ                   = 0x5003,       /*Сʱ����*/
    OI_DAY_FRZ                    = 0x5004,       /*�ն���*/
    OI_BILL_FRZ                   = 0x5005,       /*�����ն���*/
    OI_MONTH_FRZ                  = 0x5006,       /*�¶���*/
    OI_YEAR_FRZ                   = 0x5007,       /*�궳��*/
    OI_SEASON_TBL_SW_FRZ          = 0x5008,       /*ʱ�����л�����*/
    OI_DAY_TBL_SW_FRZ             = 0x5009,       /*��ʱ�α��л�����*/
    OI_TARIFF_TBL_SW_FRZ          = 0x500A,       /*���ʵ���л�����*/
    OI_STEP_TBL_SW_FRZ            = 0x500B,       /*�����л�����*/
    
    OI_STEP_BILL_TBL_SW_FRZ       = 0x5011 ,       /*�����л�����*/
    /*��������ʶ END*/
    
    OI_REMOTE                     = 0x8000,       /*ң��*/
    OI_REMAIN                     = 0x8001,       /*����*/

    OI_FILE                       = 0xf001,       /*�ļ�*/

    OI_ESAM                       = 0xF100,       /*ESAM*/   
    OI_SAFE_MODE                  = 0xF101,       /*��ȫģʽ*/    

    /*��������豸������ʶ*/
    OI_RS232_DEVICE               = 0xf200,    /*RS232*/
    OI_RS485_DEVICE               = 0xf201,    /*RS485*/
    OI_IR_DEVEICE                 = 0xf202,    /*�� ��*/
    OI_RELAY_DEVICE               = 0xf205,    /*�̵���*/
    OI_ALARM_DEVICE               = 0xf206,    /*�澯*/
    OI_MULTI_DEVICE               = 0xf207,    /*�๦�ܶ���*/
    OI_CARRIE_DEVICE              = 0xf209,    /*�ز�*/
    /*��������豸������ʶ*/

    OI_CRC_DISP                   = 0xf300,    /*�Զ�����*/
    OI_KEY_DISP                   = 0xf301,    /*��������*/
    
    /*�Զ�����չOI*/    
    OI_METER_MODE                 = 0xFF01,    /*����ģʽ��*/
    OI_RELAY_TYPE                 = 0xFF03,    /*�̵�������*/
    OI_ALM_RELAY_OUT_MODE         = 0xFF04,    /*�����̵������ģʽ��*/
    OI_ALM_BL_MODE                = 0xFF05,    /*���ⱨ��ģʽ��*/  
    OI_WAI_RELAY_TIME             = 0xFF06,    /*���ü̵������巽ʽ��բά��ʱ�䣨��λ:ms��*/
    OI_WAI_RELAY_AGAIN            = 0xFF07,    /*���ü̵������巽ʽ�ٴ���բ��������λ:Wh��*/
    OI_SOFT_BAK_NUMBER            = 0xFF08,    /*���������(�Զ���ı���)*/
    OI_EXTEND_MODE_1              = 0xFF10,    /*������չģʽ��1*/
    OI_EXTEND_MODE_2              = 0xFF11,    /*������չģʽ��2*/
    OI_EXTEND_MODE_3              = 0xFF12,    /*������չģʽ��3*/
    OI_EXTEND_MODE_4              = 0xFF13,    /*������չģʽ��4*/ 
    OI_EXTEND_MODE_5              = 0xFF14,    /*������չģʽ��5*/
    OI_EXTEND_MODE_6              = 0xFF15,    /*������չģʽ��6*/
    OI_EXTEND_MODE_7              = 0xFF16,    /*������չģʽ��7*/
    OI_EXTEND_MODE_8              = 0xFF17,    /*������չģʽ��8*/
    OI_EXTEND_MODE_9              = 0xFF18,    /*������չģʽ��9*/
    OI_EXTEND_MODE_10             = 0xFF19,    /*������չģʽ��10*/
    OI_DISP_VERSION               = 0xFF20,    /*�����ʾ�汾��*/        
    OI_SOFTWARE_VER               = 0xFF22,    /*����汾��(�Զ���)*/ 
    OI_PRODUCT_NUMBER_1           = 0xFF23,    /*����ʶ��Ψһ��1*/
    OI_PRODUCT_NUMBER_2           = 0xFF24,    /*����ʶ��Ψһ��2*/
    OI_PRODUCT_NUMBER_3           = 0xFF25,    /*����ʶ��Ψһ��2*/
    OI_CURRENT_TRIFF_NUM          = 0xFF26,    /*��ǰ���ʺ�*/
    OI_CURRENT_STEP_NUM           = 0xFF27,    /*��ǰ���ݺ�*/
    OI_STATUS_FACTOR              = 0xFF40,    /*�˳�����״̬,����ʱ��ʾ�������״̬*/
    OI_TEST_RELAY                 = 0xFF41,    /*�̵�����������*/
    OI_TEST_MULTI                 = 0xFF42,    /*�๦�ܶ���Ͷ�в���*/
    OI_TEST_CHECK                 = 0xFF43,    /*�Լ����*/
    
    OI_MAX_DL_LEN                 = 0xFF60,    /*������·����󳤶�*/    

    OI_PARAMENT_CRC               = 0xFF61,    /*����CRCֵ */
    //-------------
    OI_EXIT_FACTORY               = 0xFF62,    /*�˳�����״̬*/
    OI_CAL_EVETNT_PARA            = 0xFF67,    /*�����¼�Ĭ�ϲ���*/
    
    OI_CURRENT_PHASE              = 0xFF75,    /*�������*/
    OI_METER_RST_EVENT            = 0xff76,    /*���λ�¼�*/    
    OI_OSCILLATOR_ERR_EVENT       = 0xff77,    /*"��������¼�"*/
    OI_PD_PON_EVENT               = 0xff78,    /*"���ϵ��¼"*/
    OI_PARA_ERR_EVENT             = 0xff79,    /*"�����쳣��¼"*/
    OI_ENG_ERR_EVENT              = 0xff7a,    /*"�����쳣��¼"*/ 
    
    OI_DEMAND_MODE                = 0xff7b,    /*"����ͬ����ʽ"*/ 
    /*�Զ�����չOI END*/
    
}EN_OI_698;

#define BASE_EVENT_OI    OI_LOST_VOLT_EVENT    /*�¼��Ļ���OI*/

/********************************************************/
#define ATT_CLS1_CLS6_ATT_2_DATA    2  /*1-6������2����*/
#define ATT_L_CURRENT               4  /*���ߵ�������*/

/*CLASS1������ö�� */
typedef enum
{
    EN_CLS1_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS1_ATT_2_DATA                   = 2,  /*2���ܼ����ʵ���������*/
    EN_CLS1_ATT_3_SCALER_UNIT            = 3,  /*3�����㼰��λ*/
    EN_CLS1_ATT_4_HIGH_DATA              = 4,  /*4���߾����ܼ����ʵ���������*/
    EN_CLS1_ATT_5_HIGH_SCALER_UNIT       = 5   /*5���߾��Ȼ��㼰��λ*/
}EN_EVENT_CLASS1_ATT;

/*CLASS2������ö�� */
typedef enum
{
    EN_CLS2_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS2_ATT_2_DATA                   = 2,  /*2���ܼ����������������*/
    EN_CLS2_ATT_3_SCALER_UNIT            = 3,  /*3�����㼰��λ*/
}EN_EVENT_CLASS2_ATT;

/*CLASS3������ö�� */
typedef enum
{
    EN_CLS3_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS3_ATT_2_DATA                   = 2,  /*2��������ֵ��*/
    EN_CLS3_ATT_3_SCALER_UNIT            = 3,  /*3�����㼰��λ*/
}EN_EVENT_CLASS3_ATT;

/*CLASS4������ö�� */
typedef enum
{
    EN_CLS4_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS4_ATT_2_DATA                   = 2,  /*2���ܼ�������ֵ��*/
    EN_CLS4_ATT_3_SCALER_UNIT            = 3,  /*3�����㼰��λ*/
}EN_EVENT_CLASS4_ATT;

/*CLASS6������ö�� */
typedef enum
{
    EN_CLS6_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS6_ATT_2_DATA                   = 2,  /*2����ֵ*/
    EN_CLS6_ATT_3_SCALER_UNIT            = 3,  /*3�����㼰��λ*/
}EN_EVENT_CLASS6_ATT;

/*CLASS7������ö�� */
typedef enum
{
    EN_CLS7_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS7_ATT_2_EVENT_RECORD_TABLE     = 2,  /*2���¼���¼��*/
    EN_CLS7_ATT_3_RELATION_OBJECT_TABLE  = 3,  /*3�������������Ա�*/
    EN_CLS7_ATT_4_CURRENT_RECORD_NUM     = 4,  /*4����ǰ��¼��*/
    EN_CLS7_ATT_5_MAX_RECORD_NUM         = 5,  /*5������¼��*/
    EN_CLS7_ATT_6_SET_PARAMENT           = 6,  /*6�����ò���*/
    EN_CLS7_ATT_7_CURRENT_RECORD_TABLE   = 7,  /*7����ǰֵ��¼��*/
    EN_CLS7_ATT_8_REPORT_FLAG            = 8,  /*8���ϱ���ʶ*/
    EN_CLS7_ATT_9_VALID_FLAG             = 9,   /*9����Ч��ʶ*/
    EN_CLS7_ATT_10_TIME_STATUS_RECORD    = 10  /*10��ʱ��״̬��¼��*/
}EN_EVENT_CLASS7_ATT;

#define MAX_CLASS7_EVENT_ATT   EN_CLS7_ATT_9_VALID_FLAG

/*CLASS7�ķ���ö�� */
typedef enum
{
    EN_CLS7_METHOD_1_RESET               = 1,  /*1����λ*/
    EN_CLS7_METHOD_2_DO                  = 2,  /*2��ִ��*/
    //EN_CLS7_METHOD_3_RECORD              = 3,  /*3������һ�μ�¼*/
    EN_CLS7_METHOD_4_ADD_RELATION_OBJECT = 4,  /*4�����һ���¼�������������*/
    EN_CLS7_METHOD_5_DEL_RELATION_OBJECT = 5   /*5��ɾ��һ���¼�������������*/
}EN_EVENT_CLASS7_METHOD;

/*CLASS8������ö�� */
typedef enum
{
    EN_CLS8_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS8_ATT_2_PARAMENT               = 2   /*2������*/
}EN_EVENT_CLASS8_ATT;


/*CLASS9������ö�� */
typedef enum
{
    EN_CLS9_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS9_ATT_2_FREEZE_RECORD_TABLE    = 2,  /*2���¼���¼��*/
    EN_CLS9_ATT_3_RELATION_OBJECT_TABLE  = 3,  /*3�������������Ա�*/
}EN_EVENT_CLASS9_ATT;

/*CLASS9�ķ���ö�� */
typedef enum
{
    EN_CLS9_METHOD_1_RESET               = 1,  /*1����λ*/
    EN_CLS9_METHOD_2_DO                  = 2,  /*2��ִ��*/
    EN_CLS9_METHOD_3_RECORD              = 3,  /*3������һ�ζ���*/
    EN_CLS9_METHOD_4_ADD_RELATION_OBJECT = 4,  /*4�����һ���¼�������������*/
    EN_CLS9_METHOD_5_DEL_RELATION_OBJECT = 5,  /*5��ɾ��һ���¼�������������*/
    //----
    EN_CLS9_METHOD_7_MULI_ADD_RELATION_OBJECT = 7,  /*7��������Ӷ����������*/
    EN_CLS9_METHOD_8_CLR_RELATION_OBJECT = 8   /*8����������������Ա�*/
}EN_EVENT_CLASS9_METHOD;

/*CLASS17������ö�� */
typedef enum
{
    EN_CLS17_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS17_ATT_2_DISP_LIST              = 2,  /*2����ʾ�����б�*/
    EN_CLS17_ATT_3_DISP_TIME              = 3,  /*3����ʾʱ��*/
    EN_CLS17_ATT_4_DISP_PARAMENT          = 4,  /*4. ��ʾ����*/
}EN_EVENT_CLASS17_ATT;

/*CLASS17�ķ���ö�� */
typedef enum
{
    EN_CLS17_METHOD_1_RESET               = 1,  /*1����λ*/
    EN_CLS17_METHOD_2_DO                  = 2,  /*2��ִ��*/
    EN_CLS17_METHOD_3_UP_DISP             = 3,  /*3���·�*/
    EN_CLS17_METHOD_4_DOWN_DISP           = 4,  /*4���Ϸ�*/
    EN_CLS17_METHOD_5_DISP_LOOK           = 5,  /*5����ʾ�鿴*/ 
    EN_CLS17_METHOD_6_DISP_ALL            = 6,  /*6��ȫ��*/
}EN_EVENT_CLASS17_METHOD;


/*CLASS19������ö�� */
typedef enum
{
    EN_CLS19_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS19_ATT_2_DEVICE_DESC            = 2,  /*2���豸������*/
    EN_CLS19_ATT_3_VERSION_INFO           = 3,  /*3���汾��Ϣ*/
    EN_CLS19_ATT_4_PRODUCT_TIME           = 4,  /*4. ��������*/
    EN_CLS19_ATT_5_SUB_DEVICE_LIST        = 5,  /*5. ���豸�б�*/
    EN_CLS19_ATT_6_PROTOCOL_LIST          = 6,  /*6. ֧�ֹ�Լ�б�*/
    EN_CLS19_ATT_7_FOLLOW_REPORT          = 7,  /*7. ��������ϱ�*/
    EN_CLS19_ATT_8_ACTIVE_REPORT          = 8,  /*8. ���������ϱ�*/
    EN_CLS19_ATT_9_COMM_MASTER            = 9,  /*9. ��������վͨ��*/
    EN_CLS19_ATT_10_REPORT_CHANNEL        = 10,  /*10. �ϱ�ͨ��*/   
}EN_EVENT_CLASS19_ATT;

/*CLASS19�ķ���ö�� */
typedef enum
{
    EN_CLS19_METHOD_1_RESET               = 1,  /*1����λ*/
    EN_CLS19_METHOD_2_DO                  = 2,  /*2��ִ��*/
    EN_CLS19_METHOD_3_METER_CLR           = 3,  /*3. ���ݳ�ʼ��*/
    EN_CLS19_METHOD_4_SET_DEFAULT         = 4,  /*4. �ָ���������*/
    EN_CLS19_METHOD_5_EVENT_CLR           = 5,  /*5. �¼���ʼ��*/ 
    EN_CLS19_METHOD_6_DEMAND_CLR          = 6,  /*6. ������ʼ��*/
}EN_EVENT_CLASS19_METHOD;


/*CLASS20������ö�� */
typedef enum
{
    EN_CLS20_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS20_ATT_2_OBJECT_LIST            = 2,  /*2�������б�*/
    EN_CLS20_ATT_3_APP_INFO               = 3,  /*3��Ӧ���ﾳ��Ϣ*/
    EN_CLS20_ATT_4_CLIENT_ADDR            = 4,  /*4����ǰ���ӵĿͻ�����ַ*/
    EN_CLS20_ATT_5_ID_VERIFICTION         = 5,  /*5�������֤����*/
}EN_EVENT_CLASS20_ATT;

/*CLASS22������ö�� */
typedef enum
{
    EN_CLS22_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS22_ATT_2_DEVICE_LIST            = 2,  /*2. �豸�����б�*/
    EN_CLS22_ATT_3_DEVICE_NUM             = 3,  /*3. �豸��������*/
    EN_CLS22_ATT_4_SET_PARA               = 4,  /*4. ���ò���*/
}EN_EVENT_CLASS22_ATT;

/*CLASS24������ö�� */
typedef enum
{
    EN_CLS24_ATT_1_LOGIC_NUM              = 1,  /*1���߼���*/
    EN_CLS24_ATT_2_RELATION_OBJECT_TABLE  = 2,  /*2�������������Ա�*/
    EN_CLS24_ATT_3_CURRENT_RECORD_NUM     = 3,  /*3����ǰ��¼��*/
    EN_CLS24_ATT_4_MAX_RECORD_NUM         = 4,  /*4������¼��*/
    EN_CLS24_ATT_5_SET_PARAMENT           = 5,  /*5�����ò���*/
    EN_CLS24_ATT_6_EVENT_RECORD_1_TABLE   = 6,  /*6���¼���¼��*/
    EN_CLS24_ATT_7_EVENT_RECORD_2_TABLE   = 7,  /*7���¼���¼��*/
    EN_CLS24_ATT_8_EVENT_RECORD_3_TABLE   = 8,  /*8���¼���¼��*/
    EN_CLS24_ATT_9_EVENT_RECORD_4_TABLE   = 9,  /*9���¼���¼��*/
    EN_CLS24_ATT_10_CURRENT_RECORD_TABLE  = 10,  /*10����ǰֵ��¼��*/
    EN_CLS24_ATT_11_REPORT_FLAG           = 11,  /*11���ϱ���ʶ*/
    EN_CLS24_ATT_12_VALID_FLAG            = 12,  /*12����Ч��ʶ*/
    EN_CLS24_ATT_13_LOST_V_STATIC         = 13,  /*13.ʧѹͳ��*/
    EN_CLS24_ATT_14_TIME_STATUS_RECORD    = 14  /*14��ʱ��״̬��¼��*/
}EN_EVENT_CLASS24_ATT;

/*CLASS24�ķ���ö�� */
typedef enum
{
    EN_CLS24_METHOD_1_RESET               = 1,  /*1����λ*/
    EN_CLS24_METHOD_2_DO                  = 2,  /*2��ִ��*/
   // EN_CLS24_METHOD_3_RECORD              = 3,  /*3������һ�μ�¼*/
    EN_CLS24_METHOD_4_ADD_RELATION_OBJECT = 4,  /*4�����һ���¼�������������*/
    EN_CLS24_METHOD_5_DEL_RELATION_OBJECT = 5   /*5��ɾ��һ���¼�������������*/
}EN_EVENT_CLASS24_METHOD;

typedef enum
{
    EN_METHOD_1_RESET_ESAM          = 1,  /*1����λ*/
    EN_METHOD_2_DO_ESAM             = 2,  /*2��ִ��*/
    EN_METHOD_3_GET_ESAM            = 3,  /*3.ESAM���ݶ�ȡ*/
    EN_METHOD_4_SET_ESAM            = 4,  /*4. ���ݸ���*/
    EN_METHOD_5_RELEASE_ESAM        = 5,  /*5. Э��ʧЧ*/
    EN_METHOD_6_PURSE_ESAM          = 6,  /*Ǯ����������������ֵ���˷ѣ�*/
    EN_METHOD_7_KEY_UPDATE_ESAM     = 7,  /*7. ��Կ����*/
    EN_METHOD_8_CERT_UPDATE_ESAM    = 8,  /*8��֤�����*/
    EN_METHOD_9_AUTHTIMESET_ESAM    = 9,  /*9������Э��ʱЧ*/
    EN_METHOD_10_INIT_PACKET_ESAM   = 10,  /*10. Ǯ����ʼ��*/
    EN_METHOD_11_FARCHEK_ESAM       = 11,  /*11. ������֤����*/
    EN_METHOD_12_FARAUTH_ESAM       = 12,  /*12. ������ָ֤��*/
}EN_METHOD_ESAM;    

typedef enum
{
    EN_POWER_OIB2_SUM                = 0,/*����*/   
    EN_POWER_OIB2_A                  = 1,/*A ��*/   
    EN_POWER_OIB2_B                  = 2,/*B ��*/   
    EN_POWER_OIB2_C                  = 3,/*C ��*/   
}EN_CLASS1_POWER_OIB2;

/*����Ԫ���������Ϊ1���������߱�Ϊ2���������߱�Ϊ3��*/
typedef enum
{
   EN_METER_NUM_SINGLE   = 1,
   EN_METER_NUM_3_3_1    = 2,
   EN_METER_NUM_3_4_1    = 3,
}EN_DATA_MEMTER_NUM;


#define  MASK_BLOCK_DATA            0x80  /*��������������*/
/********************************************************/
#endif

