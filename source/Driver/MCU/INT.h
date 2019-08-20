/*"***********************************************************		
FileName: INT.h		
Author: ����       
Version : 1.0       Date: 2016.12.21	
Description: INT����ʵ��
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	


#ifndef INT_H
#define INT_H

#include "system\Sys_def.h"


typedef enum
{   
    #if 0
    E_INT_ID_RESET = 0,/*��λ����*/
    E_INT_ID_NMI ,/*��λ����*/
    E_INT_ID_HARDFAULT,/*Ӳ�����ж�����*/
    E_INT_ID_SVC,/*SVcallϵͳ��������*/
    E_INT_ID_PENDSV,/*�ɹ���ϵͳ��������*/
    E_INT_ID_SYSTICK =0,/*�ڲ���ʱ���ж�����*/

    #endif    
    E_INT_ID_WWDT =0,/*���ڿ��Ź��ж�*/
    E_INT_ID_SVD ,/*��Դ��ⱨ���ж�*/
    E_INT_ID_RTC ,/*RTC�ж�*/
    E_INT_ID_NVMIF ,/*NVMIF*/
    E_INT_ID_FDET ,/*XTLFͣ�����ж�*/
    E_INT_ID_ADC ,/*ADCת������ж�*/
    E_INT_ID_HSPI ,/*SPI�ж�*/
    E_INT_ID_SPI1 ,/*SPI�ж�*/
    E_INT_ID_SPI2 ,/*SPI�ж�*/
    E_INT_ID_UART0 ,/*UART�ж�*/
    E_INT_ID_UART1 ,/*UART�ж�*/
    E_INT_ID_UART2 ,/*UART�ж�*/
    E_INT_ID_UART3 ,/*UART�ж�*/
    E_INT_ID_UART4 ,/*UART�ж�*/
    E_INT_ID_UART5 ,/*UART�ж�*/
    E_INT_ID_U78160 ,/*7816�ж�*/
    E_INT_ID_U78161 ,/*7816�ж�*/
    E_INT_ID_I2C ,/*I2C�ж�*/
    E_INT_ID_DISP ,/*LCD�ж�*/
    E_INT_ID_AES ,/*AES�ж�*/
    E_INT_ID_LPTIM ,/*�͹��Ķ�ʱ�ж�*/
    E_INT_ID_DMA ,/*DMA�ж�*/
    E_INT_ID_TRNG ,/*������������ж�*/
    E_INT_ID_COMP ,/*ģ��Ƚ����ж�*/
    E_INT_ID_BT1 ,/*BASIC Time1�ж�*/
    E_INT_ID_BT2 ,/*BASIC Time2�ж�*/
    E_INT_ID_ET1 ,/*EXT Time1�ж�*/
    E_INT_ID_ET2 ,/*EXT Time2�ж�*/
    E_INT_ID_ET3 ,/*EXT Time3�ж�*/
    E_INT_ID_ET4 ,/*EXT Time4�ж�*/
    E_INT_ID_EXTI ,/*�ⲿ�����ж�*/
    E_INT_NUM, /*���ж���*/

}E_INT_ID;

typedef enum
{
    E_INT_PRIO_LEVEL_0=0,/*high priority level*/
    E_INT_PRIO_LEVEL_1,
    E_INT_PRIO_LEVEL_2,
    E_INT_PRIO_LEVEL_3,/*low priority level*/
}E_INT_PRIO;/*�ж����ȼ�*/



typedef  void  (*ISR_FNCT_VOID)(void);

extern void INT_Init(void);
extern void INT_VectSet (E_INT_ID IntId ,ISR_FNCT_VOID Isr);
extern void INT_IntPrioSet (E_INT_ID IntId,E_INT_PRIO Prio);
extern void INT_En(E_INT_ID IntId);
extern void INT_Dis (E_INT_ID IntId);

#define  INT_ENABLE_ALL  (__enable_irq())
#define  INT_DISABLE_ALL (__disable_irq())

extern INT32U err_code_addr;

#endif

