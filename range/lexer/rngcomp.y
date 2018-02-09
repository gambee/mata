/* rngcomp.y
 *
 * Max Gambee
 * Copyright 2018
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "../range.h"

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
	SINGLETON {rngset(&range, yylval.singleton);}
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

int main(int argc, char** argv)
{
	rnginit(&range);
	yyparse();
	printf("rangstr:\t%s\n", rngexpr(&range));
	return 0;
}
