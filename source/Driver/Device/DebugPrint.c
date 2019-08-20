/*"***************************************************************************"*/
/*"FileName: DebugPrint.c		                            "*/
/*"Author: 邹杨                                             "*/                
/*"Version : 1.1       Date: 2017.01.09	                    "*/
/*"Description:   调试打印接口实现                       "*/
/*"Function List:                                           "*/
/*   1、实现了标准C库部分printf的功能  		                "*/
/*"  2、存在下列缺陷                 "*/     
/*"        ①、每次调用printf的最大打印字符为 DEBUG_BUF_LEN 字节        "*/
/*"        ②、每次调用DB_PrintfNByte只支持输出一个参变量               "*/
/*"        ③、使用轮训输出的优点是，能打印出实时的状态信息，且实现较为简单
              缺点是会占用较多主循环时间，不过只用来作调试用还行"*/
/*"<FunctionName>		                                    "*/    
/*"<author> <time> <version > <desc>		                "*/
/*"************************************************************************"*/
#include "Driver\device\Debugprint.h"
#include <stdarg.h>
#include <stdio.h>
#include "Driver\MCU\uart.h"
#include "Driver\MCU\GPIO.h"
#include "app\userinterface\ProtocolStack\PS_698\DL_698.h"

#ifdef DEBUG_OPEN 


#define  OUTPUT_UARTID     (E_UART_ID_485)
#define  DEBUG_BUF_LEN     (128)
#define  NEWLINE_NUM (16)


INT8U DebugBuf[DEBUG_BUF_LEN];

void DB_Printf (const INT8U *Format,...);
INT16U DB_PrintfNByte (INT8U *Format, INT8U *Pdata, INT16U Len);


void DB_Printf (const INT8U *Format,...)
{

    INT16U Count;
    va_list ap;
	
    va_start(ap, Format);
    Count = vsprintf((char*)&DebugBuf[0], (char const*)Format, ap);
   
    va_end(ap);

    if(Count > DEBUG_BUF_LEN) /*超出最大缓冲 不打印*/
    {
        return ;
    }

    UART_TranStaCtrl (OUTPUT_UARTID,  TRANSTA_SO_RC);
    INT_Dis(E_INT_ID_UART2);

    UART_PollSend(OUTPUT_UARTID ,DebugBuf, Count);

    DL_698_Init();


}


INT16U DB_PrintfNByte (INT8U *Format, INT8U *Pdata, INT16U Len)
{
    INT16U i;   
    for(i = 0; i < Len; i++)
    {
        DB_Printf(Format, *(Pdata + i));
        if(((i+1)%NEWLINE_NUM) == 0)
        {
            DB_Printf("\n");
        }
    }
    return Len;
}

#endif 

