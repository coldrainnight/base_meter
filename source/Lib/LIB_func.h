/*"**********************************************************"*/		
/*"**********************************************************"*/		
/*"FileName: LIB_func.h                                      "*/
/*"Author:                                                   "*/ 
/*"Version : 1.0       Date: 2017.11.7	                     "*/
/*"Description: 公用库函数头文件	                         "*/
/*"Function List: 			                                 "*/
/*"<FunctionName>		                                     "*/	
/*"<author> <time> <version > <desc>			             "*/
/*"	                                                         "*/
/*"***********************************************************"*/

#ifndef _LIB_FUNC_H_
#define _LIB_FUNC_H_

#define INCLUDE_SYSDEF    1

#if INCLUDE_SYSDEF
#include "System\Sys_def.h"
#else
#define TRUE      1
#define FALSE     0
#define NULL       (INT8U*)0
#define _SINGLE_PHASE   1
#define INCLUDE_
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
/***************************typedef for data type******************************/
typedef  unsigned char          BOOL;       /*8-bit  oolean or logical  */
typedef  unsigned char          INT8U;      /*8-bit  unsigned integer   */
typedef  signed char            INT8S;      /*8-bit    signed integer   */                            
typedef  unsigned short         INT16U;     /*16-bit unsigned integer   */                       
typedef  signed short           INT16S;     /*16-bit   signed integer   */
typedef  unsigned long          INT32U;     /*32-bit unsigned integer   */                           
typedef  signed long            INT32S;     /*32-bit   signed integer   */                           
typedef  unsigned long long     INT64U;     /*64-bit unsigned integer   */                          
typedef  signed long long       INT64S;     /*64-bit   signed integer   */                           
typedef  float                  FP32;       /*32-bit floating point     */                          
typedef  double                 FP64;       /*64-bit floating point     */

typedef struct
{
  INT8U format;   /*"数据格式"*/
  INT8U unit;     /*"数据单位"*/
  INT8S exp;      /*"数据小数位数"*/
}ST_FMT_ATTR;
#endif
#include "Driver\MCU\CRC.h"

#define  EQUAL   0
#define  BIG  1
#define  SMALL  2
#define  NOT_EQUAL 3

#define DATETIME_BCDFORMAT   1  //定义是否为BCD码时间格式
#define FARCONST_TYPE     0     //是否定义const far远指针

#if  FARCONST_TYPE
typedef const INT8U __far  CONST;
#else
typedef const INT8U   CONST;
#endif


#if   (APP_TIME_FRMT == HEX_FRMT)
 #define LIB_ChkDateTime      LIB_ChkDateTimeHEX
#define  LIB_CalDay           LIB_CalDayHEX
#else
 #define LIB_ChkDateTime      LIB_ChkDateTimeBCD
 #define LIB_CalDay           LIB_CalDayBCD
#endif

typedef struct 
{
    INT8U year;
    INT8U month;
    INT8U day;
    INT8U week;
    INT8U hour;
    INT8U minute;
    INT8U second;
}ST_DATETIME;
extern INT8U LIB_CalWeekBCD (ST_6TIME_FORM *dateTime);
extern BOOL LIB_CheckBcdByte (INT8U data);
extern BOOL LIB_CheckBCD (INT8U *srcData, INT8U dataLen);
extern INT8U LIB_CharToBcd (INT8U srcData);
extern INT8U LIB_BcdToChar (INT8U srcData);
extern INT16U  LIB_ShortToBcd (INT16U srcData);
extern INT16U LIB_BcdToShort(INT8U *srcData);
extern BOOL  LIB_LongToBcd (INT8U *srcData, INT8U *destData);
extern INT32U LIB_BcdToLong (INT8U *srcData);
extern void LIB_HexToBcd (INT8U len, INT8U *srcData, INT8U * destData);
extern void LIB_BcdToHex (INT8U len, INT8U * srcData, INT8U * destData);
extern void LIB_CharToBcdNByte(INT8U *pdata, INT8U len);
extern void LIB_BcdToCharNByte(INT8U *pdata, INT8U len);
extern INT64S  LIB_NbyteIntsToInt64s(INT8U *pDataIn, INT8U byteNum);
extern void  LIB_Int64sToNbyteInts(INT64S DataIn, INT8U *pDataOut, INT8U byteNum);
extern INT8U LIB_BcdByteAdd(INT8U bcdData);
extern INT16U LIB_BcdWordAdd (INT16U bcdData);
extern INT8U LIB_BytesToBcd( INT8U Length,CONST *inpBuf,INT8U *outpBuf);
extern INT32U LIB_AscToHex(CONST *inpBuf,  INT8U length);
extern INT8U  LIB_BcdToAsc( INT8U inBcd, INT8U *outpBuf);
extern INT8U LIB_BcdToStr(CONST *inBcd,  INT8U length, INT8U *outpBuf, INT8U  sign);
extern INT8U LIB_ByteToAsc( INT8U inByte, INT8U*outpBuf);
extern INT8U LIB_BytesToAsc(CONST *inByte,  INT8U length, INT8U *outpBuf, INT8U  sign);


extern void LIB_MemCpy(INT8U *src, INT8U *dest, INT16U len);
extern void LIB_ConstMemCpy(CONST *src, INT8U *dest, INT16U len);
extern INT8S LIB_MemCmp (INT8U *buf1, INT8U *buf2, INT16U len);
extern INT8S LIB_ConstMemCmp (CONST *buf1, CONST *buf2, INT8U len);
extern INT8S LIB_RvsMemCmp (INT8U *buf1, INT8U *buf2, INT8U len);
extern void LIB_MemSet(INT8U setValue, INT8U *dest, INT16U len);
extern void LIB_Reverse (INT8U *src, INT8U *dest, INT8U len);
extern void LIB_RvsSelf(INT8U *srcData, INT8U len);
extern INT16U LIB_HToN16 (INT16U data);
extern INT32U LIB_HToN32 (INT32U data);
extern BOOL LIB_SwapArray(INT8U*srcData1,INT8U*srcData2,INT16U len);
extern INT8U LIB_ChkValue(INT8U * srcData, INT8U len, INT8U value);
extern void LIB_Sort(INT8U *srcData, INT8U len);
extern INT32U  LIB_GetMax(INT32U x, INT32U y, INT32U z);


extern INT32U LIB_DToStdDate (ST_6TIME_FORM *dateTime);
extern INT32U LIB_HToStdDate (ST_6TIME_FORM *dateTime);
extern INT32U LIB_MToStdDate (ST_6TIME_FORM *dateTime);
extern INT32U LIB_SToStdDate (ST_6TIME_FORM *dateTime);
extern INT32S LIB_CmpTime (ST_6TIME_FORM *t1,ST_6TIME_FORM *t2);
extern INT8U LIB_CalWeek (ST_6TIME_FORM *dateTime);
extern INT8U LIB_ChkDateTimeBCD(ST_6TIME_FORM *dateTime);
extern INT8U LIB_ChkDateTimeHEX(ST_6TIME_FORM *dateTime);
extern INT8U LIB_CalDayBCD(INT8U year,INT8U month);
extern INT8U LIB_CalDayHEX(INT8U year,INT8U month);
extern void LIB_TIDToDateTime( INT32U tid,ST_DATETIME *inpDateTime) ;
extern INT8U LIB_IsLeapYear(INT8U year);
	

extern INT32U LIB_ABS(INT32S data);
extern INT8U LIB_DataCheckSum(INT8U *inpbuf, INT16U  length,INT8U init);
extern INT8U LIB_CalBCC(INT8U *inpbuf, INT32U length);
extern INT8U LIB_ChkDataValid(INT8U *pData, INT16U DataLen, CRC_DATA_TP CmpCrc);
extern INT32U LIB_Pow10(INT8U n);
#endif
