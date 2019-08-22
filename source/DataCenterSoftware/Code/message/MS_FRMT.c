/*"*************************************************************"*/
/*"FileName: MS_FRMT.c"*/
/*"Author:    		Version :  "*/
/*"2019-08-21 11:03:30"*/
/*"Description: 电能表软件平台数据格式列表定义"*/
/*"总字节数:          "*/
/*"Function List:                     "*/
/*"                                    "*/
/*"History:                           "*/
/*"<author> <time> <version > <desc>  "*/
/*"************************************************************"*/


#include "PlatForm.h"
#include "App\message\FRMT_def.h"
#include "App\LegallyRelevant\LRM_Config.h"

const ST_FMT_ATTR stFormatList[] = 
 {  
     {E_S_4_BYTE,E_A_UNIT,CURR_DECIMAL_POINT},//0x00
     {E_U_1_BYTE,E_NONE_UNIT,0},//0x01
     {E_U_4_BYTE,E_HZ_UNIT,FREQ_DECIMAL_POINT},//0x02
     {E_U_4_BYTE,E_KW_UNIT,-4},//0x03
     {E_U_4_BYTE,E_MINUTE_UNIT,0},//0x04
     {E_U_4_BYTE,E_NONE_UNIT,0},//0x05
     {E_U_2_BYTE,E_V_UNIT,VOLT_DECIMAL_POINT},//0x06
     {E_U_ENGY_X_BYTE,E_KWH_UNIT,-3},//0x07
     {E_YYMMDDHHMMSSWW,E_NONE_UNIT,0},//0x08
     {E_S_4_BYTE,E_KW_UNIT,PWR_DECIMAL_POINT},//0x09
     {E_S_4_BYTE,E_NONE_UNIT,FACTOR_DECIMAL_POINT},//0x0A
     {E_YYMMDDHHMMSS,E_NONE_UNIT,0},//0x0B
     {E_U_2_BYTE,E_NONE_UNIT,0},//0x0C
     {E_U_8_BYTE,E_NONE_UNIT,0},//0x0D
     {E_U_ENGY_X_BYTE,E_KWH_UNIT,ENGY_DECIMAL_POINT},//0x0E
     {E_SHOW_CODE_9_BYTE,E_NONE_UNIT,0},//0x0F
     {E_U_4_BYTE,E_YUAN_UNIT,-2},//0x10
     {E_U_6_BYTE,E_NONE_UNIT,0},//0x11
     {E_U_4_BYTE,E_YUAN_UNIT,-4},//0x12
     {E_NN_HHMM,E_NONE_UNIT,0},//0x13
     {E_NN_MMDD,E_NONE_UNIT,0},//0x14
     {E_NN_YYMMDD,E_NONE_UNIT,0},//0x15
     {E_YYMMDDHHMM,E_NONE_UNIT,0},//0x16
     {E_NN_HHMM_42,E_NONE_UNIT,0},//0x17
     {E_ASCII_6_LENGTH,E_NONE_UNIT,0},//0x18
     {E_NONE_FORMAT,E_NONE_UNIT,0},//0x19
     {E_U_4_BYTE,E_KWH_UNIT,-2},//0x1A
     {E_ASCII_4_LENGTH,E_NONE_UNIT,0},//0x1B
     {E_ASCII_10_LENGTH,E_NONE_UNIT,0},//0x1C
     {E_ASCII_16_LENGTH,E_NONE_UNIT,0},//0x1D
     {E_ASCII_32_LENGTH,E_NONE_UNIT,0},//0x1E
     {E_ASCII_8_LENGTH,E_NONE_UNIT,0},//0x1F
     {E_U_4_BYTE,E_A_UNIT,-4},//0x20
     {E_ASCII_2_LENGTH,E_NONE_UNIT,0},//0x21
     {E_DDHH,E_NONE_UNIT,0},//0x22
     {E_S_ENGY_X_BYTE,E_KWH_UNIT,ENGY_DECIMAL_POINT},//0x23
     {E_U_16_BYTE,E_NONE_UNIT,0},//0x24
     {E_NN_MMDD_42,E_NONE_UNIT,0},//0x25
     {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0},//0x26
     {E_DLT645_10_ID,E_NONE_UNIT,0},//0x27
     {E_U_2_BYTE,E_V_UNIT,-3},//0x28
     {E_MMDDHH,E_NONE_UNIT,0},//0x29
     {E_U_5_BYTE,E_NONE_UNIT,0},//0x2A
     {E_ASCII_20_LENGTH,E_NONE_UNIT,0},//0x2B
     {E_U_1_BYTE,E_SECOND_UNIT,0},//0x2C
     {E_MMDDHHMM,E_NONE_UNIT,0},//0x2D
     {E_UBCD_2_BYTE,E_V_UNIT,-1},//0x2E
     {E_BCD_3_BYTE,E_A_UNIT,-3},//0x2F
     {E_UBCD_2_BYTE,E_HZ_UNIT,-2},//0x30
     {E_BCD_3_BYTE,E_KW_UNIT,-4},//0x31
     {E_BCD_2_BYTE,E_NONE_UNIT,-3},//0x32
     {E_UBCD_4_BYTE,E_KWH_UNIT,-2},//0x33
     {E_U_ENGY_X_BYTE,E_KVARH_UNIT,ENGY_DECIMAL_POINT},//0x34
     {E_ID_CODE_4_FORMAT,E_NONE_UNIT,0},//0x35
     {E_UBCD_5_BYTE,E_KVARH_UNIT,-4},//0x36
     {E_UBCD_5_BYTE,E_KWH_UNIT,-4},//0x37
     {E_S_2_BYTE,E_NONE_UNIT,-1},//0x38
     {E_S_4_BYTE,E_NONE_UNIT,-1},//0x39
     {E_S_4_BYTE,E_KVAR_UNIT,PWR_DECIMAL_POINT},//0x3A
     {E_BCD_3_BYTE,E_KVAR_UNIT,-4},//0x3B
     {E_U_2_BYTE,E_NONE_UNIT,-2},//0x3C
     {E_S_4_BYTE,E_NONE_UNIT,-4},//0x3D
     {E_U_2_BYTE,E_PER_UNIT,HARM_DECIMAL_POINT},//0x3E
     {E_U_4_BYTE,E_A_UNIT,-4},//0x3F
     {E_U_4_BYTE,E_V_UNIT,-4},//0x40
};
