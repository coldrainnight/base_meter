/*"*****************************************************************************"*/
/*"  FileName: [pp_record.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  �¼��붳���¼��ش������ļ�   "*/
/*"  Version:         "*/
/*"  V1.0       ��ʼ�汾"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "App\UserInterface\PP\PP_698\Class\class_comm.h"
#include "App\message\MS.h"

/*  ��׼�¼�    */
const ST_OAD STANDEVENT[] ={    
	{{0x2022},0x02,0x00},    /*  �¼���¼���   */
	{{0x201e},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  �¼�����Դ     */
	{{0x3300},0x02,0x00},    /*  �¼��ϱ�״̬   */
};

/*  ����¼�    */
const ST_OAD PROGRAMEVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  �¼���¼���   */
	{{0x201e},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  �¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  �¼��ϱ�״̬      */
	{{0x3302},0x02,0x06},    /*  ��̶����б�      */

};

/*ʱ�α����¼�*/
const ST_OAD DAILY_SLOT_PROGRAMEVENT[] ={       						
	{{0x2022},0x02,0x00},    /*  1.�¼���¼���   */
	{{0x201e},0x02,0x00},    /*  2.�¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  3.�¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  4.�¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  5.�¼��ϱ�״̬      */
	{{0x3314},0x02,0x06},    /*  6.���ʱ�α����    OAD*/
	{{0x3314},0x02,0x07},    /*  7.���ǰʱ�α�  ��ʱ�α�*/

};

/*  �쳣�忨�¼���¼��Ԫ  */
const ST_OAD ERRORCARDEVENT[] ={       					
	{{0x2022},0x02,0x00},    /*  1.�¼���¼���   */
	{{0x201e},0x02,0x00},    /*  2.�¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  3.�¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  4.�¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  5.�¼��ϱ�״̬      */
	{{0x3310},0x02,0x06},    /*  6.�����к�	octet-string     */
	{{0x3310},0x02,0x07},    /*  7.�忨������Ϣ��	unsigned */
	{{0x3310},0x02,0x08},    /*  8.�忨��������ͷ	octet-string */
	{{0x3310},0x02,0x09},    /*  9.�忨������Ӧ״̬  long-unsigned */
};

/*  �˷��¼���¼��Ԫ  */
const ST_OAD REFUNDEVENT[] ={       			
	{{0x2022},0x02,0x00},    /*  1.�¼���¼���   */
	{{0x201e},0x02,0x00},    /*  2.�¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  3.�¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  4.�¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  5.�¼��ϱ�״̬      */
	{{0x3311},0x02,0x06},    /*  6.�˷ѽ��      double-long-unsigned  */

};

/*�����ڼ��ձ���¼�*/
const ST_OAD HOLLIDAY_PROGRAMEVENT[] ={       						  
	{{0x2022},0x02,0x00},    /*  1.�¼���¼���   */
	{{0x201e},0x02,0x00},    /*  2.�¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  3.�¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  4.�¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  5.�¼��ϱ�״̬      */
	{{0x3315},0x02,0x06},    /*  6.��̽ڼ��ն��� OAD         */
    {{0x3315},0x02,0x07},    /*  7.���ǰ�ڼ�������  �������� */
};

#ifdef THREE_PHASE
/*  ���������¼�    */
const ST_OAD DEMANDOVEREVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  �¼���¼���   */
	{{0x201e},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  �¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  �¼��ϱ�״̬      */
	{{0x3308},0x02,0x06},    /*  �����ڼ������й��������ֵ      */
	{{0x3308},0x02,0x07},    /*  �����ڼ��������ֵ����ʱ��      */
};
#endif

/*  ͣ/�ϵ��¼�    */
const ST_OAD POWEREVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  �¼���¼���   */
	{{0x201e},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  �¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  �¼��ϱ�״̬      */
	{{0x3309},0x02,0x06},    /*  ���Ա�־      */
};

 /*  �¼������¼���¼    */
const ST_OAD EVENTCLEAREVENT[] ={       						
	{{0x2022},0x02,0x00},    /*  �¼���¼���   */
	{{0x201e},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2024},0x02,0x00},    /*  �¼�����Դ      */
	{{0x3300},0x02,0x00},    /*  �¼��ϱ�״̬      */
	{{0x330c},0x02,0x06},    /*  �¼������б�      */

};

/*  �����¼    */
const ST_OAD FROZENEVENT[] ={       						 
	
	{{0x2023},0x02,0x00},    /*  �����¼���   */
	{{0x2021},0x02,0x00},    /*  ���ݶ���ʱ��   */

};

/*  24���¼���¼    */
const ST_OAD FIXCLASS24EVENT[] ={       						 
	{{0x2022},0x02,0x00},    /*  �¼���¼���   */
	{{0x201e},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x2020},0x02,0x00},    /*  �¼�����ʱ��   */
	{{0x3300},0x02,0x00},    /*  �¼��ϱ�״̬      */
};


const ST_Fixed_RCSD EVENT_FIXED_RCSD[]=
{
    #ifdef THREE_PHASE
	{0x3009,7,DEMANDOVEREVENT},    /*  3009���ܱ������й����������¼�   */
	{0x300A,7,DEMANDOVEREVENT},    /*  300A���ܱ����й����������¼�   */
    #endif
	{0x3012,6,PROGRAMEVENT}, 	   /*  3012���ܱ����¼�   */
	{0x3015,6,EVENTCLEAREVENT},	   /*  3015���ܱ��¼������¼�   */ 
	{0x3017,7,DAILY_SLOT_PROGRAMEVENT},   /*  3017���ܱ�ʱ�α��¼���¼��Ԫ     */
	{0x3021,7,HOLLIDAY_PROGRAMEVENT},     /*  3021���ܱ�ڼ��ձ��¼���¼��Ԫ   */
	{0x3027,9,ERRORCARDEVENT},      /*  3027�쳣�忨�¼���¼��Ԫ  */
	{0x3029,6,REFUNDEVENT}          /*  3029�˷��¼���¼��Ԫ  */
};    

/*****************************************************************************
 �� �� ��  : Get_CaptureList
 ��������  : ��ȡ������������
 �������  : ST_Next_Frame_Record_Data *pGet_Record_Data_Buf  
             INT8U *pbuf                                      
 �������  : ������������͹�����������
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 03 05
    ��    ��   : xyt
    �޸�����   : �����ɺ���

�Ȼ�ȡ�̶������������ݣ��ٻ�ȡ�ɱ�Ĺ�����������
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

    Oad.AId = 3;  /*7���붳��������������� AID=3*/
    
    /*1.��ȡ�̶���������*/
    
    /*24������¼�*/
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
    else if((OI_MMT_FRZ<=Oi)&&(OI_METER_RST_EVENT>Oi)) /*������*/
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

    /*2.��ȡ�ײ�����������*/
    PP_OadToObis(&Oad, &Obis, NULL, NULL);
    
    /*2.��ȡ�ײ��������*/
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


/*����RCSD*/
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
 �� �� ��  : encode_record_dar
 ��������  : ���RCSD���
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 18
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U *encode_record_dar(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,INT8U *pdata,INT8U DarValue)
{
    INT8U *pdst = pdata;
    
    pdst = EnCodeRCSD(pGet_Record_Data_Buf,pdst);

    *pdst++ = 0;

    *pdst++ = DarValue;    

    return pdst;
    
}


/*����һЩ������α�����û�ж���Ķ��󣬵��¼���¼����Ҫ��OAD�����Ա��ҵ����뷽��*/
const _Event_ObjectList EVENTOBJECTLIST[] =
{
		{{{0x3302},0x02,0x06},AR_OAD},     /*  1.��̶����б� */
#ifdef THREE_PHASE
		{{{0x3308},0x02,0x06},TYPE_U32},   /*  2.�����ڼ������й��������ֵ */	
		{{{0x3308},0x02,0x07},TYPE_DATES}, /*  3.�����ڼ��������ֵ����ʱ�� */
#endif
		{{{0x3309},0x02,0x06},TYPE_BITS_8}, /*  4.���Ա�־ */   /*ͣ�ϵ��¼���¼��Ԫ*/
		{{{0x330c},0x02,0x06},AR_OMD},      /*  5.�¼������б� */
		{{{0x3314},0x02,0x06},TYPE_OAD},         /*  6.���ǰʱ�α�  OAD*/
		{{{0x3314},0x02,0x07},AR_8_ST_3_DAILY_SLOT_TABLE},  /*  7.���ǰʱ�α�  ��ʱ�α�*/
		{{{0x3315},0x02,0x06},TYPE_OAD},         /* 8. ��̽ڼ��ն���    OAD*/
		{{{0x3315},0x02,0x07},ST_2_DATE_U8},       /*  9.���ǰ�ڼ�������  ��������*/
		{{{0x3310},0x02,0x06},TYPE_OCTS_8},   /*  10.�����к�	octet-string(5)*/
		{{{0x3310},0x02,0x07},TYPE_U8},       /*  11.�忨������Ϣ��	unsigned */
		{{{0x3310},0x02,0x08},TYPE_OCTS_5},    /*  12.�忨��������ͷ	octet-string(8) */
		{{{0x3310},0x02,0x09},TYPE_U16},       /*  13.�忨������Ӧ״̬  long-unsigned */
		{{{0x3311},0x02,0x06},TYPE_U32},       /*  14.�˷ѽ��      double-long-unsigned */
		{{{0x3027},0x0B,0x00},TYPE_U32},       /*  15.�Ƿ��忨�ܴ��� double-long-unsigned */	
};

#define EVENTOBJECTLISTMAX ARRAY_SIZE(EVENTOBJECTLIST)


/*****************************************************************************
 �� �� ��  : Get_Event_Data_len
 ��������  : ��ȡ�¼���һЩ����Ĵ������
 �������  : ST_OAD Oad           
             SUB_OBJECT *pobject  
             INT8U  *pret         
 �������  : ��
 �� �� ֵ  : 0:�����¼�����OAD����0���������¼�OAD
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 01 08
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
 �� �� ��  : encode_record_data
 ��������  : �����¼���ݣ����Խ�磬����retʧ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 18
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
        
        /*�¼���¼��ʱ����ʱ�α����ǹ̶�������*/
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
            if(0 == len)  /*���ó����жϿ�,������У�����ret�ж�*/
            {                
                len =  Get_OAD_Data_len(Oad,&object,&ret);  
            }
            else
            {
               Oad.IId = 0;   /*�¼���������0����ʾ������� */
            }    
        }    

        if(pdst+len>pEndEncodeAddr)
        {
            *pRet = DAR_3_NOACESS;  
            Fix_Len_Varible = 0;
            return pdata;      /*ע:����pdata,�Ա���DAR*/
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
 �� �� ��  : Prepare_Get_Record
 ��������  : ׼����ȡ��¼������
 �������  : ST_Next_Frame_Record_Data *pGet_Record_Data  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2017 12 18
    ��    ��   : xyt
    �޸�����   : �����ɺ���
����9:�ȶ�ȡ��������
����2:�Ȼ�ȡ�����������
����1:���ô���
*****************************************************************************/
void Prepare_Get_Record(ST_Next_Frame_Record_Data *pGet_Record_Data_Buf,
                             ST_Get_Record_Data *pGet_Record_Data,void *pdout)
{
     /*׼������*/
     /*���CSD =0,����Ҫ��ȡ��������*/
     if(0 == pGet_Record_Data_Buf->OadListNumber)
     {
         Get_CaptureList(pGet_Record_Data_Buf,pdout);
     } 

     /*ע:����ĳ���8(poadlist��pDdataFilterָ���С)�������ƽ̨�����ܲ�һ��*/   
     LIB_MemCpy((INT8U*)pGet_Record_Data_Buf, (INT8U*)pGet_Record_Data, sizeof(ST_Get_Record_Data)-8); 

     pGet_Record_Data->poadlist     = pGet_Record_Data_Buf->oadlist;
     pGet_Record_Data->pDdataFilter = pGet_Record_Data_Buf->DdataFilter;
    
    /*1.��ȡ��¼������*/
    if(0 == pGet_Record_Data_Buf->TotalLine)  /*֮���������жϣ�����Ϊ�����Ǻ���֡������*/
    {
        pGet_Record_Data_Buf->TotalLine = (INT8U)GetRcrd(pGet_Record_Data->Obis_event_freeze,(void *)pGet_Record_Data,(void *)pdout,MAX_ONE_RECORD_DATA_LEN);
        pGet_Record_Data->TotalLine     = pGet_Record_Data_Buf->TotalLine;         
    }     
}

/*****************************************************************************
 �� �� ��  : Encode_Record_List_Dar
 ��������  : �����¼�����ݽ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : INT8U
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 03 13
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
 �� �� ��  : get_record_process
 ��������  : get record ���ݻ�ȡ����
 �������  : INT8U *pdata  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2017 12 08
    ��    ��   : xyt
    �޸�����   : �����ɺ���

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
        
        /*1.�ж����ݺϷ���*/
        if(DAR_0_OK!=gDarDecode)  /*����Ҫ��Ҫ��list,�����б�Ҫ�ٿ���*/
        {
            pdst = encode_record_dar(pGet_Record_Data_Buf,pdst,gDarDecode);  
            continue;
        }    

        /*�����ж��Ƿ��ж����Ȩ�޿���,��Ϊget_record_process��ֱ�ӽ����ģ�û���ж�OAD�Ϸ���*/
        LIB_MemCpy((INT8U * )&pGet_Record_Data_Buf->Oad, (INT8U * )&Object.Oad, sizeof(ST_OAD));
        //Object.Oad.AId = 2;   /*2018-02-02 ΪʲôҪ��Ϊ2?*/
        Object.Class   = 0;

        Ret = Check_Object(EN_GETREQUEST,&Object,NULL);        
        if(DAR_0_OK != Ret)
        {
            pdst = encode_record_dar(pGet_Record_Data_Buf,pdata,Ret);
            continue;
        }       
        /*�����ж��Ƿ��ж����Ȩ�޿��� END*/
        
        /*3.׼����ȡ��������*/
        Prepare_Get_Record(pGet_Record_Data_Buf,&Get_Record_Data,temp_data);

        /*4.���RCSD*/
        if(0 == Get_Record_Data.CurrentLine)
        {
           pdst = EnCodeRCSD(pGet_Record_Data_Buf,pdst);
           *pdst++ = 1;            /*��¼����*/ 
           Get_Record_Data.CurrentLine       = 1;
           pGet_Record_Data_Buf->CurrentLine = 1;

           /*5.�����¼����*/
            /*��¼����Ϊ0*/
            if(0 == Get_Record_Data.TotalLine)
            {
                *pdst++ = null_data;    /*��*/      
                continue;
            } 
            /*�ܶ�ȡ�·���˵���ܴ����������Դ��������ݲ��ж������Ƿ�Խ�磬�Ժ��л��ᣬ�������ж������ݳ���*/
            else
            {   
                *pdst++ = Get_Record_Data.TotalLine;    /*��¼����*/  /*����Ҫ�����SEQUECE NUM*/ 
            }    
        }    

        /*6.��������¼����*/
        i= pGet_Record_Data_Buf->CurrentLine;         
        
        for(;i<=pGet_Record_Data_Buf->TotalLine;i++)
        {     
            GetRcrd(Get_Record_Data.Obis_event_freeze,&Get_Record_Data,&GetRcrdOut,MAX_ONE_RECORD_DATA_LEN);             

            pdst = encode_record_data(pGet_Record_Data_Buf,&GetRcrdOut,pdst,&Ret);
            
            if(DAR_0_OK!=Ret)
            {
               
                #if APP_NEXT_FRAME_NO_ACCESS
                /*������ֱ�ӷ��أ����Ǽ����ú���֡*/
                pdst = Encode_Record_List_Dar(pdata);  /*���������pdata��Ϊ���Σ��Ա������ݽ��*/
                #endif               
                
                /*���£��Ա��´��ٶ�*/
                return pdst;                  
            }  

            pGet_Record_Data_Buf->CurrentLine = i;   /*���£��Ա��´δ��µļ�¼�п�ʼ����,��Ϊencode_record_data������ش������ʾ�˴λ�ȡ������δ���������ȡֵi*/          

            Get_Record_Data.CurrentLine       = i+1; /*���£��Ա��´δ��µļ�¼�п�ʼ����*/          
        }          
        
    } 

    return pdst;

}  

/*��ʾר����ɸѡ�����ĳ���������ݣ����������ݳ���*/
INT8U PP_RecordColumnFilterDisp(ST_OAD *pOad,ST_OAD *pRoad, INT8U *pbuf)
{
   ST_Get_Record_Data Get_Record_Data;
   ST_OAD Oad;
   INT8U  temp_data[2]={1,E_SELELTOR9_DISP};     /*����9����1�ֽڱ�ʾ�ϼ��Σ���2�ֽڷ�0����ʾ����ʾ*/
   INT16U Obis[MAX_ID_LIST_NUM];
   
   INT8U  Leng = 0;
   ST_GetRcrdOut GetRcrdOut;

   GetRcrdOut.pLenList = &Leng;
   GetRcrdOut.pBuff    = pbuf;

   LIB_MemCpy((INT8U*)pOad, (INT8U*)&Oad, sizeof(ST_OAD));
   temp_data[0] = Oad.IId;     /*�ϼ���*/
   if(0 == temp_data[0])
   {
        temp_data[0] = 1;
   }  

   Oad.IId = 0;
   
   PP_OadToObis(&Oad, Obis, NULL, NULL);   /*����OBIS��Ҫ��Ҫ�ݴ���Խ�磬��PP_OadToObis()�л��ж��Ƿ���*/
   
   Get_Record_Data.TotalLine   = 0;              /*Ҫ��ȡ��������(��¼����) =0 ʱ���ײ�ֱ�ӷ��ؼ�¼����*/
   Get_Record_Data.CurrentLine = temp_data[0];   /*Ҫ��ȡ�ĵ�ǰ����(��¼����) =0:��Ҫ���RCSD*/
      
   Get_Record_Data.Obis_event_freeze = Obis[0];  /*�¼��򶳽��OBIS*/
   
   Get_Record_Data.SelectNumber  = 9;    /*ɸѡ����*/
   Get_Record_Data.OadListNumber = 1;    /*ɸѡ����OAD����*/
         
   Get_Record_Data.poadlist     = pRoad;        /*ɸѡ����OAD,���￴Ҫ��Ҫ֧��ROAD*/

   Get_Record_Data.pDdataFilter = temp_data;     /*����9����2�ֽڷ�0����ʾ����ʾ*/

   /*û��¼��������ô�����ǰ�������0���ǲ���ʾ?*/ 
   if(0 ==GetRcrd(Obis[0],(void *)&Get_Record_Data,(void *)&GetRcrdOut,MAX_ONE_OAD_DATA_LEN))
   {
      return 0;
   } 

   Get_Record_Data.TotalLine   = 1;      /*Ҫ��ȡ��������(��¼����) =0 ʱ���ײ�ֱ�ӷ��ؼ�¼����*/

   GetRcrd(Obis[0],(void *)&Get_Record_Data,(void *)&GetRcrdOut,MAX_ONE_OAD_DATA_LEN);
    
   return Leng;
    
}


