/*"***********************************************************"*/
/*"FileName: RTC_8025T.h                                      "*/
/*"Author:                                                    "*/
/*"Version : 1.0       Date: 2016.02.17                       "*/
/*"Description: 外置RTC设备驱动头文件，提供模块对外接口                        "*/
/*"Function List:                                             "*/
/*"<FunctionName>                                             "*/
/*"<author> <time> <version > <desc>                          "*/
/*"***********************************************************"*/

#ifndef RTC_8025T_H
#define RTC_8025T_H
#include "System\sys_def.h"

#define   INIT_MODE0_8025T    0  /*"初始化8025T时间寄存器和控制寄存器"*/
#define   INIT_MODE1_8025T    1  /*"初始化8025T控制寄存器"*/
#define   SEC_PULSE_EN        1  /*"使能8025T秒脉冲输出"*/
#define   SEC_PULSE_DIS       0  /*"失能8025T秒脉冲输出"*/
/********************************************************************/

extern volatile INT32U exSecCnt;

extern INT8U EXRTC_Init(INT8U initMode);
extern INT8U EXRTC_ReadTime(INT8U *t);
extern INT8U EXRTC_WriteTime(INT8U  *t);
extern void  close_exrtc_sec(void);

/********************************************************************/

#endif /*" RTC_8025T_H "*/
