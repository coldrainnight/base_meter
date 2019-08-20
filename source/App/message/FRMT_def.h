/*"*****************************************************************************"*/
/*"  FileName: FMT_def.h"*/
/*"  Description:     该文件定义电能表数据单位、数据类型、小数位宏定义"*/
/*"                   定义电能表标准数据格式枚举类型，用于标准数据格式列表定义"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.01.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
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


/*对应长度为frmtLen[]*/
typedef enum
{
    E_BCDFORMAT = 0, /*"有符号BCD格式数据类型定义"*/
    E_BCD_1_BYTE,
    E_BCD_2_BYTE,
    E_BCD_3_BYTE,
    E_BCD_4_BYTE,
    E_ENGY_H_INT,          /*"高精度电量显示整数部分"*/
    E_ENGY_H_DECIMAL,      /*"高精度电量显示小数部分"*/

    E_UBCDFORMAT, /*"无符号BCD格式数据类型定义"*/
    E_UBCD_1_BYTE,
    E_UBCD_2_BYTE,
    E_UBCD_3_BYTE,
    E_UBCD_4_BYTE,
    E_UBCD_5_BYTE,
    E_UENGY_H_INT,          /*"高精度电量显示整数部分"*/
    E_UENGY_H_DECIMAL,      /*"高精度电量显示小数部分"*/

    E_HEX_FORMAT, /*" HEX码数据类型定义"*/
    E_S_ENGY_X_BYTE, /*"有符号HEX码数据类型定义"*/
    E_S_ENGY_8_BYTE, /*"高精度电量传输格式"*/
    E_S_4_BYTE,
    E_S_2_BYTE,
    E_S_1_BYTE,
    E_U_ENGY_X_BYTE, /*"无符号HEX码数据类型定义"*/
    E_U_ENGY_8_BYTE, /*"高精度电量传输格式"*/
    E_U_4_BYTE,
    E_U_2_BYTE,
    E_U_1_BYTE,

    E_TIME_FORMAT, /*"时间格式数据类型定义"*/
    E_YYMMDD,
    E_HHMMSS,
    E_YYMMDDHHMM,
    E_YYMMDDHH,
    E_MMDDHHMM,
    E_YYMMDDHHMMSS,
    E_YYYYMMDDHHMMSS,
    E_YYYYMMDDHHMMSS_FF,/*"秒为FF的格式"*/
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

    /*--------下面的都是BCD格式------------*/
    E_TIME_FORMAT_BCD, /*"时间格式数据类型BCD定义"*/
    E_DDHH_BCD,           /*BCD格式的时间 0*/    
    E_HHMM_BCD,           /*BCD格式的时间 1*/
    E_NN_MMDD_BCD,        /*BCD格式的时间 2*/
    E_NN_HHMM_BCD,        /*BCD格式的时间 2*/
    E_HHMMSS_BCD,         /*BCD格式的时间 3*/
    E_YYMMDD_BCD,         /*BCD格式的时间 4*/
    E_MMDDHH_BCD,         /*BCD格式的时间 5*/
    E_MMDDHHMM_BCD,       /*BCD格式的时间 7*/
    E_YYMMDDHH_BCD,       /*BCD格式的时间 7*/    
    E_WW_YYMMDD_BCD,      /*BCD格式的时间 8*/
    E_YYMMDDHHMM_BCD,     /*BCD格式的时间 9*/    
    E_YYMMDDHHMMSS_BCD,   /*BCD格式的时间 10*/
    E_NN_YYMMDD_BCD,      /*BCD格式的时间 11*/ 
    E_NN_HHMM_42_BCD,     /*BCD格式的时间 12 时段表*/ 
    E_NN_MMDD_42_BCD,     /*BCD格式的时间 13 时区表*/     
    /*--------上面的都是BCD格式------------*/
    
    E_STRING, /*"字符串格式数据类型定义"*/
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

    /*"以下数据格式为带前导零的数据格式,格式定义顺序同非前导零"*/
    /*" 格式定义顺序，方便采用偏移方式转换格式"*/
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
