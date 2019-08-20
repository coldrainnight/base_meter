#ifndef  SID_H
#define  SID_H

#include "System\sys_def.h"

/*"״̬��������"*/
#ifdef THREE_PHASE
#define SID_FAC               (0)    /*"����״̬"*/
#define SID_MTRCOVER     (1)   /*"�ϸ�"*/
#define SID_RAILCOVER    (2)   /*"����"*/
#define SID_REALY            (3)   /*"�̵���״̬���"*/
#define SID_CARD             (4)    /*����λ"*/
#define SID_MAGNET        (5)    /*"�㶨�ų�����"*/
#define SID_MAX               (6)   /*"״̬������"*/
#else
#define SID_FAC          (0)  /*"����״̬"*/
#define SID_MTRCOVER     (1)  /*"���Ǽ��"*/
#define SID_REALY        (2)  /*"�̵���״̬���"*/
#define SID_CARD         (4)  /*"�����"*/
#define SID_MAX          (3)  /*"״̬������"*/
#endif

/*"״̬����Ĳ�ѯ���"*/
#define SID_UNDEF    0x02
#define SID_TRUE       0x01   /*" SID_TRUE �ĺ��� "*/
                              /*"����״̬"*/
                              /*"�ϸǿ���"*/
                              /*"���ǿ���"*/
                              /*"�̵�����բ"*/
                              /*"�п�"*/
                              /*"�дų�����"*/                        
#define SID_FALSE      0x00


extern void SID_Init(INT8U mode,INT8U sel);  /*"״̬����ĳ�ʼ��"*/


extern void SID_Release(INT8U mode,INT8U sel);  /*"״̬������ͷ�"*/


extern INT8U SID_PollRead(INT8U sel);  /*"״̬�����IO�ڶ�����"*/


extern INT8U SID_Read(INT8U sel);  /*"״̬����Ķ�����"*/

extern INT8U SID_IntVectSet(INT8U sel, ISR_FNCT_VOID intfunc);

extern void SID_NullIntFunc(void); /*"״̬����Ŀյ��жϺ���"*/

#endif