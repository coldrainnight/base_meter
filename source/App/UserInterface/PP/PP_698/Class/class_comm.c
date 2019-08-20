/*"*****************************************************************************"*/
/*"  FileName: class_comm.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  类通用文件   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // 历史修改记录"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h"
#include "..\\Process\Pp_macro.h"
#include "..\\Process\Pp_obis.h"
#include "App\message\FRMT_def.h"
#include "App\UserInterface\UI_dataProcess.h"


/*类型指针在实际应用中，先用类型指针指向，如果后面优化程序，可以用一个偏移位置表定义*/

/*类型定义简写说明:AR =ARRAY,             ST =STRUCT,
                   U8 =unsigned_t,        S8 =integer,
                   U16=longunsigned,      S16=long_t,
                   U32=doublelongunsigned,S32=doublelong
                   U64=long64unsigned,    S64=long64  
                   VIS =visiblestring     OCTS=octstring
                   BITS=bitstring         EN=enum
                   DATES=date_time_s

ARRAY后跟随数组个数，小于0x80,表示数组个数固定，大于0x80,表示变长，需要根据单独编码函数计算个数，变长定义名称统一为AR_0_开头，                
*/
/*------------------------------------------------------*/
/*简单类型定义，以TYPE开头*/

/* */
const INT8U TYPE_OAD[2]    = {OAD_t,4}; 

const INT8U TYPE_BOOL[2]   = {bool_t,1};

const INT8U TYPE_DATES[2] ={date_time_s,7};

//const INT8U TYPE_COMDCB[2] = {COMDCB,5};

const INT8U TYPE_NULL[2]   = {null_data,0};
const INT8U TYPE_OI[2]  = {OI_t,2};
const INT8U TYPE_U16[2] = {longunsigned,2};
const INT8U TYPE_S16[2] = {long_t,2};
const INT8U TYPE_ENUM[2]= {enum_t,1};

const INT8U TYPE_S8[2]  = {integer,1};
const INT8U TYPE_U8[2]  = {unsigned_t,1};
const INT8U TYPE_U32[2] = {doublelongunsigned,4};
const INT8U TYPE_S32[2] = {doublelong,4};

const INT8U TYPE_BITS_8[2]   = {bitstring,8};
const INT8U TYPE_BITS_16[2]  = {bitstring,16};
const INT8U TYPE_BITS_32[2]  = {bitstring,32};
//const INT8U TYPE_BITS_128[2] = {bitstring,128};

const INT8U TYPE_VIS_4[2]  = {visiblestring,4};
const INT8U TYPE_VIS_6[2]  = {visiblestring,6};
const INT8U TYPE_VIS_8[2]  = {visiblestring,8};
const INT8U TYPE_VIS_16[2] = {visiblestring,16};
const INT8U TYPE_VIS_32[2] = {visiblestring,32};

//const INT8U TYPE_OCTS_2[2] = {octetstring,2};

/*  插卡操作命令头	octet-string(5) ,C21_ATT3_F100 */
const INT8U TYPE_OCTS_5[2] = {octetstring,5};  

/*4001 通信地址, 4002 表号  */
const INT8U TYPE_OCTS_6[2] = {octetstring,6}; 

/*这两个，能不能定义成一个变长?*/
/*  卡序列号	octet-string(8) ,C21_ATT2_F100*/
const INT8U TYPE_OCTS_8[2] = {octetstring,8};  

//const INT8U TYPE_OCTS_14[2] = {octetstring,14};

/*C21_ATT4_F100*/
const INT8U TYPE_OCTS_16[2] = {octetstring,16}; 

#ifdef THREE_PHASE

/*4117 期间需量冻结周期*/
const INT8U TYPE_TI[2] = {TI_t,3};

#endif
/*简单类型定义，END*/
/*------------------------------------------------------*/

/*"************************************************"*/
/*数组类型指针定义，以ARRAY开头*/
//const INT8U AR_U8[4] = {array,4,unsigned_t,1};
//const INT8U AR_S8[4] = {array,4,integer,1};

const INT8U AR_4_U32[4] = {array,4,doublelongunsigned,4};

const INT8U AR_PHASE_U16[4] = {array,E_PHASE_VARIBLE_ID,longunsigned,2};


//const INT8U AR_S16[4] = {array,4,long_t,2};
/*功率因素*/
const INT8U AR_SUM_PHASE_S16[4] = {array,E_PHASE_VARIBLE_SUM_ID,long_t,2};

/*4018 费率电价，*/
//const INT8U AR_U32[4] = {array,4,doublelongunsigned,4};

const INT8U AR_SUM_PHASE_U32[4] = {array,E_PHASE_VARIBLE_SUM_ID,doublelongunsigned,4};
const INT8U AR_TRIFF_SUM_U32[4] = {array,E_TARIFF_ID,doublelongunsigned,4};
const INT8U AR_TRIFF_U32[4] = {array,E_TRIFF_ID_PHASE,doublelongunsigned,4};
#ifdef THREE_PHASE
const INT8U AR_6_U32[4]     = {array,6,doublelongunsigned,4};
const INT8U AR_1_U32[4]     = {array,1,doublelongunsigned,4};
const INT8U AR_1_S32[4]     = {array,1,doublelong,4};

const INT8U AR_1_U64[4]     = {array,1,long64unsigned,8};
const INT8U AR_1_S64[4]     = {array,1,long64,8};

#endif
const INT8U AR_PHASE_S32[4] = {array,E_PHASE_VARIBLE_ID,doublelong,4};
//const INT8U AR_S32[4] = {array,4,doublelong,4};

/*总及分相*/
const INT8U AR_SUM_PHASE_S32[4] = {array,E_PHASE_VARIBLE_SUM_ID,doublelong,4};


/*总及费率数*/
const INT8U AR_TRIFF_S32[4] = {array,E_TARIFF_ID,doublelong,4};

//const INT8U AR_U64[4] = {array,4,long64unsigned,4};
//const INT8U AR_S64[4] = {array,4,long64,4};

const INT8U AR_TRIFF_U64[4] = {array,E_TARIFF_ID,long64unsigned,8};
const INT8U AR_TRIFF_S64[4] = {array,E_TARIFF_ID,long64,8};

const INT8U AR_OAD[4] = {array,E_RELATION_OBJECT_ID,OAD_t,4};
const INT8U AR_OMD[4] = {array,E_RELATION_OBJECT_ID,OMD_t,4};

const INT8U AR_OI[4]  = {array,0,OI_t,2}; 

const INT8U AR_N_OI[4]= {array,E_RELATION_OBJECT_ID,OI_t,2};

#ifdef THREE_PHASE
const INT8U AR_U64[4] = {array,15,long64unsigned,8};
#endif

/*CLASS19ATT6 支持规约列表*/
const INT8U AR_VIS[4] = {array,1,visiblestring,32};

/*4005 组地址*/
const INT8U AR_OCTS[4]={array,0x01,octetstring,6};


/*C6_ATT2_2014 电能表运行状态字*/
const INT8U AR_7_BITS_16[4] = {array,7,bitstring,16}; 


#ifdef THREE_PHASE

/*正需量值*/
//const INT8U AR_DEMAND[8]    = {array,0,structure,2,doublelongunsigned,4,date_time_s,7};
const INT8U AR_TRIFF_DEMAND[8]= {array,E_TARIFF_ID,structure,2,doublelongunsigned,4,date_time_s,7};

/*负需量值*/
const INT8U AR_TRIFF_DEMAND_R[8]    = {array,E_TARIFF_ID,structure,2,doublelong,4,date_time_s,7};

#endif


/*数组类型指针定义 END*/
/*------------------------------------------------------*/

/*结构体类型指针定义，以ST开头 如果是一样的，则后面不再重复*/
/*300E,300F,3010,3011,302A,302B,302C,302E,302F*/
const INT8U ST_1_U8[4]  = {structure,1,unsigned_t,1};

const INT8U ST_2_DATE_U8[6]  = {structure,0x02,date,5,unsigned_t,1};   

/*3005,3007,3008*/
const INT8U ST_2_S32_U8[6]   = {structure,2,doublelong,4,unsigned_t,1};

/*时钟源 4006  */
const INT8U ST_2_EN[8] ={structure,0x02,enum_t,1,enum_t,1};

/*80000200 */
const INT8U ST_2_U32_U16[6] = {structure,2,doublelongunsigned,4,longunsigned,2};


/*F1010200  */
const INT8U ST_2_OI_U16[6] = {structure,2,OI_t,2,longunsigned,2};

#ifdef THREE_PHASE
const INT8U ST_2_NULL_NULL[6]= {structure,2,null_data,0,null_data,0};
#endif

/*2025 401E报警金额限值*/
const INT8U ST_2_U32[6] = {structure,2,doublelongunsigned,4,doublelongunsigned,4};

/*C22_ATT2_F202*/
const INT8U AR_ST_2_VIS_5_COMDCB[8] = {array,1,structure,2,visiblestring,5,COMDCB,5};

/*C22_ATT4_F206*/
//const INT8U ST_2_DATEB[] = {structure,2,time,7,time,7};

/*C22_ATT127_F202,C22_ATT128_F209*/
const INT8U ST_2_OAD_COMDCB[6] = {structure,2,OAD_t,4,COMDCB,5};

/*C22_ATT127_F205*/
const INT8U ST_2_OAD_EN[6] = {structure,2,OAD_t,4,enum_t,1};

/*CLASS17_ATT4*/
const INT8U ST_2_U8[6]={structure,2,unsigned_t,1,unsigned_t,1};

/*C22_ATT127_F200*/
const INT8U ST_3_OAD_COMDCB_EN[8] = {structure,3,OAD_t,4,COMDCB,5,enum_t,1};


/*其它金额限值  401F  */
const INT8U ST_3_U32_U32_U32[8]={structure,0x03,doublelongunsigned,4,doublelongunsigned,4,doublelongunsigned,4};

/*FF60 链路层长度 */
const INT8U ST_3_U16[8] = {structure,3,longunsigned,2,longunsigned,2,longunsigned,2};

/*C21_ATT7_F100*/
const INT8U ST_3_U32[8]={structure,3,doublelongunsigned,4,doublelongunsigned,4,doublelongunsigned,4}; 


/*CLASS9_METHOD4  冻结对象*/
const INT8U ST_3_U16_OAD_U16[8]={structure,3,longunsigned,2,OAD_t,4,longunsigned,2};

/*CLASS17_METHOD_5 显示查看 */
const INT8U ST_3_CSD_U8_U16[8]={structure,3,CSD,5,unsigned_t,1,longunsigned,2};

/*C22_ATT2_F201*/
const INT8U AR_ST_3_VIS_5_COMDCB_EN[10] = {array,1,structure,3,visiblestring,5,COMDCB,5,enum_t,1};

const INT8U AR_ST_3_VIS_6_COMDCB_EN[10] = {array,2,structure,3,visiblestring,6,COMDCB,5,enum_t,1};

/*C22_ATT2_F209*/
const INT8U AR_ST_3_NULL_COMDCB_NULL[10] = {array,1,structure,3,null_data,0,COMDCB,5,null_data,0};

/*40300200,*/
const INT8U ST_4_U16[10] = {structure,4,longunsigned,2,longunsigned,2,longunsigned,2,longunsigned,2};

const INT8U AR_TYPE_ENUM[4]= {array,1,enum_t,1};

/*C22_ATT2_F205*/
const INT8U AR_ST_4_VIS_1_EN_EN_EN[12] = {array,NUM_RELAY,structure,4,visiblestring,1,enum_t,1,enum_t,1,enum_t,1};


/*4000400 */
const INT8U ST_5_U8[12]={structure,5,unsigned_t,1,unsigned_t,1,unsigned_t,1,unsigned_t,1,unsigned_t,1};


/*版本信息*/
const INT8U TYPE_VERSION_INFO[14] = {structure,6,visiblestring,4,visiblestring,4,visiblestring,6,visiblestring,4,
                                     visiblestring,6,visiblestring,8};

/*应用语境信息*/
const INT8U TYPE_CONNECT_INFO[16] = 
{
    structure,7,longunsigned,2,longunsigned,2,longunsigned,2,longunsigned,2,
                bitstring,8,bitstring,16,doublelongunsigned,4       
        
};

/*设备地理位置 4004 */
const INT8U TYPE_LOCATION[24]=
{structure,0x03,
 structure,0x04,enum_t,1,unsigned_t,1,unsigned_t,1,unsigned_t,1,
 structure,0x04,enum_t,1,unsigned_t,1,unsigned_t,1,unsigned_t,1,
 doublelongunsigned,4};

/*时区时段数 400C  */
//const INT8U TYPE_YEARLY_DAILY[12]={structure,0x05,unsigned_t,1,unsigned_t,1,unsigned_t,1,unsigned_t,1,unsigned_t,1};

/*LCD参数 4007  */
const INT8U TYPE_LCD_PARAMENT[16]={structure,0x07,unsigned_t,1,longunsigned,2,longunsigned,2,longunsigned,2,unsigned_t,1,unsigned_t,1,unsigned_t,1};

#ifdef THREE_PHASE

const INT8U ST_2_U16_U8[6] = {structure,2,longunsigned,2,unsigned_t,1};
/*3001,3002*/

const INT8U ST_2_U32_U8[6] = {structure,2,doublelongunsigned,4,unsigned_t,1};
/*3009,300A,300B,301D,301E,302D*/

const INT8U ST_2_S16_U8[6] = {structure,2,long_t,2,unsigned_t,1};
/*300C,301D,301E,302D*/

const INT8U ST_3_U16_S32_U8[8] = {structure,3,longunsigned,2,doublelong,4,unsigned_t,1};
/*3003,3006*/

/*三相表，补充为两路485，描述符分别为RS4851和RS4852 C22_ATT2_F201_2*/
//const INT8U ST_3_VIS_6_COMDCB_EN[8] = {structure,3,visiblestring,6,COMDCB,5,enum_t,1};


const INT8U ST_4_U16_S32_S32_U8[10] = {structure,4,longunsigned,2,doublelong,4,doublelong,4,unsigned_t,1};
/*3004*/

const INT8U ST_4_U16_U16_S32_U8[10]= {structure,4,longunsigned,2,longunsigned,2,doublelong,4,unsigned_t,1};
/* 3000,*/

/*电能失压事件，属性13*/ 
const INT8U ST_4_U32_U32_DATE_S_DATE_S[] = {structure,4,doublelongunsigned,4,doublelongunsigned,4,date_time_s,7,date_time_s,7}; 

/*需量，定义为STRUCT,但命名还是用TYPE*/
/*正需量值*/
//const INT8U TYPE_DEMAND[6]    = {structure,2,doublelongunsigned,4,date_time_s,7};
/*负需量值*/
const INT8U TYPE_DEMAND_R[6]  = {structure,2,doublelong,4,date_time_s,7};

/*FF650200*/
//const INT8U ST_4_U32[10] = {structure,4,doublelongunsigned,4,doublelongunsigned,4,doublelongunsigned,4,doublelongunsigned,4};

/*CLASS7 3000 ATT13电能失压事件，属性3*/
//const INT8U TYPE_LOST_VOLT_STATIC[10] = {structure,4,doublelongunsigned,4,doublelongunsigned,4,date_time_s,7,date_time_s,7}; 


#endif

/*结构体类型指针定义 END */

/*-------------------------------------------------------*/


/*复合结构体类型定义 以ARRAY_STRUCT开头*/

/*事件上报状态 ATT2_3300*/
//const INT8U TYPE_REPORT_STATUS[8] = {array,1,structure,02,OAD_t,4,unsigned_t,1};       

/*4011 公共假日*/
const INT8U TYPE_HOLIDAY_TABLE[8] = {array,E_SPECIAL_DAY_ID,structure,0x02,date,5,unsigned_t,1};

/*24类当前值记录表*/
const INT8U AR_ST_2_U32[8]    = {array,0x04,structure,2,doublelongunsigned,4,doublelongunsigned,4};

/*7类当前值记录表  以无功组合方式编程为类型记录，以便计算长度时，不会溢出*/
#ifdef SINGLE_PHASE
const INT8U AR_ST_2_ENUM_ST_2_U32[12] = {array,01,structure,2,null_data,0,structure,2,doublelongunsigned,4,doublelongunsigned,4};
#else
const INT8U AR_ST_2_ENUM_ST_2_U32[12] = {array,02,structure,2,enum_t,1,structure,2,doublelongunsigned,4,doublelongunsigned,4};
#endif

//const INT8U AR_ST_2_NULL_ST_2_U16_NULL[12] = {array,01,structure,2,null_data,0,structure,2,longunsigned,2,longunsigned,2};


/*24类时间状态记录表*/
const INT8U AR_ST_2_DATES[8]  = {array,0x04,structure,2,date_time_s,2,date_time_s,2};

/*7类时间状态记录表*/
#ifdef SINGLE_PHASE
const INT8U AR_ST_2_NULL_ST_2_DATES[12] = {array,0x01,structure,2,null_data,0,structure,2,date_time_s,2,date_time_s,2};
#else
const INT8U AR_ST_2_NULL_ST_2_DATES[12] = {array,0x02,structure,2,enum_t,1,structure,2,date_time_s,2,date_time_s,2};
#endif 

/*下面8000的方法130类型*/
const INT8U AR_ST_2_OAD_EN[8]    = {array,0xFE,structure,2,OAD_t,4,enum_t,1};

/*F1010300 显式安全模式参数*/
const INT8U AR_ST_2_OI_U16[8] = {array,0xFE,structure,2,OI_t,2,longunsigned,2};

/*CLASS17_ATT2*/
const INT8U AR_ST_2_CSD_U8[8] = {array,0xfe,structure,2,CSD,5,unsigned_t,1};

/*4014 时区表*/
const INT8U AR_ST_3_YEARLY_TABLE[10] = {array,E_SEASON_SLOT_ID,structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};

/*时段表*/
const INT8U AR_ST_3_DAILY_SLOT_TABLE[12] = {array,E_DAY_TBL_ID,array,E_DAY_SLOT_ID,structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};

const INT8U AR_8_ST_3_DAILY_SLOT_TABLE[12] = {array,14,structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};

//const INT8U AR_1_ST_3_DAILY_SLOT_TABLE[12] = {array,1,array,8,structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};

/*下面为8000的方法131类型 电表明文合闸（参数）*/
const INT8U AR_ST_3_OAD_EN_AS[10]  = {array,0xFE,structure,3,OAD_t,4,enum_t,1,visiblestring,8};

/*CLASS9_ATT3_METHOD7  批量添加冻结对象属性*/
const INT8U AR_ST_3_U16_OAD_U16[10]={array,E_RELATION_FREEZE_OBJECT_ID,structure,3,longunsigned,2,OAD_t,4,longunsigned,2};

/*下面为8000的方法129类型 跳闸（参数）*/
const INT8U AR_ST_4_OAD_U8_U16_BOOL[12] = {array,0xFE,structure,4,OAD_t,4,unsigned_t,1,longunsigned,2,bool_t,1};

/*CLASS18_ATT10_METHOD7  文件比对*/
const INT8U ST_5_U8_U8_U32_U32_U16[12]={structure,5,unsigned_t,1,unsigned_t,1,doublelongunsigned,4,doublelongunsigned,4,longunsigned,2};


/*4016 时段表*/
/*
const INT8U TYPE_DAILY_TABLE[12] =
{array,0xf2,
 array,0xe3,
 structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};*/

/*401A 阶梯电价*/
#if 0
const INT8U TYPE_STEP_PRICE[20] = 
{structure,0x03,
 array,0xd4,doublelongunsigned,4,
 array,0xd5,doublelongunsigned,4,
 array,0xd6,
 structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};  
#endif

/*4116 结算日*/
const INT8U TYPE_BILL_DAY[20] = {array,3, structure,2,unsigned_t,1,unsigned_t,1};

/*CLASS7_ATT10_CLASS24_ATT14 */ /*7类型属性10和24类型属性14最近一次发生时间和最近一次结束时间*/ 
//const INT8U TYPE_LAST_START_TIME_END_TIME[8] = {array,4,structure,2,date_time_s,7,date_time_s,7}; 


/*C7_ATT2_401A 阶梯电价编码，先这样定，后面有机会再完善*/
const INT8U TYPE_STEP_PRICE[66] = 
{structure,0x03,
 array,0x06,doublelongunsigned,4,
 array,0x07,doublelongunsigned,4,
 array,0x04,structure,0x03,unsigned_t,1,unsigned_t,1,unsigned_t,1};  

#ifdef THREE_PHASE
/*需量数组*/
//const INT8U ARRAY_TYPE_DEMAND[8]   = {array,11,structure,2,doublelongunsigned,4,date_time_s,7};
//const INT8U ARRAY_TYPE_DEMAND_R[8] = {array,11,structure,2,doublelong,4,date_time_s,7};

/*总电压合格率 C6_ATT2_2130*/
const INT8U TYPE_VOLT_OK_RATE[26]=
{structure,0x02,
 structure,5,doublelongunsigned,4,longunsigned,2,longunsigned,2,doublelongunsigned,4,doublelongunsigned,4,
 structure,5,doublelongunsigned,4,longunsigned,2,longunsigned,2,doublelongunsigned,4,doublelongunsigned,4};

#endif

/*复合结构体类型定义 END*/
/*-----------------------------------------*/
#if 0
typedef struct
{
   INT8U Scaler;           /*量纲，换算关系，即10的几次方(负和正)*/
   INT8U UNIT;             /*单位*/ 
   const INT8U *Pointer;   /*类型指针*/
}st_object_ele;            /*属性元素*/
#endif

/*1-6类的无素方案*/
/*这个顺序不能随意变化，否则要同步修改枚举EN_OBJECT_ELE相关内容*/
const st_object_ele CLS_1_6_OBJECT_ELE[]=
{
    /*电能量 class1*/
    {0xfe,KWH,       AR_TRIFF_S32},      /*0.组合有功*/
    {0xfe,KWH,       AR_TRIFF_SUM_U32},  /*1.有功电能*/
    {0xfe,kvarh,      AR_TRIFF_S32},      /*2.组合无功电能*/
    {0xfe,kvarh,      AR_TRIFF_SUM_U32},  /*3.分象限无功电能*/

    /*class3*/
    {0xff,VOLTAGE,   AR_PHASE_U16},      /*4.电压*/
    {0xfd,AMP,       AR_PHASE_S32},      /*5.电流*/
    {0xff,PHASEANGLE,AR_PHASE_U16},      /*6.相角*/	

    /*class4*/
    {0xff,UNIT_W,    AR_SUM_PHASE_S32},  /*7.有功功率*/
    {0xff,var,       AR_SUM_PHASE_S32},  /*8.无功功率*/
    {0xff,VA,        AR_SUM_PHASE_S32},  /*9.视在功率*/
    {0xfd,NOUNIT,    AR_SUM_PHASE_S16},  /*10.功率因数*/

    /*class6*/
    {0xfe,HZ,        TYPE_U16},        /*11.电网频率*/
    {0xfe,VOLTAGE,   TYPE_U16},        /*12.时钟电池电压*/
    {0x00,NOUNIT,    AR_7_BITS_16},    /*13.电能表运行状态字*/
    {0xfc,UPRICE,    TYPE_U32},        /*14.电价 */
    {0x00,MIN,       TYPE_U16},	       /*15.安全认证剩余时长*/
    {0xfe,MONEY,     TYPE_U32},        /*16.金额*/ 
    {0x00,NOUNIT,    TYPE_BITS_16},    /*17.状态字*/
    {0x00,NOUNIT,    TYPE_BITS_32},    /*18.控制命令执行状态字*/
    {0x00,MIN,       TYPE_U32},	       /*19.时钟电池工作时间*/   

    /*高精度电能量-----------*/
    {0xfc,KWH,       AR_TRIFF_S64},          /*20.组合有功*/
    {0xfc,KWH,       AR_TRIFF_U64},          /*21.有功电能*/

 #ifdef THREE_PHASE
    {0xfc,kvar,      AR_TRIFF_S64},          /*22.组合无功电能*/
    {0xfc,kvar,      AR_TRIFF_U64},          /*23.分象限无功电能*/
    
    /*最大需量---------------*/
    {0xfc,KW,        AR_TRIFF_DEMAND},       /*24.有功最大需量*/
    {0xfc,KW,        TYPE_DEMAND_R},         /*25.无*/
    {0xfc,kvar,      AR_TRIFF_DEMAND},       /*26.象限无功最大需量*/
    {0xfc,kvar,      AR_TRIFF_DEMAND_R},     /*27.组合无功最大需量*/

    /*变量类*/
    {0xff,TOC,       TYPE_S16},	       /*28.表内温度*/	
    {0xfe,VOLTAGE,   TYPE_U16},        /*29.停电抄表电池电压*/
    {0xfc,KW,        TYPE_S32},	       /*30.当前有功需量 */
    {0xfc,kvar,      TYPE_S32},	       /*31.当前无功需量 */	
    {0xfe,UNIT_H,    TYPE_U16},	       /*32.不平衡率 */
    {0xfe,AH,        AR_SUM_PHASE_U32},	       /*33.安时值 */    
    {0x00,NOUNIT,    TYPE_VOLT_OK_RATE},/*34.总电压合格率*/   

    {0xfe,KWH,        AR_1_U32},       /*35.分相有功电能*/
    {0xfe,kvarh,      AR_1_S32},       /*36.分相组合无功电能*/
    {0xfe,kvarh,      AR_1_U32},       /*37.分相分象限无功电能*/

    {0xfc,KWH,        AR_1_U64},       /*35.分相有功电能*/
    {0xfc,kvarh,      AR_1_S64},       /*36.分相组合无功电能*/
    {0xfc,kvarh,      AR_1_U64},       /*37.分相分象限无功电能*/

 #endif   
    
};

/*----------------------------------------------------------------*/
/*方法类指针*/
const ST_OI_METHOD AID_1_METHOD[] =
{
    {0xF101, E_OBIS_NULL,        TYPE_S8},          /*复位安全模式参数     */
    {0x4300, E_OBIS_NULL,        TYPE_NULL},        /*复位电表     */
    {0x3000, E_RCRD_CLR,        TYPE_S8},          /*事件单独清0     */
};
#define AID_1_NUM  ARRAY_SIZE(AID_1_METHOD)

const ST_OI_METHOD AID_3_METHOD[] =
{
    {0x4300, E_OBIS_NULL,        TYPE_NULL},          /*数据初始化     */
    {0x5000, E_RCRD_TRIGGER_RCRD,TYPE_U16},          /*触发一次冻结  */
    {0xF301, E_SHOW_SCREEN_NEXT, TYPE_NULL},         /*下翻     */
};
#define AID_3_NUM  ARRAY_SIZE(AID_3_METHOD)

const ST_OI_METHOD AID_4_METHOD[] =
{
    {0x4300, E_OBIS_NULL,        TYPE_NULL},         /*恢复出厂参数   暂不支持 */
    {0x5000, E_RCRD_ADD_ONE_ROAD,ST_3_U16_OAD_U16},  /*添加一个冻结对象属性   */
    {0xF301, E_SHOW_SCREEN_LAST, TYPE_NULL},         /*上翻     */
    {0x3000, E_RCRD_ADD_ONE_ROAD,TYPE_OAD},          /*添加一个事件关联对象属性     */
};
#define AID_4_NUM  ARRAY_SIZE(AID_4_METHOD)

const ST_OI_METHOD AID_5_METHOD[] =
{
    {0x4300, E_RCRD_CLR,        TYPE_NULL},          /*事件初始化，即事件清0    */
    {0x5000, E_RCRD_DEL_ONE_ROAD,TYPE_OAD},           /*删除一个冻结对象属性    */
    {0xF301, E_SHOW_SCREEN_DLT698,  ST_3_CSD_U8_U16},    /*显示查看     */
    {0xF300, E_SHOW_SCREEN_DLT698,  ST_3_CSD_U8_U16},    /*显示查看     */
    {0x3000, E_RCRD_DEL_ONE_ROAD,    TYPE_OAD},           /*删除一个事件关联对象属性     */
};  
#define AID_5_NUM  ARRAY_SIZE(AID_5_METHOD)

const ST_OI_METHOD AID_6_METHOD[] =
{
    {0x4300, E_MAX_DEMAND_CLR,  TYPE_NULL},          /*需量初始化，即需量清0     */
    {0xF301, E_SHOW_SCREEN_ALL,        TYPE_U16},           /*全显     */
    {0xF300, E_SHOW_SCREEN_ALL,        TYPE_U16},           /*全显     */
}; 
#define AID_6_NUM  ARRAY_SIZE(AID_6_METHOD)

const ST_OI_METHOD AID_7_METHOD[] =
{
    {0x5000, E_RCRD_ADD_MULT_ROAD,AR_ST_3_U16_OAD_U16},/*批量添加冻结对象属性     */
}; 
#define AID_7_NUM  ARRAY_SIZE(AID_7_METHOD)


const ST_OI_METHOD AID_8_METHOD[] =
{
    {0x5000, E_RCRD_DEL_ALL_ROAD,TYPE_NULL},           /*清除关联对象属性表     */
}; 
#define AID_8_NUM  ARRAY_SIZE(AID_8_METHOD)



const ST_OI_METHOD AID_10_METHOD[] =
{
    {0xF001, E_OBIS_NULL,        ST_5_U8_U8_U32_U32_U16},           /*文件比对     */
}; 
#define AID_10_NUM  ARRAY_SIZE(AID_10_METHOD)




const ST_OI_METHOD AID_127_METHOD[] =
{
    {0x2015, E_OBIS_NULL,        TYPE_BITS_32},      /*确认电能表跟随上报状态字     */
    {0x4000, E_OBIS_NULL,        TYPE_DATES},        /*日期时间方法号127 广播校时 */
    {0xF101, E_OBIS_NULL/*已特殊处理*/, ST_2_OI_U16},       /*增加显式安全模式参数    */
    {0xF201, E_OBIS_NULL,        ST_3_OAD_COMDCB_EN},/*RS485 配置端口   */
    {0xF202, E_OBIS_NULL,        ST_2_OAD_COMDCB},   /*红外  配置端口  */
    {0xF205, E_OBIS_NULL,        ST_2_OAD_EN},       /*继电器输出  修改开关属性  */
    {0xF207, E_OBIS_NULL/*已特殊处理*/, ST_2_OAD_EN},  /*多功能端子  修改工作模式   */
};
#define AID_127_NUM  ARRAY_SIZE(AID_127_METHOD)


const ST_OI_METHOD AID_128_METHOD[] =
{
    {0xF101, E_OBIS_NULL,        TYPE_OI},            /*删除显式安全模式参数     */
    {0xF209, E_OBIS_NULL,        ST_2_OAD_COMDCB},    /*载波 配置端口参数   */
};
#define AID_128_NUM  ARRAY_SIZE(AID_128_METHOD)


const ST_OI_METHOD AID_129_METHOD[] =
{
    {0xF101, E_OBIS_NULL,        AR_ST_2_OI_U16},            /*批量增加显式安全模式参数    */
};
#define AID_129_NUM  ARRAY_SIZE(AID_129_METHOD)




/*方法类OBIS列表*/
const ST_OI_METHOD_NUM_POINTER OI_METHOD_NUM_POINTER_ALL[16]=
{
   {1,   AID_1_NUM,   AID_1_METHOD},
   {3,   AID_3_NUM,   AID_3_METHOD},
   {4,   AID_4_NUM,   AID_4_METHOD},
   {5,   AID_5_NUM,   AID_5_METHOD},
   {6,   AID_6_NUM,   AID_6_METHOD},
   {7,   AID_7_NUM,   AID_7_METHOD},
   {8,   AID_8_NUM,   AID_8_METHOD},
   {127, AID_127_NUM, AID_127_METHOD},
   {128, AID_128_NUM, AID_128_METHOD},
   {129, AID_129_NUM, AID_129_METHOD},
};


/*"format:数据格式,unit:数据单位,exp:数据小数位数"*/  
const ST_FMT_ATTR stFormat[] = 
{
    /*"数据格式"*/ /*"数据单位"*/ /*"换算:数据小数位数"*/
    {E_BCD_4_BYTE,E_KWH_UNIT,-2},       //0000
    {E_BCD_4_BYTE,E_KWH_UNIT,E_MATUO},  //0001
    {E_UBCD_4_BYTE,E_KWH_UNIT,-2},      //0002
    {E_UBCD_4_BYTE,E_KWH_UNIT,E_MATUO}, //0003
    {E_YYMMDDHH,E_NONE_UNIT,0},         //0004
    {E_UBCD_4_BYTE,E_YUAN_UNIT,-2},     //0005
    {E_BCDFORMAT,0,0},                  //0006
    {E_UBCD_2_BYTE,E_V_UNIT,-1},        //0007
    {E_BCD_3_BYTE,E_A_UNIT,-3},         //0008
    {E_UBCD_3_BYTE,E_A_UNIT,-3},        //0009
    {E_BCD_3_BYTE,E_KW_UNIT,E_DATUO},   //000A
    {E_UBCD_3_BYTE,E_KW_UNIT,E_DATUO},  //000B
    {E_BCD_2_BYTE,E_NONE_UNIT,-3},      //000C
    {E_UBCD_2_BYTE,E_NONE_UNIT,-3},     //000D
    {E_UBCD_2_BYTE,E_HZ_UNIT,-2},       //000E
    {E_UBCD_2_BYTE,E_V_UNIT,-2},        //000F
    {E_UBCD_4_BYTE,E_NONE_UNIT,0},      //0010
    {E_UBCD_4_BYTE,E_YUAN_UNIT,-4},     //0011
    {E_UBCD_1_BYTE,E_NONE_UNIT,0},      //0012
    {E_UBCD_2_BYTE,E_MINUTE_UNIT,0},    //0013
    {E_UBCD_1_BYTE,E_MINUTE_UNIT,0},    //0014
    {E_WW_YYMMDD,E_NONE_UNIT,0},        //0015
    {E_YYMMDD,E_NONE_UNIT,0},           //0016
    {E_HHMMSS,E_NONE_UNIT,0},           //0017
    {E_YYMMDDHHMM,E_NONE_UNIT,0},       //0018
    {E_UBCD_2_BYTE,E_NONE_UNIT,0},      //0019
    {E_U_1_BYTE,E_NONE_UNIT,0},         //001A
    {E_UBCD_3_BYTE,E_NONE_UNIT,0},      //001B
    {E_ID_CODE_4_FORMAT,0,0},           //001C  /**地址表号低8位/
    {E_ID_CODE_2_FORMAT,E_NONE_UNIT,0}, //001D  /**地址表号高4位/
    {E_ASCII_32_LENGTH,E_NONE_UNIT,0},  //0x1E
    {E_ASCII_6_LENGTH,E_NONE_UNIT,0},   //0x1F
    {E_ASCII_4_LENGTH,E_NONE_UNIT,0},   //0x20
    {E_ASCII_10_LENGTH,E_NONE_UNIT,0},  //0x21
    {E_ASCII_16_LENGTH,E_NONE_UNIT,0},  //0x22
    {E_BCD_4_BYTE,E_NONE_UNIT,0},       //0023
    {E_BCD_3_BYTE,E_NONE_UNIT,0},       //0024
    {E_U_2_BYTE,E_NONE_UNIT,0},         //0025
    {E_U_4_BYTE,E_MINUTE_UNIT,0},       //0x26
    {E_MMDDHHMM,E_NONE_UNIT,0},         //0027
    {E_DDHH,E_NONE_UNIT,0},             //0028 /*"显示格式: 未做BCD转换"*/
    {E_U_6_BYTE,E_NONE_UNIT,0},         //0x29
    {E_HHMM,E_NONE_UNIT,0},             //002A
    {E_NN_MMDD,E_NONE_UNIT,0},          //002B
    {E_NN_HHMM_42,E_NONE_UNIT,0},       //002C
    {E_NN_YYMMDD,E_NONE_UNIT,0},        //002D
    {E_YYYYMMDDHHMMSS_FF,E_NONE_UNIT,0},//002E  XYT 切换时间
    {E_MMDDHH,E_NONE_UNIT,0},           //002F /*"显示格式: 未做BCD转换"*/
    {E_ASCII_8_LENGTH,E_NONE_UNIT,0},   //0x30
    {E_YYMMDDHHMMSS,E_NONE_UNIT,0},     //0031
    {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0},//{E_YYYYMMDDHHMMSS,E_NONE_UNIT,0},   //0032  yl     //0032  /*先暂时定个重复的，否则后面的都不对了*/
    {E_U_2_BYTE,E_V_UNIT,-1},           //0033
    {E_S_4_BYTE,E_A_UNIT,-3},           //0034
    {E_S_4_BYTE,E_KW_UNIT,-4},          //0x35      /*2018-01-29:改成kw*/
    {E_S_2_BYTE,E_NONE_UNIT,-3},        //0036
    {E_U_2_BYTE,E_HZ_UNIT,-2},          //0037
    {E_U_4_BYTE,E_KWH_UNIT,-2},         //0038
    {E_S_4_BYTE,E_KWH_UNIT,-2},         //0039
    {E_U_1_BYTE,E_NONE_UNIT,0},         //003A
    {E_U_4_BYTE,E_YUAN_UNIT,-4},        //003B
    {E_U_4_BYTE,E_YUAN_UNIT,-2},        //003C
    {E_U_4_BYTE,E_NONE_UNIT,0},         //0x3D
    {E_U_4_BYTE,E_SECOND_UNIT,0},       //0x3E
    {E_U_6_BYTE,E_NONE_UNIT,0},         //0x3F
    {E_BCDFORMAT,0,0},  //0x40  /*先暂时定个重复的，否则后面的都不对了*/,         //0x40 /*先暂时定个重复的，否则后面的都不对了*/
    {E_U_4_BYTE,E_A_UNIT,-3},           //0x41
    {E_U_4_BYTE,E_W_UNIT,-1},           //0x42
    {E_U_2_BYTE,E_NONE_UNIT,-3},        //0x43
    {E_BCDFORMAT,0,0},        //0x44  /*先暂时定个重复的，否则后面的都不对了*/
    {E_NN_MMDD_42,E_NONE_UNIT,0},       //0x45
    {E_U_4_BYTE,E_NONE_UNIT,-2},        //0046
    {E_BCDFORMAT,0,0},        //0047 /*先暂时定个重复的，否则后面的都不对了*/
    {E_ASCII_8_LENGTH,E_NONE_UNIT,0},   //0x48
    {E_DDHH,E_NONE_UNIT,0},             //0049
    {E_NN_YYYYMMDDWW,E_NONE_UNIT,0},    //004A   /*先暂时定个重复的，否则后面的都不对了*/
    {E_MMDDHH,E_NONE_UNIT,0},           //0x4B
    {E_U_2_BYTE,E_V_UNIT,-2},           //004C
    {E_BCDFORMAT,0,0},                  //004D   /*先暂时定个重复的，否则后面的都不对了*/
    {E_BCDFORMAT,0,0},                   //004E   /*先暂时定个重复的，否则后面的都不对了*/
    {E_U_5_BYTE,E_NONE_UNIT,0},         //0x4F
    {E_S_4_BYTE,E_A_UNIT,-4},           //0x50

    {E_U_ENGY_8_BYTE,E_KWH_UNIT,-4},    //0x5b
    {E_S_ENGY_8_BYTE,E_KWH_UNIT,-4},    //0x5c
    
    {E_UENGY_H_INT,E_KWH_UNIT,0},       //0x66
    {E_UENGY_H_DECIMAL,E_KWH_UNIT,-4},  //0x67
    {E_ENGY_H_INT,E_KWH_UNIT,0},        //0x68
    {E_ENGY_H_DECIMAL,E_KWH_UNIT,-4},   //0x69

    #ifdef THREE_PHASE
    {E_S_4_BYTE,E_KVAR_UNIT,-4},        //0x51  
    {E_S_4_BYTE,E_KVA_UNIT,-4},         //0x52    
    {E_BCD_3_BYTE,E_KVAR_UNIT,E_DATUO}, //0x53
    {E_BCD_3_BYTE,E_KVA_UNIT,E_DATUO},  //0x54    
    {E_S_2_BYTE,E_NONE_UNIT,-2},        //0x55
    {E_S_2_BYTE,E_NONE_UNIT,-1},        //0x56    
    {E_U_4_BYTE,E_KW_UNIT,-4},          //0x57 
    {E_U_2_BYTE,E_NONE_UNIT,-2},        //0x58
    {E_DEMAND_AND_TIME_698,E_NONE_UNIT,0},//0x59    
    {E_DEMAND_AND_TIME,E_KW_UNIT,E_DATUO},  //0x5A /*显示用 实际没有用*/
       
    {E_S_4_BYTE,E_KVARH_UNIT,-2},         //0X5d    
    {E_S_ENGY_8_BYTE,E_KVARH_UNIT,-4},    //0X5E
    {E_BCD_4_BYTE,E_KVARH_UNIT,E_MATUO},  //0X5F
    
    {E_U_4_BYTE,E_KVARH_UNIT,-2},         //0X60
    {E_U_ENGY_8_BYTE,E_KVARH_UNIT,-4},    //0X61
    {E_UBCD_4_BYTE,E_KVARH_UNIT,E_MATUO}, //0X62
    {E_U_2_BYTE,E_NONE_UNIT,-1},          //0x63
    {E_UBCD_2_BYTE,E_NONE_UNIT,-1},       //0x64   
    {E_UBCD_3_BYTE,E_NONE_UNIT,-2},       //0x65
    
    {E_UENGY_H_INT,E_KVARH_UNIT,0},    //0x6A
    {E_UENGY_H_DECIMAL,E_KVARH_UNIT,-4},//0x6B
    {E_ENGY_H_INT,E_KVARH_UNIT,0},     //0x6C
    {E_ENGY_H_DECIMAL,E_KVARH_UNIT,-4}, //0x6D

    #endif    
};


/*通信与显示格式的ID  具体格式对应上面表格 stFormat*/
const ST_FORMAT_ID stFormatId[] = 
{
    /*分别对应通信、显示,具体格式对应上表 stFormat*/
    {E_FORMAT_U_2_V_1,      E_FORMAT_UBCD_2_V_1},     //FORMAT_V
    {E_FORMAT_S_4_A_3,      E_FORMAT_BCD_3_A_3},      //FORMAT_A
    {E_FORMAT_S_4_KW_4,     E_FORMAT_BCD_3_KW_DATUO}, //FORMAT_PW
    {E_FORMAT_S_2_NONE_3,   E_FORMAT_BCD_2_NONE_3},   //FORMAT_FACTOR
    {E_FORMAT_U_2_HZ_2,     E_FORMAT_UBCD_2_HZ_2},    //FORMAT_FREQ
    {E_FORMAT_U_4_KWH_2,    E_FORMAT_UBCD_4_KWH_MATOU},//FORMAT_ENGY
    {E_FORMAT_S_4_KWH_2,    E_FORMAT_BCD_4_KWH_MATOU},//FORMAT_COMBENGY
    {E_FORMAT_U_1_NONE_0,   E_FORMAT_NONE_6},         //FORMAT_CTRLPARA
    {E_FORMAT_YYYYMMDDHHMMSS_NONE_0,E_FORMAT_YYMMDD_NONE_0},//FORMAT_DATETIME
    {E_FORMAT_YYYYMMDDHHMMSS_NONE_0,E_FORMAT_HHMMSS_NONE_0},//FORMAT_TIME
    {E_FORMAT_U_6_NONE_0,   E_FORMAT_ID_CODE_2_NONE_0},//FORMAT_ADDR_H
    {E_FORMAT_ID_CODE_2_NONE_0,E_FORMAT_ID_CODE_4_NONE_0},//FORMAT_ADDR_L
    {E_FORMAT_U_6_NONE_0,   E_FORMAT_ID_CODE_2_NONE_0},//FORMAT_MTRNO_H
    {E_FORMAT_U_6_NONE_0,   E_FORMAT_ID_CODE_4_NONE_0},//FORMAT_MTRNO_L
    {E_FORMAT_U_2_NONE_0,   E_FORMAT_NONE_6},       //FORMAT_WORD
    {E_FORMAT_U_4_YUAN_4,   E_FORMAT_UBCD_4_YUAN_4},//FORMAT_PRICE
    {E_FORMAT_U_4_YUAN_2,   E_FORMAT_UBCD_4_YUAN_2},//FORMAT_MONEY
    {E_FORMAT_U_4_NONE_0,   E_FORMAT_UBCD_2_NONE_0},//FORMAT_BUYTIMES
    {E_FORMAT_U_4_KWH_2,    E_FORMAT_BCD_4_KWH_2},  //FORMAT_BILLENGY
    {E_FORMAT_DDHH_NONE_0,  E_FORMAT_DDHH_NONE_0},  //FORMAT_BILLDATE 
    {E_FORMAT_ID_CODE_4_NONE_0,E_FORMAT_ID_CODE_4_NONE_0},//FORMAT_IDCODE
    {E_FORMAT_S_4_A_3,      E_FORMAT_UBCD_3_A_3},   //FORMAT_N_A
    {E_FORMAT_U_4_NONE_0,   E_FORMAT_UBCD_3_NONE_0},//FORMAT_CONST
    {E_FORMAT_U_4_KWH_2,    E_FORMAT_UBCD_4_KWH_2}, //FORMAT_STEPENGY
    {E_FORMAT_MMDDHH_NONE_0,E_FORMAT_MMDDHH_NONE_0},//FORMAT_YEARBILL
    {E_FORMAT_NN_HHMM_NONE_0,E_FORMAT_NONE_6},      //FORMAT_DAILYSLOT
    {E_FORMAT_NONE_6,      E_FORMAT_NONE_6},       //FORMAT_DISP
    {E_FORMAT_U_4_NONE_0,   E_FORMAT_NONE_6},       //FORMAT_P4
    {E_FORMAT_NONE_6,       E_FORMAT_NONE_6},       //FORMAT_NULL
    {E_FORMAT_YYYYMMDDHHMMSS_FF_NONE_0,E_FORMAT_YYMMDD_NONE_0},//FORMAT_SWTIME
    {E_FORMAT_NN_YYYYMMDDWW_NONE_0, E_FORMAT_NONE_6},//FORMAT_HOLIDAY
    {E_FORMAT_NN_MMDD_NONE_0,E_FORMAT_NONE_6},      //FORMAT_YEARSLOT
    {E_FORMAT_ASCII_4_NONE_0,E_FORMAT_NONE_6},      //FORMAT_ASCII4
    {E_FORMAT_ASCII_6_NONE_0,E_FORMAT_NONE_6},      //FORMAT_ASCII6
    {E_FORMAT_ASCII_8_NONE_0,E_FORMAT_NONE_6},      //FORMAT_ASCII8
    {E_FORMAT_ASCII_16_NONE_0,E_FORMAT_NONE_6},     //FORMAT_ASCII16
    {E_FORMAT_ASCII_32_NONE_0,E_FORMAT_NONE_6},     //FORMAT_ASCII32
    //{0x31,0x06},//FORMAT_EVTTIME
    {E_FORMAT_U_4_A_3,        E_FORMAT_NONE_6},     //FORMAT_EVT_A
    {E_FORMAT_U_4_W_1,        E_FORMAT_NONE_6},     //FORMAT_EVT_PW
    {E_FORMAT_U_2_NONE_3,     E_FORMAT_NONE_6},     //FORMAT_EVT_FACTOR
    //{0x18,0x06},//FORMAT_FRZTIME
    {E_FORMAT_NONE_44,        E_FORMAT_NONE_6},     //FORMAT_PRGOAD
    {E_FORMAT_NN_MMDD_42_NONE_3,E_FORMAT_NONE_6},   //FORMAT_YSLOTTAB
    {E_FORMAT_U_4_NONE_0,     E_FORMAT_UBCD_4_NONE_0},//FORMAT_U4_UBCD4
    {E_FORMAT_U_2_V_2,        E_FORMAT_UBCD_2_V_1}, //FORMAT_BV
    {E_FORMAT_NONE_4D,        E_FORMAT_NONE_6},     //FORMAT_RCDOAD
    {E_FORMAT_NONE_4E,        E_FORMAT_NONE_6},     //FORMAT_EVT_STEP
    {E_FORMAT_U_5_NONE_0,     E_FORMAT_NONE_6},     //FORMAT_CARD_CMDHEAD
    {E_FORMAT_NONE_47,        E_FORMAT_NONE_6},     //FORMAT_BYTE16
    {E_FORMAT_U_4_MINUTE_0,   E_FORMAT_NONE_6},     //FORMAT_MINUTE
    {E_FORMAT_U_2_NONE_0,     E_FORMAT_UBCD_2_NONE_0},//FORMAT_U2_UBCD2 
    {E_FORMAT_U_1_NONE_0,     E_FORMAT_UBCD_2_NONE_0},//FORMAT_U1_UBCD2 
    {E_FORMAT_S_4_A_4,        E_FORMAT_UBCD_3_A_3},   //FORMAT_A_4
    
    {E_FORMAT_U_8_KWH_4,    E_FORMAT_U_H_INT_KWH_0},      //FORMAT_ENGY_H
    {E_FORMAT_S_8_KWH_4,    E_FORMAT_S_H_INT_KWH_0},      //FORMAT_COMBENGY_H
    
    #ifdef THREE_PHASE
    {E_FORMAT_S_4_KVAR_4,     E_FORMAT_BCD_3_KVAR_DATUO},//FORMAT_R_PW
    {E_FORMAT_S_4_KVA_4,      E_FORMAT_BCD_3_KVA_DATUO}, //FORMAT_VA_PW
    {E_FORMAT_S_2_NONE_2,     E_FORMAT_UBCD_2_NONE_0},  //FORMAT_S2_2_UBCD2_0 
    {E_FORMAT_S_2_NONE_1,     E_FORMAT_UBCD_2_NONE_1},  //FORMAT_S2_1_UBCD2_1 
    {E_FORMAT_U_4_KW_4,       E_FORMAT_UBCD_3_KW_DATUO},//FORMAT_A_PW
    {E_FORMAT_U_2_NONE_2,     E_FORMAT_UBCD_2_NONE_0},  //FORMAT_U2_2_UBCD2_0 
    
    {E_FORMAT_DEMAND_AND_TIME_698,E_FORMAT_DEMAND_AND_TIME},//FORMAT_DEMAND
    
    {E_FORMAT_S_4_KVARH_2,  E_FORMAT_BCD_4_KVARH_MATOU},  //FORMAT_COMBENGY_R
    {E_FORMAT_S_8_KVARH_4,  E_FORMAT_S_H_INT_KVARH_0},    //FORMAT_COMBENGY_R_H
    
    {E_FORMAT_U_4_KVARH_2,  E_FORMAT_UBCD_4_KVARH_MATOU}, //FORMAT_ENGY_R
    {E_FORMAT_U_8_KVARH_4,  E_FORMAT_U_H_INT_KVARH_0},    //FORMAT_ENGY_R_H
    
    {E_FORMAT_U_2_NONE_1,  E_FORMAT_UBCD_2_NONE_1},       //FORMAT_U2_1_UBCD2_1
    #endif
};

#if 0   //b0-b8
        UINT8 Group:1;    /*"组合"*/
        UINT8 Month:1;   /*"月--> 当前月or 上X月时使用"*/
        UINT8 N:1;      	/*"N   -->零线"*/
        UINT8 C:1;      	/*"C   -->C相"*/
        UINT8 Curr:1;      /*"当前"*/
        UINT8 Befor:1;	/*"上 -->上X次 or上X月时使用"*/
        UINT8 A:1;       	/*"A   -->B相"*/
        UINT8 B:1;     	/*"A   -->B相"*/

        UINT8 Noth:1;      	/*"无"*/
        UINT8 Dir:1;      	/*"向"*/
        UINT8 Rem:1;     /*"剩余"*/
        UINT8 Step:1;	/*"阶梯"*/
        UINT8 Nega:1; 	/*"反"*/
        UINT8 Plus:1;     	/*"正"*/
        UINT8 cos:1;   	/*"COS"*/
        UINT8 Theta:1;    /*"θ "*/

        UINT8 I:1;      	/*"I"*/
        UINT8 II:1;      	/*"II"*/
        UINT8 Mea:1;     	/*"量"*/
        UINT8 Elec:1;	/*"电"*/
        UINT8 Ac:1; 		/*"有"*/	//active
        UINT8 Tive:1;     	/*"功"*/
        UINT8 Dem:1;   	/*"需"*/
        UINT8 Triff:1;    	/*"Flg0   -->显示总尖峰平谷"*/

        UINT8 MonBef:1;	/*"Flg1   -->当前/上X月使用"*/
        UINT8 CntBef:1;	/*"Flg2   -->上X次使?*/
        UINT8 Volt:1;     	/*"压"*/
        UINT8 Lost:1;	/*"失"*/
        UINT8 V:1; 		/*"V   -->第5"*/
        UINT8 Total:1;  	/*"总-->"*/
        UINT8 Fee:1;   	/*"费"*/
        UINT8 Price:1;  	/*"价"*/

        UINT8 Flash:1;   	/*"Flg3   -->闪烁象限组合"*/
        UINT8 Dir:1;      	/*"段"*/
        UINT8 Room:1;  	/*"间"*/
        UINT8 Time:1;	/*"时"*/
        UINT8 B4:1; 		/*"Flg4   -->"*/
        UINT8 B5:1;     	/*"Flg5   -->"*/
        UINT8 Flow:1;   	/*"流"*/
        UINT8 Power:1;   /*"功率"*/
#endif 

/*显示配置信息表*/
const ST_SHOW_INFO stShowinfo[] = 
{
    {0x11,0x00,0xBC,0x00,0x00},//0000  /*当前   费率 组合有功     电量*/
    {0x10,0x22,0xBC,0x00,0x00},//0001  /*当前   费率 正向有功 电量*/
    {0x10,0x12,0xBC,0x00,0x00},//0002  /*当前   费率 反向有功 电量*/
    {0x23,0x00,0xBC,0x00,0x00},//0003  /*上*月  费率 组合有功     电量*/
    {0x22,0x22,0xBC,0x00,0x00},//0004  /*上*月  费率 正向有功 电量*/
    {0x22,0x12,0xBC,0x00,0x00},//0005  /*上*月  费率 反向有功 电量*/  
    {0x12,0x00,0x0C,0x20,0x10},//0006  /*当前上*月  总用 电量 */
    {0x22,0x00,0x0C,0x20,0x10},//0007  /*上*月  总用 电量 */
    {0x00,0x00,0x00,0x00,0x0C},//0008  /*时间*/
#ifdef SINGLE_PHASE
    {0x00,0x04,0x00,0x40,0x00},//0009  /*剩余 金额*/
#else
    {0x10,0x04,0x08,0x40,0x00},//0009  /*剩余 电费*/
#endif
#ifdef SINGLE_PHASE
    {0x00,0x00,0x00,0x40,0x60},//000A  /*透支 金额*/
#else
    {0x10,0x00,0x08,0x40,0x00},//000A  /*透支电费*/
#endif
    {0x00,0x00,0x00,0x00,0x00},//000B  /*空白*/
    {0x40,0x00,0x00,0x00,0x00},//000C  /*上*/
#ifdef SINGLE_PHASE
    {0x00,0x00,0x30,0x00,0x80},//000D  /*有功功率*/
#else
    {0x00,0x00,0x30,0x20,0x80},//000D  /*总有功功率*/
#endif

#ifdef THREE_PHASE
    {0x40,0x00,0x30,0x00,0x80},//000E  /*"A相有功功率"*/
    {0x80,0x00,0x30,0x00,0x80},//000F  /*"B相有功功率"*/
    {0x08,0x00,0x30,0x00,0x80},//0010  /*"C相有功功率"*/
#endif
    
#ifdef SINGLE_PHASE
    {0x00,0xC0,0x00,0x00,0x00},//0011  /*COSΦ*/
#else
    {0x00,0xC0,0x00,0x20,0x00},//0011  /*COSΦ总*/
#endif

#ifdef THREE_PHASE
    {0x40,0xC0,0x00,0x00,0x00},//000F  /*COSΦA*/
    {0x80,0xC0,0x00,0x00,0x00},//0028  /*"COSΦB"*/
    {0x08,0xC0,0x00,0x00,0x00},//0029  /*"COSΦC"*/
    {0x04,0x00,0x08,0x00,0x40},//0010  /*N 电流*/
#else
    {0x04,0x00,0x00,0x00,0x00},//0010  /*N*/
#endif
    
    {0x10,0x08,0x08,0x80,0x00},//0011  /*当前 阶梯 电 价*/
    {0x10,0x00,0x08,0x80,0x00},//0012  /*当前 电 价*/
    {0x10,0x00,0x00,0x00,0x0C},//0013  /*当前 时间*/
    {0x08,0x00,0x00,0x00,0x01},//0014  /*表号*/
    
#ifdef SINGLE_PHASE
    {0x00,0x00,0x00,0x01,0x00},//0015  /*有功常数*/
#else
    {0x00,0x00,0x30,0x00,0x00},//0015  /*有功常数*/
#endif

    {0x00,0x00,0x00,0x40,0x00},//0x16  /*总*/
#ifdef SINGLE_PHASE
    {0x08,0x00,0x00,0x02,0x00},//0x17  /*户号*/
#else
    {0x00,0x00,0x00,0x00,0x00},//0x17  /*户号*/
#endif

    {0x10,0x08,0x0C,0x20,0x10},//0x18  /*当前 阶梯 总用电量*/
    {0x20,0x08,0x0C,0x20,0x10},//0x19  /*上 阶梯 总用电量*/
    {0x10,0x00,0x88,0x80,0x00},//0x1A  /*当前 费率 电价*/
  
    {0x10,0x08,0x0C,0x00,0x00},//0x1C  /*阶梯 电价 金额*/
    {0x10,0x08,0x00,0x00,0x00},//0x1D  /*阶梯*/
    
    {0x00,0x08,0x08,0x80,0x00},//0x1B  /*阶梯 电价*/
    {0x00,0x08,0x0C,0x00,0x00},//0x1C  /*阶梯 电价 金额*/
    {0x00,0x08,0x00,0x00,0x00},//0x1D  /*阶梯*/
    {0x00,0x00,0x08,0x80,0x00},//0x1A  /*费率 电价*/
  
#ifdef THREE_PHASE
    {0x00,0x01,0x20,0x00,0x00},//001E  /*"无功常数"*/

    {0x40,0x00,0x08,0x00,0x40},//001F  /*"A相电流"*/
    {0x80,0x00,0x08,0x00,0x40},//0020  /*"B相电流"*/
    {0x08,0x00,0x08,0x00,0x40},//0021  /*"C相电流"*/
    {0x40,0x00,0x08,0x04,0x00},//0022  /*"A相电压"*/
    {0x80,0x00,0x08,0x04,0x00},//0023  /*"B相电压"*/
    {0x08,0x00,0x08,0x04,0x00},//0024  /*"C相电压"*/
    
    {0x40,0x80,0x00,0x00,0x00},//000F  /*"ΦA"*/
    {0x80,0x80,0x00,0x00,0x00},//0028  /*"ΦB"*/
    {0x08,0x80,0x00,0x00,0x00},//0029  /*"ΦC"*/

    {0x00,0x01,0x20,0x20,0x80},//0042  /*总无功功率*/
    {0x40,0x01,0x20,0x00,0x80},//002A  /*"A相无功功率"*/
    {0x80,0x01,0x20,0x00,0x80},//002B  /*"B相无功功率"*/
    {0x08,0x01,0x20,0x00,0x80},//002C  /*"C相无功功率"*/

    {0x00,0x00,0x00,0x20,0x80},//0042  /*总功率*/
    {0x40,0x00,0x00,0x00,0x80},//002A  /*"A功率"*/
    {0x80,0x00,0x00,0x00,0x80},//002B  /*"B功率"*/
    {0x08,0x00,0x00,0x00,0x80},//002C  /*"C功率"*/
 
    {0x11,0x01,0xAD,0x00,0x00},//002E  /*"当前组合费率 无功 1  电量"*/
    {0x11,0x01,0xAE,0x00,0x00},//002F  /*"当前组合费率 无功 2  电量"*/
    {0x10,0x01,0xAD,0x00,0x00},//0030  /*"当前费率 I象限无功 电量"*/
    {0x10,0x01,0xAE,0x00,0x00},//0031  /*"当前费率 II象限无功 电量"*/
    {0x10,0x01,0xAF,0x00,0x00},//0032  /*"当前费率 III象限无功 电量"*/
    {0x10,0x01,0xAD,0x10,0x00},//0033  /*"当前费率 IV象限无功 电量"*/

    {0x10,0x00,0x74,0x00,0x00},//0034  /*"当前 有功 需量"*/
    {0x10,0x01,0x64,0x00,0x00},//0034  /*"当前 无功 需量"*/
    
    {0x10,0x22,0xF4,0x00,0x00},//0034  /*"当前费率 正向有功 需量"*/
    {0x10,0x22,0xF4,0x00,0x0C},//0035  /*"当前费率 正向有功 需量发生时间"*/
    {0x10,0x12,0xF4,0x00,0x00},//0042  /*"当前费率 反向有功 需量"*/
    {0x10,0x12,0xF4,0x00,0x0C},//0043  /*"当前费率 反向有功 需量发生时间"*/

    {0x11,0x01,0xE5,0x00,0x00},//002E  /*"当前组合费率 无功 1  需量"*/
    {0x11,0x01,0xE5,0x00,0x0C},//002E  /*"当前组合费率 无功 1  需量 时间"*/
    {0x11,0x01,0xE6,0x00,0x00},//002F  /*"当前组合费率 无功 2  需量"*/
    {0x11,0x01,0xE6,0x00,0x0C},//002F  /*"当前组合费率 无功 2  需量 时间"*/    
    {0x10,0x01,0xE5,0x00,0x00},//0030  /*"当前费率 I象限无功 需量"*/
    {0x10,0x01,0xE5,0x00,0x0C},//0030  /*"当前费率 I象限无功 需量时间"*/    
    {0x10,0x01,0xE6,0x00,0x00},//0031  /*"当前费率 II象限无功 需量"*/
    {0x10,0x01,0xE6,0x00,0x0C},//0031  /*"当前费率 II象限无功 需量时间"*/    
    {0x10,0x01,0xE7,0x00,0x00},//0031  /*"当前费率 III象限无功 需量"*/
    {0x10,0x01,0xE7,0x00,0x0C},//0031  /*"当前费率 III象限无功 需量时间"*/    
    {0x10,0x01,0xE5,0x10,0x00},//0033  /*"当前费率 IV象限无功 需量"*/
    {0x10,0x01,0xE5,0x10,0x0C},//0033  /*"当前费率 IV象限无功 需量时间"*/


    {0x23,0x00,0xBC,0x00,0x00},//0036  /*"上N月  组合费率 有功     电量"*/
    {0x23,0x01,0xAD,0x00,0x00},//0037  /*"上N月组合费率 无功 1  电量"*/
    {0x23,0x01,0xAE,0x00,0x00},//0038  /*"上N月组合费率 无功 2  电量"*/
    {0x22,0x01,0xAD,0x00,0x00},//0039  /*"上N月费率 I象限无功 电量"*/
    {0x22,0x01,0xAE,0x00,0x00},//003A  /*"上N月费率 II象限无功 电量"*/
    {0x22,0x01,0xAF,0x00,0x00},//003B  /*"上N月费率 III象限无功 电量"*/
    {0x22,0x01,0xAD,0x10,0x00},//003C  /*"上N月费率 IV象限无功 电量"*/

    {0x60,0x00,0x00,0x0C,0x0C},//003D  /*"上N  次A 相失压时间"*/
    {0xC0,0x00,0x00,0x0C,0x0C},//003E  /*"上N  次B 相失压时间"*/
    {0x28,0x00,0x00,0x0C,0x0C},//003F  /*"上N  次C 相失压时间"*/
    {0x00,0x00,0x00,0x2C,0x00},//0040  /*"总失压次数"*/
    {0x00,0x00,0x00,0x2C,0x0C},//0041  /*"总失压累计时间"*/  
    {0x20,0x00,0x00,0x0C,0x0C},//0042  /*"上1  次 相失压时间"*/

#endif
};


/*读前写前函数*/
const ST_FUNCTION stFunction[] = 
{
    {0,0},                   //FUN_NULL
    {0,UI_GetRunStatues},      //FUN_RUNSTATUS
    {BeforeFunSetSystemTime,0},//FUN_SET_TIME
    {UI_ChkLCDParaValid,0},    //FUN_LCDPARA
    {0,UI_GetMultiTermn},      //FUN_MULTI
    {0,UI_ReadRlyStatus},      //FUN_RLYSTATUS
    {0,0},      //FUN_EVTRPTSTATUS
    #ifdef THREE_PHASE
    {BeforeFunSetDuPara,0},    //FUN_SET_DU_PARA
    #endif
    
}; 

void class_comm_build(void)
{
    BUILD_BUG_ON(MAX_E_FORMAT!=ARRAY_SIZE(stFormat));
    
    BUILD_BUG_ON(E_ELE_MAX!=ARRAY_SIZE(CLS_1_6_OBJECT_ELE));

    BUILD_BUG_ON(MAX_FORMAT_ID!=ARRAY_SIZE(stFormatId));

    BUILD_BUG_ON(E_MAX_SHOWINFO_ID!=ARRAY_SIZE(stShowinfo));

    BUILD_BUG_ON(MAX_FUN_ID!=ARRAY_SIZE(stFunction));    
}    
/*"************************************************"*/
