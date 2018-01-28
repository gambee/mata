#include <stdio.h>

#define TABLE_DEBUG
#include "table.h"

int main(int argc, char** argv)
{
	struct sym_tab table;
	struct tab_entry* tmp = NULL;

	tab_init(&table);

	tab_add(&table, "token1", 1, 1);
		
	tab_add(&table, "token1", 1, 1);

	tab_add(&table, "token1", 1, 1);

	tab_printall(&table);

	return 0;
}
