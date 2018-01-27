/* hash.h
 *
 * Max Gambee
 * Copyright 2018
 *
 * Some test hash functions for strings
 */

#define	SYM_TAB_SIZE	9997
#define POLN_X			31

unsigned long horner_hash(char* symbol)
{
	char* cur;
	unsigned long val;
	if(symbol)
		for(cur = symbol, val = 0; *cur; ++cur)
			val = val * POLN_X + (unsigned long) *(cur++);
	return val % SYM_TAB_SIZE;
}
