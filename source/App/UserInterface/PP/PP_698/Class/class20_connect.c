/*"*****************************************************************************"*/
/*"  FileName: class20_connect.c"*/
/*"  Author: xyt       Version :  V1.0        Date:2017.11"*/
/*"  Description:  应用连接类文件   "*/
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
#include "App\UserInterface\ProtocolStack\PS_698\AL_698.h"
/*"*******************************************************************************"*/
const INT8U C20_Att3_FactoryVersion[]=
{	
	longunsigned,0x00,0x14,/*协议版本信息       long-unsigned*/
	longunsigned,0x07,0xD0,/*最大接收APDU尺寸   long-unsigned*/
	longunsigned,0x07,0xD0,/*最大发送APDU尺寸   long-unsigned*/
	longunsigned,0x07,0xD0,/*最大可处理APDU尺寸   long-unsigned*/
	bitstring,64,0xff,0xff,0xff,0xff,0xC0,0x00,0x00,0x00,/*协议一致性块        bit-string(64)*/
	bitstring,0x81,0x80,0xff,0xFE,0xC4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*功能一致性块        bit-string(128)*/
};


/*应用连接类，只有一个OI=4400*/
/*20类应用连接属性列表*/
const ST_SPECIAL_CLASS_AID_INFO CLASS20_AID_LIST[3]=
{
    /*1,逻辑名*/
    /*2.对象列表*/
    {3, READ_ONLY,  0x07,TYPE_CONNECT_INFO},   /*3.应用语境信息*/        
    {4, READ_ONLY,  0x02,TYPE_U8},             /*4.当前连接的客户机地址*/
    {5, READ_ONLY,  0xFF,TYPE_ENUM}           /*5.身份验证机制*/
};

/*20类OBIS列表*/
const ST_CLASS20_OI_AID_OBIS CLASS20_OI_AID_OBIS[1] = 
{
    {OI_CONNECT, E_LRM_JUSTMETER,E_LRM_JUSTMETER,E_LRM_JUSTMETER} /*电气设备*/  
};

/*CLASS20 属性3每个IID对应的偏移和长度*/
const INT8U C20_ATT3_IID_OFFSET[7]={0,3,6,9,12,22};
const INT8U C20_ATT3_IID_LEN[7]   ={3,3,3,3,10,19};


/*没用，获取的OBIS都是假的，看是否返回个数为0?*/
INT8U get_class20_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak)
{
   INT8U  seq_num,AID;
     
    
   seq_num = pObject->seq_num;
   AID     = pObject->Oad.AId;
   pObisInfo_bak->Obis = *((INT16U *)&CLASS20_OI_AID_OBIS[seq_num] + AID-2); 
   pObisInfo_bak->Offset = 0;
   pObisInfo_bak->num = 1;  //

   /*显示格式，看是否要特殊定义*/

   return 1;
      
}



/*****************************************************************************
 函 数 名  : get_class20_connect_data
 功能描述  : 应用连接类处理函数
 输入参数  : ST_OAD Oad    
             INT8U *pdata  
             INT8U *pRet   
 输出参数  : 无
 返 回 值  : INT8U
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 01 11
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U *get_class20_connect_data(ST_OAD Oad,INT8U *pdata,INT8U *pRet)
{
	INT8U i,j;
	INT32U time;
    INT8U  IId;
    INT8U  AID;
    INT8U *pdst=pdata;
	
    AID = Oad.AId;
    IId = Oad.IId;

    pdst = EnCodeOAD(&Oad, pdst);
    
    *pdst++ =  E_RESULT_CHOICE_DATA;     /*表示返回的是数据*/
    
	if(EN_CLS20_ATT_3_APP_INFO == AID)   /* 应用语境信息  */
	{			
        IId = Oad.IId;
        
        if(0!=IId)
        {
            i = IId;
            j = IId;                
        }
        else /*IID=0或其它大于7的IID也走这里，省代码*/
        {
            *pdst++ = structure;
            *pdst++ = 7;
            i = 1;
            j = 7;
        }                
        
        for(;i<=j;i++)
        {              
            if(i<7)
            {      
			    LIB_MemCpy((INT8U *)C20_Att3_FactoryVersion + C20_ATT3_IID_OFFSET[i-1], pdst, C20_ATT3_IID_LEN[i-1]);
			    pdst += C20_ATT3_IID_LEN[i-1];	
            }
            else //(7 ==i)  /*静态超时时间*/
            {    
			    time=ConnectInfo[ComPort].ConnectOverTime;
                pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_U32,(INT8U*)&time,pdst,0); 
            }                 
        }            
	}
	else if(EN_CLS20_ATT_4_CLIENT_ADDR == AID)   /* 当前连接的客户机地址  */
	{		 
	    pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_U8,(INT8U *)&ConnectInfo[ComPort].ClientAddr,pdst,0); 
	}
	else //if(5 == AID)            /* 当前连接状态  */
	{		
	    pdst += EnCode_Data((ST_ENCODE_INFO *)TYPE_ENUM,(INT8U *)&ConnectInfo[ComPort].ConnectLevel,pdst,0); 
	}	

    return pdst;
}




/*"************************************************"*/
