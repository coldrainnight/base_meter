/*"*****************************************************************************"*/
/*"  FileName: AUTO_dataProcess.c"*/
/*"  Description:     该文件提供自动化生产模块对外对象访问接口"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.01.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
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
/*"  Description:    数据交换函数，根据数据标识和输入/输出控制，对数据进行相应的交换处理"*/
/*"  Calls:          "*/
/*"  Called By:      LRM_Get,LRM_Set"*/
/*"  Input:          id 数据标识，通过数据标识管理数据中心数据"*/
/*"                  pbuffer 输入/输出数据地址指针                 "*/
/*"                  R_W    输入/输出控制  1 输出数据到数据中心    0 从数据中心获取数据"*/
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
		dataLen = stSingleDataAttr.len; /*"直接获取定义的长度"*/

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
			rsult = dataLen;/*"返回抄读数据长度"*/
		}
		else
		{
			if(InRamFlag == TRUE)
			{
				LIB_MemCpy(pbuffer,(INT8U *)RamAddr,dataLen);/*"写RAM后，还需写EEPROM"*/
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
/*"  Description:    预付费模块数据对象抄读接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      message模块"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"  Output:         pBuff     数据接收缓存地址指针"*/
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
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数设置指定对象数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"                  pData     输入数据指针"*/
/*"                  len       设置数据长度"*/
/*"  Output:         "*/
/*"  Return:         TURE 对象设置成功 FALSE 对象设置失败"*/
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
/*"  Description:    预付费模块方法访问接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      MS"*/
/*"  Input:          id        对象标识  "*/
/*"                  pData     输入数据指针"*/
/*"  Output:         pbuff     输出数据缓存指针"*/
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
/*"  Description:    获取指定ID对象数据格式"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
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


