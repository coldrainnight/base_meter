#ifndef AdcDrv_H
#define AdcDrv_H
#include "system\Sys_def.h"



#define AC_ADCID_BAT          0      /*"Battery sampling SA-ADC"*/
#define AC_ADCID_TEMP         1      /*"Temperature sampling SA-ADC"*/
#define AC_ADCID_VDD          2      /*"µÁ‘¥µÁ—π SA-ADC"*/

#define AC_COMPID_BAT          0      /*"Battery Comparator"*/


extern void AC_AdcInit(INT8U mode,INT8U  AdcId);
extern INT32U AC_AdcEnable(INT8U AdcId);
extern void AC_AdcDisable(INT8U AdcId);
extern INT16U Cal_VolVal(INT8U AdcId);


extern void AC_CompInit(INT8U mode,INT8U CompId);
extern INT8U  AC_CompRet(INT8U CompId);
extern void  AC_CompDisable(INT8U CompId);




#endif
