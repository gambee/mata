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
#include "matatypes.h"
#include "ast.h"

int line_no;
struct sym_tab table;
struct ast syntax_tree;

extern int yylex(void);
void yyerror(char* input){printf("%s\n",input); }
%}

%union{
	struct CClass* cclass;
	struct State* state;
	struct ast_node* astnode;
	struct root_node* rootnode;
}

%token <cclass> CCLASS
%token <state> STATE
%token <astnode> DEFLT_RNG
%token DECL_OP
%type <astnode> Map Maps Declaration Deflt_Map 

%%

Machine:
	Declarations
Declarations:
	Declaration Declarations {ast_add(&syntax_tree, $1);}
	| Declaration		{ast_add(&syntax_tree, $1);}
Declaration:
	STATE DECL_OP Maps 	{
						$$ = mk_node(mk_node(NULL, NULL, STATE, $1),
								$3, DECL_OP, NULL);
						tab_add(&table, $1->entry->symbol, STATE, 1);
						}
Maps:
	Map Maps 			{$$ = $1;
						$$->right = $2; }
	| Deflt_Map			{ $$ = $1; }
Map:
	CCLASS STATE			{$$ = mk_node(mk_node(NULL, NULL, STATE, $2),
								NULL, CCLASS, $1);
						}
Deflt_Map:
	DEFLT_RNG STATE		{$$ = mk_node(mk_node(NULL, NULL, STATE, $2),
								NULL, DEFLT_RNG, NULL);
						}

%%

struct CClass* mk_CClass(char* cclass, int line)
{
	struct CClass* tmp = (struct CClass*) malloc(sizeof(struct CClass));
	if(cclass == NULL)
		return NULL; //passed unititialized string
	if(tmp == NULL)
		return NULL; //failed malloc!
	tmp->cclass = (char*) malloc(strlen(cclass)+1);
	if(tmp->cclass == NULL)
	{	//failed malloc!
		free(tmp);
		return NULL;
	}
	strcpy(tmp->cclass, cclass);
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
	tab_add(&table, state, STATE, 0);
	tmp->entry = tab_lookup(&table, state);
	tmp->line = line;
	return tmp;

}
	
int main(int argc, char** argv)
{
	char filename[100];
	FILE* file = NULL;

	tab_init(&table);
	ast_init(&syntax_tree);
	line_no = 1;

	yyparse();
	if(!ast_df_checkdecl(&syntax_tree))
	{
		if(argc == 2)
		{
			if(file = fopen(argv[1], "w"))
				print_as_dot(&syntax_tree, file);
		}
		else
		{
			if(file = fopen("ast.dot", "w"))
				print_as_dot(&syntax_tree, file);
		}
	}
	
	return 0;
}

