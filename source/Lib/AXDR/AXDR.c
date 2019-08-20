/*"***************************************************************************"*/
/*"FileName: AXDR.c		                                "*/
/*"Author: ����                                             "*/                
/*"Version : 1.1       Date: 2017.07.21	                    "*/
/*"Description:   A-XDR�������ʵ��                             "*/
/*"Function List:                                           "*/
/*   1��ʵ����A-XDR�����,�������볤�ȣ�ʵ�ֽ���������ж�"*/
/*"<FunctionName>		                                    "*/    
/*"<author> <time> <version > <desc>		                "*/
/*"************************************************************************"*/

#include "System\Sys_def.h"
#include "Lib\LIB_func.h"
#include "AXDR.h"




/*OAD\OMD�����ʽ*/
const ST_ENCODE_INFO OamdInfo[] = {{longunsigned,2},{unsigned_t,1},{unsigned_t,1}};
const ST_ENCODE_INFO OiInfo[] = {{longunsigned,2},};/*OI�����ʽ��Ϣ*/

const ST_ENCODE_INFO UnitInfo[] = {{integer,1},{enum_t,1}};
const ST_ENCODE_INFO ComdcbInfo[] = {{enum_t,1},{enum_t,1},{enum_t,1},{enum_t,1},{enum_t,1}};
const ST_ENCODE_INFO DateInfo[] ={{longunsigned,2},{unsigned_t,1},{unsigned_t,1},{unsigned_t,1} };
const ST_ENCODE_INFO TimeInfo[] = {{unsigned_t,1},{unsigned_t,1},{unsigned_t,1} ,};
const ST_ENCODE_INFO DatetimesInfo[] = {{longunsigned,2},{unsigned_t,1},{unsigned_t,1},
                                        {unsigned_t,1} , {unsigned_t,1},{unsigned_t,1}};
const ST_ENCODE_INFO TiInfo[] = {{enum_t,1},{longunsigned,2}};
const ST_ENCODE_INFO COMDCBInfo[]={{enum_t,1},{enum_t,1},{enum_t,1},{enum_t,1},{enum_t,1}};
const ST_ENCODE_INFO MacInfo[]={{octetstring,4}};
const ST_ENCODE_INFO SIDInfo[]={{doublelongunsigned,4},{octetstring,SID_ADDDATA_LEN}};
const ST_ENCODE_INFO SIDMACInfo[]={{doublelongunsigned,4},{octetstring,SID_ADDDATA_LEN},{octetstring,4}};
const ST_ENCODE_INFO RNInfo[]= {{octetstring,8}};

#define SUPPORT_MOD (2)/*���֧��ģ�鶨��*/ 
static ArrStrLenHook GetDataLenHookFun[SUPPORT_MOD]={NULL,NULL};/*��ȡ�ɱ䳤�ȹ��Ӻ���*/


/******************************���벿��**********************************************/



void AXDR_DataLenGetInit(ArrStrLenHook HookFun,INT8U RegId);
INT16U EnCode_LenArea(INT16U Len,INT8U *pDst);
INT16U EnCode_Base(INT8U *pSrc,ENUM_DATATYPE Type, INT16U SrcLen,INT8U *pDst);
INT16U EnCode_Simple(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *pDst);
INT16U EnCode_Data(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum);
INT8U EnCode_Option(INT8U Flag,INT8U *pDst);


static INT16U GetBaseOrgLen(ENUM_DATATYPE Type,INT16U Len);
static void BitRevers(INT8U *Pdata);
static INT16U EnCode_BaseSquence(ST_ENCODE_INFO * pInfo,INT16U *pSrcLen,INT8U SquNum,INT8U *pSrc,INT8U *pDst);

static INT16U EnCode_SimpleData(INT8U *pSrc,INT16U *PSrcLen,ST_ENCODE_INFO * pInfo,INT8U *pDst,INT8U EnChoice);
static INT16U EnCode_ArrStrHead(ST_ENCODE_INFO * pInfo,INT8U *pDst,INT8U EnChoice);
static INT16U EnCode_ComplexData(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum,INT8U EnChoice);
static INT16U DataLenGet(ENUM_DATALEN_ID Id);



/******************************���벿��**********************************************/
INT16U DeCode_LenArea(INT8U *pSrc,INT8U *DataOffset);



/******************************���벿��**********************************************/

void   AXDR_ArrStrLenGetInit(ArrStrLenHook HookFun,INT8U Mod)
{

    GetDataLenHookFun[Mod] = HookFun;
}
/*"************************************************			
Function: 	GetBaseOrgLen		
Description:����Data�������ݵı���ǰ���ݵĳ���
Input:  
        Type:�������ͣ�Ŀǰ֧��Data����
        Len:���ڱ䳤���ݣ���Ҫ�������ݳ��ȣ���Ҫ��Ϊ��
Output: 	
Return:���� 
Others: 
   
************************************************"*/	
static INT16U GetBaseOrgLen(ENUM_DATATYPE Type,INT16U Len)
{
    INT16U RetLen;
    if(null_data == Type)
    {
        RetLen = 0;
    }
    if((integer == Type) || (unsigned_t == Type)||(enum_t == Type)||(bool_t == Type)
        )
    {
        RetLen = 1;
    }
    else if((long_t == Type) || (longunsigned == Type))
    {
        RetLen = 2;
    }
    else if( (doublelong== Type) || (doublelongunsigned == Type))
    {
        RetLen = 4;
    }
    else if( (long64== Type) || (long64unsigned == Type))
    {
        RetLen = 8;
    }
    /*�ǹ̶����Ȳ��ֳ���*/
    else if((octetstring== Type)||(visiblestring== Type)||(UTF8string == Type))
    {
        RetLen = Len;
    }
    else if(bitstring == Type)     
    {
        if((Len%8)!= 0)
        {
            RetLen = 1;
        }
        else
        {
            RetLen = 0;
        }
        RetLen += (Len/8);
    }
    else /*no support yet*/
    {
        RetLen = 0;
    }
    return RetLen;

}


/*"************************************************			
Function: 	EnCode_LenArea		
Description: ���볤����
Input:  len�����ݳ���
Output: pDst:�����������Ŀ�ĵ�ַ,��pDstΪNULLʱ����ʾ������������ݣ����ڼ��㳤�ȡ�	
Return: �������ݳ���	
Others: 
   
************************************************"*/	
INT16U EnCode_LenArea(INT16U Len,INT8U *pDst)
{
    INT8U RetLen;
    if(Len < 128)
    {
        if(pDst !=NULL)
        {
            *pDst =(INT8U)Len;
        }
        RetLen = 1;
    }	   
    else if(Len > 255)
    {
        if(pDst !=NULL)
        {
            *pDst =0x82;		   	  
            *(pDst+1) = (INT8U)(Len >> 8);
            *(pDst+2) = (INT8U)(Len & 0x00FF);
        }
        RetLen = 3;
    }
    else
    {    	
        if(pDst !=NULL)
        {
            *pDst = 0x81;
            *(pDst+1) = (INT8U)Len;
        }
        RetLen = 2;
    }
    return RetLen;
}

/*��תbitλ*/
static void BitRevers(INT8U *Pdata)
{
	INT8U i,rtn;

	rtn = 0;

	for(i = 0;i < 8; i ++)
	{
		if((1<<i) & (*Pdata))
		{
			rtn |= (1<<(7-i)) ;
		}
	}
    (*Pdata) = rtn;
}


/*"************************************************			
Function: 	EnCode_Base		
Description: ����Data�����л��������ݣ���� ENUM_DATATYPE�л������͵Ķ���
Input:  pSrc :Դ���ݵ�ַָ��
        Type:Դ��������
        SrcLen��Դ���ݳ���
Output: pDst:�����������Ŀ�ĵ�ַ����pDstΪNULLʱ����ʾ�������������	
Return: ��������ݳ���	
Others: 
************************************************"*/	
INT16U EnCode_Base(INT8U *pSrc,ENUM_DATATYPE Type, INT16U SrcLen,INT8U *pDst)
{
    INT16U RetLen;
    INT16U TmpLen;
    INT8U Cnt;
    
    RetLen = 0;
    /*�̶����ȱ��룬��Ҫ���Ǵ�С�˵Ĳ���*/
    if( 
        (doublelong == Type) ||
        (doublelongunsigned == Type)||
        (long_t == Type ) ||
        (longunsigned == Type)||
        (long64 == Type)||
        (long64unsigned == Type)
        )
    {
        RetLen = GetBaseOrgLen(Type,NULL);
        if(pDst !=NULL)
        {
            #ifdef BIG_ENDIAN
            LIB_MemCpy(pSrc, pDst, (INT8U)RetLen);
            #else
            LIB_Reverse(pSrc, pDst, (INT8U)RetLen);
            #endif
            
        }
    }
    /*�̶����ȱ��룬����Ҫ���Ǵ�С�˲���*/
    else if(
        (bool_t == Type)||
        (integer == Type )||
        (unsigned_t == Type)||
        (enum_t == Type)
        )
    {
        RetLen = 1;
        if(pDst !=NULL)
        {
            *pDst = *pSrc;
        }
    }
    else if ((octetstring == Type)||  /*��Ҫ�೤��ͷ���������*/
        (visiblestring	== Type)||
        (UTF8string == Type) )
    {
        RetLen = EnCode_LenArea(SrcLen,pDst);
        if(pDst != NULL)
        {
            LIB_MemCpy(pSrc, pDst+RetLen, SrcLen);
        }
        RetLen += SrcLen;
    }
    else if(bitstring == Type) /*bitstring���ͱ���*/
    {
        RetLen = EnCode_LenArea(SrcLen,pDst);
        TmpLen = GetBaseOrgLen(Type,SrcLen);

        if(pDst !=NULL)
        {
            LIB_MemCpy(pSrc, pDst+RetLen, TmpLen);
            for(Cnt = 0;Cnt < (INT8U)TmpLen;Cnt++)
            {
                BitRevers((pDst+RetLen) + Cnt);
            }
        }
        RetLen += TmpLen;
    }
    else if(null_data== Type)
    {
        RetLen = 0;
    }
    else
    {
        /*no support yet*/
    }

    return RetLen;
}


/*"************************************************			
Function: 	EnCode_BaseSquence		
Description: �Ի���������ɵ�Squence���б���

Input:  pInfo ����������Ϣ
        SquNum :squence�ĳ���
        pSrc:Դ���ݵ�ַ
        
Output: pSrcLen :Դ���ݳ���,����ԭʼ��������ƫ�ơ�
        pDst:�����������Ŀ�ĵ�ַ����pDstΪNULLʱ����������롣	
Return: ��������ݳ���	
Others: 
************************************************"*/	
static INT16U  EnCode_BaseSquence(ST_ENCODE_INFO * pInfo,
                                    INT16U *pSrcLen,
                                   INT8U SquNum,
                                   INT8U *pSrc,
                                   INT8U *pDst)
{
    INT8U Cnt;
    INT8U *TmppSrc;
    INT8U *TmppDst;
    INT8U *Tmpp;
    INT16U TmpLen;
    INT16U TmpSrcLen;
    INT16U RetSrcLen;

    INT16U RetLen;

    TmppSrc = pSrc; 
    TmppDst = pDst;
    RetLen = 0;
    RetSrcLen = 0;
    
    for(Cnt=0; Cnt < SquNum; Cnt++)
    {
        TmpSrcLen = GetBaseOrgLen(pInfo[Cnt].Type,pInfo[Cnt].Len);
        /*������Ӧ�������ͱ���*/   
        if(TmpSrcLen < 128)
        {
        }
        else/*��Ϣ��>=128�ĳ������ݣ���Ϊ�ǿɱ�����ݳ��ȣ���Ҫ�ӽӿڻ�ȡ*/
        {
            TmpSrcLen = DataLenGet((ENUM_DATALEN_ID)TmpSrcLen);
        }
        
        if(pDst == NULL)
        {
            Tmpp = NULL;
        }
        else
        {
            Tmpp = TmppDst;
        }
        TmpLen  = EnCode_Base(TmppSrc,pInfo[Cnt].Type, TmpSrcLen,Tmpp);
        TmppSrc += TmpSrcLen;
        RetSrcLen += TmpSrcLen;
        TmppDst += TmpLen;
        RetLen += TmpLen;
    }
    *pSrcLen = RetSrcLen;
    return RetLen;
}


/*"************************************************			
Function: 	EnCode_SimpleData	
Description: ����һ��Data��������,������array��struct
Input:  
        pSrc:Դ���ݵ�ַ 
        pDst:Ŀ�����ݵ�ַ�����Ŀ�����ݵ�ַΪ_NULL,��ʵ�ʱ��룬��Ҫ������������ĳ��� 
        pInfo ����������Ϣ
        EnChoice:�Ƿ������������
        
Output: pDst:Ŀ�����ݵ�ַ,PdstΪNULLʱ����ʾ������������ݡ�	
        pSrcLen Դ���ݳ���
Return: ���볤��		
Others: 
	    	
************************************************"*/	

static INT16U  EnCode_SimpleData(INT8U *pSrc,
                                INT16U *pSrcLen,
                                ST_ENCODE_INFO  * pInfo,
                                INT8U *pDst,
                                INT8U EnChoice)
{
    INT16U RetLen;
    INT8U SquNum;
    ST_ENCODE_INFO * TmppInfo = NULL;
    INT16U TmpLen;
    INT16U TmpSrcLen;
    INT8U i;
    INT8U *TmpDst;

    RetLen = 0;

    if(EnChoice == TRUE)
    {
        if(pDst !=NULL)
        {
            *pDst = (INT8U)pInfo->Type; /*�������ͷ���*/
        }
        RetLen++;
    }

    if(pInfo->Type < date_time) /*���������ݣ�OIǰ����*/
    {
        /*������Ӧ�������ͱ���*/   
        if(pInfo->Len < 128)
        {
            TmpLen = (INT8U)pInfo->Len;
        }
        else/*��Ϣ��>128�ĳ������ݣ���Ϊ�ǿɱ�����ݳ��ȣ���Ҫ�ӽӿڻ�ȡ*/
        {
            TmpLen = DataLenGet((ENUM_DATALEN_ID)pInfo->Len);
        }
        if(pDst==NULL)
        {
            TmpDst = NULL;
        }
        else
        {
            TmpDst = pDst+RetLen;
        }
        RetLen += EnCode_Base(pSrc,pInfo->Type,TmpLen,TmpDst);
        *pSrcLen = GetBaseOrgLen(pInfo->Type,TmpLen);
    }
    else/*��չ���ͱ��� Squence*/
    {
        if(pInfo->Type == date)
        {
            SquNum = ARRAY_SIZE(DateInfo);
            TmppInfo = (ST_ENCODE_INFO * )&DateInfo;
        }
        else if(pInfo->Type == time)
        {
            SquNum = ARRAY_SIZE(TimeInfo);
            TmppInfo = (ST_ENCODE_INFO * )TimeInfo;
        }
        else if(pInfo->Type == date_time_s)
        {
            SquNum = ARRAY_SIZE(DatetimesInfo);
            TmppInfo = (ST_ENCODE_INFO * )DatetimesInfo;
        }
        else if(pInfo->Type == OI_t)
        {
            SquNum = ARRAY_SIZE(OiInfo);
            TmppInfo = (ST_ENCODE_INFO * )OiInfo;
        }
        else if((pInfo->Type == OAD_t) || (pInfo->Type == OMD_t))
        {
            SquNum = ARRAY_SIZE(OamdInfo);
            TmppInfo = (ST_ENCODE_INFO * )OamdInfo;
        }
        else if(pInfo->Type == TI_t)
        {
            SquNum = ARRAY_SIZE(TiInfo);
            TmppInfo = (ST_ENCODE_INFO * )TiInfo;
        }
        else if(pInfo->Type == Scaler_Unit)
        {
            SquNum = ARRAY_SIZE(UnitInfo);
            TmppInfo = (ST_ENCODE_INFO * )UnitInfo;
        }
        else if(pInfo->Type == COMDCB)
        {
            SquNum = ARRAY_SIZE(COMDCBInfo);
            TmppInfo = (ST_ENCODE_INFO * )COMDCBInfo;
        }
        else if(pInfo->Type == MAC)
        {
            SquNum = ARRAY_SIZE(MacInfo);
            TmppInfo = (ST_ENCODE_INFO * )MacInfo;
        }
        else if(pInfo->Type == SID)
        {
            SquNum = ARRAY_SIZE(SIDInfo);
            TmppInfo = (ST_ENCODE_INFO * )SIDInfo;
        }
        else if(pInfo->Type == SID_MAC)
        {
            SquNum = ARRAY_SIZE(SIDMACInfo);
            TmppInfo = (ST_ENCODE_INFO * )SIDMACInfo;
        }
        else
        {
            //not support yet
        }
        
        if(TmppInfo != NULL)
        {   
            if(pDst==NULL)
            {
                TmpDst = NULL;
            }
            else
            {
                TmpDst = pDst+RetLen;
            }
            RetLen += EnCode_BaseSquence(TmppInfo,pSrcLen,SquNum,pSrc,TmpDst);
        }


        if(pInfo->Type == ROAD_t)/*���⴦��*/
        {

            
        }
        else if (pInfo->Type == CSD)/*���⴦��*/
        {
            if(pDst != NULL)
            {
                *(pDst+RetLen) = *pSrc;/*����choice*/
            }
            RetLen++;
            /*����OAD*/
            SquNum = ARRAY_SIZE(OamdInfo);
            TmppInfo = (ST_ENCODE_INFO * )OamdInfo;
            if(pDst==NULL)
            {
                TmpDst = NULL;
            }
            else
            {
                TmpDst = pDst+RetLen;
            }
            RetLen += EnCode_BaseSquence(TmppInfo,&TmpSrcLen,SquNum,pSrc+1,TmpDst);
            *pSrcLen = TmpSrcLen+1;
            
            if(*pSrc == 0)/*Choice ==0 OAD*/
            {
                
            }
            else if(*pSrc == 1)/*Choice ==1 ROAD*/
            {
                if(pDst==NULL)
                {
                    TmpDst = NULL;
                }
                else
                {
                    TmpDst = pDst+RetLen;
                }
                RetLen += EnCode_LenArea(1, TmpDst);/*OAD NUM �̶�Ϊ1 */
                if(pDst==NULL)
                {
                    TmpDst = NULL;
                }
                else
                {
                    TmpDst = pDst+RetLen;
                }
                RetLen += EnCode_BaseSquence(TmppInfo,&TmpSrcLen,SquNum,pSrc+(*pSrcLen),TmpDst);
                *pSrcLen += TmpSrcLen;
            }
        }
        else if (pInfo->Type == RCSD)/*���⴦��*/
        {
            if(pDst==NULL)
            {
                TmpDst = NULL;
            }
            else
            {
                TmpDst = pDst+RetLen;
            }
            RetLen += EnCode_LenArea(pInfo->Len, TmpDst);/*Seq num of CSD*/
            for(i=0;i<pInfo->Len;i++)
            {
                if(pDst !=NULL)
                {
                    *(pDst+RetLen) = CHOICE_OAD;/*OAD*/
                }
                RetLen++;
               
                SquNum = ARRAY_SIZE(OamdInfo);
                TmppInfo = (ST_ENCODE_INFO * )OamdInfo;
                if(pDst==NULL)
                {
                    TmpDst = NULL;
                }
                else
                {
                    TmpDst = pDst+RetLen;
                }
                RetLen += EnCode_BaseSquence(TmppInfo,&TmpSrcLen,SquNum,pSrc,TmpDst);
            }
        }
        
    }

    return RetLen;
}



/*"************************************************			
Function: 	EnCode_Simple	
Description: ����һ��Data��������,������array��struct��������ʽ��Ϣ
Input:  
        pSrc:Դ���ݵ�ַ 
        pDst:Ŀ�����ݵ�ַ�����Ŀ�����ݵ�ַΪ_NULL,��ʵ�ʱ��룬��Ҫ������������ĳ��� 
        pInfo ����������Ϣ
        
Output: pDst:Ŀ�����ݵ�ַ,pDstΪNULLʱ����ʾ������������ݡ�		
Return: ���볤��		
Others: 
	    	
************************************************"*/	
INT16U EnCode_Simple(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *pDst)
{
    INT16U TmpSrcLen;
    INT16U RetLen;
        
    RetLen = EnCode_SimpleData(pSrc,&TmpSrcLen,pInfo,pDst,FALSE);
    return RetLen;
}


/*"************************************************			
Function: 	AXDR_DataLenGetInit	
Description: ע���ϲ㴫�ݵĳ��Ȼ�ȡ���������ڶԲ�ͬ��ENUM_DATALEN_ID���������ĳ��ȡ�
Input:   HookFun  ���뺯��     
         Mod  ����ģ�� Ŀǰ֧��Ϊ���3���� 0, 1 ,2
Output: 
        
Return: ���볤��		
Others: 
	    	
************************************************"*/	
void AXDR_DataLenGetInit(ArrStrLenHook HookFun,INT8U Mod)
{
    if(Mod<SUPPORT_MOD)
    {
        GetDataLenHookFun[Mod] = HookFun;
    }
}


static INT16U DataLenGet(ENUM_DATALEN_ID Id)
{
    INT8U Cnt;
    INT16S TmpLen;
     for(Cnt=0;Cnt<SUPPORT_MOD;Cnt++)
     {
        if(GetDataLenHookFun[Cnt]!= NULL)
        {
            TmpLen = GetDataLenHookFun[Cnt](Id);
            if(TmpLen>0)
            {
                break;
            }
        }
     }

    if(TmpLen< 0)
    {
        TmpLen = 0;
    }
    return TmpLen;
}
/*"************************************************			
Function: 	EnCode_ArrStrHead	
Description: ����һ��ArrStr���͵�ͷ��ֻ����"����(��ѡ)+����"�����ء�

Input:  
        pDst:Ŀ�����ݵ�ַ�����Ŀ�����ݵ�ַΪ_NULL,��ʵ�ʱ��룬��Ҫ������������ĳ��� 
        pInfo ����������Ϣ
        EnChoice:�Ƿ������������
        
Output: pDst:Ŀ�����ݵ�ַ,pDstΪNULLʱ����ʾ������������ݡ�		
Return: ���볤��		
Others: 
	    	
************************************************"*/	
static INT16U  EnCode_ArrStrHead(ST_ENCODE_INFO * pInfo,INT8U *pDst,INT8U EnChoice)
{
    INT16U RetLen;

    RetLen = 0;
    
    if(EnChoice == TRUE)
    {
        if(pDst !=NULL)
        {
            *pDst = (INT8U)pInfo->Type; /*�������ͷ���*/
        }
        RetLen++;
    }
    if(pDst !=NULL)
    {
        if(pInfo->Len < 128)
        {
            *(pDst + RetLen) = (INT8U)pInfo->Len; /*����127�Ͳ�֧����*/
        }
        else/*��Ϣ��>128�ĳ������ݣ���Ϊ�ǿɱ�����ݳ��ȣ���Ҫ�ӽӿڻ�ȡ*/
        {
            *(pDst + RetLen) = (INT8U)DataLenGet((ENUM_DATALEN_ID)pInfo->Len);
        }
    }
    RetLen++;
    
    return RetLen;
}


/*"************************************************			
Function: 	EnCode_ComplexData		
Description: ���븴�����͵�Data����
��:Ŀǰ���֧��3����룬���������Array��struct����Ϊ��һ��
��:��ʱδ����ʹ�õݹ�ʵ��

Input:  pInfo :������Ϣָ��
        pSrc:����Դ��ַ        
        DataNum:����Ϊ0ʱ����ʾ����������Ϣ���ݣ�����Ϊ1~nʱ����ʾ�²��ĳ������
Output: pDst:�����������Ŀ�ĵ�ַ,pDstΪNULLʱ����ʾ������������ݡ�	
Return: �������ݳ���	
Others: 
   
************************************************"*/	
static INT16U EnCode_ComplexData(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum,INT8U EnChoice)
{
    ST_ENCODE_INFO *TmpPInfo;
    INT8U *TmpPSrc;
    INT8U *TmpPDst;
    INT16U TmpSrcLen;

    INT16U EncodeLen;/*���볤��*/
    INT16U TmpLen;
    INT8U i,j,k;

    ST_ENCODE_INFO *pDeep1Info;/*�������ͻ���*/
    ST_ENCODE_INFO *pDeep2Info;/*�������ͻ���*/
    ST_ENCODE_INFO *pDeep3Info;/*�������ͻ���*/

    INT8U Step1Cnt;
    INT8U Step2Cnt;
    INT8U Step3Cnt;

    INT8U* TmpP;

    TmpPInfo = pInfo;
    TmpPSrc = pSrc;
    TmpPDst = pDst;
    EncodeLen = 0;
    TmpLen = 0;
    
    if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))/*��һ��*/
    {
        if(DataNum == 0)/*�������ݶ�����ʱ���ű����һ��*/
        {   
            /*���������͡�����*/
            if(pDst == NULL)
            {
                TmpP = NULL;
            }
            else
            {
                TmpP = TmpPDst;
            }
            TmpLen  = EnCode_ArrStrHead(TmpPInfo,TmpP,EnChoice);
            TmpPDst += TmpLen; /*Ŀ��ָ������ƫ�ƣ�Դ��ַָ�벻��Ҫƫ��*/
            EncodeLen +=  TmpLen;
        }
        
        pDeep1Info = TmpPInfo;/*������һ����Ϣ*/

        if(pDeep1Info->Len<128)        
        {
            Step1Cnt = pDeep1Info->Len;
        }
        else
        {
            Step1Cnt = (INT8U)DataLenGet((ENUM_DATALEN_ID)pDeep1Info->Len);
        }
        for(i = 0;i < Step1Cnt; i++)
        {
            if(pDeep1Info->Type == array)
                TmpPInfo = pDeep1Info +1 ;/*�ָ���һ��ָ����Ϣ*/
            else
                TmpPInfo ++;
                
            /*****************************�ڶ���***************************/
            if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))
            {
                if((DataNum == 0) || (DataNum == (i+1)))/*���ƥ��������*/
                {
                    if(pDst == NULL)
                    {
                        TmpP = NULL;
                    }
                    else
                    {
                        TmpP = TmpPDst;
                    }
                    TmpLen  = EnCode_ArrStrHead(TmpPInfo,TmpP,EnChoice);
                    TmpPDst += TmpLen;/*Ŀ��ָ������ƫ�ƣ�Դ��ַָ�벻��Ҫƫ��*/
                    EncodeLen += TmpLen;
                }
                pDeep2Info = TmpPInfo;/*�����ڶ�����Ϣ*/
                
                if(pDeep2Info->Len<128)        
                {
                    Step2Cnt = pDeep2Info->Len;
                }
                else
                {
                    Step2Cnt = (INT8U)DataLenGet((ENUM_DATALEN_ID)pDeep2Info->Len);
                }
                
                for(j = 0; j < Step2Cnt; j++)
                {
                    if(pDeep2Info->Type == array)
                        TmpPInfo = pDeep2Info +1 ;/*�ָ���2��ָ����Ϣ*/
                    else
                        TmpPInfo ++;
                    /*****************************������***************************/
                    if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))
                    {
                        if((DataNum == 0) || (DataNum == (i+1)))/*���ƥ��������*/
                        {
                            if(pDst == NULL)
                            {
                                TmpP = NULL;
                            }
                            else
                            {
                                TmpP = TmpPDst;
                            }
                            TmpLen  = EnCode_ArrStrHead(TmpPInfo,TmpP,EnChoice);
                            TmpPDst += TmpLen;/*Ŀ��ָ������ƫ�ƣ�Դ��ַָ�벻��Ҫƫ��*/
                            EncodeLen += TmpLen;
                        }
                        pDeep3Info = TmpPInfo;/*������������Ϣ*/
                        if(pDeep3Info->Len<128)        
                        {
                            Step3Cnt = pDeep3Info->Len;
                        }
                        else
                        {
                            Step3Cnt = (INT8U)DataLenGet((ENUM_DATALEN_ID)pDeep3Info->Len);
                        }
                        
                        for(k = 0; k < Step3Cnt; k++)
                        {
                            if(pDeep3Info->Type == array)
                                TmpPInfo = pDeep3Info +1 ;/*�ָ���2��ָ����Ϣ*/
                            else
                                TmpPInfo ++;
                            /*****************************���Ĳ�***************************/
                            if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))
                            {
                                return (0);/*��֧�ֵ�4��*/
                            }
                            /*****************************���������ݱ���***************************/
                            else
                            {
                                if((DataNum == 0) || (DataNum == (i+1)))/*�㼶ƥ��ű���ͼ��㳤��*/
                                {
                                    if(pDst == NULL)
                                    {
                                        TmpP = NULL;
                                    }
                                    else
                                    {
                                        TmpP = TmpPDst;
                                    }
                                    TmpLen  = EnCode_SimpleData(TmpPSrc, &TmpSrcLen,TmpPInfo,TmpP,EnChoice);
                                    TmpPSrc +=  TmpSrcLen;
                                    TmpPDst += TmpLen;/*Ŀ��ָ�����*/
                                    EncodeLen += TmpLen; 
                                }
                            }
                        }
                    }
                     /*****************************�ڶ������ݱ���***************************/
                    else /*ֻ�д˲��ˣ�����*/
                    {           
                        if((DataNum == 0) || (DataNum == (i+1)))/*�㼶ƥ��ű���ͼ��㳤��*/
                        {
                            if(pDst == NULL)
                            {
                                TmpP = NULL;
                            }
                            else
                            {
                                TmpP = TmpPDst;
                            }
                            TmpLen  = EnCode_SimpleData(TmpPSrc,&TmpSrcLen, TmpPInfo,TmpP,EnChoice);
                            TmpPSrc +=  TmpSrcLen;
                            TmpPDst += TmpLen;/*Ŀ��ָ�����*/
                            EncodeLen += TmpLen;
                        }
                        
                    }
                }
            }
            else/*һ�����ݱ������*/
            {
                if((DataNum == 0) || (DataNum == (i+1)))/*�㼶ƥ��ű���ͼ��㳤��*/
                {
                    if(pDst == NULL)
                    {
                        TmpP = NULL;
                    }
                    else
                    {
                        TmpP = TmpPDst;
                    }
                    TmpLen  = EnCode_SimpleData(TmpPSrc,&TmpSrcLen, TmpPInfo,TmpP,EnChoice);
                    TmpPSrc +=  TmpSrcLen;
                    TmpPDst += TmpLen;/*Ŀ��ָ�����*/
                    EncodeLen += TmpLen; 
                }
            }
        }
    }
    else/*�ײ����*/
    {
        /*DataNumû�в㼶��ȫ������*/
         if(pDst == NULL)
        {
            TmpP = NULL;
        }
        else
        {
            TmpP = TmpPDst;
        }
        TmpLen   = EnCode_SimpleData(TmpPSrc, &TmpSrcLen,TmpPInfo,TmpPDst,EnChoice);
        TmpPSrc +=  TmpSrcLen;
        TmpPDst += TmpLen;
        EncodeLen += TmpLen;
    }

    return EncodeLen;
}


/*"************************************************			
Function: 	EnCode_Dada		
Description: ����Data�������ݣ�����ʽ������Ϣ
��:Ŀǰ���֧��3����룬���������Array��struct����Ϊ��һ��

Input:  pInfo :������Ϣָ��
        pSrc:����Դ��ַ
        DataNum:����Ϊ0ʱ����ʾ����������Ϣ���ݣ�����Ϊ1~nʱ����ʾ�²��ĳ������
Output: pDst:�����������Ŀ�ĵ�ַ��Ŀ�ĵ�ַΪ��ʱ��������ʵ�����ݣ��������㳤��	
Return: �������ݳ���	
Others: 
   
************************************************"*/	
INT16U EnCode_Data(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum)
{
    return EnCode_ComplexData(pInfo,pSrc,pDst,DataNum,TRUE);
}


/*"************************************************			
Function: 	EnCode_Option		
Description: �����ѡ��Ϣ

Input:  Flag :TRUE ���� FALSE ������
Output: pDst:�����������Ŀ�ĵ�ַ
Return: �������ݳ���	
Others: 
   
************************************************"*/	
INT8U EnCode_Option(INT8U Flag,INT8U *pDst)
{
    if(Flag == TRUE)
    {
        *pDst = 0x01;                    
    }
    else
    {
        *pDst = 0x00; 
    }
    return 1;
}


#define DECODE_BEGIN 1

/*"************************************************			
Function: 	DeCode_LenArea		
Description: ���볤���򲢷��������������ƫ��
Input:  pSrc :Դ���ݵ�ַָ��
Output: DataOffset:ʵ�����������PSrc��ʼƫ��,�ȳ�������ĳ���	
Return: RetLen,ʵ�����ݳ���	
Others: 
   
************************************************"*/	
INT16U DeCode_LenArea(INT8U *pSrc,INT8U *DataOffset)
{
    INT8U TmpData;
    INT16U RetLen;

    RetLen = 0;
    TmpData = (*pSrc);
    
    if(TmpData < 0x80)/*�޳�����*/
    {
        RetLen = *pSrc;
        *DataOffset= 1;
    }	   
    else if(TmpData == 0x81)/*�����򳤶�Ϊ1*/
    {
        RetLen = *(pSrc+1);
        *DataOffset= 2;
    }
    else if(TmpData== 0x82)/*�����򳤶�Ϊ2*/
    {    	
       RetLen = (*(pSrc+1))<<8;
       RetLen += *(pSrc+2);
       *DataOffset = 3;
    }
    else
    {
        /*not support*/
    }
    return RetLen;
}


/*"************************************************			
Function: 	DeCode_Base_SrcLen		
Description: ���ݴ�����������ͣ�������������ݣ�������ԭʼ���ݳ��ȡ�

Input:  pSrc :Դ���ݵ�ַָ��
        Type:Դ��������
        pSrcLen��ԭʼ���ݳ���ָ��
Output: pDst:�����������Ŀ�ĵ�ַ����pDst=NULLʱ��������������ݡ�	
Return: ��������ݳ���,Ϊ����ʱ����ʾ�������	
Others: 
************************************************"*/	
static INT16S DeCode_Base_SrcLen(INT8U *pSrc,ENUM_DATATYPE Type,INT16U *pSrcLen,INT8U *pDst)
{
    INT16S RetLen;
    INT8U Cnt;
    INT8U TmpOffset;
    
    RetLen = 0;
    /*�̶����ȱ��룬��Ҫ���Ǵ�С�˵Ĳ���*/
    if( 
        (doublelong == Type) ||
        (doublelongunsigned == Type)||
        (long_t == Type ) ||
        (longunsigned == Type)||
        (long64 == Type)||
        (long64unsigned == Type)
        )
    {
        RetLen = GetBaseOrgLen(Type,NULL);
        if(pDst != NULL)
        {
            #ifdef BIG_ENDIAN
            LIB_MemCpy(pSrc, pDst, (INT8U)RetLen);
            #else
            LIB_Reverse(pSrc, pDst, (INT8U)RetLen);
            #endif
        }
        *pSrcLen = (INT16U)RetLen;
    }
    /*�̶����ȱ��룬����Ҫ���Ǵ�С�˲���*/
    else if(
        (integer == Type)||
        (unsigned_t == Type)||
        (enum_t == Type)||
        (bool_t == Type)
        )
    {
        if(pDst != NULL)
        {
            *pDst = *pSrc;
        }
        RetLen = 1;
        *pSrcLen = 1;
    }
    else if ((octetstring == Type)||  /*��Ҫ�೤��ͷ���������*/
        (visiblestring	== Type)||
        (UTF8string == Type) )
    {

        RetLen = DeCode_LenArea(pSrc,&TmpOffset);
        if(pDst != NULL)
        {
            LIB_MemCpy(pSrc+TmpOffset, pDst, (INT16U)RetLen);
        }
        *pSrcLen = RetLen + TmpOffset;
    }
    else if(bitstring == Type) /*bitstring���ͱ���*/
    {
        RetLen = DeCode_LenArea(pSrc,&TmpOffset);
        
        RetLen = GetBaseOrgLen(Type,RetLen);

        *pSrcLen = RetLen + TmpOffset;

        if(pDst != NULL)
        {
            #ifdef BIG_ENDIAN
            LIB_Reverse(pSrc+TmpOffset, pDst, (INT8U)RetLen);
            #else
            LIB_MemCpy(pSrc+TmpOffset, pDst, (INT8U)RetLen);
            #endif
            for(Cnt = 0;Cnt < (INT8U)RetLen;Cnt++)
            {
                BitRevers(pDst + Cnt);
            }
        }
    }
    else if(null_data == Type)
    {
        RetLen = 0;
        *pSrcLen = 0;
    }
    else
    {
        RetLen = DECODE_NO_SUPPORT;/*not support*/
    }

    return RetLen;
}


/*"************************************************			
Function: 	DeCode_Base		
Description: ���ݴ�����������ͣ��������������

Input:  pSrc :Դ���ݵ�ַָ��
        Type:Դ��������
Output: pDst:�����������Ŀ�ĵ�ַ����pDst=NULLʱ��������������ݡ�	
Return: ��������ݳ���,Ϊ����ʱ����ʾ�������	
Others: 
************************************************"*/	
INT16S DeCode_Base(INT8U *pSrc,ENUM_DATATYPE Type,INT8U *pDst)
{
    INT16U TmpSrcLen;
    return DeCode_Base_SrcLen(pSrc,Type,&TmpSrcLen,pDst);
}
/*"************************************************			
Function: 	DeCode_BaseSquence		
Description: ����һ������

Input:  pInfo :�����Ľ�����������
        SquNum : squence ����
        pSrc:�������ݵ�ַָ��
Output: pDst:�����������Ŀ�ĵ�ַ,��pDst=NULLʱ��������������ݡ�	
        pSrcLen:ԭʼ���ݳ���ָ��
Return: ��������ݳ���	
Others: 
************************************************"*/	
INT16S  DeCode_BaseSquence(ST_ENCODE_INFO * pInfo,
                                    INT16U *pSrcLen,
                                    INT8U SquNum,
                                    INT8U *pSrc,
                                    INT8U *pDst)
{
    INT8U Cnt;
    INT8U *TmppSrc;
    INT8U *TmppDst;
    INT16S TmpLen;
    INT16U TmpSrcLen;
    INT16U RetSrcLen;
    INT16U RetLen;

    TmppSrc = pSrc; 
    TmppDst = pDst;
    RetLen = 0;
    TmpSrcLen = 0;
    RetSrcLen = 0;
    
    for(Cnt=0; Cnt < SquNum; Cnt++)
    {
        if(pDst == NULL)
        {
            TmppDst = NULL;
        }
        else
        {

        }
        TmpLen  = DeCode_Base_SrcLen(TmppSrc,pInfo[Cnt].Type, &TmpSrcLen,TmppDst);
        if(TmpLen <0)
        {
            return TmpLen;
        }
        else
        {
            TmppSrc += TmpSrcLen;
            RetSrcLen+= TmpSrcLen;
            TmppDst += TmpLen;
            RetLen += TmpLen;
        }
    }
    *pSrcLen = RetSrcLen;
    return RetLen;
}


/*"************************************************			
Function: 	DeCode_SimpleData		
Description: ����򵥵�һ������(�Ƿ���choice��Ϣ��ѡ)

Input:  pSrc :Դ���ݵ�ַָ��
        pInfo:Դ�������ͣ��������͡���������������ΪNULLʱ�����ж�ԭʼ��ʽ�Ƿ���ȷ��
        DeChoice:���������Ƿ���choice����
Output: pDst:�������������Ŀ�ĵ�ַ����pDst=NULLʱ��������������ݡ�	
        PSrcLen 
Return: ����������ݳ���,����������ݳ���<0,��ʾ����ʧ�ܣ�����Ԥ�ڵĽ��벻����
        -1 :��ʽ��ƥ��
        -2 :���Ȳ�ƥ��
Others: 
************************************************"*/	
INT16S DeCode_SimpleData(INT8U *pSrc,
                                INT16U *pSrcLen,
                                ST_ENCODE_INFO * pInfo,
                                INT8U *pDst,
                                INT8U DeChoice)
{
    INT16S RetLen;
    INT8U *TmppSrc;
    INT16U Tmp;
    INT8U SquNum;
    ST_ENCODE_INFO *TmppInfo;
    INT16U TmpSrcLen;
    INT8U * PTmpDst;
    ENUM_DATATYPE TmpType;

    TmppInfo = pInfo;
    TmppSrc = pSrc;
    RetLen = 0;
    TmpSrcLen = 0;

    
    if(DeChoice == TRUE)/*�������������data��choice��Ϣ*/
    {
        TmpType = (ENUM_DATATYPE)(*TmppSrc);

        if(TmppInfo != NULL) 
        {
            if(TmppInfo->Type != TmpType)/**/
            {
                /*��������������Ŀ�������Ϣ��һ��*/
                return DECODE_ERR_TYPE;
            }
        }
        else
        {

        }
        TmppSrc++;/*����ָ������ƫ��+1*/
        TmpSrcLen ++;
    }
    else
    {
        if(TmppInfo == NULL)/*û��choice�ĸ�ʽ��Ϣ�������Ҫ���ڴ���Ϣ*/
        {
            return DECODE_ERR_TYPE;
        }
        TmpType = TmppInfo->Type;
    }
    
    if(TmpType < date_time) /*���������ݣ�OIǰ����*/
    {
        /*������Ӧ�������ͱ���*/   
        if(pDst == NULL)
        {
            PTmpDst = NULL;
        }
        else
        {
            PTmpDst = pDst+RetLen;
        }
        RetLen += DeCode_Base_SrcLen(TmppSrc,TmpType,&Tmp,PTmpDst);
        TmpSrcLen += Tmp; 
    }
    else/*��չ���ͱ��� Squence*/
    {
        TmppInfo = NULL;
        if(TmpType == date)
        {
            SquNum = ARRAY_SIZE(DateInfo);
            TmppInfo = (ST_ENCODE_INFO * )DateInfo;
        }
        else if(TmpType == time)
        {
            SquNum = ARRAY_SIZE(TimeInfo);
            TmppInfo = (ST_ENCODE_INFO * )TimeInfo;
        }
        else if(TmpType == date_time_s)
        {
            SquNum = ARRAY_SIZE(DatetimesInfo);
            TmppInfo = (ST_ENCODE_INFO * )DatetimesInfo;
        }
        else if(TmpType == OI_t)
        {
            SquNum = ARRAY_SIZE(OiInfo);
            TmppInfo = (ST_ENCODE_INFO * )OiInfo;
        }
        else if((TmpType == OAD_t) || (TmpType == OMD_t))
        {
            SquNum = ARRAY_SIZE(OamdInfo);
            TmppInfo = (ST_ENCODE_INFO * )OamdInfo;
        }
        else if(TmpType == TI_t)
        {
            SquNum = ARRAY_SIZE(TiInfo);
            TmppInfo = (ST_ENCODE_INFO * )TiInfo;
        }
        else if(TmpType == Scaler_Unit)
        {
            SquNum = ARRAY_SIZE(UnitInfo);
            TmppInfo = (ST_ENCODE_INFO * )UnitInfo;
        }
        else if(TmpType == COMDCB)
        {
            SquNum = ARRAY_SIZE(COMDCBInfo);
            TmppInfo = (ST_ENCODE_INFO * )COMDCBInfo;
        }
        else if(TmpType == MAC)
        {
            SquNum = ARRAY_SIZE(MacInfo);
            TmppInfo = (ST_ENCODE_INFO * )MacInfo;
        }
        else if(TmpType == SID_MAC)
        {
            SquNum = ARRAY_SIZE(SIDMACInfo);
            TmppInfo = (ST_ENCODE_INFO * )SIDMACInfo;
        }
        else if(TmpType == SID)
        {
            SquNum = ARRAY_SIZE(SIDInfo);
            TmppInfo = (ST_ENCODE_INFO * )SIDInfo;
        }
        else if(TmpType == RN)
        {
            SquNum = ARRAY_SIZE(RNInfo);
            TmppInfo = (ST_ENCODE_INFO * )RNInfo;
        }
        else
        {
            //not support yet
            //return DECODE_ERR_TYPE;
        }
        
        if(TmppInfo != NULL)
        {   
            if(pDst == NULL)
            {
                PTmpDst = NULL;
            }
            else
            {
                PTmpDst = pDst+RetLen;
            }
            RetLen += DeCode_BaseSquence(TmppInfo,&Tmp,SquNum,TmppSrc,PTmpDst);
            TmpSrcLen += Tmp; 
        }

        if(TmpType == CSD)
        {
            INT8U ChoiceType;
            ChoiceType = *TmppSrc;
            /*����choice*/
            if(pDst != NULL)
            {
               *(pDst+RetLen)= ChoiceType;
            }
            else
            {
            }
            RetLen++; 
            TmppSrc++;/*ָ�뵽OAD or ROAD*/
            TmpSrcLen++;
            
            if(ChoiceType== CHOICE_ROAD)
            {
                SquNum = ARRAY_SIZE(OamdInfo);
                TmppInfo = (ST_ENCODE_INFO * )OamdInfo;
                if(pDst == NULL)
                {
                    PTmpDst = NULL;
                }
                else
                {
                    PTmpDst = pDst+RetLen;
                }
                RetLen += DeCode_BaseSquence(TmppInfo,&Tmp,SquNum,TmppSrc,PTmpDst);
                TmpSrcLen += Tmp; 
                TmppSrc += Tmp; 
                
                /*SQUENCE OF OAD*/
                /*����Squence NUM*/
                if(pDst != NULL)
                {
                    if(*TmppSrc>1)/*��֧��>1�����*/
                    {
                        return DECODE_ERR_TYPE;
                    }
                   *(pDst+RetLen)= *TmppSrc;
                }
                else
                {
                }
                RetLen++; 
                TmppSrc++;/*ָ�뵽OAD or ROAD*/
                TmpSrcLen++;

                if(pDst == NULL)
                {
                    PTmpDst = NULL;
                }
                else
                {
                    PTmpDst = pDst+RetLen;
                }
                /*ĿǰROAD ֻ��һ��OAD*/
                RetLen += DeCode_BaseSquence(TmppInfo,&Tmp,SquNum,TmppSrc,PTmpDst);
                TmpSrcLen += Tmp; 
                TmppSrc += Tmp; 

            }
            else if(ChoiceType == CHOICE_OAD)
            {
                SquNum = ARRAY_SIZE(OamdInfo);
                TmppInfo = (ST_ENCODE_INFO * )OamdInfo;
                if(pDst == NULL)
                {
                    PTmpDst = NULL;
                }
                else
                {
                    PTmpDst = pDst+RetLen;
                }
                RetLen += DeCode_BaseSquence(TmppInfo,&Tmp,SquNum,TmppSrc,PTmpDst);
                TmpSrcLen += Tmp; 
                TmppSrc += Tmp; 

            }
            else
            {

            }
        }
        else
        {
            if(TmppInfo == NULL)
            {
                return DECODE_ERR_TYPE;
            }
        }
        
    }

    if(pSrcLen != NULL)
    {
        *pSrcLen = TmpSrcLen;
    }
    
    return RetLen;

}


/*"************************************************			
Function: 	DeCode_Simple		
Description: ���ݴ����������Ϣ������һ��Data���ݣ�������array��struct����������ʽ��Ϣ

Input:  pSrc :Դ���ݵ�ַָ��
        pInfo:Դ�������ͣ���������(���ڲ�����ʽ��Ϣ�������������ĸ�ʽ)��
Output: pDst:�������������Ŀ�ĵ�ַ����pDst=NULLʱ��������������ݡ�	
Return: ����������ݳ���,����������ݳ���<0,��ʾ����ʧ�ܣ�����Ԥ�ڵĽ��벻����

Others: 
************************************************"*/	
INT16S DeCode_Simple(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *pDst)

{
    INT16U SrcLen;
    return DeCode_SimpleData(pSrc,&SrcLen,pInfo,pDst,FALSE);
}

/*"************************************************			
Function: 	DeCode_ArrStrHead	
Description: ����һ��ArrStr���͵�ͷ�����ж���������ʱ��һ�¡�
             ARRAY�����ж��������ͣ����ж����ȡ�STRUCT ���������ͺͳ���

Input:  pSrc:Դ���ݵ�ַ 
        pInfo �����Ľ���������Ϣ�����ΪNULL,���ж��������ͺͳ���
Output: Offset:��������ƫ�Ƶĳ���
Return: Arr_ STR�ĸ���	����������	DECODE_ERR_TYPE (-1) DECODE_ERR_LEN  (-2)
Others: 
	    	
************************************************"*/	
INT16S  DeCode_ArrStrHead(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *Offset)
{
    INT8U * TmppSrc;
    INT16S RetLen;
    ENUM_DATATYPE SrcType;

    TmppSrc = pSrc;
    SrcType = (ENUM_DATATYPE)(*TmppSrc);

    if(pInfo != NULL)
    {
        if(pInfo->Type != SrcType)
        {
            /*��������������Ŀ�������Ϣ��һ��*/
            return DECODE_ERR_TYPE;
        }
    }
    else/*Ϊ���򲻱Ƚ�*/
    {
    
    }

    TmppSrc++;/*����ָ������ƫ��+1*/

    if((SrcType == array) || (SrcType == structure))
    {
        /*���Ƚ��볤����Ϣ*/
        RetLen = DeCode_LenArea(TmppSrc,Offset);
        if(pInfo != NULL)
        {
            if(pInfo->Type == structure)/*struct �ж�����*/
            {
                if(pInfo->Len != RetLen)
                {
                    return DECODE_ERR_LEN;
                }
            }
            else/*arr�� �ж�����*/
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

    return RetLen;
}

/*"************************************************			
Function: 	Cal_EnInfoByDataNum		
Description: ͨ��DataNum�����ʼ��������ʽ��ʼλ��
Input:  
    pInfo :�����������ݸ�ʽָ��
    DataNum:���ݲ㼶
Output: OutPInfo:ָ���������ָ����ʼָ��� ָ��
Return: �Ƿ�ɹ�
Others: 
   
************************************************"*/	
INT8U Cal_EnInfoByDataNum(ST_ENCODE_INFO * pInfo ,INT8U DataNum,ST_ENCODE_INFO ** OutPInfo)
{
    INT8U i,j,k;

    ST_ENCODE_INFO *TmpPInfo;

    INT8U Deep1Num;
    INT8U Deep2Num;
    INT8U Deep3Num;

    TmpPInfo = pInfo;
    
    /*����DataNum �ҵ���Ӧ�Ľ������ʽ��ʼλ��*/
    if(DataNum == 0)/*ȫ����*/
    {
        /*����ƫ����*/
    }
    else
    {   
        if((TmpPInfo->Type == array) || (TmpPInfo->Type == structure))
        {
            if(DataNum > TmpPInfo->Len )
            {
                return FALSE;
            }
            
            if(TmpPInfo->Type == array)/*array������һ����ϢΪ�̶���*/
            {
                TmpPInfo ++;
                *OutPInfo = TmpPInfo;
                return TRUE;
            }
            else/*struct*/
            {
                Deep1Num = TmpPInfo->Len;
                /*��һ��*/
                for(i=0;i<Deep1Num;i++)
                {
                    TmpPInfo ++;
                    if((i+1)==DataNum)/* ƥ��*/
                    {
                        break;
                    }
                    else
                    {
                    
                        if((TmpPInfo->Type == array) || (TmpPInfo->Type == structure))
                        {
                            if(TmpPInfo->Type == array)
                            {
                                Deep2Num = 1;
                            }
                            else
                            {
                                Deep2Num = TmpPInfo->Len;
                            }
                            /*�ڶ���*/
                            for(j=0;j<Deep2Num;j++)
                            {
                                TmpPInfo ++;
                                if((TmpPInfo->Type == array) || (TmpPInfo->Type == structure))
                                {
                                    if(TmpPInfo->Type == array)
                                    {
                                        Deep3Num = 1;
                                    }
                                    else
                                    {
                                        Deep3Num = TmpPInfo->Len;
                                    }
                                    for(k=0;k<Deep3Num;k++)
                                    {
                                        TmpPInfo ++;
                                        if((TmpPInfo->Type == array) || (TmpPInfo->Type == structure))
                                        {
                                            /*no support*/
                                            return FALSE;

                                        }
                                        else
                                        {

                                        }
                                    }
                                }

                            }
                        }
                        else
                        {

                        }
                    }
                                
                }
            }
        }
        else/*û�д˲�*/
        {
            return FALSE ;

        }
    }
    
    *OutPInfo = TmpPInfo;
    
    return TRUE;
}


/*"************************************************			
Function: 	DeCode_ComplexData		
Description: ���븴�����ݸ�ʽ
Input:  
        pInfo :�����Ľ������ݸ�ʽ����pInfoΪNULLʱ����ʾ���ж�������ʽ��
        pSrc : ����ԭʼ���ݵ�ַָ��
        
        DataNum:���ݲ㼶��00Ϊȫ�� 01~NΪ�²�ĳ����
        DeChoice:�Ƿ����choice��Ϣ
Output: 
        pDst : ��������Ŀ�ĵ�ַ����pDstΪNULLʱ����ʾ������������ݡ�
        pSrcLen:�����ԭʼ���ݳ���ָ�룬pSrcLen ΪNULL,��ʾ�������
Return: 
    >0 ��ȷ RetLen,���ȣ�
    <0 ���� define DECODE_ERR_TYPE (-1)
       #define DECODE_ERR_LEN  (-2)
       #define DECODE_NO_SUPPORT  (-3)
Others: 
   
************************************************"*/	
static INT16S DeCode_ComplexData(ST_ENCODE_INFO * pInfo,
                                  INT8U *pSrc,
                                  INT8U *pDst,
                                  INT8U DataNum,
                                  INT8U DeChoice,
                                  INT16U *pSrcLen)
{
    ST_ENCODE_INFO *TmpPInfo;
    INT8U *TmpPSrc;
    INT8U *TmpPDst;
    INT8U TmpOffset;
    INT16S TmpLen;
    INT16S RetLen;
    INT16U TmpSrcLen;
    INT16U i,j,k;

    ST_ENCODE_INFO *pDeep1Info;/*�������ͻ���*/
    ST_ENCODE_INFO *pDeep2Info;/*�������ͻ���*/
    ST_ENCODE_INFO *pDeep3Info;/*�������ͻ���*/

    INT16U Deep1Num;
    INT16U Deep2Num;
    INT16U Deep3Num;

    TmpPSrc = pSrc;
    TmpPDst = pDst;
    TmpLen = 0;
    RetLen = 0;
    TmpSrcLen = 0;
    TmpPInfo = pInfo;

    if(pInfo != NULL)
    {
        Cal_EnInfoByDataNum(pInfo ,DataNum,&TmpPInfo);
    }
       
    if((*TmpPSrc == array)||(*TmpPSrc == structure))
    {
        TmpLen = DeCode_ArrStrHead(TmpPSrc,TmpPInfo,&TmpOffset);
        if(TmpLen<0)/*��ʽ��Ϣ��ƥ�䷵�أ����жϸ���*/
        {
            return TmpLen;
        }
        
        Deep1Num = TmpLen;/*���������ĸ���*/
        pDeep1Info = TmpPInfo;/*���������ĸ�ʽ��Ϣ*/

        TmpPSrc += (TmpOffset+1);/*������Դ��ַ����ƫ�Ʊ���������򳤶�*/
        
        for(i=0; i<Deep1Num; i++) /*��һ��*/
        {
            if(pInfo != NULL)
            {
                if(pDeep1Info->Type == array)
                {
                    TmpPInfo = pDeep1Info +1 ;/*�ָ���һ��ָ����Ϣ*/
                }
                else
                {
                    TmpPInfo ++;
                }
            }
            
            if((*TmpPSrc == array)||(*TmpPSrc == structure))
            {
                    TmpLen = DeCode_ArrStrHead(TmpPSrc,TmpPInfo,&TmpOffset);
                    if(TmpLen < 0)/*��ƥ�䷵��*/
                    {
                        return TmpLen;
                    }
                    TmpPSrc += (TmpOffset+1);/*����Դ��ַ����ƫ�Ʊ���������򳤶�*/

                    Deep2Num = TmpLen;      
                    pDeep2Info = TmpPInfo;/*�����ڶ�����Ϣָ��*/
                    for(j=0; j<Deep2Num; j++) /*�ڶ���*/
                    {
                        if(pInfo != NULL)
                        {
                            if(pDeep2Info->Type == array)
                            {
                                TmpPInfo = pDeep2Info +1 ;/*�ָ��ڶ���ָ����Ϣ*/
                            }
                            else
                            {
                                TmpPInfo ++;
                            }
                        }
                        if((*TmpPSrc == array)||(*TmpPSrc == structure))
                        {
                        
                            TmpLen = DeCode_ArrStrHead(TmpPSrc,TmpPInfo,&TmpOffset);
                            if(TmpLen < 0)/*��ƥ�䷵��*/
                            {
                                return TmpLen;
                            }
                            TmpPSrc += (TmpOffset+1);/*����Դ��ַ����ƫ�Ʊ���������򳤶�*/
                            Deep3Num = TmpLen;
                        
                            pDeep3Info = TmpPInfo;/*������������Ϣָ��*/
                            for(k=0; k<Deep3Num; k++) /*������*/
                            {
                                if(pInfo != NULL)
                                {
                                    if(pDeep3Info->Type == array)
                                    {
                                        TmpPInfo = pDeep3Info +1 ;/*�ָ�������ָ����Ϣ*/
                                    }
                                    else
                                    {
                                        TmpPInfo ++;
                                    }
                                }
                                if((*TmpPSrc == array)||(*TmpPSrc == structure))/*��4�㲻֧��*/
                                {
                                       return DECODE_NO_SUPPORT; /*not support*/                      
                                }
                                else
                                {
                                        if(pDst == NULL)
                                        {
                                            TmpPDst = NULL;
                                        }
                                        else
                                        {

                                        }
                                        TmpLen = DeCode_SimpleData(TmpPSrc,&TmpSrcLen,TmpPInfo,TmpPDst,DeChoice);
                                        if(TmpLen < 0)/*��ƥ�䷵��*/
                                        {
                                            return TmpLen;
                                        }
                                        TmpPSrc += TmpSrcLen;
                                        RetLen += TmpLen;
                                        TmpPDst+=TmpLen;
                                }
                            }
                        }
                        else
                        {
                            if(pDst == NULL)
                            {
                                TmpPDst = NULL;
                            }
                            else
                            {

                            }
                            TmpLen = DeCode_SimpleData(TmpPSrc,&TmpSrcLen,TmpPInfo,TmpPDst,DeChoice);
                            if(TmpLen < 0)/*��ƥ�䷵��*/
                            {
                                return TmpLen;
                            }
                            TmpPSrc += TmpSrcLen;
                            RetLen += TmpLen;
                            TmpPDst+=TmpLen;
                        }
                    }
                }
                else
                {
                    if(pDst == NULL)
                    {
                        TmpPDst = NULL;
                    }
                    else
                    {

                    }
                    TmpLen = DeCode_SimpleData(TmpPSrc,&TmpSrcLen,TmpPInfo,TmpPDst,DeChoice);
                    if(TmpLen < 0)/*��ƥ�䷵��*/
                    {
                        return TmpLen;
                    }
                    TmpPSrc += TmpSrcLen;
                    RetLen += TmpLen;
                    TmpPDst+=TmpLen;
                }
            }
        }
        else /*�����������*/
        {
            if(pDst == NULL)
            {
                TmpPDst = NULL;
            }
            else
            {

            }
            RetLen = DeCode_SimpleData(TmpPSrc,&TmpSrcLen,TmpPInfo,TmpPDst,DeChoice);
            TmpPSrc += TmpSrcLen;
        }

        if(pSrcLen!= NULL)
        {
            *pSrcLen =  (TmpPSrc - pSrc);
        }
        return RetLen;
  
}


/*"************************************************			
Function: 	DeCode_Data		
Description: ����Data�������ݣ�����ʽ������Ϣ��Ŀǰ���֧��3����룬Array��struct����Ϊ��һ�㡣
Input:  
        pInfo :�����Ľ������ݸ�ʽ����pInfoΪNULLʱ����ʾ���ж�������ʽ��
        pSrc : ����ԭʼ���ݵ�ַָ��
        DataNum:Ϊ0ʱ����ʾ����������Ϣ���ݣ�Ϊ1~nʱ����ʾ��һ��ĵ�N�����ݵĽ���
Output: 
        pDst : ��������Ŀ�ĵ�ַ����pDstΪNULLʱ����ʾ������������ݡ�
Return: 
    >0 ��ȷ RetLen,���ȣ�
    <0 ���� define DECODE_ERR_TYPE (-1)
       #define DECODE_ERR_LEN  (-2)
       #define DECODE_NO_SUPPORT  (-3)
Others: 
   
************************************************"*/	

INT16S DeCode_Data(ST_ENCODE_INFO * pInfo,
                                  INT8U *pSrc,
                                  INT8U *pDst,
                                  INT8U DataNum)

{
    return DeCode_ComplexData(pInfo,pSrc,pDst,DataNum,TRUE,NULL);
}



/*"************************************************			
Function: 	DeCode_Data_Len		
Description: ��ȡ���������ݼ�ԭʼ���ݳ��ȡ�
Input:  
    pInfo :�����Ľ������ݸ�ʽ�����PInfoΪNULL������Ϊ���ж��������ݸ�ʽ��
    pSrc : ����ԭʼ����ָ��
    DataNum:Ϊ0ʱ����ʾ����������Ϣ���ȣ�Ϊ1~nʱ����ʾ��һ��ĵ�N�����ݵĳ���
Output: pSrcLenԭʼ���ݳ���ָ��,ΪNULL��ʾ������ԭʼ���ݳ���
Return: >0 ��ȷ RetLen,���ȣ�
       <0 ���� #define DECODE_ERR_TYPE (-1)
               #define DECODE_ERR_LEN  (-2)
               #define DECODE_NO_SUPPORT  (-3)
Others: 
   
************************************************"*/	
INT16S DeCode_Data_Len(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U DataNum,INT16U * pSrcLen)
{

    INT8U DataNumCnt;
    INT16S TmpLen;
    INT8U TmpOffset;
    INT8U * TmpPSrc;
    INT16S RetLen;
    INT16U SrcLen;
    
    TmpPSrc = pSrc;
    RetLen = 0;
        
    if(DataNum == 0)
    {
        return DeCode_ComplexData(pInfo,pSrc,NULL,0,TRUE,pSrcLen);
    }
    else
    {
        TmpLen = DeCode_ArrStrHead(TmpPSrc,pInfo,&TmpOffset);
        if(TmpLen < 0)/*��ƥ�䷵��*/
        {
            return TmpLen;
        }
        TmpPSrc += (TmpOffset+1);/*����Դ��ַ����ƫ�Ʊ���������򳤶�*/
        
        for(DataNumCnt=0;DataNumCnt<DataNum;DataNumCnt++)
        {
            RetLen = DeCode_ComplexData(pInfo,TmpPSrc,NULL,(DataNumCnt+1),TRUE,&SrcLen);
            TmpPSrc += SrcLen;
        }
        if(pSrcLen!= NULL)
        {
            *pSrcLen = TmpPSrc - pSrc;
        }
    }

    return RetLen;
}



/*"************************************************			
Function: 	DeCode_Option		
Description: ���� ��ѡ���ݵ�ͷ
Input:  
    pSrc : ����ԭʼ����ָ��
Output: 
Return: TRUE ���� ;FALSE ������
Others: 
   
************************************************"*/	
INT8U  DeCode_Option(INT8U *pSrc) 
{
    INT8U Ret;

    Ret = FALSE;
    if(*pSrc == 0x01)
    {
        Ret = TRUE; 
    }
    else if(*pSrc == 0x00)
    {
        Ret = FALSE; 
    }
    
    return Ret;

}

