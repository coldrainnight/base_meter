#ifndef UART_H
#define UART_H

#include "system\Sys_def.h"
#include "INT.h"

typedef enum
{
    E_UART_ID_485 = 0,
    E_UART_ID_SLAVE,/*����о������оͨ��*/
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
        INT8U DataBits:2; /*1:9λ 2:7λ3:8λ ����λ*/
        INT8U Parity:2;   /*0:��У�� 1:��У�� 2:żУ��*/
        INT8U End:1;      /*0:1λֹͣλ 1:2λֹͣλ*/
        INT8U Unused:3;   /*����Ϊ0*/
    }bits;
}UN_UARTFORM;

#define UARTFORM_8DATA_EVEN_1STOP (0x0B)  /*8λ����λ��żУ�飬1λֹͣλ*/

typedef union   
{
    INT8U byte;          
    struct
    {
        INT8U ReceiveMode:1; /*0:���չرգ�1�����տ���*/
        INT8U SendMode:1;    /*0:���͹رգ�1�����Ϳ���*/
        INT8U unuse :6  ;    /*����*/
    }bits;
}UN_TRAN_STA;

#define TRANSTA_SC_RC  (0x00)  /*���ա����;��ر�*/
#define TRANSTA_SO_RC  (0x02)  /*���Ϳ��������չر�*/
#define TRANSTA_SC_RO  (0x01)  /*���͹رգ����տ���*/
#define TRANSTA_SO_RO  (0x03)  /*���ա����;�����*/


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
