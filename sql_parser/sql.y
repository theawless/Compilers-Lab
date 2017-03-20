%{
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* uses variable list of args*/
void yyerror (char *s, ...);
/* this function used to display when a command is accepted */
void disp(const char *state, const char *string);
%}

/* Yacc definitions */
/* these are our tokens, that have their regexes defined in the lex file. */
%token ALL
%token ANDOP
%token ADD
%token ANY
%token ALTER
%token AS
%token ASC
%token AUTO_INCREMENT
%token WHERE
%token BY
%token CHAR
%token COLUMN
%token VARCHAR
%token CREATE
%token DEFAULT
%token DATABASE
%token DECIMAL
%token DELETE
%token DROP
%token DESC
%token DISTINCT
%token FOREIGN
%token FROM
%token GRANT
%token GROUP
%token HAVING
%token IN
%token INSERT
%token INT
%token INTO
%token IS
%token KEY
%token LIKE
%token MODIFY
%token MOD
%token NOT
%token NULLX
%token NUMERIC
%token OPTION
%token OR
%token ORDER
%token PRIMARY
%token REFERENCES
%token REGEXP
%token SET
%token SELECT
%token TABLE
%token UNION
%token UNIQUE
%token UPDATE
%token USER
%token VALUES
%token XOR

%token COMPARISON
%token ARITHMETIC_SUB
%token ARITHMETIC_ADD
%token ARITHMETIC_MUL
%token ARITHMETIC_DIV
%token ARITHMETIC_NOT
%token STARTPARENTHESIS
%token ENDPARENTHESIS
%token COMMA
%token DOT
%token SEMICOLON

%token BOOL

%token STRING

%token NAME

%token INTNUM

%start stmt_list
%%

/* this grammar is non left recursive, and left factored */
/* bison is a bottom up parser, but we did the above so that we can use the same grammar in our descent recursive parser*/
/*

we removed left recursion by introducing a new non terminal p_LR for non terminal p which causes left recursion
we took care of left factoring by introducing p_LF for non terminal p which cause a problem 

*/
        /* start state*/
stmt_list: stmt SEMICOLON stmt_list_LR

stmt_list_LR: /* empty */
            | stmt SEMICOLON stmt_list_LR

stmt: select_stmt {disp("Accepted", "select");}
    | delete_stmt {disp("Accepted", "delete");}
    | insert_stmt {disp("Accepted", "insert");}
    | update_stmt {disp("Accepted", "update");}
    | create_database_stmt {disp("Accepted", "create db");}
    | create_table_stmt {disp("Accepted", "create table");}
    | alter_table_stmt {disp("Accepted", "alter table");}
    | drop_table_stmt {disp("Accepted", "drop table");}

select_stmt: SELECT select_opts select_expr_list select_stmt_LF

select_stmt_LF: /* empty */
            | FROM table_references opt_where 
                opt_groupby opt_having opt_into_list

opt_where: /* empty */
        | WHERE expr

opt_groupby: /* empty */
            | GROUP BY groupby_list

groupby_list: expr opt_asc_desc groupby_list_LR

groupby_list_LR: /* empty */
                | COMMA expr opt_asc_desc groupby_list_LR

opt_asc_desc: /* empty */
            | ASC
            | DESC

opt_having: /* empty */
        | HAVING expr

opt_into_list: /* empty */
            | INTO column_list

column_list:  NAME column_list_LR
            | STRING column_list_LR

column_list_LR: /* empty */
            | COMMA column_list_LR_LF

column_list_LR_LF: NAME column_list_LR
                | STRING column_list_LR

select_opts: select_opts_LR

select_opts_LR: /* empty */
            | ALL select_opts_LR
            | DISTINCT select_opts_LR

select_expr_list: select_expr select_expr_list_LR
                | ARITHMETIC_MUL select_expr_list_LR

select_expr_list_LR: /* empty */
                    | COMMA select_expr select_expr_list_LR

select_expr: expr opt_as_alias

table_references: table_factor table_references_LF

table_references_LF: /* empty */
                    | COMMA table_references

table_factor: NAME table_factor_LF2
            | STARTPARENTHESIS table_factor_LF1

table_factor_LF1: table_references ENDPARENTHESIS
                | table_subquery opt_as NAME

table_factor_LF2: DOT NAME opt_as_alias
                | opt_as_alias

table_subquery: select_stmt ENDPARENTHESIS

opt_as: AS
    | /* empty */

opt_as_alias: AS NAME
            | NAME
            | /* empty */

delete_stmt: DELETE FROM NAME opt_where

insert_stmt: INSERT opt_into NAME opt_col_names insert_stmt_LF

insert_stmt_LF: VALUES insert_vals_list
            | select_stmt

opt_into: INTO
        | /* empty */

opt_col_names: /* empty */
            | STARTPARENTHESIS column_list ENDPARENTHESIS

insert_vals_list: STARTPARENTHESIS insert_vals ENDPARENTHESIS insert_vals_list_LR

insert_vals_list_LR: /* empty */
                    | COMMA STARTPARENTHESIS insert_vals ENDPARENTHESIS insert_vals_list_LR

insert_vals: expr insert_vals_LR
            | DEFAULT insert_vals_LR

insert_vals_LR: /* empty */
            | COMMA insert_vals

update_stmt: UPDATE table_references SET update_asgn_list opt_where

update_asgn_list: NAME update_asgn_list_LF

update_asgn_list_LF: COMPARISON expr update_asgn_list_LR
                    | DOT NAME COMPARISON expr update_asgn_list_LR

update_asgn_list_LR: /* empty */
                    | COMMA NAME update_asgn_list_LR_LF

update_asgn_list_LR_LF: COMPARISON expr update_asgn_list_LR
                    | DOT NAME COMPARISON expr update_asgn_list_LR

create_database_stmt: CREATE DATABASE NAME

create_table_stmt: CREATE TABLE NAME create_table_stmt_LF

create_table_stmt_LF: STARTPARENTHESIS create_col_list ENDPARENTHESIS create_table_stmt_LF3
                    | DOT NAME create_table_stmt_LF2

create_table_stmt_LF2: STARTPARENTHESIS create_col_list ENDPARENTHESIS create_table_stmt_LF3
                    |  create_select_statement

create_table_stmt_LF3: /**/
                    |  create_select_statement

create_col_list: create_definition create_col_list_LR

create_col_list_LR: /* empty */
                | COMMA create_definition create_col_list_LR

create_definition: NAME data_type column_atts
                | PRIMARY KEY STARTPARENTHESIS column_list ENDPARENTHESIS
                | KEY STARTPARENTHESIS column_list ENDPARENTHESIS

column_atts: column_atts_LR

column_atts_LR: /* empty */
            | NOT NULLX column_atts_LR
            | NULLX column_atts_LR
            | DEFAULT column_atts_LR_LF1
            | AUTO_INCREMENT column_atts_LR
            | UNIQUE column_atts_LR_LF2
            | PRIMARY KEY column_atts_LR
            | KEY column_atts_LR

column_atts_LR_LF1: STRING column_atts_LR
                | INTNUM column_atts_LR
                | BOOL column_atts_LR

column_atts_LR_LF2: STARTPARENTHESIS column_list ENDPARENTHESIS column_atts_LR
                | KEY column_atts_LR

opt_length: /* empty */
        | STARTPARENTHESIS INTNUM opt_length_LF

opt_length_LF: ENDPARENTHESIS
        | COMMA INTNUM ENDPARENTHESIS

data_type: INT opt_length
        | CHAR opt_length
        | VARCHAR STARTPARENTHESIS INTNUM ENDPARENTHESIS

enum_list: STRING enum_list_LR

enum_list_LR: /* empty */
            | COMMA STRING enum_list_LR

create_select_statement: opt_as select_stmt

val_list: expr val_list_LF

val_list_LF: /* empty */
            | COMMA val_list

opt_val_list: /* empty */
            | val_list

expr: NAME expr_LF
    | STRING expr_LR
    | INTNUM expr_LR
    | BOOL expr_LR
    | NOT expr expr_LR
    | ARITHMETIC_NOT expr expr_LR
    | ARITHMETIC_SUB expr expr_LR

expr_LF: STARTPARENTHESIS opt_val_list ENDPARENTHESIS expr_LR
    | DOT NAME expr_LR
    | expr_LR

expr_LR: ARITHMETIC_ADD expr expr_LR
    | ARITHMETIC_SUB expr expr_LR
    | ARITHMETIC_MUL expr expr_LR
    | ARITHMETIC_DIV expr expr_LR
    | MOD expr expr_LR
    | ANDOP expr expr_LR
    | OR expr expr_LR
    | XOR expr expr_LR
    | COMPARISON expr_LR_LF1
    | IS expr_LR_LF2
    | IN STARTPARENTHESIS expr_LR_LF4
    | NOT expr_LR_LF5
    | LIKE expr expr_LR
    | REGEXP expr expr_LR
    | /* empty */

expr_LR_LF1: expr expr_LR
        | STARTPARENTHESIS select_stmt ENDPARENTHESIS expr_LR
        | ANY STARTPARENTHESIS select_stmt ENDPARENTHESIS expr_LR
        | ALL STARTPARENTHESIS select_stmt ENDPARENTHESIS expr_LR

expr_LR_LF2: NOT expr_LR_LF3 
        | NULLX expr_LR
        | BOOL expr_LR

expr_LR_LF3: NULLX expr_LR
        | BOOL expr_LR

expr_LR_LF4: val_list ENDPARENTHESIS expr_LR
        | select_stmt ENDPARENTHESIS expr_LR

expr_LR_LF5: IN STARTPARENTHESIS expr_LR_LF4
        | REGEXP expr expr_LR
        | LIKE expr expr_LR

drop_table_stmt: DROP TABLE NAME

alter_table_stmt: ALTER TABLE NAME column_modify

column_modify: ALTER COLUMN NAME data_type
                | DROP COLUMN NAME
                | MODIFY column_modify_LF
                | ADD NAME data_type

column_modify_LF: COLUMN NAME data_type
                    | NAME data_type
%%
                     /* C code */
//void yyerror (char *s) {fprintf (stderr, "%s\n", s);} 

void disp(const char *state,const char *string)
{
  printf("display: %s %s statement", state, string);
  printf("\n");
}

void yyerror(char *s, ...)
{

  extern yylineno;
  /* open up the variable args list */
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, ".\n");

  va_end(ap);
}

int main (void) {
  yyparse();
}