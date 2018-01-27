/* tablist.h
 *
 * Max Gambee
 * Copyright 2018
 */

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
	char* symbol;		//actual string
	int occurs;			//occurances
	int first_occurance;//line number of first occurance
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
int tablist_add(struct tab_entry** head, char* symbol, int at_line)
{
	struct tab_entry* tmp;

	if(!symbol) //passed NULL symbol
		return -2;
	
	if(NULL != (tmp = tablist_lookup(*head, symbol)))
	{
		tmp->occurs++;
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
		tmp->occurs = 1;
		tmp->first_occurance = at_line;
		tmp->next = *head;
		*head = tmp;
	}
	return 0;
}

#ifdef DEBUG
#	ifndef DEBUG_LIBS
#		include <stdio.h>
#		define DEBUG_LIBS
#	endif

/* Function: tablist_printentry
 * ----------------------------
 */
int tablist_printentry(struct tab_entry* entry)
{
	if(entry)
	{
		printf("Entry:\t%s\nOccurances:\t%d\nFirst Occurs:\t%d\n--------------\n",
			entry->symbol, entry->occurs, entry->first_occurance);
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
