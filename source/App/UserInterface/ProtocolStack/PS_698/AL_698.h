#ifndef AL_698_H
#define AL_698_H
#include "system\Sys_def.h"


typedef enum
{
    UN_ENCRYPTED = 0,    /*明文*/
    UN_ENCRYPTED_WITH_SIDMAC,/*明文+SIDMAC*/
    UN_ENCRYPTED_WITH_RN,/*明文+随机数*/
    UN_ENCRYPTED_WITH_RNMAC,/*明文+RNMAC*/
    UN_ENCRYPTED_WITH_SID,/*明文+SID*/

    ENCRYPTED = 0x80,/*密文*/
    ENCRYPTED_WITH_SIDMAC,/*密文+SIDMAC*/
    ENCRYPTED_WITH_RN,/*密文+随机数*/
    ENCRYPTED_WITH_RNMAC,/*密文+RNMAC*/
    ENCRYPTED_WITH_SID,/*密文+SID*/

}EN_SECURITY_TYPE;

/*应用连接结果*/
typedef enum
{
    CONNECT_OK = 0,/*允许建立应用链接*/
    PASSWORD_ERR=1,
    DE_ERR_1=2,/*对称解密错误*/
    DE_ERR_2=3,/*非对称解密错误*/
    DE_ERR_3=4,/*签名错误*/
    VERSION_ERR =5,/*协议版本不匹配*/
    OTHER_ERR = 255
}EN_CONNECT_RESULT;



/*异常类型 		ENUMERATED
{
  APDU无法解析	（1），
  服务不支持	     （2），
  其他			（255）
}*/
    
typedef enum
{
   E_APDU_NO_DECODE   = 1,
   E_NO_SERVICE       = 2,
   E_OTHER_ERR        = 255
}EN_ERR_RESPONSE_TYPE;    


/*应用连接结果*/
typedef enum
{
    NULL_SECURITY = 0,/*公共连接*/
    PASSWORD_SECURITY=1,/*一般密码*/
    SYMMETRY_SECURITY=2,/*对称加密*/
    SIGNATURE_SECURITY=3,/*数字签名*/
}EN_CONNECT_MECHANISM_INFO;


typedef struct
{
    INT32U ConnectOverTime;/*商定的应用连接超时时间*/
    INT32U ConnectTimeRem;/*剩余的应用连接超时时间*/
    EN_CONNECT_MECHANISM_INFO  ConnectLevel;  

    INT16U MaxRecSendLen; /*当前连接最大接收和发送APDU尺寸*/
    INT8U  ClientAddr;/*当前连接客户机地址*/

}ST_CONNECT_INFO;

#define MAX_APDU_LEN (250) /*最大可处理APDU长度*/

extern EN_COM_ID NowComId;/*当前通道*/
extern ST_CONNECT_INFO ConnectInfo[E_MAX_COM_ID];

extern const INT8U ConnectResInfo[];

extern void AL_698_Init(void);

 
extern void AL_698_Main(void);


extern void AL_698_1Sec(void);


#endif
