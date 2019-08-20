/*"*****************************************************************************"*/
/*"  FileName: protocol_tree.c"*/
/*"  Author: ganjp       Version :  V1.0        Date:2012.1.11"*/
/*"  Description:     该文件提供对外数据交换接口函数，包括通信读、通信写、显示数据接口"*/
/*"                   数据传递采用查询协议树方式 ，找到数据标识对应的叶子，进行相应的数"*/
/*"                   据处理。     "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"    1. PP_CommGetData   通信读数据接口函数"*/
/*"    2. PP_CommSetData   通信写数据接口函数"*/
/*"    2. PP_ShowGetData   显示读数据接口函数"*/
/*"  History:         // 历史修改记录"*/
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
static UN_COMM_ERR Error_Infor;/*"通信错误信息类型"*/
static UN_ID645 dataId;      /*"645_2007协议数据标识"*/
static UN_ID645 NodedataId;      /*"用于节点遍历的数据标识，用于具有相同协议属性的协议进行压缩处理"*/
//static INT8U SetParaFuncFlg;/*"写前处理标志，一个标识对应多个叶子时，写前处理只在处理第一个叶子的时候调用"*/
static INT16U fileIndex;    /*"记录文件索引，读记录文件时，用于解压上几次记录文件"*/
static INT8U  noFrzDataFlg;/*"读负荷冻结模式字时，如果冻结数据类模式字没开，则置TRUE,否则为FALSE"*/
static INT8U SetParaFuncFlg;/*"写前处理标志，一个标识对应多个叶子时，写前处理只在处理第一个叶子的时候调用"*/

/*"******************************PUBLIC FUNCTION*******************************"*/
/*"************写前、写后、读前处理函数*********************"*/
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
/*"  Description:    通信读数据接口函数，通信模块通过调用该接口函数获取数据标识对应数据"*/
/*"  Calls:          "*/
/*"  Called By:      通信功能模块"*/
/*"  Input:          framedata 645-2007协议帧解析结果                 "*/
/*"                  Start_addr 数据标识对应数据内容的数据偏移地址，即可以从数据块中截取数据返回。"*/
/*"                  MaxLen     返回数据最大长度，超过该长度需置有后续数据标志"*/
/*"  Output:         panswer_data  通信应答数据指针"*/
/*"  "*/
/*"  Return:         UN_COMM_ERR  通信错误信息类型"*/
/*"  Others:         code size  230byte"*/
/*"*****************************************************************************"*/
INT8U PP_CommGetData (ST_FRM_RESULT *framedata, INT16U Start_addr, INT16U MaxLen,ST_ANSWER_DATA * panswer_data)
{
	INT16U dataLen;    /*"数据标识对应数据长度"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"节点对应叶子列表"*/
	INT8U  Leaf_Num;/*"节点对应叶子数"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"块标志叶子位置列表"*/
	INT8U block_num;
	INT8U *pBuffer; /*"输出数据缓存地址指针"*/
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

	if((0 != Leaf_Num) && (Leaf_Num < MAX_LEAF_NUM))/*"节点对应叶子不为空"*/
    {
        dataLen = Get_Node_Data_Comm((TREE_LEAF_LIST *) &Leaf_adrr_list,
                               Leaf_Num, Start_addr, MaxLen, pBuffer, block_num,block_addr_list);

        if(0 == dataLen)/*"读数据长度为0，返回无请求数据错误状态"*/
        {
            if(0 == Error_Infor.asByte)
            {
                Error_Infor.ST_ASBITS.errObis = 1;
            }
        }
    }
	else   /*"未找到对应的叶子，检查是否为抄读负荷曲线"*/
	{ 
		if(((0x0611 == dataId.asWord[1]) && (dataId.asBytes[1] <= 0x09) ) ||
		((0x0e10 == dataId.asWord[1]) && (dataId.asWord[0] >= 0x63) && (dataId.asWord[0] <= 0x66)))
		{
		    Error_Infor.asByte = 0;

                ctrlbyte = framedata->data - 6;    /*"数据指针倒移6位，获取控制码"*/
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
	panswer_data->Data_len.int_len = dataLen;/*"输出数据长度及后续帧信息"*/
	
	return (Error_Infor.asByte);
}


/*"*****************************************************************************"*/
/*"  Function:       PP_CommGetDataExt"*/
/*"  Description:    通信读数据接口函数，通信模块通过调用该接口函数获取数据标识对应数据"*/
/*"  Calls:          "*/
/*"  Called By:      通信功能模块"*/
/*"  Input:          framedata 645-2007协议帧解析结果                 "*/
/*"                  Start_addr 数据标识对应数据内容的数据偏移地址，即可以从数据块中截取数据返回。"*/
/*"                  MaxLen     返回数据最大长度，超过该长度需置有后续数据标志"*/
/*"  Output:         panswer_data  通信应答数据指针"*/
/*"  "*/
/*"  Return:         UN_COMM_ERR  通信错误信息类型"*/
/*"  Others:         code size  230byte"*/
/*"*****************************************************************************"*/
INT8U PP_CommGetDataExt (UN_ID645 dataId,
		            INT16U Start_addr, INT16U MaxLen,
		            ST_ANSWER_DATA * panswer_data)
{
	INT16U dataLen;    /*"数据标识对应数据长度"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"节点对应叶子列表"*/
	INT8U  Leaf_Num;/*"节点对应叶子数"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"块标志叶子位置列表"*/
	INT8U block_num;
	INT8U *pBuffer; /*"输出数据缓存地址指针"*/


	/*"local data initial"*/
	block_num = 0;
	dataLen = 0;

	/*"global data initial"*/
	fileIndex = 0xff;

	pBuffer = panswer_data->Data_addr;
	
	Leaf_Num = ProtocolTreeChk(&dataId, Leaf_adrr_list,
	                           &block_num, block_addr_list);

	if((0 != Leaf_Num) && (Leaf_Num < MAX_LEAF_NUM))/*"节点对应叶子不为空"*/
    {
        dataLen = Get_Node_Data_Comm((TREE_LEAF_LIST *) &Leaf_adrr_list,
                               Leaf_Num, Start_addr, MaxLen, pBuffer, block_num,block_addr_list);

        if(0 == dataLen)/*"读数据长度为0，返回无请求数据错误状态"*/
        {
            if(0 == Error_Infor.asByte)
            {
                Error_Infor.ST_ASBITS.errObis = 1;
            }
        }
    }
	else   /*"不支持抄读负荷曲线"*/
	{
        if(0 == Error_Infor.asByte)
        {
		        Error_Infor.ST_ASBITS.errObis = 1;
        }
	}
	panswer_data->Data_len.int_len = dataLen;/*"输出数据长度及后续帧信息"*/
	
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
	INT8U  Condition;/*"电表运行环境  0 工厂 1 实验室   2现场"*/
    //INT16U offSet;
    INT8U  Ret_8;
    INT16U obis_WR; 
    INT8U format_save;
	
	Communal_Purview =(const struct COMMUNAL_PURVIEW  FAR_POINTER*)&Protocol_Purview[Tree_Leaf->Communal_Purview_Id];
	Communal_Function=(const struct COMMUNAL_FUNCTION FAR_POINTER*)&Protocol_Function[Tree_Leaf->Communal_Function_Id];    
	Communal_Id      =(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];

    LIB_ConstMemCpy((CONST *)&Protocol_Format[Communal_Id->Communal_Trans_Id], (INT8U *)&TranFrmt, sizeof(ST_FMT_ATTR));
    
    //Communal_Trans   =(const ST_FMT_ATTR   FAR_POINTER*)&Protocol_Format[Communal_Id->Communal_Trans_Id];

    /*(Sep 21, 2018,13:30:23)故意改成E_NN_MMDD，以便转换*/
    if((E_ACTIVE_CALENDAR_SEASON_TBL == Tree_Leaf->obis)||(E_PASSIVE_CALENDAR_SEASON_TBL == Tree_Leaf->obis))
    {
        TranFrmt.format = E_NN_MMDD_BCD;
    } 
    else if ((E_ACTIVE_CALENDAR_DAY_TBL <= Tree_Leaf->obis)&&(E_ACTIVE_CALENDAR_SEASON_TBL > Tree_Leaf->obis))
    {
        TranFrmt.format = E_NN_HHMM_BCD;
    }   

    /*"工厂状态"*/
    if(TRUE == ChkStatus(E_FAC))
    {
        Condition = 0;
    }
    else                              /*"现场状态"*/
    {
        Condition = 2;
    }
	
	/*"密码 权限字为0，表示不能写"*/
	if(Communal_Purview->write_mode[Condition] == 0x00)
	{
		Error_Infor.ST_ASBITS.errPassword = 1;
		return 0;
	}

	/*"写之前的函数一般用来检验数据是否超限和记录事件"*/		
	if((Communal_Function->OnWrite_Beford != NULL) && (SetParaFuncFlg == 0))
	{
	 	if(Communal_Function->OnWrite_Beford(pdata) == FALSE) /*"写数据失败"*/
	 	{
	 		return 0;
	 	}
		SetParaFuncFlg = 0xff;
	}

	
	/*"写入的数据的长度"*/	
	s_len = FRMT_sizeofFormat(TranFrmt.format);
	
	ret = TRUE;
	//offSet = Revise + Tree_Leaf->offSet;
	
    /*"写前函数中已经设置完成的参数不需要进行下面的处理，如设置时间和循显、按显参数"*/
	if((E_SYS_TIME != Tree_Leaf->obis) && (E_KEY_DIS_OBJ_LIST != Tree_Leaf->obis) && (E_CYCLE_DIS_OBJ_LIST != Tree_Leaf->obis))
	{
	    INT8U dBuff[42+24];  /*预留24字节*/
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

        /*"存储格式与传输格式不一致，需进行数据转换"*/
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
	        /*"数据存储格式与输入数据格式一致，不需要进行数据转换，直接进行数据存储"*/
	        datalen = s_len;
	    } 


	    if((0 != datalen) && (0xff != datalen))
	    {
            if(E_U_6_BYTE == format_save)  /*通信地址，表号反序*/
            {
                LIB_RvsSelf(psour,6);
            }   

            /*ASCII码要反序*/
            if((format_save >=E_ASCII_2_LENGTH)&&(format_save <=E_ASCII_32_LENGTH))
            {
                LIB_RvsSelf(psour,datalen);
            }  
            
	        s_len = datalen;
            num_all_obis= 1;
            obis_WR = Tree_Leaf->obis+Revise;

            /*(Oct 09, 2018,9:52:21)公共假日的长度要特殊处理下,因为底层有判长度，实际格式转换后的数据是对的*/
            if((obis_WR<=(E_SPECIAL_DAY_TBL+SPEC_DAY_NUM))&&(obis_WR>=(E_SPECIAL_DAY_TBL+1)))
            {
                s_len = SPEC_DAY_SET_SIZE;
                if(EQUAL == LIB_ChkValue(buff, 4, 0))  /*公共假日支持全0设置*/
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
/*"  Description:    通信设置节点对应数据，一个节点下面包含N
个叶子，数据处理的最小单元是叶子"*/
/*"                  节点。"*/
/*"  Calls:          "*/
/*"  Called By:      PP_CommSetData"*/
/*"  Input:          Leaf_adrr_list 节点对应的叶子列表"*/
/*"                  pBuffer  设置输入数据缓存地址指针"*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         无"*/
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
        if(Communal_Trans->format == E_ID_CODE_2_FORMAT)    /*"表号、通信地址参数设置第二个叶子不用处理"*/
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
/*"  Description:    通信写数据接口函数，通信模块通过调用该接口函数写数据标识对应数据"*/
/*"  Calls:          "*/
/*"  Called By:      通信功能模块"*/
/*"  Input:          framedata 645-2007协议帧解析结果                 "*/
/*"  Output:         panswer_data  通信应答数据指针"*/
/*"  "*/
/*"  Return:         UN_COMM_ERR  通信错误信息类型"*/
/*"  Others:         code size  180byte"*/
/*"*****************************************************************************"*/
#define _CHK_SW_ID_MAX    7
const INT16U ChkSwId[_CHK_SW_ID_MAX] =
        {
                0x0106,/*"两套时区切换时间"*/
                0x0107,/*"两套时段切换时间"*/
                0x0108,/*"两套分时费率切换时间"*/
                0x0109,/*"两套阶梯电价切换时间"*/
                0x0101,/*"日期"*/
                0x0102,/*"时间"*/
                0x010C /*"日期+时间"*/
        };

INT8U PP_CommSetData (ST_FRM_RESULT *framedata,
		            ST_ANSWER_DATA *panswer_data)
{
	INT8U *pBuffer; /*"输出数据缓存地址指针"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"节点对应叶子列表"*/
	INT8U  Leaf_Num;/*"节点对应叶子数"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"块标志叶子位置列表"*/
	INT8U block_num;
	//INT8U i;

	block_num = 0;

    //Len_dl645 = framedata->dataLen;
    
#ifdef DEBUG_LOG
    if((NULL != framedata->data) && (0 != framedata->dataLen))/*"输入数据域地址不为空且数据长度不为0"*/
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

	if((0 != Leaf_Num) && (Leaf_Num < MAX_LEAF_NUM))/*"节点对应叶子不为空"*/
    {
        Set_Node_Data_Comm((TREE_LEAF_LIST *) &Leaf_adrr_list,
                            Leaf_Num, pBuffer, framedata->authority);
        if(Error_Infor.asByte == 0)/*"参数设置成功"*/
        {
#if 0
            /*"设置日期/时间/时区时段切换时间，需检查时区时段切换"*/
            if(0x0400 == dataId.asWord[1])
            {
                for(i = 0; i < _CHK_SW_ID_MAX; i++)
                {
                    if(dataId.asWord[0] == ChkSwId[i])
                    {                       
                        Action(E_CHECK_SWITCH_ACTION,NULL,NULL);/*"调用时段切换，为了显示①②"*/
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
/*"  Description:    显示读数据接口函数，显示模块通过调用该接口函数获取数据标识对应数据"*/
/*"  Calls:          "*/
/*"  Called By:      显示功能模块"*/
/*"  Input:          pdata_in   显示数据标识及叶子号                 "*/
/*"                  "*/
/*"  Output:         pdata_out  显示数据内容及提示信息、数据格式"*/
/*"  "*/
/*"  Return:         0  取数不成功    len 取数数据长度，取数成功"*/
/*"  Others:         code size  164byte"*/
/*"*****************************************************************************"*/
INT8U PP_ShowGetData (ST_DISP_OBJ *pdata_in,ST_SHOWDATA_OUTPUT *pdata_out )
{
	INT8U dataLen;    /*"数据标识对应数据长度"*/
	TREE_LEAF_LIST Leaf_adrr_list[MAX_LEAF_NUM];/*"节点对应叶子列表"*/
	INT8U  Leaf_Num;/*"节点对应叶子数"*/
	INT8U block_addr_list[PROTOCOL_MAX_BLOCK_NUM];/*"块标志叶子位置列表"*/
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

	/*"节点对应叶子不为空,同时节点叶子数大于需要显示的叶子号"*/
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
/*"  Description:    协议树节点遍历函数"*/
/*"  Calls:          "*/
/*"  Called By:      通信读、写数据接口，显示读接口"*/
/*"  Input:          pdataId   输入数据标识    "*/
/*"                  "*/
/*"  Output:         Leaf_adrr_list 叶子列表缓存"*/
/*"                  block_num  数据块数    block_list 块分割位置列表"*/
/*"  "*/
/*"  Return:         Leaf_Num 叶子数 "*/
/*"  Others:         code size  164byte"*/
/*"*****************************************************************************"*/
static INT8U  ProtocolTreeChk(UN_ID645 *pdataId,TREE_LEAF_LIST *Leaf_adrr_list,
                              INT8U *block_num, INT8U *block_list)
{
    INT16U  nodePoint;        /*"节点位置"*/
    CHECK_NODE_RET nodeAttr;  /*"数据标识对应节点属性  如果有压缩，则是压缩信息"*/
    INT8U Leaf_Num;

    nodeAttr.byte = 0;
    Leaf_Num = 0;

    Error_Infor.asByte = 0;
    dataId.asLong = pdataId->asLong;
    NodedataId.asLong = pdataId->asLong;

    if(FALSE == LocalProtocolChk())/*"远程表部分协议不支持"*/
    {
        return (0);
    }

    ChkFrzDataZip();

    /*"查找数据标识对应节点位置"*/
    nodePoint = Search_Node(&nodeAttr);

    if(0xffff != nodePoint)/*"数据节点存在"*/
    {
        /*"节点遍历，查找节点对应的叶子列表"*/
        Leaf_Num = Traverse_Node(nodePoint, nodeAttr.byte,
                                 Leaf_adrr_list,block_num, block_list);
    }

    return Leaf_Num;
}
/*"*****************************************************************************"*/
/*"  Function:       Search_Node"*/
/*"  Description:    查找数据标识对应的节点位置"*/
/*"  Calls:          "*/
/*"  Called By:      数据输入输出接口函数"*/
/*"  Input:          dataId 数据标识                 "*/
/*"                  "*/
/*"  Output:         nodeAttr  节点属性"*/
/*"  "*/
/*"  Return:         节点位置"*/
/*"  Others:         code size  84byte"*/
/*"*****************************************************************************"*/
static INT16U Search_Node(CHECK_NODE_RET * nodeAttr)
{
	INT16U nodeAddr,nodeAddrNow;/*"用于嵌套查节点位置，nodeAddrNow 为当前节点地址，nodeAddr为下级满足查找要求的节点地址"*/
	CHECK_NODE_RET  ret;/*"节点属性解析结果"*/
	
	nodeAddrNow = Protocol_Tree_Root[NodedataId.asBytes[3]];/*"找到根节点位置"*/
	if(nodeAddrNow == 0xFFFF)/*"找不到根节点"*/
	{
	    return nodeAddrNow;
	}
	
	ret.byte = SearchNodeNesting(nodeAddrNow, (INT16U *)&nodeAddr, 0);
		
	if(ret.bit.error != 0)/*"没找到对应节点"*/
	{
		Error_Infor.ST_ASBITS.errObis = 1;
		nodeAddr = 0xffff;
	}
	else/*"找到"*/
	{
		nodeAttr->byte = ret.byte;
	}

	return nodeAddr;
}

/*"*****************************************************************************"*/
/*"  Function:       Traverse_Node"*/
/*"  Description:    遍历节点对应的叶子，并将叶子添加到叶子列表中"*/
/*"  Calls:          "*/
/*"  Called By:      数据输入输出接口函数"*/
/*"  Input:          node_addr 节点位置"*/
/*"                  nodeAttr 节点属性                 "*/
/*"                  dataId 645协议数据标识                 "*/
/*"                  "*/
/*"  Output:         Leaf_adrr_list  节点对应的叶子列表"*/
/*"                  dataLen        节点对应数据长度（按传输格式计算）"*/
/*"                  block_num       数据分块数（冻结数据分块，块间隔符为AA）"*/
/*"                  block_list      分块位置列表"*/
/*"  "*/
/*"  Return:         节点对应的叶子数"*/
/*"  Others:         code size  116byte"*/
/*"*****************************************************************************"*/
static INT8U Traverse_Node(INT16U node_addr, INT8U nodeAttr,
		            TREE_LEAF_LIST *Leaf_adrr_list,INT8U *block_num, INT8U *block_list)
{
	INT8U zip_id[2];/*"压缩的数据标识位数据值，最多支持2级压缩"*/
	CHECK_NODE_RET zip;
	INT8U Leaf_num;
	INT8U Leaf_num_return;
	
	Leaf_num = 0;
	Leaf_num_return = 0;
	zip_id[0]=0xff;
	zip_id[1]=0xff;

	/*"对压缩的ID进行处理"*/
	zip.byte = nodeAttr;

	if(zip.bit.zip1==1)
	{
		zip_id[0]=NodedataId.asBytes[zip.bit.num1]; 
		fileIndex = zip_id[0];/*"解压压缩数据位，用于抄读记录文件"*/ 
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
/*"  Description:    查找数据标识对应的节点位置嵌套函数"*/
/*"  Calls:          "*/
/*"  Called By:      数据输入输出接口函数"*/
/*"  Input:          dataId   数据标识  "*/
/*"                  nodeAddrLower 下级节点位置       "*/
/*"                  nodeAttr  上级节点属性        "*/
/*"                  "*/
/*"  Output:         Node_addr 查找到符合查找要求的节点地址指针"*/
/*"  "*/
/*"  Return:         节点位置"*/
/*"  Others:         code size  148byte"*/
/*"*****************************************************************************"*/
static INT8U SearchNodeNesting(INT16U nodeAddrLower, INT16U * nodeAddr, INT8U nodeAttr)
{
	CHECK_NODE_RET ret;   /*"当前节点属性解析结果"*/
	CHECK_NODE_RET retPre;/*"上级节点属性解析结果"*/
	INT16U nodeLower;     /*"下级节点位置"*/
	const struct TREE_NODE  FAR_POINTER *treeNode;/*"当前节点信息"*/
	
	do
	{
		treeNode = (const struct TREE_NODE  FAR_POINTER*)&Protocol_Tree_Node[nodeAddrLower];
		
		ret.byte = ChkNode((const INT8U  FAR_POINTER*)treeNode);
		if(ret.byte == FALSE)/*"节点位置不合法"*/
		{
			return 0xfe;    /*bit error = 1*/
		}
		
		ret.byte = ChkNodeID(treeNode, nodeAttr);
		
		if(ret.bit.error == 1)/*"本级查找错误"*/
		{			
			if(treeNode->Next_Node != 0xffff)/*"非本节点"*/
			{
				nodeAddrLower = treeNode->Next_Node;/*"指向同级下一个节点"*/
			}
			else
			{
				/*"返回上级节点 此条件实际为失败出口，如果不在本节点下返回上级节点也不可能找到"*/
				return 0xff;        /*bit error = 1*/
			}
		}
		else
		{
			if(ret.bit.low == 0)/*"找到数据标识对应的节点或叶子"*/
			{								
				*nodeAddr = nodeAddrLower;
				return ret.byte;
			}
			else/*"在下级节点"*/
			{			
			    retPre.byte = ret.byte;
				nodeLower = treeNode->Low_Node;
				nodeAttr |= ret.byte;
				ret.byte = SearchNodeNesting(nodeLower, nodeAddr,nodeAttr);
				    				
				return ((INT8U)(ret.byte|retPre.byte));
			}
		}
	}while(1);/*"分析是否有可能死循环"*/
			
	//return 0xff;					
}

/*"*****************************************************************************"*/
/*"  Function:       Traverse_Node_Nesting"*/
/*"  Description:    遍历节点对应的叶子，并将叶子添加到叶子列表中"*/
/*"  Calls:          "*/
/*"  Called By:      数据输入输出接口函数"*/
/*"  Input:          node_addr 节点位置"*/
/*"                  Nest_level 嵌套调用层次  "*/
/*"                  zip_id     节点压缩信息               "*/
/*"                  "*/
/*"  Output:         Leaf_adrr_list  节点对应的叶子列表"*/
/*"                  dataLen    节点对应数据长度"*/
/*"  "*/
/*"  Return:         本次节点遍历对应的叶子数"*/
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
	
	
	if(Nest_level != 0)/*"如果本节点不是第一层节点那么需要计算，因为压缩引起的循环次数"*/
	{
		Tree_Node=(const struct TREE_NODE FAR_POINTER*)&Protocol_Tree_Node[node];
		get_leaf_cyc_para(Tree_Node,&zip_leaf_start,&zip_leaf_end);				
	}

    
	do
	{
		Tree_Node=(const struct TREE_NODE FAR_POINTER*)&Protocol_Tree_Node[node];
		block_flag=Tree_Node->Node_mode.bit.zip;
		
		/*"如果此节点指向的是叶子，添加叶子节点到叶子链表"*/
		if(Tree_Node->Node_mode.bit.mode==0)
		{
		    /*OBIS对象地址*/
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
			
			/*"叶子偏移信息解析,节点可以有两级压缩，地址偏移信息只有一级，当节点两级压缩时，第二级压缩对应地址信息偏移"*/
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
		else/*结点*/
		{
			id_n = set_zip_id(zip_id, zip_leaf_start, id_n);
			Traverse_Node_Nesting(Tree_Node->Low_Node, (INT8U)(Nest_level + 1),&Leaf_num_temp,
					                      zip_id,Leaf_adrr_list, &block_num_temp, block_list);
			
			*Leaf_num = Leaf_num_temp;
		}
		
		/*"节点遍历结束判断"*/				

		if(Nest_level == 0)/*"遍历的根节点遍历结束"*/
		{			
		    *block_num = block_num_temp;
			return Leaf_num_temp;
		}
        
		if(zip_leaf_start<zip_leaf_end)/*"有压缩,未遍历完成"*/
		{
			zip_leaf_start++;
			ret=0;	
		}
		else                       /*"完成遍历"*/
		{
			ret=check_nodePoint(Tree_Node->Next_Node);
			node=Tree_Node->Next_Node;	
			id_n=set_zip_id(zip_id,0xff,id_n);
			id_n=0xff;
		}

		/*"抄读数据时需增加块标志分割符"*/
		if(block_flag == 1)
		{
		    *(block_list + block_num_temp) = (INT8U)(Leaf_num_temp - 1);
		    block_num_temp++;
		    *block_num = block_num_temp;
		}

        /*"防止数组溢出"*/
		if((Leaf_num_temp > MAX_LEAF_NUM) || (block_num_temp > PROTOCOL_MAX_BLOCK_NUM))
		{
		    *Leaf_num = 0xff;
		    return 0xff;
		}
	}while(ret == 0);/*"遍历节点，直到判断遍历结束"*/
	
	*block_num = block_num_temp;
	
	return Leaf_num_temp;
}

/*"****************************************************************************
*"*/
/*"  Function:       ChkSlotObis"*/
/*"  Description:    
判断数据标识为时区表/时段表数据标识，时区时段表抄读时需进行特殊操作"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Node_Data_Comm"*/
/*"  Input:          无"*/
/*"  Output          "*/
/*"  Return:         _TRUE 时区表/时段表数据标识  _FALSE 
时区表/时段表数据标识"*/
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
 函 数 名  : Get_Soft_Record_Id
 功能描述  : 读软件备案号
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年9月25日
    作    者   : xyt
    修改内容   : 新生成函数

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

    return 8;  /*8字节*/
}    

/*"*****************************************************************************"*/
/*"  Function:       Get_Node_Data_Comm        "*/
/*"  Description:    通信获取节点对应数据，一个节点下面包含N个叶子，数据处理的最小单元是叶子"*/
/*"                  节点，当整个节点数据长度超过帧格式的最大数据长度，需进行分割处理。"*/
/*"  Calls:          "*/
/*"  Called By:      PP_CommGetData"*/
/*"  Input:          Leaf_adrr_list 节点对应的叶子列表"*/
/*"                  Start_addr 获取节点从Start_addr字节开始的数据内容  "*/
/*"                  MaxLen     一次读取的最大数据长度               "*/
/*"                  "*/
/*"  Output:         pBuffer  获取数据接收缓存"*/
/*"  "*/
/*"  Return:         获取数据长度"*/
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
        
		if(Node_data_len > Start_addr)/*"从通信要求的数据起始位置开始抄读数据，用于后续帧数据处理"*/
		{
		    LIB_MemCpy((INT8U *)&Leaf_adrr_list[i].Revise[0], (INT8U *)&Revise, 2);

			len = Get_Leaf_Data_Comm(Tree_Leaf, pBuffer + dataLen, dataLen, MaxLen,Revise);
			
			/*"取数不成功，或数据格式不匹配时数据长度为0，返回错误（冻结数据模式字关闭除外）"*/
			if((len == 0) && (FALSE == noFrzDataFlg))
			{
			    dataLen =0;
			    break;
			}
            
			dataLen += len;
			if(dataLen > MaxLen)/*"应答数据已超出帧数据最大长度，需采用后续帧方式"*/
			{
				break;
			}
			if(TRUE == blockFlg)
			{
			    /*"添加块分隔符0xaa"*/
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
    

/*"抄读年时区表、日时段表时，根据参数年时区数、日时段数返回数据长度"*/
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
     
	/*"抄读费率电价表时，根据费率数参数返回数据长度"*/
	if(TRUE == ChkTariffPrice())
	{
        GetSingle(E_TARIFF_NUM, &slotNum);
        dataLen = slotNum << 2;/*"数据长度为费率数 * 4"*/
	}
    
	if((i < Leaf_Num) && (dataLen > 0))/*"有后续帧"*/
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
	
	/*"没有显示格式时候使用通信格式"*/
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

	if(len <= MAX_SHOWDATA)/*"显示数据接收缓存区越界检查"*/
    {

        len = FRMT_DataChange(Sbuff, Data_buff, saveFormat, showFormat);

        if((showFormat.format > E_TIME_FORMAT) && (showFormat.format < E_STRING))/*"time format"*/
        {/*"时间格式转BCD码显示"*/
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
/*"  Description:    显示获取相应叶子号数据对应数据及显示信息。"*/
/*"  Calls:          "*/
/*"  Called By:      PP_ShowGetData"*/
/*"  Input:          Leaf_adrr_list 节点对应的叶子列表"*/
/*"                  Leaf_Num       显示标识节点下叶子数"*/
/*"                  Show_leaf      显示的叶子号            "*/
/*"                  "*/
/*"  Output:         pBuffer  输出数据接收缓存"*/
/*"  "*/
/*"  Return:         获取数据长度"*/
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
	
	/*"初始化原始数据缓存区"*/
	LIB_MemSet(0, (INT8U *)&dbuf, MAX_FUN_BUFF_SIZE);

	/*"获取叶子节点数据的原始数据，即格式转换前数据（存储格式数据）"*/
    len_temp = Get_Leaf_Data(Tree_Leaf, (INT8U *) &dbuf, Revise, &Communal_Save);
    len_save = (INT8U)len_temp;

    /*"按显示要求的数据格式进行数据转换"*/
    if(0 != len_save)
    {
        len_Trans = Data_Transfer_Show(&Communal_Save, (INT8U *) &dbuf, pBuffer,
                                       Communal_Show, Communal_Trans, len_save);
    }
    else
    {
        len_Trans = 0;
    }

	/*"输出显示配置信息,显示格式信息在Data_Transfer_Show中已经处理"*/
    LIB_MemCpy((INT8U *)&(Protocol_Showinfo[Tree_Leaf->Communal_Showinfo_Id].schar[0]),
               &(pBuffer->stShowTable.schar[0]), MAX_SCHAR_NUM);

	return len_Trans;		
}
#endif
/*"*****************************************************************************"*/
/*"  Function:       Get_Leaf_Data"*/
/*"  Description:    获取叶子原始数据(组合运行后原始数据)"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Leaf_Data_Comm, Get_Leaf_Data_Show,Get_Leaf_Data_Prepay"*/
/*"  Input:          Tree_Leaf 叶子节点地址指针                    "*/
/*"                  Revise    地址修正值"*/
/*"                  "*/
/*"  Output:         buff  获取数据保存地址指针"*/
/*"  "*/
/*"  Return:         获取数据长度"*/
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
	INT8U Onread_result = 0;/*"-1表示该类数据不允许读用于冻结数据"*/
	                         /*"0x5a表示读正在发生电压类事件累计安时值数据"*/

	INT16U reviseAdrr;
	INT8U i;
	INT8U dataNum;

	INT8U condition;
	INT8U frzOpenFlg;/*"读冻结数据中不支持的数据项时，需先检查模式字是否开启，如果没开，标志置FALSE,否则为TRUE,其他不支持的数据项为TRUE"*/
	EN_RCRD_FILE_ID  rcrdObis;
    INT16S getRst;
    
    ret=0;

    Communal_Purview =(const struct COMMUNAL_PURVIEW FAR_POINTER*)&Protocol_Purview[Tree_Leaf->Communal_Purview_Id];
    Communal_Id=(const struct COMMUNAL_ID FAR_POINTER*)&Protocol_Id[Tree_Leaf->Communal_Id];
    Communal_Function=(const struct COMMUNAL_FUNCTION FAR_POINTER*)&Protocol_Function[Tree_Leaf->Communal_Function_Id];
    Communal_Mathinfo=(const struct COMMUNAL_MATHINFO FAR_POINTER*)&Protocol_MATHINFO[Tree_Leaf->Communal_Math_Mode_Id];
    
    /*"判读读数据的权限"*/
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

    if(((Communal_Purview->read_mode & condition) & 0x07) == 0)/*"在此状态下不允许读"*/
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
    	/*"FALSE表示该类数据不允许读, 用于冻结数据"*/
    	if(Onread_result == FALSE)
    	{
    	    return 0;
    	}
#if defined (THREE_PHASE)
    	else if(Onread_result == 0x5a)/*"0x5a表示读正在发生电压类事件累计安时值数据"*/
    	{
    	}
#endif
    	else if(Onread_result == 0x55)/*"0x55表示读表内没有的数据，需返回FF"*/
    	{
    	    INT8U eventTimesOut;

    	    eventTimesOut = TRUE;
    	    frzOpenFlg = TRUE;
    	    /*"事件记录和冻结数据需判断是否已经发生，已经发生才补FF，没发生补0"*/
    	    if((dataId.asBytes[3] == 0x03) || (dataId.asBytes[3] == 0x05) || (dataId.asBytes[3] > 0x0E))
    	    {
    	        eventTimesOut = RCRD_ChkEvntCnt(rcrdObis, fileIndex);
    	    }
    	    if(dataId.asBytes[3] == 0x05)/*"读冻结数据中不支持数据项时，需检测模式字是否开启"*/
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
    	else if(0xbb == Onread_result)/*"0xbb表示读冻结数据块，无功电量及需量的块结束符需补AA"*/
    	{
    	    LIB_MemSet(0xaa, buff, 8);
    	    return 8;
    	}
    	else if(Onread_result == 0xa5)/*"单相表显示参数抄读"*/
    	{
    	    Communal_Save->format = E_SHOW_CODE_5_BYTE;
    	    return 5;
    	}

    	else if(Onread_result == 0xaa)/*"脉冲常数抄读"*/
    	{
            Communal_Save->format = E_UBCD_3_BYTE;
            return 3;
    	}
        else if(8 == Onread_result)   /*软件备案号*/
        {
            Communal_Save->format = E_ASCII_8_LENGTH; 
            return 8;
        }    
    	else if((Onread_result >= 14) && (Onread_result <= 39))/*"事件上报状态字及事件1~N新增次数特殊处理"*/
    	{
    	    return Onread_result;
    	}
    }
    
    /*"取组合方式字"*/	
    if(Communal_Mathinfo->mathModeObis < 0x100)/*"只有一字节有效为立即数"*/
    {    	
    	math_mode =(INT8U)(Communal_Mathinfo->mathModeObis);
    }
    else                                  /*"两字节有效位地址"*/
    {
        //DC_GetData(Communal_Mathinfo->mathModeObis, (INT8U *)&math_mode, 0, 1);
        GetSingle(Communal_Mathinfo->mathModeObis, (INT8U *)&math_mode);
    }         

    reviseAdrr = Revise + Tree_Leaf->offSet;/*"计算偏移地址=压缩偏移量+对象内偏移量"*/
    DataLen = 0;
    dataNum = 1;
    
    if(Communal_Mathinfo->math_type != 0)/*"组合数据需根据组合数据数多次获取原始数据"*/
    {
        dataNum = Communal_Mathinfo->Ass_data_num;
    }
    else
    {

    } 


    for(i = 0; i < dataNum; i++)
    {
        reviseAdrr += i * Communal_Mathinfo->Ass_data_addr;

        if(Tree_Leaf->obis < 0x7000 || Tree_Leaf->obis > E_STEP_BILL_FRZ_RCRD)/*"获取基础类数据"*/
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
        else                /*"获取记录文件类数据"*/
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


    if((Communal_Mathinfo->math_type != 0) && (0xFF != DataLen))/*"有组合计算"*/
    {
        if((Communal_Save->format > E_S_1_BYTE) && (Communal_Save->format < E_TIME_FORMAT))/*"无符号16进制数，转换为有符号16进制数"*/
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
 函 数 名  : Change_Bandvalue_645
 功能描述  : 转换波特率数据
 输入参数  : INT8U Value  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年9月7日
    作    者   : xyt
    修改内容   : 新生成函数

300bps（0），   600bps（1），     1200bps（2），
2400bps（3），  4800bps（4），    7200bps（5），
9600bps（6），  19200bps（7），   38400bps（8），
57600bps（9）， 115200bps（10）

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
/*"  Description:    通信模式获取叶子数据(按照通信要求输出数据格式进行转换)"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Node_Data_Comm"*/
/*"  Input:          Tree_Leaf 叶子节点地址指针             "*/
/*"                  Revise    地址修正值"*/
/*"                  "*/
/*"  Output:         buff  获取数据保存地址指针"*/
/*"  "*/
/*"  Return:         获取数据长度"*/
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
	
	/*"取数，放在dbuf的开始字节，如果有组合计算机算完"*/
	LIB_MemSet(0, (INT8U *)&dbuf, MAX_FUN_BUFF_SIZE);

	/*"获取叶子节点数据的原始数据，即格式转换前数据（存储格式数据）"*/
	len_temp = Get_Leaf_Data(Tree_Leaf, (INT8U *)&dbuf, Revise, (ST_FMT_ATTR *)&Communal_Save);
	len_save = (INT8U)len_temp;
	
    /*"按通讯要求的数据格式进行数据转换"*/
    if(0 != len_save)
	{
        len_Trans = len_save;/*"一般来说，数据中心存储的数据大小不小于通信传输数据"*/
        if(len_Trans + Offset > MaxLen)/*"加上本叶子的数据长度后 超出了缓冲区"*/
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
/*"  Description:    判断节点是否合法，指向的节点指针是否合法"*/
/*"  Calls:          "*/
/*"  Called By:      SearchNodeNesting"*/
/*"  Input:          node  节点指针                "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE 合法    FALSE  非法"*/
/*"  Others:         code size  52byte"*/
/*"*****************************************************************************"*/
static INT8U ChkNode(const INT8U FAR_POINTER *node)
{
	const INT8U FAR_POINTER*p_temp;
	INT8U rsult;
	
	rsult = TRUE;

	p_temp = (const INT8U FAR_POINTER*)&Protocol_Tree_Node[0];
	
	if((node < p_temp) ||
	   (node - p_temp) > sizeof(Protocol_Tree_Node)) /*"指针处于非法区域"*/
	{
	    rsult = FALSE;
	}
	return rsult;
}
/*"判断节点内的指针如果返回为ff表示节点结束，fe指针超范围，0正常"*/
/*"code size  32byte"*/
static INT8U check_nodePoint(INT16U node)
{
	if(node == 0xffff)
	{
	    return 0xff;
	}
	else if((node * sizeof(struct TREE_NODE)) > sizeof( Protocol_Tree_Node))
	{
	    return 0xfe;/*"指针处于非法区域"*/
	}
	else
	{
	    return 0;
	}
}
/*"*****************************************************************************"*/
/*"  Function:       ChkNodeID"*/
/*"  Description:    判断是否找到节点"*/
/*"  Calls:          "*/
/*"  Called By:      SearchNodeNesting"*/
/*"  Input:          id  数据标识"*/
/*"                  node 节点指针   "*/
/*"                  uzip 节点属性             "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         0 合法    0xff  非法        (Jul 30, 2018,16:27:21)CHECK_NODE_RET 节点信息"*/
/*"  Others:         code size  622byte"*/
/*"*****************************************************************************"*/
#define  _NO_ZIP    0
#define  _ZIP_IN_RANGE  1
#define  _ZIP_OUT_RANGE  2
static INT8U ChkNodeID(const struct TREE_NODE FAR_POINTER *node,INT8U uzip_in)
{
    INT8U zipNum;/*"数据标识压缩字节号"*/
    INT8U i, block;
    CHECK_NODE_RET ret;
    UN_ID645 id_temp;
    INT8U ID_start = 0, ID_end = 0;
    CHECK_NODE_RET uzip;
    INT8U zipFlg;

    if(node->Node_mode.bit.ID_Enable == 0)/*"数据标识无效，返回节点查找错误"*/
    {
        return 0xff;
    }

    uzip.byte = uzip_in;
    zipNum = 0;
    zipNum = node->Node_mode.bit.num;
    /*"数据标识相等,且为非压缩节点"*/
    if((NodedataId.asLong == node->ID) && (zipNum == 0xf))/*"数据标识相等，如果有压缩，还得处理压缩属性"*/
    {
        return 0;
    }

    /*"块标识"*/
    id_temp.asLong = node->ID;
    block = 0xff;
    for(i = 0; i < 4; i++)/*"只支持一个0XFF块数据"*/
    {
        if(id_temp.asBytes[i] == 0xFF)
        {
            block = i;
            break;
        }
    }

    ret.byte = uzip.byte;

    /*"检测节点的压缩属性，压缩属性保存在zipFlg，属性值有三种：_NO_ZIP 无压缩、"*/
    /*"_ZIP_OUT_RANGE 有压缩，且协议标识在压缩范围内、_ZIP_OUT_RANGE 有压缩，但协议标识在压缩范围外"*/
    get_leaf_cyc_para(node, &ID_start, &ID_end);
    zipFlg = _NO_ZIP;
    if(zipNum != 0xf)/*"节点/叶子有压缩"*/
    {
        zipFlg = _ZIP_OUT_RANGE;
        if(TRUE == ChkIdRange(zipNum, ID_start, ID_end))/*"数据标识在压缩范围内"*/
        {
            ret.byte = push_zip_info(uzip.byte, zipNum);
            zipFlg = _ZIP_IN_RANGE;
        }
    }

    /*"检查协议标识与节点标识的一致性，除块抄标识位和压缩标识为之外，其他标识位必须一致，则在本节点或本节点下"*/
    for(i = 0; i < 4; i++)
    {
        if(NodedataId.asBytes[i] != id_temp.asBytes[i])/*"非压缩位数据标识不一致"*/
        {
            if(i != block)/*"数据标识位为非块抄标识位"*/
            {
                /*"数据标识的某一位为一级压缩或二级压缩"*/
                if(((i == ret.bit.num1) && (ret.bit.zip1 == 1))
                        || ((i == ret.bit.num2) && (ret.bit.zip2 == 1)))
                {

                }
                else/*"非压缩位标识"*/
                {
                    return 0xff;
                }
            }
        }
    }

    if(node->Node_mode.bit.mode == 0)/*"如果是叶子"*/
    {
        if(block != 0xff)/*"叶子不可能出现块抄标识"*/
        {
            return 0xff;
        }
        
        if(_NO_ZIP != zipFlg)/*"块压缩叶子"*/
        {
            /*"压缩位数标识在压缩范围内,是本叶子 压缩的叶子"*/
            if(_ZIP_IN_RANGE == zipFlg)
            {
                ret.bit.low = 0;
                return ret.byte;
            }
        }
        else/*"是本叶子 未压缩的叶子"*/
        {
            ret.byte = 0;
            return ret.byte;
        }
    }
    else /*"如果是节点"*/
    {
        if(block != 0xff)/*"块节点"*/
        {
            if(_NO_ZIP != zipFlg)/*"是数据块压缩节点"*/
            {
                /*"压缩位数据标识在压缩范围内"*/
                if(_ZIP_IN_RANGE == zipFlg)
                {
                    if(node->Node_mode.bit.Low_Node_Num == 1)/*"下级节点有数据标识"*/
                    {
                        /*"非块抄命令"*/
                        if(NodedataId.asBytes[block] != id_temp.asBytes[block])
                        {
                            ret.bit.low = 1;
                        }
                        else
                        {
                            ret.bit.low = 0;
                        }
                        return ret.byte;/*"数据标识在范围内，是本节点下"*/
                    }
                }
            }
            else/*"非数据块压缩节点"*/
            {
                ret.byte = 0;
                ret.bit.low = 1;
                return ret.byte;
            }
        }
        else/*"非块抄数据标识"*/
        {
            if(_NO_ZIP != zipFlg)/*"是数据块压缩节点"*/
            {
                /*"压缩位数据标识在压缩范围内"*/
                if(_ZIP_IN_RANGE == zipFlg)
                {
                    if(node->Node_mode.bit.Low_Node_Num == 1)/*"下级节点有数据标识"*/
                    {
                        ret.bit.low = 1;
                    }
                    else
                    {
                        ret.bit.low = 0;
                    }
                    return ret.byte;                         /*"数据标识在范围内，是本节点下"*/
                }
            }
            else/*"非数据块压缩节点"*/
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
/*"  Description:    判断数据标识压缩位是否在压缩范围内"*/
/*"  Calls:          "*/
/*"  Called By:      ChkNodeID"*/
/*"  Input:          ZipNum  数据标识压缩位"*/
/*"                  IdStart 压缩起始   "*/
/*"                  IdEnd 压缩结束            "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         TRUE 在压缩范围内    FALSE  在压缩范围外"*/
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
/*"  Description:    添加节点压缩信息"*/
/*"  Calls:          "*/
/*"  Called By:      ChkNodeID"*/
/*"  Input:          val  添加前压缩信息"*/
/*"                  num  压缩数据标识位 0~3     "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         添加后压缩信息"*/
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
/*"  Description:    level 解析压缩位号(DIn) n = 0~3，最大支持2级压缩。"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          zip_id  压缩位标识修正信息缓存  :0xff表示没有缓存压缩信息，即可以进行压缩 "*/
/*"                  num     压缩数据范围起始值  "*/
/*"                  level              "*/
/*"                  "*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         添加后压缩信息  (Jul 30, 2018,14:33:29)即压缩了多少数据"*/
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
/*"  Description:    获取节点循环信息,"*/
/*"                  如果本节点不是第一层节点那么需要计算，因为压缩引起的循环次数"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Tree_Node  节点信息"*/
/*"                                 "*/
/*"                  "*/
/*"  Output:         start  节点循环起始值 "*/
/*"                  end    节点循环结束值"*/
/*"  "*/
/*"  Return:         添加后压缩信息"*/
/*"  Others:         code size  162byte"*/
/*"*****************************************************************************"*/
static void get_leaf_cyc_para(const struct TREE_NODE FAR_POINTER*Tree_Node,INT8U *start,INT8U *end)
{
	const struct COMMUNAL_RANGE FAR_POINTER*Communal_Range;
	INT8U ID_start = 0, ID_end = 0;
	
	Communal_Range = (const struct COMMUNAL_RANGE  FAR_POINTER*)&Protocol_Range[Tree_Node->Range_ID];

	if(Communal_Range->idStartObis < 0x100)/*"ID_start 只能是立即数，因此这里没有考虑else情况"*/
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
            /*"费率表编程事件，费率数小于4时按4费率返回"*/
            if((dataId.asLong & 0xffffff00) == 0x03300f00)
            {
                if(ID_end < 4)
                {
                    ID_end = 4;
                }
            }
            else
            {
                /*"费率数大于4或非电量块抄(冻结数据电量块抄标识不为0xff需排除)，费率遍历范围为电量存储最大费率数"*/
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

    /*2018-07-20 节点叶子有压缩*/         
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
/*"函数名："*/
/*"用途：地址修正函数"*/
/*"参数："*/
/*"返回："*/
/*"作者：trend 版本：1.00   code size  214byte"*/
/*"============================================================"*/
static INT16U Revise_Addr(INT16U leaf_addr,const struct COMMUNAL_REVISE FAR_POINTER*REVISE ,INT8U ID)
{
	INT16U num;
	INT8U basce;             /*"地址修正起始量"*/
		
	num = 0;
	if((REVISE->reviseMode & 0x01) == 0)/*"偏移参数有效，否则不参与运算"*/
	{
		return leaf_addr;
	}

	basce = REVISE->reviseBase;
			
	if((REVISE->reviseMode & 0x02) != 0)/*"负偏移,目前不支持"*/
	{		
	    //todo   add debug code
	}
	else /*"正偏移"*/
	{		
		if((REVISE->reviseMode & 0x04) == 0)/*"不循环"*/
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
		else    /*"循环,目前不支持"*/
		{
		    //todo   add debug code
		}
	}
	
	leaf_addr += (num * REVISE->reviseSize);
	return leaf_addr;
}

/*"============================================================"*/
/*"函数名：Ram_math"*/
/*"用途：组合数据计算"*/
/*"参数：mode 组合数据计算方法 ：加减法/最大值法/需量发生时间算法"*/
/*"     getbuff  组合数据源"*/
/*"     math_mode  组合方式"*/
/*"     Ass_num  组合数据个数"*/
/*"     dataLen  单个数据长度"*/
/*"返回：数据长度"*/
/*"作者：GANJP 版本：1.00  code size  78byte"*/
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
/*"  Description:    数据组合计算,根据数据组合模式字，计算组合数据值"*/
/*"  Calls:"*/
/*"  Called By:"*/
/*"  Input:          startaddr  参与组合计算数据起始地址指针,"*/
/*"                             如果为EEPROM或FLASH，指针指向的是EEPROM或FLASH地址"*/
/*"                  math_mode  组合计算模式"*/
/*"                  format     数据格式"*/
/*"                  Ass_num    参与组合计算成员数"*/
/*"                  Ass_addr   参与组合计算成员偏移地址"*/
/*"                  filename   0~5000 为flash文件名，"*/
/*"                             RAM_DEMAND_MATH表示RAM中数据"*/
/*"                             EEPROM_DEMAND_MATH表示EEPROM中数据"*/
/*""*/
/*"  Output:         getbuff  组合计算结果"*/
/*""*/
/*"  Return:         数据长度"*/
/*"  Others:         code size  278byte"*/
/*这个版本针对底层为5字节的数据，转换时不是很好处理 (Oct 23, 2018,10:16:39)*/
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

	

	/*"组合"*/
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
    /*ASCII码只反序*/
    if((format_save >=E_ASCII_2_LENGTH)&&(format_save <=E_ASCII_32_LENGTH))
    {
        LIB_Reverse(Sbuff, Dbuff, FRMT_sizeofFormat(format_tran));
        return FRMT_sizeofFormat(format_tran); 
    }    
    
	/*"全0 或 没有数据格式数据不进行数据转换"*/
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

    if(E_U_6_BYTE == format_save)  /*通信地址，表号反序*/
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
/*"  Description:    新645设置日期写处理函数，对下发日期数据进行合法性检查，并进行校时事件记录"*/
/*"  Calls:          SaveEvent_TimeDot，Postfix_WriteDatetime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata 下发日期数据地址指针  "*/
/*"  Input:          sort   0 set data  1   set time  2 set data and time"*/
/*"  Output:         无"*/
/*"  "*/
/*"  Return:         -1 日期设置不成功  0 日期设置成功"*/
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
    
    LIB_BcdToCharNByte(pdata,7);    /*不管怎么样，都把7个数BCD转成HEX*/
    
    switch(sort)/*"周次在写RTC之前会自动计算，这里可以不用管通信下发值"*/
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
/*"  Description:    645设置日期写处理函数"*/
/*"  Calls:          SetSysTime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata 下发日期数据地址指针  "*/

/*"  Output:         无"*/
/*"  "*/
/*"  Return:         -1 时间设置不成功  0 时间设置成功"*/
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
/*"  Description:    645设置时间写处理函数"*/
/*"  Calls:          SetSysTime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata 下发时间数据地址指针  "*/

/*"  Output:         无"*/
/*"  "*/
/*"  Return:         -1 时间设置不成功  0 时间设置成功"*/
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
/*"  Description:    645设置日期、时间写处理函数"*/
/*"  Calls:          SetSysTime"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata 下发日期、时间数据地址指针  "*/

/*"  Output:         无"*/
/*"  "*/
/*"  Return:         -1 时间设置不成功  0 时间设置成功"*/
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
/*"  Description:    645协议中表计没有的数据，以FF填充返回"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
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
/*"  Description:    计量计算谐波各次含有率"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         len       "*/
/*"  Others:         code size  6byte"*/
/*"*****************************************************************************"*/
#define R_ALLH      0x206F  /*"     R       48	    -----           打包读谐波数据"*/
extern void  MU_ReadInstantData(INT16U reg);
INT8U  readHRVoltCurr(INT8U *pdata)
{
    MU_ReadInstantData(R_ALLH); /*"打包读谐波数据"*/
    return TRUE;
}

#if 0
/*"*****************************************************************************"*/
/*"  Function:       readDis"*/
/*"  Description:    单相表读循环/按键显示参数，需进行显示编码与显示代码的转换"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         特殊处理标志字0xa5"*/
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
冻结数据读前处理接口函数，根据冻结模式字判断哪些冻结数据是可读的"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Type      冻结数据类"*/
/*"  Output          "*/
/*"  Return:         _TRUE 冻结数据可读  _FALSE 冻结数据不可读"*/
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
/*南网扩展费控模式切换冻结*/
                {0x050A, (INT8U *)&FrzPara.aptFrzMode.byte},
/*南网扩展阶梯时区切换冻结*/

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
/*" 读冻结数据第1类数据 "*/
INT8U  ReadFrzBit0Data(INT8U *pdata)
{

    return ReadFrzData(0);
}

/*" 读冻结数据第2 类数据 "*/
INT8U  ReadFrzBit1Data(INT8U *pdata)
{

    return ReadFrzData(1);
}

/*" 读冻结数据第8类数据 "*/
INT8U  ReadFrzBit7Data(INT8U *pdata)
{

    return ReadFrzData(7);
}

/*"*****************************************************************************"*/
/*"  Function:       setBaudRate"*/
/*"  Description:    检测参数的合法性"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         TRUE 参数在允许范围内  FALSE 参数在允许范围外"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U setBaudRate(INT8U *pdata)
{
    INT8U rsult;
#if 0
    INT8U commId;/*"通信口号"*/
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
/*"  Description:    设置有功组合模式字参数写前处理接口函数，检测参数的合法性"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         TRUE 参数在允许范围内  FALSE 参数在允许范围外"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U setActEngyMode(INT8U *pdata)
{
    if(*(pdata) > 0x0f)
    {
        return FALSE;
    }

//    DC_SaveRcrdData(OBIS_PRG_AEMODE, EVTLST_PRGAEMODE);/*"有功组合模式字编程事件"*/

    return TRUE;
}
/*"*****************************************************************************"*/
/*"  Function:       setCommAddr"*/
/*"  Description:    设置通信地址参数写前处理接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         TRUE 参数在允许范围内  FALSE 参数在允许范围外"*/
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
/*"  Description:    冻结数据读前处理接口函数，根据冻结模式字判断哪些冻结数据是可读的"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Type      冻结数据类"*/
/*"  Output          "*/
/*"  Return:         TRUE 冻结数据可读  FALSE 冻结数据不可读"*/
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
/*" 读冻结数据第1类数据 "*/
INT8U  ReadFrzBit0Data(INT8U *pdata)
{

    return ReadFrzData(0);
}

/*" 读冻结数据第2 类数据 "*/
INT8U  ReadFrzBit1Data(INT8U *pdata)
{

    return ReadFrzData(1);
}

/*" 读冻结数据第8类数据 "*/
INT8U  ReadFrzBit7Data(INT8U *pdata)
{

    return ReadFrzData(7);
}

/*" Type = 0x01 表示 运算当前结算周期总"*/
/*" Type = 0x02 表示 运算上一结算周期总"*/
void ReadBillEgy(INT8U Type)
{
    INT32U TmpEngy1[2] = {0,0};/*"还未有上1次结算数据时，默认为0"*/
    INT32U TmpEngy2[2] = {0,0};/*"还未有上2次结算数据时，默认为0"*/
    INT32U TmpResult[2];
    INT16U TmpObis;

    /*"取上1次结算 结算电量"*/
    DC_ReadRcrdData(OBIS_ENGY_BILL, 1, POACT_OFFSET , (INT8U *)&TmpEngy2[0], NULL);
    DC_ReadRcrdData(OBIS_ENGY_BILL, 1, NEACT_OFFSET , (INT8U *)&TmpEngy2[1], NULL);

    if(Type == 0x01)
    {
        /*"取当前正\反向有功总电量"*/
        DC_GetData(POACT_ENGY, (INT8U *)&TmpEngy1[0], 0, 4);
        DC_GetData(NEACT_ENGY, (INT8U *)&TmpEngy1[1], 0, 4);

        /*"当前减去上1次"*/
        TmpResult[0] =  TmpEngy1[0] - TmpEngy2[0];
        TmpResult[1] =  TmpEngy1[1] - TmpEngy2[1];
        TmpObis = NOW_PRD_ENGY;
    }
    else
    {
        /*"取上2月结算结算电量"*/
        DC_ReadRcrdData(OBIS_ENGY_BILL, 2, POACT_OFFSET , (INT8U *)&TmpEngy1[0], NULL);
        DC_ReadRcrdData(OBIS_ENGY_BILL, 2, NEACT_OFFSET , (INT8U *)&TmpEngy1[1], NULL);

        /*"上1次减去上2次结算电量"*/
        TmpResult[0] =  TmpEngy2[0]- TmpEngy1[0];
        TmpResult[1] =  TmpEngy2[1] - TmpEngy1[1];
        TmpObis = LST_PRD_ENGY;
    }

    DC_OutputData(TmpObis, (INT8U *)(&TmpResult), 0, 8);

}

/*"对应数据标识000B0000,抄读当前结算周期组合有功总累计用电量"*/
INT8U ReadNowBillEgy(INT8U *pdata)
{
    ReadBillEgy(1);

    return TRUE;
}


/*"对应数据标识000B0001,抄读上一结算周期组合有功总累计用电量"*/
INT8U ReadLastBillEgy(INT8U *pdata)
{
    ReadBillEgy(2);
    return TRUE;
}

/*"返回电池电压"*/
INT8U OutPut_BattVal(INT8U *pdata)
{
    INT16U Tmp;
    Tmp = GP_GetBattery_Val();/*"由于数据中心为单位V 两位小数"*/
    return DC_OutputData(BAT_VOLT, (INT8U *) &Tmp, 0, 2);

}
#endif

/*"本地费控表专有协议列表，LocalProtocolID为单项数据列表，LocalProtocolZipID为压缩数据列表，列表中不包括冻结数据"*/
#define  _LOCAL_PRO_ID_MAX    7
const INT32U LocalProtocolID[_LOCAL_PRO_ID_MAX] =
        {
                0x0280000B,      /*"当前阶梯电价"*/
                0x02800020,      /*"当前电价"*/
                0x02800021,      /*"当前费率电价"*/
                0x04000108,      /*"两套分时费率切换时间"*/
                0x04000109,      /*"两套阶梯切换时间"*/
                0x04000207,      /*"阶梯数"*/
                0x04001502       /*"插卡状态字"*/
        };
#define _LOCAL_PRO_ZIP_ID_MAX    18
const INT32U LocalProtocolZipID[_LOCAL_PRO_ZIP_ID_MAX] =
        {
                0x000d0000,  /*"年结算日期/年度组合有功总累计用电量"*/
                0x00900200,  /*"剩余金额/透支金额"*/
                0x03300f00,  /*"费率表编程"*/
                0x03301000,  /*"阶梯表编程"*/
                0x03301300,  /*"异常插卡"*/
                0x03301400,  /*"非法插卡总次数"*/
                0x03330100,  /*"购电日期"*/
                0x03330200,  /*"购电后总购电次数"*/
                0x03330300,  /*"购电金额"*/
                0x03330400,  /*"购电前剩余金额"*/
                0x03330500,  /*"购电后剩余金额"*/
                0x03330600,  /*"购电后累计购电金额"*/
                0x03340000,  /*"退费记录"*/
                0x04001000,  /*"报警金额1/2/透支金额限值/囤积金额限值/合闸允许金额限值/退费金额"*/
                0x04050100,  /*"当前套费率电价"*/
                0x04050200,  /*"备用套费率电价"*/
                0x04060400,  /*"当前套阶梯"*/
                0x04060500   /*"备用套阶梯"*/
        }; 
/*"*****************************************************************************"*/
/*"  Function:       LocalProtocolChk"*/
/*"  Description:    根据表型判断，是否支持此协议:"*/
/*"                     1、远程表不支持本地表协议"*/
/*"  Calls:          "*/
/*"  Called By:      协议数据读写接口函数"*/
/*"  Input:          无"*/
/*"  Output          "*/
/*"  Return:         TRUE 支持本协议  FALSE 不支持本协议"*/
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
    else/*"远程表及其他表型不支持本地表专有数据"*/
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
            /*"分时费率切换/两套梯度切换/剩余金额日冻结"*/
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
/*"  Description:    对具有相同协议结构的冻结记录压缩处理"*/
/*"  Calls:          "*/
/*"  Called By:      ProtocolTreeChk"*/
/*"  Input:          无"*/
/*"  Output          无"*/
/*"  Return:         0~6  冻结记录有压缩  0xff 冻结记录无压缩"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U ChkFrzDataZip(void)
{
    INT8U i;
    INT8U rsult;

    rsult = 0xff;
    if(0x05 == dataId.asBytes[3])/*"冻结数据"*/
    {
        for(i = 0; i < 7; i++)
        {
            if(dataId.asBytes[2] == frzId2List[i])/*"参与压缩的冻结数据类"*/
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
/*"  Description:    对具有相同协议结构的冻结记录压缩处理,压缩协议上N次结束位解压处理"*/
/*"  Calls:          "*/
/*"  Called By:      get_leaf_cyc_para"*/
/*"  Input:          无"*/
/*"  Output          无"*/
/*"  Return:         无"*/
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
/*"  Description:    对具有相同协议结构的冻结记录压缩处理,压缩协议冻结数据对象解压处理"*/
/*"  Calls:          "*/
/*"  Called By:      ProtocolTreeChk"*/
/*"  Input:          无"*/
/*"  Output          无"*/
/*"  Return:         无"*/
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
/*"  Description:    判断数据标识为费率电价数据块标识，费率电价数据块表抄读时需按实际费率数返回费率电价数据块"*/
/*"  Calls:          "*/
/*"  Called By:      Get_Node_Data_Comm"*/
/*"  Input:          无"*/
/*"  Output          "*/
/*"  Return:         TRUE 费率电价表数据标识  FALSE 费率电价表数据标识"*/
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
/*"  Description:    读冻结数据块时，单相表不支持的无功及需量数据块保留块结束符AA"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         0xBB"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U readFrzBlk(INT8U *pdata)
{
    return 0xBB;
} 


