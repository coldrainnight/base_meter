/*"*********************************************************** "*/ 
/*"  Author: 关靓华             Date:2014.3.25 "*/ 
/*"  Description:  计算CRC值      "*/ 
/*"  Function List:   // 主要函数及其功能，头文件可省略 "*/ 
/*"1.  ------- "*/ 
/*"2.  ------- "*/ 
/*"  History:         // 历史修改记录"*/  
/*"      <author>  <time>      <desc> "*/ 
/*"      David    96/10/12     build this moudle  "*/ 
/*"**********************************************************"*/ 

#include "system\Sys_def.h"
#include "CRC.h"
CRC_DATA_TP CRC16(INT8U *startaddr, INT16U len);

const INT16U CrcTable [16]= {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
};
/*"************************************************"*/ 			
/*"Function: 	CRC		"*/
/*"Description: 计算16位CRC"*/
/*"Input:  *startaddr ：数据开始地址，len ： 数据长度"*/
/*"Output: 无	"*/
/*"Return: CrcRreg：16位CRC值"*/
/*"Others: 		"*/
/*"************************************************"*/	
CRC_DATA_TP CRC16(INT8U *startaddr, INT16U len)
{

    INT8U da;
    INT8U *ptr;
    INT16U CrcRreg;
    ptr = (INT8U *)startaddr;

    CrcRreg = 0xffff;

    while(len--!=0)
    {
        da=CrcRreg>>12;
        CrcRreg<<=4;
        CrcRreg^=CrcTable[da^(*ptr/16)];

        da=CrcRreg>>12;
        CrcRreg<<=4;
        CrcRreg^=CrcTable[da^(*ptr&0x0f)];
        ptr++;
    }
    return(CrcRreg);
}

