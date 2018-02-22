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


extern struct CClass* mk_CClass(char* cclass, int line);
extern struct State* mk_State(char* state, int line);
extern int line_no;
extern struct sym_tab table;

%}

%%

[ \t]					{ }//ignore white space
[\n]					{++line_no;}//so that line_no works?

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

":"					{return DECL_OP;}
"~"					{return DEFLT_RNG;}


[_A-Za-z]+[_A-Za-z0-9]*	{//printf("Found Identifier:\t%s\nStrlen:\t%d\n", yytext, strlen(yytext));
						yylval.state = mk_State(yytext, line_no);
						return STATE;}

"[".*[^\\]"]"			{//printf("Found CClass:\t%s\n", yytext);
						yylval.cclass = mk_CClass(yytext, line_no);
						return CCLASS;}

%%
