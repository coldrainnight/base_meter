/*"***********************************************************		
FileName: INT.c		
Author: 邹杨       
Version : 1.0       Date: 2016.12.21	
Description: INT驱动实现
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#include "Driver\MCU\INT.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\Uart.h"

//#define DEBUG_INT
static  ISR_FNCT_VOID  IntVectTb[E_INT_NUM];
#ifdef DEBUG_INT
static  INT32U  IntCnt[E_INT_NUM];
#endif

/*对外接口*/
void INT_Init(void);
void INT_VectSet (E_INT_ID IntId ,ISR_FNCT_VOID Isr);
void INT_IntPrioSet (E_INT_ID IntId,E_INT_PRIO Prio);
void INT_En(E_INT_ID IntId);
void INT_Dis (E_INT_ID IntId);



/*内部函数*/

static void INT_HandleNull(void);
static void INT_Handler(INT8U IntId);

static void INT_HandleNull(void)
{

}

/*********************************************************/
static  void  INT_Handler (INT8U  IntId)
{

    ISR_FNCT_VOID  isr;

    if (IntId < E_INT_NUM) 
    {
        isr = IntVectTb[IntId];
        #ifdef DEBUG_INT
        IntCnt[IntId]++;
        #endif
        if (isr != (ISR_FNCT_VOID)0) {
            isr();
        }
    }
}


/*中断处理函数，函数的定义在startup文件中*/

void NMI_Handler(void)
{
    INT_HandleNull();
}
void SVC_Handler()
{
    INT_HandleNull();

}
void PendSV_Handler()
{
    INT_HandleNull();
}


/*系统时钟处理*/
void    SysTick_Handler()
{
    INT_DISABLE_ALL;
    
    INT_ENABLE_ALL;
}

/*窗口看门狗中断处理*/
void  WWDT_IRQHandler(void)  
{ 
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_WWDT);            
    INT_ENABLE_ALL;

}

/*电源监测报警中断*/
void  SVD_IRQHandler(void)  
{ 
    INT_DISABLE_ALL;
    
    if(ANAC->SVDSIF & 0x00000001)/*电源恢复中断*/
    {
        ANAC->SVDSIF |= 0x00000001;/*清除中断标志位*/
        if(ANAC->SVDSIF & 0x00000100)/*确认电源状态为恢复状态*/
        {
            if(ANAC->SVDSIF & 0x00000100)/*确认电源状态为恢复状态*/
            {
                INT_Handler(E_INT_ID_SVD);       
            }
        }
    }
    else if(ANAC->SVDSIF & 0x00000002)/*电源跌落中断*/
    {
        ANAC->SVDSIF |= 0x00000002;/*清除中断标志位*/
        if((ANAC->SVDSIF & 0x00000100) ==0)/*确认电源状态为跌落状态*/
        {
            if((ANAC->SVDSIF & 0x00000100) ==0)
            {
                INT_Handler(E_INT_ID_SVD);       
            }
        }
    }
    else
    {

    }
     
    INT_ENABLE_ALL;
}

/*RTC中断处理*/
void RTC_IRQHandler(void)
{
    INT_DISABLE_ALL;

    if(RTC->RTCIF & 0x0008)/*秒中断*/
    {
        INT_Handler(E_INT_ID_RTC);
    }

    RTC->RTCIF = 0xFF;/*清零所有中断标识*/
    INT_ENABLE_ALL;
}

/*NVMIF中断*/
void NVMIF_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_NVMIF);        
    INT_ENABLE_ALL;
}

/*停震检测中断*/
void FDET_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_FDET);        
    INT_ENABLE_ALL;
}


/*ADC转换完成中断*/
void ADC_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_ADC);        
    INT_ENABLE_ALL;
}

/*SPI0中断*/
void HSPI_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_HSPI);        
    INT_ENABLE_ALL;

}

/*SPI1中断*/
void  SPI1_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_SPI1);        
    INT_ENABLE_ALL;
}

/*SPI2中断*/
void  SPI2_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_SPI2);        
    INT_ENABLE_ALL;
}

void  UART0_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_UART0);        
    INT_ENABLE_ALL;
}

void  UART1_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_UART1);        
    INT_ENABLE_ALL;
}

void  UART2_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_UART2);        
    INT_ENABLE_ALL;
}

void  UART3_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_UART3);        
    INT_ENABLE_ALL;
}

void  UART4_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_UART4);        
    INT_ENABLE_ALL;
}

void  UART5_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_UART5);        
    INT_ENABLE_ALL;
}

/*78160*/
void  USART0_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_U78160);        
    INT_ENABLE_ALL;

}

void  USART1_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_U78161);        
    INT_ENABLE_ALL;
}

void  I2C_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_I2C);        
    INT_ENABLE_ALL;
}

/*LCD*/
void  DISP_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_DISP);        
    INT_ENABLE_ALL;
}

/*AES*/
void  AES_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_AES);        
    INT_ENABLE_ALL;
}

/*低功耗定时中断*/
void  LPTIM_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_LPTIM);
    INT_ENABLE_ALL;
}


void  DMA_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_DMA);        
    INT_ENABLE_ALL;
}

void  TRNG_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_TRNG);        
    INT_ENABLE_ALL;
}
void  COMP_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_COMP);        
    INT_ENABLE_ALL;
}
void  BT1_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_BT1);        
    INT_ENABLE_ALL;
}

void  BT2_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_BT2);        
    INT_ENABLE_ALL;
}

void  ETIMER1_IRQHandler(void)
{
    INT_DISABLE_ALL;
    ETIMER1->ETxIF =  0xFFFF;
    INT_Handler(E_INT_ID_ET1);        
    INT_ENABLE_ALL;
}
void  ETIMER2_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_ET2);        
    INT_ENABLE_ALL;
}

void  ETIMER3_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_ET3);        
    INT_ENABLE_ALL;
}

void  ETIMER4_IRQHandler(void)	
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_ET4);        
    INT_ENABLE_ALL;
}

extern volatile INT32U gLineTempIntCnt;
extern volatile INT8U gLineTempIntFlag;
void  GPIO_IRQHandler(void)
{
	INT_DISABLE_ALL;
	#if 0
    if(gLineTempIntFlag == 0x01)
    {
       gLineTempIntCnt++;
       
       GPIO_common->EXTI0IF = 0xFFFFFFFF;
       GPIO_common->EXTI1IF = 0xFFFFFFFF;
       GPIO_common->EXTI2IF = 0xFFFFFFFF;

       INT_ENABLE_ALL;
       return;
    }
	#endif
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_EXTI);   
	
    INT_ENABLE_ALL;
}

/************************************/

void INT_Init(void)
{
    INT8U IntId;

    for (IntId = 0; IntId < E_INT_NUM; IntId++) 
    {
        INT_VectSet((E_INT_ID)IntId, INT_HandleNull);
    }
    #ifdef DEBUG_INT
    LIB_MemSet(0x00, (INT8U * )&IntCnt[0], sizeof(IntCnt[E_INT_NUM]));
    #endif
}

/*"************************************************			
Function: 	INT_VectSet		
Description: 提供给上层调用，传递上层的中断处理函数
Input:  
        E_INT_ID  中断ID号
        isr       传递的中断处理函数

Output: 无	
Return: 无
Others: 		
************************************************"*/	
void  INT_VectSet (E_INT_ID   IntId,
                      ISR_FNCT_VOID  isr)
{
    if (IntId < E_INT_NUM) 
    {
        IntVectTb[IntId] = isr;
    }
}


/*INTID to IrQn*/
static IRQn_Type IntIdToIRQn(E_INT_ID IntId)
{
    return (IRQn_Type)IntId;
}

/*"************************************************			
Function: 	INT_IntPrioSet		
Description: 设置中断的中断优先级
Input:  
        E_INT_ID  中断ID号
        isr       传递的中断处理函数

Output: 无	
Return: 无
Others: 		
************************************************"*/	
void INT_IntPrioSet (E_INT_ID IntId,E_INT_PRIO Prio)
{
    /*国网698平台暂未考虑使用优先级*/
}



/*"************************************************			
Function: 	INT_En		
Description: 中断使能,并清除中断请求
Input:  
        E_INT_ID  中断ID号
Output: 无	
Return: 无
Others: 		
************************************************"*/	
void INT_En(E_INT_ID IntId)
{
    IRQn_Type TmpIRQn;
    
    TmpIRQn = IntIdToIRQn(IntId);

    NVIC_EnableIRQ(TmpIRQn);
}

/*"************************************************			
Function: 	INT_Dis		
Description: 中断关闭，不清除已有中断请求
Input:  
        E_INT_ID  中断ID号
Output: 无	
Return: 无
Others: 		
************************************************"*/	
void INT_Dis (E_INT_ID IntId)
{
    IRQn_Type TmpIRQn;

    TmpIRQn = IntIdToIRQn(IntId);

    NVIC_DisableIRQ(TmpIRQn);
}


NO_INIT INT32U err_code_addr;

void HardFault_Handler(void)
{
    INT32U sp_addr;

	sp_addr = __get_SP();
	err_code_addr = *(INT32U *)(sp_addr + 20);
	MCU_Reset();
}


