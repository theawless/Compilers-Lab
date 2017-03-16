%{
#include "sql.h"
%}
%option caseless

%%
		/*keywords*/
"ALL"	return ALL;
"AND"	return (AND);
"COUNT"	return (AMMSC);
"MAX"	return (AMMSC);
"AVG"	return (AMMSC);
"MIN"	return (AMMSC);
"SUM"	return (AMMSC);
"ANY"	return (ANY);
"AS"	return (AS);
"WHERE"	return (BETWEEN);
"BY"		return (BY);
"CHAR"	return (CHARACTER);
"VARCHAR"	return (CHARACTER);
"CHAR"	return (CHARACTER);
"VARCHARACTER"	return (CHARACTER);
"CREATE"	return (CREATE);
"DECIMAL"	return (DECIMAL);
"DELETE"	return (DELETE);
"DESC"		return (DESC);
"DISTINCT"	return (DISTINCT);
"FOREIGN"	return (FOREIGN);
"FROM"	return (FROM);
"GRANT"	return (GRANT);
"GROUP"	return (GROUP);
"HAVING"		return (HAVING);
"IN"			return (IN);
"INSERT"		return (INSERT);
"INT"	return (INTEGER);
"INTEGER"	return (INTEGER);
"INTO"	return (INTO);
"IS"	return (IS);
"KEY"	return (KEY);
"LIKE"	return (LIKE);
"NOT"	return (NOT);
"NULL"	return (NULLX);
"NUMERIC"	return (NUMERIC);
"OPTION"	return (OPTION);
"OR"		return (OR);
"ORDER"		return (ORDER);
"PRIMARY"	return (PRIMARY);
"REFERENCES"	return (REFERENCES);
"SELECT"	return (SELECT);
"TABLE"		return (TABLE);
"UNION"		return (UNION);
"UNIQUE"	return (UNIQUE);
"UPDATE"	return (UPDATE);
"USER"		return (USER);
"VALUES"	return (VALUES);

		/*ops*/
"="				|
"<>" 			|
"<"				|
">"				|
"<="			|
">="			 			return  COMPARISON;	
[-+*/(),.;]	 	 			return  ARITHMETIC;

["][A-Za-z%][][A-Za-z0-9_%]*["] return LITERALS;
['][A-Za-z%][][A-Za-z0-9_%]*['] return LITERALS;

[A-Za-z][A-Za-z0-9_]*		return IDENTIFIERS;

[0-9]+			|
[0-9]+"."[0-9]* |
"."[0-9]*					return INT_NUMBER;

%%

int yywrap(void)
{
	return 1;
}
