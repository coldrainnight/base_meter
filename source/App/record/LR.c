/*"*****************************************************************************"*/
/*"  FileName: LR.c"*/
/*"  Author:        Version :  V1.0        Date:2018/9/3"*/
/*"  Description:     645负荷记录模块"*/
/*"                   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/

/*"  Function List:   "*/
/*"    1. "*/
/*"    2. "*/
/*"    3. "*/
/*"    4. "*/
/*"  History:         // 历史修改记录"*/
/*"  code size  xxxbyte  table size xxxbyte   ram size  "*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      quantp    2018/9/3     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "system\Sys_def.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
//#include "app\userinterface\PP\PP_698\Class\class_comm.h"
//#include "app\userinterface\PP\PP_698\Process\Pp_obis.h"
//#include "app\userinterface\PP\PP_698\Process\Pp_macro.h"
//#include "app\userinterface\PP\PP_698\Process\Pp_process.h"
#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"

#include "RCRD.h"
#include "LR.h"
#include "RCRD_dataProcess.h"

#define TMSTAMP_LEN     4
#define TMSTAMP_MSK     0x00FFFFFFUL

#define INVALID_SEQ     0xFFFFFFFFUL


#define MAX_DOT_LEN     256L /*" 分钟冻结一条记录长度不能 达到256字节 "*/

#define PER_IDX_SZ  (sizeof(ST_SCHEME_INDEX_ATTR))
#define NUM_IDX_PER_SECTOR  (2048/PER_IDX_SZ)   /*" 一个扇区只使用2KB,可放256个索引 "*/
/*" 索引占扇区个数 , 目前是按每个方案深度都相同并且最大深度进行配置"*/
#define NUM_IDX_SECTOR     ( (INDEX_LIST_DEEP + NUM_IDX_PER_SECTOR - 1) / NUM_IDX_PER_SECTOR )    

/*" 负荷记录数据存储起始文件内偏移地址 "*/
#define LR_SAVE_START_ADDR  0x0L  
#define LR_END_ADDR         (MIN_FRZ_SZ)

/*" 负荷记录索引存储起始文件内偏移地址 "*/
#define LR_IDX_START_ADDR  0x0L  

#define NUM_PER_SECTOR(per_sz) (FLASH_SECTOR_SZ / (per_sz))    /*" 计算一个扇区内记录条数 "*/
//#define SUM_SECTOR(deep, num_per_sec) (((deep) / (num_per_sec)) + 2) /*" 计算需要扇区数 "*/


NO_INIT static ST_LR_CTR_DATA_FORM mLrCtrlInfor[MIN_FRZ_CLASS_NUM];/*"分钟冻结控制信息"*/

static INT8U lr_mask;

INT8U IndexRefFlg[MIN_FRZ_CLASS_NUM]; /*"记录索引表异常点维护标志（掉电、时间调整）TRUE需新增  FALSE不需要新增"*/
static ST_SCHEME_ATTR  mSchemeAttr;/*"方案属性信息"*/
void LR_Init(INT8U mode);
void LR_Main(void);
INT8U LR_ClrRecord(void);

static void getSchemeAttr(void);
static void FrzChk(void);
static INT8U saveLrData(INT8U channel);
static void RangeIdAdd(INDEX_RANGE_TYPE * RangeId);
static INDEX_RANGE_TYPE SearchTimeStamp(INT8U Channel, INT32U *pTimeStart, INT32U *pTimeEnd, INDEX_RANGE_TYPE *dotID);
static INT16U BinarySearchIteration(INT16U *Range, INT32U TimeStamp, INT8U channel);
static INT8U  SearchDotInstamp(INDEX_RANGE_TYPE stampId, INT8U Channel, INT32U inTime, INT32U * end_tm, INT32U *pRcrdId);
static void GetIndexAttr(INT8U channel, INDEX_RANGE_TYPE IndexId, ST_SCHEME_INDEX_ATTR *IndexAttr);
 void MinToDateTime(INT32U Min, ST_6TIME_FORM *pDateTime);


extern void PP_InitFrzMinPrjList(void);

/*" 获取索引在记录文件中的偏移地址 "*/
INT32U get_idx_addr(INT8U channel, INDEX_RANGE_TYPE IndexId)
{
    INT32U addr;
#ifdef IDX_IN_FLASH
    INT32U off;
    off = (IndexId / NUM_IDX_PER_SECTOR) * 4096L + (IndexId % NUM_IDX_PER_SECTOR) * PER_IDX_SZ;
    addr = LR_IDX_START_ADDR + channel * (NUM_IDX_SECTOR*4096L) + off;
#else
    /*" 内卡里面的为内部偏移 "*/
    addr = channel * PER_IDX_SZ * INDEX_LIST_DEEP + IndexId * PER_IDX_SZ;
#endif
    return addr;
}

INT32U LR_get_rcrd_save_addr(INT8U ch, INT32U seq)
{
    INT32U SaveAddr;
    ST_SCHEME_ATTR_FORM  *SchemeAttrTmp;
    
    SchemeAttrTmp = &mSchemeAttr.SchemeAttr[ch];
#ifdef LD_DATA_IN_FLASH
    INT16U num_per_sec;
    INT16U sum_sector;
    INT32U addr_off;

    num_per_sec = NUM_PER_SECTOR(SchemeAttrTmp->DotLen);
    sum_sector = SchemeAttrTmp->sum_sector;
    addr_off = get_data_addr_off(num_per_sec, sum_sector, SchemeAttrTmp->DotLen, seq);
    SaveAddr = SchemeAttrTmp->startAddr + addr_off;
#else
    SaveAddr = SchemeAttrTmp->startAddr + (INT16U)(seq %
        (SchemeAttrTmp->deep + MAX_DOT_NUM_PER_STAMP)) * SchemeAttrTmp->DotLen;
#endif
    return SaveAddr;
}

/*"***************************************************************"*/
/*"  Function:       GetIndexAttr"*/
/*"  Description:   从数据中心获取索引属性封装函数（节省空间）"*/
/*"  Input:         IndexId 索引序号"*/
/*"                 channel 指定方案"*/
/*"  Output:        IndexAttr 索引属性缓存指针"*/
/*"                 "*/
/*"  Return:         "*/
/*"****************************************************************"*/
static void GetIndexAttr(INT8U channel, INDEX_RANGE_TYPE IndexId, ST_SCHEME_INDEX_ATTR *IndexAttr)
{
    INT32U idx_addr;
    
    idx_addr = get_idx_addr(channel, IndexId);

    FS_ReadFile(E_FILE_MIN_FRZ_IDX, idx_addr, (INT8U *)IndexAttr, PER_IDX_SZ);
}

INT32U get_ch_first_rcrd_seq(INT8U ch)
{
    //ST_SCHEME_ATTR_FORM  *SchemeAttrTmp;
    INT32U seq;

    //SchemeAttrTmp = &mSchemeAttr.SchemeAttr[ch];
    if(mLrCtrlInfor[ch].dotCnt > mLrCtrlInfor[ch].real_dotCnt)
    {
        seq = mLrCtrlInfor[ch].dotCnt - mLrCtrlInfor[ch].real_dotCnt;
    }
    else
    {
        seq = 0;
    }

    return seq;
}

INT32U get_ch_last_rcrd_seq(INT8U ch)
{
    INT32U seq;

    if(mLrCtrlInfor[ch].dotCnt > 0)
    {
        seq = mLrCtrlInfor[ch].dotCnt - 1;
    }
    else
    {
        seq = INVALID_SEQ;
    }

    return seq;
}


INT32U get_ch_first_rcrd_tm(INT8U ch)
{
    ST_SCHEME_INDEX_ATTR IndexAttr;
    ST_LR_CTR_DATA_FORM  *pLrCtrlData;

    pLrCtrlData = &mLrCtrlInfor[ch];
    GetIndexAttr(ch, pLrCtrlData->startIndexId, &IndexAttr);
    return (IndexAttr.TimeStamp & TMSTAMP_MSK);
}


/*" 获取一个方案最后一个记录的时间 "*/
INT32U get_ch_lst_rcd_tm(INT32U ch)
{
    ST_LR_CTR_DATA_FORM  *pLrCtrlData;
    ST_SCHEME_INDEX_ATTR idx;
    INT16U prd;

    pLrCtrlData = &mLrCtrlInfor[ch]; 
    GetIndexAttr(ch, pLrCtrlData->endIndexId, &idx);
    prd = idx.TimeStamp >> 24;
    return ((idx.TimeStamp & TMSTAMP_MSK) + (pLrCtrlData->dotCnt - idx.startDotId - 1)*prd);
}

/*" 获得方案最大记录点数 "*/
INT32U get_ch_max_dot_num(INT8U ch)
{
    ST_SCHEME_ATTR_FORM  *SchemeAttrTmp = &mSchemeAttr.SchemeAttr[ch];
    INT32U max;

#ifdef LD_DATA_IN_FLASH
    INT16U num_per_sec;
    INT16U sum_sector;
    
    num_per_sec = NUM_PER_SECTOR(SchemeAttrTmp->DotLen);
    sum_sector = SchemeAttrTmp->sum_sector;
    max = num_per_sec * sum_sector;
#else
    max = SchemeAttrTmp->deep + MAX_DOT_NUM_PER_STAMP;
#endif
    return max;
}


void RangeStartIdMove(ST_LR_CTR_DATA_FORM *pLrCtrlInfor)
{
#ifdef IDX_IN_FLASH
    if(pLrCtrlInfor->endIndexId == pLrCtrlInfor->startIndexId)
    {
        /*"表示索引回环了，startIndexId要指向下个索引扇区的第一个索引位置 "*/
        pLrCtrlInfor->startIndexId = 
        (pLrCtrlInfor->startIndexId + NUM_IDX_PER_SECTOR - (pLrCtrlInfor->startIndexId % NUM_IDX_PER_SECTOR)) % INDEX_LIST_DEEP;
    }
#else
    if(pLrCtrlInfor->endIndexId == pLrCtrlInfor->startIndexId)
    {
        /*"表示索引回环了，startIndexId要指向下个索引扇区的第一个索引位置 "*/
        pLrCtrlInfor->startIndexId = (pLrCtrlInfor->startIndexId + 1) % INDEX_LIST_DEEP;
    }
#endif

}

/*"***************************************************************"*/
/*"  Function:       RefIndexAttr"*/
/*"  Description:   更新通道对应索引表数据"*/
/*"  Input:         channel：方案号(冻结周期一样的关联对象)"*/
/*"                 kind：0 第一个时间戳   1 非第一个时间戳"*/
/*"  Output:       无 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
static void Ref_CurIndexAttr(INT8U channel, INT8U kind)
{
    ST_LR_CTR_DATA_FORM *pLrCtrlDataTmp;
    RECORD_ADDR_TYPE idx_addr; 
    ST_SCHEME_INDEX_ATTR idx;
    INT32U lst_tm;
    INT16U period;

    if(channel > MIN_FRZ_CLASS_NUM)
    {
        return;
    }

    pLrCtrlDataTmp = &mLrCtrlInfor[channel]; 
    
    if(kind == 1)
    {
        period = rcrd_para.LrPara.LoadRecordPeriod[channel];
        //GetIndexAttr(channel, pLrCtrlDataTmp->endIndexId, &idx);

        lst_tm = get_ch_lst_rcd_tm(channel);
        if(mMinCnt != (lst_tm + period))
        {
            pLrCtrlDataTmp->NowStampDotCnt = 0;
            RangeIdAdd(&pLrCtrlDataTmp->endIndexId);
            RangeStartIdMove(pLrCtrlDataTmp);
        }
        else
        {/*" 每个时间戳里面记录都是连续的 "*/
            return;
        }
    }

    /*"当前索引记录点数已达到最大值，或异常点，需新增索引"*/
    idx.startDotId = pLrCtrlDataTmp->dotCnt;
    idx.TimeStamp = mMinCnt;
    idx.TimeStamp &= TMSTAMP_MSK;
    idx.TimeStamp |= ((INT32U)rcrd_para.LrPara.LoadRecordPeriod[channel] << 24);
    IndexRefFlg[channel] = 0;
    idx_addr = get_idx_addr(channel, pLrCtrlDataTmp->endIndexId);

    FS_WriteFile(E_FILE_MIN_FRZ_IDX, idx_addr, (INT8U *)&idx, PER_IDX_SZ);
    return;
}

/*"***************************************************************"*/
/*"  Function:       LR_Init"*/
/*"  Description:   分钟冻结模块上电初始化"*/
/*"  Input:         "*/
/*"                 "*/
/*"  Output:       无 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
void LR_Init(INT8U mode)
{
    CRC_DATA_TP CrcTmp;
    INT8U unj;
    INT8U validFlg;
    //ST_TIME_ADJUST_INPUT adjust;
    //ST_7TIME_FORM now;
    //ST_7TIME_FORM pdtime;
    //INT16S res, res1;

    mMinCnt = 0;

    getSchemeAttr();
    for(unj = 0; unj < mSchemeAttr.SchemeNum; unj++)
    {
        CrcTmp = CRC16((INT8U *) &mLrCtrlInfor[unj], FIND(ST_LR_CTR_DATA_FORM, crcValue));
        if((CrcTmp != mLrCtrlInfor[unj].crcValue))/*"TRUE  1  FALSE  0"*/
        {
            validFlg = poweron_init_ctrl_info(unj);
            if(0 == validFlg)
            {
                LR_ClrRecord();
            }
        }
    }
    
    //PP_InitFrzMinPrjList();/*"分钟冻结方案初始化"*/
#if 0
    /*" 掉电与往前改时间一样处理 "*/
    if(FIRST_PWR_ON != mode)
    {
        res = GetSingle(E_PD_TIME, (INT8U *)&pdtime);/*"从数据中心获取掉电时间"*/
        res1 = GetSingle(E_SYS_TIME, (INT8U *)&now);/*"取系统时间"*/  
        if((FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)&pdtime)) 
            || (FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)&now))
            || (LIB_MemCmp((INT8U *)&pdtime, (INT8U *)&now, 6) != SMALL)
            || (res < MS_SUCCESS) || (res1 < MS_SUCCESS))
        {
            LIB_MemSet(0x1, IndexRefFlg, sizeof(IndexRefFlg));
            return;
        }

        LIB_MemCpy((INT8U *)&pdtime , (INT8U *)&adjust.TimeOld, 6);
        LIB_MemCpy((INT8U *)&now , (INT8U *)&adjust.TimeNew, 6);
        LR_TimeAdjust((INT8U *)&adjust);
    }
#endif
}

//INT8U evt_test;
/*"***************************************************************"*/
/*"  Function:       LR_Main"*/
/*"  Description:   分钟冻结模块主循环调度接口函数"*/
/*"  Input:         "*/
/*"                 "*/
/*"  Output:       无 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
void LR_Main(void)
{
    CRC_DATA_TP CrcTmp;

    //mMinCnt = LIB_MToStdDate((ST_6TIME_FORM *)&mstNowTime);

    FrzChk();

    if((mMinCnt % 15) == 0) /*" "*/
    {
        CrcTmp = CRC16((INT8U *) &mSchemeAttr, FIND(ST_SCHEME_ATTR, crcValue));
        if(CrcTmp != mSchemeAttr.crcValue)
        {
            getSchemeAttr();
        }
    }
}

void LR_ClrAllIdx(void)
{
    INT8U i;
    INT16U sz;
    INT16U len;
    
    sz = sizeof(ST_SCHEME_INDEX_ATTR) * INDEX_LIST_DEEP;
    for(i=0; i<mSchemeAttr.SchemeNum; i++)
    {
        len = sizeof(ST_SCHEME_INDEX_ATTR) * 0x01;
        if(mLrCtrlInfor[i].endIndexId > mLrCtrlInfor[i].startIndexId)
        {
            len = sizeof(ST_SCHEME_INDEX_ATTR) * (mLrCtrlInfor[i].endIndexId + 0x01);
            if(len > sz)
            {
                len = sz;
            }
        }
        else if(mLrCtrlInfor[i].endIndexId < mLrCtrlInfor[i].startIndexId)
        {
            len = sz;
        }
        FS_DelFile(E_FILE_MIN_FRZ_IDX, i*sz, len); 
        WDT_FeedWdg();
    }
    return;
}

/*" 分钟冻结索引默认初始化 "*/
void LR_SetDefaultIdx(void)
{
    INT8U i;
    INT16U sz;
    
    sz = sizeof(ST_SCHEME_INDEX_ATTR) * INDEX_LIST_DEEP;
    for(i=0; i<mSchemeAttr.SchemeNum; i++)
    {/*" TODO 三相表到时测试下擦除时间, 单相表一个方案80ms，8个590ms "*/
        FS_DelFile(E_FILE_MIN_FRZ_IDX, i*sz, sz); 
        WDT_FeedWdg();
    }
    return;
}

/*"***************************************************************"*/
/*"  Function:       Lr_Clr"*/
/*"  Description:   清分钟冻结数据，根据关联对象属性重新分配各关联对象存储空间（设参时需先更新参数，后清零）"*/
/*"  Input:         channel：通道号(关联对象号)"*/
/*"                 dotShift：记录数据列表中当前记录点在循环列表中的偏移点"*/
/*"                 deep：指定通道数据列表深度"*/
/*"                 scr：1 记录已满（循环队列已经发生过掉头）  0记录未满"*/
/*"  Output:       无 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
INT8U LR_ClrRecord(void)
{
    INT8U rsult;
    INT8U uni;

    rsult = TRUE;

    getSchemeAttr();
    
#ifdef IDX_IN_FLASH
    for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
    {
        FS_DelFile(E_FILE_MIN_FRZ_IDX, NUM_IDX_SECTOR*FLASH_SECTOR_SZ*uni, NUM_IDX_SECTOR*FLASH_SECTOR_SZ);
    }
#else
    LR_ClrAllIdx();
#endif

    LIB_MemSet(0, (INT8U *)&mLrCtrlInfor, sizeof(mLrCtrlInfor));
    for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
    {
        mLrCtrlInfor[uni].crcValue = CRC16((INT8U *) &mLrCtrlInfor[uni], FIND(ST_LR_CTR_DATA_FORM, crcValue));
        upd_all_ee_ctrl_info(uni);
    }
    
    return (rsult);
}

/*"***************************************************************"*/
/*"  Function:      getSchemeAttr"*/
/*"  Description:   更新分钟冻结存储方案属性（增加删除关联对象、上电初始化时调用）"*/
/*"  Input:         "*/
/*"                 "*/
/*"  Output:       无 "*/
/*"  Return:        无"*/
/*"****************************************************************"*/
static void getSchemeAttr(void)
{
    //ST_OAD *cur_oad;
    //ST_FRZ_OBJ_ATTR FrzObjTmp[MAX_OAD_MIN_FRZ];
    INT8U uni;
    //INT8U cur_len;
    //ST_FMT_ATTR fmt_dst[MAX_OBIS_NUM_ONE_OAD];/*"OAD成员数据格式"*/
    //ST_FMT_ATTR fmt_save;
    //ST_FRZ_OBJ_ATTR *ObjTmp;
    ST_SCHEME_ATTR_FORM *pSchemeAttr;
    INT32U  startAddr;
    //INT8U cur_prj;
    //INT8U cls_prj;
    //INT8U fst_idx;
    INT8U dotlen;
#ifdef LD_DATA_IN_FLASH
    INT32U num_per_s;
    INT16U num_sector;
#endif

    meter_type = GetMeterType();
    //GetObjList((INT8U *)&FrzObjTmp);
    LIB_MemSet(0, (INT8U *)&mSchemeAttr, sizeof(ST_SCHEME_ATTR));
    
    mSchemeAttr.SchemeNum = MIN_FRZ_CLASS_NUM;
    startAddr = LR_SAVE_START_ADDR;
    for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
    {
        pSchemeAttr = &mSchemeAttr.SchemeAttr[uni];
        switch(uni)
        {
            case 0:
                dotlen = sizeof(struct LR_RECORD);
                break;
            case 1:
                dotlen = sizeof(struct RECORD2);
                break;
            case 2:
                dotlen = sizeof(struct RECORD3);
                break;
            case 3:
                dotlen = sizeof(struct RECORD4);
                break;
            case 4:
                dotlen = sizeof(struct RECORD5);
                break;
            case 5:
                dotlen = sizeof(struct RECORD6);
                break;
            default:
                dotlen = 0;
                break;
        }
#if 0
        pSchemeAttr->DotLen = TMSTAMP_LEN;/*"时间保存4byte"*/
        for(unj = 0; unj < pSchemeAttr->AssObjNum; unj++)
        {
            cur_oad = &FrzObjTmp[pSchemeAttr->frzObjIndex[unj]].oad;
            cur_len = Get_Oad_Src_Data_Fmt_Len(cur_oad, &fmt_save, (ST_FMT_ATTR *)&fmt_dst, NULL);
            if(((INT16U)pSchemeAttr->DotLen + (INT16U)cur_len) >= (MAX_DOT_LEN - LD_CRC_SZ))
            {
                break;
            }
            pSchemeAttr->DotLen += cur_len;
        }
        
        pSchemeAttr->DotLen += LD_CRC_SZ;
#endif
#if LD_CRC_SZ
        dotlen += LD_CRC_SZ;
#endif
        pSchemeAttr->DotLen = dotlen;
#if defined (SINGLE_PHASE)
        pSchemeAttr->deep = 525600; 
#else
        pSchemeAttr->deep = 525600; /*" TODO 三相表要 调整 "*/
#endif
        pSchemeAttr->priod = rcrd_para.LrPara.LoadRecordPeriod[uni];

#ifndef LD_DATA_IN_FLASH
        if(startAddr >= LR_END_ADDR)
        {
            pSchemeAttr->deep = 0;
            continue;
        }
        pSchemeAttr->startAddr = startAddr;
        startAddr += ((INT32U)pSchemeAttr->DotLen * (pSchemeAttr->deep + MAX_DOT_NUM_PER_STAMP));

        if(startAddr > LR_END_ADDR)
        {
            pSchemeAttr->deep = (LR_END_ADDR - pSchemeAttr->startAddr) / pSchemeAttr->DotLen;

            if(pSchemeAttr->deep > MAX_DOT_NUM_PER_STAMP)
            {
                pSchemeAttr->deep -= MAX_DOT_NUM_PER_STAMP;
            }
            else
            {
                pSchemeAttr->deep = 0;
            }
        }
#else
        if(startAddr >= LR_END_ADDR)
        {
            pSchemeAttr->deep = 0;
            continue;
        }
        pSchemeAttr->startAddr = startAddr;
        num_per_s = FLASH_SECTOR_SZ / pSchemeAttr->DotLen;
        num_sector = pSchemeAttr->deep / num_per_s;
        if((pSchemeAttr->deep % num_per_s) != 0)
        {
            num_sector += 1;
        }
        num_sector += (MAX_DOT_NUM_PER_STAMP/num_per_s + 2);
        pSchemeAttr->sum_sector = num_sector;
        
        startAddr += (num_sector * FLASH_SECTOR_SZ);
        if(startAddr > LR_END_ADDR)
        {
            pSchemeAttr->sum_sector = (LR_END_ADDR - pSchemeAttr->startAddr)/FLASH_SECTOR_SZ;
            pSchemeAttr->deep = pSchemeAttr->sum_sector * num_per_s;
            if(pSchemeAttr->deep > MAX_DOT_NUM_PER_STAMP)
            {
                pSchemeAttr->deep -= MAX_DOT_NUM_PER_STAMP;
            }
            else
            {
                pSchemeAttr->deep = 0;
            }
        }
#endif
    }
    mSchemeAttr.crcValue = CRC16((INT8U *)&mSchemeAttr, FIND(ST_SCHEME_ATTR, crcValue));
}

INT8U get_cur_lr_mask(void)
{
    INT8U uni;
    INT8U mask;

    mask = 0;
    for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
    {
        if((rcrd_para.LrPara.LoadRecordMode.byte & (1 << uni)) != 0)
        {
            mask |= (1 << uni);
        }
    }
    return mask;
}

INT8U check_chanel_need_save(INT8U ch)
{
    if((rcrd_para.LrPara.LoadRecordMode.byte & (1 << ch)) != 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*"***************************************************************"*/
/*"  Function:      FrzChk"*/
/*"  Description:   分钟冻结冻结条件判断及记录内容检查，满足记录条件调记录存储接口函数"*/
/*"  Input:         "*/
/*"                 "*/
/*"  Output:       无 "*/
/*"  Return:        无"*/
/*"****************************************************************"*/
static void FrzChk(void)
{
    INT8U uni;
    ST_SCHEME_ATTR_FORM *pSchemeAttr;

    lr_mask = get_cur_lr_mask();
    pSchemeAttr = &mSchemeAttr.SchemeAttr[0];
    for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
    {
        if(uni > 4)
        {
            break;
        }
        //if(((mMinCnt % rcrd_para.LrPara.LoadRecordPeriod[uni]) == 0) && (check_chanel_need_save(uni) == 1))
        {
            saveLrData(uni);
        }
        pSchemeAttr++;
    }
}

/*" RAM控制信息更新到内卡 "*/
void upd_all_ee_ctrl_info(INT8U channel)
{
    ST_LR_CTR_DATA_FORM *pLrCtrlInfor;
    INT16U addr, bak_addr;
    INT8U i;

    pLrCtrlInfor = &mLrCtrlInfor[channel];
    
    addr = E2_LR_CTR_DATA + sizeof(ST_LR_CTR_DATA_FORM)*NUM_LD_CTRL_SPACE*channel;
    for(i=0; i<NUM_LD_CTRL_SPACE; i++)
    {
        bak_addr = addr + i*sizeof(ST_LR_CTR_DATA_FORM);
        FS_WriteFile(E_FILE_RCRD_DATA, bak_addr, (INT8U *)pLrCtrlInfor, sizeof(ST_LR_CTR_DATA_FORM));
    }
    
}

/*" RAM无效时从内卡获取 "*/
INT8U poweron_init_ctrl_info(INT8U channel)
{
    ST_LR_CTR_DATA_FORM ctrl,bak_ctrl;
    ST_LR_CTR_DATA_FORM *pLrCtrlInfor;
    INT16U addr, bak_addr;
    CRC_DATA_TP CrcTmp;
    INT8U i;
    INT8U has;

    pLrCtrlInfor = &mLrCtrlInfor[channel];
    
    addr = E2_LR_CTR_DATA + sizeof(ST_LR_CTR_DATA_FORM)*NUM_LD_CTRL_SPACE*channel;
    ctrl.dotCnt = 0;
    has = 0;
    for(i=0; i<NUM_LD_CTRL_SPACE; i++)
    {
        bak_addr = addr + i*sizeof(ST_LR_CTR_DATA_FORM);
        FS_ReadFile(E_FILE_RCRD_DATA, bak_addr, (INT8U *)&bak_ctrl, sizeof(ST_LR_CTR_DATA_FORM));
        CrcTmp = CRC16((INT8U *)&bak_ctrl, FIND(ST_LR_CTR_DATA_FORM, crcValue));
        if((bak_ctrl.dotCnt > ctrl.dotCnt) && (CrcTmp == bak_ctrl.crcValue))
        {
            LIB_MemCpy((INT8U *)&bak_ctrl, (INT8U *)&ctrl, sizeof(ST_LR_CTR_DATA_FORM));
            has = 1;
        }
    }
    if(has)
    {
        LIB_MemCpy((INT8U *)&ctrl, (INT8U *)pLrCtrlInfor, sizeof(ST_LR_CTR_DATA_FORM));
    }
    return has;
}

/*" 循环保存RAM信息到内卡 "*/
void save_ctrl_info(INT8U channel)
{
    ST_LR_CTR_DATA_FORM *pLrCtrlInfor;
    INT16U addr;

    pLrCtrlInfor = &mLrCtrlInfor[channel];
    addr = E2_LR_CTR_DATA + sizeof(ST_LR_CTR_DATA_FORM)*NUM_LD_CTRL_SPACE*channel;
    addr += ((pLrCtrlInfor->dotCnt % NUM_LD_CTRL_SPACE)*sizeof(ST_LR_CTR_DATA_FORM));
    FS_WriteFile(E_FILE_RCRD_DATA, addr, (INT8U *)pLrCtrlInfor, sizeof(ST_LR_CTR_DATA_FORM));
}


/*"***************************************************************"*/
/*"  Function:      saveLrData"*/
/*"  Description:   保存分钟冻结数据"*/
/*"  Input:         channel：满足记录条件方案号"*/
/*"                 "*/
/*"                 "*/
/*"  Output:        "*/
/*"  Return:        TRUE 成功 FALSE 失败"*/
/*"****************************************************************"*/
static INT8U saveLrData(INT8U channel)
{
    //ST_FRZ_OBJ_ATTR FrzObjTmp[MAX_OAD_MIN_FRZ];
    //INT8U rsult;
    INT8U BuffTemp[MAX_DOT_LEN];/*"冻结数据缓存"*/
    //INT8U uni;
    INT16U buffLen;
    RECORD_ADDR_TYPE SaveAddr; /*"冻结数据保存地址"*/
    ST_LR_CTR_DATA_FORM *pLrCtrlInfor;
    ST_SCHEME_ATTR_FORM  *SchemeAttrTmp;
#ifdef LD_DATA_IN_FLASH
    INT16U num_per_sec;
    INT16U sum_sector;
    INT32U addr_off;
    INDEX_RANGE_TYPE start_id, nxt_id;
    ST_SCHEME_INDEX_ATTR idx_data, nxt_idx_data;
    INT32U start_seq, end_seq;
    INT32U start_addr, end_addr;
#endif
    //INT8U cur_len;
#if LD_CRC_SZ
    CRC_DATA_TP crc;
#endif
    //rsult = TRUE;
    buffLen = 0;

    meter_type = GetMeterType();

    SchemeAttrTmp = &mSchemeAttr.SchemeAttr[channel];
    get_rcrd_data_to_buf((EN_RCRD_FILE_ID)(E_LR_TYPE1_RCRD + channel), (EN_FELXOBJ_ID)(E_FRZ_LST_LR_TYPE1 + channel), BuffTemp, &buffLen);
    BuffTemp[3] = lr_mask;
    
#if LD_CRC_SZ
    crc = CRC16(BuffTemp, buffLen);
    LIB_MemCpy((INT8U *)&crc, &BuffTemp[buffLen], LD_CRC_SZ);
    buffLen += LD_CRC_SZ;
#endif

    /*" 加个防错判断，最终版本删除 "*/
    if(SchemeAttrTmp->DotLen != buffLen)
    {
        return FALSE;
    }

    if(SchemeAttrTmp->deep > 0)
    {
        pLrCtrlInfor = &mLrCtrlInfor[channel];
        SaveAddr = LR_get_rcrd_save_addr(channel, pLrCtrlInfor->dotCnt);
        
        FS_WriteFile(E_FILE_MIN_FRZ, SaveAddr, BuffTemp, SchemeAttrTmp->DotLen);
#ifdef LD_DATA_IN_FLASH
        num_per_sec = NUM_PER_SECTOR(SchemeAttrTmp->DotLen);
        sum_sector = SchemeAttrTmp->sum_sector;
        addr_off = get_data_addr_off(num_per_sec, 
            sum_sector, SchemeAttrTmp->DotLen, pLrCtrlInfor->dotCnt);
        if((pLrCtrlInfor->dotCnt >= (num_per_sec * sum_sector)) && ((addr_off % FLASH_SECTOR_SZ) == 0))
        {/*" 数据扇区回环利用，每次进入一个新的要覆盖历史数据的扇区时,同步删除时间戳 "*/
            pLrCtrlInfor->real_dotCnt -= (num_per_sec); /*" 灭掉一个扇区 "*/
            start_id = pLrCtrlInfor->startIndexId;
            for(; start_id != pLrCtrlInfor->endIndexId; )
            {
                nxt_id = start_id;
                RangeIdAdd(&nxt_id);
                GetIndexAttr(channel, start_id, &idx_data);
                GetIndexAttr(channel, nxt_id, &nxt_idx_data);
                start_seq = idx_data.startDotId;
                end_seq = nxt_idx_data.startDotId - 1;
                start_addr = get_data_addr_off(num_per_sec, sum_sector, SchemeAttrTmp->DotLen, start_seq);
                end_addr = get_data_addr_off(num_per_sec, sum_sector, SchemeAttrTmp->DotLen, end_seq);
                if(((start_addr >= addr_off) && (start_addr < (addr_off + FLASH_SECTOR_SZ))) /*" 索引对应记录落在被覆盖的扇区 "*/
                    || ((end_addr >= addr_off) && (end_addr < (addr_off + FLASH_SECTOR_SZ))))
                {
                    start_id = nxt_id;
                    pLrCtrlInfor->startIndexId = nxt_id; /*" 指针后移 "*/
                }
                else
                {
                    break;
                }
            }
        }
#else

#endif
        /*"关闭掉电检测中断"*/
        MCU_PwrChkCloseINT();
        if(pLrCtrlInfor->dotCnt == 0) /*" 第一条记录 "*/
        {
            pLrCtrlInfor->NowStampDotCnt = 0;
            Ref_CurIndexAttr(channel, 0);
        }
        else if(IndexRefFlg[channel] == 1 || pLrCtrlInfor->NowStampDotCnt == MAX_DOT_NUM_PER_STAMP) 
        {/* 掉电导致中间有空点 || 往后改时间导致有空点 || 上个索引下记录数达到最大值 */
            
            pLrCtrlInfor->NowStampDotCnt = 0;
            RangeIdAdd(&pLrCtrlInfor->endIndexId);
            RangeStartIdMove(pLrCtrlInfor);
            Ref_CurIndexAttr(channel, 0);
        }
        else if(pLrCtrlInfor->NowStampDotCnt < MAX_DOT_NUM_PER_STAMP)
        {
            Ref_CurIndexAttr(channel, 1);
        }
        
        pLrCtrlInfor->NowStampDotCnt++;
        pLrCtrlInfor->dotCnt++;

        if(pLrCtrlInfor->real_dotCnt < get_ch_max_dot_num(channel))
        {
            pLrCtrlInfor->real_dotCnt++;
        }
        pLrCtrlInfor->crcValue = CRC16((INT8U *) pLrCtrlInfor, FIND(ST_LR_CTR_DATA_FORM, crcValue));
        save_ctrl_info(channel);
        /*"开启掉电检测中断"*/
        MCU_PwrChkOpenINT();
    }

    return TRUE;
}

/*"***************************************************************"*/
/*"  Function:       RangeIdAdd"*/
/*"  Description:   索引范围内索引序号循环累加"*/
/*"  Input:         RangeId：索引序号指针"*/
/*"                 "*/
/*"  Output:       无 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
static void RangeIdAdd(INDEX_RANGE_TYPE * RangeId)
{
    (*RangeId)++;
    if(*RangeId >= INDEX_LIST_DEEP)
    {
        *RangeId = 0;
    }
}


/*" 将时间对齐 "*/
INT32U get_align_tm(INT32U Time, INT16U prd)
{
    INT32U nxt;

    nxt = (Time + (prd - 1))/prd;
    nxt = nxt * prd;
    return nxt;
}

INT32U get_nxt_frz_tm(INT32U Time, INT16U prd)
{
    INT32U nxt;

    nxt = Time/prd;
    nxt = nxt * prd;
    nxt += prd;
    return nxt;
}

/*"***************************************************************"*/
/*"  Function:       Get_Rcd_from_time "*/
/*"  Description:    获得指定时间开始的最早一条记录 (也就是时间大于等于time) "*/
/*"  Input:         ch 方案号，time 请求时间 "*/
/*"                 "*/
/*"  Output:        stamp  记录所在时间戳序号 "*/
/*"                 seq 记录序号 ; rcd_tm 记录实际时间"*/
/*"                 "*/
/*"  Return:        筛选结果  TRUE  成功 FALSE 失败 "*/
/*"****************************************************************"*/
INT8U Get_Rcd_from_time(INT8U ch, INT32U time, INDEX_RANGE_TYPE *stamp, INT32U *seq, INT32U *rcd_tm)
{
    INDEX_RANGE_TYPE StampStartId;
    INT16U  StampNum;
    INT8U res;
    INT32U startDotId;
    ST_LR_CTR_DATA_FORM *pLrCtrlDataTmp;
    ST_SCHEME_INDEX_ATTR IndexAttr;
    INT32U end_tm;
    INT8U prd;

    pLrCtrlDataTmp = &mLrCtrlInfor[ch];
    
    StampNum = SearchTimeStamp(ch, &time, NULL, &StampStartId);
    if(StampNum == 0)
    {
        return FALSE;
    }
    res = SearchDotInstamp(StampStartId, ch, time, &end_tm, &startDotId);
    if(FALSE == res)
    {
        /*" 获取下一个时间戳，然后从中获得第一条记录 "*/
        if(StampStartId != pLrCtrlDataTmp->endIndexId)
        {
            RangeIdAdd(&StampStartId);
            GetIndexAttr(ch, StampStartId, &IndexAttr);
            startDotId = IndexAttr.startDotId;
            *rcd_tm = (IndexAttr.TimeStamp & TMSTAMP_MSK);
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        GetIndexAttr(ch, StampStartId, &IndexAttr);
        prd = IndexAttr.TimeStamp >> 24;
        *rcd_tm = (IndexAttr.TimeStamp & TMSTAMP_MSK) + (startDotId - IndexAttr.startDotId)*prd;
    }
    *seq = startDotId;
    *stamp = StampStartId;
    return TRUE;
}


/*" 获取单个数据标识的数据长度 "*/
INT8U get_single_id_data_len(INT8U di0, INT8U di1)
{
    INT8U len;
    
    switch(di1)
    {
    case 1:
        len = (di0 == 0xff)? (2*MU_ELEMENT_NUM): 2;
        break;
    case 2:
        len = (di0 == 0xff)? (3*MU_ELEMENT_NUM): 3;
        break;   
    case 3:
        len = (di0 == 0xff)? (3*(MU_ELEMENT_NUM+1)):3;
        break; 
    case 4:
        len = (di0 == 0xff)? (3*(MU_ELEMENT_NUM+1)): 3;
        break;
#if 1
    case 5:
        len = (di0 == 0xff)? (2*(MU_ELEMENT_NUM+1)): 2;
        break; 
    case 6:
        len = (di0 == 0xff)? (2*ENERGY_SIZE): ENERGY_SIZE;/*" 无组合电量"*/
        break; 
    case 7:
        len = (di0 == 0xff)? (4*ENERGY_SIZE): ENERGY_SIZE;
        break; 
    case 8:
        len = (di0 == 0xff)? 6: 3;
        break; 
    case 9:
        len = 2;
        break;
#endif
    default:
        len = 0;
    }
    return len;
}


void clr_ch_all_dot(INT8U uni)
{
    mLrCtrlInfor[uni].dotCnt = 0;
    mLrCtrlInfor[uni].real_dotCnt = 0;
    mLrCtrlInfor[uni].endIndexId = 0;
    mLrCtrlInfor[uni].startIndexId = 0;
    mLrCtrlInfor[uni].NowStampDotCnt = 0;
    mLrCtrlInfor[uni].crcValue = CRC16((INT8U *)&mLrCtrlInfor[uni], FIND(ST_LR_CTR_DATA_FORM, crcValue));  
    upd_all_ee_ctrl_info(uni);
}

/*"***************************************************************"*/
/*"  Function:       LR_TimeAdjust"*/
/*"  Description:   分钟冻结时钟调整接口函数，完成索引表维护"*/
/*"  Input:         pTimeOld 时钟调整前时间"*/
/*"                 pTimeNew 调整后时间"*/
/*"                 "*/
/*"                 "*/
/*"  Output:       无 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
void LR_TimeAdjust(INT8U *pdata)
{
    INDEX_RANGE_TYPE StampStartId;/*"查询结果中时间戳起始ID号"*/
    INT32U TimeStartTmp;/*"起始时间相对2000年1月1日0点0分的分钟数"*/
    INT32U TimeEndTmp;  /*"结束时间相对2000年1月1日0点0分的分钟数"*/
    ST_TIME_ADJUST_INPUT *pTimeAdjustIn;
    INT8U uni;
    INT32U startDotId;
    INT8U res;
    ST_SCHEME_INDEX_ATTR IndexAttr;
    INT32U nxt;
    INT32U rcd_tm;
    INT32U delta;
    INT32U lst_rcd_tm;
    INT32U fst_rcd_tm;
#ifdef LD_DATA_IN_FLASH
    INT32U SaveAddr;
#endif
    
    pTimeAdjustIn = (ST_TIME_ADJUST_INPUT *)pdata;
    
    if(BIG == LIB_MemCmp((INT8U *) &pTimeAdjustIn->TimeOld, (INT8U *)&pTimeAdjustIn->TimeNew, sizeof(ST_6TIME_FORM)))/*"时钟向前调整，需维护索引表"*/
    {
        TimeStartTmp = LIB_MToStdDate(&pTimeAdjustIn->TimeNew);
        TimeEndTmp = LIB_MToStdDate(&pTimeAdjustIn->TimeOld);

        if(pTimeAdjustIn->TimeNew.sec > 0)
        {
            TimeStartTmp += 1;
        }

        MCU_PwrChkCloseINT();
        for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
        {
            lst_rcd_tm = get_ch_lst_rcd_tm(uni);
            //nxt = get_nxt_frz_tm(TimeStartTmp, mSchemeAttr.SchemeAttr[uni].priod);
            if(TimeStartTmp < lst_rcd_tm)
            {
                IndexRefFlg[uni] = 1;
                startDotId = 0;

                //GetIndexAttr(uni, mLrCtrlInfor[uni].startIndexId, &IndexAttr);
                fst_rcd_tm = get_ch_first_rcrd_tm(uni);
                if(TimeStartTmp < fst_rcd_tm)
                {
                    clr_ch_all_dot(uni);
                    continue;
                }

                res = Get_Rcd_from_time(uni, nxt, &StampStartId, &startDotId, &rcd_tm);
                if(res == FALSE)
                {
                    continue;
                }
                delta = mLrCtrlInfor[uni].dotCnt - startDotId;
                mLrCtrlInfor[uni].dotCnt = startDotId;
                
                if(delta < mLrCtrlInfor[uni].real_dotCnt)
                {
                    mLrCtrlInfor[uni].real_dotCnt -= delta;
                }
                else
                {
                    clr_ch_all_dot(uni);
                    continue;
                }
                
                
                GetIndexAttr(uni, StampStartId, &IndexAttr);
                if(IndexAttr.startDotId == startDotId)
                {
                    /*" 这个索引的所有记录全部删掉了 "*/
                    if(startDotId == 0)
                    {/*" 这个方案的记录全部清零  "*/
                        clr_ch_all_dot(uni);
                    }
                    else
                    {
                        if(StampStartId > 0)
                        {
                            StampStartId--;
                        }
                        else
                        {
                            StampStartId = INDEX_LIST_DEEP - 1;
                        }
                        mLrCtrlInfor[uni].endIndexId = StampStartId;
                    }
                }
                else
                {
                    mLrCtrlInfor[uni].endIndexId = StampStartId;
                    if(startDotId > IndexAttr.startDotId)
                    {
                        mLrCtrlInfor[uni].NowStampDotCnt = startDotId - IndexAttr.startDotId;
                        IndexRefFlg[uni] = 0;
                    }
                }
                mLrCtrlInfor[uni].crcValue = CRC16((INT8U *)&mLrCtrlInfor[uni], FIND(ST_LR_CTR_DATA_FORM, crcValue));
                upd_all_ee_ctrl_info(uni);
#ifdef LD_DATA_IN_FLASH
                SaveAddr = LR_get_rcrd_save_addr(uni, mLrCtrlInfor[uni].dotCnt);
                erase_flash_sector_part_data(E_FILE_MIN_FRZ, SaveAddr);
#endif
            }
            else
            {
            }
        }

        MCU_PwrChkOpenINT();
    }
    else/*"时钟往后调整"*/
    {
        TimeEndTmp = LIB_MToStdDate(&pTimeAdjustIn->TimeNew);
        TimeStartTmp = LIB_MToStdDate(&pTimeAdjustIn->TimeOld);
        for(uni = 0; uni < mSchemeAttr.SchemeNum; uni++)
        {
            nxt = get_nxt_frz_tm(TimeStartTmp, rcrd_para.LrPara.LoadRecordPeriod[uni]);
            if((nxt <= TimeEndTmp) && (mLrCtrlInfor[uni].dotCnt != 0))/*" 改时间中间有空点 "*/
            {
                IndexRefFlg[uni] = 1;
            }
        }
    }
}

/*"***************************************************************"*/
/*"  Function:       SearchTimeStamp"*/
/*"  Description:   查找指定时间的时间戳在索引表中的位置"*/
/*"  Input:         Channel 指定方案"*/
/*"                 pTimeStart 查找起始时间"*/
/*"                 pTimeEnd   查找结束时间  如果结束时间为NULL，标识单点查询"*/
/*"  Output:        dotID 起始时间戳在索引列表中的序号"*/
/*"                此接口只是表示对应时间点记录的时间戳在哪里，但是并不代表对应时间点就一定有记录，可能因为掉电而没有记录"*/
/*"                 "*/
/*"  Return:        查找结果中时间戳数 0表示没有查到  "*/
/*"****************************************************************"*/
static INDEX_RANGE_TYPE SearchTimeStamp(INT8U Channel, INT32U *pTimeStart, INT32U *pTimeEnd, INDEX_RANGE_TYPE *dotID)
{
    INDEX_RANGE_TYPE stampNum;
    INT16U StampIdTmp[2];/*"二分查找算法范围，0 start 1 end，end < start时加上INDEX_LIST_DEEP"*/
    INT16U SearchRault[2];/*"二分查找结果"*/
    ST_LR_CTR_DATA_FORM *pLrCtrlDataTmp;

    stampNum = 0;
    pLrCtrlDataTmp = &mLrCtrlInfor[Channel];
    /*"计算二分查找范围"*/
    StampIdTmp[0] = pLrCtrlDataTmp->startIndexId;
    StampIdTmp[1] = pLrCtrlDataTmp->endIndexId;
    if(StampIdTmp[1] < StampIdTmp[0])
    {
        StampIdTmp[1] += INDEX_LIST_DEEP;
    }
    else
    {
    }
    /*"二分查找起始点位置"*/
    SearchRault[0] = BinarySearchIteration(&StampIdTmp[0], *pTimeStart, Channel);
    if(NULL != pTimeEnd)
    {
        /*"二分查找起结束点位置"*/
        SearchRault[1] = BinarySearchIteration(&StampIdTmp[0], *pTimeEnd, Channel);
        if(0xffff != SearchRault[1])
        {
            if(0xffff != SearchRault[0])
            {
                stampNum = (SearchRault[1] - SearchRault[0]) + 1;
                *dotID = (INDEX_RANGE_TYPE) (SearchRault[0] % INDEX_LIST_DEEP);
            }
            else
            {
                stampNum = (SearchRault[1] - pLrCtrlDataTmp->startIndexId) + 1;
                *dotID = pLrCtrlDataTmp->startIndexId;
            }
        }
        else
        {
            if(0xffff == SearchRault[0])/*"起始点结束点都未找到，查找范围不在记录区间里"*/
            {

            }
            else/*"理论上能够找到起始点，就能找到结束点，结束点最多就是超出当前时间，超出当前时间的点不考虑"*/
            {
                //todo debug
            }
        }
    }
    else
    {
        if(0xffff != SearchRault[0])
        {
            *dotID = (INDEX_RANGE_TYPE) (SearchRault[0] % INDEX_LIST_DEEP);
            stampNum = 1;
        }
    }

    return (stampNum);
}

/*"***************************************************************"*/
/*"  Function:      BinarySearchIteration"*/
/*"  Description:   分钟冻结索引列表二分查找算法（查找指定时间戳在列表中的位置）"*/
/*"  Input:         Range 查找范围指针"*/
/*"                 TimeStamp 指定时间"*/
/*"                 channel 指定方案"*/
/*"  Output:        "*/
/*"                 "*/
/*"                 "*/
/*"  Return:        指定时间戳在索引范围中的位置"*/
/*"****************************************************************"*/
static INT16U BinarySearchIteration(INT16U *Range, INT32U TimeStamp, INT8U channel)
{
    ST_SCHEME_INDEX_ATTR IndexAttr[2];/*"当前索引点属性信息"*/
    INT16U StampIdTmp[2];/*"二分查找算法范围 0 起始   1结束"*/
    INT16U Mid;  /*"二分查找变量"*/
    INT32U TimeStampTmp[2];/*"二分查找中用于比较临时保存的时间戳"*/
    INT8U  GetDotNum;/*"mid 不到最后一个点可以一次获取2点时间戳进行比较，最后一个点只能读一个"*/
    INT8U rsult;

    StampIdTmp[0] = Range[0];
    StampIdTmp[1] = Range[1];
    TimeStampTmp[0] = 0;
    TimeStampTmp[1] = 0;

    rsult = FALSE;
    for(; StampIdTmp[0] <= StampIdTmp[1];)
    {
        Mid = StampIdTmp[0] + (StampIdTmp[1] - StampIdTmp[0]) / 2;
        GetIndexAttr(channel, Mid % INDEX_LIST_DEEP, &IndexAttr[0]);
        if(Mid == Range[1])/*"查到最近的一个时间戳直接返回该时间戳位置，由外面做进一步查询"*/
        {
            GetDotNum = 1;
        }
        else
        {
            GetDotNum = 2;
            GetIndexAttr(channel, (Mid + 1) % INDEX_LIST_DEEP, &IndexAttr[1]);
        }

        TimeStampTmp[0] = IndexAttr[0].TimeStamp & TMSTAMP_MSK;
        TimeStampTmp[1] = IndexAttr[GetDotNum - 1].TimeStamp & TMSTAMP_MSK;/*"有可能只取了一个点"*/
        if((TimeStamp >= TimeStampTmp[0])
                && ((TimeStamp <= TimeStampTmp[1]) || (1 == GetDotNum)))/*"找到范围点"*/
        {
            if((TimeStamp ==  TimeStampTmp[1]) && (GetDotNum == 2))
            {
                Mid++;
            }
            rsult = TRUE;
            break;
        }
        else if(TimeStamp > TimeStampTmp[1])
        {
            StampIdTmp[0] = Mid + 1;
        }
        else if(TimeStamp < TimeStampTmp[0])
        {
            if(Mid > StampIdTmp[0])
            {
                StampIdTmp[1] = Mid - 1;
            }
            else
            {
                break;
            }
        }
        else
        {

        }
    }

    if(TRUE == rsult)
    {
        return(Mid);
    }
    else
    {
        return(0xffff);
    }
}

/*"***************************************************************"*/
/*"  Function:       SearchDotInstamp"*/
/*"  Description:   在指定时间戳对应的记录中查找指定时间记录，输出记录序号"*/
/*"  Input:         stampId 指定的时间戳序号"*/
/*"                 Channel 记录方案号"*/
/*"                 inTime 输入时间"*/
/*"                 LastDotId 最近一条记录在存储队列中的序号"*/
/*"  Output:        end_tm:此时间戳最后一条记录的时间；pRcrdId查找结果记录序号"*/
/*"  这个接口只能查找精确相等的记录，不能查找大于用户时间的记录,同时请求时间不是对齐的也返回失败 "*/
/*"                 "*/
/*"  Return:        TRUE 找到相应记录点 FALSE 未找到相应记录点"*/
/*"****************************************************************"*/
static INT8U  SearchDotInstamp(INDEX_RANGE_TYPE stampId, INT8U Channel, INT32U inTime, INT32U *end_tm, INT32U *pRcrdId)
{
    INT8U rsult;
    INT16U DotId;
    ST_SCHEME_INDEX_ATTR IndexAttr, next_idx;
    INT16U   priod;
    ST_LR_CTR_DATA_FORM *pLrCtrlData;
    INT16U   dotNum;
    INDEX_RANGE_TYPE nxt_stampid;

    //priod = mSchemeAttr.SchemeAttr[Channel].priod;
    pLrCtrlData = &mLrCtrlInfor[Channel];
    GetIndexAttr(Channel, stampId, &IndexAttr);
    priod = IndexAttr.TimeStamp >> 24;

    rsult = FALSE;
    if(stampId == pLrCtrlData->endIndexId)
    {
        dotNum = pLrCtrlData->dotCnt - IndexAttr.startDotId;
    }
    else
    {

        nxt_stampid = stampId;
        RangeIdAdd(&nxt_stampid);
        GetIndexAttr(Channel, nxt_stampid, &next_idx);
        dotNum = next_idx.startDotId - IndexAttr.startDotId;
    }
    *end_tm = (IndexAttr.TimeStamp & TMSTAMP_MSK) + (dotNum - 1) * priod;
    if(inTime >= (IndexAttr.TimeStamp & TMSTAMP_MSK))
    {
        INT16U DivMin;

        DivMin = inTime - (IndexAttr.TimeStamp & TMSTAMP_MSK);
        if((DivMin % priod) == 0)
        {
            DotId = DivMin / priod;

            if(DotId < dotNum)
            {
                *pRcrdId = IndexAttr.startDotId + DotId;
                rsult = TRUE;
            }
            else
            {/*" 每个时间戳里面记录都是连续的 "*/
            }
        }
        else
        {
        }
    }
    else
    {
    }
    return rsult;
}

/*"***************************************************************"*/
/*"  Function:       SearchDotInstamp"*/
/*"  Description:   在指定时间戳对应的记录中查找指定时间记录，输出记录序号"*/
/*"  Input:         stampId 指定的时间戳序号"*/
/*"                 Channel 记录方案号"*/
/*"                 inTime 输入时间"*/
/*"                 LastDotId 最近一条记录在存储队列中的序号"*/
/*"  Output:        end_tm:此时间戳最后一条记录的时间；pRcrdId查找结果记录序号"*/
/*"  这个接口只能查找精确相等的记录，不能查找大于用户时间的记录,同时请求时间不是对齐的也返回失败 "*/
/*"                 "*/
/*"  Return:        TRUE 找到相应记录点 FALSE 未找到相应记录点"*/
/*"****************************************************************"*/
static INT8U  SearchDotInstamp_ByInterval(INDEX_RANGE_TYPE stampId, INT8U Channel, INT16U intval,
    INT32U *Time, INT32U *pRcrdId)
{
    INT16U DotId;
    ST_SCHEME_INDEX_ATTR IndexAttr, next_idx;
    INT16U   priod;
    ST_LR_CTR_DATA_FORM *pLrCtrlData;
    INT16U   dotNum;
    INT32U inTime, end_tm;
    INT16U DivMin;
    INT8U sum;
    INT32U lst_tm;
    INT32U fst_seq;
    
    inTime = Time[0];
    lst_tm = inTime;

    fst_seq = get_ch_first_rcrd_seq(Channel);

    //priod = mSchemeAttr.SchemeAttr[Channel].priod;
    pLrCtrlData = &mLrCtrlInfor[Channel];
    GetIndexAttr(Channel, stampId, &IndexAttr);
    priod = IndexAttr.TimeStamp >> 24;

    if(stampId == pLrCtrlData->endIndexId)
    {
        dotNum = pLrCtrlData->dotCnt - IndexAttr.startDotId;
    }
    else
    {
        RangeIdAdd(&stampId);
        GetIndexAttr(Channel, stampId, &next_idx);
        if(next_idx.startDotId < fst_seq)
        {
            return 0;
        }
        dotNum = next_idx.startDotId - IndexAttr.startDotId;
    }
    end_tm = (IndexAttr.TimeStamp & TMSTAMP_MSK) + (dotNum - 1) * priod;
    sum = 0;
    while(inTime <= end_tm && inTime <= Time[1])
    {
        if(inTime >= (IndexAttr.TimeStamp & TMSTAMP_MSK))
        {
            DivMin = inTime - (IndexAttr.TimeStamp & TMSTAMP_MSK);
            if((DivMin % priod) == 0)
            {
                DotId = DivMin / priod;

                if(DotId < dotNum)
                {
                    if(IndexAttr.startDotId + DotId >= fst_seq)
                    {
                        lst_tm = inTime;
                        pRcrdId[sum] = IndexAttr.startDotId + DotId;
                        sum++;
                    }
                }
                else
                {/*" 每个时间戳里面记录都是连续的 "*/
                    break;
                }
            }
        }
        
        inTime += intval;
    }


    Time[0] = lst_tm;
    return sum;
}

INT8U LRSearchRcrdInTimeByInterval(INT8U Channel, INT16U Interval, INT32U *tm)
{
    ST_SCHEME_INDEX_ATTR NxtIndex;
    INT32U delta;
    INT16U LineNum;
    INT32U inTime[2];/*"0  start time   1 end time"*/
    INDEX_RANGE_TYPE  StartStampId;/*"查询结果起始索引号"*/
    INDEX_RANGE_TYPE  StampNum;/*"查询结果索引数"*/
    ST_LR_CTR_DATA_FORM  *pLrCtrlData;
    //INT32U rcd_tm, cur_tm, TimeStamp;
    //INT8U res;
    //INT32U start_tm, end_tm;
    //INT32U fst_seq;
    INT8U cur_dot_num;
    INT32U res_addr[MAX_DOT_NUM_PER_STAMP];

    pLrCtrlData = &mLrCtrlInfor[Channel];
    inTime[0] = tm[0];
    inTime[1] = tm[1];

    RsultAttr.num = 0;
    StampNum = SearchTimeStamp(Channel, &inTime[0], NULL, &StartStampId); /* 这个应该调用一次就可以了 QQQ */
    if(StampNum > 0)
    {
        LineNum = 0;
        do
        {
            cur_dot_num = SearchDotInstamp_ByInterval(StartStampId, Channel, Interval, &inTime[0], res_addr);
            if(cur_dot_num > 0)
            {
                if(LineNum + cur_dot_num >= MAX_LINE_RSULT_NUM)
                {
                    cur_dot_num = (MAX_LINE_RSULT_NUM - LineNum);
                    LIB_MemCpy((INT8U *)res_addr, (INT8U *)&RsultAttr.seq_no[LineNum], cur_dot_num*4);
                    LineNum += cur_dot_num;
                    break;
                }
                else
                {
                    LIB_MemCpy((INT8U *)res_addr, (INT8U *)&RsultAttr.seq_no[LineNum], cur_dot_num*4);
                    LineNum += cur_dot_num;
                }
            }
            
            
            if(StartStampId == pLrCtrlData->endIndexId)
            {/*" 遍历完最后一个时间戳 "*/
                break;
            }
            RangeIdAdd(&StartStampId);
            GetIndexAttr(Channel, StartStampId, &NxtIndex);
            if((NxtIndex.TimeStamp & TMSTAMP_MSK) > inTime[1])
            {
                break;
            }

            if(cur_dot_num > 0 || (NxtIndex.TimeStamp & TMSTAMP_MSK) > inTime[0])
            {
                if((NxtIndex.TimeStamp & TMSTAMP_MSK) > (inTime[0] + Interval))
                {/*" 中间发生掉电 ， 没有掉电应该会小于等于 "*/
                    delta = ((NxtIndex.TimeStamp & TMSTAMP_MSK) - inTime[0] + (Interval - 1))/Interval;
                    inTime[0] += (INT32U)Interval*(INT32U)delta;
                }
                else
                {
                    inTime[0] += Interval;
                }
            }
        }
        while(inTime[0] <= inTime[1]);
                            
        RsultAttr.num = LineNum;
    }
    return RsultAttr.num;
}

const INT16U MonthDaysCnt[2][13] =
        {
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 },
        {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 },
        };
/*"***************************************************************"*/
/*"  Function:       MinToDateTime"*/
/*"  Description:   根据与2000年1月1日0点0分的分钟数推算日期时间"*/
/*"  Input:         Min 分钟数"*/
/*"                 "*/
/*"  Output:        pDateTime 日期时间"*/
/*"                 "*/
/*"  Return:        "*/
/*"****************************************************************"*/
 void MinToDateTime(INT32U Min, ST_6TIME_FORM *pDateTime)
{
    INT16U  time;
    INT16U  Days;
    INT8U   Years;
    INT16U   modDays;
    INT8U  leapYear;
    INT8U  uni;

    Days = Min / 1440 + 1;/*"不满整天的那天没有算在天数里，反推时需把这天加入"*/
    time = Min % 1440;
    pDateTime->min = time % 60;
    pDateTime->hour = time / 60;
    pDateTime->sec = 0;

    Years = 0;
    leapYear = 0;
    if(Days > 1461)/*"按4年一个周期推算，天数固定为1461天"*/
    {
        Years += ((Days / 1461) << 2);
    }
    modDays = Days % 1461;

    if((Years != 0) && (modDays == 0)) /*" 4年的最后一天 "*/
    {
        Years -= 1;
        modDays = 365;
    }
    else if(modDays > 366)/*"4年周期中第一年为润年"*/
    {
        Years += (modDays - 1) / 365;
        modDays = (modDays - 1) % 365;
        if(modDays == 0) /*" 非闰年的最后一天 "*/
        {
            Years -= 1;
            modDays = 365;
        }
    }
    else
    {
        leapYear = 1;
    }

    for(uni = 0; uni <13; uni++)
    {
        if(modDays <= MonthDaysCnt[leapYear][uni])
        {
            pDateTime->month = uni;
            break;
        }
    }
    pDateTime->day = modDays - MonthDaysCnt[leapYear][uni -1];
    pDateTime->year = Years;
}

#define  F_SIZE_LD      196  /*"帧数据最大字节数"*/

static INT8U per_len;
INT8U user_req_pnt;
INT8U sum_done_pnt;
INT32U req_inTime[2];
INT8U lst_pnt_data[16];
INT8U last_frm_no[4]; /*" 上次抄读帧序号 "*/
INT8U last_frm_len;
INT8U last_frm[250];

INT8U Read_Count[4];
INT32U ch_seq[4][6]; /*" 4个通道，6类 "*/
INT8U used_seq[4];


/*" 获取单个数据标识的数据长度 "*/
/*" 纯645按抄读格式存储 "*/
#if 0
INT8U get_single_id_ch_off(INT8U di0, INT8U di1, INT8U *off)
{
    //INT8U len;
    INT8U ch;
    
    switch(di1)
    {
    case 1: /*" 电压 "*/
        ch = 0;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD1, VA[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD1, VA[0]) + (di0 - 1)*2;
        }
        //len = (di0 == 0xff)? 6: 2;
        break;
    case 2:  /*" 电流 "*/
        ch = 0;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD1, IA[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD1, IA[0]) + (di0 - 1)*3;
        }
        //len = (di0 == 0xff)? 9: 3;
        break;  
        
    case 3:
        ch = 1;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD2, PAll[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD2, PAll[0]) + (di0 - 0)*3;
        }
        //len = (di0 == 0xff)? 12: 3;
        break; 
#if defined(THREE_PHASE)
    case 4:
        ch = 1;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD2, RAll[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD2, RAll[0]) + (di0 - 0)*3;
        }
        //len = (di0 == 0xff)? 12: 3;
        break;
#endif        

    case 5:
        ch = 2;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD3, Factor[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD3, Factor[0]) + (di0 - 0)*2;
        }
        //len = (di0 == 0xff)? 8: 2;
        break; 
        
    case 6:
        ch = 3;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD4, PP);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD4, PP) + (di0 - 1)*4;
        }
        //len = (di0 == 0xff)? 16: 4;
        break; 
#if defined(THREE_PHASE)        
    case 7:
        ch = 4;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD5, Quad1);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD5, Quad1) + (di0 - 1)*4;
        }
        //len = (di0 == 0xff)? 16: 4;
        break; 
        
    case 8:
        ch = 5;
        if(di0 == 0xff)
        {
            *off = FIND(struct RECORD6, DemandActive);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct RECORD6, DemandActive) + (di0 - 1)*3;
        }
        //len = (di0 == 0xff)? 6: 3;
        break; 
#endif
    default:
        ch = 0;
        //len = 0;
    }
    return ch;
}
#else

INT8U get_single_id_ch_off(INT8U di0, INT8U di1, INT8U *off)
{
    INT8U ch;

    ch = 0;
    switch(di1)
    {
    case 1: /*" 电压 "*/
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, VA[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, VA[0]) + (di0 - 1)*2;
        }
        break;
    case 2:  /*" 电流 "*/
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, IA[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, IA[0]) + (di0 - 1)*3;
        }
        break;  
        
    case 3:
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, PAll[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, PAll[0]) + (di0 - 0)*3;
        }
        break; 
    case 4:
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, RAll[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, RAll[0]) + (di0 - 0)*3;
        }
        break;

    case 5:
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, Factor[0]);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, Factor[0]) + (di0 - 0)*2;
        }
        break; 
        
    case 6:
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, PP);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, PP) + (di0 - 1)*ENERGY_SIZE;
        }
        break; 
       
    case 7:
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, Quad1);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, Quad1) + (di0 - 1)*ENERGY_SIZE;
        }
        break; 
#if defined(THREE_PHASE)         
    case 8:
        if(di0 == 0xff)
        {
            *off = FIND(struct LR_RECORD, DemandActive);
        }
        else if(di0 <= 3)
        {
            *off = FIND(struct LR_RECORD, DemandActive) + (di0 - 1)*3;
        }
        break; 
#endif
    case 9:
        *off = FIND(struct LR_RECORD, Freq[0]);
        break;
    default:
        ch = 0;
        break;
    }
    return ch;
}


#endif


void patch_no_data_pnt(INT8U di0, INT8U di1, INT8U *pBuf)
{
    //INT16U blk;
    //INT16S ret;
    
    if (di1 == 6 || di1 == 7)
    {
        LIB_MemCpy(lst_pnt_data, pBuf, per_len);
    }
    else
    {
        LIB_MemSet(0xFF, pBuf, per_len);
    }
}


INT8U lr_one_pnt_data(INT8U channel, INT32U seq_no, INT8U off, INT8U len, INT8U *dstbuf)
{
    INT32U dataAddr;

    dataAddr = LR_get_rcrd_save_addr(channel, seq_no);
    FS_ReadFile(E_FILE_MIN_FRZ, dataAddr + off, dstbuf, len);
    
    return len;
}



/*" 新的负荷曲线读取方式，基于时间和数据标识读取，连续读取96个点 "*/
/*" id:数据标识； rCommand: 数据内容；rBuffer: 指向645报文偏移14的地方"*/
/*" mode: 是否为后续帧读取； uartx: 通道"*/
INT16S read_record_base_time_id(UN_ID645 id,INT8U *rCommand, INT8U *rBuffer,
								INT8U mode,INT8U uartx)
{
    ST_6TIME_FORM rcd_date;
    ST_6TIME_FORM req_tm;
    INT8U max_pnt;
    INT32U cur_tm;
    //ST_SCHEME_INDEX_ATTR EndIndexAttr;
    INT32U inTime[2];/*"0  start time   1 end time"*/
    INT16U period, Interval;
    INT8U  Channel;/*"方案号"*/
    //ST_LR_CTR_DATA_FORM  *pLrCtrlData;
    INT32U TimeStamp;
    INT32U start_tm, lst_rcd_tm, fst_rcd_tm;
    INT8U i;
    INT32U dataAddr;
    INT8U *pBuf;
    INT8U cur_done_pnt;
    INT32U seq;
    //INT8U num_oad;
    INT8U off;
    INDEX_RANGE_TYPE  StartStampId;
    INT32U start_seq;
    INT32U rcd_tm;
    INT8U res;
    INT8U cur_frm_no;

/*
    num_oad = get_oad_base_645id(id.asBytes[0], id.asBytes[1], oad, fmt_comm);
    if(num_oad == 0)
    {
        return 0;
    }
    GetObjList((INT8U *)&road_list);

    Channel = get_oad_ch(road_list, oad);
*/
    Channel = get_single_id_ch_off(id.asBytes[0], id.asBytes[1], &off);
    if(Channel >= mSchemeAttr.SchemeNum)
    {
        return 0;
    }
    //pSchemeAttr = &mSchemeAttr.SchemeAttr[Channel];
    //pLrCtrlData = &mLrCtrlInfor[Channel];
    
    period = rcrd_para.LrPara.LoadRecordPeriod[Channel]; /*" 运行过程改周期协议有问题"*/
        
    per_len = get_single_id_data_len(id.asBytes[0], id.asBytes[1]);

    Interval = period;
    if (mode == 0)
    {
        last_frm_no[uartx] = 0;
        sum_done_pnt = 0;
        max_pnt = (F_SIZE_LD - 5) / per_len;
        user_req_pnt = LIB_BcdToChar(rCommand[0]);
        if (user_req_pnt == 0)
        {
            return 0;
        }
        
        req_tm.sec = 0;
        #if   (APP_TIME_FRMT == HEX_FRMT)
        req_tm.min =	LIB_BcdToChar(rCommand[1]);
    	req_tm.hour = LIB_BcdToChar(rCommand[2]);
    	req_tm.day = LIB_BcdToChar(rCommand[3]);
    	req_tm.month = LIB_BcdToChar(rCommand[4]);
    	req_tm.year = LIB_BcdToChar(rCommand[5]);
        #else
        LIB_Reverse(&rCommand[1], (INT8U *)&req_tm, 5);
        #endif
        if(FALSE == LIB_ChkDateTime(&req_tm)) /*"时间非法"*/
        {
            user_req_pnt = 0;
            return 0;
        }
        inTime[0] = LIB_MToStdDate((ST_6TIME_FORM *)&req_tm);
        inTime[0] = get_align_tm(inTime[0], period); /*" 将时间按此类的周期进行对齐 "*/
        req_inTime[1] = inTime[0] + period * (user_req_pnt - 1);
        
        fst_rcd_tm = get_ch_first_rcrd_tm(Channel);
        if(inTime[0] < fst_rcd_tm)
        {
            inTime[0] = fst_rcd_tm;
        }

        if(req_inTime[1] < fst_rcd_tm)
        {
            user_req_pnt = 0;
            return 0;
        }

        req_inTime[0] = inTime[0];

        //GetIndexAttr(Channel, pLrCtrlData->endIndexId, &EndIndexAttr);
        lst_rcd_tm = get_ch_lst_rcd_tm(Channel);
        if(inTime[0] > lst_rcd_tm)
        {
            user_req_pnt = 0;
            return 0;
        }
        
        if(req_inTime[1] > lst_rcd_tm)
        {
            req_inTime[1] = lst_rcd_tm;
        }
        user_req_pnt =  (req_inTime[1] - req_inTime[0]) / period + 1;

        if (max_pnt > user_req_pnt)
        {
            max_pnt = user_req_pnt;
        }
        inTime[1] = inTime[0] + period * (max_pnt - 1);
            
        start_tm = inTime[0];
        //end_tm = inTime[1];
        MinToDateTime(start_tm, &rcd_date);
        #if   (APP_TIME_FRMT == HEX_FRMT)
        rBuffer[0] = LIB_CharToBcd(rcd_date.min);
        rBuffer[1] = LIB_CharToBcd(rcd_date.hour);
        rBuffer[2] = LIB_CharToBcd(rcd_date.day);
        rBuffer[3] = LIB_CharToBcd(rcd_date.month);
        rBuffer[4] = LIB_CharToBcd(rcd_date.year);
        #else
        LIB_Reverse((INT8U *)&rcd_date, &rBuffer[0], 5);
        #endif

        //LIB_MemCpy((INT8U *)&rCommand[1], rBuffer, 5); /*" 同一个buf "*/
        pBuf = rBuffer + 5; /*"前面5个字节时标"*/
    }
    else
    {
        cur_frm_no = rCommand[0];

        if((last_frm_no[uartx] + 1) == cur_frm_no)
        {
            max_pnt = (F_SIZE_LD - 1) / per_len;
            if((INT8U)(sum_done_pnt + max_pnt) > user_req_pnt)
            {
                max_pnt = user_req_pnt - sum_done_pnt;
            }
            
            inTime[0] = req_inTime[0];
            inTime[1] = inTime[0] + period * (max_pnt - 1);
            start_tm = inTime[0];
            //end_tm = inTime[1];

            pBuf = rBuffer;
            last_frm_no[uartx]++;
        }
        else if(last_frm_no[uartx] == cur_frm_no)
        {
            LIB_MemCpy(last_frm, rBuffer, last_frm_len);
            return last_frm_len;
        }
        else
        {
            user_req_pnt = 0;
            return 0;
        }
    }
    
    LRSearchRcrdInTimeByInterval(Channel, Interval, inTime);

    cur_tm = start_tm;
    cur_done_pnt = 0;
    for(i=0; i<RsultAttr.num; i++)
    {
        //WDT_FeedWdg();
        dataAddr = LR_get_rcrd_save_addr(Channel, RsultAttr.seq_no[i]);
        //FS_ReadFile(E_FILE_MIN_FRZ, dataAddr, BuffTemp, pSchemeAttr->DotLen);
        //LIB_MemCpy((INT8U *) &BuffTemp[0], (INT8U *)&TimeStamp, TMSTAMP_LEN);
        FS_ReadFile(E_FILE_MIN_FRZ, dataAddr, (INT8U *)&TimeStamp, TMSTAMP_LEN);
        TimeStamp &= TMSTAMP_MSK;
        if(cur_tm < TimeStamp)
        {
            while(cur_tm < TimeStamp)
            {
                if ((id.asBytes[1] == 6 || id.asBytes[1] == 7) && ((cur_done_pnt == 0) && (mode == 0)))
                {
                    if(RsultAttr.seq_no[i] != 0)
                    {
                        seq = RsultAttr.seq_no[i] - 1;
                        lr_one_pnt_data(Channel, seq, off, per_len, pBuf);
                    }
                    else
                    {
                        seq = RsultAttr.seq_no[i];
                        lr_one_pnt_data(Channel, seq, off, per_len, pBuf);
                    }
                    LIB_MemCpy(pBuf, lst_pnt_data, per_len);
                }
                patch_no_data_pnt(id.asBytes[0], id.asBytes[1], pBuf);
                
                pBuf += per_len;
                cur_tm += period;
                cur_done_pnt++;
                if(cur_done_pnt >= max_pnt)
                {
                    return pBuf-rBuffer;
                }
            }
        }
        
        /*"TODO 从记录中获取数据并格式转换 "*/
        seq = RsultAttr.seq_no[i];
        lr_one_pnt_data(Channel, seq, off, per_len, pBuf);
                    
        //LIB_MemCpy(BuffTemp+off, pBuf, per_len); 
        if (id.asBytes[1] == 6 || id.asBytes[1] == 7)
        {
            LIB_MemCpy(pBuf, lst_pnt_data, per_len);
        }
        
        pBuf += per_len;
        cur_tm += period;
        cur_done_pnt++;
        if(cur_done_pnt >= max_pnt)
        {
            break;
        }
    }

    /*" 一个点都没有 "*/
    if((cur_done_pnt == 0) && (mode == 0) && (id.asBytes[1] == 6 || id.asBytes[1] == 7))
    {
        res = Get_Rcd_from_time(Channel, inTime[0], &StartStampId, &start_seq, &rcd_tm);
        if(res == FALSE || start_seq == 0)
        {
            LIB_MemSet(0x00, pBuf, per_len);
        }
        else
        {
            seq = start_seq - 1;
            lr_one_pnt_data(Channel, seq, off, per_len, pBuf);
        }
        LIB_MemCpy(pBuf, lst_pnt_data, per_len);    
    }

    /*" 如果一个点都没有呢 "*/
    while(cur_done_pnt < max_pnt)
    {
        patch_no_data_pnt(id.asBytes[0], id.asBytes[1], pBuf);
        pBuf += per_len;
        cur_done_pnt++;
        cur_tm += period;
    }
    sum_done_pnt += cur_done_pnt;
    req_inTime[0] = cur_tm;

    return pBuf-rBuffer;
}


INT8U Find_First_Record(INT8U sort, INT32U * seq)
{
    INT8U ch;
    INT8U ret;

    if(sort == 0)
    {
        ret = FALSE;
        for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
        {
            seq[ch] = get_ch_first_rcrd_seq(ch);
            if(get_ch_last_rcrd_seq(ch) == INVALID_SEQ)
            {
                seq[ch] = INVALID_SEQ;
            }
            else
            {
                ret = TRUE;
            }
        }
    }
    else
    {
        ch = sort - 1;
        seq[0] = get_ch_first_rcrd_seq(ch);
        ret = TRUE;
        if(get_ch_last_rcrd_seq(ch) == INVALID_SEQ)
        {
            seq[0] = INVALID_SEQ;
            ret = FALSE;
        }
    }
    return ret;
}

INT8U Find_Next_Record(INT8U uartx, INT8U sort, INT32U * seq, INT32U * nxt_seq)
{
    INT8U ch;
    INT8U ret;
    
    if(sort == 0)
    {
        ret = FALSE;
        for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
        {
            if((used_seq[uartx] & (1 << ch)) == 0 )
            {
                nxt_seq[ch] = seq[ch];
            }
            else
            {
                if(seq[ch] < get_ch_last_rcrd_seq(ch))
                {
                    nxt_seq[ch] = seq[ch] + 1;
                }
                else
                {
                    nxt_seq[ch] = INVALID_SEQ;
                }
            }

            if(nxt_seq[ch] != INVALID_SEQ)
            {
                ret = TRUE;
            }
        }
    }
    else
    {
        ch = sort - 1;
        if(seq[0] < get_ch_last_rcrd_seq(ch))
        {
            nxt_seq[0] = seq[0] + 1;
            ret = TRUE;
        }
        else
        {
            nxt_seq[0] = INVALID_SEQ;
            ret = FALSE;
        }
    }   
    return ret;
}

INT8U Find_Near_Record(INT8U sort, INT32U * seq)
{
    INT8U ch;
    INT8U ret;

    if(sort == 0)
    {
        ret = FALSE;
        for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
        {
            seq[ch] = get_ch_last_rcrd_seq(ch);
            if(seq[ch] == INVALID_SEQ)
            {
                seq[ch] = INVALID_SEQ;
            }
            else
            {
                ret = TRUE;
            }
        }
    }
    else
    {
        ch = sort - 1;
        seq[0] = get_ch_last_rcrd_seq(ch);
        ret = TRUE;
        if(seq[0] == INVALID_SEQ)
        {
            seq[0] = INVALID_SEQ;
            ret = FALSE;
        }
    }
    return ret;
}

INT32U LR645_Get_Record_From_Time(INT8U ch, INT32U stamp)
{
    INT8U ret;
    INDEX_RANGE_TYPE idx_id;
    INT32U seq;
    INT32U rcd_tm, fst_rcd_tm;

    fst_rcd_tm = get_ch_first_rcrd_tm(ch);
    if(stamp < fst_rcd_tm)
    {
        stamp = fst_rcd_tm;
    }
    
    ret = Get_Rcd_from_time(ch, stamp, &idx_id, &seq, &rcd_tm);
    if(ret == TRUE)
    {
        return seq;
    }
    else
    {
        return INVALID_SEQ;
    }
}

INT8U Find_Time_Record(INT8U sort, ST_6TIME_FORM *time, INT32U * seq)
{
    INT8U ch;
    INT8U ret;
    INT32U stamp;

    stamp = LIB_MToStdDate(time);
    if(sort == 0)
    {
        ret = FALSE;
        for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
        {
            seq[ch] = LR645_Get_Record_From_Time(ch, stamp);
            if(seq[ch] == INVALID_SEQ)
            {
                ;
            }
            else
            {
                ret = TRUE;
            }
        }
    }
    else
    {
        ch = sort - 1;
        seq[0] = LR645_Get_Record_From_Time(ch, stamp);
        ret = TRUE;
        if(seq[0] == INVALID_SEQ)
        {
            ret = FALSE;
        }
    }
    return ret;
}


/*" 再加个参数，取最早的还是最晚的记录 "*/
INT8U Get_Record_Data_Base_Seq(INT8U uartx, INT8U sort, INT8U fst_rcd, INT32U * seq, INT8U *buf)
{
    INT8U i;
    INT8U ch;
    INT8U len;
    INT8U dotlen;
    INT8U sum;
    INT32U dataAddr;
    INT8U *pbuf;
    INT8U data[6][40];
    INT32U stamp[6];
    INT32U cur_stamp;
    ST_6TIME_FORM rcd_date;

    len = 0;
    buf[0] = 0xA0;
    buf[1] = 0xA0;
    buf[2] = len; /*" 长度 "*/
    pbuf = buf + 3;
    if(sort == 0)
    {
        cur_stamp = 0xFFFFFFFFUL;
        for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
        {
            if(seq[ch] != INVALID_SEQ)
            {
                dotlen = mSchemeAttr.SchemeAttr[ch].DotLen;
                dataAddr = LR_get_rcrd_save_addr(ch, seq[ch]);
                FS_ReadFile(E_FILE_MIN_FRZ, dataAddr, &data[ch][0], dotlen);
                LIB_MemCpy((INT8U *)&data[ch][0], (INT8U *)&stamp[ch], TMSTAMP_LEN);
                stamp[ch] &= TMSTAMP_MSK;
                cur_stamp = stamp[ch];
            }
            else
            {
                stamp[ch] = 0xFFFFFFFFUL;
            }
        }

        if(cur_stamp == 0xFFFFFFFFUL)
        {
            return 0;
        }

        //cur_stamp = stamp[0];
        if(fst_rcd == 1)
        {
            for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
            {
                if(cur_stamp > stamp[ch])
                {
                    cur_stamp = stamp[ch];
                }
            }
        }
        else
        {
            for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
            {
                if((cur_stamp < stamp[ch]) && (stamp[ch] != 0xFFFFFFFFUL))
                {
                    cur_stamp = stamp[ch];
                }
            }
        }

        
        MinToDateTime(cur_stamp, &rcd_date);
        LIB_CharToBcdNByte((INT8U *)&rcd_date, 6);
        LIB_MemCpy((INT8U *)&rcd_date, pbuf, 5);
        LIB_RvsSelf(pbuf, 5);
        pbuf += 5;

        used_seq[uartx] = 0;
        for(ch=0; ch<mSchemeAttr.SchemeNum; ch++)
        {
            if(cur_stamp == stamp[ch])
            {
                used_seq[uartx] |= (1 << ch);
                dotlen = mSchemeAttr.SchemeAttr[ch].DotLen;
                LIB_MemCpy((INT8U *)&data[ch][TMSTAMP_LEN], pbuf, dotlen - TMSTAMP_LEN - LD_CRC_SZ);
                pbuf += (dotlen - TMSTAMP_LEN - LD_CRC_SZ);
            }
            pbuf[0] = 0xAA;
            pbuf++;
        }
        buf[2] = pbuf - buf - 3; /*" 长度 "*/
    }
    else
    {
        ch = sort - 1;
        if(seq[0] != INVALID_SEQ)
        {
            dotlen = mSchemeAttr.SchemeAttr[ch].DotLen;
            dataAddr = LR_get_rcrd_save_addr(ch, seq[0]);
            FS_ReadFile(E_FILE_MIN_FRZ, dataAddr, &data[ch][0], dotlen);
            LIB_MemCpy((INT8U *)&data[ch][0], (INT8U *)&cur_stamp, TMSTAMP_LEN);
            cur_stamp &= TMSTAMP_MSK;

            MinToDateTime(cur_stamp, &rcd_date);
            LIB_CharToBcdNByte((INT8U *)&rcd_date, 6);
            LIB_MemCpy((INT8U *)&rcd_date, pbuf, 5);
            LIB_RvsSelf(pbuf, 5);
            pbuf += 5;

            LIB_MemCpy((INT8U *)&data[ch][TMSTAMP_LEN], pbuf, dotlen - TMSTAMP_LEN - LD_CRC_SZ);
            pbuf += (dotlen - TMSTAMP_LEN - LD_CRC_SZ);

            pbuf[0] = 0xAA;
            pbuf++;

            buf[2] = pbuf - buf - 3; /*" 长度 "*/
        }
        else
        {
            buf[0] = 0xE0;
            buf[1] = 0xE0;
            buf[2] = 0; /*" 长度 "*/
        }
    }

    sum = 0;
	for(i=0; i<pbuf - buf; i++)
	{
		sum += buf[i];
	}
	*(pbuf++) = sum;
	*(pbuf++) = 0xe5;

    return pbuf - buf;
}

INT16U LR645_Read_Data(UN_ID645 id, INT8U *rCommand, INT8U *rBuffer, INT8U mode, INT8U uartx)
{
    INT8U sort;
    INT8U fst_rcd;
    INT16U len;
    INT8U read_mode,cur_frm_no;	
    INT16S ret;
    ST_6TIME_FORM date;
    INT32U seq[6];
	
	if (id.asBytes[2] == 0x11)
	{
        len = read_record_base_time_id(id, rCommand, rBuffer, mode, uartx);
        if (sum_done_pnt < user_req_pnt)
        {
            len |= 0x8000;
        }
        return len;
	}
    
	read_mode = id.asBytes[0];	/*"读数据模式"*/
	                            /*"0x00，最早时间"*/
                                /*"0x01，按给定日期"*/
								/*"0x02，最近的"*/
                                /*"0x03，上次读的下条"*/
								/*"0x04，上次读的"*/
								/*"0x05，0地址"*/

    sort = id.asBytes[2];
    fst_rcd = 1;
	if(mode == 0)
	{
        last_frm_no[uartx] = 0;
        if(LIB_CheckBcdByte(rCommand[0]) == FALSE)
        {
            return 0;
        }
        Read_Count[uartx] = LIB_BcdToChar(rCommand[0]);

        if(Read_Count[uartx] == 0)
        {
            return 0;
        }

		switch(read_mode)
		{	
			case 0x00:
				ret = Find_First_Record(sort, seq);
				break;	
            case 0x01:
				date.sec = 0;
				date.min =	*(rCommand+1);
				date.hour =		*(rCommand+2);
				date.day =		*(rCommand+3);
				date.month =	*(rCommand+4);
				date.year =		*(rCommand+5);
                if (LIB_CheckBCD(&date.year, 6) == FALSE)
                {
                    return 0;
                }
                #if   (APP_TIME_FRMT == HEX_FRMT)
                LIB_BcdToCharNByte(&date.year, 6);
                #endif
				ret = Find_Time_Record(sort, &date, seq);
				break;
			case 0x02:
				if(Read_Count[uartx] != 1)  
                {
                    return 0;
                }
				fst_rcd = 0;
				ret = Find_Near_Record(sort, seq);			
				break;
            case 0x03:
				ret = Find_Next_Record(uartx, sort, &ch_seq[uartx][0], seq);
				break;	
			case 0x04:
				//ret=Find_Last_Record(uartx,&nReadPage);
				break;	
			case 0x05:
				//ret=Find_Addr_Record(&nReadPage,&nReadByte,0);
				break;	
		}

		if(ret == FALSE)
		{
			return 0;
		}
        LIB_MemCpy((INT8U *)&seq[0], (INT8U *)&ch_seq[uartx][0], sizeof(seq));
	}
	else
	{
		cur_frm_no = rCommand[0];/*" 帧序号为16进制 ?? "*/
		if((last_frm_no[uartx] + 1) == cur_frm_no)
		{/*"请求的块号等于上一帧的块号+1,返回下一帧"*/
			if(Read_Count[uartx] != 0)
			{
				ret = Find_Next_Record(uartx, sort, &ch_seq[uartx][0], seq);
                LIB_MemCpy((INT8U *)&seq[0], (INT8U *)&ch_seq[uartx][0], sizeof(seq));
			}
			else
			{
				return 0x8000;
			}
			if(ret != TRUE) 
            {
                return 0x8000;
			}
		}
		else if(last_frm_no[uartx] == cur_frm_no)
		{/*"请求的块号等于上一帧的块号,重发上一帧"*/
			LIB_MemCpy((INT8U *)&ch_seq[uartx][0], (INT8U *)&seq[0], sizeof(seq));
				
		}
		else
		{
			return 0x8000;
		}
		
	}

    len = Get_Record_Data_Base_Seq(uartx, sort, fst_rcd, seq, rBuffer);

	if(mode != 0)
    {
		if(last_frm_no[uartx] != cur_frm_no)
		{	
			last_frm_no[uartx]++;
			Read_Count[uartx]--;
		}
    }
	else
    {
        Read_Count[uartx]--;
    }
	
	if(Read_Count[uartx] != 0)
	{
        if(Find_Next_Record(uartx, sort, &ch_seq[uartx][0], seq) != TRUE)
        {
            Read_Count[uartx] = 0;
        }
    }

	if(Read_Count[uartx] != 0)
	{
		return len|0x8000;
	}
	else
	{
		return len;
	}
}

INT16S check_id_error(INT8U di0, INT8U di1)
{
    INT16S ret;

    ret = -1;
    switch(di1)
    {
    case 1:
        if((di0 > 0 && di0 < 4) || di0 == 0xFF)
            ret = 0;
        break;
    case 2:
        if((di0 > 0 && di0 < 4) || di0 == 0xFF)
            ret = 0;
        break;   
        
    case 3:
        if(( di0 < 4) || di0 == 0xFF)
            ret = 0;
        break; 
    case 4:
        if((di0 < 4) || di0 == 0xFF)
            ret = 0;
        break; 
    case 5:
        if((di0 < 4) || di0 == 0xFF)
            ret = 0;
        break; 
        
    case 6:
        if((di0 >= 1 && di0 <= 4) || di0 == 0xFF)
            ret = 0;
        break; 
    case 7:
        if((di0 >= 1 && di0 <= 4) || di0 == 0xFF)
            ret = 0;
        break; 
    case 8:
        if((di0 >= 1 && di0 <= 2) || di0 == 0xFF)
            ret = 0;
        break; 

    default:
        break;
    } 
#ifdef SINGLE_PHASE
    if((di1 >= 3 && di1 <= 5) && (di0 == 0xFF))
    {
        ret = -1;
    }
#endif
    return ret;
}


INT8U check_is_lr645_id(UN_ID645 id, INT8U mode, INT8U data_len)
{
    if (id.asBytes[2]!=0x11)
    {
		if(mode==0)
		{
			if((id.asBytes[0]==0)&&(data_len!=1)) return 0;
			if((id.asBytes[0]==1)&&(data_len!=6)) return 0;
			if((id.asBytes[0]==2)&&(data_len!=1)) return 0;
		}
		else if(mode==1)
		{
			if(data_len!=1) return 0;
		}
		else
		{
			return 0;
		}
    }
    else
    {
        if (check_id_error(id.asBytes[0], id.asBytes[1]) != 0)
        {
            return 0;
        }
        if(mode==0)
		{
			if(data_len!=6) return 0;
		}
		else if(mode==1)
		{
			if(data_len!=1) return 0;
		}
		else
		{
			return 0;
		}
    }
    return 1;
}

/*"****************************************************
name:LR_GetRecord
function:get of records  
input:645-pointer
output:data buffer-pointer
return:data length,0 indicats failure
call:
     CommandCheck
     SelfParaCheck
     OrganizeData
debug:2014-05-27   PASS
*****************************************************"*/
INT16U LR_GetRecord(ST_FRM_RESULT *pInBuff,ST_ANSWER_DATA *pOutBuff, INT8U mode)
{
    INT16U len;

    len =  LR645_Read_Data(pInBuff->dataId, 
        pInBuff->data, pOutBuff->Data_addr, mode, pInBuff->commPort);
    pOutBuff->Data_len.int_len = len;

    return len;
}

