
#include "AXDR.h"
#include "System\Sys_def.h"
#include "Lib\LIB_func.h"

INT16S DecodeTest(void);


INT8U SrcBuf[255];
INT8U DstBuf[500];



#define _DAILYSLOT_NUM 14        /*"日时段数"*/
#define _EACH_SLOT_SIZE  3        /*"年时区表，日时段表的成员个数"*/


/*"显示取数输入信息"*/
typedef struct
{
    ST_OAD Oad;/*"CSD: 基本OAD"*/
    ST_OAD releOad;/*"当显示对象CSD为ROAD时，对应关联OAD"*/
    INT8U scrNo;/*"屏序号"*/
}ST_SHOWDATA_INPUT; /*"提示字符和显示格式信息"*/



/*轮显，显示对象列表 F300*/
const ST_ENCODE_INFO cycle_dis_obj_list_info[]={{array,DIS_OBJ_NUM_CYC},{structure,2},{CSD,NULL},{unsigned_t,1}};/*显示对象列表*/

/*事件的关联对象列表*/
const ST_ENCODE_INFO evt_r_oad_info[]={{array,UNFIXED_LEN},{OAD_t,4}};

/*日时段表*/
const ST_ENCODE_INFO dslot_info[]={{array,EN_LEN_DSLOT_NUM},/*日时段表数*/
                                    {array,_DAILYSLOT_NUM}, /*日时段数*/
                                    {structure,0x03},
                                    {unsigned_t,1},{unsigned_t,1},{unsigned_t,1}};

/*事件记录当前值记录表*/
const ST_ENCODE_INFO evt_now_value_info[]={array,0x01,
                                            structure,0x2,doublelongunsigned,NULL,null_data,NULL};


/*设备地理位置*/
const ST_ENCODE_INFO dev_position_info[]={{structure,0x03},
                                          {structure,0x04},{enum_t,0x01},{unsigned_t,0x01},{unsigned_t,0x01},{unsigned_t,0x01},
                                          {structure,0x04},{enum_t,0x01},{unsigned_t,0x01},{unsigned_t,0x01},{unsigned_t,0x01},
                                          {doublelongunsigned,0x04},
                                          };

const ST_ENCODE_INFO data_updata_info[]={{structure,0x02},
                                          {octetstring,UNFIXED_LEN},
                                          {SID_MAC,UNFIXED_LEN},
};


const ST_ENCODE_INFO BillDayInfo[] = {
    {array,3},
    {structure,2},{unsigned_t,1},{unsigned_t,1},

};

const ST_ENCODE_INFO OverCurrInfo[]   = {
    {structure,2},
    {doublelong,4},{unsigned_t,1},
};


INT8U SidMacLen;    
INT16S LenGet_PP(ENUM_DATALEN_ID Id)
{
    INT16U RetLen;
    if(Id == DIS_OBJ_NUM_CYC)
    {
        RetLen = 3;
    }
    else if (Id == EN_LEN_DSLOT_NUM)
    {
        RetLen = 2;
    }
    else if(Id == UNFIXED_LEN)
    {
        RetLen = 0;
    }
    else
    {
        return -1;
    }
    return RetLen;
}

INT16S LenGet_ESam(ENUM_DATALEN_ID Id)
{
    INT16U RetLen;

    if(Id == SID_ADDDATA_LEN)
    {
        RetLen = SidMacLen;
    }
    else
    {
        RetLen = -1;
    }
    return RetLen;
}


/*test CSD*/
const INT8U csd_en_1[] = {CHOICE_OAD, 0x40,0x23,0x01,0x02};
const INT8U csd_en_2[] = {CHOICE_ROAD,0x40,0x23,0x01,0x02,0x01,0x50,0x12,0x02,0x03};
const ST_SHOWDATA_INPUT TabDis[]={
    { {0x5002,0x2,0x1},{0x0001,0x2,0x1},0 },
    { {0x0002,0x2,0x2},{0x0000,0x0,0x0},1 },
    { {0x7002,0x2,0x3},{0x0003,0x2,0x1},2 },
} ;
const INT8U csd_en_3[]={
array,0x03,
structure,0x02,CSD,CHOICE_ROAD,0x50,0x02,0x02,0x01, 0x01, 0x00,0x01,0x02,0x01,unsigned_t,0x00,
structure,0x02,CSD,CHOICE_OAD, 0x00,0x02,0x02,0x02,unsigned_t,0x01,
structure,0x02,CSD,CHOICE_ROAD,0x70,0x02,0x02,0x03, 0x01, 0x00,0x03,0x02,0x01,unsigned_t,0x02,
};

const INT8U csd_en_4[]={

structure,0x02,CSD,CHOICE_OAD, 0x00,0x02,0x02,0x02,unsigned_t,0x01,
};

INT16S TestCsd(void)
{
    ST_ENCODE_INFO TmpInfo;
    ST_OAD TmpOad;
    INT16U RetLen;

////////////////CSD//////////////////////////////////////    
    /*test CSD 单独编码CSD*/
    TmpInfo.Type = CSD;
    
    SrcBuf[0]= 0;/*choice=0 OAD*/
    TmpOad.Oi.asInt = 0x4023;
    TmpOad.AId = 0x01;
    TmpOad.IId = 0x02;
    LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&SrcBuf[1], sizeof(ST_OAD));
    TmpOad.Oi.asInt = 0x5012;
    TmpOad.AId = 0x02;
    TmpOad.IId = 0x03;
    LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&SrcBuf[5], sizeof(ST_OAD));
    RetLen = EnCode_Simple(SrcBuf, &TmpInfo, DstBuf);

    if(RetLen == sizeof(csd_en_1))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&csd_en_1, sizeof(csd_en_1)))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    
    SrcBuf[0]= 1;/*choice=1 ROAD*/
    RetLen = EnCode_Simple(SrcBuf, &TmpInfo, DstBuf);

    if(RetLen == sizeof(csd_en_2))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&csd_en_2, sizeof(csd_en_2)))
        {
            return -2;
        }
    }
    else
    {
        return -2;
    }

    /*test data include CSD*/

    
    SrcBuf[0]= CHOICE_ROAD;
    LIB_MemCpy((INT8U*)&TabDis[0], &SrcBuf[1], 9);
    SrcBuf[10]= CHOICE_OAD;
    LIB_MemCpy((INT8U*)&TabDis[1], &SrcBuf[11], 4);
    LIB_MemCpy((INT8U*)&TabDis[1]+ FIND(ST_SHOWDATA_INPUT,scrNo), &SrcBuf[15], 1);

    SrcBuf[16]= CHOICE_ROAD;
    LIB_MemCpy((INT8U*)&TabDis[2], &SrcBuf[17], 9);

    RetLen = EnCode_Data((ST_ENCODE_INFO *)cycle_dis_obj_list_info, SrcBuf, DstBuf, 0);
    
    if(RetLen == sizeof(csd_en_3))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&csd_en_3, sizeof(csd_en_3)))
        {
            return -3;
        }
    }
    else
    {
        return -3;
    }

    return 1;
}


const INT8U SidTab[] ={0x99,0x88,0x77};
const INT8U MacTab[]={0x11,0x22,0x33,0x44};

const INT8U sidmac_en_1[]={0x12,0x34,0x56,0x78,
                           0x03,0x99,0x88,0x77,
                           0x04,0x11,0x22,0x33,0x44};

INT16S TestSidMac(void)
{
    ST_ENCODE_INFO TmpInfo;
    INT16U RetLen;
    INT32U Sid;
    
    /*填充原始数据*/
    Sid = 0x12345678;
    LIB_MemCpy((INT8U *)&Sid, SrcBuf, sizeof(INT32U));
    RetLen = 4;
    LIB_MemCpy((INT8U*)SidTab, &SrcBuf[RetLen], sizeof(SidTab));
    RetLen += 3;
    LIB_MemCpy((INT8U*)MacTab, &SrcBuf[RetLen], sizeof(MacTab));

    ////////////////SID_MAC//////////////////////////////////////    
    TmpInfo.Type = SID_MAC;
    SidMacLen = 3;/*需要确定SidMac的长度*/

    
    RetLen = EnCode_Simple(SrcBuf, &TmpInfo , DstBuf);
    if(RetLen == sizeof(sidmac_en_1))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U *)sidmac_en_1, sizeof(sidmac_en_1)))
        {
            return -4;
        }
    }
    else
    {
        return -4;
    }

    return 1;
}


const ST_OAD OadTab[]={{{0x0001},0x1,0x02},
                       {{0x0002},0x3,0x04}, 
                       {{0x1234},0x3,0x04}
};
const INT8U oad_en_1[]={0x00,0x01,0x01,0x02};

const INT8U oad_en_2[]={array,ARRAY_SIZE(OadTab),
                        OAD_t,0x00,0x01,0x01,0x02,
                        OAD_t,0x00,0x02,0x03,0x04,
                        OAD_t,0x12,0x34,0x03,0x04,
                        };
const INT8U oad_en_3[]={array,0x00};
const INT8U oad_en_4[]={array,0x01,
                        OAD_t,0x00,0x01,0x01,0x02};

INT16S TestOad(void)
{
    INT16U RetLen;
    ST_ENCODE_INFO TmpInfo;
    ST_ENCODE_INFO Tmp_evt_r_oad[ARRAY_SIZE(evt_r_oad_info)];

    LIB_MemCpy((INT8U*)&OadTab[0], SrcBuf, 4);

    TmpInfo.Type = OAD_t;
    RetLen = EnCode_Simple(SrcBuf, &TmpInfo , DstBuf);

    if(RetLen == sizeof(oad_en_1))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)oad_en_1, sizeof(oad_en_1)))
        {
            return -5;
        }
    }
    else
    {
        return -5;
    }

    /*test oad in data,array 长度可变处理*/
    LIB_MemCpy((INT8U * )&evt_r_oad_info, (INT8U * )&Tmp_evt_r_oad, sizeof(evt_r_oad_info));

    Tmp_evt_r_oad[0].Len = ARRAY_SIZE(OadTab);/*长度可变的处理*/

    LIB_MemCpy((INT8U * )&OadTab[0], SrcBuf, sizeof(OadTab));
    
    RetLen = EnCode_Data(Tmp_evt_r_oad, SrcBuf, DstBuf, 0);

    if(RetLen == sizeof(oad_en_2))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)oad_en_2, sizeof(oad_en_2)))
        {
            return -5;
        }
    }

    /*编码长度测试*/
    RetLen = EnCode_Data(Tmp_evt_r_oad, SrcBuf, NULL, 0);
    if(RetLen != sizeof(oad_en_2))
    {
        return -5;
    }


    /*OAD 个数为0时编码*/
    Tmp_evt_r_oad[0].Len = 0;
    RetLen = EnCode_Data(Tmp_evt_r_oad, SrcBuf, DstBuf, 0);
    if(RetLen == sizeof(oad_en_3))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)oad_en_3, sizeof(oad_en_3)))
        {
            return -5;
        }
    }
    
    /*OAD 个数为1时编码*/
    Tmp_evt_r_oad[0].Len = 1;
    RetLen = EnCode_Data(Tmp_evt_r_oad, SrcBuf, DstBuf, 0);
    if(RetLen == sizeof(oad_en_4))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)oad_en_4, sizeof(oad_en_4)))
        {
            return -5;
        }
    }
    return 1;
}


const INT8U SegData[2][_DAILYSLOT_NUM * _EACH_SLOT_SIZE] =
{
    {/*第1套日时段表默认参数*/
    0x00, 0x00, 0x01,
    0x03, 0x00, 0x02,
    0x06, 0x00, 0x03,
    0x09, 0x00, 0x04,
    0x0C, 0x00, 0x01,
    0x0F, 0x00, 0x02,
    0x12, 0x00, 0x03,
    0x15, 0x00, 0x04,
    0x15, 0x00, 0x04,
    0x15, 0x00, 0x04,
    0x15, 0x00, 0x04,
    0x15, 0x00, 0x04,
    0x15, 0x00, 0x04,
    0x15, 0x00, 0x04,
    },
    {/*第2套日时段表默认参数*/
	0x00, 0x00, 0x01,
	0x02, 0x00, 0x02,
	0x04, 0x00, 0x03,
	0x06, 0x00, 0x04,
	0x08, 0x00, 0x01,
	0x0A, 0x00, 0x02,
	0x0C, 0x00, 0x03,
	0x0E, 0x00, 0x04,
    0x0E, 0x00, 0x04,
    0x0E, 0x00, 0x04,
    0x0E, 0x00, 0x04,
    0x0E, 0x00, 0x04,
    0x0E, 0x00, 0x04,
    0x0E, 0x00, 0x04,
    },
};

const INT8U Dslot_en_1[]=
{
    array,0x02,
    array,14,
    structure,0x03,unsigned_t,0x00,unsigned_t,0x00,unsigned_t,0x01,
    structure,0x03,unsigned_t,0x03,unsigned_t,0x00,unsigned_t,0x02,
    structure,0x03,unsigned_t,0x06,unsigned_t,0x00,unsigned_t,0x03,
    structure,0x03,unsigned_t,0x09,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0C,unsigned_t,0x00,unsigned_t,0x01,
    structure,0x03,unsigned_t,0x0F,unsigned_t,0x00,unsigned_t,0x02,
    structure,0x03,unsigned_t,0x12,unsigned_t,0x00,unsigned_t,0x03,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x15,unsigned_t,0x00,unsigned_t,0x04,
    array,14,
    structure,0x03,unsigned_t,0x00,unsigned_t,0x00,unsigned_t,0x01,
    structure,0x03,unsigned_t,0x02,unsigned_t,0x00,unsigned_t,0x02,
    structure,0x03,unsigned_t,0x04,unsigned_t,0x00,unsigned_t,0x03,
    structure,0x03,unsigned_t,0x06,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x08,unsigned_t,0x00,unsigned_t,0x01,
    structure,0x03,unsigned_t,0x0A,unsigned_t,0x00,unsigned_t,0x02,
    structure,0x03,unsigned_t,0x0C,unsigned_t,0x00,unsigned_t,0x03,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
    structure,0x03,unsigned_t,0x0E,unsigned_t,0x00,unsigned_t,0x04,
};


INT16S TestDslot(void)
{
    //ST_ENCODE_INFO Tmp_dslot_info[ARRAY_SIZE(dslot_info)];
    INT16U RetLen;

    //LIB_MemCpy((INT8U *)&dslot_info, (INT8U * )&Tmp_dslot_info, sizeof(dslot_info));
    
    //Tmp_dslot_info[0].Len = 2;/*两个日时段表*/

    LIB_MemCpy((INT8U*)&SegData[0][0], SrcBuf, sizeof(SegData));
    
    RetLen = EnCode_Data((ST_ENCODE_INFO*)dslot_info, SrcBuf, DstBuf, 0);

    if(RetLen == sizeof(Dslot_en_1))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)Dslot_en_1, sizeof(Dslot_en_1)))
        {
            return -6;
        }
    }
    else
    {
        return -6;
    }

    /*测试计算编码长度是否正确*/
    RetLen = EnCode_Data((ST_ENCODE_INFO*)dslot_info, SrcBuf, NULL, 0);
    if(RetLen == sizeof(Dslot_en_1))
    {

    }
    else
    {
        return -6;
    }
    
    /*编码叶子1*/
    RetLen = EnCode_Data((ST_ENCODE_INFO*)dslot_info, SrcBuf, DstBuf, 1);
    if(RetLen == (sizeof(Dslot_en_1)-2)/2)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&Dslot_en_1[2], RetLen))
        {
            return -6;
        }
    }
    else
    {
        return -6;
    }

    /*编码叶子2*/
    RetLen = EnCode_Data((ST_ENCODE_INFO*)dslot_info, (INT8U*)&SrcBuf[42], DstBuf, 2);
    if(RetLen == (sizeof(Dslot_en_1)-2)/2)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&Dslot_en_1[2+RetLen], RetLen))
        {
            return -6;
        }
    }
    else
    {
        return -6;
    }

    
    return 1;
}

const INT8U null_en_1[]={array,0x01,
                         structure,0x02,
                         doublelongunsigned,0x12,0x34,0x56,0x78,
                         null_data};

const ST_ENCODE_INFO TestArray0[]={{array,0x00}};
const INT8U null_en_2[]={array,0x00};
INT16S TestNULL(void)
{
    INT16U RetLen;
    INT32U Times;
    
    Times= 0x12345678;

    LIB_MemCpy((INT8U * )&Times, (INT8U * )&SrcBuf, sizeof(INT32U));
    RetLen = EnCode_Data((ST_ENCODE_INFO *)evt_now_value_info, (INT8U*)&SrcBuf[0], DstBuf, 0);

    if(RetLen == sizeof(null_en_1))
    {
        if(EQUAL !=LIB_MemCmp(DstBuf, (INT8U*)&null_en_1[0], RetLen))
        {
            return -7;
        }
    }
    else
    {
        return -7;
    }

    RetLen = EnCode_Data((ST_ENCODE_INFO *)TestArray0, (INT8U*)&SrcBuf[0], DstBuf, 0);
    if(RetLen == sizeof(null_en_2))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&null_en_2[0], RetLen))
        {
            return -7;
        }
    }
    else 
    {
        return -7;
    }
    return TRUE;
}

const ST_ENCODE_INFO TestDateTimes[]={{date_time_s,0x07}};
const INT8U DateTimesEn1[]={date_time_s,0x07,0xE1,12,21,17,05,36};
INT16S TestEncodeDateTimes(void)
{
    INT16U RetLen;
    INT16U Year;

    Year = 2017;

    LIB_MemCpy((INT8U * )&Year, SrcBuf, 2);
    SrcBuf[2] = 12;/*月*/
    SrcBuf[3] = 21;/*日*/
    SrcBuf[4] = 17;/*时*/
    SrcBuf[5] = 05;/*分*/
    SrcBuf[6] = 36;/*秒*/
    
    RetLen = EnCode_Data((ST_ENCODE_INFO *)TestDateTimes, SrcBuf, DstBuf, 0);
    if(RetLen == 8)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&DateTimesEn1[0], RetLen))
        {
            return -8;
        }
    }
    else
    {
        return -8;
    }
    return TRUE;
}



const ST_ENCODE_INFO TestRunStaForm[]={{array,0x07},{bitstring,16}};
const INT16U RunSta[7] = {
0x0001,
0x8000,
0x0003,
0x1004,
0x0210,
0x0020,
0x8030,
};

const INT8U EncodeRunsta1[]=
{
    array,0x07,
    bitstring,16,0x80,0x00,
    bitstring,16,0x00,0x01,
    bitstring,16,0xC0,0x00,
    bitstring,16,0x20,0x08,
    bitstring,16,0x08,0x40,
    bitstring,16,0x04,0x00,
    bitstring,16,0x0C,0x01,

};
    
INT16S TestBitString(void)
{
    INT16U RetLen;
    
    LIB_MemCpy((INT8U *)&RunSta, SrcBuf, 14);
    RetLen = EnCode_Data((ST_ENCODE_INFO *)TestRunStaForm, SrcBuf, DstBuf, 0);
    if(RetLen == sizeof(EncodeRunsta1))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)&EncodeRunsta1[0], RetLen))
        {
            return -9;
        }
    }
    else
    {
        return -9;
    }
    return TRUE;

}

INT16S EncodeTest(void)
{


    AXDR_DataLenGetInit(LenGet_PP,0);
    AXDR_DataLenGetInit(LenGet_ESam,1);

    if(TestCsd()<0)
    {
        return -1;
    }
    
    if(TestSidMac()<0)
    {
        return -2;
    }

    if(TestOad()<0)
    {
        return -3;
    }

    if(TestDslot()<0)
    {
        return -4;
    }

    if(TestNULL()<0)
    {

        return -5;
    }

    if(TestEncodeDateTimes()<0)
    {
        return -6;

    }

    if( TestBitString()<0 )
    {
        return -7;
    }

    DecodeTest();
    
    return 1;
}



const ST_OAD de_oad_1={0x0010,0x02,0x00}; 
INT16S DeCodeOad(void)
{
    ST_ENCODE_INFO TmpInfo;
    ST_OAD TmpOad;
    INT16S RetLen;

    SrcBuf[0] = 0x00;
    SrcBuf[1] = 0x10;
    SrcBuf[2] = 0x02;
    SrcBuf[3] = 0x00;

    TmpInfo.Type = OAD_t;
    
    RetLen =DeCode_Simple(SrcBuf,&TmpInfo,(INT8U* )&TmpOad);

    if(RetLen == 4)
    {
        if(EQUAL != LIB_MemCmp((INT8U* )&TmpOad, (INT8U * )&de_oad_1, 4))
        {
            return -1;
        }
    } 
    else
    {
        return -1;
    }
    
    return TRUE;
}



INT16S   DeCodeDslot(void)
{
    INT16S Ret;

    /*拷贝待解码数据*/
    LIB_MemCpy((INT8U*)Dslot_en_1, SrcBuf, sizeof(Dslot_en_1));

    /*测试全解码 是否正确*/
    Ret = DeCode_Data((ST_ENCODE_INFO*)dslot_info,SrcBuf,DstBuf,0);    
    if(Ret == sizeof(SegData))
    {
        if(EQUAL !=LIB_MemCmp(DstBuf, (INT8U * )&SegData[0][0], Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    

    /*测试不输出解码，只计算长度*/
    Ret = DeCode_Data((ST_ENCODE_INFO*)dslot_info,SrcBuf,NULL,0); 
    if(Ret != sizeof(SegData))
    {
        return -1;
    }

    /*测试和信息不匹配码是否报错*/
    SrcBuf[5]= 0x04;/*修改struct的个数不为03*/
    Ret = DeCode_Data((ST_ENCODE_INFO*)dslot_info,SrcBuf,DstBuf,0);   
    if(Ret != DECODE_ERR_LEN)
    {
        return -1;
    }
    

    /*测试子层数据解码*/
    /*1st*/
    LIB_MemCpy((INT8U*)&Dslot_en_1[2], SrcBuf, (sizeof(Dslot_en_1)-2)/2);
    Ret = DeCode_Data((ST_ENCODE_INFO*)dslot_info,SrcBuf,DstBuf,1);    

    if(Ret == (sizeof(SegData)/2))
    {
        if(EQUAL !=LIB_MemCmp(DstBuf, (INT8U*)&SegData[0][0], Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    /*2st*/
    LIB_MemCpy((INT8U*)&Dslot_en_1[2+((sizeof(Dslot_en_1)-2)/2)], SrcBuf, (sizeof(Dslot_en_1)-2)/2);
    Ret = DeCode_Data((ST_ENCODE_INFO*)dslot_info,SrcBuf,DstBuf,1);    

    if(Ret == (sizeof(SegData)/2))
    {
        if(EQUAL !=LIB_MemCmp(DstBuf, (INT8U*)&SegData[1][0], Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    
    return TRUE;
    
}

const INT8U devPos_en_1[]=
{
    structure,0x03,
    structure,0x04,enum_t,0x00,unsigned_t,0x01,unsigned_t,0x02,unsigned_t,0x03,
    structure,0x04,enum_t,0x01,unsigned_t,0x04,unsigned_t,0x05,unsigned_t,0x06,
    doublelongunsigned,0x12,0x34,0x56,0x78,

};

typedef struct{
    INT8U EW[4];
    INT8U SN[4];
    INT32U higher;
}ST_DEV_POS;

const ST_DEV_POS DevPos = {
    {0x00,0x01,0x02,0x03},
    {0x01,0x04,0x05,0x06},
    {0x12345678},
};

INT16S DeCodePosition()
{
    INT16S Ret;

    LIB_MemCpy((INT8U*)devPos_en_1, SrcBuf, sizeof(devPos_en_1));
    
    Ret = DeCode_Data((ST_ENCODE_INFO *)dev_position_info,SrcBuf,DstBuf,0);
    if(Ret == sizeof(DevPos))
    {
       if(EQUAL != LIB_MemCmp((INT8U*)&DevPos, DstBuf,Ret))
       {
            return -1;
       }
    }
    else
    {
        return -1;
    }

    /*解码部分1*/
    LIB_MemCpy((INT8U*)&devPos_en_1[2], SrcBuf, 10);
    Ret = DeCode_Data((ST_ENCODE_INFO *)dev_position_info,SrcBuf,DstBuf,1);
    if(Ret == 4)
    {
        if(EQUAL != LIB_MemCmp((INT8U*)&DevPos, DstBuf,Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;

    }


    /*解码部分2*/
    LIB_MemCpy((INT8U*)&devPos_en_1[12], SrcBuf, 10);
    Ret = DeCode_Data((ST_ENCODE_INFO *)dev_position_info,SrcBuf,DstBuf,2);
    if(Ret == 4)
    {
        if(EQUAL != LIB_MemCmp((INT8U*)&DevPos+4, DstBuf,Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;

    }

    /*解码部分3*/
    LIB_MemCpy((INT8U*)&devPos_en_1[22], SrcBuf, 5);
    Ret = DeCode_Data((ST_ENCODE_INFO *)dev_position_info,SrcBuf,DstBuf,3);
    if(Ret == 4)
    {
        if(EQUAL != LIB_MemCmp((INT8U*)&DevPos+8, DstBuf,Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;

    }
    return TRUE;
}

const INT8U data_update_en_1[]=
{
    structure,0x02,
    octetstring,0x05,0x11,0x22,0x33,0x44,0x55,
    SID_MAC,
    0x12,0x34,0x56,0x78,
    0x03,0x77,0x66,0x55,
    0x04,0x11,0x22,0x33,0x44,

};

const INT8U data_update_org[] = {
0x11,0x22,0x33,0x44,0x55,/*data*/    
0x78,0x56,0x34,0x12,/*SID*/
0x77,0x66,0x55,/*add data*/
0x11,0x22,0x33,0x44,/*MAC*/
};
INT16S DeCode_dataUpdate(void)
{

    INT16S Ret;
    INT16U SrcLen;

    LIB_MemCpy((INT8U*)data_update_en_1, SrcBuf, sizeof(data_update_en_1));
    Ret = DeCode_Data((ST_ENCODE_INFO *)data_updata_info,SrcBuf,DstBuf,0);
    if(Ret == sizeof(data_update_org))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)data_update_org, Ret))
        {
            return -1;
        }
        
    }
    else
    {
        return -1;
    }

    /*测试不输入期望的类型，并返回原始数据长度*/
    Ret = DeCode_Data_Len(NULL,SrcBuf,0,&SrcLen);
    if(Ret == sizeof(data_update_org))
    {
        if(SrcLen == sizeof(data_update_en_1))
        {
            
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    
    Ret = DeCode_Data_Len((ST_ENCODE_INFO *)data_updata_info,SrcBuf,1,NULL);
    if(Ret == 5)
    {

    }
    else
    {
        return -1;
    }

    /*测试不输入期望的类型，并返回原始数据长度*/
    Ret = DeCode_Data_Len(NULL,SrcBuf,1,&SrcLen);
    if(Ret == 5)
    {
        if(SrcLen == (2+5+2))
        {
            
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    

    Ret = DeCode_Data_Len((ST_ENCODE_INFO *)data_updata_info,SrcBuf,2,NULL);
    if(Ret == 11)
    {

    }
    else
    {
        return -1;
    }
    

    return TRUE;

}

const INT8U data_Len_Test_en_1[]=
{
    octetstring,0x06,
    0x11,0x11,0x11,0x11,0x11,0x11,
};

const INT8U data_Len_Test_en_2[]=
{
    SID,
    0x80,0x2C,0x00,0x01,
    0x0B,0x00,0x09,0x40,0x19,0x26,0x8C,0xE5,0x00,0x05,0x00,0x08

};
INT16S DecodeLenTest(void)
{
    INT16S Ret;
    INT16U SrcLen;

    LIB_MemCpy((INT8U*)data_Len_Test_en_1, SrcBuf, sizeof(data_Len_Test_en_1));
    Ret = DeCode_Data_Len(NULL,SrcBuf,0,&SrcLen);
    if(Ret == 6)
    {
        if(SrcLen == sizeof(data_Len_Test_en_1))
        {
            
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    LIB_MemCpy((INT8U*)data_Len_Test_en_2, SrcBuf, sizeof(data_Len_Test_en_2));
    Ret = DeCode_Data_Len(NULL,SrcBuf,0,&SrcLen);
    if(Ret == 15)
    {
        if(SrcLen == sizeof(data_Len_Test_en_2))
        {
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    
    return TRUE;
}

const INT8U DateTimes_en_1[]=
{
    date_time_s,0x07,0xE1,
    12,25,17,59,28
};

const INT8U DateTimes_DeCode[]=
{
    0xE1,0x07,
    12,25,17,59,28
};
INT16S DecodeDateTimeS(void)
{
    INT16S Ret;
    INT16U SrcLen;

    LIB_MemCpy((INT8U*)DateTimes_en_1, SrcBuf, sizeof(DateTimes_en_1));
    Ret = DeCode_Data(NULL,SrcBuf,DstBuf,0);
    if(Ret == 7)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)DateTimes_DeCode, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    Ret = DeCode_Data_Len(NULL,SrcBuf,0,&SrcLen);
    if(Ret == 7)
    {


    }
    else
    {
        return -1;

    }
    return TRUE;

}



const INT8U BillDay_en_1[]=
{
    structure,0x02,unsigned_t,0x02,unsigned_t,0x08,
};


const INT8U BillDay_de_1[]=
{
    0x02,0x08,
};


const INT8U OverCurr_en_1[]=
{
    doublelong,0x00,0x98,0x96,0x80,
};

const INT8U OverCurr_de_1[]=
{
    0x80,0x96,0x98,0x00,
};

const INT8U OverCurr_en_2[]=
{
    unsigned_t,0xA5,
};

const INT8U OverCurr_de_2[]=
{
    0xA5,
};

INT16S DecodeBillDay(void)
{
    INT16S Ret;

    LIB_MemCpy((INT8U*)&BillDay_en_1, SrcBuf, sizeof(BillDay_en_1));

    Ret =DeCode_Data((ST_ENCODE_INFO *)&BillDayInfo,SrcBuf,DstBuf,1);
    if(Ret == 2)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)BillDay_de_1, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
        
    Ret =DeCode_Data((ST_ENCODE_INFO *)&BillDayInfo,SrcBuf,DstBuf,2);
    if(Ret == 2)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)BillDay_de_1, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }


    Ret =DeCode_Data((ST_ENCODE_INFO *)&BillDayInfo,SrcBuf,DstBuf,3);
    if(Ret == 2)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)BillDay_de_1, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    
    LIB_MemCpy((INT8U*)&OverCurr_en_1, SrcBuf, sizeof(OverCurr_en_1));
    Ret =DeCode_Data((ST_ENCODE_INFO *)&OverCurrInfo,SrcBuf,DstBuf,1);
    if(Ret == 4)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)OverCurr_de_1, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    LIB_MemCpy((INT8U*)&OverCurr_en_2, SrcBuf, sizeof(OverCurr_en_2));
    Ret =DeCode_Data((ST_ENCODE_INFO *)&OverCurrInfo,SrcBuf,DstBuf,2);
    if(Ret == 1)
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)OverCurr_de_2, Ret))
        {
            return -1;

        }
    }
    else
    {
        return -1;
    }
    return TRUE;
}


INT16S DecodeCSD(void)
{
    INT16S Ret;
    ST_ENCODE_INFO pInfo;
    INT8U csd_de_1[5];
    INT8U csd_de_2[10];
    INT8U csd_de_3[28];

    ST_OAD TmpOad;
    INT16U Len;

    LIB_MemCpy((INT8U * )&csd_en_1, SrcBuf, sizeof(csd_en_1));
    
    pInfo.Type = CSD;
    Ret = DeCode_Simple(SrcBuf,&pInfo,DstBuf);
    if(Ret ==  sizeof(csd_en_1))
    {
        csd_de_1[0]= CHOICE_OAD;/*choice=0 OAD*/
        TmpOad.Oi.asInt = 0x4023;
        TmpOad.AId = 0x01;
        TmpOad.IId = 0x02;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_1[1], sizeof(ST_OAD));
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)csd_de_1, Ret))
        {
            return -1;
        }
    }
    else
    {
            return -1;

    }
    
    LIB_MemCpy((INT8U * )&csd_en_2, SrcBuf, sizeof(csd_en_2));
    pInfo.Type = CSD;
    Ret = DeCode_Simple(SrcBuf,&pInfo,DstBuf);
    if(Ret ==  sizeof(csd_en_2))
    {
        csd_de_2[0]= CHOICE_ROAD;/*choice=0 OAD*/
        TmpOad.Oi.asInt = 0x4023;
        TmpOad.AId = 0x01;
        TmpOad.IId = 0x02;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_2[1], sizeof(ST_OAD));

        csd_de_2[5] = 0x1;/*OAD NUM */
        TmpOad.Oi.asInt = 0x5012;
        TmpOad.AId = 0x02;
        TmpOad.IId = 0x03;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_2[6], sizeof(ST_OAD));
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U*)csd_de_2, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    LIB_MemCpy((INT8U*)&csd_en_3, SrcBuf, sizeof(csd_en_3));
    Ret =DeCode_Data(NULL,SrcBuf,DstBuf,0);
    if(Ret == 28)
    {
        csd_de_3[0]= CHOICE_ROAD;/*choice=0 OAD*/
        TmpOad.Oi.asInt = 0x5002;
        TmpOad.AId = 0x02;
        TmpOad.IId = 0x01;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_3[1], sizeof(ST_OAD));
        csd_de_3[5] = 0x1;/*OAD NUM */
        TmpOad.Oi.asInt = 0x0010;
        TmpOad.AId = 0x02;
        TmpOad.IId = 0x01;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_2[6], sizeof(ST_OAD));

        csd_de_3[10] = 0x00;
        
        csd_de_3[11]= CHOICE_OAD;/*choice=0 OAD*/
        TmpOad.Oi.asInt = 0x0002;
        TmpOad.AId = 0x02;
        TmpOad.IId = 0x02;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_3[12], sizeof(ST_OAD));

        csd_de_3[16] = 0x01;

        csd_de_3[17]= CHOICE_ROAD;/*choice=0 OAD*/
        TmpOad.Oi.asInt = 0x7002;
        TmpOad.AId = 0x02;
        TmpOad.IId = 0x03;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_3[18], sizeof(ST_OAD));
        csd_de_3[22] = 0x1;/*OAD NUM */
        TmpOad.Oi.asInt = 0x0003;
        TmpOad.AId = 0x02;
        TmpOad.IId = 0x01;
        LIB_MemCpy((INT8U*)&TmpOad, (INT8U*)&csd_de_3[23], sizeof(ST_OAD));

        csd_de_3[27] = 0x02;
    }
    else
    {
        return -1;
    }


    LIB_MemCpy((INT8U*)&csd_en_4, SrcBuf, sizeof(csd_en_4));

    Ret = DeCode_Data_Len(NULL,SrcBuf,0,&Len);
    if( Ret == 6)
    {

    }
    else
    {
        return -1;
    }
    return TRUE;
}

const INT8U BitString_en_1[]=
{
    bitstring,0x08,0x80,
};

const INT8U BitString_De_1[]=
{
    0x01,
};

const ST_ENCODE_INFO AEngyComInfo[]=
{
    {bitstring,0x08},
};

INT16S DecodeBitString(void)
{
    INT16S Ret;


    LIB_MemCpy((INT8U * )&BitString_en_1,(INT8U * )&SrcBuf, sizeof(BitString_en_1));
    Ret = DeCode_Data((ST_ENCODE_INFO *)&AEngyComInfo,SrcBuf,DstBuf,0);
    if(Ret == sizeof(BitString_De_1))
    {
        if(EQUAL != LIB_MemCmp(DstBuf, (INT8U * )&BitString_De_1, Ret))
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }

    return TRUE;
}



INT16S DecodeNull(void)
{
    INT8U NullData;
    INT16U len;
    INT16S Ret;
    
    NullData = null_data;
    
    Ret = DeCode_Data_Len(NULL,&NullData,0,&len);
    if(Ret == 0)
    {
        if(len!=1)
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
    
    return TRUE;
}


INT16S DecodeTest(void)
{
    INT16S Ret;
    
    Ret = DeCodeOad();
    if(Ret<0)
    {
        return -1;

    }

    Ret = DeCodeDslot();
    if(Ret<0)
    {
        return -2;

    }
    Ret = DeCodePosition();
    if(Ret<0)
    {
        return -3;

    }
    Ret = DeCode_dataUpdate();
    if(Ret<0)
    {
        return -4;
    }

    Ret =  DecodeLenTest();
    if(Ret<0)
    {
        return -5;
    }

    Ret =  DecodeDateTimeS();
    if(Ret<0)
    {
        return -6;
    }

    Ret =  DecodeBillDay();
    if(Ret<0)
    {
        return -7;
    }

    Ret =  DecodeCSD();
    if(Ret<0)
    {
        return -8;
    }

    Ret =  DecodeBitString();
    if(Ret<0)
    {
        return -9;
    }


    Ret =  DecodeNull();
    if(Ret<0)
    {

    }
    
    return TRUE;
}
