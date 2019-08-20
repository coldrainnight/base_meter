/*"************************************************************"*/
/*"FileName: SC.c"*/
/*"Author:   Version : 1.0       Date: 2014-03-24"*/
/*" Description: 系统时钟处理"*/
/*" Function List:  主要函数及其功能"*/
/*" 1. void main(void)"*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/
#ifndef SC_H
#define SC_H

#include "PlatForm.h"

#define _SC_DEBUG
extern ST_7TIME_FORM stSysTime;

extern void SC_SysClkInit(INT8U Mode);
extern void SC_main (void);
extern void SC_Task1SecPD(void);
extern INT8S LRM_JustTime(void *pdin, void *pdout); 
#endif



