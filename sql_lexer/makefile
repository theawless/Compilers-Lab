all: lexer manual_lexer

lexer: sql.l sql.c
	lex -t sql.l > lex.yy.c
	cc lex.yy.c -ll
	gcc sql.c lex.yy.c -o lexer
	rm -f a.out lex.yy.c

manual_lexer:
	gcc lexer.c -o manual_lexer

clean:
	rm -f lex.c manual_lexer lexer lex.yy.c a.out
