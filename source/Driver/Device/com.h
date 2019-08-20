#ifndef   _COM_H
#define  _COM_H
#include "System\sys_def.h"
#include "Driver\MCU\Uart.h"

#define NO_USED 0xff  /*"端口/数据未使用状态"*/

/*" 定义comID对应的uart口的使用状态"*/
#define UART_IDLE     0x00    /*" 空闲"*/
#define UART_WAIT     0x01   /*" 等待"*/
#define UART_BUSY     0x02   /*" 正忙"*/

#define PLC_MODE	  0x00   /*"PLC模块"*/
#define RF_MODE	  	  0x01	 /*"RF模块"*/

#define UART_TIMEOUT_TIME (10*60L)/*" 超时时间，单位s"*/
/*"com口名称定义"*/
typedef enum 
{
    E_COM_RS485_1 = 0x00,  /*" 第一路485 "*/
    #ifdef THREE_PHASE    
    E_COM_RS485_2,   /*" 第二路485 "*/
    #endif
    E_COM_SLAVE,   /*" 主板和计量板芯片通信"*/
    E_MAX_COM_ID
}EN_COM_ID;

typedef struct 
{
    INT8U com_ID;/*"通信口编号"*/
    INT8U uart_ID;/*"串口编号"*/
    //INT8U uart_Rx_ID;/*"串口接收中断编号"*/
    //INT8U uart_Tx_ID;/*"串口发送中断编号"*/
    void  (*Com_Rx)(void);/*" 中断注册函数原型"*/
    void  (*Com_Tx)(void);/*" 中断注册函数原型"*/
}ST_COM_UART_CFG;

typedef struct 
{
    INT8U comChannel_ID;/*"通信口编号"*/
    void 	(*Uart_Rx)(INT8U channel,INT8U ReceiveByte);/*"接收数据回调函数"*/
}ST_COM_RX_NODE;

typedef struct 
{
    INT8U comChannel_ID;   /*"通信口编号"*/
    INT8U status;           /*"状态"*/
    INT16U len_Tx;           /*"发送的剩余数据长度"*/
    INT8U *pdata_Tx;     /*"发送区当前指针位置"*/
    void (*UartTx_CallBackFun)(INT8U channelID);/*"发送结束后回调函数"*/
    }ST_COM_TX_NODE;

typedef struct 
{
    INT8U uart_ID;/*"串口编号"*/
    INT8U com_ID;/*"com口编号"*/
    INT8U uart_Bps;/*"串口bps"*/
    INT8U uart_Form;/*"串口数据位、效验位、停止位配置"*/
}ST_UART_PARAM;

/*"本模块函数声明"*/
extern void COM_VarInit(void);
extern void COM_Init (INT8U ComId,INT8U BitRate,UN_UARTFORM UartForm,void (*COM_Rx_Callback)(INT8U channel,INT8U ReceiveByte),void (*COM_Tx_Callback)(INT8U channelID));
extern void COM_Rlease(INT8U ComId);
extern void COM_RxTx_Ctrl(INT8U ComId, INT8U RxTxEnable);
extern void COM_Send(INT8U UartId);
extern void COM_StartSend(INT8U ComId,INT8U *pSendData,INT16U dataLenth);
extern void COM_Monitor_1Sec (void);
extern void CMD_Init(INT8U mode);
extern void CMD_Eventout_On (void);
extern void CMD_Eventout_Off (void);
#endif

