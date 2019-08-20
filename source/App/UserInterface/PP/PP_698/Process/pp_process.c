/*"*****************************************************************************"*/
/*"  FileName: pp_process.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  协议数据处理文件   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // 历史修改记录"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "App\UserInterface\PP\PP_698\Class\class_comm.h"
#include "APP\message\MS.h"
#include "APP\UserInterface\PP\PP_698\Class\class8_parament.h" 
#include "app\userinterface\ProtocolStack\PS_698\AL_698.h"
#include "App\UserInterface\UI.h"

/*协议处理模块数据变量定义*/
INT8U gServiceChoice;       /*CHOICE格式 用全局变量，以便处理后续帧*/

INT8U *pEndEncodeAddr;   /*应用层给的最终编码长度*/

/*后续帧暂存数据*/
INT8U gBak_data[MAXGETRECORDLISTNUMBER][LEN_NEXT_FRAM_RECORD_DATA];

INT8U Record_Num_List; /*分别用于暂存当前的recordlist和recordlistnum*/       

/*编程OAD*/
ST_OAD Program_Oad[10]; 




/*****************************************************************************
 函 数 名  : PP_PrgEvent
 功能描述  : 记录编程记录
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期六 2018 04 21
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
/*"记录编程记录"*/   
void PP_PrgEvent(ST_OAD Oad)
{
    ST_OAD Tmp_Oad[10];

    /*增加判断是否是自定义的，防止退厂等也记录编程记录*/
    if(Oad.Oi.asInt>=OI_METER_MODE)
    {
        return;
    }     

    /*编程事件的设置与方法，要清掉编程状态*/
    if((OI_PRG_EVENT == Oad.Oi.asInt)&&(8!=Oad.AId))
    {
        Push(E_PRGEVENT_START,FALSE);/*"编程结束"*/
    }      
    
    if(ChkStatus(E_PRGEVENT_START) == FALSE)  /*"编程未开始"*/   
    {
        LIB_MemSet(0xFF, (INT8U *)&Program_Oad[0], 40);        
        LIB_MemCpy((INT8U *)&Oad, (INT8U *)&Program_Oad[0], sizeof(ST_OAD));
        //SaveRcrd(E_PRG_EVNT_RCRD, E_EVTLST_PRG_S);
        //SaveRcrd(E_PRG_EVNT_RCRD, E_EVTLST_PRG_E);
        Push(E_PRGEVENT_START,TRUE);
    }
    else/*"写后续编程事件记录"*/   
    {
        LIB_MemCpy((INT8U *)&Program_Oad[0], (INT8U *)&Tmp_Oad[1], sizeof(ST_OAD)*9);
        LIB_MemCpy((INT8U *)&Oad, (INT8U *)&Tmp_Oad[0], sizeof(ST_OAD));        
        LIB_MemCpy((INT8U *)&Tmp_Oad[0], (INT8U *)&Program_Oad[0], sizeof(ST_OAD)*10); 
        
        //SaveRcrd(E_PRG_EVNT_RCRD, E_EVTLST_PRG_E);
    }
}


/*"编程记录方法"*/   
INT8S PrgEvent_Rcrd(void *pdin, void *pdout)
{
	ST_OAD Oad;
	INT8U *pdata;
    
	pdata = (INT8U *)pdin;	
	LIB_MemCpy(pdata,(INT8U *)&Oad,sizeof(ST_OAD));

	PP_PrgEvent(Oad);

	return DAR_0_OK;
}

/*return:0:ok, 0xff:not ok*/
INT8U check_iid(INT8U ServiceType, ST_OAD Oad)
{
	INT8U  iid;
	INT8S  Ret = DAR_0_OK;
	INT16U OI;
 
	iid      = Oad.IId;
	OI       = Oad.Oi.asInt; 

    if(0 == Oad.AId)
    {
        return DAR_5_OANOADAPT;
    }    

	switch(OI)
	{
	    #if 0 /*在get_oad_object中已判断*/
		case OI_STATUS_METER:   /*运行状态字*/
			if(iid>MAX_IID_2014)
			{
				Ret = DAR_8_OVERFLOW;
			}
			break;
        #endif    
		case OI_PUBLIC_HOLIDAY:        
			/*公共假日，不能直接用0编程*/
			if((0 == iid)&&(EN_SETREQUEST == ServiceType))
			{
				Ret = DAR_5_OANOADAPT;
			}            
			break;
       #if 0     
       /*时区数判断*/     
       case OI_CURRENT_SLOT_TABLE:
       case OI_BAK_SLOT_TABLE:   
            if(MAX_IID_YEALY_SLOT_TABLE<iid)
            {
                Ret = DAR_8_OVERFLOW;
            }    
            break;  
        #endif     
        /*时段数判断*/        
        case OI_BAK_DAILY_SLOT_TABLE:
        case OI_CURRENT_DAILY_SLOT_TABLE:    
            /*电能表时段表编程，一次只能设置一个时段表*/
			if((0 == iid)&&(EN_SETREQUEST == ServiceType))
			{
				Ret = DAR_5_OANOADAPT;
			}  
            
            #if 0 
            if(MAX_IID_DAILY_SLOT_TABLE<iid)
            {
                Ret = DAR_8_OVERFLOW;
            }   
            #endif
            
            break;
       #if 0      
       /*费率数判断*/     
       case OI_CURRENT_TRIFF_PRICE:
       case OI_BAK_TRIFF_PRICE:   
            if(MAX_IID_TRIFF<iid)
            {
                Ret = DAR_8_OVERFLOW;
            }    
            break;  
       #endif     
	}
	
	return Ret;
	
}


/*****************************************************************************
 函 数 名  : Check_Special_OAD
 功能描述  : 检查可以明文操作的OAD，如果是的，则直接返回
 输入参数  : ST_OAD Oad  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年7月10日
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Check_Special_OAD(ST_OAD Oad)
{
    INT8U  AID;
    INT16U OI;

    OI  = Oad.Oi.asInt; 
	AID = Oad.AId;

	/*45000900 信号强度设置*/ /*可以直接用明文确认红外请求和红外查询可以明文*/ 
    if((OI_SIGNAL_GPRS==OI)||((OI_ESAM == OI)&&((0x0B ==AID)||(0x0C ==AID))))  
    {
	    return DAR_0_OK;   
    }	

    /*2017-10-31:放在红外权限控制后面，防止红外没有通过认证后操作 ,2018-06-22这两个明文权限还是放开*/    
    if(((OI_REMOTE==OI)&&(131==AID))||                     /*支持明文密码合闸*/ 
	    ((OI_REPORT_METER==OI)&&(127 ==AID)))              /*上报状态确认*/ 
    {
        return DAR_0_OK;
    } 

    return DAR_FF_OTHERERR;
    
}    

/*****************************************************************************
 函 数 名  : check_object_right
 功能描述  : 检查对象权限
 输入参数  : TypeService:服务类型
             pObject:
             pdata:方法与设置的源数据地址，以便与类型指针比较；读数据时，没有用
 输出参数  : 无
 返 回 值  : DAR_0_OK:Ok,Others:not ok
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期六 2017 12 09
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U check_object_right(INT8U ServiceType,SUB_OBJECT *pObject,INT8U *pData)
{
    INT8U  iid;
    INT16U OI;
    INT8S  Ret;
    
    
    /*2017-11-18 厂内，直接退出*/
    if (TRUE == ChkStatus(E_FAC))
    {
        return DAR_0_OK;
    }  

    if(DAR_0_OK == Check_Special_OAD(pObject->Oad))
    {
        return DAR_0_OK;
    }    

    OI  = pObject->Oad.Oi.asInt;
    iid = pObject->Oad.IId;  

#ifdef PM_MODLE       
    /*-----------------挂起判断-------------------------*/    
    GetSingle(E_HANGUP_FLAG, &F_ESAM_IS_OFF);


    /*2017-08-17:ESAM挂起，不允许操作方法和设置 2017-10-31 这里有没有问题，是所有方法和设置吗?可以明文操作的也不可以?*/
    if((EN_SETREQUEST == ServiceType)||(EN_ACTIONREQUEST == ServiceType))
    {     
        if((OI_REMOTE == OI)||(OI_REMAIN == OI))
        {       
            temp_data[0] = 0; 
            temp_data[1] = 0; 
            SetSingle(E_CTRCMD_STATUS,&temp_data[0],2);       /*1.先清命令执行状态字*/       
            
            if (0!=F_ESAM_IS_OFF)
            {    
                /*2017-11-29 远程控制，置挂起标志*/  
                temp_data[0] = 1;                          
            }              
            
            SetSingle(E_CTRERR_STATUS, &temp_data[0], 2);    /*2.根据是否挂起，置错误状态字*/         
        }
        
        if (0!=F_ESAM_IS_OFF)
        {            
            return DAR_31_HANGUP;
        }	        
    }   
    /*-----------------挂起判断------END-------------------*/    
#endif
    /*电能表不应支持"恢复出厂参数"，返回拒绝读写（ DAR=3）。由于是action，所以不能用读写权限控制*/
    if((4 == pObject->Oad.AId )&&(OI_ELECTRI_DEVICE==OI)&&(EN_ACTIONREQUEST == ServiceType))
    {
        return DAR_3_NOACESS;
    }  

    /*判读写权限，有些参数为只读属性，密码不能读出*/
    if(((EN_SETREQUEST == ServiceType)&&(0==((pObject->Right_RW)& WRITE_ONLY)))||
        ((EN_GETREQUEST == ServiceType)&& (0==((pObject->Right_RW)& READ_ONLY))))/*(OI_CONNECT_PASS==OI)))*/	/*(0==((pObject->Right_RW)& READ_ONLY)))*/
    {
        return DAR_3_NOACESS;
    }    

    /*判断安全模式*/
    Ret = get_oi_right_type(ServiceType,pObject);
    if(DAR_0_OK!=Ret)
    {
        return DAR_20_NO_RIGHT;
    }  

    Ret = check_iid(ServiceType,pObject->Oad);    
    if(DAR_0_OK!=Ret)
    {
        return Ret;
    }    
    
    #if 0    
    /*冻结类不允许设置操作?这里是不是判的不全?*/
	if((d->Class == 9)&&(d->Oad.AId == 4)&&(ACTIONREQUEST !=ServiceType))
	{
		d->Value = VALUEDAR8;
	}
    #endif

    /*ESAM不支持类型指针，所以不判断，一般不会有Pointer为NULL，但为了可靠性，还是增加判断一下，防止有其它指针*/
    if((NULL != pObject->Pointer)&&(OI_ESAM!=OI))
    {
        if((EN_SETREQUEST == ServiceType)||(EN_ACTIONREQUEST == ServiceType))
        {		     
            /*2017-12-09 下面的IID，还要再考虑下*/
            if(EN_ACTIONREQUEST == ServiceType)
            {
                iid = 0;
            }                
                
            if(0>DeCode_Data((ST_ENCODE_INFO *)pObject->Pointer,pData,NULL,iid))
            {
                return DAR_7_TYPENOADAPT;
            }      

            /*增加下地址长度判断*/
            if(((OI_METER_ADDR==OI)||(OI_GROUP_ADDR == OI))&&(MAX_ADDR_LEN!=pData[1]))
            {
                return DAR_5_OANOADAPT;
            }    
            
        }   
    }    
#ifdef PM_MODLE       
     /*厂外，本地表参数，远程表不允许读写*/
	if(Action(E_LOCALOI_CHECK,&OI,NULL) != DAR_0_OK)
    {
        return DAR_6_OANOEXIST;
    }  
#endif    
    return DAR_0_OK; 
}    

/*****************************************************************************
 函 数 名  : Get_Special_Oad_Data
 功能描述  : 一些特殊OAD的数据处理，数据中心没有OBIS
 输入参数  : ST_OAD Oad    
             INT8U *pdata  
 输出参数  : 无
 返 回 值  : DAR_0_OK: 特殊处理了，直接返回数据， DAR_FF_OTHERERR:需通过OBIS来获取数据
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 01 16
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Get_Special_Oad_Data(ST_OAD Oad,INT8U *pdata)
{
    INT8U  iid,AId,i;   
    INT16U Oi;    
    #ifdef THREE_PHASE
    INT16U UiState;
    #endif    
    INT8U  Ret = DAR_0_OK;
    INT8U  Temp;
    INT32U TmpShowVersion;
    
    Oi  = Oad.Oi.asInt;
    iid = Oad.IId;
    AId = Oad.AId;
    
    switch ( Oi )
    {
        case OI_ELECTRI_DEVICE:
            if(5 != AId)    /*子设备列表*/
            { 
                Ret = DAR_FF_OTHERERR;
            }  
            else
            {
                *pdata = 0; /*无子设备列表*/     
            }    
            break;    
        case OI_DATE_TIME:
            if(3 == AId)
            {
                *pdata = 0;  /*主站授时（0），*/
            }  
            else
            {
                Ret = DAR_FF_OTHERERR;
            }    
            break;            
        case OI_CLOCK_SOURCE:
            Temp = ChkStatus(E_CLK_ERR);  /*状态  enum{可用（0），不可用（1）}*/
            
            *pdata = EN_CLR_SRC_RTC;    /*时钟源固定为时钟芯片*/
            
            if(0 == iid)
            {
                *(pdata+1) = Temp;
            }
            else if(2 == iid)
            {
                *pdata = Temp;
            }  
            
            break;  
        case OI_METER_NUMBER:  
            /*计量元件数单相表为1，三相三线表为2，三相四线表为3。*/
            #ifdef THREE_PHASE
            *pdata = EN_METER_NUM_3_4_1;   
            GetSingle(E_MU_STATUS, (INT8U *)&UiState);
            if(UiState&0x01)   /*"bit值为0表示3P4W，为1表示3P3W"*/
            {
               *pdata = EN_METER_NUM_3_3_1;    
            }
            #else
            *pdata = EN_METER_NUM_SINGLE;    
            #endif
            break;
        case OI_DISP_VERSION:
            TmpShowVersion = 0;
            for(i = 0; i < 8; i++) 
            {
                TmpShowVersion |= ((INT32U)(ShowVersion[i] - 0x30) << (7 - i) * 4);
            }
 
            LIB_MemCpy((INT8U *)&TmpShowVersion, pdata, sizeof(INT32U));

            break;
#ifdef PM_MODLE               
        case OI_REMAIN:
            if(2 == AId)
            {
                Action(E_PMRUNSTA_FRESH,NULL,NULL);/*"刷新预付费运行状态字"*/
                *pdata = ChkStatus(E_RELAY_PROTECT);
            } 
            else
            {
                Ret = DAR_FF_OTHERERR;
            }    
            break;
#endif            
        case OI_METER_POWER_DOWN_EVENT:
            if(6 == AId)
            {    
                *pdata = 60;    /*固定为60*/
            }
            else
            {
               Ret = DAR_FF_OTHERERR;
            }    
            break;
        default: 
            Ret = DAR_FF_OTHERERR;
            break;
    }   

    return Ret;
}    
/*****************************************************************************
 函 数 名  : Get_Special_Class_Data
 功能描述  : 特殊类数据处理
 输入参数  : INT8U *pdata  
             ST_OAD Oad    
             INT8U *pRet    
 输出参数  : 无
 返 回 值  : INT8U* 编码后的地址 如果不等于pdata，则表示有编码
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2018年6月21日
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Get_Special_Class_Data(INT8U *pdata,SUB_OBJECT *pObject,INT8U *pRet)
{
    INT8U AId;
    INT8U *pdst = pdata;
    ST_OAD Oad;   
    INT8U  Class = 0;   
    
    Class = pObject->Class;
    LIB_MemCpy((INT8U *)&pObject->Oad,(INT8U *)&Oad,sizeof(ST_OAD));   /*重新再COPY一次，在Get_OAD_Data_len()中，object.Oad可能被改*/ 

    AId = Oad.AId;  
    
    /*1.逻辑名*/
    if(1 == AId)
    {        
        pdst = EncodeAtt1LogicName(&Oad,pdst);        
    } 
    /*2.换算单位*/
    else if(((3 == AId)&&(Class<=6))||((5 == AId)&&(Class<=1))) /*换算单位*/
    {
        pdst = Encode_Scale_Unit(&Oad,pdst,pObject->element);        
    } 
    else if(17 == Class)                                        /*17类由于CSD列表比较特殊，需要特殊处理*/
    {
        pdst = get_class17_disp_data(Oad,pdst,pRet);        
    }   
    else if(20 == Class)
    {
        pdst = get_class20_connect_data(Oad,pdst,pRet);        
    }  
    else if(7 == Class)
    {
        if(7 == AId)                      /*当前值记录表*/
        {
            pdst = Get_Class7_Att7(pdst,Oad);   
        }    
        if(10 == AId)                     /*时间状态记录表*/
        {
            pdst = Get_Class7_Att10(pdst,Oad);     
        }    
    }   
    else if(24 == Class)
    {
        if(3 == AId)                                /*当前记录数*/
        {
            pdst = Get_Class24_Att3(pdst,Oad);
        } 
        else if(10 == AId)                          /*当前值记录表*/
        {
            pdst =Get_Class24_Att10(pdst,Oad);
        }        
        #ifdef THREE_PHASE
        else if(13 == AId)
        {
            pdst =Get_Class24_Att13(pdst,Oad);        /*要getlist格式转换，所以特殊处理*/
        }    
        #endif
        else if(14 == AId)                          /*时间状态记录表*/
        {
            pdst =Get_Class24_Att14(pdst,Oad);          
        } 
    }    
    else if ((OI_SAFE_MODE == Oad.Oi.asInt)&&(3 == AId))  /*安全模式变长处理*/
    {
        pdst = Get_safe_mode(Oad,pdst);        
    } 

    return pdst;
    
}    


/*****************************************************************************
 函 数 名  : SetSingle
 功能描述  : 将SET函数的单独设置，封装成一个函数，以方便使用
 输入参数  : INT16U OBIS   
             INT8U *pData  
             INT8U len     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 01 31
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U SetSingle(INT16U OBIS,INT8U *pData,INT8U len)
{
    return  (0 -Set(&OBIS,1,pData,&len));
}

/*****************************************************************************
 函 数 名  : Set_Object_Single
 功能描述  : 设置某个OAD的数据
 输入参数  : ST_OAD Oad    
             INT8U *pdata:指向类型的数据
 输出参数  : 无
 返 回 值  : 设置DAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2017 12 04
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Set_Object_Single(SUB_OBJECT *pObject,INT8U *pdata,INT16U len_data)
{
    INT8U num_obis,num_all_obis,iid;    
    INT8U Ret,CLASS,AID;
    ST_PARAMENT_OAD_OBIS_INFO Obis_Info[MAX_OBIS_NUM_ONE_OAD];
    ST_FMT_ATTR Format_list[MAX_OBIS_NUM_ONE_OAD];
    const ST_FUNCTION *pFunc;
    
    ST_FMT_ATTR Format_Block;  /*存储格式*/

    INT16U Obis[MAX_OBIS_NUM_ONE_OAD];
    
    INT8U temp_data_2[MAX_DATA_LEN_SET/2];
    
    INT8U len,temp_len;
    
    /*-------*/
    num_obis = PP_OadToObis_Info(pObject,Obis_Info);
    iid      = pObject->Oad.IId;
    num_all_obis = PP_GetOadToObisList(iid,num_obis,Obis,Obis_Info,Format_list);   
    if(num_all_obis>MASK_BLOCK_DATA)
    {
        num_all_obis = 1;
    }    
    
    /*2017-12-12:这里增加写前函数*/
    /*增加写前函数*/
    //2018-03-14:正式程序用这个代替 if((FUN_NULL <Obis_Info[0].FunctionId)&&(MAX_FUN_ID >Obis_Info[0].FunctionId))

    if(FUN_NULL !=Obis_Info[0].FunctionId)
    {
        pFunc = (const ST_FUNCTION *)&stFunction[Obis_Info[0].FunctionId];
        if(pFunc->OnWrite_Beford != 0) 
        {            
            Ret = pFunc->OnWrite_Beford(pdata);
            if(DAR_0_OK != Ret)
            {
                return (Ret);
            }

            /*设置时间，直接在写前函数中完成*/
            if((OI_DATE_TIME ==  pObject->Oad.Oi.asInt)&&(2 ==pObject->Oad.AId))
            {
                return DAR_0_OK;
            }    
        }   
    }     
    

    /*2017-12-12:这里增加格式转换   */ /*2017-12-16:这里的LEN_DATA，估计要底层不判断才行，或者是判总长度*/
    len =  len_data;

    /*只有单数据，且数据格式需要转换的才转*/
    if((1 == num_all_obis)&&(E_NONE_FORMAT!=Format_list[0].format))
    {
        Ret = GetFrmt(Obis[0],&Format_Block);
        if(0xff!=Ret)
        {  
            temp_len = FRMT_NdataChange(pdata,temp_data_2,Format_list[0],Format_Block,(INT16U)len_data);
            if(0 != temp_len)
            {  
                LIB_MemCpy(temp_data_2, pdata, temp_len);
                len = temp_len;
            }  
        }
    }    
#ifdef PM_MODLE   
    /*特殊处理，设置阶梯电价时，需采用E_STEP_TABLE2对象设置*/
	if(Obis[0] == E_SETPVAULE_PASBLK)
	{
		num_all_obis = 1;
		Obis[0] = E_STEP_TABLE2;
	}
#endif
    Ret = (0- Set(Obis,num_all_obis,pdata,&len));

    /*2018-03-01:设置关联对象，清发生时间和累计时间，防止ACTION嵌套*/
    if(DAR_0_OK == Ret)
    {
        CLASS = pObject->Class;
        AID   = pObject->Oad.AId;
        
        if(((EN_CLS7_ATT_3_RELATION_OBJECT_TABLE == AID)&&(7 == CLASS))||           
           ((EN_CLS24_ATT_2_RELATION_OBJECT_TABLE == AID)&&(24 == CLASS)))
        {
           Action(E_MNT_EVNT_CLR,&Obis[0],NULL); 
        } 
    }    
    
    return Ret; 
    
}   

/*****************************************************************************
 函 数 名  : get_oad_object
 功能描述  : 根据OAD，获取元素内容，并检查AID和IID大小，以便确定是否合法OAD
 输入参数  : ST_OAD Oad           
             SUB_OBJECT *pObject  
 输出参数  : 无
 返 回 值  : dar:一般是未定义，也可能是时区数，时段数超
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 12 07
    作    者   : xyt
    修改内容   : 新生成函数

注:入口参数，pObject->class一定初始化为0
*****************************************************************************/
INT8U get_oad_object(SUB_OBJECT *pObject)
{
    INT8U ret;
    INT8U Oi0,Oi1,tmp_oi0;  

    /*先赋值下，防止编码出错*/
    pObject->Pointer   = NULL;
    pObject->Right_RW  = READ_ONLY;  /*先初始化下读写权限，否则可能有的地方(如读记录表)，导致该值为0了*/
#ifdef PM_MODLE   
    /*先判断是否本地表数据*/
    if(Action(E_LOCALOI_CHECK,&pObject->Oad.Oi.asInt,NULL) != DAR_0_OK)
    {
        return DAR_6_OANOEXIST;
    }    
#endif
    ret = get_special_class_object(pObject);
    if(DAR_0_OK == ret)   
    {
        return DAR_0_OK;
    }    

    /*找到了，但AID或IID不对*/
    if(0!=pObject->Class)   /*或者是用(DAR_8_OVERFLOW == ret)，不能用*/
    {
        return ret;
    }       

    Oi0 = pObject->Oad.Oi.asByte[OIH];

    Oi1 = pObject->Oad.Oi.asByte[OIL];
    tmp_oi0 = Oi0 & 0xf0;

    switch(tmp_oi0)
    {
        case 0x00:
             ret = get_class1_6_object(pObject,1); /*电量  */
             break;
             
#ifdef THREE_PHASE
       case 0x10:
            ret = get_class1_6_object(pObject,2); ;/*最大需量  */
            break;
#endif
       case 0x20: 
            ret = Get_Variable_Object(pObject);    /*变量  */
            if(DAR_4_OANODEF == ret)
            {
                ret = get_class8_object(pObject); /*参变量  */
            }
            break;
            
      case 0x30:
            if(0x33 ==Oi0)  
            {
                ret = get_class8_object(pObject); /*参变量  */
            }
            else
            {
                ret = get_class7_object(pObject); /*7类事件  */
                if(DAR_4_OANODEF == ret)
                {
                    ret = get_class24_object(pObject); /*24类事件  */
                }    
            }
            break;
            
        case 0x40:
            ret = Get_Variable_Object(pObject);    /*变量  */
            if(DAR_4_OANODEF == ret)
            {
                 ret = get_class8_object(pObject); /*参变量 */
            }
            break;    
        case 0x50:
            ret = get_class9_freeze_object(pObject);/*冻结  */
            break;
            
        case 0x80:         
            ret = get_class8_object(pObject);     /*参变量  */
            break;
            
        case 0xf0:
             /* 安全模式参数 本来可以直接赋值，为了省代码，及表格变化时不变化，所以还是用函数*/
             if(((0xf1 == Oi0)&&(0x01 == Oi1))||(0xff == Oi0))  
             {
                 ret = get_class8_object(pObject); /*参变量  */
             }
             else if(0xf2 == Oi0)
             {
                 ret = get_class22_input_output_object(pObject);   /*输入输出设备*/                   
             }
            break;
            
        default:
            ret = DAR_4_OANODEF;
            break;
    }
    
    return ret;   

}    

/*记录数据只能通过get record*/
INT8U check_event_freeze_get_record(SUB_OBJECT *pObject)
{   
    INT8U AID,CLASS,i;
    INT16U OI;
    const INT16U EVENT_OI[]={0x2026,0x2027,0x2029};  /*记录OI，不能直接读取*/

    CLASS = pObject->Class;

    AID   = pObject->Oad.AId;

    if(((2 == AID)&&(7 == CLASS))||
       ((2 == AID)&&(9 == CLASS))||
       ((24 == CLASS)&&(6<= AID)&&(9 >= AID)))
    {
        return DAR_5_OANOADAPT;
    } 

    OI = pObject->Oad.Oi.asInt;

    for(i=0;i<ARRAY_SIZE(EVENT_OI);i++)
    {
        if(OI==EVENT_OI[i])
        {
            return DAR_5_OANOADAPT;
        }    
    }    
    

    return DAR_0_OK;
    
}  

/*****************************************************************************
 函 数 名  : Check_Object
 功能描述  : 检查对象是否定义，或权限和安全模式是否够
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 03 13
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Check_Object(INT8U ServiceType,SUB_OBJECT *pObject,INT8U *pData)
{
    INT8U ret;
    
    ret = get_oad_object(pObject);   /*确定是否有定义*/

    if(DAR_0_OK != ret)
    {
        return ret;
    }   

    ret = check_object_right(ServiceType,pObject,pData);

    return ret;    
     
}


/*****************************************************************************
 函 数 名  : get_single_normal_process
 功能描述  : 单独获取某个OAD的数据，并打包
 输入参数  : ST_OAD Oad  
             INT8U *ret  返回处理结果，0:正常，非0，超过最大编码长度，要用后续帧处理
             INT8U *pdata 编码启始地址
 输出参数  : 无
 返 回 值  : INT8U* 编码后的地址长度
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2017 12 08
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *get_single_normal_process(ST_OAD Oad,INT8U *pdata,INT8U *is_need_next_frame)
{
    INT8U ret = DAR_0_OK;
    SUB_OBJECT object;
    INT16U len;
    INT8U *pdst = pdata;

    *is_need_next_frame = 0;   
    
    LIB_MemCpy((INT8U * )&Oad, (INT8U *) &CurOad, sizeof(ST_OAD));
    
    /*2.获取实际数据*/
    if(DAR_0_OK == gDarDecode) /*优先以解码格式作为结论*/
    {      
        /*获取长度的同时，会判断OAD的合法性，如是否定义等*/
        len = Get_OAD_Data_len(Oad,&object,&ret);

        if(DAR_0_OK == ret)
        {    
            ret = check_event_freeze_get_record(&object);
        }

        

        if(DAR_0_OK == ret)
        {    
            if(pdst+len+sizeof(ST_OAD)>pEndEncodeAddr)
            {
               *is_need_next_frame = 0xff;
               return pdst;
            }
            
            LIB_MemCpy((INT8U *)&Oad,(INT8U *)&object.Oad ,sizeof(ST_OAD));   /*重新再COPY一次，在Get_OAD_Data_len()中，object.Oad可能被改*/             
        }        
    }   
    else
    {
        ret = gDarDecode;
    }  

    /*--------------*/
    if(DAR_0_OK != ret)
    {   
       /*打包DAR也要判断长度是否越界*/
       if(pdst+LEN_GET_DAR>pEndEncodeAddr)
       {
           *is_need_next_frame = 0xff;
           return pdst;
       }       
    
       pdst = EncodeGetDar(&Oad,ret,pdst);
    }  

    return pdst;

}    


/*****************************************************************************
 函 数 名  : get_normal_process
 功能描述  : get normal类服务数据获取
 输入参数  : INT8U *pdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2017 12 08
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *get_normal_process(INT8U *pdata)
{
    INT8U i,j;
    INT8U is_need_next_frame;//= 0;
    ST_Next_Frame_Normal_Data *pGet_Normal_Data;
    INT8U *pdst = pdata;
        
    pGet_Normal_Data = (ST_Next_Frame_Normal_Data *)gBak_data;

    i= pGet_Normal_Data->CurrentLine;

    for(;i<pGet_Normal_Data->TotalLine;i++)
    {      
        pdst=get_single_normal_process(pGet_Normal_Data->oadlist[i],pdst,&is_need_next_frame);

        if(0!=is_need_next_frame)
        {
            #if APP_NEXT_FRAME_NO_ACCESS
            pdst = pdata; 
            /*所有LIST都改为拒绝读写*/
            for(j=0;j<pGet_Normal_Data->TotalLine;j++)
            {    
                pdst = EncodeGetDar(&pGet_Normal_Data->oadlist[i], DAR_3_NOACESS, pdst);
            }
            #endif
            
            return pdst;  /*后续看要不要打包成拒绝读写*/
        }          

        pGet_Normal_Data->CurrentLine = i; /*更新，以便下次从新的OAD开始读数*/  
        
    }

    return pdst;  
}    
/*****************************************************************************
 函 数 名  : Get_Service_Process
 功能描述  : 读取类服务处理流程
 输入参数  : INT8U *pdata:编码地址
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 12 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Get_Service_Process(INT8U *pdata)
{    
    INT8U choice;
    INT8U *pdst;

    choice = gServiceChoice;

    if((EN_GET_RECORD == choice)||(EN_GET_RECORD_LIST == choice))
    {
        pdst = get_record_process(pdata);
    }    
    else
    {
        pdst = get_normal_process(pdata);
    }   

    return pdst;
}  


/*****************************************************************************
 函 数 名  : Encode_SetThenGet_List_Dar
 功能描述  : 打包SetThenGetListDAR，纯粹DAR，没有数据
 输入参数  : INT8U *pSetResult  
             INT8U *pdata       
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 03 15
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U * Encode_SetThenGet_List_Dar(INT8U *pSetResult,INT8U *pdata)   
{
    INT8U *pdst = pdata;
    INT8U i; 
    ST_SET_THEN_GET_SERVICE *pSet_Then_Data;

    pSet_Then_Data  = (ST_SET_THEN_GET_SERVICE *)gBak_data;
    
    for(i=0;i<pSet_Then_Data->ListNumber;i++)
    {      
        pdst = EncodeSetDar(&pSet_Then_Data->SetOad[i], pSetResult[i], pdst);  
        pdst = EncodeGetDar(&pSet_Then_Data->GetOad[i],DAR_3_NOACESS,pdst);
    }    

     return pdst;
}    
  
/*****************************************************************************
 函 数 名  : Set_Service_Process
 功能描述  : 设置类服务处理流程
 输入参数  : INT8U *pdata :编码地址 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 12 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Set_Service_Process(INT8U *pdata)
{
    INT8U i;
    ST_OAD Oad;
    ST_SET_SERVICE *pSet_Data;
    ST_SET_THEN_GET_SERVICE *pSet_Then_Data;
    INT8U *pdst = pdata; 
    INT8U is_need_next_frame;
    
    INT8U SetResult[MAXSETLISTNUMBER];
    INT8U choice;

    choice = gServiceChoice;

    switch ( choice )
    {
        case EN_SET_NORMAL :           
        case EN_SET_NORMAL_LIST :
            pSet_Data  = (ST_SET_SERVICE *)gBak_data;
            for(i=0;i<pSet_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pSet_Data->SetOad[i], (INT8U *)&Oad, sizeof(ST_OAD));

               if(DAR_0_OK!=gDarDecode)
               {
                  SetResult[i] = gDarDecode;
               }

               /*暂时不判是否缓冲越界,因为一般不会越界，设置数据肯定大些。*/
               pdst = EncodeSetDar(&Oad, SetResult[i], pdst);               
            }
            break;
            
        case EN_SET_THEN_GET_NORMAL_LIST :
            pSet_Then_Data  = (ST_SET_THEN_GET_SERVICE *)gBak_data;
            /*1.设置全部LIST数据*/
            for(i=0;i<pSet_Then_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pSet_Then_Data->SetOad[i], (INT8U *)&Oad, sizeof(ST_OAD));
               if(DAR_0_OK!=gDarDecode)
               {
                  SetResult[i] = gDarDecode;
               }
            }

            is_need_next_frame = 0;  /*先置下0，虽然在get_single_normal_process中也有置*/

            for(i=0;i<pSet_Then_Data->ListNumber;i++)
            {
               /*2.分开打包SET DAR*/ 
                /*省代码暂时不判是否缓冲越界*/
               pdst = EncodeSetDar(&pSet_Then_Data->SetOad[i], SetResult[i], pdst);  
               
               /*3.GET数据并打包*/              
               pdst = get_single_normal_process(pSet_Then_Data->GetOad[i],pdst,&is_need_next_frame);

               /*get的判一下，这样可以在判缓冲越界时，预留5个字节*/  
               if((0!=is_need_next_frame) ||(DAR_0_OK!=gDarDecode))
               {        
                  pdst = Encode_SetThenGet_List_Dar(SetResult,pdata);  /*如果后续帧处理，则这行不用*/  
                  
                  return pdst;  /*后面看要不要改成打包成拒绝读写*/
               } 
            }                       
            break;
        default:
             
            break;
    } 

    return pdst;
}  


/*****************************************************************************
 函 数 名  : get_oad_action_object
 功能描述  : 获取方法类的对象内容
 输入参数  : ST_OAD Oad           
             SUB_OBJECT *pObject  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2017 12 08
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U get_oad_action_object(ST_OAD Oad,ACTION_OBJECT *pObject)
{
    INT8U i;
    INT8U AID,OI_NUM;
    INT16U OI;
    ST_OI_METHOD *pOiMethod;
 
    OI  = Oad.Oi.asInt;
    AID = Oad.AId;
    //IID = Oad.IId;

    /*1.先找AID*/
    for(i=0;i<ARRAY_SIZE(OI_METHOD_NUM_POINTER_ALL);i++)
    {
        if(AID == OI_METHOD_NUM_POINTER_ALL[i].AID)
        {
            OI_NUM    = OI_METHOD_NUM_POINTER_ALL[i].OI_NUM;
            pOiMethod = (ST_OI_METHOD *)OI_METHOD_NUM_POINTER_ALL[i].Pointer;
            break;
        }    
    }   

    /*AID没有找到*/
    if(ARRAY_SIZE(OI_METHOD_NUM_POINTER_ALL)<=i)
    {
        return DAR_4_OANODEF;
    }    

    /*2.查找OI*/
    for(i=0;i<OI_NUM;i++)
    {
        if(OI == pOiMethod->OI)
        {
           pObject->Pointer = pOiMethod->Pointer;
           pObject->ObjMeth = pOiMethod->ObjMeth;
           return DAR_0_OK;
        }    
        pOiMethod++;
    }  

    return DAR_4_OANODEF;
    
}    


/*****************************************************************************
 函 数 名  : Init_Money_Packet
 功能描述  : 钱包初始化
 输入参数  : INT8U len     
             INT8U *pdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 12 28
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
    

/*****************************************************************************
 函 数 名  : Clr_Event_Process
 功能描述  : 事件清0，改为分开ACTION操作
 输入参数  : INT16U Obis  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 12 28
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Clr_Event_Process(INT16U Obis)
{
    INT8S SRet;
    
    /*厂外 私钥下不能清电表清0事件*/
    if((OI_MTR_CLR_EVENT == CurOad.Oi.asInt)&&(FALSE == ChkStatus(E_FAC)))
    {
        return DAR_15_PSWERR;
    } 
    
    /*1.清累计次数*/
    SRet = Action(E_RCRD_CLR,&Obis,NULL); 
    if(0!=SRet)
    {
        return (0-SRet);
    }  
#ifdef PM_MODLE   
	if(Obis == E_CARD_IN_ERR_EVNT_RCRD)/*"异常插卡事件需清非法卡次数"*/
	{
		unlawcnt = 0;
		SetSingle(E_UNLAW_PLUG_CNT,(INT8U *)&unlawcnt,4);
	}
#endif
    /*2.清事件发生标志和累计时间*/
    SRet = Action(E_MNT_EVNT_CLR,&Obis,NULL); 

    /*3.清编程事件和记录事件清0*/
    if(0==SRet)
    {   
        /*清编程事件，清编程标志*/
        if((OI_PRG_EVENT == CurOad.Oi.asInt)||(CLR_ALL_EVENT_OBIS == Obis))
        {    
            /*清编程标志*/
            Push(E_PRGEVENT_START,FALSE);
        }
        
        SaveRcrd(E_EVNT_CLR_EVNT_RCRD, E_EVTLST_EVNT_CLR);

    }  
	
    return (0-SRet);

}


/*****************************************************************************
 函 数 名  : Clr_Meter_Process
 功能描述  : 电表清0，改为函数操作，分开调ACTION
 输入参数  : INT16U Obis  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2017 12 28
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Clr_Meter_Process(void)
{   
    /*本地表不允许电表清0*/
	

#ifdef PM_MODLE    
    GetSingle(E_METER_MODE, (INT8U *)&MeterType); 

    if((LOCALMETER == (MeterType&METERTPYE))&&(FALSE == ChkStatus(E_FAC)))
    {
        return DAR_3_NOACESS;
    }   
#endif    
    /*判断条件 END*/
    
	CLR_MTR();
    
    /*清编程标志*/
	Push(E_PRGEVENT_START,FALSE);

    return DAR_0_OK;
}



/*****************************************************************************
 函 数 名  : Check_Relation_Oad_Ok
 功能描述  : 判断关联对象OAD的合 法性
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 01 29
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U Check_Relation_Oad_Ok(INT8U *pdata,ST_OAD Oad,INT16U len)
{
    INT8U  Oi0,i;
    ST_OAD TmpOad;
    SUB_OBJECT object;
    INT8U ret = DAR_0_OK;

	/*通过SET服务设置关联对象属性表时，OAD的属性内元素索引必须为00，否则返回拒绝读写。
      虽然操作类的也走这里，但方法类操作没有IID不等于0的，所以可以这样判*/
    if(0!=Oad.IId)
    {
       return DAR_3_NOACESS; 
    }    

    /*私钥下，不允许设置电表清0关联对象*/
    if((TRUE ==ChkStatus(E_PRIVATE))&&(OI_MTR_CLR_EVENT == Oad.Oi.asInt)) 
    {
        return DAR_3_NOACESS; 
    }    
        
    Oi0 = Oad.Oi.asByte[OIH];
    
    /*冻结*/
    if(0x50 == Oi0)
    {
        for(i=0;i<len/8;i++)
        {
            LIB_MemCpy(pdata+i*8+2, (INT8U*)&TmpOad, sizeof(ST_OAD));
            Get_OAD_Data_len(TmpOad,&object,&ret);  /*之所以不直接用get_oad_object，是因为有个mask要与*/
            if(DAR_0_OK !=ret)
            {
                return DAR_3_NOACESS;
            }    
        }    
    }
    else /*事件*/
    {
        for(i=0;i<len/4;i++)
        {
            LIB_MemCpy(pdata+i*4, (INT8U *)&TmpOad, sizeof(ST_OAD));
            Get_OAD_Data_len(TmpOad,&object,&ret);
            if(DAR_0_OK !=ret)
            {
                return DAR_3_NOACESS;
            }  
        } 
    }    
    
    return DAR_0_OK;
}    


/*****************************************************************************
 函 数 名  : Pp_Esam_Action
 功能描述  : ESAM类方法特殊处理，由于ESAM可能有返回值，或设置，所以进行特殊处理
 输入参数  : ST_OAD Oad       
             INT8U *psrcdata  
             INT8U *pdstdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期六 2017 12 23
    作    者   : xyt
    修改内容   : 新生成函数

注:这个函数是直接返回的编码后的数据，不是返回数据结果
*****************************************************************************/
INT8U* Pp_Esam_Action(ST_OAD Oad,INT8U *psrcdata,INT8U *pdstdata)
{
    INT8S Ret;
    ACTION_OBJECT ActionObject;
    INT8U  DataAction[MAX_DATA_LEN_ACTION];
    
    
    INT8S SRet;
    INT8U *pdst = pdstdata;
    
    //ST_OAD Oad_Tmp;    
    
    /*1.查找是否有定义，并获取数据类型和数据中心方法ID*/
    Ret = get_oad_action_object(Oad,&ActionObject);   /*确定是否有定义*/

    if(DAR_0_OK != Ret)
    {
        pdst = EncodeAcionDar(&Oad, Ret, pdst);   
        return pdst;
    }  

    SRet = Action(ActionObject.ObjMeth,psrcdata,DataAction);
    
    if(0>SRet)
    {
        pdst = EncodeAcionDar(&Oad, (0-SRet), pdst);        
    }    
    else
    {          
#ifdef PM_MODLE            
      OBIS_ESAM = ActionObject.ObjMeth;
        /*ESAM设置*/
        if(E_ESAM_SET == OBIS_ESAM)
        {
           Len_data    = DataAction[0];
           pActionData = &DataAction[1];

           LIB_MemCpy((INT8U *)pActionData, (INT8U *)&object.Oad, sizeof(ST_OAD));

           object.Class = 0;                /*先置为0，以便后面判断*/
    
           Ret_1 = get_oad_object(&object);   /*确定是否有定义*/

           if(DAR_0_OK == Ret_1)
           {
               Ret_1 = Set_Object_Single(&object,pActionData+sizeof(ST_OAD),(INT16U)Len_data);
               if(DAR_0_OK == Ret_1)
               { 
                    PP_PrgEvent(object.Oad);
               }     
           }   
           /*这里要不要打包VALUE为Ret_1*/
  		   pdst = EncodeAcionDar(&Oad, DAR_0_OK, pdst);        
        }   
        else
        {      
			if(E_ESAM_GET == OBIS_ESAM)
			{
			   LIB_MemCpy((INT8U *)&DataAction[0],(INT8U *)&Len_data,2);
			   pActionData = &DataAction[2]; 
			}	
			else
			{
			   Len_data = (INT16U)SRet;
			   pActionData = &DataAction[0];  
			}  

			if(Len_data >0)
			{
				pdst  = EnCodeOAD(&Oad,pdst);/*OAD*/
				*pdst++  = DAR_0_OK;
				*pdst++  = 1;				 /*Data  OPTIONAL*/

				LIB_MemCpy(pActionData, pdst, Len_data);
				pdst += Len_data;	 
			}
			else
			{
				pdst = EncodeAcionDar(&Oad, DAR_0_OK, pdst);  /*"返回数据长度为0，无DATA"*/
			}  
        }    
#endif		        
    }   

    return pdst;
   
}    


/*****************************************************************************
 函 数 名  : Pp_CodeCmp
 功能描述  : 文件比对，和ESAM一样，要打包数据，特殊处理
 输入参数  : INT8U *pdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 01 22
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U * Pp_CodeCmp(ST_OAD Oad,INT8U *psrcdata,INT8U *pdstdata)
{
    INT16S Len_data;
    INT8U  DataAction[MAX_CODE_CMP_LEN];
    INT8U  Ret = DAR_0_OK;
    INT8U *pdst = pdstdata;
    INT16U length;

    /*比较数据合法性*/
	Len_data = DeCode_Data((ST_ENCODE_INFO *)ST_5_U8_U8_U32_U32_U16,psrcdata,DataAction,0/*Oad.IId*/);
	if(0>Len_data)
	{
	    Ret  = DAR_5_OANOADAPT;
	} 
    else
    {   

    }

    if(DAR_0_OK!=Ret)
    {    
        pdst = EncodeAcionDar(&Oad, Ret, pdst); 
    } 
    else
    {
		pdst  = EnCodeOAD(&Oad,pdst);/*OAD*/
		*pdst++  = DAR_0_OK;
		*pdst++  = 1;				 /*Data  OPTIONAL*/

	    if(0 == Oad.IId)
	    {
            *pdst++  = structure;
            *pdst++  = 1;
	    }
        
        *pdst++ = octetstring;
			
		length  = (((INT16U)DataAction[1])<<8)+DataAction[0];	

        pdst    += EnCode_Base(&DataAction[2],octetstring,length,pdst);    
        
    }        
    
    return pdst;
    
}    


/*****************************************************************************
 函 数 名  : Encode_ActionThenGet_List_Dar
 功能描述  : 打包ActionThenGetListDAR，纯粹DAR，没有数据
 输入参数  : INT8U *pSetResult  
             INT8U *pdata       
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 03 15
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U * Encode_ActionThenGet_List_Dar(INT8U *pSetResult,INT8U *pdata)   
{
    INT8U *pdst = pdata;
    INT8U i; 
    ST_ACTION_THEN_GET_SERVICE *pAction_Then_Data;

    pAction_Then_Data  = (ST_ACTION_THEN_GET_SERVICE *)gBak_data;
    
    for(i=0;i<pAction_Then_Data->ListNumber;i++)
    {      
        pdst = EncodeAcionDar((ST_OAD*)&pAction_Then_Data->ActionOmd[i], pSetResult[i], pdst);  
        pdst = EncodeGetDar(&pAction_Then_Data->GetOad[i],DAR_3_NOACESS,pdst);
    }    

     return pdst;
}  
/*****************************************************************************
 函 数 名  : Acion_Service_Process
 功能描述  : 操作方法类服务处理流程
 输入参数  : INT8U *pdata:编码地址
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 12 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Acion_Service_Process(INT8U *pdata)
{
    INT8U i,AId;
    ST_OAD Oad;
    ST_ACTION_SERVICE *pAction_Data;
    ST_ACTION_THEN_GET_SERVICE *pAction_Then_Data;
    INT8U *pdst = pdata;
    INT8U is_need_next_frame;
    
    INT8U ActionResult[MAXACTIONLISTNUMBER];
    INT8U choice;
    INT8U *PointerAction;
    INT8U DataAddr_EsamGet[MAXACTIONLISTNUMBER][MAXESAMGETDATALEN];

    choice = gServiceChoice;

    switch ( choice )
    {
        case EN_ACTION_NORMAL :           
        case EN_ACTION_NORMAL_LIST :
            pAction_Data  = (ST_ACTION_SERVICE *)gBak_data;

            /*2018-03-13 ESAM get先保存POINTER，否则可能被冲掉*/
            for(i=0;i<pAction_Data->ListNumber;i++) 
            {
                LIB_MemCpy((INT8U *)&pAction_Data->ActionOmd[i], (INT8U *)&Oad, sizeof(ST_OAD));
                if((OI_ESAM == Oad.Oi.asInt)&&(EN_METHOD_3_GET_ESAM==Oad.AId))  
                {
                   LIB_MemCpy(pAction_Data->Pointer[i], DataAddr_EsamGet[i], MAXESAMGETDATALEN);
                }    
            }
            /*ESAM get先保存POINTER，否则可能被冲掉  end*/
            
            for(i=0;i<pAction_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pAction_Data->ActionOmd[i], (INT8U *)&Oad, sizeof(ST_OAD));
               AId = Oad.AId;
               if(DAR_0_OK!=gDarDecode)
               {
                   ActionResult[i] = gDarDecode;
               }
               else
               {                     
                   PointerAction = pAction_Data->Pointer[i];                   
                  
                   if((OI_ESAM == Oad.Oi.asInt)&&(EN_METHOD_10_INIT_PACKET_ESAM!=AId))
                   {
                       if(EN_METHOD_3_GET_ESAM == AId)   /*ESAM get*/
                       {
                           PointerAction = DataAddr_EsamGet[i];
                       }     
                       
                        /*ESAM类方法,直接返回的编码后地址*/ 
                       pdst = Pp_Esam_Action(Oad,PointerAction,pdst);
                       continue;                       
                   }    
                   else if ((OI_FILE == Oad.Oi.asInt)&&(10==AId))
                   {                       
                       pdst = Pp_CodeCmp(Oad,PointerAction,pdst);
                       continue;
                   }
               
               } 
               
               /*暂时不判是否缓冲越界*/
               pdst = EncodeAcionDar(&Oad, ActionResult[i], pdst);               
            }
            break;
            
        case EN_ACTION_THEN_GET_NORMAL_LIST :
            pAction_Then_Data  = (ST_ACTION_THEN_GET_SERVICE *)gBak_data;
            for(i=0;i<pAction_Then_Data->ListNumber;i++)
            {    
               LIB_MemCpy((INT8U *)&pAction_Then_Data->ActionOmd[i], (INT8U *)&Oad, sizeof(ST_OAD));
               if(DAR_0_OK!=gDarDecode)
               {
                  ActionResult[i] = gDarDecode;
               }
            }

            is_need_next_frame = 0;  /*先清下0*/

            for(i=0;i<pAction_Then_Data->ListNumber;i++)
            {
                /*省代码暂时不判是否缓冲越界*/
               pdst = EncodeAcionDar(&Oad, ActionResult[i], pdst);  
               
               pdst = get_single_normal_process(pAction_Then_Data->GetOad[i],pdst,&is_need_next_frame);

               /*get的判一下，这样可以在判缓冲越界时，预留6个字节*/  
               if((0!=is_need_next_frame) ||(DAR_0_OK!=gDarDecode))
               {
                  pdst = Encode_ActionThenGet_List_Dar(ActionResult,pdata);  /*如果后续帧处理，则这行不用*/                  

                  return pdst;  
               } 
            }                       
            break;
        default:
             
            break;
    }   

    return pdst;
}  


/*****************************************************************************
 函 数 名  : Process_698
 功能描述  : 698函数处理
 输入参数  : INT8U servicetype  
             INT8 *pdata        
 输出参数  : 无
 返 回 值  : 返回编码地址，以后后面打包时间标签与跟随上报内容
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2017 12 06
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *Process_698(INT8U servicetype,INT8U *pdata)
{
    INT8U *pFlowReport = pdata;   /*还是初始化下吧，防止异常了*/
    
    switch(servicetype)
    {
        case EN_GETREQUEST:
            pFlowReport = Get_Service_Process(pdata);
            break;
        case EN_SETREQUEST:
            pFlowReport = Set_Service_Process(pdata);
            break;
        case EN_ACTIONREQUEST:
            pFlowReport = Acion_Service_Process(pdata);
            break;
        default:
            break;
    }

    return pFlowReport;
} 

#ifdef DM_DLT698
#if 0
const ST_ROAD_SHOW_INFO mstROadShowInfo[] = 
{
    	{0x0000, E_INFO_LAST_TRIFF_A_ENGY},   /*上*月  费率 有功     电量*/
    	{0x0010, E_INFO_LAST_TRIFF_PA_ENGY},  /*上*月  费率 正向有功 电量*/
    	{0x0020, E_INFO_LAST_TRIFF_NA_ENGY},  /*上*月  费率 反向有功 电量*/
    	#ifdef THREE_PHASE
    	{0x0030, E_INFO_LAST_COMM_TRIFF_R_1_ENGY},/*"上*月组合费率 无功 1  电量"*/
        {0x0040, E_INFO_LAST_COMM_TRIFF_R_2_ENGY},/*"上*月组合费率 无功 2  电量"*/
        {0x0050, E_INFO_LAST_COMM_TRIFF_R_I_ENGY},  /*"上N月费率 I象限无功 电量"*/
        {0x0060, E_INFO_LAST_COMM_TRIFF_R_II_ENGY}, /*"上N月费率 II象限无功 电量"*/
        {0x0070, E_INFO_LAST_COMM_TRIFF_R_III_ENGY},/*"上N月费率 III象限无功 电量"*/
        {0x0080, E_INFO_LAST_COMM_TRIFF_R_IV_ENGY}, /*"上N月费率 IV象限无功 电量"*/        
        #endif
        {0x202C, E_INFO_REMAIN_MONEY},        /*剩余 金额*/
    	{0x2031, E_INFO_LAST_SUM_ENGY},       /*上*月  总用 电量 */
    	{0x2032, E_INFO_LAST_STEP_SUM_ENGY},  /*上 阶梯 总用电量*/
    	//{0x201E, 0x16},/*"事件发生时间"*/
};
/*特殊ROAD的提示信息*/
static INT8U GetROadShowInfoId(UN_OI *pOi)
{
    INT8U i;

    for(i = 0; i < ARRAY_SIZE(mstROadShowInfo); i++)
    {
        if(pOi->asInt == mstROadShowInfo[i].Oi.asInt)
        {
            return (mstROadShowInfo[i].ShowinfoId);
        }
    }
    return (E_INFO_NULL); /*"无提示信息"*/
} 
#endif

/*****************************************************************************
 函 数 名  : PP_GetOadToObisShow
 功能描述  : 根据IID获取对应的OBIS列表，以及格式列表
 输入参数  : INT8U IID                            
             INT16U *pObis                        
             ST_PARAMENT_OAD_OBIS_INFO *pObis_Info  
             ST_FMT_ATTR *pFormat                 
 输出参数  : 无
 返 回 值  : OBIS个数
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 12
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U PP_GetOadToObisShow(INT8U IID,INT8U num_obis,INT16U *pObis,ST_PARAMENT_OAD_OBIS_INFO *pObis_Info,INT8U *format_id,INT8U *ShowInfoId,INT8U *comm_id)
{
    INT8U i,j,temp_iid,num_ms_obis,Tmp_num_obis,ShowinfoId;
    const ST_FORMAT_ID *pObisFmt;    
    INT16U temp_Obis;
    INT8U f_block_data = 0;
    INT8U Offset_Format;
    
    //num_ms_obis = pObis_Info->num;
    
    temp_iid   = 1;      
    
    for(i=0;i<num_obis;i++)
    {
        temp_Obis   = pObis_Info[i].Obis;
        Tmp_num_obis = pObis_Info[i].num;
        
        if(Tmp_num_obis>E_SEASON_SLOT_ID)
        {
           temp_Obis++;     /*块数据，OBIS要加1*/
           
           Fix_Len_Varible = 0xff;   /*PP_GetOadToObisList()中根据ComPort获取数据格式*/
           num_ms_obis     = LenGet_PP_698((ENUM_DATALEN_ID)Tmp_num_obis);   
           Fix_Len_Varible = 0; 
           
           f_block_data = 0xff;
        } 
        else
        {
            num_ms_obis = Tmp_num_obis;
        }    
        
        for(j=0;j<num_ms_obis;j++)        
        {                                
            if(temp_iid==IID)                
            {
                *pObis = temp_Obis;    
                
                /*下面内容打包成一个函数，可用于显示与传输用*/    
                /*指针容下错，以便只取OBIS，不取格式*/    
                /*FormatId容下错*/ 
                if(0!=f_block_data)
                {
                   Offset_Format = 0;    /*2018-03-05:Offset_Format是不是应该就取i就可以了,为什么特意改?主要是为了防止越界*/
                }
                else
                {    
                   Offset_Format = i;                       
                }
                                            
                if(MAX_FORMAT_ID>pObis_Info[Offset_Format].FormatId)
                {    
                    pObisFmt = (const ST_FORMAT_ID *)&stFormatId[pObis_Info[Offset_Format].FormatId];
                    
                    *format_id = pObisFmt->ShowId;  
                    *comm_id   = pObisFmt->TransId;
                }                
                else
                {
                    *format_id = MAX_FORMAT_ID;
                    *comm_id   = MAX_FORMAT_ID;                
                }   

                ShowinfoId = pObis_Info[Offset_Format].ShowinfoId; 
                #ifdef THREE_PHASE
                if(((E_PHASE_VARIBLE_ID == Tmp_num_obis)||(E_PHASE_VARIBLE_SUM_ID == Tmp_num_obis))&&(2<=IID))  
                {
                   *ShowInfoId = ShowinfoId + IID-1; 
                }                
                else
                #endif    
                {
                    *ShowInfoId = ShowinfoId;
                }    
                
                return 1;
            }    

            /*自动加，因为是连续值*/  
            temp_Obis++;
            
            temp_iid++;
            
       } 
        
    }    

     return 0;//temp_iid;    
}    

#ifdef THREE_PHASE
const  INT8U demand_disp_format[]   ={E_FORMAT_UBCD_3_KW_DATUO,E_FORMAT_YYMMDD_NONE_0,E_FORMAT_HHMMSS_NONE_0};
const  INT8U r_demand_disp_format[] ={E_FORMAT_BCD_3_KVAR_DATUO,E_FORMAT_YYMMDD_NONE_0,E_FORMAT_HHMMSS_NONE_0};
const  INT8U volt_stat_disp_format[]={E_FORMAT_UBCD_4_NONE_0,E_FORMAT_UBCD_3_NONE_2,E_FORMAT_UBCD_3_NONE_2,E_FORMAT_UBCD_4_NONE_0,E_FORMAT_UBCD_4_NONE_0};
const  INT8U volt_stat_len[]        ={0,4,2,2,4,4};
const  INT8U volt_stat_Comm_Format[]={E_FORMAT_U_4_NONE_0,E_FORMAT_U_2_NONE_2,E_FORMAT_U_2_NONE_2,E_FORMAT_U_4_NONE_0,E_FORMAT_U_4_NONE_0};
#endif

const ST_DISP_FORTMAT_SRCNO disp_format_list[]=
{
    {E_FORMAT_YYMMDD_NONE_0,E_FORMAT_HHMMSS_NONE_0},        /*FORMAT_DATETIME*/
    {E_FORMAT_ID_CODE_2_NONE_0,E_FORMAT_ID_CODE_4_NONE_0},  /*FORMAT_ADDR_H*/
    {E_FORMAT_U_H_INT_KWH_0,E_FORMAT_U_H_DECIMAL_KWH_MATUO},
    {E_FORMAT_S_H_INT_KWH_0,E_FORMAT_S_H_DECIMAL_KWH_MATUO},
#ifdef THREE_PHASE  
    {E_FORMAT_U_H_INT_KVARH_0,E_FORMAT_U_H_DECIMAL_KVARH_MATUO},
    {E_FORMAT_S_H_INT_KVARH_0,E_FORMAT_S_H_DECIMAL_KVARH_MATUO},
#endif  
};    


/*****************************************************************************
 函 数 名  : check_src_no_format
 功能描述  : 根据屏序号，处理特殊OAD的显示内容
 输入参数  : INT8U scrNo      
             INT8U format_id  
 输出参数  : 无
 返 回 值  : 屏对应的格式
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2017 12 26
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U check_src_no_format(INT8U scrNo,INT8U format_id)
{
    INT8U i;
        
    for(i=0;i<ARRAY_SIZE(disp_format_list);i++)
    {
        if(disp_format_list[i].format_1 == format_id)
        {               
            return *(&disp_format_list[i].format_1 + scrNo-1);
        }    
    }    

    return format_id;
}  


/*****************************************************************************
 函 数 名  : PP_Get_Special_Show_Data
 功能描述  : 检查是否一些特殊的显示数据，以便格式转换或数据切取
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 04 25
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void PP_Check_Special_Show_Data(SUB_OBJECT *pobject,INT8U scrNo,INT8U *pformat_id,INT8U *pData_len,INT8U *pdata)
{
    INT8U AId,ClsId;
    INT16U Duration_OBIS = NULL;
    #ifdef THREE_PHASE
    INT16U Record_Obis;
    INT8U TmpData[32],offset = 0,i;
    #endif
    ST_OAD Oad;
    

    LIB_MemCpy((INT8U *)&pobject->Oad, (INT8U *)&Oad, sizeof(ST_OAD));

    AId  = Oad.AId;
    ClsId= pobject->Class;

    #ifdef THREE_PHASE
    /*电压合格率*/
    if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
    {                  
        *pformat_id = volt_stat_disp_format[scrNo-1];
        Duration_OBIS = E_DA_VOLTHEALTH + ((Oad.Oi.asInt-OI_VOLT_STAT_A_RT)<<1) + (Oad.IId-1);
        GetSingle(Duration_OBIS,TmpData);  
        for(i=0;i<scrNo;i++)
        {    
            offset += volt_stat_len[i];
        }
        LIB_MemCpy(TmpData+offset, pdata, volt_stat_len[scrNo]);  
        *pData_len = volt_stat_len[scrNo];
        return;
    }    
    
    #endif
    
    

    /*时间状态记录表 最近一次发生时间和结束时间*/
    if(((24 == ClsId)&&(0x0E==AId))||((7 == ClsId)&&(0x0A==AId)))
    { 
        *pData_len = 6;
        if((3 == scrNo)||(4 == scrNo))
        {
            LIB_MemCpy(&pdata[6], &pdata[0], 6); 
            if(4 == scrNo)
            {
               *pformat_id = E_FORMAT_HHMMSS_NONE_0;    
            }    
        }    
    }  
    /*当前值记录表 第二屏累计时间*/ /*24类属性10或7类属性7*/
    else if((2 == scrNo)&&
            (((24 == ClsId)&&(0x0A==AId))||((7 == ClsId)&&(0x07==AId))))
    {
        *pData_len = 4; 
         #ifdef SINGLE_PHASE
         if(24 == ClsId)
         {            
            Duration_OBIS = E_OVER_CURR_LA_DURATION;                            
         }
         else
         {
            Duration_OBIS = E_PWR_DOWN_DURATION;
         }  
         #else
         if(24 == ClsId)
         {                     
            Oad.AId = EN_CLS24_ATT_6_EVENT_RECORD_1_TABLE+Oad.IId-1;                     
         }
         else
         {
            Oad.AId = EN_CLS7_ATT_2_EVENT_RECORD_TABLE;
         }   

         Oad.IId = 0;   /*要清成0，否则PP_OADTOOBIS可能不对*/
         
         PP_OadToObis(&Oad, &Record_Obis, NULL, NULL);
            
         Duration_OBIS = Get_Duration_OBIS(Record_Obis);
         #endif
         
         if(NULL == Duration_OBIS)
         {
             LIB_MemSet(0, pdata, sizeof(INT32U));
         } 
         else
         {
             GetSingle(Duration_OBIS,pdata);   
         }      
    }  
}    



/*****************************************************************************
 函 数 名  : PP_Set_Show_Char_Info
 功能描述  : 根据OAD和ROAD点亮液晶显示信息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 05 14
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
#ifdef THREE_PHASE
const INT8U CHAR_INFO_ABC_MASK[]={0x40,0x80,0x08};
#endif
void PP_Set_Show_Char_Info(ST_DISP_OBJ *pdata_in,INT8U *pschar)
{
    INT16U Oi;
    #ifdef THREE_PHASE
    INT8U OIB2;
    INT8U AId,IID;
    AId = pdata_in->Oad.AId;
    IID = pdata_in->Oad.IId;
    #endif
    
    Oi  = pdata_in->Oad.Oi.asInt;
    
    /*特殊处理下，记录型数据都不显示当前*/
    if(0!=pdata_in->releOad.AId)
    {
        pschar[0] &= ~(0x10); 
    }    
    
    /*点亮上月*/
    if((OI_BILL_FRZ==Oi)||(OI_MONTH_FRZ==Oi))       
    {
       pschar[0] |= 0x22;
    } 

    #ifdef THREE_PHASE
    /*分相电能显示ABC*/
    if(OI_NAP_ENGY_LC>=Oi)
    {
        OIB2 = Oi&0x000F;
        if(EN_POWER_OIB2_SUM != OIB2)
        {
            pschar[0] |= CHAR_INFO_ABC_MASK[OIB2 -EN_POWER_OIB2_A];     
        }         
    }    
    
     /*ABC相失压/流总次数*/ /*ABC相失压/流总累计时间*/
     if(((3 == AId)||(10 == AId))&&((OI_LOST_VOLT_EVENT == Oi)||(OI_LOST_CURR_EVENT == Oi)))
     {
        if(1!=IID)
        {    
            if(OI_LOST_VOLT_EVENT == Oi)
            {
               pschar[3] |= 0x2C;        /*总失压*/
            }   
            
             
            if(OI_LOST_CURR_EVENT == Oi)
            {
               pschar[3] |= 0x28;        /*总失*/
               pschar[4] |= 0x40;        /*流*/
            }  

            if((10 == AId)&&(2 == pdata_in->scrNo))
            {
               pschar[4] |= 0x0c;       /*时间*/ 
            }  

            pschar[0] |= CHAR_INFO_ABC_MASK[IID -2];     
        }
     }   
     
    
    /*7类事件*/
    if((OI_MEASURE_CHIP_ERR_EVENT>=Oi)&&(OI_OVER_PA_DEMAND_EVENT<=Oi)&&
        (EN_CLS7_ATT_2_EVENT_RECORD_TABLE==AId))
    {
       pschar[0] |= 0x20;   /*上*/ 
    }
        
    
    /*点亮上，ABC 失压等 */
    if((OI_OVER_PWR_EVENT>=Oi)&&(OI_LOST_VOLT_EVENT<=Oi)&&
        (EN_CLS24_ATT_6_EVENT_RECORD_1_TABLE<=AId)&&
        (EN_CLS24_ATT_9_EVENT_RECORD_4_TABLE>=AId))
    {
        pschar[0] |= 0x20;   /*上*/

        /*点亮ABC*/
        pschar[0] |= CHAR_INFO_ABC_MASK[AId-7];        

        /*失压事件，点亮失压*/
        if(OI_LOST_VOLT_EVENT == Oi)
        {
           pschar[3] |= 0x0C; 
        }   
        
        /*失流事件，点亮失流*/
        if(OI_LOST_CURR_EVENT == Oi)
        {
           pschar[3] |= 0x08;        /*失*/
           pschar[4] |= 0x40;        /*流*/
        }    

        
    }   
    #endif
}    

/*显示数据格式转换函数声明*/
/*"code size  284byte"*/
INT8U PP_Data_Transfer_Show(const ST_FMT_ATTR FAR_POINTER*Communal_Save, INT8U *Sbuff,
		                 ST_SHOWDATA_OUTPUT *Dbuff,const ST_FMT_ATTR FAR_POINTER*Communal_Show,
		                 const ST_FMT_ATTR FAR_POINTER*Communal_Trans, INT8U dataLen)
{
    ST_FMT_ATTR saveFormat;
    ST_FMT_ATTR showFormat;
	INT8U len;
	INT8U *Data_buff;
	INT8U engyPoint;
	INT8U pwrPoint;
	
	Data_buff = (INT8U *)&Dbuff->showDataBuff;
	saveFormat = *Communal_Save;
	
       GetSingle(E_ENGY_SHOW_POINT, &engyPoint);
       GetSingle(E_PWR_SHOW_POINT, &pwrPoint);

	if(Communal_Show->exp == E_MATUO)
	{
		if(engyPoint < 5)
		{
		    showFormat.exp = (INT8S)(0 - engyPoint);
		}
		else
		{
		    showFormat.exp = -2;
		}
	}	        		
	else if(Communal_Show->exp == E_DATUO)
	{
		if(pwrPoint < 5)
		{
		    showFormat.exp = (INT8S)(0 - pwrPoint);
		}
		else
		{
		    showFormat.exp = -4;
		}
	}
	else
	{
	    showFormat.exp = Communal_Show->exp;
	}
	
	/*"没有显示格式时候使用通信格式"*/
	if(Communal_Show->format == 0) 
	{
	    showFormat.format = Communal_Trans->format;
	    showFormat.unit = Communal_Trans->unit;
	}
	else
	{
		showFormat.format = Communal_Show->format;
		showFormat.unit = Communal_Show->unit;
	}	

	len = FRMT_sizeofFormat(showFormat.format);

	if(len <= MAX_SHOWDATA)/*"显示数据接收缓存区越界检查"*/
    {

        len = FRMT_DataChange(Sbuff, Data_buff, saveFormat, showFormat);

        if((showFormat.format > E_TIME_FORMAT) && (showFormat.format < E_STRING))/*"time format"*/
        {/*"时间格式转BCD码显示"*/
            LIB_CharToBcdNByte(Data_buff, len);
        }

        Dbuff->stShowTable.stShowFormat = showFormat;
    }
	else
	{
	    len = 0;
	}

	return len;	
}
/*"*****************************************************************************"*/
/*"  Function:       PP_ShowGetData"*/
/*"  Description:    显示读数据接口函数，显示模块通过调用该接口函数获取数据标识对应数据"*/
/*"  Calls:          "*/
/*"  Called By:      显示功能模块"*/
/*"  Input:          pdata_in   显示数据标识及叶子号                 "*/
/*"                  "*/
/*"  Output:         pdata_out  显示数据内容及提示信息、数据格式"*/
/*"  "*/
/*"  Return:         0  取数不成功    len 取数数据长度，取数成功"*/
/*"  Others:         code size  164byte"*/
/*"*****************************************************************************"*/
const INT8U MAX_STEP_PRICE_SRC_NO[]={6,7,TARIFF_NUM};
const INT32U BAND_DATA_COMM[]={0x300,0x600,0x1200,0x2400,0x4800,0x7200,0x9600,0x19200,0x38400};  /*BCD形式显示*/
INT8U PP_ShowGetData (ST_DISP_OBJ *pdata_in,ST_SHOWDATA_OUTPUT *pdata_out)	            
{
    INT8U num_obis,len,data_len = 0,ShowInfoId;   /*"显示配置表ID"*/
    INT16S SRet;
    ST_FMT_ATTR Format_show;   /*显示格式*/
    ST_FMT_ATTR Format_Block;  /*存储格式*/
    ST_FMT_ATTR Format_comm;   /*通信格式*/
    
    INT16U Obis[MAX_ID_LIST_NUM],Oi;
    INT8U temp_data[MAX_ONE_OAD_DATA_LEN];
    
    ST_OAD Oad;        /*OAD*/
    INT8U AId;         /*"属性标识"*/
   
    INT8U  scrNo;      /*"屏序号"*/
    SUB_OBJECT object;
    ST_PARAMENT_OAD_OBIS_INFO Obis_Info[MAX_OBIS_NUM_ONE_OAD];
    INT8U format_id;       /*显示格式ID*/
    INT8U Comm_Format_id;  /*通信格式ID*/

    #ifdef THREE_PHASE
    INT8U i,offset = 0;
    INT8U temp_data2[8];   /*本来最大四个字节，容余四个字节*/
    #endif

    /*注意:修改OBIS列表的显示格式后，要同步修改对应的OBIS，否则存储格式不对的话，可能格式转换出错，导致显示不出来*/
    
    scrNo   = pdata_in->scrNo;

    AId = pdata_in->Oad.AId;
    Oi  = pdata_in->Oad.Oi.asInt;
    
    /*容下错*/
    if((0 == AId)||(0xff == AId))
    {
        return 0;
    }  
    /*24类，大部份没有总*/
    if((OI_OVER_PWR_EVENT>=Oi)&&(OI_LOST_VOLT_EVENT<=Oi)&&((3 == AId)||(10 == AId)||(14 == AId)))
    {
        if((OI_PWR_REVERSE_EVENT!=Oi)&&(1>=pdata_in->Oad.IId))
        {
            return 0;
        }    
    }    


    /*2017-01-30:固定先清前面16字节，防止是ASCII码或切换时间为FF时，后面是乱码*/
    LIB_MemSet(0, temp_data, 16);
    
    /*记录型数据*/
    if(0!=pdata_in->releOad.AId)
    {
        //LIB_MemCpy((INT8U*)&pdata_in->Oad, (INT8U*)&Road, sizeof(ST_OAD));
        data_len = PP_RecordColumnFilterDisp(&pdata_in->Oad,&pdata_in->releOad,temp_data);
        if(0 == data_len)
        {   
            LIB_MemSet(0, temp_data, MAX_SHOWDATA);
            data_len = MAX_SHOWDATA;
            //return MAX_SHOWDATA;   /*先暂时写为0*/
        }

        /*先COPY，求格式*/
        LIB_MemCpy((INT8U*)&pdata_in->releOad, (INT8U*)&Oad, sizeof(ST_OAD));
        Oad.AId = Oad.AId & MASK_AID_SELECT;    /*要与掉，才能得到正确格式*/            
    }    
    else
    {
        LIB_MemCpy((INT8U*)&pdata_in->Oad, (INT8U*)&Oad, sizeof(ST_OAD));
    }

    if(0 == Oad.IId)
    {
        Oad.IId = 1;
    }    
    
    object.Class = 0;                    /*先置为0，以便后面判断*/

    LIB_MemCpy((INT8U*)&Oad,(INT8U *)&object.Oad ,sizeof(ST_OAD));

    /*确定是否有定义,同时获取元素位置*/
    if(DAR_0_OK != get_oad_object(&object))   
    {
        return 0;
    }           
    
    num_obis = PP_OadToObis_Info(&object,Obis_Info);

    if(0 ==PP_GetOadToObisShow(Oad.IId,num_obis,Obis,Obis_Info,&format_id,&ShowInfoId,&Comm_Format_id))
    {
        return 0;
    }    

    /*看下是否是特殊显示的数据*/
    #ifdef THREE_PHASE
    /*电压合格率*/
    if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
    {
        if((0==scrNo)||(5<scrNo))
        {
            scrNo = 1;
        }   
    }
    #endif

    /*记录型数据，在上面已取数*/
    if(0==pdata_in->releOad.AId)  /*普通数据*/
    {   
        /*阶梯表的OBIS要特殊处理下*/
        if((OI_CURRNET_STEP_PRICE == Oad.Oi.asInt)||
           (OI_BAK_STEP_PRICE == Oad.Oi.asInt))
        {       
           /*判一下屏序号合法性*/
           if(scrNo>MAX_STEP_PRICE_SRC_NO[Oad.IId-1])
           {
             return 0;   
           }  
           else if(0 == scrNo)   /*屏序号有没有可能为0?*/
           {
              scrNo = 1;  
           }             
           
           Obis[0] += scrNo;            
        } 
        
        SRet = GetSingle(Obis[0],temp_data);            
        if(0>SRet)
        {
            return 0;
        }   
        else
        {        
            /*低功耗下，瞬时变量数据显示为0*/
            if(TRUE == ChkStatus(E_LOW_PWR))
            {    
                if((3 == object.Class)||(4 == object.Class))
                {
                    LIB_MemSet(0, temp_data, 16);
                } 
            }
            
            data_len = (INT8U)SRet;  

            /*波特率显示修改*/
            #ifdef THREE_PHASE
            if((E_RS485_1_BAUDRATE == Obis[0])||(E_IR_BAUDRATE == Obis[0])||
               (E_RS485_1_BAUDRATE == Obis[0])||(E_RS485_2_BAUDRATE == Obis[0]))
            #else
            if((E_RS485_1_BAUDRATE == Obis[0]))
            #endif
            {
               if(temp_data[0]<=6)
               { 
                   *(INT16U *)temp_data = BAND_DATA_COMM[temp_data[0]]; 
                   data_len = 4;
               }
               else if(temp_data[0]<=8)
               {
                   *(INT32U *)temp_data = BAND_DATA_COMM[temp_data[0]]; 
                   data_len = 5; 
               } 
            }   
            
            
            PP_Check_Special_Show_Data(&object,scrNo,&format_id,&data_len,temp_data);
            
            
        }    
    }    
    else
    {
        #ifdef THREE_PHASE
        /*电压合格率*/
        if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
        {                  
            format_id = volt_stat_disp_format[scrNo-1];
            for(i=0;i<scrNo;i++)
            {    
                offset += volt_stat_len[i];
            }
            
            data_len = volt_stat_len[scrNo];
            
            LIB_MemCpy(temp_data+offset, temp_data2, data_len);  
            LIB_MemCpy(temp_data2, temp_data, data_len);            /*再COPY回来*/ 
        }    
        
        #endif
    }    

    /*第二屏可能需要计算格式*/
    if(2==scrNo)
    {    
        format_id = check_src_no_format(scrNo,format_id);            
    }  

    #ifdef THREE_PHASE
    /*需量显示格式转换*/
    if(2 == object.Class)
    {        
        if((3 < scrNo)||(0 == scrNo))
        {
            scrNo = 1;
        }  
        if(Oad.Oi.asByte[OIL]<0x30)
        {    
            format_id  =  demand_disp_format[scrNo-1];
        }
        else
        {    
            format_id  =  r_demand_disp_format[scrNo-1];
        }

        if(2<=scrNo)
        {
            ShowInfoId = ShowInfoId+1;    
        }    
    } 
    #endif    

    if(MAX_FORMAT_ID!=format_id)
    {        
         LIB_MemCpy((INT8U *)&stFormat[format_id], (INT8U *)(&Format_show), sizeof(ST_FMT_ATTR));
         LIB_MemCpy((INT8U *)&stFormat[Comm_Format_id], (INT8U *)(&Format_comm), sizeof(ST_FMT_ATTR));

         #ifdef THREE_PHASE
         /*电压合格率*/
         if((OI_VOLT_STAT_A_RT<=Oad.Oi.asInt)&&(OI_VOLT_STAT_C_RT>=Oad.Oi.asInt))
         {
             LIB_MemCpy((INT8U *)&stFormat[volt_stat_Comm_Format[scrNo-1]], (INT8U *)(&Format_Block), sizeof(ST_FMT_ATTR));
         }
         else
         #endif         
         GetFrmt(Obis[0],&Format_Block);

          //len = FRMT_NdataChange(temp_data,pdata_out->showDataBuff,Format_Block,Format_list,(INT16U)SRet);
         if((pdata_in->Oad.Oi.asInt >= 0x4001) && (pdata_in->Oad.Oi.asInt <= 0x4003))/*"通信地址、表号、 客户编号"*/
         {
             LIB_RvsSelf(temp_data, data_len);
         }

         len = PP_Data_Transfer_Show(&Format_Block,temp_data,pdata_out,&Format_show,&Format_comm,data_len);
          
    }
    else
    {   
        len = data_len;
        LIB_MemCpy(temp_data,pdata_out->showDataBuff,len);
        LIB_MemCpy((INT8U *)&stFormat[E_FORMAT_NONE_6], (INT8U*)&pdata_out->stShowTable.stShowFormat, sizeof(ST_FMT_ATTR));
    }   


    /*"输出显示配置信息,显示格式信息在Data_Transfer_Show中已经处理"*/
    LIB_MemCpy((INT8U *)&stShowinfo[ShowInfoId].schar[0],
                 (INT8U *)&pdata_out->stShowTable.schar[0], MAX_SCHAR_NUM); 

    /*-----------------------------*/
    PP_Set_Show_Char_Info(pdata_in,pdata_out->stShowTable.schar);   
    
    return len;
}    

#endif


/*"*****************************************************************************"*/
/*"  Function:       SetSystemTime"*/
/*"  Description:    设置日期写处理函数，对下发日期数据进行合法性检查，并进行校时事件记录"*/
/*"  Calls:          SaveRcrd，action"*/
/*"  Called By:      Set_Leaf_Data"*/
/*"  Input:          pdata 下发日期数据地址指针  "*/
/*"  Output:         无"*/
/*"  "*/
/*"  Return:         TRUE 日期设置不成功  FALSE 日期设置成功"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/ 
INT8U Pp_SetSystemTime(INT8U *pTime)
{
    if(TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)pTime))/*"data valid check"*/
    {
        /*"因为马上要记录校时事件，因此这里需及时更新系统时钟"*/
        SaveRcrd(E_JUST_TIME_EVNT_RCRD, E_EVTLST_JUSTTIME_S);

        Action(E_LRM_JUSTIME,pTime,NULL);   /*校时,暂时没有判写时间是否成功*/

        SaveRcrd(E_JUST_TIME_EVNT_RCRD, E_EVTLST_JUSTTIME_E);   
            
        return DAR_0_OK; 
       
    }
    else
    {
        return DAR_9_DBNOVAILD;
    }
   
}


/*****************************************************************************
 函 数 名  : BeforeFunSetSystemTime
 功能描述  : 设置时间写前函数实现
 输入参数  : INT8U *pdata  
 输出参数  : 无
 返 回 值  : DAR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 02 05
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U BeforeFunSetSystemTime(INT8U *pdata)
{
    INT8U rsult;
    ST_6TIME_FORM  tempT[2];  /*:校时前时间，1:校时后时间*/
    ST_7TIME_FORM TmpstSysTime;

    GetSingle(E_SYS_TIME, (INT8U *)&TmpstSysTime);
    
    LIB_MemCpy((INT8U *)&TmpstSysTime, (INT8U *) &tempT[0], sizeof(ST_6TIME_FORM));
    
    rsult = Pp_SetSystemTime(pdata);
    if(DAR_0_OK == rsult)
    {
        LIB_MemCpy((INT8U *)pdata, (INT8U *)&tempT[1], sizeof(ST_6TIME_FORM));
#ifdef PM_MODLE        
        Action(E_TRSTEP_SWCHECK, NULL, NULL);
#endif
        Action(E_RCRD_TIME_ADJUST, (INT8U *) &tempT[0], NULL);
        
        #ifdef THREE_PHASE
        Action(E_CURRENT_DEMAND_CLR, NULL, NULL);
      	//Action(E_VOLTHEALTH_ADJUST_TIME, (INT8U *) &tempT[0], NULL);	
        #endif
    }

    return rsult;
    
}     

#ifdef THREE_PHASE
/*****************************************************************************
 函 数 名  : BeforeFunSetDuPara
 功能描述  : 需量参数设置写前函数
 输入参数  : INT8U *pdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 10
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U BeforeFunSetDuPara(INT8U *pdata)
{
    INT8U rsult,mode_value,demand_value,DataTmp;
    
    DataTmp = *pdata;

    if((0 == DataTmp)||(DataTmp>60))
    {
        return DAR_FF_OTHERERR;
    }    
    
    if(OI_DEMAND_PERIOD == CurOad.Oi.asInt)
    {
        GetSingle(E_DEMAND_SLID_PERIOD, &demand_value);        
        mode_value = DataTmp%demand_value;
    }
    else
    {
        GetSingle(E_DEMAND_PERIOD, &demand_value);
        mode_value = demand_value%DataTmp;      
    }    

    if(mode_value)
    {    
        rsult = DAR_FF_OTHERERR;
    }
    else
    {
        rsult = DAR_0_OK;    
    }    

    return rsult;
}    
#endif

