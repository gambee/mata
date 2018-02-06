#include <stdio.h>
#include "range.h"

	
int main(int argc, char **argv)
{
	char input[501];
	char *str;
	int i;
	rng_ind rng;

	*input = 0;
	rnginit(&rng);
	fprintf(stderr, "testing errors\n");

	while(strcmp("quit",input))
	{
		printf("regex: ");
		scanf("%s\n", input);

		compexpr(&rng, input);
		rngprintbits(&rng);
		str = rngexpr(&rng);
		if(str)
			printf("%s\n", str);
	}



	return 0;
}
