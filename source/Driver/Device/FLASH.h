/*"***********************************************************		
FileName: FLASH.h	
Author: ��ά       
Version : 1.0       Date: 2017.01.20	
Description: FLASH�豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/		 

#ifndef __FLASH_H__
#define __FLASH_H__


/*----------------------------------------------*
 * ����ͷ�ļ�                                   *
 *----------------------------------------------*/
#include "system\Sys_def.h"
/*----------------------------------------------*
 * �궨��                                       *
 *----------------------------------------------*/

#define   NORMAL_MODE      (0) /*�����ϵ��ʼ��ģʽ*/
#define   FIRST_INIT_MODE     (1) /*���γ�ʼ��ģʽ*/
#define   PD_INIT_MODE     (2) /*�͹���ʼ��ģʽ*/

#define   FLASH_ID          (0xC213)   /*�豸ID*/
#define   FLASH_SIZE        (0x2000000)  /*�豸������Ϣ���ֽڣ�*/
#define   FLASH_SECTOR_SIZE  (0x1000)   /*������С��4k��*/
#define   FLASH_SECTOR_NUM  (FLASH_SIZE/ FLASH_SECTOR_SIZE)  /*������Ŀ*/
#define   FLASH_PAGE_SIZE     (256)     /*ҳд����ֽ���*/
#define   FLASH_MAX_ADDR        (FLASH_SIZE-1) 
#define   SPI_ID_FLASH0         (0) 	  /* FLASH�豸0*/ 
#define   SPI_ID_FLASH1        (1)	      /* FLASH�豸1*/
#define   SPI_ID_FLASH2       (2)	      /* FLASH�豸2*/ 


                     
/*----------------------------------------------*
 * �ṹ������                                 *
 *----------------------------------------------*/

typedef union 
{
    INT32U Addr;
    struct 
	{
		INT8U Byte0;
		INT8U Byte1;
		INT8U Byte2;
		INT8U Byte3;
	}AsByte;
}UN_FLASH_ADDR;



 /*----------------------------------------------*
 * �ⲿ����ԭ��˵��                             *
 *----------------------------------------------*/
 
extern INT8U Flash_Init(INT8U Mode);
extern INT8U Flash_Read(INT32U Addr, INT8U *Buff, INT16U Len);
extern INT8U Flash_Write(INT32U Addr, INT8U *Buff, INT16U Len);
extern INT8U Flash_BlockErase(INT32U Addr);
extern INT8U Flash_SectorErase(INT32U Addr);
extern INT8U Flash_GetErrSta(void);

          
#endif /* __FLASH_H__ */
