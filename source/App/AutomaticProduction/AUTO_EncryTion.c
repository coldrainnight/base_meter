/*"*****************************************************************************"*/
/*"  FileName: AUTO_EncryTion.c"*/
/*"  Description:     ���ļ�ʵ��������ݼ���"*/
/*"  Author: "*/
/*"  Version:   V1.0"*/
/*"  Date:2017.10.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // ��ʷ�޸ļ�¼"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      2017/10/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "..\..\PlatForm.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "App\message\MS.h"

#include "AUTO_EncryTion.h"

const INT8U SupPassWord[8] = {0x0A, 0xBC, 0x12, 0xDE, 0x78, 0xE3, 0x5B, 0x9D};

/*"Key�任����    Key_E == Key_D"*/
const INT8U Key_DIS[8]= {0x0A, 0xBC, 0x12, 0xDE, 0x00, 0x20, 0x38, 0x2A};
const INT8U Table1[16] = {12, 10, 8, 4, 3, 15, 0, 2, 14, 1, 5, 13, 6, 9, 7, 11};
const INT8U Table2[16] = {6, 9, 7, 4, 3, 10, 12, 14, 2, 13, 1, 15, 0, 11, 8, 5};
const INT8U Ptable_E[64]= {29, 27, 34, 9, 16, 62, 55, 2,
							40, 49, 38, 25, 33, 61, 30, 23,
							1, 41, 21, 57, 42, 15, 5, 58,
							19, 53, 22, 17, 48, 28, 24, 39,
							3, 60, 36, 14, 11, 52, 54, 12,
							31, 51, 10, 26, 0, 45, 37, 43,
							44, 6, 59, 4, 7, 35, 56, 50,
							13, 18, 32, 47, 46, 63, 20, 8}; /*���ܱ�*/
const INT8U Ptable_D[64]= {44, 16, 7, 32, 51, 22, 49, 52,
							63, 3, 42, 36, 39, 56, 35, 21,
							4, 27, 57, 24, 62, 18, 26, 15,
							30, 11, 43, 1, 29, 0, 14, 40,
							58, 12, 2, 53, 34, 46, 10, 31,
							8, 17, 20, 47, 48, 45, 60, 59,
							28, 9, 55, 41, 37, 25, 38, 6,
							54, 19, 23, 50, 33, 13, 5, 61};/*���ܱ�*/

static void Encryption(INT8U *Data_E, INT8U *Key_E,
        const INT8U *SubstitutionTable1, const INT8U *SubstitutionTable2,
        const INT8U *PermutationTable);

static void Decryption(INT8U *Data_D, INT8U *Key_D,
        const INT8U *SubstitutionTable1, const INT8U *SubstitutionTable2,
        const INT8U *PermutationTable);

/*********************************************
*	�������ƣ�Data_EncrypTion
*	����	�����ܣ�ÿ�μ��ܵ����ݳ���Ϊ8�ֽ�
*	����	��
*				1. Indata �����ܵ����ݵ�ͷָ��, ���� 8byte
*				2. OutData ���ܺ������ָ��
*********************************************/
void Data_EncrypTion(INT8U *EncryData)
{
    INT8U Key_E[8],MeterAddr[6];
	GetSingle(E_COMM_ADDR,MeterAddr);
	LIB_MemCpy((INT8U *)Key_DIS,Key_E,8);
	LIB_MemCpy(MeterAddr+2,Key_E,4);
	
	Encryption(EncryData, Key_E, Table1, Table2, Ptable_E);  
}


/*********************************************
*	�������ƣ�Data_DecrypTion
*	����	�����ܣ�ÿ�ν��ܵ����ݳ���Ϊ8�ֽ�
*	����	��
*				1. Indata �����ܵ����ݵ�ͷָ��, ���� 8byte
*				2. OutData ���ܺ������ָ��
*********************************************/
void Data_DecrypTion(INT8U *DecryData)
{
    INT8U Key_D[8],MeterAddr[6];
	GetSingle(E_COMM_ADDR,MeterAddr);
	LIB_MemCpy((INT8U *)Key_DIS,Key_D,8);
	LIB_MemCpy(MeterAddr+2,Key_D,4);

	Decryption(DecryData, Key_D, Table1, Table2, Ptable_D);  
}

/*****************************************************************************
 �� �� ��  : GetKeyNibbleBit
 ��������  : ȡ��key�ĵ�i��nibble���ж���ָ��bitλ�Ƿ�Ϊ1
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
INT8U GetKeyNibbleBit(INT8U EorD, const INT8U *Key, INT8U p, INT8U factor)
{
    INT8U tmp,bit;
	INT8U nibble;

    if(EorD == ENCRYPED) 
	{
		bit = 3;
	}
	else
	{
		bit = 0;
	}

    tmp = *(Key + p);
    nibble = (tmp >> factor) & 0x0F;
    return (INT8U)((nibble >> bit) & 0x01);
}

/*****************************************************************************
 �� �� ��  : SubstitutionDataWithTable
 ��������  : ��Table�Ķ�ӦNibble�û�Data��nibble
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
void SubstitutionDataWithTable(INT8U *Data, const INT8U *STable, INT8U p, INT8U factor)
{
    INT8U tmp;
	INT8U nibble;
	
	tmp = *(Data + p);
    nibble = (tmp >> factor) & 0x0F;
    *(Data + p) = (STable[nibble] << factor) | (*(Data + p) & (0xF0 >> factor));
}

void Substitution_Proc(INT8U EorD, INT8U *Data, /*const*/ INT8U *Key,
                        const INT8U *STable1, const INT8U *STable2)
{
    INT8U NibbleCount,p,horl,factor;
	
	p = 0; 		/*"����λ��"*/
    horl = 0;	/*"�߰��ֽڻ�Ͱ��ֽڣ�0 -- Ϊ�߰��ֽڣ�  1 -- Ϊ�Ͱ��ֽ�"*/
    factor =  0;

    for(NibbleCount = 0; NibbleCount <= 15; NibbleCount++)
    {
        p = NibbleCount / 2;
        horl = NibbleCount % 2;
        factor =  4 * (horl ^ 1);

        if(GetKeyNibbleBit(EorD, Key, p, factor))
        {
			/*"��Data��Ӧnibble��table2���û�"*/
            SubstitutionDataWithTable(Data, STable2, p, factor);
        }
        else
        {
			/*"��Data��Ӧnibble��table1���û�"*/
            SubstitutionDataWithTable(Data, STable1, p, factor);
        }
    }
};


/*****************************************************************************
 �� �� ��  : Permutation_Proc
 ��������  : ����Ptable_E����λ�任  ,  PTable[0] ��Ӧ����λ
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
void Permutation_Proc(INT8U *Data, const INT8U *PTable)
{
  INT8U i,i1,i2,j1,j2;
  INT8U Pvalue;
  INT8U DataTmp[8] = {0};
  INT8U tmp1;
 
  i1 = 0;
  i2 = 0;
  j1 = 0;
  j2 = 0;

  for(i = 0; i < 64; i ++)
  {
    i1 = (63 - i) / 8;	/*"�����е�λ��"*/
    j1 = i % 8;			/*"ֵ�ı���λ������Ϊ0λ"*/


    Pvalue = PTable[i]; /*"��   ��bitλ��Pvalueλ��ֵ"*/

    i2 = (63 - Pvalue) / 8;/*"�����е�λ��"*/
    j2 = Pvalue % 8;       /*"ֵ�ı���λ"*/


    tmp1 = (*(Data + i1)) & (0x01 << j1); 

    if(tmp1 > 0)  /*"��Ŀ��λ��Ϊ1"*/
        *(DataTmp + i2) += 0x01 << j2;
  }

  LIB_MemCpy(DataTmp,Data,8);
  
};

/*****************************************************************************
 �� �� ��  : KeyRotationLeft_Proc
 ��������  : ��Key����һλ,�ŵ��ұ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
void KeyRotationLeft_Proc(INT8U *Key)
{

  INT8U i;
  INT8U left = 0;
  INT8U factor1, factor2;  
  
  left = 0;
  factor1 = 0;
  factor2 = 0; 
  
  for(i = 0; i < 8; i ++)
  {
        factor1 = ((*(Key + i)) & 0x80) > 0 ? 1 : 0;
        if(i == 0) /*"���类�Ƴ���λ"*/
		{
			left = factor1;
		}  

        if(i == 7)
		{
		    factor2 = left;	
		}
        else
		{
		    factor2 = ((*(Key + i + 1)) & 0x80) > 0 ? 1 : 0;/*"�˴������λ"*/
		}

        *(Key + i) = (INT8U)((*(Key + i)) << 1) + factor2;
  }
};


/*****************************************************************************
 �� �� ��  : KeyRotationRight_Proc
 ��������  : ��Key����һλ,�ŵ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
*****************************************************************************/
void KeyRotationRight_Proc(INT8U *Key)
{
  signed char i;
  INT8U right;
  INT8U factor1, factor2;  
  
  right = 0;
  factor1 = 0;
  factor2 = 0;
  
  for(i = 7; i >= 0; i --)
  {
        factor1 = ((*(Key + i)) & 0x01) > 0 ? 1 : 0;
        if(i == 7)/*"���类�Ƴ���λ"*/
		{
			right = factor1; 
		}     

        if(i == 0)
		{            
			factor2 = right;
		}
        else
		{
			factor2 = ((*(Key + i - 1)) & 0x01) > 0 ? 1 : 0;  /*"�˴������λ"*/
		}

        *(Key + i) = (INT8U)((*(Key + i)) >> 1) + (factor2 << 7);
  }

};

void ComplementedAndRotation_Proc(INT8U *Key_E)
{
    INT8U i;
	INT8U tmp1;
	INT8U tmp2;
	
	for(i = 0; i < 8; i ++)
    {
      Key_E[i] = (0xF0 - (Key_E[i] & 0xF0)) + (0x0F - (Key_E[i] & 0x0F));
    }

    tmp1 = Key_E[7];
    tmp2 = Key_E[6];

    for(i = 6; i > 0; i --)
    {
        Key_E[i + 1] = ((Key_E[i] & 0xF0) >> 4) + ((Key_E[i - 1] & 0x0F) << 4);
    }

    Key_E[1] = ((Key_E[0] & 0xF0) >> 4) + ((tmp1 & 0x0F) << 4);
    Key_E[0] = ((tmp1 & 0xF0) >> 4) + ((tmp2 & 0x0F) << 4);
};



static void Encryption(INT8U *Data_E, INT8U *Key_E,
        const INT8U *SubstitutionTable1, const INT8U *SubstitutionTable2,
        const INT8U *PermutationTable)
{
    INT8U i;
	INT8U *lpData = Data_E;
    INT8U *lpKey = Key_E;

    ComplementedAndRotation_Proc(lpKey);

    for(i = 0; i < 16; i ++)
    {
        Substitution_Proc(ENCRYPED, lpData, lpKey, SubstitutionTable1, SubstitutionTable2);
        Permutation_Proc(lpData, PermutationTable);

        KeyRotationLeft_Proc(lpKey);
    }

}


static void Decryption(INT8U *Data_D, INT8U *Key_D,
        const INT8U *SubstitutionTable1, const INT8U *SubstitutionTable2,
        const INT8U *PermutationTable)
{
    INT8U i;
	INT8U *lpData = Data_D;
    INT8U *lpKey = Key_D;

    for(i = 0; i < 16; i ++)
    {
        Permutation_Proc(lpData, PermutationTable);
        Substitution_Proc(DECRYPED, lpData, lpKey, SubstitutionTable1, SubstitutionTable2);

        KeyRotationRight_Proc(lpKey);
    }
}


