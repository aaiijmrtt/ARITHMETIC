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

//swap macro
#define swap(T,a,b) { T t=a; a=b; b=t; }

//lint data structure
typedef struct {
	char *digits;
	int length;
	char sign;
} lint;

//returns a new lint initialized as + with 0s of length l
lint *create(int l) {
	lint *n=(lint*)malloc(sizeof(lint));
	n->digits=(char*)malloc(l*sizeof(char));
	n->length=l;
	n->sign=PLUS;
	memset(n->digits,0,l*sizeof(char));
	return n;
}

//returns a new lint cloned from lint a
lint *clone(lint *a) {
	lint *n=create(a->length);
	int i;
	for(i=0;i<a->length;++i)
		n->digits[i]=a->digits[i];
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
lint *conv(char *s) {
	lint *n=create(len(s));
	int i,j;
	for(i=n->length-1,j=0;j<n->length;--i,++j)
		n->digits[i]=s[j]-'0';
	return n;
}

//returns lint n with sign negated
lint *neg(lint *n) {
	n->sign=-n->sign;
	return n;
}

//returns a new lint with lint n leftshifted by l places
lint *shift(lint *n,int l) {
	lint *newn=create(n->length+l);
	int i=0,j=l;
	while(i<n->length)
		newn->digits[j++]=n->digits[i++];
	return newn;
}

//returns a new lint with lint n trimmed of leading 0s
lint *shrink(lint *n) {
	int i=n->length;
	for(i=n->length-1;(i>=0)&&(n->digits[i]==0);--i);
	if(i<0)
		return create(1);
	lint *newn=create(i+1);
	newn->sign=n->sign;
	for(;i>=0;--i)
		newn->digits[i]=n->digits[i];
	return newn;
}

//prints lint n
void print(lint *n) {
	if(n->sign==PLUS)
		printf("+");
	else
		printf("-");
	int i=n->length;
	while(i)
		printf("%c",'0'+n->digits[--i]);
}

//deletes lint n
void del(lint *n) {
	free(n->digits);
	free(n);
}

//destroys lint array n of length l
void dest(lint **n,int l) {
	int i;
	for(i=0;i<l;++i)
		del(*(n+i));
	free(n);
}

//returns comparison of lint a and b, assumed trimmed and +ve
char _comp(lint *a,lint *b) {
	if(a->length>b->length)
		return GREATER;
	if(b->length>a->length)
		return LESS;
	int i;
	for(i=a->length-1;i>=0;--i)
		if(a->digits[i]>b->digits[i])
			return GREATER;
		else if(a->digits[i]<b->digits[i])
			return LESS;
	return EQUAL;
}

//returns comparison of lint a and b, assumed trimmed
char comp(lint *a,lint *b) {
	if(a->sign==PLUS)
		if(b->sign==MINUS)
			return GREATER;
		else
			return _comp(a,b);
	else
		if(b->sign==PLUS)
			return LESS;
		else
			switch(_comp(a,b)) {
				case GREATER:
					return LESS;
				case LESS:
					return GREATER;
				case EQUAL:
					return EQUAL;
			}
}

//returns new lint with sum of lint a and b, assumed trimmed and +ve with a>b
lint *_add(lint *a,lint *b) {
	int i,car,s;
	lint *sum=create(a->length+1);
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
lint *_sub(lint *a,lint *b) {
	int i,bor,d;
	lint *diff=create(a->length);
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
	for(;i<a->length;++i)
		diff->digits[i]=a->digits[i];
	return diff;
}

//returns new lint with product of lint n and m, assumed +ve
lint *into(lint *n,char m) {
	lint *prod=create(n->length+1);
	int i,c;
	for(i=c=0;i<n->length;++i) {
		prod->digits[i]+=(c+n->digits[i]*m)%10;
		c=(c+n->digits[i]*m)/10;
	}
	prod->digits[n->length]=c;
	return prod;
}

//returns new lint with product of lint a and b, assumed trimmed and +ve with a>b
lint *_mult(lint *a,lint *b) {
	lint *prod=create(a->length),*tmp1,*tmp2,*tmp3,*tmp4;
	int i;
	for(i=b->length-1;i>=0;--i) {
		tmp1=shift(prod,1);
		tmp2=into(a,b->digits[i]);
		tmp3=_add(tmp1,tmp2);
		tmp4=shrink(tmp3);
		del(prod);
		del(tmp3);
		del(tmp2);
		del(tmp1);
		prod=tmp4;
	}
	return prod;
}

//returns new lint array with tail l digits of and head digits of lint n, assumed to have non0 head
lint **_splt(lint *n,int l) {
	lint **newn=(lint **)malloc(2*sizeof(lint *));
	*(newn+1)=create(n->length-l);
	*newn=create(l);
	int i;
	for(i=0;i<l;++i)
		(*newn)->digits[i]=n->digits[i];
	for(i=l;i<n->length;++i)
		(*(newn+1))->digits[i-l]=n->digits[i];
	return newn;
}

//returns new lint array with tail l digits of and head digits of lint n
lint **splt(lint *n,int l) {
	if(l<n->length)
		return _splt(n,l);
	lint **newn=(lint **)malloc(2*sizeof(lint *));
	*newn=clone(n);
	*(newn+1)=create(1);
	return newn;
}

//returns new lint with product of lint a and b, assumed trimmed and +ve
lint *_kmult(lint *a,lint *b) {
	lint *tmp1=shrink(a),*tmp2=shrink(b),*tmp3,*tmp4,*tmp5;
	if(_comp(tmp1,tmp2)==LESS)
		swap(lint *,tmp1,tmp2);
	if(tmp2->length<50) {
		tmp3=_mult(tmp1,tmp2);
		tmp4=shrink(tmp3);
		del(tmp1);
		del(tmp2);
		del(tmp3);
		return tmp4;
	}
	int l=tmp2->length/2;
	lint **ktmp1=_splt(tmp1,l),**ktmp2=_splt(tmp2,l);
	del(tmp1);
	del(tmp2);
	tmp1=_kmult(*ktmp1,*ktmp2);
	tmp2=_add(*(ktmp1+1),*ktmp1);
	tmp3=_add(*(ktmp2+1),*ktmp2);
	tmp4=_kmult(tmp2,tmp3);
	tmp5=_kmult(*(ktmp1+1),*(ktmp2+1));
	del(tmp2);
	del(tmp3);
	dest(ktmp1,2);
	dest(ktmp2,2);
	tmp2=shrink(tmp1);
	tmp3=shrink(tmp5);
	del(tmp1);
	del(tmp5);
	tmp1=tmp2;
	tmp5=tmp3;
	tmp2=_sub(tmp4,tmp1);
	tmp3=_sub(tmp2,tmp5);
	del(tmp2);
	del(tmp4);
	tmp2=shift(tmp5,2*l);
	tmp4=shift(tmp3,l);
	del(tmp3);
	del(tmp5);
	tmp3=shrink(tmp1);
	del(tmp1);
	tmp1=tmp3;
	tmp3=_add(tmp4,tmp1);
	del(tmp1);
	tmp1=shrink(tmp3);
	del(tmp3);
	tmp3=tmp1;
	tmp1=_add(tmp2,tmp3);
	del(tmp2);
	del(tmp3);
	del(tmp4);
	return tmp1;
}

//returns new lint array with quotient and remainder of lint a and b, assumed trimmed and +ve
lint **_divd(lint *a,lint *b) {
	lint *quo=create(1),*div=create(1),*tmp1,*tmp2,*tmp3,*tmp4;
	int i;
	for(i=0;i<a->length;++i) {
		tmp1=shift(quo,1);
		tmp2=shrink(tmp1);
		tmp3=shift(div,1);
		tmp4=shrink(tmp3);
		del(quo);
		del(tmp1);
		del(div);
		del(tmp3);
		quo=tmp2;
		div=tmp4;
		div->digits[0]=a->digits[a->length-1-i];
		while(_comp(div,b)!=LESS) {
			tmp1=_sub(div,b);
			tmp2=shrink(tmp1);
			del(tmp1);
			del(div);
			div=tmp2;
			quo->digits[0]++;
		}
	}
	lint **res=(lint **)malloc(2*sizeof(lint *));
	*res=quo;
	*(res+1)=div;
	return res;
}

//returns new lint with lint a to the power b, assumed trimmed and +ve
lint *_pwr(lint *a,lint *b) {
	lint *one=conv("1"),*two,*mul,**div,*pow,*tmp;
	switch(_comp(b,one)) {
		case GREATER:
			two=conv("2");
			div=_divd(b,two);
			del(two);
			switch(_comp(*(div+1),one)) {
				case LESS:
					del(one);
					pow=_pwr(a,*div);
					dest(div,2);
					mul=_kmult(pow,pow);
					del(pow);
					return mul;
				case EQUAL:
					del(one);
					pow=_pwr(a,*div);
					dest(div,2);
					mul=_kmult(pow,pow);
					del(pow);
					tmp=_kmult(mul,a);
					del(mul);
					return tmp;
			}
		case EQUAL:
			del(one);
			return clone(a);
		case LESS:
			return one;
	}
}

//returns new lint with sum of lint a and b, assumed trimmed
lint *add(lint *a,lint *b) {
	if(a->sign==PLUS)
		if(b->sign==PLUS)
			if(_comp(a,b)==GREATER)
				return _add(a,b);
			else
				return _add(b,a);
		else
			if(_comp(a,b)==GREATER)
				return _sub(a,b);
			else
				return neg(_sub(b,a));
	else
		if(b->sign==MINUS)
			if(_comp(a,b)==GREATER)
				return neg(_add(a,b));
			else
				return neg(_add(b,a));
		else
			if(_comp(a,b)==GREATER)
				return neg(_sub(a,b));
			else
				return _sub(b,a);
}

//returns new lint with difference of lint a and b, assumed trimmed
lint *sub(lint *a,lint *b) {
	if(a->sign==PLUS)
		if(b->sign==PLUS)
			if(_comp(a,b)==GREATER)
				return _sub(a,b);
			else
				return neg(_sub(b,a));
		else
			if(_comp(a,b)==GREATER)
				return _add(a,b);
			else
				return _add(b,a);
	else
		if(b->sign==PLUS)
			if(_comp(a,b)==GREATER)
				return neg(_add(a,b));
			else
				return neg(_add(b,a));
		else
			if(_comp(a,b)==GREATER)
				return neg(_sub(a,b));
			else
				return _sub(b,a);
}

//returns new lint with difference of lint a and b, assumed trimmed
lint *mult(lint *a,lint *b) {
	if(a->sign==b->sign)
		return _kmult(a,b);
	return neg(_kmult(a,b));
}

//returns new lint array with quotient and remainder of lint a and b, assumed trimmed
lint **divd(lint *a,lint *b) {
	lint **res=_divd(a,b),*tmp1,*tmp2;
	if(a->sign==MINUS) {
		tmp1=conv("1");
		tmp2=add(*res,tmp1);
		del(tmp1);
		del(*res);
		*res=tmp2;
		tmp1=_sub(b,*(res+1));
		del(*(res+1));
		*(res+1)=tmp1;
	}
	if(a->sign!=b->sign)
		neg(*res);
	return res;
}

//returns new lint with lint a to the power b, assumed trimmed
lint *pwr(lint *a,lint *b) {
	if(b->sign==MINUS)
		return create(1);
	if(a->sign==PLUS)
		return _pwr(a,b);
	lint *one=conv("1"),*two=conv("2"),**div=_divd(b,two);
	switch(_comp(*(div+1),one)) {
		case LESS:
			dest(div,2);
			del(one);
			del(two);
			return _pwr(a,b);
		case EQUAL:
			dest(div,2);
			del(one);
			del(two);
			return neg(_pwr(a,b));
	}
}

//returns new lint of n random digits
lint *randone(int n) {
	lint *random=create(n);
	int i;
	for(i=0;i<n;++i)
		random->digits[i]=rand()%10;
	return random;
}

//returns new lint array of size m of n random digits
lint **randlist(int m,int n) {
	lint **list=(lint**)malloc(m*sizeof(lint*));
	int i;
	srand(time(NULL));
	for(i=0;i<m;++i)
		list[i]=randone(n);
	return list;
}
