/* matatypes.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef MATATYPES_H
#	define MATATYPES_H

#include "charclass.tab.h"


struct CClass{
	char* cclass;
	charclass cc;
	int line;
};

struct State{
	struct tab_entry* entry;
	int line;
};

#endif
