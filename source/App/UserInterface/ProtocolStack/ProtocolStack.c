#include "ProtocolStack.h"
#include "system\Task\Task.h"
#include "app\userinterface\ProtocolStack\PS_698\DL_698.h"
#include "app\userinterface\ProtocolStack\PS_698\AL_698.h"
//#include "Driver\Device\pkt_proc.h"

static INT8U gCm_Buff_Rx[CM_BUFF_RX_SIZE];/*"cm通信模块通道接收缓冲区"*/	
static INT8U gCm_Buff_Tx[CM_BUFF_TX_SIZE];/*"cm通信模块通道发送缓冲区"*/	
static INT8U gRS4851_Buff_Rx[RS4851_BUFF_RX_SIZE];/*"第1路485通道缓冲区"*/	
#ifdef THREE_PHASE
static INT8U gRS4852_Buff_Rx[RS4852_BUFF_RX_SIZE];/*"第2路485通道缓冲区"*/	
#endif


/*"端口缓冲区配置表"*/
const ST_COMBUFF_NODE ComBuffTable[E_MAX_COM_ID]=
{
    {E_COM_RS485_1, gRS4851_Buff_Rx,gRS4851_Buff_Rx},
#ifdef THREE_PHASE
    {E_COM_RS485_2, gRS4852_Buff_Rx,gRS4852_Buff_Rx},
#endif
    {E_COM_SLAVE, gCm_Buff_Rx,gCm_Buff_Tx},
};

const INT16U mCommBuffMax[E_MAX_COM_ID] = 
{
    RS4851_BUFF_RX_SIZE,
#ifdef THREE_PHASE
    RS4852_BUFF_RX_SIZE,
#endif
    CM_BUFF_RX_SIZE,
};

extern void Master_TX_Com_Init(INT8U dev);
void ProtocolStackInit(void)
{
    COM_VarInit();

    DL_698_Init();
    AL_698_Init();

    Master_TX_Com_Init(0);
}

extern void Host_Commu_Sec_Proc(void);
extern void Master_TX_Com_Task(void);
void ProtocolStackTask(void)
{
    DL_698_Main();
    AL_698_Main();
    Master_TX_Com_Task();

    if(TRUE == Task_ChkTsk_1s (MAINTASK_CM))
    {
        Host_Commu_Sec_Proc();
        COM_Monitor_1Sec();
        AL_698_1Sec();

    }

    if(TRUE == Task_ChkTsk_1min(MAINTASK_CM))
    {
    }

}




