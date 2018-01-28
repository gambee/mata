/* parser.y
 *
 * Max Gambee
 * Copyright 2018
 */

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_DEBUG
#include "table.h"

int line_no;
struct sym_tab table;

extern int yylex(void);
void yyerror(char* input){ }

struct Range{
	char* range;
	int line;
};

struct State{
	struct tab_entry* entry;
	int line;
};

struct Range* mk_Range(char* range, int line)
{
	struct Range* tmp = (struct Range*) malloc(sizeof(struct Range));
	if(range == NULL)
		return NULL; //passed unititialized string
	if(tmp == NULL)
		return NULL; //failed malloc!
	tmp->range = (char*) malloc(strlen(range)+1);
	if(tmp->range == NULL)
	{	//failed malloc!
		free(tmp);
		return NULL;
	}
	strcpy(tmp->range, range);
	tmp->line = line;
	return tmp;
}

struct State* mk_State(char* state, int line)
{
	struct State* tmp = (struct State*) malloc(sizeof(struct State));
	if(state == NULL)
		return NULL; //passed uninitialized string
	if(tmp == NULL)
		return NULL; //failed malloc!
	//tab_add(
	return NULL;

}
	
%}

%union{
	struct Range* range;
	struct State* state;
}

%token <range> RANGE
%token <state> STATE

%%

Machine:
	Declarations
Declarations:
	Declaration Declarations | Declaration
Declaration:
	STATE ':' Maps
Maps:
	Map Maps | Map
Map:
	RANGE STATE

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

