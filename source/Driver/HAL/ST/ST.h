/*"************************************************************"*/
/*"FileName: ST.h"*/
/*"Author:     Version : 1.0       Date: "*/
/*" Description: ��ʱ�Ӷ���ӿں���������ģ��������"*/
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
    E_RTC_SCR_NUM     /*"Ӳ��RTCʱ��Դ��"*/
}EN_RTC_SCR_ID;

/*"�궨��RTC״̬"*/
#define  RTC_OK       1   /*"RTC���ݺϷ�"*/
#define  RTC_ERR      2   /*"RTC���ݲ��Ϸ���RTC���ڣ�"*/
#define  RTC_ABSENT   3   /*"RTC������"*/

#define   SEC_PULSE_IN   TRUE   /*"select the RTC second pulse in or not in"*/

/*"����RTCʱ��Դ����ӿں���ָ������"*/
typedef INT8U  (* RTC_init)(INT8U mode);
typedef INT8U  (* RTC_readTime)(INT8U *t);
typedef INT8U  (* RTC_writeTime)(INT8U *t);
typedef void   (* RTC_secPulseEn)(void);
typedef void   (* RTC_secPulseDis)(void);

typedef struct
{
    RTC_init  init;               /*"Ӳ��RTC��ʼ���ӿ�"*/
    RTC_readTime readTime;        /*"Ӳ��RTC��ʱ��ӿ�"*/
    RTC_writeTime writeTime;      /*"Ӳ��RTCдʱ��ӿ�"*/
#if (SEC_PULSE_IN == TRUE)
    RTC_secPulseEn secPulseEn;    /*"Ӳ��RTCʹ��������ӿ�"*/
    RTC_secPulseDis secPulseDis;  /*"Ӳ��RTCʧ��������ӿ�"*/
    volatile INT32U *pSecAddCnt;           /*"Ӳ��RTC���ۼ�ֵ"*/
#endif
}ST_RTC_INTERFACE_LIST;

/*"***************ʱ��Ƭ��־*****************
 ************ ���ڸ�ģ���ʱ����***********"*/
typedef union _TIME_SLICE_FLG
{
    struct
    {
        INT8U flg_1s:7;       /*"1��ʱ��Ƭ��־"*/
        INT8U flg_1min:1;     /*"1����ʱ��Ƭ��־"*/
    }bit;
    INT8U byte;
}ST_TIME_SLICE_FLG;

extern ST_TIME_SLICE_FLG mTimeSliceFlg;/*"ʱ��Ƭ��־"*/

extern INT8U ST_Init(INT8U Mode, EN_RTC_SCR_ID  ClkScr);
extern INT8U ST_RefSysClk(INT8U *pTime, INT8U mode);
extern void  ST_GetSysClkPd(INT8U *ptime);
extern void  ST_GetSysClk(INT8U *pTime);
extern void ST_SynStClkToSysClk(INT8U *ptime);
extern INT8U ST_SynSysClk(void);
extern void ST_secPulseChk(void);
extern INT8U LIB_ChkDateTime(ST_6TIME_FORM *dateTime);
#endif
