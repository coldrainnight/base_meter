/*"***********************************************************		
FileName: SPI.c		
Author: ����       
Version : 1.0       Date: 2017.01.09	
Description: SPI����ʵ��
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#include "Driver\MCU\SPI.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"


#define SPI_DELAY_US     (1)
#define SPI_SOFT_MODE    (0) //����ʱ��ʽ 


/****************����ӿ�**********************/
void SPI_Init(INT8U SPI_ID,INT8U mode,INT8U *SPI_Cfg);			/*��SPI��ʼ��������*/
INT8U SPI_ReadNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*��SPI�����ݺ�����*/
INT8U SPI_WriteNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*��SPIд���ݺ�����*/

/***************�ڲ�����**********************/

/*****************************************************************************
 �� �� ��  : FLASH_SPI_MI_OUT_0
 ��������  : �޸�FLASH MCU MI�ڿ���ʱ���0
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��14��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void EX_Flash_Spi_MI_OUT_0(void)
{
    GPIO_SetMulti(FLASH_SOUT,  GPIO_Mode_OUT);
    GPIO_ConfigPin(FLASH_SOUT, OUTPUT, NORMAL_OUT);
    GPIO_OutputPin(FLASH_SOUT, LOW);
    MCU_DelayUs(1);   
}
/*****************************************************************************
 �� �� ��  : EX_Flash_Spi_MI_Fun_Mode
 ��������  : SPI Mi�ڸĳɶ๦��ģʽ
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��14��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void EX_Flash_Spi_MI_Fun_Mode(void)
{
    SPI_Init(SPI_ID_FLASH,SPI_NORMAL,NULL);    
}    
/*"************************************************			
Function: SPI_Init		
Description: ����SPI�ӿ��ϵ�͵͹�������µ�SPI�ӿڵ�ƽ��I/O�ڷ���SPI�Ĵ�����ͨ�����ʣ����Եȵȡ�					
Input:  SPI_ID����ʼ���ӿڱ�ţ���ָ���·SPI
	   Mode������ģʽ
	   SPI_Cfg��Ӳ��SPI�ӿڼĴ�����ַָ�룬Ԥ��
Output: ��	
Return: ��	
Others: 		
************************************************"*/	
void SPI_Init(INT8U SPI_ID,INT8U mode,INT8U *SPI_Cfg)
{

    switch(SPI_ID)
    { 
        case SPI_ID_SLAVE:
        if(mode == SPI_NORMAL)
        {
            GPIO_ConfigPin(SPI_SLAVE_CS,INPUT,NULL_IN); 
			//GPIO_ConfigPin(SPI_SLAVE_CLK,OUTPUT,NORMAL_OUT);
			//GPIO_ConfigPin(SPI_SLAVE_MOSI,OUTPUT,NORMAL_OUT);
			//GPIO_ConfigPin(SPI_SLAVE_MISO,INPUT,NULL_IN);

			MCU_PeriphClkSet(SPI1_num, ENABLE);
			MCU_DelayUs(100);
            GPIO_SetMulti(SPI_SLAVE_CS,  GPIO_Mode_DIG); /*" CSҪ����Ϊ���ֿ�?? "*/
			GPIO_SetMulti(SPI_SLAVE_CLK,  GPIO_Mode_DIG);
			GPIO_SetMulti(SPI_SLAVE_MOSI, GPIO_Mode_DIG);
			GPIO_SetMulti(SPI_SLAVE_MISO, GPIO_Mode_DIG);
			MCU_DelayUs(100);

            SPI1->SPICR1 = ( SPIx_SPICR1_MM_SLAVE | SPIx_SPICR1_BAUD_PCLK_16 | SPIx_SPICR1_CPHOL_LOW | SPIx_SPICR1_CPHA_SECOND);/*master /32 MSB mode 3 750k */ 
			SPI1->SPICR2 = SPIx_SPICR2_FLTEN_Msk;
			SPI1->SPICR3 = 0x000F;  /*������ͽ��ջ������Ĵ���*/
			SPI1->SPIIE  = 0x0000;
			SPI1->SPISTA = 0x0018;  /*�������*/
			MCU_DelayUs(20);
			SPI1->SPICR2 |= SPIx_SPICR2_SPIEN_Msk;/*enable*/ 
        }
        break;        

        case SPI_ID_ESAM:   /*��ѡ��ESAMоƬ��*/
        if(mode == SPI_NORMAL)
        {   
            GPIO_OutputPin(SPI_ESAM_CS,HIGH);
            MCU_PeriphClkSet(HSPI_num, ENABLE);
			MCU_DelayUs(100);
            GPIO_SetMulti(SPI_ESAM_CLK,  GPIO_Mode_DIG);
            GPIO_SetMulti(SPI_ESAM_MISO, GPIO_Mode_DIG);
            GPIO_SetMulti(SPI_ESAM_MOSI, GPIO_Mode_DIG);
			MCU_DelayUs(100);
            HSPI->SPICR1 = 0x011B;/*master 1MHZ MSB mode 3*/
			HSPI->SPICR2 = 0x0002;
            HSPI->SPICR3 = 0x000F;/*������ͽ��ջ������Ĵ���*/
            HSPI->SPIIE = 0x0000;
            HSPI->SPISTA = 0x0018;/*�������*/
			MCU_DelayUs(20);
			HSPI->SPICR2 |= 0x0001;/*enable*/

        }    
            
            break;
        case SPI_ID_FLASH:/*��ѡ��*/
            if(mode == SPI_NORMAL)
            {   
#if SPI_SOFT_MODE              
                GPIO_ConfigPin(FLASH_CS, OUTPUT, OPENDRAIN_OUT);
                GPIO_ConfigPin(FLASH_SCK, OUTPUT, NORMAL_OUT);
                GPIO_ConfigPin(FLASH_SIN, OUTPUT, NORMAL_OUT);
                GPIO_ConfigPin(FLASH_SOUT, INPUT, NULL_IN);
                /*ʹ��ģʽ0*/
                GPIO_OutputPin(FLASH_CS, HIGH);
                GPIO_OutputPin(FLASH_SCK, LOW);
                GPIO_OutputPin(FLASH_SIN, LOW);
#else
                GPIO_ConfigPin(FLASH_CS,OUTPUT,NORMAL_OUT);
				GPIO_ConfigPin(FLASH_SCK,OUTPUT,NORMAL_OUT);
				GPIO_ConfigPin(FLASH_SIN,OUTPUT,NORMAL_OUT);
				GPIO_ConfigPin(FLASH_SOUT,INPUT,NULL_IN);

				GPIO_OutputPin(FLASH_CS,HIGH);
				MCU_PeriphClkSet(SPI2_num, ENABLE);
				MCU_DelayUs(100);
				GPIO_SetMulti(FLASH_SCK,  GPIO_Mode_DIG);
				GPIO_SetMulti(FLASH_SIN, GPIO_Mode_DIG);
				GPIO_SetMulti(FLASH_SOUT, GPIO_Mode_DIG);
				MCU_DelayUs(100);
				//HSPI->SPICR1 = 0x011B;  /*master 1MHZ MSB mode 3*/
                SPI2->SPICR1 = 0x012B;/*master /32 MSB mode 3 750k */ 
				SPI2->SPICR2 = 0x0002;
				SPI2->SPICR3 = 0x000F;  /*������ͽ��ջ������Ĵ���*/
				SPI2->SPIIE  = 0x0000;
				SPI2->SPISTA = 0x0018;  /*�������*/
				MCU_DelayUs(20);
				SPI2->SPICR2 |= 0x0001;/*enable*/
#endif                

            }    
        break; 
        default:
            break; 
    }
}




/*"************************************************			
Function: SPI_ReadNByte		
Description: ͨ��SPI�ӿ��źŵı仯��MCU��ȡ���豸�����ݡ�					
Input:     pBuf: �����ݵ�ַָ��
		len:�����ݳ���
		SPI_ID���豸��ţ���ָ���·SPI
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 		
************************************************"*/	

INT8U SPI_ReadNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len)
{
    INT32U i;
    INT8U  RetVal = TRUE;  
    INT16U Count;
#if SPI_SOFT_MODE        
    INT8U BitCnt;
    INT8U TmpData;
#endif
    if(SPI_ID == SPI_ID_SLAVE)
    {
        for(Count=0;Count<len;Count++)
	    {    
            SPI1->SPITXF = 0;/*����ʱ��*/
            i = 2000;   
            while(i)
            {
                if (SPI1->SPISTA & 0x01)/*���ջ�������*/
                {
                    break;
                }
                i--;
            }
            if(i==0)
            {
                return FALSE;
            }
            pBuf[Count] = SPI1->SPIRXF;
	    }
    }
    else if(SPI_ID == SPI_ID_ESAM)
    {
        for(Count=0;Count<len;Count++)
        {    

			HSPI->SPITXF = 0;/*����ʱ��*/
			i = 2000;	
			while(i)
            {
				if (HSPI->SPISTA & 0x01)/*���ջ�������*/
                {
                    break;
                }
				i--;
            }
            if(i==0)
            {
                return FALSE;
            }
            pBuf[Count] = HSPI->SPIRXF;
        }

    }
    else if(SPI_ID == SPI_ID_FLASH)
    {
#if SPI_SOFT_MODE         
        for(Count=0;Count<len;Count++)
        {
            TmpData = 0;
            for(BitCnt=8;BitCnt>0;BitCnt--)
            {
                //GPIO_OutputPin(FLASH_SCK, LOW);
                BUILD_BUG_ON(FLASH_SCK != 0x27);//0x22
                GPIOC->BRESETL = 0x04;
                if(HIGH == GPIO_ReadPin(FLASH_SOUT))
                {
                    TmpData |= 0x01<<(BitCnt-1);
                }
                else
                {

                }
                //GPIO_OutputPin(FLASH_SCK, HIGH);
                GPIOC->BSETH = 0x04;
                MCU_DelayUs(SPI_DELAY_US);
            }
            //GPIO_OutputPin(FLASH_SCK, LOW);
            GPIOC->BRESETL = 0x04;
            MCU_DelayUs(SPI_DELAY_US*2);

            pBuf[Count] = TmpData;
        }
#else
	  for(Count=0;Count<len;Count++)
	  {    

		  SPI2->SPITXF = 0;/*����ʱ��*/
		  i = 2000;   
		  while(i)
		  {
			  if (SPI2->SPISTA & 0x01)/*���ջ�������*/
			  {
				  break;
			  }
			  i--;
		  }
		  if(i==0)
		  {
			  return FALSE;
		  }
		  pBuf[Count] = SPI2->SPIRXF;
	  }

#endif

    }

    return RetVal;
}
/*"************************************************			
Function: SPI_WriteNByte
Description: ͨ��SPI�ӿ��źŵı仯��MCU�ѳ���Ϊlen,ָ���ַΪpBuf������д�����豸�� 					
Input:     pBuf: д����ַָ��
		len:д���ݳ���
		SPI_ID���豸��ţ���ָ���·SPI
Output: ��	
Return: TRUE  �ɹ�  FALSE  ʧ��		
Others: 		
************************************************"*/	

INT8U SPI_WriteNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len)
{
    INT16U i;
    INT16U Count;/*"�ֽڴ������"*/
#pragma diag_suppress=Pe550
    INT8U temp;
#pragma diag_default=Pe550
    INT8U  RetVal = TRUE;     
#if SPI_SOFT_MODE       
    INT8U TmpData;
    INT8U BitCnt;
#endif
    if(SPI_ID == SPI_ID_SLAVE)
    {
        for(Count=0;Count<len;Count++)
        {	
            SPI1->SPITXF = pBuf[Count];
            i = 2000;
            while(i)
            {	
               if (SPI1->SPISTA & 0x01)/*���ջ�������*/
               {
            	   
            	   break;
               }
               i--;
            }
            if(i==0)
            {
               return FALSE;
            }
            temp = SPI1->SPIRXF;
        }
    }
    else if(SPI_ID == SPI_ID_ESAM)
    {
        for(Count=0;Count<len;Count++)
        {    

				HSPI->SPITXF = pBuf[Count];
				i = 2000;
				while(i)
                {    
					if (HSPI->SPISTA & 0x01)/*���ջ�������*/
                    {
						
                        break;
                    }
					i--;
                }
                if(i==0)
                {
                    return FALSE;
                }
				temp = HSPI->SPIRXF;
            }

    }
    else if(SPI_ID == SPI_ID_FLASH)
    {
#if SPI_SOFT_MODE      
        for(Count=0;Count<len;Count++)
        {
            TmpData = pBuf[Count];
            for(BitCnt=8;BitCnt>0;BitCnt--)
            {
                //GPIO_OutputPin(FLASH_SCK, LOW);
                GPIOC->BRESETL = 0x04;

                if(TmpData & (0x01<<(BitCnt-1)))
                {
                    //GPIO_OutputPin(FLASH_SIN,HIGH);
                    BUILD_BUG_ON(FLASH_SIN != 0x29);//0x23
                    GPIOC->BSETH = 0x08;
                }
                else
                {
                    //GPIO_OutputPin(FLASH_SIN,LOW);
                    GPIOC->BRESETL = 0x08;
                }
                MCU_DelayUs(SPI_DELAY_US);
                //GPIO_OutputPin(FLASH_SCK, HIGH);
                GPIOC->BSETH = 0x04;

                MCU_DelayUs(SPI_DELAY_US);
            }
            //GPIO_OutputPin(FLASH_SCK, LOW);
            GPIOC->BRESETL = 0x04;
            MCU_DelayUs(SPI_DELAY_US*2);

        }
#else
	   for(Count=0;Count<len;Count++)
	   {	

			   SPI2->SPITXF = pBuf[Count];
			   i = 2000;
			   while(i)
			   {	
				   if (SPI2->SPISTA & 0x01)/*���ջ�������*/
				   {
					   
					   break;
				   }
				   i--;
			   }
			   if(i==0)
			   {
				   return FALSE;
			   }
			   temp = SPI2->SPIRXF;
		  }

#endif
    }
    return  RetVal; 
}

/*"************************************************			
Function: SPI_Start		
Description: ��SPI��д����ǰ��׼������������CSƬѡ�źš����ö�Ӧ��SPI�Ĵ�����					
Input:  SPI_ID���豸��ţ���ָ���·SPI
Output: ��	
Return: ��	
Others: 		
************************************************"*/	
void SPI_Start (INT8U SPI_ID)
{
    switch(SPI_ID)
    { 
#if SPI_SOFT_MODE    
		#if 0
		case SPI_ID_FLASH:/*��ѡ�������豸��*/
        {
            GPIO_OutputPin(SPI_FLASH_CS,LOW);	
        }
        break;
		#endif
#else
		case SPI_ID_FLASH:/*��ѡ�������豸��*/
		{
			GPIO_OutputPin(FLASH_CS,LOW);	
		}
                break;

#endif	
        case SPI_ID_ESAM:/*��ѡ�������豸��*/
        {
            GPIO_OutputPin(SPI_ESAM_CS,LOW);	
        }
        break;
		default:
        break; 
    }
}

/*"************************************************			
Function:SPI_End	
Description: ��SPI��д����������׼������������CSƬѡ�źš����ö�Ӧ��SPI�Ĵ�����					
Input:  SPI_ID���豸��ţ���ָ���·SPI
Output: ��	
Return: ��		
Others: 		
************************************************"*/	
void SPI_End (INT8U SPI_ID)
{
    switch(SPI_ID)
    { 
#if SPI_SOFT_MODE      
		#if 0
		case SPI_ID_FLASH: /*��ѡ�������豸��*/
        {
            GPIO_OutputPin(SPI_FLASH_CS,HIGH);
        }
        break;
		#endif
#else
		case SPI_ID_FLASH: /*��ѡ�������豸��*/
		{
			GPIO_OutputPin(FLASH_CS,HIGH);	
		}
                break;
#endif			
        case SPI_ID_ESAM: /*��ѡ�������豸��*/
        {
            GPIO_OutputPin(SPI_ESAM_CS,HIGH);	
        }
        break;
		default:
        break; 
    }
}

