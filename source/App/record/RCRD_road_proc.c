#include "system\Sys_def.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
#include "RCRD.h"
#include "LR.h"
#include "RCRD_dataProcess.h"
#include "DataCenterSoftware\Code\record\RCRD_data.h"
#include "app\userinterface\PP\PP_698\Class\class_comm.h"
#include "app\userinterface\PP\PP_698\Process\Pp_macro.h"
#include "app\userinterface\PP\PP_698\Process\Pp_process.h"
//static ST_FRZ_PRJ  mstFrzPrj[MAX_FRZ_PRJ];/*"分钟冻结方案列表"*/
#define FRZ_PRJ_LIST_LEN    (MAX_FRZ_PRJ * sizeof(ST_FRZ_PRJ))
#define ROADINFO_BUFF_LEN   (MAX_RCRD_ROAD_NUM*8)

INT8U mSetROadLstFlg;

#ifdef THREE_PHASE
/*事件关联默认关联关联OAD初始化*/
/*"3000 失压、3001欠压、3002过压、3003段相"*/
const ST_OAD DefLostVoltRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0030,0x22,0x01},/*" 事件发生时刻组合无功1总电能"*/
    {0x0040,0x22,0x01},/*" 事件发生时刻组合无功2总电能"*/
    {0x0011,0x22,0x01},/*" 事件发生时刻A相正向有功电能"*/
    {0x0021,0x22,0x01},/*" 事件发生时刻A相反向有功电能"*/
    {0x0031,0x22,0x01},/*" 事件发生时刻A相组合无功1电能"*/
    {0x0041,0x22,0x01},/*" 事件发生时刻A相组合无功2电能"*/
    {0x0012,0x22,0x01},/*" 事件发生时刻B相正向有功电能"*/
    {0x0022,0x22,0x01},/*" 事件发生时刻B相反向有功电能"*/
    {0x0032,0x22,0x01},/*" 事件发生时刻B相组合无功1电能"*/
    {0x0042,0x22,0x01},/*" 事件发生时刻B相组合无功2电能"*/
    {0x0013,0x22,0x01},/*" 事件发生时刻C相正向有功电能"*/
    {0x0023,0x22,0x01},/*" 事件发生时刻C相反向有功电能"*/
    {0x0033,0x22,0x01},/*" 事件发生时刻C相组合无功1电能"*/
    {0x0043,0x22,0x01},/*" 事件发生时刻C相组合无功2电能"*/
    {0x2000,0x22,0x00},/*" 事件发生时刻电压"*/
    {0x2001,0x22,0x00},/*" 事件发生时刻电流"*/
    {0x2004,0x22,0x00},/*" 事件发生时刻有功功率"*/
    {0x2005,0x22,0x00},/*" 事件发生时刻无功功率"*/
    {0x200A,0x22,0x00},/*" 事件发生时刻功率因数"*/
    {0x2029,0x62,0x00},/*" 事件发生期间安时值"*/
    {0x0010,0x82,0x01},/*" 事件结束后正向有功总电能"*/
    {0x0020,0x82,0x01},/*" 事件结束后反向有功总电能"*/
    {0x0030,0x82,0x01},/*" 事件结束后组合无功1总电能"*/
    {0x0040,0x82,0x01},/*" 事件结束后组合无功2总电能"*/
    {0x0011,0x82,0x01},/*" 事件结束后A相正向有功电能"*/
    {0x0021,0x82,0x01},/*" 事件结束后A相反向有功电能"*/
    {0x0031,0x82,0x01},/*" 事件结束后A相组合无功1电能"*/
    {0x0041,0x82,0x01},/*" 事件结束后A相组合无功2电能"*/
    {0x0012,0x82,0x01},/*" 事件结束后B相正向有功电能"*/
    {0x0022,0x82,0x01},/*" 事件结束后B相反向有功电能"*/
    {0x0032,0x82,0x01},/*" 事件结束后B相组合无功1电能"*/
    {0x0042,0x82,0x01},/*" 事件结束后B相组合无功2电能"*/
    {0x0013,0x82,0x01},/*" 事件结束后C相正向有功电能"*/
    {0x0023,0x82,0x01},/*" 事件结束后C相反向有功电能"*/
    {0x0033,0x82,0x01},/*" 事件结束后C相组合无功1电能"*/
    {0x0043,0x82,0x01},/*" 事件结束后C相组合无功2电能"*/
};

/*过流事件*/
/*过流事件记录 3005*/
const ST_OAD DefOverCurRoadInfo[]=
{
    {0x0010,0x22,0x01}, /*00102201--事件发生时刻正向有功总电能*/
    {0x0020,0x22,0x01}, /*00202201--事件发生时刻反向有功总电能*/   
    {0x2000,0x22,0x00}, /*20002200--事件发生时刻电压*/   
    {0x2001,0x22,0x00}, /*20012200--事件发生时刻电流*/   
    {0x2004,0x22,0x00}, /*20042200--事件发生时刻有功功率*/   
    {0x200A,0x22,0x00}, /*200A2200--事件发生时刻功率因数*/   
    {0x0010,0x82,0x01}, /*00108201--事件结束后正向有功总电能*/   
    {0x0020,0x82,0x01}, /*00208201--事件结束后反向有功总电能*/   

};

/*" 失流/断流/过流"*/
const ST_OAD DefLostCurrtRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0030,0x22,0x01},/*" 事件发生时刻组合无功1总电能"*/
    {0x0040,0x22,0x01},/*" 事件发生时刻组合无功2总电能"*/
    {0x0011,0x22,0x01},/*" 事件发生时刻A相正向有功电能"*/
    {0x0021,0x22,0x01},/*" 事件发生时刻A相反向有功电能"*/
    {0x0031,0x22,0x01},/*" 事件发生时刻A相组合无功1电能"*/
    {0x0041,0x22,0x01},/*" 事件发生时刻A相组合无功2电能"*/
    {0x0012,0x22,0x01},/*" 事件发生时刻B相正向有功电能"*/
    {0x0022,0x22,0x01},/*" 事件发生时刻B相反向有功电能"*/
    {0x0032,0x22,0x01},/*" 事件发生时刻B相组合无功1电能"*/
    {0x0042,0x22,0x01},/*" 事件发生时刻B相组合无功2电能"*/
    {0x0013,0x22,0x01},/*" 事件发生时刻C相正向有功电能"*/
    {0x0023,0x22,0x01},/*" 事件发生时刻C相反向有功电能"*/
    {0x0033,0x22,0x01},/*" 事件发生时刻C相组合无功1电能"*/
    {0x0043,0x22,0x01},/*" 事件发生时刻C相组合无功2电能"*/
    {0x2000,0x22,0x00},/*" 事件发生时刻电压"*/
    {0x2001,0x22,0x00},/*" 事件发生时刻电流"*/
    {0x2004,0x22,0x00},/*" 事件发生时刻有功功率"*/
    {0x2005,0x22,0x00},/*" 事件发生时刻无功功率"*/
    {0x200A,0x22,0x00},/*" 事件发生时刻功率因数"*/
    {0x0010,0x82,0x01},/*" 事件结束后正向有功总电能"*/
    {0x0020,0x82,0x01},/*" 事件结束后反向有功总电能"*/
    {0x0030,0x82,0x01},/*" 事件结束后组合无功1总电能"*/
    {0x0040,0x82,0x01},/*" 事件结束后组合无功2总电能"*/
    {0x0011,0x82,0x01},/*" 事件结束后A相正向有功电能"*/
    {0x0021,0x82,0x01},/*" 事件结束后A相反向有功电能"*/
    {0x0031,0x82,0x01},/*" 事件结束后A相组合无功1电能"*/
    {0x0041,0x82,0x01},/*" 事件结束后A相组合无功2电能"*/
    {0x0012,0x82,0x01},/*" 事件结束后B相正向有功电能"*/
    {0x0022,0x82,0x01},/*" 事件结束后B相反向有功电能"*/
    {0x0032,0x82,0x01},/*" 事件结束后B相组合无功1电能"*/
    {0x0042,0x82,0x01},/*" 事件结束后B相组合无功2电能"*/
    {0x0013,0x82,0x01},/*" 事件结束后C相正向有功电能"*/
    {0x0023,0x82,0x01},/*" 事件结束后C相反向有功电能"*/
    {0x0033,0x82,0x01},/*" 事件结束后C相组合无功1电能"*/
    {0x0043,0x82,0x01},/*" 事件结束后C相组合无功2电能"*/
};



/*"3007 潮流反向、3008过载、300F电压逆相序事件、3010电流逆相序"*/
const ST_OAD DefUpLoadRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0030,0x22,0x01},/*" 事件发生时刻组合无功1总电能"*/
    {0x0040,0x22,0x01},/*" 事件发生时刻组合无功2总电能"*/
    {0x0011,0x22,0x01},/*" 事件发生时刻A相正向有功电能"*/
    {0x0021,0x22,0x01},/*" 事件发生时刻A相反向有功电能"*/
    {0x0031,0x22,0x01},/*" 事件发生时刻A相组合无功1电能"*/
    {0x0041,0x22,0x01},/*" 事件发生时刻A相组合无功2电能"*/
    {0x0012,0x22,0x01},/*" 事件发生时刻B相正向有功电能"*/
    {0x0022,0x22,0x01},/*" 事件发生时刻B相反向有功电能"*/
    {0x0032,0x22,0x01},/*" 事件发生时刻B相组合无功1电能"*/
    {0x0042,0x22,0x01},/*" 事件发生时刻B相组合无功2电能"*/
    {0x0013,0x22,0x01},/*" 事件发生时刻C相正向有功电能"*/
    {0x0023,0x22,0x01},/*" 事件发生时刻C相反向有功电能"*/
    {0x0033,0x22,0x01},/*" 事件发生时刻C相组合无功1电能"*/
    {0x0043,0x22,0x01},/*" 事件发生时刻C相组合无功2电能"*/

    {0x0010,0x82,0x01},/*" 事件结束后正向有功总电能"*/
    {0x0020,0x82,0x01},/*" 事件结束后反向有功总电能"*/
    {0x0030,0x82,0x01},/*" 事件结束后组合无功1总电能"*/
    {0x0040,0x82,0x01},/*" 事件结束后组合无功2总电能"*/
    {0x0011,0x82,0x01},/*" 事件结束后A相正向有功电能"*/
    {0x0021,0x82,0x01},/*" 事件结束后A相反向有功电能"*/
    {0x0031,0x82,0x01},/*" 事件结束后A相组合无功1电能"*/
    {0x0041,0x82,0x01},/*" 事件结束后A相组合无功2电能"*/
    {0x0012,0x82,0x01},/*" 事件结束后B相正向有功电能"*/
    {0x0022,0x82,0x01},/*" 事件结束后B相反向有功电能"*/
    {0x0032,0x82,0x01},/*" 事件结束后B相组合无功1电能"*/
    {0x0042,0x82,0x01},/*" 事件结束后B相组合无功2电能"*/
    {0x0013,0x82,0x01},/*" 事件结束后C相正向有功电能"*/
    {0x0023,0x82,0x01},/*" 事件结束后C相反向有功电能"*/
    {0x0033,0x82,0x01},/*" 事件结束后C相组合无功1电能"*/
    {0x0043,0x82,0x01},/*" 事件结束后C相组合无功2电能"*/
};

/*" 3009 300A 300B 无关联对象"*/

/*" 300C 电能表功率因数超下限事件"*/
const ST_OAD DefOverFactorRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0030,0x22,0x01},/*" 事件发生时刻组合无功1总电能"*/
    {0x0040,0x22,0x01},/*" 事件发生时刻组合无功2总电能"*/


    {0x0010,0x82,0x01},/*" 事件结束后正向有功总电能"*/
    {0x0020,0x82,0x01},/*" 事件结束后反向有功总电能"*/
    {0x0030,0x82,0x01},/*" 事件结束后组合无功1总电能"*/
    {0x0040,0x82,0x01},/*" 事件结束后组合无功2总电能"*/
};

/*" 300d 全失压事件"*/
const ST_OAD DefPhase3LostVoltRoad[] =
{
    {0x2001,0x22,0x00},/*" 事件发生前电流 "*/
};

/*" 300E 3011 3012无关联对象"*/


/*"3013 电表清零"*/
const ST_OAD Def_ClearMeterRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0050,0x22,0x01},/*" 电表清零前第一象限无功总电能"*/
    {0x0060,0x22,0x01},/*" 电表清零前第二象限无功总电能"*/
    {0x0070,0x22,0x01},/*" 电表清零前第三象限无功总电能"*/
    {0x0080,0x22,0x01},/*" 电表清零前第四象限无功总电能"*/
        
    {0x0011,0x22,0x01},/*" 事件发生时刻A正向有功总电能"*/
    {0x0021,0x22,0x01},/*" 事件发生时刻A反向有功总电能"*/
    {0x0051,0x22,0x01},/*" 电表清零前第A一象限无功总电能"*/
    {0x0061,0x22,0x01},/*" 电表清零前第A二象限无功总电能"*/
    {0x0071,0x22,0x01},/*" 电表清零前第A三象限无功总电能"*/
    {0x0081,0x22,0x01},/*" 电表清零前第A四象限无功总电能"*/

    {0x0012,0x22,0x01},/*" 事件发生时刻B正向有功总电能"*/
    {0x0022,0x22,0x01},/*" 事件发生时刻B反向有功总电能"*/
    {0x0052,0x22,0x01},/*" 电表清零前第B一象限无功总电能"*/
    {0x0062,0x22,0x01},/*" 电表清零前第B二象限无功总电能"*/
    {0x0072,0x22,0x01},/*" 电表清零前第B三象限无功总电能"*/
    {0x0082,0x22,0x01},/*" 电表清零前第B四象限无功总电能"*/

    {0x0013,0x22,0x01},/*" 事件发生时刻C正向有功总电能"*/
    {0x0023,0x22,0x01},/*" 事件发生时刻C反向有功总电能"*/
    {0x0053,0x22,0x01},/*" 电表清零前第C一象限无功总电能"*/
    {0x0063,0x22,0x01},/*" 电表清零前第C二象限无功总电能"*/
    {0x0073,0x22,0x01},/*" 电表清零前第C三象限无功总电能"*/
    {0x0083,0x22,0x01},/*" 电表清零前第C四象限无功总电能"*/

};

/*"3014 需量清零"*/
const ST_OAD Def_ClearDemadnRoad[] =
{
    {0x1010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x1020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x1050,0x22,0x01},/*" 电表清零前第一象限无功总电能"*/
    {0x1060,0x22,0x01},/*" 电表清零前第二象限无功总电能"*/
    {0x1070,0x22,0x01},/*" 电表清零前第三象限无功总电能"*/
    {0x1080,0x22,0x01},/*" 电表清零前第四象限无功总电能"*/
#if 0        
    {0x1011,0x22,0x01},/*" 事件发生时刻A正向有功总电能"*/
    {0x1021,0x22,0x01},/*" 事件发生时刻A反向有功总电能"*/
    {0x1051,0x22,0x01},/*" 电表清零前第A一象限无功总电能"*/
    {0x1061,0x22,0x01},/*" 电表清零前第A二象限无功总电能"*/
    {0x1071,0x22,0x01},/*" 电表清零前第A三象限无功总电能"*/
    {0x1081,0x22,0x01},/*" 电表清零前第A四象限无功总电能"*/

    {0x1012,0x22,0x01},/*" 事件发生时刻B正向有功总电能"*/
    {0x1022,0x22,0x01},/*" 事件发生时刻B反向有功总电能"*/
    {0x1052,0x22,0x01},/*" 电表清零前第B一象限无功总电能"*/
    {0x1062,0x22,0x01},/*" 电表清零前第B二象限无功总电能"*/
    {0x1072,0x22,0x01},/*" 电表清零前第B三象限无功总电能"*/
    {0x1082,0x22,0x01},/*" 电表清零前第B四象限无功总电能"*/

    {0x1013,0x22,0x01},/*" 事件发生时刻C正向有功总电能"*/
    {0x1023,0x22,0x01},/*" 事件发生时刻C反向有功总电能"*/
    {0x1053,0x22,0x01},/*" 电表清零前第C一象限无功总电能"*/
    {0x1063,0x22,0x01},/*" 电表清零前第C二象限无功总电能"*/
    {0x1073,0x22,0x01},/*" 电表清零前第C三象限无功总电能"*/
    {0x1083,0x22,0x01},/*" 电表清零前第C四象限无功总电能"*/
#endif
};

/*" 3015 3017无关联对象"*/


/*3016 校时事件记录默认关联OAD存储内容 3016*/
const ST_OAD DefJustTimeRoadInfo[]=
{
    {0x4000,0x22,0x00},/*40002200--校时前时间*/
    {0x4000,0x82,0x00},/*40008200--校时后时间*/
};


/* 3018 时区编程事件记录默认关联OAD存储内容3018*/
const ST_OAD DefPrgYslotRoadInfo[]=
{
    //{0x4014,0x22,0x00},/*40142200--时区编程前当前套时区表*/
    {0x4015,0x22,0x00},/*40152200--时区编程前备用套时区表*/
};


/*3019 周休日编程事件记录默认关联OAD存储内容 3019
40132200--编程前周休日釆用的日时段表号
*/
const ST_OAD DefPrgWeekRoadInfo[]=
{
    {0x4013,0x22,0x00},/*40132200--编程前周休日釆用的日时段表号*/
};

/*结算日编程事件记录默认关联OAD存储内容 301A
*/
const ST_OAD DefPrgBillRoadInfo[]=
{
    {0x4116,0x22,0x00},/*41162200--编程前结算日*/

};

/* 301B 301C 开盖事件事件记录默认关联OAD */
/* 301F 跳闸事件记录默认关联OAD存储内容 */
/* 3020 合闸事件记录默认关联OAD存储内容*/
const ST_OAD DefOpenCvrRoadInfo[]=
{
    {0x0010,0x22,0x01},   /*00102201--开表盖前正向有功总电能*/
    {0x0020,0x22,0x01},   /* 00202201--开表盖前反向有功总电能*/
    {0x0050,0x22,0x01},/*" 电表清零前第一象限无功总电能"*/
    {0x0060,0x22,0x01},/*" 电表清零前第二象限无功总电能"*/
    {0x0070,0x22,0x01},/*" 电表清零前第三象限无功总电能"*/
    {0x0080,0x22,0x01},/*" 电表清零前第四象限无功总电能"*/

    {0x0010,0x82,0x01},   /* 00108201--开表盖后正向有功总电能*/  
    {0x0020,0x82,0x01},    /*00208201--开表盖后反向有功总电能*/
    {0x0050,0x82,0x01},/*" 电表清零前第一象限无功总电能"*/
    {0x0060,0x82,0x01},/*" 电表清零前第二象限无功总电能"*/
    {0x0070,0x82,0x01},/*" 电表清零前第三象限无功总电能"*/
    {0x0080,0x82,0x01},/*" 电表清零前第四象限无功总电能"*/
};

/*" 301d "*/
const ST_OAD DefVoltImBalanceRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0030,0x22,0x01},/*" 事件发生时刻组合无功1总电能"*/
    {0x0040,0x22,0x01},/*" 事件发生时刻组合无功2总电能"*/
    {0x0011,0x22,0x01},/*" 事件发生时刻A相正向有功电能"*/
    {0x0021,0x22,0x01},/*" 事件发生时刻A相反向有功电能"*/
    {0x0031,0x22,0x01},/*" 事件发生时刻A相组合无功1电能"*/
    {0x0041,0x22,0x01},/*" 事件发生时刻A相组合无功2电能"*/
    {0x0012,0x22,0x01},/*" 事件发生时刻B相正向有功电能"*/
    {0x0022,0x22,0x01},/*" 事件发生时刻B相反向有功电能"*/
    {0x0032,0x22,0x01},/*" 事件发生时刻B相组合无功1电能"*/
    {0x0042,0x22,0x01},/*" 事件发生时刻B相组合无功2电能"*/
    {0x0013,0x22,0x01},/*" 事件发生时刻C相正向有功电能"*/
    {0x0023,0x22,0x01},/*" 事件发生时刻C相反向有功电能"*/
    {0x0033,0x22,0x01},/*" 事件发生时刻C相组合无功1电能"*/
    {0x0043,0x22,0x01},/*" 事件发生时刻C相组合无功2电能"*/

    {0x2026,0x62,0x00},/*" 事件发生期间电压不平衡率0x2026, 0x2027电流不平衡"*/

    {0x0010,0x82,0x01},/*" 事件结束后正向有功总电能"*/
    {0x0020,0x82,0x01},/*" 事件结束后反向有功总电能"*/
    {0x0030,0x82,0x01},/*" 事件结束后组合无功1总电能"*/
    {0x0040,0x82,0x01},/*" 事件结束后组合无功2总电能"*/
    {0x0011,0x82,0x01},/*" 事件结束后A相正向有功电能"*/
    {0x0021,0x82,0x01},/*" 事件结束后A相反向有功电能"*/
    {0x0031,0x82,0x01},/*" 事件结束后A相组合无功1电能"*/
    {0x0041,0x82,0x01},/*" 事件结束后A相组合无功2电能"*/
    {0x0012,0x82,0x01},/*" 事件结束后B相正向有功电能"*/
    {0x0022,0x82,0x01},/*" 事件结束后B相反向有功电能"*/
    {0x0032,0x82,0x01},/*" 事件结束后B相组合无功1电能"*/
    {0x0042,0x82,0x01},/*" 事件结束后B相组合无功2电能"*/
    {0x0013,0x82,0x01},/*" 事件结束后C相正向有功电能"*/
    {0x0023,0x82,0x01},/*" 事件结束后C相反向有功电能"*/
    {0x0033,0x82,0x01},/*" 事件结束后C相组合无功1电能"*/
    {0x0043,0x82,0x01},/*" 事件结束后C相组合无功2电能"*/
};

/* 301F 跳闸事件记录默认关联OAD存储内容 */
/* 3020 合闸事件记录默认关联OAD存储内容*/
const ST_OAD DefRelayOnOffRoadInfo[]=
{
    {0x0010,0x22,0x01},   /*00102201--开表盖前正向有功总电能*/
    {0x0020,0x22,0x01},   /* 00202201--开表盖前反向有功总电能*/
    {0x0050,0x22,0x01},/*" 电表清零前第一象限无功总电能"*/
    {0x0060,0x22,0x01},/*" 电表清零前第二象限无功总电能"*/
    {0x0070,0x22,0x01},/*" 电表清零前第三象限无功总电能"*/
    {0x0080,0x22,0x01},/*" 电表清零前第四象限无功总电能"*/
};


/*" 301E 302D 电流不平衡 "*/
const ST_OAD DefCurrImBalanceRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
    {0x0030,0x22,0x01},/*" 事件发生时刻组合无功1总电能"*/
    {0x0040,0x22,0x01},/*" 事件发生时刻组合无功2总电能"*/
    {0x0011,0x22,0x01},/*" 事件发生时刻A相正向有功电能"*/
    {0x0021,0x22,0x01},/*" 事件发生时刻A相反向有功电能"*/
    {0x0031,0x22,0x01},/*" 事件发生时刻A相组合无功1电能"*/
    {0x0041,0x22,0x01},/*" 事件发生时刻A相组合无功2电能"*/
    {0x0012,0x22,0x01},/*" 事件发生时刻B相正向有功电能"*/
    {0x0022,0x22,0x01},/*" 事件发生时刻B相反向有功电能"*/
    {0x0032,0x22,0x01},/*" 事件发生时刻B相组合无功1电能"*/
    {0x0042,0x22,0x01},/*" 事件发生时刻B相组合无功2电能"*/
    {0x0013,0x22,0x01},/*" 事件发生时刻C相正向有功电能"*/
    {0x0023,0x22,0x01},/*" 事件发生时刻C相反向有功电能"*/
    {0x0033,0x22,0x01},/*" 事件发生时刻C相组合无功1电能"*/
    {0x0043,0x22,0x01},/*" 事件发生时刻C相组合无功2电能"*/

    {0x2027,0x62,0x00},/*" 事件发生期间电压不平衡率"*/

    {0x0010,0x82,0x01},/*" 事件结束后正向有功总电能"*/
    {0x0020,0x82,0x01},/*" 事件结束后反向有功总电能"*/
    {0x0030,0x82,0x01},/*" 事件结束后组合无功1总电能"*/
    {0x0040,0x82,0x01},/*" 事件结束后组合无功2总电能"*/
    {0x0011,0x82,0x01},/*" 事件结束后A相正向有功电能"*/
    {0x0021,0x82,0x01},/*" 事件结束后A相反向有功电能"*/
    {0x0031,0x82,0x01},/*" 事件结束后A相组合无功1电能"*/
    {0x0041,0x82,0x01},/*" 事件结束后A相组合无功2电能"*/
    {0x0012,0x82,0x01},/*" 事件结束后B相正向有功电能"*/
    {0x0022,0x82,0x01},/*" 事件结束后B相反向有功电能"*/
    {0x0032,0x82,0x01},/*" 事件结束后B相组合无功1电能"*/
    {0x0042,0x82,0x01},/*" 事件结束后B相组合无功2电能"*/
    {0x0013,0x82,0x01},/*" 事件结束后C相正向有功电能"*/
    {0x0023,0x82,0x01},/*" 事件结束后C相反向有功电能"*/
    {0x0033,0x82,0x01},/*" 事件结束后C相组合无功1电能"*/
    {0x0043,0x82,0x01},/*" 事件结束后C相组合无功2电能"*/
};


/*" 3021无关联对象"*/


/*有功组合方式特征字默认关联OAD存储内容3022*/
const ST_OAD DefPrgAemodeRoadInfo[] =
{
    {0x4112,0x22,0x00},     /*41122200--编程前有功组合方式特征字*/
};

/*无功组合方式编程事件默认关联OAD存储内容3023*/
const ST_OAD DefProgRaEModeRoadInfo[] =
{
    {0x4113,0x22,0x00},     /*41122200--编程前有功组合方式特征字*/
    {0x4114,0x22,0x00},
};


/*费率参数编程事件编程事件记录3024*/
/*默认关联OAD存储内容*/
const ST_OAD DefPrgTariffRoadInfo[] =
{
    {0x4018,0x22,0x00},/*40182200--编程前当前套费率电价*/
    {0x4019,0x22,0x00},/*40192200--编程前备用套费率电价*/
};


/*阶梯表编程事件编程事件记录3025*/
/*默认关联OAD存储内容*/
const ST_OAD DefPrgStepRoadInfo[] =
{
    {0x401A,0x22,0x00},/*401A2200--编程前当前套阶梯电价*/
    {0x401B,0x22,0x00},/*401B2200--编程前备用套阶梯电价*/
};
/***************************************************************************************/

/***************************************************************************************/
/*秘钥更新事件编程事件记录3026*/
/*默认关联OAD存储内容
F1002400--更新前的密钥状态字
*/
const ST_OAD DefKeyUpdateRoadInfo[] =
{
    {0xF100,0x24,0x00}, /*F1002400--更新前的密钥状态字*/
};

/***************************************************************************************/
/***************************************************************************************/
/*购电事件编程事件记录3028*/
/*默认关联OAD存储内容*/
const ST_OAD DefBuyEngyRoadInfo[] =
{
    {0x202C,0x22,0x01}, /*202C2201--购电前剩余金额*/
    {0x202E,0x22,0x00}, /*202E2200--购电前累计购电金额*/
    {0x202D,0x22,0x00}, /*202D2200--购电前透支金额*/
        
    {0x202C,0x82,0x02}, /*202C8202--购电后购电次数*/
    {0x202C,0x82,0x01},/*202C8201--购电后剩余金额*/
    {0x202E,0x82,0x00},/*202E8200--购电后累计购电金额*/
};



/* 3029 无关联对象*/

/* 302A 恒定磁场干扰*/
const ST_OAD DefConstantMagnetRoadInfo[]=
{
    {0x0010,0x22,0x01},   /*00102201--开表盖前正向有功总电能*/
    {0x0020,0x22,0x01},   /* 00202201--开表盖前反向有功总电能*/

    {0x0010,0x82,0x01},   /* 00108201--开表盖后正向有功总电能*/  
    {0x0020,0x82,0x01}    /*00208201--开表盖后反向有功总电能*/
};


/***************************************************************************************/

/***************************************************************************************/
/*负荷开关误动作事件记录302B*/
/*默认关联OAD存储内容*/
const ST_OAD DefLoadSwErrRoadInfo[] =
{
    {0xF205,0x22,0x01},/*F2050201--负荷开关误动作时状态*/
    {0x0010,0x22,0x01},/*00102201--负荷开关误动作前正向有功总电能*/
    {0x0020,0x22,0x01},/*00202201--负荷开关误动作前反向有功总电能*/
    {0x0010,0x82,0x01},/*00108201--负荷开关误动作后正向有功总电能*/
    {0x0020,0x82,0x01},/*00208201--负荷开关误动作后反向有功总电能*/
};

/***************************************************************************************/
/***************************************************************************************/
/*电源异常事件记录302C*/
/*默认关联OAD存储内容*/
const ST_OAD DefPwrErrRoadInfo[] =
{
    {0x0010,0x22,0x01},/*00102201--电源异常事件前正向有功总电能、*/
    {0x0020,0x22,0x01},/*00202201--电源异常事件前反向有功总电能*/
};

/***************************************************************************************/

/*" 电量异常 "*/
const ST_OAD DefEngErrRoadInfo[] =
{
    {0x0010, 0x02, 0x00},
    {0x0020, 0x02, 0x00},

    {0x0050, 0x02, 0x00},
    {0x0060, 0x02, 0x00},
    {0x0070, 0x02, 0x00},
    {0x0080, 0x02, 0x00},
};

/*" 5000  瞬时冻结"*/
const ST_ROAD_FRZ_INFO DefMMFrzRoadInfo[] =
{
    {0x00, {0x0010, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0020, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0030, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0040, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0050, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0060, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0070, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x0080, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x1010, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x1020, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x2004, 0x02, 0x00}, NUM_MMT_FRZ},
    {0x00, {0x2005, 0x02, 0x00}, NUM_MMT_FRZ},
};

/*分钟冻结5002 */
const ST_ROAD_FRZ_INFO DefMinFrzRoadInfo[] = 
{
    {15,{0x0010,0x02,0x01},NUM_MIN_FRZ,},/*"正向有功电量总"*/
    {15,{0x0020,0x02,0x01},NUM_MIN_FRZ},/*"反向有功电量总"*/
    {15,{0x0050,0x02,0x01},NUM_MIN_FRZ},
    {15,{0x0060,0x02,0x01},NUM_MIN_FRZ},
    {15,{0x0070,0x02,0x01},NUM_MIN_FRZ},
    {15,{0x0080,0x02,0x01},NUM_MIN_FRZ},
        
    {15,{0x2017,0x02,0x00},NUM_MIN_FRZ},//20170200--有功需量
    {15,{0x2018,0x02,0x00},NUM_MIN_FRZ},//20180200--无功需量
        
    {15,{0x2000,0x02,0x00},NUM_MIN_FRZ},/*"电压"*/
    {15,{0x2001,0x02,0x00},NUM_MIN_FRZ},/*"电流"*/
    {15,{0x2004,0x02,0x00},NUM_MIN_FRZ},/*"有功功率"*/
    {15,{0x200A,0x02,0x00},NUM_MIN_FRZ},/*"功率因数"*/

};


/*小时冻结 5003*/
const ST_ROAD_FRZ_INFO DefHourFrzRoadInfo[] = 
{
    {1,{0x0010,0x02,0x01},NUM_HOUR_FRZ},//00100200--正向有功总电能
    {1,{0x0020,0x02,0x01},NUM_HOUR_FRZ},//00200200--反向有功总电能
};

/*日冻结 5004*/
const ST_ROAD_FRZ_INFO DefDayFrzRoadInfo[] = 
{
    {1, {0x0010, 0x02, 0x00},NUM_DAY_FRZ},//00100200--正向有功电能
    {1, {0x0020, 0x02, 0x00},NUM_DAY_FRZ},//00200200--反向有功电能
    {1, {0x0030, 0x02, 0x00},NUM_DAY_FRZ},//00300200--组合无功1电能
    {1, {0x0040, 0x02, 0x00}, NUM_DAY_FRZ},//00400200--组合无功2电能
    {1, {0x0050, 0x02, 0x00}, NUM_DAY_FRZ},//00500200--第一象限无功电能
    {1, {0x0060, 0x02, 0x00}, NUM_DAY_FRZ},//00500200--第二象限无功电能
    {1, {0x0070, 0x02, 0x00}, NUM_DAY_FRZ},//00500200--第三象限无功电能
    {1, {0x0080, 0x02, 0x00}, NUM_DAY_FRZ},//00500200--第四象限无功电能
    {1, {0x1010, 0x02, 0x00}, NUM_DAY_FRZ},//10100200--正向有功最大需量及发生时间
    {1, {0x1020, 0x02, 0x00}, NUM_DAY_FRZ},//10200200--反向有功最大需量及发生时间
    {1, {0x2004, 0x02, 0x00}, NUM_DAY_FRZ},//20040200--有功功率
    {1, {0x2005, 0x02, 0x00}, NUM_DAY_FRZ},//20040200--无功功率
    //{1, {0x202C, 0x02, 0x00}, NUM_DAY_FRZ},//202C0200--剩余金额、购电次数
    //{1, {0x202D, 0x02, 0x00}, NUM_DAY_FRZ},//202D0200--透支金额
    {1, {0x2131, 0x02, 0x01}, NUM_DAY_FRZ},
    {1, {0x2132, 0x02, 0x01}, NUM_DAY_FRZ},
    {1, {0x2133, 0x02, 0x01}, NUM_DAY_FRZ},
};

/*结算日冻结 5005*/
const ST_ROAD_FRZ_INFO DefBillFrzRoadInfo[] = 
{
    {1, {0x0000, 0x02, 0x00}, NUM_ENGY_BILL},//00000200--组合有功电能
    {1, {0x0010, 0x02, 0x00}, NUM_ENGY_BILL},//00100200--正向有功电能
    {1, {0x0020, 0x02, 0x00}, NUM_ENGY_BILL},//00200200--反向有功电能
    {1, {0x0030, 0x02, 0x00}, NUM_ENGY_BILL},//00300200--组合无功1电能
    {1, {0x0040, 0x02, 0x00}, NUM_ENGY_BILL},//00400200--组合无功2电能
    {1, {0x0050, 0x02, 0x00}, NUM_ENGY_BILL},//00500200--第一象限无功电能
    {1, {0x0060, 0x02, 0x00}, NUM_ENGY_BILL},//00500200--第二象限无功电能
    {1, {0x0070, 0x02, 0x00}, NUM_ENGY_BILL},//00500200--第三象限无功电能
    {1, {0x0080, 0x02, 0x00}, NUM_ENGY_BILL},//00500200--第四象限无功电能
    {1, {0x1010, 0x02, 0x00}, NUM_ENGY_BILL},//10100200--正向有功最大需量及发生时间
    {1, {0x1020, 0x02, 0x00}, NUM_ENGY_BILL},//10200200--反向有功最大需量及发生时间
    {1, {0x2031, 0x02, 0x00}, NUM_ENGY_BILL},//20310200--月度用电量 

};

/*月冻结 5006*/
const ST_ROAD_FRZ_INFO DefMonFrzRoadInfo[] = 
{
    {1, {0x0000, 0x02, 0x00}, NUM_MON_FRZ},//00000200--组合有功电能
    
    {1, {0x0010, 0x02, 0x00}, NUM_MON_FRZ},//00100200--总正向有功电能
    {1, {0x0011, 0x02, 0x00}, NUM_MON_FRZ},//00100200--A相正向有功电能
    {1, {0x0012, 0x02, 0x00}, NUM_MON_FRZ},//00100200--B相正向有功电能
    {1, {0x0013, 0x02, 0x00}, NUM_MON_FRZ},//00100200--C相正向有功电能
    
    {1, {0x0020, 0x02, 0x00}, NUM_MON_FRZ},//00200200--反向有功电能
    {1, {0x0021, 0x02, 0x00}, NUM_MON_FRZ},//00200200--反向有功电能
    {1, {0x0022, 0x02, 0x00}, NUM_MON_FRZ},//00200200--反向有功电能
    {1, {0x0023, 0x02, 0x00}, NUM_MON_FRZ},//00200200--反向有功电能
    
    {1, {0x0030, 0x02, 0x00}, NUM_MON_FRZ},//00300200--组合无功1电能
    {1, {0x0040, 0x02, 0x00}, NUM_MON_FRZ},//00400200--组合无功2电能
    
    {1, {0x0050, 0x02, 0x00}, NUM_MON_FRZ},//00500200--第一象限无功电能
    //{1, {0x0051, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第一象限无功电能
    //{1, {0x0052, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第一象限无功电能
    //{1, {0x0053, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第一象限无功电能
            
    {1, {0x0060, 0x02, 0x00}, NUM_MON_FRZ},//00500200--第二象限无功电能
    //{1, {0x0061, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第二象限无功电能
    //{1, {0x0062, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第二象限无功电能
    //{1, {0x0063, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第二象限无功电能
    
    {1, {0x0070, 0x02, 0x00}, NUM_MON_FRZ},//00500200--第三象限无功电能
    //{1, {0x0071, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第三象限无功电能
    //{1, {0x0072, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第三象限无功电能
    //{1, {0x0073, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第三象限无功电能
    
    {1, {0x0080, 0x02, 0x00}, NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x0081, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x0082, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x0083, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能

    //{1, {0x0090, 0x02, 0x00}, _NUM_MON_FRZ},//00900200--正向视在电能
    //{1, {0x0091, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x0092, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x0093, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能

    //{1, {0x00A0, 0x02, 0x00}, _NUM_MON_FRZ},//00A00200--反向视在电能
    //{1, {0x00A1, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x00A2, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    //{1, {0x00A3, 0x02, 0x00}, _NUM_MON_FRZ},//00500200--第四象限无功电能
    
    
    
    {1, {0x1010, 0x02, 0x00}, NUM_MON_FRZ},//10100200--正向有功最大需量及发生时间
    {1, {0x1020, 0x02, 0x00}, NUM_MON_FRZ},//10200200--反向有功最大需量及发生时间

    {1, {0x2131, 0x02,0x02}, NUM_MON_FRZ},//21310202--A相电压合格率
    {1, {0x2132, 0x02,0x02}, NUM_MON_FRZ},//21320202--B相电压合格率
    {1, {0x2133, 0x02,0x02}, NUM_MON_FRZ},//21330202--C相电压合格率
};

/*阶梯结算冻结 5011*/
const ST_ROAD_FRZ_INFO DefStepBillFrzRoadInfo[] = 
{
    {1, {0x2032,0x02,0x00} ,NUM_YEAR_BILL},//20320200--阶梯结算用电量   

};

/*时区表切换冻结 5008*/

/*" 5000  瞬时冻结"*/
const ST_ROAD_FRZ_INFO defSwitchFrzRoadInfo[] =
{
    {0x01, {0x0010, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0020, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0030, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0040, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0050, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0060, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0070, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x0080, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x1010, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x1020, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x2004, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
    {0x01, {0x2005, 0x02, 0x00}, NUM_YSLOT_SW_FRZ},
};

#else

/*"3013 电表清零"*/
const ST_OAD Def_ClearMeterRoad[] =
{
    {0x0010,0x22,0x01},/*" 事件发生时刻正向有功总电能"*/
    {0x0020,0x22,0x01},/*" 事件发生时刻反向有功总电能"*/
};


/*过流事件*/
/*过流事件记录 3005*/
const ST_OAD DefOverCurRoadInfo[]=
{
    {0x0010,0x22,0x01}, /*00102201--事件发生时刻正向有功总电能*/
    {0x0020,0x22,0x01}, /*00202201--事件发生时刻反向有功总电能*/   
    {0x2000,0x22,0x00}, /*20002200--事件发生时刻电压*/   
    {0x2001,0x22,0x00}, /*20012200--事件发生时刻电流*/   
    {0x2004,0x22,0x00}, /*20042200--事件发生时刻有功功率*/   
    {0x200A,0x22,0x00}, /*200A2200--事件发生时刻功率因数*/   
    {0x0010,0x82,0x01}, /*00108201--事件结束后正向有功总电能*/   
    {0x0020,0x82,0x01}, /*00208201--事件结束后反向有功总电能*/   

};

/*校时事件记录默认关联OAD存储内容 3016*/
const ST_OAD DefJustTimeRoadInfo[]=
{
    {0x4000,0x22,0x00},/*40002200--校时前时间*/
    {0x4000,0x82,0x00},/*40008200--校时后时间*/
};


/*时区编程事件记录默认关联OAD存储内容3018*/
const ST_OAD DefPrgYslotRoadInfo[]=
{
    #if 0
    {0x4014,0x22,0x00},/*40142200--时区编程前当前套时区表*/
    #endif
    {0x4015,0x22,0x00},/*40152200--时区编程前备用套时区表*/
};


/*周休日编程事件记录默认关联OAD存储内容 3019
40132200--编程前周休日釆用的日时段表号
*/
const ST_OAD DefPrgWeekRoadInfo[]=
{
    {0x4013,0x22,0x00},/*40132200--编程前周休日釆用的日时段表号*/
};

/*结算日编程事件记录默认关联OAD存储内容 301A
*/
const ST_OAD DefPrgBillRoadInfo[]=
{
    {0x4116,0x22,0x00},/*41162200--编程前结算日*/

};

/*开盖事件事件记录默认关联OAD存储内容 301B*/

/*"前面两个OAD"*/
/*跳闸事件记录默认关联OAD存储内容 301F*//*合闸事件记录默认关联OAD存储内容3020*/
/*电源异常事件记录302C*/
/*电能表清零事件默认关联OAD存储内容 3013*/

const ST_OAD DefOpenCvrRoadInfo[]=
{
    {0x0010,0x22,0x01},   /*00102201--开表盖前正向有功总电能*/
    {0x0020,0x22,0x01},   /* 00202201--开表盖前反向有功总电能*/
    {0x0010,0x82,0x01},   /* 00108201--开表盖后正向有功总电能*/  
    {0x0020,0x82,0x01}    /*00208201--开表盖后反向有功总电能*/
};

/*电源异常事件记录302C*/
/*默认关联OAD存储内容*/
const ST_OAD DefPwrErrRoadInfo[] =
{
    {0x0010,0x22,0x01},/*00102201--电源异常事件前正向有功总电能、*/
    {0x0020,0x22,0x01},/*00202201--电源异常事件前反向有功总电能*/
};

/*有功组合方式特征字默认关联OAD存储内容3022*/
const ST_OAD DefPrgAemodeRoadInfo[] =
{
    {0x4112,0x22,0x00},     /*41122200--编程前有功组合方式特征字*/
};

/* 301F 跳闸事件记录默认关联OAD存储内容 */
/* 3020 合闸事件记录默认关联OAD存储内容*/
const ST_OAD DefRelayOnOffRoadInfo[]=
{
    {0x0010,0x22,0x01},   /*00102201--开表盖前正向有功总电能*/
    {0x0020,0x22,0x01},   /* 00202201--开表盖前反向有功总电能*/
};


/*费率参数编程事件编程事件记录3024*/
/*默认关联OAD存储内容*/
const ST_OAD DefPrgTariffRoadInfo[] =
{
    {0x4018,0x22,0x00},/*40182200--编程前当前套费率电价*/
    {0x4019,0x22,0x00},/*40192200--编程前备用套费率电价*/
};


/*阶梯表编程事件编程事件记录3025*/
/*默认关联OAD存储内容*/
const ST_OAD DefPrgStepRoadInfo[] =
{
    {0x401A,0x22,0x00},/*401A2200--编程前当前套阶梯电价*/
    {0x401B,0x22,0x00},/*401B2200--编程前备用套阶梯电价*/
};
/***************************************************************************************/

/***************************************************************************************/
/*秘钥更新事件编程事件记录3026*/
/*默认关联OAD存储内容
F1002400--更新前的密钥状态字
*/
const ST_OAD DefKeyUpdateRoadInfo[] =
{
    {0xF100,0x24,0x00}, /*F1002400--更新前的密钥状态字*/
};

/***************************************************************************************/
/***************************************************************************************/
/*购电事件编程事件记录3028*/
/*默认关联OAD存储内容*/
const ST_OAD DefBuyEngyRoadInfo[] =
{
    {0x202C,0x22,0x01}, /*202C2201--购电前剩余金额*/
    {0x202E,0x22,0x00}, /*202E2200--购电前累计购电金额*/
    {0x202D,0x22,0x00}, /*202D2200--购电前透支金额*/
        
    {0x202C,0x82,0x02}, /*202C8202--购电后购电次数*/
    {0x202C,0x82,0x01},/*202C8201--购电后剩余金额*/
    {0x202E,0x82,0x00},/*202E8200--购电后累计购电金额*/
};

/***************************************************************************************/

/***************************************************************************************/
/*负荷开关误动作事件记录302B*/
/*默认关联OAD存储内容*/
const ST_OAD DefLoadSwErrRoadInfo[] =
{
    {0xF205,0x22,0x01},/*F2052201--负荷开关误动作时状态*/
    {0x0010,0x22,0x01},/*00102201--负荷开关误动作前正向有功总电能*/
    {0x0020,0x22,0x01},/*00202201--负荷开关误动作前反向有功总电能*/
    {0x0010,0x82,0x01},/*00108201--负荷开关误动作后正向有功总电能*/
    {0x0020,0x82,0x01},/*00208201--负荷开关误动作后反向有功总电能*/
};

/***************************************************************************************/

/*冻结关联默认关联关联OAD初始化*/

/*瞬时冻结 5000*/
const ST_ROAD_FRZ_INFO DefMMFrzRoadInfo[] =
{
    {0,0x0010,0x02,0x00,NUM_MMT_FRZ},//00100200--正向有功电能
    {0,0x0020,0x02,0x00,NUM_MMT_FRZ},//00200200--反向有功电能
    {0,0x2004,0x02,0x00,NUM_MMT_FRZ},//20040200--有功功率
};

/*分钟冻结5002 */
const ST_ROAD_FRZ_INFO DefMinFrzRoadInfo[] = 
{
    {15,0x0010,0x02,0x01,288},/*"正向有功电量总"*/
    {15,0x0020,0x02,0x01,288},/*"反向有功电量总"*/
    {15,0x2000,0x02,0x00,288},/*"电压"*/
    {15,0x2001,0x02,0x00,288},/*"电流"*/
    {15,0x2004,0x02,0x00,288},/*"有功功率"*/
    {15,0x200A,0x02,0x00,288},/*"功率因数"*/

};



/*小时冻结 5003*/
const ST_ROAD_FRZ_INFO DefHourFrzRoadInfo[] = 
{
    {1,0x0010,0x02,0x01,NUM_HOUR_FRZ},//00100200--正向有功总电能
    {1,0x0020,0x02,0x01,NUM_HOUR_FRZ},//00200200--反向有功总电能
};

/*日冻结 5004*/
const ST_ROAD_FRZ_INFO DefDayFrzRoadInfo[] = 
{
    {1,0x0010,0x02,0x00,NUM_DAY_FRZ},//00100200--正向有功电能
    {1,0x0020,0x02,0x00,NUM_DAY_FRZ},//00200200--反向有功电能
    {1,0x2004,0x02,0x00,NUM_DAY_FRZ},//20040200--有功功率
    //{1,0x202C,0x02,0x00,NUM_DAY_FRZ},//202C0200--剩余金额、购电次数
    //{1,0x202D,0x02,0x00,NUM_DAY_FRZ},//202D0200--透支金额
};

/*结算日冻结 5005*/
const ST_ROAD_FRZ_INFO DefBillFrzRoadInfo[] = 
{
    {0,0x0000,0x02,0x00,NUM_ENGY_BILL},//00000200--组合有功电能
    {0,0x0010,0x02,0x00,NUM_ENGY_BILL},//00100200--正向有功电能
    {0,0x0020,0x02,0x00,NUM_ENGY_BILL},//00200200--反向有功电能
    {0,0x2031,0x02,0x00,NUM_ENGY_BILL},//20310200--月度用电量
};

/*月冻结 5006*/
const ST_ROAD_FRZ_INFO DefMonFrzRoadInfo[] = 
{
    {1, 0x0000,0x02,0x00,NUM_MON_FRZ},//00000200--组合有功电能    
    {1, 0x0010,0x02,0x00,NUM_MON_FRZ},//00100200--正向有功电能
    {1, 0x0020,0x02,0x00,NUM_MON_FRZ},//00200200--反向有功电能
};

/*阶梯结算冻结 5011*/
const ST_ROAD_FRZ_INFO DefStepBillFrzRoadInfo[] = 
{
    {1, 0x2032,0x02,0x00,NUM_YEAR_BILL},//20320200--阶梯结算用电量   

};

/*时区表切换冻结 5008*//*时段表切换冻结 5009*//*费率电价切换冻结 500A*//*阶梯电价切换冻结 500B*/
const ST_ROAD_FRZ_INFO defSwitchFrzRoadInfo[] = 
{
    {1,0x0010,0x02,0x00,NUM_YSLOT_SW_FRZ},//00100200--正向有功电能
    {1,0x0020,0x02,0x00,NUM_YSLOT_SW_FRZ},//00200200--反向有功电能
    {1,0x2004,0x02,0x00,NUM_YSLOT_SW_FRZ},//20040200--有功功率
};

#endif

/*MCU RST EVT */
const ST_OAD DefMcuRstRoadInfo[] = 
{
    {0xff50,0x02,0x00},//复位原因
    {0xff51,0x02,0x00},//复位地址
};

/*冻结关联默认关联关联OAD初始化*/
typedef struct
{
    const INT8U* SrcAddr;
    INT16U RoadObis;
    INT16U Len;
}ST_EVT_ROAD_TAB;

#if 0
const ST_EVT_ROAD_TAB EvtRoadDefTab[] =
{
#if defined (THREE_PHASE)
    {(INT8U*)&DefLostVoltRoad, LOST_VOLT_OBJ_LIST, sizeof(DefLostVoltRoad)},/* 00 */
    {(INT8U*)&DefLostVoltRoad, LACK_VOLT_OBJ_LIST, sizeof(DefLostVoltRoad)},// 01 
    {(INT8U*)&DefLostVoltRoad, OVER_VOLT_OBJ_LIST, sizeof(DefLostVoltRoad)},// 02
    {(INT8U*)&DefLostVoltRoad, BRK_PHASE_OBJ_LIST, sizeof(DefLostVoltRoad)},// 03
    {(INT8U*)&DefLostCurrtRoad, LOST_CURR_OBJ_LIST, sizeof(DefLostCurrtRoad)},/* 04 */
    {(INT8U*)&DefLostCurrtRoad, OVERCURR_OBJ_LIST, sizeof(DefLostCurrtRoad)},
    {(INT8U*)&DefLostCurrtRoad, BRK_CURR_OBJ_LIST, sizeof(DefLostCurrtRoad)},
    {(INT8U*)&DefUpLoadRoad, PWR_REVERSE_OBJ_LIST, sizeof(DefUpLoadRoad)},/* 07 */
    {(INT8U*)&DefUpLoadRoad, OVER_LOAD_OBJ_LIST, sizeof(DefUpLoadRoad)},// 08 
    {NULL, OVER_PA_DEMAND_OBJ_LIST, MAX_OAD_OVER_PA_DEMAND*4},/* 09 */
    {NULL, OVER_NA_DEMAND_OBJ_LIST, MAX_OAD_OVER_NA_DEMAND*4},/* 0A */
    {NULL, OVER_R_DEMAND_OBJ_LIST, MAX_OAD_OVER_R_DEMAND*4},/* 0B */
    {(INT8U*)&DefOverFactorRoad, FACTOR_LOW_OBJ_LIST, sizeof(DefOverFactorRoad)},/* 0C */
    {(INT8U*)&DefPhase3LostVoltRoad, ALL_LOST_VOLT_OBJ_LIST, sizeof(DefPhase3LostVoltRoad)},/* 0D */
    //{NULL, , 0},/* 0e */
    {(INT8U*)&DefUpLoadRoad, VOLT_INVERSE_OBJ_LIST, sizeof(DefUpLoadRoad)},/* 0F */
    {(INT8U*)&DefUpLoadRoad, CURR_INVERSE_OBJ_LIST, sizeof(DefUpLoadRoad)},/* 10 */
    {NULL, PWR_DOWN_OBJ_LIST, MAX_OAD_PWR_DOWN*4},/* 11 */
    {NULL, PRG_EVT_OBJ_LIST, MAX_OAD_PRG_EVNT*4},/* 12 */
    {(INT8U*)&Def_ClearMeterRoad, CLRMTR_OBJ_LIST, sizeof(Def_ClearMeterRoad)},/* 13 */
    {(INT8U*)&Def_ClearDemadnRoad, DEMAND_CLR_OBJ_LIST, sizeof(Def_ClearDemadnRoad)},/* 14 */
    {NULL, EVNT_CLR_OBJ_LIST, MAX_OAD_EVNT_CLR*4},/* 15 */
    {(INT8U*)&DefJustTimeRoadInfo, JUSTTIME_EVT_OBJ_LIST, sizeof(DefJustTimeRoadInfo)},/* 16 */
    {NULL, DAY_TBL_PRG_OBJ_LIST, MAX_OAD_DAY_TBL_PRG*4},/* 17 */
    {(INT8U*)&DefPrgYslotRoadInfo, SEASON_TBL_PRG_OBJ_LIST, sizeof(DefPrgYslotRoadInfo)},/* 18 */
    {(INT8U*)&DefPrgWeekRoadInfo, WEEKEND_PRG_OBJ_LIST, sizeof(DefPrgWeekRoadInfo)},/* 19 */
    {(INT8U*)&DefPrgBillRoadInfo, BILL_DAY_PRG_OBJ_LIST, sizeof(DefPrgBillRoadInfo)},/* 1A */
    {(INT8U*)&DefOpenCvrRoadInfo, MTR_COVER_OPEN_OBJ_LIST, sizeof(DefOpenCvrRoadInfo)},/* 1B */
    {(INT8U*)&DefOpenCvrRoadInfo, TAIL_COVER_OPEN_OBJ_LIST, sizeof(DefOpenCvrRoadInfo)},/* 1C */
    {(INT8U*)&DefVoltImBalanceRoad, VOLT_IMBALANCE_OBJ_LIST, sizeof(DefVoltImBalanceRoad)},/* 1D */
    {(INT8U*)&DefCurrImBalanceRoad, CURR_IMBALANCE_OBJ_LIST, sizeof(DefCurrImBalanceRoad)},/* 1e */
    {(INT8U*)&DefRelayOnOffRoadInfo, RELAY_TRIP_OBJ_LIST, sizeof(DefRelayOnOffRoadInfo)},/* 1F */
    {(INT8U*)&DefRelayOnOffRoadInfo, RELAY_CLOSE_OBJ_LIST, sizeof(DefRelayOnOffRoadInfo)},/* 20 */
    {NULL, HOLIDAY_PRG_OBJ_LIST, MAX_OAD_HOLIDAY_PRG*4},/* 21 */
    {(INT8U*)&DefPrgAemodeRoadInfo, A_ENGY_COM_OBJ_LIST, sizeof(DefPrgAemodeRoadInfo)},/* 22 */
    {(INT8U*)&DefProgRaEModeRoadInfo, R_ENGY_COM_OBJ_LIST, sizeof(DefProgRaEModeRoadInfo)},/* 23 */
    {(INT8U*)&DefPrgTariffRoadInfo, TRIFF_PRG_OBJ_LIST, sizeof(DefPrgTariffRoadInfo)},/* 24 */
    {(INT8U*)&DefPrgStepRoadInfo, STEP_PRG_OBJ_LIST, sizeof(DefPrgStepRoadInfo)},/* 25 */
    {(INT8U*)&DefKeyUpdateRoadInfo, KEY_UPDATE_OBJ_LIST, sizeof(DefKeyUpdateRoadInfo)},/* 26 */
    {NULL, CARD_IN_ERR_OBJ_LIST, MAX_OAD_CARD_IN_ERR*4},/* 27 */
    {(INT8U*)&DefBuyEngyRoadInfo, BUY_ENGY_OBJ_LIST, sizeof(DefBuyEngyRoadInfo)},/* 28 */  
    {NULL, REFUND_EVNT_OBJ_LIST, MAX_OAD_REFUND_EVNT*4},/* 29 */
    {(INT8U*)&DefConstantMagnetRoadInfo, CONSTANT_MGNT_DISTURB_OBJ_LIST, sizeof(DefConstantMagnetRoadInfo)},/* 2a */ 
    {(INT8U*)&DefLoadSwErrRoadInfo, RELAY_ERR_OBJ_LIST, sizeof(DefLoadSwErrRoadInfo)},/* 2B */ 
    {(INT8U*)&DefPwrErrRoadInfo, PWR_FAILURE_OBJ_LIST, sizeof(DefPwrErrRoadInfo)},/* 2C*/ 
    {(INT8U*)&DefCurrImBalanceRoad, CURR_IMBALANCE_BADLY_OBJ_LIST, sizeof(DefCurrImBalanceRoad)},/* 2d */
    {(INT8U*)&DefEngErrRoadInfo, ENG_ERR_OBJ_LIST, sizeof(DefEngErrRoadInfo)},/* ff7a */
    {NULL, RTC_ERR_OBJ_LIST, MAX_OAD_RTC_ERR*4},/* 2e */
    {NULL, MS_ERR_OBJ_LIST, MAX_OAD_MS_ERR*4},/* 2f */
#else
    {NULL, PWR_DOWN_OBJ_LIST, MAX_OAD_PWR_DOWN*4},/* 11 */
    {NULL, PRG_EVT_OBJ_LIST, MAX_OAD_PRG_EVNT*4},/* 12 */
    {(INT8U*)&Def_ClearMeterRoad, CLRMTR_OBJ_LIST, sizeof(Def_ClearMeterRoad)},/* 13 */
    {NULL, EVNT_CLR_OBJ_LIST, MAX_OAD_EVNT_CLR*4},/* 15 */
    {(INT8U*)&DefJustTimeRoadInfo, JUSTTIME_EVT_OBJ_LIST, sizeof(DefJustTimeRoadInfo)},/* 16 */
    {NULL, DAY_TBL_PRG_OBJ_LIST, MAX_OAD_DAY_TBL_PRG*4},/* 17 */
    {(INT8U*)&DefPrgYslotRoadInfo, SEASON_TBL_PRG_OBJ_LIST, sizeof(DefPrgYslotRoadInfo)},/* 18 */
    {(INT8U*)&DefPrgWeekRoadInfo, WEEKEND_PRG_OBJ_LIST, sizeof(DefPrgWeekRoadInfo)},/* 19 */
    {(INT8U*)&DefPrgBillRoadInfo, BILL_DAY_PRG_OBJ_LIST, sizeof(DefPrgBillRoadInfo)},/* 1A */
    {(INT8U*)&DefOpenCvrRoadInfo, MTR_COVER_OPEN_OBJ_LIST, sizeof(DefOpenCvrRoadInfo)},/* 1B */
    {(INT8U*)&DefRelayOnOffRoadInfo, RELAY_TRIP_OBJ_LIST, sizeof(DefRelayOnOffRoadInfo)},/* 1F */
    {(INT8U*)&DefRelayOnOffRoadInfo, RELAY_CLOSE_OBJ_LIST, sizeof(DefRelayOnOffRoadInfo)},/* 20 */
    {NULL, HOLIDAY_PRG_OBJ_LIST, MAX_OAD_HOLIDAY_PRG*4},/* 21 */
    {(INT8U*)&DefPrgAemodeRoadInfo, A_ENGY_COM_OBJ_LIST, sizeof(DefPrgAemodeRoadInfo)},/* 22 */
    {(INT8U*)&DefPrgTariffRoadInfo, TRIFF_PRG_OBJ_LIST, sizeof(DefPrgTariffRoadInfo)},/* 24 */
    {(INT8U*)&DefPrgStepRoadInfo, STEP_PRG_OBJ_LIST, sizeof(DefPrgStepRoadInfo)},/* 25 */
    {(INT8U*)&DefKeyUpdateRoadInfo, KEY_UPDATE_OBJ_LIST, sizeof(DefKeyUpdateRoadInfo)},/* 26 */
    {NULL, CARD_IN_ERR_OBJ_LIST, MAX_OAD_CARD_IN_ERR*4},/* 27 */
    {(INT8U*)&DefBuyEngyRoadInfo, BUY_ENGY_OBJ_LIST, sizeof(DefBuyEngyRoadInfo)},/* 28 */  
    {NULL, REFUND_EVNT_OBJ_LIST, MAX_OAD_REFUND_EVNT*4},/* 29 */
    {(INT8U*)&DefLoadSwErrRoadInfo, RELAY_ERR_OBJ_LIST, sizeof(DefLoadSwErrRoadInfo)},/* 2B */ 
    {(INT8U*)&DefPwrErrRoadInfo, PWR_FAILURE_OBJ_LIST, sizeof(DefPwrErrRoadInfo)},/* 2C*/ 
    {(INT8U*)&DefOverCurRoadInfo, OVERCURR_OBJ_LIST, sizeof(DefOverCurRoadInfo)},
    {NULL, RTC_ERR_OBJ_LIST, MAX_OAD_RTC_ERR*4},/* 2e */
    {NULL, MS_ERR_OBJ_LIST, MAX_OAD_MS_ERR*4},/* 2f */
#endif
    //{(INT8U*)&DefMcuRstRoadInfo, MCU_RST_OBJ_LIST, sizeof(DefMcuRstRoadInfo)},/* ff76 */
        
    {(INT8U*)&DefMMFrzRoadInfo, MMT_FRZ_OBJ_LIST, sizeof(DefMMFrzRoadInfo)},//00
    //{NULL, 0},//01
    {(INT8U*)&DefMinFrzRoadInfo, MIN_FRZ_OBJ_LIST, sizeof(DefMinFrzRoadInfo)},//02
    {(INT8U*)&DefHourFrzRoadInfo, HOUR_FRZ_OBJ_LIST, sizeof(DefHourFrzRoadInfo)},//03
    {(INT8U*)&DefDayFrzRoadInfo, DAY_FRZ_OBJ_LIST, sizeof(DefDayFrzRoadInfo)},//04
    {(INT8U*)&DefBillFrzRoadInfo, MOUTH_BILL_FRZ_OBJ_LIST, sizeof(DefBillFrzRoadInfo)},//05
    {(INT8U*)&DefMonFrzRoadInfo, MOUTH_FRZ_OBJ_LIST, sizeof(DefMonFrzRoadInfo)},//06
    //{NULL, 0},//07
    {(INT8U*)&defSwitchFrzRoadInfo, SEASON_TBL_SW_FRZ_OBJ_LIST, sizeof(defSwitchFrzRoadInfo)},//08
    {(INT8U*)&defSwitchFrzRoadInfo, DAY_TBL_SW_FRZ_OBJ_LIST, sizeof(defSwitchFrzRoadInfo)},//09
    {(INT8U*)&defSwitchFrzRoadInfo, TRIFF_TBL_SW_FRZ_OBJ_LIST, sizeof(defSwitchFrzRoadInfo)},//0a
    {(INT8U*)&defSwitchFrzRoadInfo, STEP_TBL_SW_FRZ_OBJ_LIST, sizeof(defSwitchFrzRoadInfo)},//0B
    //{NULL, 0},//0C
    //{NULL, 0},//0D
    //{NULL, 0},//0e
    //{defMMFrzRoadInfo, sizeof(defMMFrzRoadInfo)},//0F
    //{NULL, 0}, //10
    {(INT8U*)&DefStepBillFrzRoadInfo, STEP_BILL_FRZ_OBJ_LIST, sizeof(DefStepBillFrzRoadInfo)}, //11
};


/*时段表切换冻结 5009*/


/*费率电价切换冻结 500A*/


/*阶梯电价切换冻结 500B*/



void SetDefaultRoad(void)
{
    INT8U Cnt;
    INT16U obis;
    ENUM_FS_ID file;
    INT16U off;
    INT16U len;
    INT8U buf[ROADINFO_BUFF_LEN];
    
    for(Cnt=0;Cnt<ARRAY_SIZE(EvtRoadDefTab);Cnt++)
    {
        obis = EvtRoadDefTab[Cnt].RoadObis;
        get_single_obis_file_off_len(obis, &file, &off, &len);
        if((len < ROADINFO_BUFF_LEN) && (file < E_FILE_FLASH_START))
        {
            LIB_MemSet(0x00, buf, len);
            if(EvtRoadDefTab[Cnt].SrcAddr != NULL)
            {
                LIB_MemCpy((INT8U *)EvtRoadDefTab[Cnt].SrcAddr, buf, EvtRoadDefTab[Cnt].Len);
            }
            FS_WriteFile(file, off, buf, len);
        }
    }
}

INT8U GetRcrdRoad(EN_RCRD_FILE_ID rcrd_obis, INT8U *buf, INT16U *sum_len)
{
    //INT16U start_obj;
    INT16U road_obj;
    ENUM_FS_ID filename;
    INT16U off;
    INT16U len;
    INT8U res;

    //start_obj = stFileDataList[(rcrd_obis & 0x0fff)].startFelxObj;
    road_obj = PWR_DOWN_OBJ_LIST;
    res = get_single_obis_file_off_len(road_obj, &filename, &off, &len);
    if(res == TRUE)
    {
        res = FS_ReadFile(filename, off, buf, len);
        *sum_len = len;
    }
    else
    {
        *sum_len = 0;
    }
    return res;
}

INT16U GetRcrdRoadListNum(EN_RCRD_FILE_ID rcrd_obis, INT8U *buf)
{
    INT8U cnt, len;
    INT8U n;
    INT8U *pTemp;
    INT16U sum_len;
    INT8U num;


    if(rcrd_obis < E_EVT_END_RCRD)
    {
        len = sizeof(ST_OAD);
    }
    else if (rcrd_obis > E_EVT_END_RCRD && rcrd_obis <= E_STEP_BILL_FRZ_RCRD)
    {
        len = sizeof(ST_ROAD_FRZ_INFO);
    }
    else
    {
        return 0;
    }

    pTemp = buf;
    GetRcrdRoad(rcrd_obis, buf, &sum_len);
    cnt = sum_len/len;

    num = 0;
    for(n = 0; n < cnt; n++)
    {
        if(EQUAL == LIB_ChkValue(pTemp, len, 0))
        {
            break;
        }
        num++;
        pTemp += len;
    }
    return (num*len);
}

INT8U SaveRcrdRoad(EN_RCRD_FILE_ID rcrd_obis, INT8U *buf, INT16U sum_len)
{
    //INT16U start_obj;
    INT16U road_obj;
    ENUM_FS_ID filename;
    INT16U off;
    INT16U len;
    INT8U res;

    //start_obj = stFileDataList[(rcrd_obis & 0x0fff)].startFelxObj;
    road_obj = PWR_DOWN_OBJ_LIST;
    res = get_single_obis_file_off_len(road_obj, &filename, &off, &len);
    if(res == TRUE)
    {
        res = FS_WriteFile(filename, off, buf, len);
    }
    return res;
}

INT8U GetRcrdMaxRoad(INT16U rcrd_obis)
{
    //INT16U start_obj;
    INT16U road_obj;
    ENUM_FS_ID filename;
    INT16U off;
    INT16U len;
    INT8U res;
    INT8U num;

    //start_obj = stFileDataList[(rcrd_obis & 0x0fff)].startFelxObj;
    road_obj = PWR_DOWN_OBJ_LIST;
    res = get_single_obis_file_off_len(road_obj, &filename, &off, &len);
    num = 0;
    if(res == TRUE)
    {
        if(rcrd_obis < E_EVT_END_RCRD)
        {
            num = len/4;
        }
        else
        {
            num = len/8;
        }
    }
    return num;
}

static INT8U ChkEvtROadAid(INT8U *pAId)
{
    INT8U temp;

    temp = (*pAId) >> 5; /*"属性特征"*/
    if(temp <= 4)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


INT8U is_not_cfg_road(ST_OAD * oad)
{
    INT8U OIA1;
    INT8U org_aid;
    //INT8U OIA2;
    //INT8U OIB1;
    //INT8U OIB2;
    //INT8U AID;
    //INT8U IID;
    INT16U oi;

    oi = oad->Oi.asInt;
    org_aid = oad->AId & 0x1F;
    OIA1 = oad->Oi.asByte[1] >> 4;
    //OIA2 = oad->Oi.asByte[OIH] & 0xf;
    //OIB1 = oad->Oi.asByte[OIL] >> 4;
    //OIB2 = oad->Oi.asByte[OIL] & 0xf; 
    if((oi == 0x2001 && org_aid == 4)
        || (oi == 0xF205 && org_aid == 2) 
        || (oi == 0xF100 && org_aid == 4))
    {
        return FALSE;
    }

    if(OIA1 == 3 || (OIA1 >= 5) || (oi >= 0x201e && oi <= 0x2025)
        || ((OIA1 == 0) && (org_aid != 2 && org_aid != 4))
        || ((OIA1 >= 1 && OIA1 <= 4) && (org_aid != 2) )
        || ((oi == 0x4016 || oi == 0x4017) && (oad->IId == 0)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void PP_InitFrzMinPrjList(void)
{
    INT8U i, j, *pTemp;
    INT8U prjNo, prjNum, newFlg;
    INT8U OadList[ROADINFO_BUFF_LEN];
    INT16U len;

    GetRcrdRoad(E_MIN_FRZ_RCRD, OadList, &len);
    LIB_MemSet(0xFF, (INT8U *)&mstFrzPrj, FRZ_PRJ_LIST_LEN);
    prjNum = 0;
    pTemp = &OadList[0];

    for(i = 0; i < MAX_OAD_MIN_FRZ; i++)
    {
        if(EQUAL != LIB_ChkValue(pTemp, sizeof(ST_ROAD_FRZ_INFO), 0)) /*"有效ROAD"*/
        {
            newFlg = 0;
            prjNo = *(pTemp + 4) >> 5;/*"属性特征值"*/

            if(0 != prjNum)
            {
                for(j = 0; j < prjNum; j++)
                {
                    if(prjNo == mstFrzPrj[j].prjNo)
                    {
                        break;/*"相同的属性特征"*/
                    }
                }

                if(j >= prjNum)
                {
                    newFlg = 0x55;/*"新的属性特征"*/
                }
            }
            else
            {/*" 第一个方案 "*/
                newFlg = 0x55;/*"新的属性特征"*/
            }

            if(0x55 == newFlg)
            {
                mstFrzPrj[prjNum].prjNo = prjNo;
                mstFrzPrj[prjNum].index= i;						
                prjNum++;
            }
        }
        else
        {
            break;
        }

        pTemp += sizeof(ST_ROAD_FRZ_INFO);
    }
}



/*"*****************************************************************************"*/
/*"  Function:       AddROad"*/
/*"  Description:    增加一个关联对象属性"*/
/*"  Calls:          "*/
/*"  Called By:    "*/
/*"  Input:          pdata: 数据指针"*/
/*"                     index: 冻结对象ID"*/
/*"                     pList: 冻结关联属性列表"*/
/*"                     pPrjList: 冻结方案列表"*/
/*"  Output:       "*/
/*"  Return:       TRUE: 写入成功, FALSE: 写入失败"*/
/*"*****************************************************************************"*/
static INT8U AddROad(INT8U *pdata, INT16U rcrd_obis, INT8U *pList, INT8U *pPrjList, INT16U sum_len)
{
    INT8U n, off;
    INT8U *pTemp, len;
    INT8U i, prjNo, *pCmp;
    INT8U result;
    ST_FRZ_PRJ *pPrj;
    ST_OAD road;
    INT8U ROadMax;
    INT8U cur_aid;
    INT8U new_aid;
    INT8U *p_st, *p_end;

    result = FALSE;
    pTemp = pList;

    if(rcrd_obis > E_EVT_END_RCRD)
    {
        len = sizeof(ST_ROAD_FRZ_INFO);
        off = 2;
    }
    else
    {
        len = sizeof(ST_OAD);
        off = 0;
        new_aid = pdata[2];
    }
    ROadMax = sum_len/len;
    LIB_MemCpy((pdata + off), (INT8U *)&road, sizeof(ST_OAD));


    if(FALSE == is_not_cfg_road(&road))
    {
        for(n = 0; n < ROadMax; n++)
        {
            if(EQUAL == LIB_MemCmp(pTemp, pdata, len)) /*"增加的OAD已存在"*/        
            {
                break;
            }

            if(EQUAL == LIB_ChkValue(pTemp, len, 0)) /*"新增OAD"*/
            {
                if(rcrd_obis > E_EVT_END_RCRD) 
                {
                    prjNo = *(pdata + 4) >> 5;/*"属性特征值"*/                        

                    if(E_MIN_FRZ_RCRD == rcrd_obis)
                    {
                        pPrj = (ST_FRZ_PRJ *)pPrjList;

                        for(i = 0; i < MAX_FRZ_PRJ; i++)
                        {
                            if(0xFF != pPrj->prjNo)
                            {
                                if(prjNo == pPrj->prjNo)/*"已存在的冻结方案"*/
                                {
                                    pCmp = pList + pPrj->index * sizeof(ST_ROAD_FRZ_INFO);
                                    if((EQUAL == LIB_MemCmp(pdata, pCmp, 2)) &&
                                    (EQUAL == LIB_MemCmp((pdata + 6), (pCmp + 6), 2)))
                                    {/*"冻结周期或深度一致"*/
                                        result = TRUE;
                                    }
                                    break;
                                }
                            }
                            else/*"新的冻结方案"*/
                            {
                                if((pdata[0] == 0) && (pdata[1] == 0))
                                {
                                    break;
                                }
                                pPrj->prjNo = prjNo;
                                pPrj->index= n;						
                                result = TRUE;
                                break;
                            }

                            pPrj++;
                        }
                    }
                    else/*"其它冻结"*/
                    {
                        if(0 != prjNo) /*" 属性特征值固定为0"*/
                        {
                            break;
                        }

                        if((0 == n) && (FALSE == ChkIsNonPeriodFrz(rcrd_obis)))/*"周期冻结，只判断第1个"*/
                        {
                            if(((1 != *pdata) && (0 != *pdata)) || (0 != *(pdata + 1)))/*"冻结周期固定为1或0"*/
                            {
                                break;
                            }
                        }

                        if((0 != n) && /*" 周期、深度须与首个关联OAD一致"*/
                        ((EQUAL != LIB_MemCmp(pList, pdata, 2)) || 
                        (EQUAL != LIB_MemCmp((pList + 6), (pdata + 6), 2))))
                        {
                            break;
                        }

                        result = TRUE;
                    }
                }
                else/*"事件记录"*/
                {
                    result = TRUE;
                }

                if(TRUE == result)
                {
                    LIB_MemCpy(pdata, pTemp, len);
                }

                break; 
            }
            else
            {
                if(rcrd_obis < E_EVT_END_RCRD) 
                {
                    cur_aid = pTemp[2];
                    if((AidType_is_evt_start(new_aid)) && (AidType_is_evt_end(cur_aid)))
                    {/*" 发生前关联对象要插入在发生后关联对象之前 "*/
                        p_st = pList + (sum_len - 4); /*" 最后一个位置 "*/
                        if(EQUAL == LIB_ChkValue(p_st, len, 0)) /*"尚有空间新增OAD"*/
                        {
                            p_end = pTemp;
                            for(;p_st >= p_end; p_st-=4)
                            {
                                if(EQUAL != LIB_ChkValue(p_st, 4, 0))
                                {
                                    LIB_MemCpy(p_st, p_st + 4, 4);
                                }
                            }
                            LIB_MemCpy(pdata, pTemp, len);
                            result = TRUE;
                        }
                        break;
                    }
                }
            }

            pTemp += len;
        }
    }

    if(FALSE == result)
    {
    }

    return result;
}


/*"事件记录配置关联属性列表排序"*/
static INT8U EvtROadSort(INT8U *pdata, INT16U rcrd_obis, INT8U *pList)
{
    INT8U n, k, sortFlg, dataType, ROadMax;
    INT8U sCnt, eCnt;
    INT8U *pOad, *pLast;
    ST_OAD tempOad;
    ST_OAD *cur;

    sortFlg = 0;
    cur = (ST_OAD *)pdata;
    for(n = 0; n < ARRAY_SIZE(EvtWithEnd); n++)
    {
        if(rcrd_obis == EvtWithEnd[n].evtObis)/*"记录数据有发生和结束"*/
        {
            dataType = (INT8U)(cur->AId >> 5);
            if(AidType_is_evt_start(dataType)) /*"新增为发生时的ROAD"*/
            {
                sortFlg = 0x55;
            }
            break;
        }
    }

    if(0x55 == sortFlg)
    {
        sCnt = 0;
        eCnt = 0;
        pOad = pList;
        ROadMax = GetRcrdMaxRoad(rcrd_obis);
        for(n = 0; n < ROadMax; n++)
        {
            if(EQUAL == LIB_ChkValue(pOad, sizeof(ST_OAD), 0))
            {
                break;
            }

            cur = (ST_OAD *)pOad;
            dataType = (INT8U)(cur->AId >> 5);
            if(AidType_is_evt_start(dataType))
            {
                sCnt++;

                if(eCnt > 0)/*"发生ROAD排在结束OAD后，排序"*/
                {
                    LIB_MemCpy(pOad, (INT8U *)&tempOad, sizeof(ST_OAD));

                    for(k = 0; k < eCnt; k++)
                    {
                        pLast = pOad - sizeof(ST_OAD);
                        LIB_MemCpy(pLast, pOad, sizeof(ST_OAD));                        
                        pOad -= sizeof(ST_OAD);
                    }
                    LIB_MemCpy((INT8U *)&tempOad, pOad, sizeof(ST_OAD));

                    return TRUE;
                }
            }
            else
            {
                eCnt++;
            }

            pOad += sizeof(ST_OAD);
        }
    }
    return FALSE;
}



static INT8U DelROad(INT8U *pdata, EN_RCRD_FILE_ID rcrd_obis)
{
    INT8U cnt, len;
    INT8U n, k, off;
    INT8U OadList[ROADINFO_BUFF_LEN]; /*"取冻结ROAD列表最大长度(需取冻结与事件ROAD最大长度)"*/
    INT8U *pTemp, *pNext;
    INT16U sum_len;

    if(rcrd_obis > E_EVT_END_RCRD)
    {
        len = sizeof(ST_ROAD_FRZ_INFO);
        off = 2;
    }
    else
    {
        len = sizeof(ST_OAD);
        off = 0;
    }

    pTemp = &OadList[0];
    GetRcrdRoad(rcrd_obis, OadList, &sum_len);
    cnt = sum_len/len;

    for(n = 0; n < cnt; n++)
    {
        if(EQUAL == LIB_MemCmp((pTemp + off), pdata, sizeof(ST_OAD))) 
        {/*"OAD存在"*/
            pNext = pTemp + len;

            for(k = n; k < cnt; k++)
            {
                if((k < (cnt - 1)) && (EQUAL != LIB_ChkValue(pNext, len, 0)) )
                {/*"后续有效OAD"*/
                    LIB_MemCpy(pNext, pTemp, len);
                }
                else
                {
                    LIB_MemSet(0, pTemp, len);
                    break;
                }

                pTemp += len;
                pNext += len;
            }

            SaveRcrdRoad(rcrd_obis, OadList, sum_len);
            if(E_MIN_FRZ_RCRD == rcrd_obis)
            {
                PP_InitFrzMinPrjList();
            }
            RCRD_ClrDataMdfRoad(rcrd_obis);/*"清除记录数据"*/

            return TRUE; 
        }

        pTemp += len;
    }

    return FALSE;
}

INT8S RCRD_AddOneRoad(void *pdin, void *pdout)
{
    INT16U sum_len;
    INT8U OadList[ROADINFO_BUFF_LEN];//
    INT8U result;
    ST_ACTION_DATa *road_op;
    //INT8U mErrInfo;
    INT8U  PrjList[FRZ_PRJ_LIST_LEN];
    EN_RCRD_FILE_ID rcrd_obis;
    INT8S dar;

    road_op = (ST_ACTION_DATa *)pdin;
    rcrd_obis = (EN_RCRD_FILE_ID)road_op->Obis;
    dar = -DAR_3_NOACESS;

    /*" 自己扩的复位事件不支持关联对象操作 "*/
    if(is_private_rcrd(rcrd_obis) || rcrd_id_is_error(rcrd_obis))
    {
        return dar;
    }

    /*私钥状态下，电能表清零事件中的关联对象列表不允许修改*/
    if((rcrd_obis == E_MTR_CLR_EVNT_RCRD) && (ChkStatus(E_PRIVATE) == TRUE))
    {
        return dar;
    }

    result = FALSE;
    if((rcrd_obis < E_EVT_END_RCRD)
     && (TRUE == ChkEvtROadAid(road_op->pdata + 2)) )
    {
        result = TRUE;
    }
    else if(rcrd_obis > E_EVT_END_RCRD && rcrd_obis <= E_STEP_BILL_FRZ_RCRD)
    {
        result = TRUE;
        if(rcrd_obis == E_MIN_FRZ_RCRD)
        {
            LIB_MemCpy((INT8U *)&mstFrzPrj, PrjList, FRZ_PRJ_LIST_LEN);
        }
    }
    
    if(result == TRUE)
    {
        GetRcrdRoad(rcrd_obis, OadList, &sum_len);

        result = AddROad(road_op->pdata, rcrd_obis, OadList, PrjList, sum_len);

        if(TRUE == result)
        {
            dar = DAR_0_OK;
            SaveRcrdRoad(rcrd_obis, OadList, sum_len);
            if(rcrd_obis == E_MIN_FRZ_RCRD)
            {
                LIB_MemCpy(PrjList, (INT8U *)mstFrzPrj, FRZ_PRJ_LIST_LEN);
            }
            RCRD_ClrDataMdfRoad(rcrd_obis);/*"清除记录数据"*/
        }
    }

    return dar;
}

INT8S RCRD_AddMultRoad(void *pdin, void *pdout)
{
    INT8U i, cnt, len;
    INT8U OadList[ROADINFO_BUFF_LEN];
    INT8U *pSet;
    INT8U result;
    INT8U  PrjList[FRZ_PRJ_LIST_LEN];
    ST_ACTION_DATa *road_op;
    EN_RCRD_FILE_ID rcrd_obis;
    INT8U ROadMax;
    INT16U sum_len;
    INT8S dar;

    road_op = (ST_ACTION_DATa *)pdin;
    rcrd_obis = (EN_RCRD_FILE_ID)road_op->Obis;
    
    pSet = road_op->pdata;
    result = FALSE;
    dar = -DAR_3_NOACESS;

    /*" 自己扩的复位事件不支持关联对象操作 "*/
    if(is_private_rcrd(rcrd_obis) || rcrd_id_is_error(rcrd_obis))
    {
        return dar;
    }

    if(rcrd_obis < E_EVT_END_RCRD)
    {
        len = sizeof(ST_OAD);
    }
    else if (rcrd_obis > E_EVT_END_RCRD && rcrd_obis <= E_STEP_BILL_FRZ_RCRD)
    {
        len = sizeof(ST_ROAD_FRZ_INFO);
    }
    else
    {
        return dar;
    }
    cnt = road_op->len/len;
    
    ROadMax = GetRcrdMaxRoad(rcrd_obis);

    if((cnt > 0) && (cnt <= ROadMax))
    {
        sum_len = ROadMax * len;

        if(0x55 == mSetROadLstFlg)
        {
            /*"SET服务批量添加前先批量清除"*/
            LIB_MemSet(0, OadList, ROADINFO_BUFF_LEN);
            if(rcrd_obis == E_MIN_FRZ_RCRD)
            {
                LIB_MemSet(0xFF, (INT8U *)&mstFrzPrj, FRZ_PRJ_LIST_LEN);
            }
        }
        else
        {
            GetRcrdRoad(rcrd_obis, OadList, &sum_len);
        }

        if(rcrd_obis == E_MIN_FRZ_RCRD)
        {
            LIB_MemCpy((INT8U *)&mstFrzPrj, PrjList, FRZ_PRJ_LIST_LEN);
        }

        
        for(i = 0; i < cnt; i++)
        {
            result = AddROad(pSet, rcrd_obis, OadList, PrjList, sum_len);
            if(FALSE == result)
            {
                break;
            }

            pSet += len;
        }

        if(TRUE == result)
        {
            dar = DAR_0_OK;
            SaveRcrdRoad(rcrd_obis, OadList, sum_len);
            if(rcrd_obis == E_MIN_FRZ_RCRD)
            {
                if(0x55 == mSetROadLstFlg)
                {
                    PP_InitFrzMinPrjList();
                }
                else
                {
                    LIB_MemCpy(PrjList, (INT8U *)mstFrzPrj, FRZ_PRJ_LIST_LEN);
                }
            }
            RCRD_ClrDataMdfRoad(rcrd_obis);/*"清除记录数据"*/
        }
    }

    return dar;
}


INT8S RCRD_DelOneRoad(void *pdin, void *pdout)
{
    INT8U result;
    ST_ACTION_DATa *road_op;
    EN_RCRD_FILE_ID rcrd_obis;
    INT8U *pdata;
    INT8S dar;

    road_op = (ST_ACTION_DATa *)pdin;
    pdata = road_op->pdata;
    rcrd_obis = (EN_RCRD_FILE_ID)road_op->Obis;
    dar = -DAR_3_NOACESS;

    /*" 自己扩的复位事件不支持关联对象操作 "*/
    if(is_private_rcrd(rcrd_obis) || rcrd_id_is_error(rcrd_obis))
    {
        return dar;
    }

    /*私钥状态下，电能表清零事件中的关联对象列表不允许修改*/
    if((rcrd_obis == E_MTR_CLR_EVNT_RCRD) && (ChkStatus(E_PRIVATE) == TRUE))
    {
        return dar;
    }

    result = FALSE;
    if((rcrd_obis < E_EVT_END_RCRD)
     && (EQUAL != LIB_ChkValue(pdata, sizeof(ST_OAD), 0)) )
    {
        result = TRUE;
    }
    else if((rcrd_obis > E_EVT_END_RCRD && rcrd_obis <= E_STEP_BILL_FRZ_RCRD)
        && (EQUAL != LIB_ChkValue(pdata, sizeof(ST_ROAD_FRZ_INFO), 0)))
    {
        result = TRUE;
    }
    if(result == TRUE)
    {
        result = DelROad(pdata, rcrd_obis);
        if(result == TRUE)
        {
            dar = DAR_0_OK;
        }
    }

    return dar;
}

INT8S RCRD_DelAllRoad(void *pdin, void *pdout)
{
    INT8U OadList[ROADINFO_BUFF_LEN];
    INT8U len;
    ST_ACTION_DATa *road_op;
    EN_RCRD_FILE_ID rcrd_obis;
    INT8U ROadMax;
    INT16U tLen;
    INT8S dar;

    road_op = (ST_ACTION_DATa *)pdin;
    rcrd_obis = (EN_RCRD_FILE_ID)road_op->Obis;
    dar = -DAR_3_NOACESS;
    
    /*" 自己扩的复位事件不支持关联对象操作 "*/
    if(is_private_rcrd(rcrd_obis) || rcrd_id_is_error(rcrd_obis))
    {
        return dar;
    }
    
    if((rcrd_obis <= E_STEP_BILL_FRZ_RCRD))
    {
        if(rcrd_obis < E_EVT_END_RCRD)
        {
            len = sizeof(ST_OAD);
        }
        else if (rcrd_obis > E_EVT_END_RCRD && rcrd_obis <= E_STEP_BILL_FRZ_RCRD)
        {
            len = sizeof(ST_ROAD_FRZ_INFO);
        }
        else
        {
            return dar;
        }

        dar = DAR_0_OK;
        LIB_MemSet(0, OadList, ROADINFO_BUFF_LEN);

        ROadMax = GetRcrdMaxRoad(rcrd_obis);
        tLen = ROadMax * len;

        SaveRcrdRoad(rcrd_obis, OadList, tLen);
        if(E_MIN_FRZ_RCRD == rcrd_obis)
        {
            LIB_MemSet(0xFF, (INT8U *)&mstFrzPrj, FRZ_PRJ_LIST_LEN);
        }

        RCRD_ClrDataMdfRoad(rcrd_obis); /*" 清除记录数据"*/
    }

    return dar;
}

#endif