#include "charclass.h"

int main(int argc, char** argv)
{
	int i;
	charclass cclass;
	cc_init(&cclass);

	for(i='a';i<'z';++i)
		cc_set(&cclass, i);
	
	cc_printbits(&cclass);

	for(i='h';i<'k';++i)
		cc_unset(&cclass, i);

	cc_printbits(&cclass);

	return 0;
}
