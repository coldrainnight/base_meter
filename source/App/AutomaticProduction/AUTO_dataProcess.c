/*"*****************************************************************************"*/
/*"  FileName: AUTO_dataProcess.c"*/
/*"  Description:     ���ļ��ṩ�Զ�������ģ����������ʽӿ�"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.01.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      ganjp    2017/1/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "..\..\System\Sys_def.h"
#include "AUTO.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "App\message\FRMT_def.h"

/*"*****************************************************************************"*/
/*"  Function:       AUTO_exChangeData"*/
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
static INT16S AUTO_exChangeData(INT16U id, INT8U *pbuffer,INT8U R_W)
{
    INT16U dataId;
    INT16S rsult;
	INT8U InRamFlag;
	INT16U dataLen,Offset;
	DATA_ADDR RamAddr;
	ENUM_FS_ID FileId;

    dataId = (id & 0x0fff);
    dataLen = 0;
    rsult = MS_SUCCESS;
    if(dataId < 0x0e00)
    {
         rsult = MS_OBIS_ERR;
    }
    else
	{
		ST_SINGLE_DATA_ATTR stSingleDataAttr;

		stSingleDataAttr = stAutoSingleDataList[dataId -0x0E00];
		dataLen = stSingleDataAttr.len; /*"ֱ�ӻ�ȡ����ĳ���"*/

		MSADDR_IN_RAM(stSingleDataAttr.addr,InRamFlag);
		if(InRamFlag == TRUE)
		{
			MSADDR_TO_RAMADDR(stSingleDataAttr.addr,RamAddr);
		}
		else
		{
			MSADDR_TO_FILEINFO(stSingleDataAttr.addr,FileId,Offset);
		}

		if(R_MODE == R_W)
		{
			if(InRamFlag == TRUE)
			{
				LIB_MemCpy((INT8U *)RamAddr,pbuffer,dataLen);
			}
			else/*"EEPROM address"*/
			{
				FS_ReadFile(FileId,(INT32U)Offset,pbuffer,dataLen);
			}
			rsult = dataLen;/*"���س������ݳ���"*/
		}
		else
		{
			if(InRamFlag == TRUE)
			{
				LIB_MemCpy(pbuffer,(INT8U *)RamAddr,dataLen);/*"дRAM�󣬻���дEEPROM"*/
			}
			else/*"EEPROM address"*/
			{
				FS_WriteFile(FileId,(INT32U)Offset,pbuffer,dataLen);
			}
		}
	}
    
    return rsult;
	
}


/*"*****************************************************************************"*/
/*"  Function:       PM_Get"*/
/*"  Description:    Ԥ����ģ�����ݶ��󳭶��ӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      messageģ��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         pBuff     ���ݽ��ջ����ַָ��"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S AUTO_Get (INT16U id, INT8U *pBuff)
{
    INT16S rsult;

    rsult = AUTO_exChangeData(id, pBuff, R_MODE);
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       PM_Set"*/
/*"  Description:    ���ݽ����ӿں���������ģ��ͨ�����øýӿں�������ָ����������"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"                  pData     ��������ָ��"*/
/*"                  len       �������ݳ���"*/
/*"  Output:         "*/
/*"  Return:         TURE �������óɹ� FALSE ��������ʧ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S AUTO_Set (INT16U id, INT8U *pData, INT16U len)
{
    INT8S rsult;
	
    rsult = (INT8S)AUTO_exChangeData(id, pData, W_MODE);
	
    return (rsult);
}


/*"*****************************************************************************"*/
/*"  Function:       PM_Action"*/
/*"  Description:    Ԥ����ģ�鷽�����ʽӿں���"*/
/*"  Calls:          "*/
/*"  Called By:      MS"*/
/*"  Input:          id        �����ʶ  "*/
/*"                  pData     ��������ָ��"*/
/*"  Output:         pbuff     ������ݻ���ָ��"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S AUTO_Action (INT16U id, void * pdata, void * pbuff)
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
        rsult = stAutoActionList[FunId].Fuction(pdata, pbuff);
    }
    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       AUTO_GetFrmt"*/
/*"  Description:    ��ȡָ��ID�������ݸ�ʽ"*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id        �����ʶ��ͨ�������׼���ƽ��ж������  "*/
/*"  Output:         "*/
/*"  Return:         format id"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U AUTO_GetFrmt (INT16U id)
{
    INT8U frmtId;
    INT16U dataId;

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
            stBlkDataAttr = stAutoBlkDataList[dataId >> 4];
            frmtId = stBlkDataAttr.format_id;
        }
        else
        {
            ST_SINGLE_DATA_ATTR stSingleDataAttr;
            stSingleDataAttr = stAutoSingleDataList[dataId -0x0E00];
            frmtId = stSingleDataAttr.format_id;
        }
    }

    return (frmtId);
	
}


