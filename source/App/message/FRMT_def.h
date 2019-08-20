/*"*****************************************************************************"*/
/*"  FileName: FMT_def.h"*/
/*"  Description:     ���ļ�������ܱ����ݵ�λ���������͡�С��λ�궨��"*/
/*"                   ������ܱ��׼���ݸ�ʽö�����ͣ����ڱ�׼���ݸ�ʽ�б���"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.01.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      ganjp    2017/1/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#ifndef FMT_DEF_H
#define FMT_DEF_H

enum  DATA_UNIT
{
    E_NONE_UNIT = 0,
    E_KWH_UNIT,
    E_WH_UNIT,
    E_KW_UNIT,
    E_W_UNIT,
    E_V_UNIT,
    E_A_UNIT,
    E_SECOND_UNIT,
    E_MINUTE_UNIT,
    E_HOUR_UNIT,
    E_DAY_UNIT,
    E_DEGREE_UNIT,
    E_HZ_UNIT,
    E_YUAN_UNIT,
//#ifdef THREE_PHASE
    E_KVARH_UNIT,
    E_VARH_UNIT,
    E_KVAR_UNIT,
    E_VAR_UNIT,
    E_WAN_YUAN_UNIT,
    E_KV_UNIT,
    E_KA_UNIT,
    E_KVA_UNIT,
    E_VA_UNIT,
    E_VAH_UNIT,
    E_KVAH_UNIT,
    E_AH_UNIT,
    E_PER_UNIT,  //%
//#endif
    E_MAX_UNIT
};


/*��Ӧ����ΪfrmtLen[]*/
typedef enum
{
    E_BCDFORMAT = 0, /*"�з���BCD��ʽ�������Ͷ���"*/
    E_BCD_1_BYTE,
    E_BCD_2_BYTE,
    E_BCD_3_BYTE,
    E_BCD_4_BYTE,
    E_ENGY_H_INT,          /*"�߾��ȵ�����ʾ��������"*/
    E_ENGY_H_DECIMAL,      /*"�߾��ȵ�����ʾС������"*/

    E_UBCDFORMAT, /*"�޷���BCD��ʽ�������Ͷ���"*/
    E_UBCD_1_BYTE,
    E_UBCD_2_BYTE,
    E_UBCD_3_BYTE,
    E_UBCD_4_BYTE,
    E_UBCD_5_BYTE,
    E_UENGY_H_INT,          /*"�߾��ȵ�����ʾ��������"*/
    E_UENGY_H_DECIMAL,      /*"�߾��ȵ�����ʾС������"*/

    E_HEX_FORMAT, /*" HEX���������Ͷ���"*/
    E_S_ENGY_X_BYTE, /*"�з���HEX���������Ͷ���"*/
    E_S_ENGY_8_BYTE, /*"�߾��ȵ��������ʽ"*/
    E_S_4_BYTE,
    E_S_2_BYTE,
    E_S_1_BYTE,
    E_U_ENGY_X_BYTE, /*"�޷���HEX���������Ͷ���"*/
    E_U_ENGY_8_BYTE, /*"�߾��ȵ��������ʽ"*/
    E_U_4_BYTE,
    E_U_2_BYTE,
    E_U_1_BYTE,

    E_TIME_FORMAT, /*"ʱ���ʽ�������Ͷ���"*/
    E_YYMMDD,
    E_HHMMSS,
    E_YYMMDDHHMM,
    E_YYMMDDHH,
    E_MMDDHHMM,
    E_YYMMDDHHMMSS,
    E_YYYYMMDDHHMMSS,
    E_YYYYMMDDHHMMSS_FF,/*"��ΪFF�ĸ�ʽ"*/
    E_YYYYMMDDHHMM,
    E_YYMMDDHHMMSSWW,
    E_YYMMDDWWHHMMSS,
    E_MMDD,
    E_MMDDHH,
    E_HHMM,
    E_DDHH,
    E_NN_MMDD,
    E_NN_HHMM,
    E_NN_YYMMDD,
    E_WW_YYMMDD,
    E_NN_YYYYMMDDWW,
    E_NN_HHMM_42,
    E_NN_MMDD_42,

    /*--------����Ķ���BCD��ʽ------------*/
    E_TIME_FORMAT_BCD, /*"ʱ���ʽ��������BCD����"*/
    E_DDHH_BCD,           /*BCD��ʽ��ʱ�� 0*/    
    E_HHMM_BCD,           /*BCD��ʽ��ʱ�� 1*/
    E_NN_MMDD_BCD,        /*BCD��ʽ��ʱ�� 2*/
    E_NN_HHMM_BCD,        /*BCD��ʽ��ʱ�� 2*/
    E_HHMMSS_BCD,         /*BCD��ʽ��ʱ�� 3*/
    E_YYMMDD_BCD,         /*BCD��ʽ��ʱ�� 4*/
    E_MMDDHH_BCD,         /*BCD��ʽ��ʱ�� 5*/
    E_MMDDHHMM_BCD,       /*BCD��ʽ��ʱ�� 7*/
    E_YYMMDDHH_BCD,       /*BCD��ʽ��ʱ�� 7*/    
    E_WW_YYMMDD_BCD,      /*BCD��ʽ��ʱ�� 8*/
    E_YYMMDDHHMM_BCD,     /*BCD��ʽ��ʱ�� 9*/    
    E_YYMMDDHHMMSS_BCD,   /*BCD��ʽ��ʱ�� 10*/
    E_NN_YYMMDD_BCD,      /*BCD��ʽ��ʱ�� 11*/ 
    E_NN_HHMM_42_BCD,     /*BCD��ʽ��ʱ�� 12 ʱ�α�*/ 
    E_NN_MMDD_42_BCD,     /*BCD��ʽ��ʱ�� 13 ʱ����*/     
    /*--------����Ķ���BCD��ʽ------------*/
    
    E_STRING, /*"�ַ�����ʽ�������Ͷ���"*/
    E_U_5_BYTE,
    E_U_6_BYTE,
    E_U_8_BYTE,
    E_U_12_BYTE,
    E_U_16_BYTE,
    E_SHOW_CODE_5_BYTE,
    E_SHOW_CODE_9_BYTE,
    E_ASCII_2_LENGTH,
    E_ASCII_4_LENGTH,
    E_ASCII_6_LENGTH,
    E_ASCII_8_LENGTH,
    E_ASCII_10_LENGTH,
    E_ASCII_16_LENGTH,
    E_ASCII_20_LENGTH,
    E_ASCII_32_LENGTH,
#ifdef THREE_PHASE
    E_DLT_645_07_ID,
    E_USERID_SHOW,
    E_DLT645_07ID,
#endif
    E_ID_CODE_2_FORMAT,
    E_ID_CODE_4_FORMAT,
#ifdef THREE_PHASE
    E_RMB_MAUTO,
#endif
    E_DATUO,
    E_MATUO,
#ifdef THREE_PHASE
    E_DEMAND_AND_TIME,
    E_DEMAND_AND_TIME_698,
    E_DEMAND_AND_TIME_645,
#endif
    E_DLT645_10_ID,
    E_DATA_TYPE_MAX,

    /*"�������ݸ�ʽΪ��ǰ��������ݸ�ʽ,��ʽ����˳��ͬ��ǰ����"*/
    /*" ��ʽ����˳�򣬷������ƫ�Ʒ�ʽת����ʽ"*/
    E_L_BCDFORMAT,
    E_L_BCD_1_BYTE,
    E_L_BCD_2_BYTE,
    E_L_BCD_3_BYTE,
    E_L_BCD_4_BYTE,

    E_L_UBCDFORMAT,
    E_L_UBCD_1_BYTE,
    E_L_UBCD_2_BYTE,
    E_L_UBCD_3_BYTE,
    E_L_UBCD_4_BYTE,

    E_NONE_FORMAT = 0xff
}EN_DATA_TYPE;
#endif
