/*"*****************************************************************************"*/
/*"  FileName: EC.c"*/
/*"  Description:     ���ļ�ʵ��SIO7816 ADPU�����"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      xxx    2017/10/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/

#include "Driver\DriverComm.h"
#include "Ec.h"
#include "Lib\LIB_func.h"

INT8U mECCurChn; /*"��ǰͨ��"*/
volatile INT16U mCurRevLen;/*"��ǰ���ճ���"*/
INT8U mECBuff[_ECBUFFCAP];/*"���ݻ�����"*/

ST_ESAMINFO mEsamInfo;

/*"************************************************			
Function:EC_DataToBuf
Description: �������ݵ�ָ���Ļ�����
Input:  ���յ�������
Output:None
Return: None
Others: None
************************************************"*/
void EC_DataToBuf(INT8U data)
{
	if(mCurRevLen >= 250)
	{
		mCurRevLen = 0; 
	}	
	pECRevBuff[mCurRevLen] = data;    
	mCurRevLen++;
}
/*"************************************************			
Function:EC_Tx1Byte
Description: ����һ���ֽڵ�����
Input:  ��Ҫ���͵�����
Output:None
Return: None
Others: �ú���Ϊ����һ���ֽڣ�����CPU��ESAM�����ݴ�����Ʋ�ͬʱ��
         ���ͨ�������жϣ����ò�ͬ�ķ��ͽӿ�
************************************************"*/
void EC_Tx1Byte(INT8U T_data)
{
    EC_UartTx(T_data);
}

/*"************************************************			
Function:EC_WaitRx
Description: ����len���ȵ�����
Input:  �������ݵĳ���
Output:None
Return: None
Others: None
************************************************"*/
INT8U EC_WaitRx(INT8U len)
{
	INT16U i;
	INT8U result,tmpRevLen,tmpLen;/*"tmpRevLen�������浱ǰ�����ַ����ȣ�
	                                           �����ֵ�����ı䣬��˵�����յ��µ��ַ�"*/
	                                          
	tmpLen = len;
	result = _PAY_OK;
	i = 0;
    tmpRevLen = mCurRevLen;/*���浱ǰ�����ַ�����*/
    for( ;mCurRevLen < tmpLen; )
    {
	    /*��ǰ�ֽ�������һ���ֽ�����һ��ʱ��˵�����ܵ��µ��ֽ�*/
	    if( tmpRevLen != mCurRevLen )
	    {
		    tmpRevLen = mCurRevLen;
		    i = 0;
	    }
        EC_Dly(1);
		i++;
		if( i > 9600 )/*" �����ֽ�֮����ʱ��Χ��2etu~9600��etu֮��"*/
		{
		    result = _PAY_ERR;
			break;
		}
    }
	return result;
}


/*"************************************************			
Function:EC_EnRx
Description: ����ʹ��
Input:  None
Output:None
Return: None
Others: ��ESAM��CPU�����ݽ����ܲ�ͬ�ļĴ�������ʱ��
         ������ͨ���жϣ����ò�ͬ�Ŀ��ƺ���
************************************************"*/
void EC_EnRx(void)
{
	mCurRevLen = 0;/*��ǰ���ճ�������*/
    EC_UartEnRx();
}

/*"************************************************			
Function:EC_EnTx
Description: ����ʹ��
Input: None
Output:None
Return: None
Others: None
************************************************"*/
void EC_EnTx(void)
{ 
    EC_UartEnTx();
    EC_Dly(12);
}

/*"************************************************			
Function:EC_ATR
Description: 7816��λ
Input:  ECIDͨ��     pBuf: ��λ����
Output:_PAY_OK(�ɹ�)_PAY_ERR(ʧ��)
Return: None
Others: None
************************************************"*/
INT8U EC_ATR(INT8U ECID, INT8U *pBuf)
{
	INT8U len,result;
	
	result = _PAY_ERR;
	
    mECCurChn = ECID;
	if(_EC_ID_CPU == mECCurChn)
	{
		EC_UartInit();	  /*"7816�Ĵ�����ʼ������"*/
    	EC_ChnSel();  /*" ͨ���л�"*/
		EC_EnRx();    /*" SD��ת���� "*/
	}
	else	/*"ESAM����"*/
	{
		EC_EInitIO();
        EC_EPwrOff();
        MCU_DelayMs(10);		/*"����ʱ"*/
		EC_ERstCool(); /*"�ϵ�ѹ���Ǹ�λ"*/
		SPI_Init(SPI_ID_ESAM,SPI_NORMAL,NULL);
		MCU_DelayMs(120);		/*"����ʱ"*/
	}
    
	/*" 7816�临λʱ�� "*/
	if( ECID == _EC_ID_ESAM)
	{
		if(EC_Read(_EC_ID_ESAM,0x02,0x00,0x08,pBuf)== _PAY_ERR)
		{
			return _PAY_ERR;
		}
		else
		{
			return _PAY_OK;
		}
	}
	else
	{
		EC_CRstCool();
	}
	/*" �ȵȽ��� �����ֽڣ�Ȼ���ٽ���ָ�����ݳ���"*/
	if( EC_WaitRx(2) == _PAY_OK )
	{
		len = pECRevBuff[1];
		len &= 0x0F;
		len += 4;
		if( EC_WaitRx(len) == _PAY_OK )
		{
			if(pECRevBuff[0]==0x3b)
			{
				/*"���渴λ���ݺ�8���ֽ�"*/
				LIB_MemCpy((pECRevBuff + len - 8),pBuf,8);
				result = _PAY_OK;
			}
		}
	}

	/*"��ART ֮����ʱ120��ETU"*/
	EC_Dly(120);
	return result;	
}

/*"************************************************			
Function:EC_Cmd_Send
Description: 7816ָ���д
Input:  
Output:None
Return: None
Others: None
************************************************"*/
static INT8U mECGetResOrder[]= {0x00,0xC0,0x00,0x00};/*"ȡ��Ӧ��������"*/
INT8U EC_Cmd_Send(INT8U *Order_Head,           /*"����ͷ"*/  
									INT8U W_Len,     /*"д���ݳ���"*/
									INT8U *W_Data,   /*"д������"*/
									INT8U R_Len,     /*"�������ݳ���"*/
									INT8U ECID,      /*"ͨ��"*/
									INT8U R_Spec_Len) /*"����ĺ�������֡����"*/
{
	INT8U i,result;

	result = _PAY_ERR;
    mECCurChn = ECID;
	#if 0
    EC_ChnSel();/*"ͨ��ѡ��"*/
	#endif
	EC_EnTx();/*"SD��ת����"*/
    
	/*"�ȷ���CLA  INS  P1  P2  "*/
	for(i=0; i<4; i++ )
	{
		EC_Tx1Byte(*(Order_Head+i)); 
        mEsamInfo.ApduCmd[i] = *(Order_Head+i);
	}
	
	if( W_Len !=0 )
	{
		EC_Tx1Byte(W_Len);
        mEsamInfo.ApduCmd[4] = W_Len;
	}
	else if( R_Len !=0 )
	{
		EC_Tx1Byte(R_Len);
        mEsamInfo.ApduCmd[4] = R_Len;
	}
	else
	{
		EC_Tx1Byte(0);
        mEsamInfo.ApduCmd[4] = 0;
	}
	/*"�ȴ��ֽڷ������"*/
	EC_EnRx();	/*"SD��ת����"*/
    
	/*"�Ƚ���һ���ֽڵĹ����ֽ�INS"*/
	if( EC_WaitRx(1) == _PAY_OK)
	{
		if(pECRevBuff[0] == *(Order_Head+1))
		{
			if( W_Len !=0 )
			{
				EC_EnTx();
				/*"�ڷ���������֮������16ETUʱ��"*/

				EC_Dly(36);

				/*"���ͺ����ֽ�"*/
				for( i=0;i<W_Len;i++ )
				{
					EC_Tx1Byte(*(W_Data+i));
				}

				/*"SD��ת����"*/
				EC_EnRx();
			}
			else
			{
				mCurRevLen = 0;
			}


			i = (INT8U)(R_Len +2);
			if( EC_WaitRx(i) == _PAY_OK )
			{
				if(pECRevBuff[0+R_Len]==0x61)
				{
					if( *(Order_Head+1)!= 0xC0 )
					{
	        			if( R_Spec_Len!=0 )
	        			{
							i = R_Spec_Len;
						}
						else
						{
							i = pECRevBuff[1+R_Len];
						}

						result = EC_Cmd_Send(mECGetResOrder,0,0,i,ECID,0) ;
					}
					else
					{
						result =  _PAY_OK;
					}
				}
				else if((pECRevBuff[0+R_Len]==0x90)&&(pECRevBuff[1+R_Len]==0))
				{
					result = _PAY_OK; 
				}
                else
                {
				}
			}
            else
            {
				if(mCurRevLen < 2)/*"�յ�������С�����ֽ�"*/
				{
					LIB_MemSet(0xFF,(INT8U *)pECRevBuff,2);/*"û�յ�����ֵ��FF"*/
				}
			}
		}
		else
		{
			mCurRevLen = 1;
			EC_WaitRx(2);/*"�ٽ���һ�������ֽ�"*/
		}
	}
    else
    {
		LIB_MemSet(0x00,(INT8U *)pECRevBuff,2);/*"û�յ�����ֵ����"*/
	}
	
    LIB_Reverse((INT8U *)pECRevBuff,(INT8U *)&mEsamInfo.Sw,2);/*�õ���󷵻�ֵ*/

	if(result == _PAY_ERR)
	{
		if(mEsamInfo.Sw.word == 0x6988)
		{
			mEsamInfo.mCardStatReg = 0x10;	/*"MAC����"*/
		}
		else
		{
			mEsamInfo.mCardStatReg = 0x19;	/*"����IC������"*/
		}
	}
    else
    {
    }
	return result;	
}


/*"************************************************			
Function:SPIESAM_CRC
Description:ESAMͨ��ʱ����У��
Input:  
Output:None
Return: None
Others: None
************************************************"*/
INT8U SPIESAM_CRC(INT8U *pBuf,INT16U Len)
{
	INT8U *pdata,result;
	INT16U i;
	pdata = pBuf;

	result = *pdata;
	
	for(i=1;i<Len;i++)
	{
		result ^= pdata[i];
	}
	
	result = (INT8U)~result;

	return result;
}

/*"************************************************			
Function:SPIESAM_Cmd_Send
Description: SPI��ʽͨ��ESAM
Input:  
Output:None
Return: None
Others: None ÿ�����д256���ֽ�
************************************************"*/
INT8U SPIESAM_Cmd_Send(INT8U *Order_Head,           /*"����ͷ"*/  
									INT16U W_Len,     /*"д���ݳ���"*/
									INT8U *W_Data,   /*"д������"*/
									INT16U *R_Len)      /*"�������ݳ��ȷ���ֵ"*/
{
	INT8U result,CRCData;
	INT8U i,CLA;
	INT16U WaitTime,TmpWLen,SendLen;

	result = _PAY_ERR;
	TmpWLen = W_Len;
	SPI_Start(SPI_ID_ESAM);/*"ѡ��ESAM"*/
	MCU_DelayUs(60);
	pECRevBuff[0] = 0x55;

	LIB_MemCpy(Order_Head,&pECRevBuff[1],4);
	LIB_MemCpy(Order_Head,mEsamInfo.ApduCmd,4);
	
	pECRevBuff[5] = (INT8U)(W_Len>>8);		/*���ֽڵ�ַ�̶���0*/
	mEsamInfo.ApduCmd[4] = pECRevBuff[5];

	LIB_MemSet(0x00,(INT8U *)&mEsamInfo.Sw,2);/*"����ǰ�巵�ؽ��"*/
	pECRevBuff[6] = (INT8U)W_Len;		/*"д�����ֽڳ���"*/

	for(i=0;;i++)
	{
		if(TmpWLen > _MAX_SPIlEN)
		{
			SendLen = _MAX_SPIlEN;
			TmpWLen -= SendLen;
		}
		else
		{
			SendLen = TmpWLen;
			TmpWLen = 0;
		}
		if(i==0)
		{
			LIB_MemCpy(W_Data,&pECRevBuff[7],SendLen);
			if(TmpWLen == 0)
			{
				pECRevBuff[7+W_Len] = SPIESAM_CRC((INT8U *)&pECRevBuff[1],SendLen+6); /*"LRC1"*/
				SendLen += 8; /*0x55 CLA INS P1 P2 LEN1 LEN2 data CRC1*/
			}
			else
			{
				CRCData = SPIESAM_CRC((INT8U *)&pECRevBuff[1],SendLen+6); /*"LRC1"*/
				SendLen += 7;
			}
			SPI_WriteNByte(pECRevBuff,SPI_ID_ESAM,SendLen);
			if(TmpWLen == 0)
			{
				break;
			}
		}
		else
		{
			pECRevBuff[0] = (INT8U)~CRCData;
			CRCData = SPIESAM_CRC((INT8U *)&pECRevBuff[0],SendLen+1); /*"LRC1"*/
			if(TmpWLen == 0)
			{
				pECRevBuff[1+SendLen] = CRCData;
				SendLen += 1;
			}
			SPI_WriteNByte(pECRevBuff+1,SPI_ID_ESAM,SendLen);
			if(TmpWLen == 0)
			{
				break;
			}
		}
	}

	MCU_DelayUs(6);
	SPI_End(SPI_ID_ESAM);
	MCU_DelayUs(20);
	
	SPI_Start(SPI_ID_ESAM);
	MCU_DelayUs(60);
	CLA = 0x00;
	
	for(WaitTime=14500;WaitTime>0;WaitTime--)/*"����ʱ��Ƶ�ʵ��������������ȴ�ʱ�� 980ms"*/
	{
		SPI_ReadNByte(&CLA,SPI_ID_ESAM,1); /*"ÿ����4800��ʱ������325ms"*/
		if(CLA == 0x55) /*"���յ�״̬�֣�������"*/
		{
			result = _PAY_OK;
			break;
		}
		MCU_DelayUs(20);
	}
	
	if(result == _PAY_OK)	/*"��ȡESAM����"*/
	{
		SPI_ReadNByte(&mECBuff[0],SPI_ID_ESAM,2);/*"��ȡESAM���ص�SW1��SW2"*/
		if((mECBuff[0] == 0x90)&&(mECBuff[1] == 0x00))
		{
			SPI_ReadNByte(&mECBuff[2],SPI_ID_ESAM,2); /*"��ȡESAM�������ݳ���"*/
			TmpWLen = mECBuff[2];
			TmpWLen <<= 8;
			TmpWLen += mECBuff[3];
			if(TmpWLen > _MAX_SPIlEN )
			{
				while(1)
				{
					if(TmpWLen > _MAX_SPIlEN)
					{
						SendLen = _MAX_SPIlEN;
						TmpWLen -= SendLen;
					}
					else
					{
						SendLen = TmpWLen;
						TmpWLen = 0;
					}
					SPI_ReadNByte(&mECBuff[4],SPI_ID_ESAM,SendLen);/*"��ȡESAM���ص�data������"*/
					if(TmpWLen == 0)
					{
						break;
					}
				}
			}
			else
			{	
				SPI_ReadNByte(&mECBuff[4],SPI_ID_ESAM,(TmpWLen+1));/*"��ȡESAM���ص�data������"*/
				CRCData = SPIESAM_CRC(mECBuff,TmpWLen+4);
								
				if(pECRevBuff[TmpWLen] != CRCData)
				{
					result = _PAY_ERR;
				}
			}
	
		}
		else
		{
			result = _PAY_ERR;
		}
	}

	MCU_DelayUs(6);
	SPI_End(SPI_ID_ESAM);		/*"�ͷ�SPI����"*/
	MCU_DelayUs(20);

	LIB_Reverse((INT8U *)(mECBuff),(INT8U *)&mEsamInfo.Sw,2);/*�õ���󷵻�ֵ*/

	if(result == _PAY_ERR)
	{
		if(mEsamInfo.Sw.byte[1] == 0x69)/*""MAC���жϿ���һ��*/
		{
			mEsamInfo.mCardStatReg = 0x10;/*"MAC����"*/
		}
		else
		{
			mEsamInfo.mCardStatReg = 0x02;/*"����ESAM����"*/
		}
	}
	else
	{
		if(R_Len != NULL)
		{
			*R_Len = ((INT16U) mECBuff[2]<<8)+mECBuff[3];
		}
	}
	return result;

}
 


/*"********************************************************"*/
/*"*******************7816Ӧ�ò�***********************"*/

/*"************************************************			
Function:EC_SelectFile
Description: ѡ���ļ�
Input:  ECID:����ͨ����ʶ��fileName:�ļ���ʶ
Output:None
Return: _PAY_ERR(ʧ��) _PAY_OK(�ɹ�)
Others: None
************************************************"*/
const INT8U _SELECTFILE[4] = {0x00,0xA4,0x00,0x00};
INT8U EC_SelectFile(INT8U ECID, INT16U fileName)
{
     INT8U W_Data[2],result;
	 INT16U Rlen;
	 Rlen = 0;/*"��ֵ��ֹ����"*/

	 result = _PAY_OK;
	 /*"MCU��С�˴洢"*/
	 W_Data[1]  = (INT8U)fileName;
	 fileName >>= 8;
	 W_Data[0] = (INT8U)fileName;

	 if(ECID == _EC_ID_CPU)
	 {
		 if( EC_Cmd_Send((INT8U *)&_SELECTFILE[0],0x02,W_Data,0,ECID,0) == _PAY_ERR)
		 {
			 result = _PAY_ERR;
		 }
	 }
	 else
	 {
		 if( SPIESAM_Cmd_Send((INT8U *)&_SELECTFILE[0],0x02,W_Data,&Rlen) == _PAY_ERR)
		 {
			 result = _PAY_ERR;
		 }
	 }
	 
     return result;
}

/*"************************************************			
Function:EC_GetRnd
Description: ȡ�����
Input:  ECID:����ͨ����ʶ��len: ��ȡ���� output: ������ݴ洢��ַ
Output:None
Return: _PAY_ERR(ʧ��) _PAY_OK(�ɹ�)
Others: None
************************************************"*/
const INT8U _GETRND[4]={0x00,0x84,0x00,0x00};

INT8U EC_GetRnd(INT8U ECID, INT8U len,INT8U *output)
{
    INT8U result;
    INT8U cmdHead[4];
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/
	result = _PAY_OK;

	if(ECID == _EC_ID_CPU)
	{
		if( EC_Cmd_Send((INT8U *)&_GETRND[0],0,0,len,ECID,0) == _PAY_ERR)
		{
			result = _PAY_ERR;
		}
		Rlen = (INT16U)len;
	}
	else
	{
		cmdHead[0] = 0x80;
		cmdHead[1] = 0x04;
		cmdHead[2] = 0x00;
		cmdHead[3] = len;

		if( SPIESAM_Cmd_Send(cmdHead,0,0,&Rlen) == _PAY_ERR)
		{
			result = _PAY_ERR;
		}
	}

	if(result == _PAY_OK)
	{
		LIB_MemCpy(pECRevBuff,output,Rlen);
	}

	return result;

}

/*"************************************************			
Function:EC_Read
Description: ���Ķ��ļ�
Input:  ECID:����ͨ����ʶ��file:�ļ���ʶ
          offset : ƫ����                 len: ��ȡ���� 
Output:output: ������ݴ洢��ַ
Return: _PAY_ERR(ʧ��) _PAY_OK(�ɹ�)
Others: None
************************************************"*/
INT8U EC_Read(INT8U ECID, INT8U file, INT16U offset, INT8U len, INT8U * output)
{
    INT8U cmdHead[4];
	INT8U result;
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/
	result = _PAY_OK;

	if(ECID == _EC_ID_CPU)
	{
		cmdHead[0] = 0x00;
		cmdHead[1] = 0xB0;
		cmdHead[2] = 0x80+file;
		cmdHead[3] = offset;
		
		if(EC_Cmd_Send(cmdHead,0,0,len,ECID,0) == _PAY_ERR)
		{
			result =  _PAY_ERR;
		}
		Rlen = (INT16U)len;
	}
	else
	{
		cmdHead[0] = 0x80;
		cmdHead[1] = 0x36;
		cmdHead[2] = 0x00;
		cmdHead[3] = file;
		if(SPIESAM_Cmd_Send(cmdHead,0,0,&Rlen) == _PAY_ERR)
		{
			result =  _PAY_ERR;
		}
	}

	if(result == _PAY_OK)
	{
		LIB_MemCpy(pECRevBuff,output,Rlen);
	}

    return result;
}


/*"************************************************			
Function:EC_EsamRead
Description: ���Ķ�ESAM�ļ�
Input:  ECID:����ͨ����ʶ��file:�ļ���ʶ
          offset : ƫ����                 len: ��ȡ���� 
Output:output: ������ݴ洢��ַ
Return: _PAY_ERR(ʧ��) _PAY_OK(�ɹ�)
Others: None
************************************************"*/
INT8U EC_EsamRead(INT8U file, INT16U offset, INT8U len, INT8U * output)
{
    INT8U cmdHead[4],W_Data[5];
	INT8U result;
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/

	result = _PAY_OK;

	cmdHead[0] = 0x80;
	cmdHead[1] = 0x2C;
	cmdHead[2] = 0x00;
	cmdHead[3] = file;

	W_Data[0] = 0x00;
	W_Data[2]  = (INT8U)offset;
	offset >>= 8;
	W_Data[1] = (INT8U)offset;
	W_Data[3] = 0x00;
	W_Data[4] = len;
	
	if(SPIESAM_Cmd_Send(cmdHead,0x05,W_Data,&Rlen) == _PAY_ERR)
	{
		result =  _PAY_ERR;
	}
	else
	{
		LIB_MemCpy(pECRevBuff,output,Rlen);
	}
    return result;
	
}


/*"************************************************			
Function:EC_Read
Description: ����д�ļ�
Input:  ECID:����ͨ����ʶ��file:�ļ���ʶ
          offset : ƫ����                 len: ��ȡ���� input: д�����ݴ洢��ַ
Output:None
Return: _PAY_ERR(ʧ��) _PAY_OK(�ɹ�)
Others: None
************************************************"*/
const INT8U _WRITEORDER[4] = {0x00, 0xD6, 0x80, 0x00};
INT8U EC_Write(INT8U ECID, INT8U file, INT16U offset, INT8U len, INT8U *input)
{
    INT8U cmdHead[4],result;
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/

    result = _PAY_OK;
	if(ECID == _EC_ID_CPU)
	{
		cmdHead[0] = 0x00;
		cmdHead[1] = 0xD6;
		cmdHead[2] = (INT8U)(0x80 + file);
		cmdHead[3] = offset;
		if(EC_Cmd_Send(cmdHead,len,input,0,ECID, 0) == _PAY_ERR)
		{
			result = _PAY_ERR;
		}
	}
	else
	{
		cmdHead[0] = 0x80;
		cmdHead[1] = 0x2A;
		cmdHead[2] = 0x00;
		cmdHead[3] = file;

		input[0] = 0x00;
		input[2]  = (INT8U)offset;
		 offset >>= 8;
		input[1] = (INT8U)offset;

		if(SPIESAM_Cmd_Send(cmdHead,(INT16U)(len+3),input,&Rlen) == _PAY_ERR)
		{
			result =  _PAY_ERR;
		}
	}
    return result;
}

/*"************************************************			
Function:EC_ReadMnyBag
Description: ���Ķ�ESAMǮ���ļ���
Input:  None
Output:data:Ǯ���ļ������ݣ�����Ҫ����14�ֽ�
�������ݸ��ֽ���ǰ��˳��Ϊ:���������������ͻ����
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
INT8U EC_ReadMnyBag(INT8U *data)
{
    INT8U cmdHead[4],W_Data[2];
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/
	cmdHead[0] = 0x80;
	cmdHead[1] = 0x48;
	cmdHead[2] = 0x00;
	cmdHead[3] = 0x00;

	W_Data[0] = 0x00;

	if(SPIESAM_Cmd_Send(cmdHead,0x01,W_Data,&Rlen) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
	
	LIB_Reverse(pECRevBuff,data,4);
	LIB_Reverse(pECRevBuff+4,data+4,4);
	LIB_Reverse(pECRevBuff+8,data+8,6);

    return _PAY_OK;   
}

/*"************************************************			
Function:EC_MnyBagDec
Description: �ۼ�ESAM�е�ʣ����
Input: RmnMny:��ǰʣ����
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
INT8U EC_MnyBagDec(INT32U RmnMny)
{
    INT32U MnyBagData;
	INT8U cmdHead[4];
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/

    EC_ReadMnyBag(_CARDFILEBUF);
	LIB_MemCpy(_CARDFILEBUF,(INT8U *)&MnyBagData,4);/*"ʣ����"*/

    if(RmnMny > 0)
    {
        if(RmnMny < MnyBagData)
        {
            MnyBagData -= RmnMny;
        }
        else
        {
            MnyBagData = 0;
        }
    }

    if(MnyBagData > 0)
    {
		cmdHead[0] = 0x80;
		cmdHead[1] = 0x46;
		cmdHead[2] = 0x00;
		cmdHead[3] = 0x00;
		LIB_RvsSelf((INT8U *)&MnyBagData,4);
		if(SPIESAM_Cmd_Send(cmdHead,4,(INT8U *)&MnyBagData,&Rlen)== _PAY_ERR)
        {
            return _PAY_ERR;
        }
    }
    return _PAY_OK;
}

/*"************************************************         
Function:EC_InterAuth
Description: �ڲ���֤
Input:CpuNum:CPU����λ����
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
const INT8U _CPUINTAUTH1[4] = {0x00,0x88,0x00,0x01};
const INT8U _CPUINTAUTH2[4] = {0x80,0x08,0x08,0x01};
INT8U EC_InterAuth(INT8U *CpuNum)
{
    INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/

	LIB_MemCpy(CpuNum,pECRevBuff+100,8);
	EC_GetRnd(_EC_ID_ESAM,0x08,pECRevBuff+108);
	if(SPIESAM_Cmd_Send((INT8U *)&_CPUINTAUTH2[0],0x10,pECRevBuff+100,&Rlen) == _PAY_ERR)
	{
        return _PAY_ERR;
	}
	
    LIB_MemCpy(pECRevBuff,pECRevBuff+150,Rlen);
    if(EC_Cmd_Send((INT8U *)&_CPUINTAUTH1[0],0x08,pECRevBuff+108,0,_EC_ID_CPU,0) == _PAY_ERR)
    {
        return _PAY_ERR;
    }

    if(LIB_MemCmp(pECRevBuff,pECRevBuff+150,0x08))
    {
        return _PAY_ERR;
    }

    return _PAY_OK;
}


/*"************************************************         
Function:EC_IrAuthChk
Description: ������֤����
Input:disperse: ��ɢ���� Rnd:����� 
Output: output:��������1
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
const INT8U _IRAUTHCHK1[4] = {0x80,0x08,0x08,0x03};
INT8U EC_IrAuthChk(INT8U *disperse,INT8U *Rnd,INT8U *output)
{
	INT16U Rlen;
	INT8U tmpBuff[16];
	Rlen = 0;/*"��ֵ��ֹ����"*/

	LIB_MemCpy(disperse,tmpBuff,8);
	LIB_MemCpy(Rnd,tmpBuff+8,8);

	if(SPIESAM_Cmd_Send((INT8U *)&_IRAUTHCHK1[0],0x10,tmpBuff,&Rlen) == _PAY_ERR)
	{
        return _PAY_ERR;
	}

	LIB_MemCpy(pECRevBuff,output,8);

    return _PAY_OK;
}

/*"************************************************         
Function:EC_IrAuthChk
Description: ������֤
Input:Rnd:�����������
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
const INT8U _IRAUTH[4] = {0x80,0x06,0x00,0x01};
INT8U EC_IrAuth(INT8U *Rnd)
{
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/

	if(SPIESAM_Cmd_Send((INT8U *)&_IRAUTH[0],0x08,Rnd,&Rlen) == _PAY_ERR)
	{
		return _PAY_ERR;
	}
	
    return _PAY_OK;
}

/*"************************************************         
Function:EC_ExterAuth
Description: �ⲿ��֤
Input:CpuNum:CPU����λ����
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
const INT8U _CPUEXTAUTH1[4] = {0x80,0x08,0x08,0x02};
const INT8U _CPUEXTAUTH2[4] = {0x00,0x82,0x00,0x02};
INT8U EC_ExterAuth(INT8U *CpuNum)
{
    INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/
    LIB_MemCpy(CpuNum,pECRevBuff+100,0x08);
    EC_GetRnd(_EC_ID_CPU,0x08,pECRevBuff+108);

	if(SPIESAM_Cmd_Send((INT8U *)&_CPUEXTAUTH1[0],0x10,pECRevBuff+100,&Rlen) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
	
    LIB_MemCpy(pECRevBuff,pECRevBuff+100,Rlen);
    if(EC_Cmd_Send((INT8U *)&_CPUEXTAUTH2[0],0x08,pECRevBuff+100,0,_EC_ID_CPU,0) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
      
    return _PAY_OK;
}

/*"************************************************			
Function:EC_MnyBagInit
Description: ����Ԥ�ÿ�ESAMǮ����ʼ��
Input:  None
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ�� �µ�ʣ����
Others: None
************************************************"*/
const INT8U _BAGINIT1[4] = {0x83,0x3E,0x00,0x00};
const INT8U _BAGINIT2[4] = {0x04,0xB0,0x82,0x00};
INT8U EC_MnyBagInit(INT8U *data)
{
    INT8U tmpBuff[9];
	INT16U Rlen;
    Rlen = 0;/*"��ֵ��ֹ����"*/
    if(EC_GetRnd(_EC_ID_ESAM,0x04,tmpBuff) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
	
   	LIB_MemCpy((INT8U *)&_BAGINIT1[0],tmpBuff+4,4);
    tmpBuff[8] = 0x08;

    if(EC_Cmd_Send((INT8U *)&_BAGINIT2[0],0x09,tmpBuff,0,_EC_ID_CPU,0x08) == _PAY_ERR)
    {
        return _PAY_ERR;
    }

    LIB_MemCpy(pECRevBuff,tmpBuff,0x08);
	LIB_Reverse(pECRevBuff,data,4);/*"Ԥ�ƽ��"*/
    
    if(SPIESAM_Cmd_Send((INT8U *)&_BAGINIT1[0],0x08,tmpBuff,&Rlen)== _PAY_ERR)
    {
        return _PAY_ERR;
    } 

    return _PAY_OK;

}

/*"************************************************         
Function:EC_EsamUpdate
Description: ESAM�����ļ�����
Input:CpuFileName:CPU���ļ���ʶ   EsamFileName:ESAM�ļ���ʶ
      CpuAddr:CPU���ݸ�����ʼ��ַ EsamAddr: ESAM���ݸ�����ʼ��ַ
      len:���ݳ���                output:
      MeterInsFlg:                BcdMeterID:
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
INT8U EC_EsamUpdate(INT8U CpuFileName,
                        INT8U EsamFileName,
                        INT8U CpuAddr,
                        INT8U EsamAddr,
                        INT8U len,
                        INT8U *output)
{
    INT8U cmdHead[4],tmpBuff[9];
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/
    if(EC_GetRnd(_EC_ID_ESAM,0x04,tmpBuff) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
    tmpBuff[4] = 0x83;
    tmpBuff[5] = 0x2A;
    tmpBuff[6] = (INT8U)(0x80+EsamFileName);;
    tmpBuff[7] = EsamAddr;
    tmpBuff[8] = (INT8U)(len+4);

    cmdHead[0] = 0x04;
    cmdHead[1] = 0xB0;
    cmdHead[2] = (INT8U)(0x80+CpuFileName);
    cmdHead[3] = CpuAddr;

    if(EC_Cmd_Send(cmdHead,0x09,tmpBuff,0,_EC_ID_CPU,(INT8U)(len+4)) == _PAY_ERR)
    {
        return _PAY_ERR;
    }

    LIB_MemCpy(pECRevBuff,pECRevBuff+140,(INT8U)(len+4));
    if(output != 0)
    {
        LIB_MemCpy(pECRevBuff,output,len);
    }

    cmdHead[0] = 0x83;
    cmdHead[1] = 0x2A;
    cmdHead[2] = (INT8U)(0x80+EsamFileName);;
    cmdHead[3] = EsamAddr;

	LIB_MemCpy((INT8U *)&tmpBuff[4],(INT8U *)&cmdHead[0],4);
	
    if(SPIESAM_Cmd_Send(cmdHead,(INT16U)(len+4),pECRevBuff+140,&Rlen) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
     
    return _PAY_OK;
}

/*"************************************************			
Function:EC_MnyBagAdd ��CPU������Ǯ�����ֵ
Description: ��addrΪ��ʱ��ESAMǮ����ֵ����addrΪ�ǿ�ʱ������CPU��ǰʣ���Ѽ��������
Input: fileName: ����ʣ�������ڵ��ļ���
          offset: ����ʣ�������ڵ��ļ����׵�ַ��ƫ����
Output:addr:��ָ��Ϊ����ʱ������ESAM�е�ʣ����������������Ϊ��ʱ������
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
INT8U EC_MnyBagAdd(INT8U fileName,INT8U offset,INT8U *addr)
{
    INT8U tmpBuf[20],cmdHead[4];
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/
    if(EC_GetRnd(_EC_ID_ESAM,0x04,tmpBuf) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
    tmpBuf[4] = 0x80;
    tmpBuf[5] = 0x42;
    tmpBuf[6] = 0x00;
    tmpBuf[7] = 0x00;
    tmpBuf[8] = 0x0C;
    
    cmdHead[0] = 0x04;
    cmdHead[1] = 0xB0;
    cmdHead[2] = (INT8U)(0x80+fileName);
    cmdHead[3] = offset;
    if(EC_Cmd_Send(cmdHead,0x09,tmpBuf,0,_EC_ID_CPU,0x0C) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
    
    if(addr != 0)
    {
        LIB_Reverse(pECRevBuff,addr,0x04);
        LIB_Reverse(pECRevBuff+4,addr+4,0x04);
    }
    else
    {
        cmdHead[0] = 0x80;
        cmdHead[1] = 0x42;
        cmdHead[2] = 0x00;
        cmdHead[3] = 0x00;
		LIB_MemCpy((INT8U *)&tmpBuf[4],(INT8U *)&cmdHead[0],4);
        LIB_MemCpy(pECRevBuff,tmpBuf,0x0C);

	 	if(SPIESAM_Cmd_Send(cmdHead,0x0C,tmpBuf,&Rlen) == _PAY_ERR)
     	{
        	return _PAY_ERR;
    	}
    }
    return _PAY_OK;
}

/*"************************************************         
Function:EC_CpuUpdate
Description: ESAM�����ļ�����
Input:CpuFileName:CPU���ļ���ʶ   EsamFileName:ESAM�ļ���ʶ
      CpuAddr:CPU���ݸ�����ʼ��ַ EsamAddr: ESAM���ݸ�����ʼ��ַ
      len:���ݳ���                output:
      MeterInsFlg:                BcdMeterID:
Output:None
Return: _PAY_OK�ɹ�_PAY_ERRʧ��
Others: None
************************************************"*/
INT8U EC_CpuUpdate(INT8U cpuFileName,
                        INT8U EsamFileName,
                        INT8U CpuAddr,
                        INT8U EsamAddr,
                        INT8U len,
                        INT8U *CpuNum)
{
    INT8U tmpBuff[30],cmdHead[5];
	INT16U Rlen;
	Rlen = 0;/*"��ֵ��ֹ����"*/

	tmpBuff[0] 	= 0x48;
	LIB_MemCpy(CpuNum,tmpBuff+1,8);
    if(EC_GetRnd(_EC_ID_CPU,4,(INT8U *)&tmpBuff[9]) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
	tmpBuff[13] = 0x00;
    tmpBuff[14] = EsamAddr;
	tmpBuff[15] = 0x00;
	tmpBuff[16] = len;
	tmpBuff[17] = 0x04;
    tmpBuff[18] = 0xD6;
    tmpBuff[19] = (INT8U)(0x80+cpuFileName);
    tmpBuff[20] = CpuAddr;
    tmpBuff[21] = (INT8U)(len+4);
    
    cmdHead[0] = 0x83;
    cmdHead[1] = 0x2C;
    cmdHead[2] = 0x00;
    cmdHead[3] = EsamFileName;

    if(SPIESAM_Cmd_Send(cmdHead,22,tmpBuff, &Rlen) == _PAY_ERR)
    {
        return _PAY_ERR;
    }
    
    LIB_MemCpy(pECRevBuff,pECRevBuff+100,len+4);
    cmdHead[0] = 0x04;
    cmdHead[1] = 0xD6;
    cmdHead[2] = (INT8U)(0x80+cpuFileName);
    cmdHead[3] = CpuAddr;
    if(EC_Cmd_Send(cmdHead,(INT8U)(len+4),pECRevBuff+100,0,_EC_ID_CPU,0)  ==  _PAY_ERR)
    {
        return _PAY_ERR;
    }

    return _PAY_OK;
}
