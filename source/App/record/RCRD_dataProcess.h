#ifndef RCRD_DATAPROC_H
#define RCRD_DATAPROC_H

#define MAX_FRZ_PRJ  (8)/*"最大冻结方案数"*/

typedef struct
{
    INT16U evtObis;
    INT16U flgObis;
    //INT8U offset;
}ST_EVT_FLG;/*"有发生、结束数据项的事件发生标志"*/

typedef struct
{
    INT8U prjNo;/*"冻结方案号"*/
    INT8U index;/*"对应关联属性列表的首个索引"*/
}ST_FRZ_PRJ;


typedef  struct
{
    INT16U  obis;
    INT8U   zipLeafS;
    INT8U   zipLeafE;
}ST_DRV_OBJ_ZIP_ATTR;

#define  RCRD_SINGLE_LIST_NUM  (sizeof(stRcrdSingleDataList)/sizeof(ST_SINGLE_DATA_ATTR))
#define  RCRD_LIST_NUM  (sizeof(stFileDataList)/sizeof(ST_FILE_ATTR))

#define  RCRD_CNT(obis) (E_RCRD_CNT_BLK + ((obis) & 0x00ff))

extern INT8U meter_type;
#if defined(THREE_PHASE)
extern INT32U lst_pd_amp[3];
extern void upd_evt_amp_eeprom(void);
extern void poweron_init_evt_amp(void);
#endif


extern INT8U RCRD_GetRcrd(EN_RCRD_FILE_ID id, void *pdin, void *pdout, INT16U len);
extern INT8U RCRD_SaveData(EN_RCRD_FILE_ID file, EN_FELXOBJ_ID obis);
extern INT16S RCRD_Get(INT16U id, INT8U *pBuff);
extern INT8S RCRD_Set (INT16U id, INT8U *pData, INT16U len);
extern INT8S RCRD_Action (INT16U id, void * pdata, void * pbuff);
extern INT8U RCRD_GetFrmt (INT16U id);

extern INT32U get_data_addr_off(INT16U num_per_sec, INT16U sum_sector, INT16U per_sz, INT32U seq_no);
extern INT8S RCRD_ClrRecordData(EN_RCRD_FILE_ID obis);
extern INT8U ChkIsNonPeriodFrz(INT16U rcrd_obis);
extern INT8U is_private_rcrd(EN_RCRD_FILE_ID rcrd_obis);
extern INT8U rcrd_id_is_error(EN_RCRD_FILE_ID rcrd_obis);
extern INT8S RCRD_ClrDataMdfRoad(EN_RCRD_FILE_ID rcrd_obis);

extern INT8U real_prd_frz_num_proc(INT32U ee_addr, INT32U max_num, INT32U add);
extern INT8U real_prd_frz_num_dec(INT32U ee_addr, INT32U dec);
extern void RCRD_ClrSpecData(EN_RCRD_FILE_ID rcrd_obis);
extern INT8U check_evt_is_end(EN_RCRD_FILE_ID rcrd_obis, INT32U seq, INT8U *buf);
extern void erase_flash_sector_part_data(ENUM_FS_ID flname, INT32U StartAdrr);
extern INT16U RCRD_ReadRcrdData(EN_RCRD_FILE_ID file, INT16U index,  INT8U leaf, INT8U *pbuff, ST_FMT_ATTR *pformat);
extern INT8U Set_LoadRecoedPeriod(INT8U *pdata);
extern INT8U Set_LoadRecoedBeginTime(INT8U *pdata);
extern INT8U get_rcrd_data_to_buf(EN_RCRD_FILE_ID file, EN_FELXOBJ_ID obis, INT8U *Buff, INT16U *Len);
extern INT8U RCRD_ChkEvntCnt(EN_RCRD_FILE_ID file, INT16U index);

#endif
