#include <stdio.h>

extern int yylex();
extern char* yytext;

int main()
{
	int ntoken, vtoken;
	ntoken = yylex();
	while(ntoken)
	{
		if(ntoken >= 1 && ntoken <= 44)
			printf("Keyword : %s\n", yytext);
		else if(ntoken == 101 || ntoken == 102)
			printf("Operator : %s\n", yytext);
		else if(ntoken == 103)
			printf("Identifier : %s\n", yytext);
		else if(ntoken == 104 || ntoken == 52)
			printf("Literal : %s\n", yytext);
		ntoken = yylex();
	}
	return 0;
}
