#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//lint sign constants
#define PLUS 1
#define MINUS -1

//lint comparison constants
#define GREATER 1
#define EQUAL 0
#define LESS -1

//lint multiplication algorithm switch constant
#define KARATSUBA_SPLIT 50

//swap macro
#define swap(T,a,b) { T t=a; a=b; b=t; }

//lint data structure
typedef struct {
	char *digits;
	int length;
	char sign;
} lint;

//returns a new lint initialized as + with 0s of length l
lint *createI(int l) {
	lint *n=(lint*)malloc(sizeof(lint));
	n->digits=(char*)malloc(l*sizeof(char));
	n->length=l;
	n->sign=PLUS;
	memset(n->digits,0,l*sizeof(char));
	return n;
}

//returns a new lint cloned from lint a
lint *cloneI(lint *a) {
	lint *n=createI(a->length);
	int i;
	for(i=0;i<a->length;++i) n->digits[i]=a->digits[i];
	n->sign=a->sign;
	return n;
}

//returns the length of the null terminated string s
int len(char *s) {
	int i;
	for(i=0;s[i]!='\0';++i);
	return i;
}

//returns a new lint as + with absolute value of null terminated string s
lint *convertI(char *s) {
	lint *n=createI(len(s));
	int i,j;
	for(i=n->length-1,j=0;j<n->length;--i,++j) n->digits[i]=s[j]-'0';
	return n;
}

//returns lint n with sign negated
lint *negateI(lint *n) {
	n->sign=-n->sign;
	return n;
}

//returns a new lint with lint n leftshifted by l places
lint *shiftI(lint *n,int l) {
	lint *newn=createI(n->length+l);
	int i=0,j=l;
	while(i<n->length) newn->digits[j++]=n->digits[i++];
	return newn;
}

//returns a new lint with lint n trimmed of leading 0s
lint *shrinkI(lint *n) {
	int i;
	for(i=n->length-1;(i>=0)&&(n->digits[i]==0);--i);
	if(i<0) return createI(1);
	lint *newn=createI(i+1);
	newn->sign=n->sign;
	for(;i>=0;--i) newn->digits[i]=n->digits[i];
	return newn;
}

//returns a new lint with lint n truncated to size l
lint *truncateI(lint *n,int l) {
	lint *newn=shrinkI(n);
	if(l>newn->length) return newn;
	char *newdigits=(char*)malloc(l*sizeof(char));
	int i;
	for(i=newn->length-1;i+l>=newn->length;--i) newdigits[i-newn->length+l]=newn->digits[i];
	free(newn->digits);
	newn->digits=newdigits;
	newn->length=l;
	return newn;
}

//prints lint n
void printI(lint *n) {
	if(n->sign==PLUS) printf("+");
	else printf("-");
	int i=n->length;
	while(i) printf("%c",'0'+n->digits[--i]);
}

//deletes lint n
void deleteI(lint *n) {
	free(n->digits);
	free(n);
}

//destroys lint array n of length l
void destroyI(lint **n,int l) {
	int i;
	for(i=0;i<l;++i) deleteI(*(n+i));
	free(n);
}

//returns comparison of lint a and b, assumed trimmed and +ve
char _compareI(lint *a,lint *b) {
	if(a->length>b->length) return GREATER;
	if(b->length>a->length) return LESS;
	int i;
	for(i=a->length-1;i>=0;--i)
		if(a->digits[i]>b->digits[i]) return GREATER;
		else if(a->digits[i]<b->digits[i]) return LESS;
	return EQUAL;
}

//returns comparison of lint a and b, assumed trimmed
char compareI(lint *a,lint *b) {
	if(a->sign==PLUS)
		if(b->sign==MINUS) return GREATER;
		else return _compareI(a,b);
	else
		if(b->sign==PLUS) return LESS;
		else switch(_compareI(a,b)) {
				case GREATER: return LESS;
				case LESS: return GREATER;
				case EQUAL: return EQUAL;
			}
}

//returns new lint with sum of lint a and b, assumed trimmed and +ve with a>b
lint *_additionI(lint *a,lint *b) {
	int i,car,s;
	lint *sum=createI(a->length+1);
	for(i=car=0;i<b->length;++i) {
		s=car+a->digits[i]+b->digits[i];
		sum->digits[i]=s%10;
		car=s>9?1:0;
	}
	for(i=b->length;i<a->length;++i) {
		s=car+a->digits[i];
		sum->digits[i]=s%10;
		car=s>9?1:0;
	}
	sum->digits[a->length]=car;
	return sum;
}

//returns new lint with difference of lint a and b, assumed trimmed and +ve with a>b
lint *_subtractionI(lint *a,lint *b) {
	int i,bor,d;
	lint *diff=createI(a->length);
	for(i=bor=0;i<b->length;++i) {
		d=a->digits[i]-b->digits[i]-bor;
		diff->digits[i]=(10+d)%10;
		bor=d<0?1:0;
	}
	for(i=b->length;(i<a->length)&&(bor);++i) {
		d=a->digits[i]-bor;
		diff->digits[i]=(10+d)%10;
		bor=d<0?1:0;
	}
	for(;i<a->length;++i) diff->digits[i]=a->digits[i];
	return diff;
}

//returns new lint with product of lint n and m, assumed +ve
lint *intoI(lint *n,char m) {
	lint *prod=createI(n->length+1);
	int i,c;
	for(i=c=0;i<n->length;++i) {
		prod->digits[i]+=(c+n->digits[i]*m)%10;
		c=(c+n->digits[i]*m)/10;
	}
	prod->digits[n->length]=c;
	return prod;
}

//returns new lint with product of lint a and b, assumed trimmed and +ve with a>b
lint *_multiplicationI(lint *a,lint *b) {
	lint *prod=createI(a->length),*tmp1,*tmp2,*tmp3,*tmp4;
	int i;
	for(i=b->length-1;i>=0;--i) {
		tmp1=shiftI(prod,1);
		tmp2=intoI(a,b->digits[i]);
		tmp3=_additionI(tmp1,tmp2);
		tmp4=shrinkI(tmp3);
		deleteI(prod);
		deleteI(tmp3);
		deleteI(tmp2);
		deleteI(tmp1);
		prod=tmp4;
	}
	return prod;
}

//returns new lint array with tail l digits of and head digits of lint n, assumed to have non0 head
lint **_splitI(lint *n,int l) {
	lint **newn=(lint **)malloc(2*sizeof(lint *));
	*(newn+1)=createI(n->length-l);
	*newn=createI(l);
	int i;
	for(i=0;i<l;++i) (*newn)->digits[i]=n->digits[i];
	for(i=l;i<n->length;++i) (*(newn+1))->digits[i-l]=n->digits[i];
	return newn;
}

//returns new lint array with tail l digits of and head digits of lint n
lint **splitI(lint *n,int l) {
	if(l<n->length) return _splitI(n,l);
	lint **newn=(lint**)malloc(2*sizeof(lint*));
	*newn=cloneI(n);
	*(newn+1)=createI(1);
	return newn;
}

//returns new lint with product of lint a and b, assumed trimmed and +ve
lint *_kmultiplicationI(lint *a,lint *b) {
	lint *tmp1=shrinkI(a),*tmp2=shrinkI(b),*tmp3,*tmp4,*tmp5;
	if(_compareI(tmp1,tmp2)==LESS) swap(lint *,tmp1,tmp2);
	if(tmp2->length<KARATSUBA_SPLIT) {
		tmp3=_multiplicationI(tmp1,tmp2);
		tmp4=shrinkI(tmp3);
		deleteI(tmp1);
		deleteI(tmp2);
		deleteI(tmp3);
		return tmp4;
	}
	int l=tmp2->length/2;
	lint **ktmp1=_splitI(tmp1,l),**ktmp2=_splitI(tmp2,l);
	deleteI(tmp1);
	deleteI(tmp2);
	tmp1=_kmultiplicationI(*ktmp1,*ktmp2);
	tmp2=_additionI(*(ktmp1+1),*ktmp1);
	tmp3=_additionI(*(ktmp2+1),*ktmp2);
	tmp4=_kmultiplicationI(tmp2,tmp3);
	tmp5=_kmultiplicationI(*(ktmp1+1),*(ktmp2+1));
	deleteI(tmp2);
	deleteI(tmp3);
	destroyI(ktmp1,2);
	destroyI(ktmp2,2);
	tmp2=shrinkI(tmp1);
	tmp3=shrinkI(tmp5);
	deleteI(tmp1);
	deleteI(tmp5);
	tmp1=tmp2;
	tmp5=tmp3;
	tmp2=_subtractionI(tmp4,tmp1);
	tmp3=_subtractionI(tmp2,tmp5);
	deleteI(tmp2);
	deleteI(tmp4);
	tmp2=shiftI(tmp5,2*l);
	tmp4=shiftI(tmp3,l);
	deleteI(tmp3);
	deleteI(tmp5);
	tmp3=shrinkI(tmp1);
	deleteI(tmp1);
	tmp1=tmp3;
	tmp3=_additionI(tmp4,tmp1);
	deleteI(tmp1);
	tmp1=shrinkI(tmp3);
	deleteI(tmp3);
	tmp3=tmp1;
	tmp1=_additionI(tmp2,tmp3);
	deleteI(tmp2);
	deleteI(tmp3);
	deleteI(tmp4);
	return tmp1;
}

//returns new lint array with quotient and remainder of lint a and b, assumed trimmed and +ve
lint **_divisionI(lint *a,lint *b) {
	lint *quo=createI(1),*div=createI(1),*tmp1,*tmp2,*tmp3,*tmp4;
	int i;
	for(i=0;i<a->length;++i) {
		tmp1=shiftI(quo,1);
		tmp2=shrinkI(tmp1);
		tmp3=shiftI(div,1);
		tmp4=shrinkI(tmp3);
		deleteI(quo);
		deleteI(tmp1);
		deleteI(div);
		deleteI(tmp3);
		quo=tmp2;
		div=tmp4;
		div->digits[0]=a->digits[a->length-1-i];
		while(_compareI(div,b)!=LESS) {
			tmp1=_subtractionI(div,b);
			tmp2=shrinkI(tmp1);
			deleteI(tmp1);
			deleteI(div);
			div=tmp2;
			quo->digits[0]++;
		}
	}
	lint **res=(lint**)malloc(2*sizeof(lint*));
	*res=quo;
	*(res+1)=div;
	return res;
}

//returns new lint with lint a to the power b, assumed trimmed and +ve
lint *_powerI(lint *a,lint *b) {
	lint *one=convertI("1"),*two,*mul,**div,*pow,*tmp;
	switch(_compareI(b,one)) {
		case GREATER:
			two=convertI("2");
			div=_divisionI(b,two);
			deleteI(two);
			switch(_compareI(*(div+1),one)) {
				case LESS:
					deleteI(one);
					pow=_powerI(a,*div);
					destroyI(div,2);
					mul=_kmultiplicationI(pow,pow);
					deleteI(pow);
					return mul;
				case EQUAL:
					deleteI(one);
					pow=_powerI(a,*div);
					destroyI(div,2);
					mul=_kmultiplicationI(pow,pow);
					deleteI(pow);
					tmp=_kmultiplicationI(mul,a);
					deleteI(mul);
					return tmp;
			}
		case EQUAL:
			deleteI(one);
			return cloneI(a);
		case LESS:
			return one;
	}
}

//returns new lint with sum of lint a and b, assumed trimmed
lint *additionI(lint *a,lint *b) {
	if(a->sign==PLUS)
		if(b->sign==PLUS)
			if(_compareI(a,b)==GREATER) return _additionI(a,b);
			else return _additionI(b,a);
		else
			if(_compareI(a,b)==GREATER) return _subtractionI(a,b);
			else return negateI(_subtractionI(b,a));
	else
		if(b->sign==MINUS)
			if(_compareI(a,b)==GREATER) return negateI(_additionI(a,b));
			else return negateI(_additionI(b,a));
		else
			if(_compareI(a,b)==GREATER) return negateI(_subtractionI(a,b));
			else return _subtractionI(b,a);
}

//returns new lint with difference of lint a and b, assumed trimmed
lint *subtractionI(lint *a,lint *b) {
	if(a->sign==PLUS)
		if(b->sign==PLUS)
			if(_compareI(a,b)==GREATER) return _subtractionI(a,b);
			else return negateI(_subtractionI(b,a));
		else
			if(_compareI(a,b)==GREATER) return _additionI(a,b);
			else return _additionI(b,a);
	else
		if(b->sign==PLUS)
			if(_compareI(a,b)==GREATER) return negateI(_additionI(a,b));
			else return negateI(_additionI(b,a));
		else
			if(_compareI(a,b)==GREATER) return negateI(_subtractionI(a,b));
			else return _subtractionI(b,a);
}

//returns new lint with difference of lint a and b, assumed trimmed
lint *multiplicationI(lint *a,lint *b) {
	if(a->sign==b->sign) return _kmultiplicationI(a,b);
	return negateI(_kmultiplicationI(a,b));
}

//returns new lint array with quotient and remainder of lint a and b, assumed trimmed
lint **divisionI(lint *a,lint *b) {
	lint **res=_divisionI(a,b),*tmp1,*tmp2;
	if(a->sign==MINUS) {
		tmp1=convertI("1");
		tmp2=additionI(*res,tmp1);
		deleteI(tmp1);
		deleteI(*res);
		*res=tmp2;
		tmp1=_subtractionI(b,*(res+1));
		deleteI(*(res+1));
		*(res+1)=tmp1;
	}
	if(a->sign!=b->sign) negateI(*res);
	return res;
}

//returns new lint with lint a to the power b, assumed trimmed
lint *powerI(lint *a,lint *b) {
	if(b->sign==MINUS) return createI(1);
	if(a->sign==PLUS) return _powerI(a,b);
	lint *one=convertI("1"),*two=convertI("2"),**div=_divisionI(b,two);
	switch(_compareI(*(div+1),one)) {
		case LESS:
			destroyI(div,2);
			deleteI(one);
			deleteI(two);
			return _powerI(a,b);
		case EQUAL:
			destroyI(div,2);
			deleteI(one);
			deleteI(two);
			return negateI(_powerI(a,b));
	}
}

//returns new lint of n random digits
lint *randoneI(int n) {
	lint *random=createI(n);
	int i;
	for(i=0;i<n;++i) random->digits[i]=rand()%10;
	return random;
}

//returns new lint array of size m of n random digits
lint **randlistI(int m,int n) {
	lint **list=(lint**)malloc(m*sizeof(lint*));
	int i;
	srand(time(NULL));
	for(i=0;i<m;++i) list[i]=randoneI(n);
	return list;
}
