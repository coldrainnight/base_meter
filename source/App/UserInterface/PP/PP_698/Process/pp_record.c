/*"*****************************************************************************"*/
/*"  FileName: [pp_record.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  事件与冻结记录相关处理函数文件   "*/
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
#include "App\message\MS.h"

/*  标准事件    */
const ST_OAD STANDEVENT[] ={    
	{{0x2022},0x02,0x00},    /*  事件记录序号   */
	{{0x201e},0x02,0x00},    /*  事件发生时间   */
	{{0x2020},0x02,0x00},    /*  事件结束时间   */
	{{0x2024},0x02,0x00},    /*  事件发生源     */
	{{0x3300},0x02,0x00},    /*  事件上报状态   */
};

/*  编程事件    */
const ST_OAD PROGRAMEVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  事件记录序号   */
	{{0x201e},0x02,0x00},    /*  事件发生时间   */
	{{0x2020},0x02,0x00},    /*  事件结束时间   */
	{{0x2024},0x02,0x00},    /*  事件发生源      */
	{{0x3300},0x02,0x00},    /*  事件上报状态      */
	{{0x3302},0x02,0x06},    /*  编程对象列表      */

};

/*时段表编程事件*/
const ST_OAD DAILY_SLOT_PROGRAMEVENT[] ={       						
	{{0x2022},0x02,0x00},    /*  1.事件记录序号   */
	{{0x201e},0x02,0x00},    /*  2.事件发生时间   */
	{{0x2020},0x02,0x00},    /*  3.事件结束时间   */
	{{0x2024},0x02,0x00},    /*  4.事件发生源      */
	{{0x3300},0x02,0x00},    /*  5.事件上报状态      */
	{{0x3314},0x02,0x06},    /*  6.编程时段表对象    OAD*/
	{{0x3314},0x02,0x07},    /*  7.编程前时段表  日时段表*/

};

/*  异常插卡事件记录单元  */
const ST_OAD ERRORCARDEVENT[] ={       					
	{{0x2022},0x02,0x00},    /*  1.事件记录序号   */
	{{0x201e},0x02,0x00},    /*  2.事件发生时间   */
	{{0x2020},0x02,0x00},    /*  3.事件结束时间   */
	{{0x2024},0x02,0x00},    /*  4.事件发生源      */
	{{0x3300},0x02,0x00},    /*  5.事件上报状态      */
	{{0x3310},0x02,0x06},    /*  6.卡序列号	octet-string     */
	{{0x3310},0x02,0x07},    /*  7.插卡错误信息字	unsigned */
	{{0x3310},0x02,0x08},    /*  8.插卡操作命令头	octet-string */
	{{0x3310},0x02,0x09},    /*  9.插卡错误响应状态  long-unsigned */
};

/*  退费事件记录单元  */
const ST_OAD REFUNDEVENT[] ={       			
	{{0x2022},0x02,0x00},    /*  1.事件记录序号   */
	{{0x201e},0x02,0x00},    /*  2.事件发生时间   */
	{{0x2020},0x02,0x00},    /*  3.事件结束时间   */
	{{0x2024},0x02,0x00},    /*  4.事件发生源      */
	{{0x3300},0x02,0x00},    /*  5.事件上报状态      */
	{{0x3311},0x02,0x06},    /*  6.退费金额      double-long-unsigned  */

};

/*公共节假日编程事件*/
const ST_OAD HOLLIDAY_PROGRAMEVENT[] ={       						  
	{{0x2022},0x02,0x00},    /*  1.事件记录序号   */
	{{0x201e},0x02,0x00},    /*  2.事件发生时间   */
	{{0x2020},0x02,0x00},    /*  3.事件结束时间   */
	{{0x2024},0x02,0x00},    /*  4.事件发生源      */
	{{0x3300},0x02,0x00},    /*  5.事件上报状态      */
	{{0x3315},0x02,0x06},    /*  6.编程节假日对象 OAD         */
    {{0x3315},0x02,0x07},    /*  7.编程前节假日内容  公共假日 */
};

#ifdef THREE_PHASE
/*  需量超限事件    */
const ST_OAD DEMANDOVEREVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  事件记录序号   */
	{{0x201e},0x02,0x00},    /*  事件发生时间   */
	{{0x2020},0x02,0x00},    /*  事件结束时间   */
	{{0x2024},0x02,0x00},    /*  事件发生源      */
	{{0x3300},0x02,0x00},    /*  事件上报状态      */
	{{0x3308},0x02,0x06},    /*  超限期间正向有功需量最大值      */
	{{0x3308},0x02,0x07},    /*  超限期间需量最大值发生时间      */
};
#endif

/*  停/上电事件    */
const ST_OAD POWEREVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  事件记录序号   */
	{{0x201e},0x02,0x00},    /*  事件发生时间   */
	{{0x2020},0x02,0x00},    /*  事件结束时间   */
	{{0x2024},0x02,0x00},    /*  事件发生源      */
	{{0x3300},0x02,0x00},    /*  事件上报状态      */
	{{0x3309},0x02,0x06},    /*  属性标志      */
};

 /*  事件清零事件记录    */
const ST_OAD EVENTCLEAREVENT[] ={       						
	{{0x2022},0x02,0x00},    /*  事件记录序号   */
	{{0x201e},0x02,0x00},    /*  事件发生时间   */
	{{0x2020},0x02,0x00},    /*  事件结束时间   */
	{{0x2024},0x02,0x00},    /*  事件发生源      */
	{{0x3300},0x02,0x00},    /*  事件上报状态      */
	{{0x330c},0x02,0x06},    /*  事件清零列表      */

};

/*  冻结记录    */
const ST_OAD FROZENEVENT[] ={       						 
	
	{{0x2023},0x02,0x00},    /*  冻结记录序号   */
	{{0x2021},0x02,0x00},    /*  数据冻结时间   */

};

/*  24类事件记录    */
const ST_OAD FIXCLASS24EVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  事件记录序号   */
	{{0x201e},0x02,0x00},    /*  事件发生时间   */
	{{0x2020},0x02,0x00},    /*  事件结束时间   */
	{{0x3300},0x02,0x00},    /*  事件上报状态      */
};


const ST_Fixed_RCSD EVENT_FIXED_RCSD[]=
{
    #ifdef THREE_PHASE
	{0x3009,7,DEMANDOVEREVENT},    /*  3009电能表正向有功需量超限事件   */
	{0x300A,7,DEMANDOVEREVENT},    /*  300A电能表反向有功需量超限事件   */
    #endif
	{0x3012,6,PROGRAMEVENT}, 	   /*  3012电能表编程事件   */
	{0x3015,6,EVENTCLEAREVENT},	   /*  3015电能表事件清零事件   */ 
	{0x3017,7,DAILY_SLOT_PROGRAMEVENT},   /*  3017电能表时段表事件记录单元     */
	{0x3021,7,HOLLIDAY_PROGRAMEVENT},     /*  3021电能表节假日表事件记录单元   */
	{0x3027,9,ERRORCARDEVENT},      /*  3027异常插卡事件记录单元  */
	{0x3029,6,REFUNDEVENT}          /*  3029退费事件记录单元  */
};    

/*****************************************************************************
 函 数 名  : Get_CaptureList
 功能描述  : 获取关联对象内容
 输入参数  : ST_Next_Frame_Record_Data *pGet_Record_Data_Buf  
             INT8U *pbuf                                      
 输出参数  : 关联对象个数和关联对象内容
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 03 05
    作    者   : xyt
    修改内容   : 新生成函数

先获取固定关联对象内容，再获取可变的关联对象内容
*****************************************************************************/
void Get_CaptureList(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,INT8U *pbuf)
{
    ST_OAD Oad;
    INT8U num,Fixlistnum,i;
    ST_OAD *pOadlist;
    INT16U Oi;
    const ST_OAD* pFixOad;
    INT16U Obis;
    INT8U len_data;
    
    LIB_MemCpy((INT8U*)&pGet_Record_Data_Buf->Oad, (INT8U*)&Oad, sizeof(ST_OAD));

    Oi = Oad.Oi.asInt;

    Oad.AId = 3;  /*7类与冻结类关联对象属性 AID=3*/
    
    /*1.获取固定关联对象*/
    
    /*24类分项事件*/
    if(OI_OVER_PWR_EVENT>=Oi)
    {
        Oad.AId    = 2;
        Fixlistnum = ARRAY_SIZE(FIXCLASS24EVENT);
        pFixOad    = FIXCLASS24EVENT;        
    } 
    #ifdef THREE_PHASE
    else if(OI_OVER_R_DEMAND_EVENT == Oi)
    {
        Oad.AId    = 2;
        Fixlistnum = ARRAY_SIZE(DEMANDOVEREVENT);
        pFixOad    = DEMANDOVEREVENT;        

    }   
    #endif
    else if((OI_MMT_FRZ<=Oi)&&(OI_METER_RST_EVENT>Oi)) /*冻结类*/
    {
        Fixlistnum = ARRAY_SIZE(FROZENEVENT);
        pFixOad    = FROZENEVENT;  
    }  
    else
    {
       Fixlistnum = ARRAY_SIZE(STANDEVENT);
       pFixOad    = STANDEVENT;  
       
       for(i=0;i<ARRAY_SIZE(EVENT_FIXED_RCSD);i++)
       {
            if(Oi == EVENT_FIXED_RCSD[i].Oi)
            {
                Fixlistnum = EVENT_FIXED_RCSD[i].Fixed_RCSD_Num;
                pFixOad    = EVENT_FIXED_RCSD[i].Pointer; 
            }    
       }  
    }    

    LIB_MemCpy((INT8U*)pFixOad, (INT8U*)pGet_Record_Data_Buf->oadlist, Fixlistnum*sizeof(ST_OAD));

    pOadlist = &pGet_Record_Data_Buf->oadlist[Fixlistnum];

    /*2.获取底层关联对象个数*/
    PP_OadToObis(&Oad, &Obis, NULL, NULL);
    
    /*2.获取底层关联对象*/
    len_data = (INT8U)GetSingle(Obis,pbuf); 

    if(Oad.Oi.asInt>=OI_MMT_FRZ)
    {
        num = len_data>>3;
        for(i=0;i<num;i++)
        {
           LIB_MemCpy(pbuf+i*8+2, (INT8U*)pOadlist+i*4, sizeof(ST_OAD)); 
        }    
    }
    else
    {    
        num = len_data>>2;
        LIB_MemCpy(pbuf, (INT8U*)pOadlist, num*sizeof(ST_OAD));
    }
    
    pGet_Record_Data_Buf->OadListNumber = num + Fixlistnum;
    
}


/*编码RCSD*/
INT8U *EnCodeRCSD(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,INT8U *pdata)
{
	INT8U i,num;
    INT8U *pdst = pdata;
	
    pdst = EnCodeOAD(&pGet_Record_Data_Buf->Oad, pdst);
    
	num  = pGet_Record_Data_Buf->OadListNumber;

    *pdst++ = num;
	
	for(i = 0;i < num;i ++)
	{
		*pdst++ = 0;   /*OAD CHOICE*/
		pdst = EnCodeOAD(&pGet_Record_Data_Buf->oadlist[i], pdst);
	}

    return pdst;
    
}


/*****************************************************************************
 函 数 名  : encode_record_dar
 功能描述  : 打包RCSD结果
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 18
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *encode_record_dar(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,INT8U *pdata,INT8U DarValue)
{
    INT8U *pdst = pdata;
    
    pdst = EnCodeRCSD(pGet_Record_Data_Buf,pdst);

    *pdst++ = 0;

    *pdst++ = DarValue;    

    return pdst;
    
}


/*用于一些变量或参变量中没有定义的对象，但事件记录又需要的OAD对象，以便找到编码方法*/
const _Event_ObjectList EVENTOBJECTLIST[] =
{
		{{{0x3302},0x02,0x06},AR_OAD},     /*  1.编程对象列表 */
#ifdef THREE_PHASE
		{{{0x3308},0x02,0x06},TYPE_U32},   /*  2.超限期间正向有功需量最大值 */	
		{{{0x3308},0x02,0x07},TYPE_DATES}, /*  3.超限期间需量最大值发生时间 */
#endif
		{{{0x3309},0x02,0x06},TYPE_BITS_8}, /*  4.属性标志 */   /*停上电事件记录单元*/
		{{{0x330c},0x02,0x06},AR_OMD},      /*  5.事件清零列表 */
		{{{0x3314},0x02,0x06},TYPE_OAD},         /*  6.编程前时段表  OAD*/
		{{{0x3314},0x02,0x07},AR_8_ST_3_DAILY_SLOT_TABLE},  /*  7.编程前时段表  日时段表*/
		{{{0x3315},0x02,0x06},TYPE_OAD},         /* 8. 编程节假日对象    OAD*/
		{{{0x3315},0x02,0x07},ST_2_DATE_U8},       /*  9.编程前节假日内容  公共假日*/
		{{{0x3310},0x02,0x06},TYPE_OCTS_8},   /*  10.卡序列号	octet-string(5)*/
		{{{0x3310},0x02,0x07},TYPE_U8},       /*  11.插卡错误信息字	unsigned */
		{{{0x3310},0x02,0x08},TYPE_OCTS_5},    /*  12.插卡操作命令头	octet-string(8) */
		{{{0x3310},0x02,0x09},TYPE_U16},       /*  13.插卡错误响应状态  long-unsigned */
		{{{0x3311},0x02,0x06},TYPE_U32},       /*  14.退费金额      double-long-unsigned */
		{{{0x3027},0x0B,0x00},TYPE_U32},       /*  15.非法插卡总次数 double-long-unsigned */	
};

#define EVENTOBJECTLISTMAX ARRAY_SIZE(EVENTOBJECTLIST)


/*****************************************************************************
 函 数 名  : Get_Event_Data_len
 功能描述  : 获取事件类一些特殊的打包处理
 输入参数  : ST_OAD Oad           
             SUB_OBJECT *pobject  
             INT8U  *pret         
 输出参数  : 无
 返 回 值  : 0:不是事件特殊OAD，非0，是特殊事件OAD
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 01 08
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Get_Event_Data_len(ST_OAD Oad,SUB_OBJECT *pobject)
{
    INT8U i;
    ST_OAD TmpOad;
        
    for ( i = 0 ; i < EVENTOBJECTLISTMAX; i++ )
    {
        LIB_MemCpy((INT8U * )&EVENTOBJECTLIST[i].Oad, (INT8U * )&TmpOad, sizeof(ST_OAD));
        if((TmpOad.Oi.asInt == Oad.Oi.asInt)&&
           (TmpOad.AId      == Oad.AId)&&
           (TmpOad.IId      == Oad.IId))
        {
           pobject->Pointer = EVENTOBJECTLIST[i].pointer;
           return Cal_EnCode_Data_Len((ST_ENCODE_INFO *)pobject->Pointer,0); 
        }   
    }
    
    
    return 0;
    
}

/*****************************************************************************
 函 数 名  : encode_record_data
 功能描述  : 打包记录数据，如果越界，返回ret失败
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 18
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *encode_record_data(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,
                               ST_GetRcrdOut *pGetRcrdOut, INT8U *pdata,INT8U *pRet)
{
    INT8U i,tmp_len,ret;
    INT8U *pdst = pdata;
    INT8U *pLenList = pGetRcrdOut->pLenList;
    INT8U *pBuff    = pGetRcrdOut->pBuff;
    SUB_OBJECT object;
    INT16U len,Oi;
    ST_OAD Oad;

    for(i=0;i<pGet_Record_Data_Buf->OadListNumber;i++)
    {   
        LIB_MemCpy((INT8U*) &pGet_Record_Data_Buf->oadlist[i], (INT8U*)&Oad, sizeof(ST_OAD));
        Oad.AId = Oad.AId & MASK_AID_SELECT;

        Oi      = Oad.Oi.asInt;
        
        /*事件记录的时区表，时段表，都是固定最大个数*/
        if((OI_CURRENT_SLOT_TABLE<=Oi)&&(OI_BAK_DAILY_SLOT_TABLE>=Oi))
        {
            Fix_Len_Varible = 0xff;
        }    
        else
        {
            Fix_Len_Varible = 0;
        }    
        
        tmp_len       = pLenList[i];
        LenSingleData = tmp_len;
        if(0 == tmp_len)
        {
            len = 1;
        } 
        else
        {
            len =  Get_Event_Data_len(Oad,&object);
            if(0 == len)  /*先用长度判断看,如果不行，则用ret判断*/
            {                
                len =  Get_OAD_Data_len(Oad,&object,&ret);  
            }
            else
            {
               Oad.IId = 0;   /*事件特殊类用0，表示整体编码 */
            }    
        }    

        if(pdst+len>pEndEncodeAddr)
        {
            *pRet = DAR_3_NOACESS;  
            Fix_Len_Varible = 0;
            return pdata;      /*注:返回pdata,以便打包DAR*/
        }  
        else
        {
            if(0 == tmp_len)
            {
                *pdst++=null_data;
            }
            else
            {     
                pdst += EnCode_Data((ST_ENCODE_INFO *)object.Pointer,pBuff,pdst,Oad.IId);
                pBuff += tmp_len;
            }
        }            
    }
    
    Fix_Len_Varible = 0;
    
    return pdst;  
}    


/*****************************************************************************
 函 数 名  : Prepare_Get_Record
 功能描述  : 准备获取记录的数据
 输入参数  : ST_Next_Frame_Record_Data *pGet_Record_Data  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 18
    作    者   : xyt
    修改内容   : 新生成函数
方法9:先读取关联对象。
方法2:先获取最大总条数。
方法1:不用处理。
*****************************************************************************/
void Prepare_Get_Record(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,
                             ST_Get_Record_Data *pGet_Record_Data,void *pdout)
{
     /*准备数据*/
     /*如果CSD =0,则需要获取关联对象*/
     if(0 == pGet_Record_Data_Buf->OadListNumber)
     {
         Get_CaptureList(pGet_Record_Data_Buf,pdout);
     } 

     /*注:下面的常数8(poadlist和pDdataFilter指针大小)，如果换平台，可能不一样*/   
     LIB_MemCpy((INT8U*)pGet_Record_Data_Buf, (INT8U*)pGet_Record_Data, sizeof(ST_Get_Record_Data)-8); 

     pGet_Record_Data->poadlist     = pGet_Record_Data_Buf->oadlist;
     pGet_Record_Data->pDdataFilter = pGet_Record_Data_Buf->DdataFilter;
    
    /*1.获取记录总条数*/
    if(0 == pGet_Record_Data_Buf->TotalLine)  /*之所以这样判断，是因为可能是后续帧进来的*/
    {
        pGet_Record_Data_Buf->TotalLine = (INT8U)GetRcrd(pGet_Record_Data->Obis_event_freeze,(void *)pGet_Record_Data,(void *)pdout,MAX_ONE_RECORD_DATA_LEN);
        pGet_Record_Data->TotalLine     = pGet_Record_Data_Buf->TotalLine;         
    }     
}

/*****************************************************************************
 函 数 名  : Encode_Record_List_Dar
 功能描述  : 编码记录型数据结果
 输入参数  : 无
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 03 13
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U * Encode_Record_List_Dar(INT8U *pdata)
{
    INT8U *pdst = pdata;
    INT8U j;
    ST_Next_Frame_Record_Data *pGet_Record_Data_Buf;    
    
    for(j=0;j<Record_Num_List;j++)
    {  
        pGet_Record_Data_Buf = (ST_Next_Frame_Record_Data *)gBak_data[j];

        LIB_MemCpy((INT8U * )&pGet_Record_Data_Buf->Oad,(INT8U *) &CurOad, sizeof(ST_OAD)); 
        
        pdst = encode_record_dar(pGet_Record_Data_Buf,pdst,DAR_3_NOACESS);  
     }    

     return pdst;
}    


/*****************************************************************************
 函 数 名  : get_record_process
 功能描述  : get record 数据获取处理
 输入参数  : INT8U *pdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2017 12 08
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *get_record_process(INT8U *pdata)
{
    INT8U i,j,Ret = 0;
    ST_Next_Frame_Record_Data *pGet_Record_Data_Buf;
    ST_Get_Record_Data Get_Record_Data;
    INT8U temp_data[MAX_ONE_RECORD_DATA_LEN];
    INT8U Leng[MAXCSD];
    ST_GetRcrdOut GetRcrdOut;
    INT8U *pdst = pdata; 
    SUB_OBJECT Object;

    GetRcrdOut.pBuff   = temp_data;
    GetRcrdOut.pLenList= Leng;
        
    for(j=0;j<Record_Num_List;j++)
    {  
        pGet_Record_Data_Buf = (ST_Next_Frame_Record_Data *)gBak_data[j];

        LIB_MemCpy((INT8U * )&pGet_Record_Data_Buf->Oad,(INT8U *) &CurOad, sizeof(ST_OAD)); 
        
        /*1.判断数据合法性*/
        if(DAR_0_OK!=gDarDecode)  /*这里要不要分list,后续有必要再考虑*/
        {
            pdst = encode_record_dar(pGet_Record_Data_Buf,pdst,gDarDecode);  
            continue;
        }    

        /*增加判断是否有定义和权限控制,因为get_record_process是直接进来的，没有判断OAD合法性*/
        LIB_MemCpy((INT8U * )&pGet_Record_Data_Buf->Oad, (INT8U * )&Object.Oad, sizeof(ST_OAD));
        //Object.Oad.AId = 2;   /*2018-02-02 为什么要置为2?*/
        Object.Class   = 0;

        Ret = Check_Object(EN_GETREQUEST,&Object,NULL);        
        if(DAR_0_OK != Ret)
        {
            pdst = encode_record_dar(pGet_Record_Data_Buf,pdata,Ret);
            continue;
        }       
        /*增加判断是否有定义和权限控制 END*/
        
        /*3.准备读取数据内容*/
        Prepare_Get_Record(pGet_Record_Data_Buf,&Get_Record_Data,temp_data);

        /*4.打包RCSD*/
        if(0 == Get_Record_Data.CurrentLine)
        {
           pdst = EnCodeRCSD(pGet_Record_Data_Buf,pdst);
           *pdst++ = 1;            /*记录数据*/ 
           Get_Record_Data.CurrentLine       = 1;
           pGet_Record_Data_Buf->CurrentLine = 1;

           /*5.打包记录条数*/
            /*记录条数为0*/
            if(0 == Get_Record_Data.TotalLine)
            {
                *pdst++ = null_data;    /*空*/      
                continue;
            } 
            /*能读取下发，说明能打包结果，所以打包结果，暂不判断数据是否越界，以后有机会，再完善判断下数据长度*/
            else
            {   
                *pdst++ = Get_Record_Data.TotalLine;    /*记录条数*/  /*这里要打包成SEQUECE NUM*/ 
            }    
        }    

        /*6.打包具体记录内容*/
        i= pGet_Record_Data_Buf->CurrentLine;         
        
        for(;i<=pGet_Record_Data_Buf->TotalLine;i++)
        {     
            GetRcrd(Get_Record_Data.Obis_event_freeze,&Get_Record_Data,&GetRcrdOut,MAX_ONE_RECORD_DATA_LEN);             

            pdst = encode_record_data(pGet_Record_Data_Buf,&GetRcrdOut,pdst,&Ret);
            
            if(DAR_0_OK!=Ret)
            {
               
                #if APP_NEXT_FRAME_NO_ACCESS
                /*这里用直接返回，或是继续用后续帧*/
                pdst = Encode_Record_List_Dar(pdata);  /*这里故意用pdata作为传参，以便打包数据结果*/
                #endif               
                
                /*更新，以便下次再读*/
                return pdst;                  
            }  

            pGet_Record_Data_Buf->CurrentLine = i;   /*更新，以便下次从新的记录行开始读数,因为encode_record_data如果返回错误，则表示此次获取的数据未打包，所以取值i*/          

            Get_Record_Data.CurrentLine       = i+1; /*更新，以便下次从新的记录行开始读数*/          
        }          
        
    } 

    return pdst;

}  

/*显示专用列筛选，输出某个对象数据，并返回数据长度*/
INT8U PP_RecordColumnFilterDisp(ST_OAD *pOad,ST_OAD *pRoad, INT8U *pbuf)
{
   ST_Get_Record_Data Get_Record_Data;
   ST_OAD Oad;
   INT8U  temp_data[2]={1,E_SELELTOR9_DISP};     /*方法9，第1字节表示上几次，第2字节非0，表示是显示*/
   INT16U Obis[MAX_ID_LIST_NUM];
   
   INT8U  Leng = 0;
   ST_GetRcrdOut GetRcrdOut;

   GetRcrdOut.pLenList = &Leng;
   GetRcrdOut.pBuff    = pbuf;

   LIB_MemCpy((INT8U*)pOad, (INT8U*)&Oad, sizeof(ST_OAD));
   temp_data[0] = Oad.IId;     /*上几次*/
   if(0 == temp_data[0])
   {
        temp_data[0] = 1;
   }  

   Oad.IId = 0;
   
   PP_OadToObis(&Oad, Obis, NULL, NULL);   /*这里OBIS看要不要容错，防越界，在PP_OadToObis()中会判断是否定义*/
   
   Get_Record_Data.TotalLine   = 0;              /*要读取的总行数(记录条数) =0 时，底层直接返回记录条数*/
   Get_Record_Data.CurrentLine = temp_data[0];   /*要读取的当前行数(记录条数) =0:需要打包RCSD*/
      
   Get_Record_Data.Obis_event_freeze = Obis[0];  /*事件或冻结的OBIS*/
   
   Get_Record_Data.SelectNumber  = 9;    /*筛选方法*/
   Get_Record_Data.OadListNumber = 1;    /*筛选的列OAD个数*/
         
   Get_Record_Data.poadlist     = pRoad;        /*筛选的列OAD,这里看要不要支持ROAD*/

   Get_Record_Data.pDdataFilter = temp_data;     /*方法9，第2字节非0，表示是显示*/

   /*没记录数，看怎么处理，是把数据置0还是不显示?*/ 
   if(0 ==GetRcrd(Obis[0],(void *)&Get_Record_Data,(void *)&GetRcrdOut,MAX_ONE_OAD_DATA_LEN))
   {
      return 0;
   } 

   Get_Record_Data.TotalLine   = 1;      /*要读取的总行数(记录条数) =0 时，底层直接返回记录条数*/

   GetRcrd(Obis[0],(void *)&Get_Record_Data,(void *)&GetRcrdOut,MAX_ONE_OAD_DATA_LEN);
    
   return Leng;
    
}


