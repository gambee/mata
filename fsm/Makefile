parser: lex.yy.c table.h parser.tab.c ast.h cc_parse.h cc_lex.c
	gcc lex.yy.c parser.tab.c cc_parse.c -lfl -o parser
	cp parser examples/parser
lex.yy.c: lexer.l parser.tab.h
	flex lexer.l
parser.tab.h: parser.tab.c
parser.tab.c: parser.y
	bison -d parser.y
cc_parse.h: cc_parse.tab.h
cc_parse.tab.h: cc_parse.y
	bison cc_parse.y
dot: ast.dot
	dot -Tsvg ast.dot > ast.svg
cc_test: cc_test.c charclass.h cc_lex.c cc_parse.h cc_parse.tab.h
	gcc cc_test.c cc_lex.c -o cc_test
tr_test: tr_test.c charclass.h cc_parse.h cc_lex.c trans_list.h
	gcc tr_test.c cc_lex.c -o tr_test
clean: lex.yy.c parser.tab.c parser.tab.h cc_parse.tab.h cc_parse.c
	rm lex.yy.c parser.tab.c parser.tab.h cc_parse.tab.h cc_parse.c
