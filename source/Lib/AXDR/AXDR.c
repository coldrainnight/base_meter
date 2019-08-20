/*"***************************************************************************"*/
/*"FileName: AXDR.c		                                "*/
/*"Author: 邹杨                                             "*/                
/*"Version : 1.1       Date: 2017.07.21	                    "*/
/*"Description:   A-XDR编码解码实现                             "*/
/*"Function List:                                           "*/
/*   1、实现了A-XDR编解码,计算编解码长度，实现解码的类型判定"*/
/*"<FunctionName>		                                    "*/    
/*"<author> <time> <version > <desc>		                "*/
/*"************************************************************************"*/

#include "System\Sys_def.h"
#include "Lib\LIB_func.h"
#include "AXDR.h"




/*OAD\OMD编码格式*/
const ST_ENCODE_INFO OamdInfo[] = {{longunsigned,2},{unsigned_t,1},{unsigned_t,1}};
const ST_ENCODE_INFO OiInfo[] = {{longunsigned,2},};/*OI编码格式信息*/

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

#define SUPPORT_MOD (2)/*最大支持模块定义*/ 
static ArrStrLenHook GetDataLenHookFun[SUPPORT_MOD]={NULL,NULL};/*获取可变长度钩子函数*/


/******************************编码部分**********************************************/



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



/******************************解码部分**********************************************/
INT16U DeCode_LenArea(INT8U *pSrc,INT8U *DataOffset);



/******************************编码部分**********************************************/

void   AXDR_ArrStrLenGetInit(ArrStrLenHook HookFun,INT8U Mod)
{

    GetDataLenHookFun[Mod] = HookFun;
}
/*"************************************************			
Function: 	GetBaseOrgLen		
Description:计算Data类型数据的编码前数据的长度
Input:  
        Type:数据类型，目前支持Data类型
        Len:对于变长数据，需要传入数据长度，主要是为了
Output: 	
Return:返回 
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
    /*非固定长度部分长度*/
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
Description: 编码长度域
Input:  len：数据长度
Output: pDst:编码数据输出目的地址,当pDst为NULL时，表示不输出编码数据，用于计算长度。	
Return: 编码数据长度	
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

/*反转bit位*/
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
Description: 编码Data类型中基础类数据，详见 ENUM_DATATYPE中基础类型的定义
Input:  pSrc :源数据地址指针
        Type:源数据类型
        SrcLen：源数据长度
Output: pDst:编码数据输出目的地址，当pDst为NULL时，表示不输出编码数据	
Return: 编码后数据长度	
Others: 
************************************************"*/	
INT16U EnCode_Base(INT8U *pSrc,ENUM_DATATYPE Type, INT16U SrcLen,INT8U *pDst)
{
    INT16U RetLen;
    INT16U TmpLen;
    INT8U Cnt;
    
    RetLen = 0;
    /*固定长度编码，需要考虑大小端的部分*/
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
    /*固定长度编码，不需要考虑大小端部分*/
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
    else if ((octetstring == Type)||  /*需要编长度头数据类编码*/
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
    else if(bitstring == Type) /*bitstring类型编码*/
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
Description: 对基础数据组成的Squence进行编码

Input:  pInfo 编码数据信息
        SquNum :squence的长度
        pSrc:源数据地址
        
Output: pSrcLen :源数据长度,用于原始数据往后偏移。
        pDst:编码数据输出目的地址，当pDst为NULL时，不输出编码。	
Return: 编码后数据长度	
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
        /*调用相应基础类型编码*/   
        if(TmpSrcLen < 128)
        {
        }
        else/*信息中>=128的长度数据，认为是可变的数据长度，需要从接口获取*/
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
Description: 编码一个Data类型数据,不包含array和struct
Input:  
        pSrc:源数据地址 
        pDst:目的数据地址，如果目的数据地址为_NULL,则不实际编码，主要用来计算编码后的长度 
        pInfo 编码数据信息
        EnChoice:是否编码数据类型
        
Output: pDst:目的数据地址,Pdst为NULL时，表示不输出编码数据。	
        pSrcLen 源数据长度
Return: 编码长度		
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
            *pDst = (INT8U)pInfo->Type; /*编码类型返回*/
        }
        RetLen++;
    }

    if(pInfo->Type < date_time) /*基础类数据，OI前数据*/
    {
        /*调用相应基础类型编码*/   
        if(pInfo->Len < 128)
        {
            TmpLen = (INT8U)pInfo->Len;
        }
        else/*信息中>128的长度数据，认为是可变的数据长度，需要从接口获取*/
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
    else/*扩展类型编码 Squence*/
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


        if(pInfo->Type == ROAD_t)/*特殊处理*/
        {

            
        }
        else if (pInfo->Type == CSD)/*特殊处理*/
        {
            if(pDst != NULL)
            {
                *(pDst+RetLen) = *pSrc;/*编码choice*/
            }
            RetLen++;
            /*编码OAD*/
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
                RetLen += EnCode_LenArea(1, TmpDst);/*OAD NUM 固定为1 */
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
        else if (pInfo->Type == RCSD)/*特殊处理*/
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
Description: 编码一个Data类型数据,不包含array和struct，不含格式信息
Input:  
        pSrc:源数据地址 
        pDst:目的数据地址，如果目的数据地址为_NULL,则不实际编码，主要用来计算编码后的长度 
        pInfo 编码数据信息
        
Output: pDst:目的数据地址,pDst为NULL时，表示不输出编码数据。		
Return: 编码长度		
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
Description: 注册上层传递的长度获取函数。用于对不同的ENUM_DATALEN_ID计算出具体的长度。
Input:   HookFun  传入函数     
         Mod  传入模块 目前支持为最大3个， 0, 1 ,2
Output: 
        
Return: 编码长度		
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
Description: 编码一个ArrStr类型的头，只编码"类型(可选)+长度"并返回。

Input:  
        pDst:目的数据地址，如果目的数据地址为_NULL,则不实际编码，主要用来计算编码后的长度 
        pInfo 编码数据信息
        EnChoice:是否编码数据类型
        
Output: pDst:目的数据地址,pDst为NULL时，表示不输出编码数据。		
Return: 编码长度		
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
            *pDst = (INT8U)pInfo->Type; /*编码类型返回*/
        }
        RetLen++;
    }
    if(pDst !=NULL)
    {
        if(pInfo->Len < 128)
        {
            *(pDst + RetLen) = (INT8U)pInfo->Len; /*超过127就不支持了*/
        }
        else/*信息中>128的长度数据，认为是可变的数据长度，需要从接口获取*/
        {
            *(pDst + RetLen) = (INT8U)DataLenGet((ENUM_DATALEN_ID)pInfo->Len);
        }
    }
    RetLen++;
    
    return RetLen;
}


/*"************************************************			
Function: 	EnCode_ComplexData		
Description: 编码复杂类型的Data数据
①:目前最多支持3层编码，如果是碰到Array或struct就认为是一层
②:暂时未考虑使用递归实现

Input:  pInfo :编码信息指针
        pSrc:编码源地址        
        DataNum:编码为0时，表示整个编码信息数据，编码为1~n时，表示下层的某个数据
Output: pDst:编码数据输出目的地址,pDst为NULL时，表示不输出编码数据。	
Return: 编码数据长度	
Others: 
   
************************************************"*/	
static INT16U EnCode_ComplexData(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum,INT8U EnChoice)
{
    ST_ENCODE_INFO *TmpPInfo;
    INT8U *TmpPSrc;
    INT8U *TmpPDst;
    INT16U TmpSrcLen;

    INT16U EncodeLen;/*编码长度*/
    INT16U TmpLen;
    INT8U i,j,k;

    ST_ENCODE_INFO *pDeep1Info;/*数据类型缓存*/
    ST_ENCODE_INFO *pDeep2Info;/*数据类型缓存*/
    ST_ENCODE_INFO *pDeep3Info;/*数据类型缓存*/

    INT8U Step1Cnt;
    INT8U Step2Cnt;
    INT8U Step3Cnt;

    INT8U* TmpP;

    TmpPInfo = pInfo;
    TmpPSrc = pSrc;
    TmpPDst = pDst;
    EncodeLen = 0;
    TmpLen = 0;
    
    if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))/*第一层*/
    {
        if(DataNum == 0)/*所有数据都编码时，才编码第一层*/
        {   
            /*仅编码类型、长度*/
            if(pDst == NULL)
            {
                TmpP = NULL;
            }
            else
            {
                TmpP = TmpPDst;
            }
            TmpLen  = EnCode_ArrStrHead(TmpPInfo,TmpP,EnChoice);
            TmpPDst += TmpLen; /*目的指针往后偏移，源地址指针不需要偏移*/
            EncodeLen +=  TmpLen;
        }
        
        pDeep1Info = TmpPInfo;/*保留第一层信息*/

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
                TmpPInfo = pDeep1Info +1 ;/*恢复第一层指针信息*/
            else
                TmpPInfo ++;
                
            /*****************************第二层***************************/
            if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))
            {
                if((DataNum == 0) || (DataNum == (i+1)))/*层次匹配才需编码*/
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
                    TmpPDst += TmpLen;/*目的指针往后偏移，源地址指针不需要偏移*/
                    EncodeLen += TmpLen;
                }
                pDeep2Info = TmpPInfo;/*保留第二层信息*/
                
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
                        TmpPInfo = pDeep2Info +1 ;/*恢复第2层指针信息*/
                    else
                        TmpPInfo ++;
                    /*****************************第三层***************************/
                    if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))
                    {
                        if((DataNum == 0) || (DataNum == (i+1)))/*层次匹配才需编码*/
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
                            TmpPDst += TmpLen;/*目的指针往后偏移，源地址指针不需要偏移*/
                            EncodeLen += TmpLen;
                        }
                        pDeep3Info = TmpPInfo;/*保留第三层信息*/
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
                                TmpPInfo = pDeep3Info +1 ;/*恢复第2层指针信息*/
                            else
                                TmpPInfo ++;
                            /*****************************第四层***************************/
                            if((TmpPInfo->Type == array)  || (TmpPInfo->Type == structure))
                            {
                                return (0);/*不支持第4层*/
                            }
                            /*****************************第三层数据编码***************************/
                            else
                            {
                                if((DataNum == 0) || (DataNum == (i+1)))/*层级匹配才编码和计算长度*/
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
                                    TmpPDst += TmpLen;/*目的指针后移*/
                                    EncodeLen += TmpLen; 
                                }
                            }
                        }
                    }
                     /*****************************第二层数据编码***************************/
                    else /*只有此层了，编码*/
                    {           
                        if((DataNum == 0) || (DataNum == (i+1)))/*层级匹配才编码和计算长度*/
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
                            TmpPDst += TmpLen;/*目的指针后移*/
                            EncodeLen += TmpLen;
                        }
                        
                    }
                }
            }
            else/*一层数据编码编码*/
            {
                if((DataNum == 0) || (DataNum == (i+1)))/*层级匹配才编码和计算长度*/
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
                    TmpPDst += TmpLen;/*目的指针后移*/
                    EncodeLen += TmpLen; 
                }
            }
        }
    }
    else/*首层编码*/
    {
        /*DataNum没有层级，全部编码*/
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
Description: 编码Data类型数据，带格式编码信息
①:目前最多支持3层编码，如果是碰到Array或struct就认为是一层

Input:  pInfo :编码信息指针
        pSrc:编码源地址
        DataNum:编码为0时，表示整个编码信息数据，编码为1~n时，表示下层的某个数据
Output: pDst:编码数据输出目的地址，目的地址为空时，不拷贝实际数据，用来计算长度	
Return: 编码数据长度	
Others: 
   
************************************************"*/	
INT16U EnCode_Data(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum)
{
    return EnCode_ComplexData(pInfo,pSrc,pDst,DataNum,TRUE);
}


/*"************************************************			
Function: 	EnCode_Option		
Description: 编码可选信息

Input:  Flag :TRUE 存在 FALSE 不存在
Output: pDst:编码数据输出目的地址
Return: 编码数据长度	
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
Description: 解码长度域并返回数据内容域的偏移
Input:  pSrc :源数据地址指针
Output: DataOffset:实际数据相对于PSrc起始偏移,既长度区域的长度	
Return: RetLen,实际数据长度	
Others: 
   
************************************************"*/	
INT16U DeCode_LenArea(INT8U *pSrc,INT8U *DataOffset)
{
    INT8U TmpData;
    INT16U RetLen;

    RetLen = 0;
    TmpData = (*pSrc);
    
    if(TmpData < 0x80)/*无长度域*/
    {
        RetLen = *pSrc;
        *DataOffset= 1;
    }	   
    else if(TmpData == 0x81)/*长度域长度为1*/
    {
        RetLen = *(pSrc+1);
        *DataOffset= 2;
    }
    else if(TmpData== 0x82)/*长度域长度为2*/
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
Description: 根据传入的数据类型，解码基础类数据，并返回原始数据长度。

Input:  pSrc :源数据地址指针
        Type:源数据类型
        pSrcLen：原始数据长度指针
Output: pDst:编码数据输出目的地址。当pDst=NULL时，不输出解码数据。	
Return: 解码后数据长度,为负数时，表示解码错误	
Others: 
************************************************"*/	
static INT16S DeCode_Base_SrcLen(INT8U *pSrc,ENUM_DATATYPE Type,INT16U *pSrcLen,INT8U *pDst)
{
    INT16S RetLen;
    INT8U Cnt;
    INT8U TmpOffset;
    
    RetLen = 0;
    /*固定长度编码，需要考虑大小端的部分*/
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
    /*固定长度编码，不需要考虑大小端部分*/
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
    else if ((octetstring == Type)||  /*需要编长度头数据类编码*/
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
    else if(bitstring == Type) /*bitstring类型编码*/
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
Description: 根据传入的数据类型，解码基础类数据

Input:  pSrc :源数据地址指针
        Type:源数据类型
Output: pDst:编码数据输出目的地址。当pDst=NULL时，不输出解码数据。	
Return: 解码后数据长度,为负数时，表示解码错误	
Others: 
************************************************"*/	
INT16S DeCode_Base(INT8U *pSrc,ENUM_DATATYPE Type,INT8U *pDst)
{
    INT16U TmpSrcLen;
    return DeCode_Base_SrcLen(pSrc,Type,&TmpSrcLen,pDst);
}
/*"************************************************			
Function: 	DeCode_BaseSquence		
Description: 解码一串数据

Input:  pInfo :期望的解码数据类型
        SquNum : squence 个数
        pSrc:解码数据地址指针
Output: pDst:编码数据输出目的地址,当pDst=NULL时，不输出解码数据。	
        pSrcLen:原始数据长度指针
Return: 解码后数据长度	
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
Description: 解码简单的一个数据(是否有choice信息可选)

Input:  pSrc :源数据地址指针
        pInfo:源数据类型，期望类型。当期望数据类型为NULL时，则不判定原始格式是否正确。
        DeChoice:解码数据是否是choice类型
Output: pDst:解码码数据输出目的地址，当pDst=NULL时，不输出解码数据。	
        PSrcLen 
Return: 解码码后数据长度,如果返回数据长度<0,表示解码失败，既与预期的解码不符合
        -1 :格式不匹配
        -2 :长度不匹配
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

    
    if(DeChoice == TRUE)/*如果解码数据有data的choice信息*/
    {
        TmpType = (ENUM_DATATYPE)(*TmppSrc);

        if(TmppInfo != NULL) 
        {
            if(TmppInfo->Type != TmpType)/**/
            {
                /*解码类型数据与目标解码信息不一致*/
                return DECODE_ERR_TYPE;
            }
        }
        else
        {

        }
        TmppSrc++;/*解码指针往后偏移+1*/
        TmpSrcLen ++;
    }
    else
    {
        if(TmppInfo == NULL)/*没有choice的格式信息，则必须要有期待信息*/
        {
            return DECODE_ERR_TYPE;
        }
        TmpType = TmppInfo->Type;
    }
    
    if(TmpType < date_time) /*基础类数据，OI前数据*/
    {
        /*调用相应基础类型编码*/   
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
    else/*扩展类型编码 Squence*/
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
            /*返回choice*/
            if(pDst != NULL)
            {
               *(pDst+RetLen)= ChoiceType;
            }
            else
            {
            }
            RetLen++; 
            TmppSrc++;/*指针到OAD or ROAD*/
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
                /*返回Squence NUM*/
                if(pDst != NULL)
                {
                    if(*TmppSrc>1)/*不支持>1的情况*/
                    {
                        return DECODE_ERR_TYPE;
                    }
                   *(pDst+RetLen)= *TmppSrc;
                }
                else
                {
                }
                RetLen++; 
                TmppSrc++;/*指针到OAD or ROAD*/
                TmpSrcLen++;

                if(pDst == NULL)
                {
                    PTmpDst = NULL;
                }
                else
                {
                    PTmpDst = pDst+RetLen;
                }
                /*目前ROAD 只带一个OAD*/
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
Description: 根据传入的类型信息，解码一个Data数据（不包含array和struct），不含格式信息

Input:  pSrc :源数据地址指针
        pInfo:源数据类型，期望类型(由于不含格式信息，必须有期望的格式)。
Output: pDst:解码码数据输出目的地址，当pDst=NULL时，不输出解码数据。	
Return: 解码码后数据长度,如果返回数据长度<0,表示解码失败，既与预期的解码不符合

Others: 
************************************************"*/	
INT16S DeCode_Simple(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *pDst)

{
    INT16U SrcLen;
    return DeCode_SimpleData(pSrc,&SrcLen,pInfo,pDst,FALSE);
}

/*"************************************************			
Function: 	DeCode_ArrStrHead	
Description: 解码一个ArrStr类型的头，并判定与期望的时候一致。
             ARRAY类型判断数据类型，不判定长度。STRUCT 判数据类型和长度

Input:  pSrc:源数据地址 
        pInfo 期望的解码数据信息，如果为NULL,则不判定数据类型和长度
Output: Offset:数据往后偏移的长度
Return: Arr_ STR的个数	，其他返回	DECODE_ERR_TYPE (-1) DECODE_ERR_LEN  (-2)
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
            /*解码类型数据与目标解码信息不一致*/
            return DECODE_ERR_TYPE;
        }
    }
    else/*为空则不比较*/
    {
    
    }

    TmppSrc++;/*解码指针往后偏移+1*/

    if((SrcType == array) || (SrcType == structure))
    {
        /*首先解码长度信息*/
        RetLen = DeCode_LenArea(TmppSrc,Offset);
        if(pInfo != NULL)
        {
            if(pInfo->Type == structure)/*struct 判定长度*/
            {
                if(pInfo->Len != RetLen)
                {
                    return DECODE_ERR_LEN;
                }
            }
            else/*arr不 判定长度*/
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
Description: 通过DataNum算出起始的期望格式起始位置
Input:  
    pInfo :解码期望数据格式指针
    DataNum:数据层级
Output: OutPInfo:指向解码数据指针起始指针的 指针
Return: 是否成功
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
    
    /*根据DataNum 找到相应的解码码格式启始位置*/
    if(DataNum == 0)/*全解码*/
    {
        /*不用偏移了*/
    }
    else
    {   
        if((TmpPInfo->Type == array) || (TmpPInfo->Type == structure))
        {
            if(DataNum > TmpPInfo->Len )
            {
                return FALSE;
            }
            
            if(TmpPInfo->Type == array)/*array类型下一层信息为固定的*/
            {
                TmpPInfo ++;
                *OutPInfo = TmpPInfo;
                return TRUE;
            }
            else/*struct*/
            {
                Deep1Num = TmpPInfo->Len;
                /*第一层*/
                for(i=0;i<Deep1Num;i++)
                {
                    TmpPInfo ++;
                    if((i+1)==DataNum)/* 匹配*/
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
                            /*第二层*/
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
        else/*没有此层*/
        {
            return FALSE ;

        }
    }
    
    *OutPInfo = TmpPInfo;
    
    return TRUE;
}


/*"************************************************			
Function: 	DeCode_ComplexData		
Description: 解码复杂数据格式
Input:  
        pInfo :期望的解码数据格式，当pInfo为NULL时，表示不判定期望格式。
        pSrc : 解码原始数据地址指针
        
        DataNum:数据层级，00为全部 01~N为下层某数据
        DeChoice:是否包含choice信息
Output: 
        pDst : 解码数据目的地址，当pDst为NULL时，表示不输出解码数据。
        pSrcLen:解码的原始数据长度指针，pSrcLen 为NULL,表示不输出。
Return: 
    >0 正确 RetLen,长度，
    <0 错误 define DECODE_ERR_TYPE (-1)
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

    ST_ENCODE_INFO *pDeep1Info;/*数据类型缓存*/
    ST_ENCODE_INFO *pDeep2Info;/*数据类型缓存*/
    ST_ENCODE_INFO *pDeep3Info;/*数据类型缓存*/

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
        if(TmpLen<0)/*格式信息不匹配返回，不判断个数*/
        {
            return TmpLen;
        }
        
        Deep1Num = TmpLen;/*保留解码后的个数*/
        pDeep1Info = TmpPInfo;/*保留期望的格式信息*/

        TmpPSrc += (TmpOffset+1);/*解码码源地址往后偏移编码和数据域长度*/
        
        for(i=0; i<Deep1Num; i++) /*第一层*/
        {
            if(pInfo != NULL)
            {
                if(pDeep1Info->Type == array)
                {
                    TmpPInfo = pDeep1Info +1 ;/*恢复第一层指针信息*/
                }
                else
                {
                    TmpPInfo ++;
                }
            }
            
            if((*TmpPSrc == array)||(*TmpPSrc == structure))
            {
                    TmpLen = DeCode_ArrStrHead(TmpPSrc,TmpPInfo,&TmpOffset);
                    if(TmpLen < 0)/*不匹配返回*/
                    {
                        return TmpLen;
                    }
                    TmpPSrc += (TmpOffset+1);/*编码源地址往后偏移编码和数据域长度*/

                    Deep2Num = TmpLen;      
                    pDeep2Info = TmpPInfo;/*保留第二层信息指针*/
                    for(j=0; j<Deep2Num; j++) /*第二层*/
                    {
                        if(pInfo != NULL)
                        {
                            if(pDeep2Info->Type == array)
                            {
                                TmpPInfo = pDeep2Info +1 ;/*恢复第二层指针信息*/
                            }
                            else
                            {
                                TmpPInfo ++;
                            }
                        }
                        if((*TmpPSrc == array)||(*TmpPSrc == structure))
                        {
                        
                            TmpLen = DeCode_ArrStrHead(TmpPSrc,TmpPInfo,&TmpOffset);
                            if(TmpLen < 0)/*不匹配返回*/
                            {
                                return TmpLen;
                            }
                            TmpPSrc += (TmpOffset+1);/*编码源地址往后偏移编码和数据域长度*/
                            Deep3Num = TmpLen;
                        
                            pDeep3Info = TmpPInfo;/*保留第三层信息指针*/
                            for(k=0; k<Deep3Num; k++) /*第三层*/
                            {
                                if(pInfo != NULL)
                                {
                                    if(pDeep3Info->Type == array)
                                    {
                                        TmpPInfo = pDeep3Info +1 ;/*恢复第三层指针信息*/
                                    }
                                    else
                                    {
                                        TmpPInfo ++;
                                    }
                                }
                                if((*TmpPSrc == array)||(*TmpPSrc == structure))/*第4层不支持*/
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
                                        if(TmpLen < 0)/*不匹配返回*/
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
                            if(TmpLen < 0)/*不匹配返回*/
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
                    if(TmpLen < 0)/*不匹配返回*/
                    {
                        return TmpLen;
                    }
                    TmpPSrc += TmpSrcLen;
                    RetLen += TmpLen;
                    TmpPDst+=TmpLen;
                }
            }
        }
        else /*解码基础数据*/
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
Description: 解码Data类型数据，带格式编码信息，目前最多支持3层编码，Array或struct就认为是一层。
Input:  
        pInfo :期望的解码数据格式，当pInfo为NULL时，表示不判定期望格式。
        pSrc : 解码原始数据地址指针
        DataNum:为0时，表示整个解码信息数据，为1~n时，表示下一层的第N个数据的解码
Output: 
        pDst : 解码数据目的地址，当pDst为NULL时，表示不输出解码数据。
Return: 
    >0 正确 RetLen,长度，
    <0 错误 define DECODE_ERR_TYPE (-1)
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
Description: 获取解码后的数据及原始数据长度。
Input:  
    pInfo :期望的解码数据格式，如果PInfo为NULL，则认为不判定期望数据格式。
    pSrc : 解码原始数据指针
    DataNum:为0时，表示整个解码信息长度，为1~n时，表示下一层的第N个数据的长度
Output: pSrcLen原始数据长度指针,为NULL表示不返回原始数据长度
Return: >0 正确 RetLen,长度，
       <0 错误 #define DECODE_ERR_TYPE (-1)
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
        if(TmpLen < 0)/*不匹配返回*/
        {
            return TmpLen;
        }
        TmpPSrc += (TmpOffset+1);/*编码源地址往后偏移编码和数据域长度*/
        
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
Description: 解码 可选数据的头
Input:  
    pSrc : 解码原始数据指针
Output: 
Return: TRUE 存在 ;FALSE 不存在
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

