/*"***********************************************************		
FileName: FLASH.h	
Author: 刘维       
Version : 1.0       Date: 2017.01.20	
Description: FLASH设备层驱动
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/		 

#ifndef __FLASH_H__
#define __FLASH_H__


/*----------------------------------------------*
 * 包含头文件                                   *
 *----------------------------------------------*/
#include "system\Sys_def.h"
/*----------------------------------------------*
 * 宏定义                                       *
 *----------------------------------------------*/

#define   NORMAL_MODE      (0) /*正常上电初始化模式*/
#define   FIRST_INIT_MODE     (1) /*初次初始化模式*/
#define   PD_INIT_MODE     (2) /*低功耗始化模式*/

#define   FLASH_ID          (0xC213)   /*设备ID*/
#define   FLASH_SIZE        (0x2000000)  /*设备容量信息（字节）*/
#define   FLASH_SECTOR_SIZE  (0x1000)   /*扇区大小（4k）*/
#define   FLASH_SECTOR_NUM  (FLASH_SIZE/ FLASH_SECTOR_SIZE)  /*扇区数目*/
#define   FLASH_PAGE_SIZE     (256)     /*页写最大字节数*/
#define   FLASH_MAX_ADDR        (FLASH_SIZE-1) 
#define   SPI_ID_FLASH0         (0) 	  /* FLASH设备0*/ 
#define   SPI_ID_FLASH1        (1)	      /* FLASH设备1*/
#define   SPI_ID_FLASH2       (2)	      /* FLASH设备2*/ 


                     
/*----------------------------------------------*
 * 结构体声明                                 *
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
 * 外部函数原型说明                             *
 *----------------------------------------------*/
 
extern INT8U Flash_Init(INT8U Mode);
extern INT8U Flash_Read(INT32U Addr, INT8U *Buff, INT16U Len);
extern INT8U Flash_Write(INT32U Addr, INT8U *Buff, INT16U Len);
extern INT8U Flash_BlockErase(INT32U Addr);
extern INT8U Flash_SectorErase(INT32U Addr);
extern INT8U Flash_GetErrSta(void);

          
#endif /* __FLASH_H__ */
