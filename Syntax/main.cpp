#include "lexical.h"
#include "syntax.h"

int main()
{
    string la;
    string sa;
    char input[20], output[20] = { 0 };

    // lexical analyzer 진행
    do {
        cout << "<< Enter the execution command of your lexical analyzer >>" << endl;
        cout << "*** input => lexical_analyzer <input_file_name>" << endl;
        cout << "*** ex) lexical_analyzer test.txt" << endl;
        cout << "=>";
        cin >> la >> input;
        cout << "\n";
    } while (la.compare("lexical_analyzer") != 0);

    lexicalAnalyzer(input);
 
    // syntax analyzer 진행
    do {
        cout << "<< Enter the execution command of your syntax analyzer >>" << endl;
        cout << "*** input => syntax_analyzer <input_file_name>" << endl;
        cout << "*** ex) syntax_analyzer test.txt.out.txt" << endl;
        cout << "=>";
        cin >> sa >> output;
        cout << "\n";
    } while (sa.compare("syntax_analyzer") != 0);
    
    syntaxAnalyzer(output);

    system("pause");

    return 0;
}