/* tablist.h
 *
 * Max Gambee
 * Copyright 2018
 */
#ifndef TABLIST_H
#	define TABLIST_H

#ifndef TABLE_LIBS
#	include <stdlib.h>
#	include <string.h>
#	define TABLE_LIBS
#endif

/* Section: Structures
 * ===================
 */

/* Structure: entry
 * ---------------
 */
struct tab_entry
{
	struct tab_entry* next;	//LLL for handling collisions
	char* symbol;			//actual string
	int declared;			//number of times declared
	int type;				//type of token
};

/* Section: Functions 
 * ==================
 */


/* Function: tablist_lookup
 * -------------------------
 */
struct tab_entry* tablist_lookup(struct tab_entry* head, char* symbol)
{
	struct tab_entry* cur = head;

	if(!symbol)
		return NULL;
	
	while(cur)
	{
		if(cur->symbol)
		{
			if(!strcmp(symbol, cur->symbol))
				break;
		}
		cur = cur->next;
	}
	return cur; //number of occurences
}



/* Function: tablist_add
 * ---------------------
 */
int tablist_add(struct tab_entry** head, char* symbol, int type, int decl)
{
	struct tab_entry* tmp;

	if(!symbol) //passed NULL symbol
		return -2;
	
	if(NULL != (tmp = tablist_lookup(*head, symbol)))
	{
		if(tmp->type && type && tmp->type !=type)
			return -5;
		else
			tmp->type = type;
		tmp->declared += decl;
	}
	else
	{
		tmp = (struct tab_entry*) malloc(sizeof(struct tab_entry));
		if(!tmp)	//malloc failed to allocate a new node
			return -3;

		tmp->symbol = (char*) malloc(strlen(symbol) + 1);
		if(!tmp->symbol) //malloc failed to allocate a node's symbol string
		{
			free(tmp);
			return -4; 
		}

		strcpy(tmp->symbol, symbol);
		tmp->declared = decl;
		tmp->type = type;
		tmp->next = *head;
		*head = tmp;
	}
	return 0;
}

#ifdef TABLE_DEBUG
#	ifndef TABLE_DEBUG_LIBS
#		include <stdio.h>
#		define TABLE_DEBUG_LIBS
#	endif

/* Function: tablist_printentry
 * ----------------------------
 */
int tablist_printentry(struct tab_entry* entry)
{
	if(entry)
	{
		printf("Entry:\t%s\nDeclared:\t%d\nType:\t%d\n--------------\n",
			entry->symbol, entry->declared, entry->type);
		return 0;
	}
	else return 1;
}

/* Function: tablist_printall
 * --------------------------
 */
 int tablist_printall(struct tab_entry* head)
 {
	 int i = 0;
	 struct tab_entry* cur = head;
	 while(cur)
	 {
		 tablist_printentry(cur);
		 i++;
		 cur = cur->next;
	 }
	 return i;
 }
	 
#endif
#endif
