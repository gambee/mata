/* lexer.l
 * 
 * Max Gambee
 * Copyright 2018
 */

%option noyywrap 

%{

#include <stdio.h>
#define TABLE_DEBUG
#include "table.h"

int line_no;
struct sym_tab table;
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

"//"					{ //skip single line c-style comments
						int c;
						while((c = input()) != '\n');
						++line_no;
						}

[:~]					{printf("Found Operator:\t%s\n", yytext);}
[A-Za-z]+[A-Za-z0-9]*	{tab_add(&table, yytext, line_no);}

"[".*[^\\]"]"			{printf("Found Range:\t%s\n", yytext);}

[ \t]					;//ignore white space
[\n]					{++line_no;}//so that line_no works?

%%


int main(int argc, char** argv)
{
	printf("Initializing Symbol Table.....%s\n",
		tab_init(&table) ? (exit(1),1) ? "FAILED!\n":"\n":"SUCCESS\n");
	line_no = 1;
	yylex();
	tab_printall(&table);
	return 0;
}

