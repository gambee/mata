#include <stdio.h>

#include "cc_parse.h"
#include "charclass.h"


void cc_setbuf(char**);
extern charclass cc_parse_cclass;

int main(void)
{
	charclass cc;
	char Exp [] = "[\\^]";
	char output[101];
	char* exp = Exp;

	cc_init(&cc);
	cc_init(&cc_parse_cclass);

	cc_setbuf(&exp);

	printf("cc:\n");
	cc_printbits(&cc);
	printf("cc_parse_cclass:\n");
	cc_printbits(&cc_parse_cclass);

	printf("\nparsing....\n\n");
	cc_parse();

	printf("cc:\n");
	cc_printbits(&cc);
	printf("cc_parse_cclass:\n");
	cc_printbits(&cc_parse_cclass);

	printf("\ncopying....\n\n");
	cc_copy(&cc, &cc_parse_cclass);

	printf("cc:\n");
	cc_printbits(&cc);
	printf("cc_parse_cclass:\n");
	cc_printbits(&cc_parse_cclass);

	cc_str(output, &cc, 100);
	printf("output: %s\n", output);

	return 0;
}
