/*"*****************************************************************************"*/
/*"  FileName: class22_input_output.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  输入输出接口类文件   "*/
/*"  Version:         "*/
/*"  V1.0       初始版本"*/
/*"  "*/
/*"  Function List:   "*/
/*"  History:         // 历史修改记录"*/
/*"  code size     table size  0byte   ram size"*/

/*"      <author>  <time>   <version >   <desc>"*/
/*"      xyt       2017/11    1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "class_comm.h" 
#include "APP\message\MS.h"
#include "App\UserInterface\UI.h"
#include "App\UserInterface\UI_dataProcess.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
/*"************************************************"*/
/*22类输入输出属性2列表 设备对象列表*/
const ST_Class22_ObjectList CLASS22OBISLIST_AID_2[]=
{	/*注类型指针，没有加数组，所有数据都是有数组类型的，所以如果IID=0,则要编数组*/
    #ifdef THREE_PHASE
	{0xF201,0x02,1,AR_ST_3_VIS_5_COMDCB_EN},  /*RS485*/
	#else
    {0xF201,0x01,1,AR_ST_3_VIS_5_COMDCB_EN},  /*RS485*/
    #endif
    
	{0xF202,0x01,2,AR_ST_2_VIS_5_COMDCB},     /*红外 */
	{0xF205,NUM_RELAY,3,AR_ST_4_VIS_1_EN_EN_EN},   /*继电器输出 */
	{0xF206,0x01,4,AR_TYPE_ENUM},             /*告警输出*/
	{0xF207,0x01,5,AR_TYPE_ENUM},             /*多功能端子  */
	{0xF209,0x01,6,AR_ST_3_NULL_COMDCB_NULL}, /*载波/微功率无线接口*/
};

/*22类OBIS列表 属性2 设备对象列表*/
const ST_Class22_OAD_OBIS_INFO CLASS22_OI_AID_OBIS[] = 
{
	{1,0x02,E_RS485_1_BAUDRATE/*用于显示*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},   /*RS485*/
	{2,0x01,E_RS485_1_BAUDRATE/*用于显示*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*红外 */
	{3,NUM_RELAY,E_RELAY_UNIT_POWER, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},             /*继电器输出 */
	{4,0x02,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},             /*告警输出*/
	{5,0x01,E_OBIS_NULL/*已特殊处理*/, 0, MAX_FORMAT_ID, FUN_MULTI, E_INFO_NULL},            /*多功能端子  */
	{6,0x01,E_RS485_1_BAUDRATE/*用于显示*/, 0, MAX_FORMAT_ID, FUN_NULL, E_INFO_NULL},        /*载波/微功率无线接口*/    
};


INT8U get_class22_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{ 
    return get_class_parament_Obis(pObject->seq_num,pObisInfo_bak,
                                (const ST_PARAMENT_OAD_OBIS_INFO *)CLASS22_OI_AID_OBIS,ARRAY_SIZE(CLASS22_OI_AID_OBIS));
    
}    



/*22类事件元素查找*/
INT8U get_class22_input_output_object(SUB_OBJECT *pObject)
{
    INT8U  i,AID,IID;
    INT16U OI; 
    #ifdef THREE_PHASE
    INT8U MeterType;
    #endif
      
    OI  = pObject->Oad.Oi.asInt;
    AID = pObject->Oad.AId;
    IID = pObject->Oad.IId;

    /*最大AID为3*/
    if(AID>3)
    {
        return DAR_4_OANODEF;
    } 

    for(i=0;i<ARRAY_SIZE(CLASS22OBISLIST_AID_2);i++)
    {
        if(OI == CLASS22OBISLIST_AID_2[i].OI)
        {
            pObject->Class = 22;
            if(IID>CLASS22OBISLIST_AID_2[i].MAX_IID)
            {
                return DAR_8_OVERFLOW;
            }   
            else
            {
                pObject->Right_RW = READ_ONLY;
                pObject->element  = i;
                pObject->seq_num  = CLASS22OBISLIST_AID_2[i].Seq_Num;
                if(3==AID)
                {
                    pObject->Pointer = TYPE_U8;
                } 
                else
                {     
                    pObject->Pointer  = CLASS22OBISLIST_AID_2[i].Pointer;
                }

                #ifdef THREE_PHASE
                /*智能表的设备描述不一样*/
                GetSingle(E_METER_MODE, (INT8U *)&MeterType); 
                if(SMARTMETER == (MeterType&METERTPYE))
                {
                    if(OI_RS485_DEVICE == OI)
                    {
                        pObject->Pointer = AR_ST_3_VIS_6_COMDCB_EN;                        
                    }   
                }                     
                #endif
                
                return DAR_0_OK;
            }    
        }    
    }  

    return DAR_4_OANODEF;
    
}


/***********************************************/
const char string_com_698[2][PORT_DESCRIPT_LEN_6]={"RS485","INFRA"};
//const char string_carrie[MAX_NUM_BYPE_PORT_DESCRIPT] = {0};	//"载波"
/*几个固定参数*/
const INT8U rs_para_com_698[]={2,8,1,0,0};
/*return:数据长度，现在没有什么用了*/   


/*****************************************************************************
 函 数 名  : GetClass22Obis
 功能描述  : 获取通信口的波特率OBIS
 输入参数  : ST_OAD Oad  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 05 21
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16U GetClass22Obis(ST_OAD Oad)
{
    INT16U Oi,OBIS;

    Oi = Oad.Oi.asInt;
    
    /*载波*/
    if(OI_CARRIE_DEVICE == Oi)
    {
        OBIS = E_RS485_1_BAUDRATE;
    }  
    else if(OI_IR_DEVEICE == Oi)
    {
       OBIS  = E_RS485_1_BAUDRATE;
    }    
    else //if(OI_RS485_DEVICE == Oi) 
    { 
        #ifdef THREE_PHASE
        if(2 == Oad.IId)
        {    
            OBIS = E_RS485_2_BAUDRATE;
        }
        else
        #endif    
        {
            OBIS = E_RS485_1_BAUDRATE;
        }    
    }  

    return OBIS;
}


/*****************************************************************************
 函 数 名  : class22_input_output_get
 功能描述  : 输入输出设备GET实现
 输入参数  : ST_OAD Oad    
             INT8U *pdata  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 05 22
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT16S class22_input_output_get(ST_OAD Oad, INT8U *pdata)
{
    INT16U           Oi,OBIS;
    INT8U            IID,MAX_IID = 1;
    INT8U            *p_rs_device;
    BOOL             F_zhi;   /*智能表标志*/
    
    STR_COMDCB *p_comdcb;      /*端口参数指针*/  
    INT8U *pdst = pdata;

    #ifdef THREE_PHASE
	INT8U MeterType;    
    GetSingle(E_METER_MODE, (INT8U *)&MeterType); 

    if(SMARTMETER == (MeterType&METERTPYE))
    {
       F_zhi = 1; 
    } 
    else
    #endif    
    {
       F_zhi = 0; 
    } 

    Oi   = Oad.Oi.asInt;
    IID  = Oad.IId;    

    /*三相表把报警继电器也算*/
    
    /*设备数量*/
    if(3 == Oad.AId)
    {   
        if(0!=F_zhi)
        {
            *pdata = 2;
        }    
        else
        {    
            *pdata = 1;
        } 
        
        return DAR_0_OK;
    }  
    
    /*智能表*/
    if(0!=F_zhi)
    {        
        MAX_IID = 2;        
    }  
    else 
    {
        MAX_IID = 1;
    }  

    /*判断下合法性*/
    if(IID>MAX_IID)
    {
        return (0-DAR_8_OVERFLOW);
    }        
    
    if(0 == IID)
    {        
       IID = 1;
       
       if(0!=F_zhi)
       {
          MAX_IID = 2;
       }
       else
       {
          MAX_IID = 1;
       }  
    } 
    else
    {
        //IID = MAX_IID;
        MAX_IID = IID;        /*以保证只读一组数据*/
    }    
     
 //------1.载波---------------------        
	if(OI_CARRIE_DEVICE == Oi)
	{		
	    //按8号补遗，载波端口描述符为NULL
		p_comdcb = (STR_COMDCB *)pdata;
        GetSingle(E_RS485_1_BAUDRATE, &p_comdcb->commrate);
		p_comdcb->check_bit = 2;
		p_comdcb->data_bit	= 8;
		p_comdcb->stop_bit	= 1;
		p_comdcb->flow_ctr	= 0;
        
        return DAR_0_OK;
        //return sizeof(STR_COMDCB);
	}	
//-------2.485和红外--------------------    
    if((Oi>=OI_RS485_DEVICE)&&(Oi<=OI_IR_DEVEICE))
    { 
       p_rs_device = (INT8U *)pdata;
       
       for(;IID<=MAX_IID;IID++)
       {          
           /*RS485或INFRA*/
           LIB_MemCpy((INT8U*)string_com_698[(INT8U)(Oi-OI_RS485_DEVICE)],p_rs_device,PORT_DESCRIPT_LEN_5);

           #ifdef THREE_PHASE
           if (Oi==OI_RS485_DEVICE)
    	   {
        		//第一路:RS485（如果支持多路RS485，则设置RS4851）
        		if ((IID==0x01)&&(0!=F_zhi)) //智能表
        		{
        			p_rs_device[PORT_DESCRIPT_LEN_5]='1';
                    p_rs_device++;
                }
        		//第二路:RS4852
        		else if (IID==0x02)
                {                  
        			p_rs_device[PORT_DESCRIPT_LEN_5]='2';
                    p_rs_device++; 
                    Oad.IId = 2;     /*改一下，否则GetClass22Obis不对*/
                }              
    	   }
           #endif
           

           p_rs_device += PORT_DESCRIPT_LEN_5;

           p_comdcb = (STR_COMDCB *)p_rs_device;

           OBIS = GetClass22Obis(Oad);    
           GetSingle(OBIS, &p_comdcb->commrate);           
           
           p_rs_device++;
           LIB_MemCpy((INT8U *)rs_para_com_698, p_rs_device, sizeof(rs_para_com_698));
           p_rs_device += sizeof(rs_para_com_698);
       }     
        
       /*5+5+1=11即，数据长度为11或12*/
       //return DAR_0_OK; //(p_rs_device - pdata);
       
    }
//-----------3.继电器输出-----------------------------------
    else if(OI_RELAY_DEVICE ==Oi)
    {        
        for(;IID<=MAX_IID;IID++)
        {   
          //OBIS =  E_RELAY_UNIT_POWER+IID-1;
          pdst += UI_GetRelayOutput(&IID,pdst); 
          //pdst+=GetSingle(OBIS, pdst);
        }       
    }
//------------4.告警输出----------------------------------
    else if(OI_ALARM_DEVICE == Oi)
    {
        #ifdef THREE_PHASE
        *pdata = OC_GetAlmRelayCmd();  
        #else
        *pdata = 0;  
        #endif
        
    }
//------------5.多功能输出----------------------------------
    else if(OI_MULTI_DEVICE == Oi)
    {
        UI_GetMultiTermn(pdata);
    }
    else
    {
        return (0-DAR_4_OANODEF);
    }

    return DAR_0_OK;

}

/*****************************************************************************
 函 数 名  : 输入输出设备SET功能实现
 功能描述  : 
 输入参数  : Oad，pdata
 输出参数  : DAR
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 05 22
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
/*支持通讯口的设置，在厂内使用*/
INT8U class22_input_output_set(ST_OAD Oad, INT8U *pdata)
{
    INT16U     Oi;
    STR_COMDCB *p_comdcb;      /*端口参数指针*/
    //UINT8 temp;
    INT8U AID,ChangePort; 
    INT16U OBIS;
    #ifdef THREE_PHASE
    INT8U MeterType;
    #endif
    
    AID = Oad.AId;
    
    if(ATTRIBUTE_2_SET_PARAMENT!=AID)
    {
        return 0;
    }

    Oi = Oad.Oi.asInt; 

    /*红外不能设*/
    if(OI_IR_DEVEICE == Oi)
    {
        return DAR_16_BAUDERR;
    }
    
    //---------------------------
    if((OI_RS485_DEVICE == Oi)||(OI_CARRIE_DEVICE == Oi))
    { 
        if(OI_CARRIE_DEVICE == Oi)
        {
            p_comdcb   = (STR_COMDCB *)pdata;    /*NULL*/
            ChangePort = E_COM_RS485_1;
        }    
        else
        {
            ChangePort = E_COM_RS485_1;
            #ifdef THREE_PHASE
            GetSingle(E_METER_MODE, (INT8U *)&MeterType); 
            if(SMARTMETER == (MeterType&METERTPYE))
            {
               p_comdcb = (STR_COMDCB *)(pdata+6);  /*RS4851或RS4852*/ 
               if(2 == (*(pdata+5)))
               {
                  ChangePort = E_COM_RS485_2;  
               } 
            }
            else
            #endif    
            {
               p_comdcb   = (STR_COMDCB *)(pdata+5);  /*RS485*/               
            }    
        }  

        OBIS = GetClass22Obis(Oad);        

        mBpsChangeFlg[ChangePort] = TRUE;
        
        return SetSingle(OBIS,&p_comdcb->commrate,1);    
               
    }

    return DAR_4_OANODEF;

}    
 
/*****************************************************************************
 函 数 名  : class22_input_output_action
 功能描述  : 输入输出设备方法操作功能实现
 输入参数  : ST_OAD Oad    
             INT8U *pdata  
 输出参数  : 无
 返 回 值  : dar
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 05 22
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U class22_input_output_action(ST_OAD Oad, INT8U *pdata)
{
    INT16U            Oi;
    STR_RS_DEVICE_SET *p_rs_device;
    INT8U temp,ChangePort;
    INT8U AID; 
    INT16U OBIS;

    AID = Oad.AId;
    
    if((METHOD_CLASS22_127!=AID)&&(METHOD_CLASS22_128!=AID))
    {
        return DAR_4_OANODEF;
    }

    Oi = Oad.Oi.asInt;

    /*红外不能设*/
    if(OI_IR_DEVEICE == Oi)
    {
        return DAR_16_BAUDERR;
    }    

    //--------1.波特率设置-------------------
    if((OI_RS485_DEVICE == Oi)||(OI_CARRIE_DEVICE == Oi))
    {
        p_rs_device = (STR_RS_DEVICE_SET *)pdata;

        if(OI_CARRIE_DEVICE == Oi)  /*载波*/
        {
            OBIS       = E_RS485_1_BAUDRATE;
            ChangePort = E_COM_RS485_1;
        } 
        else //(OI_RS485_DEVICE == Oi)
        {    
            #ifdef THREE_PHASE            
            if(2 == p_rs_device->OAD.IId)
            {
                OBIS = E_RS485_2_BAUDRATE;
                ChangePort = E_COM_RS485_2;
            }  
            else
            #endif    
            {
                OBIS = E_RS485_1_BAUDRATE;
                ChangePort = E_COM_RS485_1;
            }  
        }        

        mBpsChangeFlg[ChangePort] = TRUE;
        
        return SetSingle(OBIS,&p_rs_device->port_com_dcb.commrate,1);
    }
    //---------------------------
    else if(OI_RELAY_DEVICE == Oi)
    {
        /*先暂时这样写*/
        return DAR_1_HW_FAIL;
    }
    //----------------------------------------------
    else if(OI_MULTI_DEVICE == Oi)
    {
        temp =  *(INT8U *)(pdata+4);
        
        if(temp>E_MULTI_TRAFFSWITCH)
        {
            return DAR_FF_OTHERERR;
        }
        #ifdef SINGLE_PHASE
        if(E_MULTI_DEMANDPERIOD == temp)
        {
            return DAR_FF_OTHERERR;
        }
        #endif
        
        OC_MultiTermnSw((EN_OC_MultiTermnMode) temp);

        return DAR_0_OK;
    }
    else
    {
        return DAR_4_OANODEF;
    }

}

/*"************************************************"*/
