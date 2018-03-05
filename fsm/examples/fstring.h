/*	fstring.h
 *
 *	Max Gambee
 *	Copyright 2018
 *
 *	This header contains routines for working with formatted and unformatted
 *	c strings (null terminated), as well as other strings. 
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

int getsline(char* dest)
{
	int i = 0;
	char *cur = dest;
	while((*(cur++) = getchar()) != '\n');
	*(--cur) = 0;
	return i;
}

int printunf(char* str)
{
	int i;
	char *cur;
	for(i = 0, cur = str; *cur; ++cur, ++i)
	{
		if(*cur == '\n' || *cur == ' ' || *cur == '\t' || *cur == '\r')
		{
			switch(*cur)
			{
				case '\\':
					printf("\\\\");
					break;
				case '\n':
					printf("\\n");
					break;
				case ' ':
					printf("\\s");
					break;
				case '\t':
					printf("\\t");
					break;
				case '\r':
					printf("\\r");
					break;
			}
		}
		else if(isgraph((unsigned char)*cur))
			putchar(*cur);
		else printf("<%d>", *cur);
	}
	return i;
}
