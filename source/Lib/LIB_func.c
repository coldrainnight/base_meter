/*"**********************************************************"*/		
/*"FileName: LIB_func.c                                      "*/
/*"Author:                                                   "*/ 
/*"Version : 1.0       Date: 2017.11.7	                     "*/
/*"Description: 公用库函数	                                 "*/
/*"Function List: 			                                 "*/
/*"<FunctionName>		                                     "*/	
/*"<author> <time> <version > <desc>			             "*/
/*"	                                                         "*/
/*"***********************************************************"*/
#include "LIB_func.h"


/********************************************************************************/
/* 函 数 名：LIB_CheckBcdByte  */
/* 功    能：检查单字节数据是否为BCD码"*/
/* 说    明：*/
/* 输入参数：data 输入数据*/
/* 输出参数：无*/
/* 返 回 值：FALSE 非BCD码  / TRUE  BCD码*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"******************************************************************************"*/
BOOL LIB_CheckBcdByte (INT8U data)
{

	if(((data & 0xF0) > 0x90) || ((data & 0x0F) > 0x09))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
	
}
/*"***************************************************************"*/
/* 函 数 名：LIB_CheckBcdByte  */
/* 功    能：检查多字节数据是否为BCD码"*/
/* 说    明：*/
/* 输入参数：srcData：BCD码源数据指针   dataLen：待检验的BCD码数据长度*/
/* 输出参数：无*/
/* 返 回 值：FALSE 非BCD码  / TRUE  BCD码*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
BOOL  LIB_CheckBCD (INT8U *srcData, INT8U dataLen)
{
	INT8U i;

	for(i = 0; i < dataLen; i++)
	{
		if(FALSE == LIB_CheckBcdByte(*(srcData + i)))
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_CharToBcd  */
/* 功    能：单字节无符号16进制数转换成单字节BCD码*/
/* 说    明：*/
/* 输入参数：单字节十六进制数*/
/* 输出参数: 无*/
/* 返 回 值:转换结果（单字节BCD码）*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_CharToBcd (INT8U srcData)
{
	INT8U data,high,low;

	data = (INT8U)(srcData %100);
	high = (INT8U)(data / 10);
	low = (INT8U)(data % 10);
	
	return ((INT8U)((high << 4) | (INT8U)low));
}
/*"***************************************************************"*/
/* 函 数 名：LIB_BcdToChar  */
/* 功    能：单字节BCD码转换为十六进制，函数中对输入数据进行BCD合法性检查*/
/* 说    明：*/
/* 输入参数：输入单字节BCD码数据*/
/* 输出参数：无*/
/* 返 回 值：转换结果（单字节16进制数） */
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_BcdToChar (INT8U srcData)
{
	return ((INT8U)((srcData & 0x0F) + (srcData >> 4) * 10));
}
/*"***************************************************************"*/
/* 函 数 名：LIB_ShortToBcd  */
/* 功    能：2字节16进制数转2字节BCD码*/
/* 说    明：*/
/* 输入参数：srcData：2字节十六进制数*/
/* 输出参数：*/
/* 返 回 值：BCD值*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT16U  LIB_ShortToBcd (INT16U srcData)
{ 
	INT16U res;
    
	res = LIB_CharToBcd((INT8U) (srcData % 100));     
	srcData /= 100;
	res |= (LIB_CharToBcd((INT8U) (srcData % 100)))<< 8;  
  
	return res;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_BcdToShort  */
/* 功    能：2字节BCD码转16进制数*/
/* 说    明：*/
/* 输入参数：srcData: 源数据地址指针（BCD码数据）*/
/* 输出参数：*/
/* 返 回 值：转换结果（2字节16进制数）*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT16U LIB_BcdToShort(INT8U *srcData)
{
	return (INT16U)((INT16U)LIB_BcdToChar(srcData[1])*100 + (INT16U)LIB_BcdToChar(srcData[0]));
}
/*"***************************************************************"*/
/* 函 数 名：LIB_LongToBcd  */
/* 功    能：long型转4字节BCD码*/
/* 说    明：*/
/* 输入参数：srcData：4字节十六进制数  (小端)*/
/* 输出参数：destData: 转换后的BCD码指针(小端)*/
/* 返 回 值：*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
BOOL  LIB_LongToBcd (INT8U *srcData, INT8U *destData)
{ 
	INT8U i; 
	INT8U temp;
	INT32U scrTmp;

    
	scrTmp = ((INT32U)srcData[0]) | (((INT32U)srcData[1]) << 8 ) 
           | (((INT32U)srcData[2]) << 16 ) | (((INT32U)srcData[3]) << 24 )  ;

	if(scrTmp > 99999999)
	{
		scrTmp=99999999;	//return FALSE;
	}
    
	for(i=0; i<4; i++) 
	{ 
		temp =(INT8U) (scrTmp % 100);
		*(destData+i) = LIB_CharToBcd(temp);
		scrTmp /= 100;
	} 
    
	return TRUE;
}

BOOL  LIB_5BHexToBcd (INT8U *srcData, INT8U *destData)
{ 
	INT8U i; 
	INT8U temp;
	INT64U scrTmp;

    
	scrTmp = ((INT32U)srcData[0]) | (((INT32U)srcData[1]) << 8 ) 
           | (((INT32U)srcData[2]) << 16 ) | (((INT32U)srcData[3]) << 24 )  | (((INT64U)srcData[4]) << 32 ) ;

	if(scrTmp > 9999999999ULL)
	{
		scrTmp=9999999999ULL;	//return FALSE;
	}
    
	for(i=0; i<5; i++) 
	{ 
		temp =(INT8U) (scrTmp % 100);
		*(destData+i) = LIB_CharToBcd(temp);
		scrTmp /= 100;
	} 
    
	return TRUE;
}


/*"***************************************************************"*/
/* 函 数 名：LIB_BcdToLong  */
/* 功    能：4字节BCD码转16进制数*/
/* 说    明：*/
/* 输入参数：srcData: 源数据地址指针（BCD码数据） 小端*/
/* 输出参数：*/
/* 返 回 值：转换结果（4字节16进制数）(小端)*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT32U LIB_BcdToLong (INT8U *srcData)
{
	INT8U i;
	INT8U Tmp;
	INT32U RetVal = 0;
	INT32U Mult = 1;
	
	for (i = 0; i < 4; i++)
	{
		Tmp = LIB_BcdToChar(srcData[i]);
		RetVal += Mult * Tmp;
		Mult *= 100;
	}
	return RetVal;
}
/*"***************************************************************"*/
/*"  Function:     LIB_HexToBcd "*/
/*"  Description:   多字节十六进制数转BCD码 "*/
/*"  Input:      len : 输入转换字节长度"*/
/*"                 srcData: 源数据地址指针（BCD码数据）"*/
/*"  Output:      destData: 目标数据地址指针（十六进制数据） "*/
/*"  Return:      无"*/
/*"****************************************************************"*/
void LIB_HexToBcd (INT8U len, INT8U *srcData, INT8U * destData)
{
#if 0
    INT8U input[6], temp, msb, i, j, flag;
    INT8U output[6] =  { 0, 0, 0, 0, 0, 0 };

    flag = 0;
    for(i = 0; i < len; i++)
    {
        input[i] = *(srcData + i);
    }
    for(i = 8 * len; i > 0; i--)
    {
        msb = 0;
        temp = input[0];
        input[0] <<= 1;

        if((temp & 0x80) == 0x80)
        {
            msb = 1;
        }

        for(j = 1; j < len; j++)
        {
            temp = input[j];
            input[j] <<= 1;

            if(msb == 1)
            {
                input[j] += 1;
            }

            msb = 0;
            if((temp & 0x80) == 0x80)
            {
                msb = 1;
            }
        }

        temp = output[0];
        output[0] = (output[0] + output[0] + msb);
        msb = 0;
        if((temp & 0x80) == 0x80)
        {
            msb = 1;
        }

        if(((output[0] & 0x0f) > 0x09) || ((temp & 0x08) == 0x08))
        {
            output[0] += 0x06;
        }

        if(((output[0] & 0xf0) > 0x90) || msb)
        {
            msb = 1;
            output[0] += 0x60;
        }

        temp = output[1];
        output[1] = (output[1] + output[1] + msb);
        msb = 0;
        if((temp & 0x80) == 0x80)
        {
            msb = 1;
        }

        if(((output[1] & 0x0f) > 0x09) || ((temp & 0x08) == 0x08))
        {
            output[1] += 0x06;
        }

        if(((output[1] & 0xf0) > 0x90) || msb)
        {
            msb = 1;
            output[1] += 0x60;
        }

        temp = output[2];
        output[2] = (output[2] + output[2] + msb);
        msb = 0;
        if((temp & 0x80) == 0x80)
        {
            msb = 1;
        }

        if(((output[2] & 0x0f) > 0x09) || ((temp & 0x08) == 0x08))
        {
            output[2] += 0x06;
        }

        if(((output[2] & 0xf0) > 0x90) || msb)
        {
            flag = msb = 1;
            output[2] += 0x60;
        }
        if(flag == 1)
        {
            for(j = 3; j < 6; j++)
            {
                temp = output[j];
                output[j] = (output[j] + output[j] + msb);
                msb = 0;
                if((temp & 0x80) == 0x80)
                {
                    msb = 1;
                }

                if(((output[j] & 0x0f) > 0x09) || ((temp & 0x08) == 0x08))
                {
                    output[j] += 0x06;
                }

                if(((output[j] & 0xf0) > 0x90) || msb)
                {
                    flag = msb = 1;
                    output[j] += 0x60;
                }
            }
        }
    }
    for(i = 0; i < len; i++)
    {
        *(destData + i) = output[i];
    }

#else
    INT32U srcTmp,dstTmp;

    switch(len)
    {
    case 1:
        *destData = LIB_CharToBcd(*srcData);
        break;
    case 2:
    {
        INT16U  scrTmpInt;
        INT16U  destTmpInt;
        LIB_MemCpy(srcData, (INT8U *)&scrTmpInt, 2);
        destTmpInt = LIB_ShortToBcd(scrTmpInt);
        LIB_MemCpy((INT8U *)&destTmpInt, destData, 2);
    }
        break;
    case 3:
        srcTmp = 0;
        dstTmp = 0;
        LIB_MemCpy(srcData, (INT8U *)&srcTmp, 3);

        LIB_LongToBcd((INT8U *)&srcTmp, (INT8U *)&dstTmp);
        LIB_MemCpy((INT8U *)&dstTmp, destData, 3);
        break;
    case 4:
        LIB_LongToBcd(srcData, destData);
        break;
    case 5:
        LIB_5BHexToBcd(srcData, destData);
        break;
    default:
        break;
    }
#endif
}


/*"***************************************************************"*/
/*"  Function:       LIB_BcdToHex"*/
/*"  Description:  多字节BCD码转换为十六进制 "*/
/*"  Input:         len : 输入转换字节长度"*/
/*"                    srcData: 源数据地址指针（十六进制数据）"*/
/*"  Output:      destData: 目标数据地址指针（BCD码数据） "*/
/*"  Return:     无  "*/
/*"****************************************************************"*/
void LIB_BcdToHex (INT8U len, INT8U * srcData, INT8U * destData)
{
#if _THREE_PHASE
    INT8U input[6], temp, msb, i, j;
    INT8U output[6] = { 0, 0, 0, 0, 0, 0 };
    for(i = 0; i < len; i++)
    {
        input[i] = *(srcData + len - i - 1);
    }
    for(i = 8 * len; i > 0; i--)
    {
        msb = 0;
        temp = input[0];
        input[0] >>= 1;
        if((temp & 0x01) == 0x01)
        {
            msb = 1;
        }

        j = 1;
        for(; j < len; j++)
        {
            temp = input[j];
            input[j] >>= 1;
            if(msb == 1)
            {
                input[j] |= 0x80;
            }

            msb = 0;
            if((temp & 0x01) == 0x01)
            {
                msb = 1;
            }
        }

        j = 0;
        for(; j < len; j++)
        {
            temp = output[j];
            output[j] >>= 1;
            if(msb == 1)
            {
                output[j] |= 0x80;
            }

            msb = 0;
            if((temp & 0x01) == 0x01)
            {
                msb = 1;
            }
        }

        j = 0;
        for(; j < len; j++)
        {
            if((input[j] & 0x80) == 0x80)
            {
                input[j] += 0xd0;
            }
            if((input[j] & 0x08) == 0x08)
            {
                input[j] += 0xfd;
            }

        }

    }
    for(i = 0; i < len; i++)
    {
        *(destData + len - 1 - i) = output[i];
    }

#else
    INT32U srcTmp,dstTmp;
    INT16U dstTmp16;

    switch(len)
    {
    case 1:
        *destData = LIB_BcdToChar(*srcData);
        break;
    case 2:
        dstTmp16 = LIB_BcdToShort(srcData);
        LIB_MemCpy((INT8U *)&dstTmp16, destData, 2);
        break;
    case 3:
        srcTmp = 0;
        LIB_MemCpy(srcData, (INT8U *)&srcTmp, 3);
        dstTmp= LIB_BcdToLong((INT8U *)&srcTmp);
        LIB_MemCpy((INT8U *)&dstTmp, destData, 3);
        break;
    case 4:
        dstTmp = LIB_BcdToLong(srcData);
        LIB_MemCpy((INT8U *)&dstTmp, destData, 4);
        break;
    default:
        break;
    }
#endif
}
/*"***************************************************************"*/
/*"  Function:       LIB_CharToBcdNByte"*/
/*"  Description:    hex串转bcd串"*/
/*"  Input:         len : 输入转换字节长度"*/
/*"                    pdata: 源/目标数据地址指针"*/
/*"  Output:      pdata: 目标数据地址指针 "*/
/*"  Return:     无  "*/
/*"****************************************************************"*/
void LIB_CharToBcdNByte(INT8U *pdata, INT8U len)
{
    INT8U i;

    if(NULL != pdata)
    {
        for(i = 0; i < len; i++)
        {
            *(pdata + i) = LIB_CharToBcd(*(pdata + i));
        }
    }
}
/*"***************************************************************"*/
/*"  Function:       LIB_BcdToCharNByte"*/
/*"  Description:    BCD串转HEX串"*/
/*"  Input:         len : 输入转换字节长度"*/
/*"                    pdata: 源/目标数据地址指针"*/
/*"  Output:      pdata: 目标数据地址指针 "*/
/*"  Return:     无  "*/
/*"****************************************************************"*/
void LIB_BcdToCharNByte(INT8U *pdata, INT8U len)
{
    INT8U i;

    if(NULL != pdata)
    {
        for(i = 0; i < len; i++)
        {
            *(pdata + i) = LIB_BcdToChar(*(pdata + i));
        }
    }
}

/*"***************************************************************"*/
/*"  Function:       LIB_NbyteIntsToInt64s"*/
/*"  Description:    N字节有符号HEX码转INT64S"*/
/*"  Input:         byteNum : 输入源数据长度"*/
/*"                 pDataIn: 源数据地址指针"*/
/*"  Output:       "*/
/*"  Return:     转换INT64S结果  "*/
/*"****************************************************************"*/
INT64S  LIB_NbyteIntsToInt64s(INT8U *pDataIn, INT8U byteNum)
{
    INT8U SignFlg;
    INT64S rsult;

    rsult = 0;
    if(byteNum > 4)
    {
        SignFlg = pDataIn[byteNum-1];
        SignFlg = (SignFlg & 0x80);

        if(SignFlg == 0)/*"正数"*/
        {
            LIB_MemCpy(pDataIn, (INT8U *)&rsult, byteNum);
        }
        else/*"负数"*/
        {
            pDataIn[byteNum-1] &= 0x7f;
            LIB_MemCpy(pDataIn, (INT8U *)&rsult, byteNum);
            rsult = 0 - rsult;
        }
    }
    else
    {

    }
    return rsult;
}
/*"***************************************************************"*/
/*"  Function:       LIB_Int64sToNbyteInts"*/
/*"  Description:   INT64S转N字节有符号HEX码"*/
/*"  Input:         byteNum : 输出目标数据长度"*/
/*"                 DataIn: 源数据值"*/
/*"  Output:       pDataOut：输出目标地址指针"*/
/*"  Return:     "*/
/*"****************************************************************"*/
void  LIB_Int64sToNbyteInts(INT64S DataIn, INT8U *pDataOut, INT8U byteNum)
{
    INT64S temp;

    if(DataIn >= 0)/*"正数"*/
    {
        LIB_MemCpy((INT8U *)&DataIn, pDataOut, byteNum);

    }
    else
    {
        temp = 0 - DataIn;
        LIB_MemCpy((INT8U *)&temp, pDataOut, byteNum);
        pDataOut[byteNum - 1] |= 0x80;
    }
}
/*"***************************************************************"*/
/* 函 数 名：LIB_BcdByteAdd  */
/* 功    能：单字节BCD码自加运算*/
/* 说    明：*/
/* 输入参数：bcdData: 源数据，BCD码*/
/* 输出参数：*/
/* 返 回 值：转换结果，BCD码*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_BcdByteAdd(INT8U bcdData)
{
	return   LIB_CharToBcd (LIB_BcdToChar(bcdData)+1);
}
/*"***************************************************************"*/
/* 函 数 名：LIB_BcdWordAdd  */
/* 功    能：字BCD码自加运算*/
/* 说    明：*/
/* 输入参数：bcdData: 源数据，BCD码*/
/* 输出参数：*/
/* 返 回 值：转换结果，BCD码*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT16U LIB_BcdWordAdd (INT16U bcdData)
{
	return   LIB_ShortToBcd (LIB_BcdToShort( (INT8U*) &bcdData)+1);
}
/*"***************************************************************"*/
/* 函 数 名：LIB_BytesToBcd  */
/* 功    能：0~9数字串，拼成BCD码串*/
/* 说    明：*/
/* 输入参数：inpBuf:源数据指针; Length:输入数据长度(<=20)*/
/* 输出参数：outpBuf:转换输出数据指针*/
/* 返 回 值：输出BCD的字节数*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_BytesToBcd( INT8U Length,CONST *inpBuf,INT8U *outpBuf)
{
	INT8U i;
	INT8U fill;
	INT8U tmpBin[20];

	if(Length > 20)
	{
		return 0;
	}

	fill = Length % 2;
	if (fill != 0)  //输入字节数为奇数，前补0
	{
		tmpBin[0] = 0;
	}

	for (i = 0;i < Length;i++)
	{
		tmpBin[i + fill] = inpBuf[i];
	}

	for (i = 0;i < ((Length / 2) + fill);i++)
	{
		outpBuf[i] = tmpBin[2 * i]<<4;
		outpBuf[i] |= (tmpBin[2 * i + 1] & 0x0f);
	}

	return i;
}
/*"***************************************************************"*/
/* 函 数 LIB_AsctoHex  */
/* 功    能：输入ASCII码流转HEX*/
/* 说    明：*/
/* 输入参数：inpBuf:输入ASCII流;length ：输入数据长度(<=8)*/
/* 输出参数：*/
/* 返 回 值：0xFFFFFFFF:输入数据非法，其他：转换值*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT32U LIB_AscToHex(CONST *inpBuf,  INT8U length)
{
    INT32U retval = 0;
    INT32U mult = 1;
    INT8U i;
    INT8U tmphex[8];

    
    if((length == 0) || (length > 8))
    {
        return 0xFFFFFFFF;
    }
    for (i = 0; i < length; i++)
    {
        if ((inpBuf[i] >= '0') && (inpBuf[i] <= '9'))
        {
            tmphex[i ] = inpBuf[i] - '0';
        }
        else
        {
            return 0xFFFFFFFF;
        }
    }
    

    for (i = 0; i < length; i++)
    {
        retval += mult * tmphex[length - i - 1];
        mult *= 10;
    }

    return retval;
}
/*"***************************************************************"*/
/* 函 数 LIB_BcdtoAsc  */
/* 功    能：输入BCD转ASCII*/
/* 说    明：*/
/* 输入参数：inBcd:输入BCD*/
/* 输出参数：outpBuf:转换ASCII码*/
/* 返 回 值：转换后的ASCII码个数*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT8U  LIB_BcdToAsc( INT8U inBcd, INT8U *outpBuf)
{
	if(outpBuf == NULL)
	{
		return 0;
	}

	outpBuf[0] = (((inBcd & 0xf0) >> 4) + '0');
	outpBuf[1] = ((inBcd & 0x0f) + '0');
	return 2;
}
/*"***************************************************************"*/
/* 函 数 LIB_BcdToStr  */
/* 功    能：输入BCD流转ASCII*/
/* 说    明：*/
/* 输入参数：inBcd:输入BCD指针，length:输入BCD个数; sign:正反向 */
/* 输出参数：outpBuf:转换ASCII码*/
/* 返 回 值：转换后的ASCII码个数*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT8U LIB_BcdToStr(CONST *inBcd,  INT8U length, INT8U *outpBuf, INT8U  sign)
{
	INT8U i;
	INT8U  len,NumberTemp;
	
    for (i = 0, len = 0; i < length; i++)
    {
        if(sign)
        {
            NumberTemp=i;
        }
        else
        {
            NumberTemp=length - i - 1;
        }
        len += LIB_BcdToAsc(inBcd[NumberTemp], &outpBuf[len]);
    }
	return len;
}
/*"***************************************************************"*/
/* 函 数 LIB_ByteToAsc  */
/* 功    能：输入BYTE转ASCII*/
/* 说    明：*/
/* 输入参数：inBcd:输入BYTE*/
/* 输出参数：outpBuf:转换ASCII码*/
/* 返 回 值：转换后的ASCII码个数*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT8U  LIB_ByteToAsc( INT8U inByte, INT8U*outpBuf)
{
	INT8U  tmp;

	tmp = (inByte & 0xf0) >> 4;
	*outpBuf++ = tmp > 9 ? (tmp + 'A' - 10) : (tmp + '0');
	
	tmp = (inByte & 0x0f);
	*outpBuf++ = tmp > 9 ? (tmp + 'A' - 10) : (tmp + '0');

	return 2;
}
/*"***************************************************************"*/
/* 函 数 LIB_BytesToAsc  */
/* 功    能：输入多个字节转ASCII流*/
/* 说    明：*/
/* 输入参数：inByte:输入字节指针;length :输入数据长度(<128): sign 转化顺序*/
/* 输出参数：outpBuf:转换后ASCII码指针*/
/* 返 回 值：转换后的ASCII码个数*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT8U LIB_BytesToAsc(CONST *inByte, INT8U length, INT8U *outpBuf, INT8U  sign)
{
	INT8U i;
	INT8U len;
	
	if(sign)
	{

	    for (i = 0, len = 0; i < length; i++)
		{
			len += LIB_ByteToAsc(inByte[i], &outpBuf[len]);
		}
		return len;

	}
	else
	{
		for (i = 0, len = (length - 1) * 2; i < length; i++)
		{
			len -= LIB_ByteToAsc(inByte[i], &outpBuf[len]);

		}
		return length * 2;
	}
}
/*"***************************************************************"*/
/* 函 数 名：LIB_MemCpy  */
/* 功    能：从源src所指的内存地址的起始位置开始拷贝len个字节到目标dest所指的内存地址的起始位置中。*/
/* 说    明：*/
/* 输入参数：src: 源地址；len: 数据长度；*/
/* 输出参数：dest: 目的地址；*/
/* 返 回 值：无*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_MemCpy(INT8U *src, INT8U *dest, INT16U len)
{
    if((dest != NULL) && (src != NULL))
    {
        if(dest <= src)
        {
            while(len--)
            {
                (*dest++) = (*src++);
            }
        }
        else
        {
            dest += len-1;
            src += len-1;
            while(len--)
            {
                (*dest--) = (*src--);
            }
        }        
    }  
}
/*"***************************************************************"*/
/* 函 数 名：LIB_MemCpy  */
/* 功    能：从源src所指的const地址的起始位置开始拷贝len个字节到目标dest所指的内存地址的起始位置中。*/
/* 说    明：*/
/* 输入参数：src: 源地址；len: 数据长度；*/
/* 输出参数：dest: 目的地址；*/
/* 返 回 值：无*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_ConstMemCpy(CONST *src, INT8U *dest, INT16U len)
{
	while(len--) *dest++=*src++;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_MemCmp  */
/* 功    能：比较内存区域buf1和buf2的len个字节。*/
/* 说    明：*/
/* 输入参数：buf1、buf2: 待比较数据指针；len: 数据长度*/
/* 输出参数：*/
/* 返 回 值：SMALL ：buf1<buf2  EQUAL ：buf1=buf2   BIG：buf1>buf2
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8S LIB_MemCmp (INT8U *buf1, INT8U *buf2, INT16U len)
{
	INT16U i;
	INT8S rsult;

	rsult = EQUAL;

	for(i = len; i >0; i--)
	{
		if(*buf1>*buf2)
		{
			rsult = BIG;
			break;
		}
		if(*buf1 < *buf2)
		{
			rsult = SMALL;
			break;
		}
		buf1++;
		buf2++;
	}
    return rsult;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_ConstMemCmp  */
/* 功    能：比较常量buf1和buf2的len个字节。*/
/* 说    明：*/
/* 输入参数：buf1、buf2: 待比较数据指针；len: 数据长度*/
/* 输出参数：*/
/* 返 回 值：SMALL ：buf1<buf2  EQUAL ：buf1=buf2   BIG：buf1>buf2
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8S LIB_ConstMemCmp (CONST *buf1, CONST  *buf2, INT8U len)
{
	INT8U i;
	INT8S rsult;

	rsult = EQUAL;

	for(i = len; i >0; i--)
	{
		if(*buf1>*buf2)
		{
			rsult = BIG;
			break;
		}
		if(*buf1 < *buf2)
		{
			rsult = SMALL;
			break;
		}
		buf1++;
		buf2++;
	}
    return rsult;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_RvsMemCmp  */
/* 功    能：比较内存区域buf1和buf2的前len个字节。从高地址到低地址*/
/* 说    明：*/
/* 输入参数：buf1、buf2: 待比较数据指针；len: 数据长度*/
/* 输出参数：*/
/* 返 回 值：BUF1_SMALL ：buf1<buf2  EQUAL ：buf1=buf2   BUF1_BIG：buf1>buf2
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/

INT8S LIB_RvsMemCmp (INT8U *buf1, INT8U *buf2, INT8U len)
{
	INT8U i;
	INT8S rsult;

	rsult = EQUAL;
    
	buf1 += len-1;
	buf2 += len-1;
	for(i = len; i >0; i--)
	{
		if(*buf1>*buf2)
		{
			rsult = BIG;
			break;
		}
		if(*buf1 < *buf2)
		{
			rsult = SMALL;
			break;
		}
		buf1--;
		buf2--;
	}
	return rsult;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_MemSet  */
/* 功    能：内存数据设置*/
/* 说    明：*/
/* 输入参数：setValue：设置值；len: 数据长度
*/
/* 输出参数：*/
/* 返 回 值：
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_MemSet(INT8U setValue, INT8U *dest, INT16U len)
{
	INT16U i;

	if(dest != NULL)
	{
		for(i = 0; i < len; i++)
		 {
			*dest = setValue;
			dest++;
		}
	}
}
/*"***************************************************************"*/
/* 函 数 名：LIB_Reverse  */
/* 功    能：数组数据前后置换*/
/* 说    明：*/
/* 输入参数：src：源数据指针; dest:目的数据指针；len: 数据长度
*/
/* 输出参数：*/
/* 返 回 值：
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_Reverse (INT8U *src, INT8U *dest, INT8U len)
{
	if((NULL == src) || (NULL == dest))
	{
		return;
	}
    LIB_MemCpy(src,dest,len);
    LIB_RvsSelf(dest,len);
    
}
/*"***************************************************************"*/
/* 函 数 名：LIB_RvsSelf  */
/* 功    能：数组高低字节自翻转"*/
/* 说    明：*/
/* 输入参数：setValue：设置值；len: 数据长度
*/
/* 输出参数：*/
/* 返 回 值：
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_RvsSelf(INT8U *srcData, INT8U len)
{
	INT8U *begin,*end,tmp;
	
	 for(begin = srcData,end = srcData+len-1;begin<end;begin++,end--)
	{
		tmp = *end;
		*end = *begin;
		*begin = tmp;
	}
}
/*"***************************************************************"*/
/* 函 数 LIB_HToN16  */
/* 功    能：16位小端大端相互转换*/
/* 说    明：*/
/* 输入参数：输入数据*/
/* 输出参数：*/
/* 返 回 值：转换后的数据*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT16U LIB_HToN16 (INT16U data)
{

    return ( INT16U)(( data & 0xff) << 8) | (( data & 0xff00) >> 8);
}
/*"***************************************************************"*/
/* 函 数 LIB_HToN32  */
/* 功    能：32位小端转化为大端*/
/* 说    明：*/
/* 输入参数：输入小端数据*/
/* 输出参数：*/
/* 返 回 值：转换后的大端数据*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT32U LIB_HToN32 (INT32U data)
{
    return ( INT32U)(( data & 0xff) << 24) | (( data & 0xff00) << 8) | (( data & 0xff0000) >> 8) | (( data & 0xff000000) >> 24)  ;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_SwapArray  */
/* 功    能：两数组字节交换*/
/* 说    明：*/
/* 输入参数：srcData1，srcData2:待交换数据的两数组
*/
/* 输出参数：交换是否成功*/
/* 返 回 值：
*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
BOOL LIB_SwapArray(INT8U*srcData1,INT8U*srcData2,INT16U len)
{
	INT8U temp;
    
    if( ((srcData1+len)<=srcData2 )|| ((srcData2+len)<=srcData1) )
    {
        return FALSE;  //数据有交叉不处理
    }

	while(len--)
	{
		temp = *srcData2;
		*srcData2 = *srcData1;
		*srcData1 = temp;
		srcData1 ++;
		srcData2 ++;	
	}
    return TRUE;
}
/*"***************************************************************"*/
/* 函 数 名：LIB_RvsSelf  */
/* 功    能：检查输入数据是否全为某一个值"*/
/* 说    明：*/
/* 输入参数：srcData  源数据指针;len  数据长度;value  比较值.*/
/* 输出参数：是否相等*/
/* 返 回 值：EQUAL:源数据为全value值 NOT_EQUAL:源数据不全为value值*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_ChkValue(INT8U * srcData, INT8U len, INT8U value)
{
	INT8U cnt,ret;

	ret = EQUAL;

	for(cnt=0; cnt<len; cnt++)
	{
		if(srcData[cnt] != value)
		{
			ret =NOT_EQUAL;
			break;
		}
	}
	
	return ret;
}
/*"***************************************************************"*/
/* 函 数 LIB_Sort  */
/* 功    能：对输入数组进行从小到大排序"*/
/* 说    明：*/
/* 输入参数：srcData: 待排序数组；len：数组元素个数*/
/* 输出参数：*/
/* 返 回 值：srcData: 排序后数组*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_Sort(INT8U *srcData, INT8U len)
{
	INT8U i,j,temp;

	
	for (i=0; i<len; i++)  
	{
        	for (j=0; j<len-i; j++)  
        	{  
			if ( *(srcData+j) >*(srcData+j+1) )  
			{  
				 temp = *(srcData+j+1);  
				*(srcData+j+1) = *(srcData+j);  
				*(srcData+j) = temp;  
			}  
		} 
	}

}
/*"***************************************************************"*/
/* 函 数 LIB_GetMax  */
/* 功    能：找出输入三个数据的最大者*/
/* 说    明：*/
/* 输入参数：x,y,z: 待比较数据*/
/* 输出参数：*/
/* 返 回 值：最大值*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"****************************************************************"*/
INT32U  LIB_GetMax(INT32U x, INT32U y, INT32U z)
{
	INT32U max;

	max = x;
	
	if(y>x)
	{
		max = y;
	}
	 
	if(max  < z)
	{
		max = z;
	}

	return max;

}
const INT8U DaysOfMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const INT16U DaysBeforeMonth[12] = {0, 31, 59, 90, 120,151,181,212,243,273,304,334};
/*"***************************************************************"*/
/*"  Function:    LIB_DToStdDate   "*/
/*"  Description:  计算指定时间距离基准时间(2000年1月1日)的天数,不含本日 "*/
/*"  Input:       dateTime: 给定时间地址指针  "*/
/*"                    "*/
/*"  Output:     无"*/
/*"  Return:      距离基准时间的天数 "*/
/*"****************************************************************"*/
INT32U LIB_DToStdDate (ST_6TIME_FORM *dateTime)
{
    INT8U nYear;
    INT8U nMonth;
    INT8U nDay;
    INT32U nDays;

#if(APP_TIME_FRMT == HEX_FRMT)
    nYear = dateTime->year;
    nMonth = dateTime->month;
    nDay = dateTime->day;
#else
    nYear = LIB_BcdToChar(dateTime->year);
    nMonth = LIB_BcdToChar(dateTime->month);
    nDay = LIB_BcdToChar(dateTime->day);
#endif


    nDays = 365ul * (INT32U)nYear + (INT32U)(nYear>>2);

    nDays += DaysBeforeMonth[nMonth - 1];

    nDays += nDay;

    if((nYear & 0x03) > 0 || nMonth > 2)/*"过闰年润月，天数加1"*/
    {
        nDays++;
    }

    return (nDays - 1);
}


/*"***************************************************************"*/
/*"  Function:      LIB_HToStdDate "*/
/*"  Description:   计算指定时间距离基准时间(2000年1月1日)的小时数"*/
/*"  Input:        dateTime: 给定时间地址指针 "*/
/*"                    "*/
/*"  Output:       无"*/
/*"  Return:       距离基准时间的小时数"*/
/*"****************************************************************"*/
INT32U LIB_HToStdDate (ST_6TIME_FORM *dateTime)
{
    INT32U Days;
    INT8U Hours;
    Days    = LIB_DToStdDate(dateTime);
#if(APP_TIME_FRMT == HEX_FRMT)
    Hours   = dateTime->hour;
#else
    Hours   = LIB_BcdToChar(dateTime->hour);
#endif
    return (Days * 24 + Hours);
}


/*"***************************************************************"*/
/*"  Function:      LIB_MToStdDate "*/
/*"  Description:  计算指定时间距离基准时间(2000年1月1日)的分钟数 "*/
/*"  Input:        dateTime: 给定时间地址指针 "*/
/*"                    "*/
/*"  Output:       无"*/
/*"  Return:      距离基准时间的分钟数 "*/
/*"****************************************************************"*/
INT32U LIB_MToStdDate (ST_6TIME_FORM *dateTime)
{
    INT32U Hours;
    INT8U Minutes;

    Hours    = LIB_HToStdDate(dateTime);
#if(APP_TIME_FRMT == HEX_FRMT)
    Minutes = dateTime->min;
#else
    Minutes = LIB_BcdToChar(dateTime->min);
#endif
    
    return (Hours * 60 + Minutes);
}


/*"***************************************************************"*/
/*"  Function:      LIB_SToStdDate "*/
/*"  Description:  计算指定时间距离基准时间(2000年1月1日)的秒数 "*/
/*"  Input:        dateTime: 给定时间地址指针 "*/
/*"                    "*/
/*"  Output:       无"*/
/*"  Return:       距离基准时间的秒数"*/
/*"****************************************************************"*/
INT32U LIB_SToStdDate (ST_6TIME_FORM *dateTime)
{
    INT32U Minutes;
    INT8U seconds;

    if(FALSE == LIB_ChkDateTime(dateTime))
    {
        return FALSE;
    }

    Minutes = LIB_MToStdDate(dateTime);
#if(APP_TIME_FRMT == HEX_FRMT)
    seconds = dateTime->sec;
#else
    seconds = LIB_BcdToChar(dateTime->sec);
#endif


    return (Minutes * 60 + seconds);
}
/*"***************************************************************"*/
/*"  Function:      LIB_CmpTime "*/
/*"  Description:   计算两个指定时间的秒数差，时间格式为年月日时分秒。"*/
/*"  需要在调用函数前检查时间的有效性，若参数为无效时间，本函数将返回0"*/
/*"  Input:         t1 给定时间1地址指针"*/
/*"                   t2 给定时间2地址指针 "*/
/*"  Output:      无"*/
/*"  Return:      t1-t2的秒数 "*/
/*"****************************************************************"*/
INT32S LIB_CmpTime (ST_6TIME_FORM *t1,ST_6TIME_FORM *t2)
{
    INT32U data1,data2;
    INT32S result = 0;

    data1 = LIB_SToStdDate((ST_6TIME_FORM *)t1);
    data2 = LIB_SToStdDate((ST_6TIME_FORM *)t2);

    if(( data1 != FALSE) && ( data2 != FALSE))
    {
        if(data1 > data2)
        {
            result = data1 - data2;
        }
        else
        {
            result = data2 - data1;
            result = 0 - result;
        }
    }
    else
    {

    }
    return result;
}
/*"***************************************************************"*/
/*"  Function:      LIB_CalWeek/ LIB_CalWeekBCD"*/
/*"  Description:  计算指定年月日时间的周次，年月日数据为BCD码时调用LIB_CalWeekBCD"*/
/*"  Input:        dateTime: 给定时间地址指针 "*/
/*"                    "*/
/*"  Output:       无"*/
/*"  Return:      计算出的周次 "*/
/*"****************************************************************"*/
INT8U LIB_CalWeek (ST_6TIME_FORM *dateTime)
{
    INT32U x;
    INT8U week;

    x = LIB_DToStdDate(dateTime);
    week =(INT8U)((x + 6) % 7);

    return week;
}

INT8U LIB_CalWeekBCD (ST_6TIME_FORM *dateTime)
{
    INT8U week;
    INT8U i;
    ST_6TIME_FORM tmpTime;

    for (i = 0; i < 6; i++)
    {
        *((INT8U *) &tmpTime + i) = LIB_BcdToChar(*((INT8U *) dateTime + i));
    }

    week = LIB_CalWeek(&tmpTime);

    return week;
}


/*"***************************************************************"*/
/*"  Function:       LIB_ChkDateTime"*/
/*"  Description:   检查指定日期时间数据的合法性。"*/
/*"                   1、时间数据都为BCD码数据； "*/
/*"                    2、对年月日时分秒数据范围进行检查"*/
/*"  Input:        dateTime: 给定时间地址指针 "*/
/*"                    "*/
/*"  Output:      无 "*/
/*"  Return:      TRUE:有效时间 "*/
/*"                   FALSE:无效时间"*/
/*"****************************************************************"*/
INT8U LIB_ChkDateTimeBCD(ST_6TIME_FORM *dateTime)
{
    INT8U nMaxDay;
    INT8U rsult;

    rsult= LIB_CheckBCD((INT8U *) dateTime, sizeof(ST_6TIME_FORM));
    if(rsult != FALSE)
    {
        if((dateTime->hour > 0x23) || (dateTime->min > 0x59)
           || (dateTime->sec > 0x59))
        {
            rsult =  FALSE;
        }
        else if((dateTime->month < 1) || (dateTime->month > 0x12))
        {
            rsult =  FALSE;
        }
        else
        {
            nMaxDay = LIB_CalDayBCD(dateTime->year, dateTime->month);
            if((0 == dateTime->day) || (LIB_BcdToChar(dateTime->day) > nMaxDay))
            {
                rsult = FALSE;
            }
        }

    }

    return rsult;
}

/*"***************************************************************"*/
/*"  Function:       LIB_ChkDateTimeHEX"*/
/*"  Description:   检查指定日期时间数据的合法性。"*/
/*"                   1、时间数据都为HEX码数据； "*/
/*"                    2、对年月日时分秒数据范围进行检查"*/
/*"  Input:        dateTime: 给定时间地址指针 "*/
/*"                    "*/
/*"  Output:      无 "*/
/*"  Return:      TRUE:有效时间 "*/
/*"                   FALSE:无效时间"*/
/*"****************************************************************"*/
INT8U LIB_ChkDateTimeHEX(ST_6TIME_FORM *dateTime)
{
    INT8U nMaxDay;
    INT8U rsult;
    
    rsult = TRUE;
    if((dateTime->hour > 23) || (dateTime->min > 59)
           || (dateTime->sec > 59))
    {
        rsult =  FALSE;
    }
    else if((dateTime->month < 1) || (dateTime->month > 12))
    {
        rsult =  FALSE;
    }
    else
    {
        nMaxDay = LIB_CalDayHEX(dateTime->year, dateTime->month);
        if((0 == dateTime->day) || (dateTime->day > nMaxDay))
        {
            rsult = FALSE;
        }
    }
    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:      IsLeapYear"*/
/*"  Description:   判断是否闰年，采用简易算法，即能被4整除的年份为闰年，"*/
/*"                 年份高位能被2整除，低位能被4整除，或高位为奇数，低位能被2整除但不能被4整除"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          year  一个字节BCD码表示年份，用于表示2000年后的年份，如：12表示2012年"*/

/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE 闰年，FALSE   NOT闰年"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U LIB_IsLeapYear(INT8U year)
{
    if((year & 0x13) == 0 || (year & 0x13) == 0x12)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*"*****************************************************************************"*/
/*"  Function:       CalDay"*/
/*"  Description:    计算给定某年某月有多少天"*/
/*"  Input:          year  指定年份(BCD码)"*/
/*"                  month 指定月份(BCD码)"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         天数"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U LIB_CalDayBCD(INT8U year, INT8U month)
{
    INT8U Day;
    
    if((month <=0x12) && (month > 0) )
    {
        Day = DaysOfMonth[LIB_BcdToChar(month) - 1];

        if((TRUE == LIB_IsLeapYear(year)) && (month == 0x02))
        {
            Day++;
        }
    }
    else /*"月不正确的话 返回最小的天数，正好可以兼容定时冻结时间月为0x99情况"*/
    {
        Day = 28;
    }
    return Day;
}
/*"*****************************************************************************"*/
/*"  Function:       LIB_CalDayHEX"*/
/*"  Description:    计算给定某年某月有多少天"*/
/*"  Input:          year  指定年份(HEX码)"*/
/*"                  month 指定月份(HEX码)"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         天数"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U LIB_CalDayHEX(INT8U year, INT8U month)
{
    INT8U Day;
    
    if((month <=12) && (month > 0) )
    {
        Day = DaysOfMonth[month - 1];

        if((TRUE == LIB_IsLeapYear(LIB_CharToBcd(year))) && (month == 0x02))
        {
            Day++;
        }
    }
    else /*"月不正确的话 返回最小的天数，正好可以兼容定时冻结时间月为0x99情况"*/
    {
        Day = 28;
    }
    return Day;
}
#if 0
 /*"***************************************************************"*/
/* 函 数 LIB_TIDToDateTime  */
/* 功    能：TID转换成日期时间*/
/* 说    明：*/
/* 输入参数：tid: 当前距离1993年零点的分钟数*/
/* 输出参数：当前日期时间，BCD格式/HEX格式 可选*/
/* 返 回 值：*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/                                                  
void LIB_TIDToDateTime( INT32U tid,ST_DATETIME *outpDateTime) 
{
	const INT32U  YEAR_MINUTES[36] = {
									525600, 525600, 525600, 527040,  //1993~1996.
									525600, 525600, 525600, 527040,  //1997~2000.
									525600, 525600, 525600, 527040,  //2001~2004.
									525600, 525600, 525600, 527040,  //2005~2008.
									525600, 525600, 525600, 527040,  //2009~2012.
									525600, 525600, 525600, 527040,  //2013~2016.
									525600, 525600, 525600, 527040,  //2017~2020.
									525600, 525600, 525600, 527040,  //2021~2024.
									525600, 525600, 525600, 527040   //2025~2028.   
								};

	const INT32U  MONTH_MINUTES[] = {
									44640,40320,44640,43200, // 0~4月
									44640,43200,44640,44640, //5~8月.
									43200,44640,43200,44640  //9~12月.  
								};

	INT8U i;
	INT32U temp;

#if DATETIME_BCDFORMAT	
	outpDateTime->year = 0x93;
	outpDateTime->month = 0x01;
	outpDateTime->day = 0x01;

	outpDateTime->hour = 0x00;
	outpDateTime->minute = 0x00;
  
	for(i=0; i<36; i++)
	{
		if(tid >= YEAR_MINUTES[i])
		{
			tid -= YEAR_MINUTES[i];
			outpDateTime->year = LIB_BcdByteAdd(outpDateTime->year);
		}
    		else
    		{
   			 break;   
    		}
  	}

	for(i=0;i<12;i++)
	{
		

		if(i==1)  //  2月
		{
			 if( LIB_BcdToChar( outpDateTime->year) % 4 == 0  )
			{
				temp = 41760;
			}
			else
			{
				temp = MONTH_MINUTES[i];
			}
		}
		else
		{
			temp = MONTH_MINUTES[i];
		}
		
		if(tid >= temp )
		{
			tid	-=	temp;
			outpDateTime->month = LIB_BcdByteAdd(outpDateTime->month);
		}
        else
        {
            break;    	
        }
  	}
	outpDateTime->day =  LIB_CharToBcd (tid/1440 +1);
	
	tid%=1440;
	outpDateTime->hour =  LIB_CharToBcd (tid/60 );
	outpDateTime->minute =  LIB_CharToBcd (tid%60 );
    outpDateTime->week = LIB_GetWeekNum(outpDateTime);
#else
	outpDateTime->year = 93;
	outpDateTime->month = 1;
	outpDateTime->day = 1;

	outpDateTime->hour = 0x00;
	outpDateTime->minute = 0x00;
  
	for(i=0; i<36; i++)
	{
		if(tid >= YEAR_MINUTES[i])
		{
			tid -= YEAR_MINUTES[i];
			outpDateTime->year = (++outpDateTime->year)%100 ;
		}
    		else
    		{
   			 break;   
    		}
  	} 
	for(i=0;i<12;i++)
	{
		

		if(i==1)  //  2月
		{
			 if(  outpDateTime->year% 4 == 0  )
			{
				temp = 41760;
			}
			else
			{
				temp = MONTH_MINUTES[i];
			}
		}
		else
		{
			temp = MONTH_MINUTES[i];
		}
		
		if(tid >= temp )
		{
			tid	-=	temp;
			outpDateTime->month++;
		}
    		else
    		{
    			break;    	
    		}
  	}


	outpDateTime->day =tid/1440 +1;
	
	tid%=1440;
	outpDateTime->hour = tid/60 ;
	outpDateTime->minute = tid%60 ;
    outpDateTime->week = LIB_GetWeekNum(outpDateTime);
#endif
}
#endif
/*"***************************************************************"*/
/* 函 数 LIB_ABS  */
/* 功    能：对输入的有符号数据进行取绝对值处理，返回输入数据的绝对值数据*/
/* 说    明：*/
/* 输入参数：输入有符号值数据*/
/* 输出参数：*/
/* 返 回 值：data的绝对值*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT32U LIB_ABS(INT32S data)
{
    INT32U abs;
    if(data < 0)
    {
        abs = 0 - data;
    }
    else
    {
        abs = data;
    }

    return abs;
}
/*"***************************************************************"*/
/* 函 数 LIB_DataCheckSum  */
/* 功    能：计算数组的累加效验和*/
/* 说    明：*/
/* 输入参数：inpBuf:输入数据地址 ;inLength:输入数据长度，init：初始累加值*/
/* 输出参数：*/
/* 返 回 值：累加效验和*/
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT8U  LIB_DataCheckSum(INT8U *inpbuf, INT16U  length,INT8U init)
{
	INT8U res;
    INT16U i;

	res = init;
 	for( i = 0; i < length; i++ )
 	{
 		res += *inpbuf++;
 	}
	
	return res;
}
/*"***************************************************************"*/
/* 函 数 LIB_CalBCC  */
/* 功    能：计算数组的异或值*/
/* 说    明：*/
/* 输入参数：inpbuf:输入数据地址 ;length:输入数据长度*/
/* 输出参数：*/
/* 返 回 值： */
/* 设   计：            创建时间: 2017-08-01"*/
/*"***************************************************************"*/
INT8U LIB_CalBCC(INT8U *inpbuf, INT32U length) 
{
	INT32U i;
	
	INT8U BCC = *inpbuf++;
	
	for(i = 1; i < length; i ++)
	{
		BCC ^= *inpbuf++;
	}
	return BCC;
}

/*"*****************************************************************************"*/
/*"  Function:       LIB_ChkDataValid"*/
/*"  Description:    RAM数据合法性检查，结构体数据前两个字节为特征值5a5a，"*/
/*"                  结构整体带CRC校验的数据合法性可采用该公用函数进行合法性检查"*/
/*"  Input:          pData  数据域起始地址指针"*/
/*"                  DataLen 数据长度"*/
/*"                  CmpCrc  crc比较值"*/
/*"  Output:         "*/
/*"  Return:         TRUE  数据合法  FALSE  数据非法"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U LIB_ChkDataValid(INT8U *pData, INT16U DataLen, CRC_DATA_TP CmpCrc)
{
    INT16U DataFlg;
    CRC_DATA_TP crcTmp;

    crcTmp = CRC16(pData, DataLen);
    LIB_MemCpy(pData, (INT8U *)&DataFlg, 2);

    if((crcTmp == CmpCrc)&& (0x5a5a == DataFlg))
    {
        return (TRUE);
    }
    else
    {
        return (FALSE);
    }
}
/*"*****************************************************************************"*/
/*"  Function:       LIB_Pow10"*/
/*"  Description:    计算10的N次方"*/
/*"  Input:          n  次方数"*/
/*"  Output:         "*/
/*"  Return:         计算结果"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT32U LIB_Pow10(INT8U n)
{
    INT32U  rsult;
    INT8U i;

    rsult = 1;
    for(i = 0; i < n; i++)
    {
        rsult *= 10;
    }

    return rsult;
}

INT8U get_sys_tm_645_fmt(INT8U *tmp)
{
    INT8U buf[10];
    
    //GetSingle(E_SYS_TIME, tmp);
    //LIB_BcdToCharNByte(tmp, 7);
	LIB_CharToBcdNByte(tmp, 7);/*电表时钟是hex类型*/
    buf[0] = tmp[6]; //WW
    buf[1] = tmp[3]; //hh
    buf[2] = tmp[4]; //mm
    buf[3] = tmp[5]; //ss
    
    tmp[3] = buf[0];
    tmp[4] = buf[1];
    tmp[5] = buf[2];
    tmp[6] = buf[3];

    LIB_RvsSelf(tmp, 7);

    return 7;
}