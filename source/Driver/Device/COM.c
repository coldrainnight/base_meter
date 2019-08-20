/*"***********************************************************"*/		
/*"�ļ���: com.c	"*/	
/*"����: ����Ȩ     "*/ 
/*"�汾: 1.0       ����: 2014.03.11"*/	
/*"�汾: 2.0       ����: 2014.08.25"*/	
/*"�汾: 3.0       ����: 2017.01.9"*/	
/*"�ۺϹ��ڡ������Ƶ������������ͨ����������ǰ����ȣ���������Ѵ���ͬ"*/	
/*"�ļ�����: ͨ���豸��:485�����⡢�ز���ͨ�Žӿ��豸�ӿ�ʵ��"*/
/*"�����б�: "*/		
/*"<������>	"*/	
/*"***********************************************************"*/		
#include "System\sys_def.h"
#include "Lib\LIB_func.h"
#include "Driver\MCU\Uart.h"
#include "Driver\MCU\INT.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "com.h"
#include "Driver\Device\spi_slave.h"

void COM_458_1_RX(void);
void COM_458_2_RX(void);
void COM_IR_RX(void);
void COM_SLAVE_RX(void);
void COM_458_1_TX(void);
void COM_458_2_TX(void);
void COM_IR_TX(void);
void COM_SLAVE_TX(void);
/*"�����б�"*/	
static ST_COM_RX_NODE mstComRxList[E_MAX_COM_ID];
/*"�����б�"*/	
static ST_COM_TX_NODE mstComTxList[E_MAX_COM_ID];
/*"com��uart�ڵ���Թ�ϵ"*/	
/*"Ϊ�򻯲��ҹ�ϵ��com�ڶ�Ӧ�������ΪEN_COM_ID�е����ݣ���˳���ܸı�"*/	
const ST_COM_UART_CFG mstComUartConfig[E_MAX_COM_ID]=
{
    {E_COM_RS485_1,E_UART_ID_485,COM_458_1_RX,COM_458_1_TX},
    {E_COM_SLAVE,E_UART_ID_SLAVE,COM_SLAVE_RX,COM_SLAVE_TX},
};
/*"�������ò����б�"*/
static ST_UART_PARAM mstUartParam[E_UART_NUM];
/*"uart�ڵ�״̬�б�"*/	
static INT8U mUartTxStatus[E_UART_NUM];
/*"uart�ڵĳ�ʱ�б��б�"*/	
static INT16U mUart_TimeOut[E_UART_NUM];


void COM_StartTX(INT8U ComId);
/*"*****************************************************************************"*/
/*"  Function:       COM_VarInit"*/
/*"  Description:   ģ����ر�����ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_VarInit(void)
{
    INT8U uni;
/*"�б��ʼ��"*/	
    LIB_MemSet(0,(INT8U *)&mstComRxList[0],sizeof(ST_COM_RX_NODE));
    LIB_MemSet(0,(INT8U *)&mstComTxList[0],sizeof(ST_COM_TX_NODE));
    LIB_MemSet(0,(INT8U *)&mstUartParam[0],sizeof(ST_UART_PARAM));
    LIB_MemSet(0,(INT8U *)&mUartTxStatus[0],sizeof(INT8U)*E_UART_NUM);
    /*"�������ݳ�ʼ��"*/	
    for(uni=0;uni<E_MAX_COM_ID;uni++)
    {
        mstComRxList[uni].comChannel_ID=NO_USED;
        mstComTxList[uni].comChannel_ID=NO_USED;
    }
    for(uni=0;uni<E_UART_NUM;uni++)
    {
        mstUartParam[uni].uart_ID=NO_USED;
        mUart_TimeOut[uni]=UART_TIMEOUT_TIME;
    } 
}
/*"*****************************************************************************"*/
/*"  Function:       COM_Init"*/
/*"  Description:   ĳ·com�ڳ�ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_Init (INT8U ComId,INT8U BitRate,UN_UARTFORM UartForm,void (*COM_Rx_Callback)(INT8U channel,INT8U ReceiveByte),void (*COM_Tx_Callback)(INT8U channelID))
{
    INT8U channelTmp;
    ST_UARTCONFIG UartConf;
    /*"�Ե�·com����"*/	
    if(ComId>=E_MAX_COM_ID) 
    {
        return;
    }
    mstComRxList[ComId].comChannel_ID=ComId;
    mstComRxList[ComId].Uart_Rx=COM_Rx_Callback;
    mstComTxList[ComId].comChannel_ID=ComId;
    mstComTxList[ComId].UartTx_CallBackFun=COM_Tx_Callback;
    mstComTxList[ComId].status=UART_IDLE;
    if(ComId != E_COM_SLAVE)
    {
        channelTmp=mstComUartConfig[ComId].uart_ID;
        mUartTxStatus[channelTmp]=UART_IDLE;
        mUart_TimeOut[channelTmp]=UART_TIMEOUT_TIME;
        /*"�������"*/
        mstUartParam[channelTmp].uart_ID=channelTmp;
        mstUartParam[channelTmp].com_ID=ComId;
        mstUartParam[channelTmp].uart_Bps=BitRate;
        mstUartParam[channelTmp].uart_Form=UartForm.byte; 
    }
    /*"����MCU"*/
    UartConf.Baud = BitRate;
    UartConf.UartForm.byte = UartForm.byte;
    UartConf.TranSta.byte = TRANSTA_SC_RO;

    UartConf.SendIsr = mstComUartConfig[ComId].Com_Tx;
    UartConf.ReceiveIsr = mstComUartConfig[ComId].Com_Rx;

    if(ComId == E_COM_SLAVE)
    {
        //Slave_Spi_Init();
    }
    else
    {
        UART_Open(mstComUartConfig[ComId].uart_ID,&UartConf);
    }
  

}
/*"*****************************************************************************"*/
/*"  Function:       COM_Rlease"*/
/*"  Description:   ĳ·com���ͷ�"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_Rlease(INT8U ComId)
{
    if(ComId>=E_MAX_COM_ID) 
    {
        return;
    }
    mstComRxList[mstComUartConfig[ComId].uart_ID].comChannel_ID=NO_USED;
    mstComTxList[mstComUartConfig[ComId].uart_ID].comChannel_ID=NO_USED;
}
/*"*****************************************************************************"*/
/*"  Function:       COM_RxTx_Ctrl"*/
/*"  Description:   ĳ·com���շ�״̬�л�"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_RxTx_Ctrl(INT8U ComId, INT8U RxTxEnable)
{
    if(ComId>=E_MAX_COM_ID) 
    {
        return;
    }
    UART_TranStaCtrl(mstComUartConfig[ComId].uart_ID,RxTxEnable);
}
/*"*****************************************************************************"*/
/*"  Function:       COM_Receive"*/
/*"  Description:   ĳ·uart�ڽ������ݣ����ж�Ӧcom���ݵĽ���"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_Receive(INT8U UartId)
{
    INT8U num,receiveData;
    /*"ȡ����"*/	
    receiveData=UART_Receive1Byte(UartId);

    /*"�����ݴ����Ӧcom�ڣ���ʵ�ֵ�uart�ڶ�Ӧ���com�豸�Ļ���"*/	
    for(num=0;num<E_MAX_COM_ID;num++)/*"���Ҷ�Ӧcom�ڱ��"*/
    {
        if(UartId==mstComUartConfig[num].uart_ID)
        {
            mstComRxList[mstComUartConfig[num].com_ID].Uart_Rx(num,receiveData);
            mUart_TimeOut[UartId]=UART_TIMEOUT_TIME;
        }
    }
}
/*"���ڽ����ж�ע�ắ��"*/	
void COM_458_1_RX(void)
{
    COM_Receive(E_UART_ID_485);
}
void COM_458_2_RX(void)
{
    //COM_Receive(E_UART_ID_7816);
    COM_Receive(E_UART_ID_485);
}

void COM_SLAVE_RX(void)
{
   COM_Receive(E_UART_ID_SLAVE);
}
/*"*****************************************************************************"*/
/*"  Function:       COM_Send"*/
/*"  Description:   ĳ·uart�ڷ������ݣ�ʵ�ֶ�Ӧcom���ݵķ���"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_Send(INT8U UartId)
{
    INT8U num;
    
  for(num=0;num<E_MAX_COM_ID;num++)
    {
        if((UartId==mstComUartConfig[num].uart_ID)&&(mstComTxList[mstComUartConfig[num].com_ID].status==UART_BUSY))/*"���Ҷ�Ӧcom�ڱ�š�״̬"*/
        {
            if(mstComTxList[mstComUartConfig[num].com_ID].len_Tx==0)/*"�Ƿ�Ϊ���һ���ֽڷ������"*/
            { 
                if(mstComTxList[mstComUartConfig[num].com_ID].UartTx_CallBackFun!= NULL)
                {
                    if(mstComUartConfig[num].com_ID!=E_COM_SLAVE)/*CM���Ϊ��˫��*/  
                    {
                      COM_RxTx_Ctrl(mstComUartConfig[num].com_ID,TRANSTA_SC_RO);
                    }
                    else
                    {
                      COM_RxTx_Ctrl(mstComUartConfig[num].com_ID,TRANSTA_SO_RO);
                    }                 
                    mstComTxList[mstComUartConfig[num].com_ID].UartTx_CallBackFun(mstComUartConfig[num].com_ID); 

                }
                else
                { 
                }
				mUart_TimeOut[mstComUartConfig[num].uart_ID] = UART_TIMEOUT_TIME;
                mstComTxList[mstComUartConfig[num].com_ID].status=UART_IDLE;
                mUartTxStatus[UartId]=UART_IDLE;
            }
            else/*"��Ϊ���һ���ֽ�����һ���ֽ�"*/
            {
                UART_Send1Byte(UartId,*mstComTxList[mstComUartConfig[num].com_ID].pdata_Tx);
                mstComTxList[mstComUartConfig[num].com_ID].len_Tx--;
                mstComTxList[mstComUartConfig[num].com_ID].pdata_Tx++;
            }
            break;
         }
    }
}
/*"���ڷ����ж�ע�ắ��"*/	
void COM_458_1_TX(void)
{
    COM_Send(E_UART_ID_485);
}
void COM_458_2_TX(void)
{
    //COM_Send(E_UART_ID_7816);
    COM_Send(E_UART_ID_485);
}

void COM_SLAVE_TX(void)
{
    COM_Send(E_UART_ID_SLAVE);
}
/*"*****************************************************************************"*/
/*"  Function:       COM_StartSend"*/
/*"  Description:   ����ĳ·com���ݷ���"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
void COM_StartSend(INT8U ComId,INT8U *pSendData,INT16U dataLenth)
{
    if((ComId>=E_MAX_COM_ID)||(mstComTxList[ComId].status==UART_BUSY)) 
    {
        return;
    }
    mstComTxList[ComId].len_Tx=dataLenth;
    mstComTxList[ComId].pdata_Tx=pSendData;
    mstComTxList[ComId].status=UART_WAIT;
    /*"�ж��Ƿ���Ҫ����������������"*/
    if(mUartTxStatus[mstComUartConfig[ComId].uart_ID]==UART_IDLE)
    {
        COM_StartTX(ComId);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       COM_Monitor_1Sec"*/
/*"  Description:   �ȴ����͵�com�ڵļ�������������Գ�ʱ���"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:       COM_MonitorҪ��1s����һ�� code size  byte"*/
/*"*****************************************************************************"*/
void COM_Monitor_1Sec (void)
{
    INT8U num,comNum;
    ST_UARTCONFIG UartConf;
    /*"��ѯ���ڵȴ�״̬��com���Ƿ��������������������"*/
    for(comNum=0;comNum<E_MAX_COM_ID;comNum++)
    {
        if((mstComTxList[comNum].status==UART_WAIT)&&(mUartTxStatus[mstComUartConfig[comNum].uart_ID]==UART_IDLE))
        {
            COM_StartTX(comNum);
        }
    }
    /*"��ѯuart����ʱ���Ƿ�����Ҫ��"*/
    for(num=0;num<E_UART_NUM;num++)
    {
        if(mUart_TimeOut[num]>0)
        {
            mUart_TimeOut[num]--;
        }
        else
        {
            for(comNum=0;comNum<E_MAX_COM_ID;comNum++)
            {
                if(mstComUartConfig[comNum].uart_ID==num)
                {
                    mstComTxList[comNum].status=UART_IDLE;
                }
            }    
            mUart_TimeOut[num]=UART_TIMEOUT_TIME;
            mUartTxStatus[num]=UART_IDLE;
            /*"�˴����ó�ʼ��uart��Ϊ���һ�α��������"*/
            UartConf.Baud = mstUartParam[num].uart_Bps;
            UartConf.UartForm.byte = mstUartParam[num].uart_Form;
            UartConf.TranSta.byte = TRANSTA_SC_RO;
            UartConf.SendIsr = mstComUartConfig[mstUartParam[num].com_ID].Com_Tx;
            UartConf.ReceiveIsr = mstComUartConfig[mstUartParam[num].com_ID].Com_Rx;
            UART_Open(mstUartParam[num].uart_ID,&UartConf);

        }
    }
    
}
 /*"�������͵�һ���ֽڵ�����"*/
void COM_StartTX(INT8U ComId)
{
    INT8U firstData;
    /*"����uart�ڷ��ͺ��������ô򿪷������ã���������һ���ֽڷ���"*/
    if(ComId!=E_COM_SLAVE)/*CM���Ϊ��˫��*/  
    {
      COM_RxTx_Ctrl(ComId,TRANSTA_SO_RC);
    }
    else
    {
      COM_RxTx_Ctrl(ComId,TRANSTA_SO_RO);
    }
	    /*"�޸�״̬Ϊæ"*/
    mstComTxList[ComId].status = UART_BUSY;
    mUartTxStatus[mstComUartConfig[ComId].uart_ID] = UART_BUSY;
    firstData=*mstComTxList[ComId].pdata_Tx;
    mstComTxList[ComId].len_Tx--;
    mstComTxList[ComId].pdata_Tx++;    
    UART_Send1Byte(mstComUartConfig[ComId].uart_ID,firstData);

}


/*"*****************************************************************************"*/
/*"  Function:       CMD_Init"*/
/*"  Description:   �ز�/RFģ��˿ڳ�ʼ��,����λ����"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:       "*/
/*"*****************************************************************************"*/

#define _ZBResetDelay  200    /*"�ز���λ��ʱʱ������λ/ms"*/
#define _RFResetDelay  12    /*"RF��λ��ʱʱ������λ/ms"*/
void CMD_Init(INT8U mode)
{
    switch(mode)
    {
    case PLC_MODE:
      GPIO_ConfigPin(PLC_RST,OUTPUT,NORMAL_OUT);
      GPIO_OutputPin(PLC_RST,LOW); /*"��ͨ��ģ�鸴λ������0"*/
      MCU_DelayMs(_ZBResetDelay); /*"��ʱ200ms"*/
      GPIO_OutputPin(PLC_RST,HIGH);/*"��ͨ��ģ�鸴λ������1"*/	
      break;
    case RF_MODE:
      GPIO_ConfigPin(RF_RST,OUTPUT,NORMAL_OUT);
      GPIO_OutputPin(RF_RST,LOW); /*"��ͨ��ģ�鸴λ������0"*/
      MCU_DelayMs(_RFResetDelay); /*"��ʱ200ms"*/
      GPIO_OutputPin(RF_RST,HIGH);/*"��ͨ��ģ�鸴λ������1"*/	
      break;
    }
}

/*"*****************************************************************************"*/
/*"  Function:       CMD_Eventout_On"*/
/*"  Description:   ģ�������ϱ���"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:       "*/
/*"*****************************************************************************"*/
void CMD_Eventout_On (void)
{
	GPIO_OutputPin(EVENT_OUT,HIGH);
}

/*"*****************************************************************************"*/
/*"  Function:       CMD_Eventout_Off"*/
/*"  Description:   ģ�������ϱ��ر�"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:       "*/
/*"*****************************************************************************"*/
void CMD_Eventout_Off (void)
{
	GPIO_OutputPin(EVENT_OUT,LOW);
}

