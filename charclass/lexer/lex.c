/* lex.my.c
 *
 * Max Gambee
 * Copyright 2018
 *
 * This is a hand coded lexical analyzer intended to be used by bison.
 * The reason for opting for this, is that in this particular case, the 
 * lexical analysis is sufficiently simple, but involves enough odd quarks
 * in flex regex/start states, that it is unnecessarily difficult and
 * unintuitive to code/maintain this as a flex scanner.
 */
#include <ctype.h>
#include "cc_comp.tab.h"
#include "buffer.h"

enum state{INITIAL, INITIAL_B, CHARCLASS, INTEGER, CTLCHAR, INTCLASS, END};

int fsm(char**, int*, int*);
int yylex(void);

int yystate;
char **yybuf;

void yysetbuf(char**substr)
{
	yystate = INITIAL;
	yybuf = substr;
}

int yylex(void){ return fsm(yybuf, &yystate, &yylval.singleton); }

int fsm(char** substr, int* STATE, int* SING)
{
	//returns token types
	int i, j, sing, len;
	char* cur = *substr;
	char* tmp;
	struct BUF_buffer buffer;
	
	if(!*substr)
		return -1;

	BUF_init(&buffer);

	switch(*STATE)
	{
		case INITIAL:	goto Initial;
		case INITIAL_B:	goto Initial_B;
		case CHARCLASS:	goto CharClass;
		case INTEGER:	goto Integer;
		case CTLCHAR:	goto CtlChar;
		case INTCLASS:	goto IntClass;
		default:	 	goto End;
	}

	Initial:	switch(*cur)
	{
		case '[':	++cur;
					goto Initial_B;
		default :	goto End;
	}

	Initial_B:	switch(*cur)
	{
		case '^':	*substr = cur + 1;
					*STATE = CHARCLASS;
					//printf("CMPSTART\n");
					return CMPSTART;
		default:	*substr = cur;
					*STATE = CHARCLASS;
					//printf("STDSTART\n");
					return STDSTART;
	}

	CharClass:	switch(*cur)
	{
		case 0:		printf("Error: Unterminated Character Class\n");
					goto End;
		case '-':	*substr = cur + 1;
					//printf("TO\n");
					return TO;
		case '\\':	++cur;
					goto CtlChar;
		case '<':	++cur;
					goto Integer;
		case '{':	++cur;
					goto IntClass;
		case ']':	++cur;
					//printf("STDEND\n");
					*STATE = END;
					return STDEND;
		default:	*STATE = CHARCLASS;
					*SING = *cur;
					*substr = cur + 1;
					//printf("SINGLETON: %u, %c\n", *SING, *SING);
					return SINGLETON;
	}

	CtlChar:	switch(*cur)
	{
		case '\\':
		case '{':
		case '-':
		case '<':
		case ']':	*SING = *cur;
					break;
		case 'n':	*SING = '\n';
					break;
		case 'r':	*SING = '\r';
					break;
		case 'f':	*SING = '\f';
					break;
		case 'v':	*SING = '\v';
					break;
		case 't':	*SING = '\t';
					break;
		case 's':	*SING = ' ';
					break;
		default:	printf("Unrecognized Control Sequence: \\%c\n", *cur);
					goto End;
	}
	*substr = cur + 1;
	*STATE = CHARCLASS;
	//printf("SINGLETON: %u, %c\n", *SING, *SING);
	return SINGLETON;

	Integer:
	if(isdigit(*cur))
	{
		do{
			BUF_putc(&buffer, *cur);
			++cur;
		}while(isdigit(*cur));

		tmp = (char*) malloc((len = BUF_line_len(&buffer) + 1));

		for(i=0;i<len;++i)
			tmp[i] = BUF_getc(&buffer);
		tmp[i] = 0;

		if(*cur == '>')
		{
			*SING = atoi(tmp);
			free(tmp);
			*substr = cur + 1;
			*STATE = CHARCLASS;
			while(BUF_getc(&buffer));
			//printf("SINGLETON: %u, %c\n", *SING, *SING);
			return SINGLETON;
		}
		else
		{
			printf("Expeced \'>\' after Integer\n");
			goto End;
		}
	}
	else{
		printf("Expected Integer after \'<\'\n");
		goto End;
	}


	IntClass:
	if(isdigit(*cur))
	{
		do{
			BUF_putc(&buffer, *cur);
			++cur;
		}while(isdigit(*cur));

		tmp = (char*) malloc((len = BUF_line_len(&buffer) + 1));

		for(i=0;i<len;++i)
			tmp[i] = BUF_getc(&buffer);
		tmp[i] = 0;

		*SING = atoi(tmp);
		free(tmp);
		*substr = cur;
		while(BUF_getc(&buffer));
		*STATE = INTCLASS;
		//printf("SINGLETON: %u, %c\n", *SING, *SING);
		return SINGLETON;
	}
	else if(*cur == ',' || *cur == '}' || *cur == '.')
	{
		switch(*cur)
		{
			case ',':	++cur;
						goto IntClass;
			case '}':	++cur;
						goto CharClass;
			case '.':	if(*(++cur)=='.')
						{
							*substr = cur + 1;
							*STATE = INTCLASS;
							//printf("TO\n");
							return TO;
						}
						else 
						{
							printf("Expected another \'.\'\n");
							goto End;
						}
			default:	printf("Expeced \'}\' after Integer\n");
						goto End;
		}
	}
	else{
		printf("Expected Integer after \'<\'\n");
		goto End;
	}

	End:	return 0;
}
	
