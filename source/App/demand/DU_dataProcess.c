/*"*****************************************************************************"*/
/*"  FileName: MNT_dataProcess.c"*/
/*"  Description:     ���ļ��ṩ���״̬���ģ����������ʽӿ�"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.09.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
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
 �� �� ��  : get_com_r_demand_data
 ��������  : ����й��������㷽��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 04 13
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void get_com_r_demand_data(INT8U triff_no, INT16U ListId,INT8U *pBuff)
{
    INT8U mode_value,i; 
    ST_MD_DEF Demand[4];
    ST_MD_DEF temp;
    INT32U Offset;
    INT8U which = 0;
    INT32S Value_R_COM;   /*����޹�����Ϊ�з�����*/
        
    if(((E_COM1_R_MAX_DEMAND_BLK&0x0fff)>>4) == ListId)
    {
        mode_value = g_DU_Para.DemandComMode1;
    }    
    else
    {
        mode_value = g_DU_Para.DemandComMode2; 
    }    

    /*1.ȡ��4����������*/
    for(i=0;i<4;i++)
    {
       Offset = DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(triff_no)+(i+2)*sizeof(ST_MD_DEF);

       FS_ReadFile(E_FILE_DU_DATA,Offset,(INT8U *)&Demand[i],sizeof(ST_MD_DEF)); 
    }    

    LIB_MemSet(0, (INT8U *)&temp, sizeof(ST_MD_DEF)); 
    
    /*2.�Ƚ�����2����������*/
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

    /* ����޹������������������������־�����ķ�������޹���������Ӳ�����ϵ�������������г�ȡ���ֵ�������������3��4���Ը����ϴ�*/
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
/*"  Description:    ��ȡ�����ݶ�����ָ����Ա����"*/
/*"  Calls:          "*/
/*"  Called By:      exChangeDUData"*/
/*"  Input:          triff_no   ָ�������ݳ�Ա�ţ���Ա�Ŵ�0��ʼ   ���ʺ�"*/
/*"                  ListId  �ֱ��Ӧ MAX_MD_NUM �и���Ա"*/ 
/*"  Output:         pBuff ���ݽ��ջ���ָ��"*/
/*"  Return:         ���ݳ���"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U GetBlkDUData(INT8U triff_no, INT16U ListId,INT8U *pBuff)
{
    ST_MD_DEF Tmp_Data;

    INT32U Offset;    

    /*����й����⴦��*/
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
    
    return (sizeof(INT32U)+sizeof(DEMAND_DATE_TIME));   /*���ﲻ��ֱ����sizeof(ST_MD_DEF),��������9*/

}
/*"*****************************************************************************"*/
/*"  Function:       exChangeDUData"*/
/*"  Description:    ���ݽ����������������ݱ�ʶ������/������ƣ������ݽ�����Ӧ�Ľ�������"*/
/*"  Calls:          "*/
/*"  Called By:      DU_Get,DU_Set"*/
/*"  Input:          id ���ݱ�ʶ��ͨ�����ݱ�ʶ����������������"*/
/*"                  pbuffer ����/������ݵ�ַָ��                 "*/
/*"                  R_W    ����/�������  1 ������ݵ���������    0 ���������Ļ�ȡ����"*/
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
                    if(Num == 0)  /*������*/
                    {     
                        /*��ǰ������ʱ������*/
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
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�����ȡģ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         pBuff     ���ݽ��ջ����ַָ��"*/
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
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�������ָ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"                  pData     ��������ָ��"*/
/*"                  len       �������ݳ���"*/
/*"  Output:         "*/
/*"  Return:        DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S DU_Set (INT16U id, INT8U * pData, INT16U len)
{
    INT8S rsult;
    INT8U clr_value = DU_CUR_AVE_DM;
    
    rsult = exChangeDUData(id, pData,W_MODE);

    /*���²���*/
    if(MS_SUCCESS == rsult)
    {   
        /*����������������*/
        g_DU_Para.NumOfPeriod = g_DU_Para.DemandTime/g_DU_Para.DemandPeriod;

        /*���º�Ҫ�嵱ǰ����*/
        DU_Clr(&clr_value,NULL);
        
        g_DU_Para.Crc = CRC16((INT8U *) &g_DU_Para, FIND(ST_DU_PARA, Crc));
        FS_WriteFile(E_FILE_DU_PARA, DU_PARA_NUM_PERIOD_SHIFT, (INT8U *) &g_DU_Para,sizeof(ST_DU_PARA));
    }    
    
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       DU_Action"*/
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�������ָ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"                    pdata     ��������ָ��"*/
/*"  Output:        pbuff     ������ݻ���ָ��"*/
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
/*"  Description:    ��ȡָ��ID�������ݸ�ʽ"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
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




