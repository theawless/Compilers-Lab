//Use std=c++11 option

#include <iostream>
#include <bits/stdc++.h>
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
    do
    {
        current = 0;
        token_ids.clear();
        tokens.clear();
        string line;
        cin >> line;
        lexify_line(line);
        for (auto i = 0; i < tokens.size(); i++)
        {
            cout << tokens[i] << " " << token_ids[i] << " ";
        }
        if (stmt_list())
        {
            cout << "Accepted.";
        }
        else
        {
            cout << "Rejected.";
        }
    } while (1);
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

bool stmt_list(Node *currentNode)
{
    int old_position = current;
    Node *n1 = createNode("stmt");
    Node *n2 = createNode("SEMICOLON");
    Node *n3 = createNode("stmt_list_LR");
    if (stmt(n1) && match(SEMICOLON) && stmt_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return false;
}

bool stmt_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (stmt(n) && match(SEMICOLON) && stmt_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (select_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (delete_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (insert_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (update_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (create_database_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (create_table_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (drop_table_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (alter_table_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool select_stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(SELECT) && select_opts(n) && select_expr_list(n) && select_stmt_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool select_stmt_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(FROM) && table_references(n) && opt_where(n) && opt_groupby(n) && opt_having(n) && opt_into_list(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_where(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(WHERE) && expr(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_groupby(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(GROUP) && match(BY) && groupby_list(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool groupby_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (expr(n) && opt_asc_desc(n) && groupby_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool groupby_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && expr(n) && opt_asc_desc(n) && groupby_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_asc_desc(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(ASC))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DESC))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_having(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(HAVING) && expr(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_into_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(INTO) && column_list(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool column_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NAME) && column_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(STRING) && column_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool column_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && column_list_LR_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool column_list_LR_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NAME) && column_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(STRING) && column_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool select_opts(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (select_opts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool select_opts_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(ALL) && select_opts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DISTINCT) && select_opts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool select_expr_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (select_expr(n) && select_expr_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ARITHMETIC_MUL) && select_expr_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool select_expr_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && select_expr(n) && select_expr_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool select_expr(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (expr(n) && opt_as_alias(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool table_references(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (table_factor(n) && table_references_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool table_references_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && table_references(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool table_factor(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NAME) && table_factor_LF2(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && table_factor_LF1(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool table_factor_LF1(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (table_references(n) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (table_subquery(n) && opt_as(n) && match(NAME))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool table_factor_LF2(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(DOT) && match(NAME) && opt_as_alias(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (opt_as_alias(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool table_subquery(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (select_stmt(n) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool opt_as(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(AS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_as_alias(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(AS) && match(NAME))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(NAME))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool delete_stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(DELETE) && match(FROM) && match(NAME) && opt_where(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(INSERT) && opt_into(n) && match(NAME) && opt_col_names(n) && insert_stmt_LF())
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_stmt_LF()
{
    int old_position = current;
    Node *n = createNode("");
    if (match(VALUES) && insert_vals_list(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (select_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool opt_into(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(INTO))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_col_names(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && column_list(n) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool insert_vals_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && insert_vals(n) && match(ENDPARENTHESIS) && insert_vals_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_vals_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && match(STARTPARENTHESIS) && insert_vals(n) && match(ENDPARENTHESIS) && insert_vals_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool insert_vals(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (expr(n) && insert_vals_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DEFAULT) && insert_vals_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_vals_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && insert_vals(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool update_stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(UPDATE) && table_references(n) && match(SET) && update_asgn_list(n) && opt_where(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool update_asgn_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NAME) && update_asgn_list_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool update_asgn_list_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMPARISON) && expr(n) && update_asgn_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DOT) && match(NAME) && match(COMPARISON) && expr(n) && update_asgn_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool update_asgn_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && match(NAME) && update_asgn_list_LR_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool update_asgn_list_LR_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMPARISON) && expr(n) && update_asgn_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DOT) && match(NAME) && match(COMPARISON) && expr(n) && update_asgn_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool create_database_stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(CREATE) && match(DATABASE) && match(NAME))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(CREATE) && match(TABLE) && match(NAME) && create_table_stmt_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && create_col_list(n) && match(ENDPARENTHESIS) && create_table_stmt_LF3(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DOT) && match(NAME) && create_table_stmt_LF2(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt_LF2(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && create_col_list(n) && match(ENDPARENTHESIS) && create_table_stmt_LF3(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (create_select_statement(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt_LF3(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (create_select_statement(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool create_col_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (create_definition(n) && create_col_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool create_col_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && create_definition(n) && create_col_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool create_definition(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NAME) && data_type(n) && column_atts(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(PRIMARY) && match(KEY) && match(STARTPARENTHESIS) && column_list(n) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(KEY) && match(STARTPARENTHESIS) && column_list(n) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool column_atts(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool column_atts_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NOT) && match(NULLX) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(NULLX) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DEFAULT) && column_atts_LR_LF1(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(AUTO_INCREMENT) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(UNIQUE) && column_atts_LR_LF2(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(PRIMARY) && match(KEY) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(KEY) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool column_atts_LR_LF1(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STRING) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(INTNUM) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(BOOL) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool column_atts_LR_LF2(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && column_list(n) && match(ENDPARENTHESIS) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(KEY) && column_atts_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool opt_length(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && match(INTNUM) && opt_length_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_length_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(COMMA) && match(INTNUM) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool data_type(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(INT) && opt_length(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(INTEGER) && opt_length(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(CHAR) && opt_length(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(VARCHAR) && match(STARTPARENTHESIS) && match(INTNUM) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool enum_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(STRING) && enum_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool enum_list_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && match(STRING) && enum_list_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool create_select_statement(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (opt_as(n) && select_stmt(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool val_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (expr(n) && val_list_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool val_list_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COMMA) && val_list(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_val_list(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (val_list(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool expr(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NAME) && expr_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(STRING) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(INTNUM) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(BOOL) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(NOT) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ARITHMETIC_NOT) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ARITHMETIC_SUB) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LF(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && opt_val_list(n) && match(ENDPARENTHESIS) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DOT) && match(NAME) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(ARITHMETIC_ADD) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ARITHMETIC_SUB) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ARITHMETIC_MUL) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ARITHMETIC_DIV) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(MOD) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ANDOP) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(OR) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(XOR) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(COMPARISON) && expr_LR_LF1(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(IS) && expr_LR_LF2(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(BETWEEN) && expr(n) && match(AND) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(IN) && match(STARTPARENTHESIS) && expr_LR_LF4(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(NOT) && expr_LR_LF5(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(LIKE) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(REGEXP) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return true;
}

bool expr_LR_LF1(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(STARTPARENTHESIS) && select_stmt(n) && match(ENDPARENTHESIS) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ANY) && match(STARTPARENTHESIS) && select_stmt(n) && match(ENDPARENTHESIS) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ALL) && match(STARTPARENTHESIS) && select_stmt(n) && match(ENDPARENTHESIS) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF2(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NOT) && expr_LR_LF3(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(NULLX) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(BOOL) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF3(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(NULLX) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(BOOL) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF4(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (val_list(n) && match(ENDPARENTHESIS) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (select_stmt(n) && match(ENDPARENTHESIS) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF5(Node *currentNode)
{
    int old_position = current;
    Node *n = createNode("");
    if (match(IN) && match(STARTPARENTHESIS) && expr_LR_LF4(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(REGEXP) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(LIKE) && expr(n) && expr_LR(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool drop_table_stmt()
{
    int old_position = current;
    Node *n = createNode("");
    if (match(DROP) && match(TABLE) && match(NAME))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool alter_table_stmt()
{
    int old_position = current;
    Node *n = createNode("");
    if (match(ALTER) && match(TABLE) && match(NAME) && column_modify(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool column_modify()
{
    int old_position = current;
    Node *n = createNode("");
    if (match(ALTER) && match(COLUMN) && match(NAME) && data_type(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(DROP) && match(COLUMN) && match(NAME))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(MODIFY) && column_modify_LF(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(ADD) && match(NAME) && data_type(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}

bool column_modify_LF()
{
    int old_position = current;
    Node *n = createNode("");
    if (match(COLUMN) && match(NAME) && data_type)
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    Node *n = createNode("");
    if (match(NAME) && data_type(n))
    {
        add_edge(currentNode, n);
        return true;
    }
    current = old_position;
    return false;
}