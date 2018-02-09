/* range.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef RANGE_H
#	define RANGE_H
#	include <string.h>
#	include "rngbuf.h"
#	include "cbits.h"

typedef struct {char member[32];} rng_ind;

void rnginit(rng_ind*);
int rngbitcnt(rng_ind*);
int rngbit(rng_ind*, int);
int rngset(rng_ind*, int);
int rngunset(rng_ind*, int);
int rngflip(rng_ind*, int);

int rngstr(char*,rng_ind*,int);
void rngprintbits(rng_ind*);
int bufputch(struct BUF_buffer*, unsigned int);

char* rngexpr(rng_ind*);
int compexpr(rng_ind*, char*);


int compexpr(rng_ind* rng, char* expr)
{
	unsigned char tmp[100];
	unsigned char last = 0;
	unsigned char *cur;
	int i;
	int compliment = 0;
	if(!rng || !expr)
		return -1;

	rnginit(rng);
	cur = expr;

	if(*(cur++) == '[')
	{
		if(*cur == '-')
			fprintf(stderr, "ERROR: \'-\' has no lower bound\n");
		if(*cur && *cur == '^')
		{
			++cur;
			compliment = 1;
		}
		while(*cur)
		{
			switch(*(cur))
			{
				case '\\':
					switch(*(++cur))
					{
						case 'n':
							rngset(rng, '\n');
							last = '\n';
							break;
						case 'r':
							rngset(rng, '\r');
							last = '\r';
							break;
						case 't':
							rngset(rng, '\t');
							last = '\t';
							break;
						case 'f':
							rngset(rng, '\f');
							last = '\f';
							break;
						case 'v':
							rngset(rng, '\v');
							last = '\v';
							break;
						case '-':
						case '{':
						case '<':
						case '\\':
						case '[':
							rngset(rng, *cur);
							last = *cur;
							break;
						case 's':
							rngset(rng, ' ');
							last = ' ';
							break;
						default:
							fprintf(stderr, "ERROR: Unrecognized control character: \\%c\n", *cur);
							return -2;
					}
					break;
				case '<':
					++cur;
					for(i=0; isdigit(*cur); ++i, ++cur)
						tmp[i] = *cur;
					tmp[i] = 0;
					if(!*tmp)
					{
						fprintf(stderr, "ERROR: No digits found in ASCII Integer\n");
						return -2;
					}
					i = atoi(tmp);
					if((i < 256)&&(i >= 0))
						rngset(rng, i);
					else
					{
						fprintf(stderr, "ERROR: ASCII Integer out of range: %u\n", i);
						return -2;
					}
					if(*cur == '>')
						last = '>';
					else
					{
						fprintf(stderr, "ERROR: Expected \'>\'\n");
						return -2;
					}
					break;
				case ']':
					last = ']';
					break;
				default:
					rngset(rng, *cur);
					break;
				}
				++cur;
		}

		if(compliment)
			for(i=0;i<32;i++)
				rng->member[i] = ~(rng->member[i]);
		

		
		return 0;
	}
	else return -2;
}


int bufputch(struct BUF_buffer *buf, unsigned int c)
{
	char fmt[100];
	if(!buf)
		return -1;

	switch(c)
	{
		case '\n':
			BUF_puts(buf, "\\n");
			return 0;
		case '\t':
			BUF_puts(buf, "\\t");
			return 0;
		case '\f':
			BUF_puts(buf, "\\f");
			return 0;
		case '\v':
			BUF_puts(buf, "\\v");
			return 0;
		case '\r':
			BUF_puts(buf, "\\r");
			return 0;
		case ' ':
			BUF_puts(buf, "\\s");
			return 0;
		case '\\':
			BUF_puts(buf, "\\\\");
			return 0;
		case '-':
			BUF_puts(buf, "\\-");
			return 0;
		case '<':
			BUF_puts(buf, "\\<");
			return 0;
		case '{':
			BUF_puts(buf, "\\{");
			return 0;
		case ']':
			BUF_puts(buf, "\\]");
			return 0;
	}

	if(isgraph(c))
		BUF_putc(buf, c);
	else
	{
		sprintf(fmt, "<%u>", c);
		printf("%s\n", fmt);
		BUF_puts(buf, fmt);
	}

	return 0;
}

char* rngexpr(rng_ind* rng)
{
	struct BUF_buffer buffer;
	int i, flag, blen;
	char *ret, *cur;

	if(!rng)
		return NULL;

	BUF_init(&buffer);

	BUF_putc(&buffer, '[');

	for(i=1,flag=0;i<256;i++)
	{
		if(flag)
		{
			if(!rngbit(rng,i))
			{
				bufputch(&buffer, i-1);
				flag = 0;
			}
		}
		else
		{
			if(rngbit(rng,i))
			{
				bufputch(&buffer, i);
				if(rngbit(rng,i+1))
				{
					BUF_putc(&buffer, '-');
					flag = 1;
				}
			}
		}
	}
	if(flag)
		bufputch(&buffer, i-1);
	BUF_putc(&buffer, ']');

	
	blen = BUF_line_len(&buffer);
	blen = (blen < 0) ? -blen : blen; 
	ret = (char*) malloc(blen+1);

	if(ret)
		for(cur = ret; *cur = BUF_getc(&buffer); ++cur);
	else while(BUF_getc(&buffer));

	return ret;
}

void rnginit(rng_ind* rng){memset(rng->member,0,32);}
	 
int rngbit(rng_ind* rng, int i)
{
	if(!rng)
		return -1;
	return charbit(rng->member[i/8], i%8);
}

int rngunset(rng_ind* rng, int i)
{
	if(rng&&(i<256))
		return charunset(rng->member + i/8, i%8);
	else
		return -1;
}

int rngset(rng_ind* rng, int i)
{
	if(rng&&(i<256))
		return charset(rng->member + i/8, i%8);
	else
		return -1;
}

int rngflip(rng_ind* rng, int i)
{
	if(rng&&(i<256))
	{
		return charflip(rng->member + i/8, i%8);
	}
	else return -1;
}

int rngbitcnt(rng_ind* rng)
{
	int i, cnt;
	
	if(!rng)
		return -1;
	
	for(i=0,cnt=0;i<32;i++)
		cnt += (rng->member[i] ? cbitcnt(rng->member[i]) : 0);
	
	return cnt;
}

int rngstr(char* dest, rng_ind* rng, int size)
{
	int i, j, index;

	if(!rng || !dest)
		return -1;
	if(size < rngbitcnt(rng) + 1)
	{
		*dest = 0;
		return -2;
	}
	memset(dest, 0, size); 
	
	for(i=0,index=0;i<32;i++)
		if(rng->member[i])
			for(j=0;j<8;j++)
				if(charbit(rng->member[i], j)>0)
				{
					dest[index] = i*8 + j;
					++index;
				}

	return index;
}


void rngprintbits(rng_ind* rng)
{
	int i,j;
	if(rng)
		for(i=0;i<32;i+=8)
		{
			for(j=0;j<8;j++)
				printf("%08d ", showbits(rng->member[j+i]));
			printf("\n");
		}
		printf("\n");
}

#endif //ifndef RANGE_H
