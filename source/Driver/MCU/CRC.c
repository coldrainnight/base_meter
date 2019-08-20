/*"*********************************************************** "*/ 
/*"  Author: ������             Date:2014.3.25 "*/ 
/*"  Description:  ����CRCֵ      "*/ 
/*"  Function List:   // ��Ҫ�������书�ܣ�ͷ�ļ���ʡ�� "*/ 
/*"1.  ------- "*/ 
/*"2.  ------- "*/ 
/*"  History:         // ��ʷ�޸ļ�¼"*/  
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
/*"Description: ����16λCRC"*/
/*"Input:  *startaddr �����ݿ�ʼ��ַ��len �� ���ݳ���"*/
/*"Output: ��	"*/
/*"Return: CrcRreg��16λCRCֵ"*/
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

