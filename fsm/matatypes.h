/* matatypes.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef MATATYPES_H
#	define MATATYPES_H

struct CClass{
	char* cclass;
	int line;
};

struct State{
	struct tab_entry* entry;
	int line;
};

#endif
