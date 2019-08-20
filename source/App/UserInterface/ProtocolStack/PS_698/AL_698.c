#include "..\..\..\..\PlatForm.h"
#include"AL_698.h"
#include"DL_698.h"
#include"driver\device\com.h"
#include"lib\LiB_func.h"
#include"app\userinterface\PP\PP_698\Process\Pp_macro.h"
#include"app\userinterface\PP\PP_698\Process\Pp_process.h"
#include"Lib\AXDR\AXDR.h"
#include"DataCenterSoftware\Code\message\MS_OBIS.h"
#include "app\userinterface\UI.h"

/*�������İ�ȫ�����*/
//#define AL_DEBUG_SECURITY_REQ

EN_COM_ID NowComId = E_COM_RS485_1;/*��ǰͨ��*/
INT8U ApduBuf[E_MAX_COM_ID][MAX_APDU_LEN];
ST_CONNECT_INFO ConnectInfo[E_MAX_COM_ID];


static INT8U Deal_Security_Req(INT8U *pBuf,INT16U * OutLen,EN_ENCRYPT_TYPE *EcMode);
static INT8U Deal_Security_Resp(INT8U *pBuf,INT16U SendAdpuLen,INT16U *OutLen,EN_SECURITY_TYPE SecurityType);
static void Deal_Release_Req(INT8U *pBuf,INT16U * OutLen,INT8U ComId);
static INT8U IsSeviceTypeVaild(EN_SERVICE_TYPE SeviceType);
static void SetDisConnectState(EN_COM_ID ComId);

void AL_698_Init(void)
{
    EN_COM_ID i;
    for(i=E_COM_RS485_1;i < E_MAX_COM_ID;i++)
    {
        SetDisConnectState(i);
    }
}


/*****************************************************************************
 �� �� ��  : Encode_Error_Response
 ��������  : ����쳣��Ӧ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 01 12
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void Encode_Error_Response(INT8U *pApduBuf,INT8U ErrType)
{    
    pApduBuf[0]= EN_ABNORMAL_REPONSE;
    pApduBuf[1]= 0;           /*PIID �޷�׼ȷ���أ���ʱ�ȷ���0��*/
    pApduBuf[2]= ErrType;     /*�쳣����::����֧��*/
    pApduBuf[3]= 0;           /*�޸����ϱ�*/
    pApduBuf[4]= 0;           /*��ʱ���ǩ*/
}
 
void AL_698_Main(void)
{

    INT16S TmpRet;
    ST_698_FRAME_INFO FrameInfo;
    INT8U *pApduBuf;
    EN_SERVICE_TYPE SeviceType;
    ST_OBJECT TmpObj;
    INT16U SendApduLen;
    INT16U CryptRet;
    EN_ENCRYPT_TYPE TmpEcMode;
    INT8U CryptFlag;
    INT16U TmpLen;
    INT16S RetPP;

        
    SendApduLen = 0;

    /*E_COM_RS485_1 ����Ҫ��0��ʼ�������������*/
    BUILD_BUG_ON(E_COM_RS485_1 != 0);
    
    for(NowComId = E_COM_RS485_1;NowComId < E_MAX_COM_ID;NowComId++)
    {
        
        pApduBuf = &ApduBuf[NowComId][0];
        TmpRet = DL_698_FrameCheck(NowComId, pApduBuf, MAX_APDU_LEN,&FrameInfo);
        if(TmpRet > 0)/*��⵽�Ϸ�֡*/
        {
            /*ˢ��Ӧ�����ӳ�ʱʱ��*/
            ConnectInfo[NowComId].ConnectTimeRem = ConnectInfo[NowComId].ConnectOverTime;
            TmpObj.AddrType = FrameInfo.AddrType;/*��ַ����*/
            
            /*�������ʹ���*/
            SeviceType = (EN_SERVICE_TYPE)pApduBuf[0];
            if(SeviceType == EN_SECURITYREQUEST)/*��ȫ�����Ƚ���*/
            {
                CryptFlag = pApduBuf[1];/*�������Ļ�������*/
				
                /*���ý��ܵĽӿڽ�������*/
				#ifdef THREE_PHASE
				if(TRUE == ChkStatus(E_LOW_PWR))/*"�����͹���ģʽ�²�����ִ��ESAM"*/
				{
					CryptRet = DAR_1_HW_FAIL;
				}
				else
				#endif
				{
                	CryptRet = Deal_Security_Req(&pApduBuf[0],&TmpObj.Len_data,&TmpEcMode);
				}
				
                if(CryptRet == DAR_0_OK)
                {
                    if(CryptFlag == 0)/*����*/
                    {
                        TmpObj.Crypt_Type = (EN_SECURITY_TYPE)(TmpEcMode+1);
                    }
                    else/*����*/
                    {
                        TmpObj.Crypt_Type = (EN_SECURITY_TYPE)((TmpEcMode+1)|ENCRYPTED);
                    }
                    /*�����͵�Э�����ģ��ǰ�Ĵ���*/
                    TmpObj.servicetype = (EN_SERVICE_TYPE)pApduBuf[0];
                    TmpObj.pdata =  &pApduBuf[1];

                    #ifdef AL_DEBUG_SECURITY_REQ
                    if(NowComId != E_COM_RS485_1)
                    {
                        LIB_MemCpy(&pApduBuf[0],&ApduBuf[E_COM_RS485_1][0],TmpObj.Len_data);
                        COM_StartSend(E_COM_RS485_1,&ApduBuf[E_COM_RS485_1][0],TmpObj.Len_data);
                    }
                    #endif
                }
                else
                {
                    /*����ʧ�ܲ�ֱ�Ӵ��*/
                    pApduBuf[0] = EN_SECURITYREQUEST|0x80;
                    pApduBuf[1] = 0x02;/*�쳣����*/
                    pApduBuf[2] = CryptRet;
                    pApduBuf[3] = 0;  /*CHOICE   OPTIONAL ��*/
                    SendApduLen = 4;
                    DL_698_FrameSend(NowComId, pApduBuf,SendApduLen);
                    return ;
                }   
            }
            else/*�ǰ�ȫ��������*/
            {   
                TmpObj.servicetype = SeviceType;
                TmpObj.Crypt_Type  = UN_ENCRYPTED;
                TmpObj.Len_data    = TmpRet;
                TmpObj.pdata       = &pApduBuf[1];
            }
            
            /*�ж�sevrice �Ƿ�Ϸ�,���Ϸ����쳣��Ӧ*/
            if(TRUE== IsSeviceTypeVaild(TmpObj.servicetype))
            {
                if(TmpObj.servicetype == EN_CONNECTREQUEST)
                {
                }
                else if(TmpObj.servicetype == EN_DISCONNECTREQUEST)
                {
                    Deal_Release_Req(&pApduBuf[0],&SendApduLen,NowComId);
                }
                else /*���ݵ�Э�鴦��ģ��*/
                {
                    RetPP = PP_process(&TmpObj);/*TmpLen���Ȳ������������ͣ���˺�����Ҫ+1*/ 
                    if(0>RetPP)
                    {
                        Encode_Error_Response(pApduBuf,E_OTHER_ERR);
                        SendApduLen= 5;
                    }
                    else
                    {       
                        TmpLen = RetPP;
                        if(SeviceType == EN_SECURITYREQUEST)
                        {
                            /*���ص����������Ѿ��ŵ�pApduBuf[1],���ü��ܽӿڼ�������*/
                            pApduBuf[0] = TmpObj.servicetype|0x80;/*��service type�ŵ���һ���ֽ�*/
                            CryptRet = Deal_Security_Resp(&pApduBuf[0],TmpLen+1,&SendApduLen,(EN_SECURITY_TYPE)TmpObj.Crypt_Type);
                            /*��Ҫ���� EN_SECURITYREQUEST|80 + ���Ļ����� + ���ȱ���*/
                            TmpLen = 2;
    						if(CryptRet == DAR_0_OK)
    						{
    							if(UN_ENCRYPTED_WITH_RN == TmpObj.Crypt_Type)/*���ĳ���*/
    							{
    							    /*"SendApduLen-7   �������������ȥ01 00 04 +4�ֽ�MACֵ"*/
    								TmpLen += EnCode_LenArea(SendApduLen-7, NULL);
    								LIB_MemCpy(&pApduBuf[0], &pApduBuf[TmpLen], SendApduLen);/*����N�ֽ�*/
    								SendApduLen += EnCode_LenArea(SendApduLen-7, &pApduBuf[2]);/*���볤��*//*"SendApduLen��ӳ����ֽ�"*/
    								/*"SendApduLen-7   �������������ȥ01 00 04 +4�ֽ�MACֵ"*/
    								CryptRet = UN_ENCRYPTED;	/*"����ѡ������"*/

    							}
    							else
    							{
    								LIB_MemCpy(&pApduBuf[0], &pApduBuf[TmpLen], SendApduLen);/*����N�ֽ�*/
    								if(TmpObj.Crypt_Type & ENCRYPTED)
    								{
    									CryptRet = 1;	/*"����ѡ������"*/
    								}
    								else
    								{
    									CryptRet = UN_ENCRYPTED;	/*"����ѡ������+MAC"*/
    								}
    							}
    						}
    						else
    						{
    							pApduBuf[2] = CryptRet;	/*"������"*/
    							CryptRet = 0x02;		/*"����ѡ��DAR"*/
    							SendApduLen = 1;
    						}

    						pApduBuf[0] = EN_SECURITYREQUEST|0x80;
                            pApduBuf[1] = CryptRet;
    						SendApduLen += 2;/*"SendApduLen��ӷ�������2�ֽ�"*/
                        }
                        else/*�ǰ�ȫ�����Ӧ*/
                        {   
                            SendApduLen = TmpLen;
                            pApduBuf[0]=SeviceType|0x80;
                            SendApduLen ++;
                        }
                    }
                }
            }
            else
            {
                Encode_Error_Response(pApduBuf,E_NO_SERVICE);
                SendApduLen= 5;
            }

            DL_698_FrameSend(NowComId, pApduBuf,SendApduLen);
        }
        else if(TmpRet < 0)/*δ��⵽�Ϸ�֡*/
        {
        }
        else
        {

        }

    }

}


void AL_698_1Sec(void)
{
#ifdef PM_MODLE

    EN_COM_ID ComNum;
    INT32U TemRmTime;
    
    for(ComNum = E_COM_RS485_1;ComNum < E_MAX_COM_ID;ComNum++)
    {
        if(ConnectInfo[ComNum].ConnectTimeRem>0)
        {
            ConnectInfo[ComNum].ConnectTimeRem--;
            GetSingle(E_IDAUTH_RMN_TIME,(INT8U*)&TemRmTime);
            
            if(TemRmTime == 0)/*�����֤��Чʱ�䵽*/
            {
                SetDisConnectState(ComNum);
            }
            else/*�����֤ʱ��δ��*/
            {
                if(ConnectInfo[ComNum].ConnectTimeRem == 0)/*ͨ������ʱ������*/
                {
                    SetDisConnectState(ComNum);
                    Action (E_ESAM_DISCONNECT, NULL, NULL);
                }
            }
        }
        else
        {
        }
    }
#endif    
}




/*"*******************************************************************			
Function:AL_DataDecrypt
Description:Э��Ӧ�����ݲ��ð�ȫ����ʱ�����ݽ���
Input:  ComString:ָ��Ӧ�����ݵ�Ԫ  10+CHOICE����ָ��00��01�����ܺ��������ʼ�ֽ�ָ��
	    Len:���ܺ�����ݳ���
	    EcMode:������֤��Ϣ  CHOICEģʽ������ģʽ
	    
		������֤��		[0]  SID_MAC��
		�����				[1]  RN��
		�����+����MAC	[2]  RN_MAC��
		��ȫ��ʶ			[3]  SID
Output:None
Return: DAR   ���ݷ��ʽ��
Others: None
*********************************************************************"*/
INT8U AL_DataDecrypt(INT8U *ComString,INT16U *Len,EN_ENCRYPT_TYPE *EcMode)
{
    INT8U *pdata;
    INT8U Offset,result;
    INT8U Type_Encrypt,ecmode;
	INT16U ApLen,ApLen1;

	result = DAR_0_OK;

    pdata = ComString;
    Type_Encrypt = *(pdata+1);   /*"�������ͣ����Ļ�����"*/
    Offset = 0;
    /*"ǰ���ֽ�Ϊsec(0x10) + ���� ��sec(0x10) + ����"*/
    /*"pdata+2�������(��0x81+1�ֽڳ��ȳ�128����0x82+���ֽڳ��ȳ�256)"*/
	ApLen = DeCode_LenArea(pdata+2,&Offset);
	ApLen1 =(ApLen+Offset+2);/*2�ֽڰ���:sec(0x10) + ����(CHOICE)*/
   
    ecmode = *(pdata+ApLen1);/*"��������"*/
    *EcMode = (EN_ENCRYPT_TYPE)ecmode;/*"���ؼ���ģʽ"*/
	if(0 == Type_Encrypt)        /*����+ MAC������+�����*/
	{
        if(ecmode == RNDATA)/*"�·���ʽΪ�����"*/
		{
		    /*������ݴ棬���ֽڰ���01+10+�����*/
			LIB_MemCpy(pdata+ApLen1+2, mEsamInfo.APRnd, 16);/*"������̶�ȡ16�ֽ�"*/
            LIB_MemCpy(pdata+Offset+2,pdata,ApLen); /*"�����������ݵ�������ݵ�ַ"*/
        }
		else if((ecmode == SIDMACDATA) ||(ecmode == SIDDATA)) /*"�·���ʽΪSID_MAC"*/
		{ 
		}       
	}
	else if(1 == Type_Encrypt)/*����*/
	{		
	}
	else
    {     
		result = DAR_23_AUTHFAIL;
    } 

	*Len = ApLen;/*"���ص��������ݳ���"*/

    return result;
	
}    



/*"************************************************			
Function: 	Deal_Security_Req		
Description: ����ȫ��������
             ���Ļ��沿�����ݣ������ش��� 					
Input:  
Output: ��	
Return: 	
Others: 
************************************************"*/	
INT8U Deal_Security_Req(INT8U *pBuf,INT16U *OutLen,EN_ENCRYPT_TYPE *EcMode)
{
   
    INT16U Len;
    INT8U Ret;

    Ret = DAR_0_OK;

	Ret = AL_DataDecrypt(pBuf,&Len,EcMode);

	if(Ret == DAR_0_OK)
	{
		*OutLen = Len;
	}

    return Ret;
}    


INT8U Deal_Security_Resp(INT8U *pBuf,INT16U SendAdpuLen,INT16U *OutLen,EN_SECURITY_TYPE SecrityType)
{
	INT8U result;

	result = DAR_0_OK;
    if(UN_ENCRYPTED_WITH_RN == SecrityType)/*���ĳ���*/
    {

    }
    else
    {

    }
	return result;
}

INT8U IsSeviceTypeVaild(EN_SERVICE_TYPE SeviceType)
{
    if( (SeviceType == EN_CONNECTREQUEST)
        ||(SeviceType == EN_DISCONNECTREQUEST)
        ||(SeviceType == EN_GETREQUEST)
        ||(SeviceType == EN_SETREQUEST)
        ||(SeviceType == EN_ACTIONREQUEST)
        ||(SeviceType == EN_SECURITYREQUEST)
        ||(SeviceType == EN_ABNORMAL_REPONSE)
        )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}


static void SetDisConnectState(EN_COM_ID ComId)
{
    INT8U i;
    INT8U Flag;

    Flag = TRUE;
    ConnectInfo[ComId].ConnectLevel = NULL_SECURITY;
    ConnectInfo[ComId].ConnectOverTime = 0;
    ConnectInfo[ComId].ConnectTimeRem = 0;
    ConnectInfo[ComId].MaxRecSendLen = 0;
    ConnectInfo[ComId].ClientAddr = 0;

    for(i=0;i<E_MAX_COM_ID;i++)
    {
        if(ConnectInfo[ComId].ConnectLevel == NULL_SECURITY)
        {
        }
        else
        {
            Flag = FALSE;
            break;  
        }
    }
    /*����ͨ����Ϊ�Ͽ�����״̬*/
    if(TRUE == Flag)
    {
        Push(E_AUTH_STATUS,FALSE);
    }
}

const INT8U ConnectResInfo[]=
{
    /*�̶���Э��һ���Կ�  PROTOCOLCONFORMANCE  FFFFFFFFC0000000H ȫ��֧��*/
	0xff,0xff,0xff,0xff,0xC0,0x00,0x00,0x00, 
    /*�̶��Ĺ���һ���Կ�FUNCTIONCONFORMANCE  FFFEC400000000000000000000000000H - bit0-bit14��bit16��bit17��bit21*/
	0xff,0xFE,0xC4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};


void Deal_Release_Req(INT8U *pBuf,INT16U * OutLen,INT8U ComId)
{
#ifdef PM_MODLE

    INT8U *TmppBuf = pBuf;
    INT8U Dar = CONNECT_OK;
    INT8U i;
    INT8U TimeFlag;
    INT8U TimeTag[LEN_TIME_TAG];


    TimeFlag = FALSE;
    
    /*ƫ�Ƶ�ʱ���ǩ*/
    TmppBuf +=2;   
    
    /*ʱ���ǩ����*/
    if(TRUE == DeCode_Option(TmppBuf))
    {
        TimeFlag = TRUE;
        Dar = check_time_tag(TmppBuf);
        if(Dar!= DAR_0_OK)
        {
            Dar = OTHER_ERR;  
        }
        LIB_MemCpy(TmppBuf, TimeTag, LEN_TIME_TAG);
        TmppBuf += LEN_TIME_TAG;
    }
    else
    {

    }

    /*׼������*/
    TmppBuf = pBuf;
    *TmppBuf = EN_DISCONNECTREQUEST|0x80;/*Ӧ�����ӷ���*/
    TmppBuf += 2;/*ƫ�Ƶ�PIID_ACD��*/

    if(CONNECT_OK == Dar)
    {
        for(i= 0;i<E_MAX_COM_ID;i++)/*����ͨ�����Ͽ�*/
        {
            SetDisConnectState((EN_COM_ID)i);
        }    
        Action (E_ESAM_DISCONNECT, NULL, NULL);/*���ESAM������Ϣ*/
    }
    
    LIB_MemSet(Dar, TmppBuf, 1);
    TmppBuf++;

    /*�����ϱ���Ϣ��*/
    LIB_MemSet(0x00, TmppBuf, 1);
    TmppBuf++;
    
    if(TimeFlag == TRUE)
    {
        LIB_MemCpy(TimeTag,TmppBuf, LEN_TIME_TAG);
        TmppBuf += LEN_TIME_TAG;
    }
    else/*��ʱ���ǩ*/
    {
        LIB_MemSet(0x00, TmppBuf, 1);
        TmppBuf++;
    }

    *OutLen = (INT16U)(TmppBuf- pBuf);
#endif    
}
