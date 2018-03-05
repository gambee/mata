#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../buffer.h"
#include "../charclass.h"
#include "FSM.h"
#include "fstring.h"

int main(void)
{
	int state, i;
	char *cur, *str;
	char input[500];

	printf("Enter String: ");
	getsline(input);

	while(strcmp(input, "quit"))
	{
		printf("\n\tRunning FSM on input string \"%s\"\n", input);
		for(cur = input, STATE_init(state), i = 0; *cur; ++i, ++cur)
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
		getsline(input);
	}

	printf("Enter String: ");
	getsline(input);

	while(strcmp(input, "quit"))
	{
		printf("\n\tRunning FSM on input string \"%s\"\n", input);
		if(i = FSM_finite_control(input))
			printf("\tExited in error, on state %s\n\n", STATE_text(i));
		else printf("\tSuccess!!\n\n");

		printf("Enter String: ");
		getsline(input);
	}
	return 0;
}


