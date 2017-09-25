%{
	#include "y.tab.h"
%}
%option caseless noyywrap yylineno

preDir									("#include"[ \t]+.*)|("#define"[ \t]+.*)

lineComment								"//".*
blockComment							"/*"((("*"[^/])?)|[^*])*"*/"

string									\"([^\"]|\\.)*\"
charLit									\'([^\']|\\.)?\'

letter									[a-zA-Z]
digit									[0-9]
ws										[ \t\v\f]
digits									{digit}+
number									(-)?{digits}("."{digits})?(E("+"|"-")?{digits})?

illegal									{`~@#$_\':?}

dt										("char"|"double"|"float"|"int"|"long"|"short"|"signed"|"unsigned")
modifier                                ("const"|"static")
id										({letter}|_)({letter}|_|{digit})*

%%

"void"									{return (VOID);}
"if"									{return (IF);}
"else"									{return (ELSE);}
"while"									{return (WHILE);}
"do"									{return (DO);}
"for"									{return (FOR);}
"return"								{return (RETURN);}
"continue"								{return (CONTINUE);}
"break"									{return (BREAK);}
"struct"								{return (STRUCT);}

{lineComment} 							{}
{blockComment}  						{}
{preDir}								{}
{string}			 					{return (STRING);}
{charLit}			 					{return (CHAR_LIT);}
{dt}									{return (DATA_TYPE);}
{modifier}                              {return (MODIFIER);}
{digits}								{return (INTEGER);}
{number}								{return (FLOATING_POINT);}
{id}									{return (IDEN);}
"+="									{return (ADD_ASSIGN);}
"-="									{return (SUB_ASSIGN);}
"*="									{return (MUL_ASSIGN);}
"/="									{return (DIV_ASSIGN);}
"%="									{return (MOD_ASSIGN);}
"&="									{return (AND_ASSIGN);}
"^="									{return (XOR_ASSIGN);}
"|="									{return (OR_ASSIGN);}
">>"									{return (RIGHT_OP);}
"<<"									{return (LEFT_OP);}
"++"									{return (INC_OP);}
"--"									{return (DEC_OP);}
"->"									{return (PTR_OP);}
"&&"									{return (AND_OP);}
"||"									{return (OR_OP);}
"<="									{return (LE_OP);}
">="									{return (GE_OP);}
"=="									{return (EQ_OP);}
"!="									{return (NE_OP);}
";"										{return (';');}
("{")	    							{return ('{');}
("}")   								{return ('}');}
","										{return (',');}
":"										{return (':');}
"="										{return ('=');}
"("										{return ('(');}
")"										{return (')');}
("[")	    							{return ('[');}
("]")   								{return (']');}
"."										{return ('.');}
"&"										{return ('&');}
"!"										{return ('!');}
"~"										{return ('~');}
"-"										{return ('-');}
"+"										{return ('+');}
"*"										{return ('*');}
"/"										{return ('/');}
"%"										{return ('%');}
"<"										{return ('<');}
">"										{return ('>');}
"^"										{return ('^');}
"|"										{return ('|');}
"?"										{return ('?');}
\n										{}
{ws}									{}
.                                       {}

%%