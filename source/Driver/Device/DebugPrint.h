
#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H
#include "system\Sys_def.h"

#define DEBUG_OPEN

#ifdef DEBUG_OPEN 

extern void  DB_Printf (const INT8U *Format,...);

INT16U DB_PrintfNByte (INT8U *Format, INT8U *Pdata, INT16U Len);

#endif

#endif
