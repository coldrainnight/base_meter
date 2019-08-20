#ifndef AL_698_H
#define AL_698_H
#include "system\Sys_def.h"


typedef enum
{
    UN_ENCRYPTED = 0,    /*����*/
    UN_ENCRYPTED_WITH_SIDMAC,/*����+SIDMAC*/
    UN_ENCRYPTED_WITH_RN,/*����+�����*/
    UN_ENCRYPTED_WITH_RNMAC,/*����+RNMAC*/
    UN_ENCRYPTED_WITH_SID,/*����+SID*/

    ENCRYPTED = 0x80,/*����*/
    ENCRYPTED_WITH_SIDMAC,/*����+SIDMAC*/
    ENCRYPTED_WITH_RN,/*����+�����*/
    ENCRYPTED_WITH_RNMAC,/*����+RNMAC*/
    ENCRYPTED_WITH_SID,/*����+SID*/

}EN_SECURITY_TYPE;

/*Ӧ�����ӽ��*/
typedef enum
{
    CONNECT_OK = 0,/*������Ӧ������*/
    PASSWORD_ERR=1,
    DE_ERR_1=2,/*�Գƽ��ܴ���*/
    DE_ERR_2=3,/*�ǶԳƽ��ܴ���*/
    DE_ERR_3=4,/*ǩ������*/
    VERSION_ERR =5,/*Э��汾��ƥ��*/
    OTHER_ERR = 255
}EN_CONNECT_RESULT;



/*�쳣���� 		ENUMERATED
{
  APDU�޷�����	��1����
  ����֧��	     ��2����
  ����			��255��
}*/
    
typedef enum
{
   E_APDU_NO_DECODE   = 1,
   E_NO_SERVICE       = 2,
   E_OTHER_ERR        = 255
}EN_ERR_RESPONSE_TYPE;    


/*Ӧ�����ӽ��*/
typedef enum
{
    NULL_SECURITY = 0,/*��������*/
    PASSWORD_SECURITY=1,/*һ������*/
    SYMMETRY_SECURITY=2,/*�ԳƼ���*/
    SIGNATURE_SECURITY=3,/*����ǩ��*/
}EN_CONNECT_MECHANISM_INFO;


typedef struct
{
    INT32U ConnectOverTime;/*�̶���Ӧ�����ӳ�ʱʱ��*/
    INT32U ConnectTimeRem;/*ʣ���Ӧ�����ӳ�ʱʱ��*/
    EN_CONNECT_MECHANISM_INFO  ConnectLevel;  

    INT16U MaxRecSendLen; /*��ǰ���������պͷ���APDU�ߴ�*/
    INT8U  ClientAddr;/*��ǰ���ӿͻ�����ַ*/

}ST_CONNECT_INFO;

#define MAX_APDU_LEN (250) /*���ɴ���APDU����*/

extern EN_COM_ID NowComId;/*��ǰͨ��*/
extern ST_CONNECT_INFO ConnectInfo[E_MAX_COM_ID];

extern const INT8U ConnectResInfo[];

extern void AL_698_Init(void);

 
extern void AL_698_Main(void);


extern void AL_698_1Sec(void);


#endif
