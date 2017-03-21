#include <iostream>
#include <bits/stdc++.h>
#include "sql.h"
using namespace std;
// node class to help us print the syntax tree
class Node
{
  public:
    string tokenName;
    bool isNonTerminal;
    int height;
    Node *parent;
    vector<Node *> children;
    Node(string name);
    ~Node();
};

Node::Node(string name)
{
    isNonTerminal = false;
    height = 0;
    tokenName = name;
    parent = NULL;
}

// prints the syntax tree
void printTree(Node *currentNode);
// returns true if token matches the terminal
bool match(int x);
// declarations of all the functions corresponding to the nonterminals
bool stmt_list(Node *currentNode);
bool stmt_list_LR(Node *currentNode);
bool stmt(Node *currentNode);
bool select_stmt(Node *currentNode);
bool select_stmt_LF(Node *currentNode);
bool opt_where(Node *currentNode);
bool opt_groupby(Node *currentNode);
bool groupby_list(Node *currentNode);
bool groupby_list_LR(Node *currentNode);
bool opt_asc_desc(Node *currentNode);
bool opt_having(Node *currentNode);
bool opt_into_list(Node *currentNode);
bool column_list(Node *currentNode);
bool column_list_LR(Node *currentNode);
bool column_list_LR_LF(Node *currentNode);
bool select_opts(Node *currentNode);
bool select_opts_LR(Node *currentNode);
bool select_expr_list(Node *currentNode);
bool select_expr_list_LR(Node *currentNode);
bool select_expr(Node *currentNode);
bool table_references(Node *currentNode);
bool table_references_LF(Node *currentNode);
bool table_factor(Node *currentNode);
bool table_factor_LF1(Node *currentNode);
bool table_factor_LF2(Node *currentNode);
bool table_subquery(Node *currentNode);
bool opt_as(Node *currentNode);
bool opt_as_alias(Node *currentNode);
bool delete_stmt(Node *currentNode);
bool insert_stmt(Node *currentNode);
bool insert_stmt_LF(Node *currentNode);
bool opt_into(Node *currentNode);
bool opt_col_names(Node *currentNode);
bool insert_vals_list(Node *currentNode);
bool insert_vals_list_LR(Node *currentNode);
bool insert_vals(Node *currentNode);
bool insert_vals_LR(Node *currentNode);
bool update_stmt(Node *currentNode);
bool update_asgn_list(Node *currentNode);
bool update_asgn_list_LF(Node *currentNode);
bool update_asgn_list_LR(Node *currentNode);
bool update_asgn_list_LR_LF(Node *currentNode);
bool create_database_stmt(Node *currentNode);
bool create_table_stmt(Node *currentNode);
bool create_table_stmt_LF(Node *currentNode);
bool create_table_stmt_LF2(Node *currentNode);
bool create_table_stmt_LF3(Node *currentNode);
bool create_col_list(Node *currentNode);
bool create_col_list_LR(Node *currentNode);
bool create_definition(Node *currentNode);
bool column_atts(Node *currentNode);
bool column_atts_LR(Node *currentNode);
bool column_atts_LR_LF1(Node *currentNode);
bool column_atts_LR_LF2(Node *currentNode);
bool opt_length(Node *currentNode);
bool opt_length_LF(Node *currentNode);
bool data_type(Node *currentNode);
bool enum_list(Node *currentNode);
bool enum_list_LR(Node *currentNode);
bool create_select_statement(Node *currentNode);
bool val_list(Node *currentNode);
bool val_list_LF(Node *currentNode);
bool opt_val_list(Node *currentNode);
bool expr(Node *currentNode);
bool expr_LF(Node *currentNode);
bool expr_LR(Node *currentNode);
bool expr_LR_LF1(Node *currentNode);
bool expr_LR_LF2(Node *currentNode);
bool expr_LR_LF3(Node *currentNode);
bool expr_LR_LF4(Node *currentNode);
bool expr_LR_LF5(Node *currentNode);
bool drop_table_stmt(Node *currentNode);
bool alter_table_stmt(Node *currentNode);
bool column_modify(Node *currentNode);
bool column_modify_LF(Node *currentNode);
void printError();

// stores the current pointer of the tokens and farthest matched token
int current;
int farthest;

// list of all tokens of input, defined in lexer
extern vector<string> tokens;
// list of all tokens ids of input for matching terminals, defined in lexer
extern vector<int> token_ids;

// populates the above lists , defined in lexer
void lexify_line(string &line);

// iteratively take the input statements
void run_loop()
{
    while (true)
    {
        current = farthest = 0;
        token_ids.clear();
        tokens.clear();
        cout << "Enter statement: ";
        string line;

        // wait till we get a ';' in the input line, otherwise keep inputting the line
        getline(cin, line, ';');
        if (!cin)
        {
            return;
        }
        line += ';';
        lexify_line(line);

        /*        for (auto i = 0; i < tokens.size(); i++)
        {
            cout << tokens[i] << " " << token_ids[i] << " ";
        }
*/
        cout << endl;
        // this is the head node, stmt_list is the start non terminal corresponding to first production
        Node *head = new Node("stmt_list");
        if (stmt_list(head))
        {
            cout << "Accepted." << endl;
            printTree(head);
        }
        else
        {
            printError();
            cout << "Rejected." << endl;
        }
    }
}

// entry point
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
        if (current > farthest)
        {
            farthest = current;
        }
        return true;
    }
    else
        return false;
}

// creates the node for nonterminal symbol identified by name
Node *createNode(string name)
{
    Node *temp = new Node(name);
    return temp;
}

// adds an edge between parent and child for each production rule
void add_edge(Node *par, Node *child)
{
    (*par).children.push_back(child);
    (*child).parent = par;
}

void printError()
{
    cout << "Syntax Error." << endl;
    cout << farthest << ":: "
         << "Unexpected token \"" << tokens[farthest] << "\"" << endl;
}

void printTree(Node *currentNode)
{
    vector<Node *> temp = (*currentNode).children;
    if (!(*currentNode).isNonTerminal)
    {
        return;
    }
    for (int i = 0; i < (*currentNode).height; ++i)
    {
        cout << "\t";
    }
    cout << (*currentNode).tokenName << " -> ";
    if (temp.size() == 0)
    {
        cout << "EPSILON" << endl;
        return;
    }
    else
    {
        for (int i = 0; i < temp.size(); ++i)
        {
            cout << (*(temp[i])).tokenName << " ";
        }
        cout << endl;
        for (int i = 0; i < temp.size(); ++i)
        {
            temp[i]->height = currentNode->height + 1;
            printTree(temp[i]);
        }
        return;
    }
}

/**
production rule:
    stmt_list: stmt SEMICOLON stmt_list_LR

return true if it matches any production
**/
bool stmt_list(Node *currentNode)
{
    // mark the current node as non terminal
    (*currentNode).isNonTerminal = true;
    // remember the current index in token list
    int old_position = current;
    // create nodes for each child of the production
    Node *n1 = createNode("stmt");
    Node *n2 = createNode("SEMICOLON");
    Node *n3 = createNode("stmt_list_LR");
    // if all children match then add the corresponding egde in the syntax tree
    if (stmt(n1) && match(SEMICOLON) && stmt_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    // if it is false, then update the current pointer back to the old position and return false
    // in case the production produces epsilon then we always return true -- for example look stmt_list_LR

    current = old_position;
    return false;
}

bool stmt_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
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
    // always return true because production produces epsilon
    return true;
}

bool stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("select_stmt");
    if (select_stmt(n1))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    Node *n2 = createNode("insert_stmt");
    if (insert_stmt(n2))
    {
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("update_stmt");
    if (update_stmt(n3))
    {
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("create_database_stmt");
    if (create_database_stmt(n4))
    {
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("create_table_stmt");
    if (create_table_stmt(n5))
    {
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    Node *n6 = createNode("drop_table_stmt");
    if (drop_table_stmt(n6))
    {
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    Node *n7 = createNode("alter_table_stmt");
    if (alter_table_stmt(n7))
    {
        add_edge(currentNode, n7);
        return true;
    }
    current = old_position;
    Node *n8 = createNode("delete_stmt");
    if (delete_stmt(n8))
    {
        add_edge(currentNode, n8);
        return true;
    }
    current = old_position;
    return false;
}

bool select_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("SELECT");
    Node *n2 = createNode("select_opts");
    Node *n3 = createNode("select_expr_list");
    Node *n4 = createNode("select_stmt_LF");
    if (match(SELECT) && select_opts(n2) && select_expr_list(n3) && select_stmt_LF(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool select_stmt_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("FROM");
    Node *n2 = createNode("table_references");
    Node *n3 = createNode("opt_where");
    Node *n4 = createNode("opt_groupby");
    Node *n5 = createNode("opt_having");
    Node *n6 = createNode("opt_into_list");
    if (match(FROM) && table_references(n2) && opt_where(n3) && opt_groupby(n4) && opt_having(n5) && opt_into_list(n6))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_where(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("WHERE");
    Node *n2 = createNode("expr");
    if (match(WHERE) && expr(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_groupby(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("GROUP");
    Node *n2 = createNode("BY");
    Node *n3 = createNode("groupby_list");
    if (match(GROUP) && match(BY) && groupby_list(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool groupby_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("expr");
    Node *n2 = createNode("opt_asc_desc");
    Node *n3 = createNode("groupby_list_LR");
    if (expr(n1) && opt_asc_desc(n2) && groupby_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return false;
}

bool groupby_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("expr");
    Node *n3 = createNode("opt_asc_desc");
    Node *n4 = createNode("groupby_list_LR");
    if (match(COMMA) && expr(n2) && opt_asc_desc(n3) && groupby_list_LR(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_asc_desc(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("ASC");
    if (match(ASC))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    Node *n2 = createNode("DESC");
    if (match(DESC))
    {
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_having(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("HAVING");
    Node *n2 = createNode("expr");
    if (match(HAVING) && expr(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_into_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("INTO");
    Node *n2 = createNode("column_list");
    if (match(INTO) && column_list(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool column_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NAME");
    Node *n2 = createNode("column_list_LR");
    if (match(NAME) && column_list_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("STRING");
    Node *n4 = createNode("column_list_LR");
    if (match(STRING) && column_list_LR(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool column_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("column_list_LR_LF");
    if (match(COMMA) && column_list_LR_LF(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool column_list_LR_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NAME");
    Node *n2 = createNode("column_list_LR");
    if (match(NAME) && column_list_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("STRING");
    Node *n4 = createNode("column_list_LR");
    if (match(STRING) && column_list_LR(n2))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool select_opts(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("select_opts_LR");
    if (select_opts_LR(n1))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    return false;
}

bool select_opts_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("ALL");
    Node *n2 = createNode("select_opts_LR");
    if (match(ALL) && select_opts_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("DISTINCT");
    Node *n4 = createNode("select_opts_LR");
    if (match(DISTINCT) && select_opts_LR(n2))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return true;
}

bool select_expr_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("select_expr");
    Node *n2 = createNode("select_expr_list_LR");
    if (select_expr(n1) && select_expr_list_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("ARITHMETIC_MUL");
    Node *n4 = createNode("select_expr_list_LR");
    if (match(ARITHMETIC_MUL) && select_expr_list_LR(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool select_expr_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("select_expr");
    Node *n3 = createNode("select_expr_list_LR");
    if (match(COMMA) && select_expr(n2) && select_expr_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool select_expr(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("expr");
    Node *n2 = createNode("opt_as_alias");
    if (expr(n1) && opt_as_alias(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool table_references(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("table_factor");
    Node *n2 = createNode("table_references_LF");
    if (table_factor(n1) && table_references_LF(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool table_references_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("table_references");
    if (match(COMMA) && table_references(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool table_factor(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NAME");
    Node *n2 = createNode("table_factor_LF2");
    if (match(NAME) && table_factor_LF2(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("STARTPARENTHESIS");
    Node *n4 = createNode("table_factor_LF1");
    if (match(STARTPARENTHESIS) && table_factor_LF1(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool table_factor_LF1(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("table_references");
    Node *n2 = createNode("ENDPARENTHESIS");
    if (table_references(n1) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("table_subquery");
    Node *n4 = createNode("opt_as");
    Node *n5 = createNode("match");
    if (table_subquery(n3) && opt_as(n4) && match(NAME))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    return false;
}

bool table_factor_LF2(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("DOT");
    Node *n2 = createNode("NAME");
    Node *n3 = createNode("opt_as_alias");
    if (match(DOT) && match(NAME) && opt_as_alias(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("opt_as_alias");
    if (opt_as_alias(n4))
    {
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool table_subquery(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("select_stmt");
    Node *n2 = createNode("ENDPARENTHESIS");
    if (select_stmt(n1) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool opt_as(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("AS");
    if (match(AS))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_as_alias(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("AS");
    Node *n2 = createNode("NAME");
    if (match(AS) && match(NAME))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("NAME");
    if (match(NAME))
    {
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool delete_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("DELETE");
    Node *n2 = createNode("FROM");
    Node *n3 = createNode("NAME");
    Node *n4 = createNode("opt_where");
    if (match(DELETE) && match(FROM) && match(NAME) && opt_where(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("INSERT");
    Node *n2 = createNode("opt_into");
    Node *n3 = createNode("NAME");
    Node *n4 = createNode("opt_col_names");
    Node *n5 = createNode("insert_stmt_LF");
    if (match(INSERT) && opt_into(n2) && match(NAME) && opt_col_names(n4) && insert_stmt_LF(n5))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_stmt_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("VALUES");
    Node *n2 = createNode("insert_vals_list");
    if (match(VALUES) && insert_vals_list(n2))
    {
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("select_stmt");
    if (select_stmt(n3))
    {
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return false;
}

bool opt_into(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("INTO");
    if (match(INTO))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_col_names(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STARTPARENTHESIS");
    Node *n2 = createNode("column_list");
    Node *n3 = createNode("ENDPARENTHESIS");
    if (match(STARTPARENTHESIS) && column_list(n2) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool insert_vals_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STARTPARENTHESIS");
    Node *n2 = createNode("insert_vals");
    Node *n3 = createNode("ENDPARENTHESIS");
    Node *n4 = createNode("insert_vals_list_LR");
    if (match(STARTPARENTHESIS) && insert_vals(n2) && match(ENDPARENTHESIS) && insert_vals_list_LR(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_vals_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("STARTPARENTHESIS");
    Node *n3 = createNode("insert_vals");
    Node *n4 = createNode("ENDPARENTHESIS");
    Node *n5 = createNode("insert_vals_list_LR");
    if (match(COMMA) && match(STARTPARENTHESIS) && insert_vals(n3) && match(ENDPARENTHESIS) && insert_vals_list_LR(n5))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    return true;
}

bool insert_vals(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("expr");
    Node *n2 = createNode("insert_vals_LR");
    if (expr(n1) && insert_vals_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("DEFAULT");
    Node *n4 = createNode("insert_vals_LR");
    if (match(DEFAULT) && insert_vals_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool insert_vals_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("insert_vals");
    if (match(COMMA) && insert_vals(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool update_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("UPDATE");
    Node *n2 = createNode("table_references");
    Node *n3 = createNode("SET");
    Node *n4 = createNode("update_asgn_list");
    Node *n5 = createNode("opt_where");
    if (match(UPDATE) && table_references(n2) && match(SET) && update_asgn_list(n4) && opt_where(n5))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    return false;
}

bool update_asgn_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("");
    Node *n2 = createNode("");
    if (match(NAME) && update_asgn_list_LF(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool update_asgn_list_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMPARISON");
    Node *n2 = createNode("expr");
    Node *n3 = createNode("update_asgn_list_LR");
    if (match(COMPARISON) && expr(n2) && update_asgn_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("DOT");
    Node *n5 = createNode("NAME");
    Node *n6 = createNode("COMPARISON");
    Node *n7 = createNode("expr");
    Node *n8 = createNode("update_asgn_list_LR");
    if (match(DOT) && match(NAME) && match(COMPARISON) && expr(n7) && update_asgn_list_LR(n8))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        return true;
    }
    current = old_position;
    return false;
}

bool update_asgn_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("");
    Node *n2 = createNode("");
    Node *n3 = createNode("");
    if (match(COMMA) && match(NAME) && update_asgn_list_LR_LF(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool update_asgn_list_LR_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("");
    Node *n2 = createNode("");
    Node *n3 = createNode("");
    if (match(COMPARISON) && expr(n2) && update_asgn_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("DOT");
    Node *n5 = createNode("NAME");
    Node *n6 = createNode("COMPARISON");
    Node *n7 = createNode("expr");
    Node *n8 = createNode("update_asgn_list_LR");
    if (match(DOT) && match(NAME) && match(COMPARISON) && expr(n7) && update_asgn_list_LR(n8))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        return true;
    }
    current = old_position;
    return false;
}

bool create_database_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("CREATE");
    Node *n2 = createNode("DATABASE");
    Node *n3 = createNode("NAME");
    if (match(CREATE) && match(DATABASE) && match(NAME))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("CREATE");
    Node *n2 = createNode("TABLE");
    Node *n3 = createNode("NAME");
    Node *n4 = createNode("create_table_stmt_LF");
    if (match(CREATE) && match(TABLE) && match(NAME) && create_table_stmt_LF(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STARTPARENTHESIS");
    Node *n2 = createNode("create_col_list");
    Node *n3 = createNode("ENDPARENTHESIS");
    Node *n4 = createNode("create_table_stmt_LF3");
    if (match(STARTPARENTHESIS) && create_col_list(n2) && match(ENDPARENTHESIS) && create_table_stmt_LF3(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("DOT");
    Node *n6 = createNode("NAME");
    Node *n7 = createNode("create_table_stmt_LF2");
    if (match(DOT) && match(NAME) && create_table_stmt_LF2(n7))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt_LF2(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STARTPARENTHESIS");
    Node *n2 = createNode("create_col_list");
    Node *n3 = createNode("ENDPARENTHESIS");
    Node *n4 = createNode("create_table_stmt_LF3");
    if (match(STARTPARENTHESIS) && create_col_list(n2) && match(ENDPARENTHESIS) && create_table_stmt_LF3(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("create_select_statement");
    if (create_select_statement(n5))
    {
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    return false;
}

bool create_table_stmt_LF3(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("create_select_statement");
    if (create_select_statement(n1))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    return true;
}

bool create_col_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("create_definition");
    Node *n2 = createNode("create_col_list_LR");
    if (create_definition(n1) && create_col_list_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool create_col_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("create_definition");
    Node *n3 = createNode("create_col_list_LR");
    if (match(COMMA) && create_definition(n2) && create_col_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool create_definition(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NAME");
    Node *n2 = createNode("data_type");
    Node *n3 = createNode("column_atts");
    if (match(NAME) && data_type(n2) && column_atts(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("PRIMARY");
    Node *n5 = createNode("KEY");
    Node *n6 = createNode("STARTPARENTHESIS");
    Node *n7 = createNode("column_list");
    Node *n8 = createNode("ENDPARENTHESIS");
    if (match(PRIMARY) && match(KEY) && match(STARTPARENTHESIS) && column_list(n7) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        return true;
    }
    current = old_position;
    Node *n9 = createNode("KEY");
    Node *n10 = createNode("STARTPARENTHESIS");
    Node *n11 = createNode("column_list");
    Node *n12 = createNode("ENDPARENTHESIS");
    if (match(KEY) && match(STARTPARENTHESIS) && column_list(n11) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n9);
        add_edge(currentNode, n10);
        add_edge(currentNode, n11);
        add_edge(currentNode, n12);
        return true;
    }
    current = old_position;
    return false;
}

bool column_atts(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("column_atts_LR");
    if (column_atts_LR(n1))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    return false;
}

bool column_atts_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NOT");
    Node *n2 = createNode("NULLX");
    Node *n3 = createNode("column_atts_LR");
    if (match(NOT) && match(NULLX) && column_atts_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("NULLX");
    Node *n5 = createNode("column_atts_LR");
    if (match(NULLX) && column_atts_LR(n5))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    Node *n6 = createNode("DEFAULT");
    Node *n7 = createNode("column_atts_LR_LF1");
    if (match(DEFAULT) && column_atts_LR_LF1(n7))
    {
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        return true;
    }
    current = old_position;
    Node *n8 = createNode("AUTO_INCREMENT");
    Node *n9 = createNode("column_atts_LR");
    if (match(AUTO_INCREMENT) && column_atts_LR(n9))
    {
        add_edge(currentNode, n8);
        add_edge(currentNode, n9);
        return true;
    }
    current = old_position;
    Node *n10 = createNode("UNIQUE");
    Node *n11 = createNode("column_atts_LR_LF2");
    if (match(UNIQUE) && column_atts_LR_LF2(n11))
    {
        add_edge(currentNode, n10);
        add_edge(currentNode, n11);
        return true;
    }
    current = old_position;
    Node *n12 = createNode("PRIMARY");
    Node *n13 = createNode("KEY");
    Node *n14 = createNode("column_atts_LR");
    if (match(PRIMARY) && match(KEY) && column_atts_LR(n14))
    {
        add_edge(currentNode, n12);
        add_edge(currentNode, n13);
        add_edge(currentNode, n14);
        return true;
    }
    current = old_position;
    Node *n15 = createNode("KEY");
    Node *n16 = createNode("column_atts_LR");
    if (match(KEY) && column_atts_LR(n16))
    {
        add_edge(currentNode, n15);
        add_edge(currentNode, n16);
        return true;
    }
    current = old_position;
    return true;
}

bool column_atts_LR_LF1(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STRING");
    Node *n2 = createNode("column_atts_LR");
    if (match(STRING) && column_atts_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("INTNUM");
    Node *n4 = createNode("column_atts_LR");
    if (match(INTNUM) && column_atts_LR(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("BOOL");
    Node *n6 = createNode("column_atts_LR");
    if (match(BOOL) && column_atts_LR(n6))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    return false;
}

bool column_atts_LR_LF2(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STARTPARENTHESIS");
    Node *n2 = createNode("column_list");
    Node *n3 = createNode("ENDPARENTHESIS");
    Node *n4 = createNode("column_atts_LR");
    if (match(STARTPARENTHESIS) && column_list(n2) && match(ENDPARENTHESIS) && column_atts_LR(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("KEY");
    Node *n6 = createNode("column_atts_LR");
    if (match(KEY) && column_atts_LR(n6))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    return false;
}

bool opt_length(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STARTPARENTHESIS");
    Node *n2 = createNode("INTNUM");
    Node *n3 = createNode("opt_length_LF");
    if (match(STARTPARENTHESIS) && match(INTNUM) && opt_length_LF(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_length_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("ENDPARENTHESIS");
    if (match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    Node *n2 = createNode("COMMA");
    Node *n3 = createNode("INTNUM");
    Node *n4 = createNode("ENDPARENTHESIS");
    if (match(COMMA) && match(INTNUM) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool data_type(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("INT");
    Node *n2 = createNode("opt_length");
    if (match(INT) && opt_length(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("CHAR");
    Node *n6 = createNode("opt_length");
    if (match(CHAR) && opt_length(n6))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    Node *n7 = createNode("VARCHAR");
    Node *n8 = createNode("STARTPARENTHESIS");
    Node *n9 = createNode("INTNUM");
    Node *n10 = createNode("ENDPARENTHESIS");
    if (match(VARCHAR) && match(STARTPARENTHESIS) && match(INTNUM) && match(ENDPARENTHESIS))
    {
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        add_edge(currentNode, n9);
        add_edge(currentNode, n10);
        return true;
    }
    current = old_position;
    return false;
}

bool enum_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("STRING");
    Node *n2 = createNode("enum_list_LR");
    if (match(STRING) && enum_list_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool enum_list_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("STRING");
    Node *n3 = createNode("enum_list_LR");
    if (match(COMMA) && match(STRING) && enum_list_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return true;
}

bool create_select_statement(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("opt_as");
    Node *n2 = createNode("select_stmt");
    if (opt_as(n1) && select_stmt(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool val_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("expr");
    Node *n2 = createNode("val_list_LF");
    if (expr(n1) && val_list_LF(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return false;
}

bool val_list_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COMMA");
    Node *n2 = createNode("val_list");
    if (match(COMMA) && val_list(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    return true;
}

bool opt_val_list(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n = createNode("val_list");
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
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NAME");
    Node *n2 = createNode("expr_LF");
    if (match(NAME) && expr_LF(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("STRING");
    Node *n4 = createNode("expr_LR");
    if (match(STRING) && expr_LR(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("INTNUM");
    Node *n6 = createNode("expr_LR");
    if (match(INTNUM) && expr_LR(n6))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    Node *n7 = createNode("BOOL");
    Node *n8 = createNode("expr_LR");
    if (match(BOOL) && expr_LR(n8))
    {
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        return true;
    }
    current = old_position;
    Node *n9 = createNode("NOT");
    Node *n10 = createNode("expr");
    Node *n11 = createNode("expr_LR");
    if (match(NOT) && expr(n10) && expr_LR(n11))
    {
        add_edge(currentNode, n9);
        add_edge(currentNode, n10);
        add_edge(currentNode, n11);
        return true;
    }
    current = old_position;
    Node *n12 = createNode("ARITHMETIC_NOT");
    Node *n13 = createNode("expr");
    Node *n14 = createNode("expr_LR");
    if (match(ARITHMETIC_NOT) && expr(n13) && expr_LR(n14))
    {
        add_edge(currentNode, n12);
        add_edge(currentNode, n13);
        add_edge(currentNode, n14);
        return true;
    }
    current = old_position;
    Node *n15 = createNode("ARITHMETIC_SUB");
    Node *n16 = createNode("expr");
    Node *n17 = createNode("expr_LR");
    if (match(ARITHMETIC_SUB) && expr(n16) && expr_LR(n17))
    {
        add_edge(currentNode, n15);
        add_edge(currentNode, n16);
        add_edge(currentNode, n17);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n2 = createNode("STARTPARENTHESIS");
    Node *n3 = createNode("opt_val_list");
    Node *n4 = createNode("ENDPARENTHESIS");
    Node *n5 = createNode("expr_LR");
    if (match(STARTPARENTHESIS) && opt_val_list(n3) && match(ENDPARENTHESIS) && expr_LR(n5))
    {
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    Node *n6 = createNode("DOT");
    Node *n7 = createNode("NAME");
    Node *n8 = createNode("expr_LR");
    if (match(DOT) && match(NAME) && expr_LR(n8))
    {
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        return true;
    }
    current = old_position;
    Node *n1 = createNode("expr_LR");
    if (expr_LR(n1))
    {
        add_edge(currentNode, n1);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("ARITHMETIC_ADD");
    Node *n2 = createNode("expr");
    Node *n3 = createNode("expr_LR");
    if (match(ARITHMETIC_ADD) && expr(n2) && expr_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("ARITHMETIC_SUB");
    Node *n5 = createNode("expr");
    Node *n6 = createNode("expr_LR");
    if (match(ARITHMETIC_SUB) && expr(n5) && expr_LR(n6))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    Node *n7 = createNode("ARITHMETIC_MUL");
    Node *n8 = createNode("expr");
    Node *n9 = createNode("expr_LR");
    if (match(ARITHMETIC_MUL) && expr(n8) && expr_LR(n9))
    {
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        add_edge(currentNode, n9);
        return true;
    }
    current = old_position;
    Node *n10 = createNode("ARITHMETIC_DIV");
    Node *n11 = createNode("expr");
    Node *n12 = createNode("expr_LR");
    if (match(ARITHMETIC_DIV) && expr(n11) && expr_LR(n12))
    {
        add_edge(currentNode, n10);
        add_edge(currentNode, n11);
        add_edge(currentNode, n12);
        return true;
    }
    current = old_position;
    Node *n13 = createNode("MOD");
    Node *n14 = createNode("expr");
    Node *n15 = createNode("expr_LR");
    if (match(MOD) && expr(n14) && expr_LR(n15))
    {
        add_edge(currentNode, n13);
        add_edge(currentNode, n14);
        add_edge(currentNode, n15);
        return true;
    }
    current = old_position;
    Node *n16 = createNode("ANDOP");
    Node *n17 = createNode("expr");
    Node *n18 = createNode("expr_LR");
    if (match(ANDOP) && expr(n17) && expr_LR(n18))
    {
        add_edge(currentNode, n16);
        add_edge(currentNode, n17);
        add_edge(currentNode, n18);
        return true;
    }
    current = old_position;
    Node *n19 = createNode("OR");
    Node *n20 = createNode("expr");
    Node *n21 = createNode("expr_LR");
    if (match(OR) && expr(n20) && expr_LR(n21))
    {
        add_edge(currentNode, n19);
        add_edge(currentNode, n20);
        add_edge(currentNode, n21);
        return true;
    }
    current = old_position;
    Node *n22 = createNode("XOR");
    Node *n23 = createNode("expr");
    Node *n24 = createNode("expr_LR");
    if (match(XOR) && expr(n23) && expr_LR(n24))
    {
        add_edge(currentNode, n22);
        add_edge(currentNode, n23);
        add_edge(currentNode, n24);
        return true;
    }
    current = old_position;
    Node *n25 = createNode("COMPARISON");
    Node *n26 = createNode("expr_LR_LF1");
    if (match(COMPARISON) && expr_LR_LF1(n26))
    {
        add_edge(currentNode, n25);
        add_edge(currentNode, n26);
        return true;
    }
    current = old_position;
    Node *n27 = createNode("IS");
    Node *n28 = createNode("expr_LR_LF2");
    if (match(IS) && expr_LR_LF2(n28))
    {
        add_edge(currentNode, n27);
        add_edge(currentNode, n28);
        return true;
    }
    current = old_position;
    Node *n34 = createNode("IN");
    Node *n35 = createNode("STARTPARENTHESIS");
    Node *n36 = createNode("expr_LR_LF4");
    if (match(IN) && match(STARTPARENTHESIS) && expr_LR_LF4(n36))
    {
        add_edge(currentNode, n34);
        add_edge(currentNode, n35);
        add_edge(currentNode, n36);
        return true;
    }
    current = old_position;
    Node *n37 = createNode("NOT");
    Node *n38 = createNode("expr_LR_LF5");
    if (match(NOT) && expr_LR_LF5(n38))
    {
        add_edge(currentNode, n37);
        add_edge(currentNode, n38);
        return true;
    }
    current = old_position;
    Node *n39 = createNode("LIKE");
    Node *n40 = createNode("expr");
    Node *n41 = createNode("expr_LR");
    if (match(LIKE) && expr(n40) && expr_LR(n41))
    {
        add_edge(currentNode, n39);
        add_edge(currentNode, n40);
        add_edge(currentNode, n41);
        return true;
    }
    current = old_position;
    Node *n42 = createNode("REGEXP");
    Node *n43 = createNode("expr");
    Node *n44 = createNode("expr_LR");
    if (match(REGEXP) && expr(n43) && expr_LR(n44))
    {
        add_edge(currentNode, n42);
        add_edge(currentNode, n43);
        add_edge(currentNode, n44);
        return true;
    }
    current = old_position;
    return true;
}

bool expr_LR_LF1(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("expr");
    Node *n2 = createNode("expr_LR");
    if (expr(n1) && expr_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("STARTPARENTHESIS");
    Node *n4 = createNode("select_stmt");
    Node *n5 = createNode("ENDPARENTHESIS");
    Node *n6 = createNode("expr_LR");
    if (match(STARTPARENTHESIS) && select_stmt(n4) && match(ENDPARENTHESIS) && expr_LR(n6))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    Node *n7 = createNode("ANY");
    Node *n8 = createNode("STARTPARENTHESIS");
    Node *n9 = createNode("select_stmt");
    Node *n10 = createNode("ENDPARENTHESIS");
    Node *n11 = createNode("expr_LR");
    if (match(ANY) && match(STARTPARENTHESIS) && select_stmt(n9) && match(ENDPARENTHESIS) && expr_LR(n11))
    {
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        add_edge(currentNode, n9);
        add_edge(currentNode, n10);
        add_edge(currentNode, n11);
        return true;
    }
    current = old_position;
    Node *n12 = createNode("ALL");
    Node *n13 = createNode("STARTPARENTHESIS");
    Node *n14 = createNode("select_stmt");
    Node *n15 = createNode("ENDPARENTHESIS");
    Node *n16 = createNode("expr_LR");
    if (match(ALL) && match(STARTPARENTHESIS) && select_stmt(n14) && match(ENDPARENTHESIS) && expr_LR(n16))
    {
        add_edge(currentNode, n12);
        add_edge(currentNode, n13);
        add_edge(currentNode, n14);
        add_edge(currentNode, n15);
        add_edge(currentNode, n16);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF2(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NOT");
    Node *n2 = createNode("expr_LR_LF3");
    if (match(NOT) && expr_LR_LF3(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("NULLX");
    Node *n4 = createNode("expr_LR_LF4");
    if (match(NULLX) && expr_LR(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("BOOL");
    Node *n6 = createNode("expr_LR");
    if (match(BOOL) && expr_LR(n6))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF3(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("NULLX");
    Node *n2 = createNode("expr_LR");
    if (match(NULLX) && expr_LR(n2))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        return true;
    }
    current = old_position;
    Node *n3 = createNode("BOOL");
    Node *n4 = createNode("expr_LR");
    if (match(BOOL) && expr_LR(n4))
    {
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF4(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("val_list");
    Node *n2 = createNode("ENDPARENTHESIS");
    Node *n3 = createNode("expr_LR");
    if (val_list(n1) && match(ENDPARENTHESIS) && expr_LR(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("select_stmt");
    Node *n5 = createNode("ENDPARENTHESIS");
    Node *n6 = createNode("expr_LR");
    if (select_stmt(n4) && match(ENDPARENTHESIS) && expr_LR(n6))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    return false;
}

bool expr_LR_LF5(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("IN");
    Node *n2 = createNode("STARTPARENTHESIS");
    Node *n3 = createNode("expr_LR_LF4");
    if (match(IN) && match(STARTPARENTHESIS) && expr_LR_LF4(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("REGEXP");
    Node *n5 = createNode("expr");
    Node *n6 = createNode("expr_LR");
    if (match(REGEXP) && expr(n5) && expr_LR(n6))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        return true;
    }
    current = old_position;
    Node *n7 = createNode("LIKE");
    Node *n8 = createNode("expr");
    Node *n9 = createNode("expr_LR");
    if (match(LIKE) && expr(n8) && expr_LR(n9))
    {
        add_edge(currentNode, n7);
        add_edge(currentNode, n8);
        add_edge(currentNode, n9);
        return true;
    }
    current = old_position;
    return false;
}

bool drop_table_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("DROP");
    Node *n2 = createNode("TABLE");
    Node *n3 = createNode("NAME");
    if (match(DROP) && match(TABLE) && match(NAME))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    return false;
}

bool alter_table_stmt(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("ALTER");
    Node *n2 = createNode("TABLE");
    Node *n3 = createNode("NAME");
    Node *n4 = createNode("column_modify");
    if (match(ALTER) && match(TABLE) && match(NAME) && column_modify(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    return false;
}

bool column_modify(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("ALTER");
    Node *n2 = createNode("COLUMN");
    Node *n3 = createNode("NAME");
    Node *n4 = createNode("data_type");
    if (match(ALTER) && match(COLUMN) && match(NAME) && data_type(n4))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        add_edge(currentNode, n4);
        return true;
    }
    current = old_position;
    Node *n5 = createNode("DROP");
    Node *n6 = createNode("COLUMN");
    Node *n7 = createNode("NAME");
    if (match(DROP) && match(COLUMN) && match(NAME))
    {
        add_edge(currentNode, n5);
        add_edge(currentNode, n6);
        add_edge(currentNode, n7);
        return true;
    }
    current = old_position;
    Node *n8 = createNode("MODIFY");
    Node *n9 = createNode("column_modify_LF");
    if (match(MODIFY) && column_modify_LF(n9))
    {
        add_edge(currentNode, n8);
        add_edge(currentNode, n9);
        return true;
    }
    current = old_position;
    Node *n10 = createNode("ADD");
    Node *n11 = createNode("NAME");
    Node *n12 = createNode("data_type");
    if (match(ADD) && match(NAME) && data_type(n12))
    {
        add_edge(currentNode, n10);
        add_edge(currentNode, n11);
        add_edge(currentNode, n12);
        return true;
    }
    current = old_position;
    return false;
}

bool column_modify_LF(Node *currentNode)
{
    (*currentNode).isNonTerminal = true;
    int old_position = current;
    Node *n1 = createNode("COLUMN");
    Node *n2 = createNode("NAME");
    Node *n3 = createNode("data_type");
    if (match(COLUMN) && match(NAME) && data_type(n3))
    {
        add_edge(currentNode, n1);
        add_edge(currentNode, n2);
        add_edge(currentNode, n3);
        return true;
    }
    current = old_position;
    Node *n4 = createNode("NAME");
    Node *n5 = createNode("data_type");
    if (match(NAME) && data_type(n5))
    {
        add_edge(currentNode, n4);
        add_edge(currentNode, n5);
        return true;
    }
    current = old_position;
    return false;
}