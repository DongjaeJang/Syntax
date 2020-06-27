#include "lexical.h"
/*
TermProject01�� Lexical�� ���� ����� ��
1. keyword�� if, for, while, else, return 5������ ����
    - �ش� ��ū���� �̸��� �������־��� (if : if / for : for / while : while / else : else / return : return
2. arithmetic���� +�� -�� addsub ����, *�� / �� multdiv�� ������ ++, --, += ���� arithmetic���� ��µǵ��� ��.
    - ���� syntax analyzer ������ +, -, *, / �ܿ� �ٸ� �����ڰ� ������ �Ǹ� reject�� ����ϰ� �ȴ�.( addsub �� multdiv �ۿ� �����Ƿ� )
3. character Ÿ�� ���� literal�� �߰�����.
4. ��ū Ÿ�Ը� ����ϵ��� ����.
*/

// TYPE
bool isType(const string& str)
{
    const vector<string> types{ "int", "float", "char", "bool" };
    for (const auto& type : types)
        if (type == str)
            return true;
    return false;
}

// INTEGER
bool isInteger(const string& str)
{
    if (str.size() == 1)
    {
        if (isdigit(str[0]))
            return true;
        else
            return false;
    }
    else
    {
        if (str[0] == '0')
            return false;
        else if (str[0] == '-')
        {
            for (int i = 1; i < str.size(); i++)
            {
                if (!isdigit(str[i]))
                    return false;
            }
            return true;
        }
        else
        {
            for (int i = 0; i < str.size(); i++)
            {
                if (!isdigit(str[i]))
                    return false;
            }
            return true;
        }
    }
}

// STRING
bool isString(const string& str)
{
    return (str[0] == '"' && str[str.size() - 1] == '"');
}

// CHARACTER

bool isCharacter(const string& str)
{
    if (str.size() == 3)
    {
        if (str[0] == '\'' && str[2] == '\'')
            return true;
    }
    return false;
}

// BOOL STRING
bool isBoolstring(const string& str)
{
    return str == "true" || str == "false";
}

// FLOAT
bool isFloat(const string& str)
{
    int dot_count = 0;
    int dot_index = 0;
    if (str[0] == '-')
    {
        if (str[1] == '0' && isdigit(str[2]))
            return false;
        if (str[1] == '.' || str[str.size() - 1] == '.')
            return false;
        else
        {
            for (int i = 1; i < str.size(); i++)
            {
                if (str[i] != '.' && !isdigit(str[i]))
                    return false;

                if (str[i] == '.')
                {
                    dot_count++;
                    dot_index = i;
                }
                if (i - dot_index == 7)
                    return false;
            }
        }
        if (dot_count != 1) return false;
        else return true;
    }
    else if (isdigit(str[0]))
    {
        if (str[0] == '0' && isdigit(str[1]))
            return false;
        if (str[str.size() - 1] == '.')
            return false;
        else
        {
            for (int i = 1; i < str.size(); i++)
            {
                if (str[i] != '.' && !isdigit(str[i]))
                    return false;

                if (str[i] == '.')
                {
                    dot_count++;
                    dot_index = i;
                }
                if (i - dot_index == 7)
                    return false;
            }
        }
        if (dot_count != 1) return false;
        else return true;
    }
    else return false;
}

// ID
bool isID(const string& str)
{
    if (isdigit(str[0]))
        return false;
    else if (isalpha(str[0]) || str[0] == '_')
    {
        for (int i = 1; i < str.size(); i++)
        {
            if (!(isdigit(str[i]) || isalpha(str[i]) || str[i] == '_'))
                return false;
        }
    }
    else 
        return false;

    return true;
}


// KEYWORD
bool isKeyword(const string& str)
{
    const vector<string> keywords{ "if", "else", "while", "for", "return" };
    for (const auto& keyword : keywords)
        if (keyword == str)
            return true;

    return false;
}

// ARITHMETIC
bool isArithmetic(const string& str)
{
    const vector<string> arithmetics{ "*=", "/=", "++", "+=", "--", "*", "+", "-", "/", "-=" };
    for (const auto& arithmetic : arithmetics)
        if (arithmetic == str)
            return true;

    return false;
}

// BITWISE
bool isBitwise(const string& str)
{
    const vector<string> bitwises{ "<<", ">>", "&", "|" };
    for (const auto& bitwise : bitwises)
        if (bitwise == str)
            return true;

    return false;
}

// ASSIGMENT
bool isAssignment(const string& str)
{
    const vector<string> assignments{ "=" };
    for (const auto& assignment : assignments)
        if (assignment == str)
            return true;

    return false;
}

// COMPARISON
bool isComparison(const string& str)
{
    const vector<string> comparisons{ ">=", "<=", ">", "<", "==", "!=" };
    for (const auto& comparison : comparisons)
        if (comparison == str)
            return true;

    return false;
}

// SEPARATOR
bool isSeparator(const string& str)
{
    const vector<string> separators{ "{", "}", ",", "(", ")", ";" };
    for (const auto& separator : separators)
        if (separator == str)
            return true;

    return false;
}

// WHITESPACE
bool isWhitespace(const string& str)
{
    return str == " " || str == "\n" || str == "\t";
}

// ��ū�� �Է¹ް� ��ū����� ������ ��ȯ���ݴϴ�.
string tokenRole(const string& token)
{
    if (isType(token))
        return "<vtype>";
    else if (isKeyword(token))
    {
        if (token == "if")
            return "<if>";
        else if (token == "else")
            return "<else>";
        else if (token == "for")
            return "<for>";
        else if (token == "while")
            return "<while>";
        else if (token == "return")
            return "<return>";
    }
    else if (isBitwise(token))
        return "<bitwise>";
    else if (isInteger(token))
        return "<num>";
       //\'" + token + "\' >";
    else if (isArithmetic(token))
    {
        if (token == "+" || token == "-")
            return "<addsub>";
        else if (token == "*" || token == "/")
            return "<multdiv>";
        else return "<arithmetic>";
    }
    else if (isComparison(token))
        return "<comp>";
    else if (isAssignment(token))
        return "<assign>";
    else if (isSeparator(token))
    {
        if (token == "(")
            return "<lparen>";
        else if (token == ")")
            return "<rparen>";
        else if (token == "{")
            return "<lbrace>";
        else if (token == "}")
            return "<rbrace>";
        else if (token == ",")
            return "<comma>";
        else
            return "<semi>";
    }
    else if (isFloat(token))
        return "<float>";
        //: \'" + token + "\'>";
    else if (isBoolstring(token))
        return "<boolstring>";
    else if (isString(token))
        return "<literal>";
    //: \'" + token + "\' >";
    else if (isID(token))
        return "<id>";
    //: \'" + token + "\' >";
    else if (isCharacter(token))
        return "<literal>";
    else
        return "< Error. Cannot read token : \'" + token + "\' >";
}

// ���ϸ��� �Է¹ް� '���ϸ�.out.txt'�� �����ϰ� ��ū���� �����մϴ�.
void lexicalAnalyzer(const string& inputFile)
{
    char ch;
    string buffer;
    ifstream input(inputFile, fstream::in);
    ofstream output(inputFile + ".out.txt");
    
    if (!input.is_open())
    {
        cout << "error while opening the file\n";
        exit(0);
    }

    // string �� �ν��ϱ� ���� ����
    int startString = 0;
    while (input >> noskipws >> ch)
    {
        // " �� ������ string�� �����ϴ� ���̹Ƿ� ������ 1�� �����ݴϴ�.
        if (ch == '"') startString += 1;

        // ù��° '"'�� ������ ���⸦ �����Ͽ� buffer�� �����մϴ�.
        if (startString == 1) {
            buffer += ch;
            continue;
        }

        // �ι�° '"'�� ������ �׵��� buffer�� ����� ���� String���� ����մϴ�.
        else if (startString == 2) {
            buffer += ch;
            output << tokenRole(buffer) << endl;
            startString = 0;
            buffer = "";
            continue;
        }

        // Whitespace�� ������ ��ū�� �������ݴϴ�.
        if (isWhitespace(string(1, ch)))
        {
            if (!buffer.empty())
            {
                output << tokenRole(buffer) << endl;
                buffer = "";
            }
            continue;

        }

        // Arithmetic�� ������ ��ū�� �������ݴϴ�.
        if (isArithmetic(string(1, ch)))
        {
            if (!buffer.empty() && !isArithmetic(buffer))
            {
                output << tokenRole(buffer) << endl;
                buffer = "";
            }
        }

        // Bitwise�� ������ ��ū�� �������ݴϴ�.
        if (isBitwise(string(1, ch)))
        {
            if (!buffer.empty() && !isBitwise(buffer))
            {
                output << tokenRole(buffer) << endl;
                buffer = "";
            }
        }

        // Comparison�� ������ ��ū�� �������ݴϴ�.
        if (isComparison(string(1, ch)))
        {
            if (!buffer.empty() && !isComparison(buffer))
            {
                output << tokenRole(buffer) <<endl;
                buffer = "";
            }
        }

        // Separator�� ������ ��ū�� �������ݴϴ�. Separator�� �ϳ��� ��ū���μ� ���� ������ݴϴ�.
        if (isSeparator(string(1, ch)))
        {
            if (!buffer.empty())
            {
                output << tokenRole(buffer) << endl;
                buffer = "";
            }
            if (isSeparator(string(1, ch)))
            {
                output << tokenRole(string(1, ch)) << endl;
                continue;
            }
        }
        buffer += ch;
    }
    input.close();
    output.close();
}