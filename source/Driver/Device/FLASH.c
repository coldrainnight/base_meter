/*"***********************************************************		
FileName: FLASH.c		
Author: ��ά       
Version : 1.0       Date: 2017.01.20	
Description: FLASH�豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/		 
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\SPI.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\WDT.h"
#include "Driver\device\FLASH.h"

static INT16U mFlashRwErrTimes ;   /*"Flash��д�������"*/


const INT32U FlashWriteFlag_1=0x10101010;   /*���ڽ����������Ĺ̶���־1*/
const INT32U FlashWriteFlag_2=0x18181818;   /*���ڽ����������Ĺ̶���־2*/

#define WRITE_ENABLE_ID              (0x06)     /*дʹ������ID*/
#define WRITE_DISABLE_ID             (0x04)     /*д��ֹ����ID*/
#define READ_STATUS_REGISTER_ID      (0x05)     /*��״̬�Ĵ�������ID*/
#define WRITE_STATUS_REGISTER_ID     (0x01)     /*д״̬�Ĵ�������ID*/
#define CHIP_ERASE_ID                (0x60)     /*��Ƭ��������ID*/
#define SECTOR_ERASE_ID              (0x20)     /*������������ID*/
#define READ_DATA_ID                 (0x03)     /*����������ID*/
#define WRITE_PAGE_ID                (0x02)     /*ҳд����ID*/
#define READ_MANUFACTURER_AND_DEVICE_ID    (0x90)   /*���豸������ID*/
#define ENTER_4B_MODE_ID             (0xB7)


#define  STATUS_REG_WRITE_DELAYMS    (120)  /*��״̬�Ĵ��������ʱ*/
#define  PAGE_WRITE_DELAYMS         (10)    /*ҳд��ʱ*/
#define  SECTOR_EARSE_DELAYMS      (450)   /*����������ʱ*/
#define  CHIP_EARSE_DELAYMS        (35000)  /*��Ƭ������ʱ*/

#define FLASH_ERR_TIME (3)    /*"�ж�FLASH����ķ�ֵ"*/

/*----------------------------------------------*
 * �ڲ�����ԭ��˵��                             *
 *----------------------------------------------*/
void Flash_CsLow(INT8U FlashId);
void Flash_CsHigh(INT8U FlashId);
INT8U Flash_WriteEnable(void);
INT8U Flash_WriteDisable(void);
INT8U Flash_ReadSta(void);
INT8U Flash_WriteSta(INT8U StaReg);
INT8U Flash_WaitWriteEnd(INT16U MaxDelayMs);
INT8U Flash_PageWrite(INT32U Addr, INT8U *Buff, INT16U Len);
INT8U Flash_CheckSzie(void);

/*"************************************************			
Function: 	Flash_CsLow	
Description: FlashƬѡCS�õ�
Input:  ��
Output: ��	
Return: �� 
Others: 		
************************************************"*/	

void Flash_CsLow(INT8U FlashId)
{
    switch(FlashId)
    { 
	 case SPI_ID_FLASH0:/*��ѡ���豸0��*/
        {
            GPIO_OutputPin(FLASH_CS,LOW);	
            MCU_DelayUs(10);
			
        }
        break;
	
	default:
       break; 
    }
}

/*"************************************************			
Function: 	Flash_CsHigh	
Description: FlashƬѡCS�ø�
Input:  ��
Output: ��	
Return: �� 
Others: 		
************************************************"*/	

void Flash_CsHigh(INT8U FlashId)
{
   switch(FlashId)
    { 
	 case SPI_ID_FLASH0: /*��ѡ���豸0��*/
        {
            GPIO_OutputPin(FLASH_CS,HIGH);
        }
        break;
	default:
        break; 
    }
}


/*"************************************************			
Function: 	Flash_WriteEnable	
Description: дʹ��
Input:  ��
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	


INT8U Flash_WriteEnable(void)
{
    INT8U buf[1];
    
    Flash_CsLow(SPI_ID_FLASH0);

    buf[0] = WRITE_ENABLE_ID; /*write enable*/
    SPI_WriteNByte(buf,SPI_ID_FLASH,1);
    Flash_CsHigh(SPI_ID_FLASH0);

    return Flash_WaitWriteEnd(STATUS_REG_WRITE_DELAYMS);

}


/*"************************************************			
Function: 	Flash_WriteDisable
Description: д��ֹ
Input:  ��
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	

INT8U Flash_WriteDisable(void)
{
    INT8U buf[1];
    
    Flash_CsLow(SPI_ID_FLASH0);

    buf[0] = WRITE_DISABLE_ID; /*write Disable*/
    SPI_WriteNByte(buf,SPI_ID_FLASH,1);
    Flash_CsHigh(SPI_ID_FLASH0);

    return Flash_WaitWriteEnd(STATUS_REG_WRITE_DELAYMS);

}

INT8U Flash_Enter4BMode(void)
{
    INT8U buf[1];
    
    Flash_CsLow(SPI_ID_FLASH0);

    buf[0] = ENTER_4B_MODE_ID; /*write enable*/
    SPI_WriteNByte(buf,SPI_ID_FLASH,1);
    Flash_CsHigh(SPI_ID_FLASH0);

    return Flash_WaitWriteEnd(STATUS_REG_WRITE_DELAYMS);

}


/*"************************************************			
Function: 	Flash_ReadSta
Description: ��״̬�Ĵ���
Input:  ��
Output: ��	
Return: ���ض�����״̬�Ĵ���ֵ
Others: 		
************************************************"*/	

INT8U Flash_ReadSta(void)
{
    INT8U buf[1];
    INT8U sta;
    
    Flash_CsLow(SPI_ID_FLASH0);

    buf[0] = READ_STATUS_REGISTER_ID; /*Read Status Register*/
    SPI_WriteNByte(buf,SPI_ID_FLASH,1);
    
    SPI_ReadNByte(&sta,SPI_ID_FLASH,1);

    Flash_CsHigh(SPI_ID_FLASH0);
    
    return sta;
}


/*"************************************************			
Function: 	Flash_WriteSta
Description: д״̬�Ĵ���
Input:  ��
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	

INT8U Flash_WriteSta(INT8U StaReg)
{
    INT8U buf[2];
    INT8U ret;
    
    Flash_WriteEnable();
    
    Flash_CsLow(SPI_ID_FLASH0);

    buf[0] = WRITE_STATUS_REGISTER_ID; /*write Status Register*/
    buf[1] = StaReg; /*set to */
    ret = SPI_WriteNByte(buf,SPI_ID_FLASH,2);
    
    Flash_CsHigh(SPI_ID_FLASH0);
    Flash_WriteDisable();
    return ret;
}


/*"************************************************			
Function: 	Flash_WaitWriteEnd
Description: �ȴ�д���
Input:  INT16U MaxDelayMs�����ȴ�ʱ��ֵ
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	

INT8U Flash_WaitWriteEnd(INT16U MaxDelayMs)
{
    INT16U Cnt;
    INT8U Sta;
    
    for(Cnt=0;Cnt<MaxDelayMs;Cnt++)
    {
        Sta = Flash_ReadSta();
        if((Sta&0x01) == 0 )/*not in write operation*/
        {
            break;
        }
        WDT_FeedWdg();
	 MCU_DelayMs(1);
    }
	
    if(Cnt >= MaxDelayMs)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}


/*"************************************************			
Function: 	Flash_Init
Description: ��ʼ��FLASH
Input:  INT8U Mode 
             NORMAL_MODE      (0) �����ϵ��ʼ��ģʽ
             FIRST_INIT_MODE  (1) ���γ�ʼ��ģʽ
             PD_INIT_MODE     (2) �͹���ʼ��ģʽ
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U FlashId1;
INT8U FlashId2;
INT8U FlashId3;


INT8U Flash_Init(INT8U Mode)
{
    INT8U ret1;
    INT8U buf[4];
    //INT16U FlashId;
    ret1 = TRUE;
    
    if((Mode == MCU_RUNMODE )|| (Mode == FIRST_PWR_ON))
    {
        GPIO_ConfigPin(FLASH_PWR,OUTPUT,NORMAL_OUT);
		GPIO_OutputPin(FLASH_PWR, LOW);
        SPI_Init(SPI_ID_FLASH,SPI_NORMAL,NULL);

        #if  0 /*���ڵ���*/
        INT8U buf[4];
        INT16U FlashId;
        FlashId = 0;
        /*��ȡID*/
        Flash_CsLow(SPI_ID_FLASH0);
        buf[0]= 0x90; /*REMS*/
        buf[1]= 0x00; /*dummy*/
        buf[2]= 0x00; /*dummy*/
        buf[3]= 0x01; /*manufacturer Id first(0) or device id first(1)*/
        SPI_WriteNByte(buf, SPI_ID_FLASH, 4);
        SPI_ReadNByte((INT8U *)&FlashId , SPI_ID_FLASH, 2);
        Flash_CsHigh(SPI_ID_FLASH0);
        #endif
        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = 0x05;
        SPI_WriteNByte(buf, SPI_ID_FLASH, 1);	
        SPI_ReadNByte((INT8U *)&FlashId1 , SPI_ID_FLASH, 1);
    	Flash_CsHigh(SPI_ID_FLASH0);
        
        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = 0x35;
        SPI_WriteNByte(buf, SPI_ID_FLASH, 1);	
        SPI_ReadNByte((INT8U *)&FlashId2 , SPI_ID_FLASH, 1);
    	Flash_CsHigh(SPI_ID_FLASH0);

        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = 0x15;
        SPI_WriteNByte(buf, SPI_ID_FLASH, 1);	
        SPI_ReadNByte((INT8U *)&FlashId3 , SPI_ID_FLASH, 1);
    	Flash_CsHigh(SPI_ID_FLASH0);
#if 1
        Flash_Enter4BMode();
        
        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = 0x05;
        SPI_WriteNByte(buf, SPI_ID_FLASH, 1);	
        SPI_ReadNByte((INT8U *)&FlashId1 , SPI_ID_FLASH, 1);
    	Flash_CsHigh(SPI_ID_FLASH0);
        
        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = 0x35;
        SPI_WriteNByte(buf, SPI_ID_FLASH, 1);	
        SPI_ReadNByte((INT8U *)&FlashId2 , SPI_ID_FLASH, 1);
    	Flash_CsHigh(SPI_ID_FLASH0);

        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = 0x15;
        SPI_WriteNByte(buf, SPI_ID_FLASH, 1);	
        SPI_ReadNByte((INT8U *)&FlashId3 , SPI_ID_FLASH, 1);
    	Flash_CsHigh(SPI_ID_FLASH0);   
#endif
        if(Mode == FIRST_PWR_ON)
        {
            ret1 = Flash_CheckSzie();
        }
        else
        {

        }
        
    }
    else 
    {
    }

    /*�ú�رգ����0����ֹ�����*/
    EX_Flash_Spi_MI_OUT_0();
    
    return ret1;
}


/*"************************************************			
Function: 	Flash_Read
Description: ��Flash����
Input:  INT32U Addr�������ݵ�ַ
             INT8U *Buff������BUFFERָ��
             INT16U Len�������ݳ���
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U Flash_Read(INT32U Addr, INT8U *Buff, INT16U Len)
{
    UN_FLASH_ADDR TmpAddr;
    INT8U Ret;
    INT8U buf[5];
    
    TmpAddr.Addr = Addr;
    if(((Len + Addr) > FLASH_MAX_ADDR)  ||
        (Len == 0)            ||
        (Buff == NULL))
    {
        return FALSE;
    }
    EX_Flash_Spi_MI_Fun_Mode(); 

    Flash_CsLow(SPI_ID_FLASH0);
  //  Flash_WriteEnable();

    buf[0] = READ_DATA_ID; /*read*/
    buf[1] = TmpAddr.AsByte.Byte3;
    buf[2] = TmpAddr.AsByte.Byte2;
    buf[3] = TmpAddr.AsByte.Byte1;
    buf[4] = TmpAddr.AsByte.Byte0;        
    Ret = SPI_WriteNByte(buf,SPI_ID_FLASH,5);

    if(Ret == TRUE)
    {
        Ret = SPI_ReadNByte(Buff,SPI_ID_FLASH,Len);
    }
    Flash_CsHigh(SPI_ID_FLASH0);

    /*�ú�رգ����0����ֹ�����*/
    EX_Flash_Spi_MI_OUT_0();
    
    return Ret;

}


/*"************************************************			
Function: 	Flash_PageWrite
Description: Flashҳд
Input:  INT32U Addr��д���ݵ�ַ
             INT8U *Buff������BUFFERָ��
             INT16U Len��д���ݳ���  
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U Flash_PageWrite(INT32U Addr, INT8U *Buff, INT16U Len)
{
    UN_FLASH_ADDR TmpAddr;
    INT8U Ret;
    INT8U buf[5];
    
    TmpAddr.Addr = Addr;
    if(((Len + Addr) > FLASH_MAX_ADDR)  ||
        (Len == 0)            ||
        ((TmpAddr.AsByte.Byte0 + Len) > FLASH_PAGE_SIZE )
      )
    {
        return FALSE;
    }

    /*(Sep 14, 2018,10:8:10) Ҫ����ʹ��һ��*/
    EX_Flash_Spi_MI_Fun_Mode(); 
    
    Ret = Flash_WriteEnable();

    if(Ret == TRUE)
    {
        Flash_CsLow(SPI_ID_FLASH0);
        buf[0] = WRITE_PAGE_ID; /*write*/
        buf[1] = TmpAddr.AsByte.Byte3;
        buf[2] = TmpAddr.AsByte.Byte2;
        buf[3] = TmpAddr.AsByte.Byte1;
        buf[4] = TmpAddr.AsByte.Byte0;        
        SPI_WriteNByte(buf,SPI_ID_FLASH,5);
        SPI_WriteNByte(Buff,SPI_ID_FLASH,Len);
        Flash_CsHigh(SPI_ID_FLASH0);
    }
    
    /*�ȴ�д���*/
    Ret= Flash_WaitWriteEnd(PAGE_WRITE_DELAYMS);
    Flash_WriteDisable();

    /*�ú�رգ����0����ֹ�����*/
    EX_Flash_Spi_MI_OUT_0();
    
    return Ret;
}


/*"************************************************			
Function: 	Flash_SectorErase
Description: ��������
Input:  INT32U Addr��Ҫ�����������ĵ�ַ            
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U Flash_SectorErase(INT32U Addr)
{
    INT8U buf[5];
    UN_FLASH_ADDR TmpAddr;
    INT8U ret;
    
    TmpAddr.Addr = Addr;
    if( TmpAddr.Addr > FLASH_MAX_ADDR) /*������������*/
    {
        return FALSE;
    }

    /*(Sep 14, 2018,10:8:10) Ҫ����ʹ��һ��*/
    EX_Flash_Spi_MI_Fun_Mode(); 
    
    Flash_WriteEnable();

    /*sector earse*/
    Flash_CsLow(SPI_ID_FLASH0);
    buf[0] = SECTOR_ERASE_ID; /*sector erase*/
    buf[1] = TmpAddr.AsByte.Byte3;
    buf[2] = TmpAddr.AsByte.Byte2;
    buf[3] = TmpAddr.AsByte.Byte1;
    buf[4] = TmpAddr.AsByte.Byte0; 
    SPI_WriteNByte(buf,SPI_ID_FLASH,5);
    Flash_CsHigh(SPI_ID_FLASH0);
    ret = Flash_WaitWriteEnd(SECTOR_EARSE_DELAYMS);
    Flash_WriteDisable();

    /*�ú�رգ����0����ֹ�����*/
    EX_Flash_Spi_MI_OUT_0();
 
    return ret;
}

/*"************************************************			
Function: 	Flash_BlockErase
Description: ������ƬFLASH
Input:  ��       
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U Flash_BlockErase(INT32U Addr)
{
    return FALSE;
}


/*"************************************************			
Function: 	Flash_Write
Description: FLASHд����
Input:  ��       
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U Flash_Write(INT32U Addr, INT8U *Buff, INT16U Len)
{
    UN_FLASH_ADDR TmpAddr;
    INT16U Offset,Offset2;
    INT8U Ret;
  
    TmpAddr.Addr = Addr;
    if(((Len + Addr) > FLASH_MAX_ADDR)  ||
        (Len == 0) )
    {
        return FALSE;
    }

   Offset=0;
   Offset2=0;
   
    while(Len)
    {
        if((TmpAddr.Addr % FLASH_SECTOR_SIZE) == 0)/*������Խ��SECROR����ʼ���Ȳ���*/
        {
            Flash_SectorErase(TmpAddr.Addr);
        }
    
        if((TmpAddr.AsByte.Byte0 + Len) >=FLASH_PAGE_SIZE)
        	{
        	   Offset=FLASH_PAGE_SIZE-TmpAddr.AsByte.Byte0;
                Ret=Flash_PageWrite(TmpAddr.Addr,Buff+Offset2,Offset);
                Len=Len-Offset;
                TmpAddr.Addr=TmpAddr.Addr+Offset;
                Offset2=Offset2+Offset;
        	}
        else
        	{
        	   Ret=Flash_PageWrite(TmpAddr.Addr,Buff+Offset2,Len);
        	   Len=0;
        	}
        	
        if(Ret==FALSE)
        	break;
   }
    return Ret;
}


/*"************************************************			
Function: 	Flash_CheckSzie
Description: ���FLASH�����Ƿ�Ϊ��������
Input:  ��       
Output: ��	
Return: TRUE  �ɹ�  FALSEʧ�� 
Others: 		
************************************************"*/	
INT8U Flash_CheckSzie(void)
{
    INT8U Ret;
    INT32U FlashReadFlag;
	
	    Ret=TRUE;
           Flash_SectorErase(0x000000);
	    Flash_PageWrite(0x000000, (INT8U*)&FlashWriteFlag_1,4);
	    Flash_SectorErase(0x100000);
	    Ret=Flash_PageWrite(0x100000, (INT8U*)&FlashWriteFlag_2,4);
	    Flash_Read(0x000000,(INT8U*)&FlashReadFlag ,4);
	    if((FlashReadFlag==FlashWriteFlag_1)&&(Ret==TRUE))
	    	{ 
	    		    	 Flash_SectorErase(0x200000);
	    	              Ret=Flash_PageWrite(0x200000, (INT8U*)&FlashWriteFlag_2,4);
		             FlashReadFlag=0;
		             Flash_Read(0x000000,(INT8U*)&FlashReadFlag ,4);
		             if((FlashReadFlag==FlashWriteFlag_1)&&(Ret==TRUE))
		    	       { 
		    	           Flash_SectorErase(0x1000000);
	    	                 Ret=Flash_PageWrite(0x1000000, (INT8U*)&FlashWriteFlag_2,4);
		                 FlashReadFlag=0;
		                 Flash_Read(0x000000,(INT8U*)&FlashReadFlag ,4);
		                 if((FlashReadFlag==FlashWriteFlag_1)&&(Ret==TRUE))
		                 	{
                                      Flash_SectorErase(0x2000000);
	    	                        Ret=Flash_PageWrite(0x2000000, (INT8U*)&FlashWriteFlag_2,4);
		                        FlashReadFlag=0;
		                        Flash_Read(0x000000,(INT8U*)&FlashReadFlag ,4);
					     if((FlashReadFlag==FlashWriteFlag_1)&&(Ret==TRUE))
					     	{
								Ret=FALSE;
					     	}
					      else
					      	{
					      	      if(0x2000000==FLASH_SIZE)
		    	       	 	       {
		    	       	 		         Ret=TRUE;
		    	       	 	       }
		    	       	            else
		    	       	 	       {
		    	       	 		         Ret=FALSE;	 
		    	       	 	       }
					      	}
		                 	}
		                 else
		                 	{
		                 		   if(0x400000==FLASH_SIZE)
		    	       	 	       {
		    	       	 		         Ret=TRUE;
		    	       	 	       }
		    	       	         else
		    	       	 	       {
		    	       	 		         Ret=FALSE;	 
		    	       	 	       }
		                 	}
		    	       }
		             else
		    	       {
		    	       	 if(0x200000==FLASH_SIZE)
		    	       	 	{
		    	       	 		 Ret=TRUE;
		    	       	 	}
		    	       	 else
		    	       	 	{
		    	       	 		Ret=FALSE;	 
		    	       	 	}
		    	       }
	    	}
	    else
	    	{
	    		if((0x100000==FLASH_SIZE) && (FlashReadFlag==FlashWriteFlag_1))
	    		  	{
	    		  		Ret=TRUE;
	    		  	}
	    		else
	    			 {
	    			 	  Ret=FALSE;	
	    			 }
	    	}  
	    	
    return Ret;
}


/*"************************************************			
Function: 	Flash_GetErrSta		
Description: ����Flash�豸�Ƿ�Ϊ����״̬
             ��Flash��дʱ ��������EEP_ERR_TIMES��,���ж�ΪFlash����					
Input:  
Return: TRUE  ����״̬  _FALSE  ����״̬		
Others: 
   	    	
************************************************"*/	
INT8U Flash_GetErrSta(void)
{
    if(mFlashRwErrTimes >FLASH_ERR_TIME)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }

}


