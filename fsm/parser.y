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
//#include "table.h"
//#include "matatypes.h"
#include "buffer.h"
#include "ast.h"
#include "trans_list.h"
#include "cc_parse.tab.h"
#include "cc_lex.c"
//#include "cc_parse.h"

int line_no;
struct sym_tab table;
struct ast syntax_tree;
struct tr_list transitions;
extern charclass cc_parse_cclass;


int cc_parse(void);
void cc_setbufline(char**, int);

int yylex(void);
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

	cc_init(&cc_parse_cclass);
	cc_setbufline(&cclass, line);
	cc_parse();
	cc_copy(&(tmp->cc), &cc_parse_cclass);
	//tr_add(&transitions, "edge_name", &cc_parse_cclass);//to be modified
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

int codegen(FILE* outfile)
{
	char* tmp;
	tr_codegen(&transitions, outfile);
	tab_codegen_populate(&table, outfile);

	//storing state_enum code in a string and printing to outfile
	tmp = NULL;
	BUF_gets(&state_enum, &tmp);
	if(!tmp)
		return -1; //didn't work!
	fprintf(outfile, tmp);
	free(tmp);
	tmp = NULL;

	//storing state_text code in a string and printing to outfile
	tmp = NULL;
	BUF_gets(&state_text, &tmp);
	if(!tmp)
		return -1; //didn't work!
	fprintf(outfile, tmp);
	free(tmp);
	tmp = NULL;

	//storing state_textfunc code in a string and printing to outfile
	tmp = NULL;
	BUF_gets(&state_textfunc, &tmp);
	if(!tmp)
		return -1; //didn't work!
	fprintf(outfile, tmp);
	free(tmp);
	tmp = NULL;

	//storing jump_switch code into a string, for later
	tmp = NULL;
	BUF_gets(&jump_switch, &tmp);
	if(!tmp)
		return -1; //didn't work!

	fprintf(outfile, "int FSM(int STATE, char input)\n{\n");
	fprintf(outfile, tmp);
	free(tmp);
	tmp = NULL;
	ast_codegen(&syntax_tree, outfile);
	fprintf(outfile, "}\n");

	return 0;
}
	
int main(int argc, char** argv)
{
	int i;
	char* cur;
	char filename[100];
	FILE *dotfile = NULL, *hfile = NULL;

	if(argc == 2)
		freopen(argv[1], "r", stdin);

	tr_init(&transitions);
	tab_init(&table);
	ast_init(&syntax_tree);
	line_no = 1;

	yyparse();
	if(!ast_df_checkdecl(&syntax_tree))
	{
		if(ast_mk_tr_list(&transitions, &syntax_tree))
			return -1;

		if(argc == 2)
		{
			strcpy(filename, argv[1]);
			for(cur=filename; *cur != '.' && *cur; cur++);
			if(*cur)
			{
				if(strcmp(".mata", cur))
				{
					printf("ERROR, unknown file type extension \"%s\"\n"
					,cur);
					return -1;
				} 
				++cur;

				*cur = 0;
				strcat(filename, "dot");
				dotfile = fopen(filename, "w");

				*cur = 0;
				strcat(filename, "h");
				hfile = fopen(filename, "w");
			}
			else
			{
					printf("ERROR, unknown file type extension <blank>\n");
					return -1;
			}
		}
		else
		{
			dotfile = fopen("out.dot", "w");
			if(hfile = fopen("out.h", "w"));
		}

		if(dotfile)
		{
			print_as_dot(&syntax_tree, dotfile);
			fclose(dotfile);
		}

		if(hfile)
		{
			codegen(hfile);
			fclose(hfile);
		}
	}
	
	return 0;
}

