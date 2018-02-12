/* rngcomp.y
 *
 * Max Gambee
 * Copyright 2018
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../range.h"
#define PRINT(STR, ARG) printf("BISON: " #STR " %d\n", ARG )

extern void yysetbuf(char**);
extern int yylex(void);
void yyerror(char* input){printf("%s\n", input);}
rng_ind range;
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
	SINGLETON {rngset(&range, $1);}
	| Range
Range:
	SINGLETON TO SINGLETON
	{
		int i;
		for(i=$1;i<=$3;i++)
			rngset(&range, i);
	}
End:
	STDEND
	{
		int i;
		if(compliment)
			for(i=0;i<32;i++)
				range.member[i] = ~range.member[i];
	}
%%


int main(void)
{
	char input[501];
	char *substr = input;
	int singleton, state, token;


	do{
		rnginit(&range);
		printf("Enter Character Class: ");
		scanf(" %s", input);
		substr = input;
		yysetbuf(&substr);
		rnginit(&range);
		yyparse();
		rngprintbits(&range);
	}while(strcmp(input, "quit"));

	return 0;
}
