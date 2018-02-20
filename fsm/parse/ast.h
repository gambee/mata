/* ast.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef AST_H
#	define AST_H
#include <stdio.h>
#include <stdlib.h>
#include "parser.tab.h"
#include "matatypes.h"
#include "table.h"


struct ast_node
{
	struct ast_node *left;
	struct ast_node *right;
	int type;
	union{
		struct State* state;
		struct Range* range;
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
		case DECL_OP:
			if(symbol)
				return NULL; //error, passed non null pointer 
			break;
		case DEFLT_RNG:
			if(symbol)
				return NULL; //error, passed non null pointer 
			break;
		case RANGE:
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
		case DECL_OP:
			tmp->symbol.state = NULL;
			break;
		case DEFLT_RNG:
			tmp->symbol.state = NULL;
			break;
		case RANGE:
			tmp->symbol.range = (struct Range*) symbol;
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
		case DECL_OP:
			printf(":");
			break;
		case DEFLT_RNG:
			printf("~");
			break;
		case RANGE:
			if(to_print->symbol.range == NULL)
				return -2;
			if(to_print->symbol.range->range == NULL)
				return -3;
			printf(to_print->symbol.range->range);
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
		case DECL_OP:
		case DEFLT_RNG:
		case RANGE:
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
