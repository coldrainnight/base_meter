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

/*调试密文安全请求宏*/
//#define AL_DEBUG_SECURITY_REQ

EN_COM_ID NowComId = E_COM_RS485_1;/*当前通道*/
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
 函 数 名  : Encode_Error_Response
 功能描述  : 打包异常响应
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2018 01 12
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void Encode_Error_Response(INT8U *pApduBuf,INT8U ErrType)
{    
    pApduBuf[0]= EN_ABNORMAL_REPONSE;
    pApduBuf[1]= 0;           /*PIID 无法准确返回，暂时先返回0吧*/
    pApduBuf[2]= ErrType;     /*异常类型::服务不支持*/
    pApduBuf[3]= 0;           /*无跟随上报*/
    pApduBuf[4]= 0;           /*无时间标签*/
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

    /*E_COM_RS485_1 必须要从0开始，否则会有问题*/
    BUILD_BUG_ON(E_COM_RS485_1 != 0);
    
    for(NowComId = E_COM_RS485_1;NowComId < E_MAX_COM_ID;NowComId++)
    {
        
        pApduBuf = &ApduBuf[NowComId][0];
        TmpRet = DL_698_FrameCheck(NowComId, pApduBuf, MAX_APDU_LEN,&FrameInfo);
        if(TmpRet > 0)/*检测到合法帧*/
        {
            /*刷新应用连接超时时间*/
            ConnectInfo[NowComId].ConnectTimeRem = ConnectInfo[NowComId].ConnectOverTime;
            TmpObj.AddrType = FrameInfo.AddrType;/*地址类型*/
            
            /*服务类型处理*/
            SeviceType = (EN_SERVICE_TYPE)pApduBuf[0];
            if(SeviceType == EN_SECURITYREQUEST)/*安全请求先解密*/
            {
                CryptFlag = pApduBuf[1];/*解析明文或是密文*/
				
                /*调用解密的接口解密数据*/
				#ifdef THREE_PHASE
				if(TRUE == ChkStatus(E_LOW_PWR))/*"三相表低功耗模式下不允许执行ESAM"*/
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
                    if(CryptFlag == 0)/*明文*/
                    {
                        TmpObj.Crypt_Type = (EN_SECURITY_TYPE)(TmpEcMode+1);
                    }
                    else/*密文*/
                    {
                        TmpObj.Crypt_Type = (EN_SECURITY_TYPE)((TmpEcMode+1)|ENCRYPTED);
                    }
                    /*处理传送到协议解析模块前的处理*/
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
                    /*返回失败并直接打包*/
                    pApduBuf[0] = EN_SECURITYREQUEST|0x80;
                    pApduBuf[1] = 0x02;/*异常错误*/
                    pApduBuf[2] = CryptRet;
                    pApduBuf[3] = 0;  /*CHOICE   OPTIONAL 无*/
                    SendApduLen = 4;
                    DL_698_FrameSend(NowComId, pApduBuf,SendApduLen);
                    return ;
                }   
            }
            else/*非安全请求数据*/
            {   
                TmpObj.servicetype = SeviceType;
                TmpObj.Crypt_Type  = UN_ENCRYPTED;
                TmpObj.Len_data    = TmpRet;
                TmpObj.pdata       = &pApduBuf[1];
            }
            
            /*判定sevrice 是否合法,不合法回异常响应*/
            if(TRUE== IsSeviceTypeVaild(TmpObj.servicetype))
            {
                if(TmpObj.servicetype == EN_CONNECTREQUEST)
                {
                }
                else if(TmpObj.servicetype == EN_DISCONNECTREQUEST)
                {
                    Deal_Release_Req(&pApduBuf[0],&SendApduLen,NowComId);
                }
                else /*传递到协议处理模块*/
                {
                    RetPP = PP_process(&TmpObj);/*TmpLen长度不包括服务类型，因此后续需要+1*/ 
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
                            /*返回的明文数据已经放到pApduBuf[1],调用加密接口加密数据*/
                            pApduBuf[0] = TmpObj.servicetype|0x80;/*把service type放到第一个字节*/
                            CryptRet = Deal_Security_Resp(&pApduBuf[0],TmpLen+1,&SendApduLen,(EN_SECURITY_TYPE)TmpObj.Crypt_Type);
                            /*需要插入 EN_SECURITYREQUEST|80 + 明文或密文 + 长度编码*/
                            TmpLen = 2;
    						if(CryptRet == DAR_0_OK)
    						{
    							if(UN_ENCRYPTED_WITH_RN == TmpObj.Crypt_Type)/*明文抄读*/
    							{
    							    /*"SendApduLen-7   数据区长度需减去01 00 04 +4字节MAC值"*/
    								TmpLen += EnCode_LenArea(SendApduLen-7, NULL);
    								LIB_MemCpy(&pApduBuf[0], &pApduBuf[TmpLen], SendApduLen);/*后移N字节*/
    								SendApduLen += EnCode_LenArea(SendApduLen-7, &pApduBuf[2]);/*插入长度*//*"SendApduLen需加长度字节"*/
    								/*"SendApduLen-7   数据区长度需减去01 00 04 +4字节MAC值"*/
    								CryptRet = UN_ENCRYPTED;	/*"返回选择明文"*/

    							}
    							else
    							{
    								LIB_MemCpy(&pApduBuf[0], &pApduBuf[TmpLen], SendApduLen);/*后移N字节*/
    								if(TmpObj.Crypt_Type & ENCRYPTED)
    								{
    									CryptRet = 1;	/*"返回选择密文"*/
    								}
    								else
    								{
    									CryptRet = UN_ENCRYPTED;	/*"返回选择明文+MAC"*/
    								}
    							}
    						}
    						else
    						{
    							pApduBuf[2] = CryptRet;	/*"错误码"*/
    							CryptRet = 0x02;		/*"返回选择DAR"*/
    							SendApduLen = 1;
    						}

    						pApduBuf[0] = EN_SECURITYREQUEST|0x80;
                            pApduBuf[1] = CryptRet;
    						SendApduLen += 2;/*"SendApduLen需加服务类型2字节"*/
                        }
                        else/*非安全请求回应*/
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
        else if(TmpRet < 0)/*未检测到合法帧*/
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
            
            if(TemRmTime == 0)/*身份认证有效时间到*/
            {
                SetDisConnectState(ComNum);
            }
            else/*身份认证时长未到*/
            {
                if(ConnectInfo[ComNum].ConnectTimeRem == 0)/*通信连接时长到了*/
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
Description:协议应用数据采用安全传输时，数据解密
Input:  ComString:指向应用数据单元  10+CHOICE数据指针00或01，解密后的数据起始字节指针
	    Len:解密后的数据长度
	    EcMode:数据验证信息  CHOICE模式，加密模式
	    
		数据验证码		[0]  SID_MAC，
		随机数				[1]  RN，
		随机数+数据MAC	[2]  RN_MAC，
		安全标识			[3]  SID
Output:None
Return: DAR   数据访问结果
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
    Type_Encrypt = *(pdata+1);   /*"加密类型，明文或密文"*/
    Offset = 0;
    /*"前两字节为sec(0x10) + 明文 或sec(0x10) + 密文"*/
    /*"pdata+2后面就是(或0x81+1字节长度超128、或0x82+两字节长度超256)"*/
	ApLen = DeCode_LenArea(pdata+2,&Offset);
	ApLen1 =(ApLen+Offset+2);/*2字节包括:sec(0x10) + 明文(CHOICE)*/
   
    ecmode = *(pdata+ApLen1);/*"加密类型"*/
    *EcMode = (EN_ENCRYPT_TYPE)ecmode;/*"返回加密模式"*/
	if(0 == Type_Encrypt)        /*明文+ MAC或明文+随机数*/
	{
        if(ecmode == RNDATA)/*"下发格式为随机数"*/
		{
		    /*随机数暂存，两字节包括01+10+随机数*/
			LIB_MemCpy(pdata+ApLen1+2, mEsamInfo.APRnd, 16);/*"随机数固定取16字节"*/
            LIB_MemCpy(pdata+Offset+2,pdata,ApLen); /*"拷贝明文数据到输出数据地址"*/
        }
		else if((ecmode == SIDMACDATA) ||(ecmode == SIDDATA)) /*"下发格式为SID_MAC"*/
		{ 
		}       
	}
	else if(1 == Type_Encrypt)/*密文*/
	{		
	}
	else
    {     
		result = DAR_23_AUTHFAIL;
    } 

	*Len = ApLen;/*"返回的明文数据长度"*/

    return result;
	
}    



/*"************************************************			
Function: 	Deal_Security_Req		
Description: 处理安全传输请求
             明文缓存部分数据，待返回处理 					
Input:  
Output: 无	
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
    if(UN_ENCRYPTED_WITH_RN == SecrityType)/*明文抄读*/
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
    /*所有通道均为断开连接状态*/
    if(TRUE == Flag)
    {
        Push(E_AUTH_STATUS,FALSE);
    }
}

const INT8U ConnectResInfo[]=
{
    /*商定的协议一致性块  PROTOCOLCONFORMANCE  FFFFFFFFC0000000H 全部支持*/
	0xff,0xff,0xff,0xff,0xC0,0x00,0x00,0x00, 
    /*商定的功能一致性块FUNCTIONCONFORMANCE  FFFEC400000000000000000000000000H - bit0-bit14，bit16，bit17，bit21*/
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
    
    /*偏移到时间标签*/
    TmppBuf +=2;   
    
    /*时间标签处理*/
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

    /*准备返回*/
    TmppBuf = pBuf;
    *TmppBuf = EN_DISCONNECTREQUEST|0x80;/*应用连接返回*/
    TmppBuf += 2;/*偏移到PIID_ACD后*/

    if(CONNECT_OK == Dar)
    {
        for(i= 0;i<E_MAX_COM_ID;i++)/*所有通道均断开*/
        {
            SetDisConnectState((EN_COM_ID)i);
        }    
        Action (E_ESAM_DISCONNECT, NULL, NULL);/*清除ESAM连接信息*/
    }
    
    LIB_MemSet(Dar, TmppBuf, 1);
    TmppBuf++;

    /*跟随上报信息域*/
    LIB_MemSet(0x00, TmppBuf, 1);
    TmppBuf++;
    
    if(TimeFlag == TRUE)
    {
        LIB_MemCpy(TimeTag,TmppBuf, LEN_TIME_TAG);
        TmppBuf += LEN_TIME_TAG;
    }
    else/*无时间标签*/
    {
        LIB_MemSet(0x00, TmppBuf, 1);
        TmppBuf++;
    }

    *OutLen = (INT16U)(TmppBuf- pBuf);
#endif    
}
