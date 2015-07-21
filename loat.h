#ifndef LOAT_H
#define LOAT_H

#include"lint.h"

typedef struct {
	lint *mantissa;
	int exponent;
} loat;

loat *createF(int);
loat *alignF(loat*,int);
loat *cloneF(loat*);
int indx(char*,char);
loat *convertF(char*);
loat *negateF(loat*);
loat *shiftF(loat*,int);
loat *shrinkF(loat*);
loat *truncateF(loat*,int);
loat *padF(loat*,int);
void printF(loat*);
void deleteF(loat*);
void destroyF(loat**,int);
char _compareF(loat*,loat*);
char compareF(loat*,loat*);
loat *_additionF(loat*,loat*);
loat *_subtractionF(loat*,loat*);
loat *_kmultiplicationF(loat*,loat*);
loat *_divisionF(loat*,loat*,int);
loat *additionF(loat*,loat*);
loat *subtractionF(loat*,loat*);
loat *multiplicationF(loat*,loat*);
loat *divisionF(loat*,loat*,int);
loat *randoneF(int);
loat **randlistF(int,int);

#endif
