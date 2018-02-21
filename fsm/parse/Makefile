parser: lex.yy.c table.h parser.tab.c ast.h
	gcc lex.yy.c parser.tab.c -lfl -o parser
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

