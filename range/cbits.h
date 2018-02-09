/* cbits.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef CBITS_H
#	define CBITS_H

int showbits(char);
int cbit(int);
int cbitcnt(unsigned char);

int charbit(unsigned char, int);
int charunset(unsigned char*, int);
int charflip(unsigned char*, int);

int cbit(int i)
{
	if((i>=0)&&(i<8))
		return (1<<i);
	else
		return -1;
}

int showbits(char c)
{
	int ret = 0, i,ten=1;
	for(i=7;i>=0;i--,ten*=10)
		ret += ((!!((cbit(i)&c)))*(ten));
	return ret;
}

int cbitcnt(unsigned char c)
{
	int i,j;
	for(i=0, j=0; i<8; i++)
		j+=!!(c&cbit(i));
	return j;
}

int charbit(unsigned char byte, int i) {return (i<9) ? ((byte>>i)&1) : -1;}

int charset(unsigned char* c, int i)
{
	if(c && (i<8))
		return (*c = *c | cbit(i));
	else 
		return -1;
}

int charunset(unsigned char* c, int i)
{
	if(c && (i<8))
		return (*c = *c & ~cbit(i));
	else 
		return -1;
}

int charflip(unsigned char* c, int i)
{
	if(!c||(i<0)||(i>8))
		return -1;
	else
		return (*c = ( *c & ~cbit(i)) | ((*c & cbit(i)) ? 0 : cbit(i)));
}

#endif // ifndef CBITS_H
