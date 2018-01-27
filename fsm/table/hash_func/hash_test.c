/* hash_test.c
 *
 * Max Gambee
 * Copyright 2018
 *
 * A test wrapper application for the string hashes for the sym_tab
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

int get_symbol(char* dest)
{
	char* line = NULL;
	size_t n = 0;
	int delim_index = getline(&line, &n, stdin);
	line[delim_index - 1] = 0;
	strcpy(dest, line);
	free(line);
	return strlen(dest);
}
	
	
int main(int argc, char** argv)
{
	int i;
	int hits[SYM_TAB_SIZE];
	char line[101];
	
	memset(line, 0, 101);						//initialize line to 0 (avoid seg-faults)
	memset(hits, 0, sizeof(int)*SYM_TAB_SIZE);	//initialize hits to 0

	while(!feof(stdin))
		if(i = get_symbol(line))
			hits[horner_hash(line)]++;



	//print all hits to stdout in line-delimited format
	for(i=0;i<SYM_TAB_SIZE;i++)
		printf("%d\n", hits[i]);


	return 0;
}
	
	
