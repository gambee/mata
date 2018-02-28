/* charclass.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef CHARCLASS_H
#	define CHARCLASS_H
#	include <string.h>
#	include "buffer.tab.h"
#	include "cbits.h"

#ifndef CHARCLASS_STRUCT
# 	define CHARCLASS_STRUCT
typedef struct {char member[32];} charclass;
#endif

void cc_init(charclass*);
int cc_set(charclass*, int);
int cc_unset(charclass*, int);
int cc_flip(charclass*, int);
int cc_copy(charclass*, charclass*);

int cc_bitcnt(charclass*);
int cc_bit(charclass*, int);
int cc_str(char*,charclass*,int);
void cc_printbits(charclass*);
int bufputch(struct BUF_buffer*, unsigned int);

char* cc_expr(charclass*);
int compexpr(charclass*, char*);
void cc_codegen(charclass*, char*, FILE*);

int cc_copy(charclass* dest, charclass* src)
{
	int i;
	if(dest && src)
		for(i=0; i<32; ++i)
			dest->member[i] = src->member[i];
	else return -1;
}

void cc_codegen(charclass* cclass, char* name, FILE* outfile)
{
	int i;
	fprintf(outfile, "static struct{char member[32];}%s={{\n", name);
	for(i = 0; i < 31; ++i)
	{
		if(i == 8 || i == 16 || i == 24)
			putc('\n', outfile);
		fprintf(outfile, " 0x%02x,", (unsigned char) cclass->member[i]);
	}
	fprintf(outfile, " 0x%02x}};\n\n", (unsigned char) cclass->member[i]);
}

int compexpr(charclass* cclass, char* expr)
{
	unsigned char tmp[100];
	unsigned char last = 0;
	unsigned char *cur;
	int i;
	int compliment = 0;
	if(!cclass || !expr)
		return -1;

	cc_init(cclass);
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
							cc_set(cclass, '\n');
							last = '\n';
							break;
						case 'r':
							cc_set(cclass, '\r');
							last = '\r';
							break;
						case 't':
							cc_set(cclass, '\t');
							last = '\t';
							break;
						case 'f':
							cc_set(cclass, '\f');
							last = '\f';
							break;
						case 'v':
							cc_set(cclass, '\v');
							last = '\v';
							break;
						case '-':
						case '{':
						case '<':
						case '\\':
						case '[':
							cc_set(cclass, *cur);
							last = *cur;
							break;
						case 's':
							cc_set(cclass, ' ');
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
						cc_set(cclass, i);
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
					cc_set(cclass, *cur);
					break;
				}
				++cur;
		}

		if(compliment)
			for(i=0;i<32;i++)
				cclass->member[i] = ~(cclass->member[i]);
		

		
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

char* cc_expr(charclass* cclass)
{
	struct BUF_buffer buffer;
	int i, flag, blen;
	char *ret, *cur;

	if(!cclass)
		return NULL;

	BUF_init(&buffer);

	BUF_putc(&buffer, '[');

	for(i=1,flag=0;i<256;i++)
	{
		if(flag)
		{
			if(!cc_bit(cclass,i))
			{
				bufputch(&buffer, i-1);
				flag = 0;
			}
		}
		else
		{
			if(cc_bit(cclass,i))
			{
				bufputch(&buffer, i);
				if(cc_bit(cclass,i+1))
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

	
	blen = BUF_len(&buffer);
	blen = (blen < 0) ? -blen : blen; 
	ret = (char*) malloc(blen+1);

	if(ret)
		for(cur = ret; *cur = BUF_getc(&buffer); ++cur);
	else while(BUF_getc(&buffer));

	return ret;
}

void cc_init(charclass* cclass){memset(cclass->member,0,32);}
	 
int cc_bit(charclass* cclass, int i)
{
	if(!cclass)
		return -1;
	return charbit(cclass->member[i/8], i%8);
}

int cc_unset(charclass* cclass, int i)
{
	if(cclass&&(i<256))
		return charunset(cclass->member + i/8, i%8);
	else
		return -1;
}

int cc_set(charclass* cclass, int i)
{
	if(cclass&&(i<256))
		return charset(cclass->member + i/8, i%8);
	else
		return -1;
}

int cc_flip(charclass* cclass, int i)
{
	if(cclass&&(i<256))
	{
		return charflip(cclass->member + i/8, i%8);
	}
	else return -1;
}

int cc_bitcnt(charclass* cclass)
{
	int i, cnt;
	
	if(!cclass)
		return -1;
	
	for(i=0,cnt=0;i<32;i++)
		cnt += (cclass->member[i] ? cbitcnt(cclass->member[i]) : 0);
	
	return cnt;
}

int cc_str(char* dest, charclass* cclass, int size)
{
	int i, j, index;

	if(!cclass || !dest)
		return -1;
	if(size < cc_bitcnt(cclass) + 1)
	{
		*dest = 0;
		return -2;
	}
	memset(dest, 0, size); 
	
	for(i=0,index=0;i<32;i++)
		if(cclass->member[i])
			for(j=0;j<8;j++)
				if(charbit(cclass->member[i], j)>0)
				{
					dest[index] = i*8 + j;
					++index;
				}

	return index;
}


void cc_printbits(charclass* cclass)
{
	int i,j;
	if(cclass)
		for(i=0;i<32;i+=8)
		{
			for(j=0;j<8;j++)
				printf("%08d ", showbits(cclass->member[j+i]));
			printf("\n");
		}
		printf("\n");
}

#endif //ifndef RANGE_H
