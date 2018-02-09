test: lex.yy.c rngcomp.tab.c rngcomp.tab.h lex.h
	gcc  -lfl lex.yy.c rngcomp.tab.c  -o test

lex.h: lex.yy.c

lex.yy.c: rngcomp.l
	flex rngcomp.l

rngcomp.tab.h: rngcomp.y
	bison -d rngcomp.y
