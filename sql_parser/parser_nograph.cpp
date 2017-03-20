//Use std=c++11 option

#include <iostream>
#include <bits/stdc++.h>
#include "sql.h"
using namespace std;

int current;
bool error = false;

bool match(int x);
bool stmt_list();
bool stmt_list_LR();
bool stmt();
bool select_stmt();
bool select_stmt_LF();
bool opt_where();
bool opt_groupby();
bool groupby_list();
bool groupby_list_LR();
bool opt_asc_desc();
bool opt_having();
bool opt_into_list();
bool column_list();
bool column_list_LR();
bool column_list_LR_LF();
bool select_opts();
bool select_opts_LR();
bool select_expr_list();
bool select_expr_list_LR();
bool select_expr();
bool table_references();
bool table_references_LF();
bool table_factor();
bool table_factor_LF1();
bool table_factor_LF2();
bool table_subquery();
bool opt_as();
bool opt_as_alias();
bool delete_stmt();
bool insert_stmt();
bool insert_stmt_LF();
bool opt_into();
bool opt_col_names();
bool insert_vals_list();
bool insert_vals_list_LR();
bool insert_vals();
bool insert_vals_LR();
bool update_stmt();
bool update_asgn_list();
bool update_asgn_list_LF();
bool update_asgn_list_LR();
bool update_asgn_list_LR_LF();
bool create_database_stmt();
bool create_table_stmt();
bool create_table_stmt_LF();
bool create_table_stmt_LF2();
bool create_table_stmt_LF3();
bool create_col_list();
bool create_col_list_LR();
bool create_definition();
bool column_atts();
bool column_atts_LR();
bool column_atts_LR_LF1();
bool column_atts_LR_LF2();
bool opt_length();
bool opt_length_LF();
bool data_type();
bool enum_list();
bool enum_list_LR();
bool create_select_statement();
bool val_list();
bool val_list_LF();
bool opt_val_list();
bool expr();
bool expr_LF();
bool expr_LR();
bool expr_LR_LF1();
bool expr_LR_LF2();
bool expr_LR_LF3();
bool expr_LR_LF4();
bool expr_LR_LF5();
bool drop_table_stmt();
bool alter_table_stmt();
bool column_modify();
bool column_modify_LF();

extern vector<string> tokens;
extern vector<int> token_ids;
void lexify_line(string &line);
void run_loop()
{
    while (true)
    {
        current = 0;
        token_ids.clear();
        tokens.clear();
        cout << "Enter statement: ";
        string line;
        getline(cin, line, ';');
        line += ';';
        lexify_line(line);

        for (auto i = 0; i < tokens.size(); i++)
        {
            cout << tokens[i] << " " << token_ids[i] << " ";
        }
        cout << endl;
        if (stmt_list())
        {
            cout << "Accepted." << endl;
        }
        else
        {
            cout << "Rejected." << endl;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 1)
        cerr << "Wrong number of arguments supplied!" << endl;
    run_loop();
    return 0;
}

bool match(int x)
{
    if (token_ids[current] == x)
    {
        current++;
        return true;
    }
    else
        return false;
}

bool stmt_list()
{
    int old_position = current;
    if (stmt() && match(SEMICOLON) && stmt_list_LR())
        return true;
    current = old_position;
    return false;
}

bool stmt_list_LR()
{
    int old_position = current;
    if (stmt() && match(SEMICOLON) && stmt_list_LR())
        return true;
    current = old_position;
    return true;
}

bool stmt()
{
    int old_position = current;
    if (select_stmt())
        return true;
    current = old_position;
    if (delete_stmt())
        return true;
    current = old_position;
    if (insert_stmt())
        return true;
    current = old_position;
    if (update_stmt())
        return true;
    current = old_position;
    if (create_database_stmt())
        return true;
    current = old_position;
    if (create_table_stmt())
        return true;
    current = old_position;
    if (drop_table_stmt())
        return true;
    current = old_position;
    if (alter_table_stmt())
        return true;
    current = old_position;
    return false;
}

bool select_stmt()
{
    int old_position = current;
    if (match(SELECT) && select_opts() && select_expr_list() && select_stmt_LF())
        return true;
    current = old_position;
    return false;
}

bool select_stmt_LF()
{
    int old_position = current;
    if (match(FROM) && table_references() && opt_where() && opt_groupby() && opt_having() && opt_into_list())
        return true;
    current = old_position;
    return true;
}

bool opt_where()
{
    int old_position = current;
    if (match(WHERE) && expr())
        return true;
    current = old_position;
    return true;
}

bool opt_groupby()
{
    int old_position = current;
    if (match(GROUP) && match(BY) && groupby_list())
        return true;
    current = old_position;
    return true;
}

bool groupby_list()
{
    int old_position = current;
    if (expr() && opt_asc_desc() && groupby_list_LR())
        return true;
    current = old_position;
    return false;
}

bool groupby_list_LR()
{
    int old_position = current;
    if (match(COMMA) && expr() && opt_asc_desc() && groupby_list_LR())
        return true;
    current = old_position;
    return true;
}

bool opt_asc_desc()
{
    int old_position = current;
    if (match(ASC))
        return true;
    current = old_position;
    if (match(DESC))
        return true;
    current = old_position;
    return true;
}

bool opt_having()
{
    int old_position = current;
    if (match(HAVING) && expr())
        return true;
    current = old_position;
    return true;
}

bool opt_into_list()
{
    int old_position = current;
    if (match(INTO) && column_list())
        return true;
    current = old_position;
    return true;
}

bool column_list()
{
    int old_position = current;
    if (match(NAME) && column_list_LR())
        return true;
    current = old_position;
    if (match(STRING) && column_list_LR())
        return true;
    current = old_position;
    return false;
}

bool column_list_LR()
{
    int old_position = current;
    if (match(COMMA) && column_list_LR_LF())
        return true;
    current = old_position;
    return true;
}

bool column_list_LR_LF()
{
    int old_position = current;
    if (match(NAME) && column_list_LR())
        return true;
    current = old_position;
    if (match(STRING) && column_list_LR())
        return true;
    current = old_position;
    return false;
}

bool select_opts()
{
    int old_position = current;
    if (select_opts_LR())
        return true;
    current = old_position;
    return false;
}

bool select_opts_LR()
{
    int old_position = current;
    if (match(ALL) && select_opts_LR())
        return true;
    current = old_position;
    if (match(DISTINCT) && select_opts_LR())
        return true;
    current = old_position;
    return true;
}

bool select_expr_list()
{
    int old_position = current;
    if (select_expr() && select_expr_list_LR())
        return true;
    current = old_position;
    if (match(ARITHMETIC_MUL) && select_expr_list_LR())
        return true;
    current = old_position;
    return false;
}

bool select_expr_list_LR()
{
    int old_position = current;
    if (match(COMMA) && select_expr() && select_expr_list_LR())
        return true;
    current = old_position;
    return true;
}

bool select_expr()
{
    int old_position = current;
    if (expr() && opt_as_alias())
        return true;
    current = old_position;
    return false;
}

bool table_references()
{
    int old_position = current;
    if (table_factor() && table_references_LF())
        return true;
    current = old_position;
    return false;
}

bool table_references_LF()
{
    int old_position = current;
    if (match(COMMA) && table_references())
        return true;
    current = old_position;
    return true;
}

bool table_factor()
{
    int old_position = current;
    if (match(NAME) && table_factor_LF2())
        return true;
    current = old_position;
    if (match(STARTPARENTHESIS) && table_factor_LF1())
        return true;
    current = old_position;
    return false;
}

bool table_factor_LF1()
{
    int old_position = current;
    if (table_references() && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    if (table_subquery() && opt_as() && match(NAME))
        return true;
    current = old_position;
    return false;
}

bool table_factor_LF2()
{
    int old_position = current;
    if (match(DOT) && match(NAME) && opt_as_alias())
        return true;
    current = old_position;
    if (opt_as_alias())
        return true;
    current = old_position;
    return false;
}

bool table_subquery()
{
    int old_position = current;
    if (select_stmt() && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    return false;
}

bool opt_as()
{
    int old_position = current;
    if (match(AS))
        return true;
    current = old_position;
    return true;
}

bool opt_as_alias()
{
    int old_position = current;
    if (match(AS) && match(NAME))
        return true;
    current = old_position;
    if (match(NAME))
        return true;
    current = old_position;
    return true;
}

bool delete_stmt()
{
    int old_position = current;
    if (match(DELETE) && match(FROM) && match(NAME) && opt_where())
        return true;
    current = old_position;
    return false;
}

bool insert_stmt()
{
    int old_position = current;
    if (match(INSERT) && opt_into() && match(NAME) && opt_col_names() && insert_stmt_LF())
        return true;
    current = old_position;
    return false;
}

bool insert_stmt_LF()
{
    int old_position = current;
    if (match(VALUES) && insert_vals_list())
        return true;
    current = old_position;
    if (select_stmt())
        return true;
    current = old_position;
    return false;
}

bool opt_into()
{
    int old_position = current;
    if (match(INTO))
        return true;
    current = old_position;
    return true;
}

bool opt_col_names()
{
    int old_position = current;
    if (match(STARTPARENTHESIS) && column_list() && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    return true;
}

bool insert_vals_list()
{
    int old_position = current;
    if (match(STARTPARENTHESIS) && insert_vals() && match(ENDPARENTHESIS) && insert_vals_list_LR())
        return true;
    current = old_position;
    return false;
}

bool insert_vals_list_LR()
{
    int old_position = current;
    if (match(COMMA) && match(STARTPARENTHESIS) && insert_vals() && match(ENDPARENTHESIS) && insert_vals_list_LR())
        return true;
    current = old_position;
    return true;
}

bool insert_vals()
{
    int old_position = current;
    if (expr() && insert_vals_LR())
        return true;
    current = old_position;
    if (match(DEFAULT) && insert_vals_LR())
        return true;
    current = old_position;
    return false;
}

bool insert_vals_LR()
{
    int old_position = current;
    if (match(COMMA) && insert_vals())
        return true;
    current = old_position;
    return true;
}

bool update_stmt()
{
    int old_position = current;
    if (match(UPDATE) && table_references() && match(SET) && update_asgn_list() && opt_where())
        return true;
    current = old_position;
    return false;
}

bool update_asgn_list()
{
    int old_position = current;
    if (match(NAME) && update_asgn_list_LF())
        return true;
    current = old_position;
    return false;
}

bool update_asgn_list_LF()
{
    int old_position = current;
    if (match(COMPARISON) && expr() && update_asgn_list_LR())
        return true;
    current = old_position;
    if (match(DOT) && match(NAME) && match(COMPARISON) && expr() && update_asgn_list_LR())
        return true;
    current = old_position;
    return false;
}

bool update_asgn_list_LR()
{
    int old_position = current;
    if (match(COMMA) && match(NAME) && update_asgn_list_LR_LF())
        return true;
    current = old_position;
    return true;
}

bool update_asgn_list_LR_LF()
{
    int old_position = current;
    if (match(COMPARISON) && expr() && update_asgn_list_LR())
        return true;
    current = old_position;
    if (match(DOT) && match(NAME) && match(COMPARISON) && expr() && update_asgn_list_LR())
        return true;
    current = old_position;
    return false;
}

bool create_database_stmt()
{
    int old_position = current;
    if (match(CREATE) && match(DATABASE) && match(NAME))
        return true;
    current = old_position;
    return false;
}

bool create_table_stmt()
{
    int old_position = current;
    if (match(CREATE) && match(TABLE) && match(NAME) && create_table_stmt_LF())
        return true;
    current = old_position;
    return false;
}

bool create_table_stmt_LF()
{
    int old_position = current;
    if (match(STARTPARENTHESIS) && create_col_list() && match(ENDPARENTHESIS) && create_table_stmt_LF3())
        return true;
    current = old_position;
    if (match(DOT) && match(NAME) && create_table_stmt_LF2())
        return true;
    current = old_position;
    return false;
}

bool create_table_stmt_LF2()
{
    int old_position = current;
    if (match(STARTPARENTHESIS) && create_col_list() && match(ENDPARENTHESIS) && create_table_stmt_LF3())
        return true;
    current = old_position;
    if (create_select_statement())
        return true;
    current = old_position;
    return false;
}

bool create_table_stmt_LF3()
{
    int old_position = current;
    if (create_select_statement())
        return true;
    current = old_position;
    return true;
}

bool create_col_list()
{
    int old_position = current;
    if (create_definition() && create_col_list_LR())
        return true;
    current = old_position;
    return false;
}

bool create_col_list_LR()
{
    int old_position = current;
    if (match(COMMA) && create_definition() && create_col_list_LR())
        return true;
    current = old_position;
    return true;
}

bool create_definition()
{
    int old_position = current;
    if (match(NAME) && data_type() && column_atts())
        return true;
    current = old_position;
    if (match(PRIMARY) && match(KEY) && match(STARTPARENTHESIS) && column_list() && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    if (match(KEY) && match(STARTPARENTHESIS) && column_list() && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    return false;
}

bool column_atts()
{
    int old_position = current;
    if (column_atts_LR())
        return true;
    current = old_position;
    return false;
}

bool column_atts_LR()
{
    int old_position = current;
    if (match(NOT) && match(NULLX) && column_atts_LR())
        return true;
    current = old_position;
    if (match(NULLX) && column_atts_LR())
        return true;
    current = old_position;
    if (match(DEFAULT) && column_atts_LR_LF1())
        return true;
    current = old_position;
    if (match(AUTO_INCREMENT) && column_atts_LR())
        return true;
    current = old_position;
    if (match(UNIQUE) && column_atts_LR_LF2())
        return true;
    current = old_position;
    if (match(PRIMARY) && match(KEY) && column_atts_LR())
        return true;
    current = old_position;
    if (match(KEY) && column_atts_LR())
        return true;
    current = old_position;
    return true;
}

bool column_atts_LR_LF1()
{
    int old_position = current;
    if (match(STRING) && column_atts_LR())
        return true;
    current = old_position;
    if (match(INTNUM) && column_atts_LR())
        return true;
    current = old_position;
    if (match(BOOL) && column_atts_LR())
        return true;
    current = old_position;
    return false;
}

bool column_atts_LR_LF2()
{
    int old_position = current;
    if (match(STARTPARENTHESIS) && column_list() && match(ENDPARENTHESIS) && column_atts_LR())
        return true;
    current = old_position;
    if (match(KEY) && column_atts_LR())
        return true;
    current = old_position;
    return false;
}

bool opt_length()
{
    int old_position = current;
    if (match(STARTPARENTHESIS) && match(INTNUM) && opt_length_LF())
        return true;
    current = old_position;
    return true;
}

bool opt_length_LF()
{
    int old_position = current;
    if (match(ENDPARENTHESIS))
        return true;
    current = old_position;
    if (match(COMMA) && match(INTNUM) && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    return false;
}

bool data_type()
{
    int old_position = current;
    if (match(INT) && opt_length())
        return true;
    current = old_position;
    if (match(CHAR) && opt_length())
        return true;
    current = old_position;
    if (match(VARCHAR) && match(STARTPARENTHESIS) && match(INTNUM) && match(ENDPARENTHESIS))
        return true;
    current = old_position;
    return false;
}

bool enum_list()
{
    int old_position = current;
    if (match(STRING) && enum_list_LR())
        return true;
    current = old_position;
    return false;
}

bool enum_list_LR()
{
    int old_position = current;
    if (match(COMMA) && match(STRING) && enum_list_LR())
        return true;
    current = old_position;
    return true;
}

bool create_select_statement()
{
    int old_position = current;
    if (opt_as() && select_stmt())
        return true;
    current = old_position;
    return false;
}

bool val_list()
{
    int old_position = current;
    if (expr() && val_list_LF())
        return true;
    current = old_position;
    return false;
}

bool val_list_LF()
{
    int old_position = current;
    if (match(COMMA) && val_list())
        return true;
    current = old_position;
    return true;
}

bool opt_val_list()
{
    int old_position = current;
    if (val_list())
        return true;
    current = old_position;
    return true;
}

bool expr()
{
    int old_position = current;
    if (match(NAME) && expr_LF())
        return true;
    current = old_position;
    if (match(STRING) && expr_LR())
        return true;
    current = old_position;
    if (match(INTNUM) && expr_LR())
        return true;
    current = old_position;
    if (match(BOOL) && expr_LR())
        return true;
    current = old_position;
    if (match(NOT) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(ARITHMETIC_NOT) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(ARITHMETIC_SUB) && expr() && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool expr_LF()
{
    int old_position = current;
    if (expr_LR())
        return true;
    current = old_position;
    if (match(STARTPARENTHESIS) && opt_val_list() && match(ENDPARENTHESIS) && expr_LR())
        return true;
    current = old_position;
    if (match(DOT) && match(NAME) && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool expr_LR()
{
    int old_position = current;
    if (match(ARITHMETIC_ADD) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(ARITHMETIC_SUB) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(ARITHMETIC_MUL) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(ARITHMETIC_DIV) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(MOD) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(ANDOP) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(OR) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(XOR) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(COMPARISON) && expr_LR_LF1())
        return true;
    current = old_position;
    if (match(IS) && expr_LR_LF2())
        return true;
    current = old_position;
    if (match(IN) && match(STARTPARENTHESIS) && expr_LR_LF4())
        return true;
    current = old_position;
    if (match(NOT) && expr_LR_LF5())
        return true;
    current = old_position;
    if (match(LIKE) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(REGEXP) && expr() && expr_LR())
        return true;
    current = old_position;
    return true;
}

bool expr_LR_LF1()
{
    int old_position = current;
    if (expr() && expr_LR())
        return true;
    current = old_position;
    if (match(STARTPARENTHESIS) && select_stmt() && match(ENDPARENTHESIS) && expr_LR())
        return true;
    current = old_position;
    if (match(ANY) && match(STARTPARENTHESIS) && select_stmt() && match(ENDPARENTHESIS) && expr_LR())
        return true;
    current = old_position;
    if (match(ALL) && match(STARTPARENTHESIS) && select_stmt() && match(ENDPARENTHESIS) && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool expr_LR_LF2()
{
    int old_position = current;
    if (match(NOT) && expr_LR_LF3())
        return true;
    current = old_position;
    if (match(NULLX) && expr_LR())
        return true;
    current = old_position;
    if (match(BOOL) && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool expr_LR_LF3()
{
    int old_position = current;
    if (match(NULLX) && expr_LR())
        return true;
    current = old_position;
    if (match(BOOL) && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool expr_LR_LF4()
{
    int old_position = current;
    if (val_list() && match(ENDPARENTHESIS) && expr_LR())
        return true;
    current = old_position;
    if (select_stmt() && match(ENDPARENTHESIS) && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool expr_LR_LF5()
{
    int old_position = current;
    if (match(IN) && match(STARTPARENTHESIS) && expr_LR_LF4())
        return true;
    current = old_position;
    if (match(REGEXP) && expr() && expr_LR())
        return true;
    current = old_position;
    if (match(LIKE) && expr() && expr_LR())
        return true;
    current = old_position;
    return false;
}

bool drop_table_stmt()
{
    int old_position = current;
    if (match(DROP) && match(TABLE) && match(NAME))
        return true;
    current = old_position;
    return false;
}

bool alter_table_stmt()
{
    int old_position = current;
    if (match(ALTER) && match(TABLE) && match(NAME) && column_modify())
        return true;
    current = old_position;
    return false;
}

bool column_modify()
{
    int old_position = current;
    if (match(ALTER) && match(COLUMN) && match(NAME) && data_type())
        return true;
    current = old_position;
    if (match(DROP) && match(COLUMN) && match(NAME))
        return true;
    current = old_position;
    if (match(MODIFY) && column_modify_LF())
        return true;
    current = old_position;
    if (match(ADD) && match(NAME) && data_type())
        return true;
    current = old_position;
    return false;
}

bool column_modify_LF()
{
    int old_position = current;
    if (match(COLUMN) && match(NAME) && data_type)
        return true;
    current = old_position;
    if (match(NAME) && data_type())
        return true;
    current = old_position;
    return false;
}