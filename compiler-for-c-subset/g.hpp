#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include "c.hpp"

using namespace std;

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

string getNewRegister()
{
    static int count = 0;
    return string("register_" + to_string(++count));
}

string genNewLabel()
{
    static int label = 0;
    return string("label_" + to_string(++label) + ":");
}

void printSpace(int tabCount)
{
    for (int i = 0; i < tabCount; i++)
    {
        cout << "    ";
    }
}

void dfs(Node *node, int tabCount)
{
    printSpace(tabCount);
    if (node == nullptr || node->name == "")
    {
        cout << "end" << endl;
        return;
    }
    cout << node->code << endl;
    for (int i = 0; i < (node->rhs).size(); ++i)
    {
        dfs((node->rhs)[i], tabCount + 1);
    }
}

string codeGen(Node *node)
{
    if (node == nullptr) // not sure
    {
        return "";
    }
    switch (node->code)
    {
    case ID:
        return node->name;
    case EXPR_LR1:
        switch ((node->rhs)[0]->code)
        {
        case LOGICAL_OP:
        case BINARY_OP:
            string a = codeGen();

        case ASSIGMENT_OP:
        case RELATIONAL_OP:
        }
    case EXPR:
        {
            switch ((node->rhs)[0]->code)
            {
                case NUMBER:
                    if(((node->rhs)[1]->rhs).size() != 0){
                        string t1 = codeGen(((node->rhs)[1]->rhs)[1]);
                        cout << t1 + "1" << "=" << t1 << ((node->rhs)[1]->rhs)[0]->value << (node->rhs)[0]->value;
                        return t1 + "1";
                    } else {
                        cout << (node->rhs)[0]->value;
                        return (node->rhs)[0]->value;
                    }
                case ID:
                    if (((node->rhs)[1]->rhs).size() == 1)
                    {
                        Node *temp = ((node->rhs)[1])->rhs[0];
                        if((temp->rhs).size() != 0){
                            string t1 = codeGen((temp->rhs)[1]);
                            cout << t1 + "1" << "=" << t1 << (temp->rhs)[0]->value << (node->rhs)[0]->value;
                            return t1 + "1";
                        } else {
                            cout << (temp->rhs)[0]->value;
                            return (temp->rhs)[0]->value;
                        }   
                    }
            }
        }
    default:
        return "";
    }
}

void startCodeGen()
{
    int scope = 0;
    codeGen(root);
}