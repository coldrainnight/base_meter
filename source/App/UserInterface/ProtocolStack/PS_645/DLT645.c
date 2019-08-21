

#include "App\UserInterface\ProtocolStack\ProtocolStack.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
#include "Lib\LIB_func.h"
#include "App\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "app\userinterface\UI.h"
#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"
#include "App\UserInterface\ProtocolStack\host_commu\host_commu.h"
//#include "Driver\Device\pkt_proc.h"
#include "Driver\Device\spi_slave.h"
#include "app\userinterface\Security\tca.h"


static INT8U mCommAddrType; /*"ÿ��ͨ�Ž�֡��ͨ�ŵ�ַ����"*/
static INT8U mAuthority;/*""��ǰ�ܼ�*/
static INT8U IrStateflg;/*"�����Ƿ�ͨ����־"*/

static ST_CONTINUEDFRAME mstNextFrame[E_MAX_COM_ID];  /*"����֡��Ϣ"*/
 INT8U reportComBegin[E_MAX_COM_ID] = {0};/*"���������ϱ�����֡Ӧ��"*/

 INT8U CM_AnswerCmd(INT8U rCommPort, INT8U *pdataBuff, INT8U len);
static INT8U CM_ReadData(ST_FRAME645 *rpResult, INT16S *rpDataLength);
static INT8U CM_WriteData(ST_FRAME645 *rpResult);
static INT8U CM_15Cmd(ST_FRAME645 *rpResult);
//static INT8U CM_1CCmd(ST_FRAME645 *rpResult);  
static INT8U CM_ClrCmd(ST_FRAME645 *rpResult);   
static INT16U CM_Far03Cmd(INT8U *Pdata);
static INT8U CM_ReadListData(ST_FRM_RESULT *framedata, ST_ANSWER_DATA * nAnswer);
extern INT8U bm_slave_rx_proc(ST_FRAME645 *frm, INT16S *ans_len);
extern INT8U pkt_queue_rx_proc(ST_FRAME645 *res, INT8U len, INT8U dev);


const INT8U mResetPwd[7] = {0x07,0xb8,0x72,0x05,0x49,0x91,0xcd};

const ST_PROCESS_INFO ProCess_Info[] = 
{
	//{0x0E200001,E_METER_MODE,1},            /*"����ģʽ��"*/
	//{0x0E200003,E_RELAY_TYPE,1},			/*"�̵���ģʽ��"*/
	{0x0E200004,E_EVT_LCDALARM_MODE,4},		/*"�����̵������ģʽ��"*/
	{0x0E200005,E_EVT_BGTALARM_MODE,4},		/*"���ⱨ��ģʽ��"*/
	//{0x0E200006,E_RELAY_ACT_TIME,2},		/*"���ü̵������巽ʽ��բά��ʱ��"*/
	//{0x0E200007,E_RELAY_ACT_ENGY,2},		/*"���ü̵������巽ʽ�ٴ���բ����"*/
	{0x0E200008,E_SOFT_RECORD_ID,16},		/*"���������"*/
    {0x0E200010,E_EXT_MODE1,1},             /*"��չ����ģʽ��1"*/
    {0x0E200011,E_EXT_MODE2,1},             /*"��չ����ģʽ��2"*/
    {0x0E200012,E_EXT_MODE3,1},             /*"��չ����ģʽ��3"*/
    {0x0E200013,E_EXT_MODE4,1},             /*"��չ����ģʽ��4"*/
    {0x0E200014,E_EXT_MODE5,1},             /*"��չ����ģʽ��5"*/
    {0x0E200015,E_EXT_MODE6,1},             /*"��չ����ģʽ��6"*/
    {0x0E200016,E_EXT_MODE7,1},             /*"��չ����ģʽ��7"*/
    {0x0E200017,E_EXT_MODE8,1},             /*"��չ����ģʽ��8"*/
    {0x0E200018,E_EXT_MODE9,1},            	/*"��չ����ģʽ��9"*/
    {0x0E200019,E_EXT_MODE10,1},            /*"��չ����ģʽ��10"*/
};
#define PROCESSNUM  ARRAY_SIZE(ProCess_Info)

#define _SUPPORT_ID_NUM (20)/*֧��һ�γ������Ĵ���*/

UN_ID645 SumIdList[_SUPPORT_ID_NUM];
UN_ID645 PeerIdList[_SUPPORT_ID_NUM];
INT8U peer_id_num;
INT8U sum_id_num;

/*"���ͨ�ŵ�ַ�Ϸ���"*/
INT8U Check_Address(INT8U *rpAddress)
{
    INT8U nIndex;
    INT8U nIsZipedAddress;
    INT8U nIsMyAddress;
    INT8U nAddressCode1;
    INT8U nAddressCode2;
    INT8U result;
    INT8S i;
    INT8U Tmp645Addr[6];

    LIB_Reverse(mMtrAddr, Tmp645Addr, 6);

    if(EQUAL==LIB_ChkValue(rpAddress,6,0x99))
    {
        result = ADDR_BROADCAST;
    }
    else if (EQUAL == LIB_MemCmp(Tmp645Addr,rpAddress,6))   //�����ַ
    {
        result = ADDR_SELFADDR;
    }
    else
    {
    
	nIsZipedAddress = TRUE;
	nIsMyAddress = TRUE;
	for (i = 11; i >= 0; i--)
	{
		nIndex = i >> 1;
		if((i & 0x01) == 0)
		{
			nAddressCode1 = rpAddress[nIndex] & 0x0F;
			nAddressCode2 = Tmp645Addr[nIndex] & 0x0F;
		}
		else
		{
			nAddressCode1 = (rpAddress[nIndex] >> 4) & 0x0F;
			nAddressCode2 = (Tmp645Addr[nIndex] >> 4) & 0x0F;
		}
		if(nAddressCode1 != 0x0A)
		{
			nIsZipedAddress = FALSE;
		}
		else
		{
		}
		if(!nIsZipedAddress && nAddressCode1!= nAddressCode2)
		{
			nIsMyAddress = FALSE;
		}
	}

	if (TRUE==nIsMyAddress)
	{
		result = ADDR_ABBREVIATE;
	}
	else
	{
		result = ADDR_UNRESOLVED;
	}
    }
    return result;
}

/*"������յ�һ֡���ݣ��Ը�֡���д���"*/
INT8U DLT645_ProcessRecBytes(INT8U *pDataBuff, INT16U length,INT8U DeviceIndex)
{
    INT8U uni, nCheckSum, FrameLen, nFrameResult;
    INT8U i;

    nFrameResult = CM_UNUSUAL;

    for(i = 0; i <= (length - 12); i++)/*"12: ��data����ֽڳ���"*/
    {
        if((pDataBuff[i] == 0x68) && (pDataBuff[i + 7] == 0x68))
        {
            FrameLen = pDataBuff[i + 9] + 12;
            if((i + FrameLen) > length)
            {/*"�ѽ��յ����ݲ�������֡���ų������룬������֡ͷ"*/
                continue;
            }
            else if((FrameLen <= length) && (pDataBuff[i + FrameLen - 1] == 0x16))
            {
                nCheckSum = 0;
                for(uni = 0; uni < (FrameLen - 2); uni++)
                {
                    nCheckSum += pDataBuff[i + uni];
                }
                if(nCheckSum != pDataBuff[i + FrameLen - 2])/*"У��"*/
                {
                    continue; //return nFrameResult;
                }
            }
            else
            {
                continue;//return nFrameResult;
            }
            /*"�Ƚ�ͨ�ŵ�ַ���˴�ʡ��"*/
            mCommAddrType = Check_Address(&pDataBuff[i + 1]);
            if(ADDR_UNRESOLVED == mCommAddrType)
            {
                continue; //return nFrameResult;   /*"ͨ�ŵ�ַ��ʱ�˳���������֡ͷ"*/ 
            }
            else
            {
            }

            if(0 != i)
            {/*"����ʱ��֡ͷ������0��ʼ"*/
                LIB_MemCpy(&pDataBuff[i], &pDataBuff[0], FrameLen);
            }

            /*"��������"*/
            nFrameResult = CM_AnswerCmd(DeviceIndex, pDataBuff, FrameLen);
            break;
        }
    }

    return nFrameResult;
}

extern INT8U Master_TX_Com_ProcessRecBytes(INT8U *pDataBuff, INT16U length);

INT16U rx_ok_tms;
INT16U rx_err_tms;


/*" ��ѭ�����SlaveӦ������ "*/
void slave_ans_data(void)
{
    INT8U result;
    INT8U buff[256];
    INT8U len;

	//INT_DISABLE_ALL; /*" �������ϴν��յ���֡����û����ѭ����ѯ��������һ��֡��"*/
    len = get_slave_spi_pkt(buff);
	//INT_ENABLE_ALL;
	if((len != 0))
	{
		len = Master_TX_Com_ProcessRecBytes(buff, len);
	    if((len != 0))
	    {
	    	rx_ok_tms++;
	        result = CM_AnswerCmd(E_COM_SLAVE, buff, len); /*" rCommPort ?? "*/
	        if(CM_UNUSUAL != result)
	        {
	            send_slave_ans_data(buff, result);
	        }
	    }
		else
		{
			rx_err_tms++;
		}
	}
}

/*"97��:01 ��,04д,05У��,0Aдͨ�ŵ�ַ,0C�޸�bps,0F�޸�����,10������"*/
/*"07��:03��ȫ����,08Уʱ,11 ��,13��ͨ�ŵ�ַ,14д,15дͨ�ŵ�ַ,16��������,17�޸�bps"*/
/*"07��:18�޸�����,19������,1a����,1B�¼�����,1CԶ�̿���,1d�๦��,1F�ز�"*/
/*"����֡������Ϣ����"*/
void ParseFrameNoMdf(INT8U *rpFrame, ST_FRAME645 *rpResult)
{
    INT16U nLength;
    //INT16U i;
    INT8U lenTmp,dataIdTmp,operatorTmp;
    
    rpResult->beginData= rpFrame;
    rpResult->command = rpFrame[8];
    lenTmp=0;
    dataIdTmp=0;
    operatorTmp=0;
    nLength = rpFrame[9];
	if((0xA6==nLength)&&(0x05==rpResult->command))
		{
		rpResult->dataLen=nLength;
		return;
		}
    /*
    if(rpResult->command != 0x05)
    {
        for(i = 0; i < nLength; i++)
        {
            rpFrame[10 + i] -= 0x33;
        }
    }
    */
    
    /*"���ݿ���������ṹ"*/
    switch(rpResult->command)
    {
    /*"97��"*/    
    case 0x01:  /*"��ȡ����97"*/
    case 0x04:  /*"д����97"*/
    case 0x05:
        lenTmp=2;
        rpResult->dataId.asBytes[0] = rpFrame[10] - 0x33;
        rpResult->dataId.asBytes[1] = rpFrame[11] - 0x33;
        break;        
    /*"07��"*/    
    case 0x08:
    case 0x13:    
    case 0x15:
    case 0x16:
    case 0x17:    
        break;
    case 0x06:
    case 0x86:
    case 0x11:
    case 0x12:
    case 0x91:
        lenTmp=4;
        dataIdTmp=10;
        break;
    case 0x14:
        lenTmp=12;
        dataIdTmp=10;
        rpResult->authority = rpFrame[14] - 0x33;
        operatorTmp=18;
        break;
    case 0x18:
        rpResult->errorCode = COMM_ERR_PASSWORD;
        break;
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:       
    case 0x1F:       
        lenTmp=8;
        rpResult->authority = rpFrame[10] - 0x33;
        operatorTmp=14;
        break;
    case 0x03:
        operatorTmp=14;
        rpResult->authority = rpFrame[10] - 0x33;
        break;
    case 0x1D:
        rpResult->authority = rpFrame[10] - 0x33;
        break;

    default:
        rpResult->errorCode = COMM_ERR_OTHERS;
        break;
    }
    
    if(COMM_ERR_NOERR==rpResult->errorCode )
    {
        rpResult->data = &rpFrame[10+lenTmp];
        rpResult->dataLen = nLength - lenTmp;
        if(dataIdTmp>0)
        {
            LIB_MemCpy(&rpFrame[dataIdTmp], &rpResult->dataId.asBytes[0],4);
            rpResult->dataId.asBytes[0] -= 0x33;
            rpResult->dataId.asBytes[1] -= 0x33;
            rpResult->dataId.asBytes[2] -= 0x33;
            rpResult->dataId.asBytes[3] -= 0x33;
        }
        else
        {
        }
        
        if(operatorTmp>0)
        {
            LIB_MemCpy(&rpFrame[operatorTmp], &rpResult->operator.asBytes[0],4);
            rpResult->operator.asBytes[0] -= 0x33;
            rpResult->operator.asBytes[1] -= 0x33;
            rpResult->operator.asBytes[2] -= 0x33;
            rpResult->operator.asBytes[3] -= 0x33;
        }
        else
        {
        }      
    }
}

INT8U hostAddr[6];
void FrameDataDec0x33(ST_FRAME645 *rpResult)
{
    INT16U nLength;
    INT16U i;
    INT8U *rpFrame;
    INT8U command;

    rpFrame = rpResult->beginData;
    command = rpFrame[8];
    nLength = rpFrame[9];
    if(command != 0x05)
    {
        for(i = 0; i < nLength; i++)
        {
            rpFrame[10 + i] -= 0x33;
        }
    }
}

/*"97��:01 ��,04д,05У��,0Aдͨ�ŵ�ַ,0C�޸�bps,0F�޸�����,10������"*/
/*"07��:03��ȫ����,08Уʱ,11 ��,13��ͨ�ŵ�ַ,14д,15дͨ�ŵ�ַ,16��������,17�޸�bps"*/
/*"07��:18�޸�����,19������,1a����,1B�¼�����,1CԶ�̿���,1d�๦��,1F�ز�"*/
/*"����֡������Ϣ����"*/
void ParseFrame(INT8U *rpFrame, ST_FRAME645 *rpResult)
{
    INT16U nLength;
    INT16U i;
    INT8U lenTmp,dataIdTmp,operatorTmp;
    
    rpResult->beginData= rpFrame;
    rpResult->command = rpFrame[8];
    lenTmp=0;
    dataIdTmp=0;
    operatorTmp=0;
    nLength = rpFrame[9];
	LIB_MemCpy((INT8U *)&rpFrame[1], hostAddr, 6);
    if(rpResult->command != 0x05)
    {
        for(i = 0; i < nLength; i++)
        {
            rpFrame[10 + i] -= 0x33;
        }
    }
    
    /*"���ݿ���������ṹ"*/
    switch(rpResult->command)
    {
    /*"97��"*/    
    case 0x01:  /*"��ȡ����97"*/
    case 0x04:  /*"д����97"*/
    case 0x05:
        lenTmp=2;
        rpResult->dataId.asBytes[0] = rpFrame[10];
        rpResult->dataId.asBytes[1] = rpFrame[11];
        break;        
    /*"07��"*/    
    case 0x08:
    case 0x13:    
    case 0x15:
    case 0x16:
    case 0x17:    
        break;
    case 0x06:
    case 0x86:
    case 0x11:
    case 0x12:
    case 0x91:
        lenTmp=4;
        dataIdTmp=10;
        break;
    case 0x14:
        lenTmp=12;
        dataIdTmp=10;
        rpResult->authority = rpFrame[14];
        operatorTmp=18;
        break;
    case 0x18:
        rpResult->errorCode = COMM_ERR_PASSWORD;
        break;
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:       
    case 0x1F:       
        lenTmp=8;
        rpResult->authority = rpFrame[10];
        operatorTmp=14;
        break;
    case 0x03:
        operatorTmp=14;
        rpResult->authority = rpFrame[10];
        break;
    case 0x1D:
        rpResult->authority = rpFrame[10];
        break;

    default:
        rpResult->errorCode = COMM_ERR_OTHERS;
        break;
    }
    
    if(COMM_ERR_NOERR==rpResult->errorCode )
    {
        rpResult->data = &rpFrame[10+lenTmp];
        rpResult->dataLen = nLength - lenTmp;
        if(dataIdTmp>0)
        {
            LIB_MemCpy(&rpFrame[dataIdTmp], &rpResult->dataId.asBytes[0],4);
        }
        else
        {
        }
        
        if(operatorTmp>0)
        {
            LIB_MemCpy(&rpFrame[operatorTmp], &rpResult->operator.asBytes[0],4);
        }
        else
        {
        }      
    }
}

//static INT8U Now645ComId;

void Ans_MtrClr (void)
{
    INT8U Buf[12];
    INT8U i;

    Buf[0]= 0x68;
    LIB_Reverse(mMtrAddr,&Buf[1],6);    
    Buf[7]=0x68;
    Buf[8]=0x85;
    Buf[9]=0x00;
    Buf[10] = 0;
    for(i=0; i<10; i++)
	{		
        Buf[10] += Buf[i];
	}
    Buf[11]=0x16;
    //COM_StartSend(Now645ComId, Buf, 12);
    Master_TX_Com_StartSend(0, Buf, 12);
    MCU_DelayMs(150);
}


/*"*************************************************************"*/			
/*"Function: 	CM_ReadData		"*/
/*"Description: ͨ�Ŷ�����֡���ݴ���"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CM_ReadNextData(ST_FRAME645 *rpResult,INT16S *rpDataLength)
{
    INT8U nErrorCode = 0;
    INT8U nSeq;
    INT16U dataLenTmp;
    ST_CMDATA nAnswer; // return data from protocol tree visit
    ST_FRM_RESULT nResult;
    
    if (mstNextFrame[rpResult->commPort].dataId.asLong != rpResult->dataId.asLong)
    {
        nErrorCode = COMM_ERR_OTHERS;
    }
    else if (0 == *(rpResult->data))
    {
        nErrorCode = COMM_ERR_OTHERS;
    }
    else
    {
    	nSeq = *(rpResult->data);
    	nAnswer.data= rpResult->data;
    	nResult.authority = rpResult->authority;
    	nResult.commPort = rpResult->commPort;
    	nResult.dataId.asLong = rpResult->dataId.asLong;
    	nResult.data = rpResult->data;
    	nResult.dataLen = rpResult->dataLen;

    	if (mstNextFrame[rpResult->commPort].seq == nSeq)
    	{
    	    dataLenTmp=mstNextFrame[rpResult->commPort].prevBytes;
    	}
    	else if (mstNextFrame[rpResult->commPort].seq + 1 == nSeq)
    	{
    	    dataLenTmp=mstNextFrame[rpResult->commPort].bytes;
    	}
    	else
    	{
    	    nErrorCode = COMM_ERR_OTHERS;
    	}		
    }
    
    if(COMM_ERR_NOERR==nErrorCode)
    {
        nErrorCode = PP_CommGetData((ST_FRM_RESULT *)&nResult, dataLenTmp, COMM_DATA_MAX, (ST_ANSWER_DATA *)&nAnswer);
        *rpDataLength = nAnswer.dataLen;    
    }
    else
    {
    }
    return nErrorCode;	
}


    /*"������״̬"*/    
static INT8U Check_IrState(void)
{
    #if 0
    INT8U LoginFlg;
    INT32U IRAuthRmnTime;
    LoginFlg = TRUE;

    GetSingle(E_IRAUTH_RMN_TIME, (INT8U *)&IRAuthRmnTime);
    /*"����ͨ����û��ͨ��������֤"*/
    if((E_COM_IR == Now645ComId)&&(0 ==IRAuthRmnTime)&&(ChkStatus(E_FAC) == FALSE))  
    {
        LoginFlg = FALSE;
    }
    return LoginFlg;
    #else
    return TRUE;
    #endif
}


INT8U set_bm_info_pkt(ST_FRAME645 *nResult)
{
    INT32U id;
    INT8U tmp[16];
    INT8U len;
    UN_ID645 s_id;

    id = nResult->dataId.asLong;
    switch(nResult->command)
    {
        case 0x1A: /*" ������� "*/
            s_id.asLong = ID_CLR_MTR;
            tx_pkt_to_peer(0, 0x11, s_id, NULL, 0);
            break;
        case 0x1B: /*" �¼����� "*/
            s_id.asLong = ID_CLR_EVT;
            tx_pkt_to_peer(0, 0x11, s_id, NULL, 0);
            break;
        case 0x15: /*" д���ַ "*/
            GetSingle(E_COMM_ADDR, tmp);
            len = 6;
            s_id.asLong = ID_SET_BM_ADDR;
            tx_pkt_to_peer(0, 0x11, s_id, tmp, len);
            break;
        case 0x14:
            if(id == 0x04000101 || id == 0x04000102)
            {/*" дʱ�� "*/
                GetSingle(E_SYS_TIME, tmp);
                len = 7;
                s_id.asLong = ID_SET_BM_TM;
                tx_pkt_to_peer(0, 0x11, s_id, tmp, len);
            }
            if(id == 0x04000401)
            {/*" д���ַ "*/
                GetSingle(E_COMM_ADDR, tmp);
                len = 6;
                s_id.asLong = ID_SET_BM_ADDR;
                tx_pkt_to_peer(0, 0x11, s_id, tmp, len);
            }
            break;
        default:
            return 0;
    }
    return 1;
}

INT8U get_debug_data(INT8U *ptr)
{
    INT32U addr;
    INT8U len;

    len = ptr[0];
    LIB_MemCpy(ptr + 1, (INT8U *)&addr, 4);
    LIB_MemCpy((INT8U *)addr, ptr + 1, len);
    return len + 2;
}

const INT16U BillDayId[]={E_MOUTH_BILLDAY1,E_MOUTH_BILLDAY2,E_MOUTH_BILLDAY3};

/*****************************************************************************
 �� �� ��  : Broadcast_Just_Time
 ��������  : �㲥Уʱ��������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
*****************************************************************************/
INT8U Broadcast_Just_Time(INT8U *pdata)
{
    INT8U TempData[10];    /*ʱ��7�ֽڣ�Ԥ�����ֽ�*/
    ST_6TIME_FORM  AdjTime;/*��Уʱʱ��*/
    ST_6TIME_FORM  TempSysTime;
    //ST_6TIME_FORM  TempTime;
    INT8S SRet;
    INT8U Ret;
    INT32S DayStdNow;
    INT32S DayStdLastAdj;
    INT32S SecTemp;
    INT8U AdjBuf[12];
    //INT8U Cnt;
    //ST_DAY_HOUR DayHour;
        
    Ret = COMM_ERR_NOERR;
    
    /*1.������������ת������ΪУʱʱ��*/
    LIB_MemCpy(pdata, &AdjTime.year, sizeof(ST_6TIME_FORM));
    
    if(TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&AdjTime))
    {
        /*��ȡ��1��Уʱʱ��*/
        FS_ReadFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,LastBroadcastAdjTime), (INT8U *)&DayStdLastAdj, 4);
        
        /*2.��ȡϵͳʱ��*/
        GetSingle(E_SYS_TIME, (INT8U *)&TempData);
        LIB_MemCpy(&TempData[0], (INT8U*)&TempSysTime, sizeof(ST_6TIME_FORM));

        DayStdNow = LIB_DToStdDate(&TempSysTime); 

        if (DayStdNow != DayStdLastAdj)/*"����������ٴ�Уʱ"*/
        {
            if (((TempSysTime.min < 5) && (TempSysTime.hour == 0))
                || ((TempSysTime.min >= 55)&& (TempSysTime.hour == 23))) /*"�������5min�ڲ�׼Уʱ"*/
            {
                Ret = COMM_ERR_OTHERS;
            }
            else
            { 
                SecTemp = LIB_CmpTime(&TempSysTime, &AdjTime);
                if(LIB_ABS(SecTemp) > 630) /*"�뵱ǰʱ���ֵ����10min������Уʱ"*/
                {
                    Ret = COMM_ERR_OTHERS;
                }
                else
                {
#if 0
                    /*�ж��Ƿ��ڽ���ת������10��������*/
                    for(Cnt=0;Cnt<ARRAY_SIZE(BillDayId);Cnt++)
                    {
                        GetSingle(BillDayId[Cnt], (INT8U *)&DayHour);
                        /*�ж��������Ƿ���Ч*/
                        if((DayHour.day != 99) && (DayHour.hour != 99))
                        {
                            TempTime.year = TempSysTime.year;
                            TempTime.month= TempSysTime.month;
                            TempTime.day = DayHour.day;
                            TempTime.hour = DayHour.hour;
                            TempTime.min = 0;
                            TempTime.sec = 0;
                            SecTemp = LIB_CmpTime(&TempSysTime, &TempTime);
                            if(LIB_ABS(SecTemp) < 630) /*"����������10�����ڲ�����Уʱ"*/
                            {
                                Ret = COMM_ERR_OTHERS;
                                break;
                            }

                        }
                        else
                        {
                        }
                    }
#endif
                }
            }           
            
           
        }
        else
        {
            Ret = COMM_ERR_OTHERS;
        }

    }
    else
    {
        Ret = COMM_ERR_OTHERS;
                      
    }

    if(Ret == DAR_0_OK)
    {
        /*ʱ��ͬ��*/

        /*�洢�ϵ�ǰ�Ĳ�ֵ��Ϊ��һ�ε�Уʱʱ��*/
        FS_WriteFile(E_FILE_UI_DATA , FIND(ST_UI_FILE_DATA,LastBroadcastAdjTime), (INT8U *)&DayStdNow, 4);

        SaveRcrd(E_BROADCAST_JUST_TIME_RCRD, E_EVTLST_JUSTTIME_S);
        SRet = Action(E_LRM_JUSTIME,&AdjTime,NULL);/*Уʱ*/
        if(0!=SRet)
        {
            return (0-SRet);
        }
        SaveRcrd(E_BROADCAST_JUST_TIME_RCRD, E_EVTLST_JUSTTIME_E);
        
#ifdef PM_MODLE                       
        SRet = Action(E_TRSTEP_SWCHECK, NULL, NULL);
        if(0!=SRet)
        {
            return (0-SRet);
        }  
#endif        
        /*Уʱǰʱ�䣬��Уʱ��ʱ��*/
        LIB_MemCpy((INT8U * )&TempSysTime, (INT8U *)&AdjBuf[0], 6);
        LIB_MemCpy((INT8U * )&AdjTime, (INT8U *)&AdjBuf[6], 6);
        SRet = Action(E_RCRD_TIME_ADJUST, (INT8U *)&AdjBuf[0], NULL);        
        if(0!=SRet)
        {
            return (0-SRet);
        }  

        #ifdef THREE_PHASE
        Action(E_CURRENT_DEMAND_CLR, NULL, NULL);      /*�嵱ǰ����*/
        //Action(E_VOLTHEALTH_ADJUST_TIME, (INT8U *)&AdjBuf[0], NULL);
        #endif
    }
    
    return (Ret);  
}    


/*"*************************************************************"*/			
/*"Function: 	CM_08Cmd		"*/
/*"Description: �㲥Уʱ"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CM_08Cmd(ST_FRAME645 *rpResult)
{
    INT8U ret;
    ST_7TIME_FORM nDateTime1; // time in the request frame

    LIB_Reverse(rpResult->data,(INT8U *)&nDateTime1,6);
    nDateTime1.week= 0;

    if(!LIB_ChkDateTimeBCD((ST_6TIME_FORM *)&nDateTime1))
    {
        return COMM_ERR_OTHERS;
    }
    LIB_BcdToCharNByte((INT8U *)&nDateTime1, 7);

    ret = Broadcast_Just_Time((INT8U *)&nDateTime1);
    if(ret == COMM_ERR_NOERR)
    {
        Host_Commu_Bm_Data_Sync(3);
    }
    return ret;
    /*" ��Ӧ��"*/
}


INT8U CM_AnswerCmd(INT8U rCommPort, INT8U *pdataBuff, INT8U len)
{
    ST_FRAME645 nResult;
    INT8U nErrorCode;
    INT16U n03CmdErrCode;
    INT8U nCheckSum;
    INT8U ndataContinued;
    INT16S nAnswerDataLength;
    INT8U AnswerLen;
    INT8U i;
    INT8U cmdResult;
    
    INT8U seqTmp;
    INT8U reportFlgTmp;
    UN_BYTE_DEF  mtrRunModeTmp; 

    /*�����ϱ�����֡����ʱ��Ϊ0*/
    mtrRunModeTmp.byte = 0;
    reportFlgTmp       = 0;
    
    /*"����֡������Ϣ����"*/
    LIB_MemSet(0, (INT8U *)&nResult, sizeof(ST_FRAME645));
    nResult.commPort = rCommPort;
    nResult.frmLen = len;
    ParseFrame(pdataBuff, &nResult);
    //nResult.dataId.asLong = 0x03300D01;
    //nResult.dataId.asLong = 0x03300101;
    //nResult.dataId.asLong = 0x03810001;
    //nResult.dataId.asLong = 0x03820001;
    //nResult.dataId.asLong = 0x03830001;
    //nResult.command = 0x11;

    //Now645ComId = rCommPort;
    nAnswerDataLength = 0;
    nErrorCode = nResult.errorCode;
    n03CmdErrCode = 0;
    ndataContinued = FALSE;
    cmdResult = CM_UNUSUAL;
    IrStateflg=Check_IrState(); 
    
    /*"���������"*/
    if(nErrorCode == 0)
    {
#if 0
        if(rCommPort < E_COM_SLAVE && ((nResult.dataId.asLong & 0xFFFFFF00UL) == 0xEEEEEE00UL))
        {
            nErrorCode  = check_ReadList(&nResult);
            if(nErrorCode != COMM_ERR_NOERR)
            {
                return nErrorCode; /*" ���쳣���쳣Ӧ�� "*/
            }
            if(ListDataHasPeerData() == TRUE)
            {
                /*" ���Զ�ȥȡ�� "*/
                cfg_read_peer_list_data_to_peer(&nResult);
                return CM_UNUSUAL;
            }
            /*" �޶Զ����ݣ��߳�������  "*/
        }
#endif
        if((nResult.command & 0x1F) == 0x06)
        {
            pkt_queue_rx_proc(&nResult, len, 0);
            return CM_UNUSUAL;
        }

        LIB_MemCpy((INT8U *)&nResult.operator, oprateCode, 4);
        /*"ȡ����״̬������������ж�"*/
        switch(nResult.command)
        {
            case 0x01:  /*"��ȡ����97"*/
#if 0
                nErrorCode = DLT645_UIUReadData(&nResult, &nAnswerDataLength);
#endif
                if(nErrorCode == COMM_ERR_NOERR)
                {
                    nAnswerDataLength += 2;/*"���ݱ�ʶ����"*/
                }
                break;

            case 0x03:  /*"��ȫ����"*/
                n03CmdErrCode = CM_Far03Cmd(pdataBuff+7); 
		  nAnswerDataLength = pdataBuff[9];    		  
                break;
                
            case 0x04:  /*"д����97"*/
#if 0
                nErrorCode = DLT645_UIUWriteData(&nResult, &nAnswerDataLength);
#endif
                nErrorCode = COMM_ERR_OTHERS;
                cmdResult =  CM_UNUSUAL;
                break;
            case 0x05:  /*"У��Уʱ,�ڿ���ʼ��"*/
                if(pdataBuff[10] == 0xce)
                {/*" 68 11 11 11 11 11 11 68 05 06 CD len 18 12 00 20 57 16  "*/
                    /* 68 11 11 11 11 11 11 68 05 06 CD 07 18 12 00 20 5F 16  */
                    nAnswerDataLength = get_debug_data(pdataBuff + 11);
                    break;
                }
                //if(ChkStatus(E_FAC)== TRUE)
                {   /*" 68 99 99 99 99 99 99 68 05 07 07 B8 72 05 49 91 CD 4F 16 "*/
                    if (EQUAL == LIB_MemCmp(pdataBuff + 10, (INT8U *)&mResetPwd[0], 7))/*"�ڿ���ʼ��"*/
                    {
                        /*
                                        if(rCommPort < E_COM_SLAVE)
                                        {
                                        	UN_ID645 id;

                                        	id.asLong = ID_INIT_MTR;
                                        	tx_pkt_to_peer_now(0, 0x11, id, NULL, 0);
                                        }
                                        */
                    	MTR_INIT();
                    }
                    else
                    {

                    }
                }
				/*"У���ʼ��:  68 11 11 11 11 11 11 68 05 06 21 FA 00 00 00 01 5D 16 "*/
                nErrorCode = Action(E_LRM_JUSTMETER, pdataBuff+7, NULL);
                if(COMM_ERR_NOERR==nErrorCode)
                {
                    nAnswerDataLength = pdataBuff[9];
                }
                else
                {
                }  
                break;
            case 0x08:
                nErrorCode = CM_08Cmd(&nResult);
                break;
            case 0x11:  /*"07 ��"*/
                nErrorCode = CM_ReadData(&nResult,&nAnswerDataLength);
                if(nErrorCode == COMM_ERR_NOERR)
                {
                    if((nAnswerDataLength < 0) && (nResult.protocol == DLT645_2007))
                    {
                        nAnswerDataLength &= 0x7FFF;
                        mstNextFrame[rCommPort].bytes = nAnswerDataLength;
                        mstNextFrame[rCommPort].dataId.asLong = nResult.dataId.asLong;
                        ndataContinued = TRUE;
                    }
                    else
                    {
                        mstNextFrame[rCommPort].dataId.asLong = INVALID_DATA_ID;
                        mstNextFrame[rCommPort].bytes = 0;
                    }
                    mstNextFrame[rCommPort].prevBytes = 0;
                    mstNextFrame[rCommPort].seq = 0;
                    nAnswerDataLength += 2;/*"���ݱ�ʶ����"*/
                    if(nResult.protocol == DLT645_2007)
                    {
                        nAnswerDataLength += 2;/*"���ݱ�ʶ����"*/
                    }
                }
                break;	
            case 0x12:  /*"������֡"*/
		       seqTmp=*nResult.data;
		       if((seqTmp==mstNextFrame[nResult.commPort].seq)||(seqTmp==(mstNextFrame[nResult.commPort].seq+1)))
		       {
			        nErrorCode = CM_ReadNextData(&nResult,&nAnswerDataLength);
    			    if(nErrorCode == COMM_ERR_NOERR)
                    {
                  
                        if(nAnswerDataLength < 0)
                        {
                            nAnswerDataLength &= 0x7FFF;
                            ndataContinued = TRUE;
                        }
                        /*"�޺���֡ʱ���ٽ��ձȱ��κ���֡�Ŵ������"*/
                        if((mstNextFrame[nResult.commPort].seq != seqTmp)&&(TRUE==ndataContinued))
                        {
                            mstNextFrame[nResult.commPort].seq++;
                            mstNextFrame[nResult.commPort].prevBytes = mstNextFrame[nResult.commPort].bytes;
                            mstNextFrame[nResult.commPort].bytes += nAnswerDataLength;
                        }    
                        nAnswerDataLength += 4; /*"���ݱ�ʶ����"*/
                        pdataBuff[10 + nAnswerDataLength] = seqTmp;
                        nAnswerDataLength++;
                    }
                    else
                    {
                    }			    
			    }
    			else
    			{
    			    nErrorCode = COMM_ERR_NODATA;
    			}
                
                if(nErrorCode!=COMM_ERR_NOERR)
                {
                    if((1==reportFlgTmp)&&(1==mtrRunModeTmp.bit.b2)&&((1==reportComBegin[rCommPort])||(seqTmp>0)))
                    {
                        nErrorCode=COMM_ERR_NOERR;
                        nResult.dataId.asLong=0x04001501;
                        LIB_MemCpy(nResult.dataId.asBytes,pdataBuff+10,4);
                        nErrorCode = CM_ReadData(&nResult,&nAnswerDataLength);
                        reportComBegin[rCommPort]=0;
                        nAnswerDataLength += 4; /*"���ݱ�ʶ����"*/
                        pdataBuff[10 + nAnswerDataLength] = seqTmp;
                        nAnswerDataLength++;
                    }
                    else
                    {
                    }
                 }
			    break;
            //case 0x1F:   /*"��ȡ���ַ"*/
            case 0x13:   /*"��ͨ�ŵ�ַ"*/
                LIB_Reverse(mMtrAddr,pdataBuff+10,6);
                nAnswerDataLength = 6;
                break;
            case 0x14:  /*"д����"*/
                nErrorCode = CM_WriteData(&nResult);
                break;
            case 0x15:/*"����ͨ�ŵ�ַ"*/
                nErrorCode =CM_15Cmd(&nResult);
                break;
            case 0x1A: /*" ������� "*/
            case 0x1B: /*" �¼����� "*/      
                    nErrorCode =CM_ClrCmd(&nResult);
                break;

            case 0x1C:
                    if((*(pdataBuff+13)==0x01)&&(*(pdataBuff+10)==T2_CLASS))
                    {
                         nAnswerDataLength=2;
                    }
                    else
                    {
                    }       
                    //nErrorCode = CM_1CCmd(&nResult);
                    if((0==nAnswerDataLength)&&(nErrorCode != COMM_ERR_NOERR))
                    {
                        nAnswerDataLength=1;
                        pdataBuff[10] = nErrorCode;
                    }
                    else
                    {
                    }
                break;
            case 0x1D:
                OC_MultiTermnSw((EN_OC_MultiTermnMode)*nResult.data);
                break;
            default:
                nErrorCode = COMM_ERR_OTHERS;
                cmdResult =  CM_UNUSUAL;
                break;
        } //end of switch(rpResult->command)

    }

    /*" �Ǽ��ܵ���������ߵ����� "*/
    //if((rCommPort < E_COM_SLAVE) && (nErrorCode == COMM_ERR_NOERR))
    //{
    //    set_bm_info_pkt(&nResult);
    //}
    
    /*"  --------------------------------------------------------------------"*/
    if(nAnswerDataLength>(COMM_DATA_MAX+12))
    {
        nErrorCode=COMM_ERR_OTHERS;
        AnswerLen = 1;		 
    }
    else
    {
        AnswerLen=(INT8U) nAnswerDataLength;
    }
    if((nErrorCode > 0)&&(nResult.command!=0x1c))
    {
        pdataBuff[10] = nErrorCode;
        AnswerLen = 1;		     
    }
    else if(n03CmdErrCode>0)
    {
        pdataBuff[10+nAnswerDataLength] = (INT8U)n03CmdErrCode;
        pdataBuff[11+nAnswerDataLength] = (INT8U)(n03CmdErrCode>>8);
        AnswerLen = nAnswerDataLength + 2;		/*�������ݱ�ʶ�Ϳ�����*/
    }
    else
    {
    }
    /*"�㲥Уʱ���޸�ͨ�ŵ�ַ�쳣ʱ����ҪӦ��"*/
    if(((0x15==nResult.command)&&(nErrorCode > 0)))
    {
        cmdResult =  CM_UNUSUAL;
    }
    else
    {
        /*"��ͨ��Ӧ��֡"*/
        pdataBuff[0] = 0x68;
        //-LIB_Reverse(mMtrAddr,pdataBuff+1,6);//del
       LIB_MemCpy(hostAddr,pdataBuff+1, 6);
    	pdataBuff[7] = 0x68;
    	if((nErrorCode > 0)||(n03CmdErrCode>0))
    	{
                pdataBuff[8] = nResult.command + 0xC0;
    	}
    	else
    	{
                pdataBuff[8] = nResult.command + 0x80;
                if(ndataContinued)
                {
                    pdataBuff[8] += 0x20;
                }
    	}
    	pdataBuff[9] = AnswerLen;
    	if(nResult.command != 0x05) /*"05�����33"*/
        {
            for(i = 0; i < AnswerLen; i++)
            {
                pdataBuff[10 + i] += 0x33;
            }
        }
    	nCheckSum = 0;
    	for(i=0; i<AnswerLen+10; i++)
    	{		
                nCheckSum += pdataBuff[i];
    	}
    	pdataBuff[10+AnswerLen] = nCheckSum;
    	pdataBuff[11+AnswerLen] = 0x16;
    	cmdResult = AnswerLen + 12; 
    }
#if 0
    if((nErrorCode == 0) && (nResult.command == 0x04))
    { /*"RF��Ƶ���»�ȡ���ַ."*/
      DLT645_UIURFReNewAddr(&nResult, 1);
    }
#endif
    return cmdResult;
}

#if 0
/*"*************************************************************"*/			
/*"Function: 	CommReadData		"*/
/*"Description:645Э�����ݴ���"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CommReadData(ST_FRM_RESULT *framedata,INT16U *pLen)
{
	INT8U i,ValTmp;
	
	ValTmp = COMM_ERR_NODATA;
	for(i=0;i<PROCESSNUM;i++)
	{
		if(framedata->dataId.asLong == ProCess_Info[i].asLong)
		{
			GetSingle(ProCess_Info[i].Obis,framedata->data);
			if(framedata->dataId.asLong == 0x0E200008)/*"�跴��"*/
			{
				 LIB_RvsSelf(framedata->data,ProCess_Info[i].SetLen);
			}
			*pLen = ProCess_Info[i].SetLen;
			ValTmp = COMM_ERR_NOERR;
			break;
		}
	}

	return ValTmp ;
	
}
#endif

/*"*************************************************************"*/			
/*"Function: 	CM_ReadData		"*/
/*"Description: ͨ�Ŷ����ݴ���"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CM_ReadData(ST_FRAME645 *rpResult, INT16S *rpDataLength)
{
    INT8U nErrorCode;
    INT8U i;
    INT8U beginTmp;
    INT16U nDtaLenNum;
    ST_FRM_RESULT nResult;
    ST_CMDATA nAnswer; // return data from protocol tree visit

    nErrorCode = COMM_ERR_NOERR;
    nDtaLenNum=0;
    nResult.dataId.asLong = rpResult->dataId.asLong;  
    
    if(COMM_ERR_NOERR==nErrorCode)
    {
        nResult.authority = rpResult->authority;
        nResult.commPort = rpResult->commPort;
        nResult.data = rpResult->data;
        nResult.dataLen = rpResult->dataLen;
        nAnswer.data = rpResult->data;
        /*"   ������ʷ���ݿ鳭 "*/
        //if((rpResult->commPort == E_COM_SLAVE) && (nResult.dataId.asLong > 0xFF000000))
        //{
        //    nErrorCode = bm_slave_rx_proc(rpResult, &nAnswer.dataLen);
        //}
        if((nResult.dataId.asBytes[3]==0x00)&&(nResult.dataId.asBytes[0]==0xff)&&(nResult.dataId.asBytes[1]!=0xff)&&(nResult.dataId.asBytes[2]!=0xff))   
        {
            if(nResult.dataId.asBytes[2]==0xFE)
            {
                beginTmp=1;
            }
            else
            {
                beginTmp=0;
            }        
            for(i=beginTmp;i<=0x0c;i++)
            {
                nResult.dataId.asBytes[0]=i;
                nErrorCode = PP_CommGetData((ST_FRM_RESULT *)&nResult, 0,COMM_DATA_MAX, (ST_ANSWER_DATA *)&nAnswer);
                if(nErrorCode == COMM_ERR_NOERR)
                {
                    nAnswer.data+=nAnswer.dataLen;
                    nDtaLenNum+=nAnswer.dataLen;
                }
                else
                {
                    break;
                }
            }
        }
        /*"   ����բ���ݿ鳭, �������ݿ鳭 "*/
        else if(((nResult.dataId.asBytes[3]==0x1D)||(nResult.dataId.asBytes[3]==0x1E)||(nResult.dataId.asBytes[3]==0x19))&&(nResult.dataId.asBytes[0]==0xff))   
        {      
            for(i=1;i<=0x0A;i++)
            {
                nResult.dataId.asBytes[0]=i;
                nErrorCode = PP_CommGetData((ST_FRM_RESULT *)&nResult, 0,COMM_DATA_MAX, (ST_ANSWER_DATA *)&nAnswer);
                if(nErrorCode ==COMM_ERR_NOERR)
                {
                    nAnswer.data+=nAnswer.dataLen;
                    nDtaLenNum+=nAnswer.dataLen;
                }
                else
                {
                    break;
                }
            }
        }
        /*"   Һ���鿴 "*/
        else if((0x05==nResult.dataLen)&&(0x04040300==nResult.dataId.asLong))
        {
                    /*"Һ���鿴ִ�к���"*/
                    #if 0 //(Jul 21, 2018,10:39:19)
                    if(_FALSE==DM_ShowSreen((ST_SHOWDATA_INPUT *)nResult.data,10,LEVEL_1))
                    {
                        nErrorCode=COMM_ERR_NODATA;
                    }
                    else
                    {
                        nErrorCode=COMM_ERR_NOERR;
                        nAnswer.dataLen=5;
                    }
                    #endif
        }
        /*"��չ������"*/
        else if((0x0E20==nResult.dataId.asWord[1])&&(nResult.dataId.asBytes[0]>=0x23)&&(nResult.dataId.asBytes[0]<0x70)) 
        {
            //(Jul 21, 2018,10:38:16)nAnswer.dataLen = STM_ReadExtData((ST_FRM_RESULT *)&nResult);
        }
		//else if(nResult.dataId.asLong == 0x04001501)
		//{
		//	nAnswer.dataLen = SI_645Report(nResult.data);
		//}
		else if(nResult.dataId.asLong == 0x04000301)
		{
			nAnswer.dataLen = 1;
			//(Jul 21, 2018,10:37:26)DisNumCheck(0,nResult.data);
			nErrorCode=COMM_ERR_NOERR;
		}
		else if(nResult.dataId.asLong == 0x04000305)
		{
			nAnswer.dataLen = 1;
			//(Jul 21, 2018,10:37:8)DisNumCheck(1, nResult.data);
			nErrorCode=COMM_ERR_NOERR;
		}
        else if((nResult.dataId.asBytes[3]==0xEE)&&(nResult.dataId.asBytes[2]==0xEE)
            &&(nResult.dataId.asBytes[1]==0xEE))//���������ݳ���
        {
            nErrorCode = CM_ReadListData((ST_FRM_RESULT *)&nResult,(ST_ANSWER_DATA *)&nAnswer);
        }
        else
        {        
            nErrorCode = PP_CommGetData((ST_FRM_RESULT *)&nResult, 0,COMM_DATA_MAX, (ST_ANSWER_DATA *)&nAnswer);
        }

        if(COMM_ERR_NOERR==nErrorCode)
        {
            if(nDtaLenNum>0)
            {
                *rpDataLength = nDtaLenNum;
            }
            else
            {
                *rpDataLength = nAnswer.dataLen;
            }
            if(0x04001502==nResult.dataId.asLong) /*"�忨״̬�ֶ�����"*/
            {
                //(Jul 21, 2018,10:36:36)stMtrRunStatus.cardSta.word=0;
            }
            else
            {
            }
        }
        else
        {
        }
    }
    else
    {
    }

    return nErrorCode;
}

/*"*************************************************************"*/			
/*"Function: 	CommWriteData		"*/
/*"Description:645Э�����ݴ���"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CommWriteData(ST_FRM_RESULT *framedata)
{
	INT8U i,pLen,ValTmp;
	INT16U pObis;
	
	ValTmp = COMM_ERR_NODATA;
    
	for(i=0;i<PROCESSNUM;i++)
	{
		if(framedata->dataId.asLong == ProCess_Info[i].asLong)
		{
			pObis = ProCess_Info[i].Obis;
			pLen = ProCess_Info[i].SetLen;

			if(framedata->dataId.asLong == 0x0E200008)/*"�跴��"*/
			{
				 LIB_RvsSelf(framedata->data,ProCess_Info[i].SetLen);
			}
			Set(&pObis,1,framedata->data,&pLen);
			if(framedata->dataId.asLong == 0x0E200001)
			{
				EC_SupCpuCfg(framedata->data[0]&METERTPYE);/*"���ò忨���˿�"*/
			}
			ValTmp = COMM_ERR_NOERR;
			break;
		}
	}

	return ValTmp ;
	
}


/*****************************************************************************
 �� �� ��  : Check_645_Right
 ��������  : �ж�645��дȨ��
 �������  : ST_FRAME645 *rpResult  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��8��6��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Check_645_Right(ST_FRM_RESULT *rpResult)
{
    INT8U ValTmp; 
    INT32U TmpdataId;

    TmpdataId = rpResult->dataId.asLong;

    ValTmp = COMM_ERR_NOERR;
    /*"����ͨ����û��ͨ��������֤"*/
    if(IrStateflg == FALSE)  
    {
        return COMM_ERR_PASSWORD;
    }

    if(rpResult->commPort == E_COM_SLAVE)
    {/*" ����о��֤ͨ�������������������ж�Ȩ���Ƿ񹻣�ֱ��ͨ�� "*/
    }
    else if((T2_CLASS==mAuthority)||(T1_CLASS==mAuthority))
    {
        if(0x04001503==TmpdataId) /*"��λ�����ϱ�״̬�ֲ���98��99���ܼ�"*/
        {
        }
        else if(rpResult->commPort == E_COM_SLAVE)
        {
            /*" ����оMASTER���͹����ı��Ĳ��ж��ܼ��� ��Ϊ�ǽ��ܹ������� "*/
            /*
            if(COMM_ERR_NOERR != SEsam_Far9998Dcp(rpResult->data-15))
            {
                ValTmp = COMM_ERR_PASSWORD;
            }
            else
            {
            }
            */
        }
        else
        {
            ValTmp = COMM_ERR_PASSWORD;
        }

    }
    else/*"2,4������ֻ�г���״̬"*/
    {
        /*������չģʽ��1,7-10������ģʽ��,�������������,���Ľ�������������*/
         if(  
            (ChkStatus(E_FAC) == TRUE)||
            (TmpdataId == 0x0E200004)||  
            (TmpdataId == 0x0E200005)||  
            (TmpdataId == 0x0E200008)|| 
            (TmpdataId == 0x0E200010)||
            (TmpdataId == 0x0E200014)|| 
            (TmpdataId == 0x0E200016)||  
            (TmpdataId == 0x0E200017)||
            (TmpdataId == 0x0E200018)||
            (TmpdataId == 0x0E200019)||
            (TmpdataId == 0x0E200028)||        
            (TmpdataId == 0x04001503)   /*"��λ�����ϱ�״̬��"*/
            )
         {
         }
         else
         {
            ValTmp = COMM_ERR_PASSWORD;
         }
    }    

    return ValTmp;
    
}    

INT8U SetParaToMter(ST_FRAME645 *rpResult)
{
    ST_FRM_RESULT nResult;
    ST_ANSWR_DATA nAnswer;
    INT8U ValTmp; 
    ST_OAD TmpOad;

    ValTmp = COMM_ERR_NOERR;

    nResult.authority  = rpResult->authority;
    nResult.commPort   = rpResult->commPort;
    nResult.data       = rpResult->data;
    nResult.dataId.asLong = rpResult->dataId.asLong;
    nResult.dataLen    = rpResult->dataLen;
    
    /*"3.��չд���� INTO FAC"*/
    if((0x0E20==nResult.dataId.asWord[1])&&(nResult.dataId.asBytes[0]>=0x20)) 
    {
        if(Action(E_AUTO_WRTEXTDATA,&nResult,NULL) != COMM_ERR_NOERR)
        {
            ValTmp = COMM_ERR_NODATA;
        }
    
        return ValTmp;
    }

    /*5.set data*/
    if(CommWriteData((ST_FRM_RESULT *)&nResult) != COMM_ERR_NOERR)
    {     
        ValTmp = PP_CommSetData((ST_FRM_RESULT *)&nResult, (ST_ANSWER_DATA *)&nAnswer); 
    }
    
    if(COMM_ERR_NOERR==ValTmp) /*"��¼����¼���¼"*/
    {  
        if((nResult.dataId.asLong>=0x0E200000)||(nResult.dataId.asLong==0x04001503))
        {
            
        }
        else
        {
            LIB_Reverse((INT8U *)&nResult.dataId.asBytes[0], (INT8U *)&TmpOad, sizeof(ST_OAD));
            LIB_RvsSelf(&TmpOad.Oi.asByte[0],2);
            PP_PrgEvent(TmpOad);
        }
    }
    else
    {
    }   
    return ValTmp;
}

/*"*************************************************************"*/			
/*"Function: 	CM_ReadData		"*/
/*"Description: ͨ��д���ݴ���"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
#define WT_BUFF_LEN  (10)
static INT8U CM_WriteData(ST_FRAME645 *rpResult)   
{	
    ST_FRM_RESULT nResult;
    //ST_ANSWR_DATA nAnswer;
    INT8U ValTmp; 
    //ST_OAD TmpOad;

    nResult.authority  = rpResult->authority;
	mAuthority         = nResult.authority;
    nResult.commPort   = rpResult->commPort;
    nResult.data       = rpResult->data;
    nResult.dataId.asLong = rpResult->dataId.asLong;
    nResult.dataLen    = rpResult->dataLen;
   
    ValTmp = COMM_ERR_NOERR;
    /*1.��ַ�ж�*/
    if((mCommAddrType !=ADDR_SELFADDR)&& (FALSE== ChkStatus(E_FAC)))  /*"�ǵ�Ե��ַ������"*/
    {
        ValTmp = COMM_ERR_OTHERS;
        return ValTmp;
    }

    /*2.�˳�����״̬*/
    if(0x0E200040 == rpResult->dataId.asLong)  
	{
		ValTmp= Action(E_OUT_FAC, NULL, NULL);
        return ValTmp;
	}

    /*4.Ȩ�޿���*/
	if(COMM_ERR_NOERR!=Check_645_Right(&nResult))
    {
       ValTmp = COMM_ERR_PASSWORD;
    }    
    else
    {     
        ValTmp = SetParaToMter(rpResult);       
    }
    
    return ValTmp;
}

/*"*************************************************************"*/			
/*"Function: 	CM_15Cmd		"*/
/*"Description: дͨ�ŵ�ַ"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CM_15Cmd(ST_FRAME645 *rpResult)  
{
    INT8U ValTemp;
    
    ValTemp = COMM_ERR_NOERR;
    if (rpResult->dataLen!= 6)
    {
        ValTemp = COMM_ERR_OTHERS;
    }
    else
    {
        /*"ȡ����״̬"*/
        if(TRUE == ChkStatus(E_FAC))
        {
            LIB_RvsSelf(rpResult->data,6);
            if(DAR_0_OK != SetSingle(E_COMM_ADDR,rpResult->data,6))
            {
                ValTemp = COMM_ERR_OTHERS;
            }
            
        }
        else
        {
            ValTemp = COMM_ERR_PASSWORD;
        }
    }

    return ValTemp;
}


/*"*************************************************************"*/			
/*"Function: 	CM_ClrCmd		"*/
/*"Description: ������,�¼���������"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CM_ClrCmd(ST_FRAME645 *rpResult)   
{
    //UN_ID645 id;
    INT8U buf[2];
    INT8U ValTemp;
    INT8U LoginFlg;
    INT8U MeterLockTmp;
    INT8U mtrTypeModeTmp;
    ValTemp = COMM_ERR_NOERR;
    LoginFlg = FALSE;

    //GetSingle(E_METER_MODE,(INT8U *)&mtrTypeModeTmp);
    mtrTypeModeTmp = 0;
    mtrTypeModeTmp &= METERTPYE;
    if(mCommAddrType == ADDR_BROADCAST)   /*"�㲥��ַ����������"*/
    {
         ValTemp = COMM_ERR_PASSWORD;
    }     
    else if(ChkStatus(E_FAC) == TRUE)
    {
         LoginFlg = TRUE;
    }
    else if(FALSE==IrStateflg)/*"����ͨ����û��ͨ��������֤"*/
    {
         ValTemp = COMM_ERR_PASSWORD;
    }
            /*"���ر�����������,����״̬��������"*/
    else if((0x1A==rpResult->command)&&(LOCALMETER==mtrTypeModeTmp))
    {
         ValTemp = COMM_ERR_PASSWORD;
    }
    else
    {
    }  

    if(rpResult->commPort == E_COM_SLAVE)
    {/*" ���ذ��·������� "*/
        LoginFlg = TRUE;
    }

    //GetSingle(E_HANGUP_FLAG, (INT8U *)&MeterLockTmp);
    MeterLockTmp = 0;
    if((COMM_ERR_NOERR==ValTemp)&&(T2_CLASS==rpResult->authority)&&(FALSE==MeterLockTmp))/*"�ҷǱ���״̬"*/
    {
        //user_req_peer_data_proc(0, rpResult->commPort, &rpResult, rpResult->frmLen);
        //return CM_UNUSUAL;
        /*
        if(DAR_0_OK==SEsam_FarClrCmd(rpResult->beginData+7))
        {
            LoginFlg = (INT8U)Action(E_REMOTE_CTR,rpResult->beginData+7,NULL);
            if(_PAY_OK==LoginFlg)
            {
                LIB_MemCpy(rpResult->data+8, rpResult->data, 4);
                LoginFlg = TRUE;
            }
            else
            {
                LoginFlg = FALSE;
            }
        }
        else
        {
        }
        */
    }
    else
    {
    }
    
    if(TRUE==LoginFlg)
    {
    /*"�������"*/
        if(0x1A==rpResult->command)
        {
            /* TODO
            if(rpResult->commPort < E_COM_SLAVE)
            {
                id.asLong = ID_CLR_MTR;
                tx_pkt_to_peer(0, 0x11, id, NULL, 0);
            }
            */
            CLR_MTR();
        }
        else
        {
            /* TODO
            if(rpResult->commPort < E_COM_SLAVE)
            {
                id.asLong = ID_CLR_EVT;
                tx_pkt_to_peer(0, 0x11, id, NULL, 0);
            }
            */
            buf[0] = 0xfe;
            buf[1] = 0xff;
            Action(E_RCRD_CLR, &buf[0], NULL);
            //ValTemp=CM_ClrEvent(rpResult);
        }
    }
    else
    {
        ValTemp = COMM_ERR_PASSWORD;
    }
    return ValTemp;
}

#if 0
/*"*************************************************************"*/			
/*"Function: 	CM_1CCmd		"*/
/*"Description: Զ�̿�������"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8U CM_1CCmd(ST_FRAME645 *rpResult)   
{
    INT8U ValTemp;
    INT8U LoginFlg;
    INT8U errVal;
    INT8U MeterLockTmp;
	INT8U relayMode;
    INT16U *errTmp;
    ValTemp = COMM_ERR_NOERR;
    LoginFlg = FALSE;
    errTmp=(INT16U *)&rpResult->beginData[10];
    if(mCommAddrType ==ADDR_BROADCAST)   /*"�㲥��ַ������"*/
    {
        ValTemp = COMM_ERR_PASSWORD;
    }
    else if(ChkStatus(E_FAC) == TRUE)
    {
        LoginFlg = TRUE;
    }
    else if((2==rpResult->authority)||(4==rpResult->authority))/*"���ĺ�բ"*/
    {
	    GetSingle(E_RELAY_TYPE,(INT8U *)&relayMode);
		if((relayMode &ENMWRELAY)&&(COMM_ERR_NOERR == Action(E_PASSWORD_CHK,rpResult->data-8,NULL)))/*"��������բ������"*/
		{
			LoginFlg = TRUE;
		}
		else if(((0x1B==rpResult->data[0])||(0x1C==rpResult->data[0]))&&(COMM_ERR_NOERR == Action(E_PASSWORD_CHK,rpResult->data-8,NULL)))
        {
            LoginFlg = TRUE;
        }
        else
        {
            ValTemp = COMM_ERR_PASSWORD;
        }
    }
    else
    {   
        
    }  
    if((COMM_ERR_NOERR==ValTemp)&&(0x98==rpResult->authority))
    {
        GetSingle(E_HANGUP_FLAG, (INT8U *)&MeterLockTmp);
        if(TRUE==MeterLockTmp)/*"����״̬"*/
        {
            ValTemp = COMM_ERR_PASSWORD;
            *errTmp=0x0001;
        }
        else
        {   
            errVal=SEsam_FarClrCmd(rpResult->beginData+7);/*"Զ�̿��ƽ���"*/
            if(RET_OK==errVal)
            {
                LoginFlg=TRUE;
            }
            else
            { 
                ValTemp = COMM_ERR_PASSWORD;
                *errTmp=(INT16U)errVal;
                LoginFlg=FALSE;
            }
        }  
    }
    else
    {
    }
    if(TRUE==LoginFlg)
    {
          /*"Զ�̿����������"*/
          ValTemp = (INT8U)Action(E_REMOTE_CTR,rpResult->beginData+7,NULL);
          if(ValTemp>0)
          {
                ValTemp = COMM_ERR_PASSWORD;
          }
          else
          {
          }
    }
    else
    {
        ValTemp = COMM_ERR_PASSWORD;
    } 
    return ValTemp;
}


/*"************************************************			
Function:DL645_03IntAuth
Description: Զ�������֤
Input:  pPkg:03�������������
Output:None
Return: None
Others: None
************************************************"*/
static INT8U DL645_03IntAuth(ST_CMD03 *pPkg)
{
    INT8U RetVal;

    RetVal = SEsam_FarIntAuth(pPkg->pData,pPkg->DI + DILEN);
    
    Push(E_PRGEVENT_START,FALSE);/*"��̽���"*/
    
    if(RetVal == RET_OK)
    {
        pPkg->len = sizeof(ST_RINTAUTHINFO);
    }
    else
    {
        Action (E_ESAM_DISCONNECT, NULL, NULL);/*"�ͷ�ESAM"*/
    }

    return RetVal;
}


/*"************************************************			
Function:DL645_03Handle
Description: 03���������
Input:  pPkg:03�������������
Output:None
Return: None
Others: None
************************************************"*/
static INT8U DL645_03Handle(ST_CMD03 *pPkg)
{
    INT8U RetVal;

    RetVal = 0xFF;
	testp = pPkg;

    switch(pPkg->di)
    	{
		case 0x070402FF:
			RetVal = TCA_Auth(pPkg);
			break;
		case 0x07040202:
			RetVal = TCA_Auth_Init(pPkg);
			break;
		case 0x07040203:
			RetVal = TCA_Backup(pPkg);
			break;
		default:
			break;
    	}
        pPkg->len = 5;
        pPkg->pData[0] = RetVal;
		
    return RetVal;
    
}

#endif

/*"************************************************			
Function:CM_Far03Cmd
Description: 03��ȫ��֤����
Input:  Pdata: ͨ��֡�е�ַ��ӵڶ���68��ʼ�ĵ�ַ
Output:None
Return: 0x00��ʾ�޴��󣬷�0x00��ʾִ���д�
Others: None
************************************************"*/
INT16U CM_Far03Cmd(INT8U *Pdata)
{
    ST_CMD03 stCmd03;
    UN_WORD_DEF RetVal ;

    RetVal.word = 0x00FF;
    stCmd03.len = *(Pdata+2);/*"��ȡ����������"*/
    stCmd03.DI = (Pdata+3);/*"��ȡ���ݱ�ʶ�׵�ַ"*/
    LIB_MemCpy((INT8U *)stCmd03.DI,(INT8U *)&stCmd03.di,4);
    stCmd03.pData = stCmd03.DI + 4; 
    LIB_MemCpy(stCmd03.pData+4,stCmd03.pData,5);
    switch(stCmd03.di)
    {
		case 0x070402FF:		
			RetVal.byte[0]= TCA_AuthHandle(&stCmd03);
			break;
		case 0x878002FF:
			RetVal.byte[0]= FarCodeCmp(&stCmd03);
			break;
		default:
			break;
    }
    *(Pdata+2) = stCmd03.len;
    return RetVal.word;
    
}
 	    

static INT8U CM_ReadEngylistData(UN_ID645 dataId, INT16U MaxLen,ST_ANSWER_DATA * panswer_data)
{
        INT8U beginTmp;
        INT8U i;
        INT16U nDtaLenNum;
        INT8U Ret;
        ST_ANSWER_DATA TmpAnsData;
            
        nDtaLenNum = 0;
        Ret = COMM_ERR_NOERR;
        TmpAnsData.Data_addr = panswer_data->Data_addr;
        
        if(dataId.asBytes[2]==0xFE)
        {
            beginTmp=1;
        }
        else
        {
            beginTmp=0;
        }        
        for(i=beginTmp;i<=0x0c;i++)
        {
            dataId.asBytes[0]=i;
            PP_CommGetDataExt(dataId, 0, MaxLen, (ST_ANSWER_DATA *)&TmpAnsData);

            TmpAnsData.Data_addr +=TmpAnsData.Data_len.int_len;
            nDtaLenNum+=TmpAnsData.Data_len.int_len;

            if(nDtaLenNum > MaxLen || (TmpAnsData.Data_len.bit.backframe == 1))
            {
                panswer_data->Data_len.bit.backframe = 1;
                Ret = COMM_ERR_OTHERS;
                break;
            }

        }
        panswer_data->Data_len.int_len= nDtaLenNum;
        return Ret;
}


static INT8U CM_ReadEvtlistData(UN_ID645 dataId, INT16U MaxLen,ST_ANSWER_DATA * panswer_data)
{
    INT8U i;
    INT16U nDtaLenNum;
    INT8U Ret;
    ST_ANSWER_DATA TmpAnsData;

    nDtaLenNum = 0;
    Ret = COMM_ERR_NOERR;
    TmpAnsData.Data_addr = panswer_data->Data_addr;
        
    for(i=1;i<=0x0A;i++)
    {
        dataId.asBytes[0]=i;
        PP_CommGetDataExt(dataId, 0, MaxLen, (ST_ANSWER_DATA *)&TmpAnsData);

        TmpAnsData.Data_addr +=TmpAnsData.Data_len.int_len;
        nDtaLenNum+=TmpAnsData.Data_len.int_len;

        if(nDtaLenNum > MaxLen || (TmpAnsData.Data_len.bit.backframe == 1))
        {
            panswer_data->Data_len.bit.backframe = 1;
            Ret = COMM_ERR_OTHERS;
            break;
        }

    }
    panswer_data->Data_len.int_len= nDtaLenNum;
    return Ret;
}
    
INT8U check_ReadList(ST_FRAME645 *framedata)
{
    INT8U Ret;
    INT8U IdNum;/*���������ݵĸ���*/
    INT8U Cnt;
    INT8U *Databuf;
    INT8U id_bak[4];

    Databuf = framedata->data;
    
    IdNum = LIB_BcdToChar(framedata->dataId.asBytes[0]);/*����ΪBCD����*/
    
    Ret = COMM_ERR_NOERR;    
    if((IdNum > _SUPPORT_ID_NUM))
    {
        Ret = COMM_ERR_OTHERS;
    }
    else if(IdNum == 0)
    {
        Ret = COMM_ERR_NODATA;
    }
    else
    {
        if(framedata->dataLen == (INT16U)(IdNum*5 -1))/*����У����ȷ*/ 
        {
            /*���ҷָ���0xBB*/
            if(IdNum > 1)
            {
                for(Cnt=1;Cnt<IdNum;Cnt++)
                {
                    if(Databuf[5*Cnt-1] != 0xBB)
                    {
                        Ret = COMM_ERR_OTHERS;
                        break;
                    }
                }
                
            }
            else /*ֻ��һ������û��0xBB*/
            {
            }
            peer_id_num = 0;
            sum_id_num = IdNum;
            LIB_MemCpy(&framedata->dataId.asBytes[0], id_bak, 4);
            for(Cnt=0;Cnt<IdNum;Cnt++)
            {
                LIB_MemCpy(&Databuf[5*Cnt], &framedata->dataId.asBytes[0], 4);
                if(is_peer_data(0, framedata->commPort, framedata))
                {
                    LIB_MemCpy(&Databuf[5*Cnt], &PeerIdList[peer_id_num].asBytes[0], 4);
                    peer_id_num++;
                }
                else
                {
                }
                LIB_MemCpy(&Databuf[5*Cnt], &SumIdList[Cnt].asBytes[0], 4);
            }
            LIB_MemCpy(id_bak, &framedata->dataId.asBytes[0], 4);

        }
        else
        {
            Ret = COMM_ERR_OTHERS;
        }

    }

    return Ret;    
}

INT8U ListDataIsAllPeerData(void)
{
    if(sum_id_num == peer_id_num)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

INT8U ListDataHasPeerData(void)
{
    if(peer_id_num != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void cfg_read_peer_list_data_to_peer(ST_FRAME645 *res)
{
    INT8U i;
    INT8U data[200];
    INT8U len;
    INT8U pkt_len;
    UN_ID645 id;

    id.asLong = 0xEEEEEE00UL | peer_id_num;/*" �������� "*/
    len = 0;
    for(i=0; i<peer_id_num; i++)
    {
        LIB_MemCpy(&PeerIdList[i].asBytes[0], &data[5*i], 4);
        len += 4;
        
        if(i != (peer_id_num - 1)) /*" ���һ����ҪBB "*/
        {
            data[5*i + 4] = 0xBB;
            len += 1;
        }
    }
    
    pkt_len = cfg_645_pkt(0x11, id, data, len, res->beginData);
    user_req_peer_data_proc(0, res->commPort, res->beginData, pkt_len);
}

INT8U get_peer_list_data(UN_ID645 id, INT8U *buf, ST_ANSWER_DATA *ans)
{
    INT8U *ptr;
    INT8U i;
    INT8U cur_len;
    INT8U dst_len;

    ptr = buf;
    for(i=0; i<peer_id_num; i++)
    {
        cur_len = ptr[0];
        if(LIB_MemCmp(ptr + 1, &id.asBytes[0], 4) == EQUAL)
        {
            if(cur_len <= 4)
            {
                break;
            }
            dst_len = cur_len - 4;
            LIB_MemCpy(ptr + 5, ans->Data_addr, dst_len);
            ans->Data_len.int_len = dst_len;
            return COMM_ERR_NOERR;
        }
        ptr += (cur_len + 2);
    }
    return COMM_ERR_NODATA;
}

INT8U ReplyUserIdData(ST_FRAME645 *res, ST_ANSWER_DATA *ans)
{
    INT8U Ret;
    INT8U Cnt;
    INT8U datalen;
    INT8U PeerIdata[200];
    ST_ANSWER_DATA TmpAnswer;
    INT16U nDtaLenNum;
    INT8U GetRet;
    UN_ID645 CurId;
    INT8U id_bak[4];
    
    datalen = res->dataLen;
    
    LIB_MemCpy(res->data, PeerIdata, datalen);/*" �ȱ���Զ�Ӧ������ "*/

    TmpAnswer.Data_addr = ans->Data_addr;
    res->command = 0x11; /*" ǿ��Ϊ������ "*/
    res->authority = 0x00;
    /*" TODO res->commPort ��is_peer_data����ʱδ�ã��Ȳ���ʼ�� "*/
    res->dataId.asLong = 0xEEEEEE00UL | sum_id_num; /*" id�ָ�Ϊ�û�����ֵ "*/
    nDtaLenNum = 0;
    Ret = COMM_ERR_NOERR;
    LIB_MemCpy(&res->dataId.asBytes[0], id_bak, 4);
    for(Cnt=0;Cnt<sum_id_num;Cnt++)
    {
        CurId = SumIdList[Cnt];
        TmpAnswer.Data_addr++;/*�������ݳ���Lλ��*/

        /*��������ID*/
        LIB_MemCpy(&CurId.asBytes[0], TmpAnswer.Data_addr, 4);
        TmpAnswer.Data_addr+=4;/*��������IDλ��*/
        nDtaLenNum+=5;

        if(nDtaLenNum >= COMM_DATA_MAX)
        {
            Ret = COMM_ERR_OTHERS;
            break;
        }
        LIB_MemCpy(&CurId.asBytes[0], &res->dataId.asBytes[0], 4);
        if(is_peer_data(0, res->commPort, res))
        {
            GetRet = get_peer_list_data(CurId, PeerIdata, &TmpAnswer);
        }
        /*"   ������ʷ���ݿ鳭 "*/
        else if((CurId.asBytes[3]==0x00)&&(CurId.asBytes[0]==0xff)
            && (CurId.asBytes[1]!=0xff)&&(CurId.asBytes[2]!=0xff))                       
        {
            GetRet = CM_ReadEngylistData(CurId, (COMM_DATA_MAX-nDtaLenNum),
                (ST_ANSWER_DATA *)&TmpAnswer);
        }
        /*"   ����բ���ݿ鳭, �������ݿ鳭 "*/
        else if(((CurId.asBytes[3]==0x1D)||(CurId.asBytes[3]==0x1E)
            ||(CurId.asBytes[3]==0x19))&&(CurId.asBytes[0]==0xff))   
        {
            GetRet = CM_ReadEvtlistData(CurId, (COMM_DATA_MAX-nDtaLenNum),
                (ST_ANSWER_DATA *)&TmpAnswer);

        }
        else/*�������ݳ���*/
        {
            GetRet = PP_CommGetDataExt(CurId, 0, (COMM_DATA_MAX-4)-nDtaLenNum, 
                                        (ST_ANSWER_DATA *)&TmpAnswer);
        }


        if(TmpAnswer.Data_len.bit.backframe == 0)/*�������ݲ��漰����֡*/
        {
            /*�����ܳ���δ����200*/
            if((nDtaLenNum+TmpAnswer.Data_len.int_len) <= (COMM_DATA_MAX-4))
            {
                /*��䳤������*/
                *(TmpAnswer.Data_addr -5)= (INT8U)(4+ TmpAnswer.Data_len.int_len);

                if(GetRet == COMM_ERR_NOERR)
                {
                    TmpAnswer.Data_addr+=TmpAnswer.Data_len.int_len;
                    nDtaLenNum         +=TmpAnswer.Data_len.int_len;
                }
                else
                {

                }
            }
            else
            {
                Ret = COMM_ERR_OTHERS;
                break;
            }
        }
        else/*�����к���֡���϶��������ȣ����ش���*/
        {
            Ret = COMM_ERR_OTHERS;
            break;
        }

        /*��Ϊ���һ�����ݣ����0xBB*/
        if(Cnt<sum_id_num-1)
        {
            *(TmpAnswer.Data_addr)= 0xBB;
            TmpAnswer.Data_addr++;
            nDtaLenNum++;
        }
    }
    LIB_MemCpy(id_bak, &res->dataId.asBytes[0], 4);
    
    if(Ret == COMM_ERR_NOERR)
    {
        ans->Data_len.int_len = nDtaLenNum;
    }
    else
    {
        ans->Data_len.int_len = 0;
    }    
    return Ret;
}


static INT8U CM_ReadListData(ST_FRM_RESULT *framedata, ST_ANSWER_DATA * nAnswer)
{
    INT8U Ret;
    INT8U IdNum;/*���������ݵĸ���*/
    UN_ID645 IdList[_SUPPORT_ID_NUM];
    INT8U Cnt;
    INT8U * Databuf;
    INT16U nDtaLenNum;
    ST_ANSWER_DATA TmpAnswer;
    INT8U *DestBuf;/*������ݻ�������ַ*/
    INT8U GetRet;

    Databuf = framedata->data;
    Ret = COMM_ERR_NOERR;
    LIB_MemSet(0xFF, (INT8U *)&IdList[0], sizeof(IdList));
    nDtaLenNum = 0;
    DestBuf = nAnswer->Data_addr;
    TmpAnswer.Data_addr = DestBuf;
    
    IdNum = LIB_BcdToChar(framedata->dataId.asBytes[0]);/*����ΪBCD����*/
        
    if((IdNum > _SUPPORT_ID_NUM))
    {
        Ret = COMM_ERR_OTHERS;
    }
    else if(IdNum == 0)
    {
        Ret = COMM_ERR_NODATA;
    }
    else
    {
        if(framedata->dataLen == (INT16U)(IdNum*5 -1))/*����У����ȷ*/ 
        {
            /*���ҷָ���0xBB*/
            if(IdNum > 1)
            {
                for(Cnt=1;Cnt<IdNum;Cnt++)
                {
                    if(Databuf[5*Cnt-1] != 0xBB)
                    {
                        Ret = COMM_ERR_OTHERS;
                        break;
                    }
                }
                
            }
            else /*ֻ��һ������û��0xBB*/
            {
            }

            /*�Զ�ȡID���л���*/
            if(Ret == COMM_ERR_NOERR)
            {
                for(Cnt=0;Cnt<IdNum;Cnt++)
                {
                    LIB_MemCpy(&Databuf[5*Cnt], &IdList[Cnt].asBytes[0], 4);
                }

                for(Cnt=0;Cnt<IdNum;Cnt++)
                {
                    TmpAnswer.Data_addr++;/*�������ݳ���Lλ��*/

                    /*��������ID*/
                    LIB_MemCpy(&IdList[Cnt].asBytes[0], TmpAnswer.Data_addr, 4);
                    TmpAnswer.Data_addr+=4;/*��������IDλ��*/
                    nDtaLenNum+=5;

                    if(nDtaLenNum >= COMM_DATA_MAX)
                    {
                        Ret = COMM_ERR_OTHERS;
                        break;
                    }

                    /*"   ������ʷ���ݿ鳭 "*/
                    if((IdList[Cnt].asBytes[3]==0x00)&&(IdList[Cnt].asBytes[0]==0xff)
                        && (IdList[Cnt].asBytes[1]!=0xff)&&(IdList[Cnt].asBytes[2]!=0xff))                       
                    {
                        GetRet = CM_ReadEngylistData(IdList[Cnt], (COMM_DATA_MAX-nDtaLenNum),
                            (ST_ANSWER_DATA *)&TmpAnswer);
                    }
                    /*"   ����բ���ݿ鳭, �������ݿ鳭 "*/
                    else if(((IdList[Cnt].asBytes[3]==0x1D)||(IdList[Cnt].asBytes[3]==0x1E)
                        ||(IdList[Cnt].asBytes[3]==0x19))&&(IdList[Cnt].asBytes[0]==0xff))   
                    {
                        GetRet = CM_ReadEvtlistData(IdList[Cnt], (COMM_DATA_MAX-nDtaLenNum),
                            (ST_ANSWER_DATA *)&TmpAnswer);

                    }
                    else/*�������ݳ���*/
                    {
                        GetRet = PP_CommGetDataExt(IdList[Cnt], 0, (COMM_DATA_MAX-4)-nDtaLenNum, 
                                                    (ST_ANSWER_DATA *)&TmpAnswer);
                    }


                    if(TmpAnswer.Data_len.bit.backframe == 0)/*�������ݲ��漰����֡*/
                    {
                        /*�����ܳ���δ����200*/
                        if((nDtaLenNum+TmpAnswer.Data_len.int_len) <= (COMM_DATA_MAX-4))
                        {
                            /*��䳤������*/
                            *(TmpAnswer.Data_addr -5)= (INT8U)(4+ TmpAnswer.Data_len.int_len);

                            if(GetRet == COMM_ERR_NOERR)
                            {
                                TmpAnswer.Data_addr+=TmpAnswer.Data_len.int_len;
                                nDtaLenNum         +=TmpAnswer.Data_len.int_len;
                            }
                            else
                            {

                            }
                        }
                        else
                        {
                            Ret = COMM_ERR_OTHERS;
                            break;
                        }
                    }
                    else/*�����к���֡���϶��������ȣ����ش���*/
                    {
                        Ret = COMM_ERR_OTHERS;
                        break;
                    }

                    /*��Ϊ���һ�����ݣ����0xBB*/
                    if(Cnt<IdNum-1)
                    {
                        *(TmpAnswer.Data_addr)= 0xBB;
                        TmpAnswer.Data_addr++;
                        nDtaLenNum++;
                    }
                    
                }
                
            }
            else
            {
            }

        }
        else
        {
            Ret = COMM_ERR_OTHERS;
        }

    }

    if(Ret == COMM_ERR_NOERR)
    {
        nAnswer->Data_len.int_len = nDtaLenNum;
    }
    else
    {
        nAnswer->Data_len.int_len = 0;           
    }
    return Ret;    
}

