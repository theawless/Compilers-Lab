#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include "sql.h"

using namespace std;

static inline std::string &ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

static inline std::string &rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         std::not1(std::ptr_fun<int, int>(std::isspace)))
                .base(),
            s.end());
    return s;
}

static inline std::string &trim(std::string &s)
{
    return ltrim(rtrim(s));
}

const vector<int> token_numbers = {
    ALL,
    ANDOP,
    ADD,
    ANY,
    ALTER,
    ASC,
    AS,
    AUTO_INCREMENT,
    WHERE,
    BY,
    CHAR,
    CHAR,
    COLUMN,
    VARCHAR,
    VARCHAR,
    CREATE,
    DEFAULT,
    DROP,
    DATABASE,
    DECIMAL,
    DELETE,
    DESC,
    DISTINCT,
    FOREIGN,
    FROM,
    GRANT,
    GROUP,
    HAVING,
    INSERT,
    INT,
    INTO,
    INT,
    IN,
    IS,
    KEY,
    LIKE,
    MODIFY,
    MOD,
    NOT,
    NULLX,
    NUMERIC,
    OPTION,
    ORDER,
    OR,
    PRIMARY,
    REFERENCES,
    REGEXP,
    SET,
    SELECT,
    TABLE,
    UNION,
    UNIQUE,
    UPDATE,
    USER,
    VALUES,
    XOR,

    COMPARISON,
    COMPARISON,
    COMPARISON,
    COMPARISON,
    COMPARISON,
    COMPARISON,
    COMPARISON,
    ARITHMETIC_SUB,
    ARITHMETIC_ADD,
    ARITHMETIC_MUL,
    ARITHMETIC_DIV,
    ARITHMETIC_NOT,
    STARTPARENTHESIS,
    ENDPARENTHESIS,
    COMMA,
    DOT,
    SEMICOLON,

    BOOL,
    BOOL,
    BOOL,
    ANDOP,
    OR,

    STRING,
    STRING,

    NAME,

    INTNUM,
};

const vector<string> token_names = {
    R"###(^ALL\b)###",
    R"###(^AND\b)###",
    R"###(^ADD\b)###",
    R"###(^ANY\b)###",
    R"###(^ALTER\b)###",
    R"###(^ASC\b)###",
    R"###(^AS\b)###",
    R"###(^AUTO_INCREMENT\b)###",
    R"###(^WHERE\b)###",
    R"###(^BY\b)###",
    R"###(^CHARACTER\b)###",
    R"###(^CHAR\b)###",
    R"###(^COLUMN\b)###",
    R"###(^VARCHARACTER\b)###",
    R"###(^VARCHAR\b)###",
    R"###(^CREATE\b)###",
    R"###(^DEFAULT\b)###",
    R"###(^DROP\b)###",
    R"###(^DATABASE\b)###",
    R"###(^DECIMAL\b)###",
    R"###(^DELETE\b)###",
    R"###(^DESC\b)###",
    R"###(^DISTINCT\b)###",
    R"###(^FOREIGN\b)###",
    R"###(^FROM\b)###",
    R"###(^GRANT\b)###",
    R"###(^GROUP\b)###",
    R"###(^HAVING\b)###",
    R"###(^INSERT\b)###",
    R"###(^INTEGER\b)###",
    R"###(^INTO\b)###",
    R"###(^INT\b)###",
    R"###(^IN\b)###",
    R"###(^IS\b)###",
    R"###(^KEY\b)###",
    R"###(^LIKE\b)###",
    R"###(^MODIFY\b)###",
    R"###(^MOD\b)###",
    R"###(^NOT\b)###",
    R"###(^NULL\b)###",
    R"###(^NUMERIC\b)###",
    R"###(^OPTION\b)###",
    R"###(^ORDER\b)###",
    R"###(^OR\b)###",
    R"###(^PRIMARY\b)###",
    R"###(^REFERENCES\b)###",
    R"###(^REGEXP\b)###",
    R"###(^SET\b)###",
    R"###(^SELECT\b)###",
    R"###(^TABLE\b)###",
    R"###(^UNION\b)###",
    R"###(^UNIQUE\b)###",
    R"###(^UPDATE\b)###",
    R"###(^USER\b)###",
    R"###(^VALUES\b)###",
    R"###(^XOR\b)###",

    R"###(^=)###",
    R"###(^<>)###",
    R"###(^<=>)###",
    R"###(^<=)###",
    R"###(^>=)###",
    R"###(^<)###",
    R"###(^>)###",
    
    R"###(^\-)###",
    R"###(^\+)###",
    R"###(^\*)###",
    R"###(^\/)###",
    R"###(^\!)###",
    R"###(^\()###",
    R"###(^\))###",
    R"###(^\,)###",
    R"###(^\.)###",
    R"###(^\;)###",

    R"###(^TRUE\b)###",
    R"###(^UNKNOWN\b)###",
    R"###(^FALSE\b)###",
    R"###(^\&\&)###",
    R"###(^\|\|)###",

    R"###(^'(\\.|''|[^'\n])*')###",
    R"###(^\"(\\.|\"\"|[^"\n])*\")###",

    R"###(^[A-Za-z][A-Za-z0-9_]*\b)###",

    R"###(^-?[0-9]+(.[0-9]+)?\b)###",
};

vector<string> tokens = vector<string>();
vector<int> token_ids = vector<int>();

bool check(string &line, const string &token_name, const int token_number)
{
    if (line.empty())
    {
        return false;
    }
    smatch result;
    regex pattern(token_name, regex_constants::icase);
    if (regex_search(line, result, pattern))
    {
        //for (auto x:result) std::cout << x << " " << token_name << token_number << endl;
        token_ids.push_back(token_number);
        tokens.push_back(result.str());
        line = result.suffix().str();
        return true;
    }
    else
    {
        return false;
    }
}

void lexify_line(string &line)
{
    int i = 0;
    for (i = 0; i < token_names.size();)
    {
        line = trim(line);
        if (check(line, token_names[i], token_numbers[i]))
        {
            i = 0;
        }
        else
        {
            i++;
        }
        //cerr << token_names[i];
    }
    if (!line.empty() && i == token_names.size())
    {
        cerr << "Unmatched character" << endl;
    }
}
/*
void run_loop()
{
    do
    {
        token_ids.clear();
        tokens.clear();
        string line;
        cin >> line;
        lexify_line(line);

        for (auto i = 0; i < tokens.size(); i++)
        {
            cout << tokens[i] << " " << token_ids[i] << " ";
        }
        // parser stuff goes here
    } while (1);
}

int main(int argc, char **argv)
{
    if (argc != 1)
        cerr << "Wrong number of arguments supplied!" << endl;
    run_loop();
    return 0;
}
*/