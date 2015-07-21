#ifndef LINT_H
#define LINT_H

#define PLUS 1
#define MINUS -1
#define GREATER 1
#define EQUAL 0
#define LESS -1
#define swap(T,a,b) { T t=a; a=b; b=t; }
#define KARATSUBA_SPLIT 50

typedef struct {
	char *digits;
	int length;
	char sign;
} lint;

lint *createI(int);
lint *cloneI(lint*);
int len(char*);
lint *convertI(char*);
lint *negateI(lint*);
lint *shiftI(lint*,int);
lint *shrinkI(lint*);
lint *truncateI(lint*,int);
void printI(lint*);
void deleteI(lint*);
void destroyI(lint**,int);
char _compareI(lint*,lint*);
char compareI(lint*,lint*);
lint *_additionI(lint*,lint*);
lint *_subtractionI(lint*,lint*);
lint *intoI(lint*,char);
lint *_multiplicationI(lint*,lint*);
lint **_splitI(lint*,int);
lint **splitI(lint*,int);
lint *_kmultiplicationI(lint*,lint*);
lint **_divisionI(lint*,lint*);
lint *_powerI(lint*,lint*);
lint *additionI(lint*,lint*);
lint *subtractionI(lint*,lint*);
lint *multiplicationI(lint*,lint*);
lint **divisionI(lint*,lint*);
lint *powerI(lint*,lint*);
lint *randoneI(int);
lint **randlistI(int,int);

#endif
