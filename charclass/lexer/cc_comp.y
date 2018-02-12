/* cc_comp.y
 *
 * Max Gambee
 * Copyright 2018
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../charclass.h"
#define PRINT(STR, ARG) printf("BISON: " #STR " %d\n", ARG )

extern void yysetbuf(char**);
extern int yylex(void);
void yyerror(char* input){printf("%s\n", input);}
charclass cclass;
int compliment;
%}

%union{
	int singleton;
}

%token STDSTART CMPSTART STDEND TO
%token <singleton> SINGLETON

%%

CharClass:
	Head Tail
Head:
	STDSTART	{compliment = 0;}
	| CMPSTART	{compliment = 1;}
Tail:
	Expression Tail | Expression End
Expression:
	SINGLETON {cc_set(&cclass, $1);}
	| Range
Range:
	SINGLETON TO SINGLETON
	{
		int i;
		for(i=$1;i<=$3;i++)
			cc_set(&cclass, i);
	}
End:
	STDEND
	{
		int i;
		if(compliment)
			for(i=0;i<32;i++)
				cclass.member[i] = ~range.member[i];
	}
%%


int main(void)
{
	char input[501];
	char *substr = input;
	int singleton, state, token;


	do{
		cc_init(&cclass);
		printf("Enter Character Class: ");
		scanf(" %s", input);
		substr = input;
		yysetbuf(&substr);
		cc_init(&cclass);
		yyparse();
		cc_printbits(&cclass);
	}while(strcmp(input, "quit"));

	return 0;
}
