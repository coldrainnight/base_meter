/*"************************************************
Date:2014.03.20
Author:������
Description:
1)�¶Ȳ����͵�ص�ѹ�����ֿ�����;
2)�ϵ�ģʽ�����¶Ȳ����͵�ص�ѹ����;
3)����ģʽ�������¶Ȳ���;
4)�����¶Ȳ����͵�ص�ѹ����ʹ��ͬһ�ж�Դ�������
����ͨ����־��ȡ�����ṹ;
5)���ڵ�ص�ѹ�������¶Ȳ���ÿ1S����һ�Σ���˽���
��ʼ��->ʹ��->ȡ���->��ֹ���̽�������;
************************************************"*/
#include "Driver\MCU\AdcDrv.h"
#include "Driver\MCU\MCU.h"

/*"************************************************
ADC&&Temp�����ж�,�轫�жϺ��������ж�.c�ļ�
************************************************"*/

/*"************************************************
Function:Drv_AD_Init AD��ʼ��
In:	mode  �ϵ������ʼ��
       AdcId  A/D����ͨ����
*************************************************"*/
void AC_AdcInit(INT8U mode,INT8U AdcId)
{
    RCC->PERRCCCON2 &= 0x0000FFFF;
    RCC->PERRCCCON2 |= 0x00040000; /*RCHF/16*/
    MCU_PeriphClkSet(ADC_num, ENABLE);
    
    if(AdcId == AC_ADCID_BAT)/*"�ϵ������ص�ѹ"*/
    {
        /*RCHF/16*/

        /*������Ӧ����Ϊģ������*/
        GPIO_SetMulti(RTC_BVT_VOL, GPIO_Mode_ANA);
        ANAC->ANATESTSEL = 0x0000002D;/*ѡ���ⲿ����PC15��������bufʹ��*/
        ANAC->ADCCON = 0x0002;/*���������ⲿ��ѹ����ʹ��*/
        ANAC->ADCTRIM = 0x03FF;
        ANAC->ADCIF =   0x01; /*д1����*/
    }
    else if(AdcId == AC_ADCID_TEMP)
    {
       
        ANAC->ADCCON = 0x0000;/*ADC �����¶ȴ�����*/
        ANAC->ADCTRIM = 0x0640;
        ANAC->ADCIF =   0x01; /*д1����*/
    }
    else if(AdcId ==AC_ADCID_VDD)
    {
        ANAC->ANATESTSEL = 0x00000036;/*VDD */
        ANAC->ADCCON = 0x0002;/*���������ⲿ��ѹ����ʹ��*/
        ANAC->ADCTRIM = 0x03FF;
        ANAC->ADCIF =   0x01; /*д1����*/
    }
}



/*"************************************************
Function:AC_AdcDisable AD�ر�
In:  AC_AdcId:A/D����ͨ����
*************************************************"*/
void AC_AdcDisable(INT8U AC_AdcId)
{
    if((AC_AdcId == AC_ADCID_BAT) || (AC_AdcId == AC_ADCID_VDD))
    {
        ANAC->ADCCON &= ~0x00000001;/*�ر�ADC*/
    }
    else if(AC_AdcId == AC_ADCID_TEMP)
    {
        ANAC->ADCCON |= ~0x00000001;/*�ر�ADC*/               
    }
        
}

#define WAIT_ADC_MS (20)
/*"******************************************************************
Function:AC_AdcEnable AD�ر�
In:  AC_AdcId:A/D����ͨ����
Description:�ú�����Ϊ���Ͻӿڹ淶��ƣ��ӿڹ淶Ҫ
����A/D�󣬷��ز����������ˣ��ڸú����д�A/D
��ʱ100us�󷵻�ADֵ��
*******************************************************************"*/
INT32U AC_AdcEnable(INT8U AdcId)
{
    INT16U Cnt;

    ANAC->ADCCON |= 0x00000001;/*ʹ��ADC*/

    for(Cnt=0;Cnt<WAIT_ADC_MS;Cnt++)/*����ת��ʱ��Ϊ4ms*/
    {
        if(ANAC->ADCIF & 0x01)/*ת�����*/
        {
            ANAC->ADCIF =   0x01; /*д1����*/
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

/*��ȡĳ·ͨ����ѹֵ*/
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

    if((K>2000) && (K<2400))/*������ݺϷ�*/
    {
            
    }
    else/*Ĭ�����ֵ*/
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
        RetVal *= 2;/*�ָ���ѹǰ������*/
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
        DISP->SEG_EN1 &= ~0x00080;/*��ʹ��seg39*/
        GPIO_SetMulti(RTC_BVT_VOL, GPIO_Mode_ANA);

        ANAC->COMPICR &= ~0x002;/*��ʹ�ܱȽ���2�ж�*/
        ANAC->COMPICR &= ~0x040;/*ʹ�ܱȽ���buf*/
        ANAC->COMPICR &= ~0x080;/*��bypass�Ƚ���buf*/
        ANAC->COMPICR |= 0x200;/*ʹ�ܱȽ���2�����˲�*/

        
        ANAC->COMP2CR = 0x004;/*����PC15 �� vref 0.8V*/
        ANAC->COMP2CR |= 0x001;/*�����Ƚ���*/
        MCU_DelayUs(20);/*�Ƚ������ֲ��� ��ʾ ����ʱ��С��15us*/
        
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
        ANAC->COMP2CR &= ~0x001;/*�رձȽ���*/
    }
}
