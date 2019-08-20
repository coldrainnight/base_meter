/*"*****************************************************************************"*/
/*"  FileName: MNT_dataProcess.c"*/
/*"  Description:     该文件提供电表状态监测模块对外对象访问接口"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.09.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      ganjp    2017/9/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "..\..\System\Sys_def.h"
#include "APP\message\MS.h"
#include "DU.h"
#include "DU_dataProcess.h"
#include "DataCenterSoftware\Code\demand\DU_data.h" 
/*"*****************************************************************************"*/
/*****************************************************************************
 函 数 名  : get_com_r_demand_data
 功能描述  : 组合有功需量计算方法
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2018 04 13
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void get_com_r_demand_data(INT8U triff_no, INT16U ListId,INT8U *pBuff)
{
    INT8U mode_value,i; 
    ST_MD_DEF Demand[4];
    ST_MD_DEF temp;
    INT32U Offset;
    INT8U which = 0;
    INT32S Value_R_COM;   /*组合无功需量为有符号数*/
        
    if(((E_COM1_R_MAX_DEMAND_BLK&0x0fff)>>4) == ListId)
    {
        mode_value = g_DU_Para.DemandComMode1;
    }    
    else
    {
        mode_value = g_DU_Para.DemandComMode2; 
    }    

    /*1.取出4个象限数据*/
    for(i=0;i<4;i++)
    {
       Offset = DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(triff_no)+(i+2)*sizeof(ST_MD_DEF);

       FS_ReadFile(E_FILE_DU_DATA,Offset,(INT8U *)&Demand[i],sizeof(ST_MD_DEF)); 
    }    

    LIB_MemSet(0, (INT8U *)&temp, sizeof(ST_MD_DEF)); 
    
    /*2.比较其中2个象限数据*/
    for(i=0;i<4;i++)
	{
		if((mode_value&0x03)!=0)
		{
			if(Demand[i].Value>temp.Value)
            {
                which = i;                
				LIB_MemCpy((INT8U*)&Demand[i],(INT8U*)&temp,sizeof(ST_MD_DEF));
            }         	
		}
		mode_value>>=2;
	}

    /* 组合无功最大需量的正负数是用来标志潮流的方向，组合无功最大需量从参与组合电能运算的象限中抽取最大值，如果来自象限3、4，以负数上传*/
    if (which == 2 || which == 3)
    {
        Value_R_COM = 0 - temp.Value;
    }
    else
    {
        Value_R_COM = temp.Value;
    }  

    LIB_MemCpy((INT8U *)&Value_R_COM,pBuff,sizeof(INT32S));

    LIB_MemCpy((INT8U *)&temp.Time,pBuff+4,sizeof(DEMAND_DATE_TIME));    

}
/*"*****************************************************************************"*/
/*"  Function:       GetBlkDUData"*/
/*"  Description:    获取块数据对象中指定成员数据"*/
/*"  Calls:          "*/
/*"  Called By:      exChangeDUData"*/
/*"  Input:          triff_no   指定块数据成员号，成员号从0开始   费率号"*/
/*"                  ListId  分别对应 MAX_MD_NUM 中各成员"*/ 
/*"  Output:         pBuff 数据接收缓存指针"*/
/*"  Return:         数据长度"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U GetBlkDUData(INT8U triff_no, INT16U ListId,INT8U *pBuff)
{
    ST_MD_DEF Tmp_Data;

    INT32U Offset;    

    /*组合有功特殊处理*/
    if((((E_COM1_R_MAX_DEMAND_BLK&0x0fff)>>4) ==ListId)||(((E_COM2_R_MAX_DEMAND_BLK&0x0fff)>>4) ==ListId))
    {
         get_com_r_demand_data(triff_no,ListId,(INT8U *)&Tmp_Data);
    } 
    else
    {    
        if(((E_I_R_MAX_DEMAND_BLK&0x0fff)>>4) <=ListId)
        {
            ListId = ListId - 2;
        }    

        Offset = DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(triff_no)+ListId*sizeof(ST_MD_DEF);

        FS_ReadFile(E_FILE_DU_DATA,Offset,(INT8U *)&Tmp_Data,sizeof(ST_MD_DEF)); 
    }


    LIB_MemCpy((INT8U *)&Tmp_Data, pBuff, sizeof(ST_MD_DEF));
    
    //DU_Change_Date_Time_Com((INT8U *)&Tmp_Data+sizeof(INT32U),pBuff+sizeof(INT32U));    
    
    return (sizeof(INT32U)+sizeof(DEMAND_DATE_TIME));   /*这里不能直接用sizeof(ST_MD_DEF),否则会大于9*/

}
/*"*****************************************************************************"*/
/*"  Function:       exChangeDUData"*/
/*"  Description:    数据交换函数，根据数据标识和输入/输出控制，对数据进行相应的交换处理"*/
/*"  Calls:          "*/
/*"  Called By:      DU_Get,DU_Set"*/
/*"  Input:          id 数据标识，通过数据标识管理数据中心数据"*/
/*"                  pbuffer 输入/输出数据地址指针                 "*/
/*"                  R_W    输入/输出控制  1 输出数据到数据中心    0 从数据中心获取数据"*/
/*"  Output:         "*/

/*"  Return:         DAR "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT16S exChangeDUData(INT16U id, INT8U *pbuffer,INT8U R_W)
{
    INT16U dataId;
    INT16S rsult;
    INT8U Num;
    INT8U i;
    INT16U dataLen;
    INT16U ListId;

    dataId = (id & 0x0fff);
    dataLen = 0;
    rsult = MS_SUCCESS;
    if(dataId < 0x0e00)
    {
        ST_BLK_DATA_ATTR stBlkDataAttr;

        ListId = (dataId >> 4);
        if(ListId < ARRAY_SIZE(stDuBlkDataList))
        {
            stBlkDataAttr = stDuBlkDataList[ListId];
            Num = (dataId & 0x000f);
            if(Num <= stBlkDataAttr.ObjNum)
            {
                if(R_MODE == R_W)
                {
                    if(Num == 0)  /*块数据*/
                    {     
                        /*当前需量临时块数据*/
                        if(E_DEMAND_AV_POWER_BLK == id)
                        {
                             LIB_MemCpy((INT8U *)&g_DU_Ram.Data[0].power_buf[0], pbuffer, sizeof(TYPE_DEMAND_CAL)*15);
                        }  
                        else
                        {    
                            for(i = 0; i < stBlkDataAttr.ObjNum; i++)
                            {
                                dataLen += GetBlkDUData(i, ListId, pbuffer + dataLen);
                            }
                        }
                    }
                    else
                    {
                        dataLen = GetBlkDUData((Num - 1), ListId, pbuffer);
                    }
                }
                else
                {
                    rsult = MS_W_AUTH_ERR;
                }
            }
            else
            {
                rsult = MS_OBIS_ERR;
            }
        }
        else
        {
            rsult = MS_OBIS_ERR;
        }
    }
    else
    {
        ST_SINGLE_DATA_ATTR stSingleDataAttr;
        
        ListId = (dataId -0x0E00);
        if(ListId < ARRAY_SIZE(stDuSingleDataList))
        {
            stSingleDataAttr = stDuSingleDataList[ListId];
            dataLen = FRMT_sizeofFormat(stFormatList[stSingleDataAttr.format_id].format);
            if(R_MODE == R_W)
            {
                LIB_MemCpy((INT8U *) (stSingleDataAttr.addr - RAM), pbuffer, (INT8U) dataLen);
            }
            else
            {
                LIB_MemCpy(pbuffer, (INT8U *) (stSingleDataAttr.addr - RAM), (INT8U) dataLen);
            }
        }
        else
        {
            rsult = MS_OBIS_ERR;
        }
    }
    
    if((rsult >= 0) && (R_MODE == R_W))
    {
        rsult = dataLen;
    }
    
    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       DU_Get"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数获取模块所需数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"  Output:         pBuff     数据接收缓存地址指针"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S DU_Get (INT16U id, INT8U *pBuff)
{
    INT16S rsult;

    if((E_CURRENT_PA_DEMAND_TEMP<=id)&&(E_CURRENT_IV_R_DEMAND_TEMP>=id))
    {
        DU_CurAveValue(DU_TRUE);
    }    

    rsult = (INT16S)exChangeDUData(id, pBuff,R_MODE);
    
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       DU_Set"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数设置指定对象数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"                  pData     输入数据指针"*/
/*"                  len       设置数据长度"*/
/*"  Output:         "*/
/*"  Return:        DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S DU_Set (INT16U id, INT8U * pData, INT16U len)
{
    INT8S rsult;
    INT8U clr_value = DU_CUR_AVE_DM;
    
    rsult = exChangeDUData(id, pData,W_MODE);

    /*存下参数*/
    if(MS_SUCCESS == rsult)
    {   
        /*重新算需量周期数*/
        g_DU_Para.NumOfPeriod = g_DU_Para.DemandTime/g_DU_Para.DemandPeriod;

        /*更新后，要清当前需量*/
        DU_Clr(&clr_value,NULL);
        
        g_DU_Para.Crc = CRC16((INT8U *) &g_DU_Para, FIND(ST_DU_PARA, Crc));
        FS_WriteFile(E_FILE_DU_PARA, DU_PARA_NUM_PERIOD_SHIFT, (INT8U *) &g_DU_Para,sizeof(ST_DU_PARA));
    }    
    
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       DU_Action"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数设置指定对象数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"                    pdata     输入数据指针"*/
/*"  Output:        pbuff     输出数据缓存指针"*/
/*"  Return:        DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S DU_Action(INT16U id, void *pdata, void *pbuff)
{
    INT16U FunId;
    INT8S rsult;

    rsult = MS_SUCCESS;
    FunId = (id & 0x0fff);
    if(FunId < 0x0fe0)
    {
        rsult = MS_ACTION_AUTH_ERR;
    }
    else
    {
        FunId -= 0x0fe0;
        if(FunId < ARRAY_SIZE(stDuActionList))
        {
            if(id == stDuActionList[FunId].obis)
            {
                rsult = stDuActionList[FunId].Fuction(pdata, pbuff);
            }
            else
            {
                //todo debug
                rsult = MS_ACTION_AUTH_ERR;
            }
        }
        else
        {
            rsult = MS_OBIS_ERR;
        }
    }
    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       UI_GetFrmt"*/
/*"  Description:    获取指定ID对象数据格式"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"  Output:         "*/
/*"  Return:         format id"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U DU_GetFrmt (INT16U id)
{
    INT8U frmtId;
    INT16U dataId;
    INT16U ListId;

    dataId = (id & 0x0fff);

    if(dataId >= 0x0fe0)
    {
        frmtId = 0xff;
    }
    else
    {
        if(dataId < 0x0e00)
        {
            ST_BLK_DATA_ATTR stBlkDataAttr;

            ListId = (dataId >> 4);
            if(ListId < ARRAY_SIZE(stDuBlkDataList))
            {
                stBlkDataAttr = stDuBlkDataList[ListId];
                frmtId = stBlkDataAttr.format_id;
            }
            else
            {
                frmtId = 0xff;
            }
        }
        else
        {
            ST_SINGLE_DATA_ATTR stSingleDataAttr;

            ListId = (dataId -0x0E00);

            if(ListId < ARRAY_SIZE(stDuSingleDataList))
            {
                stSingleDataAttr = stDuSingleDataList[ListId];
                frmtId = stSingleDataAttr.format_id;
            }
            else
            {
                frmtId = 0xff;
            }
        }
    }

    return (frmtId);
}




