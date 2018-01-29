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
	struct Range* range;
	struct State* state;
	struct ast_node* astnode;
	struct root_node* rootnode;
}

%token <range> RANGE
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
						printf("Found Declaration: %s:\n",
							$1->entry->symbol);
						tab_add(&table, $1->entry->symbol, STATE, 1);
						}
Maps:
	Map Maps 			{$$ = $1;
						$$->right = $2; }
	| Deflt_Map			{ $$ = $1; }
Map:
	RANGE STATE			{$$ = mk_node(mk_node(NULL, NULL, STATE, $2),
								NULL, RANGE, $1);
						}
Deflt_Map:
	DEFLT_RNG STATE		{$$ = mk_node(mk_node(NULL, NULL, STATE, $2),
								NULL, DEFLT_RNG, NULL);
						}

%%

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
	tab_add(&table, state, STATE, 0);
	tmp->entry = tab_lookup(&table, state);
	tmp->line = line;
	return tmp;

}
	
int main(int argc, char** argv)
{
	tab_init(&table);
	ast_init(&syntax_tree);
	line_no = 1;
	printf("yyparse(): %d\n", yyparse());
	printf("\n\nSymbol Table Contents:\n\n");
	tab_printall(&table);
	printf("Depth First Traversal of AST:\n\n");
	ast_df_print(&syntax_tree);
	return 0;
}

