/* lexer.l
 * 
 * Max Gambee
 * Copyright 2018
 */

%option noyywrap yylineno

%{

#include <stdio.h>

%}

%%
"/*"					{ /*skip multiline c-style comments*/
						int c;
						while((c = input()) != 0)
						{
							if(c == '\n')
								++line_no;
							else if(c == '*')
							{
								if((c = input()) == '/')
									break;
								unput(c);
							}}}

"//"					{//skip single line c-style comments
						int c;
						while((c = input()) != '\n');
						++line_no;}

[:~]					{printf("Found Operator:\t%s\n", yytext);}
[A-Za-z]+[A-Za-z0-9]*	{printf("Found Identifier:\t%s\n", yytext);}

"[".*[^\\]"]"			{printf("Found Range:\t%s\n", yytext);}

[ \t]					//ignore white space

%%


int main(int argc, char** argv)
{
	yylinno = 1;
	yylex();
	return 0;
}

