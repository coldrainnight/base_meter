#ifndef   _COM_H
#define  _COM_H
#include "System\sys_def.h"
#include "Driver\MCU\Uart.h"

#define NO_USED 0xff  /*"�˿�/����δʹ��״̬"*/

/*" ����comID��Ӧ��uart�ڵ�ʹ��״̬"*/
#define UART_IDLE     0x00    /*" ����"*/
#define UART_WAIT     0x01   /*" �ȴ�"*/
#define UART_BUSY     0x02   /*" ��æ"*/

#define PLC_MODE	  0x00   /*"PLCģ��"*/
#define RF_MODE	  	  0x01	 /*"RFģ��"*/

#define UART_TIMEOUT_TIME (10*60L)/*" ��ʱʱ�䣬��λs"*/
/*"com�����ƶ���"*/
typedef enum 
{
    E_COM_RS485_1 = 0x00,  /*" ��һ·485 "*/
    #ifdef THREE_PHASE    
    E_COM_RS485_2,   /*" �ڶ�·485 "*/
    #endif
    E_COM_SLAVE,   /*" ����ͼ�����оƬͨ��"*/
    E_MAX_COM_ID
}EN_COM_ID;

typedef struct 
{
    INT8U com_ID;/*"ͨ�ſڱ��"*/
    INT8U uart_ID;/*"���ڱ��"*/
    //INT8U uart_Rx_ID;/*"���ڽ����жϱ��"*/
    //INT8U uart_Tx_ID;/*"���ڷ����жϱ��"*/
    void  (*Com_Rx)(void);/*" �ж�ע�ắ��ԭ��"*/
    void  (*Com_Tx)(void);/*" �ж�ע�ắ��ԭ��"*/
}ST_COM_UART_CFG;

typedef struct 
{
    INT8U comChannel_ID;/*"ͨ�ſڱ��"*/
    void 	(*Uart_Rx)(INT8U channel,INT8U ReceiveByte);/*"�������ݻص�����"*/
}ST_COM_RX_NODE;

typedef struct 
{
    INT8U comChannel_ID;   /*"ͨ�ſڱ��"*/
    INT8U status;           /*"״̬"*/
    INT16U len_Tx;           /*"���͵�ʣ�����ݳ���"*/
    INT8U *pdata_Tx;     /*"��������ǰָ��λ��"*/
    void (*UartTx_CallBackFun)(INT8U channelID);/*"���ͽ�����ص�����"*/
    }ST_COM_TX_NODE;

typedef struct 
{
    INT8U uart_ID;/*"���ڱ��"*/
    INT8U com_ID;/*"com�ڱ��"*/
    INT8U uart_Bps;/*"����bps"*/
    INT8U uart_Form;/*"��������λ��Ч��λ��ֹͣλ����"*/
}ST_UART_PARAM;

/*"��ģ�麯������"*/
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

