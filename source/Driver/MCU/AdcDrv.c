/*"************************************************
Date:2014.03.20
Author:朱鹏飞
Description:
1)温度采样和电池电压采样分开处理;
2)上电模式开启温度采样和电池电压采样;
3)掉电模式仅开启温度测量;
4)由于温度采样和电池电压采样使用同一中断源，因此需
根据通道标志读取采样结构;
5)由于电池电压采样和温度采样每1S采样一次，因此建议
初始化->使能->取结果->禁止流程紧密连接;
************************************************"*/
#include "Driver\MCU\AdcDrv.h"
#include "Driver\MCU\MCU.h"

/*"************************************************
ADC&&Temp采样中断,需将中断函数放入中断.c文件
************************************************"*/

/*"************************************************
Function:Drv_AD_Init AD初始化
In:	mode  上电或掉电初始化
       AdcId  A/D采样通道号
*************************************************"*/
void AC_AdcInit(INT8U mode,INT8U AdcId)
{
    RCC->PERRCCCON2 &= 0x0000FFFF;
    RCC->PERRCCCON2 |= 0x00040000; /*RCHF/16*/
    MCU_PeriphClkSet(ADC_num, ENABLE);
    
    if(AdcId == AC_ADCID_BAT)/*"上电采样电池电压"*/
    {
        /*RCHF/16*/

        /*配置相应引脚为模拟引脚*/
        GPIO_SetMulti(RTC_BVT_VOL, GPIO_Mode_ANA);
        ANAC->ANATESTSEL = 0x0000002D;/*选择外部引脚PC15，并配置buf使能*/
        ANAC->ADCCON = 0x0002;/*用作测量外部电压，不使能*/
        ANAC->ADCTRIM = 0x03FF;
        ANAC->ADCIF =   0x01; /*写1清零*/
    }
    else if(AdcId == AC_ADCID_TEMP)
    {
       
        ANAC->ADCCON = 0x0000;/*ADC 用作温度传感器*/
        ANAC->ADCTRIM = 0x0640;
        ANAC->ADCIF =   0x01; /*写1清零*/
    }
    else if(AdcId ==AC_ADCID_VDD)
    {
        ANAC->ANATESTSEL = 0x00000036;/*VDD */
        ANAC->ADCCON = 0x0002;/*用作测量外部电压，不使能*/
        ANAC->ADCTRIM = 0x03FF;
        ANAC->ADCIF =   0x01; /*写1清零*/
    }
}



/*"************************************************
Function:AC_AdcDisable AD关闭
In:  AC_AdcId:A/D采样通道号
*************************************************"*/
void AC_AdcDisable(INT8U AC_AdcId)
{
    if((AC_AdcId == AC_ADCID_BAT) || (AC_AdcId == AC_ADCID_VDD))
    {
        ANAC->ADCCON &= ~0x00000001;/*关闭ADC*/
    }
    else if(AC_AdcId == AC_ADCID_TEMP)
    {
        ANAC->ADCCON |= ~0x00000001;/*关闭ADC*/               
    }
        
}

#define WAIT_ADC_MS (20)
/*"******************************************************************
Function:AC_AdcEnable AD关闭
In:  AC_AdcId:A/D采样通道号
Description:该函数是为符合接口规范设计，接口规范要
求开启A/D后，返回采样结果。因此，在该函数中打开A/D
延时100us后返回AD值。
*******************************************************************"*/
INT32U AC_AdcEnable(INT8U AdcId)
{
    INT16U Cnt;

    ANAC->ADCCON |= 0x00000001;/*使能ADC*/

    for(Cnt=0;Cnt<WAIT_ADC_MS;Cnt++)/*典型转换时间为4ms*/
    {
        if(ANAC->ADCIF & 0x01)/*转换完成*/
        {
            ANAC->ADCIF =   0x01; /*写1清零*/
            break;
        }
        else
        {
            MCU_DelayMs(1);
        }
    }

    if(Cnt>=WAIT_ADC_MS)
    {
        return 0xFFFFFFFF;
    }
    else
    {
        return ANAC->ADCDATA;
    }
        
}
    


#define K_VAL_L (0x1FFFFC54)
#define K_VAL_H (0x1FFFFC55)
#define B_VAL_L (0x1FFFFC56)
#define B_VAL_H (0x1FFFFC57)

/*获取某路通道电压值*/
/*#define AC_ADCID_BAT          0      */
/*#define AC_ADCID_VDD          2      */

INT16U Cal_VolVal(INT8U AdcId)
{
    INT16U K;
    INT16S B;
    INT32S RetVal;
    INT32U tmpAdcVal;

    AC_AdcInit(MCU_RUNMODE,AdcId);
    tmpAdcVal=  AC_AdcEnable(AdcId);
    AC_AdcDisable(AdcId);

    K = *(INT16U*)K_VAL_L;
    B= *(INT16S*)B_VAL_L;

    if((K>2000) && (K<2400))/*拟合数据合法*/
    {
            
    }
    else/*默认拟合值*/
    {
        K= 2174;
    }

    if(tmpAdcVal< 31)
    {
        RetVal = 0;
    }
    else if(tmpAdcVal< 50)
    {
        RetVal=(tmpAdcVal-30)*5;
    }
    else if(tmpAdcVal< 150)
    {
        RetVal = ((INT32U)(tmpAdcVal*K))/1000;
    }
    else if(tmpAdcVal< 500)
    {
        RetVal = ((INT32U)(tmpAdcVal*98*K))/100000;
    }
    else
    {
        RetVal = ((INT32U)(tmpAdcVal*K))/1000 + B/100;
    }

    if(AdcId == AC_ADCID_BAT)
    {
        RetVal *= 2;/*恢复分压前的数据*/
    }
    else
    {

    }
    
    return (INT16U)RetVal;
}



void AC_CompInit(INT8U mode,INT8U CompId)
{

    if(CompId == AC_COMPID_BAT)
    {
        MCU_PeriphClkSet(ANA_num, ENABLE);

        
        /*PC15*/
        DISP->SEG_EN1 &= ~0x00080;/*不使能seg39*/
        GPIO_SetMulti(RTC_BVT_VOL, GPIO_Mode_ANA);

        ANAC->COMPICR &= ~0x002;/*不使能比较器2中断*/
        ANAC->COMPICR &= ~0x040;/*使能比较器buf*/
        ANAC->COMPICR &= ~0x080;/*不bypass比较器buf*/
        ANAC->COMPICR |= 0x200;/*使能比较器2数字滤波*/

        
        ANAC->COMP2CR = 0x004;/*正级PC15 负 vref 0.8V*/
        ANAC->COMP2CR |= 0x001;/*开启比较器*/
        MCU_DelayUs(20);/*比较器二手册上 显示 建立时间小于15us*/
        
    }
}


INT8U  AC_CompRet(INT8U CompId)
{
    INT8U Ret;
    
    if(CompId == AC_COMPID_BAT)
    {
        if(ANAC->COMP2CR & 0x100)
        {
            Ret = TRUE;
        }
        else
        {
            Ret = FALSE;
        }

    }
    else
    {

    }
    return Ret;

}


void  AC_CompDisable(INT8U CompId)
{

    if(CompId == AC_COMPID_BAT)
    {
        ANAC->COMP2CR &= ~0x001;/*关闭比较器*/
    }
}
