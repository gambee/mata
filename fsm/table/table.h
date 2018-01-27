/* table.h
 *
 * Max Gambee
 * Copyright 2018
 */
#ifndef TABLE_H
#	define TABLE_H

#ifndef TABLE_LIBS
#	include <stdlib.h> //needed for memory mngmt
#	include <string.h> //needed for memset
#	define TABLE_LIBS
#endif

#include "tablist.h"

/* Section: Defines
 * ================
 */

#define SYM_TAB_SIZE	9997	//size of the hash table
#define POLN_X			31		//the value entered into the polynomial string-hash

/* Section: Structures
 * ===================
 */

/* Structure: sym_tab
 * -----------------
 */
struct sym_tab
{
	struct tab_entry* entries[SYM_TAB_SIZE];
};


/* Section: Functions 
 * ==================
 */

/* Function: tab_hash
 * --------------
 * performs a string hash using polynomial coeficients 
 * and the horner algorithm for evaluation.
 */
unsigned long tab_hash(char* symbol)
{
	char* cur;
	unsigned long val;
	if(symbol)
		for(cur = symbol, val = 0; *cur; ++cur)
			val = val * POLN_X + (unsigned long) *(cur++);
	return val % SYM_TAB_SIZE;
}

/* Function: tab_add
 * -----------------
 */
int tab_add(struct sym_tab* table, char* symbol, int at_line)
{
	if(!table)
		return -1;
	
	return tablist_add(&(table->entries[tab_hash(symbol)]), symbol, at_line);
}

/* Function: tab_lookup
 * ---------------
 */
struct tab_entry* tab_lookup(struct sym_tab* table, char* symbol)
{
	if(!table || !symbol)
		return NULL;
	return tablist_lookup(table->entries[tab_hash(symbol)], symbol);;
}


/* Function: init_sym_tab
 * ---------------------
 */
int tab_init(struct sym_tab* to_init)
{
	if(to_init)
	{
		memset(to_init->entries, 0, SYM_TAB_SIZE*sizeof(struct entry*));
		return 0;
	}
	else return 1;
}

#ifdef TABLE_DEBUG
#	ifndef TABLE_DEBUG_LIBS
#		include <stdio.h>
#		define TABLE_DEBUG_LIBS
#	endif

/* Function: tab_printall
 * -----------------------
 */
int tab_printall(struct sym_tab* table)
{
	int i;
	for(i = 0; i < SYM_TAB_SIZE; i++)
	{
		if(table->entries[i])
		{
			printf("table->entries[%d]\n=================\n",i);
			tablist_printall(table->entries[i]);
		}
	}
	return i;
}

#endif
#endif
