#include <stdio.h>
#include "charclass.h"
#include "trans_list.h"
#include "cc_parse.h"

int main(int argc, char** argv)
{
	FILE* outfile = stdout;
	struct tr_list transitions;
	char input[200];
	char* buffer;

	tr_init(&transitions);
	
	printf("Enter Character Class: ");
	scanf(" %s", input);
	
	while(strcmp(input, "quit"))
	{
		buffer = input;
		cc_init(&cc_parse_cclass);
		cc_setbuf(&buffer);
		cc_parse();
		cc_printbits(&cc_parse_cclass);
		tr_add(&transitions,"edge_name", &cc_parse_cclass);
		printf("Enter Character Class: ");
		scanf(" %s", input);
	}

	tr_codegen(&transitions, outfile);


	return 0;
}
