/*"***********************************************************		
FileName: INT.h		
Author: 邹杨       
Version : 1.0       Date: 2016.12.21	
Description: INT驱动实现
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
    E_INT_ID_RESET = 0,/*复位向量*/
    E_INT_ID_NMI ,/*复位向量*/
    E_INT_ID_HARDFAULT,/*硬错误中断向量*/
    E_INT_ID_SVC,/*SVcall系统服务请求*/
    E_INT_ID_PENDSV,/*可挂起系统服务请求*/
    E_INT_ID_SYSTICK =0,/*内部定时器中断向量*/

    #endif    
    E_INT_ID_WWDT =0,/*窗口看门狗中断*/
    E_INT_ID_SVD ,/*电源监测报警中断*/
    E_INT_ID_RTC ,/*RTC中断*/
    E_INT_ID_NVMIF ,/*NVMIF*/
    E_INT_ID_FDET ,/*XTLF停振检测中断*/
    E_INT_ID_ADC ,/*ADC转换完成中断*/
    E_INT_ID_HSPI ,/*SPI中断*/
    E_INT_ID_SPI1 ,/*SPI中断*/
    E_INT_ID_SPI2 ,/*SPI中断*/
    E_INT_ID_UART0 ,/*UART中断*/
    E_INT_ID_UART1 ,/*UART中断*/
    E_INT_ID_UART2 ,/*UART中断*/
    E_INT_ID_UART3 ,/*UART中断*/
    E_INT_ID_UART4 ,/*UART中断*/
    E_INT_ID_UART5 ,/*UART中断*/
    E_INT_ID_U78160 ,/*7816中断*/
    E_INT_ID_U78161 ,/*7816中断*/
    E_INT_ID_I2C ,/*I2C中断*/
    E_INT_ID_DISP ,/*LCD中断*/
    E_INT_ID_AES ,/*AES中断*/
    E_INT_ID_LPTIM ,/*低功耗定时中断*/
    E_INT_ID_DMA ,/*DMA中断*/
    E_INT_ID_TRNG ,/*随机数发生器中断*/
    E_INT_ID_COMP ,/*模拟比较器中断*/
    E_INT_ID_BT1 ,/*BASIC Time1中断*/
    E_INT_ID_BT2 ,/*BASIC Time2中断*/
    E_INT_ID_ET1 ,/*EXT Time1中断*/
    E_INT_ID_ET2 ,/*EXT Time2中断*/
    E_INT_ID_ET3 ,/*EXT Time3中断*/
    E_INT_ID_ET4 ,/*EXT Time4中断*/
    E_INT_ID_EXTI ,/*外部引脚中断*/
    E_INT_NUM, /*总中断数*/

}E_INT_ID;

typedef enum
{
    E_INT_PRIO_LEVEL_0=0,/*high priority level*/
    E_INT_PRIO_LEVEL_1,
    E_INT_PRIO_LEVEL_2,
    E_INT_PRIO_LEVEL_3,/*low priority level*/
}E_INT_PRIO;/*中断优先级*/



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

