#ifndef UART_H
#define UART_H

#include "system\Sys_def.h"
#include "INT.h"

typedef enum
{
    E_UART_ID_485 = 0,
    E_UART_ID_SLAVE,/*计量芯与主控芯通信*/
    E_UART_ID_MASTER_TX,
    //E_UART_ID_IR,
    E_UART_ID_MC,
    E_UART_NUM
}UARTID;

typedef enum 
{
    E_BAUD300= 0,
    E_BAUD600 = 1,
    E_BAUD1200 = 2,
    E_BAUD2400 = 3,
    E_BAUD4800 = 4,
    E_BAUD7200 = 5,
    E_BAUD9600 = 6,
    E_BAUD19200= 7,
    E_BAUD38400= 8,
    E_BAUD57600= 9,
    E_BAUD115200= 10,
    E_BAUD230400= 11,
    E_BAUD460800= 12,
}BAUD;

typedef union  
{
    INT8U byte;          
    struct
    {
        INT8U DataBits:2; /*1:9位 2:7位3:8位 数据位*/
        INT8U Parity:2;   /*0:无校验 1:奇校验 2:偶校验*/
        INT8U End:1;      /*0:1位停止位 1:2位停止位*/
        INT8U Unused:3;   /*传输为0*/
    }bits;
}UN_UARTFORM;

#define UARTFORM_8DATA_EVEN_1STOP (0x0B)  /*8位数据位，偶校验，1位停止位*/

typedef union   
{
    INT8U byte;          
    struct
    {
        INT8U ReceiveMode:1; /*0:接收关闭，1：接收开启*/
        INT8U SendMode:1;    /*0:发送关闭，1：发送开启*/
        INT8U unuse :6  ;    /*保留*/
    }bits;
}UN_TRAN_STA;

#define TRANSTA_SC_RC  (0x00)  /*接收、发送均关闭*/
#define TRANSTA_SO_RC  (0x02)  /*发送开启，接收关闭*/
#define TRANSTA_SC_RO  (0x01)  /*发送关闭，接收开启*/
#define TRANSTA_SO_RO  (0x03)  /*接收、发送均开启*/


typedef struct 
{
    INT8U Baud;       
    UN_UARTFORM  UartForm;
    UN_TRAN_STA  TranSta ;
    ISR_FNCT_VOID SendIsr;
    ISR_FNCT_VOID ReceiveIsr;
}ST_UARTCONFIG;


typedef struct
{
    void (*Rs485Receice)(void);
    void (*Rs485Transmit)(void);
    void (*SlaveReceice)(void);
    void (*SlaveTransmit)(void);
    void (*MasterReceice)(void);
    void (*MasterTransmit)(void);
    void (*McReceice)(void);
    void (*McTransmit)(void);
}ST_UARTISR;

extern INT8U UART_Open(INT8U UartId,ST_UARTCONFIG *pUartConfig);

extern void UART_Close (INT8U UartId);

extern INT8U UART_TranStaCtrl (INT8U UartId, INT8U TRANSTA);

extern void UART_Send1Byte(INT8U UartId,INT8U Byte);

extern INT8U UART_Receive1Byte(INT8U UartId);

extern INT8U UART_PollSend(INT8U UartId,INT8U * Buf, INT16U Len);

extern INT8U UART_PollReceive(INT8U UartId,
                                    INT8U * Buf, 
                                    INT16U Len, 
                                    INT16U MaxDelayMs);

#endif
