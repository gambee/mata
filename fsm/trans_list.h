/* trans_list.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef TRANS_LIST_H
#	define TRANS_LIST_H
#	include "charclass.tab.h"

struct tr_node
{
	struct tr_node* next;
	char* edge_name;
	charclass cc;
};

struct tr_list
{
	struct tr_node* head;
};

void tr_init(struct tr_list* to_init)
{
	to_init->head = NULL;
}

struct tr_node* tr_mk_node(struct tr_node* next
							,char* edge_name, charclass* src)
{
	struct tr_node* tmp = NULL;
	if(edge_name && src)
	{
		tmp = (struct tr_node*) malloc(sizeof(struct tr_node));
		if(tmp)
		{
			tmp->edge_name = (char*) malloc(strlen(edge_name) + 1);
			if(tmp->edge_name)
			{
				tmp->next = next;
				cc_copy(&(tmp->cc), src);
				strcpy(tmp->edge_name, edge_name);
			}
			else 
			{
				free(tmp);
				tmp = NULL;
			}
		}
	}
	return tmp;
}

int tr_add(struct tr_list* list, char* edge_name, charclass* src)
{
	struct tr_node* tmp;

	if(list && edge_name && src)
		if((tmp = tr_mk_node(list->head, edge_name, src)) != NULL)
		{
			list->head = tmp;
			return 0;
		}
	
	return -1;
}


int tr_codegen(struct tr_list* list, FILE* outfile)
{
	struct tr_node* cur;
	int count = -1;

	if(list)
	{
		++count;
		cur = list->head;
		while(cur)
		{
			++count;
			cc_codegen(&(cur->cc), cur->edge_name, outfile);
			cur = cur->next;
		}
	}

	return count;
}
		
#endif //TRANS_LIST_H
