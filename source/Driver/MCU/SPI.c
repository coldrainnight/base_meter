/*"***********************************************************		
FileName: SPI.c		
Author: 邹杨       
Version : 1.0       Date: 2017.01.09	
Description: SPI驱动实现
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#include "Driver\MCU\SPI.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"


#define SPI_DELAY_US     (1)
#define SPI_SOFT_MODE    (0) //非软时序方式 


/****************对外接口**********************/
void SPI_Init(INT8U SPI_ID,INT8U mode,INT8U *SPI_Cfg);			/*”SPI初始化函数”*/
INT8U SPI_ReadNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*”SPI读数据函数”*/
INT8U SPI_WriteNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len);	/*”SPI写数据函数”*/

/***************内部函数**********************/

/*****************************************************************************
 函 数 名  : FLASH_SPI_MI_OUT_0
 功能描述  : 修改FLASH MCU MI口空闲时输出0
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年9月14日
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void EX_Flash_Spi_MI_OUT_0(void)
{
    GPIO_SetMulti(FLASH_SOUT,  GPIO_Mode_OUT);
    GPIO_ConfigPin(FLASH_SOUT, OUTPUT, NORMAL_OUT);
    GPIO_OutputPin(FLASH_SOUT, LOW);
    MCU_DelayUs(1);   
}
/*****************************************************************************
 函 数 名  : EX_Flash_Spi_MI_Fun_Mode
 功能描述  : SPI Mi口改成多功能模式
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年9月14日
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void EX_Flash_Spi_MI_Fun_Mode(void)
{
    SPI_Init(SPI_ID_FLASH,SPI_NORMAL,NULL);    
}    
/*"************************************************			
Function: SPI_Init		
Description: 配置SPI接口上电和低功耗情况下的SPI接口电平，I/O口方向，SPI寄存器，通信速率，极性等等。					
Input:  SPI_ID：初始化接口编号，可指向多路SPI
	   Mode：功耗模式
	   SPI_Cfg：硬件SPI接口寄存器地址指针，预留
Output: 无	
Return: 无	
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
            GPIO_SetMulti(SPI_SLAVE_CS,  GPIO_Mode_DIG); /*" CS要配置为数字口?? "*/
			GPIO_SetMulti(SPI_SLAVE_CLK,  GPIO_Mode_DIG);
			GPIO_SetMulti(SPI_SLAVE_MOSI, GPIO_Mode_DIG);
			GPIO_SetMulti(SPI_SLAVE_MISO, GPIO_Mode_DIG);
			MCU_DelayUs(100);

            SPI1->SPICR1 = ( SPIx_SPICR1_MM_SLAVE | SPIx_SPICR1_BAUD_PCLK_16 | SPIx_SPICR1_CPHOL_LOW | SPIx_SPICR1_CPHA_SECOND);/*master /32 MSB mode 3 750k */ 
			SPI1->SPICR2 = SPIx_SPICR2_FLTEN_Msk;
			SPI1->SPICR3 = 0x000F;  /*清除发送接收缓冲错误寄存器*/
			SPI1->SPIIE  = 0x0000;
			SPI1->SPISTA = 0x0018;  /*清除错误*/
			MCU_DelayUs(20);
			SPI1->SPICR2 |= SPIx_SPICR2_SPIEN_Msk;/*enable*/ 
        }
        break;        

        case SPI_ID_ESAM:   /*”选择ESAM芯片”*/
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
            HSPI->SPICR3 = 0x000F;/*清除发送接收缓冲错误寄存器*/
            HSPI->SPIIE = 0x0000;
            HSPI->SPISTA = 0x0018;/*清除错误*/
			MCU_DelayUs(20);
			HSPI->SPICR2 |= 0x0001;/*enable*/

        }    
            
            break;
        case SPI_ID_FLASH:/*”选择”*/
            if(mode == SPI_NORMAL)
            {   
#if SPI_SOFT_MODE              
                GPIO_ConfigPin(FLASH_CS, OUTPUT, OPENDRAIN_OUT);
                GPIO_ConfigPin(FLASH_SCK, OUTPUT, NORMAL_OUT);
                GPIO_ConfigPin(FLASH_SIN, OUTPUT, NORMAL_OUT);
                GPIO_ConfigPin(FLASH_SOUT, INPUT, NULL_IN);
                /*使用模式0*/
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
				SPI2->SPICR3 = 0x000F;  /*清除发送接收缓冲错误寄存器*/
				SPI2->SPIIE  = 0x0000;
				SPI2->SPISTA = 0x0018;  /*清除错误*/
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
Description: 通过SPI接口信号的变化，MCU读取从设备的数据。					
Input:     pBuf: 读数据地址指针
		len:读数据长度
		SPI_ID：设备编号，可指向多路SPI
Output: 无	
Return: TRUE  成功  FALSE  失败		
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
            SPI1->SPITXF = 0;/*启动时序*/
            i = 2000;   
            while(i)
            {
                if (SPI1->SPISTA & 0x01)/*接收缓冲区满*/
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

			HSPI->SPITXF = 0;/*启动时序*/
			i = 2000;	
			while(i)
            {
				if (HSPI->SPISTA & 0x01)/*接收缓冲区满*/
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

		  SPI2->SPITXF = 0;/*启动时序*/
		  i = 2000;   
		  while(i)
		  {
			  if (SPI2->SPISTA & 0x01)/*接收缓冲区满*/
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
Description: 通过SPI接口信号的变化，MCU把长度为len,指针地址为pBuf的数据写到从设备。 					
Input:     pBuf: 写数地址指针
		len:写数据长度
		SPI_ID：设备编号，可指向多路SPI
Output: 无	
Return: TRUE  成功  FALSE  失败		
Others: 		
************************************************"*/	

INT8U SPI_WriteNByte(INT8U *pBuf,INT8U SPI_ID, INT16U len)
{
    INT16U i;
    INT16U Count;/*"字节传输个数"*/
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
               if (SPI1->SPISTA & 0x01)/*接收缓冲区满*/
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
					if (HSPI->SPISTA & 0x01)/*接收缓冲区满*/
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
				   if (SPI2->SPISTA & 0x01)/*接收缓冲区满*/
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
Description: 做SPI读写操作前的准备，包括拉低CS片选信号、配置对应的SPI寄存器等					
Input:  SPI_ID：设备编号，可指向多路SPI
Output: 无	
Return: 无	
Others: 		
************************************************"*/	
void SPI_Start (INT8U SPI_ID)
{
    switch(SPI_ID)
    { 
#if SPI_SOFT_MODE    
		#if 0
		case SPI_ID_FLASH:/*”选择其他设备”*/
        {
            GPIO_OutputPin(SPI_FLASH_CS,LOW);	
        }
        break;
		#endif
#else
		case SPI_ID_FLASH:/*”选择其他设备”*/
		{
			GPIO_OutputPin(FLASH_CS,LOW);	
		}
                break;

#endif	
        case SPI_ID_ESAM:/*”选择其他设备”*/
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
Description: 做SPI读写操作结束的准备，包括拉高CS片选信号、配置对应的SPI寄存器等					
Input:  SPI_ID：设备编号，可指向多路SPI
Output: 无	
Return: 无		
Others: 		
************************************************"*/	
void SPI_End (INT8U SPI_ID)
{
    switch(SPI_ID)
    { 
#if SPI_SOFT_MODE      
		#if 0
		case SPI_ID_FLASH: /*”选择其他设备”*/
        {
            GPIO_OutputPin(SPI_FLASH_CS,HIGH);
        }
        break;
		#endif
#else
		case SPI_ID_FLASH: /*”选择其他设备”*/
		{
			GPIO_OutputPin(FLASH_CS,HIGH);	
		}
                break;
#endif			
        case SPI_ID_ESAM: /*”选择其他设备”*/
        {
            GPIO_OutputPin(SPI_ESAM_CS,HIGH);	
        }
        break;
		default:
        break; 
    }
}

