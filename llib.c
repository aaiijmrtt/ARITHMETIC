#include<stdlib.h>
#include"loat.h"

//returns loat n with sign positive
loat *absolute(loat *n) {
	n->mantissa->sign=PLUS;
	return n;
}

//returns new loat solution of equation f given initial range to desired accuracy using Bisection algorithm
loat *bisection(loat *initiala,loat *initialb,loat *accuracy,loat *(*f)(loat*)) {
	loat *a=cloneF(initiala),*b=cloneF(initialb),*c,*fa=f(a),*fb=f(b),*fc,*ei=convertF("1"),*zero=convertF("0"),*two=convertF("2"),*tmp1,*tmp2;
	do {
		tmp1=additionF(a,b);
		tmp2=shrinkF(tmp1);
		deleteF(tmp1);
		tmp1=divisionF(tmp2,two,-accuracy->exponent);
		deleteF(tmp2);
		c=shrinkF(tmp1);
		deleteF(tmp1);
		tmp1=f(c);
		fc=shrinkF(tmp1);
		deleteF(tmp1);
		if(compareF(fc,zero)==EQUAL) break;
		tmp1=subtractionF(b,a);
		tmp2=shrinkF(tmp1);
		deleteF(tmp1);
		tmp1=divisionF(tmp2,two,-accuracy->exponent);
		deleteF(tmp2);
		deleteF(ei);
		ei=shrinkF(tmp1);
		deleteF(tmp1);
		if(fa->mantissa->sign==fc->mantissa->sign) {
			deleteF(fa);
			fa=fc;
			deleteF(a);
			a=c;
		}
		else {
			deleteF(fb);
			fb=fc;
			deleteF(b);
			b=c;
		}
	} while(compareF(ei,accuracy)==GREATER);
	deleteF(a);
	deleteF(b);
	deleteF(fa);
	deleteF(fb);
	deleteF(fc);
	deleteF(ei);
	deleteF(zero);
	deleteF(two);
	tmp1=truncateF(c,-accuracy->exponent);
	deleteF(c);
	c=shrinkF(tmp1);
	deleteF(tmp1);
	return c;
}

//returns new loat solution of equation f given derivative df and initial guess to desired accuracy using Newton-Raphson's algorithm
loat *newtonraphson(loat *initial,loat *accuracy,loat *(*f)(loat*),loat *(*df)(loat*)) {
	loat *xi,*ximinus1=cloneF(initial),*fxi,*dfxi,*ei=convertF("1"),*zero=convertF("0"),*tmp1,*tmp2;
	do {
		xi=ximinus1;
		tmp1=f(xi);
		fxi=shrinkF(tmp1);
		deleteF(tmp1);
		tmp1=df(xi);
		dfxi=shrinkF(tmp1);
		deleteF(tmp1);
		if(compareF(dfxi,zero)==EQUAL) break;
		tmp1=divisionF(fxi,dfxi,-accuracy->exponent);
		tmp2=shrinkF(tmp1);
		deleteF(tmp1);
		tmp1=subtractionF(xi,tmp2);
		deleteF(tmp2);
		ximinus1=shrinkF(tmp1);
		deleteF(tmp1);
		tmp1=subtractionF(xi,ximinus1);
		deleteF(xi);
		deleteF(fxi);
		deleteF(dfxi);
		tmp2=shrinkF(tmp1);
		deleteF(tmp1);
		deleteF(ei);
		ei=absolute(tmp2);
	} while(compareF(ei,accuracy)==GREATER);
	deleteF(ei);
	deleteF(zero);
	tmp1=truncateF(ximinus1,-accuracy->exponent);
	deleteF(ximinus1);
	ximinus1=shrinkF(tmp1);
	deleteF(tmp1);
	return ximinus1;
}

//returns new loat matrix inverse of loat matrix given its rank to desired accuracy using Gauss-Jordan's algorithm
loat ***gaussjordan(int rank,loat *matrix[rank][rank],loat *accuracy) {
	int i,j,k;
	loat *tmp1,*tmp2,*tmp3,***inversematrix=(loat***)malloc(rank*rank*sizeof(loat**)),***clone=(loat***)malloc(rank*rank*sizeof(loat**));
	for(i=0;i<rank;++i) {
		*(inversematrix+i)=(loat**)malloc(rank*sizeof(loat*));
		*(clone+i)=(loat**)malloc(rank*sizeof(loat*));
		for(j=0;j<rank;++j) {
			if(j==i) *(*(inversematrix+i)+j)=convertF("1");
			else *(*(inversematrix+i)+j)=convertF("0");
			*(*(clone+i)+j)=cloneF(matrix[i][j]);
		}
	}
	for(k=0;k<rank-1;++k)
		for(i=k+1;i<rank;++i) {
			tmp1=divisionF(*(*(clone+i)+k),*(*(clone+k)+k),-accuracy->exponent);
			tmp2=shrinkF(tmp1);
			deleteF(tmp1);
			for(j=0;j<rank;++j) {
				tmp1=multiplicationF(*(*(clone+k)+j),tmp2);
				tmp3=shrinkF(tmp1);
				deleteF(tmp1);
				tmp1=subtractionF(*(*(clone+i)+j),tmp3);
				deleteF(*(*(clone+i)+j));
				deleteF(tmp3);
				*(*(clone+i)+j)=shrinkF(tmp1);
				deleteF(tmp1);
				tmp1=multiplicationF(*(*(inversematrix+k)+j),tmp2);
				tmp3=shrinkF(tmp1);
				deleteF(tmp1);
				tmp1=subtractionF(*(*(inversematrix+i)+j),tmp3);
				deleteF(*(*(inversematrix+i)+j));
				deleteF(tmp3);
				*(*(inversematrix+i)+j)=shrinkF(tmp1);
				deleteF(tmp1);
			}
			deleteF(tmp2);
		}
	for(k=rank-1;k>0;--k)
		for(i=k-1;i>=0;--i) {
			tmp1=divisionF(*(*(clone+i)+k),*(*(clone+k)+k),-accuracy->exponent);
			tmp2=shrinkF(tmp1);
			deleteF(tmp1);
			for(j=0;j<rank;++j) {
				tmp1=multiplicationF(*(*(clone+k)+j),tmp2);
				tmp3=shrinkF(tmp1);
				deleteF(tmp1);
				tmp1=subtractionF(*(*(clone+i)+j),tmp3);
				deleteF(*(*(clone+i)+j));
				deleteF(tmp3);
				*(*(clone+i)+j)=shrinkF(tmp1);
				deleteF(tmp1);
				tmp1=multiplicationF(*(*(inversematrix+k)+j),tmp2);
				tmp3=shrinkF(tmp1);
				deleteF(tmp1);
				tmp1=subtractionF(*(*(inversematrix+i)+j),tmp3);
				deleteF(*(*(inversematrix+i)+j));
				deleteF(tmp3);
				*(*(inversematrix+i)+j)=shrinkF(tmp1);
				deleteF(tmp1);
			}
			deleteF(tmp2);
		}
	for(i=0;i<rank;++i) {
		tmp1=cloneF(*(*(clone+i)+i));
		for(j=0;j<rank;++j) {
			tmp2=divisionF(*(*(clone+i)+j),tmp1,-accuracy->exponent);
			deleteF(*(*(clone+i)+j));
			*(*(clone+i)+j)=shrinkF(tmp2);
			deleteF(tmp2);
			tmp2=divisionF(*(*(inversematrix+i)+j),tmp1,-accuracy->exponent);
			deleteF(*(*(inversematrix+i)+j));
			*(*(inversematrix+i)+j)=shrinkF(tmp2);
			deleteF(tmp2);
		}
		deleteF(tmp1);
	}
	for(i=0;i<rank;++i) destroyF(*(clone+i),rank);
	free(clone);
	for(i=0;i<rank;++i)
		for(j=0;j<rank;++j) {
			tmp1=truncateF(*(*(inversematrix+i)+j),-accuracy->exponent);
			deleteF(*(*(inversematrix+i)+j));
			*(*(inversematrix+i)+j)=shrinkF(tmp1);
			deleteF(tmp1);
		}
	return inversematrix;
}
