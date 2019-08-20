#ifndef _RTC_H_
#define _RTC_H_

#include "system\sys_def.h"
#include "Driver\MCU\MCU.h"

#define ADC_30C_L (0x1FFFFC92)
#define ADC_30C_H (0x1FFFFC93)

#define MAX_CALI_VALUE  2048


extern INT16U temp_ad_buffer[8];

extern void RTC_CalibImplement (INT8U mode,INT32S value, INT32U interval);
extern INT16S RTC_CalTempVal (void);
extern void RTC_Cali_TempSen_On (INT8U mode);
extern void RTC_Cali_TempSen_Off (void);

#endif





