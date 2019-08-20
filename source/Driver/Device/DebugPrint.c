/*"***************************************************************************"*/
/*"FileName: DebugPrint.c		                            "*/
/*"Author: ����                                             "*/                
/*"Version : 1.1       Date: 2017.01.09	                    "*/
/*"Description:   ���Դ�ӡ�ӿ�ʵ��                       "*/
/*"Function List:                                           "*/
/*   1��ʵ���˱�׼C�ⲿ��printf�Ĺ���  		                "*/
/*"  2����������ȱ��                 "*/     
/*"        �١�ÿ�ε���printf������ӡ�ַ�Ϊ DEBUG_BUF_LEN �ֽ�        "*/
/*"        �ڡ�ÿ�ε���DB_PrintfNByteֻ֧�����һ���α���               "*/
/*"        �ۡ�ʹ����ѵ������ŵ��ǣ��ܴ�ӡ��ʵʱ��״̬��Ϣ����ʵ�ֽ�Ϊ��
              ȱ���ǻ�ռ�ý϶���ѭ��ʱ�䣬����ֻ�����������û���"*/
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

    if(Count > DEBUG_BUF_LEN) /*������󻺳� ����ӡ*/
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

