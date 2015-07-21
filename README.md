#ARITHMETIC

Arbitrary Precision Arithmetic Library

The code in this repository presents an optimized, easy-to-use arbitrary
precision arithmetic library in C, for C. Use lint for integer arithmetic and
loat for floating point arithmetic.

**Instructions**:

1. **Including Header Files in Code**:

		#include"lint.h"
		#include"loat.h"

2. **Creating Object Code**:

		$gcc -c -o lint lint.c
		$gcc -c -o loat loat.c

3. **Linking Object Code**:

		$gcc lint loat -o code code.c

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
		}

**Note**:

* Requires gcc.
* loat depends on lint.
