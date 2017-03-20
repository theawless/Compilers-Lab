%{
#include "y.tab.h"

//int lineno = 1;
//void yyerror(char *s);

%}
%option caseless yylineno

%%
				/*keywords*/
"ALL"			return (ALL);
"AND"			return (ANDOP);
"ADD"			return (ADD);
"ANY"			return (ANY);
"ALTER"			return (ALTER);
"AS"			return (AS);
"ASC"			return (ASC);
"AUTO_INCREMENT" return (AUTO_INCREMENT);
"WHERE"			return (WHERE);
"BY"			return (BY);
"CHAR"			return (CHAR);
"CHARACTER"		return (CHAR);
"COLUMN"		return (COLUMN);
"VARCHAR"		return (VARCHAR);
"VARCHARACTER"	return (VARCHAR);
"CREATE"		return (CREATE);
"DEFAULT"		return (DEFAULT);
"DROP"			return (DROP);
"DATABASE"		return (DATABASE);
"DECIMAL"		return (DECIMAL);
"DELETE"		return (DELETE);
"DESC"			return (DESC);
"DISTINCT"		return (DISTINCT);
"FOREIGN"		return (FOREIGN);
"FROM"			return (FROM);
"GRANT"			return (GRANT);
"GROUP"			return (GROUP);
"HAVING"		return (HAVING);
"IN"			return (IN);
"INSERT"		return (INSERT);
"INT"			return (INT);
"INTEGER"		return (INT);
"INTO"			return (INTO);
"IS"			return (IS);
"KEY"			return (KEY);
"LIKE"			return (LIKE);
"MODIFY"		return (MODIFY);
"MOD"			return (MOD);
"NOT"			return (NOT);
"NULL"			return (NULLX);
"NUMERIC"		return (NUMERIC);
"OPTION"		return (OPTION);
"OR"			return (OR);
"ORDER"			return (ORDER);
"PRIMARY"		return (PRIMARY);
"REFERENCES"	return (REFERENCES);
"REGEXP"		return (REGEXP);
"SET"			return (SET);
"SELECT"		return (SELECT);
"TABLE"			return (TABLE);
"UNION"			return (UNION);
"UNIQUE"		return (UNIQUE);
"UPDATE"		return (UPDATE);
"USER"			return (USER);
"VALUES"		return (VALUES);
"XOR"			return (XOR);

		/*ops*/
"="				|
"<>" 			|
"<=>" 			|
"<"				|
">"				|
"<="			|
">="			 			return (COMPARISON);
"-"							return (ARITHMETIC_SUB);
"+"							return (ARITHMETIC_ADD);
"*"							return (ARITHMETIC_MUL);
"/"							return (ARITHMETIC_DIV);
"!"							return (ARITHMETIC_NOT);
"("							return (STARTPARENTHESIS);
")"							return (ENDPARENTHESIS);
","							return (COMMA);
"."							return (DOT);
";"							return (SEMICOLON);

"TRUE"	return (BOOL);
"UNKNOWN"	return (BOOL);
"FALSE"	return (BOOL);

	/*old*/
	/* ["][A-Za-z%][][A-Za-z0-9_%]*["] return (STRING);
	['][A-Za-z%][][A-Za-z0-9_%]*['] return (STRING);
	*/

'(\\.|''|[^'\n])*'	|
\"(\\.|\"\"|[^"\n])*\"  return STRING;

'(\\.|[^'\n])*$	        yyerror("string not complete %s", yytext);
\"(\\.|[^"\n])*$	yyerror("string not complete %s", yytext);


[A-Za-z][A-Za-z0-9_]*		return (NAME);

-?[0-9]+(.[0-9]+)?					return (INTNUM);

[ \t\n]         /* white space */
.               { yyerror("unknown character %s", yytext); }
%%

/*
void yyerror(char *s)
{
	printf("%d: %s at %s\n", lineno, s, yytext);
}
*/

int yywrap(void)
{
	return 1;
}