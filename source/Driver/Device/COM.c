/*"***********************************************************"*/		
/*"文件名: com.c	"*/	
/*"作者: 扶忠权     "*/ 
/*"版本: 1.0       日期: 2014.03.11"*/	
/*"版本: 2.0       日期: 2014.08.25"*/	
/*"版本: 3.0       日期: 2017.01.9"*/	
/*"综合国内、海外表计的需求重新设计通信驱动，与前版相比，设计理念已大不相同"*/	
/*"文件描述: 通信设备如:485、红外、载波等通信接口设备接口实现"*/
/*"函数列表: "*/		
/*"<函数名>	"*/	
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
/*"接收列表"*/	
static ST_COM_RX_NODE mstComRxList[E_MAX_COM_ID];
/*"发送列表"*/	
static ST_COM_TX_NODE mstComTxList[E_MAX_COM_ID];
/*"com与uart口的配对关系"*/	
/*"为简化查找关系，com口对应数据务必为EN_COM_ID中的数据，且顺序不能改变"*/	
const ST_COM_UART_CFG mstComUartConfig[E_MAX_COM_ID]=
{
    {E_COM_RS485_1,E_UART_ID_485,COM_458_1_RX,COM_458_1_TX},
    {E_COM_SLAVE,E_UART_ID_SLAVE,COM_SLAVE_RX,COM_SLAVE_TX},
};
/*"串口配置参数列表"*/
static ST_UART_PARAM mstUartParam[E_UART_NUM];
/*"uart口的状态列表"*/	
static INT8U mUartTxStatus[E_UART_NUM];
/*"uart口的超时列表列表"*/	
static INT16U mUart_TimeOut[E_UART_NUM];


void COM_StartTX(INT8U ComId);
/*"*****************************************************************************"*/
/*"  Function:       COM_VarInit"*/
/*"  Description:   模块相关变量初始化"*/
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
/*"列表初始化"*/	
    LIB_MemSet(0,(INT8U *)&mstComRxList[0],sizeof(ST_COM_RX_NODE));
    LIB_MemSet(0,(INT8U *)&mstComTxList[0],sizeof(ST_COM_TX_NODE));
    LIB_MemSet(0,(INT8U *)&mstUartParam[0],sizeof(ST_UART_PARAM));
    LIB_MemSet(0,(INT8U *)&mUartTxStatus[0],sizeof(INT8U)*E_UART_NUM);
    /*"特殊数据初始化"*/	
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
/*"  Description:   某路com口初始化"*/
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
    /*"对单路com配置"*/	
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
        /*"保存参数"*/
        mstUartParam[channelTmp].uart_ID=channelTmp;
        mstUartParam[channelTmp].com_ID=ComId;
        mstUartParam[channelTmp].uart_Bps=BitRate;
        mstUartParam[channelTmp].uart_Form=UartForm.byte; 
    }
    /*"配置MCU"*/
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
/*"  Description:   某路com口释放"*/
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
/*"  Description:   某路com口收发状态切换"*/
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
/*"  Description:   某路uart口接收数据，所有对应com数据的接收"*/
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
    /*"取数据"*/	
    receiveData=UART_Receive1Byte(UartId);

    /*"将数据传入对应com口，可实现单uart口对应多个com设备的环境"*/	
    for(num=0;num<E_MAX_COM_ID;num++)/*"查找对应com口编号"*/
    {
        if(UartId==mstComUartConfig[num].uart_ID)
        {
            mstComRxList[mstComUartConfig[num].com_ID].Uart_Rx(num,receiveData);
            mUart_TimeOut[UartId]=UART_TIMEOUT_TIME;
        }
    }
}
/*"串口接收中断注册函数"*/	
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
/*"  Description:   某路uart口发送数据，实现对应com数据的发送"*/
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
        if((UartId==mstComUartConfig[num].uart_ID)&&(mstComTxList[mstComUartConfig[num].com_ID].status==UART_BUSY))/*"查找对应com口编号、状态"*/
        {
            if(mstComTxList[mstComUartConfig[num].com_ID].len_Tx==0)/*"是否为最后一个字节发送完成"*/
            { 
                if(mstComTxList[mstComUartConfig[num].com_ID].UartTx_CallBackFun!= NULL)
                {
                    if(mstComUartConfig[num].com_ID!=E_COM_SLAVE)/*CM外均为半双工*/  
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
            else/*"不为最后一个字节则发送一个字节"*/
            {
                UART_Send1Byte(UartId,*mstComTxList[mstComUartConfig[num].com_ID].pdata_Tx);
                mstComTxList[mstComUartConfig[num].com_ID].len_Tx--;
                mstComTxList[mstComUartConfig[num].com_ID].pdata_Tx++;
            }
            break;
         }
    }
}
/*"串口发送中断注册函数"*/	
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
/*"  Description:   启动某路com数据发送"*/
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
    /*"判断是否需要立即启动发送数据"*/
    if(mUartTxStatus[mstComUartConfig[ComId].uart_ID]==UART_IDLE)
    {
        COM_StartTX(ComId);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       COM_Monitor_1Sec"*/
/*"  Description:   等待发送的com口的检测启动，周期性超时检测"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:       COM_Monitor要求1s调用一次 code size  byte"*/
/*"*****************************************************************************"*/
void COM_Monitor_1Sec (void)
{
    INT8U num,comNum;
    ST_UARTCONFIG UartConf;
    /*"查询处于等待状态的com口是否可以重新启动发送流程"*/
    for(comNum=0;comNum<E_MAX_COM_ID;comNum++)
    {
        if((mstComTxList[comNum].status==UART_WAIT)&&(mUartTxStatus[mstComUartConfig[comNum].uart_ID]==UART_IDLE))
        {
            COM_StartTX(comNum);
        }
    }
    /*"轮询uart空闲时间是否满足要求"*/
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
            /*"此处调用初始化uart口为最后一次保存的配置"*/
            UartConf.Baud = mstUartParam[num].uart_Bps;
            UartConf.UartForm.byte = mstUartParam[num].uart_Form;
            UartConf.TranSta.byte = TRANSTA_SC_RO;
            UartConf.SendIsr = mstComUartConfig[mstUartParam[num].com_ID].Com_Tx;
            UartConf.ReceiveIsr = mstComUartConfig[mstUartParam[num].com_ID].Com_Rx;
            UART_Open(mstUartParam[num].uart_ID,&UartConf);

        }
    }
    
}
 /*"启动发送第一个字节的流程"*/
void COM_StartTX(INT8U ComId)
{
    INT8U firstData;
    /*"启动uart口发送函数，调用打开发送配置，并启动第一个字节发送"*/
    if(ComId!=E_COM_SLAVE)/*CM外均为半双工*/  
    {
      COM_RxTx_Ctrl(ComId,TRANSTA_SO_RC);
    }
    else
    {
      COM_RxTx_Ctrl(ComId,TRANSTA_SO_RO);
    }
	    /*"修改状态为忙"*/
    mstComTxList[ComId].status = UART_BUSY;
    mUartTxStatus[mstComUartConfig[ComId].uart_ID] = UART_BUSY;
    firstData=*mstComTxList[ComId].pdata_Tx;
    mstComTxList[ComId].len_Tx--;
    mstComTxList[ComId].pdata_Tx++;    
    UART_Send1Byte(mstComUartConfig[ComId].uart_ID,firstData);

}


/*"*****************************************************************************"*/
/*"  Function:       CMD_Init"*/
/*"  Description:   载波/RF模块端口初始化,含复位操作"*/
/*"  Calls:          "*/
/*"  Called By:      CM"*/
/*"  Input:          "*/
/*"  Output         "*/

/*"  Return:       "*/
/*"  Others:       "*/
/*"*****************************************************************************"*/

#define _ZBResetDelay  200    /*"载波复位延时时长，单位/ms"*/
#define _RFResetDelay  12    /*"RF复位延时时长，单位/ms"*/
void CMD_Init(INT8U mode)
{
    switch(mode)
    {
    case PLC_MODE:
      GPIO_ConfigPin(PLC_RST,OUTPUT,NORMAL_OUT);
      GPIO_OutputPin(PLC_RST,LOW); /*"将通信模块复位引脚置0"*/
      MCU_DelayMs(_ZBResetDelay); /*"延时200ms"*/
      GPIO_OutputPin(PLC_RST,HIGH);/*"将通信模块复位引脚置1"*/	
      break;
    case RF_MODE:
      GPIO_ConfigPin(RF_RST,OUTPUT,NORMAL_OUT);
      GPIO_OutputPin(RF_RST,LOW); /*"将通信模块复位引脚置0"*/
      MCU_DelayMs(_RFResetDelay); /*"延时200ms"*/
      GPIO_OutputPin(RF_RST,HIGH);/*"将通信模块复位引脚置1"*/	
      break;
    }
}

/*"*****************************************************************************"*/
/*"  Function:       CMD_Eventout_On"*/
/*"  Description:   模块主动上报打开"*/
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
/*"  Description:   模块主动上报关闭"*/
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

