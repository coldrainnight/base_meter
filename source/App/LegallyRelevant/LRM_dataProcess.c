/*"*****************************************************************************"*/
/*"  FileName: LRM_dataProcess.c"*/
/*"  Description:     ���ļ��ṩ��������ģ����������ʽӿ�"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.09.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      ganjp    2017/9/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "LRM_dataProcess.h"
#include "DataCenterSoftware\Code\LegallyRelevant\LRM_data.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "App\message\FRMT_def.h"
#include "LRM_Config.h"
#include "EU.h"
#include "SC.h"
#include "Driver\device\MU_IC.h"

/*"------private function-------"*/
static INT16S exChangeData(INT16U id, INT8U *pbuffer,INT8U R_W);
static INT8U GetBlkData(INT8U num, INT32U Addr, INT8U *pBuff, INT8U MaxNum);
static INT8U AssEngyMath(INT8U *pDataBuff, INT8U Assmode, INT8U Ass_num);

static INT16U  DataObis;  /*"��ʱ����get���ʵĶ����ʶ"*/
/*"*****************************************************************************"*/
/*"  Function:       LRM_Get"*/
/*"  Description:    ��������ģ�����ݶ��󳭶��ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      messageģ��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         pBuff     ���ݽ��ջ����ַָ��"*/
/*"  Return:         DAR    > 0 ���ݳ���    <=0 ��������"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S LRM_Get (INT16U id, INT8U *pBuff)
{
    INT16S rsult;

    DataObis = id;
    rsult = exChangeData(id, pBuff, R_MODE);
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       LM_Set"*/
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
#ifdef SINGLE_PHASE
const INT16U LrmParaList[] = {E_A_PULSE_CONST, E_A_ENGY_COM_TYPE};
#else
const INT16U LrmParaList[] = {E_A_PULSE_CONST, E_R_PULSE_CONST, E_A_ENGY_COM_TYPE, E_R_ENGY_COM_TYPE1, E_R_ENGY_COM_TYPE2};
#endif
#define  LRM_PARA_NUM   (sizeof(LrmParaList)/sizeof(INT16U))
INT8S LRM_Set (INT16U id, INT8U *pData, INT16U len)
{
    INT8S rsult;
    INT8U i;

    for(i = 0; i < LRM_PARA_NUM; i++)
    {
        if(id == LrmParaList[i])
        {
            break;
        }
    }
    if(i < LRM_PARA_NUM)
    {
        ChkLrmParaValid();
#ifdef SINGLE_PHASE
        if (E_A_PULSE_CONST == id)
#else
        if ((E_A_PULSE_CONST == id) || (E_R_PULSE_CONST == id))
#endif
        {
            INT32U PulseConst;
            INT8U  DeviceId;

            if(E_A_PULSE_CONST == id)
            {
                DeviceId = 0;
            }
            else
            {
                DeviceId = 1;
            }
            PulseConst = 0;
            LIB_MemCpy(pData, (INT8U *) &PulseConst, 2);
            rsult = MU_IC_PulseInit(DeviceId, PulseConst, stLrmPara.PulseWidth);
            if (TRUE == rsult)
            {
                rsult = MS_SUCCESS;
            }
            else
            {
#ifdef THREE_PHASE
                PulseConst = gMU_Para.Spec_Para.dwCFConstP;
#else
                PulseConst = gMU_Para.ram_adj_data.Ram_ecconst;
#endif
                if(1 == DeviceId)
                {
#ifdef THREE_PHASE
                    PulseConst = gMU_Para.Spec_Para.dwCFConstQ;
#endif
                }
                else
                {
                }
                MU_IC_PulseInit(DeviceId, PulseConst, stLrmPara.PulseWidth);
                rsult = MS_OTHER_ERR;
            }
        }
        else
        {
            rsult = MS_SUCCESS;
        }

        if (E_A_ENGY_COM_TYPE == id)
        {
           // SaveRcrd(E_A_ENGY_COM_PRG_EVNT_RCRD, E_EVTLST_A_ENGY_COM);
        }
#ifdef THREE_PHASE
        else if (E_R_ENGY_COM_TYPE1 == id)
        {
            DCDataBuf[0] = 0;
            SaveRcrd(E_R_ENGY_COM_PRG_EVNT_RCRD, E_EVTLST_R_ENGY_COM_PRG_S);
        }
        else if (E_R_ENGY_COM_TYPE2 == id)
        {
            DCDataBuf[0] = 1;
            SaveRcrd(E_R_ENGY_COM_PRG_EVNT_RCRD, E_EVTLST_R_ENGY_COM_PRG_S);
        }
#endif
        else
        {
        }
        if(MS_SUCCESS == rsult)
        {
            rsult = (INT8S) exChangeData(id, pData, W_MODE);
        }

        if (MS_SUCCESS == rsult)
        {
            stLrmPara.Crc = CRC16((INT8U *) &stLrmPara, FIND(ST_LRM_PARA, Crc));
            FS_WriteFile(E_FILE_LRM, LRM_PARA_BAK1_SHIFT, (INT8U *) &stLrmPara,sizeof(ST_LRM_PARA));
            FS_WriteFile(E_FILE_LRM, LRM_PARA_BAK2_SHIFT, (INT8U *) &stLrmPara,sizeof(ST_LRM_PARA));
#ifdef THREE_PHASE
            if((E_R_ENGY_COM_TYPE1 == id) || (E_R_ENGY_COM_TYPE2 == id))
            {
                Action(E_UPDATE_COM_R_MODE, (INT8U *)&stLrmPara.REngyComType1, NULL);
            }
#endif
        }
    }
    else
    {
      rsult = MS_W_AUTH_ERR; 
    }

    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       LM_Action"*/
/*"  Description:    ����ģ�鷽�����ʽӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      MS"*/
/*"  Input:          id        �����ʶ  "*/
/*"                  pData     ��������ָ��"*/
/*"  Output:         pbuff     ������ݻ���ָ��"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S LRM_Action (INT16U id, void * pdata, void * pbuff)
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
        if(FunId < LRM_ACTION_NUM)
        {
            if(id == stLrmActionList[FunId].obis)
            {
                rsult = stLrmActionList[FunId].Fuction(pdata, pbuff);
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
/*"  Function:       LRM_GetFrmt"*/
/*"  Description:    ��ȡָ��ID�������ݸ�ʽ"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         "*/
/*"  Return:         format id"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U LRM_GetFrmt (INT16U id)
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
            if(ListId < LRM_BLK_LIST_NUM)
            {
                stBlkDataAttr = stLrmBlkDataList[ListId];
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

            if(ListId < LRM_SINGLE_LIST_NUM)
            {
                stSingleDataAttr = stLrmSingleDataList[ListId];
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

        ListId = (dataId >> 4);
        if(ListId < LRM_BLK_LIST_NUM)
        {
            stBlkDataAttr = stLrmBlkDataList[ListId];
            Num = (dataId & 0x000f);
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
        if(ListId < LRM_SINGLE_LIST_NUM)
        {
            stSingleDataAttr = stLrmSingleDataList[ListId];
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
/*"  Function:       GetBlkData"*/
/*"  Description:    ��ȡ�����ݶ�����ָ����Ա����"*/
/*"  Calls:          "*/
/*"  Called By:      exChangeData"*/
/*"  Input:          num   ָ�������ݳ�Ա�ţ���Ա�Ŵ�0��ʼ"*/
/*"                  Addr  �����ַ����"*/
/*"                  MaxNum����������Ա��"*/
/*"  Output:         pBuff ���ݽ��ջ���ָ��"*/
/*"  Return:         ���ݳ���"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#if 0
#if(ENERGY_SIZE > 4)
#define ENGY_COPY_SIZE   8
#else
#define ENGY_COPY_SIZE   4
#endif
#endif

static INT8U GetBlkData(INT8U num, INT32U Addr, INT8U *pBuff, INT8U MaxNum)
{
    INT8U len;
    INT16U BlkObis;
	ST_FMT_ATTR DataFrmt;
	INT8U FrmtId;

	FrmtId = LRM_GetFrmt(DataObis);
	DataFrmt = stFormatList[FrmtId];

    BlkObis = (DataObis & 0xfff0);
    if((DataFrmt.format == E_U_ENGY_X_BYTE || DataFrmt.format == E_S_ENGY_X_BYTE) /*"�ַ������ݿ�"*/
#if (PHASE_ENGY_EN == 1)
        || (E_A_ENGY_COM_PHASE_BLK == BlkObis)/*"��������й��������ݿ�"*/
        || (E_R_ENGY_COM1_PHASE_BLK == BlkObis)/*"��������޹�1�������ݿ�"*/
        || (E_R_ENGY_COM2_PHASE_BLK == BlkObis)/*"��������޹�2�������ݿ�"*/
#endif
        )
    {
        switch(BlkObis)
        {
        case E_A_ENGY_COM_BLK:
#if (PHASE_ENGY_EN == 1)
        case E_A_ENGY_COM_PHASE_BLK:
#endif
        {
            INT8U DataBuff[32];

            LIB_MemCpy((INT8U *) ((Addr - RAM) + num * sizeof(ST_BASE_ENGY)),DataBuff, 2 * ENERGY_SIZE);
            AssEngyMath(DataBuff, stLrmPara.AEngyComType, 2);
            LIB_MemCpy(DataBuff, pBuff, ENERGY_SIZE);
        }
            break;
#if (REACTIVE_ENGY_COM == 1)
        case E_R_ENGY_COM1_BLK:
        case E_R_ENGY_COM2_BLK:
#if (PHASE_ENGY_EN == 1)
        case E_R_ENGY_COM1_PHASE_BLK:
        case E_R_ENGY_COM2_PHASE_BLK:
#endif
        {
            INT8U DataBuff[32];
            INT8U REngyComTypeTmp;

            if(BlkObis == E_R_ENGY_COM1_BLK)
            {
                REngyComTypeTmp = stLrmPara.REngyComType1;
            }
#if (PHASE_ENGY_EN == 1)
            else if(BlkObis == E_R_ENGY_COM1_PHASE_BLK)
            {
                REngyComTypeTmp = stLrmPara.REngyComType1;
            }
            else if(BlkObis == E_R_ENGY_COM2_PHASE_BLK)
            {
                REngyComTypeTmp = stLrmPara.REngyComType2;
            }
#endif
            else
            {
                REngyComTypeTmp = stLrmPara.REngyComType2;
            }
            LIB_MemCpy((INT8U *) ((Addr - RAM) + num * sizeof(ST_BASE_ENGY)), DataBuff, 4 * ENERGY_SIZE);
            AssEngyMath(DataBuff, REngyComTypeTmp, 4);
            LIB_MemCpy(DataBuff, pBuff, ENERGY_SIZE);
        }
            break;
#endif
        default:
            LIB_MemCpy((INT8U *)((Addr-RAM) + num * sizeof(ST_BASE_ENGY)), pBuff, sizeof(ST_ENERGY_FORM));
            break;
        }

        len = ENERGY_SIZE;
    }
    else/*"˲ʱ�����ݿ�"*/
    {

        len = FRMT_sizeofFormat(DataFrmt.format);
        LIB_MemCpy((INT8U *)((Addr-RAM) + (num * len)), pBuff, len);
    }

    return len;
}


/*"*****************************************************************************"*/
/*"  Function:       AssEngyMath"*/
/*"  Description:    ������������ϼ���,�����������ģʽ�֣������������ֵ"*/
/*"  Calls:"*/
/*"  Called By:"*/
/*"  Input:          Assmode  ��ϼ���ģʽ"*/
/*"                  Ass_num    ������ϼ����Ա��"*/
/*"                  pDataBuff    �����������"*/
/*"  Output:         pDataBuff    ��ϼ�����"*/
/*"  Return:         ���ݳ���"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U AssEngyMath(INT8U *pDataBuff, INT8U Assmode, INT8U Ass_num)
{

    INT8U i;
    ENGY_TYPE_S buff[4];
    ENGY_TYPE_S sum;

    LIB_MemSet(0, (INT8U *)&buff, 4 * sizeof(ENGY_TYPE_S));

    for(i = 0; i < Ass_num; i++)
    {
        LIB_MemCpy(pDataBuff + i * ENERGY_SIZE, (INT8U *)&buff[i], ENERGY_SIZE);
    }
    /*"���"*/
    sum = 0;
    for(i = 0; i < Ass_num; i++)
    {
        switch(Assmode & 0x03)
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
        Assmode >>= 2;
    }
#if ENERGY_SIZE > 4
    LIB_Int64sToNbyteInts(sum, pDataBuff, ENERGY_SIZE);
#else
    LIB_MemCpy((INT8U *)&sum, pDataBuff, ENERGY_SIZE);
#endif

    return (ENERGY_SIZE);
}
