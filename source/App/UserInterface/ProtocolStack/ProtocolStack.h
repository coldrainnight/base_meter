#ifndef PROTOCOL_STACK_H
#define PROTOCOL_STACK_H

#include "system\Sys_def.h"
#include "Driver\Device\Com.h"


#define CM_BUFF_RX_SIZE 263     /*"cm通信模块通道接收缓冲区"*/	
#define CM_BUFF_TX_SIZE 263     /*"cm通信模块通道发送缓冲区"*/	

#define RS4851_BUFF_RX_SIZE 250     /*"第1路485通道接收缓冲区"*/	
#define RS4851_BUFF_TX_SIZE 250     /*"第1路485通道发送缓冲区"*/	
#define RS4852_BUFF_RX_SIZE 250     /*"第2路485通道接收缓冲区"*/	
#define RS4852_BUFF_TX_SIZE 250     /*"第2路485通道发送缓冲区"*/	

#define IR_BUFF_RX_SIZE 208     /*"红外通道接收缓冲区"*/	
#define IR_BUFF_TX_SIZE 208     /*"红外通道发送缓冲区"*/

#define RX_BUFF_REMAIN_SIZE 8	/*"通信预留缓冲区"*/

#define MASTER_TX_COM_BUF_RX_SZ 263     /*"cm通信模块通道接收缓冲区"*/	


typedef struct 
{
    INT8U com_ID;/*"通信口编号"*/
    INT8U *RxBuffAddr;/*"接收缓冲器地址"*/
    INT8U *TxBuffAddr;/*"发送缓冲器地址"*/
}ST_COMBUFF_NODE;


extern const ST_COMBUFF_NODE ComBuffTable[E_MAX_COM_ID];
extern const INT16U mCommBuffMax[E_MAX_COM_ID];

extern void ProtocolStackTask(void);
extern void ProtocolStackInit(void);



#endif
