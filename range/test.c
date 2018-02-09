#include "range.h"

int main(int argc, char** argv)
{
	int i;
	rng_ind rng;
	rnginit(&rng);

	for(i='a';i<'z';++i)
		rngset(&rng, i);
	
	rngprintbits(&rng);

	for(i='h';i<'k';++i)
		rngunset(&rng, i);

	rngprintbits(&rng);

	return 0;
}
