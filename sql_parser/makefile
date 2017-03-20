all: sql sql_manual

sql: lex.yy.c y.tab.c
	gcc -g lex.yy.c y.tab.c -o sql

lex.yy.c: y.tab.c sql.l
	lex sql.l

y.tab.c: sql.y
	yacc -dg sql.y

clean: 
	rm -f lex.yy.c y.tab.c y.tab.h sql sql_manual sql_manual_nograph *.out

sql_manual: lexer.cpp sql.h parser.cpp
	g++ lexer.cpp parser.cpp -o sql_manual

sql_manual_nograph: lexer.cpp sql.h parser_nograph.cpp
	g++ lexer.cpp parser_nograph.cpp -o sql_manual_nograph