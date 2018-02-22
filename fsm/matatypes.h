/* matatypes.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef MATATYPES_H
#	define MATATYPES_H

struct Range{
	char* range;
	int line;
};

struct State{
	struct tab_entry* entry;
	int line;
};

#endif
