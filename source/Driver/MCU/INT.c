/*"***********************************************************		
FileName: INT.c		
Author: ����       
Version : 1.0       Date: 2016.12.21	
Description: INT����ʵ��
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

/*����ӿ�*/
void INT_Init(void);
void INT_VectSet (E_INT_ID IntId ,ISR_FNCT_VOID Isr);
void INT_IntPrioSet (E_INT_ID IntId,E_INT_PRIO Prio);
void INT_En(E_INT_ID IntId);
void INT_Dis (E_INT_ID IntId);



/*�ڲ�����*/

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


/*�жϴ������������Ķ�����startup�ļ���*/

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


/*ϵͳʱ�Ӵ���*/
void    SysTick_Handler()
{
    INT_DISABLE_ALL;
    
    INT_ENABLE_ALL;
}

/*���ڿ��Ź��жϴ���*/
void  WWDT_IRQHandler(void)  
{ 
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_WWDT);            
    INT_ENABLE_ALL;

}

/*��Դ��ⱨ���ж�*/
void  SVD_IRQHandler(void)  
{ 
    INT_DISABLE_ALL;
    
    if(ANAC->SVDSIF & 0x00000001)/*��Դ�ָ��ж�*/
    {
        ANAC->SVDSIF |= 0x00000001;/*����жϱ�־λ*/
        if(ANAC->SVDSIF & 0x00000100)/*ȷ�ϵ�Դ״̬Ϊ�ָ�״̬*/
        {
            if(ANAC->SVDSIF & 0x00000100)/*ȷ�ϵ�Դ״̬Ϊ�ָ�״̬*/
            {
                INT_Handler(E_INT_ID_SVD);       
            }
        }
    }
    else if(ANAC->SVDSIF & 0x00000002)/*��Դ�����ж�*/
    {
        ANAC->SVDSIF |= 0x00000002;/*����жϱ�־λ*/
        if((ANAC->SVDSIF & 0x00000100) ==0)/*ȷ�ϵ�Դ״̬Ϊ����״̬*/
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

/*RTC�жϴ���*/
void RTC_IRQHandler(void)
{
    INT_DISABLE_ALL;

    if(RTC->RTCIF & 0x0008)/*���ж�*/
    {
        INT_Handler(E_INT_ID_RTC);
    }

    RTC->RTCIF = 0xFF;/*���������жϱ�ʶ*/
    INT_ENABLE_ALL;
}

/*NVMIF�ж�*/
void NVMIF_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_NVMIF);        
    INT_ENABLE_ALL;
}

/*ͣ�����ж�*/
void FDET_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_FDET);        
    INT_ENABLE_ALL;
}


/*ADCת������ж�*/
void ADC_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_ADC);        
    INT_ENABLE_ALL;
}

/*SPI0�ж�*/
void HSPI_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_HSPI);        
    INT_ENABLE_ALL;

}

/*SPI1�ж�*/
void  SPI1_IRQHandler(void)
{
    INT_DISABLE_ALL;
    INT_Handler(E_INT_ID_SPI1);        
    INT_ENABLE_ALL;
}

/*SPI2�ж�*/
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

/*�͹��Ķ�ʱ�ж�*/
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
Description: �ṩ���ϲ���ã������ϲ���жϴ�����
Input:  
        E_INT_ID  �ж�ID��
        isr       ���ݵ��жϴ�����

Output: ��	
Return: ��
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
Description: �����жϵ��ж����ȼ�
Input:  
        E_INT_ID  �ж�ID��
        isr       ���ݵ��жϴ�����

Output: ��	
Return: ��
Others: 		
************************************************"*/	
void INT_IntPrioSet (E_INT_ID IntId,E_INT_PRIO Prio)
{
    /*����698ƽ̨��δ����ʹ�����ȼ�*/
}



/*"************************************************			
Function: 	INT_En		
Description: �ж�ʹ��,������ж�����
Input:  
        E_INT_ID  �ж�ID��
Output: ��	
Return: ��
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
Description: �жϹرգ�����������ж�����
Input:  
        E_INT_ID  �ж�ID��
Output: ��	
Return: ��
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


