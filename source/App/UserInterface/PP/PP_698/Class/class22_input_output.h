/*"***********************************************************		
FileName: CLASS22_INPUT_OUTPUT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: 输入输出类头文件		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS22_INPUT_OUTPUT_H_
#define _CLASS22_INPUT_OUTPUT_H_
/*"******************************************************"*/ 
/*"-------------------------------------------------------"*/
#define METHOD_CLASS22_127         127         /*22类只有方法127和128有用*/
#define METHOD_CLASS22_128         128     

typedef struct
{
   INT16U OI;             /*OI内容*/   
   INT8U  MAX_IID;        /*MAX_IID,看是否有必要定义*/ 
   INT8U  Seq_Num;        /*列表序列号，同时也凑成4字节对讲，主要是为了CLASS8 ST_OAD_OBIS_INFO列表查找用*/
   const INT8U *Pointer;  /*类型指针*/   
}ST_Class22_ObjectList;    /*属性元素*/


typedef struct
{ 
    INT8U  Seq_Num;   /*列表序列号 与ST_Class22_ObjectList中的一样*/
    INT8U  num;       /*"OAD对应OBIS中元素个数"*/
    INT16U Obis;      /*"数据中心对象  <3000,则是直接读取，否则，则是OBIS列表"*/

    INT8U  Offset;    /*"数据中心数据的偏移 都固定为0？先保留，后面看情况，再决定是否去掉"*/
    INT8U  FormatId;  /*"通信、显示格式ID"*/
    INT8U  FunctionId;/*"读写方法ID"*/
    INT8U  ShowinfoId;/*"显示配置表ID"*/
}ST_Class22_OAD_OBIS_INFO;

/****************************************************************************/
/*波特率 300bps（0），   600bps（1），     1200bps（2），2400bps（3），  4800bps（4），    7200bps（5），
9600bps（6），  19200bps（7），   38400bps（8），57600bps（9）， 115200bps（10）， 自适应（255）*/
typedef struct
{
    INT8U commrate;    /*波特率 */
    INT8U check_bit;   /*校验位  ENUMERATED {无校验（0），奇校验（1），偶校验（2）*/
    INT8U data_bit;    /*数据位  ENUMERATED {5（5），6（6），7（7），8（8）*/
    INT8U stop_bit;    /*停止位  ENUMERATED {1（1），2（2）*/
    INT8U flow_ctr;    /*流控    ENUMERATED {无(0)，硬件(1)，软件(2)*/
}STR_COMDCB;

#define PORT_DESCRIPT_LEN_6    6   /*最大6个字节*/
#define PORT_DESCRIPT_LEN_5    5   /*最大5个字节*/

/*RS232\485 输入输出设备类对象标识 设置*/
typedef struct
{
    ST_OAD    OAD;                /*OAD*/
    STR_COMDCB port_com_dcb;       /*端口参数   COMDCB*/
    INT8U      port_fun;           /*端口功能   enum{上行通信（0），抄表（1），级联（2），停用（3）*/
}STR_RS_DEVICE_SET;


/*继电器输出*/
#define MAX_BYTE_RELAY_DESCRIPT   1    /*最大1个字节 0,送检公告写的是0，怪怪的*/
typedef struct
{
   INT8U relay_descript[MAX_BYTE_RELAY_DESCRIPT];     /*描述符 visible-string*/
   INT8U current_status;                              /*当前状态  enum{未输出（0），输出（1）*/
   INT8U method;                                      /* 开关属性  enum{脉冲式（0），保持式（1）*/
   INT8U connect_status;                              /*接线状态  enum{接入（0），未接入（1)*/
}STR_RELAY_DEVICE;


/*"--------------------------------------------------------------"*/
/*外部常量或变量声明*/
extern INT8U get_class22_input_output_object(SUB_OBJECT *pObject);
extern INT8U get_class22_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);
extern INT16S class22_input_output_get(ST_OAD Oad, INT8U *pdata);
extern INT8U class22_input_output_set(ST_OAD Oad, INT8U *pdata);
extern INT8U class22_input_output_action(ST_OAD Oad, INT8U *pdata);

/*"******************************************************"*/
#endif
