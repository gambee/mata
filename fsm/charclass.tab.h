/* charclass.tab.h
 *
 * Max Gambee
 * Copyright 2018
 */

#ifndef CHARCLASS_TAB_H
#	define CHARCLASS_TAB_H
#	include "buffer.tab.h"

#ifndef CHARCLASS_STRUCT
#	define CHARCLASS_STRUCT
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
int compexpr(charclass*, char*);
void cc_codegen(charclass*, char*, FILE*);
int cc_copy(charclass* dest, charclass* src);
void cc_codegen(charclass* cclass, char* name, FILE* outfile);
int compexpr(charclass* cclass, char* expr);
int bufputch(struct BUF_buffer *buf, unsigned int c);
void cc_init(charclass* cclass);
int cc_bit(charclass* cclass, int i);
int cc_unset(charclass* cclass, int i);
int cc_set(charclass* cclass, int i);
int cc_flip(charclass* cclass, int i);
int cc_bitcnt(charclass* cclass);
int cc_str(char* dest, charclass* cclass, int size);
void cc_printbits(charclass* cclass);
#endif
