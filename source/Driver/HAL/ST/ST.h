/*"************************************************************"*/
/*"FileName: ST.h"*/
/*"Author:     Version : 1.0       Date: "*/
/*" Description: 软时钟对外接口函数声明，模块名定义"*/
/*" Function List:  "*/
/*" 1. void main(void)"*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/
#ifndef _ST_H
#define _ST_H

#include "System\Sys_def.h"
#if APP_DEMO
#include "System\Task\task_test.h"
#else
#include "System\Task\task.h"
#endif

typedef enum
{
    E_EX_RTC = 0,
    E_IN_RTC,
    E_RTC_SCR_NUM     /*"硬件RTC时钟源数"*/
}EN_RTC_SCR_ID;

/*"宏定义RTC状态"*/
#define  RTC_OK       1   /*"RTC数据合法"*/
#define  RTC_ERR      2   /*"RTC数据不合法（RTC存在）"*/
#define  RTC_ABSENT   3   /*"RTC不存在"*/

#define   SEC_PULSE_IN   TRUE   /*"select the RTC second pulse in or not in"*/

/*"定义RTC时钟源对外接口函数指针类型"*/
typedef INT8U  (* RTC_init)(INT8U mode);
typedef INT8U  (* RTC_readTime)(INT8U *t);
typedef INT8U  (* RTC_writeTime)(INT8U *t);
typedef void   (* RTC_secPulseEn)(void);
typedef void   (* RTC_secPulseDis)(void);

typedef struct
{
    RTC_init  init;               /*"硬件RTC初始化接口"*/
    RTC_readTime readTime;        /*"硬件RTC读时间接口"*/
    RTC_writeTime writeTime;      /*"硬件RTC写时间接口"*/
#if (SEC_PULSE_IN == TRUE)
    RTC_secPulseEn secPulseEn;    /*"硬件RTC使能秒脉冲接口"*/
    RTC_secPulseDis secPulseDis;  /*"硬件RTC失能秒脉冲接口"*/
    volatile INT32U *pSecAddCnt;           /*"硬件RTC秒累计值"*/
#endif
}ST_RTC_INTERFACE_LIST;

/*"***************时间片标志*****************
 ************ 用于各模块分时调用***********"*/
typedef union _TIME_SLICE_FLG
{
    struct
    {
        INT8U flg_1s:7;       /*"1秒时间片标志"*/
        INT8U flg_1min:1;     /*"1分钟时间片标志"*/
    }bit;
    INT8U byte;
}ST_TIME_SLICE_FLG;

extern ST_TIME_SLICE_FLG mTimeSliceFlg;/*"时间片标志"*/

extern INT8U ST_Init(INT8U Mode, EN_RTC_SCR_ID  ClkScr);
extern INT8U ST_RefSysClk(INT8U *pTime, INT8U mode);
extern void  ST_GetSysClkPd(INT8U *ptime);
extern void  ST_GetSysClk(INT8U *pTime);
extern void ST_SynStClkToSysClk(INT8U *ptime);
extern INT8U ST_SynSysClk(void);
extern void ST_secPulseChk(void);
extern INT8U LIB_ChkDateTime(ST_6TIME_FORM *dateTime);
#endif
