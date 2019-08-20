/*"***********************************************************"*/
/*"FileName: RTC_8025T.h                                      "*/
/*"Author:                                                    "*/
/*"Version : 1.0       Date: 2016.02.17                       "*/
/*"Description: ����RTC�豸����ͷ�ļ����ṩģ�����ӿ�                        "*/
/*"Function List:                                             "*/
/*"<FunctionName>                                             "*/
/*"<author> <time> <version > <desc>                          "*/
/*"***********************************************************"*/

#ifndef RTC_8025T_H
#define RTC_8025T_H
#include "System\sys_def.h"

#define   INIT_MODE0_8025T    0  /*"��ʼ��8025Tʱ��Ĵ����Ϳ��ƼĴ���"*/
#define   INIT_MODE1_8025T    1  /*"��ʼ��8025T���ƼĴ���"*/
#define   SEC_PULSE_EN        1  /*"ʹ��8025T���������"*/
#define   SEC_PULSE_DIS       0  /*"ʧ��8025T���������"*/
/********************************************************************/

extern volatile INT32U exSecCnt;

extern INT8U EXRTC_Init(INT8U initMode);
extern INT8U EXRTC_ReadTime(INT8U *t);
extern INT8U EXRTC_WriteTime(INT8U  *t);
extern void  close_exrtc_sec(void);

/********************************************************************/

#endif /*" RTC_8025T_H "*/
