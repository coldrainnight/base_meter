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
#include "MNT_dataProcess.h"
#include "DataCenterSoftware\Code\monitor\MNT_data.h"
#include "MNT.h"
#include "MNTConfig.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "App\message\FRMT_def.h"

/*"------private function-------"*/
static INT16S exChangeData(INT16U id, INT8U *pbuffer,INT8U R_W);
static INT8U GetBlkData(INT8U num, INT32U Addr, INT8U *pBuff, INT8U MaxNum);

/*"*****************************************************************************"*/
/*"  Function:       MNT_Get"*/
/*"  Description:    ���״̬���ģ�����ݶ��󳭶��ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      messageģ��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         pBuff     ���ݽ��ջ����ַָ��"*/
/*"  Return:         DAR    > 0 ���ݳ���    <=0 ��������"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S MNT_Get (INT16U id, INT8U *pBuff)
{
    INT16S rsult;
#ifdef THREE_PHASE
    INT16U d;
#endif

    if(E_FAC_STATUS == id)
    {
        FacStatusFlg = ChkStatus(E_FAC);
    }
    else if(E_UPDATA == id)
    {
        LIB_MemCpy((INT8U *)&mMntUpData, pBuff, sizeof(MNT_UP_DATA));
        return sizeof(MNT_UP_DATA);
    }
#ifdef THREE_PHASE
   else if(E_TOTAL_LOSTVOLT_TIME == id)
    {
        TotalLostVoltTime=EvntTimeAdd[E_EVT_LOST_VOLT_LA]+EvntTimeAdd[E_EVT_LOST_VOLT_LB]+EvntTimeAdd[E_EVT_LOST_VOLT_LC];
    }
   else if((E_OVER_ACTDMND == id) || (E_OVER_ACTDMNDTIME == id))
    {
        FS_ReadFile(E_FILE_MNT, OVMXPADEMAND_SHIFT, (INT8U *)&overMaxDemandRead, 4);
        overMaxDemandRead=overMaxDemandRead/100;
	 FS_ReadFile(E_FILE_MNT, OVMXPADEMANDTM_SHIFT, (INT8U *)&overMaxDemandTime[0], 5);
    }
   else if((E_OVER_NEGACTDMND == id) || (E_OVER_NEGACTDMNDTIME == id))
    {
        FS_ReadFile(E_FILE_MNT, OVMXNADEMAND_SHIFT, (INT8U *)&overMaxDemandRead, 4);
        overMaxDemandRead=overMaxDemandRead/100;
	 FS_ReadFile(E_FILE_MNT, OVMXNADEMANDTM_SHIFT, (INT8U *)&overMaxDemandTime[1], 5);
    }
   else if((E_OVER_PHS1RDMND == id) || (E_OVER_PHS1RDMNDTIME == id))
    {
       FS_ReadFile(E_FILE_MNT, OVMXR1DEMAND_SHIFT, (INT8U *)&overMaxDemandRead, 4);
       overMaxDemandRead=overMaxDemandRead/100;
	FS_ReadFile(E_FILE_MNT, OVMXR1DEMANDTM_SHIFT, (INT8U *)&overMaxDemandTime[2], 5);
    }
   else if((E_OVER_PHS2RDMND == id) || (E_OVER_PHS2RDMNDTIME == id))
    {
       FS_ReadFile(E_FILE_MNT, OVMXR2DEMAND_SHIFT, (INT8U *)&overMaxDemandRead, 4);
       overMaxDemandRead=overMaxDemandRead/100;
	FS_ReadFile(E_FILE_MNT, OVMXR2DEMANDTM_SHIFT, (INT8U *)&overMaxDemandTime[3], 5);
    }
   else if((E_OVER_PHS3RDMND == id) || (E_OVER_PHS3RDMNDTIME == id))
    {
        FS_ReadFile(E_FILE_MNT, OVMXR3DEMAND_SHIFT, (INT8U *)&overMaxDemandRead, 4);
        overMaxDemandRead=overMaxDemandRead/100;
	 FS_ReadFile(E_FILE_MNT, OVMXR3DEMANDTM_SHIFT, (INT8U *)&overMaxDemandTime[4], 5);
    }
   else if((E_OVER_PHS4RDMND == id) || (E_OVER_PHS4RDMNDTIME == id))
    {
       FS_ReadFile(E_FILE_MNT, OVMXR4DEMAND_SHIFT, (INT8U *)&overMaxDemandRead, 4);
       overMaxDemandRead=overMaxDemandRead/100;
	FS_ReadFile(E_FILE_MNT, OVMXR4DEMANDTM_SHIFT, (INT8U *)&overMaxDemandTime[5], 5);
    }
   else if(E_CURRIMB_RATE == id)
   {
      FS_ReadFile(E_FILE_MNT, MAXIMBCURRRATE_SHIFT, (INT8U *)&d, sizeof(d));
      if((d > maxImBCurrRate)||(maxImBCurrRate > 9999))
      {
         maxImBCurrRate=d;
      }
   }
   else if(E_BADLYCURRIMB_RATE == id)
   {
      FS_ReadFile(E_FILE_MNT, MAXBADLYIMBCURRRATE_SHIFT, (INT8U *)&d, sizeof(d));
      if((d > maxBadlyImBCurrRate)||(maxBadlyImBCurrRate > 9999))
      {
         maxBadlyImBCurrRate=d;
      }
   }
   else if(E_VOLTIMB_RATE == id)
   {
      FS_ReadFile(E_FILE_MNT, MAXIMBVOLTRATE_SHIFT, (INT8U *)&d, sizeof(d));
      if((d > maxImBVoltRate)||(maxImBVoltRate > 9999))
      {
         maxImBVoltRate=d;
      }
   }
#endif
    rsult = exChangeData(id, pBuff, R_MODE);
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       MNT_Set"*/
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�������ָ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"                  pData     ��������ָ��"*/
/*"                  len       �������ݳ���"*/
/*"  Output:         "*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_Set (INT16U id, INT8U *pData, INT16U len)
{
    INT8S rsult;
    INT16U dataId;

    rsult=MS_W_AUTH_ERR;

    dataId = (id & 0x0fff);  
    if((dataId>=0x0E00)&&((dataId-0x0E00)<MNT_SINGLE_LIST_NUM))
    {
       dataId=dataId-0x0E00;
	if(((stMntSingleDataList[dataId].addr-RAM)>=(INT32U)&MntPara.validFlg)
	&&((stMntSingleDataList[dataId].addr-RAM)<=(INT32U)&MntPara.crc))
	{
	   rsult=MS_SUCCESS;
	}
    }

    if(rsult!=MS_SUCCESS)
    {
       return (rsult);
    }
    
    if(TRUE == ChkMntParaValid())
    {
        rsult = exChangeData(id, pData, W_MODE);
        if(MS_SUCCESS == rsult)
        {
            MntPara.crc = CRC16((INT8U *) &MntPara, FIND(ST_MNT_PARA, crc));
            FS_WriteFile(E_FILE_MNT_PARA, 0, (INT8U *)&MntPara, sizeof(ST_MNT_PARA));
        }
    }
    else
    {
        rsult = MS_OTHER_ERR;
    }
    
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       MNT_Action"*/
/*"  Description:    ���״̬���ģ�鷽�����ʽӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      MS"*/
/*"  Input:          id        �����ʶ  "*/
/*"                  pData     ��������ָ��"*/
/*"  Output:         pbuff     ������ݻ���ָ��"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S MNT_Action (INT16U id, void * pdata, void * pbuff)
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
        if(FunId < MNT_ACTION_NUM)
        {
            if(id == stMntActionList[FunId].obis)
            {
                rsult = stMntActionList[FunId].Fuction(pdata, pbuff);
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
/*"  Function:       MNT_GetFrmt"*/
/*"  Description:    ��ȡָ��ID�������ݸ�ʽ"*/
/*"  Calls:          "*/
/*"  Called By:      ms"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         "*/
/*"  Return:         format id"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U MNT_GetFrmt (INT16U id)
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

	     ListId = (dataId >> 6);

            if(ListId < MNT_BLK_LIST_NUM)
            {
                stBlkDataAttr = stMntBlkDataList[ListId];
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
            if(ListId < MNT_SINGLE_LIST_NUM)
            {
                stSingleDataAttr = stMntSingleDataList[ListId];
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

/*"*****************************************************************************"*/
/*"  Function:       exChangeData"*/
/*"  Description:    ���ݽ����������������ݱ�ʶ������/������ƣ������ݽ�����Ӧ�Ľ�������"*/
/*"  Calls:          "*/
/*"  Called By:      LRM_Get,LRM_Set"*/
/*"  Input:          id ���ݱ�ʶ��ͨ�����ݱ�ʶ����������������"*/
/*"                  pbuffer ����/������ݵ�ַָ��                 "*/
/*"                  R_W    ����/�������  1 ������ݵ���������    0 ���������Ļ�ȡ����"*/
/*"  Output:         "*/

/*"  Return:         DAR "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT16S exChangeData(INT16U id, INT8U *pbuffer,INT8U R_W)
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

        ListId = (dataId >> 6);

        if(ListId < MNT_BLK_LIST_NUM)
        {
            stBlkDataAttr = stMntBlkDataList[ListId];

           Num = (dataId & 0x003f);

            if(Num <= stBlkDataAttr.ObjNum)
            {
                if(R_MODE == R_W)
                {
                    if(Num == 0)
                    {
                        for(i = 0; i < stBlkDataAttr.ObjNum; i++)
                        {
                            dataLen += GetBlkData(i, stBlkDataAttr.addr, pbuffer + dataLen, stBlkDataAttr.ObjNum);
                        }
                    }
                    else
                    {
                        dataLen = GetBlkData((Num - 1), stBlkDataAttr.addr, pbuffer, stBlkDataAttr.ObjNum);
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
        if(ListId < MNT_SINGLE_LIST_NUM)
        {
            stSingleDataAttr = stMntSingleDataList[ListId];
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

static INT8U GetBlkData(INT8U num, INT32U Addr, INT8U *pBuff, INT8U MaxNum)
{
    INT8U len;

    LIB_MemCpy((INT8U *)((Addr-RAM) + num * 4), pBuff, 4);
    len = 4;

    return len;
}
