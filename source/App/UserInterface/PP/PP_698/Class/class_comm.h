/*"***********************************************************		
FileName: CLASS_COMM.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 类公用定义头文件		 	
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
/*"IID限制"*/
#ifdef THREE_PHASE
#define  CLASS1_CLASS2_MAX_IID  9  /*定义时，用的最大费率IID，到时要做特殊处理，取值*/
#define  CLASS3_MAX_IID  3 
#define  CLASS4_MAX_IID  4
#else
#define  CLASS1_CLASS2_MAX_IID  5   //
#define  CLASS3_MAX_IID  1
#define  CLASS4_MAX_IID  2
#endif

#define MAXTRIFF_IID   CLASS1_CLASS2_MAX_IID  

#define MAXTRIFF_ABC_IID  1  /*分相电量的最大费率数*/

#define NO_READ_WRITE 0x00  /*即不能读，也不能写，针对一些没有定义OBIS的数据*/
#define READ_ONLY    0x0F   /*只读*/
#define WRITE_ONLY   0xF0   /*只写，明文合闸密码*/
#define READ_WRITE   0xFF   /*可读写*/

#ifdef THREE_PHASE
#define NUM_RELAY   1
#else
#define NUM_RELAY   1
#endif

typedef struct
{
   INT8U Scaler;           /*量纲，换算关系，即10的几次方(负和正)*/
   INT8U UNIT;             /*单位*/ 
   
   const INT8U *Pointer;   /*类型指针*/
}st_object_ele;            /*属性元素*/

typedef INT16U  ObjectMethod;

typedef struct
{
    INT16U OI;             /*"OAD编号"*/ 
    ObjectMethod ObjMeth;  /*"对象方法 ----如果是通过数据中心，则定义为方法ID即可"*/ 
    const INT8U *Pointer;  /*类型指针*/
}ST_OI_METHOD;

typedef struct
{
    INT8U  AID;            /*"AID编号"*/   
    INT8U  OI_NUM;         /*"同一个AID对应的OI个数"*/  
    const ST_OI_METHOD *Pointer;  /*方法数据指针*/    
}ST_OI_METHOD_NUM_POINTER;

/*-------------------------------------------------*/

/*"读写函数编号，对应在stFunction中的位置"*/
typedef enum
{
    FUN_NULL = 0 ,  /*"无功能"*/
    FUN_RUNSTATUS,  /*"运行状态字"*/
    FUN_SET_TIME,   /*" 设置时间"*/
    FUN_LCDPARA,    /*" LCD参数"*/
    FUN_MULTI,      /*"多功能输出"*/
    FUN_RLYSTATUS,  /*"继电器状态"*/
    FUN_EVTRPTSTATUS,/*" 主动上报状态字"*/
    #ifdef THREE_PHASE
    FUN_SET_DU_PARA,/*需量参数设置*/ 
    #endif
    MAX_FUN_ID
}EN_FUNCTION_ID;

/*格式(可以定义包括通信与显示)枚举  对应stFormatId*/
typedef enum
{
    FORMAT_V = 0,  /*"电压格式"*/
    FORMAT_A,      /*"电流格式"*/
    FORMAT_PW,     /*"功率格式"*/
    FORMAT_FACTOR, /*"功率因数格式"*/
    FORMAT_FREQ,   /*"频率格式"*/
    FORMAT_ENGY,   /*"电能量格式"*/
    FORMAT_COMBENGY, /*"组合电量格式"*/
    FORMAT_CTRLPARA, /*""1字节控制类参数格式"*/
    FORMAT_DATETIME, /*""日期时间格式"*/
    FORMAT_TIME, /*"时间格式"*/
    FORMAT_ADDR_H, /*"通信地址高位格式"*/
    FORMAT_ADDR_L, /*"通信地址低位格式"*/
    FORMAT_MTRNO_H, /*"表号高位格式"*/
    FORMAT_MTRNO_L, /*"表号低位格式"*/
    FORMAT_WORD, /*"单字格式"*/
    FORMAT_PRICE, /*"电价格式"*/
    FORMAT_MONEY, /*"剩余金额格式"*/
    FORMAT_BUYTIMES, /*"购电次数格式"*/
    FORMAT_BILLENGY, /*"用电量格式"*/
    FORMAT_BILLDATE,/*"结算日期格式"*/
    FORMAT_IDCODE,/*"字符标识"*/
    FORMAT_N_A,/*"零线电流格式"*/
    FORMAT_CONST,/*"脉冲常数格式"*/
    FORMAT_STEPENGY,/*"阶梯电量格式"*/
    FORMAT_YEARBILL,/*"年结算日格式"*/
    FORMAT_DAILYSLOT,/*"日时段表格式"*/
    FORMAT_DISP,/*"显示项格式"*/
    FORMAT_P4,/*"4字节参数格式"*/
    FORMAT_NULL,/*"无格式"*/
    FORMAT_SWTIME,/*"切换时间格式"*/
    FORMAT_HOLIDAY,/*"公共假日格式"*/
    FORMAT_YEARSLOT,/*"年时区格式"*/
    FORMAT_ASCII4,/*"4byte ASCII格式"*/
    FORMAT_ASCII6,/*"6byte ASCII格式"*/
    FORMAT_ASCII8,/*"8byte ASCII格式"*/
    FORMAT_ASCII16,/*"16byte ASCII格式"*/
    FORMAT_ASCII32,/*"32byte ASCII格式"*/
//    FORMAT_EVTTIME,/*"事件发生时间格式"*/
    FORMAT_EVT_A,/*"事件电流格式"*/
    FORMAT_EVT_PW,/*"事件功率格式"*/
    FORMAT_EVT_FACTOR,/*"事件功率因数格式"*/
//    FORMAT_FRZTIME,/*"冻结时间格式"*/
    FORMAT_PRGOAD,/*"编程OAD格式"*/
    FORMAT_YSLOTTAB,/*"年时区表格式"*/
    FORMAT_U4_UBCD4,/*"互感器变比格式"*/
    FORMAT_BV,  /*"时钟电池电压格式"*/
    FORMAT_RCDOAD,/*"记录OAD格式"*/
    FORMAT_EVT_STEP,/*"阶梯编程记录格式"*/
    FORMAT_CARD_CMDHEAD,/*"插卡操作命令头"*/
    FORMAT_BYTE16,/*"16字节数据"*/
    FORMAT_MINUTE,/*"分钟格式"*/
    FORMAT_U2_UBCD2,/*U2转成UBCD2，以便显示*/
    FORMAT_U1_UBCD2,/*U1转成UBCD2，以便显示*/
    FORMAT_A_4,     /*显示电流相关的参数*/
    FORMAT_ENGY_H,      /*高精度电能量*/
    FORMAT_COMBENGY_H,  /*高精度组合电能量*/   
    
#ifdef THREE_PHASE
    FORMAT_R_PW,     /*"无功功率格式"*/
    FORMAT_VA_PW,    /*"视在功率格式"*/
    FORMAT_S2_2_UBCD2_0,/*百分比*/ 
    FORMAT_S2_1_UBCD2_1,/*百分比*/
    FORMAT_A_PW,        /*需量触发下限*/
    FORMAT_U2_2_UBCD2_0,/*百分比*/ 
    FORMAT_DEMAND,      /*需量*/
    
    FORMAT_COMBENGY_R,  /*组合无功电能量*/
    FORMAT_COMBENGY_R_H,/*高精度组合无功电能量*/  
    
    FORMAT_ENGY_R,      /*无功电能量*/
    FORMAT_ENGY_R_H,      /*无功电能量高精度*/
    FORMAT_U2_1_UBCD2_1,  /*U2 -1转UBCD -1*/
    
    #endif
    MAX_FORMAT_ID /*即没有格式，不用进行格式转换*/
}EN_FORMAT_ID;


/*显示配置表ID枚举  对应 stShowinfo*/
typedef enum
{
    E_INFO_NOW_TRIFF_A_ENGY,    //0000  /*当前   费率 有功     电量*/
    E_INFO_NOW_TRIFF_PA_ENGY,   //0001  /*当前   费率 正向有功 电量*/
    E_INFO_NOW_TRIFF_NA_ENGY,   //0002  /*当前   费率 反向有功 电量*/
    E_INFO_LAST_TRIFF_A_ENGY,   //0003  /*上*月  费率 有功     电量*/
    E_INFO_LAST_TRIFF_PA_ENGY,  //0004  /*上*月  费率 正向有功 电量*/
    E_INFO_LAST_TRIFF_NA_ENGY,  //0005  /*上*月  费率 反向有功 电量*/
    E_INFO_NOW_LAST_SUM_ENGY,   //0006  /*当前上*月  总用 电量   ???*/
    E_INFO_LAST_SUM_ENGY,       //0007  /*上*月  总用 电量 */
    E_INFO_TIME,                //0008  /*时间*/
    E_INFO_REMAIN_MONEY,        //0009  /*剩余 金额*/
    E_INFO_OVERDRAW_MONEY,      //000A  /*透支 金额*/
    E_INFO_NULL,                //000B  /*空白*/
    E_INFO_L,                   //000C  /*L*/
    E_INFO_PA_POWER,            //000D  /*有功功率*/    
    #ifdef THREE_PHASE
    E_INFO_A_PA_POWER,          //0025  /*"A相有功功率"*/
    E_INFO_B_PA_POWER,          //0026  /*"B相有功功率"*/
    E_INFO_C_PA_POWER,          //0027  /*"C相有功功率"*/
    #endif
    
    E_INFO_COS_CREDIT_SUM,      //000E  /*COSΦ总*/
    #ifdef THREE_PHASE
    E_INFO_COS_A,               //000F  /*COSΦA*/
    E_INFO_COS_B,               //0028  /*"COSΦB"*/
    E_INFO_COS_C,               //0029  /*"COSΦC"*/
    #endif
    
    E_INFO_N,                   //0010  /*N*/
    E_INFO_NOW_STEP_PRICE,      //0011  /*当前 阶梯 电 价*/
    E_INFO_NOW_PRICE,           //0012  /*当前 电 价*/
    E_INFO_NOW_TIME,            //0013  /*当前 时间*/
    E_INFO_METER_NUM,           //0014  /*表号*/
    E_INFO_CONST,               //0015  /*常数*/
    E_INFO_SUM,                 //0x16  /*总*/
    E_INFO_CUSTOM_NUM,          //0x17  /*户号*/
    E_INFO_NOW_STEP_SUM_ENGY,   //0x18  /*当前 阶梯 总用电量*/
    E_INFO_LAST_STEP_SUM_ENGY,  //0x19  /*上 阶梯 总用电量*/
    E_INFO_NOW_TRIFF_PRICE,     //001A  /*当前 费率 电价*/
    
    E_INFO_NOW_STEP_PRICE_MONEY,//0x1C  /*当前阶梯 电量 金额*/
    E_INFO_NOW_STEP,            //0x1D  /*当前阶梯*/
    
    E_INFO_STEP_PRICE,          //0x1B  /*阶梯 电价*/
    E_INFO_STEP_PRICE_MONEY,    //0x1C  /*阶梯 电价 金额*/
    E_INFO_STEP,                //0x1D  /*阶梯*/
    E_INFO_TRIFF_PRICE,         //001A  /*费率 电价*/

#ifdef THREE_PHASE    
    E_INFO_R_CONST,             //001E  /*"无功常数"*/
    
    E_INFO_A_I,                 //001F  /*"A相电流"*/
    E_INFO_B_I,                 //0020  /*"B相电流"*/
    E_INFO_C_I,                 //0021  /*"C相电流"*/
    
    E_INFO_A_V,                 //0022  /*"A相电压"*/
    E_INFO_B_V,                 //0023  /*"B相电压"*/
    E_INFO_C_V,                 //0024  /*"C相电压"*/

    E_INFO_A_$,                 //0022  /*"A相相角"*/
    E_INFO_B_$,                 //0023  /*"B相相角"*/
    E_INFO_C_$,                 //0024  /*"C相相角"*/

    E_INFO_RA_POWER,            //002A  /*"总无功功率"*/
    E_INFO_A_RA_POWER,          //002B  /*"A相无功功率"*/
    E_INFO_B_RA_POWER,          //002C  /*"B相无功功率"*/
    E_INFO_C_RA_POWER,          //002D  /*"C相无功功率"*/

    E_INFO_SUM_POWER,            //002A  /*"总功率"*/
    E_INFO_A_POWER,              //002B  /*"A相功率"*/
    E_INFO_B_POWER,              //002C  /*"B相功率"*/
    E_INFO_C_POWER,              //002D  /*"C相功率"*/
    
    E_INFO_NOW_COMM_TRIFF_R_1_ENGY,//002F  /*"当前组合费率 无功 1  电量"*/
    E_INFO_NOW_COMM_TRIFF_R_2_ENGY,//002F  /*"当前组合费率 无功 2  电量"*/
    
    E_INFO_NOW_TRIFF_R_I_ENGY,   //0030  /*"当前费率 I象限无功 电量"*/
    E_INFO_NOW_TRIFF_R_II_ENGY,  //0031  /*"当前费率 II象限无功 电量"*/
    E_INFO_NOW_TRIFF_R_III_ENGY, //0032  /*"当前费率 III象限无功 电量"*/
    E_INFO_NOW_TRIFF_R_IV_ENGY,  //0033  /*"当前费率 V象限无功 电量"*/


    E_INFO_NOW_PA_DEMAND,  //0034  /*"当前 有功 需量"*/
    E_INFO_NOW_R_DEMAND,   //0034  /*"当前 无功 需量"*/

    E_INFO_NOW_TRIFF_PA_DEMAND,  //0034  /*"当前费率 正向有功 需量"*/
    E_INFO_NOW_TRIFF_PA_DEMAND_TIME,//0035  /*"当前费率 正向有功 需量发生时间"*/    
    E_INFO_NOW_TRIFF_NA_DEMAND,  //0036  /*"当前费率 正向有功 需量"*/
    E_INFO_NOW_TRIFF_NA_DEMAND_TIME,//0037  /*"当前费率 正向有功 需量发生时间"*/
    
    E_INFO_NOW_COMM_TRIFF_R_1_DEMAND,//002F  /*"当前组合费率 无功 1  需量"*/
    E_INFO_NOW_COMM_TRIFF_R_1_DEMAND_TIME,//002F  /*"当前组合费率 无功 1  需量 时间"*/
    E_INFO_NOW_COMM_TRIFF_R_2_DEMAND,//002F  /*"当前组合费率 无功 2  需量"*/
    E_INFO_NOW_COMM_TRIFF_R_2_DEMAND_TIME,//002F  /*"当前组合费率 无功 2  需量时间""*/
    E_INFO_NOW_TRIFF_R_I_DEMAND,   //0030  /*"当前费率 I象限无功 需量"*/
    E_INFO_NOW_TRIFF_R_I_DEMAND_TIME,   //0030  /*"当前费率 I象限无功 需量时间"*/
    E_INFO_NOW_TRIFF_R_II_DEMAND,  //0031  /*"当前费率 II象限无功 需量"*/
    E_INFO_NOW_TRIFF_R_II_DEMAND_TIME,  //0031  /*"当前费率 II象限无功 需量时间"*/
    E_INFO_NOW_TRIFF_R_III_DEMAND, //0032  /*"当前费率 III象限无功 需量"*/
    E_INFO_NOW_TRIFF_R_III_DEMAND_TIME, //0032  /*"当前费率 III象限无功 需量时间"*/
    E_INFO_NOW_TRIFF_R_IV_DEMAND,  //0033  /*"当前费率 V象限无功 需量"*/
    E_INFO_NOW_TRIFF_R_IV_DEMAND_TIME,  //0033  /*"当前费率 V象限无功 需量时间"*/

    E_INFO_LAST_COMM_TRIFF_A_ENGY,  //0038  /*"上N月  组合费率 有功     电量"*/
    E_INFO_LAST_COMM_TRIFF_R_1_ENGY,//0039  /*"上N月组合费率 无功 1  电量"*/
    E_INFO_LAST_COMM_TRIFF_R_2_ENGY,//003a  /*"上N月组合费率 无功 2  电量"*/
    
    E_INFO_LAST_COMM_TRIFF_R_I_ENGY,//003b  /*"上N月费率 I象限无功 电量"*/
    E_INFO_LAST_COMM_TRIFF_R_II_ENGY,//003C  /*"上N月费率 II象限无功 电量"*/
    E_INFO_LAST_COMM_TRIFF_R_III_ENGY,//003D  /*"上N月费率 III象限无功 电量"*/
    E_INFO_LAST_COMM_TRIFF_R_IV_ENGY,//003E  /*"上N月费率 IV象限无功 电量"*/

    E_LAST_LOST_A_V_TIME,            //003F  /*"上N  次A 相失压时间"*/
    E_LAST_LOST_B_V_TIME,            //0040  /*"上N  次B 相失压时间"*/
    E_LAST_LOST_C_V_TIME,            //0041  /*"上N  次C 相失压时间"*/
    
    E_SUM_LOST_V_COUNTS,             //0042  /*"总失压次数"*/
    E_SUM_LOST_V_TIME,               //0043  /*"总失压累计时间"*/
    E_LAST_LOST_V_TIME,              //0044  /*"上N  次 相失压时间"*/
#endif    
    E_MAX_SHOWINFO_ID,
}EN_DISP_INFO;

/*"数据单位与格式 对应 stFormat"*/  
typedef enum
{
    /*"数据格式"*/ /*"数据单位"*/ /*"换算:数据小数位数"*/
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
    E_FORMAT_DDHH_NONE_0,     //{E_DDHH,E_NONE_UNIT,0},             //0028 /*"显示格式: 未做BCD转换"*/
    E_FORMAT_U_6_NONE_0,      //{E_U_6_BYTE,E_NONE_UNIT,0},         //0x29
    E_FORMAT_HHMM_NONE_0,     //{E_HHMM,E_NONE_UNIT,0},             //002A
    E_FORMAT_NN_MMDD_NONE_0,  //{E_NN_MMDD,E_NONE_UNIT,0},          //002B
    E_FORMAT_NN_HHMM_NONE_0,  //{E_NN_HHMM_42,E_NONE_UNIT,0},       //002C  /*块数据*/
    E_FORMAT_NN_YYMMDD_NONE_0,//{E_NN_YYMMDD,E_NONE_UNIT,0},        //002D
    E_FORMAT_YYYYMMDDHHMMSS_FF_NONE_0,//{E_YYYYMMDDHHMMSS_FF,E_NONE_UNIT,0}//002E  XYT
    E_FORMAT_MMDDHH_NONE_0,   //{E_MMDDHH,E_NONE_UNIT,0},           //002F /*"显示格式: 未做BCD转换"*/
    E_FORMAT_ASCII_8_NONE_0,  //{E_ASCII_8_LENGTH,E_NONE_UNIT,0},   //0x30
    E_FORMAT_YYMMDDHHMMSS_NONE_0,//{E_YYMMDDHHMMSS,E_NONE_UNIT,0},    //0031
    E_FORMAT_YYYYMMDDHHMMSS_NONE_0,//{E_YYYYMMDDHHMMSS,E_NONE_UNIT,0}//0032  yl     //0032  /*先暂时定个重复的，否则后面的都不对了*/
    E_FORMAT_U_2_V_1,         //{E_U_2_BYTE,E_V_UNIT,-1},           //0033
    E_FORMAT_S_4_A_3,         //{E_S_4_BYTE,E_A_UNIT,-3},           //0034
    E_FORMAT_S_4_KW_4,        //{E_S_4_BYTE,E_KW_UNIT,-4},          //0x35      /*2018-01-29:改成kw*/
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
    E_FORMAT_NONE_40,         //{E_NONE_FORMAT,0,0},                //0x40  /*先暂时定个重复的，否则后面的都不对了*/,         //0x40 /*先暂时定个重复的，否则后面的都不对了*/
    E_FORMAT_U_4_A_3,         //{E_U_4_BYTE,E_A_UNIT,-3},           //0x41
    E_FORMAT_U_4_W_1,         //{E_U_4_BYTE,E_W_UNIT,-1},           //0x42
    E_FORMAT_U_2_NONE_3,      //{E_U_2_BYTE,E_NONE_UNIT,-3},        //0x43
    E_FORMAT_NONE_44,         //{E_NONE_FORMAT,0,0},                //0x44  /*先暂时定个重复的，否则后面的都不对了*/
    E_FORMAT_NN_MMDD_42_NONE_3,//{E_NN_MMDD_42,E_NONE_UNIT,0},      //0x45
    E_FORMAT_U_4_NONE_3,       //{E_U_4_BYTE,E_NONE_UNIT,-2},       //0046
    E_FORMAT_NONE_47,         //{E_NONE_FORMAT,0,0},                //0047 /*先暂时定个重复的，否则后面的都不对了*/
    E_FORMAT_ASCII_8_NONE_0_REPEAT, //{E_ASCII_8_LENGTH,E_NONE_UNIT,0},   //0x48
    E_FORMAT_DDHH_NONE_0_REPEAT,     //{E_DDHH,E_NONE_UNIT,0},             //0049
    E_FORMAT_NN_YYYYMMDDWW_NONE_0,//{E_NN_YYYYMMDDWW,E_NONE_UNIT,0}, //004A   /*先暂时定个重复的，否则后面的都不对了*/
    E_FORMAT_MMDDHH_NONE_0_REPEAT,//{E_MMDDHH,E_NONE_UNIT,0},           //0x4B
    E_FORMAT_U_2_V_2,         //{E_U_2_BYTE,E_V_UNIT,-2},           //004C
    E_FORMAT_NONE_4D,         //{E_NONE_FORMAT,0,0},                //004D   /*先暂时定个重复的，否则后面的都不对了*/
    E_FORMAT_NONE_4E,         //{E_NONE_FORMAT,0,0},                //004E   /*先暂时定个重复的，否则后面的都不对了*/
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
    E_FORMAT_DEMAND_AND_TIME,    //{E_DEMAND_AND_TIME,E_NONE_UNIT,0},  //0x5A /*显示用*/                            //0x5A
    
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

/*"方法"*/
typedef struct 
{
		TOn_Write_Event OnWrite_Beford;
		//TOn_Write_Event OnWrite_End;
		TOn_Read_Event OnRead;
}ST_FUNCTION;
#endif

/*------------------------------------------------------*/
/*简单类型定义，以TYPE开头*/


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

/*  插卡操作命令头	octet-string(5) C21_ATT3_F100*/
extern const INT8U TYPE_OCTS_5[2];  

/*4001 通信地址, 4002 表号  */
extern const INT8U TYPE_OCTS_6[2];

/*这两个，能不能定义成一个变长?*/
/*  卡序列号	octet-string(8)  C21_ATT2_F100*/
extern const INT8U TYPE_OCTS_8[2]; 

//extern const INT8U TYPE_OCTS_14[2];

/*C21_ATT4_F100*/
extern const INT8U TYPE_OCTS_16[2];

#ifdef THREE_PHASE

/*4117 期间需量冻结周期*/
extern const INT8U TYPE_TI[2];
#endif
/*简单类型定义，END*/
/*------------------------------------------------------*/

/*"************************************************"*/
/*数组类型指针定义，以ARRAY开头*/
//const INT8U AR_U8[4] = {array,4,unsigned_t,1};
//const INT8U AR_S8[4] = {array,4,integer,1};

extern const INT8U AR_4_U32[4];
extern const INT8U AR_PHASE_U16[4];

//extern const INT8U AR_S16[4];
extern const INT8U AR_SUM_PHASE_S16[4];

/*4018 费率电价，*/
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

/*4005 组地址*/
extern const INT8U AR_OCTS[4];


/*C6_ATT2_2014 电能表运行状态字*/
extern const INT8U AR_7_BITS_16[4];

#ifdef THREE_PHASE

/*正需量值*/
//extern const INT8U AR_DEMAND[8];
extern const INT8U AR_TRIFF_DEMAND[8];
/*负需量值*/
//extern const INT8U AR_DEMAND_R[8];
extern const INT8U AR_TRIFF_DEMAND_R[8];
#endif

/*数组类型指针定义 END*/
/*------------------------------------------------------*/

/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/

/*结构体类型指针定义，以ST开头 如果是一样的，则后面不再重复*/
/*300E,300F,3010,3011,302A,302B,302C,302E,302F*/
extern const INT8U ST_1_U8[4];
extern const INT8U ST_2_DATE_U8[6];

/*3005,3007,3008*/
extern const INT8U ST_2_S32_U8[6];

/*时钟源 4006  */
extern const INT8U ST_2_EN[8];
/*80000200 */
extern const INT8U ST_2_U32_U16[6];

/*F1010200  */
extern const INT8U ST_2_OI_U16[6];

#ifdef THREE_PHASE
extern const INT8U ST_2_NULL_NULL[6];
#endif

/*2025 401E报警金额限值*/
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

/*其它金额限值  401F  */
extern const INT8U ST_3_U32_U32_U32[8];
/*FF60 链路层长度 */
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
/*版本信息*/
extern const INT8U TYPE_VERSION_INFO[14];
extern const INT8U TYPE_CONNECT_INFO[16];

/*设备地理位置 4004 */
extern const INT8U TYPE_LOCATION[24];
/*时区时段数 400C  */
//extern const INT8U TYPE_YEARLY_DAILY[12];

/*LCD参数 4007  */
extern const INT8U TYPE_LCD_PARAMENT[16];

#ifdef THREE_PHASE

extern const INT8U ST_2_U16_U8[6];
/*3001,3002*/

extern const INT8U ST_2_U32_U8[6];
/*3009,300A,300B,301D,301E,302D*/

extern const INT8U ST_2_S16_U8[6];
/*300C,301D,301E,302D*/

extern const INT8U ST_3_U16_S32_U8[8];


/*三相表，补充为两路485，描述符分别为RS4851和RS4852 C22_ATT2_F201_2*/
//extern const INT8U ST_3_VIS_6_COMDCB_EN[8];


extern const INT8U ST_4_U16_S32_S32_U8[10];
/*3004*/

extern const INT8U ST_4_U16_U16_S32_U8[10];
/* 3000,*/

extern const INT8U ST_4_U32_U32_DATE_S_DATE_S[];

/*需量，定义为STRUCT,但命名还是用TYPE*/
/*正需量值*/
//extern const INT8U TYPE_DEMAND[6];
/*负需量值*/
extern const INT8U TYPE_DEMAND_R[6];
/*FF650200*/
//extern const INT8U ST_4_U32[10];
/*CLASS7 3000 ATT13电能失压事件，属性3*/
//extern const INT8U TYPE_LOST_VOLT_STATIC[10];

#endif

/*结构体类型指针定义 END */

/*-------------------------------------------------------*/

/*复合结构体类型定义 以ARRAY_STRUCT开头*/

/*事件上报状态*/
//extern const INT8U TYPE_REPORT_STATUS[8];
/*4011 公共假日*/
extern const INT8U TYPE_HOLIDAY_TABLE[8];

/*当前值记录表*/
extern const INT8U AR_ST_2_U32[8];

extern const INT8U AR_ST_2_ENUM_ST_2_U32[12];
//extern const INT8U AR_ST_2_NULL_ST_2_U16_NULL[12];
/*24 类时间状态记录表*/
extern const INT8U AR_ST_2_DATES[8];
/*7 类时间状态记录表*/
extern const INT8U AR_ST_2_NULL_ST_2_DATES[12];


/*下面三个为8000的方法129-131类型*/
extern const INT8U AR_ST_2_OAD_EN[8];
/*F1010300*/
extern const INT8U AR_ST_2_OI_U16[8];
/*CLASS17_ATT2*/
extern const INT8U AR_ST_2_CSD_U8[8];

/*4014 时区表*/
extern const INT8U AR_ST_3_YEARLY_TABLE[10];
/*时段表*/
extern const INT8U AR_ST_3_DAILY_SLOT_TABLE[12];
extern const INT8U AR_8_ST_3_DAILY_SLOT_TABLE[12];

/*下面三个为8000的方法129-131类型*/
extern const INT8U AR_ST_3_OAD_EN_AS[10];
/*CLASS9_ATT3_METHOD7*/
extern const INT8U AR_ST_3_U16_OAD_U16[10];
/*下面三个为8000的方法129-131类型*/
extern const INT8U AR_ST_4_OAD_U8_U16_BOOL[12];

/*CLASS18_ATT10_METHOD7  文件比对*/
extern const INT8U ST_5_U8_U8_U32_U32_U16[12];

/*4016 时段表*/
//extern const INT8U TYPE_DAILY_TABLE[12];

/*401A 阶梯电价*/
//extern const INT8U TYPE_STEP_PRICE[20];

/*4116 结算日*/
extern const INT8U TYPE_BILL_DAY[20];
/*CLASS7_ATT10_CLASS24_ATT14 */ /*7类型属性10和24类型属性14最近一次发生时间和最近一次结束时间*/ 
//extern const INT8U TYPE_LAST_START_TIME_END_TIME[8];


/*C7_ATT2_401A 阶梯电价编码，先这样定，后面有机会再完善*/
extern const INT8U TYPE_STEP_PRICE[66];   

#ifdef THREE_PHASE
/*需量数组*/
//extern const INT8U ARRAY_TYPE_DEMAND[8];  
//extern const INT8U ARRAY_TYPE_DEMAND_R[8]; 

/*总电压合格率 C6_ATT2_2130*/
extern const INT8U TYPE_VOLT_OK_RATE[26];
#endif

/*复合结构体类型定义 END*/
/*-----------------------------------------*/

/*元素方案定义 CLS_1_6_OBJECT_ELE */
typedef enum
{
   E_ELE_A_ENGY_COM  = 0, /*组合有功*/
   E_ELE_A_ENGY,          /*1.有功电能*/
   E_ELE_R_ENGY_COM,      /*2.组合无功电能*/
   E_ELE_R_ENGY,          /*3.分象限无功电能*/
   
   /*class3*/
   E_ELE_VOLT,            /*4.电压*/   
   E_ELE_CURR,            /*5.电流*/
   E_ELE_PHASE,           /*6.相角*/
   
   /*class4*/
   E_ELE_A_POWER,         /*7.有功功率*/
   E_ELE_R_POWER,         /*8.无功功率*/
   E_ELE_V_POWER,         /*9.视在功率*/
   E_ELE_FACTOR,          /*10.功率因数*/
   
   /*class6*/
   E_ELE_FEQ,             /*11.电网频率*/
   E_ELE_CLK_VOLT,        /*12.时钟电池电压*/
   E_ELE_STATUS_8,        /*13.电能表运行状态字*/
   E_ELE_PRICE,           /*14.电价 */
   E_ELE_SAFE_TIME,       /*15.安全认证剩余时长*/
   E_ELE_MONEY,           /*16.金额*/ 
   E_ELE_STATUS_16,       /*17.状态字*/
   E_ELE_STATUS_32,       /*18.控制命令执行状态字*/
   E_ELE_CLK_WORK_TIME,   /*19.时钟电池工作时间*/

   /*高精度电能量-----------*/
   E_ELE_A_ENGY_COM_H,    /*20.组合有功*/
   E_ELE_A_ENGY_H,        /*21.有功电能*/

#ifdef THREE_PHASE   
   E_ELE_R_ENGY_COM_H,    /*22.组合无功电能*/
   E_ELE_R_ENGY_H,        /*23.分象限无功电能*/
   
   /*最大需量---------------*/
   E_ELE_A_MAX_DEMAND,    /*24.有功最大需量*/
   E_ELE_A_NULL,          /*25.无*/
   E_ELE_R_MAX_DEMAND,    /*26.象限无功最大需量*/
   E_ELE_R_MAX_DEMAND_COM,/*27.组合无功最大需量*/
   
   /*变量类*/
   E_ELE_TEMP,            /*28.表内温度*/
   E_ELE_LOW_POWER_VOLT,  /*29.停电抄表电池电压*/
   E_ELE_CURR_A_DEMAND,   /*30.当前有功需量 */
   E_ELE_CURR_R_DEMAND,   /*31.当前无功需量 */
   E_ELE_IMBANLCE_RATE,   /*32.不平衡率 */
   E_ELE_VAH,             /*33.安时值 */    
   E_ELE_VOLT_RATE,       /*34.总电压合格率*/   

   E_ELE_PHASE_A_ENGY,     /*35.有功电能*/
   E_ELE_PHASE_R_ENGY_COM,/*36.组合无功电能*/
   E_ELE_PHASE_R_ENGY,    /*37.分象限无功电能*/

   E_ELE_PHASE_A_ENGY_H,     /*38.有功电能*/
   E_ELE_PHASE_R_ENGY_COM_H, /*39.组合无功电能*/
   E_ELE_PHASE_R_ENGY_H,     /*40.分象限无功电能*/
#endif   
   E_ELE_MAX,                
}EN_OBJECT_ELE;


/*一些类的特殊AID 的信息:读写权限，IID，类型指针*/
typedef struct
{
   INT8U AID;             /*"属性"*/    
   INT8U Right_RW;        /*"读写权限 0:只读，0x0f:可读写*/     
   INT8U Max_IID;         /*"最大IID"*/  
   /*INT8U res*/
   const INT8U *Pointer;  /*数据类型指针*/              
}ST_SPECIAL_CLASS_AID_INFO;

/*"显示"*/
 typedef struct
{
    INT8U schar[MAX_SCHAR_NUM];
}ST_SHOW_INFO;

typedef struct
{	
	INT8U TransId;  /*"通信传输格式"*/
	INT8U ShowId;  /*"显示传输格式"*/	
}ST_FORMAT_ID;

typedef struct
{
    ST_OAD Oad;        /*OAD*/
	INT8U  Class;      /*类*/
    INT8U  Right_RW;   /*读写权限*/
    INT8U  element;    /*对应ST_OBJECT_ELC的元素位置，如果是参变量，则是元素号码*/
    INT8U  seq_num;    /*OAD对应的位置*/
    
    const INT8U * Pointer; /*数据类型(编码指针) */
}SUB_OBJECT;


/*参数类OBIS信息*/
/*8 byte*/
typedef struct
{ 
    INT8U  Seq_Num;   /*列表序列号 与ST_Class8_ObjectList中的一样*/
    INT8U  num;       /*"OAD对应OBIS中元素个数"*/
    INT16U Obis;      /*"与num有关，num小于0x80,则是连续的OBIS基值，否则是OBIS块"*/

    INT8U  Offset;    /*"这个变量已没用，但由于四字节对齐，所以这里暂时留着定义。数据中心数据的偏移 都固定为0？先保留，后面看情况，再决定是否去掉 "*/
    INT8U  FormatId;  /*"通信、显示格式ID"*/
    INT8U  FunctionId;/*"读写方法ID"*/
    INT8U  ShowinfoId;/*"显示配置表ID"*/
}ST_PARAMENT_OAD_OBIS_INFO;
/*参数类OBIS信息 END*/

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


