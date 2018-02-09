/* yycomp.l
 *
 * Max Gambee
 * Copyright 2018
 */

%option noyywrap
%x CHARCLASS CTLCHAR INTEGER INTCLASS ERROR


%{

#include <stdio.h>
#include <stdlib.h>
#include "rngcomp.tab.h"

extern YYSTYPE yylval;


%}

%%

<INITIAL>"[^"			{BEGIN CHARCLASS; return CMPSTART;}
<INITIAL>"["			{BEGIN CHARCLASS; return STDSTART;}
<INITIAL>(.|\n)			{printf("Unrecognized Start Symbol: %s", yytext); BEGIN ERROR;}

<CHARCLASS>\\			{BEGIN CTLCHAR;}
<CHARCLASS></[0-9]*>	{BEGIN INTEGER;}
<CHARCLASS>"{"			{BEGIN INTCLASS;}
<CHARCLASS>"-"			{return TO;}
<CHARCLASS>"]"			{return STDEND;}
<CHARCLASS>[ \t\n]		;
<CHARCLASS>.			{yylval.singleton = (int) *yytext; return SINGLETON;}

<CTLCHAR>n		{yylval.singleton = (int) '\n'; return SINGLETON;}
<CTLCHAR>t		{yylval.singleton = (int) '\t'; return SINGLETON;}
<CTLCHAR>f		{yylval.singleton = (int) '\f'; return SINGLETON;}
<CTLCHAR>v		{yylval.singleton = (int) '\v'; return SINGLETON;}
<CTLCHAR>r		{yylval.singleton = (int) '\r'; return SINGLETON;}
<CTLCHAR>s		{yylval.singleton = (int) ' '; return SINGLETON;}
<CTLCHAR>"-"	{yylval.singleton = (int) '-'; return SINGLETON;}
<CTLCHAR>"\\"	{yylval.singleton = (int) '\\'; return SINGLETON;}
<CTLCHAR>"{"	{yylval.singleton = (int) '{'; return SINGLETON;}
<CTLCHAR>"<"	{yylval.singleton = (int) '<'; return SINGLETON;}
<CTLCHAR>"]"	{yylval.singleton = (int) ']'; return SINGLETON;}
<CTLCHAR>.		{printf("ERROR: Unknown Escape Sequence \'\\%c\'\n"
						,*yytext);
						BEGIN ERROR;}
<INTEGER>0*(1?[0-9][0-9]|2([0-4][0-9]|5[0-5]))> {yylval.singleton = atoi(yytext);
											BEGIN CHARCLASS; return SINGLETON;}
<INTEGER>[0-9]*/>	{printf("ERROR: Expected Integer Between 0 and 255, found %s\n"
						,yytext); BEGIN ERROR;}
<INTEGER>[0-9]*		{printf("ERROR: Unterminated Integer: %s\n", yytext); BEGIN ERROR;}
<INTEGER>.*			{printf("ERROR: Unrecognized Integer: %s\n", yytext); BEGIN ERROR;}

<INTCLASS>0*((1?[0-9][0-9])|(2([0-4][0-9])|(5[0-5]))) {yylval.singleton = atoi(yytext); return SINGLETON;}
<INTCLASS>".."		{return TO;}
<INTCLASS>","			;
<INTCLASS>"}"		{BEGIN CHARCLASS;}
<INTCLASS>[0-9]*/>	{printf("ERROR: Expected Integer Between 0 and 255, found %s\n"
						,yytext); BEGIN ERROR;}
<INTCLASS>.			{printf("ERROR: Expected Comma Seperated Integer List,\n\tFound: %s"
							,yytext); BEGIN ERROR;}


<ERROR>(.|\n)*			{printf("%s\n", yytext);}
