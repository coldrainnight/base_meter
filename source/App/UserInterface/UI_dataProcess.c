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
#include "DataCenterSoftware\Code\UserInterface\UI_data.h"
#include "UI.h"
#include "UI_dataProcess.h"


typedef struct
{
    INT16U obis;
    INT8U min;
    INT8U max;
}ST_BYTE_RANGE_LIST;

typedef struct
{
    INT16U obis;
    INT16U min;
    INT16U max;
}ST_WORD_RANGE_LIST;


typedef INT16S  (* On_Get_Function)(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pData);
typedef INT16S  (* On_Set_Function)(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U setLen, INT8U *pData);

typedef struct
{
    INT16U obis;
    On_Get_Function getFunc;
    On_Set_Function setFunc;
}ST_FUNC_LIST;



static INT16S exChangeUiData(INT16U id, INT8U *pbuffer, INT8U len, INT8U R_W);
static INT16S GetUiBlkData(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pData);
static INT8S SetUiBlkData(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U setLen, INT8U *pData);
static INT8S ChkUiByteRange(INT16U obis, INT8U *pData);
static INT8S ChkUiWordRange(INT16U obis, INT8U *pData);
static INT8S ChkUiINT32Range(INT16U obis, INT8U *pData);

static INT16S UI_GetSafeMode(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pBuff);
static INT16S UI_SetSafeMode(ST_BLK_DATA_ATTR* BlkAttr, INT8U index,INT8U setLen, INT8U *pBuff);

const ST_BYTE_RANGE_LIST mstUiByteRangeList[] =
{
    {E_PWRUP_DIS_ALL_TIME, 1, 30},
    {E_PWR_SHOW_POINT, 0, 4},
    {E_ENGY_SHOW_POINT, 0, 4},
    {E_DIS_SEG_MODE_CTRL_12, 0, 1},
    {E_CYCLE_SHOW_NUM, 1, CYC_DISP_MAX},
    {E_KEY_SHOW_NUM, 1, KEY_DISP_MAX},
    {E_RS485_1_BAUDRATE,E_BAUD1200,E_BAUD19200},
    #ifdef THREE_PHASE
    {E_RS485_2_BAUDRATE,E_BAUD1200,E_BAUD19200},
    #endif
    {E_RS485_1_BAUDRATE,E_BAUD9600,E_BAUD9600},

};

const ST_WORD_RANGE_LIST mstUiWordRangeList[] =
{
    {E_BACKLIGHT_SHOW_TIME, 1, 60},
    {E_BACKLIGHT_SHOW_TIME_ON_VIEW, 1, 60},
    {E_PD_NOKEY_RETURN_TIME, 1, 60},
    {E_NOKEY_RETURN_TIME, 1, 60},
    {E_CYCLE_SHOW_TIME, 1, 20},

};

const ST_FUNC_LIST mstUiFuncList[] = 
{
    {E_CYCLE_DIS_OBJ_LIST, DM_GetDisPara, DM_SetDisPara},
    {E_KEY_DIS_OBJ_LIST, DM_GetDisPara, DM_SetDisPara},
    {E_SAFE_MODE_LIST, UI_GetSafeMode, UI_SetSafeMode},

};

const INT16U mLcdParaObis[] =
{
    E_PWRUP_DIS_ALL_TIME,
    E_BACKLIGHT_SHOW_TIME,
    E_BACKLIGHT_SHOW_TIME_ON_VIEW,
    E_NOKEY_RETURN_TIME,
    E_ENGY_SHOW_POINT,
    E_PWR_SHOW_POINT,
    E_DIS_SEG_MODE_CTRL_12,
};

/*"*****************************************************************************"*/
/*"  Function:       UI_Get"*/
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�����ȡģ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         pBuff     ���ݽ��ջ����ַָ��"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S UI_Get (INT16U id, INT8U *pBuff)
{
    INT16S rsult;

    rsult = exChangeUiData(id, pBuff, 0, R_MODE);
    
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       UI_Set"*/
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
INT8S UI_Set (INT16U id, INT8U * pData, INT16U len)
{
    INT8S rsult;

    rsult = (INT8S)exChangeUiData(id, pData, (INT8U)len, W_MODE);

    if(MS_SUCCESS == rsult)
    {    

        #ifdef THREE_PHASE  
        /*2.���Ǳ�ͨ���Ĳ����ʣ�ֱ�ӳ�ʼ����ͨ����·*/
        if((E_RS485_2_BAUDRATE ==id)&&(E_COM_RS485_2!=NowComId))
        {
            DL698_DLInit(E_COM_RS485_2);
        }  
        /*�ź�ǿ����ʾ����ʱ*/
        if(E_SIGNAL_STRENGTH ==id)
        {
            GPRS_Status_Time = MAX_GPRS_STATUS_DISP_TIME;
            GPRS_Status= Change_GPRS_Value_Status(SignalStrength);
        } 
        #endif

        if((E_RS485_1_BAUDRATE ==id)&&(E_COM_RS485_1!=NowComId))
        {
            DL698_DLInit(E_COM_RS485_1);
        } 

        
        
    }    
    
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       MNT_Action"*/
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�������ָ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"                    pdata     ��������ָ��"*/
/*"  Output:        pbuff     ������ݻ���ָ��"*/
/*"  Return:        DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S UI_Action(INT16U id, void *pdata, void *pbuff)
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
        if(FunId < UI_ACTION_NUM)
        {
            if(id == stUiActionList[FunId].obis)
            {
                rsult = stUiActionList[FunId].Fuction(pdata, pbuff);
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
INT8U UI_GetFrmt (INT16U id)
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

            ListId = (dataId >> DM_BLK_LEFT_SHIFT);
            if(ListId < UI_BLK_LIST_NUM)
            {
                stBlkDataAttr = stUiBlkDataList[ListId];
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

            if(ListId < UI_SINGLE_LIST_NUM)
            {
                stSingleDataAttr = stUiSingleDataList[ListId];
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
/*"  Function:       exChangeUiData"*/
/*"  Description:    ���ݽ����������������ݱ�ʶ������/������ƣ������ݽ�����Ӧ�Ľ�������"*/
/*"  Calls:          "*/
/*"  Called By:      UI_Get,UI_Set"*/
/*"  Input:          id ���ݱ�ʶ��ͨ�����ݱ�ʶ����������������"*/
/*"                  pbuffer ����/������ݵ�ַָ��                 "*/
/*"                  len ����/������ݳ���             "*/
/*"                  R_W    ����/�������  1 ������ݵ���������    0 ���������Ļ�ȡ����"*/
/*"  Output:         "*/
/*"  Return:         DAR "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT16S exChangeUiData(INT16U id, INT8U *pbuffer, INT8U len, INT8U R_W)
{
    INT16U dataId;
    INT16S rsult;
    INT8U Num;
    ENUM_FS_ID FileId; 
    INT16U Offset;
    INT8U InRamFlag;
    INT32U RamAddr;
    INT16U ListId;

    
    dataId = (id & 0x0fff);
    rsult = MS_SUCCESS;

    if((id == E_RUN_STA_1) || 
        (id == E_RUN_STA_2)||
        (id == E_RUN_STA_3)||
        (id == E_RUN_STA_4) || 
        (id == E_RUN_STA_5)||
        (id == E_RUN_STA_6)||
        (id == E_RUN_STA_7)
    )/*������״̬��ʱ����ˢ������״̬��*/
    {
        UI_GetRunStatues(NULL);
    }
    
    if(dataId < 0x0e00)
    {
        ST_BLK_DATA_ATTR stBlkDataAttr;

        ListId = dataId >> DM_BLK_LEFT_SHIFT;/*"��ʾ�����б�������ƫ��Ϊ128"*/
        if(ListId < UI_BLK_LIST_NUM)
        {
            stBlkDataAttr = stUiBlkDataList[ListId];
            Num = dataId & 0x007F;
            if(Num <= stBlkDataAttr.ObjNum)
            {
                if(R_MODE == R_W)
                {
                    rsult = GetUiBlkData(&stBlkDataAttr, Num, pbuffer);
                }
                else/*"W_MODE"*/
                {
                    rsult = SetUiBlkData(&stBlkDataAttr, Num, len, pbuffer);
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
        INT16U len;

        ListId = (dataId -0x0E00);
        if(ListId < UI_SINGLE_LIST_NUM)
        {
            stSingleDataAttr = stUiSingleDataList[ListId];
            len = FRMT_sizeofFormat(stFormatList[stSingleDataAttr.format_id].format);
            MSADDR_IN_RAM(stSingleDataAttr.addr,InRamFlag);
            if(InRamFlag== TRUE)
            {
                MSADDR_TO_RAMADDR(stSingleDataAttr.addr,RamAddr);
            }
            else
            {
                MSADDR_TO_FILEINFO(stSingleDataAttr.addr,FileId,Offset);
            }
        
            if(R_MODE == R_W)
            {
                if( E_RELAY_UNIT_POWER == id)
                {
                    len = UI_GetRelayOutput(NULL,pbuffer);
                }
                else
                {
                    if(InRamFlag== TRUE)
                    {
                        LIB_MemCpy((INT8U *)(RamAddr), pbuffer, len);
                    }
                    else
                    {
                        FS_ReadFile(FileId,Offset,pbuffer,len);
                    }
                }
                rsult = (INT16S)len;
            }
            else
            {
                if(1 == stSingleDataAttr.len)
                {
                    rsult = ChkUiByteRange(stSingleDataAttr.obis, pbuffer);
                }
                else if(2 == stSingleDataAttr.len)
                {
                    rsult = ChkUiWordRange(stSingleDataAttr.obis, pbuffer);
                }
                else if(4 == stSingleDataAttr.len)
                {
                    rsult = ChkUiINT32Range(stSingleDataAttr.obis, pbuffer);
                }
                else
                {
                }

                if(MS_SUCCESS == rsult)
                {
                    if(TRUE == InRamFlag)/*�ڴ�������*/
                    {
						if(stSingleDataAttr.obis < E_NO_BACKUP_BEGIN)/*��Ҫ�����ļ�ϵͳ�ļ�*/
                        {
                            if((RamAddr >= (INT32U)&StUIParaWithRamBackup) && 
                                ((RamAddr+len) <= ((INT32U)&StUIParaWithRamBackup + sizeof(StUIParaWithRamBackup))))
                            {
                                LIB_MemCpy(pbuffer, (INT8U *)RamAddr, len);
                                StUIParaWithRamBackup.CrcValue = CRC16((INT8U *)&StUIParaWithRamBackup, FIND(ST_UI_PARA_WITH_BACKUP, CrcValue));
                                /*���ݵ��ļ�*/
                                FS_WriteFile(E_FILE_UI_PARA_WITH_BACKUP, 0, (INT8U *)&StUIParaWithRamBackup, sizeof(ST_UI_PARA_WITH_BACKUP));
                            }
                            else
                            {
                                rsult = MS_OBIS_ERR;
                            }
                        }
                        else /*ֻ�����ڴ�������*/
                        {
                            LIB_MemCpy(pbuffer, (INT8U *)RamAddr, len);
                        }
                    }
                    else
                    {
                        if(stSingleDataAttr.obis < E_NO_BACKUP_BEGIN)
                        {
                            return FALSE;
                        }
                        FS_WriteFile(FileId,Offset,pbuffer,len);
                    }
                }
                else
                {
                }
            }
        }
        else
        {
            rsult = MS_OBIS_ERR;
        }
    }

    return rsult;
}


/*"************************************************"*/
/*"Function:    UI_GetSafeMode"*/
/*"Description: ��ȫģʽ�ֶ�ȡ"*/
/*"Input:   obisId    ��ʾ�����ʶ"*/
/*"            index     �����Ԫ������"*/
/*"            *pBuff   �������ݵ�ַ"*/
/*"Output: ��"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
INT16S UI_GetSafeMode(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pBuff)
{

    INT16S len;
    INT32U RamAddr;
    
    len = 0;
    MSADDR_TO_RAMADDR (BlkAttr->addr,RamAddr);

    if(index<= BlkAttr->ObjNum)
    {
        if(index == 0)/*��������*/
        {
            len = BlkAttr->len*BlkAttr->ObjNum;
            LIB_MemCpy((INT8U *)RamAddr, pBuff, len);
        }
        else/*��������*/
        {
            len = BlkAttr->len;
            LIB_MemCpy((INT8U * )RamAddr+(index-1)*BlkAttr->len, pBuff, len);
        }
        return len;
    }
    else
    {
        return MS_OBIS_ERR;
    }
}




/*"************************************************"*/
/*"Function:    UI_GetSafeMode"*/
/*"Description: ��ȫģʽ�ֶ�ȡ"*/
/*"Input:   obisId    ��ʾ�����ʶ"*/
/*"            index     �����Ԫ������"*/
/*"            *pBuff   �������ݵ�ַ"*/
/*"Output: ��"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
INT16S UI_SetSafeMode(ST_BLK_DATA_ATTR* BlkAttr, INT8U index,INT8U setLen, INT8U *pBuff)
{

    INT16S len;
    INT32U RamAddr;
    
    len = 0;
    MSADDR_TO_RAMADDR (BlkAttr->addr,RamAddr);

    if(index<= BlkAttr->ObjNum)
    {
        if(index == 0)/*��������*/
        {
            len = BlkAttr->len*BlkAttr->ObjNum;
            if(len == setLen)
            {
                LIB_MemCpy(pBuff,(INT8U *)RamAddr, len);
            }
            else
            {
                return MS_LEN_ERR;
            }
        }
        else/*��������*/
        {
            len = BlkAttr->len;
            if(len == setLen)
            {
                LIB_MemCpy(pBuff,(INT8U * )RamAddr+(index-1)*BlkAttr->len,len);
            }
            else
            {
                return MS_LEN_ERR;
            }
        }
        /*����CRC*/
        StUIParaWithRamBackup.CrcValue = CRC16((INT8U *)&StUIParaWithRamBackup, FIND(ST_UI_PARA_WITH_BACKUP, CrcValue));
        /*���ݵ��ļ�*/
        FS_WriteFile(E_FILE_UI_PARA_WITH_BACKUP, 0, (INT8U *)&StUIParaWithRamBackup, sizeof(ST_UI_PARA_WITH_BACKUP));
        
        return len;
    }
    else
    {
        return MS_OBIS_ERR;
    }
}
/*"*****************************************************************************"*/
/*"  Function:       GetUiBlkData"*/
/*"  Description:   ��������"*/
/*"  Input:          obis: �����ʶ"*/
/*"                     index: ����Ԫ������������0��ʾ������"*/
/*"                     pData: �������ָ��"*/
/*"  Output         �� "*/
/*"  Return:        DAR"*/
/*"  Others:        code size  byte"*/
/*"*****************************************************************************"*/
static INT16S GetUiBlkData(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pData)
{
    INT8U i;
    INT16S result;

    result = MS_OBIS_ERR;

    for(i = 0; i < ARRAY_SIZE(mstUiFuncList); i++)
    {
        if(BlkAttr->obis == mstUiFuncList[i].obis)
        {
            result = mstUiFuncList[i].getFunc(BlkAttr, index, pData);
            break;
        }
    }

    return result;
}

/*"*****************************************************************************"*/
/*"  Function:       SetUiBlkData"*/
/*"  Description:   �����������ں���"*/
/*"  Input:          obis: �����ʶ"*/
/*"                     index: ����Ԫ������������0��ʾ������"*/
/*"                     setLen: �������ݳ���"*/
/*"                     pData: �������ָ��"*/
/*"  Output         �� "*/
/*"  Return:        DAR"*/
/*"  Others:        code size  byte"*/
/*"*****************************************************************************"*/
static INT8S SetUiBlkData(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U setLen, INT8U *pData)
{
    INT8U i;
    INT8S result;

    result = MS_OBIS_ERR;

    for(i = 0; i < ARRAY_SIZE(mstUiFuncList); i++)
    {
        if(BlkAttr->obis == mstUiFuncList[i].obis)
        {
            result = mstUiFuncList[i].setFunc(BlkAttr, index, setLen, pData);
            break;
        }
    }

    return result;
}

/*"*****************************************************************************"*/
/*"  Function:       ChkUiByteRange"*/
/*"  Description:    ��������ֽڲ������ݷ�Χ"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          obis: �����ʶID��pData:  �������ָ��"*/
/*"  Output          "*/
/*"  Return:         DAR"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
static INT8S ChkUiByteRange(INT16U obis, INT8U *pData)
{
    INT8U i;
    INT8S result;

    result = MS_SUCCESS;

    for(i = 0; i < ARRAY_SIZE(mstUiByteRangeList); i++)
    {
        if(obis == mstUiByteRangeList[i].obis)
        {
            if((*pData > mstUiByteRangeList[i].max) || (*pData < mstUiByteRangeList[i].min))
            {
                result = MS_OTHER_ERR;
            }
            break;
        }
    }

    return result;
}

/*"*****************************************************************************"*/
/*"  Function:       ChkUiWordRange"*/
/*"  Description:    ��������ֽڲ������ݷ�Χ"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          obis: �����ʶID��pData:  �������ָ��"*/
/*"  Output          "*/
/*"  Return:         DAR"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
static INT8S ChkUiWordRange(INT16U obis, INT8U *pData)
{
    INT8U i;
    INT8S result;

    result = MS_SUCCESS;

    for(i = 0; i < ARRAY_SIZE(mstUiWordRangeList); i++)
    {
        if(obis == mstUiWordRangeList[i].obis)
        {
            if((*pData > mstUiWordRangeList[i].max) || (*pData < mstUiWordRangeList[i].min))
            {
                result = MS_OTHER_ERR;
            }
            break;
        }
    }

    return result;
}


static INT8S ChkUiINT32Range(INT16U obis, INT8U *pData)
{
    INT8S result;

    result = MS_SUCCESS;

    if((obis == E_LONGITUDE)|| (obis ==  E_LATITUDE))
    {   
        if(*pData > 1)/*��λ����Ϊ0����1*/
        {
            result = MS_OTHER_ERR;
        }
    }
    else
    {

    }
    return result;
}
/*"*****************************************************************************"*/
/*"  Function:       UI_ChkLCDParaValid"*/
/*"  Description:    ���LCD�����Ϸ���"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          OAD��pdata:  ����"*/
/*"  Output          "*/
/*"  Return:         DAR"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U UI_ChkLCDParaValid(INT8U *pdata)
{
    INT16U obis;
    INT8U *pPara;
    INT8U i, cnt, frmtId, len, index;
    INT8S result;
    ST_OAD Oad;

    GetSingle(E_PP_OAD,(INT8U *)&Oad);    

    result = DAR_0_OK;

    if(0 == Oad.IId)
    {
        cnt = ARRAY_SIZE(mLcdParaObis);
        index = 0;
    }
    else if(Oad.IId <= ARRAY_SIZE(mLcdParaObis))
    {
        cnt = 1;
        index = Oad.IId - 1;
    }
    else
    {
        result = DAR_6_OANOEXIST;
    }

    if(DAR_0_OK == result)
    {
        pPara = pdata;

        for(i = 0; i < cnt; i++)
        {
            obis = mLcdParaObis[index];
            frmtId = UI_GetFrmt(obis);
            len = FRMT_sizeofFormat(stFormatList[frmtId].format);
            if(len == sizeof(INT16U))
            {
                result = ChkUiWordRange(obis, pPara);
            }
            else
            {
                result = ChkUiByteRange(obis, pPara);
            }

            if(MS_SUCCESS != result)
            {
                result = DAR_8_OVERFLOW;
                break;
            }
            else
            {
                index += 1;
                pPara += len;
            }
        }
    }

    return result;
}

/*"*****************************************************************************"*/
/*"  Function:       UI_GetMultiTermn"*/
/*"  Description:    ���๦�ܶ��ӹ���ģʽ"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          OAD��pdata:  ������ݵ�ַ"*/
/*"  Output          "*/
/*"  Return:         DAR"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8U UI_GetMultiTermn( INT8U *pdata)
{
    INT8U result;
    ST_OAD Oad;

    GetSingle(E_PP_OAD,(INT8U *)&Oad);
        
    if((0xF207 == Oad.Oi.asInt) && (Oad.IId <= 1))
    {
        *pdata = OC_ReadMultiMode();
        result = DAR_0_OK;
    }
    else
    {
        result = DAR_6_OANOEXIST;
    }

    return result;
}


