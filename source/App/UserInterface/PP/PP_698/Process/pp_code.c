/*"*****************************************************************************"*/
/*"  FileName: pp_code.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  与编码交互处理函数文件   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // 历史修改记录"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "App\UserInterface\PP\PP_698\Class\class_comm.h"
#include "app\userinterface\ProtocolStack\PS_698\AL_698.h"
#include "App\LegallyRelevant\SC.h"
#include "App\Message\MS.h"
#include "App\UserInterface\UI.h"

/*"*******************************************************************************"*/
INT8U ComPort = 0;   /*通信端口号*/

INT8U Fix_Len_Varible = 0;   /*变长数据，也采用固定数据: =0xff取固定值*/

INT8U LenSingleData = 14;   /*单次获取的数据长度*/
ST_OAD CurOad;      /*当前处理(包括方法，读，设置)的全局变量OAD*/

/*看后面这个变量是否要做成RECORDLISTNUM个数*/
INT8U gDarDecode = DAR_0_OK;  /*解析出错，如格式错误，或LIST NUM越界 解码出错，优先级最高，打包时优先以这个为准*/
 

/*固定最大，用于显示*/
const INT8U FIX_MAX_PARAMENT_NUM[]={SEASON_SLOT_NUM,DAY_TBL_NUM,DAY_SLOT_NUM,63,SPEC_DAY_NUM};

/*这个函数，后面看是不是写成取变量地址数据即可*/
INT16S LenGet_PP_698(ENUM_DATALEN_ID Id)
{
    INT8U RetLen;
    ENUM_DATALEN_ID Id_temp;
    INT16U Oi;
    
    Oi      = CurOad.Oi.asInt;
    
    Id_temp = Id;
    if(E_TRIFF_ID_PHASE == Id)
    {
        Id_temp = E_TARIFF_ID;
    }   
    
    if(E_PHASE_VARIBLE_ID == Id_temp)   /*分相数据*/
    {        
        RetLen = CLASS3_MAX_IID;
    }    
    else if(E_PHASE_VARIBLE_SUM_ID == Id_temp) /*总及分相数据*/
    {
        RetLen = CLASS4_MAX_IID;
     }    
    else if((E_RELATION_OBJECT_ID == Id_temp)||(E_TRIFF_ID_PHASE == Id_temp)) /*关联对象和费率电价*/
    {
        if((OI_NEW_NEED_RPT_EVT_LIST == Oi)&&(3==CurOad.AId))  /*需上报事件列表数据为OI，两字节*/
        {
            return LenSingleData>>1;
        }
        else
        {    
            return LenSingleData>>2;
        }
    }   
    else if(E_RELATION_FREEZE_OBJECT_ID == Id_temp)
    {
        return LenSingleData>>3;
    }      
    else if(E_BILL_DATE_ID == Id_temp)
    {
        return 3;
    }    
    else
    {   
            return (-1);   /*这里是不是改成0好些?*/
    }

   
    /*加上总的个数*/
    if(E_TARIFF_ID == Id)
    {
        if(TARIFF_NUM<RetLen)  /*取宏和费率参数较小的值*/
        {
            RetLen = TARIFF_NUM;
        }    
        RetLen++;        
    }  
    
    
    return RetLen;
}
 

/*****************************************************************************
 函 数 名  : ConvertOAD
 功能描述  : 接收OAD，将大端模式改成小端模式格式
 输入参数  : INT8U * src  
             INT8U *dst   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 05
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void ConvertOAD(INT8U *src,INT8U *dst)
{
    /*通信是大端模式，CPU是小端模式*/
	*dst=*(src+1);
	*(dst+1)=*src; 
    
	*(dst+2) = *(src+2);
	*(dst+3) = *(src+3);
}

/*****************************************************************************
 函 数 名  : EnCodeOAD
 功能描述  : 编码OAD，并返回编码后的地址
 输入参数  : 无
 输出参数  : 无
 返 回 值  : INT8U* 编码后的地址指针
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 18
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *EnCodeOAD(ST_OAD *poad,INT8U *pdata)
{
    ConvertOAD((INT8U *)poad,pdata);
    
    return (pdata+4);
}   

/*****************************************************************************
 函 数 名  : Judge_Time_Oi
 功能描述  : 判断是否是时间OI：201E,2020,2021
 输入参数  : INT16U OI  
 输出参数  : 无
 返 回 值  : 0:ok, (-1):err
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 05
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8S Judge_Time_Oi(INT16U OI)
{
    if((OI_EVENT_START_TIME == OI)||(OI_EVENT_END_TIME == OI)||(OI_DATA_FREEZE_TIME== OI))
    {
        return 0;
    }
    else
    {
        return (-1);
    }    
}    

/*把传入的2016转化为实际的RTC 的16*/
INT8U TransYear(INT8U * InYear)
{
    INT16U data16;
    
    data16 = *(InYear+1);
    data16<<= 8;
    data16 += *(InYear);
    
    data16 = data16 %100;   /*2017-09-29:这里是不是转成%100好些?*/
    
    return (INT8U)data16;
}

/*读周期性冻结（秒冻结、分钟冻结、日冻结、月冻结、年冻结等）时，数据间隔必须与冻结周期的单位一致，否则返回拒绝读写；*/
/*return:0xff,间隔不一致，0:OK*/
INT8S Check_Freeze_Interval(INT16U OI,INT8U *pTI)
{
	INT8U Unit,Unit_Oi = 0xff;

	switch(OI)
	{
		case 0x5002:
			Unit_Oi = EN_TI_UNIT_MIN;
			break;
		case 0x5003:
			Unit_Oi = EN_TI_UNIT_HOUR;
			break;
	    case 0x5004:
			Unit_Oi = EN_TI_UNIT_DAY;	
			break;
	    case 0x5006:
			Unit_Oi = EN_TI_UNIT_MONTH;	
			break;
		/*	
	    case 0x5007:
			Unit_Oi = EN_TI_UNIT_YEAR;	
			break;*/
		default: /*非周期 性冻结，不判断TI 单位*/
			return 0;   
			break;
	}

	

	Unit = *(pTI);
	
	if(Unit != Unit_Oi)
	{
		return (-1);
	}
	else
	{
		return 0;
	}
	

}

/*****************************************************************************
 函 数 名  : Change_time_meter_time
 功能描述  : 将通信的时间数据，转换成电表筛选的时间格式，年格式要转换
 输入参数  : INT8U *pdata     
             INT8U *pdst      
             INT8U selectnum  
 输出参数  : 无
 返 回 值  : 0:正常，非0:异常，如格式不对，或长度不对等
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 05
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8S Change_time_to_meter_time(INT8U *pdata,INT8U *pdst,INT8U selectnum)
{   
    INT8U temp[10];  /*暂存时间，实际只要7个字节，预留几个字节*/
    
    /*1.先解第一个时间格式，方法1和方法2都有*/
    if(0>DeCode_Data((ST_ENCODE_INFO *)TYPE_DATES,pdata,temp,0))
    {
       return (-1); 
    }    
    
    *pdst = TransYear(temp);
    LIB_MemCpy(&temp[2], &pdst[1], 5);
        
    if(EN_SELECTOR1==selectnum)
    {
        return 0;
    }  
    
    /*2.解第二个时间格式*/
    if(0>DeCode_Data((ST_ENCODE_INFO *)TYPE_DATES,pdata+8,temp,0))
    {
       return (-1); 
    }    
    
    pdst[6] = TransYear(temp);
    LIB_MemCpy(&temp[2], &pdst[7], 5);

    return 0;  
    
}    


/*****************************************************************************
 函 数 名  : DecodeRecordData
 功能描述  : 解析记录型数据内容
 输入参数  : ST_Next_Frame_Record_Data *pFrame_Record_Data  
             INT8U *pAPDU                                   
 输出参数  : 无
 返 回 值  : 0:异常，可以马上中止, >0:正常，长度
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 05
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16U DecodeRecordData(ST_Next_Frame_Record_Data *pFrame_Record_Data, INT8U *pdata)
{
    INT16U OI,Offset = 0,len = 0;
    INT8U i,SelectNumber,num;
    INT8U result = 0;
    INT8U type_ti= 0xff;
    INT8U  First_CSD_AID = 0xff,CSD_AID; 
    INT16U OI_CSD;
    ST_OAD Tmp_oad;
    INT8U *pdataTmp;

    /*1.OAD*/
    ConvertOAD(pdata, (INT8U *)&Tmp_oad);
    OI = Tmp_oad.Oi.asInt;
    
    LIB_MemCpy((INT8U *)&Tmp_oad, (INT8U *)&pFrame_Record_Data->Oad, sizeof(ST_OAD));

    pFrame_Record_Data->Obis_event_freeze = E_OBIS_NULL;
    
    if(0 == PP_OadToObis(&Tmp_oad, &pFrame_Record_Data->Obis_event_freeze, NULL, NULL))
    {
        gDarDecode = DAR_3_NOACESS;
    }    

	pdata += 4;
	Offset += 4;

    SelectNumber = *pdata++;
    Offset++;
    pFrame_Record_Data->SelectNumber = SelectNumber;

    /*2.RSD*/
    switch(SelectNumber)
    {
        case EN_SELECTOR1:    /*选择方法1*/
            /*Selector1∷=SEQUENCE
                {
                  对象属性描述符  OAD，
                  数值            Data
                }*/

            ConvertOAD(pdata, (INT8U *)&pFrame_Record_Data->Oad_Selector);
            pdata  += 4;	
			Offset += 4;  
            
            /*不支持的OAD，也要继续解数据，以便应答*/
            if(0!=Judge_Time_Oi(pFrame_Record_Data->Oad_Selector.Oi.asInt))
            {
               result = 0xff;
            }  

            /*无论是否是datetimes格式，都要先解一下，以便算出len*/
            if(0<=DeCode_Data_Len(NULL,pdata,0,(INT16U*)&len))
            {
                if(0!=Change_time_to_meter_time(pdata,&pFrame_Record_Data->DdataFilter[0],1))
                {
                    result = 0xff;                    
                }                     

                pdata  += len;	
			    Offset += len; 
            }
            else
            {   
                gDarDecode = DAR_FE_ABORNMAL;
                return 0;  
            }          
            break;           
        case EN_SELECTOR2: /*选择方法二*/
            /*
                Selector2∷=SEQUENCE
                {
                  对象属性描述符  OAD，
                  起始值          Data，
                  结束值          Data，
                  数据间隔        Data
                }
             */
            ConvertOAD(pdata, (INT8U*)&pFrame_Record_Data->Oad_Selector);
            pdata  += 4;	
			Offset += 4;  

            pdataTmp = pdata;
            /*2018-02-02:无论怎样，都要解码，然后RCSD返回，所以后面继续解码，纠结...*/
            for(i=0;i<3;i++)
            {    
                DeCode_Data_Len(NULL,pdata,0,&len);
                pdata  += len;
                Offset += len;
            }   
            
            if(0==Judge_Time_Oi(pFrame_Record_Data->Oad_Selector.Oi.asInt))
            {                                        
                if(0!=Change_time_to_meter_time(pdataTmp,&pFrame_Record_Data->DdataFilter[0],2))
                {
                   result = 0xff;                    
                }      

                pdataTmp += 16;   /*(date_time_s+7)*2 */     
                
                /*TI为空，表示全部*/
				if(0 ==*pdataTmp)
				{		
				    type_ti = 0;
					LIB_MemSet(0, &pFrame_Record_Data->DdataFilter[12], 3);
					//pdataTmp++;
                    
                    //Offset += 17;
				}
				else if(TI_t==*pdataTmp)
				{				
				    pdataTmp++;
				    /*拷贝 TI ,把TI类型也COPY了，即第一个字节TI类型也COPY了*/
                    LIB_MemCpy(pdataTmp, &pFrame_Record_Data->DdataFilter[12], 3);
                    //pdataTmp  += 3;
                    
                    //Offset += 20;                    
				}
                else
                {
                    /*
                     DeCode_Data_Len(NULL,pdata,0,(INT16U*)&len));
                     pdata  += len;
                     Offset += len;   */  
                   result = 0xff;   
                }    
            }
            else /*其它值，虽然拒绝读写，也要用RCSD返回，所以后面继续解码，纠结...*/
            {                                   
                result = 0xff;	                
            }
            break;
        case EN_SELECTOR9:
            /*Selector9∷=SEQUENCE
                {
                  上第n次记录  unsigned
                }*/
            if(0 == *pdata)
            {
                result = 0xff;	         
            }   
            else
            {
                pFrame_Record_Data->DdataFilter[0] = *pdata;
                pFrame_Record_Data->DdataFilter[1] = E_SELELTOR9_COMM;     /*这里赋值为0，表示是通信，非0，表示显示*/
            }   

             pdata  += 1;                    
             Offset += 1;  
             
            break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            return Offset;
            break;
    }
    
    /* 2017-08-02:判断周期性冻结单位*/
	if((EN_SELECTOR2 == SelectNumber)&&(0!=type_ti)&&(0!=Check_Freeze_Interval(OI,&pFrame_Record_Data->DdataFilter[12])))
	{
		result = 0xff;  
		//return Offset;			
	} 
    
	/*3.RCSD*/
	/*RCSD    2017-09-11:异常时，要把NUMBER限制下    */
	num= *pdata++;   //SEQUENCE OF CSD
	Offset++;	
	pFrame_Record_Data->OadListNumber = num;
    
	if(num>MAXCSD)  /*这里要限制，防止越界*/
	{
	    pFrame_Record_Data->OadListNumber = 0;  /*这个后面统一放在初始化中进行*/
		gDarDecode = DAR_3_NOACESS;//DAR_8_OVERFLOW;
		return Offset;
	}
    	
	/*2017-08-02:分钟冻结：读取时RCSD不允许为0，否则返回拒绝读写；*/
	if((OI_MIN_FRZ == OI)&&(0 == num))
	{	    
		gDarDecode = DAR_3_NOACESS;
		//return Offset;
	}

    /*解析CSD: [0] OAD 或[1] ROAD*/
    /*num*/
	//pdata++;
	//Offset++;	
    
	for(i = 0;i < num;i ++)
	{
		if(0 == *pdata)   //选择0  OAD
		{   
			pdata++;
			ConvertOAD(pdata, (INT8U *)&pFrame_Record_Data->oadlist[i]);
			pdata  += 4;
			
			Offset += 5;
		}
		else
		{
	        pFrame_Record_Data->OadListNumber = 0;  /*这个看后面是否支持用ROAD LIST应答，先不支持吧，ROAD解起来麻烦，内存多*/
			gDarDecode = DAR_3_NOACESS;	
            break;
		}

        /*2017-08-17:分钟冻结:RCSD中的所有OAD的属性特征取值必须一致，否则返回拒绝读写（DAR=3）；*/
        if(OI_MIN_FRZ == OI)
        {    
            OI_CSD  = pFrame_Record_Data->oadlist[i].Oi.asInt;
            if(OI_CSD == OI_DATA_FREEZE_TIME || OI_CSD == OI_FREEZE_RECORD_NUM)
            {
                continue;
            }    

            CSD_AID = pFrame_Record_Data->oadlist[i].AId;
            
            if(0xff == First_CSD_AID)
            { 
                First_CSD_AID = CSD_AID &  MASK_AID_SELECT_B5_B7;
            }    
            else
            {
                if(First_CSD_AID != (CSD_AID &  MASK_AID_SELECT_B5_B7))
                {    
                    gDarDecode = DAR_3_NOACESS;
                    //break;
                }
            }    
        }  
        /*END 2017-08-17:分钟冻结:RCSD中的所有OAD的属性特征取值必须一致，否则返回拒绝读写（DAR=3）；*/
	}

    if(0xff == result)
    {
       gDarDecode = DAR_3_NOACESS;
	   //return Offset; 
    }   

    return Offset;
     
}    

/*****************************************************************************
 函 数 名  : Get_Service_Decode
 功能描述  : 读取类服务解析处理
 输入参数  : INT8U *pAPDU       
             _CheckOBJECT *dst  
 输出参数  : pDstLen:已编码长度
 返 回 值  : 解析完后的指向地址
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 09 05
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Get_Service_Decode(INT8U *pAPDU)
{
    INT8U choice;
    INT8U ListNumber = 1;  
    INT8U i;
    INT16U Len;
    INT8U *pdata = pAPDU;

    ST_Next_Frame_Normal_Data *pFrame_Normal_Data;
    ST_Next_Frame_Record_Data *pFrame_Record_Data; 
        
    choice = *pdata++;
    
    if((0<choice)&&(EN_GET_NEXT>choice))
    {
       gServiceChoice = choice;
    }  

    pdata++;   /*iid*/    
        
    switch(choice)
    {
        case EN_GET_NORMAL:          
        case EN_GET_NORMAL_LIST:
            pFrame_Normal_Data = (ST_Next_Frame_Normal_Data *)gBak_data;
            
            if(EN_GET_NORMAL_LIST == choice)
            {    
                ListNumber = *pdata++;
            }
    
            if(ListNumber>MAXGETLISTNUMBER)
            {
                gDarDecode = DAR_FE_ABORNMAL;
                return pdata;  
            }    
            
            pFrame_Normal_Data->CurrentLine = 0;
            pFrame_Normal_Data->TotalLine = ListNumber;

            for(i = 0;i < ListNumber;i++)
            {    
                ConvertOAD(pdata,(INT8U *)&pFrame_Normal_Data->oadlist[i]);
                pdata += 4; /*指针指向 时间标签*/
            }
            
            break;
        case EN_GET_RECORD: 
        case EN_GET_RECORD_LIST:
            pFrame_Record_Data = (ST_Next_Frame_Record_Data *)gBak_data;
            
            if(EN_GET_RECORD_LIST == choice)
            {    
                ListNumber = *pdata++;
            }

            if(ListNumber>MAXGETRECORDLISTNUMBER)
            {
               gDarDecode = DAR_FE_ABORNMAL;
               return pdata;  
            }    
            
            Record_Num_List = ListNumber;
            
            for(i = 0;i < ListNumber;i++)
            {
                pFrame_Record_Data = (ST_Next_Frame_Record_Data *)gBak_data[i];

                pFrame_Record_Data ->CurrentLine = 0;
                pFrame_Record_Data ->TotalLine   = 0;
                
                Len = DecodeRecordData(pFrame_Record_Data,pdata);                 

                pdata += Len;     

                if(0!=gDarDecode)
                {
                    return pdata;  
                } 
            }               
            break;
        //case EN_GET_NEXT:  /*后面再处理*/            
            //break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            
            break;
    }   

    return pdata;  
    
}
/*****************************************************************************
 函 数 名  : Set_Service_Decode
 功能描述  : 设置类服务解析处理
 输入参数  : INT8U *pAPDU       
 输出参数  : 无
 返 回 值  : 解析完后的指向地址
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 09 05
    作    者   : e
    修改内容   : 新生成函数

在这里面，一把处理完数据
*****************************************************************************/
INT8U *Set_Service_Decode(INT8U *pAPDU)
{
    INT8U choice;
    INT8U ListNumber = 1;  
    INT8U i;
    INT16U len   = 1;  /*故意初始化为1，以便数据能慢慢解完,因为DeCode_Data_Len处理后，可能len没有改变*/
    INT8U *pdata = pAPDU;
    ST_SET_SERVICE *p_Set_data;
    ST_SET_THEN_GET_SERVICE *p_Set_then_data;
    
    choice = *pdata++;
    pdata++;   /*iid*/

    //if((EN_SET_NORMAL<=choice)&&(EN_SET_THEN_GET_NORMAL_LIST=>choice))
    {
       gServiceChoice = choice;
    }  
    
    switch (choice)
    {
        case EN_SET_NORMAL:            
        case EN_SET_NORMAL_LIST:
            p_Set_data = (ST_SET_SERVICE *)gBak_data;  /*虽然不会有后续帧，但可能影响其它端口，所以要用ComPort*/

            if(EN_SET_NORMAL_LIST == choice)
            {    
                ListNumber = *pdata++;
            }
    
            if(ListNumber>MAXSETLISTNUMBER)
            {
                gDarDecode = DAR_FE_ABORNMAL;
                return pdata;  
            }  

            p_Set_data->ListNumber = ListNumber;
            
			for(i = 0;i < ListNumber;i ++)
			{
				ConvertOAD(pdata,(INT8U *)&p_Set_data->SetOad[i]);
				pdata += 4;
				p_Set_data->pointer[i] = pdata;

                if(0<=DeCode_Data_Len(NULL,pdata,0,&len))
                {
                    pdata += len;
                }  
                else /*解码异常*/
                {    
                   gDarDecode = DAR_FE_ABORNMAL;
                   return pdata;   
                }				
			}            
            break;
        case EN_SET_THEN_GET_NORMAL_LIST:
            p_Set_then_data = (ST_SET_THEN_GET_SERVICE *)gBak_data;  /*虽然不会有后续帧，但可能影响其它端口，所以要用ComPort*/

            ListNumber = *pdata++;
            
            if(ListNumber>MAXSETTHENGETLISTNUMBER)
            {
                gDarDecode = DAR_FE_ABORNMAL;
                return pdata;  
            }  

            p_Set_then_data->ListNumber = ListNumber;

            for(i = 0;i < ListNumber;i ++)
			{
				ConvertOAD(pdata,(INT8U *)&p_Set_then_data->SetOad[i]);
				pdata += 4;
				p_Set_then_data->pointer[i] = pdata;
				if(0<=DeCode_Data_Len(NULL,pdata,0,&len))
                {
                    pdata += len;
                }  
                else /*解码异常*/
                {    
                   gDarDecode = DAR_FE_ABORNMAL;
                   return pdata;   
                }	
                
                ConvertOAD(pdata,(INT8U *)&p_Set_then_data->GetOad[i]);
				pdata += 4;
                
				pdata++;   /*延时读取时间 unsigned*/
			}
            break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            break;
    }

    return pdata;
    
}    

/*****************************************************************************
 函 数 名  : Action_Service_Decode
 功能描述  : 操作方法类解析处理
 输入参数  : INT8U *pAPDU       
             _CheckOBJECT *dst  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 09 05
    作    者   : e
    修改内容   : 新生成函数
    
在这里面，一把处理数据
*****************************************************************************/
INT8U *Action_Service_Decode(INT8U *pAPDU)
{
  /*action和set是一样的。。。*/
  return  Set_Service_Decode(pAPDU); 
}    


/*比较数据长度的合法性
 在时间标签处判断数据长度，如果与链路层长度不同，则返回失败，表示数据非法
return:0:ok,else:error
*/
INT8S CheckApduDatalen(ST_OBJECT *pstFrm698,INT8U *pTimeTag)
{
    INT16U len;

    len = pTimeTag - pstFrm698->pdata;
    
    if(1 == *pTimeTag)
    {
        len += LEN_TIME_TAG;
    }  
    else
    {
        len += 1;
    }

    len++;  /*加一个服务类型的长度，pstFrm698->Len_data包括了服务类型长度*/
    
    if(len == pstFrm698->Len_data)
    {
        return 0;
    }    
    else
    {
        return (-1);
    }    
    
} 

/*****************************************************************************
 函 数 名  : check_time_tag
 功能描述  : 判断时间标签合法性
 输入参数  : INT8U *p_time_tag
 输出参数  : 无
 返 回 值  : p->Value 非0，非法
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 星期四 2017 04 13
    作    者   : e
    修改内容   : 新生成函数

  时间标签内容
  发送时标 		   	date_time_s，
  允许传输延时时间 	TI   ENUMERATED,单位:秒:0,分:1,时:2,日:3,月:4,年,5

*****************************************************************************/
/*时间周期值，单位秒*/
const INT32U  TimeUnit[] =
{
    1, 
    60, 
    60L * 60L, 
    60L * 60L * 24L, 
    60L * 60L * 24L * 31L, 
    60L * 60L * 24L * 366L
};

/*TimeTag:SEQUENCE
{
    发送时标            date_time_s,       7byte
    允许传输延时时间    TI,                3byte
}
*/
INT8U check_time_tag(INT8U *p_time_tag_src)
{ 
    ST_6TIME_FORM  date_time_6;
    
    INT32S Time_Interval;
    
    INT8U  Time_Unit;
    INT16U Time_int;
    INT16U year;
    INT32U Time_Interval_value,TmpLong;  
    INT8U *p_time_tag;
    ST_7TIME_FORM SysTime;

    /*偏移过时间标签选项字*/
    p_time_tag = p_time_tag_src + 1;

    /*时间格式转换*/
    DeCode_Base(p_time_tag,longunsigned,(INT8U *)&year);    
    year   = year % 100;  //year = year - 2000;
    date_time_6.year = (INT8U)year; 
    LIB_MemCpy(p_time_tag+2, (INT8U * )&date_time_6.month, sizeof(ST_6TIME_FORM)-1);
    if(FALSE == LIB_ChkDateTimeHEX(&date_time_6))/*检查时间格式是否正确*/
    {
        return DAR_32_TIMETAGERR;
    }
    
    /*单位*/
    Time_Unit = *(p_time_tag+LEN_DATE_TIME_S);
    
    /* 间隔值*/
    DeCode_Base(p_time_tag+LEN_DATE_TIME_S+1,longunsigned,(INT8U *)&Time_int);
    
    //Time_int  = (INT16U)(*(p_time_tag+LEN_DATE_TIME_S+1))<<8;
    //Time_int  = Time_int + (*(p_time_tag+LEN_DATE_TIME_S+2));

    /*时间间隔为0目前处理为不拒绝，直接加上0*/
    Time_Interval_value = Time_int;

    /*时间转换*/    
    if(Time_Unit < ARRAY_SIZE(TimeUnit))      //秒
    {
        TmpLong = TimeUnit[Time_Unit];
    } 
    else
    {
        return DAR_32_TIMETAGERR;
    }   

    Time_Interval_value = Time_Interval_value*TmpLong;
    
    //------
    GetSingle(E_SYS_TIME, (INT8U * )&SysTime);

    Time_Interval = LIB_SToStdDate((ST_6TIME_FORM *)&SysTime) - LIB_SToStdDate(&date_time_6);
    
    Time_Interval -= Time_Interval_value;/*减去间隔时间秒数*/
    if(Time_Interval > 0)
    {
 
        return DAR_32_TIMETAGERR;
    }
    else
    {
        return DAR_0_OK;         
    }
    
}   


/*****************************************************************************
 函 数 名  : Check_Broad_Cast_OAD
 功能描述  : 判断可以广播操作的OAD:广播校时，多功能输出，瞬时冻结触发
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 03 29
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Check_Broad_Cast_OAD(_ST_OMD Omd)
{
    INT16U OI; 
    INT8U  MTD;
    INT8U  Result = DAR_FF_OTHERERR;

    OI  = Omd.Oi.asInt;
    MTD = Omd.MTD;

    if(((OI_DATE_TIME == OI)||(OI_MULTI_DEVICE == OI))&&(127 == MTD))
    {
        Result = DAR_0_OK;
    } 
    else if((OI_MMT_FRZ == OI)&&(3 == MTD))
    {
        Result = DAR_0_OK;
    }    

    return Result;   
    
}    


/*****************************************************************************
 函 数 名  : Check_Addr_Type_Right
 功能描述  : 检查特殊地址的权限
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 02 01
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Check_Addr_Type_Right(EN_ADDRTYPE AddrType,INT8U ServiceType)
{
    ST_ACTION_SERVICE *p_Action_data;    
    INT8U  result = DAR_0_OK;

    if(EN_GETREQUEST != ServiceType)
    {
        /*通配地址不能写和方法操作，但为了获取OAD，以便应答，还是走完下面的流程*/
        if(ADDR_ABBREVIATE == AddrType)
        {
           result = DAR_15_PSWERR;           
        } 
        /*广播地址不能写和方法操作，只能广播校时和多功能输出*/
        else if(ADDR_BROADCAST == AddrType)
        {          
             p_Action_data = (ST_ACTION_SERVICE *)gBak_data;
             if(0 == ((EN_ACTIONREQUEST == ServiceType)&&(1 == p_Action_data->ListNumber)&&(DAR_0_OK == Check_Broad_Cast_OAD(p_Action_data->ActionOmd[0]))))
             {
                result  = DAR_15_PSWERR; 
             }   
             else
             {

             }           
        }    
    }    
    else
    {
        /*读，组播地址和广播地址不处理*/
        if((ADDR_BROADCAST == AddrType)||(ADDR_GROUP == AddrType))
        {
            result = DAR_FE_ABORNMAL;            
        }    
    }       

    if(DAR_0_OK != result)
    {
        gDarDecode = result;
    }    
    
    return  result;    
}    
    

/*****************************************************************************
 函 数 名  : Check_OINeedTimeTag
 功能描述  : 判断Oi是否需要时间标签，如远程控制OI,保电或远程控制，清0等
 输入参数  : _CheckOBJECT *p
 输出参数  : 无
 返 回 值  : _FALSE:不需要判断时间标签，_TRUE:需要判断时间标签
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 星期四 2017 04 13
    作    者   : e
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Check_OI_Need_Time_Tag (ST_ACTION_SERVICE *p_Action_data)
{
    INT8U  i;
    INT16U Tmp_asInt;
    INT8U  Ret = FALSE;

    /*2017-11-18 厂内，直接退出*/
    if (TRUE == ChkStatus(E_FAC))
    {
        return FALSE;
    }  

    for(i=0;i<p_Action_data->ListNumber;i++)
    {
        Tmp_asInt = p_Action_data->ActionOmd[i].Oi.asInt& 0xff00;;
        
        /*1.保电或远程控制 ,2.清0操作*/
	    if((OI_REMOTE == Tmp_asInt)
	  	||(OI_ELECTRI_DEVICE == Tmp_asInt))
	    {	
            Ret = TRUE;
        }
        /*3.分相事件清0*/
        else if((0x3000 == Tmp_asInt)&&(1 == p_Action_data->ActionOmd[i].MTD))
	    {
            Ret = TRUE;
        }       
    }

    return Ret;
}



/*****************************************************************************
 函 数 名  : CheckApFrame
 功能描述  : 报文解析，将相关OAD LIST保存，以及后面处理时用
 输入参数  : ST_OBJECT *pObject  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 12 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void CheckApFrame(ST_OBJECT *pObject,INT8U *pTimeTagTmp)
{
    INT8U ServiceType;
    INT8U *pTimeTag = NULL;  /*指向时间标签*/  
    INT8U *pdata = pObject->pdata;

    ServiceType = pObject->servicetype;
    //choice =pdata++;   /*原样返回*/
    //IID    =pdata++;   /*原样返回*/
  
    gDarDecode = 0;    
    
    switch(ServiceType)
	{
        case EN_GETREQUEST:
            pTimeTag = Get_Service_Decode(pdata);     
    	    break;
    	case EN_SETREQUEST:
            pTimeTag = Set_Service_Decode(pdata);  		    
    	    break;
    	case EN_ACTIONREQUEST: 
            pTimeTag = Action_Service_Decode(pdata); 
            break;
        default:
            gDarDecode = DAR_FE_ABORNMAL;
            break;
    }   
    
    LIB_MemCpy(pTimeTag, pTimeTagTmp, LEN_TIME_TAG);

    if(DAR_FE_ABORNMAL == gDarDecode)
    {
        return;  
    }    
    
    /*方法和设置类，增加判断数据长度的合法性*/
    //if((SETREQUEST == Type_Service)||(ACTIONREQUEST == Type_Service))
    if(EN_GETREQUEST != ServiceType)
    {    
        if(0 != CheckApduDatalen(pObject,pTimeTag))
        {
            /*改成类型不匹配吧，否则数据长度是对的，但类型不对，会提示的是VALUE9，不太好*/
            gDarDecode = DAR_5_OANOADAPT;   
            return;
        }   
    }  

    /*检查地址的权限*/
    if(DAR_0_OK!= Check_Addr_Type_Right(pObject->AddrType,ServiceType))
    {
        return;
    }

    /*时间标签处理  pdata已偏移到了时间标签位置*/
    if(1 == *pTimeTag)
    {
       if(DAR_0_OK != check_time_tag(pTimeTag))
       {
          gDarDecode = DAR_32_TIMETAGERR;
       }
    }
    else
    {
        if(EN_ACTIONREQUEST == ServiceType)
        {    
            if(TRUE==Check_OI_Need_Time_Tag((ST_ACTION_SERVICE *)gBak_data))
            {
                gDarDecode = DAR_32_TIMETAGERR;  
            }
        }
    } 
    
}    


/*****************************************************************************
 函 数 名  : check_list_choice
 功能描述  : 判断是否是getLIST或actinolist或setlist,以便偏移数据长度
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 12 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void check_list_choice(INT8U servicetype,INT8U *LenEncode)
{
    INT8U choice;

    choice = gServiceChoice;
    
    if(EN_GETREQUEST == servicetype)
    {
        if((EN_GET_NORMAL_LIST == choice)||(EN_GET_RECORD_LIST == choice))
        {
            *LenEncode += 1;
        }    
    }    
    else /*set action的list choice是一样的*/
    {
        if((EN_SET_NORMAL_LIST == choice)||(EN_SET_THEN_GET_NORMAL_LIST == choice))
        {
            *LenEncode += 1;
        }   
    }    
}

/*****************************************************************************
 函 数 名  : Action_PP_PrgEvent
 功能描述  : 关联对象的添加删除等编程事件
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期六 2018 01 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void Action_PP_PrgEvent(void)
{
    //ST_OAD Oad;
    INT16U Oi;
    INT8U Oi0,Aid;
    INT8U F_action_prg = 0;  /*需要记录编程事件的ACTION*/
        
    Oi0 = CurOad.Oi.asByte[OIH];
    Oi  = CurOad.Oi.asInt;
    Aid = CurOad.AId;
    
    /*24类*/
    #if 0
    if((OI_OVER_PWR_EVENT>=Oi) ||(OI_OVER_R_DEMAND_EVENT == Oi))
    {
        if((4 == Aid)||(5 == Aid))
        {
            F_Relation_Obj = 0xff;
        }    
    }
    else 
    #endif
    
    if((OI_MEASURE_CHIP_ERR_EVENT>=Oi)&&(0x30 == Oi0)) /*7类和24类*/
    {
       if((4 == Aid)||(5 == Aid))
       {
           F_action_prg = 0xff;
       }     
    }  
    else if(0x50 == Oi0)                               /*冻结*/
    {
        if((4 <= Aid)||(8 >= Aid))
        {
            F_action_prg = 0xff;
        }    
    }
    else if(((OI_RS485_DEVICE == Oi)&&(0x7f==Aid)) || /*修改波特率*/
            ((OI_CARRIE_DEVICE == Oi)&&(0x80==Aid)))
    {    
        F_action_prg = 0xff;
    }
    
    if(0xff==F_action_prg)
    {
       PP_PrgEvent(CurOad);
    }       
    
}    
/*"----------------------------下面是编码相关函数-------------------------------------------"*/

/*****************************************************************************
 函 数 名  : Cal_EnCode_Data_Len
 功能描述  : 根据IID计算编码后长度，封装一下，这样函数意思明确些，好看点
 输入参数  : ST_ENCODE_INFO * pInfo  
             INT8U IID               
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 31
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16U Cal_EnCode_Data_Len(ST_ENCODE_INFO * pInfo,INT8U IID)
{
   return EnCode_Data(pInfo,NULL,NULL,IID);
}    

/*****************************************************************************
 函 数 名  : Get_OAD_Data_len
 功能描述  : 根据OAD，获取打包数据的长度,同时检查数据是否有定义
 输入参数  : ST_OAD Oad  
 输出参数  : 无
 返 回 值  : *pret 不为DAR_0_OK，表示未定义
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 18
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16U Get_OAD_Data_len(ST_OAD Oad,SUB_OBJECT *pobject,INT8U  *pret)
{
    //SUB_OBJECT object;
    INT16U len = 0;
    
    INT8U AId;/*"属性标识"*/
    INT8U Class;      /*类*/

    Oad.AId = Oad.AId & MASK_AID_SELECT;   /*之所以用OAD.AID与，主要是因为get_oad_object中要用*/
    AId = Oad.AId;
    
    pobject->Class = 0;                    /*先置为0，以便后面判断*/

    LIB_MemCpy((INT8U *)&Oad,(INT8U *)&pobject->Oad ,sizeof(ST_OAD));
    
    *pret = get_oad_object(pobject);   /*确定是否有定义*/

    Class = pobject->Class;
    
    if(DAR_0_OK == *pret)
    {
        if(1 == AId)
        {
           len = LEN_LOGIC_NAME;
        }   
        else if(((3 == AId)&&(Class<=6))||((5 == AId)&&(1 == Class))) /*换算单位*/
        {
           len = LEN_SCALE_UNIT; 
        }    
        else
        {
           if(17 == Class)
           {
              len = 4;   /*17类先固定为4*/
           }
           else
           { 
              len = Cal_EnCode_Data_Len((ST_ENCODE_INFO *)pobject->Pointer,Oad.IId); 
           }
        }   
    }
    else
    {
        
    }  

    return len;
}    


/*在这个函数前，要判断是否会越界*/
/*打包GET DAR,共打包6字节 ，return:编码后的地址*/
INT8U *EncodeGetDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdata)
{
    INT8U *pdst = pdata;
   
    pdst = EnCodeOAD(pOad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DAR;     /*表示返回的是DAR*/
    *pdst++ =  dar_value;
    
    return pdst;
}  


/*打包设置结果 5个字节*/
INT8U *EncodeSetDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdata)
{
    INT8U *pdst = pdata;
    
    pdst    = EnCodeOAD(pOad, pdst);
    
    *pdst++ =  dar_value;

    if(DAR_0_OK == dar_value)
    {
       PP_PrgEvent(*pOad);
    }  

    return pdst;
}    

/*打包方法类结果 6个字节*/
INT8U *EncodeAcionDar(ST_OAD*pOad,INT8U dar_value,INT8U *pdata)
{
    INT8U *pdst = pdata;
    
    pdst = EnCodeOAD(pOad, pdst);
    
    *pdst++ =  dar_value;
    
    *pdst++ =  0;         /*Data  OPTIONAL*/

    if(DAR_0_OK == dar_value)
    {   
        Action_PP_PrgEvent();
    }    
    
    return pdst;
}    

/*编码AID属性1，一般是逻辑名，现在用OI表示.即编码OI:OAD+OCTSTRING+2+OI*/
INT8U * EncodeAtt1LogicName(ST_OAD*pOad,INT8U *pdata)
{	
    INT8U * pdst = pdata;

    pdst = EnCodeOAD(pOad,pdst);
    
    *pdst++ = E_RESULT_CHOICE_DATA;        /*结果为数据*/
        
    *pdst++ = octetstring;                 /* OI类型  */
    *pdst++ = 2;  

    *pdst++ = pOad->Oi.asByte[OIH];        /* OI值  */
	*pdst++ = pOad->Oi.asByte[OIL]; 

    return pdst;
}


/*****************************************************************************
 函 数 名  : Encode_Scale_Unit
 功能描述  : 打包换算单位
 输入参数  : ST_OAD*pOad    
             INT8U *pdata   
             INT8U element  
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 19
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Encode_Scale_Unit(ST_OAD*pOad,INT8U *pdata,INT8U element)
{
    INT8U * pdst = pdata;

    ConvertOAD((INT8U *)pOad,pdst);    
    pdst    += 4;

    *pdst++ = E_RESULT_CHOICE_DATA;  /*结果为数据*/
    
    *pdst++ = TScaler_Unit;          /* 类型  */

    *pdst++ = CLS_1_6_OBJECT_ELE[element].Scaler;        
	*pdst++ = CLS_1_6_OBJECT_ELE[element].UNIT; 

    return pdst;
}    
 
/*****************************************************************************
 函 数 名  : Get_Crypt_Type
 功能描述  : 将解码后的数据转换成安全模式识别的级别 EN_CRYPT_TYPE_SAFE_MODE
 输入参数  : INT8U Crypt_Type_Src  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 01 18
    作    者   : xyt
    修改内容   : 新生成函数
    
1.密文
  1)密文+MAC算是 密文+验证码
  2)其它算是密文
2.明文
  1)明文算是明文
  2)其它算是明文+验证码
*****************************************************************************/
INT8U Get_Crypt_Type(INT8U Crypt_Type_Src)
{
    INT8U Crypt_Type_Tmp;    
    
    if(ENCRYPTED_WITH_SIDMAC == Crypt_Type_Src)
    {
        Crypt_Type_Tmp = E_ENCRYPT_MAC_TYPE;
    }    
    else if(ENCRYPTED <= Crypt_Type_Src)
    {
        Crypt_Type_Tmp = E_ENCRYPT_TYPE;
    }    
    else if(UN_ENCRYPTED == Crypt_Type_Src)
    {
        Crypt_Type_Tmp = E_UN_ENCRYPT_TYPE;
    }    
    else
    {
        Crypt_Type_Tmp = E_UN_ENCRYPT_MAC_TYEP;
    }    

    return Crypt_Type_Tmp;
        
}


/*返回处理结果 return :-1:异常，>=0:编码后的数据长度*/
INT16S PP_process(ST_OBJECT *pObject)
{    
    INT8U LenEncode = 2;   /*2代表 choice和PIID*/
    
    INT8U TimeTag[LEN_TIME_TAG];
    
    INT8U *pdata = pObject->pdata;

    class_comm_build();
    
    ComPort = NowComId;
    
    Crypt_Type   = Get_Crypt_Type(pObject->Crypt_Type);
    pEndEncodeAddr = pObject->pdata + MAX_APDU_LEN-LEN_TIME_TAG-MAX_LEN_FOLLOW_REPORT-LEN_RESERVE;

    /*解帧*/
    CheckApFrame(pObject,TimeTag);

    check_list_choice(pObject->servicetype,&LenEncode);

    pdata += LenEncode;  /*偏移过choice和PIID，以及list num ，即原样返回*/

    /*服务choice类型不对*/
    if(DAR_FE_ABORNMAL == gDarDecode)
    {
        return (-1);   /*看是不是自己返回*/
    } 
    
    pdata = Process_698(pObject->servicetype,pdata);

    /*异常，如LIST超大，越界,或服务choice类型不对*/
    if(DAR_FE_ABORNMAL == gDarDecode)
    {
        return (-1);   /*看是不是自己返回*/
    }  


    /*2.打包时间标签*/
    if(1 == TimeTag[0])
    {
        LIB_MemCpy(TimeTag, pdata, LEN_TIME_TAG);
        pdata += LEN_TIME_TAG;
    }
    else
    {    
       *pdata++ = 0;
    }
    
    return (pdata - (pObject->pdata)); 
    
}

/*"*****************************************************************************"*/
/*"  Function:       PP_OadToObis"*/
/*"  Description:    根据映射表查找指定OAD对应的OBIS对象及偏移"*/
/*"  Calls:          "*/
/*"  Called By:      数据中心"*/
/*"  Input:          pOad: Oad地址 "*/
/*"  Output:        pObis: Obis存储地址；pOffet: Obis偏移地址；pFormatId: Obis通信格式ID"*/
/*"  Return:        OAD对应OBIS中的元素个数(大的，可以连续的OBIS分割)，为0时表示查找失败"*/
/*"*****************************************************************************"*/
INT8U PP_OadToObis_Info(SUB_OBJECT *pobject, ST_PARAMENT_OAD_OBIS_INFO *pObis_Info)
{
    INT8U i,class_num;
    
    class_num = pobject->Class;

    /*容错，先都初始化一下，防止后面有的没有赋值*/
    for(i=0;i<MAX_OBIS_NUM_ONE_OAD;i++)
    {    
        pObis_Info[i].FormatId   = MAX_FORMAT_ID;   /*先初始化为无格式*/
        pObis_Info[i].FunctionId = FUN_NULL;      /*先初始化为函数*/
        pObis_Info[i].ShowinfoId = E_INFO_NULL;   /*先初始化为无格式*/
        pObis_Info[i].num        = 0;
    }

    /*电量、变量的OBIS只能有一个，参变量才可能多个，如果是有多个，则作为参变量类处理，或代码特殊处理*/
    if(7>class_num)
    {
        get_class1_6_Obis(pobject, pObis_Info);        
        
        return 1;    
    }    

    /*下面这个代码，到时用一个函数列表代替，先这样写，以便调试*/
    switch ( class_num )
    {       
        case 7 :
            return get_class7_Obis(pobject, pObis_Info);
            break;
        case 8 :
            return get_class8_Obis(pobject, pObis_Info);
            break;
        case 9:
            return get_class9_Obis(pobject, pObis_Info);
            break;    
        case 17 : 
            return get_class17_Obis(pobject, pObis_Info);
            break;
        case 19 :
            return get_class19_Obis(pobject, pObis_Info);
            break;
        case 20 :
            return get_class20_Obis(pobject, pObis_Info);
            break;
#ifdef PM_MODLE            
        case 21 :
            return get_class21_Obis(pobject, pObis_Info);
            break;
#endif            
        case 22 :
            return get_class22_Obis(pobject, pObis_Info);
            break;
        case 24 :
            return get_class24_Obis(pobject, pObis_Info);
            break;
        default:
            break;
    }

    return 0;
}


/*****************************************************************************
 函 数 名  : PP_GetOadToObisList
 功能描述  : 根据IID获取对应的OBIS列表，以及格式列表
 输入参数  : INT8U IID                            
             INT16U *pObis                        
             ST_PARAMENT_OAD_OBIS_INFO *pObis_Info  
             ST_FMT_ATTR *pFormat                 
 输出参数  : 无
 返 回 值  : OBIS个数
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 12
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U PP_GetOadToObisList(INT8U IID,INT8U num_obis,INT16U *pObis,ST_PARAMENT_OAD_OBIS_INFO *pObis_Info,ST_FMT_ATTR *pFormat)
{
    INT8U i,j,num_ms_obis,TransId;  /*TransId:通信格式ID*/
    const ST_FORMAT_ID *pObisFmt;    
    INT16U temp_Obis;
    INT8U f_block_data = 0;         /*非0，块数据*/
    INT8U Offset_Format,Offset_Format_Tran;
    INT8U temp_iid;                 /*实际用于计算IID=0时的OBIS个数*/

    *pObis = E_OBIS_NULL;   /*先默认一个无效的OBIS*/
   
    if(0==IID)
    {   
        num_ms_obis = pObis_Info[0].num;
        /*块数据*/
        if(num_ms_obis>=E_SEASON_SLOT_ID)
        {
            *pObis = pObis_Info->Obis;         /*取块OBIS,块只要取第一个就行*/
            num_ms_obis = LenGet_PP_698((ENUM_DATALEN_ID)num_ms_obis);
            
            pObisFmt = (const ST_FORMAT_ID *)&stFormatId[pObis_Info[0].FormatId];
            
            TransId = pObisFmt->TransId;
               
            if(NULL!=pFormat)
            {    
                LIB_MemCpy((INT8U *)&stFormat[TransId], (INT8U *)(pFormat), sizeof(ST_FMT_ATTR));
            }

            /*2018-01-19:容下错，防止越界*/
            if(num_ms_obis>=64)   /*以最大允许费率数63容错 显示列表可能比较大，但不会走到这里来，单独特殊处理了*/
            {
               return 0; 
            }
            
            return (num_ms_obis+MASK_BLOCK_DATA);  /*块数据返回为0x80+个数*/
        }    
    }  

    /*-----------------------------------------------------------------*/
    
    /*2018-01-19:容下错，防止越界*/
    if(num_obis>MAX_OBIS_NUM_ONE_OAD)
    {
        return 0; 
    }  

    temp_iid   = 0;
    
    for(i=0;i<num_obis;i++)
    {
        temp_Obis   = pObis_Info[i].Obis;
        num_ms_obis = pObis_Info[i].num;
        
        if(num_ms_obis>=E_SEASON_SLOT_ID)     /*块数据，以便打到具体的IID对应，IID=0的，已在上面处理，不会越界*/
        {
           temp_Obis++;
           num_ms_obis = 63; // LenGet_PP_698(num_ms_obis);  /*块数据如果是单独找的话，就用最大值，具体IID的合法性，在get_oad里判断，后面再看情况，重新写个函数也行*/
           f_block_data = 0xff;
        }        
        
        for(j=0;j<num_ms_obis;j++)        
        {                
            /*下面内容打包成一个函数，可用于显示与传输用*/    
            /*指针容下错，以便只取OBIS，不取格式*/    
            if(NULL!=pFormat)
            {    
                /*FormatId容下错*/ 
                /*这种情况，要求是IID=0的单OBIS不能大于MAX_OBIS_NUM_ONE_OAD，实际，因为多OBIS的，不进行格式转换，所以可以直接给0赋值即可，但这里暂时保留下*/
                if(0==IID)  
                {
                    Offset_Format_Tran = temp_iid;
                    if(Offset_Format_Tran>=MAX_OBIS_NUM_ONE_OAD)  /*2018-02-22:还是容下错吧*/
                    {
                        Offset_Format_Tran = 0;
                    }    
                }    
                else
                {
                    Offset_Format_Tran = 0;
                }  
                
                if(MAX_FORMAT_ID>pObis_Info[i].FormatId)
                {    
                    if(0!=f_block_data) /*块数据，固定取第一个格式,主要是为了防止越界*/
                    {
                       Offset_Format = 0;
                    }
                    else
                    {    
                       Offset_Format = i;                       
                    }
                    
                    pObisFmt = (const ST_FORMAT_ID *)&stFormatId[pObis_Info[Offset_Format].FormatId];                
                                           
                    TransId = pObisFmt->TransId;                     
                    
                    LIB_MemCpy((INT8U *)&stFormat[TransId], (INT8U *)(pFormat + Offset_Format_Tran), sizeof(ST_FMT_ATTR));

                }                
                else
                {    
                    LIB_MemCpy((INT8U *)&stFormat[E_FORMAT_NONE_6], (INT8U *)(pFormat + Offset_Format_Tran), sizeof(ST_FMT_ATTR)); 
                    //LIB_MemSet(E_NONE_FORMAT, (INT8U *)(pFormat + Offset_Format_Tran), sizeof(ST_FMT_ATTR)); 
                }   
            }   
               
            temp_iid++;
            
            if((temp_iid==IID)||(0==IID))
            {
                *pObis = temp_Obis;                
            }  
                         
            /*无论是IID等于几，都自动加，因为是连续值*/    
            temp_Obis++;
            
            if((temp_iid==IID)&&(0!=IID))
            {
                return 1;
            }    
            
             /*IID=0,才可能有多个，自增*/
            if(0 == IID)
            {
                pObis++;                
            }               
       } 
        
    }    

     return temp_iid;    
}    

/*"*****************************************************************************"*/
/*"  Function:       PP_OadToObis"*/
/*"  Description:    根据映射表查找指定OAD对应的OBIS对象及偏移"*/
/*"  Calls:          "*/
/*"  Called By:      数据中心"*/
/*"  Input:          pOad: Oad地址 "*/
/*"  Output:        pObis: Obis存储地址；pOffet: Obis偏移地址；pFormatId: Obis通信格式ID"*/
/*"  Return:         OAD对应OBIS中的元素个数，为0时表示查找失败,
  1.>0x80，表示是块数据，元素个数为返回值-0x80，pObis中为一个块OBIS ID
  2.<=0x80,返回的是具体元素个数，pObis存的是OBIS列表"*/

 /*注:pOffset和pFormat 注意防越界，可以为空*/
/*"*****************************************************************************"*/
INT8U PP_OadToObis(ST_OAD *pOad, INT16U *pObis, INT16U *pOffset, ST_FMT_ATTR *pFormat)
{
    INT8U num_obis,num_obis_list;
    SUB_OBJECT object;
    ST_PARAMENT_OAD_OBIS_INFO Obis_Info[MAX_OBIS_NUM_ONE_OAD];  /*只是用于暂存*/
    ST_OAD CurOadTmp;  /*用来暂存*/
    
    object.Class = 0;                    /*先置为0，以便后面判断*/

    LIB_MemCpy((INT8U*)pOad,(INT8U *)&object.Oad ,sizeof(ST_OAD));

    *pObis = E_OBIS_NULL;  /*先默认一个无效的OBIS*/
    
    /*确定是否有定义,同时获取元素位置*/
    if(DAR_0_OK != get_oad_object(&object))   
    {
        return 0;
    }    
    
    num_obis = PP_OadToObis_Info(&object,Obis_Info);

    /*需要暂存一下，以便在LenGet_PP_698()中用CurOad*/
    LIB_MemCpy((INT8U*) &CurOad, (INT8U*)&CurOadTmp, sizeof(ST_OAD));

    LIB_MemCpy((INT8U*)pOad, (INT8U*)&CurOad, sizeof(ST_OAD));

    num_obis_list = PP_GetOadToObisList(pOad->IId,num_obis,pObis,Obis_Info,pFormat); 
        
    LIB_MemCpy((INT8U*) &CurOadTmp, (INT8U*)&CurOad, sizeof(ST_OAD));

    return num_obis_list;
    
    
}


/*****************************************************************************
 函 数 名  : PP_ObisToOad
 功能描述  : OBIS转OAD函数功能，只针对特殊OAD的，如事件的，或个别特殊OAD
 输入参数  : ST_OAD Oad  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2017 12 29
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U PP_ObisToOad(INT16U Obis,ST_OAD *pOad)
{
    if(DAR_0_OK == PP_Obis_Oad_class7(Obis,pOad))
    {
        return DAR_0_OK;
    }
    else if(DAR_0_OK == PP_Obis_Oad_class24(Obis,pOad))
    {
        return DAR_0_OK;
    }    
    else
    {
        return DAR_FF_OTHERERR;
    }    
}    
/*****************************************************************************
 函 数 名  : test_pp_oad_obis
 功能描述  : 测试OAD 转OBIS功能
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 26
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void test_pp_oad_obis(void)
{
    INT8U i;    
    ST_FMT_ATTR Format_list[MAX_OBIS_NUM_ONE_OAD];
    INT16U Obis[MAX_OBIS_NUM_ONE_OAD];
    INT8U temp_data[MAX_ONE_OAD_DATA_LEN];
    INT16S SRet;
    
    ST_OAD Oad ={0x2004,0x02,0x00};        /*OAD*/

    PP_OadToObis(&Oad,Obis,NULL,Format_list);

    SRet = GetSingle(Obis[0], temp_data);

    if(0>SRet)
    {
      for(i=0;i<20;i++)
      {
          ;
      }   
    } 
    else
    {    
        for(i=0;i<10;i++)
        {
            ;
        } 
    }
    
    
    
}    
/*"*****************************************************************************"*/



