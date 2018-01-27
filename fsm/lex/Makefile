lexer: lex.yy.c table.h
	gcc -o lexer -lfl lex.yy.c
lex.yy.c: lexer.l
	flex lexer.l
table.h: ../table/table.h tablist.h
	cp ../table/table.h ./
tablist.h: ../table/tablist.h
	cp ../table/tablist.h ./
clean: lexer lex.yy.c table.h tablist.h
	rm lex.yy.c table.h tablist.h
