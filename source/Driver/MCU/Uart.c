/*"**********************************************************"*/		
/*"FileName: uart.c"*/		
/*"Author: "*/      
/*"Version : 1.0       Date: 2017.01.05	"*/
/*"Description: "*/
/*"Function List: "*/		
/*"<FunctionName>	"*/	
/*"<author> <time> <version > <desc>"*/			
/*"**********************************************************"*/
#include "Driver\MCU\Uart.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\WDT.h"
#include "Driver\MCU\Tmr.h"

ST_UARTISR UartIsr;

INT8U UART_Open(INT8U UartId,ST_UARTCONFIG *pUartConfig);
void UART_Close (INT8U UartId);
INT8U UART_TranStaCtrl (INT8U UartId, INT8U TRANSTA);

/* UART_IE_definition */

#define UART_RX_IE5                         ((INT32U)0x00000800)
#define UART_TX_IE5                         ((INT32U)0x00000400)
#define UART_RX_IE4                         ((INT32U)0x00000200)
#define UART_TX_IE4                        	((INT32U)0x00000100)
#define UART_RX_IE3                      	((INT32U)0x00000080)
#define UART_TX_IE3                        	((INT32U)0x00000040)
#define UART_RX_IE2                         ((INT32U)0x00000020)
#define UART_TX_IE2                         ((INT32U)0x00000010)
#define UART_RX_IE1                         ((INT32U)0x00000008)
#define UART_TX_IE1                      	((INT32U)0x00000004) 
#define UART_RX_IE0                         ((INT32U)0x00000002)
#define UART_TX_IE0                         ((INT32U)0x00000001)

/* UART_IF_definition */
#define UART_RX_IF5                         ((INT32U)0x00000800)
#define UART_TX_IF5                         ((INT32U)0x00000400)
#define UART_RX_IF4                         ((INT32U)0x00000200)
#define UART_TX_IF4                        	((INT32U)0x00000100)
#define UART_RX_IF3                      	((INT32U)0x00000080)
#define UART_TX_IF3                        	((INT32U)0x00000040)
#define UART_RX_IF2                         ((INT32U)0x00000020)
#define UART_TX_IF2                         ((INT32U)0x00000010)
#define UART_RX_IF1                         ((INT32U)0x00000008)
#define UART_TX_IF1                      	((INT32U)0x00000004) 
#define UART_RX_IF0                         ((INT32U)0x00000002)
#define UART_TX_IF0                         ((INT32U)0x00000001)


static INT32U CalcBaudRegVal(INT8U baud)
{
    INT32U TmpBuad;
    INT32U TmpRet;

    if(E_BAUD300 == baud)
    {
        TmpBuad = 300;
    }
    else if(E_BAUD600 == baud)
    {
        TmpBuad = 600;
    }
    else if(E_BAUD1200 == baud)
    {
        TmpBuad = 1200;
    }
    else if(E_BAUD2400 == baud)
    {
        TmpBuad = 2400;
    }
    else if(E_BAUD4800 == baud)
    {
        TmpBuad = 4800;
    }
    else if(E_BAUD9600 == baud)
    {
        TmpBuad = 9600;
    }
    else if(E_BAUD19200 == baud)
    {
        TmpBuad = 19200;
    }
    else if(E_BAUD38400 == baud)
    {
        TmpBuad = 38400;
    }
    else if(E_BAUD57600 == baud)
    {
        TmpBuad = 57600;
    }
    else if(E_BAUD115200 == baud)
    {
        TmpBuad = 115200;
    }
	else if(E_BAUD230400 == baud)
    {
        TmpBuad = 230400;
    }
	else if(E_BAUD460800 == baud)
    {
        TmpBuad = 460800;
    }
    else
    {
        TmpBuad = 9600;
    }
    
    TmpRet = (APBClock/TmpBuad)-1;//SysClockFrq

    return TmpRet;
}


static void	UART_485_INT(void)
{
    if(UART_common->UARTIF & UART_RX_IF2)
    {
        UartIsr.Rs485Receice();
    }
    
    if(UART_common->UARTIF & UART_TX_IF2)
    {
        UART_common->UARTIF = UART_TX_IF2;/*清零*/
        UartIsr.Rs485Transmit();
    }
}


static void	UART_SLAVE_INT(void)
{
    if(UART_common->UARTIF & UART_RX_IF5)
    {
        UartIsr.SlaveReceice();
    }
    
    if(UART_common->UARTIF & UART_TX_IF5)
    {
        UART_common->UARTIF = UART_TX_IF5;/*清零*/
        UartIsr.SlaveTransmit();
    }
}

static void	UART_MASTER_INT(void)
{
    if(UART_common->UARTIF & UART_RX_IF5)
    {
        UartIsr.MasterReceice();
    }
    
    if(UART_common->UARTIF & UART_TX_IF5)
    {
        UART_common->UARTIF = UART_TX_IF5;/*清零*/
        UartIsr.MasterTransmit();
    }
}


static void	UART_MC_INT(void)
{
    if(UART_common->UARTIF & UART_RX_IF0)
    {
        UartIsr.McReceice();
    }
    
    if(UART_common->UARTIF & UART_TX_IF0)
    {
        UART_common->UARTIF = UART_TX_IF0;/*清零*/
        UartIsr.McTransmit();
    }
}


/*"**************************************************************
Function：unsigned int SelectBuad(unsigned char BaudMode)
Input ：UartId  UART端口号
        pUartConfig  UART配置参数
Description：
**************************************************************"*/
INT8U UART_Open(INT8U UartId,ST_UARTCONFIG *pUartConfig)
{

    MCU_PeriphClkSet(UART_COM_num,ENABLE);/*初始化UART公共寄存器时钟*/

    if( E_UART_ID_485 == UartId )
    {
        MCU_PeriphClkSet(UART2_num,ENABLE);
        GPIO_SetMulti(RS485_TXD, GPIO_Mode_DIG);
        GPIO_SetMulti(RS485_RXD, GPIO_Mode_DIG);
        UART2->RXSTA  = 0x40;/*偶校验，关闭接收*/
        UART2->TXSTA  = 0x20;/*移位寄存器空中断，关闭红外*/
        UART2->RTXCON = 0;/*不取反向*/
        UART2->SPBRG = CalcBaudRegVal(pUartConfig->Baud);    
        UartIsr.Rs485Receice= pUartConfig->ReceiveIsr ;
        UartIsr.Rs485Transmit= pUartConfig->SendIsr ;
        INT_VectSet(E_INT_ID_UART2, UART_485_INT);
        UART_TranStaCtrl (UartId, pUartConfig->TranSta.byte);
        INT_En(E_INT_ID_UART2);
    }
    else if( E_UART_ID_SLAVE == UartId )
    {
        MCU_PeriphClkSet(UART5_num,ENABLE);
        GPIO_SetMulti(SLAVE_TXD, GPIO_Mode_DIG);
        GPIO_SetMulti(SLAVE_RXD, GPIO_Mode_DIG);
        UART5->RXSTA  = 0x40;/*偶校验，关闭接收*/
        UART5->TXSTA  = 0x20;/*移位寄存器空中断，关闭红外*/
        UART5->RTXCON = 0;/*不取反向*/
        UART5->SPBRG = CalcBaudRegVal(pUartConfig->Baud);    
        UartIsr.SlaveReceice= pUartConfig->ReceiveIsr ;
        UartIsr.SlaveTransmit= pUartConfig->SendIsr ;
        INT_VectSet(E_INT_ID_UART5,UART_SLAVE_INT);
        UART_TranStaCtrl (UartId, pUartConfig->TranSta.byte);
        INT_En(E_INT_ID_UART5);
    }
    else if( E_UART_ID_MASTER_TX == UartId )
    {
        MCU_PeriphClkSet(UART5_num,ENABLE);
        GPIO_SetMulti(MASTER_RXD, GPIO_Mode_DIG);
        GPIO_SetMulti(MASTER_TXD, GPIO_Mode_DIG);
        UART5->RXSTA  = 0x40;/*偶校验，关闭接收*/
        UART5->TXSTA  = 0x20;/*移位寄存器空中断，关闭红外*/
        UART5->RTXCON = 0;/*不取反向*/
        UART5->SPBRG = CalcBaudRegVal(pUartConfig->Baud);    
        UartIsr.MasterReceice= pUartConfig->ReceiveIsr ;
        UartIsr.MasterTransmit= pUartConfig->SendIsr ;
        INT_VectSet(E_INT_ID_UART5,UART_MASTER_INT);
        UART_TranStaCtrl (UartId, pUartConfig->TranSta.byte);
        INT_En(E_INT_ID_UART5);
    }
    else if( E_UART_ID_MC == UartId )
    {
        MCU_PeriphClkSet(UART0_num,ENABLE);
        GPIO_SetMulti(MC_TXD, GPIO_Mode_DIG);
        GPIO_SetMulti(MC_RXD, GPIO_Mode_DIG);
        UART0->RXSTA  = 0x40;/*偶校验，关闭接收*/
        UART0->TXSTA  = 0x20;/*移位寄存器空中断，关闭红外*/
        UART0->RTXCON = 0;/*不取反向*/
        UART0->SPBRG = CalcBaudRegVal(pUartConfig->Baud);    
        UartIsr.McReceice= pUartConfig->ReceiveIsr ;
        UartIsr.McTransmit= pUartConfig->SendIsr ;
        INT_VectSet(E_INT_ID_UART0,UART_MC_INT);
        UART_TranStaCtrl (UartId, pUartConfig->TranSta.byte);
        INT_En(E_INT_ID_UART0);        
    }
    return TRUE;
}

void UART_Close (INT8U UartId)
{
   
}



INT8U UART_TranStaCtrl (INT8U UartId, INT8U TranSta)
{
    UN_TRAN_STA TmpTranSta;
    
    TmpTranSta.byte = TranSta;
    MCU_IntDisableAll();
    if( E_UART_ID_485 == UartId )
    {
        UART_common->UARTIF = UART_TX_IF2;
        UART_common->UARTIF = UART_RX_IF2;

        GPIO_ConfigPin(RS485_REDE,OUTPUT,NORMAL_OUT);
        if(TmpTranSta.bits.SendMode)
        {
            GPIO_OutputPin(RS485_REDE,LOW);
            UART_common->UARTIE |=	UART_TX_IE2;
            UART2->TXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_TX_IE2;
            UART2->TXSTA &= ~(INT32U)0x10;
        }
        
        if(TmpTranSta.bits.ReceiveMode)
        {
            GPIO_OutputPin(RS485_REDE,HIGH);
            UART_common->UARTIE |=	UART_RX_IE2;
            UART2->RXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_RX_IE2;
            UART2->RXSTA &= ~(INT32U)0x10;
        }
        
    }
    else if( E_UART_ID_SLAVE == UartId )
    {
        UART_common->UARTIF = UART_TX_IF5;
        UART_common->UARTIF = UART_RX_IF5;
        
        if(TmpTranSta.bits.SendMode)
        {
            
            UART_common->UARTIE |=	UART_TX_IE5;
            UART5->TXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_TX_IE5;
            UART5->TXSTA &= ~(INT32U)0x10;
        }
        
        if(TmpTranSta.bits.ReceiveMode)
        {
            UART_common->UARTIE |=	UART_RX_IE5;
            UART5->RXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_RX_IE5;
            UART5->RXSTA &= ~(INT32U)0x10;
        }
    }
    else if( E_UART_ID_MASTER_TX == UartId )
    {
        UART_common->UARTIF = UART_TX_IF5;
        UART_common->UARTIF = UART_RX_IF5;
        
        if(TmpTranSta.bits.SendMode)
        {
            
            UART_common->UARTIE |=	UART_TX_IE5;
            UART5->TXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_TX_IE5;
            UART5->TXSTA &= ~(INT32U)0x10;
        }
        
        if(TmpTranSta.bits.ReceiveMode)
        {
            UART_common->UARTIE |=	UART_RX_IE5;
            UART5->RXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_RX_IE5;
            UART5->RXSTA &= ~(INT32U)0x10;
        }
    }
    else if( E_UART_ID_MC == UartId )
    {
        UART_common->UARTIF = UART_TX_IF0;
        UART_common->UARTIF = UART_RX_IF0;
        
        if(TmpTranSta.bits.SendMode)
        {
            
            UART_common->UARTIE |=	UART_TX_IE0;
            UART0->TXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_TX_IE0;
            UART0->TXSTA &= ~(INT32U)0x10;
        }
        
        if(TmpTranSta.bits.ReceiveMode)
        {
            UART_common->UARTIE |=	UART_RX_IE0;
            UART0->RXSTA |= (INT32U)0x10;
        }
        else
        {
            UART_common->UARTIE &=	~UART_RX_IE0;
            UART0->RXSTA &= ~(INT32U)0x10;
        }
    }
    MCU_IntEnableAll();
    return TRUE;

}

void UART_Send1Byte(INT8U UartId,INT8U Byte)
{

    if( E_UART_ID_485 == UartId )
    {
        UART2->TXREG = (INT32U)Byte;
    }
    else if( E_UART_ID_SLAVE == UartId )
    {
        UART5->TXREG = (INT32U)Byte;
    }
    else if( E_UART_ID_MASTER_TX == UartId )
    {
        UART5->TXREG = (INT32U)Byte;
    }
    else if( E_UART_ID_MC == UartId )
    {
        UART0->TXREG = (INT32U)Byte;
    }
}

INT8U UART_Receive1Byte(INT8U UartId)
{
    INT8U TmpRet;
    
    if( E_UART_ID_485 == UartId )
    {
        TmpRet = (INT8U)UART2->RXREG;
    }
    else if( E_UART_ID_SLAVE == UartId )
    {
        TmpRet = (INT8U)UART5->RXREG;
    }
    else if( E_UART_ID_MASTER_TX == UartId )
    {
        TmpRet = (INT8U)UART5->RXREG;
    }
    else if( E_UART_ID_MC == UartId )
    {
        TmpRet = (INT8U)UART0->RXREG;
    }
    return TmpRet;
}


INT8U UART_PollSend(INT8U UartId,INT8U * Buf, INT16U Len)
{
    INT16U i,wait;
    INT32U * pTmpReg;
    INT32U * pTmpRegBufSta;

    if( E_UART_ID_485 == UartId )
    {
        pTmpReg = (INT32U *)&UART2->TXSTA;
        pTmpRegBufSta = (INT32U *)&UART2->TXFIFOSTA;
    }
    else if( E_UART_ID_SLAVE == UartId )
    {
        pTmpReg = (INT32U *)&UART5->TXSTA;
        pTmpRegBufSta = (INT32U *)&UART5->TXFIFOSTA;
    }
    else if( E_UART_ID_MC == UartId )
    {
        pTmpReg = (INT32U *)&UART0->TXSTA;
        pTmpRegBufSta = (INT32U *)&UART0->TXFIFOSTA;
    }

    for(i = 0; i < Len; i++ )
    {
        if((*pTmpReg) & 0x02)/*TSR空*/
        {

        }
        wait = 400;

        UART_Send1Byte(UartId,Buf[i]);    
        while((*pTmpRegBufSta & 0x01) && (wait>0))
        {
            wait--;
        }

        while ((((*pTmpReg)&0x02)==0x00) && (wait>0))
        {
            wait--;
            MCU_DelayUs(1);
        }
      
    }
    return i;

}

INT8U UART_PollReceive(INT8U UartId,
                                    INT8U * Buf, 
                                    INT16U Len, 
                                    INT16U MaxDelayMs)
{

    return 1;
}
