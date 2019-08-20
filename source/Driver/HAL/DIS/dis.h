/*
 * showlcd_abstract.h
 *
 *  Created on: 2012-1-19
 *      Author: Administrator
 */

#ifndef _DIS_H
#define _DIS_H

#include "driver\MCU\LCD.h"


//#define DIS_RELAY_ICON_COMB /*"配置项: 继电器拉合闸符号占用2位status"*/
//#define DIS_USER_CODE /*"配置项: 用户代码显示"*/
//#define DIS_QUAD_INFO /*"配置项: 象限显示(低功耗显示使用)"*/
#define DIS_REC_TIME/*"配置项:显示电量记录的年.月而不是上几次"*/
#define DIS_SCR_NO  /*"显示屏序号"*/


#define MAX_CHAR_BYTE       ((MAX_BYTE_CHAR >> 3) + 1)/*"常显字符字节数最大值，这里多定义一个字节是给月份提示用"*/
#define MAX_POINT_NUM       (5)        /*"主数据区带小数点8字的最大数"*/
#define MAX_TPOINT_NUM     (2)        /*"主数据区带时间格式冒号最大数"*/
#define MAX_SHOWDATA        (10)
#define MAX_SCHAR_NUM      (5)
#define MAX_UINT_NUM      (15)   /*"能显示的单位个数"*/
#define MAX_LCD_UINT_NUM      (7)   /*"液晶上单位的段码的个数"*/
#ifdef DIS_USER_CODE
#define MAX_USECODE_NUM  (8)       /*"用户代码区最大数"*/
#define MAX_LEAFNO_NUM    (2) 
#endif
#ifdef DIS_QUAD_INFO
#define MAX_SUB_CHAR_NUM  (6)
#else
#define MAX_SUB_CHAR_NUM  (5)
#endif

/*"********字符段位置结构定义**************"*/
typedef struct _CHAR_SHOW_POS
{
    INT8U X_Pos;/*"X_Pos表示字符处于哪个段字节"*/
    INT8U Y_Pos;/*"Y_Pos表示字符处于段字节哪位"*/
}ST_CHAR_POS;

typedef struct _BIT
{
	INT8U b0 :1;
	INT8U b1 :1;
	INT8U b2 :1;
	INT8U b3 :1;
	INT8U b4 :1;
	INT8U b5 :1;
	INT8U b6 :1;
	INT8U b7 :1;
} BIT;

/*"显示取数输出信息"*/
typedef struct
{
    ST_FMT_ATTR stShowFormat;/*"数据格式"*/
    INT8U schar[MAX_SCHAR_NUM];/*"提示字符信息"*/
}ST_SHOW_TABLE; 

typedef struct
{
    INT8U showDataBuff[MAX_SHOWDATA]; /*"显示数据接收缓存"*/
    ST_SHOW_TABLE  stShowTable; /*"显示信息表输出信息"*/
}ST_SHOWDATA_OUTPUT;

typedef struct
{
    ST_SHOWDATA_OUTPUT Protc;/*"主显示区数据信息"*/
    INT8U charAdd[MAX_SUB_CHAR_NUM];/*"0_b0-b3:月份;b4-b7:费率"*/
                                                              /*"1_b0-b3:时间格式的数据长度;b4-b7:负号信息"*/
                                                             /*"2_屏序号 HEX码"*/
                                                             /*"3_时间:年 BCD码"*/
                                                            /*"4_时间:月 BCD码"*/  
                                                            /*"5_b0-b3: I~IV象限;b4: 0_常显/ 1_闪烁；b5-b7:保留"*/                                                              
}ST_SHOW_DATA;

#ifdef DIS_USER_CODE
typedef struct
{
    INT8U obis[MAX_USECODE_NUM]; /*"用户代码显示区显示内容"*/
    INT8U offset; /*"用户代码从offset偏移的数字区开始显示（不一定从第一个数字开始）"*/
    INT8U len; /*"用户代码显示长度（8字个数） ，取值范围1~8"*/
    UN_BYTE_DEF pointFlg; /*"BIT0~BIT7分别表示第1~8个8字后面的'.'是否点亮，0不点亮  1点亮"*/
    INT8U leafNo[MAX_LEAFNO_NUM]; /*"叶子号"*/
}ST_USERCODE_SHOW;
#endif

typedef union /*"显示控制标志"*/
{
    struct
    {
        INT8U   flash: 1; /*"闪烁"*/
        INT8U   code:1;    /*"用户代码显示"*/
    }bits;
    INT8U   byte;
}UN_DIS_FLAG;

/*"显示优先级定义，由低到高分4级优先级，1级最低，4级最高"*/
typedef enum  
{
    IDLE = 0,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
}EN_SHOW_PRIORITY;

typedef struct 
{
    INT16U lastTime; /*"显示要求持续时间"*/
    EN_SHOW_PRIORITY currPrior;/*"当前显示优先级"*/
    UN_DIS_FLAG flag;/*"显示控制标志"*/
}ST_SHOW_CTRL;



/*"********************状态提示符定义*******************************"*/
/*" *********这里没有超过255个标识符定义，因此只采用1BYTE ****"*/
/*"**void Set_StatusBit_Show(INT8U SegBit_name);********"*/
/*"*****void Clear_StatusBit_Show(INT8U SegBit_name);********"*/
/*"*****void Set_StatusBit_Flash(INT8U SegBit_name);********"*/
/*"*****void Clear_StatusBit_Flash(INT8U SegBit_name);********"*/
/*"*****这里的参数传递值为UINT8型，注意不要超范围********"*/
typedef enum  
{
    STA_STEP_1 = 0, /*"阶梯1"*/
    STA_STEP_2 , /*"阶梯2"*/
    STA_STEP_3 , /*"阶梯3"*/
    STA_STEP_4 , /*"阶梯4"*/
    STA_BAT ,/*"低功耗电池"*/  
    STA_KEY,/*"公钥"*/
    STA_PHONE,/*"通信电话符"*/  
    STA_WAVE,/*"模块通信符"*/ 
    STA_WAVE_UP,/*"模块上行"*/
    STA_WAVE_DOWN,/*"模块下行"*/  
    STA_BLUETOOTH,/*"蓝牙"*/   
    STA_LOCK,/*"闭锁"*/   
    STA_UPGRADE,/*"升级"*/     
    STA_RECHARGE,/*"请购电"*/  
    STA_RELAY_OPEN,/*"拉闸"*/  
    STA_OVERDRAFT,/*"透支"*/    
    
    STA_TARF_JIAN,/*"尖 费率"*/  
    STA_TARF_FENG,/*"峰 费率"*/
    STA_TARF_PING,/*"平 费率"*/    
    STA_TARF_GU,/*"谷 费率"*/   
    STA_TARF_1_SET,/*"第一套时段"*/     
    STA_TARF_2_SET,/*"第二套时段"*/
    STA_STEP_1_SET,/*"第一套阶梯"*/     
    STA_STEP_2_SET,/*"第二套阶梯"*/      
    STA_REVERSE,/*"功率反向 "*/    

    MAX_BYTE_STA
  }Status_SegBit;

#define _DIS_STA_NUM  ((MAX_BYTE_STA>>3) + 1)

typedef enum  
{

    CHAR_COMB = 0,/*"组合"*/
    CHAR_LAST_MON,/*"上X月"*/
    CHAR_N,/*"N"*/          
    CHAR_NUM,/*"号"*/ 
    CHAR_CURR,/*"当前"*/    
    CHAR_LAST,/*"上"*/   
    CHAR_L,/*"L"*/          
    CHAR_B,/*"B"*/ 

    CHAR_DATE,/*"日期"*/ 
    CHAR_XIANG,/*"向"*/
    CHAR_REMAIN,/*"剩余"*/    
    CHAR_STEP,/*"阶梯"*/ 
    CHAR_FAN,/*"反"*/    
    CHAR_PO,/*"正"*/    
    CHAR_COS,/*"COSΦ"*/    
    CHAR_SHE,/*"赊"*/         

    CHAR_FIRST,  /*"Ⅰ"*/                            
    CHAR_SEC,  /*"Ⅱ"*/   
    CHAR_LIANG,/*"量"*/    
    CHAR_DIAN,/*"电"*/ 
    CHAR_YOU,  /*"有"*/ 
    CHAR_GONG,  /*"功"*/     
    CHAR_XU,  /*"需"*/   
    CHAR_TARIFF,  /*"费率"*/    

    CHAR_CONST,/*"常数"*/ 
    CHAR_HU,/*"户"*/
    CHAR_YA,  /*"压"*/         
    CHAR_SHI,  /*"失"*/   
    CHAR_V,  /*"V"*/       
    CHAR_SUM,/*"总"*/   
    CHAR_MONEY,/*"金额"*/
    CHAR_JIA,/*"价"*/         

    CHAR_METER,/*"表"*/  
    CHAR_SEG,/*"段"*/
    CHAR_JIAN,/*"间"*/
    CHAR_SHI2,/*"时"*/        
    CHAR_YONG,/*"用"*/       
    CHAR_OVERDRAW,  /*"透支"*/        
    CHAR_ZHI,/*"支"*/      //CHAR_LIU,  /*"流"*/   
    CHAR_POWER,  /*"功率"*/       
    
    CHAR_MONTH1,/*"上N月预留"*/
    CHAR_MONTH2,/*"上N月预留"*/
    CHAR_MONTH3,/*"上N月预留"*/
    CHAR_MONTH4,/*"上N月预留"*/
    CHAR_TARF_GU,/*"谷"*/   
    CHAR_TARF_PING,/*"平"*/    
    CHAR_TARF_FENG,/*"峰"*/     
    CHAR_TARF_JIAN,/*"尖"*/
    CHAR_1,/*"1"*/      

    MAX_BYTE_CHAR
}Char_SegBit;

typedef enum  
{
    DIS_0 = 0,
    DIS_1,
    DIS_2,
    DIS_3,
    DIS_4,
    DIS_5,
    DIS_6,
    DIS_7,
    DIS_8,
    DIS_9,
    DIS_A,
    DIS_b,
    DIS_C,
    DIS_d,
    DIS_E,
    DIS_F,
    DIS_G,
    DIS_h,
    DIS_i,
    DIS_J,
    DIS_k,
    DIS_L,
    DIS_M,
    DIS_n,
    DIS_o,
    DIS_P,
    DIS_q,
    DIS_r,
    DIS_S,
    DIS_t,
    DIS_u,
    DIS_v,
    DIS_W,//
    DIS_X,//
    DIS_y,
    DIS_z,//
    DIS_Space,
    DIS_Minus,

    DIS_MAX,
}EN_DISP_CHAR;


extern void DIS_SetStauSeg(INT8U SegId);
extern void DIS_SetStauTarfStep(INT8U SegId,INT8U Num);
extern void DIS_ClrStauTarfStep(INT8U SegId);
extern void DIS_ClrStauSeg(INT8U SegId);
extern void DIS_FlashStauSeg(INT8U SegId);
extern void DIS_ClrFlashStauSeg(INT8U SegId);
extern INT8U DIS_PrintLcdLine1(ST_SHOW_DATA *pShowDataL1, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio);
extern INT8U DIS_PrintLcdLine3(INT8U *pShowData, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio);
extern void DIS_init(void);
extern void DIS_Main(void);
#ifdef DIS_USER_CODE
extern INT8U DIS_PrintLcdLine2 (ST_USERCODE_SHOW *pUseCode);
#endif
#if 0
extern void DIS_Char_K(void);
extern void DIS_ClrAllStauSeg(void);
#endif
extern void Dis_ShowAll(void);
extern INT8U set_show_data(INT8U *buf);

#define COM1 0X01
#define COM2 0X02
#define COM3 0X04
#define COM4 0X08
#define COM5 0x10
#define COM6 0x20
#define COM7 0x40
#define COM8 0x80

/* 组成数字与字母的bit 位*/
#define SEG7_A 0x01
#define SEG7_B 0x02
#define SEG7_C 0x04
#define SEG7_D 0x08
#define SEG7_E 0x10
#define SEG7_F 0x20
#define SEG7_G 0x40




#define _SHOW_ALL_  0x5a
#define _SHOW_NONE_ 0x5b


/*"显示数据格式"*/
typedef enum
{
    FMT_STRING = 0,  /*"字符串"*/
    FMT_DATE,  /*"日期"*/
    FMT_TIME,  /*"时间"*/

    FMT_NONE = 0xff
}EN_Data_Format;
extern void Dis_test(void);
extern void DIS_ClrPriority(void);
#endif
