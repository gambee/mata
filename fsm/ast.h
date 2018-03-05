/* ast.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef AST_H
#	define AST_H
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "buffer.tab.h"
#include "trans_list.h"
#include "parser.tab.h"
#include "matatypes.h"
#include "table.h"

int dot_node_index;

static char INIFIN_DECL_OP_TEXT [] = "<:>";
static char INITIAL_DECL_OP_TEXT [] = ":>";
static char FINAL_DECL_OP_TEXT [] = "<:";
static char NORMAL_DECL_OP_TEXT [] = ":";
static char DEFLT_RNG_TEXT [] = "~";

struct ast_node
{
	struct ast_node *left;
	struct ast_node *right;
	int type;
	union{
		struct State* state;
		struct CClass* cclass;
	}symbol;
};

struct root_node
{
	struct root_node* next;
	struct ast_node* root;
};

struct ast
{
	struct root_node* head;
};

int ast_codegen_finitecontrol(FILE* outfile)
{
	if(!outfile)
		return -1;

	//add finite control routine:
	fprintf(outfile,
		"int FSM_finite_control(char* input)\n"
		"{\n"
		"\tchar *cur;\n"
		"\tint state;\n"
		"\tif(!input)\n\t\treturn -1;\n\n"
		"\tfor(cur=input, STATE_init(state);*cur;++cur)\n\t{\n"
		"\t\tstate = FSM(state,*cur);\n"
		"\t}\n"
		"\treturn STATE_isfinal(state);\n}\n\n");
	return 0;
}

int ast_codegen_statetypes(struct ast* tree, FILE* outfile)
{
	struct root_node* cur;
	int init_states;

	if(!tree || !outfile)
		return -1;
	
	//add isfinal func via final states
	fprintf(outfile,
		"int STATE_isfinal(int STATE)\n{\n"
		"\tswitch(STATE){\n");
	for(cur = tree->head; cur; cur = cur->next)
	{
		if(cur->root->type == INIFIN_DECL_OP ||
			cur->root->type == FINAL_DECL_OP)
		{
			fprintf(outfile
				,"\t\tcase STATE_NUMBER_");
			fprintf(outfile
				,cur->root->left->symbol.state->entry->symbol);
			fprintf(outfile, ":\n");
		}
	}
	fprintf(outfile,
		"\t\t\treturn 0;\n"
		"\t\tdefault:\n\t\t\treturn STATE;\n\t}\n}\n\n");

	//add initial state 
	for(cur = tree->head, init_states = 0; cur; cur = cur->next)
	{
		if(cur->root->type == INIFIN_DECL_OP ||
			cur->root->type == INITIAL_DECL_OP)
		{
			if(init_states == 0)
			{
				fprintf(outfile
					,"const int INITIAL_FSM_STATE = STATE_NUMBER_");
				fprintf(outfile
					,cur->root->left->symbol.state->entry->symbol);
				fprintf(outfile, ";\n\n");
			}
			++init_states;
		}
	}
	if(init_states == 0)
	{
		fprintf(outfile, "static const int INITIAL_FSM_STATE = 0;\n\n");
		++init_states;
	}

	//add state init macro
	fprintf(outfile, "#define STATE_init(X)\t(X=INITIAL_FSM_STATE)\n\n");
}

int ast_codegen_transfunc(struct ast* tree, FILE* outfile)
{
	char name[200];
	char dest[100];
	char src[100];
	struct root_node* state;
	struct ast_node* cur;

	if(!outfile || !tree)
		return -1;
	
	state = tree->head;

	while(state)
	{
		cur = state->root;
		strcpy(src, cur->left->symbol.state->entry->symbol);

		fprintf(outfile, "\tSTATE_LABEL_%s:\n", src);

		cur = cur->right;

		while(cur->right)
		{
			strcpy(dest, cur->left->symbol.state->entry->symbol);
			strcpy(name, src);
			strcat(name, "__TO__");
			strcat(name, dest);

			fprintf(outfile, "\t\tif(cc_bit((charclass*)&%s, input))\n"
					"\t\t\treturn STATE_NUMBER_%s;\n"
					,name
					,dest);

			cur = cur->right;
		}

		strcpy(dest, cur->left->symbol.state->entry->symbol);
		fprintf(outfile, "\t\t//Default Transition:\n"
				"\t\t\treturn STATE_NUMBER_%s;\n"
				,dest);
		state = state->next;
	}

	return 0;

	bad_tree:
		printf("ERROR: ast_mk_tr_list: BAD TREE!!!!\n");
		return -2;
}


int ast_mk_tr_list(struct tr_list* list, struct ast* tree)
{
	char name[200];
	char src[100];
	struct root_node* state;
	struct ast_node* cur;

	if(!list || !tree)
		return -1;
	
	state = tree->head;

	while(state)
	{
		cur = state->root;
		if((cur->type == NORMAL_DECL_OP || 
			cur->type == INIFIN_DECL_OP ||
			cur->type == INITIAL_DECL_OP ||
			cur->type == FINAL_DECL_OP) && cur->left)
		{
			if(cur->left->type == STATE)
			{
				strcpy(src, cur->left->symbol.state->entry->symbol);
				cur = cur->right;
				while(cur->right)
				{
					if(cur->left)
					{
						strcpy(name, src);
						strcat(name, "__TO__");
						strcat(name, cur->left->symbol.state->entry->symbol);
						//printf("Making Edge: %s\n", name);
						tr_add(list, name, &(cur->symbol.cclass->cc));
					}
					else
					{
						printf("No left leaf!!\n");
						goto bad_tree;
					}
					cur = cur->right;
				}
			}
			else
			{
				printf("Source is of non-State type\n");
				goto bad_tree;
			}
		}
		else
		{
			printf("Expected \':\' declaration operator!!\n");
			goto bad_tree;
		}

		state = state->next;
	}

	return 0;

	bad_tree:
		printf("ERROR: ast_mk_tr_list: BAD TREE!!!!\n");
		return -2;
}

char* get_token_text(struct ast_node* node)
{
	if(node)
	{
		switch(node->type){
			case STATE: return node->symbol.state->entry->symbol;
			case CCLASS: return node->symbol.cclass->cclass;
			case DEFLT_RNG: return DEFLT_RNG_TEXT;
			case NORMAL_DECL_OP: return NORMAL_DECL_OP_TEXT;
			case INITIAL_DECL_OP: return INITIAL_DECL_OP_TEXT;
			case INIFIN_DECL_OP: return INIFIN_DECL_OP_TEXT;
			case FINAL_DECL_OP: return FINAL_DECL_OP_TEXT;
		}
	}
	return NULL;
}

int rec_print_as_dot(struct ast_node* root, FILE* outfile)
{
	int cur_dot_index = dot_node_index++;
	int count;
	char* token = NULL;

	if(root)
	{
		fprintf(outfile, "\t%d[label=\"%s\"];\n\n"
			,cur_dot_index
			,((token = get_token_text(root)) == NULL) ? "(null)" : token );
		if(root->left)
		{
			fprintf(outfile, "\t%d->%d;\n"
				,cur_dot_index, dot_node_index);
			count = rec_print_as_dot(root->left, outfile);
		}
		if(root->right)
		{
			fprintf(outfile, "\t%d->%d;\n"
				,cur_dot_index, dot_node_index);
			count += rec_print_as_dot(root->right, outfile);
		}
		return count;
	}
	else return 0;
}

int print_as_dot(struct ast* to_print, FILE* outfile)
{
	struct root_node *cur = to_print->head;
	int ret = 0, sub_root_index;

	dot_node_index = 1;
	fprintf(outfile, "digraph AST {\n\t0[label=FSM];\n\n");
	while(cur)
	{
		sub_root_index = dot_node_index;
		ret += rec_print_as_dot(cur->root, outfile);
		fprintf(outfile, "\t0->%d;\n\n", sub_root_index);
		cur = cur->next;
	}
	fprintf(outfile, "}");
	return ret;
}

int rec_print_FSM_as_dot(struct ast_node* root, char* parent, FILE* outfile)
{
	int count = 0;
	char* token = NULL;

	if(root)
	{

	}
	else return 0;
}

int print_FSM_as_dot(struct ast* to_print, FILE* outfile)
{
	int ret = 0;
	struct root_node *cur = to_print->head;

	fprintf(outfile, "digraph AST {\n\trankdir=LR;\n\n");

	//print states, with circle or double circle
	while(cur)
	{
		if(cur->root->type == INIFIN_DECL_OP ||
			cur->root->type == FINAL_DECL_OP)
		{
			fprintf(outfile, "\t%s[shape=doublecircle];\n"
				,cur->root->left->symbol.state->entry->symbol);
		}
		else
		{
			fprintf(outfile, "\t%s[shape=circle];\n"
				,cur->root->left->symbol.state->entry->symbol);
		}
		cur = cur->next;
	}

	cur = to_print->head;
	while(cur)
	{
		ret += rec_print_FSM_as_dot(cur->root->right
				,cur->root->left->symbol.state->entry->symbol
				,outfile);
		cur = cur->next;
	}
	fprintf(outfile, "}");
	return ret;
}

void ast_init(struct ast* to_init)
{
	to_init->head = NULL;
}

int ast_add(struct ast* dest, struct ast_node* root)
{
	struct root_node* tmp;

	if(dest == NULL)
		return -1;
	if(root == NULL)
		return -2;
	
	tmp = (struct root_node*) malloc(sizeof(struct root_node));
	if(tmp == NULL)
		return -3;
	
	tmp->root = root;
	tmp->next = dest->head;
	dest->head = tmp;
	return 0;
}
	

struct ast_node* mk_node(	struct ast_node* left,
							struct ast_node* right,
							int type,
							void* symbol)
{
	struct ast_node* tmp;

	switch(type){
		case INIFIN_DECL_OP:
		case INITIAL_DECL_OP:
		case FINAL_DECL_OP:
		case NORMAL_DECL_OP:
			if(symbol)
				return NULL; //error, passed non null pointer 
			break;
		case DEFLT_RNG:
			if(symbol)
				return NULL; //error, passed non null pointer 
			break;
		case CCLASS:
			if(!symbol)
				return NULL; //error, passed null pointer 
			break;
		case STATE:
			if(!symbol)
				return NULL; //error, passed null pointer 
			break;
		default:
			return NULL; //error, not a known type!!!
	}
	
	tmp = (struct ast_node*) malloc(sizeof(struct ast_node));
	if(tmp == NULL)
		return NULL; //error, failed malloc

	tmp->left = left;
	tmp->right = right;
	tmp->type = type;

	switch(type){
		case NORMAL_DECL_OP:
			tmp->symbol.state = NULL;
			break;
		case DEFLT_RNG:
			tmp->symbol.state = NULL;
			break;
		case CCLASS:
			tmp->symbol.cclass = (struct CClass*) symbol;
			break;
		case STATE:
			tmp->symbol.state = (struct State*) symbol;
			break;
	}

	return tmp;
}

int print_node(struct ast_node* to_print)
{
	if(to_print == NULL)
		return -1;
	
	switch(to_print->type) {
		case NORMAL_DECL_OP:
			printf(":");
			break;
		case DEFLT_RNG:
			printf("~");
			break;
		case CCLASS:
			if(to_print->symbol.cclass == NULL)
				return -2;
			if(to_print->symbol.cclass->cclass == NULL)
				return -3;
			printf(to_print->symbol.cclass->cclass);
			break;
		case STATE:
			if(to_print->symbol.state == NULL)
				return -2;
			if(to_print->symbol.state->entry == NULL)
				return -3;
			if(to_print->symbol.state->entry->symbol == NULL)
				return -3;
			printf(to_print->symbol.state->entry->symbol);
			break;
		default:
			return -4; //invalid type
	}
	return 0;
}

int depth_first_print(struct ast_node* root)
{
	int ret = 1;
	if(!root)
		return 0;
	print_node(root);
	ret += depth_first_print(root->left);
	ret += depth_first_print(root->right);
	return ret;
}

int ast_df_print(struct ast* to_print)
{
	int ret = 0;
	struct root_node* cur;

	if(!to_print)
		return -1;
	
	cur = to_print->head;

	while(cur)
	{
		ret += depth_first_print(cur->root);
		cur = cur->next;
	}

	return ret;
}

int check_node(struct ast_node* to_check)
{
	if(to_check == NULL)
		return -1;
	
	switch(to_check->type) {
		case NORMAL_DECL_OP:
		case DEFLT_RNG:
		case CCLASS:
			break;
		case STATE:
			if(to_check->symbol.state == NULL)
				break;
			if(to_check->symbol.state->entry == NULL)
				break;
			if(to_check->symbol.state->entry->symbol == NULL)
				break;
			if(!to_check->symbol.state->entry->declared)
			{
				printf("ERROR: State \'%s\' found on line %d"
						" is undeclared.\n"
						,to_check->symbol.state->entry->symbol
						,to_check->symbol.state->line);
				return 1;
			}
			else break;
	}
	return 0;
}

int depth_first_checkdecl(struct ast_node* root)
{
	int ret;
	if(!root)
		return 0;
	ret = check_node(root);
	ret += depth_first_checkdecl(root->left);
	ret += depth_first_checkdecl(root->right);
	return ret;
}

int ast_df_checkdecl(struct ast* to_print)
{
	int ret = 0;
	struct root_node* cur;

	if(!to_print)
		return -1;
	
	cur = to_print->head;

	while(cur)
	{
		ret += depth_first_checkdecl(cur->root);
		cur = cur->next;
	}

	if(ret)
		printf("ERROR: Found %d Undeclared States\n", ret);

	return ret;
}

#endif
