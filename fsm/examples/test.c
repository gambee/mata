#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../buffer.h"
#include "../charclass.h"
#include "FSM.h"

int main(void)
{
	int state, i;
	char *cur, *str;
	char input[500];

	printf("Enter String: ");
	scanf(" %[a-zA-Z0-9 \t]", input);

	while(strcmp(input, "quit"))
	{
		printf("\n\tRunning FSM on input stinrg \"%s\"\n", input);
		for(cur = input, state = 0, i = 0; *cur; ++i, ++cur)
		{
			printf("\t\t%2d : Next Input: %c    \t: Current State: %s\n"
					,i
					,*cur
					,STATE_text(state));

			state = FSM(state, *cur);
		}
		printf("\t\t%2d : Next Input: (none)\t: Current State: %s\n"
				,i
				,STATE_text(state));
		printf("Enter String: ");
		scanf(" %[a-zA-Z0-9 \t]", input);
	}

	return 0;
}


