/*"*****************************************************************************"*/
/*"  FileName: AUTO.c"*/
/*"  Description:     ���ļ�ʵ�ַ������Զ���ģ�����ӿں�������"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      2017/10/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "..\..\PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "..\..\Driver\HAL\FS\FS_Def.h"
#include "AUTO.h"
#include "App\MtrParamDefault\MtrParamDef.h"

static INT8S AUTO_645SelfTest(INT8U *pdata);

/*"*************************************************************"*/			
/*"Function: 	AUTO_Main		"*/
/*"Description: �Զ�������ģ�鿪�Ǽ��"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
void AUTO_Main(void)
{
	if(ChkStatus(E_FAC) == TRUE)/*"����״̬�²�ִ��"*//*"�����ڿ���״̬�У���Կ���ϲ���˸"*/
	{
		if((SID_PollRead(SID_MTRCOVER)==TRUE)
		#ifdef THREE_PHASE
		||(SID_PollRead(SID_RAILCOVER)==TRUE)
		#endif
		)
		{
			DIS_FlashStauSeg(STA_KEY);
		}
		else
		{
			DIS_ClrFlashStauSeg(STA_KEY);
		}
	}
}

#ifdef THREE_PHASE
/*"*************************************************************"*/			
/*"Function: 	AUTO_KeySelfTest		"*/
/*"Description: �Զ�������ģ�鰴�����"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_KeySelfTest(void *pdin, void *pdout)
{
	INT8U *pdata;

	pdata = (INT8U *)pdin;

	if(ChkStatus(E_FAC) == TRUE)/*"����״̬�²�ִ��"*/
	{
		AUTO_645SelfTest(pdata);
	}

	return MS_SUCCESS;
}
#endif


/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadDateVer		"*/
/*"Description: ���������ʾ�汾��"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadDateVer(INT8U *pdata)    
{   
	INT8U i;
	INT32U SofVer;
	
	SofVer = 0;
	for(i=0;i<8;i++)
	{
		SofVer |= ((INT32U)(ShowVersion[i]-0x30)<<(7-i)*4);
	}

	LIB_MemCpy((INT8U *)&SofVer,pdata,4);

    return 4;   
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadTmpID1		"*/
/*"Description: �������ܱ�Ψһ��ʶ1"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadTmpID1(INT8U *pdata)    
{   
    GetSingle(E_PRODUCT_ID1,pdata);
	LIB_RvsSelf(pdata,32);
    return 32;   
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadTmpID2		"*/
/*"Description: �������ܱ�Ψһ��ʶ2"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadTmpID2(INT8U *pdata)    
{   
    GetSingle(E_PRODUCT_ID2,pdata);
	LIB_RvsSelf(pdata,32);
    return 32;   
}


/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadTmpID3		"*/
/*"Description: �������ܱ�Ψһ��ʶ3"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadTmpID3(INT8U *pdata)    
{   
    GetSingle(E_PRODUCT_ID3,pdata);
	LIB_RvsSelf(pdata,32);
    return 32;   
}



/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadStepNum		"*/
/*"Description: ����ǰ���ݺ�"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadStepNum(INT8U *pdata)    
{
    return 1;  
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadFac		"*/
/*"Description: ������״̬"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadFac(INT8U *pdata)
{
	UN_FACWORD_FORM facword;
	facword.word = 0;
	if(ChkStatus(E_FAC) == TRUE)
	{
		facword.bits.facState = TRUE;
	}

	if(ChkStatus(E_FIRST_PWR_ON) == TRUE)
	{
		facword.bits.dfultFlg = TRUE;
	}

	if(ChkStatus(E_CLK_BAT_LOSS) == TRUE)
	{
		facword.bits.clockBat = TRUE;
	}
	
	if(ChkStatus(E_DIS_BAT_LOSS) == TRUE)
	{
		facword.bits.LowPowBat = TRUE;
	}

	if(ChkStatus(E_RELAY_PROTECT) == TRUE)
	{
		facword.bits.elecprotec = TRUE;
	}
	
	LIB_MemCpy((INT8U *)&facword.word,pdata,2);
	return 2;
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadCrc		"*/
/*"Description: 645��crc����"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadCrc(INT8U *pdata)
{
    INT8U lenTmp;

	LIB_MemSet(0x00,&pdata[0],12);
#ifdef PM_MODLE    
	/*"Ԥ���Ѳ���CRC"*/
	GetSingle(E_PMPARA_CRC,&pdata[0]);
#endif
	lenTmp=12;
	
	return lenTmp;
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645ReadSoftVer		"*/
/*"Description: ����汾��"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645ReadSoftVer(INT8U *pdata)
{
    INT8U lenTmp;

	LIB_ConstMemCpy(SOFT_VERSION_WS, pdata, sizeof(SOFT_VERSION_WS));
	LIB_RvsSelf(pdata,sizeof(SOFT_VERSION_WS));

	lenTmp=32;
	
	return lenTmp;
}

/*"*************************************************************"*/			
/*"Function: 	STM_ReadExtData		"*/
/*"Description: ��չ�����"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
const ST_COMFUNCTION mstReadFun[]=
{
	{0x20,AUTO_645ReadDateVer}, /*"���������ʾ�汾��"*/
	{0x22,AUTO_645ReadSoftVer},/*"��������汾��"*/
	{0x23,AUTO_645ReadTmpID1}, /*"�������ܱ�Ψһ��ʶ1"*/
	{0x24,AUTO_645ReadTmpID2}, /*"�������ܱ�Ψһ��ʶ2"*/
	{0x25,AUTO_645ReadTmpID3}, /*"�������ܱ�Ψһ��ʶ3"*/
#ifdef PM_MODLE
    {0x27,AUTO_645ReadStepNum},/*"����ǰ���ݺ�"*/
#endif    
    {0x40,AUTO_645ReadFac},/*"���������ʼ��״̬������״̬"*/
    {0x61,AUTO_645ReadCrc},	   /*"��������Ч����"*/
};

#define READLISTNUM  ARRAY_SIZE(mstReadFun)

INT8S AUTO_ReadExtData(void *pdin, void *pdout)
{
    INT8U i;
    INT8S LenTemp;
	ST_FRMINFO *mfrmInfo;
	mfrmInfo = (ST_FRMINFO *)pdin;

    LenTemp=MS_OTHER_ERR;/*"��ʼ��ֵʧ��"*/
    for(i=0;i<READLISTNUM;i++)
    {
        if(mfrmInfo->dataId[0]==mstReadFun[i].ComDataId)
        {
            LenTemp=mstReadFun[i].OnReadWrite(mfrmInfo->data);/*"�ɹ�������ֵ���������ص����ݳ���"*/
            break;
        }
        else
        {
        }
    }
    return LenTemp;
	
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645TraffTest		"*/
/*"Description: ʱ��Ͷ�в�������"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645TraffTest(INT8U *pdata)   
{
	OC_MultiTermnSw((EN_OC_MultiTermnMode)pdata[0]);
	OC_MultiTermn((EN_OC_MultiTermnMode)pdata[0]);/*"���һ��Ͷ���ź�"*/

    return MS_SUCCESS;
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645WriteTmpID1		"*/
/*"Description: д���ܱ�Ψһ��ʶ1"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645WriteTmpID1(INT8U *pdata)    
{
	LIB_RvsSelf(pdata,32);
	FS_WriteFile(E_FILE_AUTO_DATA,PRODUCTID1_SHIFT,pdata,32);
	
    return MS_SUCCESS;   
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645WriteTmpID2		"*/
/*"Description: д���ܱ�Ψһ��ʶ2"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645WriteTmpID2(INT8U *pdata)    
{
	LIB_RvsSelf(pdata,32);
	FS_WriteFile(E_FILE_AUTO_DATA,PRODUCTID2_SHIFT,pdata,32);
	
    return MS_SUCCESS;   
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645WriteTmpID3		"*/
/*"Description: д���ܱ�Ψһ��ʶ3"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645WriteTmpID3(INT8U *pdata)    
{
	LIB_RvsSelf(pdata,32);
	FS_WriteFile(E_FILE_AUTO_DATA,PRODUCTID3_SHIFT,pdata,32);
	
    return MS_SUCCESS;   
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645RelayTest		"*/
/*"Description: �̵���������������բһ��"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
INT8S AUTO_645RelayTest(INT8U *pdata)    
{
  #ifdef PM_MODLE
	INT8U relayMode,rlycmd;
	GetSingle(E_RELAY_TYPE,(INT8U *)&relayMode);
	relayMode = (relayMode&RLYTYPE);
	if(*pdata == 0)
	{
		rlycmd = OC_GetRelayCtrlStatus();	/*"��ȡ�̵�������״̬"*/
		if(RELAY_ACT_CLOSE == rlycmd)
		{
			OC_RelayAct(E_RELAY_SC_PREPAY,E_RELAY_BREAK,(EN_OC_RELAYACTMODE)relayMode,RELAY_ACT_TRIP,400);
			OC_LedOnContin(E_ID_LED_RELAY, E_LED_SC_SNG);/*"����բ��"*/
		}
		else if(RELAY_ACT_TRIP == rlycmd)
		{
			OC_RelayAct(E_RELAY_SC_PREPAY,E_RELAY_BREAK,(EN_OC_RELAYACTMODE)relayMode,RELAY_ACT_CLOSE,400);
			OC_LedOff(E_ID_LED_RELAY, E_LED_SC_SNG);/*"����բ��"*/
		}
	}
	#ifdef THREE_PHASE
	else
	{
		rlycmd = OC_GetAlmRelayCmd();	/*"��ȡ�����̵�������״̬"*/
		if(RELAY_ACT_CLOSE == rlycmd)
		{
			AO_Relay(E_RELAY_RELAYALARM , RELAY_ACT_TRIP ,E_RELAY_MODE_LEVEL);
		}
		else if(RELAY_ACT_TRIP == rlycmd)
		{
			AO_Relay(E_RELAY_RELAYALARM , RELAY_ACT_CLOSE ,E_RELAY_MODE_LEVEL);
		}
	}
	#endif
#endif    
	
        return MS_SUCCESS;   

}


/*"*************************************************************"*/			
/*"Function: 	STM_645SelfTest		"*/
/*"Description: �Լ�ģʽ����"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8S AUTO_645SelfTest(INT8U *pdata)   
{
    ST_SHOW_DATA showData;

	if(1==pdata[0])/*"�Լ�һ��"*/
    {
        LIB_MemSet(0, (INT8U *) &showData, sizeof(showData));
		showData.Protc.showDataBuff[7] = _SHOW_ALL_;/*"ȫ����ʾ"*/
		DIS_PrintLcdLine1(&showData,3,LEVEL_3);
		OC_LedOnSingle(E_ID_LED_RELAY, E_LED_ALWAYS_MODE, 3);/*"����բ��"*/
		OC_LedOnSingle(E_ID_LED_BGD, E_LED_ALWAYS_MODE, 3);/*"������"*/
		OC_BuzzerOnSingle(E_BUZ_CARD_SUC);/*"��������"*/
    }

    return MS_SUCCESS;
}

/*"*************************************************************"*/			
/*"Function: 	AUTO_645InToFactoy		"*/
/*"Description: �Զ����볧������"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/
static INT8S AUTO_645InToFactoy(INT8U *pdata) 
{
	INT8U mData[16];
    ST_7TIME_FORM nowTime;
    INT32U NowTime,TotalTime;
    ST_INTOFACTORY *stIntoFac;

	stIntoFac = (ST_INTOFACTORY *)mData;
	GetSingle(E_SYS_TIME, (INT8U *)&nowTime);

	LIB_MemCpy(pdata,mData,8);
	Data_DecrypTion(mData);
	LIB_RvsSelf(mData+2,6);
	LIB_BcdToCharNByte(mData+2, 6);

    if(LIB_ChkDateTime(&stIntoFac->validTime) == FALSE)
    {
        return MS_W_AUTH_ERR;
    }
    else
    {
    }

    TotalTime = LIB_SToStdDate(&stIntoFac->validTime);
    NowTime = LIB_SToStdDate((ST_6TIME_FORM *)&nowTime);
    if(TotalTime < NowTime)
    {
        return MS_W_AUTH_ERR;
    }
    else
    {
    }

	LIB_MemCpy(pdata+8,mData+8,8);
	Data_DecrypTion(mData+8);
	if(LIB_MemCmp(mData+8,(INT8U *)&SupPassWord[0],8) != EQUAL)
	{
        return MS_W_AUTH_ERR;
	}
    else
    {
    }

	if(stIntoFac->cmd != 0x5A)
	{
        return MS_W_AUTH_ERR;
	}
	else
    {
    }
	
	mData[0] = 1;
	Push(E_FAC, TRUE);
	Action(E_RESET_FAC_REMAIN_TIME, mData, NULL);

    return MS_SUCCESS;

}


/*"*************************************************************"*/			
/*"Function: 	AUTO_WriteExtData		"*/
/*"Description: ��չ����д"*/
/*"Input:  "*/
/*"Output: 	"*/
/*"Return: "*/
/*"Others: 		"*/
/*"*************************************************************"*/

const ST_COMFUNCTION mstWriteFun[]=
{
    {0x23,AUTO_645WriteTmpID1}, 
	{0x24,AUTO_645WriteTmpID2}, 
	{0x25,AUTO_645WriteTmpID3}, 
	{0x28,AUTO_645InToFactoy}, 
	{0x41,AUTO_645RelayTest},
    {0x42,AUTO_645TraffTest},
    {0x43,AUTO_645SelfTest}, 
};

#define WRITELISTNUM  ARRAY_SIZE(mstWriteFun)

INT8S AUTO_WriteExtData(void *pdin, void *pdout)
{
    INT8U i;
    INT8S ValTemp;
	ST_FRMINFO *mfrmInfo;
	mfrmInfo = (ST_FRMINFO *)pdin;

    ValTemp=MS_OTHER_ERR;
    for(i=0;i<WRITELISTNUM;i++)
    {
        if(mfrmInfo->dataId[0]==mstWriteFun[i].ComDataId)
        {
            ValTemp=mstWriteFun[i].OnReadWrite(mfrmInfo->data);
            break;
        }
        else
        {
        }
    }
    return ValTemp;
	
}

