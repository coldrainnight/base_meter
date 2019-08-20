/*"***********************************************************		
FileName: ISP.c		
Author:        
Version : 1.0       Date: 2017.5.9	
Description: ISP驱动实现
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#include "string.h"
#include "Driver\MCU\ISP\fsl.h"
#include "Driver\MCU\ISP\fsl_types.h"
#include "Driver\MCU\ISP\ISP.h"
#include "Driver\MCU\WDT.h"

/****************对外接口**********************/
INT8U  ISP_WriteFlash (INT32U  Addr, INT8U *Buff, INT16U Len);
INT8U  ISP_ReadFlash (INT32U Addr, INT8U *Buff, INT16U Len);
INT8U ISP_EraseFlash(INT32U addr, INT16U len);


/*********************************************************/

static INT8U __flash_write__(INT32U writeaddr, INT8U *bufptr)
{
    fsl_write_t         my_fsl_write_str;
    fsl_descriptor_t    fsl_descr;

    fsl_descr.fsl_flash_voltage_u08                 = 0x00;     // full speed mode
    fsl_descr.fsl_frequency_u08                     = 0x18;     // frequency = 24M
    fsl_descr.fsl_auto_status_check_u08             = 0x01;     // internal mode

    if (FSL_OK != FSL_Init((__far fsl_descriptor_t*)&fsl_descr))
    {
        return FALSE;
    }

    FSL_Open();
    FSL_PrepareFunctions();

    my_fsl_write_str.fsl_data_buffer_p_u08          = (fsl_u08 *)bufptr;
    my_fsl_write_str.fsl_word_count_u08             = 64;
    my_fsl_write_str.fsl_destination_address_u32    = writeaddr;

    if (FSL_OK != FSL_Write((fsl_write_t *)&my_fsl_write_str))
    {
        FSL_Close();
        return FALSE;
    }

    // flash块校验
    if (0x00 == ((writeaddr + 256) % FLASH_BLOCK_SIZE))
    {
        if (FSL_OK != FSL_IVerify(writeaddr / FLASH_BLOCK_SIZE))
        {
            FSL_Close();
            return FALSE;
        }
    }

    FSL_Close();
    return TRUE;
}

/********************************************************************************************************
** 函数名称: write_flash
** 功能描述: 写flash
** 输　  入: addr 数据的写入地址,ptr 数据的起始位置,len 写入长度
**
** 输　  出: false 写flash失败 / true 写flash成功
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者:
** 日　期:
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/

INT8U  ISP_WriteFlash (INT32U  Addr, INT8U *Buff, INT16U Len)
{
    static INT32U next_addr = APPLICATION_START;
    static INT16U pos, size;
    static INT8U  buf[FLASH_PAGE_SIZE];

    if (APPLICATION_START > Addr)
    {
        return FALSE;
    }


    if (Addr != next_addr)
    {
        if ((0 != pos) && ((next_addr + FLASH_PAGE_SIZE) <= Addr))
        {
            if (FALSE == __flash_write__(next_addr, &buf[0]))
            {
                return FALSE;
            }
            memset(&buf[0], 0xff, FLASH_PAGE_SIZE);
        }

        next_addr = Addr - Addr % FLASH_PAGE_SIZE;      // 调整next_addr,使之为FLASH_PAGE_SIZE的整数倍,为按页写做准备
        pos       = Addr - next_addr;                   // 调整pos,使之指向下一个欲写入的地址
    }

    while (Len > 0)
    {
        size = ((FLASH_PAGE_SIZE - pos) <= Len) ? (FLASH_PAGE_SIZE - pos) : (Len);
        memcpy(&buf[pos], Buff, size);
        Len  = Len - size;
        pos  = pos + size;
        if (FLASH_PAGE_SIZE == pos)
        {
            if (FALSE == __flash_write__(next_addr, &buf[0]))
            {
                return FALSE;
            }
            next_addr = next_addr + FLASH_PAGE_SIZE;
            Buff       = Buff + size;
            pos       = 0;
        }
    }
    memset(&buf[pos], 0xff, FLASH_PAGE_SIZE - pos);
    return TRUE;

}



INT8U  ISP_ReadFlash (INT32U Addr, INT8U *Buff, INT16U Len)
{
  INT8U *flash_ptr;
  flash_ptr =(INT8U __far *)Addr;
  DFLEN = 1;    // 允许数据闪存存取。
  for(unsigned char i=0;i<Len;i++)
  {
    *Buff=*flash_ptr;
    Buff++;
    flash_ptr++;
  }
  DFLEN = 0;    // 不允许数据闪存存取。
}


//INT8U  ISP_EraseFlash(INT8U FuncId)
//{

//}

/********************************************************************************************************
** 函数名称: ISP_EraseFlash
** 功能描述: 擦flash
** 输　  入: addr 地址,APP 程序起始地址 0x2000u  
                 len 长度:以1K 为单位
**
** 输　  出: false 写flash失败 / true 写flash成功
**
** 全局变量: 无
** 调用模块: 无
**
** 作　者:
** 日　期:
**-------------------------------------------------------------------------------------------------------
********************************************************************************************************/

INT8U ISP_EraseFlash(INT32U Addr, INT16U Len)
{
    fsl_descriptor_t    fsl_descr;

    fsl_descr.fsl_flash_voltage_u08                 = 0x00;     // full speed mode
    fsl_descr.fsl_frequency_u08                     = 0x18;     // frequency = 24M
    fsl_descr.fsl_auto_status_check_u08             = 0x01;     // internal mode

    if (FSL_OK != FSL_Init((__far fsl_descriptor_t*)&fsl_descr))
    {
        return FALSE;
    }

    FSL_Open();
    FSL_PrepareFunctions();

//    for (fsl_u32 writeaddr = 0x2000; writeaddr <= 0xFFFF; writeaddr += FLASH_BLOCK_SIZE)
    for (fsl_u32 writeaddr = Addr; writeaddr <=Addr+(Len*1024); writeaddr += FLASH_BLOCK_SIZE)
    {
        WDT_FeedWdg();
        if (FSL_OK != FSL_BlankCheck(writeaddr / FLASH_BLOCK_SIZE))
        {
            if (FSL_OK != FSL_Erase(writeaddr / FLASH_BLOCK_SIZE))
            {
                FSL_Close();
                return FALSE;
            }
        }
    }
    FSL_Close();
    return TRUE;
}


