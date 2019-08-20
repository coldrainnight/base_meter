/*"*****************************************************************************"*/
/*"  FileName: protocol_tree.c"*/
/*"  Author: ganjp       Version :  V1.0        Date:2012.1.11"*/
/*"  Description:     ���ļ��ṩ�������ݽ����ӿں���������ͨ�Ŷ���ͨ��д����ʾ���ݽӿ�"*/
/*"                   ���ݴ��ݲ��ò�ѯЭ������ʽ ���ҵ����ݱ�ʶ��Ӧ��Ҷ�ӣ�������Ӧ����"*/
/*"                   �ݴ���     "*/
/*"  Version:         "*/
/*"  V1.0       ��ʼ�汾"*/
/*"  "*/
/*"  Function List:   "*/
/*"    1. PP_CommGetData   ͨ�Ŷ����ݽӿں���"*/
/*"    2. PP_CommSetData   ͨ��д���ݽӿں���"*/
/*"    2. PP_ShowGetData   ��ʾ�����ݽӿں���"*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"  code size  5568byte   table size  0byte   ram size"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      ganjp    2012/1/11     1.0     build this moudle  "*/
/*"*******************************************************************************"*/

#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
//#include "App\UserInterface\display\dispObj.h"


//static INT8U Len_dl645 = 0;
static UN_COMM_ERR Error_Infor;/*"ͨ�Ŵ�����Ϣ����"*/
static UN_ID645 dataId;      /*"645_2007Э�����ݱ�ʶ"*/
static UN_ID645 NodedataId;      /*"���ڽڵ���������ݱ�ʶ�����ھ�����ͬЭ�����Ե�Э�����ѹ������"*/
//static INT8U SetParaFuncFlg;/*"дǰ�����־��һ����ʶ��Ӧ���Ҷ��ʱ��дǰ����ֻ�ڴ����һ��Ҷ�ӵ�ʱ�����"*/
static INT16U fileIndex;    /*"��¼�ļ�����������¼�ļ�ʱ�����ڽ�ѹ�ϼ��μ�¼�ļ�"*/
static INT8U  noFrzDataFlg;/*"�����ɶ���ģʽ��ʱ���������������ģʽ��û��������TRUE,����ΪFALSE"*/
static INT8U SetParaFuncFlg;/*"дǰ�����־��һ����ʶ��Ӧ���Ҷ��ʱ��дǰ����ֻ�ڴ����һ��Ҷ�ӵ�ʱ�����"*/

/*"******************************PUBLIC FUNCTION*******************************"*/
/*"************дǰ��д�󡢶�ǰ������*********************"*/
INT8U setSysTimeHMS(INT8U *pdata);
INT8U setSysTimeYMDW(INT8U *pdata);
INT8U setSysTimeAll(INT8U *pdata);
INT8U readDis(INT8U *pdata);
//INT8U ChkByteDataRange(INT8U *pdata);
//INT8U ChkWordDataRange(INT8U *pdata);
INT8U readNosupportData(INT8U *pdata);

/*"******************************PRIVATE FUNCTION*****************************"*/
static INT8U  ProtocolTreeChk(UN_ID645 *pdataId,TREE_LEAF_LIST *Leaf_adrr_list,
                              INT8U *block_num, INT8U *block_list);
static INT16U Search_Node(CHECK_NODE_RET * nodeAttr);
static INT8U Traverse_Node(INT16U node_addr, INT8U nodeAttr,
		            TREE_LEAF_LIST *Leaf_adrr_list,INT8U *block_num, INT8U *block_list);
static INT8U SearchNodeNesting(INT16U nodeAddrLower,
		                  INT16U * Node_addr, INT8U nodeAttr);
static INT8U Traverse_Node_Nesting(INT16U node_addr,INT8U Nest_level,INT8U *Leaf_num,
		                    INT8U *zip_id, TREE_LEAF_LIST *Leaf_adrr_list,
		                    INT8U *block_num, INT8U *block_list);
static INT16U Get_Node_Data_Comm(TREE_LEAF_LIST *Leaf_adrr_list, INT8U Leaf_Num,
		                  INT16U Start_addr, INT16U MaxLen, INT8U *pBuffer,
		                  INT8U block_num, INT8U *block_list);
#if 0
static void Set_Node_Data_Comm(TREE_LEAF_LIST *Leaf_adrr_list, INT8U Leaf_Num,
		                  INT8U *pBuffer, INT8U PassWord_Level);
#endif
#ifdef DM_DLT645
static INT8U  Get_Leaf_Data_Show(TREE_LEAF_LIST *Leaf_adrr_list,
		                  INT8U Show_leaf, ST_SHOWDATA_OUTPUT *pBuffer);
#endif
static INT16U Get_Leaf_Data(const struct TREE_LEAF FAR_POINTER*Tree_Leaf, INT8U *buff,
                          INT16U Revise,ST_FMT_ATTR *Communal_Save);
static INT16U Get_Leaf_Data_Comm(const struct TREE_LEAF FAR_POINTER*Tree_Leaf, INT8U *buff,
					      INT16U Offset, INT16U MaxLen ,INT16U Revise);
static INT8U ChkNode(const INT8U FAR_POINTER*node);
static INT8U check_nodePoint(INT16U node);
static INT8U ChkNodeID(const struct TREE_NODE FAR_POINTER*node,INT8U uzip);
static INT8U ChkIdRange(INT8U ZipNum, INT8U IdStart, INT8U IdEnd);
static INT8U push_zip_info(INT8U val,INT8U num);
static INT8U set_zip_id(INT8U *zip_id,INT8U num,INT8U level);
static void get_leaf_cyc_para(const struct TREE_NODE FAR_POINTER*Tree_Node,INT8U *start,INT8U *end);
static INT16U Revise_Addr(INT16U leaf_addr,const struct COMMUNAL_REVISE FAR_POINTER*REVISE ,INT8U ID);
static INT8U Ram_math(INT8U mode, INT8U *getbuff ,INT8U math_mode,
                      INT8U Ass_num, INT8U dataLen);
static INT8U math_Add(INT8U *getbuff, INT8U math_mode, INT8U Ass_num, INT8U len);

static INT8U Data_Transfer_Comm(const ST_FMT_ATTR FAR_POINTER*Communal_Save, INT8U *Sbuff, INT8U *Dbuff,
		                 const ST_FMT_ATTR FAR_POINTER*Communal_Trans, INT8U dataLen);
INT8U Data_Transfer_Show(const ST_FMT_ATTR FAR_POINTER*Communal_Save, INT8U *Sbuff,
		                 ST_SHOWDATA_OUTPUT *Dbuff,const ST_FMT_ATTR FAR_POINTER*Communal_Show,
		                 const ST_FMT_ATTR FAR_POINTER*Communal_Trans, INT8U dataLen);
static INT8U SetSysTime(INT8U *pdata, INT8U sort);
//static INT8U ChkSlotData(INT8U *pdata, INT8U kind);
//static INT8U ReadFrzData(INT8U Type);
static INT8U LocalProtocolChk(void);
//static INT8U ChkSlotObis(void);
//static INT16U CalMin(INT8U Hour, INT8U Min);
static INT8U ChkFrzDataZip(void);
//static void FrzDataZipEnd(INT8U *pEndId);
//static void FrzObisUnzip(INT16U *pDataObis);
static INT8U ChkTariffPrice(void);

/*"*****************************************************************************"*/
/*"  Function:       PP_CommGetData"*/
/*"  Description:    ͨ�Ŷ����ݽӿں�����ͨ��ģ��ͨ�����øýӿں�����ȡ���ݱ�ʶ��Ӧ����"*/
/*"  Calls:          "*/
/*"  Called By:      ͨ�Ź���ģ��"*/
/*"  Input:          framedata 645-2007Э��֡�������                 "*/
/*"                  Start_addr ���ݱ�ʶ��Ӧ�������ݵ�����ƫ�Ƶ�ַ�������Դ����ݿ��н�ȡ���ݷ��ء�"*/
/*"                  MaxLen     ����������󳤶ȣ������ó��������к������ݱ�־"*/
/*"  Output:         panswer_data  ͨ��Ӧ������ָ��"*/
/*"  "*/
/*"  Return:         UN_COMM_ERR  ͨ�Ŵ�����Ϣ����"*/
/*"  Others:         code size  230byte"*/
/*"*****************************************************************************"*/
INT8U PP_CommGetData (ST_FRM_RESULT *framedata, INT16U Start_addr, INT16U MaxLen,ST_ANSWER_DATA * panswer_data)
{
	INT16U dataLen;    /*"���ݱ�ʶ��Ӧ���ݳ���"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"�ڵ��ӦҶ���б�"*/
	INT8U  Leaf_Num;/*"�ڵ��ӦҶ����"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"���־Ҷ��λ���б�"*/
	INT8U block_num;
	INT8U *pBuffer; /*"������ݻ����ַָ��"*/
    INT8U *ctrlbyte;
    INT8U mode;

	/*"local data initial"*/
	block_num = 0;
	dataLen = 0;

	/*"global data initial"*/ 
	fileIndex = 0xff;
    
	pBuffer = panswer_data->Data_addr;
	
	Leaf_Num = ProtocolTreeChk((UN_ID645 *)&framedata->dataId, Leaf_adrr_list,
	                           &block_num, block_addr_list);

	if((0 != Leaf_Num) && (Leaf_Num < MAX_LEAF_NUM))/*"�ڵ��ӦҶ�Ӳ�Ϊ��"*/
    {
        dataLen = Get_Node_Data_Comm((TREE_LEAF_LIST *) &Leaf_adrr_list,
                               Leaf_Num, Start_addr, MaxLen, pBuffer, block_num,block_addr_list);

        if(0 == dataLen)/*"�����ݳ���Ϊ0���������������ݴ���״̬"*/
        {
            if(0 == Error_Infor.asByte)
            {
                Error_Infor.ST_ASBITS.errObis = 1;
            }
        }
    }
	else   /*"δ�ҵ���Ӧ��Ҷ�ӣ�����Ƿ�Ϊ������������"*/
	{ 
		if(((0x0611 == dataId.asWord[1]) && (dataId.asBytes[1] <= 0x09) ) ||
		((0x0e10 == dataId.asWord[1]) && (dataId.asWord[0] >= 0x63) && (dataId.asWord[0] <= 0x66)))
		{
		    Error_Infor.asByte = 0;

                ctrlbyte = framedata->data - 6;    /*"����ָ�뵹��6λ����ȡ������"*/
                if(*ctrlbyte == 0x11)
                {
                    mode = 0;
                }
                else
                {
                    mode = 1;
                }
                if(check_is_lr645_id(framedata->dataId, mode, framedata->dataLen) == 1)
                {
                    dataLen = LR_GetRecord(framedata,panswer_data, mode);
                }
                else
                {
                    Error_Infor.ST_ASBITS.errOther = 1;
                }
		}
		else
		{
		    if(0 == Error_Infor.asByte)
		    {
		        Error_Infor.ST_ASBITS.errObis = 1;
		    }
		} 
	}
	panswer_data->Data_len.int_len = dataLen;/*"������ݳ��ȼ�����֡��Ϣ"*/
	
	return (Error_Infor.asByte);
}


/*"*****************************************************************************"*/
/*"  Function:       PP_CommGetDataExt"*/
/*"  Description:    ͨ�Ŷ����ݽӿں�����ͨ��ģ��ͨ�����øýӿں�����ȡ���ݱ�ʶ��Ӧ����"*/
/*"  Calls:          "*/
/*"  Called By:      ͨ�Ź���ģ��"*/
/*"  Input:          framedata 645-2007Э��֡�������                 "*/
/*"                  Start_addr ���ݱ�ʶ��Ӧ�������ݵ�����ƫ�Ƶ�ַ�������Դ����ݿ��н�ȡ���ݷ��ء�"*/
/*"                  MaxLen     ����������󳤶ȣ������ó��������к������ݱ�־"*/
/*"  Output:         panswer_data  ͨ��Ӧ������ָ��"*/
/*"  "*/
/*"  Return:         UN_COMM_ERR  ͨ�Ŵ�����Ϣ����"*/
/*"  Others:         code size  230byte"*/
/*"*****************************************************************************"*/
INT8U PP_CommGetDataExt (UN_ID645 dataId,
		            INT16U Start_addr, INT16U MaxLen,
		            ST_ANSWER_DATA * panswer_data)
{
	INT16U dataLen;    /*"���ݱ�ʶ��Ӧ���ݳ���"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"�ڵ��ӦҶ���б�"*/
	INT8U  Leaf_Num;/*"�ڵ��ӦҶ����"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"���־Ҷ��λ���б�"*/
	INT8U block_num;
	INT8U *pBuffer; /*"������ݻ����ַָ��"*/


	/*"local data initial"*/
	block_num = 0;
	dataLen = 0;

	/*"global data initial"*/
	fileIndex = 0xff;

	pBuffer = panswer_data->Data_addr;
	
	Leaf_Num = ProtocolTreeChk(&dataId, Leaf_adrr_list,
	                           &block_num, block_addr_list);

	if((0 != Leaf_Num) && (Leaf_Num < MAX_LEAF_NUM))/*"�ڵ��ӦҶ�Ӳ�Ϊ��"*/
    {
        dataLen = Get_Node_Data_Comm((TREE_LEAF_LIST *) &Leaf_adrr_list,
                               Leaf_Num, Start_addr, MaxLen, pBuffer, block_num,block_addr_list);

        if(0 == dataLen)/*"�����ݳ���Ϊ0���������������ݴ���״̬"*/
        {
            if(0 == Error_Infor.asByte)
            {
                Error_Infor.ST_ASBITS.errObis = 1;
            }
        }
    }
	else   /*"��֧�ֳ�����������"*/
	{
        if(0 == Error_Infor.asByte)
        {
		        Error_Infor.ST_ASBITS.errObis = 1;
        }
	}
	panswer_data->Data_len.int_len = dataLen;/*"������ݳ��ȼ�����֡��Ϣ"*/
	
	return (Error_Infor.asByte);
}



/*"code size  456byte"*/
static INT16U Set_Leaf_Data(const struct TREE_LEAF FAR_POINTER*Tree_Leaf,INT8U *buff,INT16U Revise, INT8U PassWord_Level, INT8U *pdata)
{
	INT8U s_len;
	ST_FMT_ATTR SaveFrmt;
    ST_FMT_ATTR TranFrmt;
	//const ST_FMT_ATTR FAR_POINTER*Communal_Trans;
	const struct COMMUNAL_PURVIEW FAR_POINTER*Communal_Purview;
	const struct COMMUNAL_FUNCTION FAR_POINTER*Communal_Function;
	const struct COMMUNAL_ID FAR_POINTER*Communal_Id;
	INT16U ret;
	INT8U  Condition;/*"������л���  0 ���� 1 ʵ����   2�ֳ�"*/
    //INT16U offSet;
    INT8U  Ret_8;
    INT16U obis_WR; 
    INT8U format_save;
	
	Communal_Purview =(const struct COMMUNAL_PURVIEW  FAR_POINTER*)&Protocol_Purview[Tree_Leaf->Communal_Purview_Id];
	Communal_Function=(const struct COMMUNAL_FUNCTION FAR_POINTER*)&Protocol_Function[Tree_Leaf->Communal_Function_Id];    
	Communal_Id      =(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];

    LIB_ConstMemCpy((CONST *)&Protocol_Format[Communal_Id->Communal_Trans_Id], (INT8U *)&TranFrmt, sizeof(ST_FMT_ATTR));
    
    //Communal_Trans   =(const ST_FMT_ATTR   FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id];

    /*(Sep 21, 2018,13:30:23)����ĳ�E_NN_MMDD���Ա�ת��*/
    if((E_ACTIVE_CALENDAR_SEASON_TBL == Tree_Leaf->obis)||(E_PASSIVE_CALENDAR_SEASON_TBL == Tree_Leaf->obis))
    {
        TranFrmt.format = E_NN_MMDD_BCD;
    } 
    else if ((E_ACTIVE_CALENDAR_DAY_TBL <= Tree_Leaf->obis)&&(E_ACTIVE_CALENDAR_SEASON_TBL > Tree_Leaf->obis))
    {
        TranFrmt.format = E_NN_HHMM_BCD;
    }   

    /*"����״̬"*/
    if(TRUE == ChkStatus(E_FAC))
    {
        Condition = 0;
    }
    else                              /*"�ֳ�״̬"*/
    {
        Condition = 2;
    }
	
	/*"���� Ȩ����Ϊ0����ʾ����д"*/
	if(Communal_Purview->write_mode[Condition] == 0x00)
	{
		Error_Infor.ST_ASBITS.errPassword = 1;
		return 0;
	}

	/*"д֮ǰ�ĺ���һ���������������Ƿ��޺ͼ�¼�¼�"*/		
	if((Communal_Function->OnWrite_Beford != NULL) && (SetParaFuncFlg == 0))
	{
	 	if(Communal_Function->OnWrite_Beford(pdata) == FALSE) /*"д����ʧ��"*/
	 	{
	 		return 0;
	 	}
		SetParaFuncFlg = 0xff;
	}

	
	/*"д������ݵĳ���"*/	
	s_len = FRMT_sizeofFormat(TranFrmt.format);
	
	ret = TRUE;
	//offSet = Revise + Tree_Leaf->offSet;
	
    /*"дǰ�������Ѿ�������ɵĲ�������Ҫ��������Ĵ���������ʱ���ѭ�ԡ����Բ���"*/
	if((E_SYS_TIME != Tree_Leaf->obis) && (E_KEY_DIS_OBJ_LIST != Tree_Leaf->obis) && (E_CYCLE_DIS_OBJ_LIST != Tree_Leaf->obis))
	{
	    INT8U dBuff[42+24];  /*Ԥ��24�ֽ�*/
	    INT8U datalen;
	    INT8U *psour;
        INT8U num_all_obis;    


	    psour = buff;

	    LIB_MemSet(0, (INT8U *)&SaveFrmt, sizeof(ST_FMT_ATTR));
        GetFrmt(Tree_Leaf->obis,(ST_FMT_ATTR *)&SaveFrmt);	

        if((E_ACTIVE_CALENDAR_DAY_TBL <= Tree_Leaf->obis)&&(E_ACTIVE_CALENDAR_SEASON_TBL > Tree_Leaf->obis))
        {
            SaveFrmt.format = E_NN_HHMM;
        }    

	    datalen = 0;
        format_save = SaveFrmt.format;

        /*"�洢��ʽ�봫���ʽ��һ�£����������ת��"*/
	    if((format_save != TranFrmt.format) || 
           (SaveFrmt.exp != TranFrmt.exp) || 
           (SaveFrmt.unit != TranFrmt.unit))
	    { 
            datalen = FRMT_DataChange(buff, (INT8U *)&dBuff, TranFrmt, SaveFrmt); 
            
            if((0 != datalen) && (0xff != datalen))
	        {
	            psour = (INT8U *)&dBuff;
	        }
	    }
	    else
	    {
	        /*"���ݴ洢��ʽ���������ݸ�ʽһ�£�����Ҫ��������ת����ֱ�ӽ������ݴ洢"*/
	        datalen = s_len;
	    } 


	    if((0 != datalen) && (0xff != datalen))
	    {
            if(E_U_6_BYTE == format_save)  /*ͨ�ŵ�ַ����ŷ���*/
            {
                LIB_RvsSelf(psour,6);
            }   

            /*ASCII��Ҫ����*/
            if((format_save >=E_ASCII_2_LENGTH)&&(format_save <=E_ASCII_32_LENGTH))
            {
                LIB_RvsSelf(psour,datalen);
            }  
            
	        s_len = datalen;
            num_all_obis= 1;
            obis_WR = Tree_Leaf->obis+Revise;

            /*(Oct 09, 2018,9:52:21)�������յĳ���Ҫ���⴦����,��Ϊ�ײ����г��ȣ�ʵ�ʸ�ʽת����������ǶԵ�*/
            if((obis_WR<=(E_SPECIAL_DAY_TBL+SPEC_DAY_NUM))&&(obis_WR>=(E_SPECIAL_DAY_TBL+1)))
            {
                s_len = SPEC_DAY_SET_SIZE;
                if(EQUAL == LIB_ChkValue(buff, 4, 0))  /*��������֧��ȫ0����*/
                {
                    LIB_MemSet(0, psour, SPEC_DAY_SET_SIZE);
                }    
            }    
            
            Ret_8 = (0 -Set((INT16U *)&obis_WR, num_all_obis,psour, &s_len));
	        if(DAR_0_OK != Ret_8)
            {
                ret = FALSE;
            }       
	    }
	    else
	    {
	        ret = FALSE;
	    }
	}
	else
	{
	}
    
	if(TRUE == ret)
	{
        if(Communal_Function->OnWrite_End != 0)
        {
            ret = Communal_Function->OnWrite_End(buff);
        }
	}
    
	if(ret == TRUE)
	{
	    return s_len;
	}
	else 
	{
	    return 0;
	}
}
/*"****************************************************************************
*"*/
/*"  Function:       Set_Node_Data_Comm"*/
/*"  Description:    ͨ�����ýڵ��Ӧ���ݣ�һ���ڵ��������N
��Ҷ�ӣ����ݴ������С��Ԫ��Ҷ��"*/
/*"                  �ڵ㡣"*/
/*"  Calls:          "*/
/*"  Called By:      PP_CommSetData"*/
/*"  Input:          Leaf_adrr_list �ڵ��Ӧ��Ҷ���б�"*/
/*"                  pBuffer  �����������ݻ����ַָ��"*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         ��"*/
/*"  Others:         code size  200byte"*/
/*"****************************************************************************
*"*/
static void Set_Node_Data_Comm(TREE_LEAF_LIST *Leaf_adrr_list, INT8U Leaf_Num,
		                  INT8U *pBuffer, INT8U PassWord_Level)
{
	const struct TREE_LEAF FAR_POINTER*Tree_Leaf;
	const struct COMMUNAL_ID FAR_POINTER*Communal_Id;
	const ST_FMT_ATTR FAR_POINTER*Communal_Trans;
	INT16U Revise;
	INT8U i;
	INT8U  Data_len;
	INT8U  len;
	TREE_LEAFADDR Leaf_addr;
	INT8U *pdata;

	pdata = pBuffer;
	SetParaFuncFlg = 0;
	Data_len = 0;
	len = 0;
	
	for(i = 0; i < Leaf_Num; i++)
	{
	    LIB_MemCpy((INT8U *)&Leaf_adrr_list[i].Tree_addr[0], (INT8U *)&Leaf_addr, 2);
	    LIB_MemCpy((INT8U *)&Leaf_adrr_list[i].Revise[0], (INT8U *)&Revise, 2);

		Tree_Leaf = (const struct TREE_LEAF FAR_POINTER*)&Protocol_Tree_Leaf[Leaf_addr.Int_addr];
		
		Communal_Id=(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
		Communal_Trans = (const ST_FMT_ATTR  FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id];
        if(Communal_Trans->format == E_ID_CODE_2_FORMAT)    /*"��š�ͨ�ŵ�ַ�������õڶ���Ҷ�Ӳ��ô���"*/
        {
            continue;
            //offSet += 4;
        }


		len = (INT8U)Set_Leaf_Data(Tree_Leaf, (INT8U *)(pBuffer + Data_len), Revise, PassWord_Level, pdata);
		if(len == 0)
		{
		    if(Error_Infor.asByte == 0)
		    {
		        Error_Infor.ST_ASBITS.errOther = 1;
		    }
			return;
		}

		Data_len += len;
	}
}

/*"*****************************************************************************"*/
/*"  Function:       PP_CommSetData"*/
/*"  Description:    ͨ��д���ݽӿں�����ͨ��ģ��ͨ�����øýӿں���д���ݱ�ʶ��Ӧ����"*/
/*"  Calls:          "*/
/*"  Called By:      ͨ�Ź���ģ��"*/
/*"  Input:          framedata 645-2007Э��֡�������                 "*/
/*"  Output:         panswer_data  ͨ��Ӧ������ָ��"*/
/*"  "*/
/*"  Return:         UN_COMM_ERR  ͨ�Ŵ�����Ϣ����"*/
/*"  Others:         code size  180byte"*/
/*"*****************************************************************************"*/
#define _CHK_SW_ID_MAX    7
const INT16U ChkSwId[_CHK_SW_ID_MAX] =
        {
                0x0106,/*"����ʱ���л�ʱ��"*/
                0x0107,/*"����ʱ���л�ʱ��"*/
                0x0108,/*"���׷�ʱ�����л�ʱ��"*/
                0x0109,/*"���׽��ݵ���л�ʱ��"*/
                0x0101,/*"����"*/
                0x0102,/*"ʱ��"*/
                0x010C /*"����+ʱ��"*/
        };

INT8U PP_CommSetData (ST_FRM_RESULT *framedata,
		            ST_ANSWER_DATA *panswer_data)
{
	INT8U *pBuffer; /*"������ݻ����ַָ��"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"�ڵ��ӦҶ���б�"*/
	INT8U  Leaf_Num;/*"�ڵ��ӦҶ����"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"���־Ҷ��λ���б�"*/
	INT8U block_num;
	//INT8U i;

	block_num = 0;

    //Len_dl645 = framedata->dataLen;
    
#ifdef DEBUG_LOG
    if((NULL != framedata->data) && (0 != framedata->dataLen))/*"�����������ַ��Ϊ�������ݳ��Ȳ�Ϊ0"*/
    {
        pBuffer = framedata->data;
    }
    else
    {
        DBG_Printf("input data address is not initialized");
        return(0x80);
    }
#endif

	pBuffer = framedata->data;

	Leaf_Num = ProtocolTreeChk((UN_ID645 *)&framedata->dataId, Leaf_adrr_list,
	                               &block_num, block_addr_list);

	if((0 != Leaf_Num) && (Leaf_Num < MAX_LEAF_NUM))/*"�ڵ��ӦҶ�Ӳ�Ϊ��"*/
    {
        Set_Node_Data_Comm((TREE_LEAF_LIST *) &Leaf_adrr_list,
                            Leaf_Num, pBuffer, framedata->authority);
        if(Error_Infor.asByte == 0)/*"�������óɹ�"*/
        {
#if 0
            /*"��������/ʱ��/ʱ��ʱ���л�ʱ�䣬����ʱ��ʱ���л�"*/
            if(0x0400 == dataId.asWord[1])
            {
                for(i = 0; i < _CHK_SW_ID_MAX; i++)
                {
                    if(dataId.asWord[0] == ChkSwId[i])
                    {                       
                        Action(E_CHECK_SWITCH_ACTION,NULL,NULL);/*"����ʱ���л���Ϊ����ʾ�٢�"*/
                        break;
                    }
                }
            }

            //TR_FindNowTriff(NULL,NULL);
            Action(E_FIND_NOW_TRIFF_ACTION,NULL,NULL);            
            //PM_FindNowStep(NULL,NULL);
            //Action(E_FIND_NOWSTEP,NULL,NULL);  
#endif
        }
    }
	else
    {
	    if(0 == Error_Infor.asByte)
	    {
	        Error_Infor.ST_ASBITS.errOther = 1;
	    }
    }
	
	return (Error_Infor.asByte);
}


#ifdef DM_DLT645
/*"*****************************************************************************"*/
/*"  Function:       PP_ShowGetData"*/
/*"  Description:    ��ʾ�����ݽӿں�������ʾģ��ͨ�����øýӿں�����ȡ���ݱ�ʶ��Ӧ����"*/
/*"  Calls:          "*/
/*"  Called By:      ��ʾ����ģ��"*/
/*"  Input:          pdata_in   ��ʾ���ݱ�ʶ��Ҷ�Ӻ�                 "*/
/*"                  "*/
/*"  Output:         pdata_out  ��ʾ�������ݼ���ʾ��Ϣ�����ݸ�ʽ"*/
/*"  "*/
/*"  Return:         0  ȡ�����ɹ�    len ȡ�����ݳ��ȣ�ȡ���ɹ�"*/
/*"  Others:         code size  164byte"*/
/*"*****************************************************************************"*/
INT8U PP_ShowGetData (ST_DISP_OBJ *pdata_in,ST_SHOWDATA_OUTPUT *pdata_out )
{
	INT8U dataLen;    /*"���ݱ�ʶ��Ӧ���ݳ���"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"�ڵ��ӦҶ���б�"*/
	INT8U  Leaf_Num;/*"�ڵ��ӦҶ����"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"���־Ҷ��λ���б�"*/
	INT8U block_num;
	
	block_num = 0;
	dataLen = 0;

#ifdef DEBUG_LOG
	if(NULL == pdata_out)/*"show data output buffer is NULL"*/
	{
		DBG_Printf("output data address is not initialized");
		return(0x80);
	}
#endif

#ifdef DM_DLT645	
	Leaf_Num = ProtocolTreeChk((UN_ID645 *)&pdata_in->unId645, Leaf_adrr_list,&block_num, block_addr_list);

	/*"�ڵ��ӦҶ�Ӳ�Ϊ��,ͬʱ�ڵ�Ҷ����������Ҫ��ʾ��Ҷ�Ӻ�"*/
    if((0 != Leaf_Num) && (Leaf_Num > pdata_in->leafNum) && (Leaf_Num < MAX_LEAF_NUM))
    {
        dataLen = Get_Leaf_Data_Show(&Leaf_adrr_list[0],pdata_in->leafNum, (ST_SHOWDATA_OUTPUT *) pdata_out);
    }
    else
    {

    }
#endif	

    return (dataLen);
}
#endif 

/*"*****************************************************************************"*/
/*"  Function:       ProtocolTreeChk"*/
/*"  Description:    Э�����ڵ��������"*/
/*"  Calls:          "*/
/*"  Called By:      ͨ�Ŷ���д���ݽӿڣ���ʾ���ӿ�"*/
/*"  Input:          pdataId   �������ݱ�ʶ    "*/
/*"                  "*/
/*"  Output:         Leaf_adrr_list Ҷ���б���"*/
/*"                  block_num  ���ݿ���    block_list ��ָ�λ���б�"*/
/*"  "*/
/*"  Return:         Leaf_Num Ҷ���� "*/
/*"  Others:         code size  164byte"*/
/*"*****************************************************************************"*/
static INT8U  ProtocolTreeChk(UN_ID645 *pdataId,TREE_LEAF_LIST *Leaf_adrr_list,
                              INT8U *block_num, INT8U *block_list)
{
    INT16U  nodePoint;        /*"�ڵ�λ��"*/
    CHECK_NODE_RET nodeAttr;  /*"���ݱ�ʶ��Ӧ�ڵ�����  �����ѹ��������ѹ����Ϣ"*/
    INT8U Leaf_Num;

    nodeAttr.byte = 0;
    Leaf_Num = 0;

    Error_Infor.asByte = 0;
    dataId.asLong = pdataId->asLong;
    NodedataId.asLong = pdataId->asLong;

    if(FALSE == LocalProtocolChk())/*"Զ�̱���Э�鲻֧��"*/
    {
        return (0);
    }

    ChkFrzDataZip();

    /*"�������ݱ�ʶ��Ӧ�ڵ�λ��"*/
    nodePoint = Search_Node(&nodeAttr);

    if(0xffff != nodePoint)/*"���ݽڵ����"*/
    {
        /*"�ڵ���������ҽڵ��Ӧ��Ҷ���б�"*/
        Leaf_Num = Traverse_Node(nodePoint, nodeAttr.byte,
                                 Leaf_adrr_list,block_num, block_list);
    }

    return Leaf_Num;
}
/*"*****************************************************************************"*/
/*"  Function:       Search_Node"*/
/*"  Description:    �������ݱ�ʶ��Ӧ�Ľڵ�λ��"*/
/*"  Calls:          "*/
/*"  Called By:      ������������ӿں���"*/
/*"  Input:          dataId ���ݱ�ʶ                 "*/
/*"                  "*/
/*"  Output:         nodeAttr  �ڵ�����"*/
/*"  "*/
/*"  Return:         �ڵ�λ��"*/
/*"  Others:         code size  84byte"*/
/*"*****************************************************************************"*/
static INT16U Search_Node(CHECK_NODE_RET * nodeAttr)
{
	INT16U nodeAddr,nodeAddrNow;/*"����Ƕ�ײ�ڵ�λ�ã�nodeAddrNow Ϊ��ǰ�ڵ��ַ��nodeAddrΪ�¼��������Ҫ��Ľڵ��ַ"*/
	CHECK_NODE_RET  ret;/*"�ڵ����Խ������"*/
	
	nodeAddrNow = Protocol_Tree_Root[NodedataId.asBytes[3]];/*"�ҵ����ڵ�λ��"*/
	if(nodeAddrNow == 0xFFFF)/*"�Ҳ������ڵ�"*/
	{
	    return nodeAddrNow;
	}
	
	ret.byte = SearchNodeNesting(nodeAddrNow, (INT16U *)&nodeAddr, 0);
		
	if(ret.bit.error != 0)/*"û�ҵ���Ӧ�ڵ�"*/
	{
		Error_Infor.ST_ASBITS.errObis = 1;
		nodeAddr = 0xffff;
	}
	else/*"�ҵ�"*/
	{
		nodeAttr->byte = ret.byte;
	}

	return nodeAddr;
}

/*"*****************************************************************************"*/
/*"  Function:       Traverse_Node"*/
/*"  Description:    �����ڵ��Ӧ��Ҷ�ӣ�����Ҷ����ӵ�Ҷ���б���"*/
/*"  Calls:          "*/
/*"  Called By:      ������������ӿں���"*/
/*"  Input:          node_addr �ڵ�λ��"*/
/*"                  nodeAttr �ڵ�����                 "*/
/*"                  dataId 645Э�����ݱ�ʶ                 "*/
/*"                  "*/
/*"  Output:         Leaf_adrr_list  �ڵ��Ӧ��Ҷ���б�"*/
/*"                  dataLen        �ڵ��Ӧ���ݳ��ȣ��������ʽ���㣩"*/
/*"                  block_num       ���ݷֿ������������ݷֿ飬������ΪAA��"*/
/*"                  block_list      �ֿ�λ���б�"*/
/*"  "*/
/*"  Return:         �ڵ��Ӧ��Ҷ����"*/
/*"  Others:         code size  116byte"*/
/*"*****************************************************************************"*/
static INT8U Traverse_Node(INT16U node_addr, INT8U nodeAttr,
		            TREE_LEAF_LIST *Leaf_adrr_list,INT8U *block_num, INT8U *block_list)
{
	INT8U zip_id[2];/*"ѹ�������ݱ�ʶλ����ֵ�����֧��2��ѹ��"*/
	CHECK_NODE_RET zip;
	INT8U Leaf_num;
	INT8U Leaf_num_return;
	
	Leaf_num = 0;
	Leaf_num_return = 0;
	zip_id[0]=0xff;
	zip_id[1]=0xff;

	/*"��ѹ����ID���д���"*/
	zip.byte = nodeAttr;

	if(zip.bit.zip1==1)
	{
		zip_id[0]=NodedataId.asBytes[zip.bit.num1]; 
		fileIndex = zip_id[0];/*"��ѹѹ������λ�����ڳ�����¼�ļ�"*/ 
	}	       	
	else
	{

	}							
	if(zip.bit.zip2==1)
	{
		zip_id[1]=NodedataId.asBytes[zip.bit.num2];
	}			
	else
	{

	}
	
	Leaf_num = Traverse_Node_Nesting(node_addr, 0, &Leaf_num_return, zip_id,
            Leaf_adrr_list, block_num, block_list);
#ifdef DEBUG_LOG
	if(Leaf_num != Leaf_num_return)
	{
		DBG_Printf("Traverse_Node_Nesting err!");
		DBG_Printf("\node_addr = 0x%x%x\n", bHiByte(node_addr),bLoByte(node_addr));
	}
#endif
	return(Leaf_num);
}
/*"*****************************************************************************"*/
/*"  Function:       SearchNodeNesting"*/
/*"  Description:    �������ݱ�ʶ��Ӧ�Ľڵ�λ��Ƕ�׺���"*/
/*"  Calls:          "*/
/*"  Called By:      ������������ӿں���"*/
/*"  Input:          dataId   ���ݱ�ʶ  "*/
/*"                  nodeAddrLower �¼��ڵ�λ��       "*/
/*"                  nodeAttr  �ϼ��ڵ�����        "*/
/*"                  "*/
/*"  Output:         Node_addr ���ҵ����ϲ���Ҫ��Ľڵ��ַָ��"*/
/*"  "*/
/*"  Return:         �ڵ�λ��"*/
/*"  Others:         code size  148byte"*/
/*"*****************************************************************************"*/
static INT8U SearchNodeNesting(INT16U nodeAddrLower, INT16U * nodeAddr, INT8U nodeAttr)
{
	CHECK_NODE_RET ret;   /*"��ǰ�ڵ����Խ������"*/
	CHECK_NODE_RET retPre;/*"�ϼ��ڵ����Խ������"*/
	INT16U nodeLower;     /*"�¼��ڵ�λ��"*/
	const struct TREE_NODE  FAR_POINTER *treeNode;/*"��ǰ�ڵ���Ϣ"*/
	
	do
	{
		treeNode = (const struct TREE_NODE  FAR_POINTER*)&Protocol_Tree_Node[nodeAddrLower];
		
		ret.byte = ChkNode((const INT8U  FAR_POINTER*)treeNode);
		if(ret.byte == FALSE)/*"�ڵ�λ�ò��Ϸ�"*/
		{
			return 0xfe;    /*bit error = 1*/
		}
		
		ret.byte = ChkNodeID(treeNode, nodeAttr);
		
		if(ret.bit.error == 1)/*"�������Ҵ���"*/
		{			
			if(treeNode->Next_Node != 0xffff)/*"�Ǳ��ڵ�"*/
			{
				nodeAddrLower = treeNode->Next_Node;/*"ָ��ͬ����һ���ڵ�"*/
			}
			else
			{
				/*"�����ϼ��ڵ� ������ʵ��Ϊʧ�ܳ��ڣ�������ڱ��ڵ��·����ϼ��ڵ�Ҳ�������ҵ�"*/
				return 0xff;        /*bit error = 1*/
			}
		}
		else
		{
			if(ret.bit.low == 0)/*"�ҵ����ݱ�ʶ��Ӧ�Ľڵ��Ҷ��"*/
			{								
				*nodeAddr = nodeAddrLower;
				return ret.byte;
			}
			else/*"���¼��ڵ�"*/
			{			
			    retPre.byte = ret.byte;
				nodeLower = treeNode->Low_Node;
				nodeAttr |= ret.byte;
				ret.byte = SearchNodeNesting(nodeLower, nodeAddr,nodeAttr);
				    				
				return ((INT8U)(ret.byte|retPre.byte));
			}
		}
	}while(1);/*"�����Ƿ��п�����ѭ��"*/
			
	//return 0xff;					
}

/*"*****************************************************************************"*/
/*"  Function:       Traverse_Node_Nesting"*/
/*"  Description:    �����ڵ��Ӧ��Ҷ�ӣ�����Ҷ����ӵ�Ҷ���б���"*/
/*"  Calls:          "*/
/*"  Called By:      ������������ӿں���"*/
/*"  Input:          node_addr �ڵ�λ��"*/
/*"                  Nest_level Ƕ�׵��ò��  "*/
/*"                  zip_id     �ڵ�ѹ����Ϣ               "*/
/*"                  "*/
/*"  Output:         Leaf_adrr_list  �ڵ��Ӧ��Ҷ���б�"*/
/*"                  dataLen    �ڵ��Ӧ���ݳ���"*/
/*"  "*/
/*"  Return:         ���νڵ������Ӧ��Ҷ����"*/
/*"  Others:         code size  564byte"*/
/*"*****************************************************************************"*/
static INT8U Traverse_Node_Nesting(INT16U node_addr,INT8U Nest_level,INT8U *Leaf_num,
		                    INT8U *zip_id, TREE_LEAF_LIST *Leaf_adrr_list,
		                    INT8U *block_num, INT8U *block_list)
{
	const struct TREE_LEAF  FAR_POINTER*Tree_Leaf;
	const struct TREE_NODE  FAR_POINTER*Tree_Node;
	const struct COMMUNAL_REVISE  FAR_POINTER*Communal_Revise;
	const struct REVISE_ID  FAR_POINTER*REVISE_ID;
#if 0
	const struct COMMUNAL_ID  FAR_POINTER*Communal_Id;
	const ST_FMT_ATTR FAR_POINTER*Communal_Trans;
#endif
       INT16U node, Revise;
	INT8U  zip_leaf_start, zip_leaf_end, ret, id_n, block_flag;
	INT8U  Leaf_num_temp;
	INT8U block_num_temp;
	INT8U leafZip;
	
	node=node_addr;
	Leaf_num_temp = *Leaf_num;
	block_num_temp = *block_num;
		
	zip_leaf_start=0xff;
	zip_leaf_end=0xff;
	id_n=0xff;
	leafZip = 0xff;
	
	
	if(Nest_level != 0)/*"������ڵ㲻�ǵ�һ��ڵ���ô��Ҫ���㣬��Ϊѹ�������ѭ������"*/
	{
		Tree_Node=(const struct TREE_NODE FAR_POINTER*)&Protocol_Tree_Node[node];
		get_leaf_cyc_para(Tree_Node,&zip_leaf_start,&zip_leaf_end);				
	}

    
	do
	{
		Tree_Node=(const struct TREE_NODE FAR_POINTER*)&Protocol_Tree_Node[node];
		block_flag=Tree_Node->Node_mode.bit.zip;
		
		/*"����˽ڵ�ָ�����Ҷ�ӣ����Ҷ�ӽڵ㵽Ҷ������"*/
		if(Tree_Node->Node_mode.bit.mode==0)
		{
		    /*OBIS�����ַ*/
		    LIB_MemCpy((INT8U FAR_POINTER *)&Tree_Node->Low_Node,
		                (INT8U *)&Leaf_adrr_list[Leaf_num_temp].Tree_addr[0], 2);
			/**/
			Tree_Leaf=(const struct TREE_LEAF FAR_POINTER*)&Protocol_Tree_Leaf[Tree_Node->Low_Node];
			REVISE_ID =(const struct REVISE_ID FAR_POINTER*)&Protocol_Revise_Id[Tree_Leaf->Communal_Addr_Id];
#if 0
			Communal_Id =(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
			Communal_Trans = (const ST_FMT_ATTR FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id];
#endif			
			id_n = set_zip_id(zip_id, zip_leaf_start, id_n);
			Revise = 0;
			
			/*"Ҷ��ƫ����Ϣ����,�ڵ����������ѹ������ַƫ����Ϣֻ��һ�������ڵ�����ѹ��ʱ���ڶ���ѹ����Ӧ��ַ��Ϣƫ��"*/
			if(0xff != *(zip_id + 1))
			{
			    leafZip = *(zip_id + 1);
			}
			else
			{
			    leafZip = *zip_id;
			}
			Communal_Revise = (const struct COMMUNAL_REVISE FAR_POINTER*)&Protocol_Revise[REVISE_ID->Revise_Id];
			Revise += Revise_Addr(0,Communal_Revise,leafZip);

			LIB_MemCpy((INT8U *)&Revise, (INT8U *)&Leaf_adrr_list[Leaf_num_temp].Revise[0], 2);
			
			Leaf_num_temp++;
			*Leaf_num = Leaf_num_temp;
		}
		else/*���*/
		{
			id_n = set_zip_id(zip_id, zip_leaf_start, id_n);
			Traverse_Node_Nesting(Tree_Node->Low_Node, (INT8U)(Nest_level + 1),&Leaf_num_temp,
					                      zip_id,Leaf_adrr_list, &block_num_temp, block_list);
			
			*Leaf_num = Leaf_num_temp;
		}
		
		/*"�ڵ���������ж�"*/				

		if(Nest_level == 0)/*"�����ĸ��ڵ��������"*/
		{			
		    *block_num = block_num_temp;
			return Leaf_num_temp;
		}
        
		if(zip_leaf_start<zip_leaf_end)/*"��ѹ��,δ�������"*/
		{
			zip_leaf_start++;
			ret=0;	
		}
		else                       /*"��ɱ���"*/
		{
			ret=check_nodePoint(Tree_Node->Next_Node);
			node=Tree_Node->Next_Node;	
			id_n=set_zip_id(zip_id,0xff,id_n);
			id_n=0xff;
		}

		/*"��������ʱ�����ӿ��־�ָ��"*/
		if(block_flag == 1)
		{
		    *(block_list + block_num_temp) = (INT8U)(Leaf_num_temp - 1);
		    block_num_temp++;
		    *block_num = block_num_temp;
		}

        /*"��ֹ�������"*/
		if((Leaf_num_temp > MAX_LEAF_NUM) || (block_num_temp > PROTOCOL_MAX_BLOCK_NUM))
		{
		    *Leaf_num = 0xff;
		    return 0xff;
		}
	}while(ret == 0);/*"�����ڵ㣬ֱ���жϱ�������"*/
	
	*block_num = block_num_temp;
	
	return Leaf_num_temp;
}

/*"****************************************************************************
*"*/
/*"  Function:       ChkSlotObis"*/
/*"  Description:    
�ж����ݱ�ʶΪʱ����/ʱ�α����ݱ�ʶ��ʱ��ʱ�α���ʱ������������"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Node_Data_Comm"*/
/*"  Input:          ��"*/
/*"  Output          "*/
/*"  Return:         _TRUE ʱ����/ʱ�α����ݱ�ʶ  _FALSE 
ʱ����/ʱ�α����ݱ�ʶ"*/
/*"  Others:         "*/
/*"****************************************************************************
*"*/
static INT8U ChkSlotObis(void)
{
    INT16U id645_H16;

    id645_H16 = (INT16U) (dataId.asLong >> 16);

    if((0x0401 == id645_H16) || (0x0402 == id645_H16))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


/*****************************************************************************
 �� �� ��  : Get_Soft_Record_Id
 ��������  : �����������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��25��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U Get_Soft_Record_Id(INT8U *pdata)
{
    INT8U i;
    INT8U TmpBuf[32];
    
    GetSingle(E_SOFT_RECORD_ID,TmpBuf);

    for(i=0;i<8;i++)
    {
        *pdata++ = LIB_AscToHex(&TmpBuf[i*2],2);
    }    

    return 8;  /*8�ֽ�*/
}    

/*"*****************************************************************************"*/
/*"  Function:       Get_Node_Data_Comm        "*/
/*"  Description:    ͨ�Ż�ȡ�ڵ��Ӧ���ݣ�һ���ڵ��������N��Ҷ�ӣ����ݴ������С��Ԫ��Ҷ��"*/
/*"                  �ڵ㣬�������ڵ����ݳ��ȳ���֡��ʽ��������ݳ��ȣ�����зָ��"*/
/*"  Calls:          "*/
/*"  Called By:      PP_CommGetData"*/
/*"  Input:          Leaf_adrr_list �ڵ��Ӧ��Ҷ���б�"*/
/*"                  Start_addr ��ȡ�ڵ��Start_addr�ֽڿ�ʼ����������  "*/
/*"                  MaxLen     һ�ζ�ȡ��������ݳ���               "*/
/*"                  "*/
/*"  Output:         pBuffer  ��ȡ���ݽ��ջ���"*/
/*"  "*/
/*"  Return:         ��ȡ���ݳ���"*/
/*"  Others:         code size  390byte"*/
/*"*****************************************************************************"*/
static INT16U Get_Node_Data_Comm(TREE_LEAF_LIST *Leaf_adrr_list, INT8U Leaf_Num,
		                  INT16U Start_addr, INT16U MaxLen, INT8U *pBuffer,
		                  INT8U block_num, INT8U *block_list)
{
	const struct TREE_LEAF FAR_POINTER*Tree_Leaf;
	const struct COMMUNAL_ID FAR_POINTER*Communal_Id;
	const ST_FMT_ATTR FAR_POINTER*Communal_Trans;
	INT16U Revise;
	INT8U i, j, k;
	INT16U Node_data_len;
	INT16U  dataLen;
	INT16U  len;
	INT8U LenFormat;
	TREE_LEAFADDR  Leaf_addr;
	INT8U blockFlg; 
	INT8U slotNum;


	k = 0;
	Node_data_len = 0;
	dataLen = 0;
	len = 0;
	LenFormat = 0;
	slotNum = 0;

	for(i = 0; i < Leaf_Num; i++)
	{
	    LIB_MemCpy((INT8U *)&Leaf_adrr_list[i].Tree_addr[0], (INT8U *)&Leaf_addr, 2);

		Tree_Leaf = (const struct TREE_LEAF FAR_POINTER*)&Protocol_Tree_Leaf[Leaf_addr.Int_addr];
		Communal_Id = (const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
		Communal_Trans=(const ST_FMT_ATTR  FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id];
		LenFormat = FRMT_sizeofFormat(Communal_Trans->format);
		Node_data_len += LenFormat;
        if(Communal_Trans->format == E_ID_CODE_2_FORMAT)
        {
            continue;
        }

		blockFlg = FALSE;
		for(j = k; j < block_num; j++)
        {
            if(*(block_list + j) == i)
            {
                k = j;
                blockFlg = TRUE;
            }
        }
        
		if(Node_data_len > Start_addr)/*"��ͨ��Ҫ���������ʼλ�ÿ�ʼ�������ݣ����ں���֡���ݴ���"*/
		{
		    LIB_MemCpy((INT8U *)&Leaf_adrr_list[i].Revise[0], (INT8U *)&Revise, 2);

			len = Get_Leaf_Data_Comm(Tree_Leaf, pBuffer + dataLen, dataLen, MaxLen,Revise);
			
			/*"ȡ�����ɹ��������ݸ�ʽ��ƥ��ʱ���ݳ���Ϊ0�����ش��󣨶�������ģʽ�ֹرճ��⣩"*/
			if((len == 0) && (FALSE == noFrzDataFlg))
			{
			    dataLen =0;
			    break;
			}
            
			dataLen += len;
			if(dataLen > MaxLen)/*"Ӧ�������ѳ���֡������󳤶ȣ�����ú���֡��ʽ"*/
			{
				break;
			}
			if(TRUE == blockFlg)
			{
			    /*"��ӿ�ָ���0xaa"*/
			    *(pBuffer + dataLen) = 0xaa;
			    dataLen++;
			    Node_data_len++;
			}
		}
		else
		{
		    if(TRUE == blockFlg)
		    {
		        Node_data_len++;
		    }
		}
	}

    //if(E_ID_CODE_2_FORMAT == Communal_Trans->format)
    //{
    //    LIB_RvsSelf(pBuffer,6);
    //}  
    

/*"������ʱ������ʱ�α�ʱ�����ݲ�����ʱ��������ʱ�����������ݳ���"*/
	if(TRUE == ChkSlotObis())
	{
	    if(0 == dataId.asBytes[0])
	    {
	        GetSingle(E_SEASON_SLOT_NUM, (INT8U *)&slotNum);
	    }
	    else
	    {
	        GetSingle(E_DAY_SLOT_NUM, (INT8U *)&slotNum);
	    }        
	    
	    dataLen = slotNum * 3;
	}
     
	/*"�������ʵ�۱�ʱ�����ݷ����������������ݳ���"*/
	if(TRUE == ChkTariffPrice())
	{
        GetSingle(E_TARIFF_NUM, &slotNum);
        dataLen = slotNum << 2;/*"���ݳ���Ϊ������ * 4"*/
	}
    
	if((i < Leaf_Num) && (dataLen > 0))/*"�к���֡"*/
	{
	    dataLen -= len;
	    dataLen |= 0x8000;
	}

	return dataLen;
}

#ifdef DM_DLT645
/*"code size  284byte"*/
INT8U Data_Transfer_Show(const ST_FMT_ATTR FAR_POINTER*Communal_Save, INT8U *Sbuff,
		                 ST_SHOWDATA_OUTPUT *Dbuff,const ST_FMT_ATTR FAR_POINTER*Communal_Show,
		                 const ST_FMT_ATTR FAR_POINTER*Communal_Trans, INT8U dataLen)
{
    ST_FMT_ATTR saveFormat;
    ST_FMT_ATTR showFormat;
	INT8U len;
	INT8U *Data_buff;
	INT8U engyPoint;
	INT8U pwrPoint;
	
	Data_buff = (INT8U *)&Dbuff->showDataBuff;
	saveFormat = *Communal_Save;
	
       GetSingle(E_ENGY_SHOW_POINT, &engyPoint);
       GetSingle(E_PWR_SHOW_POINT, &pwrPoint);

	if(Communal_Show->exp == E_MATUO)
	{
		if(engyPoint < 5)
		{
		    showFormat.exp = (INT8S)(0 - engyPoint);
		}
		else
		{
		    showFormat.exp = -2;
		}
	}	        		
	else if(Communal_Show->exp == E_DATUO)
	{
		if(pwrPoint < 5)
		{
		    showFormat.exp = (INT8S)(0 - pwrPoint);
		}
		else
		{
		    showFormat.exp = -4;
		}
	}
	else
	{
	    showFormat.exp = Communal_Show->exp;
	}
	
	/*"û����ʾ��ʽʱ��ʹ��ͨ�Ÿ�ʽ"*/
	if(Communal_Show->format == 0) 
	{
	    showFormat.format = Communal_Trans->format;
	    showFormat.unit = Communal_Trans->unit;
	}
	else
	{
		showFormat.format = Communal_Show->format;
		showFormat.unit = Communal_Show->unit;
	}	

	len = FRMT_sizeofFormat(showFormat.format);

	if(len <= MAX_SHOWDATA)/*"��ʾ���ݽ��ջ�����Խ����"*/
    {

        len = FRMT_DataChange(Sbuff, Data_buff, saveFormat, showFormat);

        if((showFormat.format > E_TIME_FORMAT) && (showFormat.format < E_STRING))/*"time format"*/
        {/*"ʱ���ʽתBCD����ʾ"*/
            LIB_CharToBcdNByte(Data_buff, len);
        }

        Dbuff->stShowTable.stShowFormat = showFormat;
    }
	else
	{
	    len = 0;
	}

	return len;	
}
/*"*****************************************************************************"*/
/*"  Function:       Get_Leaf_Data_Show"*/
/*"  Description:    ��ʾ��ȡ��ӦҶ�Ӻ����ݶ�Ӧ���ݼ���ʾ��Ϣ��"*/
/*"  Calls:          "*/
/*"  Called By:      PP_ShowGetData"*/
/*"  Input:          Leaf_adrr_list �ڵ��Ӧ��Ҷ���б�"*/
/*"                  Leaf_Num       ��ʾ��ʶ�ڵ���Ҷ����"*/
/*"                  Show_leaf      ��ʾ��Ҷ�Ӻ�            "*/
/*"                  "*/
/*"  Output:         pBuffer  ������ݽ��ջ���"*/
/*"  "*/
/*"  Return:         ��ȡ���ݳ���"*/
/*"  Others:         code size  308byte"*/
/*"*****************************************************************************"*/
static INT8U Get_Leaf_Data_Show(TREE_LEAF_LIST *Leaf_adrr_list,
		                  INT8U Show_leaf, ST_SHOWDATA_OUTPUT *pBuffer)
{
	const struct TREE_LEAF FAR_POINTER*Tree_Leaf;
	const struct COMMUNAL_ID FAR_POINTER*Communal_Id;
	const ST_FMT_ATTR FAR_POINTER*Communal_Show;
	ST_FMT_ATTR Communal_Save;
	const ST_FMT_ATTR FAR_POINTER*Communal_Trans;
	INT16U Revise;
	INT8U dbuf[MAX_FUN_BUFF_SIZE];
	INT8U len_save,len_Trans;
	TREE_LEAFADDR Leaf_addr;
	INT16U len_temp = 0;
	

	LIB_MemCpy((INT8U *)&Leaf_adrr_list[Show_leaf].Tree_addr[0], (INT8U *)&Leaf_addr, 2);
	LIB_MemCpy((INT8U *)&Leaf_adrr_list[Show_leaf].Revise[0], (INT8U *)&Revise, 2);

	Tree_Leaf = (const struct TREE_LEAF FAR_POINTER*)&Protocol_Tree_Leaf[Leaf_addr.Int_addr];
	
	Communal_Id=(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
	Communal_Show=(const ST_FMT_ATTR  FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Show_Id];
	Communal_Trans = (const ST_FMT_ATTR  FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id];
	
	/*"��ʼ��ԭʼ���ݻ�����"*/
	LIB_MemSet(0, (INT8U *)&dbuf, MAX_FUN_BUFF_SIZE);

	/*"��ȡҶ�ӽڵ����ݵ�ԭʼ���ݣ�����ʽת��ǰ���ݣ��洢��ʽ���ݣ�"*/
    len_temp = Get_Leaf_Data(Tree_Leaf, (INT8U *) &dbuf, Revise, &Communal_Save);
    len_save = (INT8U)len_temp;

    /*"����ʾҪ������ݸ�ʽ��������ת��"*/
    if(0 != len_save)
    {
        len_Trans = Data_Transfer_Show(&Communal_Save, (INT8U *) &dbuf, pBuffer,
                                       Communal_Show, Communal_Trans, len_save);
    }
    else
    {
        len_Trans = 0;
    }

	/*"�����ʾ������Ϣ,��ʾ��ʽ��Ϣ��Data_Transfer_Show���Ѿ�����"*/
    LIB_MemCpy((INT8U *)&(Protocol_Showinfo[Tree_Leaf->Communal_Showinfo_Id].schar[0]),
               &(pBuffer->stShowTable.schar[0]), MAX_SCHAR_NUM);

	return len_Trans;		
}
#endif
/*"*****************************************************************************"*/
/*"  Function:       Get_Leaf_Data"*/
/*"  Description:    ��ȡҶ��ԭʼ����(������к�ԭʼ����)"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Leaf_Data_Comm, Get_Leaf_Data_Show,Get_Leaf_Data_Prepay"*/
/*"  Input:          Tree_Leaf Ҷ�ӽڵ��ַָ��                    "*/
/*"                  Revise    ��ַ����ֵ"*/
/*"                  "*/
/*"  Output:         buff  ��ȡ���ݱ����ַָ��"*/
/*"  "*/
/*"  Return:         ��ȡ���ݳ���"*/
/*"  Others:         code size  476byte"*/
/*"*****************************************************************************"*/
static INT16U Get_Leaf_Data(const struct TREE_LEAF FAR_POINTER*Tree_Leaf, INT8U *buff,
					      INT16U Revise, ST_FMT_ATTR *Communal_Save)
{

	const struct COMMUNAL_PURVIEW FAR_POINTER*Communal_Purview;
	const struct COMMUNAL_ID FAR_POINTER*Communal_Id;
	const struct COMMUNAL_FUNCTION FAR_POINTER*Communal_Function;
	const struct COMMUNAL_MATHINFO FAR_POINTER*Communal_Mathinfo;

	INT8U ret;
	INT16U DataLen;
	INT8U math_mode;
	INT8U Onread_result = 0;/*"-1��ʾ�������ݲ���������ڶ�������"*/
	                         /*"0x5a��ʾ�����ڷ�����ѹ���¼��ۼư�ʱֵ����"*/

	INT16U reviseAdrr;
	INT8U i;
	INT8U dataNum;

	INT8U condition;
	INT8U frzOpenFlg;/*"�����������в�֧�ֵ�������ʱ�����ȼ��ģʽ���Ƿ��������û������־��FALSE,����ΪTRUE,������֧�ֵ�������ΪTRUE"*/
	EN_RCRD_FILE_ID  rcrdObis;
    INT16S getRst;
    
    ret=0;

    Communal_Purview =(const struct COMMUNAL_PURVIEW FAR_POINTER*)&Protocol_Purview[Tree_Leaf->Communal_Purview_Id];
    Communal_Id=(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
    Communal_Function=(const struct COMMUNAL_FUNCTION FAR_POINTER*)&Protocol_Function[Tree_Leaf->Communal_Function_Id];
    Communal_Mathinfo=(const struct COMMUNAL_MATHINFO FAR_POINTER*)&Protocol_MATHINFO[Tree_Leaf->Communal_Math_Mode_Id];
    
    /*"�ж������ݵ�Ȩ��"*/
//    DC_GetData(RUN_ENVM_STATE, (INT8U *)&meterCondition, 0, sizeof(UN_RUN_ENVM_STATUS));

    /*"in factory status"*/
    if(ChkStatus(E_FAC) == TRUE)    
    {
        condition = 0x01;
    }
    else
    {
        condition = 0x04;
    }

    if(((Communal_Purview->read_mode & condition) & 0x07) == 0)/*"�ڴ�״̬�²������"*/
    {
    	Error_Infor.ST_ASBITS.errPassword = 1;
    	return 0;
    }
   

    LIB_MemSet(0, (INT8U *)Communal_Save, sizeof(ST_FMT_ATTR));

    rcrdObis = (EN_RCRD_FILE_ID)Tree_Leaf->obis;
    FrzObisUnzip((INT16U *)&rcrdObis);

    noFrzDataFlg = FALSE;
    if(Communal_Function->OnRead != 0)
    {
        Onread_result = Communal_Function->OnRead(buff);
    	/*"FALSE��ʾ�������ݲ������, ���ڶ�������"*/
    	if(Onread_result == FALSE)
    	{
    	    return 0;
    	}
#if defined (THREE_PHASE)
    	else if(Onread_result == 0x5a)/*"0x5a��ʾ�����ڷ�����ѹ���¼��ۼư�ʱֵ����"*/
    	{
    	}
#endif
    	else if(Onread_result == 0x55)/*"0x55��ʾ������û�е����ݣ��践��FF"*/
    	{
    	    INT8U eventTimesOut;

    	    eventTimesOut = TRUE;
    	    frzOpenFlg = TRUE;
    	    /*"�¼���¼�Ͷ����������ж��Ƿ��Ѿ��������Ѿ������Ų�FF��û������0"*/
    	    if((dataId.asBytes[3] == 0x03) || (dataId.asBytes[3] == 0x05) || (dataId.asBytes[3] > 0x0E))
    	    {
    	        eventTimesOut = RCRD_ChkEvntCnt(rcrdObis, fileIndex);
    	    }
    	    if(dataId.asBytes[3] == 0x05)/*"�����������в�֧��������ʱ������ģʽ���Ƿ���"*/
    	    {
    	        if((dataId.asBytes[1] == 0x10) || dataId.asBytes[1] == 0xff)
    	        {
    	            //frzOpenFlg = ReadFrzData(7);
    	            if(E_OBIS_NULL == (INT16U)rcrdObis)
                    {
                        return 0;
                    }     
    	        }
    	    }

    	    if(TRUE == frzOpenFlg)
            {
                if(eventTimesOut == TRUE)
                {
                    if(Tree_Leaf->offSet > MAX_FUN_BUFF_SIZE)
                    {
                        return 0;
                    }
                    LIB_MemSet(0xff, buff, Tree_Leaf->offSet);
                }
                *(Communal_Save) = *((const ST_FMT_ATTR FAR_POINTER*) &Protocol_Format[Communal_Id->Communal_Trans_Id]);
                return Tree_Leaf->offSet;
            }
    	    else
    	    {
    	        noFrzDataFlg = TRUE;
    	        return 0;
    	    }

    	}
    	else if(0xbb == Onread_result)/*"0xbb��ʾ���������ݿ飬�޹������������Ŀ�������貹AA"*/
    	{
    	    LIB_MemSet(0xaa, buff, 8);
    	    return 8;
    	}
    	else if(Onread_result == 0xa5)/*"�������ʾ��������"*/
    	{
    	    Communal_Save->format = E_SHOW_CODE_5_BYTE;
    	    return 5;
    	}

    	else if(Onread_result == 0xaa)/*"���峣������"*/
    	{
            Communal_Save->format = E_UBCD_3_BYTE;
            return 3;
    	}
        else if(8 == Onread_result)   /*���������*/
        {
            Communal_Save->format = E_ASCII_8_LENGTH; 
            return 8;
        }    
    	else if((Onread_result >= 14) && (Onread_result <= 39))/*"�¼��ϱ�״̬�ּ��¼�1~N�����������⴦��"*/
    	{
    	    return Onread_result;
    	}
    }
    
    /*"ȡ��Ϸ�ʽ��"*/	
    if(Communal_Mathinfo->mathModeObis < 0x100)/*"ֻ��һ�ֽ���ЧΪ������"*/
    {    	
    	math_mode =(INT8U)(Communal_Mathinfo->mathModeObis);
    }
    else                                  /*"���ֽ���Чλ��ַ"*/
    {
        //DC_GetData(Communal_Mathinfo->mathModeObis, (INT8U *)&math_mode, 0, 1);
        GetSingle(Communal_Mathinfo->mathModeObis, (INT8U *)&math_mode);
    }         

    reviseAdrr = Revise + Tree_Leaf->offSet;/*"����ƫ�Ƶ�ַ=ѹ��ƫ����+������ƫ����"*/
    DataLen = 0;
    dataNum = 1;
    
    if(Communal_Mathinfo->math_type != 0)/*"�����������������������λ�ȡԭʼ����"*/
    {
        dataNum = Communal_Mathinfo->Ass_data_num;
    }
    else
    {

    } 


    for(i = 0; i < dataNum; i++)
    {
        reviseAdrr += i * Communal_Mathinfo->Ass_data_addr;

        if(Tree_Leaf->obis < 0x7000 || Tree_Leaf->obis > E_STEP_BILL_FRZ_RCRD)/*"��ȡ����������"*/
        {
            GetFrmt(Tree_Leaf->obis, Communal_Save);
            getRst = GetSingle((Tree_Leaf->obis + reviseAdrr), buff + i * FRMT_sizeofFormat(Communal_Save->format));
            if(getRst < MS_SUCCESS)
            {
                return 0;
            }
            else
            {
                DataLen = FRMT_sizeofFormat(Communal_Save->format);
            }
        }
        else                /*"��ȡ��¼�ļ�������"*/
        {
            DataLen = 0;
            if(E_OBIS_NULL == (INT16U)rcrdObis)
            {
                return 0;
            }    
            DataLen = RCRD_ReadRcrdData(rcrdObis, fileIndex, (INT8U)reviseAdrr, (INT8U *)(buff + DataLen),
                                    (ST_FMT_ATTR *) Communal_Save);
        }

    }


    if((Communal_Mathinfo->math_type != 0) && (0xFF != DataLen))/*"����ϼ���"*/
    {
        if((Communal_Save->format > E_S_1_BYTE) && (Communal_Save->format < E_TIME_FORMAT))/*"�޷���16��������ת��Ϊ�з���16������"*/
        {
            Communal_Save->format -= (E_U_4_BYTE-E_S_4_BYTE);
        }
        ret = Ram_math(Communal_Mathinfo->math_type, buff, math_mode, Communal_Mathinfo->Ass_data_num, (INT8U)DataLen);
    }
    else
    {
        ret = (INT8U)DataLen;
    }

    return ret;
}


/*****************************************************************************
 �� �� ��  : Change_Bandvalue_645
 ��������  : ת������������
 �������  : INT8U Value  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2018��9��7��
    ��    ��   : xyt
    �޸�����   : �����ɺ���

300bps��0����   600bps��1����     1200bps��2����
2400bps��3����  4800bps��4����    7200bps��5����
9600bps��6����  19200bps��7����   38400bps��8����
57600bps��9���� 115200bps��10��

#define BPS_19200   0x40          		                   
#define BPS_9600    0x20
#define BPS_4800    0x10   
#define BPS_2400    0x08   
#define BPS_1200    0x04   
#define BPS_600     0x02
#define BPS_INVALID 0x00
#define BPS_7200    0x00 
*****************************************************************************/
const INT8U BandValue[]={BPS_INVALID,BPS_600,BPS_1200,BPS_2400,BPS_4800,BPS_7200,BPS_9600,BPS_19200};
INT8U Change_Bandvalue_645(INT8U Value)
{
    return BandValue[Value];
}    
/*"*****************************************************************************"*/
/*"  Function:       Get_Leaf_Data_Comm"*/
/*"  Description:    ͨ��ģʽ��ȡҶ������(����ͨ��Ҫ��������ݸ�ʽ����ת��)"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Node_Data_Comm"*/
/*"  Input:          Tree_Leaf Ҷ�ӽڵ��ַָ��             "*/
/*"                  Revise    ��ַ����ֵ"*/
/*"                  "*/
/*"  Output:         buff  ��ȡ���ݱ����ַָ��"*/
/*"  "*/
/*"  Return:         ��ȡ���ݳ���"*/
/*"  Others:         code size  180byte"*/
/*"*****************************************************************************"*/
static INT16U Get_Leaf_Data_Comm(const struct TREE_LEAF FAR_POINTER*Tree_Leaf, INT8U *buff,
                          INT16U Offset, INT16U MaxLen ,INT16U Revise)
{
	ST_FMT_ATTR Communal_Save;
	ST_FMT_ATTR Communal_Trans;
	const struct COMMUNAL_ID FAR_POINTER*Communal_Id;
	INT8U dbuf[MAX_FUN_BUFF_SIZE];
	INT8U len_save,len_Trans;
	INT16U len_temp = 0;
	
	
	Communal_Id=(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
	Communal_Trans=*((const ST_FMT_ATTR  FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id]);
	
	/*"ȡ��������dbuf�Ŀ�ʼ�ֽڣ��������ϼ��������"*/
	LIB_MemSet(0, (INT8U *)&dbuf, MAX_FUN_BUFF_SIZE);

	/*"��ȡҶ�ӽڵ����ݵ�ԭʼ���ݣ�����ʽת��ǰ���ݣ��洢��ʽ���ݣ�"*/
	len_temp = Get_Leaf_Data(Tree_Leaf, (INT8U *)&dbuf, Revise, (ST_FMT_ATTR *)&Communal_Save);
	len_save = (INT8U)len_temp;
	
    /*"��ͨѶҪ������ݸ�ʽ��������ת��"*/
    if(0 != len_save)
	{
        len_Trans = len_save;/*"һ����˵���������Ĵ洢�����ݴ�С��С��ͨ�Ŵ�������"*/
        if(len_Trans + Offset > MaxLen)/*"���ϱ�Ҷ�ӵ����ݳ��Ⱥ� �����˻�����"*/
        {
            //NOP();
        }
        else if((0x04000701 == dataId.asLong)||(0x04000703 == dataId.asLong)||(0x04000705 == dataId.asLong))
        {
            *buff = Change_Bandvalue_645(*dbuf);
        }    
        else
        {
            len_Trans = Data_Transfer_Comm((ST_FMT_ATTR *)&Communal_Save, (INT8U *)&dbuf, buff,
	                               (ST_FMT_ATTR *)&Communal_Trans, len_save);
        }
	}
	else
	{
	    len_Trans = 0;
	}

	if(len_Trans == 0xff)  /*"data transfer err"*/
	{
	    return 0;
	}

	return len_Trans;	
}

/*"*****************************************************************************"*/
/*"  Function:       ChkNode"*/
/*"  Description:    �жϽڵ��Ƿ�Ϸ���ָ��Ľڵ�ָ���Ƿ�Ϸ�"*/
/*"  Calls:          "*/
/*"  Called By:      SearchNodeNesting"*/
/*"  Input:          node  �ڵ�ָ��                "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE �Ϸ�    FALSE  �Ƿ�"*/
/*"  Others:         code size  52byte"*/
/*"*****************************************************************************"*/
static INT8U ChkNode(const INT8U FAR_POINTER *node)
{
	const INT8U FAR_POINTER*p_temp;
	INT8U rsult;
	
	rsult = TRUE;

	p_temp = (const INT8U FAR_POINTER*)&Protocol_Tree_Node[0];
	
	if((node < p_temp) ||
	   (node - p_temp) > sizeof(Protocol_Tree_Node)) /*"ָ�봦�ڷǷ�����"*/
	{
	    rsult = FALSE;
	}
	return rsult;
}
/*"�жϽڵ��ڵ�ָ���������Ϊff��ʾ�ڵ������feָ�볬��Χ��0����"*/
/*"code size  32byte"*/
static INT8U check_nodePoint(INT16U node)
{
	if(node == 0xffff)
	{
	    return 0xff;
	}
	else if((node * sizeof(struct TREE_NODE)) > sizeof( Protocol_Tree_Node))
	{
	    return 0xfe;/*"ָ�봦�ڷǷ�����"*/
	}
	else
	{
	    return 0;
	}
}
/*"*****************************************************************************"*/
/*"  Function:       ChkNodeID"*/
/*"  Description:    �ж��Ƿ��ҵ��ڵ�"*/
/*"  Calls:          "*/
/*"  Called By:      SearchNodeNesting"*/
/*"  Input:          id  ���ݱ�ʶ"*/
/*"                  node �ڵ�ָ��   "*/
/*"                  uzip �ڵ�����             "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         0 �Ϸ�    0xff  �Ƿ�        (Jul 30, 2018,16:27:21)CHECK_NODE_RET �ڵ���Ϣ"*/
/*"  Others:         code size  622byte"*/
/*"*****************************************************************************"*/
#define  _NO_ZIP    0
#define  _ZIP_IN_RANGE  1
#define  _ZIP_OUT_RANGE  2
static INT8U ChkNodeID(const struct TREE_NODE FAR_POINTER *node,INT8U uzip_in)
{
    INT8U zipNum;/*"���ݱ�ʶѹ���ֽں�"*/
    INT8U i, block;
    CHECK_NODE_RET ret;
    UN_ID645 id_temp;
    INT8U ID_start = 0, ID_end = 0;
    CHECK_NODE_RET uzip;
    INT8U zipFlg;

    if(node->Node_mode.bit.ID_Enable == 0)/*"���ݱ�ʶ��Ч�����ؽڵ���Ҵ���"*/
    {
        return 0xff;
    }

    uzip.byte = uzip_in;
    zipNum = 0;
    zipNum = node->Node_mode.bit.num;
    /*"���ݱ�ʶ���,��Ϊ��ѹ���ڵ�"*/
    if((NodedataId.asLong == node->ID) && (zipNum == 0xf))/*"���ݱ�ʶ��ȣ������ѹ�������ô���ѹ������"*/
    {
        return 0;
    }

    /*"���ʶ"*/
    id_temp.asLong = node->ID;
    block = 0xff;
    for(i = 0; i < 4; i++)/*"ֻ֧��һ��0XFF������"*/
    {
        if(id_temp.asBytes[i] == 0xFF)
        {
            block = i;
            break;
        }
    }

    ret.byte = uzip.byte;

    /*"���ڵ��ѹ�����ԣ�ѹ�����Ա�����zipFlg������ֵ�����֣�_NO_ZIP ��ѹ����"*/
    /*"_ZIP_OUT_RANGE ��ѹ������Э���ʶ��ѹ����Χ�ڡ�_ZIP_OUT_RANGE ��ѹ������Э���ʶ��ѹ����Χ��"*/
    get_leaf_cyc_para(node, &ID_start, &ID_end);
    zipFlg = _NO_ZIP;
    if(zipNum != 0xf)/*"�ڵ�/Ҷ����ѹ��"*/
    {
        zipFlg = _ZIP_OUT_RANGE;
        if(TRUE == ChkIdRange(zipNum, ID_start, ID_end))/*"���ݱ�ʶ��ѹ����Χ��"*/
        {
            ret.byte = push_zip_info(uzip.byte, zipNum);
            zipFlg = _ZIP_IN_RANGE;
        }
    }

    /*"���Э���ʶ��ڵ��ʶ��һ���ԣ����鳭��ʶλ��ѹ����ʶΪ֮�⣬������ʶλ����һ�£����ڱ��ڵ�򱾽ڵ���"*/
    for(i = 0; i < 4; i++)
    {
        if(NodedataId.asBytes[i] != id_temp.asBytes[i])/*"��ѹ��λ���ݱ�ʶ��һ��"*/
        {
            if(i != block)/*"���ݱ�ʶλΪ�ǿ鳭��ʶλ"*/
            {
                /*"���ݱ�ʶ��ĳһλΪһ��ѹ�������ѹ��"*/
                if(((i == ret.bit.num1) && (ret.bit.zip1 == 1))
                        || ((i == ret.bit.num2) && (ret.bit.zip2 == 1)))
                {

                }
                else/*"��ѹ��λ��ʶ"*/
                {
                    return 0xff;
                }
            }
        }
    }

    if(node->Node_mode.bit.mode == 0)/*"�����Ҷ��"*/
    {
        if(block != 0xff)/*"Ҷ�Ӳ����ܳ��ֿ鳭��ʶ"*/
        {
            return 0xff;
        }
        
        if(_NO_ZIP != zipFlg)/*"��ѹ��Ҷ��"*/
        {
            /*"ѹ��λ����ʶ��ѹ����Χ��,�Ǳ�Ҷ�� ѹ����Ҷ��"*/
            if(_ZIP_IN_RANGE == zipFlg)
            {
                ret.bit.low = 0;
                return ret.byte;
            }
        }
        else/*"�Ǳ�Ҷ�� δѹ����Ҷ��"*/
        {
            ret.byte = 0;
            return ret.byte;
        }
    }
    else /*"����ǽڵ�"*/
    {
        if(block != 0xff)/*"��ڵ�"*/
        {
            if(_NO_ZIP != zipFlg)/*"�����ݿ�ѹ���ڵ�"*/
            {
                /*"ѹ��λ���ݱ�ʶ��ѹ����Χ��"*/
                if(_ZIP_IN_RANGE == zipFlg)
                {
                    if(node->Node_mode.bit.Low_Node_Num == 1)/*"�¼��ڵ������ݱ�ʶ"*/
                    {
                        /*"�ǿ鳭����"*/
                        if(NodedataId.asBytes[block] != id_temp.asBytes[block])
                        {
                            ret.bit.low = 1;
                        }
                        else
                        {
                            ret.bit.low = 0;
                        }
                        return ret.byte;/*"���ݱ�ʶ�ڷ�Χ�ڣ��Ǳ��ڵ���"*/
                    }
                }
            }
            else/*"�����ݿ�ѹ���ڵ�"*/
            {
                ret.byte = 0;
                ret.bit.low = 1;
                return ret.byte;
            }
        }
        else/*"�ǿ鳭���ݱ�ʶ"*/
        {
            if(_NO_ZIP != zipFlg)/*"�����ݿ�ѹ���ڵ�"*/
            {
                /*"ѹ��λ���ݱ�ʶ��ѹ����Χ��"*/
                if(_ZIP_IN_RANGE == zipFlg)
                {
                    if(node->Node_mode.bit.Low_Node_Num == 1)/*"�¼��ڵ������ݱ�ʶ"*/
                    {
                        ret.bit.low = 1;
                    }
                    else
                    {
                        ret.bit.low = 0;
                    }
                    return ret.byte;                         /*"���ݱ�ʶ�ڷ�Χ�ڣ��Ǳ��ڵ���"*/
                }
            }
            else/*"�����ݿ�ѹ���ڵ�"*/
            {
                ret.byte = 0;
                return ret.byte;
            }
        }
    }

    return 0xff;
}
/*"*****************************************************************************"*/
/*"  Function:       ChkIdRange"*/
/*"  Description:    �ж����ݱ�ʶѹ��λ�Ƿ���ѹ����Χ��"*/
/*"  Calls:          "*/
/*"  Called By:      ChkNodeID"*/
/*"  Input:          ZipNum  ���ݱ�ʶѹ��λ"*/
/*"                  IdStart ѹ����ʼ   "*/
/*"                  IdEnd ѹ������            "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE ��ѹ����Χ��    FALSE  ��ѹ����Χ��"*/
/*"  Others:         code size byte"*/
/*"*****************************************************************************"*/
static INT8U ChkIdRange(INT8U ZipNum, INT8U IdStart, INT8U IdEnd)
{
    if((dataId.asBytes[ZipNum] >= IdStart)
        &&(dataId.asBytes[ZipNum] <= IdEnd))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*"*****************************************************************************"*/
/*"  Function:       push_zip_info"*/
/*"  Description:    ��ӽڵ�ѹ����Ϣ"*/
/*"  Calls:          "*/
/*"  Called By:      ChkNodeID"*/
/*"  Input:          val  ���ǰѹ����Ϣ"*/
/*"                  num  ѹ�����ݱ�ʶλ 0~3     "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         ��Ӻ�ѹ����Ϣ"*/
/*"  Others:         code size  64byte"*/
/*"*****************************************************************************"*/
static INT8U push_zip_info(INT8U val,INT8U num)
{
	CHECK_NODE_RET ret;
	
	ret.byte = val;
	if(ret.bit.zip1!=1)
	{
		ret.bit.zip1=1;
		ret.bit.num1=num;
	}	
	else if(ret.bit.zip2!=1)
	{
		ret.bit.zip2=1;
		ret.bit.num2=num;
	}	
	return ret.byte;
}
/*"*****************************************************************************"*/
/*"  Function:       set_zip_id"*/
/*"  Description:    level ����ѹ��λ��(DIn) n = 0~3�����֧��2��ѹ����"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          zip_id  ѹ��λ��ʶ������Ϣ����  :0xff��ʾû�л���ѹ����Ϣ�������Խ���ѹ�� "*/
/*"                  num     ѹ�����ݷ�Χ��ʼֵ  "*/
/*"                  level              "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         ��Ӻ�ѹ����Ϣ  (Jul 30, 2018,14:33:29)��ѹ���˶�������"*/
/*"  Others:         code size  56byte"*/
/*"*****************************************************************************"*/
static INT8U set_zip_id(INT8U *zip_id,INT8U num,INT8U level)
{
	if(level==0xff)
	{	
		if(*zip_id==0xff)
		{
			*zip_id=num;
			return 0;	
		}
		else if(*(zip_id+1)==0xff)	
		{	
			*(zip_id+1)=num;
			return 1;	
		}
		else
			return -1;
	}
	else
	{	
		*(zip_id+level)=num;
		return level;
	}							
}
/*"*****************************************************************************"*/
/*"  Function:       get_leaf_cyc_para"*/
/*"  Description:    ��ȡ�ڵ�ѭ����Ϣ,"*/
/*"                  ������ڵ㲻�ǵ�һ��ڵ���ô��Ҫ���㣬��Ϊѹ�������ѭ������"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Tree_Node  �ڵ���Ϣ"*/
/*"                                 "*/
/*"                  "*/
/*"  Output:         start  �ڵ�ѭ����ʼֵ "*/
/*"                  end    �ڵ�ѭ������ֵ"*/
/*"  "*/
/*"  Return:         ��Ӻ�ѹ����Ϣ"*/
/*"  Others:         code size  162byte"*/
/*"*****************************************************************************"*/
static void get_leaf_cyc_para(const struct TREE_NODE FAR_POINTER*Tree_Node,INT8U *start,INT8U *end)
{
	const struct COMMUNAL_RANGE FAR_POINTER*Communal_Range;
	INT8U ID_start = 0, ID_end = 0;
	
	Communal_Range = (const struct COMMUNAL_RANGE  FAR_POINTER*)&Protocol_Range[Tree_Node->Range_ID];

	if(Communal_Range->idStartObis < 0x100)/*"ID_start ֻ�������������������û�п���else���"*/
	{
	    ID_start = (INT8U)Communal_Range->idStartObis;
	}

	
	if(Communal_Range->idEndObis < 0x100)
	{	
		ID_end = (INT8U)Communal_Range->idEndObis;
		FrzDataZipEnd(&ID_end);
	}
	else
	{	    
        GetSingle(Communal_Range->idEndObis, (INT8U *)&ID_end);
	    if(Communal_Range->idEndObis == E_TARIFF_NUM)
	    {
            /*"���ʱ����¼���������С��4ʱ��4���ʷ���"*/
            if((dataId.asLong & 0xffffff00) == 0x03300f00)
            {
                if(ID_end < 4)
                {
                    ID_end = 4;
                }
            }
            else
            {
                /*"����������4��ǵ����鳭(�������ݵ����鳭��ʶ��Ϊ0xff���ų�)�����ʱ�����ΧΪ�����洢��������"*/
                if((ID_end > (INT8U) (_TARIFF_NUM - 1))|| ((dataId.asBytes[1] != 0xff)
                    && (dataId.asBytes[3] != 0x05)))
                {
                    ID_end = (INT8U) (_TARIFF_NUM - 1);
                }
                else
                {

                }
            }
	    }
	    else
	    {

	    }
	}

    /*2018-07-20 �ڵ�Ҷ����ѹ��*/         
	if(Tree_Node->Node_mode.bit.num != 0xf)
	{
		*start = ID_start;
		*end = ID_end;
	}
	else
	{
		*start = 0XFF;
		*end = 0xFF;
	}			
}
/*"============================================================"*/
/*"��������"*/
/*"��;����ַ��������"*/
/*"������"*/
/*"���أ�"*/
/*"���ߣ�trend �汾��1.00   code size  214byte"*/
/*"============================================================"*/
static INT16U Revise_Addr(INT16U leaf_addr,const struct COMMUNAL_REVISE FAR_POINTER*REVISE ,INT8U ID)
{
	INT16U num;
	INT8U basce;             /*"��ַ������ʼ��"*/
		
	num = 0;
	if((REVISE->reviseMode & 0x01) == 0)/*"ƫ�Ʋ�����Ч�����򲻲�������"*/
	{
		return leaf_addr;
	}

	basce = REVISE->reviseBase;
			
	if((REVISE->reviseMode & 0x02) != 0)/*"��ƫ��,Ŀǰ��֧��"*/
	{		
	    //todo   add debug code
	}
	else /*"��ƫ��"*/
	{		
		if((REVISE->reviseMode & 0x04) == 0)/*"��ѭ��"*/
		{
			if((INT8U)(ID + basce) > REVISE->reviseMaxNum)
			{
				return leaf_addr;
			}
			else
			{
				num = (INT8U)(basce + ID);
			}							
		}
		else    /*"ѭ��,Ŀǰ��֧��"*/
		{
		    //todo   add debug code
		}
	}
	
	leaf_addr += (num * REVISE->reviseSize);
	return leaf_addr;
}

/*"============================================================"*/
/*"��������Ram_math"*/
/*"��;��������ݼ���"*/
/*"������mode ������ݼ��㷽�� ���Ӽ���/���ֵ��/��������ʱ���㷨"*/
/*"     getbuff  �������Դ"*/
/*"     math_mode  ��Ϸ�ʽ"*/
/*"     Ass_num  ������ݸ���"*/
/*"     dataLen  �������ݳ���"*/
/*"���أ����ݳ���"*/
/*"���ߣ�GANJP �汾��1.00  code size  78byte"*/
/*"============================================================"*/
static INT8U Ram_math(INT8U mode, INT8U *getbuff ,INT8U math_mode,
			   INT8U Ass_num, INT8U dataLen)
{
	INT8U ret;
	if(0 == dataLen)
	{
	    return 0;
	}

	switch(mode)
	{
		case 1:
			ret = math_Add(getbuff, math_mode,Ass_num, dataLen);
			break;
#if defined (THREE_PHASE)
		case 2:
		    //todo
		    break;
#endif
		default:
			ret = 0;
			break;
	}

	return ret;
}
//------------------------------------------------------------------------------
#if defined (THREE_PHASE)
struct ENERGY_MATH_BUFFER
{
    INT32S  highdata;
    INT16U  raildata;
};
struct ENERGY_MATH_SUM
{
    INT32S  highdata;
    INT16S  raildata;
};
#endif
/*"*****************************************************************************"*/
/*"  Function:       math_Add"*/
/*"  Description:    ������ϼ���,�����������ģʽ�֣������������ֵ"*/
/*"  Calls:"*/
/*"  Called By:"*/
/*"  Input:          startaddr  ������ϼ���������ʼ��ַָ��,"*/
/*"                             ���ΪEEPROM��FLASH��ָ��ָ�����EEPROM��FLASH��ַ"*/
/*"                  math_mode  ��ϼ���ģʽ"*/
/*"                  format     ���ݸ�ʽ"*/
/*"                  Ass_num    ������ϼ����Ա��"*/
/*"                  Ass_addr   ������ϼ����Աƫ�Ƶ�ַ"*/
/*"                  filename   0~5000 Ϊflash�ļ�����"*/
/*"                             RAM_DEMAND_MATH��ʾRAM������"*/
/*"                             EEPROM_DEMAND_MATH��ʾEEPROM������"*/
/*""*/
/*"  Output:         getbuff  ��ϼ�����"*/
/*""*/
/*"  Return:         ���ݳ���"*/
/*"  Others:         code size  278byte"*/
/*����汾��Եײ�Ϊ5�ֽڵ����ݣ�ת��ʱ���Ǻܺô��� (Oct 23, 2018,10:16:39)*/
/*"*****************************************************************************"*/
static INT8U math_Add(INT8U *getbuff, INT8U math_mode, INT8U Ass_num, INT8U len)
{

	INT8U i;
	ENGY_TYPE_S buff[4];
	ENGY_TYPE_S sum;
		 
	LIB_MemSet(0, (INT8U *)&buff, 4 * sizeof(ENGY_TYPE_S));
	
	for(i = 0; i < Ass_num; i++)
	{
	    LIB_MemCpy(getbuff + i * len, (INT8U *)&buff[i], len);
	}

	

	/*"���"*/
	sum = 0;
	for(i = 0; i < Ass_num; i++)
	{
		switch(math_mode & 0x03)
		{
			case 0x01:
			    sum += buff[i];
				break;
			case 0x02:
			    sum -= buff[i];
				break;
			default:
				break;
		}
        math_mode >>= 2;
	}
    if(sizeof(ENGY_TYPE_S) < 5)
    {
        sum %= 800000000l;
    }
    else
    {
        sum %= 80000000000l;
    }
    
	LIB_MemCpy((INT8U *)&sum, getbuff, sizeof(ENGY_TYPE_S));

	return (sizeof(ENGY_TYPE_S));
}

/*"code size  174byte"*/
static INT8U Data_Transfer_Comm(const ST_FMT_ATTR FAR_POINTER*Communal_Save, INT8U *Sbuff, INT8U *Dbuff,
		                 const ST_FMT_ATTR FAR_POINTER*Communal_Trans, INT8U dataLen)
{
	ST_FMT_ATTR saveFormat;
	ST_FMT_ATTR tranFormat;
	INT8U len;

    INT8U format_save;
    INT8U format_tran;

    format_save = Communal_Save->format;
    format_tran = Communal_Trans->format;
    /*ASCII��ֻ����*/
    if((format_save >=E_ASCII_2_LENGTH)&&(format_save <=E_ASCII_32_LENGTH))
    {
        LIB_Reverse(Sbuff, Dbuff, FRMT_sizeofFormat(format_tran));
        return FRMT_sizeofFormat(format_tran); 
    }    
    
	/*"ȫ0 �� û�����ݸ�ʽ���ݲ���������ת��"*/
	if((LIB_ChkValue(Sbuff, dataLen, 0) == EQUAL)
	        || ((format_tran == 0) && (format_save == 0)) || (format_save == E_NONE_FORMAT))
	{
	    if(0 !=format_tran)
	    {
	        len = FRMT_sizeofFormat(format_tran);
	    }
	    else
	    {
	        len = dataLen;
	    }

	    LIB_MemCpy(Sbuff, Dbuff, len);
		return len;
	}
    
	saveFormat = *(Communal_Save);
	tranFormat = *(Communal_Trans);
	
	if(Communal_Trans->exp == E_MATUO)
	{
	    tranFormat.exp = -2;
	}
	else if(Communal_Trans->exp == E_DATUO)
	{
	    tranFormat.exp = -4;
	}
#ifdef THREE_PHASE
    else if (Communal_Trans->exp == E_RMB_MAUTO)//payment
    {
        tranFormat.exp = -2;
    }
#endif
	else
	{
	    tranFormat.exp = Communal_Trans->exp;
	}

    if(E_U_6_BYTE == format_save)  /*ͨ�ŵ�ַ����ŷ���*/
    {
        LIB_Reverse(Sbuff,Dbuff,6);
        len = 6;
    } 
    else
    {        
    	//len = FRMT_DataChange(Sbuff, Dbuff, saveFormat, tranFormat);
        len = FRMT_NdataChange(Sbuff, Dbuff, saveFormat, tranFormat,dataLen);
    }
    
	return len;	
}

extern INT8U BeforeFunSetSystemTime(INT8U *pdata);
/*"*****************************************************************************"*/
/*"  Function:       Set_SystemTime_YMDH"*/
/*"  Description:    ��645��������д�����������·��������ݽ��кϷ��Լ�飬������Уʱ�¼���¼"*/
/*"  Calls:          SaveEvent_TimeDot��Postfix_WriteDatetime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata �·��������ݵ�ַָ��  "*/
/*"  Input:          sort   0 set data  1   set time  2 set data and time"*/
/*"  Output:         ��"*/
/*"  "*/
/*"  Return:         -1 �������ò��ɹ�  0 �������óɹ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U  SetSysTime(INT8U *pdata, INT8U sort)
{
    INT8U result;
    ST_7TIME_FORM JustTime;    
    INT8U len;/*" time data len"*/

    len = 0;
    result = TRUE;
    switch(sort)
    {
    case 0:
        len = 4;
        break;
    case 1:
        len = 3;
        break;
    case 2:
        len = 7;
        break;
    default:
        break;
    }

    if(FALSE == LIB_CheckBCD(pdata, len))/*"INPUT DATA IS NOT BCD FORMAT"*/
    {
        return FALSE;
    }
     
    GetSingle(E_SYS_TIME, (INT8U *)&JustTime.year);
    
    LIB_BcdToCharNByte(pdata,7);    /*������ô��������7����BCDת��HEX*/
    
    switch(sort)/*"�ܴ���дRTC֮ǰ���Զ����㣬������Բ��ù�ͨ���·�ֵ"*/
    {
        case 0:            
            LIB_RvsSelf(pdata + 1, 3);
            LIB_MemCpy(pdata + 1, (INT8U *)&JustTime.year, 3);
            break;
        case 1:
            
            LIB_RvsSelf(pdata, 3);
            LIB_MemCpy(pdata, (INT8U *)&JustTime.hour, 3);
            break;
        case 2:
            
            LIB_RvsSelf(pdata + 4, 3);
            LIB_MemCpy(pdata + 4, (INT8U *)&JustTime.year, 3);
            
            LIB_RvsSelf(pdata, 3);
            LIB_MemCpy(pdata, (INT8U *)&JustTime.hour, 3);
            break;
        default:
            break;
    }

    if(DAR_0_OK == BeforeFunSetSystemTime((INT8U*)&JustTime))
    {
       result = TRUE;
    }    
    else
    {
       result = FALSE;
    }    
    

    return result;
}
/*"*****************************************************************************"*/
/*"  Function:       setSysTimeYMDW"*/
/*"  Description:    645��������д������"*/
/*"  Calls:          SetSysTime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata �·��������ݵ�ַָ��  "*/

/*"  Output:         ��"*/
/*"  "*/
/*"  Return:         -1 ʱ�����ò��ɹ�  0 ʱ�����óɹ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U setSysTimeYMDW(INT8U *pdata)
{
    INT8U rsult;

    rsult = SetSysTime(pdata ,0);

    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       setSysTimeHMS"*/
/*"  Description:    645����ʱ��д������"*/
/*"  Calls:          SetSysTime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata �·�ʱ�����ݵ�ַָ��  "*/

/*"  Output:         ��"*/
/*"  "*/
/*"  Return:         -1 ʱ�����ò��ɹ�  0 ʱ�����óɹ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U setSysTimeHMS(INT8U *pdata)
{
    INT8U rsult;

    rsult = SetSysTime(pdata ,1);

    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       setSysTimeAll"*/
/*"  Description:    645�������ڡ�ʱ��д������"*/
/*"  Calls:          SetSysTime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata �·����ڡ�ʱ�����ݵ�ַָ��  "*/

/*"  Output:         ��"*/
/*"  "*/
/*"  Return:         -1 ʱ�����ò��ɹ�  0 ʱ�����óɹ�"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U setSysTimeAll(INT8U *pdata)
{
    INT8U rsult;

    rsult = SetSysTime(pdata ,2);

    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       readNosupportData"*/
/*"  Description:    645Э���б��û�е����ݣ���FF��䷵��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         len       "*/
/*"  Others:         code size  6byte"*/
/*"*****************************************************************************"*/
INT8U  readNosupportData(INT8U *pdata)
{
    return 0x55;
}

/*"*****************************************************************************"*/
/*"  Function:       readHRVoltCurr"*/
/*"  Description:    ��������г�����κ�����"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         len       "*/
/*"  Others:         code size  6byte"*/
/*"*****************************************************************************"*/
#define R_ALLH      0x206F  /*"     R       48	    -----           �����г������"*/
extern void  MU_ReadInstantData(INT16U reg);
INT8U  readHRVoltCurr(INT8U *pdata)
{
    MU_ReadInstantData(R_ALLH); /*"�����г������"*/
    return TRUE;
}

#if 0
/*"*****************************************************************************"*/
/*"  Function:       readDis"*/
/*"  Description:    ������ѭ��/������ʾ�������������ʾ��������ʾ�����ת��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         ���⴦���־��0xa5"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U readDis(INT8U *pdata)
{
    INT8U rsult; 
    
    //rsult = GetDlt645DispItem(pdata, (dataId.asBytes[1] - 1), dataId.asBytes[0]);
    if(rsult == TRUE)
    {
        rsult = 0xA5;
    }
    return rsult;
}

INT8U setDis(INT8U *pdata)
{
    INT8U rsult; 
    
    //rsult = SetDlt645DispItem(pdata, (dataId.asBytes[1] - 1), dataId.asBytes[0]);
    return rsult;
}
#endif

/*"****************************************************************************
*"*/
/*"  Function:       ReadFrzData"*/
/*"  Description:    
�������ݶ�ǰ����ӿں��������ݶ���ģʽ���ж���Щ���������ǿɶ���"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Type      ����������"*/
/*"  Output          "*/
/*"  Return:         _TRUE �������ݿɶ�  _FALSE �������ݲ��ɶ�"*/
/*"  Others:         code size  byte"*/
/*"****************************************************************************
*"*/
static ST_FRZPARA_1HEX FrzPara;
typedef struct
{
    INT16U id645_H2;
    INT8U  *pfrzMode;
}ST_FRZ_READ;
const ST_FRZ_READ frzRead[10] =
        {
                {0x0500, (INT8U *)&FrzPara.timeFrzMode.byte},
                {0x0501, (INT8U *)&FrzPara.mmtFrzMode.byte},
                {0x0502, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0503, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0504, (INT8U *)&FrzPara.prdFrzMode.byte},
                {0x0505, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0506, (INT8U *)&FrzPara.dayFrzMode.byte},
                {0x0507, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0509, (INT8U *)&FrzPara.aptFrzMode.byte},
/*������չ�ѿ�ģʽ�л�����*/
                {0x050A, (INT8U *)&FrzPara.aptFrzMode.byte},
/*������չ����ʱ���л�����*/

        };
static INT8U ReadFrzData(INT8U Type)
{
    INT8U i;
    INT8U rsult;

    rsult = FALSE;

    //DC_GetData(FRZ_PARA_1HEX, (UINT8 *) &FrzPara, 0, sizeof(ST_FRZPARA_1HEX));
    FrzPara.dayFrzMode.byte = 0xff;
    FrzPara.prdFrzMode.byte = 0X03;

    for(i = 0; i < 10; i++)
    {
        if((frzRead[i].id645_H2 == dataId.asWord[1]) && ((*(frzRead[i].pfrzMode)) & (1 << Type)))
        {
            rsult = TRUE;
            break;
        }
    }

    if(FALSE == rsult)
    {
        noFrzDataFlg = TRUE;
    }

    return rsult;
}
/*" ���������ݵ�1������ "*/
INT8U  ReadFrzBit0Data(INT8U *pdata)
{

    return ReadFrzData(0);
}

/*" ���������ݵ�2 ������ "*/
INT8U  ReadFrzBit1Data(INT8U *pdata)
{

    return ReadFrzData(1);
}

/*" ���������ݵ�8������ "*/
INT8U  ReadFrzBit7Data(INT8U *pdata)
{

    return ReadFrzData(7);
}

/*"*****************************************************************************"*/
/*"  Function:       setBaudRate"*/
/*"  Description:    �������ĺϷ���"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         TRUE ����������Χ��  FALSE ����������Χ��"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U setBaudRate(INT8U *pdata)
{
    INT8U rsult;
#if 0
    INT8U commId;/*"ͨ�ſں�"*/
    INT8U id0;
#endif
    rsult = TRUE;
#if 0
    id0 = (INT8U)dataId.asLong;
    switch(id0)
    {
    case 0x01:/*"0x04000701"*/
        commId = E_COM_IR;
        break;
    case 0x03:/*"0x04000703"*/
        commId = E_COM_RS485_1;
        break;
    case 0x05:/*"0x04000705"*/
        commId = E_COM_CM;
        break;
    default:
        rsult = FALSE;
        break;
    }
#endif
    if(TRUE == rsult)
    {
        //rsult = CM_SetBps(commId,pdata);
        rsult = FALSE;
        if(FALSE == rsult)
        {
            Error_Infor.ST_ASBITS.errBaud = 1;
        }
    }

    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       setActEngyMode"*/
/*"  Description:    �����й����ģʽ�ֲ���дǰ����ӿں������������ĺϷ���"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         TRUE ����������Χ��  FALSE ����������Χ��"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U setActEngyMode(INT8U *pdata)
{
    if(*(pdata) > 0x0f)
    {
        return FALSE;
    }

//    DC_SaveRcrdData(OBIS_PRG_AEMODE, EVTLST_PRGAEMODE);/*"�й����ģʽ�ֱ���¼�"*/

    return TRUE;
}
/*"*****************************************************************************"*/
/*"  Function:       setCommAddr"*/
/*"  Description:    ����ͨ�ŵ�ַ����дǰ����ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         TRUE ����������Χ��  FALSE ����������Χ��"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U setCommAddr(INT8U *pdata)
{
    if(FALSE == LIB_CheckBCD(pdata, 6))
    {
        return FALSE;
    }

    return TRUE;
}
#if 0
/*"*****************************************************************************"*/
/*"  Function:       ReadFrzData"*/
/*"  Description:    �������ݶ�ǰ����ӿں��������ݶ���ģʽ���ж���Щ���������ǿɶ���"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Type      ����������"*/
/*"  Output          "*/
/*"  Return:         TRUE �������ݿɶ�  FALSE �������ݲ��ɶ�"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
static ST_FRZPARA_1HEX FrzPara;
typedef struct
{
    INT16U id645_H2;
    INT8U  *pfrzMode;
}ST_FRZ_READ;
const ST_FRZ_READ frzRead[8] =
        {
                {0x0500, (INT8U *)&FrzPara.timeFrzMode.byte},
                {0x0501, (INT8U *)&FrzPara.mmtFrzMode.byte},
                {0x0502, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0503, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0504, (INT8U *)&FrzPara.prdFrzMode.byte},
                {0x0505, (INT8U *)&FrzPara.aptFrzMode.byte},
                {0x0506, (INT8U *)&FrzPara.dayFrzMode.byte},
                {0x0507, (INT8U *)&FrzPara.aptFrzMode.byte}
        };
static INT8U ReadFrzData(INT8U Type)
{
    INT8U i;
    INT8U rsult;

    rsult = FALSE;

    DC_GetData(FRZ_PARA_1HEX, (INT8U *) &FrzPara, 0, sizeof(ST_FRZPARA_1HEX));

    for(i = 0; i < 8; i++)
    {
        if((frzRead[i].id645_H2 == dataId.asWord[1]) && ((*(frzRead[i].pfrzMode)) & (1 << Type)))
        {
            rsult = TRUE;
            break;
        }
    }

    if(FALSE == rsult)
    {
        noFrzDataFlg = TRUE;
    }

    return rsult;
}
/*" ���������ݵ�1������ "*/
INT8U  ReadFrzBit0Data(INT8U *pdata)
{

    return ReadFrzData(0);
}

/*" ���������ݵ�2 ������ "*/
INT8U  ReadFrzBit1Data(INT8U *pdata)
{

    return ReadFrzData(1);
}

/*" ���������ݵ�8������ "*/
INT8U  ReadFrzBit7Data(INT8U *pdata)
{

    return ReadFrzData(7);
}

/*" Type = 0x01 ��ʾ ���㵱ǰ����������"*/
/*" Type = 0x02 ��ʾ ������һ����������"*/
void ReadBillEgy(INT8U Type)
{
    INT32U TmpEngy1[2] = {0,0};/*"��δ����1�ν�������ʱ��Ĭ��Ϊ0"*/
    INT32U TmpEngy2[2] = {0,0};/*"��δ����2�ν�������ʱ��Ĭ��Ϊ0"*/
    INT32U TmpResult[2];
    INT16U TmpObis;

    /*"ȡ��1�ν��� �������"*/
    DC_ReadRcrdData(OBIS_ENGY_BILL, 1, POACT_OFFSET , (INT8U *)&TmpEngy2[0], NULL);
    DC_ReadRcrdData(OBIS_ENGY_BILL, 1, NEACT_OFFSET , (INT8U *)&TmpEngy2[1], NULL);

    if(Type == 0x01)
    {
        /*"ȡ��ǰ��\�����й��ܵ���"*/
        DC_GetData(POACT_ENGY, (INT8U *)&TmpEngy1[0], 0, 4);
        DC_GetData(NEACT_ENGY, (INT8U *)&TmpEngy1[1], 0, 4);

        /*"��ǰ��ȥ��1��"*/
        TmpResult[0] =  TmpEngy1[0] - TmpEngy2[0];
        TmpResult[1] =  TmpEngy1[1] - TmpEngy2[1];
        TmpObis = NOW_PRD_ENGY;
    }
    else
    {
        /*"ȡ��2�½���������"*/
        DC_ReadRcrdData(OBIS_ENGY_BILL, 2, POACT_OFFSET , (INT8U *)&TmpEngy1[0], NULL);
        DC_ReadRcrdData(OBIS_ENGY_BILL, 2, NEACT_OFFSET , (INT8U *)&TmpEngy1[1], NULL);

        /*"��1�μ�ȥ��2�ν������"*/
        TmpResult[0] =  TmpEngy2[0]- TmpEngy1[0];
        TmpResult[1] =  TmpEngy2[1] - TmpEngy1[1];
        TmpObis = LST_PRD_ENGY;
    }

    DC_OutputData(TmpObis, (INT8U *)(&TmpResult), 0, 8);

}

/*"��Ӧ���ݱ�ʶ000B0000,������ǰ������������й����ۼ��õ���"*/
INT8U ReadNowBillEgy(INT8U *pdata)
{
    ReadBillEgy(1);

    return TRUE;
}


/*"��Ӧ���ݱ�ʶ000B0001,������һ������������й����ۼ��õ���"*/
INT8U ReadLastBillEgy(INT8U *pdata)
{
    ReadBillEgy(2);
    return TRUE;
}

/*"���ص�ص�ѹ"*/
INT8U OutPut_BattVal(INT8U *pdata)
{
    INT16U Tmp;
    Tmp = GP_GetBattery_Val();/*"������������Ϊ��λV ��λС��"*/
    return DC_OutputData(BAT_VOLT, (INT8U *) &Tmp, 0, 2);

}
#endif

/*"���طѿر�ר��Э���б�LocalProtocolIDΪ���������б�LocalProtocolZipIDΪѹ�������б��б��в�������������"*/
#define  _LOCAL_PRO_ID_MAX    7
const INT32U LocalProtocolID[_LOCAL_PRO_ID_MAX] =
        {
                0x0280000B,      /*"��ǰ���ݵ��"*/
                0x02800020,      /*"��ǰ���"*/
                0x02800021,      /*"��ǰ���ʵ��"*/
                0x04000108,      /*"���׷�ʱ�����л�ʱ��"*/
                0x04000109,      /*"���׽����л�ʱ��"*/
                0x04000207,      /*"������"*/
                0x04001502       /*"�忨״̬��"*/
        };
#define _LOCAL_PRO_ZIP_ID_MAX    18
const INT32U LocalProtocolZipID[_LOCAL_PRO_ZIP_ID_MAX] =
        {
                0x000d0000,  /*"���������/�������й����ۼ��õ���"*/
                0x00900200,  /*"ʣ����/͸֧���"*/
                0x03300f00,  /*"���ʱ���"*/
                0x03301000,  /*"���ݱ���"*/
                0x03301300,  /*"�쳣�忨"*/
                0x03301400,  /*"�Ƿ��忨�ܴ���"*/
                0x03330100,  /*"��������"*/
                0x03330200,  /*"������ܹ������"*/
                0x03330300,  /*"������"*/
                0x03330400,  /*"����ǰʣ����"*/
                0x03330500,  /*"�����ʣ����"*/
                0x03330600,  /*"������ۼƹ�����"*/
                0x03340000,  /*"�˷Ѽ�¼"*/
                0x04001000,  /*"�������1/2/͸֧�����ֵ/�ڻ������ֵ/��բ��������ֵ/�˷ѽ��"*/
                0x04050100,  /*"��ǰ�׷��ʵ��"*/
                0x04050200,  /*"�����׷��ʵ��"*/
                0x04060400,  /*"��ǰ�׽���"*/
                0x04060500   /*"�����׽���"*/
        }; 
/*"*****************************************************************************"*/
/*"  Function:       LocalProtocolChk"*/
/*"  Description:    ���ݱ����жϣ��Ƿ�֧�ִ�Э��:"*/
/*"                     1��Զ�̱�֧�ֱ��ر�Э��"*/
/*"  Calls:          "*/
/*"  Called By:      Э�����ݶ�д�ӿں���"*/
/*"  Input:          ��"*/
/*"  Output          "*/
/*"  Return:         TRUE ֧�ֱ�Э��  FALSE ��֧�ֱ�Э��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U LocalProtocolChk(void)
{
    INT8U uni;
    INT8U rsult;
    
    INT8U MeterType;

    MeterType = 0;
#ifdef MAIN_METER    
    GetSingle(E_METER_MODE, (INT8U *)&MeterType); 
#endif
    if((LOCALMETER == (MeterType&METERTPYE))||(TRUE == ChkStatus(E_FAC)))
    {
         return TRUE;
    }    
    else/*"Զ�̱��������Ͳ�֧�ֱ��ر�ר������"*/
    {
        rsult = FALSE;
        for(uni = 0; uni < _LOCAL_PRO_ID_MAX; uni++)
        {
            if(dataId.asLong == LocalProtocolID[uni])
            {
                rsult = TRUE;
                break;
            }
        }
        
        if(FALSE == rsult)
        {
            for(uni = 0; uni < _LOCAL_PRO_ZIP_ID_MAX; uni++)
            {
                if((dataId.asLong & 0xffffff00)== LocalProtocolZipID[uni] )
                {
                    rsult = TRUE;
                    break;
                }
            }
        }
        
        if(FALSE == rsult)
        {
            INT16U id645_H16;

            id645_H16 = (INT16U) (dataId.asLong >> 16);
            /*"��ʱ�����л�/�����ݶ��л�/ʣ�����ն���"*/
            if((id645_H16 == 0x0505) || (id645_H16 == 0x0507) || (id645_H16 == 0x0508))
            {
                rsult = TRUE;
            }
        }

        if(TRUE == rsult)
        {
            Error_Infor.ST_ASBITS.errObis = 1;
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }
    
    //return TRUE;
}

const  INT8U frzId2List[7] = {0, 0x01, 0x02, 0x03, 0x05, 0x06, 0x07};

/*"*****************************************************************************"*/
/*"  Function:       ChkFrzDataZip"*/
/*"  Description:    �Ծ�����ͬЭ��ṹ�Ķ����¼ѹ������"*/
/*"  Calls:          "*/
/*"  Called By:      ProtocolTreeChk"*/
/*"  Input:          ��"*/
/*"  Output          ��"*/
/*"  Return:         0~6  �����¼��ѹ��  0xff �����¼��ѹ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U ChkFrzDataZip(void)
{
    INT8U i;
    INT8U rsult;

    rsult = 0xff;
    if(0x05 == dataId.asBytes[3])/*"��������"*/
    {
        for(i = 0; i < 7; i++)
        {
            if(dataId.asBytes[2] == frzId2List[i])/*"����ѹ���Ķ���������"*/
            {
                NodedataId.asBytes[2] = 0;
                rsult = i;
                break;
            }
        }
    }
    else
    {
    }

    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       FrzDataZipEnd"*/
/*"  Description:    �Ծ�����ͬЭ��ṹ�Ķ����¼ѹ������,ѹ��Э����N�ν���λ��ѹ����"*/
/*"  Calls:          "*/
/*"  Called By:      get_leaf_cyc_para"*/
/*"  Input:          ��"*/
/*"  Output          ��"*/
/*"  Return:         ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
const INT8U rcrdCntZipEnd[7] =
    {
            _NUM_TIME_FRZ,
            _NUM_MMT_FRZ,
            _NUM_YSLOT_SW_FRZ,
            _NUM_DSLOT_SW_FRZ,
            _NUM_TARIFF_SW_FRZ,
            _NUM_DAY_FRZ,
            _NUM_STEP_SW_FRZ
    };
void FrzDataZipEnd(INT8U *pEndId)
{
    INT8U num;

    num = ChkFrzDataZip();
    if(num < 7)
    {
        if(_NUM_TIME_FRZ == *pEndId)
        {
            *pEndId = rcrdCntZipEnd[num];
        }
    }
}

/*"*****************************************************************************"*/
/*"  Function:       FrzDataZipEnd"*/
/*"  Description:    �Ծ�����ͬЭ��ṹ�Ķ����¼ѹ������,ѹ��Э�鶳�����ݶ����ѹ����"*/
/*"  Calls:          "*/
/*"  Called By:      ProtocolTreeChk"*/
/*"  Input:          ��"*/
/*"  Output          ��"*/
/*"  Return:         ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
/*
const INT16U frzObis[7] =
{
        E_HOUR_FRZ_RCRD,
        E_MMT_FRZ_RCRD,
        E_SEASON_TBL_SW_FRZ_RCRD,
        E_DAY_TBL_SW_FRZ_RCRD,
        E_TRIFF_TBL_SW_FRZ_RCRD,
        E_DAY_FRZ_RCRD,
        E_STEP_TBL_SW_FRZ_RCRD
};
*/

const INT16U frzObis[7] =
{
        E_OBIS_NULL,
        E_OBIS_NULL,
        E_OBIS_NULL,
        E_OBIS_NULL,
        E_OBIS_NULL,
        E_DAY_FRZ_RCRD,
        E_OBIS_NULL
};

void FrzObisUnzip(INT16U *pDataObis)
{
    INT8U num;

    num = ChkFrzDataZip();
    if(num < 7)
    {
        *pDataObis = frzObis[num];
    }
}

/*"*****************************************************************************"*/
/*"  Function:       ChkTariffPrice"*/
/*"  Description:    �ж����ݱ�ʶΪ���ʵ�����ݿ��ʶ�����ʵ�����ݿ����ʱ�谴ʵ�ʷ��������ط��ʵ�����ݿ�"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Node_Data_Comm"*/
/*"  Input:          ��"*/
/*"  Output          "*/
/*"  Return:         TRUE ���ʵ�۱����ݱ�ʶ  FALSE ���ʵ�۱����ݱ�ʶ"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U ChkTariffPrice(void)
{
    //INT16U id645_H16;

    if((0x040501FF == dataId.asLong) || (0x040502FF == dataId.asLong) )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*"*****************************************************************************"*/
/*"  Function:       readFrzBlk"*/
/*"  Description:    ���������ݿ�ʱ�������֧�ֵ��޹����������ݿ鱣���������AA"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      �������ָ��"*/
/*"  Output          "*/
/*"  Return:         0xBB"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U readFrzBlk(INT8U *pdata)
{
    return 0xBB;
} 


