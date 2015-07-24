#ARITHMETIC

Arbitrary Precision Arithmetic Library

The code in this repository presents an optimized, easy-to-use arbitrary
precision arithmetic library in C, for C. Use lint for integer arithmetic and
loat for floating point arithmetic. llib provides a library of functions. All
code has been checked against memory leaks.

**Instructions**:

1. **Including Header Files in Code**:

		#include"lint.h"
		#include"loat.h"
		#include"llib.h"

2. **Creating Object Code**:

		$gcc -c -o lint lint.c
		$gcc -c -o loat loat.c
		$gcc -c -o llib llib.c

3. **Linking Object Code**:

		$gcc lint loat llib -o code code.c

4. **Executing**:

		$./code

**Sample Code**:

1. **To multiply 2 random 50-digit numbers**:

		#include"lint.h"
		int main(int argc,char **argv) {
			lint **randomnumbers=randlistI(2,50);
			lint *product=multiplicationI(randomnumbers[0],randomnumbers[1]);
			printI(product);
			return 0;
		}

2. **To evaluate PI to 100-digits**:

		#include"loat.h"
		int main(int argc,char **argv) {
			loat *pi_numerator=convertF("22");
			loat *pi_denominator=convertF("7");
			loat *pi_hundred=divisionF(pi_numerator,pi_denominator,100);
			printF(pi_hundred);
			return 0;
		}

3. **To invert a matrix**:

		#include<stdio.h>
		#include"llib.h"
		int main(int argc,char **argv) {
			loat *matrix[3][3]={{convertF("1"),convertF("1"),convertF("1")},
				{convertF("1"),convertF("2"),convertF("3")},
				{convertF("1"),convertF("2"),convertF("4")}};
			loat *accuracy=convertF("1e-6");
			loat ***inverse=gaussjordan(3,matrix,accuracy);
			int i,j;
			for(i=0;i<3;++i) {
				for(j=0;j<3;++j) {
					printF(*(*(inverse+i)+j));
					printf("\t");
				}
				printf("\n");
			}
			return 0;
		}

**Note**:

* Requires gcc.
* loat depends on lint.
* llib depends on loat and lint.
