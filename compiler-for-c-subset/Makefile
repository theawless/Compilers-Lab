all: parser

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h c parser

y.tab.c: c.y
	yacc -d c.y

lex.yy.c: y.tab.c c.l
	lex c.l

parser: y.tab.c lex.yy.c
	g++ --std=c++11 -g lex.yy.c y.tab.c -o parser
	rm -f lex.yy.c y.tab.c y.tab.h