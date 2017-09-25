#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

#define t_int "int"
#define t_float "float"
#define t_char "char"
#define t_func "FUNCTION"
#define t_int_array "int_array"
#define t_float_array "float_array"
#define t_char_array "char_array"
#define t_void "void"

extern int yylineno;
int scope = 0;
bool semanticError = false;
bool syntacticError = false;
bool printFlag = true;
string currentFunction;

void log(string s, bool flag)
{
    if (printFlag)
    {
        cout << s << yylineno << " " << endl;
    }
}

bool isStruct(string type)
{
    vector<string> nonStructVars = {t_int, t_float, t_char, t_func, t_int_array, t_float_array, t_char_array};
    return find(nonStructVars.begin(), nonStructVars.end(), type) == nonStructVars.end();
}

struct Node
{
    int code;
    string value;
    string type;
    string name;
    vector<int> dimlist;
    vector<string> varlist;
    vector<Node *> rhs;

    Node(int code, vector<Node *> rhs);
};

Node::Node(int code, vector<Node *> rhs)
    : code(code), value(""), type(""), dimlist({}), varlist({}), rhs(rhs)
{
}

Node *root;

struct Function
{
    string name;
    string rtype;
    vector<string> paramlist;

    Function();
    Function(string name, string rtype, vector<string> paramlist);
};

Function::Function()
    : name(""), rtype(""), paramlist({})
{
}

Function::Function(string name, string rtype, vector<string> paramlist)
    : name(name), rtype(rtype), paramlist(paramlist)
{
}

auto functionTable = vector<Function>();

struct Variable
{
    string name;
    string type;
    vector<int> dimlist;

    Variable();
    Variable(string name, string type, vector<int> dimlist);
};

Variable::Variable()
    :name(""), type(""), dimlist({})
{
}

Variable::Variable(string name, string type, vector<int> dimlist)
    : name(name), type(type), dimlist(dimlist)
{
}

struct VariableTable
{
    vector<Variable> rows;

    VariableTable();
    int indexOfVariable(string name) const;
    vector<string> getVariableNames();
    string getVariableType(string var_name);
};

VariableTable::VariableTable() : rows(vector<Variable>())
{
}

int VariableTable::indexOfVariable(string name) const
{
    auto it = find_if(rows.begin(), rows.end(), [name](const Variable &variable) { return variable.name == name; });
    return it == rows.end() ? -1 : it - rows.begin();
}

vector<string> VariableTable::getVariableNames()
{
    vector<string> variableNames = vector<string>();
    for (auto row : rows)
    {
        variableNames.push_back(row.name);
    }
    return variableNames;
}

string VariableTable::getVariableType(string var_name){
    int index = indexOfVariable(var_name);
    if (index == -1)
    {
        return "";
    } else return rows[indexOfVariable(var_name)].name;
}

auto variableTables = vector<VariableTable>(1);
auto allVariableTables = vector<pair<VariableTable, string> >(1);


struct Struct
{
    string name;
    vector<string> attribute_names;
    vector<string> attribute_types;
    vector<vector<int>> attribute_dimlists;

    Struct(string name, vector<string> attribute_names, vector<string> attribute_types, vector<vector<int>> attribute_dimlists);
};

Struct::Struct(string name, vector<string> attribute_names, vector<string> attribute_types, vector<vector<int>> attribute_dimlists)
    : name(name), attribute_names(attribute_names), attribute_types(attribute_types), attribute_dimlists(attribute_dimlists)
{
}

struct StructTable
{
    vector<Struct> rows;

    StructTable();
    int indexOfStruct(string name) const;
    vector<string> getStructNames();
};

StructTable::StructTable() : rows(vector<Struct>())
{
}

int StructTable::indexOfStruct(string name) const
{
    auto it = find_if(rows.begin(), rows.end(), [name](const Struct &structure) { return structure.name == name; });
    return it == rows.end() ? -1 : it - rows.begin();
}

vector<string> StructTable::getStructNames()
{
    vector<string> structNames = vector<string>();
    for (auto row : rows)
    {
        structNames.push_back(row.name);
    }
    return structNames;
}

auto structTables = vector<StructTable>(1);

pair<int, int> findIdentifier(string name, string type)
{
    if (isStruct(type))
    {
        for (int s = 0; s < structTables.size(); ++s)
        {
            int i = structTables[s].indexOfStruct(name);
            if (i != -1)
            {
                return make_pair(s, i);
            }
        }
        return make_pair(-1, -1);
    }
    else
    {
        for (int s = 0; s < variableTables.size(); ++s)
        {
            int i = variableTables[s].indexOfVariable(name);
            if (i != -1)
            {
                return make_pair(s, i);
            }
        }
        return make_pair(-1, -1);
    }
}

bool checkType(Node *a, Node *b)
{
    if (a->type == "")
        log("checkType: invalid first type empty", printFlag);
    if (b->type == "")
        log("checkType: invalid second type empty", printFlag);

    if (a->type == "")
    {
        cerr << "Type mismatch for";
        cerr << a->type << " " << (a->value != "") ? a->value : "";
        cerr << b->type << " " << (b->value != "") ? b->value : "";
        return false;
    }
    else if (b->type == "")
    {
        cerr << "Type mismatch for";
        cerr << a->type << " " << (a->value != "") ? a->value : "";
        cerr << b->type << " " << (b->value != "") ? b->value : "";
        return false;
    }
}

void scopepp()
{
    scope++;
    variableTables.push_back(VariableTable());
    structTables.push_back(StructTable());
}

void scopemm()
{
    scope--;
    allVariableTables.push_back(make_pair(variableTables[variableTables.size()-1], currentFunction));
    cout << "lol debug:" << currentFunction << endl;
    for (int i = 0; i < variableTables.size(); ++i)
    {
        auto variableTable = variableTables[i];
        for (auto element: variableTable.rows)
        {
            cout << "Type:" << element.type << "\t";
            cout << "name:" << element.name << "\t";
            if (element.dimlist.size()>0)
            {
                cout << "Dimlist:";
                for(auto j = 0; j < element.dimlist.size(); j++)
                cout << " " << element.dimlist[j];
            }
            cout << endl;
        }

    }
    variableTables.pop_back();
    structTables.pop_back();
}

void addToVariableTable(Node *currentNode)
{

    pair<int, int> location = findIdentifier(currentNode->name, currentNode->type);
    int varScope = location.second;
    if (varScope == -1 /* never declared */ || varScope < scope /* shadowing */)
    {
        cerr << "Scope" << scope << "Variable Table Size = " << variableTables.size() << endl;
        variableTables[scope].rows.push_back(Variable(currentNode->name, currentNode->type, currentNode->dimlist));
        log("Added variable" + currentNode->name, printFlag);
    }
    else if (varScope == scope) // current scope error
    {
        cerr << "Cannot redeclare variable";
        semanticError = true;
    }
    else if (varScope > scope) // should never happen
    {
        log("Something is wrong with scoping and variable declaration code", printFlag);
    }
}

/***
    P: ABQCD
    Q: XYPZ
       | empty
    Inserting all symbols at the target(th) position in rule 1
**/
// declaration_non_struct: data_type_non_struct assigned_identifier_list
// assigned_identifier_list: assigned_identifier optional_assigned_identifier_list_ext
// optional_assigned_identifier_list_ext: ',' assigned_identifier_list
//                                     | /* empty */
// getNodeList($3.node, 0, 1, 1);


vector<Node *> getNodeList(Node *head, int target, int position1, int position2)
{
    vector<Node *> nodes;
    Node *cur = head;
    Node *temp;
    while ((head != nullptr) && (head->rhs).size()!=0 )
    {
        nodes.push_back(head->rhs[target]);
        // if(head && head->rhs && (head->rhs)[position1] )
        temp = (head->rhs)[position1];
        if ((temp != nullptr) && (temp->rhs).size()!=0 )
        {
            head = (temp->rhs)[position2];
        } else break;
    }
    return nodes;
}

string applyOperation(Node *expr1, Node *operation, Node *expr2){
    return "";
}

bool areCoercibleType(string s1, string s2) {
    if (s1 == t_float)
    {  
        if ((s2 == t_int) || (s2 == t_char) || (s2 == t_float))
        {
            cout << "detected";
            return true;
        }
    } else if (s1 == t_int)
    {
        if ((s2 == t_char) || (s2 == t_int))
        {
            return true;
        }
    } else if (s1 == t_char && s2 == t_char)
    {
        return true;
    }
    else return s1==s2;
}

void printSymbolTables(){
    cout << "Scope" << "    " << "Name" << "    " << "TYPE" << endl; 
    for(int i=0;i<allVariableTables.size();i++){
        for(int j=0;j<allVariableTables[i].first.rows.size();j++){
            cout << allVariableTables[i].second << "    " << (allVariableTables[i].first).rows[j].name << "    " << (allVariableTables[i].first).rows[j].type << "   "  << endl;
        }
    }
}
    // allVariableTables.push_back(make_pair(variableTables[variableTables.size()-1], currentFunction));

// void addToStructTable(Node *currentNode)
// {
//     int structScope = findIdentifierScope(currentNode->name, currentNode->type);
//     if (structScope == -1 /* never declared */ || structScope < scope /* shadowing */)
//     {
//         structTables[scope].rows.push_back(Struct(currentNode->name, currentNode->attribute_names, currentNode->attribute_types, currentNode->attribute_dimlists));
//         log("Added struct" + currentNode->name, printFlag);
//     }
//     else if (structScope == scope) // current scope error
//     {
//         cerr << "Cannot redeclare struct";
//         semanticError = true;
//     }
//     else if (structScope > scope) // should never happen
//     {
//         log("Something is wrong with scoping and variable declaration code", printFlag);
//     }
// }

void printFunctionTable()
{
    for (auto function : functionTable)
    {
        cout << "function name: " << function.name << endl;
        cout << "return type: " << function.rtype << endl;
        cout << "params:";
        for (auto param : function.paramlist)
        {
            cout << " " << param;
        }
        // cout << "vars:";
        // for (auto var : function.varlist)
        // {
        //     cout << " " << var;
        // }
        cout << endl;
    }
}
