test: lex.c cc_comp.tab.c cc_comp.tab.h
	gcc lex.c cc_comp.tab.c  -o test

testdb: lex.c cc_comp.tab.c cc_comp.tab.h
	gcc -g lex.c cc_comp.tab.c  -o testdb

cc_comp.tab.c: cc_comp.tab.h
cc_comp.tab.h: cc_comp.y ../charclass.h
	bison -d cc_comp.y
