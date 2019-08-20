/*"*****************************************************************************"*/
/*"  FileName: MS.c"*/
/*"  Description:     该文件定义电能表数据交互对象解析及寻址，为各功能模块提供统一的数据交互接口"*/
/*"  Author: ganjp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.01.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      ganjp    2017/1/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "MS.h"
#include "FRMT_def.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\LegallyRelevant\LRM_dataProcess.h"
#ifdef DU_MODLE
#include "App\demand\DU_dataProcess.h"
#endif
#ifdef PM_MODLE
#include "App\Prepay\PM_dataProcess.h"
#endif
#ifdef TR_MODLE
#include "App\tariff\TR_dataProcess.h"
#endif
#ifdef MNT_MODLE
#include "App\monitor\MNT_dataProcess.h"
#endif
#ifdef RCRD_MODLE
#include "App\record\RCRD_dataProcess.h"
#endif
#ifdef UI_MODLE
#include "App\UserInterface\UI_dataProcess.h"
#endif
#ifdef AUTO_MODLE
#include "App\AutomaticProduction\AUTO_dataProcess.h"
#endif

static INT8U MtrStatus[MTR_STA_BYTE];/*"电表状态字"*/
INT8U DCDataBuf[DC_BUF_SIZE];/*"数据中心记录文件数据缓冲区，用于事件记录"*/
INT8U oprateCode[4];

typedef INT16S  (* pGet)(INT16U id, INT8U *pBuff);
typedef INT8S  (* pSet)(INT16U id, INT8U * pData, INT16U len);
typedef INT8S  (* pAction)(INT16U id, void *pdata, void *pbuff);
typedef INT8U  (* pGetFrmt)(INT16U id);
typedef struct
{
    INT16U IdMask;
    pGet FunGet;
    pSet FunSet;
    pAction FunAction;
    pGetFrmt FunGetFrmt;
}ST_ID_MAP;
const ST_ID_MAP stIdMapList[] =
        {
                {0x0000, LRM_Get, LRM_Set, LRM_Action, LRM_GetFrmt},
#ifdef DU_MODLE
                {0x1000, DU_Get, DU_Set, DU_Action, DU_GetFrmt},
#else
                {0x1000, NULL, NULL, NULL, NULL},
#endif
#ifdef TR_MODLE
                {0x2000, TR_Get, TR_Set, TR_Action, TR_GetFrmt},
#else
                {0x2000, NULL, NULL, NULL, NULL},
#endif
#ifdef PM_MODLE
                {0x3000, PM_Get, PM_Set, PM_Action, PM_GetFrmt},
#else
                {0x3000, NULL, NULL, NULL, NULL},
#endif
#ifdef MNT_MODLE
                {0x4000, MNT_Get, MNT_Set, MNT_Action, MNT_GetFrmt},
#else
                {0x4000, NULL, NULL, NULL, NULL},
#endif
#ifdef UI_MODLE
                {0x5000, UI_Get, UI_Set, UI_Action, UI_GetFrmt},
#else
                {0x5000, NULL, NULL, NULL, NULL},
#endif
#ifdef RCRD_MODLE
                {0x6000, RCRD_Get, RCRD_Set, RCRD_Action, RCRD_GetFrmt},
                {0x7000, RCRD_Get, RCRD_Set, RCRD_Action, RCRD_GetFrmt},
#else
                {0x6000, NULL, NULL, NULL, NULL},
                {0x7000, NULL, NULL, NULL, NULL},
#endif
#ifdef AUTO_MODLE
                {0x8000, AUTO_Get, AUTO_Set, AUTO_Action, AUTO_GetFrmt},
#else
                {0x8000, NULL, NULL, NULL, NULL},
#endif
        };

static INT8U TransFromChar(INT8U *src_data,INT8U *dest_data,
                      ST_FMT_ATTR inFormat,ST_FMT_ATTR outFormat);
static INT8U TransFromTime(INT8U *src_data, INT8U *dest_data,
        ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat);
static INT8U TransFromBCD(INT8U *src_data,INT8U *dest_data,
                      ST_FMT_ATTR inFormat,ST_FMT_ATTR outFormat);
static INT8U TransFromHex(INT8U *src_data,INT8U *dest_data,
                      ST_FMT_ATTR inFormat,ST_FMT_ATTR outFormat);
static INT32S tenSeq(INT8S seq);
#ifdef THREE_PHASE
static INT8U TransFromDemand(INT8U *src_data, INT8U *dest_data,
        ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat);
#endif
INT8U ChkUnitValid(INT8U inUint, INT8U outUnit);
INT32U searchUnit(INT8U unit);

typedef struct
{
    INT16U dateLen;
    INT8U  dataBuff[];
}ST_OUTPUT_BUFF;

#define MAX_ID_MAP_LIST_NUMB    ARRAY_SIZE(stIdMapList)
/*"*****************************************************************************"*/
/*"  Function:       GetSingle"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数获取模块所需单项数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"  Output:         pBuff     数据接收缓存地址指针"*/
/*"  Return:         DAR    > 0 数据长度    <=0 错误类型       "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S GetSingle(INT16U id, INT8U *pBuff)
{
    INT16U ObjectKind;
    INT8U  num;
    INT16S  rsult;

    rsult = MS_SUCCESS;
    ObjectKind = (id & 0xf000);
    num = (INT8U)(ObjectKind>>12);

    if(num >= MAX_ID_MAP_LIST_NUMB)
    {
        return (MS_OBIS_ERR);
    }

    if(ObjectKind == stIdMapList[num].IdMask)
    {
        if(NULL != stIdMapList[num].FunGet)
        {
            rsult = stIdMapList[num].FunGet(id, pBuff);
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

    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       GetList"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数获取模块所需多项数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          pIdList   对象标识列表指针，通过对象标准名称进行对象访问  "*/
/*"                  IdNum     抄读对象数"*/
/*"                  BuffSize  接收数据缓存区大小"*/
/*"  Output:         pBuff     数据接收缓存地址指针"*/
/*"  Return:         DAR    > 0 数据长度    <=0 错误类型"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S GetList(INT16U *pIdList, INT8U IdNum, INT8U *pBuff, INT16U BuffSize)
{
    INT16S rsult;
    INT8U i;
    INT16U dataLen;
    INT8U  SingleLen;
    ST_FMT_ATTR frmtTmp;
    INT8U FrmtId;

    rsult = MS_SUCCESS;
    dataLen = 0;
    if(IdNum > MAX_ID_LIST_NUM)
    {
        return (MS_LIST_NUM_OVRE);
    }

    for(i = 0; i < IdNum; i++)
    {
        FrmtId = GetFrmt(*(pIdList + i), &frmtTmp);
        if(0xff != FrmtId)
        {
            SingleLen = FRMT_sizeofFormat(frmtTmp.format);
        }
        else
        {
            SingleLen = 0;
        }

        if((dataLen + SingleLen) > BuffSize)
        {
            rsult = MS_LEN_ERR;
            break;
        }

        rsult = GetSingle(*(pIdList + i), (pBuff + dataLen));
        if(rsult > 0)
        {
            dataLen += rsult;
        }
        else
        {
            break;
        }

    }
    if(rsult > 0)
    {
        rsult = dataLen;
    }
    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       Set"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数设置指定对象数据"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          pIdList   对象标识列表指针，通过对象标准名称进行对象访问  "*/
/*"                  IdNum     抄读对象数"*/
/*"                  pData     输入数据指针"*/
/*"                  pLenList  设置数据长度列表指针"*/
/*"  Output:         "*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S Set(INT16U *pIdList, INT8U IdNum, INT8U *pData, INT8U *pLenList)
{
    INT16U ObjectKind;
    INT8U  num;
    INT8S  rsult;
    INT16U IdTmp;
    INT16U LenTmp;
    INT16U DataLen;
    INT8U i;
    INT8U frmtId;
    INT8U LenCmp;;

    rsult = MS_SUCCESS;
    LenTmp = 0;
    DataLen = 0;

    if(IdNum > MAX_ID_LIST_NUM)
    {
        return (MS_LIST_NUM_OVRE);
    }

    for(i = 0; i < IdNum; i++)
    {
        IdTmp = *(pIdList + i);


        ObjectKind = (IdTmp & 0xf000);
        num = (INT8U)(ObjectKind>>12);
        if(num >= MAX_ID_MAP_LIST_NUMB)
        {
            rsult = MS_OBIS_ERR;
            break;
        }

        if(ObjectKind == stIdMapList[num].IdMask)
        {
            if(NULL != stIdMapList[num].FunGetFrmt)
            {
                frmtId = stIdMapList[num].FunGetFrmt(IdTmp);
            }
            else
            {
                rsult = MS_OBIS_ERR;
                break;
            }

            if(frmtId < FRMT_NUM)
            {
                LenCmp = FRMT_sizeofFormat(stFormatList[frmtId].format);
            }
            else
            {
                rsult = MS_LEN_ERR;
                break;
            }
            if(1 == IdNum)/*"设置单个参数时，数据长度由传入参数定，设置多个参数时，每个数据长度通过存储格式确定"*/
            {
                LenTmp = *(pLenList);
            }
            else
            {
                LenTmp = LenCmp;
            }
#if 0
            if(LenCmp == LenTmp)/*"写数据长度与保存数据格式长度是否一致"*/
            {
#endif
                if(NULL != stIdMapList[num].FunSet)
                {
                    rsult = stIdMapList[num].FunSet(IdTmp, pData + DataLen, LenTmp);
                    if(MS_SUCCESS == rsult)
                    {
                        DataLen += LenCmp;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    rsult = MS_OBIS_ERR;
                }
#if 0
            }
            else
            {
                rsult = MS_LEN_ERR;
                break;
            }
#endif
        }
        else
        {
            rsult = MS_OBIS_ERR;
            break;
        }
    }

    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       Action"*/
/*"  Description:    数据交互接口函数，功能模块通过调用该接口函数执行指定方法"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识"*/
/*"                  pdin     输入数据指针"*/
/*"  Output:         pdout    输出数据缓存指针"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S Action (INT16U id, void *pdin, void *pdout)
{
    INT16U ObjectKind;
    INT8U  num;
    INT8S  rsult;

    rsult = MS_SUCCESS;
    ObjectKind = (id & 0xf000);
    num = (INT8U)(ObjectKind>>12);

    if(num >= MAX_ID_MAP_LIST_NUMB)
    {
        return (MS_OBIS_ERR);
    }

    if(ObjectKind == stIdMapList[num].IdMask)
    {
        if(0x7000 != ObjectKind)
        {
            rsult = stIdMapList[num].FunAction(id, pdin, pdout);
        }
    }
    else
    {
        rsult = MS_OBIS_ERR;
    }

    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       GetRcrd"*/
/*"  Description:    记录文件数据筛选接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识"*/
/*"                  pdin     输入数据指针"*/
/*"                  len      输出数据缓存大小"*/
/*"  Output:         pdout    输出数据缓存指针"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S GetRcrd (INT16U id, void *pdin, void *pdout, INT16U len)
{
    INT16U rsult;

    rsult = MS_SUCCESS;
#ifdef RCRD_MODLE
    rsult = RCRD_GetRcrd((EN_RCRD_FILE_ID)id, pdin, pdout, len);
#endif
    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       SaveRcrd"*/
/*"  Description:    保存记录文件接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          file     记录文件名"*/
/*"                  ListObis 记录关联列表对象名"*/
/*"  Output:         "*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S SaveRcrd(INT16U file, INT16U ListObis)
{
    INT16U rsult;

    rsult = MS_SUCCESS;
#ifdef RCRD_MODLE
    rsult = RCRD_SaveData((EN_RCRD_FILE_ID)file, (EN_FELXOBJ_ID)ListObis);
#endif
    return rsult;
}
/*"*****************************************************************************"*/
/*"  Function:       GetFrmt"*/
/*"  Description:    获取指定对象格式属性信息"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识  "*/
/*"  Output:         pformat   输出格式信息指针"*/
/*"  Return:         FORMAT ID  0XFF标识没有获取到格式信息 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U GetFrmt(INT16U id, ST_FMT_ATTR *pformat)
{
    INT16U ObjectKind;
    INT8U num;
    INT8U frmtId;

    ObjectKind = (id & 0xf000);
    num = (INT8U) (ObjectKind >> 12);

    if(num >= MAX_ID_MAP_LIST_NUMB)
    {
        return (MS_OBIS_ERR);
    }

    if((ObjectKind == stIdMapList[num].IdMask) && (stIdMapList[num].FunGetFrmt != NULL))
    {
        frmtId = stIdMapList[num].FunGetFrmt(id);
    }
    else
    {
        frmtId = 0xff;
    }

    if(frmtId < FRMT_NUM)
    {
        *pformat = stFormatList[frmtId];
    }
    return frmtId;
}

/*"*****************************************************************************"*/
/*"  Function:       Push"*/
/*"  Description:    电表状态推送接口"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          StaId    指定状态标识 "*/
/*"                  Status   推送状态 "*/
/*"  Output:         "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void Push(EN_MTR_STATUS StaId, INT8U Status)
{
    MCU_PwrChkCloseINT();
    if(TRUE == Status)
    {
        MtrStatus[StaId >> 3] |= (INT8U) (0x01 << (StaId & 0X07));
    }
    else
    {
        MtrStatus[StaId >> 3] &= ~(INT8U) (0x01 << (StaId & 0X07));
    }
    MCU_PwrChkOpenINT();
}

/*"*****************************************************************************"*/
/*"  Function:       Push"*/
/*"  Description:    电表状态查询接口"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          StaId    指定状态标识 "*/
/*"  Output:         "*/
/*"  Return:         E_OCCUR 状态发生   E_END 状态结束"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ChkStatus(EN_MTR_STATUS StaId)
{
    if(0 == (MtrStatus[StaId >> 3] & (0x01 << (StaId & 0X07))))
    {
        return (FALSE);
    }
    else
    {
        return (TRUE);
    }
}
/*"---------------------------------数据格式转换功能函数------------------------------"*/
/*"***************************************************************"*/
/*"  Function:     FRMT_NdataChange "*/
/*"  Description:  多个数据转换处理函数 "*/
/*"  Input:       srcData：输入数据指针 "*/
/*"               inFormat： 输入数据格式，  "*/
/*"               outFormat：输出数据格式  "*/
/*"               Len：输入数据长度  "*/
/*"  Output:      destData 输出数据指针"*/
/*"  Return:     ret 转换数据长度， "*/
/*"                  ret=_FALSE 数据转换错误  "*/
/*"****************************************************************"*/
INT16U FRMT_NdataChange(INT8U *srcData, INT8U *destData,
                      ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat,INT16U Len)
{
    INT8U Num;
    INT8U i;
    INT8U singleLenIn;
    INT8U singleLenOut;
    INT8U dataLen;

    singleLenIn = FRMT_sizeofFormat(inFormat.format);
    singleLenOut = FRMT_sizeofFormat(outFormat.format);
    if((0xff == singleLenIn) || (0xff == singleLenOut))
    {
        return (FALSE);
    }
    if((Len % singleLenIn) == 0)
    {
        Num = Len / singleLenIn;
    }
    else
    {
        return (FALSE);
    }

    dataLen = 0;
    for(i = 0; i < Num; i++)
    {
        dataLen = FRMT_DataChange(srcData + (i * singleLenIn), destData + (i * singleLenOut), inFormat, outFormat);
        if(0xff == dataLen)
        {
            dataLen = 0;
            break;
        }
    }
    
    if(0!=dataLen)
    {
       return ((INT16U)singleLenOut * Num); 
    } 
    else
    {
       return 0; 
    }    
    
}
/*"***************************************************************"*/
/*"  Function:     FRMT_DataChange "*/
/*"  Description:  数据转换处理函数，转换依据： "*/
/*"                      1、输入输出数据格式； "*/
/*"                      2、输入输出数据单位； "*/
/*"                      3、输入输出数据小数位数，主要用于645协议数 "*/
/*"                                                                              "*/
/*"  Input:       srcData：输入数据指针，   "*/
/*"                  inFormat： 输入数据格式，  "*/
/*"                  outFormat：输出数据格式  "*/
/*"  Output:       destData 输出数据指针"*/
/*"  Return:     ret 转换数据长度， "*/
/*"                  ret=_FALSE 数据转换错误  "*/
/*"****************************************************************"*/
INT8U FRMT_DataChange (INT8U *srcData, INT8U *destData,
                      ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat)
{
    INT8U ret;


    INT8U (* trans_from_xx)(INT8U *src_data,INT8U *dest_data,ST_FMT_ATTR inFmt,ST_FMT_ATTR outFmt);

    ret = 0;
    trans_from_xx = NULL;

    if((inFormat.format == outFormat.format)/*"输入输出数据格式一样，不需要进行数据转换"*/
    && (inFormat.unit == outFormat.unit) && (inFormat.exp == outFormat.exp))
    {
        ret = FRMT_sizeofFormat(outFormat.format);
        if(0xff != ret)
        {
            LIB_MemCpy(srcData, destData, ret);
        }
    }
    else if((inFormat.format > E_HEX_FORMAT) && (inFormat.format < E_TIME_FORMAT))/*"hex format"*/
    {
        trans_from_xx = TransFromHex;
    }
#if defined (THREE_PHASE)
    else if(inFormat.format == E_DEMAND_AND_TIME)
    {
        trans_from_xx = TransFromDemand;
    }
#endif
    else if((inFormat.format > E_STRING) && (inFormat.format < E_L_BCDFORMAT))/*"char string format"*/
    {
        trans_from_xx = TransFromChar;
    }
    else if((inFormat.format > E_BCDFORMAT) && (inFormat.format < E_HEX_FORMAT))/*"bcd format"*/
    {
        trans_from_xx = TransFromBCD;
    }
    else if((inFormat.format >= E_TIME_FORMAT) && (inFormat.format < E_STRING))/*"time format"*/
    {
        trans_from_xx = TransFromTime;
    }
    else
    {
        ret = 0xff;
    }

    if((ret != 0xff) && ( trans_from_xx != NULL))
    {
        ret = trans_from_xx(srcData, destData, inFormat, outFormat);
    }

    return ret;
}


/*"*****************************************************************************"*/
/*"  Function:      TransFromHex"*/
/*"  Description:   对十六进制数据进行数据转换处理，转换依据1、输入输出数据格式；"*/
/*"                 2、输入输出数据单位（单相表不支持单位转换）；3、输入输出数据小数位数"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          src_data  输入数据指针"*/
/*"                  inFormat.format 输入数据格式，outFormat.format输出数据格式"*/
/*"                  inFormat.unit 输入数据单位，outFormat.unit输出数据单位"*/
/*"                  inFormat.exp 输入小数位数， outFormat.exp输出数据小数位数"*/
/*"  Output:         dest_data 输出数据指针"*/
/*"  "*/
/*"  Return:         ret 转换数据长度，ret=0xff 数据转换错误"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U TransFromHex(INT8U *src_data,INT8U *dest_data,
                      ST_FMT_ATTR inFormat,ST_FMT_ATTR outFormat)
{
    INT8U i,ret;
    INT64U bcd_mask;
    INT32U ui,uo;
    INT32S multiple;
    INT8S exp_temp;
    INT8S OutexpTmp;
    ENGY_TYPE_U ul_temp;
    ENGY_TYPE_S l_temp;
    INT16S intTmp;
    INT8U len;
    ENGY_TYPE_U ul_tempEngy;/*"用于分离高精度电量小数部分临时计算变量"*/
    ENGY_TYPE_S l_tempEngy;/*"用于分离高精度电量小数部分临时计算变量"*/
    INT8U EngySep;/*"电量分离处理标志"*/

    ret = 0;
    EngySep = 0;

    if(inFormat.unit != outFormat.unit)/*"需进行单位转换,单相表不支持单位转换"*/
    {
#if 1
        ret = ChkUnitValid(inFormat.unit,outFormat.unit);/*"单位转换可行性检查"*/
        if(ret == FALSE)
        {
            return 0xff;
        }
#else
        return 0xff;
#endif
    }

    /*"单位合法性判断"*/
    if((inFormat.unit >= E_MAX_UNIT) || (outFormat.unit >= E_MAX_UNIT))
    {
        return 0xff;
    }


    ul_temp = 0x00;
    l_temp = 0x00;
    multiple = 1;
    len = FRMT_sizeofFormat(inFormat.format);
    switch(inFormat.format)
    {
    case E_S_4_BYTE:
	{
		INT32S  DataTmp;
		DataTmp = 0;
		LIB_MemCpy((INT8U *)src_data, (INT8U *)&DataTmp, 4);

		l_temp = DataTmp;
    	}
        break;
    case E_S_ENGY_X_BYTE:
#if ENERGY_SIZE > 4
            l_temp = LIB_NbyteIntsToInt64s(src_data, len);
#else
            LIB_MemCpy((INT8U *)src_data, (INT8U *)&l_temp, 4);
#endif
        break;
    case E_S_2_BYTE:
        LIB_MemCpy((INT8U *)src_data, (INT8U *)&intTmp, 2);
        l_temp = intTmp;
        break;
    case E_S_1_BYTE:
        l_temp = *((INT8S *) src_data);
        break;
    case E_U_4_BYTE:
    case E_U_ENGY_X_BYTE:
    case E_U_2_BYTE:
    case E_U_1_BYTE:
        LIB_MemCpy((INT8U *)src_data, (INT8U *)&ul_temp, len);
        break;
    default:
        ret = 0xff;
        break;
    }
    ul_tempEngy = ul_temp;
    l_tempEngy = l_temp;
    if(((inFormat.format == E_U_ENGY_X_BYTE) && ((outFormat.format == E_UENGY_H_INT) || (outFormat.format == E_UENGY_H_DECIMAL)))
     ||((inFormat.format == E_S_ENGY_X_BYTE) && ((outFormat.format == E_ENGY_H_INT) || (outFormat.format == E_ENGY_H_DECIMAL))))
    {
        EngySep = 0x5a;
        OutexpTmp = 0;
    }
    else
    {
        OutexpTmp = outFormat.exp;
    }
    if(0xff != ret)
    {
        /*"指数差"*/
        exp_temp =(INT8S)(OutexpTmp - inFormat.exp);
        multiple = tenSeq(exp_temp);
        if(multiple == 0)
        {
            ret = 0xff;
        }
    }

    if(0xff != ret)
    {
        /*"单位差"*/
        ui = 1;
        uo = 1;
//#ifdef  THREE_PHASE
        ui = searchUnit(inFormat.unit);
        uo = searchUnit(outFormat.unit);
//#endif

        if(ui != uo)/*"小单位转大单位除以倍数"*/
        {
//#ifdef  THREE_PHASE
#if 1
            INT32U temp;
            if(multiple < 0)/*"指数从大变小乘以倍数"*/
            {
                multiple = LIB_ABS(multiple);
                temp = LIB_ABS(l_temp);
                temp = (temp * ui * multiple) / uo;
                if(l_temp < 0)
                {
                    l_temp = 0 - temp;
                }
                else
                {
                    l_temp = temp;
                }
                ul_temp = (ul_temp * ui * multiple) / uo;
            }
            else if(multiple > 0)/*"指数从小变大除以倍数"*/
            {
                temp = LIB_ABS(l_temp);
                temp = (temp * ui) / (uo * multiple);
                if(l_temp < 0)
                {
                    l_temp = 0 - temp;
                }
                else
                {
                    l_temp = temp;
                }
                ul_temp = (ul_temp * ui) / (uo * multiple);
            }
#endif
        }
        else
        {
            if(multiple < 0)/*"乘法"*/
            {
                multiple = LIB_ABS(multiple);
                l_temp *= multiple;/*"可能溢出，溢出不处理"*/
                ul_temp *= multiple;
            }
            else if(multiple > 0)/*"除法"*/
            {
                l_temp /= multiple;/*"可能溢出，溢出不处理"*/
                ul_temp /= multiple;
            }
        }

        if((0x5a == EngySep) && ((outFormat.format == E_UENGY_H_DECIMAL) || (outFormat.format == E_ENGY_H_DECIMAL)))
        {
            ul_temp = ul_tempEngy - (ul_temp * multiple);
            l_temp = l_tempEngy - (l_temp * multiple);

            /*"指数差"*/
            exp_temp = (INT8S) (outFormat.exp - inFormat.exp);
            multiple = tenSeq(exp_temp);
            if (multiple == 0)
            {
                ret = 0xff;
            }
            if(0xff != ret)
            {
                if (multiple < 0)/*"乘法"*/
                {
                    multiple = LIB_ABS(multiple);
                    l_temp *= multiple;/*"可能溢出，溢出不处理"*/
                    ul_temp *= multiple;
                }
                else if (multiple > 0)/*"除法"*/
                {
                    l_temp /= multiple;/*"可能溢出，溢出不处理"*/
                    ul_temp /= multiple;
                }
            }
        }
        /*"格式差"*/
        if((outFormat.format < E_TIME_FORMAT) && (outFormat.format > E_HEX_FORMAT))/*"hex format"*/
        {
            /*"hex to hex"*/
            ret = FRMT_sizeofFormat(outFormat.format);
            /*"源数据是有符号的数，需要无符号的取绝对值"*/
            if((inFormat.format < E_U_ENGY_X_BYTE)
                    && (outFormat.format > E_S_1_BYTE))
            {
                if(l_temp > 0)
                {
                    ul_temp = l_temp;
                }
                else
                {
                    ul_temp = 0 - l_temp;
                }
                //ul_temp = LIB_ABS(l_temp);
#if ENERGY_SIZE > 4
#else           /*"源数据只有4字节的情况下，只能拷贝4字节数据，否则可能拷贝越界"*/
                if(outFormat.format == E_U_ENGY_8_BYTE)
                {
                    ret = 4;
                }
#endif

                LIB_MemCpy((INT8U *)&ul_temp, (INT8U *)dest_data, ret);
            }
            /*"源数据是无符号的数，需要有符号，溢出"*/
            else if((inFormat.format > E_S_1_BYTE)
                    && (outFormat.format < E_U_ENGY_X_BYTE))
            {
                ret = 0xff;
            }
            /*"如果源、目的数据都是有符号或无符号数，直接赋值" */
            else if((inFormat.format < E_U_ENGY_X_BYTE)
                    && (outFormat.format < E_U_ENGY_X_BYTE))
            {
#if ENERGY_SIZE > 4
#else           /*"源数据只有4字节的情况下，只能拷贝4字节数据，否则可能拷贝越界"*/
                if (outFormat.format == E_S_ENGY_8_BYTE)
                {
                    ret = 4;
                }
#endif
                if(ret <= 4)
                {
                    if(l_temp > 0)
                    {
                        multiple = l_temp;
                    }
                    else
                    {
                        multiple = 0 - l_temp;
                        multiple = 0 - multiple;
                    }
                    LIB_MemCpy((INT8U *)&multiple, (INT8U *)dest_data, ret);
                }
                else
                {
                    LIB_MemCpy((INT8U *)&l_temp, (INT8U *)dest_data, ret);
                }
            }
            else if((inFormat.format > E_S_1_BYTE)
                    && (outFormat.format > E_S_1_BYTE))
            {
#if ENERGY_SIZE > 4
#else           /*"源数据只有4字节的情况下，只能拷贝4字节数据，否则可能拷贝越界"*/
                if (outFormat.format == E_U_ENGY_8_BYTE)
                {
                    ret = 4;
                }
#endif
                LIB_MemCpy((INT8U *)&ul_temp, (INT8U *)dest_data, ret);
            }
        }
        else if((outFormat.format > E_BCDFORMAT)
                && (outFormat.format < E_HEX_FORMAT))/*"Hex to BCD"*/
        {
            ret = FRMT_sizeofFormat(outFormat.format);
            if(ret != 0xff)
            {
                if(inFormat.format < E_U_ENGY_X_BYTE)/*"有符号的数"*/
                {
                    if(l_temp > 0)
                    {
                        ul_temp = l_temp;
                    }
                    else
                    {
                        ul_temp = 0 - l_temp;
                    }
                }
                /*"计算取值范围"*/
                if(outFormat.format < E_UBCDFORMAT)/*"有符号最大79····"*/
                {
                    bcd_mask = 80;
                }
                else/*"无符号最大99····"*/
                {
                    bcd_mask = 100;
                }


                for(i = 1; i < ret; i++)
                {
                    bcd_mask *= 100;
                }
                ul_temp %= bcd_mask;/*"保证转换的数在取值范围以内"*/

                /*"转换为BCD"*/
                LIB_HexToBcd(ret, (INT8U *) &ul_temp, dest_data);

                /*"符号位处理，如果输入输出都是有符号的数且数据小于0那么在最高位置1"*/
                if((inFormat.format < E_U_ENGY_X_BYTE)
                        && (outFormat.format < E_UBCDFORMAT))
                {
                    if(l_temp < 0)
                    {
                        *(dest_data + ret - 1) |= 0x80;
                    }
                }
            }
        }
    }


    return ret;
}
/*"*****************************************************************************"*/
/*"  Function:      TransFromBCD"*/
/*"  Description:   对BCD码数据进行数据转换处理，先将BCD码数据转换为十六进制，在调用"*/
/*"                 TransFromHex"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          src_data  输入数据指针"*/
/*"                  inFormat.format 输入数据格式，outFormat.format输出数据格式"*/
/*"                  inFormat.unit 输入数据单位，outFormat.unit输出数据单位"*/
/*"                  inFormat.exp 输入小数位数， outFormat.exp输出数据小数位数"*/
/*"  Output:         dest_data 输出数据指针"*/
/*"  "*/
/*"  Return:         ret 转换数据长度，ret=0xff 数据转换错误"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U TransFromBCD(INT8U *src_data,INT8U *dest_data,
                      ST_FMT_ATTR inFormat,ST_FMT_ATTR outFormat)
{
    INT8U  ret, sign, flag;
    INT32S l_temp;
    INT32U ul_temp;
    ST_FMT_ATTR inFormatTemp;

    inFormatTemp = inFormat;
    ret = FRMT_sizeofFormat(inFormat.format);

    if((*(src_data + ret - 1) & 0x80) == 0)/*"BCD码数据的高字节的最高位用于表示符号位"*/
    {
        flag = 0;
    }
    else/*"先对有符号负数进行处理，否则后面的BCD数据合法性检查报错"*/
    {
        flag = (*(src_data + ret - 1));
        (*(src_data + ret - 1)) = (INT8U)(flag & 0x7f);
    }

    if(!LIB_CheckBCD((INT8U *) src_data, ret))
    {
        return 0xff;
    }

    if(flag != 0)
    {
        (*(src_data + ret - 1)) = flag;
    }

    sign = 0;
    l_temp = 0;
    ul_temp = 0;

    switch(inFormat.format)
    {
    case E_BCD_1_BYTE:
    case E_BCD_2_BYTE:
    case E_BCD_3_BYTE:
    case E_BCD_4_BYTE:
        if((*(src_data + inFormat.format - E_BCD_1_BYTE) & 0x80) != 0)/*"有符号BCD码，检查最高位符号位"*/
        {
            sign = 1;
            *(src_data + inFormat.format - E_BCD_1_BYTE) &= 0x7f;
        }

        LIB_BcdToHex((INT8U)(inFormat.format - E_BCD_1_BYTE + 1), src_data,
                (INT8U *) &l_temp);
        if(sign == 1)
        {
            l_temp = 0 - l_temp;
        }
        inFormatTemp.format = E_S_4_BYTE;
        ret = TransFromHex((INT8U *) &l_temp, (INT8U *) dest_data,
                inFormatTemp, outFormat);
        break;

    case E_UBCD_1_BYTE:
    case E_UBCD_2_BYTE:
    case E_UBCD_3_BYTE:
    case E_UBCD_4_BYTE:
        LIB_BcdToHex((INT8U)(inFormat.format - E_UBCD_1_BYTE + 1), src_data,
                (INT8U *) &ul_temp);
        inFormatTemp.format = E_U_4_BYTE;
        ret = TransFromHex((INT8U *) &ul_temp, (INT8U *) dest_data,
                inFormatTemp, outFormat);
        break;
    default:
        ret = 0xff;
        break;
    }

    return ret;
}

/*"*****************************************************************************"*/
/*"  Function:      TransFromTime"*/
/*"  Description:   对时间格式数据进行数据转换处理，转换依据1、输入输出数据格式；"*/
/*"                 2、输入输出数据单位；3、输入输出数据小数位数"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          src_data  输入数据指针"*/
/*"                  inFormat.format 输入数据格式，outFormat.format输出数据格式"*/
/*"                  inFormat.unit 输入数据单位，outFormat.unit输出数据单位"*/
/*"                  inFormat.exp 输入小数位数， outFormat.exp输出数据小数位数"*/
/*"  Output:         dest_data 输出数据指针"*/
/*"  "*/
/*"  Return:         ret 转换数据长度，ret=0xff 数据转换错误"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define SAVE_TO_SAVE    0
#define P698_TO_SAVE      1
#define SAVE_TO_P698      2

typedef struct
{
INT8U  year;
INT8U  month;
INT8U  day;
INT8U  id;
}ST_HOLIDAY_SAVE;

typedef struct
{
INT16U  year;
INT8U  month;
INT8U  day;
INT8U  week;
INT8U  id;
}ST_HOLIDAY_698;

static INT8U TransFromTime(INT8U *src_data, INT8U *dest_data,
        ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat)
{
    INT8U ret,i;
    INT8U L, SUB;
    INT16U temp;
    ST_6TIME_FORM time;
    INT8U   dir;/*"0 save to save   1  698 to save    2  save to 698"*/
    INT8U shift;

    INT8U Tmp_outFormat;
    INT8U Tmp_inFormat;
    
    Tmp_outFormat = outFormat.format;
    Tmp_inFormat  = inFormat.format;
        
    ret = 0;
    dir = SAVE_TO_SAVE;
    if((inFormat.unit != outFormat.unit) || (inFormat.exp != outFormat.exp))
    {
        return 0xff;
    }
    

    L = FRMT_sizeofFormat(Tmp_outFormat);
    SUB = 0;
    shift = 0;

    /*转成BCD格式*/
    if((E_TIME_FORMAT_BCD<=Tmp_inFormat)&&(E_STRING>Tmp_inFormat))
    {     
        LIB_BcdToCharNByte(src_data,L);
        if((E_NN_MMDD_42_BCD == Tmp_inFormat)||(E_NN_HHMM_42_BCD == Tmp_inFormat))
        {    
            for(i=0;i<42/3;i++)
            {    
                LIB_RvsSelf(&src_data[i*3],3);                        
            }  
        } 
        else
        {    
            LIB_RvsSelf(src_data,L);
        }
    }   
    
    if(E_YYMMDDHHMMSS == Tmp_inFormat)
    {
        switch(Tmp_outFormat)
        {
        case E_YYYYMMDDHHMMSS:
            dir = SAVE_TO_P698;
            break;
        case E_HHMMSS:
        case E_HHMMSS_BCD:    
            SUB = 3;
            break;
        case E_YYMMDDHHMM:
        case E_YYMMDDHHMM_BCD: 
        case E_YYMMDDHH_BCD:
            break;
        case E_YYMMDD:
        case E_YYMMDD_BCD:    
            break;
        case E_YYMMDDHHMMSS_BCD:
            break;
        default:
            ret = 0xff;
            break;
        }
    }
    else if(E_YYMMDDHHMMSSWW == Tmp_inFormat)
    {
        switch(Tmp_outFormat)
        {
        case E_YYYYMMDDHHMMSS:
            dir = SAVE_TO_P698;
            break;            
        #ifdef DLT698
        case E_WW_YYMMDD:
        case E_WW_YYMMDD_BCD:   
            SUB = 0;
            break; 
        case E_HHMMSS:
        case E_HHMMSS_BCD:    
            SUB = 3;
            break;    
        #else
        case E_WW_YYMMDD_BCD;    
            SUB = 0;
            break; 
        case E_HHMMSS:
        case E_HHMMSS_BCD:      
            SUB = 3;
            break;  
        #endif    
        case E_YYMMDDHHMMSS:
        case E_YYMMDDHHMMSS_BCD:    
        case E_YYMMDDHHMM:
        case E_YYMMDDHHMM_BCD:    
        case E_YYMMDD:
        case E_YYMMDD_BCD:    
            break;
        default:
            ret = 0xff;
            break;
        }
    }
    else if(E_YYMMDDHHMM == Tmp_inFormat)
    {
        switch(Tmp_outFormat)
        {
        case E_HHMM:
        case E_HHMM_BCD:    
            SUB = 3;
            break;
        case E_HHMMSS:
        case E_HHMMSS_BCD:    
            shift = 1;
            SUB = 3;
            break;    
        case E_YYMMDD:
        case E_YYMMDD_BCD:   
        case E_YYMMDDHHMM_BCD:    
            break;
        case E_YYYYMMDDHHMMSS:
        case E_YYYYMMDDHHMMSS_FF:
            dir = SAVE_TO_P698;
            shift = 1;
            break;
        default:
            ret = 0xff;
            break;
        }
    }
    #if 0
    else if(inFormat.format == YYYYMMDDHHMMSS)
    {
           dir = P698_TO_SAVE;
         if(YYMMDDHHMMSS != outFormat.format)
         {
            ret = 0xff;
         }
    }
    #endif
    else if(E_YYYYMMDDHHMM == Tmp_inFormat)
    {
           dir = P698_TO_SAVE;
        if(E_YYMMDDHHMM != Tmp_outFormat)
         {
            ret = 0xff;
         }
    }
    #if 0
    else if(inFormat.format == NN_YYYYMMDDWW)
    {
           dir = P698_TO_SAVE;
        if(NN_YYMMDD != outFormat.format)
         {
            ret = 0xff;
         }
    }
    #endif
    else if(E_NN_YYMMDD == Tmp_inFormat)
    { 
         if((E_NN_YYYYMMDDWW != Tmp_outFormat)&&(E_NN_YYMMDD_BCD !=Tmp_outFormat))
         {
            ret = 0xff;
         }

         if(E_NN_YYYYMMDDWW == Tmp_outFormat)
         {    
            dir = SAVE_TO_P698;
         }  
         else if(E_NN_YYMMDD_BCD == Tmp_outFormat)
         {
            if(0xff == *src_data)
            {
               LIB_MemSet(0, src_data, 4);     /*防止默认值为FF的情况，转BCD后变为55了*/
            }    
         }    
    }
    else if(E_DDHH == Tmp_inFormat)
    {
         if(E_DDHH_BCD != Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_DDHH_BCD == Tmp_inFormat)
    {
         if(E_DDHH != Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_YYMMDDHHMM_BCD == Tmp_inFormat)
    {
         if(E_YYMMDDHHMM != Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_MMDD == Tmp_inFormat)
    {
         if(E_NN_MMDD_42_BCD != Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_HHMM == Tmp_inFormat)
    {
         if(E_NN_HHMM_42_BCD != Tmp_outFormat)    /*不能判E_NN_HHMM_42，否则698格式转换后有问题*/
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_MMDD_42_BCD == Tmp_inFormat)
    {
         if((E_NN_MMDD != Tmp_outFormat)&&(E_NN_MMDD_42 != Tmp_outFormat))
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_HHMM_42_BCD == Tmp_inFormat)
    {
         if((E_NN_HHMM != Tmp_outFormat)&&(E_NN_HHMM_42 != Tmp_outFormat))
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_MMDD_42 == Tmp_inFormat)
    {
         if(E_NN_MMDD_42_BCD!= Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_HHMM_42 == Tmp_inFormat)
    {
         if(E_NN_HHMM_42_BCD != Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_YYMMDD_BCD == Tmp_inFormat)
    {
         dir = SAVE_TO_P698;
         if(E_NN_YYMMDD != Tmp_outFormat)
         {
            ret = 0xff;
         }            
    } 
    else if(E_NN_MMDD_BCD == Tmp_inFormat)
    {
         if(E_NN_MMDD != Tmp_outFormat)
         {
            ret = 0xff;
         }
    } 
    else if(E_NN_HHMM_BCD == Tmp_inFormat)
    {
         if((E_NN_HHMM != Tmp_outFormat)&&(E_NN_HHMM_42!=Tmp_outFormat))
         {
            ret = 0xff;
         }
    } 
    else if(E_MMDDHH == Tmp_inFormat)
    {
         if(E_MMDDHH_BCD!= Tmp_outFormat)
         {
             ret = 0xff;
         }   
    }    
    else
    {
        ret = 0xff;
    }

    if(0xff != ret)
    {
        ret = L;

        if(SAVE_TO_P698 == dir)
        {
            if((E_NN_YYMMDD == Tmp_inFormat)||(E_NN_YYMMDD_BCD == Tmp_inFormat))
            {
                ST_HOLIDAY_SAVE pHolidaySave;
                ST_HOLIDAY_698  pHoliday698;

                LIB_MemCpy(src_data, (INT8U *)&pHolidaySave, sizeof(ST_HOLIDAY_SAVE));

                LIB_MemSet(0x00, (INT8U *)&time, 6);
                time.day = pHolidaySave.day;
                time.month = pHolidaySave.month;
                time.year = pHolidaySave.year;
               if(TRUE != LIB_ChkDateTime(&time))
                {/*"节假日数据非法时，返回0xFF    2017.12.15"*/
                    LIB_MemSet(0xFF, dest_data, sizeof(ST_HOLIDAY_698));
                }
                else
                {
                    pHoliday698.week = LIB_CalWeek(&time);
                    pHoliday698.id = pHolidaySave.id;
                    pHoliday698.day = pHolidaySave.day;
                    pHoliday698.month = pHolidaySave.month;
                    pHoliday698.year = (INT16U) (pHolidaySave.year) + 2000;

                    LIB_MemCpy((INT8U * )&pHoliday698, dest_data, sizeof(ST_HOLIDAY_698));
                }
                
                ret = 6;  /*改成长度为6，否则底层判断会有问题*/
                 
            }
            else
            {
                if(L >= 2)
                {
                    LIB_MemCpy(src_data + 1, dest_data + 2, L - 2);
                    if (EQUAL != LIB_ChkValue(src_data, (L - 2), 0x00))
                    {
                        temp = (INT16U) (*src_data) + 2000;
                        LIB_MemCpy((INT8U *) &temp, dest_data, 2);
                    }
                    else
                    {
                        LIB_MemSet(0, dest_data, 2);
                    }
                    if (1 == shift)
                    {
                        if(E_YYYYMMDDHHMMSS_FF != Tmp_outFormat)
                        {
                            *(dest_data + L - 1) = 0;
                        }
                        else
                        {
                            *(dest_data + L - 1) = 0xff;
                        }

                    }
                }
            }
        }
        else if(P698_TO_SAVE == dir)
        {
            if(E_NN_YYYYMMDDWW == Tmp_inFormat)
            {
#if 0
                ST_HOLIDAY_SAVE *pHolidaySave;
                ST_HOLIDAY_698 *pHoliday698;

                pHolidaySave = (ST_HOLIDAY_SAVE *)dest_data;
                pHoliday698 = (ST_HOLIDAY_698 *)src_data;

                pHolidaySave->id= pHoliday698->id;/*"NN"*/
                pHolidaySave->day = pHoliday698->day;
                pHolidaySave->month = pHoliday698->month;
                pHolidaySave->year = (INT8U)(pHoliday698->year -2000);
#endif
            }
            else
            {
                LIB_MemCpy(src_data + 2, dest_data + 1, L - 1);
                LIB_MemCpy(src_data, (INT8U *)&temp, 2);
                temp %= 100;
                *(dest_data) = (INT8U)temp;
            }
        }
        else
        {
            LIB_MemCpy((src_data + SUB), dest_data, (L - shift));
            if(shift != 0)
            {
                *(dest_data + L - shift) = 0;
            }

            /*把周week放到前面*/
            if(((E_WW_YYMMDD_BCD == Tmp_outFormat)||(E_WW_YYMMDD == Tmp_outFormat)) &&
                (E_YYMMDDHHMMSSWW == Tmp_inFormat))
            {
                //LIB_MemCpy(dest_data, dest_data+1, 3);
                *(dest_data+3) = *(src_data+6);
            }    

            /*转成BCD格式*/
            if((E_TIME_FORMAT_BCD<=Tmp_outFormat)&&(E_STRING>Tmp_outFormat))
            {     
                LIB_CharToBcdNByte(dest_data,L);
                
                if((E_NN_MMDD_42_BCD == Tmp_outFormat)||(E_NN_HHMM_42_BCD == Tmp_outFormat))
                {    
                    for(i=0;i<42/3;i++)
                    {    
                        LIB_RvsSelf(&dest_data[i*3],3);                        
                    }  
                }        
                else
                {    
                    LIB_RvsSelf(dest_data,L);
                }
            }     

        }
    }

    return ret;
}
/*"*****************************************************************************"*/
/*"  Function:      TransFromChar"*/
/*"  Description:   对字符串数据转换处理，主要是进行字符串长度截取"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          src_data  输入数据指针"*/
/*"                  inFormat.format 输入数据格式，outFormat.format输出数据格式"*/
/*"                  inFormat.unit 输入数据单位，outFormat.unit输出数据单位"*/
/*"                  inFormat.exp 输入小数位数， outFormat.exp输出数据小数位数"*/
/*"  Output:         dest_data 输出数据指针"*/
/*"  "*/
/*"  Return:         ret 转换数据长度，ret=0xff 数据转换错误"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U TransFromChar(INT8U *src_data,INT8U *dest_data,
                      ST_FMT_ATTR inFormat,ST_FMT_ATTR outFormat)
{
    INT8U scrshift;
    INT8U len;

    if((inFormat.unit != outFormat.unit) || (inFormat.exp != outFormat.exp))
    {
        return 0xff;
    }

    len = FRMT_sizeofFormat(outFormat.format);
    scrshift = 0;
    switch(inFormat.format)
    {
    case E_U_6_BYTE:
        if(outFormat.format == E_ID_CODE_2_FORMAT)
        {
            scrshift = 4;
        }
        else if(outFormat.format == E_ID_CODE_4_FORMAT)
        {
            scrshift = 0;
        }
        else
        {
            len = 0xff;
        }
        break;
    case E_ID_CODE_2_FORMAT:
    case E_ID_CODE_4_FORMAT:
        if(outFormat.format == E_U_6_BYTE)
        {
            len = FRMT_sizeofFormat(E_U_6_BYTE);
        }
        else
        {
            len = 0xff;
        }
        break;
    default:
        len = 0xff;
        break;
    }

    if(len != 0xff)
    {
        LIB_MemCpy((src_data + scrshift), dest_data, len);
    }

    return len;
}
/*"*****************************************************************************"*/
/*"  Function:      TransFromDemand"*/
/*"  Description:   需量及发生时间数据格式转换，转换依据1、输入输出数据格式；"*/
/*"                 2、输入输出数据单位；3、输入输出数据小数位数"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          src_data  输入数据指针"*/
/*"                  inFormat.format 输入数据格式，outFormat.format输出数据格式"*/
/*"                  inFormat.unit 输入数据单位，outFormat.unit输出数据单位"*/
/*"                  inFormat.exp 输入小数位数， outFormat.exp输出数据小数位数"*/
/*"  Output:         dest_data 输出数据指针"*/
/*"  "*/
/*"  Return:         ret 转换数据长度，ret=0xff 数据转换错误"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#ifdef THREE_PHASE
static INT8U TransFromDemand(INT8U *src_data, INT8U *dest_data,
        ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat)
{
    INT8U ret;
    ST_FMT_ATTR inFormatTemp;
    ST_FMT_ATTR outFormatTemp;
    INT8U Cnt;

    inFormatTemp = inFormat;
    ret = 0xff;

    if(outFormat.format < E_TIME_FORMAT)/*"输出格式非时间格式"*/
    {
        inFormatTemp.format = E_S_4_BYTE;
        inFormatTemp.exp = DEMAND_DECIMAL_POINT;   /*(Oct 15, 2018,15:14:38)改成-6*/
        inFormatTemp.unit = outFormat.unit;
        
        ret = TransFromHex(src_data, dest_data, inFormatTemp, outFormat);
    }
    else if((outFormat.format > E_TIME_FORMAT) && (outFormat.format < E_STRING))/*"输出格式为时间格式"*/
    {
        inFormatTemp.format = E_YYMMDDHHMM;
        ret = TransFromTime(src_data + 4, dest_data, inFormatTemp, outFormat);
    }
    else if(outFormat.format == E_DEMAND_AND_TIME_698)
    {

        //LIB_MemCpy(src_data ,dest_data, 4);
        /*(Oct 15, 2018,15:1:42)增加需量格式转换*/
        inFormatTemp.format = E_S_4_BYTE;
        inFormatTemp.exp = DEMAND_DECIMAL_POINT;
        inFormatTemp.unit = E_NONE_UNIT;
        
        outFormatTemp.format = E_S_4_BYTE;
        outFormatTemp.exp = -4;
        outFormatTemp.unit = E_NONE_UNIT; 
        TransFromHex(src_data, dest_data, inFormatTemp, outFormatTemp);        

        inFormatTemp = inFormat;
        inFormatTemp.format = E_YYMMDDHHMM;
        outFormatTemp = outFormat;
        outFormatTemp.format = E_YYYYMMDDHHMMSS;
        TransFromTime(src_data + 4, dest_data + 4, inFormatTemp, outFormatTemp);
        ret = FRMT_sizeofFormat(outFormat.format);
    }
    else if(outFormat.format == E_DEMAND_AND_TIME_645)
    {
        inFormatTemp.format = E_S_4_BYTE;
        inFormatTemp.exp = -6;
        inFormatTemp.unit = E_NONE_UNIT;

        outFormatTemp.format = E_BCD_3_BYTE;
        outFormatTemp.exp = -4;
        outFormatTemp.unit = E_NONE_UNIT; 
        ret = TransFromHex(src_data, dest_data, inFormatTemp, outFormatTemp);
        for(Cnt=0;Cnt<5;Cnt++)
        {
            *(dest_data +3+Cnt) = LIB_CharToBcd(*(src_data+4+Cnt));
        }
        LIB_RvsSelf(dest_data+3, 5);
        ret = FRMT_sizeofFormat(outFormat.format);
    }
    else
    {
    }
    return ret;
}
#endif
/*"***************************************************************"*/
/*"  Function:      FMT_sizeofFormat "*/
/*"  Description:  计算指定数据格式数据长度"*/
/*"  Input:        format: 数据格式 "*/
/*"                    "*/
/*"  outUnit:       无"*/
/*"  Return:      数据长度 "*/
/*"****************************************************************"*/
const INT8U frmtLen[] =
    { 0xff, /*"E_BCDFORMAT"*/
    1, /*"E_BCD_1_BYTE 1字节有符号BCD码"*/
    2, /*"E_BCD_3_BYTE 2字节有符号BCD码"*/
    3, /*"E_BCD_3_BYTE 3字节有符号BCD码"*/
    4, /*"E_BCD_4_BYTE 4字节有符号BCD码"*/
    4,/*"E_ENGY_H_INT"*/
    4,/*"E_ENGY_H_DECIMAL"*/

    0xff, /*"E_UBCDFORMAT"*/
    1, /*"E_UBCD_1_BYTE 1字节无符号BCD码"*/
    2, /*"E_UBCD_2_BYTE 2字节无符号BCD码"*/
    3, /*"E_UBCD_3_BYTE 3字节无符号BCD码"*/
    4, /*"E_UBCD_4_BYTE 4字节无符号BCD码"*/
    5, /*"E_UBCD_5_BYTE 5字节无符号BCD码"*/
    4,/*"E_UENGY_H_INT"*/
    4,/*"E_UENGY_H_DECIMAL"*/
    0xff, /*"E_HEX_FORMAT"*/

    ENERGY_SIZE, /*"E_S_ENGY_X_BYTE"*/
    8, /*"E_S_ENGY_8_BYTE"*/
    4, /*"E_S_4_BYTE"*/
    2, /*"E_S_2_BYTE"*/
    1, /*"E_S_1_BYTE"*/
    ENERGY_SIZE, /*"E_U_ENGY_X_BYTE"*/
    8, /*"E_U_ENGY_8_BYTE"*/
    4, /*"E_U_4_BYTE"*/
    2, /*"E_U_2_BYTE"*/
    1, /*"E_U_1_BYTE"*/

    0xff, /*"E_TIME_FORMAT"*/
    3, /*"E_YYMMDD"*/
    3, /*"E_HHMMSS"*/
    5, /*"E_YYMMDDHHMM"*/
    4, /*"E_YYMMDDHH"*/
    4, /*"E_MMDDHHMM"*/
    6, /*"E_YYMMDDHHMMSS"*/
    7, /*"E_YYYYMMDDHHMMSS"*/
    7, /*"E_YYYYMMDDHHMMSS_FF"*/
    6, /*"E_YYYYMMDDHHMM"*/
    7, /*"E_YYMMDDHHMMSSWW"*/
    7, /*"E_YYMMDDWWHHMMSS"*/
    2, /*"E_MMDD"*/
    3, /*"E_MMDDHH"*/
    2, /*"E_HHMM"*/
    2, /*"E_DDHH"*/
    3, /*"E_NN_MMDD时段表号  月日      年时区参数"*/
    3, /*"E_NN_HHMM费率号       时分       时段表参数"*/
    4, /*"E_NN_YYMMDD时段表号  年月日   公共节假日参数"*/
    4, /*"E_WW_YYMMDD周  年月日"*/
    6, /*"E_NN_YYYYMMDDWW"*/
    42, /*"E_NN_HHMM_42"*/
    42, /*"E_NN_MMDD_42"*/
    
    0xff,//E_TIME_FORMAT_BCD, /*"时间格式数据类型BCD定义"*/
    2, //E_DDHH_BCD,           /*BCD格式的时间 0*/
    2, //E_HHMM_BCD,           /*BCD格式的时间 1*/
    3, //E_NN_MMDD_BCD,        /*BCD格式的时间 2*/
    3, //E_NN_HHMM_BCD,        /*BCD格式的时间 2*/
    3, //E_HHMMSS_BCD,         /*BCD格式的时间 3*/
    3, //E_YYMMDD_BCD,         /*BCD格式的时间 4*/
    3, //E_MMDDHH_BCD,         /*BCD格式的时间 5*/
    4, //E_MMDDHHMM_BCD,       /*BCD格式的时间 6*/   
    4, //E_YYMMDDHH_BCD,       /*BCD格式的时间 7*/    
    4, //E_WW_YYMMDD_BCD,      /*BCD格式的时间 8*/
    5, //E_YYMMDDHHMM_BCD,     /*BCD格式的时间 9*/
    6, //E_YYMMDDHHMMSS_BCD,   /*BCD格式的时间 10*/
    4, //E_NN_YYMMDD_BCD,      /*BCD格式的时间 11*/
    42, //E_NN_HHMM_42_BCD,    /*BCD格式的时间 12 时段表*/ 
    42, //E_NN_MMDD_42_BCD,    /*BCD格式的时间 13 时区表*/ 
    
    0xff, /*"E_STRING字符串"*/
    5, /*"E_U_5_BYTE"*/
    6, /*"E_U_6_BYTE"*/
    8, /*"E_U_8_BYTE"*/
    12, /*"E_U_12_BYTE"*/
    16,/*"E_U_16_BYTE"*/
    5, /*"E_SHOW_CODE_5_BYTE"*/
    9, /*"E_SHOW_CODE_9_BYTE"*/
    2, /*"E_ASCII_2_LENGTH"*/
    4, /*"E_ASCII_4_LENGTH"*/
    6, /*"E_ASCII_6_LENGTH 6字节ASCII码"*/
    8, /*"E_ASCII_8_LENGTH 8字节ASCII码"*/
    10, /*"E_ASCII_10_LENGTH 10字节ASCII码"*/
    16, /*"E_ASCII_16_LENGTH 16字节ASCII码"*/
    20, /*"E_ASCII_16_LENGTH 20字节ASCII码"*/
    32, /*"E_ASCII_32_LENGTH 32字节ASCII码"*/

#ifdef THREE_PHASE
            5, /*"E_DLT_645_07_ID 07数据标识显示格式,ID字节间用小数点隔开"*/
            5, /*"E_USERID_SHOW"*/
            5, /*"E_DLT645_07ID 07数据标识显示格式,不带小数点格式"*/
#endif
            2, /*"E_ID_CODE_2_FORMAT"*/
            4, /*"E_ID_CODE_4_FORMAT"*/
#ifdef THREE_PHASE
            0xff, /*"E_RMB_MAUTO"*/
#endif
            0xff, /*"E_DATUO"*/
            0xff, /*"E_MATUO"*/
#ifdef THREE_PHASE
            9, /*"E_DEMAND_AND_TIME"*/
            11,/*"E_DEMAND_AND_TIME_698"*/
            8,/*"E_DEMAND_AND_TIME_645"*/
#endif
            40 /*"E_DLT645_10_ID10个645标识"*/
};
INT8U FRMT_sizeofFormat(INT8U format)
{
    INT8U len;

    /*长度信息配置和格式信息不一致*/
    BUILD_BUG_ON(E_DATA_TYPE_MAX != sizeof(frmtLen)); 

    len = 0xff;
    if(format < E_DATA_TYPE_MAX)
    {
        len = frmtLen[format];
    }

    return len;
}
/*"*****************************************************************************"*/
/*"  Function:       tenSeq"*/
/*"  Description:    10的seq次方 |seq|<9"*/
/*"  Input:          seq 10的指数"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         10的指数数据"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT32S tenSeq(INT8S seq)
{
    INT8U i, temp;
    INT32S l_temp;
    INT32S rsult;

    if(seq < 0)
    {
        temp =(INT8U) (0 - seq);
    }
    else
    {
        temp = (INT8U)seq;
    }

    if(temp > 9)/*"溢出报错"*/
    {
        return 0;
    }
    if(temp == 0)/*"指数没有变化"*/
    {
        return 1;
    }

    l_temp = 10;
    for(i = 1; i < temp; i++) /*"1不乘，2乘2次，类推"*/
    {
        l_temp *= 10;
    }

    if(seq < 0)
    {
        rsult = 0 - l_temp;
    }
    else
    {
        rsult = l_temp;
    }

    return  rsult;
}

/*"*****************************************************************************"*/
/*"  Function:      ChkUnitValid"*/
/*"  Description:   根据输入、输出单位，判断单位转换是否合法"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块"*/
/*"  Input:          inUint 输入数据单位"*/
/*"                  outUnit 输出数据单位"*/
/*"  outUnit:         "*/
/*"  "*/
/*"  Return:         TRUE 数据单位转换合法   FALSE 数据单位转换非法"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#if 1
INT8U ChkUnitValid(INT8U inUint, INT8U outUnit)
{
    INT8U rsult;

    rsult = TRUE;
    switch(inUint)
    {
    //#ifdef THREE_PHASE
    case E_KWH_UNIT:
    case E_WH_UNIT:
        if((outUnit != E_WH_UNIT) && (outUnit != E_KWH_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_KVARH_UNIT:
    case E_VARH_UNIT:
        if((outUnit != E_VARH_UNIT) && (outUnit != E_KVARH_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_KW_UNIT:
    case E_W_UNIT:
        if((outUnit != E_W_UNIT) && (outUnit != E_KW_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_KVAR_UNIT:
    case E_VAR_UNIT:
        if((outUnit != E_VAR_UNIT) && (outUnit != E_KVAR_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_KV_UNIT:
    case E_V_UNIT:
        if((outUnit != E_V_UNIT) && (outUnit != E_KV_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_KA_UNIT:
    case E_A_UNIT:
        if((outUnit != E_A_UNIT) && (outUnit != E_KA_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_YUAN_UNIT:
    case E_WAN_YUAN_UNIT:
        if((outUnit != E_YUAN_UNIT) && (outUnit != E_WAN_YUAN_UNIT))
        {
            rsult = FALSE;
        }
        break;
    //#endif    
    case E_SECOND_UNIT:
    case E_MINUTE_UNIT:
    case E_HOUR_UNIT:
    case E_DAY_UNIT:
        if((outUnit != E_MINUTE_UNIT) && (outUnit != E_HOUR_UNIT)
                && (outUnit != E_DAY_UNIT) && (outUnit != E_SECOND_UNIT))
        {
            rsult = FALSE;
        }
        break;
    case E_PER_UNIT:
        if(outUnit != E_PER_UNIT)
        {
            rsult = FALSE;
        }
        break;
    default:
        rsult = FALSE;
        break;
    }

    return rsult;
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       searchUnit"*/
/*"  Description:    查找输入、输出单位对应的倍数"*/
/*"  Input:          unit  输入单位"*/
/*"  Output:         "*/
/*"  "*/
/*"  Return:         单位对于的倍数"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
//#ifdef THREE_PHASE
INT32U searchUnit(INT8U unit)
{
  INT32U value;

  value = 1;
  switch(unit)
  {
    //#ifdef THREE_PHASE
    case E_WH_UNIT:
    case E_VARH_UNIT:  
    case E_W_UNIT:
    case E_VAR_UNIT:
    case E_V_UNIT:
    case E_A_UNIT:
    case E_SECOND_UNIT:
    case E_PER_UNIT:
        value=1;
        break;
    case E_KWH_UNIT:
    case E_KVARH_UNIT :
    case E_KW_UNIT:
    case E_KVAR_UNIT:
    case E_KV_UNIT:
    case E_KA_UNIT:
        value=1000;
        break;
    //#endif    
    case E_MINUTE_UNIT:
        value=60;
        break;
    case E_HOUR_UNIT:
        value=3600;
        break;
    case E_DAY_UNIT:
        value=86400;
        break;
    default:
        break;
  }
 return value;
}
//#endif
