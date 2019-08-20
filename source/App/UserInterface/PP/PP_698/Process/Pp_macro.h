/*"***********************************************************		
FileName: pp_macro.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 协议处理宏定义头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/
#ifndef _PP_MACRO_H
#define _PP_MACRO_H
/********************************************************/
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"


#define CHANNEL 3      /*通道数*/

#define APP_NEXT_FRAME_NO_ACCESS    1  /*应用层数据超过2K，则拒绝读写*/
//#define ENABLE_SAFE_MODE   1
#define DEBUG_PARAMENT_SINGLE_OBIS  0  /*参数类的OBIS，如果多个，但定义时用一个，分别定义*/

#define DEBUG_PP_698_CODE    0  /*698协议调试代码*/

#define ADD_TIME_SET_0  1      /*累计时间是否置0，还是回NULL*/

/********************************************************/

#define OIL  0
#define OIH  1

typedef struct
{
	UN_OI Oi;
	INT8U MTD;
	INT8U MOD;
}_ST_OMD;

/*访问结果DAR枚举定义*/
typedef enum
{
    DAR_0_OK             = 0,   /*"成功"*/
    DAR_1_HW_FAIL        = 1,   /*"硬件失效"*/
    DAR_2_TEM_FAIL       = 2,   /*"暂时失效"*/
    DAR_3_NOACESS        = 3,   /*"拒绝读写"*/
    DAR_4_OANODEF        = 4,  /*"对象未定义"*/
    DAR_5_OANOADAPT      = 5,  /*"对象接口类不符合"*/
    DAR_6_OANOEXIST      = 6,  /*"对象不存在"*/
    DAR_7_TYPENOADAPT    = 7,  /*"类型不匹配"*/
    DAR_8_OVERFLOW       = 8,  /*"越界"*/
    DAR_9_DBNOVAILD      = 9,  /*"数据块不可用"*/
    DAR_10_FRAMECANCEL   = 10, /*"分帧传输已取消"*/
    DAR_11_NOFRAMWSTA    = 11, /*"不处于分帧传输状态"*/
    DAR_12_BWCANCEL      = 12, /*"块写取消"*/
    DAR_13_NOBWEXIST     = 13, /*"不存在块写状态"*/
    DAR_14_DBIDNOVAILD   = 14, /*"数据块序号无效"*/
    DAR_15_PSWERR        = 15, /*"密码错/未授权"*/
    DAR_16_BAUDERR       = 16, /*"通信速率不能更改"*/
    DAR_17_YEARZONE      = 17, /*"年时区数超"*/
    DAR_18_DAILYSLOT     = 18, /*"日时段数超"*/
    DAR_19_TRIFF         = 19, /*"费率数超"*/
    DAR_20_NO_RIGHT      = 20, /*"安全认证不匹配 "*/
    DAR_21_REPRECHARGE   = 21, /*"重复充值"*/
    DAR_22_ESAMCHKFAIL   = 22, /*"ESAM验证失败"*/
    DAR_23_AUTHFAIL      = 23, /*"安全认证失败"*/
    DAR_24_USERNO        = 24, /*"客户编号不匹配"*/
    DAR_25_BUYTIMES      = 25, /*"充值次数错误"*/
    DAR_26_HOARD         = 26, /*"购电超囤积"*/
    DAR_27_ADDRERR       = 27, /*"地址异常"*/
    DAR_28_DECRYPTERR    = 28, /*"对称解密错误"*/
    DAR_29_DECRYPTERR    = 29, /*"非对称解密错误"*/
    DAR_30_SIGNERR       = 30, /*"签名错误"*/
    DAR_31_HANGUP        = 31, /*"电能表挂起"*/
    DAR_32_TIMETAGERR    = 32, /*"时间标签无效    "*/
    DAR_33_TIMEOUT       = 33, /*"请求超时   "*/
    DAR_34_P1P2ERR       = 34, /*"ESAM的P1P2不正确*/
    DAR_35_LCERR         = 35, /*"ESAM的LC错误*/
    DAR_FE_ABORNMAL      = 254,/*异常错误 自定义的*/
    DAR_FF_OTHERERR      = 255,/*"其他错误"*/
}ENUM_DAR;

/*698单位枚举定义*/
typedef enum
{
    UNIT_YEAR     = 1,     /*年*/
    UNIT_MONTH    = 2,     /*月*/    
    MIN           = 6,     /*分*/
    PHASEANGLE    = 8,     /*相角度数*/
    TOC           = 9,
    MONEY         = 10,
    UNIT_W        = 27,    /*瓦*/
    KW            = 28,
    VA            = 29,
    KVA           = 30,
    var           = 31,
    kvar          = 32,
    KWH           = 33,
    kvarh         = 35,
    AMP           = 36,
    VOLTAGE       = 38,
    HZ            = 47,    
    UNIT_H        = 51,    /*% 百分比 */
    UPRICE        = 54,
    AH            = 55,    /*安时*/
    NOUNIT        = 255
}ENUM_UNIT;

/*TI单位枚举定义*/
typedef enum
{
	EN_TI_UNIT_SEC   = 0,  /*TI单位:秒*/
	EN_TI_UNIT_MIN   = 1,  /*TI单位:分*/
	EN_TI_UNIT_HOUR  = 2,  /*TI单位:时*/
	EN_TI_UNIT_DAY   = 3,  /*TI单位:日*/
	EN_TI_UNIT_MONTH = 4,  /*TI单位:月*/
	EN_TI_UNIT_YEAR  = 5   /*TI单位:年*/
}ENUM_TI_UNIT;

/*时钟源枚举定义*/
typedef enum
{ 
	EN_CLR_SRC_INSIDE    = 0,  /*内部（0）*/
	EN_CLR_SRC_RTC       = 1,  /*时钟芯片（1）*/
	EN_CLR_SRC_INTERNET  = 2,  /*互联网时钟（2）*/
	EN_CLR_SRC_SATELLITE = 3,  /*卫星时钟（3）*/
	EN_CLR_SRC_LONG_WAVE = 4,  /*长波时钟（4）*/	
}ENUM_CLR_SRC;

/*与ENUM_DATATYPE一样，待ENUM_DATATYPE增加注释后删掉*/
typedef enum
{
   TNULL        = 0,
   TARRAY       = 1,      /*数组*/
   TSTRUCT      = 2,      /*structure*/
   TBOOL        = 3,
   BITSTRING    = 4,
   CHAR4        = 5,      /* double-long*/
   UCHAR4       = 6,      /* double-long-unsigned*/
   OCTSTRING    = 9,
   VISIBLESTRING = 10,
   UTF8STRING    = 12,
   TBCD          = 13,
   CHAR1         = 15,
   CHAR2         = 16,
   UCHAR1        = 17,
   UCHAR2        = 18,
   CHAR8         = 20,
   UCHAR8        = 21,
   TENUM         = 22,
   FLOAT4        = 23,
   FLOAT8        = 24,
   DATEANDTIME   = 25,
   TDATE         = 26,
   TTIME         = 27,
   DATETIMEBCD   = 28,
   DATETIMEBCDH  = 29,
   DATETIMEBCDS  = 30,
   TDateBCD      = 31,
   TOI           = 80,
   TOAD          = 81,
   TROAD         = 82,
   TOMD          = 83,
   T_TI          = 84,
   TTSA          = 85,
   TMAC          = 86,
   TRN           = 87,
   TREGION       = 88,
   TScaler_Unit  = 89,
   TRSD          = 90,
   TCSD          = 91,
   TSID          = 93,      //SID 0x5d
   TSIDMAC       = 94,      //SID_MAC 0x5e
   TCOMDCB       = 95,
   SPEBITSTRING  = 243,   
   TVISIBLESTRING= 245,    /*变长可视字符串*/
   SPEPROCESS    = 246,
   DEMANDTYPE_R  = 247,
   SPEARRAY      = 248,     /*变长数组*/
   DEMANDTYPE    = 249,
   MAX_TYPE_698_DATA = 250
}EN_TYPE_698_DATA;

#define DEMANDTYPELENGTH 11


/*返回数据打包是DAR还是DATA*/
typedef enum
{
   E_RESULT_CHOICE_DAR     = 0, /*返回数据为DAR，后面跟具体DAR*/
   E_RESULT_CHOICE_DATA    = 1, /*返回数据为DATA,后面跟具体数据内容*/
}EN_RESULT_CHOICE;


#define NO_SERVICE_TYPE   0x0  
/*6.3.4　应用层协议数据单元 应用层服务  CHOICE定义*/
typedef enum
{
   EN_CONNECTREQUEST     = 2,    /*连接请求*/
   EN_DISCONNECTREQUEST  = 3,    /*断开连接*/
   EN_GETREQUEST         = 5,    /*读请求*/
   EN_SETREQUEST         = 6,    /*设置请求*/
   EN_ACTIONREQUEST      = 7,    /*方法请求*/
   EN_SECURITYREQUEST    = 0x10, /*安全请求*/
   EN_ABNORMAL_REPONSE   = 238   /*异常响应*/
}EN_SERVICE_TYPE;

/*读取类choice定义*/
typedef enum
{
   EN_GET_NORMAL         = 1,  /*读取一个对象属性*/
   EN_GET_NORMAL_LIST    = 2,  /*读取若干个对象属性*/
   EN_GET_RECORD         = 3,  /*读取一个记录型对象属性*/
   EN_GET_RECORD_LIST    = 4,  /*读取若干个记录型对象属性*/
   EN_GET_NEXT           = 5,  /*读取分帧响应的下一个数据块*/
   EN_GET_MD5            = 6   /*读取一个对象属性的MD5值*/
}EN_GET_CHOICE;


/*设置类choice定义*/
typedef enum
{
   EN_SET_NORMAL                = 1,  /*设置一个对象属性*/
   EN_SET_NORMAL_LIST           = 2,  /*设置若干个对象属性*/
   EN_SET_THEN_GET_NORMAL_LIST  = 3,  /*设置后读取若干个对象属性*/
}EN_SET_CHOICE;

/*方法类choice定义*/
typedef enum
{
   EN_ACTION_NORMAL                = 1,  /*操作一个对象属性*/
   EN_ACTION_NORMAL_LIST           = 2,  /*操作若干个对象属性*/
   EN_ACTION_THEN_GET_NORMAL_LIST  = 3,  /*操作后读取若干个对象属性*/
}EN_ACTION_CHOICE;

/*协议栈定义，后面可以删除*/
typedef enum
{
   SIDMACDATA       = 0,       /*SID_MAC 数据验证码*/
   RNDATA           = 1,       /*随机数*/
   RNMACDATA        = 2,       /*随机数+数据MAC*/
   SIDDATA          = 3,       /*安全标识*/
   MAX_ENCRYPT_TYPE = 4
}EN_ENCRYPT_TYPE;

/*RSD选择方法*/
typedef enum
{
   EN_SELECTOR1 = 1,
   EN_SELECTOR2 = 2,
   EN_SELECTOR9 = 9,
}EN_SELECTOR;


typedef enum
{
    E_UN_ENCRYPT_TYPE      = 0,   /*明文*/
    E_UN_ENCRYPT_MAC_TYEP  = 1,   /*明文+MAC*/
    E_ENCRYPT_TYPE         = 2,   /*密文*/
    E_ENCRYPT_MAC_TYPE     = 3    /*密文+MAC*/
}EN_CRYPT_TYPE_SAFE_MODE;    

/**************************************************/
/*与协议栈接口*/
typedef struct 
{
  EN_SERVICE_TYPE  servicetype; /*服务类型*/
  EN_ADDRTYPE      AddrType;   /*"地址类型"*/

  INT8U  Crypt_Type;           /*加密级别 0，明文，1，明文+MAC，2，密文，3，密文+MAC*/
  INT16U Len_data;             /*解密后的APDU长度，和编码后的数据长度*/ 

  //INT8U  PortNum;     /*通信端口*/
  //INT8U  res[3];      /*字节对齐,预留*/

  INT8U *pdata;       /*源数据，以及返回编码后的数据*/
}ST_OBJECT;

typedef struct
{
    INT16U rcrd_obis;/*" OBIS "*/
    INT8U  mode; /*" 上报模式 "*/
}ST_RPT_ACTION;

typedef struct 
{
  INT8U Year;
  INT8U Month;
  INT8U Day;
  INT8U Hour;
  INT8U Min;
  INT8U Sec;
}ST_DATA_TIME;

/*筛选方法1*/
typedef struct{
	ST_DATA_TIME Time;       /*筛选的时间*/
}_ST_RSD_SELECT1;


/*筛选方法2*/
typedef struct{ 
	ST_DATA_TIME Time_begin;  /*"起始时间"*/
	ST_DATA_TIME Time_end;    /*"结束时间"*/
	INT8U  Period[3];	          /*"时间间隔[0]为分、时选择"*/
}_ST_RSD_SELECT2;  //15byte?

#define LEN_SELECT2_DATA   sizeof(_ST_RSD_SELECT2)    //15byte?


/*与数据中心交互，记录模块结构体*/
typedef struct
{       
    INT16U Line_start;   /*筛选的开始行*/
    INT16U Line_end;     /*筛选的结束行  0xffff表示第一次筛选，返回最大条数*/
    
    INT8U  NumOad;       /*筛选的OAD个数*/ 
    INT8U  Rer[3];       /*为字节对齐，预留3字节*/
    
    ST_OAD *Poadlist;    /*筛选的OAD列表*/
    void   *pDdataFilter;/*筛选的数据条件*/
}ST_Filter_Record_In;  

/*与数据中心交互，记录模块结构体*/
typedef struct
{   
    INT16U NumRecord;   /*记录条数 主要是第一次筛选时返回*/
    INT8U  Reser[2];    /*保留 字节对齐*/
    INT8U *pleng;       /*输出长度数据:长度为0，表示无数据*/
    INT8U *pbuf;        /*数据缓存地址，依次存储*/
}ST_Filter_Record_Out;  
/**************************************************/
/*时间标签 用时注意字节对齐*/
typedef struct
{
  INT8U OptByte;         /*选项字节0:无，1:有时间标签*/  
  
  /*发送时标*/
  INT16U year;
  INT8U  month;
  INT8U  day;
  INT8U  hour;
  INT8U  minute;
  INT8U  second;

  /*允许传输延时时间*/
  INT8U   unit;         /*ENUMERATED,单位:秒:0,分:1,时:2,日:3,月:4,年,5*/
  INT16U  interval;     /*间隔值  long-unsigned*/
}ST_TIME_TAG ;

#define LEN_TIME_TAG    (11) // sizeof(ST_TIME_TAG)

/*最大的事件上报长度1,2,0x33,0x20,0x02,0x01,0x01,TOAD OAD(4) 20150200 01 BITSTRING  4 4BYTE*/
/*01 02 33 20 02 01 01 51 00 00 00 00 20 15 02 00 01 04 20 00 00 00 00*/
#define MAX_LEN_FOLLOW_REPORT  23

#define LEN_RESERVE    8       /*预留8字节，免得越界了*/


/*日期时间数据类型date_time_s 用时注意字节对齐*/
#if 0
typedef struct
{
  INT16U year;
  INT8U  month;
  INT8U  day;
  
  INT8U  hour;
  INT8U  minute;
  INT8U  second;
}ST_DATE_TIME_S ;
#endif

#define LEN_DATE_TIME_S     (7)  //sizeof(ST_DATE_TIME_S)


/*时间间隔TI数据类型*/
typedef struct
{
  INT8U   unit;         /*ENUMERATED,单位:秒:0,分:1,时:2,日:3,月:4,年,5*/
  INT16U  interval;     /*间隔值  long-unsigned*/
}ST_TI;



/*安全模式参数结构体定义*/
typedef struct
{
	UN_OI Oi;
	INT8U Rigth2;   /*参考 698协议 附录表F.2　安全模式定义*/
	INT8U Rigth3;
}ST_F101_SAFE_MODE;

/*8类控制类属性和方法枚举*/
typedef enum
{
    ATTRIBUTE_2_SET_PARAMENT  = 2,             /*属性2（配置参数)*/
    ATTRIBUTE_3_STATUS_RELAY  = 3,             /*属性3（继电器输出状态，只读)*/
    ATTRIBUTE_4_STATUS_ALARM  = 4,             /*属性4(告警状态，只读)*/
    ATTRIBUTE_5_STATUS_CMD    = 5,             /*属性5（命令状态，只读)*/
    METHOD_127_ALARM          = 127,           /*方法127：触发告警（参数）参数∷=NULL*/
    METHOD_128_CANCEL_ALARM   = 128,           /*方法128：解除报警（参数）∷=NULL*/
    METHOD_129_OPEN           = 129,           /*方法129：跳闸（参数）*/
    METHOD_130_CLOSE          = 130,           /*方法130：密文合闸（参数）*/
    METHOD_131_CLOSE          = 131,           /*方法131：明文密码合闸（参数）*/
}EN_ATT_METHOD_REMOTE_698;
/*-----------------------------------------------*/
#define LEN_RN         16      /*随机数长度*/
#define LEN_MAC_DATA   7       /*01 00 04 4byte mac*/


#define LEN_GET_DAR      6    /*get dar的打包长度*/
#define LEN_SET_DAR      5    /*set dar的打包长度*/
#define LEN_ACTION_DAR   6    /*action dar的打包长度*/

#define LEN_ATT1_ENCODE  8    /*打包ATT1长度*/

#define LEN_LOGIC_NAME   4    /*逻辑名*/

#define LEN_SCALE_UNIT   3    /*换算单位*/

/*bit5…bit7编码表示属性特征，属性特征是对象同一个属性在不同快照环境下取值模式，取值0…7，特征含义在具体类属性中描述。*/
#define MASK_AID_SELECT     0x1F   /*AID 掩码*/

#define MASK_AID_SELECT_B5_B7   0xE0   /**/

/*乘除用宏，改成位移*/
#define DIV_2(num)       ((num)>>1)  /*除2*/
#define DIV_4(num)       ((num)>>2)
#define DIV_8(num)       ((num)>>3)
#define DIV_16(num)      ((num)>>4)
#define DIV_32(num)      ((num)>>5)
#define DIV_64(num)      ((num)>>6)
#define DIV_128(num)     ((num)>>7)
#define DIV_256(num)     ((num)>>8)

#define MULTI_2(num)     ((num)<<1)   /*乘2*/
#define MULTI_4(num)     ((num)<<2)
#define MULTI_8(num)     ((num)<<3)
#define MULTI_16(num)    ((num)<<4)
#define MULTI_32(num)    ((num)<<5)
#define MULTI_64(num)    ((num)<<6)
#define MULTI_128(num)   ((num)<<7)
#define MULTI_256(num)   ((num)<<8)
/*乘除用宏，改成位移 end*/



#define MAXGETLISTNUMBER 		 64    /* get list 最大对象个数  */
#define MAXGETRECORDLISTNUMBER   3     /* getrecordlist最大对象个数 */
#define MAXCSD                   55    /*这里不应该限制吧*/
#define MAXSETLISTNUMBER 		 10
#define MAXSETTHENGETLISTNUMBER  5
#define MAXACTIONLISTNUMBER      10
#define MAXESAMGETDATALEN        32   /*ESAM GET最大方法长度*/

#define MAXACTIONTHENLISTNUMBER MAXSETTHENGETLISTNUMBER

/*是否跟随上报*/
#define  FLAG_FOLLOW_REPORT_NUM     mEvtPrtStatus.OadNum[Report_Channal()]
#define  FLAG_FOLLOW_REPORT_STATUS  mEvtPrtStatus.rptFlg[Report_Channal()]           


#define   COMM_SET_CHANGE_FORMAT    1  /*是否先转格式*/

#define MAX_IID_2014   7     /*电能表运行状态字*/

#define MAX_IID_DAILY_SLOT_TABLE 8  /*日时段表数最大IID*/
#define MAX_IID_YEALY_SLOT_TABLE 14 /*时区表最大IID*/
#define MAX_IID_TRIFF 63            /*费率电价最大IID*/


#ifdef THREE_PHASE
#define MAX_CLASS2_LIST_NUM    10
#define MAX_CLASS3_LIST_NUM    5
#define MAX_CLASS4_LIST_NUM    6
#define MAX_CLASS6_LIST_NUM    29
#else
#define MAX_CLASS3_LIST_NUM    2
#define MAX_CLASS4_LIST_NUM    2
#define MAX_CLASS6_LIST_NUM    18
#endif


#define MAX_DATA_LEN_ACTION     312   /*方法类带的数据长度*/
#define MAX_ONE_OAD_DATA_LEN    384   /*一个OAD的数据长度*/

#ifdef THREE_PHASE
#define MAX_ONE_RECORD_DATA_LEN 1000   /*一个记录的数据长度*/
#else
#define MAX_ONE_RECORD_DATA_LEN 512   /*一个记录的数据长度*/
#endif


#define MAX_CODE_CMP_LEN        (1024+16+2) /*1042空间可一次比较4096字节*/
//#if THREE_PHASE
//#define MAX_CODE_CMP_LEN        540
//#else
//#define MAX_CODE_CMP_LEN        300
//#endif

#define MAX_DATA_LEN_SET        MAX_DATA_LEN_ACTION  

#define MAX_OBIS_NUM_ONE_OAD    MAX_ID_LIST_NUM

/*事件全清，用一个特殊的OBIS代替*/
#define CLR_ALL_EVENT_OBIS    0xfffe
#define CLR_METER_OBIS        0xffff    /*电表清0 OBIS*/

#define E_OBIS_NULL             0xfff0    /*无效的OBIS*/

#define DAR_READ_FUN_DIRECT_DATA  0xAA     /*读前函数，直接返回数据*/


/*函数声明，供其它模块调用*/
extern INT16S PP_process(ST_OBJECT *pObject);
extern INT8U PP_OadToObis(ST_OAD *pOad, INT16U *pObis, INT16U *pOffset, ST_FMT_ATTR *pFormat);
extern INT8U PP_ObisToOad(INT16U Obis,ST_OAD *pOad);
extern void Init_Safe_Mode(INT8U mode);

extern ST_OAD CurOad;
extern ST_OAD Program_Oad[10]; 
/********************************************************/
#endif
