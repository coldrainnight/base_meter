#ifndef RTC_H
#define RTC_H
#include "system\Sys_def.h"

#define   INIT_MODE0_INRTC    1  /*"正常初始化"*/
#define   INIT_MODE1_INRTC    0  /*"出错等异常状态初始化"*/


#define RTC_RD_CNT  3  /*"?áRTC?êDí???′μ?′?êy"*/
#define RTC_WRT_CNT 3  /*"D′RTC?êDí???′μ?′?êy"*/

#define DATE_NUM 7
#define WEEK     6
#define YEAR     0
#define MON      1
#define DAY      2
#define HOUR     3
#define MINU     4
#define SEC      5

typedef struct 
{
    INT8U sec;
    INT8U min;
    INT8U hour;
    INT8U day;
    INT8U week;
    INT8U month;
    INT16U year;
}rtc_Calendar;



extern volatile INT32U  gsecCnt;
extern volatile INT8U gSecFlag ;

extern INT8U INRTC_Init(INT8U Mode);
extern INT8U INRTC_WriteTime(INT8U *t);
extern INT8U INRTC_ReadTime(INT8U *t);
extern void RTC_Enable(void);
extern void RTC_Disable(void);
extern void RTC_OpenSec(void);
extern void RTC_CloseSec(void);
#endif
