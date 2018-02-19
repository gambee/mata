#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "charclass.h"
#include "cc_parse.h"


int main(void)
{
	char input[501];
	char *substr = input;
	int singleton, state, token;

	printf("Enter Character Class: ");
	scanf(" %s", input);

	while(strcmp(input, "quit")){
		cc_init(&cclass);
		substr = input;
		cc_setbuf(&substr);
		cc_init(&cclass);
		cc_parse();
		cc_printbits(&cclass);
		printf("Enter Character Class: ");
		scanf(" %s", input);
	}

	return 0;
}
