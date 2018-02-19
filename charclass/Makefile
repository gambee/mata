testdb: test
	gcc -g cc_lex.c main.c -o testdb
test: main.c cc_parse.tab.h cc_parse.h buffer.h buffer.tab.h cc_lex.c charclass.h cbits.h
	gcc cc_lex.c main.c -o test

cc_parse.h: cc_parse.tab.h
cc_parse.tab.h: cc_parse.y
	bison cc_parse.y

clean: cc_parse.tab.h cc_parse.h 
	rm cc_parse.tab.h cc_parse.h 

