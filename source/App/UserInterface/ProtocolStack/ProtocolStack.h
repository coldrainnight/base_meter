#ifndef PROTOCOL_STACK_H
#define PROTOCOL_STACK_H

#include "system\Sys_def.h"
#include "Driver\Device\Com.h"


#define CM_BUFF_RX_SIZE 263     /*"cmͨ��ģ��ͨ�����ջ�����"*/	
#define CM_BUFF_TX_SIZE 263     /*"cmͨ��ģ��ͨ�����ͻ�����"*/	

#define RS4851_BUFF_RX_SIZE 250     /*"��1·485ͨ�����ջ�����"*/	
#define RS4851_BUFF_TX_SIZE 250     /*"��1·485ͨ�����ͻ�����"*/	
#define RS4852_BUFF_RX_SIZE 250     /*"��2·485ͨ�����ջ�����"*/	
#define RS4852_BUFF_TX_SIZE 250     /*"��2·485ͨ�����ͻ�����"*/	

#define IR_BUFF_RX_SIZE 208     /*"����ͨ�����ջ�����"*/	
#define IR_BUFF_TX_SIZE 208     /*"����ͨ�����ͻ�����"*/

#define RX_BUFF_REMAIN_SIZE 8	/*"ͨ��Ԥ��������"*/

#define MASTER_TX_COM_BUF_RX_SZ 263     /*"cmͨ��ģ��ͨ�����ջ�����"*/	


typedef struct 
{
    INT8U com_ID;/*"ͨ�ſڱ��"*/
    INT8U *RxBuffAddr;/*"���ջ�������ַ"*/
    INT8U *TxBuffAddr;/*"���ͻ�������ַ"*/
}ST_COMBUFF_NODE;


extern const ST_COMBUFF_NODE ComBuffTable[E_MAX_COM_ID];
extern const INT16U mCommBuffMax[E_MAX_COM_ID];

extern void ProtocolStackTask(void);
extern void ProtocolStackInit(void);



#endif
