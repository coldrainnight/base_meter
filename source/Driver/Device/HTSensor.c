/*"***********************************************************		
FileName: HTSensor.c		
Author: ����       
Version : 1.0       Date: 2018.08.07	
Description: ��ʪ�ȴ����� SHT3X �豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
***********************************************************"*/		 
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\IIC.h"
#include "Driver\device\HTSensor.h"

#define  HT_SENSOR_IIC_ADDR  (0x80)//(0x44)       /*"�豸IIC��ַ"*/

#define RETRY_CNT (3)

/*��д����*/
#define IIC_WRITE    (0xFE)     /*"EEP�豸д����slave_addrִ�� �� ����"*/
#define IIC_READ     (0x01)     /*"EEP�豸������slave_addrִ�� �� ����"*/

/*"************************************************			
Function: 	HTSensor_Init	
Description: ��ʼ��	
Input:  
Output: ��	
Return: 
Others: 		
************************************************"*/	

INT8U HTSensor_Init(void)
{
    INT8U Ret;
    INT8U Buf[3];
    INT8U Cnt;
    
    Ret = FALSE;
    IIC_Init(IIC_ID_HTSENSOR);

    IIC_Start(IIC_ID_HTSENSOR);

    Buf[0] = (HT_SENSOR_IIC_ADDR & IIC_WRITE);
    Buf[1] = 0xFE;/*soft reset 0x20;ÿ��0.5��*/
    //Buf[2]= 0x32; /*Repeatability High*/
    
    for(Cnt=0;(Cnt<RETRY_CNT) && (Ret==FALSE);Cnt++)
    {
        Ret = IIC_SendChar(IIC_ID_HTSENSOR, Buf, 2);
    }

    IIC_Stop(IIC_ID_HTSENSOR);
    
    return Ret;
}



/*"************************************************			
Function: 	HTSensor_Init	
Description: CRC-8 0x31 Polynomial(x8 + x5 + x4 + 1)	
Input:  
Output: ��	
Return: 
Others: 		
************************************************"*/	
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
                crc = (crc >> 1) ^ 0x8C;
            }
                else crc >>= 1;
        }
    }
    return crc; 
}

INT8U Crc8_ChkValue2(INT8U *Buf, INT8U len)
{
    INT8U crc;
    INT8U i;
    crc = 0;
    while(len--)
    {
        crc ^= *Buf++;
        for(i = 0;i < 8;i++)
        {
            if(crc & 0x80)
            {
                crc = (crc << 1) ^ 0x31;
            }
                else crc <<= 1;
        }
    }
    return crc; 
}


INT8U Get_HTSensorVal(INT8U t, ST_TH_VAL * ThVal)
{
    INT8U Buf[6];
    INT8U Ret;
    INT16U Rtemp;
    INT8U RHum;
    INT32U Tmp32;
    ST_TH_VAL mThVal;

    if(t == 0)
    {
        IIC_Start(IIC_ID_HTSENSOR);
    
        Buf[0] = (HT_SENSOR_IIC_ADDR & IIC_WRITE);
        Buf[1] = 0xE3;//0x0E;
        //Buf[2] =  0x00; /*Fetch Data command*/
    
        Ret = IIC_SendChar(IIC_ID_HTSENSOR, Buf, 2);
    
        if(Ret == TRUE)
        {
            IIC_Start(IIC_ID_HTSENSOR);
            Buf[0] = (HT_SENSOR_IIC_ADDR | IIC_READ);
            Ret = IIC_SendChar(IIC_ID_HTSENSOR, Buf,1);                      
        }
        else
        {
    
        }
    }
    else
    {      
        Ret=TRUE;
        IIC_ReadChar(IIC_ID_HTSENSOR, Buf, 3);

        if(Buf[2] == Crc8_ChkValue2(&Buf[0],2))//&&
           //(Buf[5] == Crc8_ChkValue2(&Buf[3],2)) )  
        {
            Rtemp= (Buf[0]<<8) + Buf[1];
            RHum = (Buf[3]<<8) + Buf[4];
            
            Tmp32 = 175*Rtemp*100;
            Tmp32 /=65535;
            mThVal.Temperature = Tmp32 - 4685;

            Tmp32 = 125*100 *RHum;
            Tmp32 /=65536;
            mThVal.Humidity = Tmp32-600;

        }
        else
        {
            Ret=FALSE;
        }
        LIB_MemCpy((INT8U *)&mThVal, (INT8U *)ThVal, sizeof(ST_TH_VAL));
           
    }
    return Ret;
}

		
   
