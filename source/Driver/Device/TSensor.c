/*"***********************************************************		
FileName: TSensor.c		
Author: ����       
Version : 1.0       Date: 2018.12.27	
Description: �¶ȴ����� DS18B20 �豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
***********************************************************"*/		 
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\device\TSensor.h"


INT32S gTempVal[E_TEMP_MAX];
volatile INT32U gLineTempIntCnt;
volatile INT8U gLineTempIntFlag;

/*"************************************************			
Function: 	OneWrieBus_ResetPresence	
Description: 1�����߸�λ�����߱�������	
Input:  
Output: ��	
Return: TRUE:����  FALSE:������
Others: 		
************************************************"*/	
INT8U OneWrieBus_ResetPresence(void)
{
    INT16U WaitCnt;
    INT16U PresenceCnt;
    
    PresenceCnt=0;
    
    /*pulling the 1-Wire bus low for a minimum of 480��s*/
    GPIO_OutputPin(T_SENSOR,LOW);
    GPIO_ConfigPin(T_SENSOR, OUTPUT, OPENDRAIN_OUT);
    MCU_DelayUs(480+96);/*Ԥ��20%����*/
    /*The bus master then releases the bus and goes into receive mode*/
    GPIO_ConfigPin(T_SENSOR, INPUT, NULL_IN);
    
    for(WaitCnt=0;WaitCnt<480;WaitCnt++)
    {
        if(LOW == GPIO_ReadPin(T_SENSOR))
        {
            PresenceCnt++;
        }
        else
        {

        }
    }
    
    if(PresenceCnt>10)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/*"************************************************			
Function: 	OneWrieBus_WriteTimeSlot	
Description: 1������WriteTimeSlot	
Input:  Data 1 or 0
Output: ��	
Return: 
Others: 		
************************************************"*/	
void OneWrieBus_WriteTimeSlot(INT8U Data)
{
    /*pulling the 1-Wire bus low*/
    GPIO_OutputPin(T_SENSOR,LOW);
    GPIO_ConfigPin(T_SENSOR, OUTPUT, OPENDRAIN_OUT);

    /*All write time slots must be a minimum of 60��s in duration with a minimum 
    of a 1��s recovery time between individual write slots.*/

    if(Data == 0)
    {
        /*the bus master must continue to hold the bus low for the duration 
        of the time slot (at least 60��s)*/
        MCU_DelayUs(60);
        /*release*/
        GPIO_ConfigPin(T_SENSOR, INPUT, NULL_IN);
    }
    else
    {
        /*the bus master must release the 1-Wire bus within 15��s,��Ҫ�����Ż�*/
        //GPIO_ConfigPin(T_SENSOR, INPUT, NULL_IN);/*ʱ��ﲻ��15us��Ҫ�Ż�*/
        //BUILD_BUG_ON(T_SENSOR != 0x63);/*����15us��*/
        GPIOG->FCR  &= ~0x000000c0; 
        GPIOG->INEN |=  0x0008;
        GPIOG->ODEN &= ~0x0008;

        /*The DS18B20 samples the 1-Wire bus during a window that 
        lasts from 15��s to 60��s after the master initiates the write time slot.*/
        MCU_DelayUs(60+12);
    }

}


/*"************************************************			
Function: 	OneWrieBus_ReadTimeSlot	
Description: 1������WriteTimeSlot	
Input:  
Output: Data 1 or 0	
Return: 
Others: 		
************************************************"*/	
INT8U OneWrieBus_ReadTimeSlot(void)
{
    INT8U LowCnt;

    LowCnt = 0;

    /*A read time slot is initiated by the master device pulling the 
    1-Wire bus low for a minimum of 1��s and then releasing the bus*/
    GPIO_OutputPin(T_SENSOR,LOW);
    GPIO_ConfigPin(T_SENSOR, OUTPUT, OPENDRAIN_OUT);
    
    //GPIO_ConfigPin(T_SENSOR, INPUT, NULL_IN);
    //BUILD_BUG_ON(T_SENSOR != 0x63);
    GPIOG->INEN |=  0x0008;
    GPIOG->FCR  &= ~0x000000c0; 
    //GPIOB->ODEN &= ~0x8000;
    NOP; NOP; NOP;NOP;
    NOP; NOP; NOP;NOP;

    //BUILD_BUG_ON(T_SENSOR != 0x63);
    if(GPIOG->IDR & 0x0008)/*��*/
    {
    }
    else
    {
        LowCnt++;
    }

    MCU_DelayUs(60);
    
    if(LowCnt>=1)
    {
        return 0;
    }
    else
    {
        return 1;
    }

}


/*"************************************************			
Function: 	OneWrieBus_Write1Byte	
Description: 	
Input:  
Output: 	
Return: 
Others: 		
************************************************"*/	
void OneWrieBus_Write1Byte(INT8U Data)
{
    INT8U BitCnt;
    
    for(BitCnt=0;BitCnt<8;BitCnt++)
    {
        OneWrieBus_WriteTimeSlot(Data&0x01);
        Data>>=1;
    }
    

}


/*"************************************************			
Function: 	OneWrieBus_Read1Byte	
Description: 	
Input:  
Output: 	
Return: 
Others: 		
************************************************"*/	
INT8U OneWrieBus_Read1Byte(void)
{
    
    INT8U BitCnt;
    INT8U Data;

    Data = 0;
    for(BitCnt=0;BitCnt<8;BitCnt++)
    {
        Data>>=1;
    
        if(1 == OneWrieBus_ReadTimeSlot())
        {
             Data |= 0x80;
        }
       
    }
    return Data;
}


/*"************************************************			
Function: 	TSensor_Init	
Description: ��ʼ��	
Input:  
Output: ��	
Return: 
Others: 		
************************************************"*/	

void TSensor_Init(void)
{
    GPIO_ConfigPin(T_SENSOR, INPUT, NULL_IN);

    GPIO_ConfigPin(TL_SENSOR, INPUT, NULL_IN);

    gLineTempIntCnt=0;
    gLineTempIntFlag=0;
}


/*"************************************************			
Function: 	TSensor_GetVal	
Description: ���ش������¶�	
Input:  
Output: ��	
Return: 
Others: 		
************************************************"*/	
#define RETRY_TIME (5)

#define SKIP_ROM_COMMAND        (0xCC)
#define READ_SCRATCHPAD_COMMAND (0xBE)
#define CONVERT_T               (0x44)


static void TSensor_ConverT(void)
{
    INT8U Ret;
    INT8U RetryCnt;

    Ret = FALSE;
    for(RetryCnt=0;RetryCnt<RETRY_TIME;RetryCnt++)
    {
        if(TRUE == OneWrieBus_ResetPresence())
        {
            Ret = TRUE;
            break;
        }
    }
    
    if(Ret == TRUE)
    {
        OneWrieBus_Write1Byte(SKIP_ROM_COMMAND);
        OneWrieBus_Write1Byte(CONVERT_T);
    }

}


INT8U Crc8_ChkValue(INT8U *Buf, INT8U len)
{
    INT8U crc;
    INT8U i;
    crc = 0;
    while(len--)
    {
        crc ^= *Buf++;
        for(i = 0;i < 8;i++)
        {
            if(crc & 0x01)
            {
                crc = (crc >> 1) ^ 0x8c;
            }
                else crc >>= 1;
        }
    }
    return crc; 
}




INT32S Temp32;

static INT32S TSensor_GetVal(void)
{
    INT8U RetryCnt;
    INT8U Buf[9];
    INT8U Cnt;
    
    Temp32 = 0;
    
    for(RetryCnt=0;RetryCnt<RETRY_TIME;RetryCnt++)
    {
        if(TRUE == OneWrieBus_ResetPresence())
        {
            OneWrieBus_Write1Byte(SKIP_ROM_COMMAND);
            OneWrieBus_Write1Byte(READ_SCRATCHPAD_COMMAND);
            for(Cnt=0;Cnt<9;Cnt++)
            {
                Buf[Cnt] = OneWrieBus_Read1Byte();
            }
            
            if(Buf[8] == Crc8_ChkValue(&Buf[0],8))
            {
                Temp32 = (INT16U)(Buf[1]<<8) + Buf[0];
                Temp32 = Temp32 & 0x87FF;/*��������Sλ�������ͳ����޷�����*/
                Temp32 = (Temp32 * 625)/100;
                break;
            }
            else
            {
                NOP;
            }
        }
        else
        {

        }
        
    }

    if((Temp32<10000) && (Temp32>-6000))
    {

    }
    else
    {
        Temp32 = 2500;
    }
    
    return Temp32;
}

static void LineTempIntISR(void)
{ 
}

void TSensorSecTask(void)
{
   ST_GPIO_EXTICONF  mIntInfo;
   INT8U i;
   INT32S d;
   INT32U t;

   TSensor_ConverT();
   gTempVal[E_TEMP_N]=TSensor_GetVal();

   gLineTempIntCnt=0;
   gLineTempIntFlag=0x01;
   
   mIntInfo.PinId=TL_SENSOR;
   mIntInfo.TrigModule=GPIO_EXTI_EDGE_RISING;
   mIntInfo.IntFuction=LineTempIntISR;
   GPIO_EXTI_Enable(&mIntInfo);

    MCU_DelayMs(10);
    if(gLineTempIntCnt > 0)
    {
       GPIO_EXTI_Disable(TL_SENSOR);
       
       gLineTempIntFlag=0;
       
       return;
    }

    for(i=0; i<50; i++)
    {
        MCU_DelayMs(1);
        if(gLineTempIntCnt > 0)
        {
           break;
        }
    }
    if(i >= 50)
    {
       GPIO_EXTI_Disable(TL_SENSOR);
       
       gLineTempIntFlag=0;
       
       return;
    }

     for(i=0; i<10; i++)
     {
        t=gLineTempIntCnt;
        MCU_DelayMs(2);
        if(t == gLineTempIntCnt)
        {
           break;
        }
     }

     d=gLineTempIntCnt*625-500625;
     gTempVal[E_TEMP_L]=d/100;

     GPIO_EXTI_Disable(TL_SENSOR);

     gLineTempIntFlag=0;
     
     return;
}


