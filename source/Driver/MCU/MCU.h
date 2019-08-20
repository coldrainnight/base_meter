/*"***********************************************************		
FileName: MCU.h		
Author:        
Version : 1.0       Date: 2016.01.05	
Description: MCU�ӿ�
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef MCU_H
#define MCU_H


#include "Driver\MCU\FM33A0XX.h"
#include "system\Sys_def.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\INT.h"


#define HALT    
#define NOP     __NOP()
#define STOP    

/*"ϵͳʱ��Ƶ��"*/
#define CLOCK_512K		512000
#define CLOCK_3M		3000000
#define CLOCK_4M		4000000
#define CLOCK_8M		8000000
#define CLOCK_12M		12000000
#define CLOCK_24M		24000000


#define PWRMODE_UP     (0)/*�ϵ�״̬*/
#define PWRMODE_DOWN    (1)/*����״̬*/

#define SLEEP       (0)
#define DEEP_SLEEP  (1)

extern INT32U SysClockFrq;
extern INT32U APBClock ;

/*��λ����*/
enum
{
    Reset_NVICN = 0,
    Reset_LOOKUP,
    Reset_WWDTN,
    Reset_IWDTN,
    Reset_SOFTN,/*�����λ��ʶ*/
    Reset_EMCN,/*EMC��λ��ʶ*/
    Reset_PDRN,/*�µ縴λ��ʶ*/
    Reset_PORN,/*�ϵ縴λ��ʶ*/
    Reset_TESTN,/*TESTN���Ÿ�λ��ʶ*/
    Reset_NRSTN,/*NRSTN���Ÿ�λ��ʶ*/
    Reset_MDUN,/*ģʽ��ϵ�·��λ��ʶ*/
    Reset_Other/*abnormal conditions*/
};                     

#define  RST_WWDTN    (0x0004UL)
#define  RST_IWDTN    (0x0010UL)
extern void MCU_Reset(void);
extern void MCU_ClkAdj(void);
extern INT8U MCU_GetResetSrc(void);
extern INT8U MCU_PwrChkInit(ISR_FNCT_VOID IsrFun);
extern void MCU_PwrChkOpenINT(void);
extern void MCU_PwrChkCloseINT(void);
extern void MCU_PwrChkOpen(void);
extern void MCU_PwrChkClose(void);
extern void MCU_PD_Deal(void);
extern INT8U MCU_GetPwrChkFlag(void);
extern void MCU_IntEnableAll(void);
extern void MCU_IntDisableAll(void);
extern INT8U MCU_GetPwrState (void);
extern void MCU_AddTick (void);
extern INT16U MCU_GetTick (void);
extern INT8U MCU_CmpTick (INT16U CmpTick,INT16U DelayMs);
extern INT8U MCU_RunModeSet(INT8U Mode);
extern INT8U MCU_Maintain(INT8U Mode);
extern void MCU_DelayMs(INT16U DelayMs);
extern void MCU_DelayUs (INT16U DelayUs);
extern void MCU_PeriphClkSet(Periph_Type periph_num,FunctionalState NewState);
extern void MCU_EnterSleep(INT8U Mode);
extern void MCU_OutSleep(void);

extern void OpenDebugIWDT(void);
extern void CloseDebugIWDT(void);
#endif
