/*"***********************************************************		
FileName: CLASS_COMM.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: �๫�ö���ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS_COMM_H_
#define _CLASS_COMM_H_
/*"******************************************************"*/
#include "System\Sys_def.h"

#include "App\UserInterface\PP\PP_698\Process\Pp_macro.h"
#include "App\UserInterface\PP\PP_698\Process\Pp_obis.h"
#include "Lib\AXDR\AXDR.h"

#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\FRMT_def.h"

/*"******************************************************"*/
/*"IID����"*/
#ifdef THREE_PHASE
#define  CLASS1_CLASS2_MAX_IID  9  /*����ʱ���õ�������IID����ʱҪ�����⴦��ȡֵ*/
#define  CLASS3_MAX_IID  3 
#define  CLASS4_MAX_IID  4
#else
#define  CLASS1_CLASS2_MAX_IID  5   //
#define  CLASS3_MAX_IID  1
#define  CLASS4_MAX_IID  2
#endif

#define MAXTRIFF_IID   CLASS1_CLASS2_MAX_IID  

#define MAXTRIFF_ABC_IID  1  /*�����������������*/

#define NO_READ_WRITE 0x00  /*�����ܶ���Ҳ����д�����һЩû�ж���OBIS������*/
#define READ_ONLY    0x0F   /*ֻ��*/
#define WRITE_ONLY   0xF0   /*ֻд�����ĺ�բ����*/
#define READ_WRITE   0xFF   /*�ɶ�д*/

#ifdef THREE_PHASE
#define NUM_RELAY   1
#else
#define NUM_RELAY   1
#endif

typedef struct
{
   INT8U Scaler;           /*���٣������ϵ����10�ļ��η�(������)*/
   INT8U UNIT;             /*��λ*/ 
   
   const INT8U *Pointer;   /*����ָ��*/
}st_object_ele;            /*����Ԫ��*/

typedef INT16U  ObjectMethod;

typedef struct
{
    INT16U OI;             /*"OAD���"*/ 
    ObjectMethod ObjMeth;  /*"���󷽷� ----�����ͨ���������ģ�����Ϊ����ID����"*/ 
    const INT8U *Pointer;  /*����ָ��*/
}ST_OI_METHOD;

typedef struct
{
    INT8U  AID;            /*"AID���"*/   
    INT8U  OI_NUM;         /*"ͬһ��AID��Ӧ��OI����"*/  
    const ST_OI_METHOD *Pointer;  /*��������ָ��*/    
}ST_OI_METHOD_NUM_POINTER;

/*-------------------------------------------------*/

/*"��д������ţ���Ӧ��stFunction�е�λ��"*/
typedef enum
{
    FUN_NULL = 0 ,  /*"�޹���"*/
    FUN_RUNSTATUS,  /*"����״̬��"*/
    FUN_SET_TIME,   /*" ����ʱ��"*/
    FUN_LCDPARA,    /*" LCD����"*/
    FUN_MULTI,      /*"�๦�����"*/
    FUN_RLYSTATUS,  /*"�̵���״̬"*/
    FUN_EVTRPTSTATUS,/*" �����ϱ�״̬��"*/
    #ifdef THREE_PHASE
    FUN_SET_DU_PARA,/*������������*/ 
    #endif
    MAX_FUN_ID
}EN_FUNCTION_ID;

/*��ʽ(���Զ������ͨ������ʾ)ö��  ��ӦstFormatId*/
typedef enum
{
    FORMAT_V = 0,  /*"��ѹ��ʽ"*/
    FORMAT_A,      /*"������ʽ"*/
    FORMAT_PW,     /*"���ʸ�ʽ"*/
    FORMAT_FACTOR, /*"����������ʽ"*/
    FORMAT_FREQ,   /*"Ƶ�ʸ�ʽ"*/
    FORMAT_ENGY,   /*"��������ʽ"*/
    FORMAT_COMBENGY, /*"��ϵ�����ʽ"*/
    FORMAT_CTRLPARA, /*""1�ֽڿ����������ʽ"*/
    FORMAT_DATETIME, /*""����ʱ���ʽ"*/
    FORMAT_TIME, /*"ʱ���ʽ"*/
    FORMAT_ADDR_H, /*"ͨ�ŵ�ַ��λ��ʽ"*/
    FORMAT_ADDR_L, /*"ͨ�ŵ�ַ��λ��ʽ"*/
    FORMAT_MTRNO_H, /*"��Ÿ�λ��ʽ"*/
    FORMAT_MTRNO_L, /*"��ŵ�λ��ʽ"*/
    FORMAT_WORD, /*"���ָ�ʽ"*/
    FORMAT_PRICE, /*"��۸�ʽ"*/
    FORMAT_MONEY, /*"ʣ�����ʽ"*/
    FORMAT_BUYTIMES, /*"���������ʽ"*/
    FORMAT_BILLENGY, /*"�õ�����ʽ"*/
    FORMAT_BILLDATE,/*"�������ڸ�ʽ"*/
    FORMAT_IDCODE,/*"�ַ���ʶ"*/
    FORMAT_N_A,/*"���ߵ�����ʽ"*/
    FORMAT_CONST,/*"���峣����ʽ"*/
    FORMAT_STEPENGY,/*"���ݵ�����ʽ"*/
    FORMAT_YEARBILL,/*"������ո�ʽ"*/
    FORMAT_DAILYSLOT,/*"��ʱ�α��ʽ"*/
    FORMAT_DISP,/*"��ʾ���ʽ"*/
    FORMAT_P4,/*"4�ֽڲ�����ʽ"*/
    FORMAT_NULL,/*"�޸�ʽ"*/
    FORMAT_SWTIME,/*"�л�ʱ���ʽ"*/
    FORMAT_HOLIDAY,/*"�������ո�ʽ"*/
    FORMAT_YEARSLOT,/*"��ʱ����ʽ"*/
    FORMAT_ASCII4,/*"4byte ASCII��ʽ"*/
    FORMAT_ASCII6,/*"6byte ASCII��ʽ"*/
    FORMAT_ASCII8,/*"8byte ASCII��ʽ"*/
    FORMAT_ASCII16,/*"16byte ASCII��ʽ"*/
    FORMAT_ASCII32,/*"32byte ASCII��ʽ"*/
//    FORMAT_EVTTIME,/*"�¼�����ʱ���ʽ"*/
    FORMAT_EVT_A,/*"�¼�������ʽ"*/
    FORMAT_EVT_PW,/*"�¼����ʸ�ʽ"*/
    FORMAT_EVT_FACTOR,/*"�¼�����������ʽ"*/
//    FORMAT_FRZTIME,/*"����ʱ���ʽ"*/
    FORMAT_PRGOAD,/*"���OAD��ʽ"*/
    FORMAT_YSLOTTAB,/*"��ʱ�����ʽ"*/
    FORMAT_U4_UBCD4,/*"��������ȸ�ʽ"*/
    FORMAT_BV,  /*"ʱ�ӵ�ص�ѹ��ʽ"*/
    FORMAT_RCDOAD,/*"��¼OAD��ʽ"*/
    FORMAT_EVT_STEP,/*"���ݱ�̼�¼��ʽ"*/
    FORMAT_CARD_CMDHEAD,/*"�忨��������ͷ"*/
    FORMAT_BYTE16,/*"16�ֽ�����"*/
    FORMAT_MINUTE,/*"���Ӹ�ʽ"*/
    FORMAT_U2_UBCD2,/*U2ת��UBCD2���Ա���ʾ*/
    FORMAT_U1_UBCD2,/*U1ת��UBCD2���Ա���ʾ*/
    FORMAT_A_4,     /*��ʾ������صĲ���*/
    FORMAT_ENGY_H,      /*�߾��ȵ�����*/
    FORMAT_COMBENGY_H,  /*�߾�����ϵ�����*/   
    
#ifdef THREE_PHASE
    FORMAT_R_PW,     /*"�޹����ʸ�ʽ"*/
    FORMAT_VA_PW,    /*"���ڹ��ʸ�ʽ"*/
    FORMAT_S2_2_UBCD2_0,/*�ٷֱ�*/ 
    FORMAT_S2_1_UBCD2_1,/*�ٷֱ�*/
    FORMAT_A_PW,        /*������������*/
    FORMAT_U2_2_UBCD2_0,/*�ٷֱ�*/ 
    FORMAT_DEMAND,      /*����*/
    
    FORMAT_COMBENGY_R,  /*����޹�������*/
    FORMAT_COMBENGY_R_H,/*�߾�������޹�������*/  
    
    FORMAT_ENGY_R,      /*�޹�������*/
    FORMAT_ENGY_R_H,      /*�޹��������߾���*/
    FORMAT_U2_1_UBCD2_1,  /*U2 -1תUBCD -1*/
    
    #endif
    MAX_FORMAT_ID /*��û�и�ʽ�����ý��и�ʽת��*/
}EN_FORMAT_ID;


/*��ʾ���ñ�IDö��  ��Ӧ stShowinfo*/
typedef enum
{
    E_INFO_NOW_TRIFF_A_ENGY,    //0000  /*��ǰ   ���� �й�     ����*/
    E_INFO_NOW_TRIFF_PA_ENGY,   //0001  /*��ǰ   ���� �����й� ����*/
    E_INFO_NOW_TRIFF_NA_ENGY,   //0002  /*��ǰ   ���� �����й� ����*/
    E_INFO_LAST_TRIFF_A_ENGY,   //0003  /*��*��  ���� �й�     ����*/
    E_INFO_LAST_TRIFF_PA_ENGY,  //0004  /*��*��  ���� �����й� ����*/
    E_INFO_LAST_TRIFF_NA_ENGY,  //0005  /*��*��  ���� �����й� ����*/
    E_INFO_NOW_LAST_SUM_ENGY,   //0006  /*��ǰ��*��  ���� ����   ???*/
    E_INFO_LAST_SUM_ENGY,       //0007  /*��*��  ���� ���� */
    E_INFO_TIME,                //0008  /*ʱ��*/
    E_INFO_REMAIN_MONEY,        //0009  /*ʣ�� ���*/
    E_INFO_OVERDRAW_MONEY,      //000A  /*͸֧ ���*/
    E_INFO_NULL,                //000B  /*�հ�*/
    E_INFO_L,                   //000C  /*L*/
    E_INFO_PA_POWER,            //000D  /*�й�����*/    
    #ifdef THREE_PHASE
    E_INFO_A_PA_POWER,          //0025  /*"A���й�����"*/
    E_INFO_B_PA_POWER,          //0026  /*"B���й�����"*/
    E_INFO_C_PA_POWER,          //0027  /*"C���й�����"*/
    #endif
    
    E_INFO_COS_CREDIT_SUM,      //000E  /*COS����*/
    #ifdef THREE_PHASE
    E_INFO_COS_A,               //000F  /*COS��A*/
    E_INFO_COS_B,               //0028  /*"COS��B"*/
    E_INFO_COS_C,               //0029  /*"COS��C"*/
    #endif
    
    E_INFO_N,                   //0010  /*N*/
    E_INFO_NOW_STEP_PRICE,      //0011  /*��ǰ ���� �� ��*/
    E_INFO_NOW_PRICE,           //0012  /*��ǰ �� ��*/
    E_INFO_NOW_TIME,            //0013  /*��ǰ ʱ��*/
    E_INFO_METER_NUM,           //0014  /*���*/
    E_INFO_CONST,               //0015  /*����*/
    E_INFO_SUM,                 //0x16  /*��*/
    E_INFO_CUSTOM_NUM,          //0x17  /*����*/
    E_INFO_NOW_STEP_SUM_ENGY,   //0x18  /*��ǰ ���� ���õ���*/
    E_INFO_LAST_STEP_SUM_ENGY,  //0x19  /*�� ���� ���õ���*/
    E_INFO_NOW_TRIFF_PRICE,     //001A  /*��ǰ ���� ���*/
    
    E_INFO_NOW_STEP_PRICE_MONEY,//0x1C  /*��ǰ���� ���� ���*/
    E_INFO_NOW_STEP,            //0x1D  /*��ǰ����*/
    
    E_INFO_STEP_PRICE,          //0x1B  /*���� ���*/
    E_INFO_STEP_PRICE_MONEY,    //0x1C  /*���� ��� ���*/
    E_INFO_STEP,                //0x1D  /*����*/
    E_INFO_TRIFF_PRICE,         //001A  /*���� ���*/

#ifdef THREE_PHASE    
    E_INFO_R_CONST,             //001E  /*"�޹�����"*/
    
    E_INFO_A_I,                 //001F  /*"A�����"*/
    E_INFO_B_I,                 //0020  /*"B�����"*/
    E_INFO_C_I,                 //0021  /*"C�����"*/
    
    E_INFO_A_V,                 //0022  /*"A���ѹ"*/
    E_INFO_B_V,                 //0023  /*"B���ѹ"*/
    E_INFO_C_V,                 //0024  /*"C���ѹ"*/

    E_INFO_A_$,                 //0022  /*"A�����"*/
    E_INFO_B_$,                 //0023  /*"B�����"*/
    E_INFO_C_$,                 //0024  /*"C�����"*/

    E_INFO_RA_POWER,            //002A  /*"���޹�����"*/
    E_INFO_A_RA_POWER,          //002B  /*"A���޹�����"*/
    E_INFO_B_RA_POWER,          //002C  /*"B���޹�����"*/
    E_INFO_C_RA_POWER,          //002D  /*"C���޹�����"*/

    E_INFO_SUM_POWER,            //002A  /*"�ܹ���"*/
    E_INFO_A_POWER,              //002B  /*"A�๦��"*/
    E_INFO_B_POWER,              //002C  /*"B�๦��"*/
    E_INFO_C_POWER,              //002D  /*"C�๦��"*/
    
    E_INFO_NOW_COMM_TRIFF_R_1_ENGY,//002F  /*"��ǰ��Ϸ��� �޹� 1  ����"*/
    E_INFO_NOW_COMM_TRIFF_R_2_ENGY,//002F  /*"��ǰ��Ϸ��� �޹� 2  ����"*/
    
    E_INFO_NOW_TRIFF_R_I_ENGY,   //0030  /*"��ǰ���� I�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_II_ENGY,  //0031  /*"��ǰ���� II�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_III_ENGY, //0032  /*"��ǰ���� III�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_IV_ENGY,  //0033  /*"��ǰ���� V�����޹� ����"*/


    E_INFO_NOW_PA_DEMAND,  //0034  /*"��ǰ �й� ����"*/
    E_INFO_NOW_R_DEMAND,   //0034  /*"��ǰ �޹� ����"*/

    E_INFO_NOW_TRIFF_PA_DEMAND,  //0034  /*"��ǰ���� �����й� ����"*/
    E_INFO_NOW_TRIFF_PA_DEMAND_TIME,//0035  /*"��ǰ���� �����й� ��������ʱ��"*/    
    E_INFO_NOW_TRIFF_NA_DEMAND,  //0036  /*"��ǰ���� �����й� ����"*/
    E_INFO_NOW_TRIFF_NA_DEMAND_TIME,//0037  /*"��ǰ���� �����й� ��������ʱ��"*/
    
    E_INFO_NOW_COMM_TRIFF_R_1_DEMAND,//002F  /*"��ǰ��Ϸ��� �޹� 1  ����"*/
    E_INFO_NOW_COMM_TRIFF_R_1_DEMAND_TIME,//002F  /*"��ǰ��Ϸ��� �޹� 1  ���� ʱ��"*/
    E_INFO_NOW_COMM_TRIFF_R_2_DEMAND,//002F  /*"��ǰ��Ϸ��� �޹� 2  ����"*/
    E_INFO_NOW_COMM_TRIFF_R_2_DEMAND_TIME,//002F  /*"��ǰ��Ϸ��� �޹� 2  ����ʱ��""*/
    E_INFO_NOW_TRIFF_R_I_DEMAND,   //0030  /*"��ǰ���� I�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_I_DEMAND_TIME,   //0030  /*"��ǰ���� I�����޹� ����ʱ��"*/
    E_INFO_NOW_TRIFF_R_II_DEMAND,  //0031  /*"��ǰ���� II�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_II_DEMAND_TIME,  //0031  /*"��ǰ���� II�����޹� ����ʱ��"*/
    E_INFO_NOW_TRIFF_R_III_DEMAND, //0032  /*"��ǰ���� III�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_III_DEMAND_TIME, //0032  /*"��ǰ���� III�����޹� ����ʱ��"*/
    E_INFO_NOW_TRIFF_R_IV_DEMAND,  //0033  /*"��ǰ���� V�����޹� ����"*/
    E_INFO_NOW_TRIFF_R_IV_DEMAND_TIME,  //0033  /*"��ǰ���� V�����޹� ����ʱ��"*/

    E_INFO_LAST_COMM_TRIFF_A_ENGY,  //0038  /*"��N��  ��Ϸ��� �й�     ����"*/
    E_INFO_LAST_COMM_TRIFF_R_1_ENGY,//0039  /*"��N����Ϸ��� �޹� 1  ����"*/
    E_INFO_LAST_COMM_TRIFF_R_2_ENGY,//003a  /*"��N����Ϸ��� �޹� 2  ����"*/
    
    E_INFO_LAST_COMM_TRIFF_R_I_ENGY,//003b  /*"��N�·��� I�����޹� ����"*/
    E_INFO_LAST_COMM_TRIFF_R_II_ENGY,//003C  /*"��N�·��� II�����޹� ����"*/
    E_INFO_LAST_COMM_TRIFF_R_III_ENGY,//003D  /*"��N�·��� III�����޹� ����"*/
    E_INFO_LAST_COMM_TRIFF_R_IV_ENGY,//003E  /*"��N�·��� IV�����޹� ����"*/

    E_LAST_LOST_A_V_TIME,            //003F  /*"��N  ��A ��ʧѹʱ��"*/
    E_LAST_LOST_B_V_TIME,            //0040  /*"��N  ��B ��ʧѹʱ��"*/
    E_LAST_LOST_C_V_TIME,            //0041  /*"��N  ��C ��ʧѹʱ��"*/
    
    E_SUM_LOST_V_COUNTS,             //0042  /*"��ʧѹ����"*/
    E_SUM_LOST_V_TIME,               //0043  /*"��ʧѹ�ۼ�ʱ��"*/
    E_LAST_LOST_V_TIME,              //0044  /*"��N  �� ��ʧѹʱ��"*/
#endif    
    E_MAX_SHOWINFO_ID,
}EN_DISP_INFO;

/*"���ݵ�λ���ʽ ��Ӧ stFormat"*/  
typedef enum
{
    /*"���ݸ�ʽ"*/ /*"���ݵ�λ"*/ /*"����:����С��λ��"*/
    E_FORMAT_BCD_4_KWH_2,     //{E_BCD_4_BYTE,E_KWH_UNIT,-2},       //0000
    E_FORMAT_BCD_4_KWH_MATOU, //{E_BCD_4_BYTE,E_KWH_UNIT,E_MATUO},  //0001
    E_FORMAT_UBCD_4_KWH_2,    //{E_UBCD_4_BYTE,E_KWH_UNIT,-2},      //0002
    E_FORMAT_UBCD_4_KWH_MATOU,//{E_UBCD_4_BYTE,E_KWH_UNIT,E_MATUO}, //0003
    E_FORMAT_YYMMDDHH_NONE_0, //{E_YYMMDDHH,E_NONE_UNIT,0},         //0004
    E_FORMAT_UBCD_4_YUAN_2,   //{E_UBCD_4_BYTE,E_YUAN_UNIT,-2},     //0005
    E_FORMAT_NONE_6,          //{E_NONE_FORMAT,0,0},                //0006
    E_FORMAT_UBCD_2_V_1,      //{E_UBCD_2_BYTE,E_V_UNIT,-1},        //0007
    E_FORMAT_BCD_3_A_3,       //{E_BCD_3_BYTE,E_A_UNIT,-3},         //0008
    E_FORMAT_UBCD_3_A_3,      //{E_UBCD_3_BYTE,E_A_UNIT,-3},        //0009
    E_FORMAT_BCD_3_KW_DATUO,  //{E_BCD_3_BYTE,E_KW_UNIT,E_DATUO},   //000A
    E_FORMAT_UBCD_3_KW_DATUO, //{E_UBCD_3_BYTE,E_KW_UNIT,E_DATUO},  //000B
    E_FORMAT_BCD_2_NONE_3,    //{E_BCD_2_BYTE,E_NONE_UNIT,-3},      //000C
    E_FORMAT_UBCD_2_NONE_3,   //{E_UBCD_2_BYTE,E_NONE_UNIT,-3},     //000D
    E_FORMAT_UBCD_2_HZ_2,     //{E_UBCD_2_BYTE,E_HZ_UNIT,-2},       //000E
    E_FORMAT_UBCD_2_V_2,      //{E_UBCD_2_BYTE,E_V_UNIT,-2},        //000F
    E_FORMAT_UBCD_4_NONE_0,   //{E_UBCD_4_BYTE,E_NONE_UNIT,0},      //0010
    E_FORMAT_UBCD_4_YUAN_4,   //{E_UBCD_4_BYTE,E_YUAN_UNIT,-4},     //0011
    E_FORMAT_UBCD_1_NONE_0,   //{E_UBCD_1_BYTE,E_NONE_UNIT,0},      //0012
    E_FORMAT_UBCD_2_MINUTE_0, //{E_UBCD_2_BYTE,E_MINUTE_UNIT,0},    //0013
    E_FORMAT_UBCD_1_MINUTE_0, //{E_UBCD_1_BYTE,E_MINUTE_UNIT,0},    //0014
    E_FORMAT_WWYYMMDD_NONE_0, //{E_WW_YYMMDD,E_NONE_UNIT,0},        //0015
    E_FORMAT_YYMMDD_NONE_0,   //{E_YYMMDD,E_NONE_UNIT,0},           //0016
    E_FORMAT_HHMMSS_NONE_0,   //{E_HHMMSS,E_NONE_UNIT,0},           //0017
    E_FORMAT_YYMMDDHHMM_NONE_0,//{E_YYMMDDHHMM,E_NONE_UNIT,0},       //0018
    E_FORMAT_UBCD_2_NONE_0,   //{E_UBCD_2_BYTE,E_NONE_UNIT,0},      //0019
    E_FORMAT_U_1_NONE_0,      //{E_U_1_BYTE,E_NONE_UNIT,0},         //001A
    E_FORMAT_UBCD_3_NONE_0,   //{E_UBCD_3_BYTE,E_NONE_UNIT,0},      //001B
    E_FORMAT_ID_CODE_4_NONE_0,//{E_ID_CODE_4_FORMAT,0,0},           //001C  
    E_FORMAT_ID_CODE_2_NONE_0,//{E_ID_CODE_2_FORMAT,E_NONE_UNIT,0}, //001D 
    E_FORMAT_ASCII_32_NONE_0, //{E_ASCII_32_LENGTH,E_NONE_UNIT,0},  //0x1E
    E_FORMAT_ASCII_6_NONE_0,  //{E_ASCII_6_LENGTH,E_NONE_UNIT,0},   //0x1F
    E_FORMAT_ASCII_4_NONE_0,  //{E_ASCII_4_LENGTH,E_NONE_UNIT,0},   //0x20
    E_FORMAT_ASCII_10_NONE_0, //{E_ASCII_10_LENGTH,E_NONE_UNIT,0},  //0x21
    E_FORMAT_ASCII_16_NONE_0, //{E_ASCII_16_LENGTH,E_NONE_UNIT,0},  //0x22
    E_FORMAT_BCD_4_NONE_0,    //{E_BCD_4_BYTE,E_NONE_UNIT,0},       //0023
    E_FORMAT_BCD_3_NONE_0,    //{E_BCD_3_BYTE,E_NONE_UNIT,0},       //0024
    E_FORMAT_U_2_NONE_0,      //{E_U_2_BYTE,E_NONE_UNIT,0},         //0025
    E_FORMAT_U_4_MINUTE_0,    //{E_U_4_BYTE,E_MINUTE_UNIT,0},       //0x26
    E_FORMAT_MMDDHHMM_NONE_0, //{E_MMDDHHMM,E_NONE_UNIT,0},         //0027
    E_FORMAT_DDHH_NONE_0,     //{E_DDHH,E_NONE_UNIT,0},             //0028 /*"��ʾ��ʽ: δ��BCDת��"*/
    E_FORMAT_U_6_NONE_0,      //{E_U_6_BYTE,E_NONE_UNIT,0},         //0x29
    E_FORMAT_HHMM_NONE_0,     //{E_HHMM,E_NONE_UNIT,0},             //002A
    E_FORMAT_NN_MMDD_NONE_0,  //{E_NN_MMDD,E_NONE_UNIT,0},          //002B
    E_FORMAT_NN_HHMM_NONE_0,  //{E_NN_HHMM_42,E_NONE_UNIT,0},       //002C  /*������*/
    E_FORMAT_NN_YYMMDD_NONE_0,//{E_NN_YYMMDD,E_NONE_UNIT,0},        //002D
    E_FORMAT_YYYYMMDDHHMMSS_FF_NONE_0,//{E_YYYYMMDDHHMMSS_FF,E_NONE_UNIT,0}//002E  XYT
    E_FORMAT_MMDDHH_NONE_0,   //{E_MMDDHH,E_NONE_UNIT,0},           //002F /*"��ʾ��ʽ: δ��BCDת��"*/
    E_FORMAT_ASCII_8_NONE_0,  //{E_ASCII_8_LENGTH,E_NONE_UNIT,0},   //0x30
    E_FORMAT_YYMMDDHHMMSS_NONE_0,//{E_YYMMDDHHMMSS,E_NONE_UNIT,0},    //0031
    E_FORMAT_YYYYMMDDHHMMSS_NONE_0,//{E_YYYYMMDDHHMMSS,E_NONE_UNIT,0}//0032  yl     //0032  /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_U_2_V_1,         //{E_U_2_BYTE,E_V_UNIT,-1},           //0033
    E_FORMAT_S_4_A_3,         //{E_S_4_BYTE,E_A_UNIT,-3},           //0034
    E_FORMAT_S_4_KW_4,        //{E_S_4_BYTE,E_KW_UNIT,-4},          //0x35      /*2018-01-29:�ĳ�kw*/
    E_FORMAT_S_2_NONE_3,      //{E_S_2_BYTE,E_NONE_UNIT,-3},        //0036
    E_FORMAT_U_2_HZ_2,        //{E_U_2_BYTE,E_HZ_UNIT,-2},          //0037
    E_FORMAT_U_4_KWH_2,       //{E_U_4_BYTE,E_KWH_UNIT,-2},         //0038
    E_FORMAT_S_4_KWH_2,       //{E_S_4_BYTE,E_KWH_UNIT,-2},         //0039
    E_FORMAT_U_1_NONE_0_REPEAT,//{E_U_1_BYTE,E_NONE_UNIT,0},         //003A
    E_FORMAT_U_4_YUAN_4,      //{E_U_4_BYTE,E_YUAN_UNIT,-4},        //003B
    E_FORMAT_U_4_YUAN_2,      //{E_U_4_BYTE,E_YUAN_UNIT,-2},        //003C
    E_FORMAT_U_4_NONE_0,      //{E_U_4_BYTE,E_NONE_UNIT,0},         //0x3D
    E_FORMAT_U_1_SECOND_0,    //{E_U_4_BYTE,E_SECOND_UNIT,0},       //0x3E
    E_FORMAT_U_6_NONE_0_REPEAT,//{E_U_6_BYTE,E_NONE_UNIT,0},         //0x3F
    E_FORMAT_NONE_40,         //{E_NONE_FORMAT,0,0},                //0x40  /*����ʱ�����ظ��ģ��������Ķ�������*/,         //0x40 /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_U_4_A_3,         //{E_U_4_BYTE,E_A_UNIT,-3},           //0x41
    E_FORMAT_U_4_W_1,         //{E_U_4_BYTE,E_W_UNIT,-1},           //0x42
    E_FORMAT_U_2_NONE_3,      //{E_U_2_BYTE,E_NONE_UNIT,-3},        //0x43
    E_FORMAT_NONE_44,         //{E_NONE_FORMAT,0,0},                //0x44  /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_NN_MMDD_42_NONE_3,//{E_NN_MMDD_42,E_NONE_UNIT,0},      //0x45
    E_FORMAT_U_4_NONE_3,       //{E_U_4_BYTE,E_NONE_UNIT,-2},       //0046
    E_FORMAT_NONE_47,         //{E_NONE_FORMAT,0,0},                //0047 /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_ASCII_8_NONE_0_REPEAT, //{E_ASCII_8_LENGTH,E_NONE_UNIT,0},   //0x48
    E_FORMAT_DDHH_NONE_0_REPEAT,     //{E_DDHH,E_NONE_UNIT,0},             //0049
    E_FORMAT_NN_YYYYMMDDWW_NONE_0,//{E_NN_YYYYMMDDWW,E_NONE_UNIT,0}, //004A   /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_MMDDHH_NONE_0_REPEAT,//{E_MMDDHH,E_NONE_UNIT,0},           //0x4B
    E_FORMAT_U_2_V_2,         //{E_U_2_BYTE,E_V_UNIT,-2},           //004C
    E_FORMAT_NONE_4D,         //{E_NONE_FORMAT,0,0},                //004D   /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_NONE_4E,         //{E_NONE_FORMAT,0,0},                //004E   /*����ʱ�����ظ��ģ��������Ķ�������*/
    E_FORMAT_U_5_NONE_0,      //{E_U_5_BYTE,E_NONE_UNIT,0},         //0x4F
    E_FORMAT_S_4_A_4,         //{E_S_4_BYTE,E_A_UNIT,-4},           //0x50
    
    E_FORMAT_U_8_KWH_4,         //{E_U_8_BYTE,E_KWH_UNIT,-4},         //0X5B
    E_FORMAT_S_8_KWH_4,         //{E_S_8_BYTE,E_KWH_UNIT,-4},         //0X5C

    E_FORMAT_U_H_INT_KWH_0,          //{E_UENGY_H_INT,E_KWH_UNIT,E_MATUO},    //0x66
    E_FORMAT_U_H_DECIMAL_KWH_MATUO,  //{E_UENGY_H_DECIMAL,E_KWH_UNIT,E_MATUO}, //0x67
    E_FORMAT_S_H_INT_KWH_0,          //{E_ENGY_H_INT,E_KWH_UNIT,E_MATUO},      //0x68
    E_FORMAT_S_H_DECIMAL_KWH_MATUO,  //{E_ENGY_H_DECIMAL,E_KWH_UNIT,E_MATUO},  //0x69

    #ifdef THREE_PHASE
    E_FORMAT_S_4_KVAR_4,        //{E_S_4_BYTE,E_KVAR_UNIT,-4},        //0x51 
    E_FORMAT_S_4_KVA_4,         //{E_S_4_BYTE,E_KVA_UNIT,-4},         //0x52 
    
    E_FORMAT_BCD_3_KVAR_DATUO, //{E_UBCD_3_BYTE,E_KVAR_UNIT,E_DATUO},//0x53
    E_FORMAT_BCD_3_KVA_DATUO,  //{E_UBCD_3_BYTE,E_KVA_UNIT,E_DATUO}, //0x54
    
    E_FORMAT_S_2_NONE_2,        //{E_S_2_BYTE,E_NONE_UNIT,-2},        //0x55
    E_FORMAT_S_2_NONE_1,        //{E_S_2_BYTE,E_NONE_UNIT,-1},        //0x56
    E_FORMAT_U_4_KW_4,          //{E_U_4_BYTE,E_KW_UNIT,-4}},         //0x57
    E_FORMAT_U_2_NONE_2,        //{E_U_2_BYTE,E_NONE_UNIT,-2},        //0x58
    E_FORMAT_DEMAND_AND_TIME_698,//{E_DEMAND_AND_TIME_698,E_NONE_UNIT,0},//0x59                                //0x59
    E_FORMAT_DEMAND_AND_TIME,    //{E_DEMAND_AND_TIME,E_NONE_UNIT,0},  //0x5A /*��ʾ��*/                            //0x5A
    
    E_FORMAT_S_4_KVARH_2,       //{E_U_8_BYTE,E_KVARH_UNIT,-2},         //0X5d
    E_FORMAT_S_8_KVARH_4,       //{E_S_8_BYTE,E_KWH_UNIT,-4},           //0X5E
    E_FORMAT_BCD_4_KVARH_MATOU, //{E_BCD_4_BYTE,E_KVARH_UNIT,E_MATUO},  //0X5F

    E_FORMAT_U_4_KVARH_2,       //{E_U_4_BYTE,E_KVARH_UNIT,-2},         //0X60
    E_FORMAT_U_8_KVARH_4,       //{E_U_8_BYTE,E_KVARH_UNIT,-4},         //0X61
    E_FORMAT_UBCD_4_KVARH_MATOU,//{E_UBCD_4_BYTE,E_KVARH_UNIT,E_MATUO}, //0X62
    E_FORMAT_U_2_NONE_1,        //{E_U_2_BYTE,E_NONE_UNIT,-1},          //0x63
    E_FORMAT_UBCD_2_NONE_1,     //{E_UBCD_2_BYTE,E_NONE_UNIT,-1},       //0x64
    E_FORMAT_UBCD_3_NONE_2,     //{E_UBCD_2_BYTE,E_NONE_UNIT,-2},       //0x65

    E_FORMAT_U_H_INT_KVARH_0,         //{E_UENGY_H_INT,E_KVARH_UNIT,E_MATUO},     //0x6A
    E_FORMAT_U_H_DECIMAL_KVARH_MATUO, //{E_UENGY_H_DECIMAL,E_KVARH_UNIT,E_MATUO}, //0x6B
    E_FORMAT_S_H_INT_KVARH_0,         //{E_ENGY_H_INT,E_KVARH_UNIT,E_MATUO},      //0x6C
    E_FORMAT_S_H_DECIMAL_KVARH_MATUO, //{E_ENGY_H_DECIMAL,E_KVARH_UNIT,E_MATUO},  //0x6D
    
    #endif 
    MAX_E_FORMAT, 
}EN_FORMAT_UNIT_SCALER;


#if 1
typedef INT8U  (* TOn_Read_Event)(INT8U *pdata);
typedef INT8U  (* TOn_Write_Event)(INT8U *pdata);
//typedef INT8U  (* ObjectMethod)(INT8U *pdata);

/*"����"*/
typedef struct 
{
		TOn_Write_Event OnWrite_Beford;
		//TOn_Write_Event OnWrite_End;
		TOn_Read_Event OnRead;
}ST_FUNCTION;
#endif

/*------------------------------------------------------*/
/*�����Ͷ��壬��TYPE��ͷ*/


/* */
extern const INT8U TYPE_OAD[2]; 

extern const INT8U TYPE_BOOL[2];

extern const INT8U TYPE_DATES[2];

//extern const INT8U TYPE_COMDCB[2];

extern const INT8U TYPE_NULL[2];
extern const INT8U TYPE_OI[2];
extern const INT8U TYPE_U16[2];
extern const INT8U TYPE_S16[2];
extern const INT8U TYPE_ENUM[2];

extern const INT8U TYPE_S8[2];
extern const INT8U TYPE_U8[2];
extern const INT8U TYPE_U32[2];

extern const INT8U TYPE_S32[2];

extern const INT8U TYPE_BITS_8[2];
extern const INT8U TYPE_BITS_16[2];
extern const INT8U TYPE_BITS_32[2];
//extern const INT8U TYPE_BITS_128[2];

extern const INT8U TYPE_VIS_32[2];
extern const INT8U TYPE_VIS_16[2];
extern const INT8U TYPE_VIS_8[2];
extern const INT8U TYPE_VIS_6[2];
extern const INT8U TYPE_VIS_4[2];

//extern const INT8U TYPE_OCTS_2[2];

/*  �忨��������ͷ	octet-string(5) C21_ATT3_F100*/
extern const INT8U TYPE_OCTS_5[2];  

/*4001 ͨ�ŵ�ַ, 4002 ���  */
extern const INT8U TYPE_OCTS_6[2];

/*���������ܲ��ܶ����һ���䳤?*/
/*  �����к�	octet-string(8)  C21_ATT2_F100*/
extern const INT8U TYPE_OCTS_8[2]; 

//extern const INT8U TYPE_OCTS_14[2];

/*C21_ATT4_F100*/
extern const INT8U TYPE_OCTS_16[2];

#ifdef THREE_PHASE

/*4117 �ڼ�������������*/
extern const INT8U TYPE_TI[2];
#endif
/*�����Ͷ��壬END*/
/*------------------------------------------------------*/

/*"************************************************"*/
/*��������ָ�붨�壬��ARRAY��ͷ*/
//const INT8U AR_U8[4] = {array,4,unsigned_t,1};
//const INT8U AR_S8[4] = {array,4,integer,1};

extern const INT8U AR_4_U32[4];
extern const INT8U AR_PHASE_U16[4];

//extern const INT8U AR_S16[4];
extern const INT8U AR_SUM_PHASE_S16[4];

/*4018 ���ʵ�ۣ�*/
//extern const INT8U AR_U32[4];
//extern const INT8U AR_S32[4];
extern const INT8U AR_SUM_PHASE_U32[4];
extern const INT8U AR_TRIFF_U32[4];
extern const INT8U AR_PHASE_S32[4];
#ifdef THREE_PHASE
extern const INT8U AR_6_U32[4];
#endif
extern const INT8U AR_TRIFF_S32[4];
extern const INT8U AR_SUM_PHASE_S32[4]; 

//extern const INT8U AR_U64[4];
//extern const INT8U AR_S64[4];

extern const INT8U AR_TRIFF_U64[4];
extern const INT8U AR_TRIFF_S64[4];

extern const INT8U AR_OAD[4];
extern const INT8U AR_OMD[4];

extern const INT8U AR_OI[4];

extern const INT8U AR_N_OI[4];

#ifdef THREE_PHASE
extern const INT8U AR_U64[4];
#endif
/*CLASS19ATT6*/
extern const INT8U AR_VIS[4];

/*4005 ���ַ*/
extern const INT8U AR_OCTS[4];


/*C6_ATT2_2014 ���ܱ�����״̬��*/
extern const INT8U AR_7_BITS_16[4];

#ifdef THREE_PHASE

/*������ֵ*/
//extern const INT8U AR_DEMAND[8];
extern const INT8U AR_TRIFF_DEMAND[8];
/*������ֵ*/
//extern const INT8U AR_DEMAND_R[8];
extern const INT8U AR_TRIFF_DEMAND_R[8];
#endif

/*��������ָ�붨�� END*/
/*------------------------------------------------------*/

/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/

/*�ṹ������ָ�붨�壬��ST��ͷ �����һ���ģ�����治���ظ�*/
/*300E,300F,3010,3011,302A,302B,302C,302E,302F*/
extern const INT8U ST_1_U8[4];
extern const INT8U ST_2_DATE_U8[6];

/*3005,3007,3008*/
extern const INT8U ST_2_S32_U8[6];

/*ʱ��Դ 4006  */
extern const INT8U ST_2_EN[8];
/*80000200 */
extern const INT8U ST_2_U32_U16[6];

/*F1010200  */
extern const INT8U ST_2_OI_U16[6];

#ifdef THREE_PHASE
extern const INT8U ST_2_NULL_NULL[6];
#endif

/*2025 401E���������ֵ*/
extern const INT8U ST_2_U32[6];
/*C22_ATT2_F202*/
extern const INT8U AR_ST_2_VIS_5_COMDCB[8];
/*C22_ATT4_F206*/
//const INT8U ST_2_DATEB[] = {structure,2,time,7,time,7};

/*C22_ATT127_F202,C22_ATT128_F209*/
extern const INT8U ST_2_OAD_COMDCB[6];
/*C22_ATT127_F205*/
extern const INT8U ST_2_OAD_EN[6];

/*CLASS17_ATT4*/
extern const INT8U ST_2_U8[6];

/*C22_ATT127_F200*/
extern const INT8U ST_3_OAD_COMDCB_EN[8];

/*���������ֵ  401F  */
extern const INT8U ST_3_U32_U32_U32[8];
/*FF60 ��·�㳤�� */
extern const INT8U ST_3_U16[8];

/*C21_ATT7_F100*/
extern const INT8U ST_3_U32[8];

/*CLASS9_METHOD4*/
extern const INT8U ST_3_U16_OAD_U16[8];
/*CLASS17_METHOD_5*/
extern const INT8U ST_3_CSD_U8_U16[8];
/*C22_ATT2_F201*/
extern const INT8U AR_ST_3_VIS_5_COMDCB_EN[10];

extern const INT8U AR_ST_3_VIS_6_COMDCB_EN[10];

/*C22_ATT2_F209*/
extern const INT8U AR_ST_3_NULL_COMDCB_NULL[10];
/*C22_ATT127_F200*/
extern const INT8U ST_3_OAD_COMDCB_EN[8];

/*40300200,*/
extern const INT8U ST_4_U16[10];
//extern const INT8U ST_4_U16_NULL_3[10]; 
//extern const INT8U ST_4_U16_NULL_1[10];

extern const INT8U AR_TYPE_ENUM[4];

/*C22_ATT2_F205*/
extern const INT8U AR_ST_4_VIS_1_EN_EN_EN[12];

/*4000400 */
extern const INT8U ST_5_U8[12];
/*�汾��Ϣ*/
extern const INT8U TYPE_VERSION_INFO[14];
extern const INT8U TYPE_CONNECT_INFO[16];

/*�豸����λ�� 4004 */
extern const INT8U TYPE_LOCATION[24];
/*ʱ��ʱ���� 400C  */
//extern const INT8U TYPE_YEARLY_DAILY[12];

/*LCD���� 4007  */
extern const INT8U TYPE_LCD_PARAMENT[16];

#ifdef THREE_PHASE

extern const INT8U ST_2_U16_U8[6];
/*3001,3002*/

extern const INT8U ST_2_U32_U8[6];
/*3009,300A,300B,301D,301E,302D*/

extern const INT8U ST_2_S16_U8[6];
/*300C,301D,301E,302D*/

extern const INT8U ST_3_U16_S32_U8[8];


/*���������Ϊ��·485���������ֱ�ΪRS4851��RS4852 C22_ATT2_F201_2*/
//extern const INT8U ST_3_VIS_6_COMDCB_EN[8];


extern const INT8U ST_4_U16_S32_S32_U8[10];
/*3004*/

extern const INT8U ST_4_U16_U16_S32_U8[10];
/* 3000,*/

extern const INT8U ST_4_U32_U32_DATE_S_DATE_S[];

/*����������ΪSTRUCT,������������TYPE*/
/*������ֵ*/
//extern const INT8U TYPE_DEMAND[6];
/*������ֵ*/
extern const INT8U TYPE_DEMAND_R[6];
/*FF650200*/
//extern const INT8U ST_4_U32[10];
/*CLASS7 3000 ATT13����ʧѹ�¼�������3*/
//extern const INT8U TYPE_LOST_VOLT_STATIC[10];

#endif

/*�ṹ������ָ�붨�� END */

/*-------------------------------------------------------*/

/*���Ͻṹ�����Ͷ��� ��ARRAY_STRUCT��ͷ*/

/*�¼��ϱ�״̬*/
//extern const INT8U TYPE_REPORT_STATUS[8];
/*4011 ��������*/
extern const INT8U TYPE_HOLIDAY_TABLE[8];

/*��ǰֵ��¼��*/
extern const INT8U AR_ST_2_U32[8];

extern const INT8U AR_ST_2_ENUM_ST_2_U32[12];
//extern const INT8U AR_ST_2_NULL_ST_2_U16_NULL[12];
/*24 ��ʱ��״̬��¼��*/
extern const INT8U AR_ST_2_DATES[8];
/*7 ��ʱ��״̬��¼��*/
extern const INT8U AR_ST_2_NULL_ST_2_DATES[12];


/*��������Ϊ8000�ķ���129-131����*/
extern const INT8U AR_ST_2_OAD_EN[8];
/*F1010300*/
extern const INT8U AR_ST_2_OI_U16[8];
/*CLASS17_ATT2*/
extern const INT8U AR_ST_2_CSD_U8[8];

/*4014 ʱ����*/
extern const INT8U AR_ST_3_YEARLY_TABLE[10];
/*ʱ�α�*/
extern const INT8U AR_ST_3_DAILY_SLOT_TABLE[12];
extern const INT8U AR_8_ST_3_DAILY_SLOT_TABLE[12];

/*��������Ϊ8000�ķ���129-131����*/
extern const INT8U AR_ST_3_OAD_EN_AS[10];
/*CLASS9_ATT3_METHOD7*/
extern const INT8U AR_ST_3_U16_OAD_U16[10];
/*��������Ϊ8000�ķ���129-131����*/
extern const INT8U AR_ST_4_OAD_U8_U16_BOOL[12];

/*CLASS18_ATT10_METHOD7  �ļ��ȶ�*/
extern const INT8U ST_5_U8_U8_U32_U32_U16[12];

/*4016 ʱ�α�*/
//extern const INT8U TYPE_DAILY_TABLE[12];

/*401A ���ݵ��*/
//extern const INT8U TYPE_STEP_PRICE[20];

/*4116 ������*/
extern const INT8U TYPE_BILL_DAY[20];
/*CLASS7_ATT10_CLASS24_ATT14 */ /*7��������10��24��������14���һ�η���ʱ������һ�ν���ʱ��*/ 
//extern const INT8U TYPE_LAST_START_TIME_END_TIME[8];


/*C7_ATT2_401A ���ݵ�۱��룬���������������л���������*/
extern const INT8U TYPE_STEP_PRICE[66];   

#ifdef THREE_PHASE
/*��������*/
//extern const INT8U ARRAY_TYPE_DEMAND[8];  
//extern const INT8U ARRAY_TYPE_DEMAND_R[8]; 

/*�ܵ�ѹ�ϸ��� C6_ATT2_2130*/
extern const INT8U TYPE_VOLT_OK_RATE[26];
#endif

/*���Ͻṹ�����Ͷ��� END*/
/*-----------------------------------------*/

/*Ԫ�ط������� CLS_1_6_OBJECT_ELE */
typedef enum
{
   E_ELE_A_ENGY_COM  = 0, /*����й�*/
   E_ELE_A_ENGY,          /*1.�й�����*/
   E_ELE_R_ENGY_COM,      /*2.����޹�����*/
   E_ELE_R_ENGY,          /*3.�������޹�����*/
   
   /*class3*/
   E_ELE_VOLT,            /*4.��ѹ*/   
   E_ELE_CURR,            /*5.����*/
   E_ELE_PHASE,           /*6.���*/
   
   /*class4*/
   E_ELE_A_POWER,         /*7.�й�����*/
   E_ELE_R_POWER,         /*8.�޹�����*/
   E_ELE_V_POWER,         /*9.���ڹ���*/
   E_ELE_FACTOR,          /*10.��������*/
   
   /*class6*/
   E_ELE_FEQ,             /*11.����Ƶ��*/
   E_ELE_CLK_VOLT,        /*12.ʱ�ӵ�ص�ѹ*/
   E_ELE_STATUS_8,        /*13.���ܱ�����״̬��*/
   E_ELE_PRICE,           /*14.��� */
   E_ELE_SAFE_TIME,       /*15.��ȫ��֤ʣ��ʱ��*/
   E_ELE_MONEY,           /*16.���*/ 
   E_ELE_STATUS_16,       /*17.״̬��*/
   E_ELE_STATUS_32,       /*18.��������ִ��״̬��*/
   E_ELE_CLK_WORK_TIME,   /*19.ʱ�ӵ�ع���ʱ��*/

   /*�߾��ȵ�����-----------*/
   E_ELE_A_ENGY_COM_H,    /*20.����й�*/
   E_ELE_A_ENGY_H,        /*21.�й�����*/

#ifdef THREE_PHASE   
   E_ELE_R_ENGY_COM_H,    /*22.����޹�����*/
   E_ELE_R_ENGY_H,        /*23.�������޹�����*/
   
   /*�������---------------*/
   E_ELE_A_MAX_DEMAND,    /*24.�й��������*/
   E_ELE_A_NULL,          /*25.��*/
   E_ELE_R_MAX_DEMAND,    /*26.�����޹��������*/
   E_ELE_R_MAX_DEMAND_COM,/*27.����޹��������*/
   
   /*������*/
   E_ELE_TEMP,            /*28.�����¶�*/
   E_ELE_LOW_POWER_VOLT,  /*29.ͣ�糭���ص�ѹ*/
   E_ELE_CURR_A_DEMAND,   /*30.��ǰ�й����� */
   E_ELE_CURR_R_DEMAND,   /*31.��ǰ�޹����� */
   E_ELE_IMBANLCE_RATE,   /*32.��ƽ���� */
   E_ELE_VAH,             /*33.��ʱֵ */    
   E_ELE_VOLT_RATE,       /*34.�ܵ�ѹ�ϸ���*/   

   E_ELE_PHASE_A_ENGY,     /*35.�й�����*/
   E_ELE_PHASE_R_ENGY_COM,/*36.����޹�����*/
   E_ELE_PHASE_R_ENGY,    /*37.�������޹�����*/

   E_ELE_PHASE_A_ENGY_H,     /*38.�й�����*/
   E_ELE_PHASE_R_ENGY_COM_H, /*39.����޹�����*/
   E_ELE_PHASE_R_ENGY_H,     /*40.�������޹�����*/
#endif   
   E_ELE_MAX,                
}EN_OBJECT_ELE;


/*һЩ�������AID ����Ϣ:��дȨ�ޣ�IID������ָ��*/
typedef struct
{
   INT8U AID;             /*"����"*/    
   INT8U Right_RW;        /*"��дȨ�� 0:ֻ����0x0f:�ɶ�д*/     
   INT8U Max_IID;         /*"���IID"*/  
   /*INT8U res*/
   const INT8U *Pointer;  /*��������ָ��*/              
}ST_SPECIAL_CLASS_AID_INFO;

/*"��ʾ"*/
 typedef struct
{
    INT8U schar[MAX_SCHAR_NUM];
}ST_SHOW_INFO;

typedef struct
{	
	INT8U TransId;  /*"ͨ�Ŵ����ʽ"*/
	INT8U ShowId;  /*"��ʾ�����ʽ"*/	
}ST_FORMAT_ID;

typedef struct
{
    ST_OAD Oad;        /*OAD*/
	INT8U  Class;      /*��*/
    INT8U  Right_RW;   /*��дȨ��*/
    INT8U  element;    /*��ӦST_OBJECT_ELC��Ԫ��λ�ã�����ǲα���������Ԫ�غ���*/
    INT8U  seq_num;    /*OAD��Ӧ��λ��*/
    
    const INT8U * Pointer; /*��������(����ָ��) */
}SUB_OBJECT;


/*������OBIS��Ϣ*/
/*8 byte*/
typedef struct
{ 
    INT8U  Seq_Num;   /*�б����к� ��ST_Class8_ObjectList�е�һ��*/
    INT8U  num;       /*"OAD��ӦOBIS��Ԫ�ظ���"*/
    INT16U Obis;      /*"��num�йأ�numС��0x80,����������OBIS��ֵ��������OBIS��"*/

    INT8U  Offset;    /*"���������û�ã����������ֽڶ��룬����������ʱ���Ŷ��塣�����������ݵ�ƫ�� ���̶�Ϊ0���ȱ��������濴������پ����Ƿ�ȥ�� "*/
    INT8U  FormatId;  /*"ͨ�š���ʾ��ʽID"*/
    INT8U  FunctionId;/*"��д����ID"*/
    INT8U  ShowinfoId;/*"��ʾ���ñ�ID"*/
}ST_PARAMENT_OAD_OBIS_INFO;
/*������OBIS��Ϣ END*/

extern const st_object_ele CLS_1_6_OBJECT_ELE[];
extern const ST_OI_METHOD_NUM_POINTER OI_METHOD_NUM_POINTER_ALL[16];
extern const ST_FORMAT_ID stFormatId[];
extern const ST_FMT_ATTR stFormat[];
extern const ST_SHOW_INFO stShowinfo[];
extern const ST_FUNCTION stFunction[];
//#define MAX_OBJECT_ELE_NUM  ARRAY_SIZE(CLS_1_6_OBJECT_ELE)
void class_comm_build(void);

#include "class1_enery.h"
#include "class2_demand.h"
#include "class3_phase_varible.h"
#include "class4_power.h"
#include "class6_data_varible.h"
#include "class7_comm_event.h"
#include "class8_parament.h"
#include "class9_freeze.h"
#include "App\UserInterface\PP\PP_698\Process\Pp_process.h"
#include "class17_display.h"
#include "class18_file.h"
#include "class19_device.h"
#include "class20_connect.h"
#include "class21_esam.h"
#include "class22_input_output.h"
#include "class24_phase_event.h"

/*"******************************************************"*/
#endif


