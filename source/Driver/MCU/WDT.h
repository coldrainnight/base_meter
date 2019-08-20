#ifndef _WDT_H_
#define _WDT_H_
#include "Driver\MCU\MCU.h"


extern void WDT_Init(INT8U Mode);
extern void WDT_FeedWdg(void);

#endif