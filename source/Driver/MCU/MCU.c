/*"***********************************************************		
FileName: MCU.C		
Author:        
Version : 1.0       Date: 2016.01.05	
Description: MCU接口
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/

#include "system\Sys_def.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\WDT.h"



INT32U SysClockFrq @ 0x20000138;
INT32U APBClock @ 0x20000140;

NO_INIT volatile static INT16U  sysTick @ 0x20000144;/*时钟滴答*/
static INT8U PowerState;
static void MCU_SetFre(INT8U fre);
/****************对外接口**********************/
void MCU_Reset(void);
INT8U MCU_GetResetSrc(void);
INT8U MCU_PwrChkInit(ISR_FNCT_VOID IsrFun);
void MCU_PwrChkOpenINT(void);
void MCU_PwrChkCloseINT(void);
INT8U MCU_GetPwrChkFlag(void);
INT8U MCU_GetPwrState (void);
void MCU_AddTick (void);
INT16U MCU_GetTick (void);
INT8U MCU_CmpTick (INT16U CmpTick,INT16U DelayMs);
INT8U MCU_RunModeSet(INT8U Mode);
INT8U MCU_Maintain(INT8U Mode);
void R_HOFC_Open(void);
void R_HOFC_Stop(void);
void MCU_DelayMs(INT16U DelayMs);
void MCU_DelayUs (INT16U DelayUs);
void MCU_ClkAdj(void);



void MCU_Reset(void)
{
    RCC->SOFTRST = 0x5C5CAABB;
    while(1);
}

INT8U MCU_GetResetSrc(void)
{
    INT8U Ret;
    INT32U temp;
    temp = RCC->RSTFLAG;
    if(temp == 0x0001)
    {
        Ret = Reset_NVICN;
    }
    else if(temp == 0x0002)
    {
        Ret = Reset_LOOKUP;
    }
    else if(temp == 0x0004)
    {
        Ret = Reset_WWDTN;
    }
    else if(temp == 0x0010)
    {
        Ret = Reset_IWDTN;
    }        
    else if(temp == 0x0020)
    {
        Ret = Reset_SOFTN;/*软件复位标识*/
    }
    else if(temp == 0x0040)
    {
        Ret = Reset_EMCN;/*EMC复位标识*/
    }
    else if(temp == 0x0100)
    {
         Ret = Reset_PDRN;/*下电复位标识*/
    }
    else if(temp == 0x0200)
    {
         Ret = Reset_PORN;/*上电复位标识*/
    }
    else if(temp == 0x0400)
    {
         Ret = Reset_TESTN;/*TESTN引脚复位标识*/
    }
    else if(temp == 0x0800)
    {
         Ret = Reset_NRSTN;/*NRSTN引脚复位标识*/
    }
    else if(temp == 0x10000)
    {
         Ret = Reset_MDUN;/*模式诊断电路复位标识*/
    }
    else
    {
        Ret = Reset_Other;
    }
    return Ret;
}

static ISR_FNCT_VOID App_PD_Fun;

/*"************************************************			
Function: 	GP_PD_ISR		
Description: 掉电及上电 中断处理函数
Input:  
Output: 
Return: TRUE: 已经掉电  FALSE :没有掉电
Others: 
************************************************"*/
void MCU_PD_Deal(void)
{
    if(PowerState== PWRMODE_UP)/*"上电状态进掉电中断"*/
    {
        if(PWRMODE_DOWN== MCU_GetPwrState())
        {
            MCU_DelayMs(1);/*"先延时1ms 再判断是上电还是掉电"*/
            /*"affirm the power down"*/
            if(PWRMODE_DOWN== MCU_GetPwrState())
            {
                MCU_DelayMs(1);
                if(PWRMODE_DOWN == MCU_GetPwrState())
                {
                    MCU_DelayMs(1);
                    if(PWRMODE_DOWN == MCU_GetPwrState())
                    {
                        MCU_DelayMs(1);
                        if(PWRMODE_DOWN == MCU_GetPwrState())
                        {
                            MCU_DelayMs(1);
                            if(PWRMODE_DOWN == MCU_GetPwrState())
                            {
                                App_PD_Fun();
                            }
                        }
                    }
                }
            }
        }
    }
}


static INT8U PwrChkLockFlg=0;
static INT8U AllIntLockFlg=0;
INT8U MCU_PwrChkInit(ISR_FNCT_VOID IsrFun)
{
    PwrChkLockFlg = 0;
    if(AllIntLockFlg>1)/*"防错"*/
    {
        AllIntLockFlg=1;
    }

    GPIO_SetMulti(PD_DECTECT, GPIO_Mode_ANA);
    INT_Dis(E_INT_ID_SVD);

	if(PWRMODE_UP == PowerState)/*上电模式*/
	{
        App_PD_Fun = IsrFun;

        ANAC->SVDCFG = 0x000002F0;	/*使能电源跌落中断*/
        ANAC->SVDCON = 1;
        MCU_DelayUs(200);
        ANAC->SVDSIF = 0x00000003;/*清除中断标志位*/
        INT_VectSet(E_INT_ID_SVD, MCU_PD_Deal);
        INT_En(E_INT_ID_SVD);

    }
    else/*低功耗降低功耗*/
    {
        //ANAC->SVDCFG = 0x000001FC;	/*使能电源恢复中断,15.625ms间歇数字滤波*/
        ANAC->SVDCFG = 0x000001F0;/*低功耗下使能电源恢复中断*/
        ANAC->SVDCON = 1;
        MCU_DelayUs(200);
        ANAC->SVDSIF = 0x00000003;/*清除中断标志位*/
        //INT_VectSet(E_INT_ID_SVD, MCU_PD_Deal);/*不注册中断函数，中断用于唤醒*/
        INT_En(E_INT_ID_SVD);
    }
   
    return TRUE;
}

void MCU_PwrChkOpenINT(void)  
{
    if(PwrChkLockFlg>0)
    {
        PwrChkLockFlg--;
    }
    else
    {
    }
    if(PwrChkLockFlg == 0)
    {
        INT_En(E_INT_ID_SVD);
    }
}

void MCU_PwrChkCloseINT(void)  //掉电检测关中断
{
    PwrChkLockFlg++;
    INT_Dis(E_INT_ID_SVD);
}

void MCU_PwrChkOpen(void)
{
    ANAC->SVDCON = 1;
    MCU_DelayUs(120);/*手册上提出输出到稳定约需要100us*/
}

void MCU_PwrChkClose(void)
{
    ANAC->SVDCON = 0;
}


/*"************************************************			
Function: 	MCU_GetPwrChkFlag		
Description: 获取掉电检测锁状态
Input:  
Output: 
Return: PwrChkLockFlg
Others: 
************************************************"*/
INT8U MCU_GetPwrChkFlag(void)  
{
    return PwrChkLockFlg;
}


/*"开总中断"*/
void MCU_IntEnableAll(void)  
{
    if(AllIntLockFlg>0)
    {
        AllIntLockFlg--;
        if(AllIntLockFlg == 0)
        {
            INT_ENABLE_ALL;
        }        
    }
    else
    {
        INT_ENABLE_ALL;
    }
}
/*"关总中断"*/
void MCU_IntDisableAll(void)  
{
    AllIntLockFlg++;
    INT_DISABLE_ALL;
}

INT8U MCU_GetPwrState (void)
{
    INT8U Ret;
    if(ANAC->SVDSIF & 0x00000100)  	
    {    
        Ret = PWRMODE_UP; 
    }
    else      
    {    
        Ret = PWRMODE_DOWN;
    }
    return Ret;
}

void MCU_AddTick (void)
{
    sysTick += SYS_TICK_MS;
}

INT16U MCU_GetTick (void)
{
    return sysTick;
}

INT8U MCU_CmpTick (INT16U CmpTick,INT16U DelayMs)
{
    if(((INT16U)(sysTick-CmpTick))>=DelayMs)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/** @defgroup RCC_PERRCCCON1_Selection
  * @{
  */
#define RCC_PERRCCCON1_LPTRCCEN_Pos               	0                                            
#define RCC_PERRCCCON1_LPTRCCEN_Msk              		(0x01UL << RCC_PERRCCCON1_LPTRCCEN_Pos) 
#define RCC_PERRCCCON1_LPTRCCEN_ON									((INT32U)0x00000001)
#define RCC_PERRCCCON1_LPTRCCEN_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON1_LPTFCCEN_Pos               	1                                           
#define RCC_PERRCCCON1_LPTFCCEN_Msk              		(0x01UL << RCC_PERRCCCON1_LPTRCCEN_Pos) 
#define RCC_PERRCCCON1_LPTFCCEN_ON									((INT32U)0x00000002)
#define RCC_PERRCCCON1_LPTFCCEN_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON1_RTCRCCEN_Pos               	2                                            
#define RCC_PERRCCCON1_RTCRCCEN_Msk              		(0x01UL << RCC_PERRCCCON1_RTCRCCEN_Pos) 
#define RCC_PERRCCCON1_RTCRCCEN_ON									((INT32U)0x00000004)
#define RCC_PERRCCCON1_RTCRCCEN_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON1_PMURCCEN_Pos               	3                                            
#define RCC_PERRCCCON1_PMURCCEN_Msk              		(0x01UL << RCC_PERRCCCON1_PMURCCEN_Pos)  
#define RCC_PERRCCCON1_PMURCCEN_ON									((INT32U)0x00000008)
#define RCC_PERRCCCON1_PMURCCEN_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON1_SCUCKEN_Pos               		4                                            
#define RCC_PERRCCCON1_SCUCKEN_Msk              		(0x01UL << RCC_PERRCCCON1_SCUCKEN_Pos) 
#define RCC_PERRCCCON1_SCUCKEN_ON               		((INT32U)0x00000010)                                            
#define RCC_PERRCCCON1_SCUCKEN_OFF              		((INT32U)0x00000000)

#define RCC_PERRCCCON1_IWDTCKEN_Pos               	5                                            
#define RCC_PERRCCCON1_IWDTCKEN_Msk              		(0x01UL << RCC_PERRCCCON1_PMURCCEN_Pos) 
#define RCC_PERRCCCON1_IWDTCKEN_ON               		((INT32U)0x00000020)                                            
#define RCC_PERRCCCON1_IWDTCKEN_OFF              		((INT32U)0x00000000)

#define RCC_PERRCCCON1_ANACKEN_Pos               		6                                            
#define RCC_PERRCCCON1_ANACKEN_Msk              		(0x01UL << RCC_PERRCCCON1_PMURCCEN_Pos) 
#define RCC_PERRCCCON1_ANACKEN_ON               		((INT32U)0x00000040)                                            
#define RCC_PERRCCCON1_ANACKEN_OFF              		((INT32U)0x00000000)

#define RCC_PERRCCCON1_PDCCKEN_Pos               		7                                            
#define RCC_PERRCCCON1_PDCCKEN_Msk              		(0x01UL << RCC_PERRCCCON1_PMURCCEN_Pos) 
#define RCC_PERRCCCON1_PDCCKEN_ON               		((uint32_t)0x00000080)                                            
#define RCC_PERRCCCON1_PDCCKEN_OFF              		((uint32_t)0x00000000)

#define RCC_PERRCCCON1_EXTICKEN0_Pos               	8                                            
#define RCC_PERRCCCON1_EXTICKEN0_Msk              	(0x01UL << RCC_PERRCCCON1_EXTICKEN0_Pos) 
#define RCC_PERRCCCON1_EXTICKEN0_ON               	((INT32U)0x00000100)                                            
#define RCC_PERRCCCON1_EXTICKEN0_OFF              	((INT32U)0x00000000)

#define RCC_PERRCCCON1_EXTICKEN1_Pos               	9                                            
#define RCC_PERRCCCON1_EXTICKEN1_Msk              	(0x01UL << RCC_PERRCCCON1_EXTICKEN1_Pos) 
#define RCC_PERRCCCON1_EXTICKEN1_ON               	((INT32U)0x00000200)                                            
#define RCC_PERRCCCON1_EXTICKEN1_OFF              	((INT32U)0x00000000)

#define RCC_PERRCCCON1_EXTICKEN2_Pos               	10                                            
#define RCC_PERRCCCON1_EXTICKEN2_Msk              	(0x01UL << RCC_PERRCCCON1_EXTICKEN2_Pos) 
#define RCC_PERRCCCON1_EXTICKEN2_ON               	((INT32U)0x00000400)                                            
#define RCC_PERRCCCON1_EXTICKEN2_OFF              	((INT32U)0x00000000)

#define RCC_PERRCCCON1_DCUCKEN_Pos               		31                                            
#define RCC_PERRCCCON1_DCUCKEN_Msk              		(0x01UL << RCC_PERRCCCON1_DCUCKEN_Pos) 
#define RCC_PERRCCCON1_DCUCKEN_ON               		((INT32U)0x80000000)                                            
#define RCC_PERRCCCON1_DCUCKEN_OFF              		((INT32U)0x00000000)

/**
  * @}
  */ 

/** @defgroup RCC_PERRCCCON2_Selection
  * @{
  */
#define RCC_PERRCCCON2_CRCCKE_Pos               		0                                            
#define RCC_PERRCCCON2_CRCCKE_Msk              			(0x01UL << RCC_PERRCCCON2_CRCCKE_Pos)  
#define RCC_PERRCCCON2_CRCCKE_ON										((INT32U)0x00000001)
#define RCC_PERRCCCON2_CRCCKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON2_RNGCKE_Pos               		1                                            
#define RCC_PERRCCCON2_RNGCKE_Msk              			(0x01UL << RCC_PERRCCCON2_RNGCKE_Pos)  
#define RCC_PERRCCCON2_RNGCKE_ON										((INT32U)0x00000002)
#define RCC_PERRCCCON2_RNGCKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON2_AESCKE_Pos               		2                                            
#define RCC_PERRCCCON2_AESCKE_Msk              			(0x01UL << RCC_PERRCCCON2_AESCKE_Pos) 
#define RCC_PERRCCCON2_AESCKE_ON										((INT32U)0x00000004)
#define RCC_PERRCCCON2_AESCKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON2_LCDCKE_Pos               		3                                            
#define RCC_PERRCCCON2_LCDCKE_Msk              			(0x01UL << RCC_PERRCCCON2_LCDCKE_Pos)  
#define RCC_PERRCCCON2_LCDCKE_ON										((INT32U)0x00000008)
#define RCC_PERRCCCON2_LCDCKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON2_DMACKE_Pos               		4                                            
#define RCC_PERRCCCON2_DMACKE_Msk              			(0x01UL << RCC_PERRCCCON2_DMACKE_Pos)  
#define RCC_PERRCCCON2_DMACKE_ON										((INT32U)0x00000010)
#define RCC_PERRCCCON2_DMACKE_OFF										((uint32_t)0x00000000)

#define RCC_PERRCCCON2_FLSCKE_Pos               		5                                            
#define RCC_PERRCCCON2_FLSCKE_Msk              			(0x01UL << RCC_PERRCCCON2_FLSCKE_Pos) 
#define RCC_PERRCCCON2_FLSCKE_ON										((INT32U)0x00000020)
#define RCC_PERRCCCON2_FLSCKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON2_RAMBISTCKE_Pos             	6                                            
#define RCC_PERRCCCON2_RAMBISTCKE_Msk              	(0x01UL << RCC_PERRCCCON2_RAMBISTCKE_Pos)  
#define RCC_PERRCCCON2_RAMBISTCKE_ON								((INT32U)0x00000040)
#define RCC_PERRCCCON2_RAMBISTCKE_OFF								((INT32U)0x00000000)

#define RCC_PERRCCCON2_WWDTCKE_Pos               		7                                            
#define RCC_PERRCCCON2_WWDTCKE_Msk              		(0x01UL << RCC_PERRCCCON2_WWDTCKE_Pos) 
#define RCC_PERRCCCON2_WWDTCKE_ON										((INT32U)0x00000080)
#define RCC_PERRCCCON2_WWDTCKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON2_ADCCKE_Pos               		8                                            
#define RCC_PERRCCCON2_ADCCKE_Msk              			(0x01UL << RCC_PERRCCCON2_ADCCKE_Pos)  
#define RCC_PERRCCCON2_ADCCKE_ON										((INT32U)0x00000100)
#define RCC_PERRCCCON2_ADCCKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON2_ADCCKSEL_Pos               	16                                            
#define RCC_PERRCCCON2_ADCCKSEL_Msk              		(0x07UL << RCC_PERRCCCON2_ADCCKSEL_Pos)   
#define RCC_PERRCCCON2_ADCCKSEL_RCHF								((INT32U)0x00000000)
#define RCC_PERRCCCON2_ADCCKSEL_RCHFDIV2						((INT32U)0x00010000)
#define RCC_PERRCCCON2_ADCCKSEL_RCHFDIV4						((INT32U)0x00020000)
#define RCC_PERRCCCON2_ADCCKSEL_RCHFDIV8						((INT32U)0x00030000)
#define RCC_PERRCCCON2_ADCCKSEL_RCHFDIV16						((INT32U)0x00040000)
#define RCC_PERRCCCON2_ADCCKSEL_RCHFDIV32						((INT32U)0x00050000)
#define RCC_PERRCCCON2_ADCCKSEL_RCHFDIV64						((INT32U)0x00060000)
#define RCC_PERRCCCON2_ADCCKSEL_RCLP								((INT32U)0x00070000)


/**
  * @}
  */ 

/** @defgroup RCC_PERRCCCON3_Selection
  * @{
  */
#define RCC_PERRCCCON3_SPI1CKE_Pos               		0                                            
#define RCC_PERRCCCON3_SPI1CKE_Msk              		(0x01UL << RCC_PERRCCCON3_SPI1CKE_Pos)  
#define RCC_PERRCCCON3_SPI1CKE_ON										((INT32U)0x00000001)
#define RCC_PERRCCCON3_SPI1CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_SPI2CKE_Pos               		1                                            
#define RCC_PERRCCCON3_SPI2CKE_Msk              		(0x01UL << RCC_PERRCCCON3_SPI2CKE_Pos) 
#define RCC_PERRCCCON3_SPI2CKE_ON										((INT32U)0x00000002)
#define RCC_PERRCCCON3_SPI2CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_HSPICKE_Pos               		2                                            
#define RCC_PERRCCCON3_HSPICKE_Msk              		(0x01UL << RCC_PERRCCCON3_HSPICKE_Pos) 
#define RCC_PERRCCCON3_HSPICKE_ON										((INT32U)0x00000004)
#define RCC_PERRCCCON3_HSPICKE_OFF									((INT32U)0x00000000)


#define RCC_PERRCCCON3_UART0CKE_Pos               	8                                            
#define RCC_PERRCCCON3_UART0CKE_Msk              		(0x01UL << RCC_PERRCCCON3_UART0CKE_Pos) 
#define RCC_PERRCCCON3_UART0CKE_ON									((INT32U)0x00000100)
#define RCC_PERRCCCON3_UART0CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_UART1CKE_Pos               	9                                            
#define RCC_PERRCCCON3_UART1CKE_Msk              		(0x01UL << RCC_PERRCCCON3_UART1CKE_Pos)  
#define RCC_PERRCCCON3_UART1CKE_ON									((INT32U)0x00000200)
#define RCC_PERRCCCON3_UART1CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_UART2CKE_Pos               	10                                            
#define RCC_PERRCCCON3_UART2CKE_Msk              		(0x01UL << RCC_PERRCCCON3_UART2CKE_Pos) 
#define RCC_PERRCCCON3_UART2CKE_ON									((INT32U)0x00000400)
#define RCC_PERRCCCON3_UART2CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_UART3CKE_Pos               	11                                            
#define RCC_PERRCCCON3_UART3CKE_Msk              		(0x01UL << RCC_PERRCCCON3_UART3CKE_Pos) 
#define RCC_PERRCCCON3_UART3CKE_ON									((INT32U)0x00000800)
#define RCC_PERRCCCON3_UART3CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_UART4CKE_Pos               	12                                            
#define RCC_PERRCCCON3_UART4CKE_Msk              		(0x01UL << RCC_PERRCCCON3_UART4CKE_Pos) 
#define RCC_PERRCCCON3_UART4CKE_ON									((INT32U)0x00001000)
#define RCC_PERRCCCON3_UART4CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_UART5CKE_Pos               	13                                            
#define RCC_PERRCCCON3_UART5CKE_Msk              		(0x01UL << RCC_PERRCCCON3_UART5CKE_Pos)
#define RCC_PERRCCCON3_UART5CKE_ON									((INT32U)0x00002000)
#define RCC_PERRCCCON3_UART5CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON3_UARTCCKE_Pos               	14                                            
#define RCC_PERRCCCON3_UARTCCKE_Msk              		(0x01UL << RCC_PERRCCCON3_UARTCCKE_Pos)
#define RCC_PERRCCCON3_UARTCCKE_ON									((INT32U)0x00004000)
#define RCC_PERRCCCON3_UARTCCKE_OFF									((INT32U)0x00000000)


#define RCC_PERRCCCON3_U78160CKE_Pos               	16                                            
#define RCC_PERRCCCON3_U78160CKE_Msk              	(0x01UL << RCC_PERRCCCON3_U78160CKE_Pos) 
#define RCC_PERRCCCON3_U78160CKE_ON									((INT32U)0x00010000)
#define RCC_PERRCCCON3_U78160CKE_OFF								((INT32U)0x00000000)

#define RCC_PERRCCCON3_U78161CKE_Pos               	17                                            
#define RCC_PERRCCCON3_U78161CKE_Msk              	(0x01UL << RCC_PERRCCCON3_U78161CKE_Pos)  
#define RCC_PERRCCCON3_U78161CKE_ON									((INT32U)0x00020000)
#define RCC_PERRCCCON3_U78161CKE_OFF								((INT32U)0x00000000)


#define RCC_PERRCCCON3_I2CCKE_Pos               		24                                            
#define RCC_PERRCCCON3_I2CCKE_Msk              			(0x01UL << RCC_PERRCCCON3_I2CCKE_Pos) 
#define RCC_PERRCCCON3_I2CCKE_ON										((INT32U)0x01000000)
#define RCC_PERRCCCON3_I2CCKE_OFF										((INT32U)0x00000000)


/**
  * @}
  */ 

/** @defgroup RCC_PERRCCCON4_Selection
  * @{
  */
#define RCC_PERRCCCON4_BT12CKE_Pos               		0                                            
#define RCC_PERRCCCON4_BT12CKE_Msk              		(0x01UL << RCC_PERRCCCON4_BT12CKE_Pos)  
#define RCC_PERRCCCON4_BT12CKE_ON										((INT32U)0x00000001)
#define RCC_PERRCCCON4_BT12CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON4_BT34CKE_Pos               		1                                            
#define RCC_PERRCCCON4_BT34CKE_Msk              		(0x01UL << RCC_PERRCCCON4_BT34CKE_Pos)  
#define RCC_PERRCCCON4_BT34CKE_ON										((INT32U)0x00000002)
#define RCC_PERRCCCON4_BT34CKE_OFF									((INT32U)0x00000000)

#define RCC_PERRCCCON4_ET1CKE_Pos               		2                                            
#define RCC_PERRCCCON4_ET1CKE_Msk              			(0x01UL << RCC_PERRCCCON4_ET1CKE_Pos)
#define RCC_PERRCCCON4_ET1CKE_ON										((INT32U)0x00000004)
#define RCC_PERRCCCON4_ET1CKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON4_ET2CKE_Pos               		3                                            
#define RCC_PERRCCCON4_ET2CKE_Msk              			(0x01UL << RCC_PERRCCCON4_ET2CKE_Pos) 
#define RCC_PERRCCCON4_ET2CKE_ON										((INT32U)0x00000008)
#define RCC_PERRCCCON4_ET2CKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON4_ET3CKE_Pos               		4                                            
#define RCC_PERRCCCON4_ET3CKE_Msk              			(0x01UL << RCC_PERRCCCON4_ET3CKE_Pos) 
#define RCC_PERRCCCON4_ET3CKE_ON										((INT32U)0x00000010)
#define RCC_PERRCCCON4_ET3CKE_OFF										((INT32U)0x00000000)

#define RCC_PERRCCCON4_ET4CKE_Pos               		5                                            
#define RCC_PERRCCCON4_ET4CKE_Msk              			(0x01UL << RCC_PERRCCCON4_ET4CKE_Pos) 
#define RCC_PERRCCCON4_ET4CKE_ON										((INT32U)0x00000020)
#define RCC_PERRCCCON4_ET4CKE_OFF										((INT32U)0x00000000)


static void RCC_PERIPH_clk_enable(Periph_Type periph_num)
{
    switch(periph_num)
    {
        //PERRCCCON1 SETTING
        case SCU_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_SCUCKEN_ON;
            break;				
        case PMU_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_PMURCCEN_ON;
            break;				
        case RTC_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_RTCRCCEN_ON;
            break;				
        case LPTRC_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_LPTRCCEN_ON;
            break;
        case LPTFC_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_LPTFCCEN_ON;
            break;
        case IWDT_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_IWDTCKEN_ON;
            break;
        case ANA_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_ANACKEN_ON;
            break;
        case PDC_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_PDCCKEN_ON;
            break;	
        case EXTI0_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_EXTICKEN0_ON;
            break;
        case EXTI1_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_EXTICKEN1_ON;
            break;				
        case EXTI2_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_EXTICKEN2_ON;
            break;	
        case DCU_num:
            RCC->PERRCCCON1 |= RCC_PERRCCCON1_DCUCKEN_ON;
            break;	
				
				
        //PERRCCCON2 SETTING
        case CRC_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_CRCCKE_ON;
            break;
        case RNG_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_RNGCKE_ON;
            break;
        case ADC_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_ADCCKE_ON;
            break;
        case LCD_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_LCDCKE_ON;
            break;
        case DMA_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_DMACKE_ON;
            break;
        case FLS_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_FLSCKE_ON;
            break;
        case AES_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_AESCKE_ON;
            break;
        case RAMBIST_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_RAMBISTCKE_ON;
            break;
        case WWDT_num:
            RCC->PERRCCCON2 |= RCC_PERRCCCON2_WWDTCKE_ON;
            break;
				
				
        //PERRCCCON3 SETTING
        case HSPI_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_HSPICKE_ON;
            break;
        case SPI1_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_SPI1CKE_ON;
            break;
        case SPI2_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_SPI2CKE_ON;
            break;
        case UART0_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UART0CKE_ON;
            break;
        case UART1_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UART1CKE_ON;
            break;
        case UART2_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UART2CKE_ON;
            break;
        case UART3_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UART3CKE_ON;
            break;
        case UART4_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UART4CKE_ON;
            break;
        case UART5_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UART5CKE_ON;
            break;
        case UART_COM_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_UARTCCKE_ON;
            break;	
        case U78160_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_U78160CKE_ON;
            break;
        case U78161_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_U78161CKE_ON;
            break;					
        case I2C_num:
            RCC->PERRCCCON3 |= RCC_PERRCCCON3_I2CCKE_ON;
            break;
				
        //PERRCCCON4 SETTING
        case BT12_num:
            RCC->PERRCCCON4 |= RCC_PERRCCCON4_BT12CKE_ON;
            break;
        case BT34_num:
            RCC->PERRCCCON4 |= RCC_PERRCCCON4_BT34CKE_ON;
            break;
        case ET1_num:
            RCC->PERRCCCON4 |= RCC_PERRCCCON4_ET1CKE_ON;
            break;
        case ET2_num:
            RCC->PERRCCCON4 |= RCC_PERRCCCON4_ET2CKE_ON;
            break;
        case ET3_num:
            RCC->PERRCCCON4 |= RCC_PERRCCCON4_ET3CKE_ON;
            break;
        case ET4_num:
            RCC->PERRCCCON4 |= RCC_PERRCCCON4_ET4CKE_ON;
            break;
								
        default:
            break;
    }
}

static void RCC_PERIPH_clk_disable(Periph_Type periph_num)
{
    switch(periph_num)
    {
				//PERRCCCON1 SETTING
        case SCU_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_SCUCKEN_ON;
            break;				
        case PMU_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_PMURCCEN_ON;
            break;				
        case RTC_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_RTCRCCEN_ON;
            break;				
        case LPTRC_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_LPTRCCEN_ON;
            break;
        case LPTFC_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_LPTFCCEN_ON;
            break;
        case IWDT_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_IWDTCKEN_ON;
            break;
        case ANA_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_ANACKEN_ON;
            break;
        case PDC_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_PDCCKEN_ON;
            break;
        case EXTI0_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_EXTICKEN0_ON;
            break;
        case EXTI1_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_EXTICKEN1_ON;
            break;				
        case EXTI2_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_EXTICKEN2_ON;
            break;	
        case DCU_num:
            RCC->PERRCCCON1 &= ~RCC_PERRCCCON1_DCUCKEN_ON;
            break;	
				
        //PERRCCCON2 SETTING
        case CRC_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_CRCCKE_ON;
            break;
        case RNG_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_RNGCKE_ON;
            break;
        case ADC_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_ADCCKE_ON;
            break;
        case LCD_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_LCDCKE_ON;
            break;
        case DMA_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_DMACKE_ON;
            break;
        case FLS_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_FLSCKE_ON;
            break;
        case AES_num:
            RCC->PERRCCCON2 &= ~RCC_PERRCCCON2_AESCKE_ON;
            break;
				
        //PERRCCCON3 SETTING
        case HSPI_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_HSPICKE_ON;
            break;
        case SPI1_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_SPI1CKE_ON;
            break;
        case SPI2_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_SPI2CKE_ON;
            break;
        case UART0_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_UART0CKE_ON;
            break;
        case UART1_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_UART1CKE_ON;
            break;
        case UART2_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_UART2CKE_ON;
            break;
        case UART3_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_UART3CKE_ON;
            break;
        case UART4_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_UART4CKE_ON;
            break;
        case UART5_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_UART5CKE_ON;
            break;
        case U78160_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_U78160CKE_ON;
            break;
        case U78161_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_U78161CKE_ON;
            break;
        case I2C_num:
            RCC->PERRCCCON3 &= ~RCC_PERRCCCON3_I2CCKE_ON;
            break;
				
        //PERRCCCON3 SETTING
        case BT12_num:
            RCC->PERRCCCON4 &= ~RCC_PERRCCCON4_BT12CKE_ON;
            break;
        case BT34_num:
            RCC->PERRCCCON4 &= ~RCC_PERRCCCON4_BT34CKE_ON;
            break;
        case ET1_num:
            RCC->PERRCCCON4 &= ~RCC_PERRCCCON4_ET1CKE_ON;
            break;
        case ET2_num:
            RCC->PERRCCCON4 &= ~RCC_PERRCCCON4_ET2CKE_ON;
            break;
        case ET3_num:
            RCC->PERRCCCON4 &= ~RCC_PERRCCCON4_ET3CKE_ON;
            break;
        case ET4_num:
            RCC->PERRCCCON4 &= ~RCC_PERRCCCON4_ET4CKE_ON;
            break;
								
        default:
            break;
    }
}


void MCU_PeriphClkSet(Periph_Type periph_num,FunctionalState NewState)
{
    if (NewState != DISABLE)
    {
        RCC_PERIPH_clk_enable(periph_num);
    }
    else
    {
        RCC_PERIPH_clk_disable(periph_num);
    }

}


#define ADDR_TRIM_8M 	(0x1FFFFD40) //RC8M 校准值HXVAR( uint32, 0x1FFFFD40 )	//RC8M 校准值
#define ADDR_TRIM_16M 	(0x1FFFFD3C)//RC16M 校准值HXVAR( uint32, 0x1FFFFD3C )	//RC16M 校准值
#define ADDR_TRIM_24M 	(0x1FFFFD38)//RC24M 校准值HXVAR( uint32, 0x1FFFFD38 )	
INT8U MCU_RunModeSet(INT8U Mode)
{    	
    if(Mode == MCU_RUNMODE)
    {
        sysTick=1;/*保证sysTick永远不为0*/
        /*停震检测到也没有实际有效方式触发起震，起震后会自动切换。因此检测停震也没有意义*/
        
    
        /*休眠模式开启RCLP,使能EXTI中断采用，AHB不分频,APB 2分频,，系统时钟选择RCHF*/
        RCC->SYSRCCSEL = 0x86040000; 
        RCC->RCHFCON   = 0x000001;        /*RCHF选择8M,并使能RCHF*/
        RCC->RCLPCON   = 0x000000;       /*开启RCLP*/
        RCC->XTLFIPW   = 0x000000;      /*推荐使用450nA*/

       // RCC->RCHFTRIM = *((INT32U*)ADDR_TRIM_8M);/*RCHF修调*/
    
        RCC->PLLCON = 0x01F30001;/*使用默认16.384M 只是开启PLL*/ 
            
        RCC->PERRCCCON1 =   RCC_PERRCCCON1_DCUCKEN_ON /*debug contorl*/
                            |RCC_PERRCCCON1_EXTICKEN0_ON
                            |RCC_PERRCCCON1_EXTICKEN1_ON
                            |RCC_PERRCCCON1_EXTICKEN2_ON
                            |RCC_PERRCCCON1_PDCCKEN_ON /*GPIO时钟*/
                            |RCC_PERRCCCON1_ANACKEN_ON /*模拟设备时钟*/
                	        |RCC_PERRCCCON1_IWDTCKEN_ON/*WDT时钟*/
                	        |RCC_PERRCCCON1_SCUCKEN_ON /*system control*/
                            |RCC_PERRCCCON1_PMURCCEN_ON /*power management unit*/
                            |RCC_PERRCCCON1_RTCRCCEN_ON;/*RTC时钟*/
                            //|RCC_PERRCCCON1_LPTFCCEN_ON
                            //|RCC_PERRCCCON1_LPTRCCEN_ON;
        RCC->PERRCCCON2 = 0x00000000;/*先全关闭一下*/
        RCC->PERRCCCON3 = 0x00000000;/*先全关闭一下*/
        RCC->PERRCCCON4 = 0x00000000;/*先全关闭一下*/
        MCU_DelayMs(1);
		#ifdef DEBUG   
            CloseDebugIWDT();//DEBUG状态关闭看门狗
		#else //正式发布的程序打开 DEBUG看门狗，仿真意外进入DEBUG模式，看门狗不起作用
		    OpenDebugIWDT();
		#endif
        
        SCU->DBGCFG = 0x03;
        ANAC->BORCON = 0x01;/*关闭BOR*/
        ANAC->PDRCON = 0x01;/*使能PDR1.5V*/
        SysClockFrq = CLOCK_24M; 
	    APBClock = CLOCK_12M;
     
        MCU_SetFre(2); /*在此处将主频设置为24M，前面还是8M*/
    	MCU_ClkAdj();
        PowerState = PWRMODE_UP;
    }
    else 
    {

        /*停震检测到也没有实际有效方式触发起震，起震后会自动切换。因此检测停震也没有意义*/
        
    
        /*休眠模式开启RCLP,使能EXTI中断采用，APB不分频,AHB16分频， ，系统时钟选择RCHF*/
        RCC->SYSRCCSEL = 0X02000700; 

        RCC->RCHFCON   = 0x000001;        /*RCHF选择8M,并使能RCHF*/
        RCC->RCLPCON   = 0x000000;       /*开启RCLP*/
        RCC->XTLFIPW   = 0x000000;      /*推荐使用450nA*/

        RCC->PLLCON = 0x00;/*不开启pll*/

        /*如果大于8M,需要有其他的修调方式*/
        RCC->RCHFTRIM = *((INT32U*)ADDR_TRIM_8M);/*RCHF修调*/

        RCC->PERRCCCON1 =   RCC_PERRCCCON1_DCUCKEN_ON
                            //|RCC_PERRCCCON1_EXTICKEN0_ON
                            //|RCC_PERRCCCON1_EXTICKEN1_ON
                            //|RCC_PERRCCCON1_EXTICKEN2_ON
                            |RCC_PERRCCCON1_PDCCKEN_ON
                            |RCC_PERRCCCON1_ANACKEN_ON
                	        //|RCC_PERRCCCON1_IWDTCKEN_ON
                	        |RCC_PERRCCCON1_SCUCKEN_ON
                            |RCC_PERRCCCON1_PMURCCEN_ON
                            |RCC_PERRCCCON1_RTCRCCEN_ON;
                            //|RCC_PERRCCCON1_LPTFCCEN_ON
                            //|RCC_PERRCCCON1_LPTRCCEN_ON;
        RCC->PERRCCCON2 = 0x00000000;/*先全关闭一下*/
        MCU_DelayMs(1);
        RCC->PERRCCCON3 = 0x00000000;/*先全关闭一下*/
        MCU_DelayMs(1);
        RCC->PERRCCCON4 = 0x00000000;/*先全关闭一下*/
        MCU_DelayMs(1);

		#ifdef DEBUG   
            CloseDebugIWDT();//DEBUG状态关闭看门狗
		#else //正式发布的程序打开 DEBUG看门狗，仿真意外进入DEBUG模式，看门狗不起作用
		    OpenDebugIWDT();
		#endif
        SCU->DBGCFG = 0x03;
        ANAC->BORCON = 0x01;/*关闭BOR*/
        ANAC->PDRCON = 0x01;/*使能PDR1.5V*/
            
        SysClockFrq = CLOCK_512K;
	    APBClock = CLOCK_512K;
        PowerState = PWRMODE_DOWN;    
    }

    return TRUE;
}

#define XTLF_CNT_MAX (10) /*连续监测到N次停震报错*/
static INT8U XtlfErrCnt = 0;

#define FDETIF (0x40012824)

INT8U MCU_Maintain(INT8U Mode)
{
    INT32U TmpReg;
    
    TmpReg = *(INT32U*)FDETIF;
    if((TmpReg & 0x00000040) == 0x00)/*XTLF停震*/
    {
        if(XtlfErrCnt < XTLF_CNT_MAX)
        {
            XtlfErrCnt++;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        XtlfErrCnt = 0;
    }
    return TRUE;
}


#define CNT_1MS_8M (1600)
#define CNT_1MS_24M   4750 // 优化等级中等:3400   //4750  优化等级最高:4750
#define CNT_1MS_512K (100) //优化等级中等:70   优化等级最高:100
void MCU_DelayMs(INT16U DelayMs)
{
    INT16U i,j,Count;
    if (SysClockFrq == CLOCK_8M)
    {         
        Count = CNT_1MS_8M; 
    }
	else if(SysClockFrq == CLOCK_24M)
	{
		Count = CNT_1MS_24M;
	}
    else /*512K*/
    {
        Count= CNT_1MS_512K;
    }
    for(i=0;i<DelayMs;i++)
    {
        for(j=0;j<Count;j++)
        {
            NOP;
        }
    }	
}


/*复旦微平台，低功耗下最小刻度为50uS,小于50us延时为50us*/
void MCU_DelayUs (INT16U DelayUs)
{   
    INT16U CntUs = 0;

	if(SysClockFrq == CLOCK_512K)
	{
		while(CntUs < DelayUs)
		{
       		CntUs += 35;
		}
	}
    else if(SysClockFrq == CLOCK_8M)
    {
        for(CntUs = 0;CntUs< DelayUs; CntUs++)
        {  
            NOP;
            NOP;
            NOP;
        }
    }
	else if(SysClockFrq == CLOCK_24M)
	{
		INT16U num;
		for(CntUs = 0; CntUs < DelayUs; CntUs++)/*"优化等级采用High"*/
		{
			NOP;
			for(num = 6; num > 0; num--) 
            { 
                NOP; 
                NOP;
                NOP; 
            }
		}
	}
    else  
    {

    }
}

/*MCU进入sleep
Mode: SLEEP
      DEEP_SLEEP

*/
void MCU_EnterSleep(INT8U Mode)
{
    PMU->IOWKFLAG = 0xFF;
    PMU->LPRIE = 0x00;
    PMU->LPRIF = 0xFF;
    if(SLEEP == Mode)
    {
        PMU->LPMCFG = 0x00060002;/*sleep + LDO15 ON*/
    }
    else
    {
        PMU->LPMCFG = 0x00060202;/*deepsleep  +LDO15 ON 不能显示*/
    }
    __WFI();/*进去休眠*/

}

void MCU_OutSleep(void)
{
    /*唤醒后为8M时钟，需切换到512K*/
    /*休眠模式开启RCLP,使能EXTI中断采用，APB不分,AHB 16分频，系统时钟选择RCHF*/
    RCC->SYSRCCSEL = 0X02000700; 

    RCC->RCHFCON   = 0x00000001;        /*RCHF选择8M,并使能RCHF*/
}


void MCU_SetFre(INT8U fre)
{
	if(fre == 0)
	{
		RCC->RCHFCON	= 0x00000001 ;
		RCC->RCHFTRIM = *((INT32U *)ADDR_TRIM_8M);
	}
	else if(fre == 1)
	{
		RCC->RCHFCON	= 0x00010001 ;
		RCC->RCHFTRIM = *((INT32U *)ADDR_TRIM_16M);
	}
	else
	{
		RCC->RCHFCON	= 0x00020001  ;
		RCC->RCHFTRIM = *((INT32U *)ADDR_TRIM_24M);
	}
}

//xtlf的32768Hz时钟作为参考源
//仅支持24M+APB2分频的情况
//ClkMode 3 = 24M
void Clk_Adj_Proc(INT8U ClkMode)
{

	INT8U n, m;
	float TrimBuf,tempbyte,Temp16,DIVclkmode = 1.0;
	if(MCU_Maintain(MCU_RUNMODE) == FALSE ) //xtlf停振
	{
		//使用测试参数
		//Init_RCHF_Trim(ClkMode);
		MCU_SetFre(2);
		return;
	}
	MCU_PeriphClkSet(ET2_num, ENABLE);
	ETIMER2->ETxCR    = 0x00000044;		//ET1~~ , 捕捉模式,周期捕捉,上沿清零，连续捕捉，上升沿捕捉
	//计数源Fcpu、捕捉源xtlf
	ETIMER2->ETxINSEL = 0x00000008;		//计数源Group1信号=apbclk(Fcpu),捕捉源Group2信号=xtlf
	ETIMER2->ETxPESCALE2 = 127;//捕捉源128分频
	ETIMER2->ETxIE  = 0x00000000;		//禁止ET1中断
	ETIMER2->ETxIF |= 0x00000007;		//bit0：溢出中断 bit1：捕捉中断；
	{
		switch(ClkMode)
		{
			//			case 1://8
			//				ETIMER1->ETxPESCALE1 = 0;//计数源1分频
			//				DIVclkmode		= 1.0;
			//				break;
			//
			//			case 2://16/2
			//				ETIMER1->ETxPESCALE1 = 1;//计数源2分频
			//				DIVclkmode		= 1.0;
			//				break;
			case 3://24/4
				ETIMER2->ETxPESCALE1 = 1;//计数源2分频
				DIVclkmode		= 8.0 / 6.0;
				break;
			default://24/4
				ETIMER2->ETxPESCALE1 = 1;//计数源2分频
				DIVclkmode		= 8.0 / 6.0;;
				break;
		}
		for(m = 0; m < 6; m++)
		{
			ETIMER2->ETxCR |= 0x00000080;	//启动ET
			for(n = 0; n < 2; n++) //理论上判断需要3.906ms = 128/32768 s
			{
				while(1)
				{
					Temp16 = 0;
					MCU_DelayMs(1);
					if(ETIMER2->ETxIF & 0x00000002)
					{
						Temp16 = 1;
						break;
					}
				}
				ETIMER2->ETxIF |= 0x00000002;
			}
			ETIMER2->ETxCR &= (~0x00000080); 	//关闭定时器
			if(Temp16 == 0) { continue; }
			Temp16 = ETIMER2->ETxIVR;
			if((Temp16 > (31250 - 120) / DIVclkmode) && (Temp16 < (31250 + 120) / DIVclkmode)) //return;//修正到理想值直接返回
			{
				break;
			}
			else
			{
				TrimBuf = RCC->RCHFTRIM;
				if(Temp16 > 31250 / DIVclkmode)	//31250 = 8M*128/32768; 156.25 = (15625*2/4*2M)*40k
				{
					Temp16 -= 31250 / DIVclkmode;
					tempbyte = (float)Temp16 / (156.25 / DIVclkmode) + 0.5;
					if(tempbyte > 0x20) { tempbyte = 0x20; }
					if(RCC->RCHFTRIM >= tempbyte)
					{
						RCC->RCHFTRIM -= tempbyte;
					}
					else
					{
						RCC->RCHFTRIM = 0x00;
					}
				}
				else
				{
					Temp16 = 31250 / DIVclkmode - Temp16;
					tempbyte = (float)Temp16 / (156.25 / DIVclkmode) + 0.5;
					if(tempbyte > 0x20) { tempbyte = 0x20; }
					if((RCC->RCHFTRIM + tempbyte) <= 0x7F)
					{
						RCC->RCHFTRIM += tempbyte;
					}
					else
					{
						RCC->RCHFTRIM = 0x7F;
					}
				}
				if(m == 5)				//未找到理想值,采用最后2次的值取平均
				{
					TrimBuf += RCC->RCHFTRIM;
					RCC->RCHFTRIM =(INT32U) (TrimBuf / 2 + 0.5);
				}
			}
		}
	}
}


void MCU_ClkAdj(void)
{
    Clk_Adj_Proc(3);
}

//------------------------------
void VectorAddrSet(void)
{
#ifdef NO_LRM_PROJ_CODE
    SCB->VTOR = 0x6000;//APPLICATION_VECTOR_OFFSET;//fw_upgrade.
#else
    SCB->VTOR = 0x76000;//APPLICATION_VECTOR_OFFSET;//fw_upgrade.
#endif
}


/*打开DEBUG模式下IWDT功能*/
void OpenDebugIWDT(void)
{
	SCU->DBGCFG = 0x02;
}

/*关闭DEBUG模式下IWDT功能*/
void CloseDebugIWDT(void)
{
	SCU->DBGCFG = 0x03;
}

