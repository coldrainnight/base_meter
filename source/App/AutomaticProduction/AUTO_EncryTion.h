#ifndef _AUTO_ENCRYTION_H
#define _AUTO_ENCRYTION_H

#define ENCRYPED 	1	/*"����"*/
#define DECRYPED 	2	/*"����"*/

extern const INT8U SupPassWord[8];

extern void Data_EncrypTion(INT8U *EncryData);
extern void Data_DecrypTion(INT8U *DecryData);


#endif
