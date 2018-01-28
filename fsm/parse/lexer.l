/* lexer.l
 * 
 * Max Gambee
 * Copyright 2018
 */

%option noyywrap 

%{

#include <stdio.h>
//#define TABLE_DEBUG
//#include "table.h"
#include "parser.tab.h"

extern int line_no;
extern struct sym_tab table;

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
[_A-Za-z]+[_A-Za-z0-9]*	{printf("Found Identifier:\t%s\n", yytext);}

"[".*[^\\]"]"			{printf("Found Range:\t%s\n", yytext);}

[ \t]					;//ignore white space
[\n]					{++line_no;}//so that line_no works?

%%

