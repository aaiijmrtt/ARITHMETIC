#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lint.h"

//loat data structure
typedef struct {
	lint *mantissa;
	int exponent;
} loat;

//returns a new loat initialized as + with 0s of length l
loat *createF(int l) {
	loat *n=(loat*)malloc(sizeof(loat));
	n->mantissa=createI(l);
	n->exponent=0;
	return n;
}

//returns a loat with e as exponent, assumed having >e as exponent
loat *alignF(loat *n,int e) {
	loat *newn=createF(n->mantissa->length+n->exponent-e);
	int i;
	for(i=0;i<n->mantissa->length;++i) newn->mantissa->digits[i-e+n->exponent]=n->mantissa->digits[i];
	newn->mantissa->sign=n->mantissa->sign;
	newn->exponent=e;
	return newn;
}

//returns a new loat cloned from loat a
loat *cloneF(loat *a) {
	loat *n=(loat*)malloc(sizeof(loat));
	n->mantissa=cloneI(a->mantissa);
	n->exponent=a->exponent;
	return n;
}

//returns the index of character c in null terminated string s
int indx(char *s,char c) {
	int i;
	for(i=0;(s[i]!='\0')&&(s[i]!=c);++i);
	return i;
}

//returns a new loat as + with absolute value of null terminated string s
loat *convertF(char *s) {
	int i,j,e=indx(s,'e'),l=len(s);
	loat *n=(loat*)malloc(sizeof(loat));
	n->mantissa=createI(e);
	for(i=n->mantissa->length-1,j=0;j<n->mantissa->length;--i,++j) n->mantissa->digits[i]=s[j]-'0';
	if(e>=l) n->exponent=0;
	else n->exponent=atoi(s+e+1);
	return n;
}

//returns loat n with sign negated
loat *negateF(loat *n) {
	n->mantissa->sign=-n->mantissa->sign;
	return n;
}

//returns a new loat with loat n shifted by l places
loat *shiftF(loat *n,int l) {
	loat *newn=cloneF(n);
	newn->exponent+=l;
	return newn;
}

//returns a new loat with loat n trimmed of leading and trailing 0s
loat *shrinkF(loat *n) {
	int i,j;
	for(i=n->mantissa->length-1;(i>=0)&&(n->mantissa->digits[i]==0);--i);
	for(j=0;(j<n->mantissa->length)&&(n->mantissa->digits[j]==0);++j);
	if(i<j) return createF(1);
	loat *newn=createF(i-j+1);
	newn->mantissa->sign=n->mantissa->sign;
	for(;i>=j;--i) newn->mantissa->digits[i-j]=n->mantissa->digits[i];
	newn->exponent=n->exponent+j;
	return newn;
}

//returns a new loat with loat n truncated to size l
loat *truncateF(loat *n,int l) {
	loat *newn=shrinkF(n);
	if(l>newn->mantissa->length) return newn;
	char *newdigits=(char*)malloc(l*sizeof(char));
	int i;
	for(i=newn->mantissa->length-1;i+l>=newn->mantissa->length;--i)
		newdigits[i-newn->mantissa->length+l]=newn->mantissa->digits[i];
	free(newn->mantissa->digits);
	newn->mantissa->digits=newdigits;
	newn->exponent+=newn->mantissa->length-l;
	newn->mantissa->length=l;
	return newn;
}

//returns a new loat with loat n padded to size l
loat *padF(loat *n,int l) {
	loat *newn=shrinkF(n);
	if(l<newn->mantissa->length) return newn;
	char *newdigits=(char*)malloc(l*sizeof(char));
	int i;
	for(i=newn->mantissa->length-1;i>=0;--i) newdigits[i-newn->mantissa->length+l]=newn->mantissa->digits[i];
	for(i=l-newn->mantissa->length-1;i>=0;--i) newdigits[i]=0;
	free(newn->mantissa->digits);
	newn->mantissa->digits=newdigits;
	newn->exponent+=n->mantissa->length-l;
	newn->mantissa->length=l;
	return newn;
}

//prints loat n
void printF(loat *n) {
	if(n->mantissa->sign==PLUS) printf("+");
	else printf("-");
	int i=n->mantissa->length;
	while(i) printf("%c",'0'+n->mantissa->digits[--i]);
	printf("e%d",n->exponent);
}

//deletes loat n
void deleteF(loat *n) {
	deleteI(n->mantissa);
	free(n);
}

//destroys loat array n of length l
void destroyF(loat **n,int l) {
	int i;
	for(i=0;i<l;++i) deleteF(*(n+i));
	free(n);
}

//returns comparison of loat a and b, assumed trimmed and +ve
char _compareF(loat *a,loat *b) {
	if(a->mantissa->length+a->exponent>b->mantissa->length+b->exponent) return GREATER;
	if(b->mantissa->length+b->exponent>a->mantissa->length+a->exponent) return LESS;
	int i;
	for(i=0;(i<a->mantissa->length)&&(i<b->mantissa->length);++i)
		if(a->mantissa->digits[a->mantissa->length-i-1]>b->mantissa->digits[b->mantissa->length-i-1]) return GREATER;
		else if(a->mantissa->digits[a->mantissa->length-i-1]<b->mantissa->digits[b->mantissa->length-i-1]) return LESS;
	if(a->mantissa->length>b->mantissa->length) return GREATER;
	if(b->mantissa->length>a->mantissa->length) return LESS;
	return EQUAL;
}

//returns comparison of loat a and b, assumed trimmed
char compareF(loat *a,loat *b) {
	lint *zero=createI(1);
	if((_compareI(a->mantissa,zero)==EQUAL)&&(_compareI(b->mantissa,zero)==EQUAL)) {
		deleteI(zero);
		return EQUAL;
	}
	if(_compareI(a->mantissa,zero)==EQUAL) {
		deleteI(zero);
		if(b->mantissa->sign==PLUS) return LESS;
		else return GREATER;
	}
	if(_compareI(b->mantissa,zero)==EQUAL) {
		deleteI(zero);
		if(a->mantissa->sign==MINUS) return LESS;
		else return GREATER;
	}
	deleteI(zero);
	if(a->mantissa->sign==PLUS)
		if(b->mantissa->sign==MINUS) return GREATER;
		else return _compareF(a,b);
	else
		if(b->mantissa->sign==PLUS) return LESS;
		else switch(_compareF(a,b)) {
				case GREATER: return LESS;
				case LESS: return GREATER;
				case EQUAL: return EQUAL;
			}
}

//returns new loat with sum of loat a and b, assumed trimmed and +ve
loat *_additionF(loat *a,loat *b) {
	loat *sum;
	lint *msum;
	if(a->exponent>b->exponent) {
		sum=alignF(a,b->exponent);
		if(b->mantissa->length>sum->mantissa->length) msum=_additionI(b->mantissa,sum->mantissa);
		else msum=_additionI(sum->mantissa,b->mantissa);
	}
	else {
		sum=alignF(b,a->exponent);
		if(a->mantissa->length>sum->mantissa->length) msum=_additionI(a->mantissa,sum->mantissa);
		else msum=_additionI(sum->mantissa,a->mantissa);
	}
	deleteI(sum->mantissa);
	sum->mantissa=msum;
	return sum;
}

//returns new loat with absolute difference of loat a and b, assumed trimmed and +ve
loat *_subtractionF(loat *a,loat *b) {
	loat *diff;
	lint *mdiff;
	if(a->exponent>b->exponent) {
		diff=alignF(a,b->exponent);
		if(_compareI(b->mantissa,diff->mantissa)==GREATER) mdiff=_subtractionI(b->mantissa,diff->mantissa);
		else mdiff=_subtractionI(diff->mantissa,b->mantissa);
	}
	else {
		diff=alignF(b,a->exponent);
		if(_compareI(a->mantissa,diff->mantissa)==GREATER) mdiff=_subtractionI(a->mantissa,diff->mantissa);
		else mdiff=_subtractionI(diff->mantissa,a->mantissa);
	}
	deleteI(diff->mantissa);
	diff->mantissa=mdiff;
	return diff;
}

//returns new loat with product of loat a and b, assumed trimmed and +ve
loat *_kmultiplicationF(loat *a,loat *b) {
	loat *prod=(loat*)malloc(sizeof(loat));
	if(a->mantissa->length>b->mantissa->length) prod->mantissa=_multiplicationI(a->mantissa,b->mantissa);
	else prod->mantissa=_multiplicationI(b->mantissa,a->mantissa);
	prod->exponent=a->exponent+b->exponent;
	return prod;
}

//returns new loat array with quotient and remainder of loat a and b accurate to p digits, assumed trimmed and +ve
loat *_divisionF(loat *a,loat *b,int p) {
	loat *quo=(loat*)malloc(sizeof(loat)),*pad=padF(a,p+b->mantissa->length);
	lint **res=_divisionI(pad->mantissa,b->mantissa);
	quo->mantissa=*res;
	quo->exponent=pad->exponent-b->exponent;
	deleteF(pad);
	deleteI(*(res+1));
	free(res);
	return quo;
}

//returns new loat with sum of loat a and b, assumed trimmed
loat *additionF(loat *a,loat *b) {
	if(a->mantissa->sign==PLUS)
		if(b->mantissa->sign==PLUS) return _additionF(a,b);
		else
			if(_compareF(a,b)==GREATER) return _subtractionF(a,b);
			else return negateF(_subtractionF(a,b));
	else
		if(b->mantissa->sign==MINUS) return negateF(_additionF(a,b));
		else
			if(_compareF(a,b)==GREATER) return negateF(_subtractionF(a,b));
			else return _subtractionF(a,b);
}

//returns new loat with difference of loat a and b, assumed trimmed
loat *subtractionF(loat *a,loat *b) {
	lint *zero=createI(1);
	if((_compareI(a->mantissa,zero)==EQUAL)&&(_compareI(b->mantissa,zero)==EQUAL)) {
		deleteI(zero);
		return createF(1);
	}
	if(_compareI(a->mantissa,zero)==EQUAL) {
		deleteI(zero);
		loat *diff=cloneF(b);
		return negateF(diff);
	}
	if(_compareI(b->mantissa,zero)==EQUAL) {
		deleteI(zero);
		return cloneF(a);
	}
	deleteI(zero);
	if(a->mantissa->sign==PLUS)
		if(b->mantissa->sign==MINUS) return _additionF(a,b);
		else
			if(_compareF(a,b)==GREATER) return _subtractionF(a,b);
			else return negateF(_subtractionF(a,b));
	else
		if(b->mantissa->sign==PLUS) return negateF(_additionF(a,b));
		else
			if(_compareF(a,b)==GREATER) return negateF(_subtractionF(a,b));
			else return _subtractionF(a,b);
}

//returns new loat with difference of loat a and b, assumed trimmed
loat *multiplicationF(loat *a,loat *b) {
	if(a->mantissa->sign==b->mantissa->sign) return _kmultiplicationF(a,b);
	return negateF(_kmultiplicationF(a,b));
}

//returns new loat array with quotient and remainder of loat a and b accurate to p digits, assumed trimmed
loat *divisionF(loat *a,loat *b,int p) {
	loat *quo=_divisionF(a,b,p);
	if(a->mantissa->sign==b->mantissa->sign) quo->mantissa->sign=PLUS;
	else quo->mantissa->sign=MINUS;
	return quo;
}

//returns new loat of n random digits in the range[0,1]
loat *randoneF(int n) {
	loat *random=createF(n);
	int i;
	for(i=0;i<n;++i) random->mantissa->digits[i]=rand()%10;
	random->exponent=-n;
	return random;
}

//returns new loat array of size m of n random digits
loat **randlistF(int m,int n) {
	loat **list=(loat**)malloc(m*sizeof(loat*));
	int i;
	srand(time(NULL));
	for(i=0;i<m;++i) list[i]=randoneF(n);
	return list;
}
