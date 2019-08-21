/*"**********************************************************"*/		
/*"FileName: LIB_func.c                                      "*/
/*"Author:                                                   "*/ 
/*"Version : 1.0       Date: 2017.11.7	                     "*/
/*"Description: ���ÿ⺯��	                                 "*/
/*"Function List: 			                                 "*/
/*"<FunctionName>		                                     "*/	
/*"<author> <time> <version > <desc>			             "*/
/*"	                                                         "*/
/*"***********************************************************"*/
#include "LIB_func.h"


/********************************************************************************/
/* �� �� ����LIB_CheckBcdByte  */
/* ��    �ܣ���鵥�ֽ������Ƿ�ΪBCD��"*/
/* ˵    ����*/
/* ���������data ��������*/
/* �����������*/
/* �� �� ֵ��FALSE ��BCD��  / TRUE  BCD��*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_CheckBcdByte  */
/* ��    �ܣ������ֽ������Ƿ�ΪBCD��"*/
/* ˵    ����*/
/* ���������srcData��BCD��Դ����ָ��   dataLen���������BCD�����ݳ���*/
/* �����������*/
/* �� �� ֵ��FALSE ��BCD��  / TRUE  BCD��*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_CharToBcd  */
/* ��    �ܣ����ֽ��޷���16������ת���ɵ��ֽ�BCD��*/
/* ˵    ����*/
/* ������������ֽ�ʮ��������*/
/* �������: ��*/
/* �� �� ֵ:ת����������ֽ�BCD�룩*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_BcdToChar  */
/* ��    �ܣ����ֽ�BCD��ת��Ϊʮ�����ƣ������ж��������ݽ���BCD�Ϸ��Լ��*/
/* ˵    ����*/
/* ������������뵥�ֽ�BCD������*/
/* �����������*/
/* �� �� ֵ��ת����������ֽ�16�������� */
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_BcdToChar (INT8U srcData)
{
	return ((INT8U)((srcData & 0x0F) + (srcData >> 4) * 10));
}
/*"***************************************************************"*/
/* �� �� ����LIB_ShortToBcd  */
/* ��    �ܣ�2�ֽ�16������ת2�ֽ�BCD��*/
/* ˵    ����*/
/* ���������srcData��2�ֽ�ʮ��������*/
/* ���������*/
/* �� �� ֵ��BCDֵ*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_BcdToShort  */
/* ��    �ܣ�2�ֽ�BCD��ת16������*/
/* ˵    ����*/
/* ���������srcData: Դ���ݵ�ַָ�루BCD�����ݣ�*/
/* ���������*/
/* �� �� ֵ��ת�������2�ֽ�16��������*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"****************************************************************"*/
INT16U LIB_BcdToShort(INT8U *srcData)
{
	return (INT16U)((INT16U)LIB_BcdToChar(srcData[1])*100 + (INT16U)LIB_BcdToChar(srcData[0]));
}
/*"***************************************************************"*/
/* �� �� ����LIB_LongToBcd  */
/* ��    �ܣ�long��ת4�ֽ�BCD��*/
/* ˵    ����*/
/* ���������srcData��4�ֽ�ʮ��������  (С��)*/
/* ���������destData: ת�����BCD��ָ��(С��)*/
/* �� �� ֵ��*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_BcdToLong  */
/* ��    �ܣ�4�ֽ�BCD��ת16������*/
/* ˵    ����*/
/* ���������srcData: Դ���ݵ�ַָ�루BCD�����ݣ� С��*/
/* ���������*/
/* �� �� ֵ��ת�������4�ֽ�16��������(С��)*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/*"  Description:   ���ֽ�ʮ��������תBCD�� "*/
/*"  Input:      len : ����ת���ֽڳ���"*/
/*"                 srcData: Դ���ݵ�ַָ�루BCD�����ݣ�"*/
/*"  Output:      destData: Ŀ�����ݵ�ַָ�루ʮ���������ݣ� "*/
/*"  Return:      ��"*/
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
/*"  Description:  ���ֽ�BCD��ת��Ϊʮ������ "*/
/*"  Input:         len : ����ת���ֽڳ���"*/
/*"                    srcData: Դ���ݵ�ַָ�루ʮ���������ݣ�"*/
/*"  Output:      destData: Ŀ�����ݵ�ַָ�루BCD�����ݣ� "*/
/*"  Return:     ��  "*/
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
/*"  Description:    hex��תbcd��"*/
/*"  Input:         len : ����ת���ֽڳ���"*/
/*"                    pdata: Դ/Ŀ�����ݵ�ַָ��"*/
/*"  Output:      pdata: Ŀ�����ݵ�ַָ�� "*/
/*"  Return:     ��  "*/
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
/*"  Description:    BCD��תHEX��"*/
/*"  Input:         len : ����ת���ֽڳ���"*/
/*"                    pdata: Դ/Ŀ�����ݵ�ַָ��"*/
/*"  Output:      pdata: Ŀ�����ݵ�ַָ�� "*/
/*"  Return:     ��  "*/
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
/*"  Description:    N�ֽ��з���HEX��תINT64S"*/
/*"  Input:         byteNum : ����Դ���ݳ���"*/
/*"                 pDataIn: Դ���ݵ�ַָ��"*/
/*"  Output:       "*/
/*"  Return:     ת��INT64S���  "*/
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

        if(SignFlg == 0)/*"����"*/
        {
            LIB_MemCpy(pDataIn, (INT8U *)&rsult, byteNum);
        }
        else/*"����"*/
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
/*"  Description:   INT64SתN�ֽ��з���HEX��"*/
/*"  Input:         byteNum : ���Ŀ�����ݳ���"*/
/*"                 DataIn: Դ����ֵ"*/
/*"  Output:       pDataOut�����Ŀ���ַָ��"*/
/*"  Return:     "*/
/*"****************************************************************"*/
void  LIB_Int64sToNbyteInts(INT64S DataIn, INT8U *pDataOut, INT8U byteNum)
{
    INT64S temp;

    if(DataIn >= 0)/*"����"*/
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
/* �� �� ����LIB_BcdByteAdd  */
/* ��    �ܣ����ֽ�BCD���Լ�����*/
/* ˵    ����*/
/* ���������bcdData: Դ���ݣ�BCD��*/
/* ���������*/
/* �� �� ֵ��ת�������BCD��*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"****************************************************************"*/
INT8U LIB_BcdByteAdd(INT8U bcdData)
{
	return   LIB_CharToBcd (LIB_BcdToChar(bcdData)+1);
}
/*"***************************************************************"*/
/* �� �� ����LIB_BcdWordAdd  */
/* ��    �ܣ���BCD���Լ�����*/
/* ˵    ����*/
/* ���������bcdData: Դ���ݣ�BCD��*/
/* ���������*/
/* �� �� ֵ��ת�������BCD��*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"****************************************************************"*/
INT16U LIB_BcdWordAdd (INT16U bcdData)
{
	return   LIB_ShortToBcd (LIB_BcdToShort( (INT8U*) &bcdData)+1);
}
/*"***************************************************************"*/
/* �� �� ����LIB_BytesToBcd  */
/* ��    �ܣ�0~9���ִ���ƴ��BCD�봮*/
/* ˵    ����*/
/* ���������inpBuf:Դ����ָ��; Length:�������ݳ���(<=20)*/
/* ���������outpBuf:ת���������ָ��*/
/* �� �� ֵ�����BCD���ֽ���*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
	if (fill != 0)  //�����ֽ���Ϊ������ǰ��0
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
/* �� �� LIB_AsctoHex  */
/* ��    �ܣ�����ASCII����תHEX*/
/* ˵    ����*/
/* ���������inpBuf:����ASCII��;length ���������ݳ���(<=8)*/
/* ���������*/
/* �� �� ֵ��0xFFFFFFFF:�������ݷǷ���������ת��ֵ*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_BcdtoAsc  */
/* ��    �ܣ�����BCDתASCII*/
/* ˵    ����*/
/* ���������inBcd:����BCD*/
/* ���������outpBuf:ת��ASCII��*/
/* �� �� ֵ��ת�����ASCII�����*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_BcdToStr  */
/* ��    �ܣ�����BCD��תASCII*/
/* ˵    ����*/
/* ���������inBcd:����BCDָ�룬length:����BCD����; sign:������ */
/* ���������outpBuf:ת��ASCII��*/
/* �� �� ֵ��ת�����ASCII�����*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_ByteToAsc  */
/* ��    �ܣ�����BYTEתASCII*/
/* ˵    ����*/
/* ���������inBcd:����BYTE*/
/* ���������outpBuf:ת��ASCII��*/
/* �� �� ֵ��ת�����ASCII�����*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_BytesToAsc  */
/* ��    �ܣ��������ֽ�תASCII��*/
/* ˵    ����*/
/* ���������inByte:�����ֽ�ָ��;length :�������ݳ���(<128): sign ת��˳��*/
/* ���������outpBuf:ת����ASCII��ָ��*/
/* �� �� ֵ��ת�����ASCII�����*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_MemCpy  */
/* ��    �ܣ���Դsrc��ָ���ڴ��ַ����ʼλ�ÿ�ʼ����len���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ���С�*/
/* ˵    ����*/
/* ���������src: Դ��ַ��len: ���ݳ��ȣ�*/
/* ���������dest: Ŀ�ĵ�ַ��*/
/* �� �� ֵ����*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_MemCpy  */
/* ��    �ܣ���Դsrc��ָ��const��ַ����ʼλ�ÿ�ʼ����len���ֽڵ�Ŀ��dest��ָ���ڴ��ַ����ʼλ���С�*/
/* ˵    ����*/
/* ���������src: Դ��ַ��len: ���ݳ��ȣ�*/
/* ���������dest: Ŀ�ĵ�ַ��*/
/* �� �� ֵ����*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"****************************************************************"*/
void LIB_ConstMemCpy(CONST *src, INT8U *dest, INT16U len)
{
	while(len--) *dest++=*src++;
}
/*"***************************************************************"*/
/* �� �� ����LIB_MemCmp  */
/* ��    �ܣ��Ƚ��ڴ�����buf1��buf2��len���ֽڡ�*/
/* ˵    ����*/
/* ���������buf1��buf2: ���Ƚ�����ָ�룻len: ���ݳ���*/
/* ���������*/
/* �� �� ֵ��SMALL ��buf1<buf2  EQUAL ��buf1=buf2   BIG��buf1>buf2
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_ConstMemCmp  */
/* ��    �ܣ��Ƚϳ���buf1��buf2��len���ֽڡ�*/
/* ˵    ����*/
/* ���������buf1��buf2: ���Ƚ�����ָ�룻len: ���ݳ���*/
/* ���������*/
/* �� �� ֵ��SMALL ��buf1<buf2  EQUAL ��buf1=buf2   BIG��buf1>buf2
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_RvsMemCmp  */
/* ��    �ܣ��Ƚ��ڴ�����buf1��buf2��ǰlen���ֽڡ��Ӹߵ�ַ���͵�ַ*/
/* ˵    ����*/
/* ���������buf1��buf2: ���Ƚ�����ָ�룻len: ���ݳ���*/
/* ���������*/
/* �� �� ֵ��BUF1_SMALL ��buf1<buf2  EQUAL ��buf1=buf2   BUF1_BIG��buf1>buf2
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_MemSet  */
/* ��    �ܣ��ڴ���������*/
/* ˵    ����*/
/* ���������setValue������ֵ��len: ���ݳ���
*/
/* ���������*/
/* �� �� ֵ��
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_Reverse  */
/* ��    �ܣ���������ǰ���û�*/
/* ˵    ����*/
/* ���������src��Դ����ָ��; dest:Ŀ������ָ�룻len: ���ݳ���
*/
/* ���������*/
/* �� �� ֵ��
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� ����LIB_RvsSelf  */
/* ��    �ܣ�����ߵ��ֽ��Է�ת"*/
/* ˵    ����*/
/* ���������setValue������ֵ��len: ���ݳ���
*/
/* ���������*/
/* �� �� ֵ��
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_HToN16  */
/* ��    �ܣ�16λС�˴���໥ת��*/
/* ˵    ����*/
/* �����������������*/
/* ���������*/
/* �� �� ֵ��ת���������*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"***************************************************************"*/
INT16U LIB_HToN16 (INT16U data)
{

    return ( INT16U)(( data & 0xff) << 8) | (( data & 0xff00) >> 8);
}
/*"***************************************************************"*/
/* �� �� LIB_HToN32  */
/* ��    �ܣ�32λС��ת��Ϊ���*/
/* ˵    ����*/
/* �������������С������*/
/* ���������*/
/* �� �� ֵ��ת����Ĵ������*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"***************************************************************"*/
INT32U LIB_HToN32 (INT32U data)
{
    return ( INT32U)(( data & 0xff) << 24) | (( data & 0xff00) << 8) | (( data & 0xff0000) >> 8) | (( data & 0xff000000) >> 24)  ;
}
/*"***************************************************************"*/
/* �� �� ����LIB_SwapArray  */
/* ��    �ܣ��������ֽڽ���*/
/* ˵    ����*/
/* ���������srcData1��srcData2:���������ݵ�������
*/
/* ��������������Ƿ�ɹ�*/
/* �� �� ֵ��
*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
/*"****************************************************************"*/
BOOL LIB_SwapArray(INT8U*srcData1,INT8U*srcData2,INT16U len)
{
	INT8U temp;
    
    if( ((srcData1+len)<=srcData2 )|| ((srcData2+len)<=srcData1) )
    {
        return FALSE;  //�����н��治����
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
/* �� �� ����LIB_RvsSelf  */
/* ��    �ܣ�������������Ƿ�ȫΪĳһ��ֵ"*/
/* ˵    ����*/
/* ���������srcData  Դ����ָ��;len  ���ݳ���;value  �Ƚ�ֵ.*/
/* ����������Ƿ����*/
/* �� �� ֵ��EQUAL:Դ����Ϊȫvalueֵ NOT_EQUAL:Դ���ݲ�ȫΪvalueֵ*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_Sort  */
/* ��    �ܣ�������������д�С��������"*/
/* ˵    ����*/
/* ���������srcData: ���������飻len������Ԫ�ظ���*/
/* ���������*/
/* �� �� ֵ��srcData: ���������*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_GetMax  */
/* ��    �ܣ��ҳ������������ݵ������*/
/* ˵    ����*/
/* ���������x,y,z: ���Ƚ�����*/
/* ���������*/
/* �� �� ֵ�����ֵ*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/*"  Description:  ����ָ��ʱ������׼ʱ��(2000��1��1��)������,�������� "*/
/*"  Input:       dateTime: ����ʱ���ַָ��  "*/
/*"                    "*/
/*"  Output:     ��"*/
/*"  Return:      �����׼ʱ������� "*/
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

    if((nYear & 0x03) > 0 || nMonth > 2)/*"���������£�������1"*/
    {
        nDays++;
    }

    return (nDays - 1);
}


/*"***************************************************************"*/
/*"  Function:      LIB_HToStdDate "*/
/*"  Description:   ����ָ��ʱ������׼ʱ��(2000��1��1��)��Сʱ��"*/
/*"  Input:        dateTime: ����ʱ���ַָ�� "*/
/*"                    "*/
/*"  Output:       ��"*/
/*"  Return:       �����׼ʱ���Сʱ��"*/
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
/*"  Description:  ����ָ��ʱ������׼ʱ��(2000��1��1��)�ķ����� "*/
/*"  Input:        dateTime: ����ʱ���ַָ�� "*/
/*"                    "*/
/*"  Output:       ��"*/
/*"  Return:      �����׼ʱ��ķ����� "*/
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
/*"  Description:  ����ָ��ʱ������׼ʱ��(2000��1��1��)������ "*/
/*"  Input:        dateTime: ����ʱ���ַָ�� "*/
/*"                    "*/
/*"  Output:       ��"*/
/*"  Return:       �����׼ʱ�������"*/
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
/*"  Description:   ��������ָ��ʱ��������ʱ���ʽΪ������ʱ���롣"*/
/*"  ��Ҫ�ڵ��ú���ǰ���ʱ�����Ч�ԣ�������Ϊ��Чʱ�䣬������������0"*/
/*"  Input:         t1 ����ʱ��1��ַָ��"*/
/*"                   t2 ����ʱ��2��ַָ�� "*/
/*"  Output:      ��"*/
/*"  Return:      t1-t2������ "*/
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
/*"  Description:  ����ָ��������ʱ����ܴΣ�����������ΪBCD��ʱ����LIB_CalWeekBCD"*/
/*"  Input:        dateTime: ����ʱ���ַָ�� "*/
/*"                    "*/
/*"  Output:       ��"*/
/*"  Return:      ��������ܴ� "*/
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
/*"  Description:   ���ָ������ʱ�����ݵĺϷ��ԡ�"*/
/*"                   1��ʱ�����ݶ�ΪBCD�����ݣ� "*/
/*"                    2����������ʱ�������ݷ�Χ���м��"*/
/*"  Input:        dateTime: ����ʱ���ַָ�� "*/
/*"                    "*/
/*"  Output:      �� "*/
/*"  Return:      TRUE:��Чʱ�� "*/
/*"                   FALSE:��Чʱ��"*/
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
/*"  Description:   ���ָ������ʱ�����ݵĺϷ��ԡ�"*/
/*"                   1��ʱ�����ݶ�ΪHEX�����ݣ� "*/
/*"                    2����������ʱ�������ݷ�Χ���м��"*/
/*"  Input:        dateTime: ����ʱ���ַָ�� "*/
/*"                    "*/
/*"  Output:      �� "*/
/*"  Return:      TRUE:��Чʱ�� "*/
/*"                   FALSE:��Чʱ��"*/
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
/*"  Description:   �ж��Ƿ����꣬���ü����㷨�����ܱ�4���������Ϊ���꣬"*/
/*"                 ��ݸ�λ�ܱ�2��������λ�ܱ�4���������λΪ��������λ�ܱ�2���������ܱ�4����"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          year  һ���ֽ�BCD���ʾ��ݣ����ڱ�ʾ2000������ݣ��磺12��ʾ2012��"*/

/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE ���꣬FALSE   NOT����"*/
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
/*"  Description:    �������ĳ��ĳ���ж�����"*/
/*"  Input:          year  ָ�����(BCD��)"*/
/*"                  month ָ���·�(BCD��)"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         ����"*/
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
    else /*"�²���ȷ�Ļ� ������С�����������ÿ��Լ��ݶ�ʱ����ʱ����Ϊ0x99���"*/
    {
        Day = 28;
    }
    return Day;
}
/*"*****************************************************************************"*/
/*"  Function:       LIB_CalDayHEX"*/
/*"  Description:    �������ĳ��ĳ���ж�����"*/
/*"  Input:          year  ָ�����(HEX��)"*/
/*"                  month ָ���·�(HEX��)"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         ����"*/
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
    else /*"�²���ȷ�Ļ� ������С�����������ÿ��Լ��ݶ�ʱ����ʱ����Ϊ0x99���"*/
    {
        Day = 28;
    }
    return Day;
}
#if 0
 /*"***************************************************************"*/
/* �� �� LIB_TIDToDateTime  */
/* ��    �ܣ�TIDת��������ʱ��*/
/* ˵    ����*/
/* ���������tid: ��ǰ����1993�����ķ�����*/
/* �����������ǰ����ʱ�䣬BCD��ʽ/HEX��ʽ ��ѡ*/
/* �� �� ֵ��*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
									44640,40320,44640,43200, // 0~4��
									44640,43200,44640,44640, //5~8��.
									43200,44640,43200,44640  //9~12��.  
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
		

		if(i==1)  //  2��
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
		

		if(i==1)  //  2��
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
/* �� �� LIB_ABS  */
/* ��    �ܣ���������з������ݽ���ȡ����ֵ���������������ݵľ���ֵ����*/
/* ˵    ����*/
/* ��������������з���ֵ����*/
/* ���������*/
/* �� �� ֵ��data�ľ���ֵ*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_DataCheckSum  */
/* ��    �ܣ�����������ۼ�Ч���*/
/* ˵    ����*/
/* ���������inpBuf:�������ݵ�ַ ;inLength:�������ݳ��ȣ�init����ʼ�ۼ�ֵ*/
/* ���������*/
/* �� �� ֵ���ۼ�Ч���*/
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/* �� �� LIB_CalBCC  */
/* ��    �ܣ�������������ֵ*/
/* ˵    ����*/
/* ���������inpbuf:�������ݵ�ַ ;length:�������ݳ���*/
/* ���������*/
/* �� �� ֵ�� */
/* ��   �ƣ�            ����ʱ��: 2017-08-01"*/
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
/*"  Description:    RAM���ݺϷ��Լ�飬�ṹ������ǰ�����ֽ�Ϊ����ֵ5a5a��"*/
/*"                  �ṹ�����CRCУ������ݺϷ��Կɲ��øù��ú������кϷ��Լ��"*/
/*"  Input:          pData  ��������ʼ��ַָ��"*/
/*"                  DataLen ���ݳ���"*/
/*"                  CmpCrc  crc�Ƚ�ֵ"*/
/*"  Output:         "*/
/*"  Return:         TRUE  ���ݺϷ�  FALSE  ���ݷǷ�"*/
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
/*"  Description:    ����10��N�η�"*/
/*"  Input:          n  �η���"*/
/*"  Output:         "*/
/*"  Return:         ������"*/
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
	LIB_CharToBcdNByte(tmp, 7);/*���ʱ����hex����*/
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