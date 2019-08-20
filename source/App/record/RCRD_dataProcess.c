/*"*****************************************************************************"*/
/*"  FileName: RCRD_dataProcess.c"*/
/*"  Description:     该文件提供记录快照模块对外对象访问接口"*/
/*"  Author: quantp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.09.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      quantp    2017/9/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/

#include "system\Sys_def.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"
#include "App\message\FRMT_def.h"
#include "App\LegallyRelevant\LRM_Config.h"
#include "app\userinterface\PP\PP_698\Class\class_comm.h"
#include "app\userinterface\PP\PP_698\Process\Pp_obis.h"
#include "app\userinterface\PP\PP_698\Process\Pp_macro.h"
#include "app\userinterface\PP\PP_698\Process\Pp_process.h"
#include "App\UserInterface\PP\PP_645\PP_Offset.h"
#include "RCRD.h"
#include "LR.h"
#include "DataCenterSoftware\Code\record\RCRD_data.h"
#include "RCRD_dataProcess.h"

#define RCRD_START_TM_OFF   0   /*" 记录发生时间存储在偏移1处 "*/
#define ARRAY_BLK_OBIS      0x80

INT8U GetRcrdCurtCnt(EN_RCRD_FILE_ID rcrd_obis, INT32U *cnt);
INT32U GetRcrdMaxtCnt(EN_RCRD_FILE_ID rcrd_obis);
static INT16U  CalDrvdObjectLen(INT16U obis, INT8U leaf, INT16U *poffset, INT8U *pformat);



#define FRZ_MAX_NUM         0x12
const INT8U frz_max_num[FRZ_MAX_NUM] =
{
    NUM_MMT_FRZ,/*0 瞬时冻结*/
    0xff,       /*2  分冻结*/
    NUM_HOUR_FRZ,/*3 小时冻结（整点冻结） */
    NUM_DAY_FRZ,/*4 日冻结 */
    NUM_ENGY_BILL,/*5 月结算 */
    NUM_MON_FRZ,/*6 月冻结 */
    NUM_YSLOT_SW_FRZ,/*8  时区表切换冻结*/
    NUM_DSLOT_SW_FRZ,/*9  日时段表切换冻结*/
    NUM_TARIFF_SW_FRZ,/*A  费率电价切换冻结*/
    NUM_STEP_SW_FRZ,/*B  阶梯切换冻结*/
    NUM_YEAR_BILL, /* 11 阶梯结算冻结  */
};

#if defined(THREE_PHASE)
static INT32U amp_in_evt[4]; /*" 当前事件安时值 "*/
static EN_RCRD_FILE_ID cur_save_rcrd;
static EN_FELXOBJ_ID cur_data_obis;
static INT32U CurRdEvtCnt; /*" 当前读取事件的发生次数 "*/
static INT16U evt_st_flg; /*" 事件上电后发生过的标志 "*/
INT32U lst_pd_amp[3];
#endif

ST_RCRD_LINE_RSULT_DATA  RsultAttr;


INT8U is_show_data; /*" 是显示抄读取数 "*/
INT8U meter_type;
static INT8U cur_triff_num; /*" 当前费率数 "*/


INT8U is_private_rcrd(EN_RCRD_FILE_ID rcrd_obis)
{
#if defined(THREE_PHASE)    
    if(rcrd_obis == E_SYS_RST_RCRD || rcrd_obis == E_OSCILLATOR_ERR_RCRD
        || rcrd_obis == E_PARA_ERR_RCRD || rcrd_obis == E_ENG_ERR_RCRD
        || rcrd_obis == E_PD_PON_RCRD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#else
    if(rcrd_obis == E_SYS_RST_RCRD || rcrd_obis == E_OSCILLATOR_ERR_RCRD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
#endif
}

INT8U rcrd_id_is_error(EN_RCRD_FILE_ID rcrd_obis)
{
    if(rcrd_obis < E_PWR_DOWN_EVNT_RCRD || rcrd_obis > E_STEP_BILL_FRZ_RCRD
        || rcrd_obis == E_EVT_END_RCRD)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*" 获取当前系统存储电能的费率数 "*/
INT8U get_cur_eng_triff_num(void)
{
    INT8U triff_num;
    INT16S res;
    
    res = GetSingle(E_TARIFF_NUM, (INT8U *)&triff_num);
    if(res < MS_SUCCESS)
    {
        triff_num = 4;
    }
    else
    {
        if(triff_num > TARIFF_NUM)
        {
            triff_num = TARIFF_NUM;
        }
    }
    return triff_num;
}

/*"***************************************************************"*/
/*"  Function:       get_data_addr_off                              "*/
/*"  Description:   计算对应记录在此类记录数据空间偏移              "*/
/*"  Input:         num_per_sec 一个扇区存放记录条数                "*/
/*"                 sum_sector 总扇区数 per_sz 一条记录实际大小 seq_no 记录序号 "*/
/*"  Output:         "*/
/*"                 "*/
/*"  Return:         返回对应记录在此类记录数据空间偏移 "*/
/*"****************************************************************"*/
INT32U get_data_addr_off(INT16U num_per_sec, INT16U sum_sector, INT16U per_sz, INT32U seq_no)
{
    INT32U max_entry;
    INT32U addr;
    INT32U pos;
    
    max_entry = (INT32U)num_per_sec * (INT32U)sum_sector;
    pos = seq_no % max_entry;
    addr = (pos / num_per_sec) * FLASH_SECTOR_SZ + (pos % num_per_sec) * per_sz;
    return addr;
}

INT32U RCRD_get_data_addr_off(ST_FILE_ATTR *FileAttri, INT16U per_sz, INT32U seq_no)
{
    INT32U addr;
    INT32U file_num;
    INT16U num_per_sec;
    INT16U sum_sector;

    file_num = FileAttri->fileSize / per_sz;

    if(FileAttri->file < E_FILE_FLASH_START)
    {/*" EEPROM "*/
        addr = (INT16U) (seq_no % file_num) * per_sz;
    }
    else
    {/*" FLASH "*/
        num_per_sec = FLASH_SECTOR_SZ / per_sz;
        sum_sector = FileAttri->fileSize / FLASH_SECTOR_SZ;
        addr = get_data_addr_off(num_per_sec, sum_sector, per_sz, seq_no);
    }
    
    return addr;
}

/*"*****************************************************************************"*/
/*"  Function:       get_single_obis_file_off_len"*/
/*"  Description:    计算单个obis的文件偏移以及长度 "*/
/*"  Calls:          "*/
/*"  Called By:      OADToLeaf"*/
/*"  Input:          obis      stRcrdSingleDataList的第一个成员 "*/
/*"                    OAD    关联对象标识 "*/
/*"                    sOre     查找事件起始or结束成员号标志"*/
/*"                            成员号分起始和结束，以事件记录实际存储顺序为准"*/
/*"                            leaf < 0x80 表示起始记录对象数据成员      leaf > 0x80表示结束记录对象成员"*/
/*"  Output:         "*/
/*"  Return:         leaf       关联OAD对应成员号"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U get_single_obis_file_off_len(INT16U obis, ENUM_FS_ID *file, INT16U *off, INT16U *len)
{
    INT16U start_obis;
    INT16U idx;
    INT8U res;
    
    start_obis = stRcrdSingleDataList[0].obis;
    if(obis >= start_obis)
    {
        idx = obis - start_obis;
        *file = (ENUM_FS_ID)(stRcrdSingleDataList[idx].addr >> 16);
        *off = stRcrdSingleDataList[idx].addr & 0xffff;
        *len = stRcrdSingleDataList[idx].len;
        res = TRUE;
    }
    else
    {
        *len = 0;
        res = FALSE;
    }
    return res;
}

/*"*****************************************************************************"*/
/*"  Function:       Get_Evt_Road "*/
/*"  Description:    获取事件的关联对象 "*/
/*"  Calls:          "*/
/*"  Called By:      OADToLeaf"*/
/*"  Input:          obis      事件关联对象的OBIS "*/
/*"                  buf    保存用的buf  "*/
/*"                  num    读取的ROAD个数 "*/
/*"  Output:         "*/
/*"  Return:         成功返回TRUE, 失败返回FALSE "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Get_Evt_Road(INT16U obis, INT8U *buf, INT8U num)
{
    ENUM_FS_ID file;
    INT16U off;
    INT16U len;
    INT8U res;

    res = get_single_obis_file_off_len(obis, &file, &off, &len);
    if(res == TRUE)
    {
        res = FS_ReadFile(file, off, buf, len);
    }
    return res;
}

INT8U Get_Frz_Road(INT16U obis, INT8U *buf, INT8U num)
{
    ST_ROAD_FRZ_INFO ObjListFrzPara[MAX_RCRD_ROAD_NUM];
    INT8U cnt;
    INT8U res;
    ENUM_FS_ID file;
    INT16U off;
    INT16U len;

    LIB_MemSet(0x00, (INT8U *)&ObjListFrzPara[0], sizeof(ObjListFrzPara));
    res = get_single_obis_file_off_len(obis, &file, &off, &len);
    if(res == TRUE)
    {
        res = FS_ReadFile(file, off, (INT8U *)ObjListFrzPara, len);
    }
   
    for(cnt=0;cnt<num;cnt++)
    {
        LIB_MemCpy((INT8U *)&ObjListFrzPara[cnt].FrzRoad, 
                    buf+cnt*4, sizeof(ST_OAD));
    }
    return res;
}

INT8U AidType_is_after_evt_end(INT8U aid)
{
    INT8U dataType = (INT8U)(aid >> 5);
    return (dataType == 4);
}

INT8U AidType_is_evt_end(INT8U aid)
{
    INT8U dataType = (INT8U)(aid >> 5);
    return ((dataType == 2) || (dataType == 3) || (dataType == 4));
}

INT8U AidType_is_evt_start(INT8U aid)
{
    INT8U dataType = (INT8U)(aid >> 5);
    return ((dataType == 0) || (dataType == 1));
}

INT8U is_local_meter_oi(INT16U oi)
{
    if((oi>=0x201A && oi<=0x201C) || (oi>=0x202c && oi<=0x202e) || (oi==0x2032)
        || (oi>=0x400A && oi<=0x400B)  || (oi==0x400d)
        || (oi>=0x4018 && oi<=0x401B)  || (oi>=0x401E && oi<=0x401F))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

#if defined(THREE_PHASE)
INT8U is_max_demand_oad(ST_OAD *oad)
{
    INT8U OIA1;
    INT8U OIB2;
    
    OIA1 = oad->Oi.asByte[OIH] >> 4;
    OIB2 = oad->Oi.asByte[OIL] & 0xf;
    if(OIA1 == 1 && OIB2 == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*" 清除无功组合方式编程事件发生源相关数据"*/
void clr_r_eng_com_prg_evt_data(void)
{
    ST_REngComPrgData tmp;
    
    LIB_MemSet(0x00, (INT8U *)&tmp, sizeof(ST_REngComPrgData));
    FS_WriteFile(E_FILE_RCRD_DATA, R_ENG_COM_PRG_DATA_BASE, (INT8U *)&tmp, sizeof(ST_REngComPrgData));
}

void poweron_init_evt_amp(void)
{
    evt_st_flg = 0;
    FS_ReadFile(E_FILE_RCRD_DATA, PD_AMP_HOUR_BASE, (INT8U *)&lst_pd_amp[0], sizeof(lst_pd_amp));
}

void upd_evt_amp_eeprom(void)
{
    INT32U amp[3];

    if((FALSE == ChkStatus(E_MTR_INIT)) && (evt_st_flg != 0))
    {
        GetSingle(E_AH_BLK, (INT8U *)&amp[0]);
        FS_WriteFile(E_FILE_RCRD_DATA, PD_AMP_HOUR_BASE, (INT8U *)&amp[0], sizeof(amp));
    }
}

/*" 获取当前事件安时值 "*/
void RCRD_CalcCurEvtAmp(EN_RCRD_FILE_ID rcrd_obis)
{
    INT32U amp[3];
    INT32U st_amp[3];
    INT16U off;
    INT8U i;
    
    if(rcrd_obis >= E_LOST_VOLT_LA_EVNT_RCRD && rcrd_obis <= E_BRK_PHASE_LC_EVNT_RCRD)
    {/*" 期间安时值 "*/
        /*"  获取当前安时值到amp "*/
        GetSingle(E_AH_BLK, (INT8U *)&amp[0]);
        if((evt_st_flg & (1L << (rcrd_obis - E_LOST_VOLT_LA_EVNT_RCRD))) == 0)
        {
            /*" 处于初始化阶段 ， 失压欠压过压断相事件上电结束 "*/
            for(i=0; i<3; i++)
            {
                amp[i] += lst_pd_amp[i];
            }
        }
        off = EVT_BEGIN_AMP_HOUR_BASE + (rcrd_obis - E_LOST_VOLT_LA_EVNT_RCRD)*3*4;
        FS_ReadFile(E_FILE_RCRD_DATA, off, (INT8U*)&st_amp[0], sizeof(st_amp));

        amp_in_evt[0] = 0;
        for(i=0; i<3; i++)
        {
            amp_in_evt[1 + i] = amp[i] - st_amp[i];
            amp_in_evt[0] += amp_in_evt[1 + i];
        }
    }    
    return;
}

INT8U get_amp_data_base_iid(INT8U * buf, INT8U iid)
{
    INT8U len = 0;
    
    if(iid == 0)
    {
        LIB_MemCpy((INT8U *)&amp_in_evt[0], buf, 16);
        len = 16;
    }
    else if(iid <= 4)
    {
        LIB_MemCpy((INT8U *)&amp_in_evt[iid - 1], buf, 4);
        len = 4;
    }
    return len;
}

void clr_lost_volt_stat_info(void)
{
    INT16U off;
    INT8U buff[12];
    
    off = LAST_LOST_VOLT_ST_TM_OFF;
    LIB_MemSet(0x00, buff, 12);
    FS_WriteFile(E_FILE_RCRD_DATA, off, buff, 12);    
}

/*" 存记录起始数据前处理 "*/
void RCRD_Prepare_StartData(EN_RCRD_FILE_ID rcrd_obis)
{
    INT32U amp[3];
    INT16U off;
    //INT8U buff[7];
    
    if(rcrd_obis >= E_LOST_VOLT_LA_EVNT_RCRD && rcrd_obis <= E_BRK_PHASE_LC_EVNT_RCRD)
    {/*" 期间安时值 "*/
        /*"  获取当前安时值到amp "*/
        evt_st_flg |= (1L << (rcrd_obis - E_LOST_VOLT_LA_EVNT_RCRD));
        GetSingle(E_AH_BLK, (INT8U *)&amp[0]);
        off = EVT_BEGIN_AMP_HOUR_BASE + (rcrd_obis - E_LOST_VOLT_LA_EVNT_RCRD)*3*4;
        FS_WriteFile(E_FILE_RCRD_DATA, off, (INT8U*)&amp[0], sizeof(amp));

        FS_WriteFile(E_FILE_RCRD_DATA, PD_AMP_HOUR_BASE, (INT8U *)&amp[0], sizeof(amp));
    }
    /*" 最近一次失压发生时间 "*/
    if(rcrd_obis >= E_LOST_VOLT_LA_EVNT_RCRD && rcrd_obis <= E_LOST_VOLT_LC_EVNT_RCRD)
    {
        //GetSingle(E_SYS_TIME, buff);
        off = LAST_LOST_VOLT_ST_TM_OFF;
        FS_WriteFile(E_FILE_RCRD_DATA, off, &DCDataBuf[0], 6);
    }
    
    return;
}

void RCRD_Prepare_EndData(EN_RCRD_FILE_ID rcrd_obis)
{
    INT16U off;
    //INT8U buff[7];
    
    RCRD_CalcCurEvtAmp(rcrd_obis); 
    if(rcrd_obis >= E_LOST_VOLT_LA_EVNT_RCRD && rcrd_obis <= E_BRK_PHASE_LC_EVNT_RCRD)
    {
        evt_st_flg &= ~(1L << (rcrd_obis - E_LOST_VOLT_LA_EVNT_RCRD));
    }
    /*" 最近一次失压结束时间 "*/
    if(rcrd_obis >= E_LOST_VOLT_LA_EVNT_RCRD && rcrd_obis <= E_LOST_VOLT_LC_EVNT_RCRD)
    {
        //GetSingle(E_SYS_TIME, buff);
        off = LAST_LOST_VOLT_END_TM_OFF;
        FS_WriteFile(E_FILE_RCRD_DATA, off, &DCDataBuf[6], 6);
    }
    return;
}

void refresh_cur_occur_evt_data(EN_RCRD_FILE_ID id, ST_OAD *cur_road, INT8U *buf)
{
    INT16U oi;
    INT8U aid;
    INT8U iid;
    INT16U obis;

    oi = cur_road->Oi.asInt;
    aid = cur_road->AId;
    iid = cur_road->IId;
    if(oi == 0x3308 && (iid == 6 || iid == 7))
    {/*" 需量超限期间最大需量、发生时间 "*/
        switch(id)
        {
        case E_OVER_PA_DEMAND_EVNT_RCRD:
            obis = (iid == 6)?E_OVER_ACTDMND:E_OVER_ACTDMNDTIME;
            break;
        case E_OVER_NA_DEMAND_EVNT_RCRD:
            obis = (iid == 6)?E_OVER_NEGACTDMND:E_OVER_NEGACTDMNDTIME;
            break; 
        case E_OVER_R_DEMAND_1_EVNT_RCRD:
            obis = (iid == 6)?E_OVER_PHS1RDMND:E_OVER_PHS1RDMNDTIME;
            break; 
        case E_OVER_R_DEMAND_2_EVNT_RCRD:
            obis = (iid == 6)?E_OVER_PHS2RDMND:E_OVER_PHS2RDMNDTIME;
            break; 
        case E_OVER_R_DEMAND_3_EVNT_RCRD:
            obis = (iid == 6)?E_OVER_PHS3RDMND:E_OVER_PHS3RDMNDTIME;
            break; 
        case E_OVER_R_DEMAND_4_EVNT_RCRD:
            obis = (iid == 6)?E_OVER_PHS4RDMND:E_OVER_PHS4RDMNDTIME;
            break;
        default:
            return;
        }
        GetSingle(obis, buf);
    }
    else if(oi == 0x2026 && aid == 0x62)
    {/*" 电压不平衡 "*/
        switch(id)
        {
        case E_VOLT_IMBALANCE_EVNT_RCRD:
            GetSingle(E_VOLTIMB_RATE, buf);
            break;
        }
    }
    else if(oi == 0x2027 && aid == 0x62)
    {/*" 电流不平衡 "*/
        switch(id)
        {
        case E_CURR_IMBALANCE_EVNT_RCRD:
            GetSingle(E_CURRIMB_RATE, buf);
            break; 
        case E_CURR_IMBALANCE_BADLY_EVNT_RCRD:
            GetSingle(E_BADLYCURRIMB_RATE, buf);
            break; 
        }
    }
    else if(oi == 0x2029 && aid == 0x62)
    {/*" 期间安时值 "*/
        if(id >= E_LOST_VOLT_LA_EVNT_RCRD && id <= E_BRK_PHASE_LC_EVNT_RCRD)
        {
            RCRD_CalcCurEvtAmp(id);
            get_amp_data_base_iid(buf, iid);
        }
    }   
}

#endif


/*"*****************************************************************************"*/
/*"  Function:       Get_Oad_Src_Data_Fmt_Len "*/
/*"  Description:    计算OAD原始存储数据长度格式信息 "*/
/*"  Input:          oad   OAD标识信息地址"*/
/*"  Output:         fmt_src  原始数据格式 "*/
/*"                  fmt_dst  目的数据格式 "*/
/*"                  per_sz   OAD对应数据如果为数组返回数组单个元素长度，否则返回0 "*/
/*"  Return:         0: 失败；非0: 返回数据长度 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Get_Oad_Src_Data_Fmt_Len(ST_OAD *oad, ST_FMT_ATTR *fmt_src, ST_FMT_ATTR *fmt_dst, INT8U *per_sz)
{
    INT8U numobis;
    INT16U obis_list[MAX_OBIS_NUM_ONE_OAD];
    INT16U DataOffset;
    INT8U len;
    INT8U i, lenFormat;
    ST_OAD cur_oad;
    INT8U OIA1;
    INT8U OIB2;
    INT8U step_sz[3] = {6, 7, 4};
    INT8U cur;

    len = 0;
    if(per_sz != NULL)
    {
        *per_sz = 0;
    }
    cur_oad.Oi.asInt = oad->Oi.asInt;
    cur_oad.AId = oad->AId & 0x1F;
    cur_oad.IId = oad->IId;

    if(meter_type != LOCALMETER)
    {
        if(is_local_meter_oi(cur_oad.Oi.asInt))
        {
            return 0;
        }
    }

#if defined(THREE_PHASE)
    /*"安时值处理"*/
    if(oad->Oi.asInt == 0x2029 && oad->AId == 0x62)
    {
        fmt_src->format = E_U_4_BYTE;
        fmt_src->unit = E_NONE_UNIT;
        fmt_src->exp = -2;
        LIB_MemCpy((INT8U *)fmt_src, (INT8U *)fmt_dst, sizeof(ST_FMT_ATTR));
        len = (oad->IId == 0)?16:4;
        return len;
    }
#endif
    
    numobis = PP_OadToObis(&cur_oad, &obis_list[0], &DataOffset, fmt_dst);
    if((0 != (numobis & 0x7f)) && (obis_list[0] < E_OBIS_NULL))
    {
        if(numobis & ARRAY_BLK_OBIS)
        {
            GetFrmt(obis_list[0], fmt_src);
            lenFormat = FRMT_sizeofFormat(fmt_src->format);
            
            OIA1 = oad->Oi.asByte[OIH] >> 4;
            OIB2 = oad->Oi.asByte[OIL] & 0xf;
            if(OIA1 <= 1 && OIB2 == 0 && oad->IId == 0) /*" 电量 与 需量 按最大费率存 "*/
            {
                len = lenFormat * (TARIFF_NUM + 1);
            }
            else
            {
                len = lenFormat * (numobis & 0x7f);
            }
            
            if(cur_oad.AId == 2 && cur_oad.IId == 0)
            {
                switch(cur_oad.Oi.asInt)
                {
                    case 0x4018:
                    case 0x4019:
                        /*" 费率表参数特殊处理，固定为128字节 "*/
                        len = 128;
                        break;
                    case 0x4014:
                    case 0x4015:
                        /*40152200--时区编程前备用套时区表*/
                        len = 42;
                        break;
                }
            }

            if(per_sz != NULL)
            {
                *per_sz = lenFormat;
            }
        }
        else
        {
            if(numobis > MAX_OBIS_NUM_ONE_OAD)
            {
                return 0;
            }
                        
            for(i=0; i<numobis; i++)
            {
                GetFrmt(obis_list[i], fmt_src); /*" TODO 如果一个OAD对应多个数据应该只有一个格式吧 "*/
                if(numobis > 1)
                {
                    LIB_MemCpy((INT8U *)fmt_src, (INT8U *)fmt_dst, sizeof(ST_FMT_ATTR)); /*" 一个OAD对应多个OBIS的无格式转换需求 "*/
                }
                if(cur_oad.Oi.asInt == 0x401A || cur_oad.Oi.asInt == 0x401B)
                {/*" 阶梯表参数特殊处理 "*/
                    LIB_MemCpy((INT8U *)fmt_src, (INT8U *)fmt_dst, sizeof(ST_FMT_ATTR));
                    if(cur_oad.IId == 0)
                    {
                        cur = step_sz[i];
                    }
                    else if(cur_oad.IId <= 3)
                    {
                        cur = step_sz[cur_oad.IId - 1];
                    }
                    else
                    {
                        cur = 0;
                    }
                    len += (FRMT_sizeofFormat(fmt_src->format)*cur);
                }
                else
                {
                    if( (cur_oad.Oi.asInt == 0xF100) || (cur_oad.Oi.asInt == 0xF205)
                        || (cur_oad.Oi.asInt == 0x4007) || (cur_oad.Oi.asInt == 0x400C)
                        || (cur_oad.Oi.asInt == 0x400D) || (cur_oad.Oi.asInt == 0x4010)
                        || (cur_oad.Oi.asInt == 0x4013) || (cur_oad.Oi.asInt == 0x4102)
                        || ((cur_oad.Oi.asInt >= 0x2131) && (cur_oad.Oi.asInt <= 0x2133)))
                    {/*" 目标通信格式无效，则不进行格式转换 "*/
                        LIB_MemCpy((INT8U *)fmt_src, (INT8U *)fmt_dst, sizeof(ST_FMT_ATTR));
                    }
                    len += FRMT_sizeofFormat(fmt_src->format);
                }
            }
        }
    }
    else
    {

    }
    return len;
}

/*"*****************************************************************************"*/
/*"  Function:       Get_Oad_Data_To_Buf "*/
/*"  Description:    获取OAD原始存储数据到指定buf "*/
/*"  Input:          oad   OAD标识信息地址"*/
/*"                  dst  目标buf  "*/
/*"                  dst_sz  目标buf空间长度  "*/
/*"  Output:         cur_sz  返回OAD对应原始数据长度 "*/
/*"  Return:         FALSE: 失败；TRUE: 成功 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U Get_Oad_Data_To_Buf(ST_OAD *oad, INT8U *dst, INT16U dst_sz, INT8U *cur_sz)
{
    INT8U BuffTemp[MAX_LEN];
    INT8U dataLen;
    INT16S rsult;
    ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    ST_FMT_ATTR fmt_src;
    INT8U cur_len;
    INT16U DataOffset;/*"关联对象在对象数据内偏移量"*/
    INT8U lensrc;
    INT8U cnt;
    INT8U numobis;
    INT16U obis_list[MAX_OBIS_NUM_ONE_OAD];
    ST_OAD cur_oad;
    INT8U OIA1;
    INT8U OIB2;


    dataLen = 0;
    cur_oad.Oi.asInt = oad->Oi.asInt;
    cur_oad.AId = oad->AId & 0x1F;
    cur_oad.IId = oad->IId;

    if(meter_type != LOCALMETER)
    {
        if(is_local_meter_oi(cur_oad.Oi.asInt))
        {
            *cur_sz = 0;
            return TRUE;
        }
    }

#if defined(THREE_PHASE)
    /*"安时值处理"*/
    if(oad->Oi.asInt == 0x2029 && oad->AId == 0x62)
    {
        cur_len = get_amp_data_base_iid(BuffTemp, oad->IId);
        if(((INT16U)dataLen + (INT16U)cur_len) > dst_sz)
        {
            *cur_sz = 0;
            return FALSE;
        }
        else
        {
            LIB_MemCpy(BuffTemp, &dst[dataLen], cur_len);
            *cur_sz = cur_len;
            return TRUE;
        }
    }
#endif
    numobis = PP_OadToObis(&cur_oad, &obis_list[0], &DataOffset, (ST_FMT_ATTR *)&fmt_dst);
#if defined(THREE_PHASE)
    if((cur_save_rcrd == E_ALL_LOST_VOLT_EVNT_RCRD) && (cur_data_obis == E_EVTLST_ALL_LOST_VOLT_S)
        && (cur_oad.Oi.asInt == 0x2001))
    {
        obis_list[0] = E_PDCURR_BLK + obis_list[0] - E_CURR_BLK;
    }
    else if((cur_save_rcrd == E_CURR_IMBALANCE_BADLY_EVNT_RCRD)
        && (cur_oad.Oi.asInt == 0x2027))
    {
        obis_list[0] = E_BADLYCURRIMB_RATE;
    }
        
#endif
    /*" QQQ 一个OAD会转换为多个OBIS不 ? 转换后每个的格式都一样　? "*/
    if((0 != (numobis & 0x7f)) && (obis_list[0] < E_OBIS_NULL))
    {
        if(numobis & ARRAY_BLK_OBIS)
        {
            GetFrmt(obis_list[0], &fmt_src);
            lensrc = FRMT_sizeofFormat(fmt_src.format);
            OIA1 = oad->Oi.asByte[OIH] >> 4;
            OIB2 = oad->Oi.asByte[OIL] & 0xf;
            if(OIA1 <= 1 && OIB2 == 0 && oad->IId == 0) /*" 电量 与 需量 按最大费率存 "*/
            {
                cur_len = lensrc * (TARIFF_NUM + 1);
                LIB_MemSet(0x00, BuffTemp, cur_len); 
                cur_len = 0;
            }
            
            rsult = GetSingle(obis_list[0], BuffTemp);
            if(rsult > MS_SUCCESS)
            {
                if(OIA1 <= 1 && OIB2 == 0 && oad->IId == 0) /*" 电量 与 需量 按最大费率存 "*/
                {
                    cur_len = lensrc * (TARIFF_NUM + 1);
                }
                else
                {
                    cur_len = lensrc * (numobis & 0x7F);
                }
            
                if(cur_oad.AId == 2 && cur_oad.IId == 0)
                {
                    switch(cur_oad.Oi.asInt)
                    {
                        case 0x4018:
                        case 0x4019:
                            /*" 费率表参数特殊处理，固定为128字节 "*/
                            cur_len = 128;
                            break;
                        case 0x4014:
                        case 0x4015:
                            /*40152200--时区编程前备用套时区表*/
                            cur_len = 42;
                            break;
                    }
                }
                            
                if(((INT16U)dataLen + (INT16U)cur_len) > dst_sz)
                {
                    *cur_sz = 0;
                    return FALSE;
                }

                //rsult = FRMT_DataChange(BuffTemp, &dst[dataLen], fmt_src, fmt_dst); /*" 存通信格式数据 "*/
                LIB_MemCpy(BuffTemp, &dst[dataLen], cur_len); /*" 存原始数据 "*/
#if defined(THREE_PHASE)
                if(is_2_3_mouth_bill && is_max_demand_oad(oad))
                {
                    LIB_MemSet(0xff, &dst[dataLen], cur_len);
                }
#endif
                dataLen += cur_len;
            }
            else
            {
                dataLen = 0;
            }
        }
        else
        {
            if(numobis > MAX_OBIS_NUM_ONE_OAD)
            {
                *cur_sz = 0;
                return TRUE;
            }
            for(cnt = 0; cnt < numobis; cnt++)
            {
                //GetFrmt(obis_list[cnt], &fmt_src);
                //lensrc = FRMT_sizeofFormat(fmt_src.format);
                rsult = GetSingle(obis_list[cnt], BuffTemp);

                if(rsult > MS_SUCCESS)
                {
                    //cur_len = lensrc; /*" 存原始数据 "*/
                    cur_len = (INT8U)rsult; /*" 以获取到的数据长度为准 "*/
                    if(((INT16U)dataLen + (INT16U)cur_len) > dst_sz)
                    {
                        *cur_sz = 0;
                        return FALSE;
                    }
                    //rsult = FRMT_DataChange(BuffTemp, &dst[dataLen], fmt_src, fmt_dst); /*" 存通信格式数据 "*/
                    LIB_MemCpy(BuffTemp, &dst[dataLen], cur_len); /*" 存原始数据 "*/
                    
                    dataLen += cur_len;
                } 
                else
                {
                    dataLen = 0;
                    break;
                }                            
            }
        }
    } 
    *cur_sz = dataLen;
    return TRUE;
}

INT8U is_same_fmt(ST_FMT_ATTR fmt_src, ST_FMT_ATTR fmt_dst)
{
    if((fmt_src.format == fmt_dst.format && fmt_src.exp == fmt_dst.exp && fmt_src.unit == fmt_dst.unit)
        ||(fmt_src.format == E_NN_MMDD || fmt_src.format == E_NN_HHMM_42 )) /*" 时区表 日时段表 不进行格式转换 "*/
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* 格式化编程对象列表*/
INT8U fmt_prog_obj_list(INT8U *src, INT8U *dst)
{
    INT8U i, j;
    INT32U oad;

    j = 0;
    for(i=0; i<10; i++)
    {
        LIB_MemCpy(src + i * 4, (INT8U *)&oad, 4);
        if(oad != 0x0 && oad != 0xFFFFFFFF)
        {
            LIB_MemCpy(src + i * 4, dst + j * 4, 4);
            j++;
        }
    }
    return j*4;
}

INT8U Oad_Data_Fmt_Switch(ST_OAD *oad, INT8U *src, INT8U src_len, INT8U *dst, INT16U space_sz,
    ST_FMT_ATTR fmt_src, ST_FMT_ATTR fmt_dst)
{
    INT8U dst_len;
    INT8U dstbuf[MAX_LEN+60];
    INT8U num, sum, per_len, per_dst_len;
    INT8U OIA1;
    INT8U OIB2;
    INT8U ret;

    OIA1 = oad->Oi.asByte[OIH] >> 4;
    OIB2 = oad->Oi.asByte[OIL] & 0xf;
#if defined(THREE_PHASE)
    if(is_max_demand_oad(oad))
    {
        if(LIB_ChkValue(src, 9, 0xff) == EQUAL)
        {/*" 针对第2、3结算日需量数据补全FF做的特殊处理 抄读返回NULL "*/
            return 0;
        }
    }
#endif
    /*" 格式转换 "*/
    if(fmt_src.format == E_DLT645_10_ID)
    {
        return fmt_prog_obj_list(src, dst);
    }
    else if( is_show_data || is_same_fmt(fmt_src, fmt_dst))
    {
        if(src_len > space_sz)
        {
            dst_len = 0;
        }
        else
        {
            if(OIA1 <= 1 && OIB2 == 0 && oad->IId == 0) /*" 电量 与 需量 按最大费率存 , 抄读时返回当前费率 "*/
            {
                per_len = FRMT_sizeofFormat(fmt_src.format);
                dst_len = (cur_triff_num + 1)*per_len;
                LIB_MemCpy(src, dst, dst_len);
            }
            else
            {
                LIB_MemCpy(src, dst, src_len);
                dst_len = src_len;
            }
        }
    }
    else
    {
        per_len = FRMT_sizeofFormat(fmt_src.format);
        per_dst_len = FRMT_sizeofFormat(fmt_dst.format);
        sum = src_len / per_len;

        if(OIA1 <= 1 && OIB2 == 0 && oad->IId == 0) /*" 电量 与 需量 按最大费率存 , 抄读时返回当前费率 "*/
        {
            sum = cur_triff_num + 1;
        }
        
        dst_len = 0;
        for(num=0; num<sum; num++)
        {
            ret = FRMT_DataChange(src + num*per_len, dstbuf + num*per_dst_len, fmt_src, fmt_dst);
            if(ret == 0xff)
            {
                return 0;
            }
            dst_len += per_dst_len;
        }
        if(dst_len > space_sz)
        {
            dst_len = 0;
        }
        else
        {
            LIB_MemCpy(dstbuf, dst, dst_len);
        }
    }
    return dst_len;
}

/*"*****************************************************************************"*/
/*"  Function:       NumbOfEvtStart"*/
/*"  Description:    计算事件记录指定关联对象OAD在关联对象列表中的成员号偏移，区分起始和结束OAD"*/
/*"  Calls:          "*/
/*"  Called By:      OADToLeaf"*/
/*"  Input:          file      记录文件对象名"*/
/*"                    OAD    关联对象标识 "*/
/*"                    sOre     查找事件起始or结束成员号标志"*/
/*"                            成员号分起始和结束，以事件记录实际存储顺序为准"*/
/*"                            leaf < 0x80 表示起始记录对象数据成员      leaf > 0x80表示结束记录对象成员"*/
/*"  Output:         "*/
/*"  Return:         leaf       关联OAD对应成员号"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#define   START_ROAD      0    /*"事件起始关联对象"*/
#define   END_ROAD          1    /*"事件结束关联对象"*/


/* 通过传入的obis判断事件是否存储有结束时间 */
INT8U RCRD_EvtWithEnd(EN_RCRD_FILE_ID Obis)
{
    if((stFileDataList[Obis & 0x0fff].endFelxObj != 0) 
        #ifdef MAIN_METER
        && (Obis != E_REFUND_EVNT_RCRD)
        #endif
    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*" 判断当前事件是否允许读取结束时间 或 使用结束时间筛选 "*/
INT8U RCRD_EvtCanGetEndTm(EN_RCRD_FILE_ID Obis)
{
    if((stFileDataList[Obis & 0x0fff].endFelxObj != 0) 
        #ifdef MAIN_METER
        && (Obis != E_REFUND_EVNT_RCRD) && (Obis != E_BUY_ENGY_EVNT_RCRD)
        #endif
    )
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#if defined(THREE_PHASE)
#define  _ZIP_MAX_NUM   10
#else
#define  _ZIP_MAX_NUM   5
#endif
const ST_DRV_OBJ_ZIP_ATTR DrvObjZipList[_ZIP_MAX_NUM] =
{
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_PO_OFFSET, TIMEFRZ_NE_OFFSET - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_NE_OFFSET, TIMEFRZ_PW_OFFSET - 1},
    {E_FRZ_LST_MOUTH_BILL_FRZ, TOTAL_OFFSET, POACT_OFFSET - 1},
    {E_FRZ_LST_MOUTH_BILL_FRZ, POACT_OFFSET, NEACT_OFFSET - 1},
    {E_FRZ_LST_MOUTH_BILL_FRZ, NEACT_OFFSET, BILL_LAST_OFFSET - 1},
#if defined(THREE_PHASE)
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_COM1_OFFSET, TIMEFRZ_COM2_OFFSET - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_COM2_OFFSET, TIMEFRZ_ENG_I_OFFSET - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_ENG_I_OFFSET, TIMEFRZ_ENG_II_OFFSET - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_ENG_II_OFFSET, TIMEFRZ_ENG_III_OFFSET - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_ENG_III_OFFSET, TIMEFRZ_ENG_IV_OFFSET - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_ENG_IV_OFFSET, TIMEFRZ_PA_MAX_DEMAND_OFF - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_PA_MAX_DEMAND_OFF, TIMEFRZ_NA_MAX_DEMAND_OFF - 1},
    {E_FRZ_LST_DAY_FRZ, TIMEFRZ_NA_MAX_DEMAND_OFF, TIMEFRZ_PW_OFFSET - 1},
#endif

        
#if 0
        {EVTLST_PRGDSLOT, 3, 10},
        {EVTLST_PRGDSLOT, 11, 18},
        {EVTLST_PRGTARIFF, 3, 3 + _TARIFF_PRC_NUM -1},
        {EVTLST_PRGTARIFF, 3 + _TARIFF_PRC_NUM, 3 + (2 * _TARIFF_PRC_NUM) -1},
        {EVTLST_PRGSTEP, _LEAF_STP1_PRC_OFFSET, _LEAF_STP1_NUM_OFFSET -1}, /*"当前套阶梯值压缩"*/
        {EVTLST_PRGSTEP, _LEAF_STP1_NUM_OFFSET, _LEAF_YBIL1_OFFSET -1},    /*"当前套阶梯电价压缩"*/
        {EVTLST_PRGSTEP, _LEAF_YBIL1_OFFSET, _LEAF_STP2_PRC_OFFSET -1},    /*"当前套年结算日压缩"*/
        {EVTLST_PRGSTEP, _LEAF_STP2_PRC_OFFSET, _LEAF_STP2_NUM_OFFSET - 1},/*"备用套阶梯值压缩"*/
        {EVTLST_PRGSTEP, _LEAF_STP2_NUM_OFFSET, _LEAF_YBIL2_OFFSET -1},    /*"备用套阶梯电价压缩"*/
        {EVTLST_PRGSTEP, _LEAF_YBIL2_OFFSET, _LEAF_YBIL2_OFFSET + 4 - 1},  /*"备用套年结算日压缩"*/
#endif
};

#define _IN_ZIP_RANGE    1
#define _OUT_ZIP_RANGE   0
#define _ZIP_END         2
static INT8U ChkDrvObjZip(INT16U obis, INT8U leaf)
{
    INT8U rsult;
    INT8U uni;
    INT8U zipLeafEtmp;

    rsult = _OUT_ZIP_RANGE;

    for(uni = 0; uni < _ZIP_MAX_NUM; uni++)
    {
        if(obis == DrvObjZipList[uni].obis)
        {
            zipLeafEtmp = DrvObjZipList[uni].zipLeafE;
            if((leaf >= DrvObjZipList[uni].zipLeafS) && (leaf < zipLeafEtmp))
            {
                rsult = _IN_ZIP_RANGE;
                break;
            }
            else if(leaf == zipLeafEtmp)
            {
                rsult = _ZIP_END;
                break;
            }
            else
            {

            }
        }
    }

    return rsult;
}


/*"*****************************************************************************"*/
/*"  Function:       CalDrvdObjectLen"*/
/*"  Description:    计算派生类数据对象数据成员数据长度，输出成员在记录对象中的偏移位置，成员数据存储格式"*/
/*"  Calls:          "*/
/*"  Called By:      DC_ReadRcrdData，DC_SaveRcrdData"*/
/*"  Input:          obis      派生类数据对象名"*/
/*"                  leaf      派生类数据对象中指定成员号(从1开始编号)，0xff表示整个派生类对象"*/
/*"  Output          poffset   成员对应数据在记录中的偏移位置，leaf为0xff时，偏移位置为0"*/
/*"                  pformat   遍历到最后一个成员的存储数据格式编号"*/

/*"  Return:         len       派生类数据对象指定成员数据长度，leaf为0xff时，数据长度为整个对象数据长度"*/
/*"  Others:         code size  286byte"*/
/*"*****************************************************************************"*/
static INT16U  CalDrvdObjectLen(INT16U obis, INT8U leaf, INT16U *poffset, INT8U *pformat)
{
    ST_FELXOBJ_DATA_ATTR  DerivedDataAttr;
    ST_FIXED_LIST_ATTR  FAR_POINTER*pDataScrList1;
    ST_FIXED_LIST_ATTR  FAR_POINTER*pDataScrList2;
    INT16U Len;/*"单个数据成员数据长度"*/
    INT16U offset;/*"数据成员在数据对象中断位置偏移"*/
    INT8U uni;
    INT8U loopMax;/*"查询成员数"*/
    INT8U lenFormat;
    INT16U  obisTmp;
    //INT8U save_fmt;

    if(obis >= 0x2000)/*计算结束*/
    {
        offset = 0;
        obisTmp = obis - 0x2000;
    }
    else
    {
        offset = RCRD_START_TM_OFF;
        obisTmp = obis;
    }

    Len = 0;
    lenFormat = 0;

    DerivedDataAttr = stDrvDataList[(obisTmp & 0x0fff)];

    if(obisTmp == DerivedDataAttr.obis)
    {
        pDataScrList1 = (ST_FIXED_LIST_ATTR FAR_POINTER*) DerivedDataAttr.pdatalis1;/*"此处的强制类型转换考虑处理方案"*/
        pDataScrList2 = (ST_FIXED_LIST_ATTR FAR_POINTER*) DerivedDataAttr.pdatalis2;
        if(NULL != pDataScrList1)/*"记录文件内容数据源列表1不为空，按照事件记录存储格式计算数据长度"*/
        {
            if(leaf < DerivedDataAttr.data_num1)
            {
                loopMax = leaf;
            }
            else
            {
                loopMax = DerivedDataAttr.data_num1;
            }
            for(uni = 0; uni < loopMax; uni++)
            {
                *pformat = pDataScrList1->format_id;
                lenFormat = FRMT_sizeofFormat(stFormatList[pDataScrList1->format_id].format);
                Len = lenFormat;
                offset += Len;
                if(_IN_ZIP_RANGE != ChkDrvObjZip(obis, (INT8U)(uni + 1)))
                {
                    pDataScrList1++;
                }
            }
        }
        if(leaf > DerivedDataAttr.data_num1)
        {
            if(0 != pDataScrList2)/*"关联数据对象表不为空，按照OAD对应的数据格式进行数据长度计算"*/
            {
                INT8U leafTemp;
                INT8U ObjListNum;
                

                ObjListNum = DerivedDataAttr.data_num2;
                
                leafTemp = (INT8U) (leaf - DerivedDataAttr.data_num1);
                if(leafTemp < ObjListNum)
                {
                    loopMax = leafTemp;
                }
                else
                {
                    loopMax = ObjListNum;
                }
                for(uni = 0; uni < loopMax; uni++)
                {
                    *pformat = pDataScrList2->format_id;
                    lenFormat = FRMT_sizeofFormat(stFormatList[pDataScrList2->format_id].format);
                    Len = lenFormat;
                    offset += Len;
                    if(_IN_ZIP_RANGE != ChkDrvObjZip(obis, (INT8U)(uni + 1 + DerivedDataAttr.data_num1)))
                    {
                        pDataScrList2++;
                    }
                }                
            }
        }

        if(0xff == leaf)
        {
            Len = offset;
            offset = 0;
        } 
        else
        {
            offset -= Len;
        }
    }
    if(NULL != poffset)
    {
        *poffset = offset;
    }

    return Len;
}

/*" 计算一条记录最大的存储空间 "*/
INT16U RCRD_get_one_rcrd_max_sz(EN_RCRD_FILE_ID RCRD_obis)
{
    ST_FILE_ATTR  FileAttri;
    INT16U sz;
    INT16U use_sector;
    INT16U num_per_sec;
    INT32U max_num;
    
    FileAttri = stFileDataList[(RCRD_obis & 0x0fff)];  
    if(FileAttri.file < E_FILE_FLASH_START)
    { /* 存在内卡中 */
        sz = FileAttri.fileSize / GetRcrdMaxtCnt(RCRD_obis);
    }
    else
    { /* 存在flash中 */
        max_num = GetRcrdMaxtCnt(RCRD_obis);
        use_sector = (FileAttri.fileSize - FLASH_SECTOR_SZ)/FLASH_SECTOR_SZ;
        num_per_sec = max_num / use_sector;
        if((max_num % use_sector) != 0)
        {
            num_per_sec++;
        }
        sz = FLASH_SECTOR_SZ / num_per_sec;
    }
    return sz;
}

INT16U RCRD_get_st_end_len(EN_RCRD_FILE_ID RCRD_obis, INT16U *st_len, INT16U *end_len, ST_OAD *oad_array)
{
    INT16U st;
    INT16U end;
    INT16U offset;
    ST_FILE_ATTR  FileAttri;
    //ST_FMT_ATTR fmt_save;
    //ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    INT8U pformat;
    
    FileAttri = stFileDataList[(RCRD_obis & 0x0fff)];

    st = 0;
    end = 0;
    if(FileAttri.startFelxObj != 0)
    {
        st = CalDrvdObjectLen(FileAttri.startFelxObj, 0xff, &offset, &pformat);
        if(FileAttri.endFelxObj != 0)
        {
            end = CalDrvdObjectLen(FileAttri.endFelxObj+0x2000, 0xff, &offset, &pformat);
        }
    }
    *st_len = st;
    *end_len = end;
    return (st + end);
}


/*"****************************************************************************
*"*/
/*"  Function:       RCRD_ReadRcrdDataByRoad "*/
/*"  Description:    通过OAD筛查记录数据内容"*/
/*"  Calls:          "*/
/*"  Called By:      协议处理单元读取记录数据"*/
/*"  Input:          file      记录文件对象名"*/
/*"                  index     上N次记录数据，0
表示当前次事件（没有当前就是上一次），以此类推 "*/
/*"                  oad      指定筛选OAD"*/
/*"  Output:         pbuff     数据接收缓存指针"*/
/*"                  pformat   
记录数据存储格式（超单项数据是输出，用于数据转换）"*/
/*"  Return:         len       读取数据长度     0表示数据读取不成功"*/
/*"  Others:         "*/
/*"****************************************************************************
*"*/
INT16U RCRD_ReadRcrdDataByRoad(EN_RCRD_FILE_ID rcrd_obis, INT32U seq_no,  
ST_OAD *road, INT8U *pbuff, 
  ST_FMT_ATTR *fmt_save, ST_FMT_ATTR *fmt_dst, ST_OAD *road_list)
{
    #if 0
    ST_FILE_ATTR  *pFileAttri;
    INT8U   rsult;
    INT16U Len;
    INT16U off;
    INT32U StartAdrr;
    INT16U st_len;
    INT16U end_len;
    INT16U per_space;

    rsult = FALSE;
    pFileAttri = (ST_FILE_ATTR *)&stFileDataList[(rcrd_obis & 0x0fff)];

    RCRD_get_st_end_len(rcrd_obis, &st_len, &end_len, road_list);
    per_space = RCRD_get_one_rcrd_max_sz(rcrd_obis);
    StartAdrr = RCRD_get_data_addr_off(pFileAttri, per_space, seq_no);
    Len = get_road_data_info(rcrd_obis, road, &off, fmt_save, fmt_dst, 
road_list);
    if((Len > 0) && ((off + Len) <= per_space) && ((off + Len) <= 
MAX_DATA_BUFF_LEN))
    {
        rsult = FS_ReadFile((ENUM_FS_ID)pFileAttri->file, StartAdrr + off, 
pbuff, Len);
        
        if(FALSE == rsult)
        {
            Len = 0;
        }
    }
    else
    {
        Len = 0;
    }
    #else
    return 0;
    #endif
}


/*"****************************************************************************
*"*/
/*"  Function:       DC_ChkEvntCnt"*/
/*"  Description:    检查指定记录数据是否已经产生"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          file      指定记录对象名"*/
/*"                  index     指定上index次记录"*/
/*"  Output          无"*/
/*"  Return:         _TRUE    记录数据已产生  _FALSE 记录数据未产生"*/
/*"  Others:         code size  byte"*/
/*"****************************************************************************
*"*/
INT8U RCRD_ChkEvntCnt(EN_RCRD_FILE_ID file, INT16U index)
{
    ST_FILE_ATTR  FileAttri;
    INT8U   rsult;
    INT32U rcrd_cnt;

    rsult = TRUE;
    FileAttri = stFileDataList[(file & 0x0fff)];
    if(FileAttri.obis == file)
    {
        /*"记录次数为0，或者抄读次数大于记录次数表示还没有记录数据产生，不返回数据"*/
        rcrd_cnt = 0;
        GetRcrdCurtCnt(file, &rcrd_cnt);
        if((0 == rcrd_cnt) || (index > rcrd_cnt))
        {
            rsult = FALSE;
        }
    }
    else
    {
        rsult = FALSE;
    }
    return rsult;
}
#if 0
#define END_OFFSET 0x80

const ST_OAD pd_evt[1] = {0x201e, 0x22, 0x00};

INT8U get_oad_base_645leaf(EN_RCRD_FILE_ID file, INT8U leaf, ST_OAD *oad_list, ST_FMT_ATTR *fmt)
{
    INT8U num;
    const ST_OAD *list;
    INT8U max;
    INT8U idx;

    num = 1;
    list = NULL;
    switch(file)
    {
        case E_PWR_DOWN_EVNT_RCRD:
            list = pd_evt;
            max = ARRAY_SIZE(pd_evt);
            break;
    }

    idx = (leaf & 0x7f) - 1;
    if(list == NULL || idx >= max)
    {
        return 0;
    }
    oad_list[0] = list[idx];
    if(leaf & END_OFFSET)
    {
        if(oad_list[0].Oi.asInt == 0x201e)
        {
            oad_list[0].Oi.asInt = 0x2020;
        }
        else
        {
            oad_list[0].AId &= 0x1f;
            oad_list[0].AId |= 0x80;
        }
    }
    return num;
}

INT16U RCRD_ReadRcrdData(EN_RCRD_FILE_ID id, INT16U index,  INT8U leaf, INT8U *pbuff, 
    ST_FMT_ATTR *pformat)
{
    INT16U   ObjListOBIStmp;      /*"记录数据关联对象列表对象标识"*/
    INT8U ObjListNum;
    const ST_FELXOBJ_DATA_ATTR  *pDrvDataList;
    const ST_FILE_ATTR  *FileAttri;
    ST_OAD RoadList[MAX_RCRD_ROAD_NUM];/*"事件对应关联对象列表"*/
    ST_OAD oad[5];
    ST_FMT_ATTR fmt_comm[5];
    INT8U num_oad;
    INT8U i, rsult;
    INT32U rcrd_cnt;
    INT32U seq_no;
    ST_FMT_ATTR fmt_save;
    ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    INT8U cur_len;
    INT16U len;

    FileAttri = &stFileDataList[(id & 0x0fff)];
    pDrvDataList = &stDrvDataList[(FileAttri->startFelxObj & 0x0fff)];

    ObjListOBIStmp = pDrvDataList->ObjListOBIS;
    ObjListNum = pDrvDataList->data_num2;

    if(id < E_EVT_END_RCRD) /*事件可配关联对象列表获取*/
    {
        rsult = Get_Evt_Road(ObjListOBIStmp, (INT8U *)&RoadList[0], ObjListNum);
    }
    else if (id > E_EVT_END_RCRD && id <= E_STEP_BILL_FRZ_RCRD)/*冻结的需要缓冲并转换,
冻结关联对象为8字节*/
    {
        rsult = Get_Frz_Road(ObjListOBIStmp, (INT8U *)&RoadList[0], ObjListNum);
    }
    if(rsult == FALSE)
    {
        return 0;
    }

    GetRcrdCurtCnt(id, &rcrd_cnt);
    seq_no = rcrd_cnt - index;
    num_oad = get_oad_base_645leaf(id, leaf, oad, fmt_comm);
    len = 0;
    for(i=0; i<num_oad; i++)
    {
        cur_len = RCRD_ReadRcrdDataByRoad(id, seq_no, &oad[i], pbuff, &fmt_save, &fmt_dst[0], 
RoadList);
        pbuff += cur_len;
        len += cur_len;
        if(i == 0)
        {
            LIB_MemCpy((INT8U *)&fmt_dst[0], (INT8U *)pformat, sizeof(ST_FMT_ATTR));
        }
    }
    return len;
}

#else
/*"****************************************************************************
*"*/
/*"  Function:       DC_ReadRcrdData"*/
/*"  Description:    读记录文件"*/
/*"  Calls:          "*/
/*"  Called By:      协议处理单元读取记录数据，或各功能模块读取记录数据"*/
/*"  Input:          file      记录文件对象名"*/
/*"                  index     上N次记录数据，0
表示当前次事件（没有当前就是上一次），以此类推 "*/
/*"                  leaf      数据项偏移(成员叶子号，编号从1开始)
，用于抄读记录中某项数据内容，"*/
/*"                            偏移量顺序由派生类对象定义顺序决定"*/
/*"                            leaf 为0xff是表示抄读整体记录数据,
抄读整体记录数据时，不输出数据格式"*/
/*"                            leaf < 0x80 表示起始记录对象数据成员      leaf 
> 0x80表示结束记录对象成员"*/
/*"  Output:         pbuff     数据接收缓存指针"*/
/*"                  pformat   
记录数据存储格式（超单项数据是输出，用于数据转换）"*/
/*"  Return:         len       读取数据长度     0表示数据读取不成功"*/
/*"  Others:         code size  394byte"*/
/*"****************************************************************************
*"*/
INT16U RCRD_ReadRcrdData(EN_RCRD_FILE_ID file, INT16U index,  INT8U leaf, INT8U *pbuff, ST_FMT_ATTR *pformat)
{
    ST_FILE_ATTR  FileAttri;
    //INT16U   fileName;
    INT16U   fileLen;
    INT8U   rsult;
    INT16U  offset;
    INT8U   formatId;
    INT8U   rcrdErr;/*"记录不存在标志   0 存在     0xff 不存在"*/
    INT32U rcrd_cnt;
    INT16U per_space; 
    INT32U seq_no;
    INT16U StartAdrr;

    rcrdErr = 0;

    /*" 文件名不合法"*/
    if(rcrd_id_is_error(file))
    {
        return FALSE;
    }
    /*"记录次数为0，或者抄读次数大于记录次数表示还没有记录数据产生，不返回数据"*/
    if(FALSE == RCRD_ChkEvntCnt(file, index))
    {
        rcrdErr = 0xff;
    }

    rsult = FALSE;
    FileAttri = stFileDataList[(file & 0x0fff)];
    fileLen = 0;
    formatId = 0;

    if(0xff == leaf)/*"抄读整条记录内容"*/
    {
        if(FileAttri.startFelxObj != 0)
        {
            fileLen = CalDrvdObjectLen(FileAttri.startFelxObj, 0xff, &offset, &formatId);
            if(FileAttri.endFelxObj != 0)
            {
                fileLen += CalDrvdObjectLen(FileAttri.endFelxObj, 0xff, &offset, &formatId);
            }
        }
    }
    else
    {
        if((leaf < 0x80)  )/*"起始记录对象成员  || (E_PRG_EVNT_RCRD == file)"*/
        {
            fileLen = CalDrvdObjectLen(FileAttri.startFelxObj, leaf, &offset, &formatId);
        }
        else /*"结束记录对象成员,偏移地址需加上起始记录对象整体数据长度"*/
        {
            INT16U offsetTemp;

            offsetTemp = 0;

            fileLen = CalDrvdObjectLen(FileAttri.startFelxObj, 0xff, &offset, &formatId);
            offsetTemp = fileLen;
            fileLen = CalDrvdObjectLen(FileAttri.endFelxObj, (INT8U) (leaf - 0x80), &offset, &formatId);
            offset += offsetTemp;
        }
    }
    if(rcrdErr != 0xff)
    {
        GetRcrdCurtCnt(file, &rcrd_cnt);
        per_space = RCRD_get_one_rcrd_max_sz(file);
        seq_no = rcrd_cnt - index;
        StartAdrr = RCRD_get_data_addr_off(&FileAttri, per_space, seq_no);
        rsult = FS_ReadFile((ENUM_FS_ID)FileAttri.file, StartAdrr + offset, pbuff, fileLen);
    }
    else   /*"记录还未产生，不执行抄读动作"*/
    {
        rsult = TRUE;
    }

    if(NULL != pformat)
    {
        *pformat = stFormatList[formatId];
    }
    if(FALSE == rsult)
    {
        fileLen = 0;
    }
    return fileLen;
}
#endif

#define RCD_ST_TIME_LEAFNO  (1) /*" 冻结、事件的记录时间叶子为1"*/ 

void AdjustTm_Clr_Powerup_Frz_Info(EN_RCRD_FILE_ID rcrd_obis, ST_6TIME_FORM *st)
{
    INT32U off;
    ST_6TIME_FORM lst_tm;
    INT8U data[6];
    
    switch(rcrd_obis)
    {
        case E_DAY_FRZ_RCRD:
            off = DAY_FRZ_LST_FRZ_DATE;
            break;
        case E_MONTH_BILL_FRZ_RCRD:
            off = MOUTH_BILL_FRZ_LST_FRZ_DATE;
            break;
        case E_STEP_BILL_FRZ_RCRD:
            off = STEP_FRZ_LST_FRZ_DATE;
            break;
        default:
            return;
    }
    
    FS_ReadFile(E_FILE_RCRD_DATA, off, (INT8U *)&lst_tm, 6);
    if(LIB_MemCmp((INT8U *)&lst_tm, (INT8U *)st, 6) == BIG)
    {
        LIB_MemSet(0x00, data, 6);
        FS_WriteFile(E_FILE_RCRD_DATA, off, data, 6);
    }
}

INT32U get_real_prd_frz_num_addr(EN_RCRD_FILE_ID rcrd_obis)
{
    INT32U addr;

    switch(rcrd_obis)
    {
        case E_HOUR_FRZ_RCRD:/* 小时*/
            addr = RCRD_HOUR_FRZ_NUM_OFF;
            break;
        case E_DAY_FRZ_RCRD:/* 日冻结 */
            addr = RCRD_DAY_FRZ_NUM_OFF;
            break;
        case E_MONTH_FRZ_RCRD: /*"月冻结"*/
            addr = RCRD_MOUTH_FRZ_NUM_OFF;
            break;
        default:
            addr = 0;
            break;
    }
    return addr;
}

INT8U real_prd_frz_num_proc(INT32U ee_addr, INT32U max_num, INT32U add)
{
    INT32U cur_num;
    
    FS_ReadFile(E_FILE_RCRD_DATA, ee_addr, (INT8U *)&cur_num, 4);
        
    if((add != 0) && (cur_num < max_num))
    {
        cur_num++;
        FS_WriteFile(E_FILE_RCRD_DATA, ee_addr, (INT8U *)&cur_num, 4);
    }
    return cur_num;
}

INT8U real_prd_frz_num_dec(INT32U ee_addr, INT32U dec)
{
    INT32U cur_num;
    INT8U res;
    
    res = FS_ReadFile(E_FILE_RCRD_DATA, ee_addr, (INT8U *)&cur_num, 4);
    if(cur_num > dec)
    {
        cur_num -= dec;
    }
    else
    {
        cur_num = 0;
    }

    res = FS_WriteFile(E_FILE_RCRD_DATA, ee_addr, (INT8U *)&cur_num, 4);
    return res;
}

INT8U real_prd_frz_num_clr(INT32U ee_addr)
{
    INT32U cur_num;
    INT8U res;

    cur_num = 0;
    res = FS_WriteFile(E_FILE_RCRD_DATA, ee_addr, (INT8U *)&cur_num, 4);
    return res;
}

INT8U ClrRcrdCurCnt(EN_RCRD_FILE_ID rcrd_obis, INT32U rcrdCnt)
{
    INT32U timesAddr;
    INT32U cnt;
    INT8U res;
#if defined(THREE_PHASE)
    INT8U buff[16];
    INT8U len;
#endif

    MCU_PwrChkCloseINT();
    timesAddr = RCRD_CNT_OFF(rcrd_obis);
    cnt = rcrdCnt;
#if defined(THREE_PHASE)
    if((cnt == 0) && (rcrd_obis >= E_LOST_VOLT_LA_EVNT_RCRD && rcrd_obis <= E_OVER_R_DEMAND_4_EVNT_RCRD))
    {
        LIB_MemSet(0x00, buff, 16);
        if(rcrd_obis >= E_PWR_REVERSE_EVNT_RCRD) /*" TODO 加强容错 "*/
        {
            len = 16;
        }
        else
        {
            len = 12;
        }
        res = FS_WriteFile(E_FILE_RCRD_DATA, timesAddr, buff, len);
    }
    else
    {
        res = FS_WriteFile(E_FILE_RCRD_DATA, timesAddr, (INT8U *)&cnt, 4);
    }
#else
    res = FS_WriteFile(E_FILE_RCRD_DATA, timesAddr, (INT8U *)&cnt, 4);
#endif
    if(cnt == 0)
    {
        RCRD_ClrSpecData(rcrd_obis);
    }
    MCU_PwrChkOpenINT();
    return res;
}

/*" 获取当前记录次数 "*/
INT8U GetRcrdCurtCnt(EN_RCRD_FILE_ID rcrd_obis, INT32U *cnt)
{
    INT8U rsult;
    INT32U timesAddr;
    
        /*从内卡读次数*/
    timesAddr = RCRD_CNT_OFF(rcrd_obis);
    rsult = FS_ReadFile(E_FILE_RCRD_DATA, timesAddr, (INT8U *)cnt, 4);
    return rsult;
}

INT32U GetRcrdMaxtCnt(EN_RCRD_FILE_ID rcrd_obis)
{
    INT32U max;
    
    if(rcrd_obis < E_EVT_END_RCRD)
    {
        max = 10; /*" 事件最大次数固定为10次 "*/
        if(rcrd_obis == E_BROADCAST_JUST_TIME_RCRD)
        {
            max = 100;
        }
#if defined(THREE_PHASE)
        if(rcrd_obis == E_PD_PON_RCRD)
        {
            max = 256;
        }
        else if(rcrd_obis == E_PARA_ERR_RCRD)
        {
            max = 256;
        }
#endif
    }
    else
    {
        max = frz_max_num[rcrd_obis - E_MMT_FRZ_RCRD];
        if(rcrd_obis == E_MIN_FRZ_RCRD)
        {
#if defined(THREE_PHASE)
            max = 28800;
#else
            max = 288;
#endif
        }
    }
    return max;
}

/*" 获取实际最大条数 "*/
INT32U GetRcrdRealMaxtCnt(EN_RCRD_FILE_ID rcrd_obis)
{
    INT32U max, real_max;
    
    max = GetRcrdMaxtCnt(rcrd_obis);
    switch(rcrd_obis)
    {
        case E_HOUR_FRZ_RCRD:/* 小时*/
            real_max = real_prd_frz_num_proc(RCRD_HOUR_FRZ_NUM_OFF, max, 0);
            if(real_max < max)
            {
                max = real_max;
            }
            break;
        case E_DAY_FRZ_RCRD:/* 日冻结 */
            real_max = real_prd_frz_num_proc(RCRD_DAY_FRZ_NUM_OFF, max, 0);
            if(real_max < max)
            {
                max = real_max;
            }
            break;
        case E_MONTH_FRZ_RCRD: /*"月冻结"*/
            real_max = real_prd_frz_num_proc(RCRD_MOUTH_FRZ_NUM_OFF, max, 0);
            if(real_max < max)
            {
                max = real_max;
            }
            break;
    }
    return max;
}


void Clr_All_Evt_Data(void)
{
    INT32U timesAddr;
    INT32U len;
    INT32U cnt[2];

    MCU_PwrChkCloseINT();
    GetRcrdCurtCnt(E_MTR_CLR_EVNT_RCRD, &cnt[0]);
    GetRcrdCurtCnt(E_EVNT_CLR_EVNT_RCRD, &cnt[1]);
    
    timesAddr = RCRD_CNT_OFF(E_PWR_DOWN_EVNT_RCRD);
    len = RCRD_CNT_OFF(E_EVT_END_RCRD) - RCRD_CNT_BLK_OFF;
    FS_DelFile(E_FILE_RCRD_DATA, timesAddr, len);

    ClrRcrdCurCnt(E_MTR_CLR_EVNT_RCRD, cnt[0]);
    //if (ChkStatus(E_FAC) == FALSE) /*" 事件清零不清事件清零记录 "*/
    {
        ClrRcrdCurCnt(E_EVNT_CLR_EVNT_RCRD, cnt[1]);
    }
#if defined(THREE_PHASE)
    clr_r_eng_com_prg_evt_data();
    clr_lost_volt_stat_info();
#endif

    MCU_PwrChkOpenINT();
}

void Clr_All_Rcrd_Data(void)
{
    INT32U len;
    INT32U cnt[2];
    ST_RCRD_DATA_FILE data;

    len = E2_LR_CTR_DATA; /*" 除分钟冻结外的记录数据都在这里清除 "*/
    LIB_MemSet(0x00, (INT8U *)&data, len);    
    MCU_PwrChkCloseINT();
    /*
    timesAddr = RCRD_CNT_OFF(E_PWR_DOWN_EVNT_RCRD);
    len = sizeof(ST_RCRD_CNTS);
    FS_DelFile(E_FILE_RCRD_DATA, timesAddr, len);
    */
    GetRcrdCurtCnt(E_MTR_CLR_EVNT_RCRD, &cnt[0]);
    FS_WriteFile(E_FILE_RCRD_DATA, 0, (INT8U *)&data, len);

    //if (ChkStatus(E_FAC) == FALSE) /*" 电表清零不清电表清零事件 "*/
    {
        ClrRcrdCurCnt(E_MTR_CLR_EVNT_RCRD, cnt[0]);
    }
    MCU_PwrChkOpenINT();
}

#if 0
static INT32U GetStdTime(ST_6TIME_FORM *pTime, INT8U interval)
{
    INT32U stdTime;

    switch(interval)
    {
    case INVL_HOUR:
        stdTime = LIB_HToStdDate(pTime);
        break;
    case INVL_DAY:
        stdTime = LIB_DToStdDate(pTime);
        break;
    case INVL_MONTH:
        stdTime = (INT32U)(pTime->year) * 12 + (INT32U)(pTime->month);
        break;
    default:
        stdTime = 0L;
        break;
    }

    return stdTime;
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       ChkNonperiodicFrz"*/
/*"  Description:   检查当前记录是否为非周期冻结"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          index: 记录类型索引"*/
/*"  Output:        无"*/
/*"  Return:        TRUE: 非周期冻结，FALSE: 其它记录类型"*/
/*"*****************************************************************************"*/
INT8U ChkIsNonPeriodFrz(INT16U rcrd_obis)
{
    INT8U result;

    if((rcrd_obis == E_HOUR_FRZ_RCRD) || (rcrd_obis == E_DAY_FRZ_RCRD)
        || (rcrd_obis == E_MONTH_FRZ_RCRD))
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }

    return result;
}


/*" 新来的cur时间点和数组array中前sum个成员比较，如果array中有时间比cur的还大的则删除这些大于等于cur的记录 "*/
INT8U add_record_remove_same_tm(INT8U sum, ST_6TIME_FORM *array, ST_6TIME_FORM *cur, INT32U cur_seq_no)
{
    INT8U i;
    INT8U new_sum;
    //INT8U has;

    for(i=0; i<sum; i++)
    {
        if(LIB_MemCmp((INT8U *)&array[i], (INT8U *)cur, 6) != SMALL)
        {
            break;
        }
    }

    LIB_MemCpy((INT8U *)cur, (INT8U *)&array[i], 6);
    RsultAttr.seq_no[i] = cur_seq_no;
    new_sum = i + 1;
    return new_sum;
}

#if 0
/*" 查找指定时间的冻结记录 "*/
static INT8U SeachRcdAccToTime(EN_RCRD_FILE_ID rcrd_obis, ST_Get_Record_Data *rcrd_select, ST_OAD *road_list)
{
    INT8U i, maxNum, rsult;
    INT8U *pTime;
    INT8U buff[7];
    INT32U rcrdCnt;
    ST_FMT_ATTR fmt_save;
    ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    INT32U seq_no;

    if(rcrd_obis > E_STEP_BILL_FRZ_RCRD)
    {
        return FALSE;
    }
    
    /*从内卡读次数*/
    rsult = GetRcrdCurtCnt(rcrd_obis, &rcrdCnt);
    if(rsult == FALSE)
    {
        return FALSE;
    }
#if defined(THREE_PHASE)
    CurRdEvtCnt = rcrdCnt;
#endif
    maxNum = (INT8U)GetRcrdRealMaxtCnt(rcrd_obis); /*" 分钟冻结不走此分支，所以最大返回值为254 "*/
    if(rcrdCnt < maxNum)
    {
        maxNum = (INT8U)rcrdCnt;
    }

    pTime = rcrd_select->pDdataFilter;
    LIB_MemSet(0x00, buff, sizeof(buff));
    for(i = 1; i <= maxNum; i++) /*" 从上1次开始往上N次查找 "*/
    {
        seq_no = rcrdCnt - i;
        RCRD_ReadRcrdDataByRoad(rcrd_obis, seq_no, &rcrd_select->Oad_Selector, buff, &fmt_save, &fmt_dst[0], road_list);/*"获取时间叶子数据"*/        
        if(EQUAL == LIB_MemCmp(buff, pTime, sizeof(ST_6TIME_FORM)))
        {
            RsultAttr.RcrdId = seq_no;
            return TRUE;
        }
    }

    return FALSE;
}


/*"查找指定时间范围内指定间隔的冻结记录"*/
static INT8U SeachRcdAccToRange(EN_RCRD_FILE_ID rcrd_obis, ST_Get_Record_Data *rcrd_select, ST_OAD *road_list)
{
    INT8U i;
    INT8U maxNum, cnt;
    INT16U invl;
    ST_7TIME_FORM tempTime;
    INT8U *pTimeS, *pTimeE;
    INT8U *pInvl;
    INT8U rsult;
    INT32U startTime, rcdTime;
    INT8U typeInvl;
    _ST_RSD_SELECT2 *select2;
    INT32U rcrdCnt;
    INT32U seq_no;
    INT8U is_prd_frz; /*" 是否为周期性冻结 "*/
    ST_FMT_ATTR fmt_save;
    ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    INT8U cur_len;
    
    select2 = (_ST_RSD_SELECT2 *)rcrd_select->pDdataFilter;
    if(ChkIsNonPeriodFrz(rcrd_obis) == TRUE)  
    {/*"非周期冻结的其它冻结和事件，忽略间隔"*/
        invl = 1;
        is_prd_frz = 0;
    }
    else
    {/*"周期冻结"*/
        is_prd_frz = 1;
        typeInvl = select2->Period[0];
        pInvl = select2->Period;
        /*"仅支持与冻结周期单位一致的TI 这个在应用层实现 "*/
        invl = ((INT16U)(pInvl[1]) << 8) +  (INT16U)(pInvl[2]);
        if(0 == invl)
        {
            invl = 1;
        }
    }

    /*从内卡读次数*/
    rsult = GetRcrdCurtCnt(rcrd_obis, &rcrdCnt);
    if(rsult == FALSE)
    {
        return FALSE;
    }
#if defined(THREE_PHASE)
    CurRdEvtCnt = rcrdCnt;
#endif
    maxNum = (INT8U)GetRcrdRealMaxtCnt(rcrd_obis); /*" 分钟冻结不走此分支，所以最大返回值为254 "*/
    if(rcrdCnt < maxNum)
    {
        maxNum = (INT8U)rcrdCnt;
    }

    pTimeS = (INT8U *)&select2->Time_begin;
    pTimeE = (INT8U *)&select2->Time_end;

    if((rcrd_obis > E_EVT_END_RCRD) && (is_prd_frz == 1)) /*周期性冻结类筛选结束时间范围不大于当前时间*/
    {
        GetSingle(E_SYS_TIME, (INT8U *)&tempTime);/*"取系统时间"*/
        if(BIG == LIB_MemCmp(pTimeE, (INT8U *)&tempTime, sizeof(ST_6TIME_FORM)))
        {
            LIB_MemCpy((INT8U *)&tempTime, pTimeE, sizeof(ST_6TIME_FORM));
        }
    }

    cnt = 0;
    for(i = maxNum; i > 0 ; i--)
    {
        seq_no = rcrdCnt - i;
        cur_len = RCRD_ReadRcrdDataByRoad(rcrd_obis, seq_no, &rcrd_select->Oad_Selector, (INT8U *)&tempTime, &fmt_save, &fmt_dst[0], road_list);/*"获取时间叶子数据"*/        

        if((cur_len > 0) && (TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&tempTime)) 
         && (SMALL != LIB_MemCmp((INT8U *)&tempTime, pTimeS, sizeof(ST_6TIME_FORM))) 
         && (SMALL == LIB_MemCmp((INT8U *)&tempTime, pTimeE, sizeof(ST_6TIME_FORM))) )
        {
            if(invl > 1)/*"周期冻结按间隔查找"*/        
            {
                if(0 == cnt)/*"首条记录，以此为基点按间隔搜索后续记录"*/        
                {
                    startTime = GetStdTime((ST_6TIME_FORM *)&tempTime, typeInvl);    
                }
                else
                {
                    rcdTime = GetStdTime((ST_6TIME_FORM *)&tempTime, typeInvl);
                    if(0 != ((rcdTime - startTime) % invl))
                    {
                        continue;
                    }
                }
            }
#if 0  /*" 非周期性冻结 删除改时间导致的重复记录 最新补遗不要求此功能 "*/          
            if(is_prd_frz == 0)
            {
                if(cntRcd < 20)
                {
                    cntRcd = add_record_remove_same_tm(cntRcd, frz_date, (ST_6TIME_FORM *)&tempTime, seq_no);
                }
            }
            else
            {
                RsultAttr.seq_no[cntRcd] = seq_no;
                cntRcd += 1;
            }
 #else
            RsultAttr.seq_no[cnt] = seq_no;
            cnt += 1;
 #endif
            if(cnt >= MAX_LINE_RSULT_NUM)
            {/*"指定范围内查找记录数限制"*/        
                break;
            }
        }
    }

    RsultAttr.num = cnt;

    return cnt;
}


INT8U RCRD_GetRcrdOneLineData(EN_RCRD_FILE_ID id, INT32U seq_no, ST_Get_Record_Data *rcrd_select, 
    ST_GetRcrdOut *pout, ST_OAD *road_list, INT16U max_buf_sz)
{
    INT8U i, cur_len, dst_len;
    INT8U *len_list;
    INT8U *data;
    INT16U len;
    ST_FMT_ATTR fmt_save;
    ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    INT8U buf[MAX_LEN];
    INT16U space_sz;
    ST_OAD *cur_road;
    INT8U is_end;

    is_end = 1;
    if(id < E_EVT_END_RCRD)
    {
        if(RCRD_EvtCanGetEndTm(id))
        {
            is_end = check_evt_is_end(id, seq_no, &buf[0]);
        }
    }


    len = 0;
    space_sz = max_buf_sz;
    len_list = pout->pLenList;
    data = pout->pBuff;
    LIB_MemSet(0x00, len_list, rcrd_select->OadListNumber);
    for(i=0; i<rcrd_select->OadListNumber; i++)
    {
        cur_road = &rcrd_select->poadlist[i];

        if((cur_road->Oi.asInt == 0x2023) || (cur_road->Oi.asInt == 0x2022))
        { /*" 记录序号 "*/
            if(space_sz > 4)
            {
                LIB_MemCpy((INT8U *)&seq_no, data + len, 4);
                len_list[i] = 4;
                len += 4;
                space_sz -= 4;
                continue;
            }
            else
            {
                break;
            }
        }
        else if(cur_road->Oi.asInt == 0x3300) /* 事件上报状态 */
        {
            len_list[i] = 0;
            continue;
        }
        else if(cur_road->Oi.asInt == 0x2024) /* 事件发生源  */
        {
            #ifdef THREE_PHASE
            if(id != E_R_ENGY_COM_PRG_EVNT_RCRD)
            {
                len_list[i] = 0;
                continue;
            }
            #else
            len_list[i] = 0;
            continue;
            #endif
        }
        else if((id < E_EVT_END_RCRD) 
            && (AidType_is_after_evt_end(cur_road->AId) || (cur_road->Oi.asInt == 0x2020)) && (is_end == 0))
        { /*" 最近一次事件正在发生未结束或结束时间非法，其结束数据返回NULL "*/
            len_list[i] = 0;
            continue;
        }
        else if((RCRD_EvtCanGetEndTm(id) == FALSE) && (cur_road->Oi.asInt == 0x2020))
        {/*" 购电记录结束时间返回NULL "*/
            len_list[i] = 0;
            continue;    
        }
        
        cur_len = RCRD_ReadRcrdDataByRoad(id, seq_no, cur_road, buf, &fmt_save, &fmt_dst[0], road_list);
        if(cur_len == 0)
        {
            len_list[i] = 0;
            continue;
        }
#if defined(THREE_PHASE)
        if((id < E_EVT_END_RCRD) && (seq_no == CurRdEvtCnt - 1) && (is_end == 0))
        {
            refresh_cur_occur_evt_data(id, cur_road, buf);
        }
#endif
        dst_len = Oad_Data_Fmt_Switch(cur_road, buf, cur_len, data + len, 
            space_sz, fmt_save, fmt_dst[0]);
        if(dst_len != 0)
        {
            len += dst_len;
            len_list[i] = dst_len;
            space_sz -= dst_len;
        }
        else
        {
            len_list[i] = 0;
        }
    }
    return TRUE;
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       Get_Data_Comm"*/
/*"  Description:    记录数据查找"*/
/*"  Input:          pOad   OAD标识信息地址"*/
/*"                  pFrzInfo  记录筛选方法 "*/
/*"                  rcdIndex  记录序号"*/
/*"  Output:     pBuffer  输出数据接收缓存"*/
/*"  Return:         0: 失败；非0: 返回记录条数 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U RCRD_GetRcrd(EN_RCRD_FILE_ID id, void *pdin, void *pdout, INT16U len)
{
    #if 0
    INT8U *pTime;
    INT8U i, dataLen, result;
    ST_Get_Record_Data *rcrd_select;
    ST_GetRcrdOut *pout;
    ST_OAD RoadList[MAX_RCRD_ROAD_NUM];/*"事件对应关联对象列表"*/
    INT16U   ObjListOBIStmp;      /*"记录数据关联对象列表对象标识"*/
    INT8U    ObjListNum;      /*"记录数据关联对象列表对象数"*/
    const ST_FELXOBJ_DATA_ATTR  *pDrvDataList;
    const ST_FILE_ATTR  *FileAttri;
    INT8U rsult;
    INT8U  filter9;/*"筛选9 上N次"*/
    INT32U rcrd_cnt;
    INT32U seq_no;

    if (rcrd_id_is_error(id))
    {
        return 0;
    }
    
    cur_triff_num = get_cur_eng_triff_num();
    meter_type = GetMeterType();
    
    rcrd_select = (ST_Get_Record_Data *)pdin;
    pout = (ST_GetRcrdOut *)pdout;
    if(id == E_MIN_FRZ_RCRD)
    {
        return MinFrzDataFilter(rcrd_select, pout, len);
    }
    
    FileAttri = &stFileDataList[(id & 0x0fff)];
    pDrvDataList = &stDrvDataList[(FileAttri->startFelxObj & 0x0fff)];

    ObjListOBIStmp = pDrvDataList->ObjListOBIS;
    ObjListNum = pDrvDataList->data_num2;
    rsult = FALSE;
    if(ObjListNum > 0)
    {
        if(id < E_EVT_END_RCRD) /*事件可配关联对象列表获取*/
        {
            rsult = Get_Evt_Road(ObjListOBIStmp, (INT8U *)&RoadList[0], ObjListNum);
        }
        else if (id > E_EVT_END_RCRD && id <= E_STEP_BILL_FRZ_RCRD)/*冻结的需要缓冲并转换,冻结关联对象为8字节*/
        {
            rsult = Get_Frz_Road(ObjListOBIStmp, (INT8U *)&RoadList[0], ObjListNum);
        }
    }
    else
    {
        LIB_MemSet(0x00, (INT8U *) &RoadList[0], sizeof(RoadList));
        rsult = TRUE;
    }
    
    if(rsult == FALSE)
    {
        return 0;
    }

    //GetSingle(E_SYS_TIME, now_t);/*"取系统时间"*/

    dataLen = 0xFF;

    if((1 == rcrd_select->SelectNumber) || (2 == rcrd_select->SelectNumber)) /*"方法1、2"*/
    {
        #if defined(THREE_PHASE)
        if(TRUE == ChkStatus(E_LOW_PWR))
        {
            return 0;
        }
        #endif
        pTime = rcrd_select->pDdataFilter;
        is_show_data = 0;
        if(0 == rcrd_select->TotalLine) /*"方法1、2 仅支持按冻结时间筛选，应用层限制"*/
        {/*"不支持其它OAD的筛选"*/
            if(OI_EVENT_END_TIME == rcrd_select->Oad_Selector.Oi.asInt) /*"RSD选择: 事件结束时间"*/
            {
                if(FALSE == RCRD_EvtCanGetEndTm(id))
                {/*"当前事件无结束记录"*/
                    return 0;
                }
            }
                
            for(i = 0; i < rcrd_select->SelectNumber; i++) 
            {
                if(FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)(pTime + i * 6)))/*"时间非法"*/
                {
                    return 0;
                }
            }

            if(1 == rcrd_select->SelectNumber)
            {/*"方法1"*/
                
                result = SeachRcdAccToTime(id, rcrd_select, RoadList);
            }
            else/*"方法2"*/
            {
                
                result = SeachRcdAccToRange(id, rcrd_select, RoadList);
            }

            return result;
        }
        else /*"RSD方法2读冻结数据"*/
        {
            if(1 == rcrd_select->SelectNumber)
            {
                seq_no = RsultAttr.RcrdId;
                RCRD_GetRcrdOneLineData(id, seq_no, rcrd_select, pout, RoadList, len);
                return 1;
            }
            else
            {
                if(rcrd_select->CurrentLine <= RsultAttr.num)
                {
                    seq_no = RsultAttr.seq_no[rcrd_select->CurrentLine - 1];
                    RCRD_GetRcrdOneLineData(id, seq_no, rcrd_select, pout, RoadList, len);
                    return 1;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    else if(9 == rcrd_select->SelectNumber)
    {
        if(0 == rcrd_select->TotalLine)
        {
            if(rcrd_select->pDdataFilter[1] == 0)
            {
                is_show_data = 0;
                #if defined(THREE_PHASE)
                if(TRUE == ChkStatus(E_LOW_PWR))
                {
                    return 0;
                }
                #endif
            }
            else
            {
                is_show_data = 1;
            }
            filter9 = rcrd_select->pDdataFilter[0];
            result = GetRcrdCurtCnt(id, &rcrd_cnt);
            if(result == TRUE && filter9 <= rcrd_cnt && filter9 <= GetRcrdRealMaxtCnt(id))
            {
#if defined(THREE_PHASE)
                CurRdEvtCnt = rcrd_cnt;
#endif
                seq_no = rcrd_cnt - filter9;
                RsultAttr.RcrdId = seq_no;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            seq_no = RsultAttr.RcrdId;
            RCRD_GetRcrdOneLineData(id, seq_no, rcrd_select, pout, RoadList, len);
            return 1;
        }
    }
    else/*"除方法1、2、9外，不支持其它方法"*/
    {
        dataLen = 0;
    }
#endif
    return 1;
}

void RCRD_SpecProcAfterSave(EN_RCRD_FILE_ID rcrd_obis)
{
#if defined (THREE_PHASE)
    INT8U r_eng_com_tag;
    INT8U date[7];
    INT32U tms;
#endif
    switch(rcrd_obis)
    {
        case E_HOUR_FRZ_RCRD:/* 小时*/
            real_prd_frz_num_proc(RCRD_HOUR_FRZ_NUM_OFF, NUM_HOUR_FRZ, 1);
            break;
        case E_DAY_FRZ_RCRD:/* 日冻结 */
            real_prd_frz_num_proc(RCRD_DAY_FRZ_NUM_OFF, NUM_DAY_FRZ, 1);
            FS_WriteFile(E_FILE_RCRD_DATA, DAY_FRZ_LST_FRZ_DATE, &DCDataBuf[0], 6);
            break;
        case E_MONTH_FRZ_RCRD: /*"月冻结"*/
            real_prd_frz_num_proc(RCRD_MOUTH_FRZ_NUM_OFF, NUM_MON_FRZ, 1);
            break;
        case E_MONTH_BILL_FRZ_RCRD: /*"月结算"*/
            FS_WriteFile(E_FILE_RCRD_DATA, MOUTH_BILL_FRZ_LST_FRZ_DATE, &DCDataBuf[0], 6);
            break;
        case E_STEP_BILL_FRZ_RCRD: /*" 阶梯结算冻结 "*/
            FS_WriteFile(E_FILE_RCRD_DATA, STEP_FRZ_LST_FRZ_DATE, &DCDataBuf[0], 6);
            break;
#if defined(THREE_PHASE)
        case E_R_ENGY_COM_PRG_EVNT_RCRD: /*"无功组合方式编程"*/
            r_eng_com_tag = DCDataBuf[0];/*"无功组合方式1特征字（0），无功组合方式2特征字（1）"*/
            GetSingle(E_SYS_TIME, date);/*"取系统时间"*/  
            if(r_eng_com_tag == 0)
            {
                FS_WriteFile(E_FILE_RCRD_DATA, R_ENG_COM1_PRG_TIME_OFF, date, 6);
            }
            else
            {
                FS_WriteFile(E_FILE_RCRD_DATA, R_ENG_COM2_PRG_TIME_OFF, date, 6);
                FS_ReadFile(E_FILE_RCRD_DATA, R_ENG_COM2_PRG_TMS_OFF, (INT8U *)&tms, 4);
                tms++;
                FS_WriteFile(E_FILE_RCRD_DATA, R_ENG_COM2_PRG_TMS_OFF, (INT8U *)&tms, 4);
            }
            
            break;
#endif
    }
    return;
}


/*" 清零时清除特定记录特殊数据 "*/
void RCRD_ClrSpecData(EN_RCRD_FILE_ID rcrd_obis)
{
    INT8U data[7];

    LIB_MemSet(0x00, data, 7);
    switch(rcrd_obis)
    {
        case E_HOUR_FRZ_RCRD:/* 小时*/
            real_prd_frz_num_clr(RCRD_HOUR_FRZ_NUM_OFF);
            break;
        case E_DAY_FRZ_RCRD:/* 日冻结 */
            real_prd_frz_num_clr(RCRD_DAY_FRZ_NUM_OFF);
            FS_WriteFile(E_FILE_RCRD_DATA, DAY_FRZ_LST_FRZ_DATE, data, 6);
            break;
        case E_MONTH_FRZ_RCRD: /*"月冻结"*/
            real_prd_frz_num_clr(RCRD_MOUTH_FRZ_NUM_OFF);
            break;
        case E_MONTH_BILL_FRZ_RCRD: /*"月结算"*/
            FS_WriteFile(E_FILE_RCRD_DATA, MOUTH_BILL_FRZ_LST_FRZ_DATE, data, 6);
            break;
        case E_STEP_BILL_FRZ_RCRD: /*" 阶梯结算冻结 "*/
            FS_WriteFile(E_FILE_RCRD_DATA, STEP_FRZ_LST_FRZ_DATE, data, 6);
            break;
#if defined(THREE_PHASE)
        case E_R_ENGY_COM_PRG_EVNT_RCRD: /*"无功组合方式编程"*/
            clr_r_eng_com_prg_evt_data();
            break;
        case E_LOST_VOLT_LA_EVNT_RCRD: /*" 失压 "*/
        case E_LOST_VOLT_LB_EVNT_RCRD:
        case E_LOST_VOLT_LC_EVNT_RCRD:
            clr_lost_volt_stat_info();
            break;
#endif

    }
    return;
}

typedef struct
{
    ST_FIXED_LIST_ATTR *pDataScrList;
    INT8U       data_num;
}ST_SCR_DATA_LIST;

INT8U get_rcrd_data_to_buf(EN_RCRD_FILE_ID file, EN_FELXOBJ_ID obis, INT8U *Buff, INT16U *Len)
{
    const ST_FELXOBJ_DATA_ATTR  *pDrvDataList;
#if defined(THREE_PHASE)
    const ST_FILE_ATTR  *FileAttri;
#endif
    ST_SCR_DATA_LIST  dataList[2];
    ST_FIXED_LIST_ATTR *pDataScrListTmp;
    INT8U dataNumTmp;
    INT8U uni;
    INT8U BuffTemp[MAX_LEN];
    INT16U dataLen;
    INT16S res, rsult;
    INT8U cur_len;
    INT8U i;
    ST_FMT_ATTR src_fmt;
    INT8U leafNum;
    INT16U offset;
    INT8U  zipNum;
    
    pDrvDataList = &stDrvDataList[(obis & 0x0fff)];
    rsult = TRUE;
    dataList[0].pDataScrList = (ST_FIXED_LIST_ATTR *)pDrvDataList->pdatalis1;/*"此处的强制类型转换考虑处理方案"*/
    dataList[0].data_num = pDrvDataList->data_num1;
    dataList[1].pDataScrList = (ST_FIXED_LIST_ATTR *)pDrvDataList->pdatalis2;
    dataList[1].data_num = pDrvDataList->data_num2;
    
#if defined(THREE_PHASE)
    FileAttri = &stFileDataList[(file & 0x0fff)];
    if(obis == FileAttri->startFelxObj)
    {
        RCRD_Prepare_StartData(file);
    }
    else
    {
        RCRD_Prepare_EndData(file);
    }
#endif
    offset = 0;
    zipNum = 0;
    dataLen = 0;
    for(i = 0; i < 2; i++)
    {
        pDataScrListTmp = dataList[i].pDataScrList;
        dataNumTmp = dataList[i].data_num;
        if(NULL != pDataScrListTmp)/*"记录文件内容数据源列表不为空，获取源数据并进行相应格式转换，否则不做任何处理"*/
        {
            for(uni = 0; uni < dataNumTmp; uni++)
            {
                if(i == 0)
                {
                    leafNum = (INT8U)(uni + 1);
                }
                else
                {
                    leafNum = (INT8U)(uni + 1 + dataList[0].data_num);
                }
                if(_OUT_ZIP_RANGE != ChkDrvObjZip(obis, leafNum))
                {
                    offset = zipNum;
                    zipNum++;
                }
                ////    
                cur_len = FRMT_sizeofFormat(stFormatList[pDataScrListTmp->format_id].format);
                if(cur_len > MAX_LEN)
                {
                    return FALSE;
                }
                res = GetSingle(pDataScrListTmp->obis + offset , BuffTemp);

                GetFrmt(pDataScrListTmp->obis, &src_fmt); /*" 原始格式 "*/

                if(res > MS_SUCCESS)
                {
                    if((dataLen + cur_len) > MAX_DATA_BUFF_LEN)
                    {
                        return FALSE;
                    }
                    //LIB_MemCpy(BuffTemp, (INT8U *) &Buff[dataLen], cur_len);
                    FRMT_DataChange(BuffTemp, &Buff[dataLen], src_fmt, stFormatList[pDataScrListTmp->format_id]);
                    dataLen += cur_len;
                } 
                else
                {
                    rsult = FALSE;
                    break;
                }
                
                ////pDataScrListTmp++;
                if(_IN_ZIP_RANGE != ChkDrvObjZip(obis, leafNum))
                {
                    pDataScrListTmp++;
                    zipNum = 0;
                    offset = 0;
                }
            }
        }
    }

    *Len = dataLen;
    return rsult;
}

INT8U RCRD_SaveData(EN_RCRD_FILE_ID file, EN_FELXOBJ_ID obis)
{
    //INT16U   ObjListOBIStmp;      /*"记录数据关联对象列表对象标识"*/
    //INT8U    ObjListNum;      /*"记录数据关联对象列表对象数"*/
    
    const ST_FELXOBJ_DATA_ATTR  *pDrvDataList;
    const ST_FILE_ATTR  *FileAttri;
    //INT8U uni;
    INT8U Buff[MAX_DATA_BUFF_LEN];
    //INT8U BuffTemp[MAX_LEN];
    INT16U dataLen;
    INT8U rsult;
    //INT16S res;
    INT32U rcrdCnt;
    ENUM_FS_ID FileName;
    INT32U StartAdrr;
    //INT8U cur_len;
    INT16U timesAddr;
    ST_OAD RoadList[MAX_RCRD_ROAD_NUM];/*"事件对应关联对象列表"*/
    INT16U file_len;
    INT16U st_len;
    INT16U end_len;
    #ifdef MAIN_METER
    ST_RPT_ACTION rpt;
    #endif
    INT16U per_space;
    //INT8U i;

    //LRm_eng_test();
    if(rcrd_id_is_error(file))
    {
        return FALSE;
    }
#if defined(THREE_PHASE)
    cur_save_rcrd = file;
    cur_data_obis = obis;
#endif    
    FileAttri = &stFileDataList[(file & 0x0fff)];
    pDrvDataList = &stDrvDataList[(obis & 0x0fff)];

    /*"派生类对象标识错误"*/
    if((obis != FileAttri->startFelxObj) && (obis != FileAttri->endFelxObj))
    {
        return FALSE;
    }

    meter_type = GetMeterType();
#if 0
    ObjListOBIStmp = pDrvDataList->ObjListOBIS;
    ObjListNum = pDrvDataList->data_num2;
    if(ObjListNum > 0)
    {
        if(file < E_EVT_END_RCRD) /*事件可配关联对象列表获取*/
        {
            rsult = Get_Evt_Road(ObjListOBIStmp, (INT8U *) &RoadList[0], ObjListNum);
        }
        else/*冻结的需要缓冲并转换,冻结关联对象为8字节*/
        {
            rsult = Get_Frz_Road(ObjListOBIStmp, (INT8U *) &RoadList[0], ObjListNum);
        }
    }
    else
    {
        LIB_MemSet(0x00, (INT8U *) &RoadList[0], sizeof(RoadList));
        rsult = TRUE;
    }
#endif
    file_len = RCRD_get_st_end_len(file, &st_len, &end_len, &RoadList[0]);        

    //rsult = TRUE;
    if((obis == pDrvDataList->obis) && (FileAttri->obis == file))
    {
        get_rcrd_data_to_buf(file, obis, Buff, &dataLen);
#if 0
        /*"关联对象列表数据保存"*/
        if(0 != ObjListOBIStmp)
        {
            INT8U dataType;/*"关联对象属性特征值"*/
            
            if(ObjListNum > MAX_RCRD_ROAD_NUM)
            {
                rsult = FALSE;	
                return rsult;
            }
            
            for(uni = 0; uni < ObjListNum; uni++)
            {
                if((RoadList[uni].Oi.asInt == 0) && (RoadList[uni].AId == 0))
                {
                    break;
                }
                else
                {
                    dataType = RoadList[uni].AId; /*判定属性特性bit5~7*/
                    if((((obis == FileAttri->startFelxObj) && (file < E_EVT_END_RCRD)) && (AidType_is_evt_start(dataType)))|| /*"事件起始OAD"*/
                       (((obis == FileAttri->endFelxObj) && (file < E_EVT_END_RCRD)) && (AidType_is_evt_end(dataType))) ||    /*"事件结束OAD"*/
                       ((obis == FileAttri->startFelxObj) && (file > E_EVT_END_RCRD)) /*冻结类的关联OAD*/
                       )
                    {
                        rsult = Get_Oad_Data_To_Buf(&RoadList[uni], &Buff[dataLen], 
                            MAX_DATA_BUFF_LEN-dataLen, &cur_len);
                        if(rsult == TRUE)
                        {
                            dataLen += cur_len;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
         }
#endif
        /*" 加个防错处理，最后可以删除 "*/
        /*
        if(obis == FileAttri->startFelxObj)
        {
            if(dataLen != st_len)
            {
                return FALSE;
            }
        }
        else
        {
            if(dataLen != end_len)
            {
                return FALSE;
            }
        }
        */
        
        if(dataLen > 0)
        {
            /*从内卡读次数*/
            rsult = GetRcrdCurtCnt(file, &rcrdCnt);
            if(rsult == FALSE)
            {
                return FALSE;
            }

            if(obis == FileAttri->endFelxObj)
            {
                if(rcrdCnt != 0)
                {
                    /*"事件发生次数在事件起始时刻加1，事件结束仍需写入上次事件，因此结束是先还原到上次文件位置写结束数据"*/
                    rcrdCnt--;
                }
            }
            per_space = RCRD_get_one_rcrd_max_sz(file);
            StartAdrr = RCRD_get_data_addr_off((ST_FILE_ATTR  *)FileAttri, per_space, rcrdCnt);
            if(obis == FileAttri->endFelxObj)
            {
                /*"记录结束数据，需计算结束数据保存位置偏移，偏移量为起始数据记录长度"*/
                StartAdrr += st_len;
                if(st_len < per_space)
                {
                    if(dataLen > (per_space - st_len))
                    {
                        dataLen = (per_space - st_len);
                    }
                }
                else
                {
                    dataLen = 0;
                }
            }
            else
            {
                if(dataLen > per_space)
                {
                    dataLen = per_space;
                }
            }
            
            FileName = (ENUM_FS_ID)FileAttri->file;
            if(obis == FileAttri->startFelxObj)
            {/*"事件起始，先清空整条记录，然后存起始记录，避免最近一次起始结束错位问题"*/
                if((FileAttri->endFelxObj != 0) && (FileName < E_FILE_FLASH_START))
                {/*" 只有内卡中的文件存储前要擦除要存储的内卡空间 "*/
                    rsult = FS_DelFile(FileName, StartAdrr, file_len);
                }
            }

            if(rsult != FALSE)
            {
                rsult = FS_WriteFile(FileName, StartAdrr, (INT8U *)&Buff[0], dataLen);
            }

            /*"事件起始，事情发生次数加1"*/
            if((obis == FileAttri->startFelxObj) && (FALSE != rsult))
            {
                MCU_PwrChkCloseINT();
                rcrdCnt++;
                timesAddr = RCRD_CNT_OFF(file);
                rsult = FS_WriteFile(E_FILE_RCRD_DATA, timesAddr, (INT8U *)&rcrdCnt, 4);
                if(TRUE == rsult)
                {
                    RCRD_SpecProcAfterSave(file);
                }
                MCU_PwrChkOpenINT();
            }
        }
    }
    else/*"派生类对象标识错误,或者记录文件对象标识错误"*/
    {
        rsult = FALSE;
    }

    if(is_private_rcrd(file))
    {
        return  rsult;
    }
#ifdef MAIN_METER    
    if(file < E_EVT_END_RCRD)
    {
        rpt.rcrd_obis = file;
        if(obis == FileAttri->startFelxObj)
        {
            rpt.mode = 1;
            Action(E_SETORCLR_RPTEVTLIST, &rpt, NULL);
        }
        else if(obis == FileAttri->endFelxObj)
        {
            rpt.mode = 2;
            Action(E_SETORCLR_RPTEVTLIST, &rpt, NULL);
        }
    }
#endif
    return  rsult;
}

/*" 检查某次事件是否结束 , 结束则获取结束时间到buf "*/
INT8U check_evt_is_end(EN_RCRD_FILE_ID rcrd_obis, INT32U seq, INT8U *buf)
{
    //ST_OAD  timeOad;
    INT16U cur_len;
    ST_FMT_ATTR fmt_save;
    //ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    
    //timeOad.Oi.asInt = 0x2020; /*"最近一次结束时间"*/
    //timeOad.AId = 2;
    //timeOad.IId = 0;
    LIB_MemSet(0x00, buf, 6);
    //cur_len = RCRD_ReadRcrdDataByRoad(rcrd_obis, seq, &timeOad, buf, &fmt_save, &fmt_dst[0], road_list);
    cur_len = RCRD_ReadRcrdData(rcrd_obis, 1, 0x81, buf, &fmt_save);
    if((cur_len == 6) && (LIB_ChkDateTime((ST_6TIME_FORM *)buf) == TRUE))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

/* 最近一次发生时间 最近一次结束时间 */
INT8U get_last_evt_start_end_tm(EN_RCRD_FILE_ID rcrd_obis, INT8U *dst)
{
    INT8U buf[20];
    INT8U len;
    INT8U cur_len;
    INT8U i;
    INT8U is_end;
    //INT16U addr;
    INT32U rcrdCnt;
    //ST_OAD  timeOad;
    ST_FMT_ATTR fmt_save;
    //ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    //INT8U OadList[ROADINFO_BUFF_LEN];
    //ST_OAD RoadList[MAX_RCRD_ROAD_NUM];/*"事件对应关联对象列表"*/
    //INT16U sum_len;

    if(rcrd_id_is_error(rcrd_obis) == TRUE)
    {
        return 0;
    }
#ifdef THREE_PHASE
    if(rcrd_obis == E_R_ENGY_COM_PRG_EVNT_RCRD || rcrd_obis == E_R_ENGY_COM_PRG_SRC2_RCRD)
    {
        if(rcrd_obis == E_R_ENGY_COM_PRG_EVNT_RCRD)
        {
            FS_ReadFile(E_FILE_RCRD_DATA, R_ENG_COM1_PRG_TIME_OFF, buf, 6);
        }
        else
        {
            FS_ReadFile(E_FILE_RCRD_DATA, R_ENG_COM2_PRG_TIME_OFF, buf, 6);
        }
        if(LIB_ChkDateTime((ST_6TIME_FORM *)&buf[0]) == FALSE)
        {
            return 0;
        }
        else
        {
            LIB_MemCpy(&buf[0], dst, 6);
            return 6;
        }
    }
#endif    
    GetRcrdCurtCnt(rcrd_obis, &rcrdCnt);
    if(rcrdCnt == 0)
    {
        return 0;
    }
    else
    {
        /*
        GetRcrdRoad(rcrd_obis, (INT8U *)&RoadList[0], &sum_len);
        if(sum_len == 0)
        {
            LIB_MemSet(0x00, (INT8U *)&RoadList[0], sizeof(RoadList));
        }
        */
        //timeOad.Oi.asInt = 0x201E; /*"最近一次起始时间"*/
        //timeOad.AId = 2;
        //timeOad.IId = 0;
        LIB_MemSet(0x00, &buf[0], 12);
        cur_len = RCRD_ReadRcrdData(rcrd_obis, 1, 0x01, buf, &fmt_save);
        //cur_len = RCRD_ReadRcrdDataByRoad(rcrd_obis, rcrdCnt-1, &timeOad, buf, &fmt_save, &fmt_dst[0], RoadList);
        if(RCRD_EvtCanGetEndTm(rcrd_obis) && (cur_len == 6))
        {
            is_end = check_evt_is_end(rcrd_obis, rcrdCnt-1, &buf[6]);
            if(is_end == 0)
            {
                if(rcrdCnt < 2)
                {
                    LIB_MemSet(0x00, &buf[6], 6);
                }
                else
                {
                    is_end = check_evt_is_end(rcrd_obis, rcrdCnt-2, &buf[6]);
                }
            }
        }
        else
        {
            LIB_MemSet(0x00, &buf[6], 6);
        }
    }

    len = 0;
    for(i=0; i<2; i++)
    {
        if(LIB_ChkDateTime((ST_6TIME_FORM *)&buf[i*6]) == FALSE)
        {
            
        }
        else
        {
            //len += FRMT_DataChange(&buf[i*6], dst + i*7, fmt_save, fmt_dst[0]);
            LIB_MemCpy(&buf[i*6], dst + i*6, 6);
            len += 6;
        }
    }
    return len;
}


/*"  	设置负荷记录间隔时间 "*/
INT8U Set_LoadRecoedPeriod(INT8U *pdata)
{
    INT16U result = 0;

    result = pdata[0] | (pdata[1] << 8);

    if((result > 0x99) || (result == 0))
    {
        return FALSE;
    }
    else 
    {
        return TRUE;
    }
}

/*"  	设置负荷记录起始时间 "*/
INT8U Set_LoadRecoedBeginTime(INT8U *pdata)
{
    INT8U n;
    INT8U data[4];
    ST_6TIME_FORM time;
    ST_7TIME_FORM now;

    for(n=0; n<4; n++)
    {
        data[n] = pdata[3 - n];
    }
    if(LIB_CheckBCD(data, 4) == FALSE)
    {
        return FALSE;
    }
    
    if(LIB_ChkValue(data, 4, 0x00) != EQUAL)
    {
        GetSingle(E_SYS_TIME, (INT8U *)&now);/*"取系统时间"*/
        time.year = now.year;
        LIB_BcdToCharNByte(data, 4);
        LIB_MemCpy(data, &time.month, 4);
        time.sec = 0;
        if(LIB_ChkDateTime(&time) != TRUE)
        {
            return FALSE;
        }
    }

    LR_ClrRecord();

    return TRUE;
}


INT8U Set_Rcrd_Para_Do(INT16U id, INT8U *pData, INT16U len)
{
    INT16U prd;
    INT8U ch;
    ST_7TIME_FORM now;
    INT16S res;
    
    if(id >= E_LD_TYPE1_PERIOD && id < (E_LD_TYPE1_PERIOD + MIN_FRZ_CLASS_NUM))
    {
        prd = pData[0] | (pData[1] << 8);
        ch = id - E_LD_TYPE1_PERIOD;
        if(prd != rcrd_para.LrPara.LoadRecordPeriod[ch])
        {
            IndexRefFlg[ch] = 1;
        }
    }
    else if(id == E_LD_ST_TIME)
    {
        res = GetSingle(E_SYS_TIME, (INT8U *)&now);/*"取系统时间"*/
        if(res >= MS_SUCCESS)
        {
            rcrd_para.LrPara.LoadRecoedBeginTime.year = now.year;
        }
    }
    return TRUE;
}

/*"*****************************************************************************"*/
/*"  Function:       RCRD_Get"*/
/*"  Description:    记录快照模块数据对象抄读接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      message模块"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"  Output:         pBuff     数据接收缓存地址指针"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT16S RCRD_Get(INT16U id, INT8U *pBuff)
{
    INT16U ListId, dataLen;
    INT16U rcrd_obis;
    INT32U rcrdCnt, maxCnt, curcnt;
    INT8U len;
    ENUM_FS_ID file;
    INT32U off;
#ifdef THREE_PHASE
    INT32U src_tms;
    INT32U sum_tms;
#endif
    
    if(id >= E_PWR_DOWN_EVNT_RCRD && id <= E_STEP_BILL_FRZ_RCRD)
    {
        return 0;//GetRcrdRoadListNum((EN_RCRD_FILE_ID)id, pBuff);
    }
    else if((id >= E_RCRD_CNT_BLK) && (id <= E_RCRD_CNT_BLK + MAX_ALL_RCRD))
    {
        rcrd_obis = E_PWR_DOWN_EVNT_RCRD + (id - E_RCRD_CNT_BLK);
        GetRcrdCurtCnt((EN_RCRD_FILE_ID)rcrd_obis , (INT32U *)pBuff);
        return 4;
    }
    else if((id >= E_RCRD_CUR_CNT) && (id <= E_RCRD_CUR_CNT + MAX_ALL_RCRD))
    {/*" 当前记录数 "*/
        rcrd_obis = E_PWR_DOWN_EVNT_RCRD + id - (E_RCRD_CUR_CNT + 1);
        GetRcrdCurtCnt((EN_RCRD_FILE_ID)rcrd_obis, &rcrdCnt);
        curcnt = rcrdCnt;
        maxCnt = GetRcrdMaxtCnt((EN_RCRD_FILE_ID)rcrd_obis);
        if(curcnt > maxCnt)
        {
            curcnt = maxCnt;
        }
        LIB_MemCpy((INT8U *)&curcnt, pBuff, 2);
        return 2;
    }
    else if((id >= E_RCRD_MAX_CNT) && (id <= E_RCRD_MAX_CNT + MAX_ALL_RCRD))
    {/*" 最大记录数 "*/
        rcrd_obis = E_PWR_DOWN_EVNT_RCRD + id - (E_RCRD_MAX_CNT + 1);
        maxCnt = GetRcrdMaxtCnt((EN_RCRD_FILE_ID)rcrd_obis);
        LIB_MemCpy((INT8U *)&maxCnt, pBuff, 2);
        return 2;
    }
    else if((id >= E_RCRD_CUR_VALUE) && (id <= E_RCRD_CUR_VALUE + MAX_EVT_RCRD))
    {/*" 当前值记录表 "*/
        rcrd_obis = E_PWR_DOWN_EVNT_RCRD + id - (E_RCRD_CUR_VALUE + 1);
#ifdef THREE_PHASE
        if(rcrd_obis == E_R_ENGY_COM_PRG_EVNT_RCRD || rcrd_obis == E_R_ENGY_COM_PRG_SRC2_RCRD)
        {
            GetRcrdCurtCnt(E_R_ENGY_COM_PRG_EVNT_RCRD, &rcrdCnt);
            FS_ReadFile(E_FILE_RCRD_DATA, R_ENG_COM2_PRG_TMS_OFF, (INT8U *)&src_tms, 4);
            if(rcrd_obis == E_R_ENGY_COM_PRG_EVNT_RCRD)
            {
                rcrdCnt = rcrdCnt - src_tms;
            }
            else
            {
                rcrdCnt = src_tms;
            }
            LIB_MemCpy((INT8U *)&rcrdCnt, pBuff, 4);
            return 4;
        }
#endif
        GetRcrdCurtCnt((EN_RCRD_FILE_ID)rcrd_obis, &rcrdCnt);
        LIB_MemCpy((INT8U *)&rcrdCnt, pBuff, 4);
        len = 4;
        return len;
    } 
    else if((id >= E_RCRD_TIME_STATUS) && (id <= E_RCRD_TIME_STATUS + MAX_EVT_RCRD))
    { /*" 时间状态记录表"*/
        rcrd_obis = E_PWR_DOWN_EVNT_RCRD + id - (E_RCRD_TIME_STATUS + 1);
        return get_last_evt_start_end_tm((EN_RCRD_FILE_ID)rcrd_obis, pBuff);
    }
    else if((id >= E_MOUTH_BILLDAY) && (id <= E_MOUTH_BILLDAY3))
    {
        return GetBillDate(id, pBuff);
    }
#ifdef THREE_PHASE
    else if(id == LOST_VOLT_SUM_CNTS)
    {
        sum_tms = 0;
        for(rcrd_obis=E_LOST_VOLT_LA_EVNT_RCRD; rcrd_obis<=E_LOST_VOLT_LC_EVNT_RCRD; rcrd_obis++)
        {
            GetRcrdCurtCnt((EN_RCRD_FILE_ID)rcrd_obis, &curcnt);
            sum_tms += curcnt;
        }
        LIB_MemCpy((INT8U *)&sum_tms, pBuff, 4);
        return 4;
    }
#endif
    else
    {
        ST_SINGLE_DATA_ATTR stSingleDataAttr;

        ListId = (id - 0x6E00);
        if(ListId < RCRD_SINGLE_LIST_NUM)
        {
            stSingleDataAttr = stRcrdSingleDataList[ListId];
            dataLen = FRMT_sizeofFormat(stFormatList[stSingleDataAttr.format_id].format);
            if(stSingleDataAttr.addr >= RAM) /*"RAM address"*/
            {
                LIB_MemCpy((INT8U *)(stSingleDataAttr.addr - RAM), pBuff, (INT8U)dataLen);
            }
            else /*"EEPROM address"*/
            {
                file = (ENUM_FS_ID)(stSingleDataAttr.addr >> 16);
                off = stSingleDataAttr.addr & 0xffff;
                FS_ReadFile(file, off, pBuff, dataLen);
            }
            return dataLen;
        }

    }
    return MS_OBIS_ERR;
}


/*"*****************************************************************************"*/
/*"  Function:       RCRD_Set"*/
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
INT8S RCRD_Set (INT16U id, INT8U *pData, INT16U len)
{
    //ST_ACTION_DATa road_op;
    //INT8S dar;
    INT8U rsult;
    INT16U off;
    ST_SINGLE_DATA_ATTR stSingleDataAttr;
    INT16U ListId;

    if(id >= E_PWR_DOWN_EVNT_RCRD && id <= E_STEP_BILL_FRZ_RCRD)
    {
        mSetROadLstFlg = 0x55;
/*
        road_op.Obis = id;
        road_op.pdata = pData;
        road_op.len = len;
        dar = RCRD_AddMultRoad(&road_op, NULL);
        mSetROadLstFlg = 0;
*/
        return 0;
    }
    else if(id >= E_MOUTH_BILLDAY && id <= E_MOUTH_BILLDAY3)
    {
        return setBillDay(id, pData, len);
    }
    else
    {
        /*"关闭掉电检测中断"*/
        MCU_PwrChkCloseINT();
        ListId = (id - 0x6E00);
        if(ListId < RCRD_SINGLE_LIST_NUM)
        {
            stSingleDataAttr = stRcrdSingleDataList[ListId];
            off = stSingleDataAttr.addr & 0xffff;

            Set_Rcrd_Para_Do(id, pData, len);

            LIB_MemCpy(pData, (INT8U *)&rcrd_para + off, len);

            if(id == E_LD_ST_TIME)
            {
                rsult = FS_WriteFile(E_FILE_RCRD_PARA, RCRD_PARA_LD_ST_TM_OFF, 
                    (INT8U *)&rcrd_para.LrPara.LoadRecoedBeginTime, 6);
            }
            else
            {
                rsult = FS_WriteFile(E_FILE_RCRD_PARA, off, pData, len);
            }
            
            rcrd_para.Crc = CRC16((INT8U *)&rcrd_para, FIND(ST_RCRD_PARA_TABLE, Crc));
            FS_WriteFile(E_FILE_RCRD_PARA, RCRD_PARA_TBL_CRC_OFF, (INT8U *)&rcrd_para.Crc, sizeof(CRC_DATA_TP));
        }
        /*"开启掉电检测中断"*/
        MCU_PwrChkOpenINT();
        if(rsult == TRUE)
        {
            return DAR_0_OK;
        }
        else
        {
            return MS_OBIS_ERR;
        }
    }
    //rsult = exChangeData(id, pData, W_MODE);
    //return MS_OBIS_ERR;
}


/*"*****************************************************************************"*/
/*"  Function:       RCRD_Action"*/
/*"  Description:    记录快照模块方法访问接口函数"*/
/*"  Calls:          "*/
/*"  Called By:      MS"*/
/*"  Input:          id        对象标识  "*/
/*"                  pData     输入数据指针"*/
/*"  Output:         pbuff     输出数据缓存指针"*/
/*"  Return:         DAR"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8S RCRD_Action (INT16U id, void * pdata, void * pbuff)
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
        rsult = stRcrdActionList[FunId].Fuction(pdata, pbuff);
    }
    return rsult;
}

/*"*****************************************************************************"*/
/*"  Function:       RCRD_GetFrmt"*/
/*"  Description:    获取指定ID对象数据格式"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块函数"*/
/*"  Input:          id        对象标识，通过对象标准名称进行对象访问  "*/
/*"  Output:         "*/
/*"  Return:         format id"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U RCRD_GetFrmt (INT16U id)
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
            stBlkDataAttr = stRcrdBlkDataList[dataId >> 7];
            frmtId = stBlkDataAttr.format_id;
        }
        else
        {
            ST_SINGLE_DATA_ATTR stSingleDataAttr;
            stSingleDataAttr = stRcrdSingleDataList[dataId - 0x0E00];
            frmtId = stSingleDataAttr.format_id;
        }
    }

    return (frmtId);
}

INT8S RCRD_ClrRecordData(EN_RCRD_FILE_ID obis)
{
    if(rcrd_id_is_error(obis))
    {
        return MS_OBIS_ERR;
    }
    
    if(obis == E_MIN_FRZ_RCRD)
    {

        LR_ClrRecord();
    }
    else
    {
        ClrRcrdCurCnt(obis, 0); 
    }
    
    return MS_SUCCESS;
}

/*" 修改关联对象时，删除记录相关所有数据 "*/
INT8S RCRD_ClrDataMdfRoad(EN_RCRD_FILE_ID rcrd_obis)
{
    RCRD_ClrRecordData(rcrd_obis);/*"清除记录数据"*/
    ///*.清事件发生标志和累计时间*/
    //Action(E_MNT_EVNT_CLR,&rcrd_obis,NULL); 
    return MS_SUCCESS;
}

INT8S RCRD_ClrRecord(void *pdin, void *pdout)
{
    INT16U obis;
    
    LIB_MemCpy((INT8U*)pdin, (INT8U *)&obis, 2);
    if(obis == 0xfffe) /*" 事件总清零 "*/
    {
        Clr_All_Evt_Data();
        return MS_SUCCESS;
    }
    else if(obis == 0xffff) /*" 电表总清零 "*/
    {
        Clr_All_Rcrd_Data();
        LR_ClrRecord();
        return MS_SUCCESS;
    }
    return RCRD_ClrRecordData((EN_RCRD_FILE_ID)obis);
}

INT8S RCRD_Trigger_Rcrd(void *pdin, void *pdout)
{
    INT16U obis;
    
    LIB_MemCpy((INT8U*)pdin, (INT8U *)&obis, 2); 
    if(obis == E_MMT_FRZ_RCRD)
    {
        RCRD_SaveData(E_MMT_FRZ_RCRD, E_FRZ_LST_MON_FRZ);
        return MS_SUCCESS;
    }
    else
    {
        return MS_ACTION_AUTH_ERR;
    }
}

void erase_flash_sector_part_data(ENUM_FS_ID flname, INT32U StartAdrr)
{
    INT32U len;
    
    if((StartAdrr % FLASH_SECTOR_SZ) != 0)
    {
        len = FLASH_SECTOR_SZ - (StartAdrr % FLASH_SECTOR_SZ);
        FS_DelFile(flname, StartAdrr, len);
    }
}

/*" 擦除序号seq开始的记录 "*/
void erase_flash_history_rcrd(EN_RCRD_FILE_ID rcrd_obis, INT32U seq)
{
    const ST_FILE_ATTR  *FileAttri;
    INT16U per_space;
    INT32U StartAdrr;
    ENUM_FS_ID flname;

    FileAttri = &stFileDataList[(rcrd_obis & 0x0fff)];
    flname = (ENUM_FS_ID)FileAttri->file;
    if(flname > E_FILE_FLASH_START)
    {
        per_space = RCRD_get_one_rcrd_max_sz(rcrd_obis);
        StartAdrr = RCRD_get_data_addr_off((ST_FILE_ATTR  *)FileAttri, per_space, seq);
        erase_flash_sector_part_data(flname, StartAdrr);
    }
            
}

INT8S Clr_Frz_From_Time(EN_RCRD_FILE_ID rcrd_obis, ST_6TIME_FORM *st)
{
    INT8U i, maxNum, rsult;
    INT8U *pTime;
    INT8U data[7];
    INT8U buff[7];
    INT32U rcrdCnt;
    //ST_FMT_ATTR fmt_save;
    //ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];
    //INT32U seq_no;
    //ST_OAD cur_oad;
    //ST_OAD RoadList[MAX_RCRD_ROAD_NUM];/*"事件对应关联对象列表"*/
    //INT16U   ObjListOBIStmp;      /*"记录数据关联对象列表对象标识"*/
    //INT8U    ObjListNum;      /*"记录数据关联对象列表对象数"*/
    //const ST_FELXOBJ_DATA_ATTR  *pDrvDataList;
    //const ST_FILE_ATTR  *FileAttri;
    ST_FMT_ATTR fmt;

    if(rcrd_obis > E_STEP_BILL_FRZ_RCRD)
    {
        return FALSE;
    }
    
    //FileAttri = &stFileDataList[(rcrd_obis & 0x0fff)];
    //pDrvDataList = &stDrvDataList[(FileAttri->startFelxObj & 0x0fff)];
#if 0
    ObjListOBIStmp = pDrvDataList->ObjListOBIS;
    ObjListNum = pDrvDataList->data_num2;
    if(rcrd_obis < E_EVT_END_RCRD) /*事件可配关联对象列表获取*/
    {
        rsult = Get_Evt_Road(ObjListOBIStmp, (INT8U *)&RoadList[0], ObjListNum);
    }
    else if (rcrd_obis > E_EVT_END_RCRD && rcrd_obis <= E_STEP_BILL_FRZ_RCRD)/*冻结的需要缓冲并转换,冻结关联对象为8字节*/
    {
        rsult = Get_Frz_Road(ObjListOBIStmp, (INT8U *)&RoadList[0], ObjListNum);
    }
#endif    
    /*从内卡读次数*/
    rsult = GetRcrdCurtCnt(rcrd_obis, &rcrdCnt);
    if(rsult == FALSE)
    {
        return FALSE;
    }

    maxNum = (INT8U)GetRcrdRealMaxtCnt(rcrd_obis); /*" 分钟冻结不走此分支，所以最大返回值为254 "*/
    if(rcrdCnt < maxNum)
    {
        maxNum = (INT8U)rcrdCnt;
    }

    pTime = (INT8U *)st;
    //cur_oad.Oi.asInt = 0x2021;
    //cur_oad.AId = 2;
    //cur_oad.IId = 0;
    for(i = maxNum; i > 0 ; i--) /*" 从上N次开始往上1次查找 "*/
    {
        //seq_no = rcrdCnt - i;
        //RCRD_ReadRcrdDataByRoad(rcrd_obis, seq_no, &cur_oad, buff, &fmt_save, &fmt_dst[0], &RoadList[0]);/*"获取时间叶子数据"*/        
        RCRD_ReadRcrdData(rcrd_obis, i, 1, buff, &fmt);
        if(SMALL != LIB_MemCmp(buff, pTime, sizeof(ST_6TIME_FORM)))
        {
            if(i == maxNum)
            {
                rcrdCnt = 0;
            }
            else
            {
                rcrdCnt -= i;
            }

            MCU_PwrChkCloseINT();
            ClrRcrdCurCnt(rcrd_obis, rcrdCnt);
#if defined(THREE_PHASE)
            erase_flash_history_rcrd(rcrd_obis, rcrdCnt);
#endif
            switch(rcrd_obis)
            {
                case E_HOUR_FRZ_RCRD:/* 小时*/
                    real_prd_frz_num_dec(RCRD_HOUR_FRZ_NUM_OFF, i);
                    break;
                case E_DAY_FRZ_RCRD:/* 日冻结 */
                    real_prd_frz_num_dec(RCRD_DAY_FRZ_NUM_OFF, i);
                    LIB_MemSet(0x00, data, 6);
                    FS_WriteFile(E_FILE_RCRD_DATA, DAY_FRZ_LST_FRZ_DATE, data, 6);
                    break;
                case E_MONTH_FRZ_RCRD: /*"月冻结"*/
                    real_prd_frz_num_dec(RCRD_MOUTH_FRZ_NUM_OFF, i);
                    break;
            }
            MCU_PwrChkOpenINT();
            
            return TRUE;
        }
    }

    return FALSE;
}

INT8S RCRD_Time_Adjust(void *pdin, void *pdout)
{
    ST_TIME_ADJUST_INPUT *pTimeAdjustIn;

    pTimeAdjustIn = pdin;
    if(BIG == LIB_MemCmp((INT8U *)&pTimeAdjustIn->TimeOld, (INT8U *)&pTimeAdjustIn->TimeNew, sizeof(ST_6TIME_FORM)))/*"时钟向前调整，需维护索引表"*/
    {
        Clr_Frz_From_Time(E_HOUR_FRZ_RCRD, &pTimeAdjustIn->TimeNew);
        Clr_Frz_From_Time(E_DAY_FRZ_RCRD, &pTimeAdjustIn->TimeNew);
        Clr_Frz_From_Time(E_MONTH_FRZ_RCRD, &pTimeAdjustIn->TimeNew);
        AdjustTm_Clr_Powerup_Frz_Info(E_MONTH_BILL_FRZ_RCRD, &pTimeAdjustIn->TimeNew);
        AdjustTm_Clr_Powerup_Frz_Info(E_STEP_BILL_FRZ_RCRD, &pTimeAdjustIn->TimeNew);
    }
    
    LR_TimeAdjust(pdin);
    return MS_SUCCESS;
}

INT8S RCRD_Pd_Action(void *pdin, void *pdout)
{
#if defined(THREE_PHASE)
    upd_evt_amp_eeprom();
#endif
    return MS_SUCCESS;
}
