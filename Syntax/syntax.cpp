#include "syntax.h"
#include "table.h"

/* 
문자열 배열 s :
	symbol에 해당하는 column 번호를 알기 위해 선언한 문자열 배열 
	엑셀로 만든 SLR table의 column 순서대로 symbol을 작성해주었다.
*/
string s[36] = { "vtype", "id", "semi", "assign", "lparen", "rparen", "lbrace", "rbrace", "comma", "if", "while", "for", "else", "literal",
				"addsub", "multdiv", "num", "float", "comp", "return", "$", "CODE", "VDECL", "ASSIGN", "FDECL", "ARG", "MOREARGS", "BLOCK", 
				"STMT",	"ELSE", "RHS", "EXPR", "TERM", "FACTOR", "COND", "RETURN" };
/*
정수형 배열 before :
	reduce를 진행할 때 변경되기 전 viable prefix의 길이를 production의 순서마다 나타내줌.
	이후 after을 이용해 viable prefix를 변경해준다.
*/
int before[33] = { 0, 2, 2, 0, 3, 3, 3, 9, 3, 0, 4, 0, 2, 0, 1, 2, 8, 7, 11, 4, 0, 1, 1, 3, 1, 3, 1, 3, 1, 1, 1, 3, 3};
/*
문자열 배열 after :
	reduce를 진행할 때 변경될 viable prefix를 지정
	위의 before의 index 순서와 같다
*/
string after[33] = { "", "CODE", "CODE", "CODE", "VDECL", "VDECL", "ASSIGN", "FDECL", "ARG", "ARG", "MOREARGS", "MOREARGS",
					"BLOCK", "BLOCK", "STMT", "STMT", "STMT", "STMT", "STMT", "ELSE", "ELSE", "RHS", "RHS", "EXPR", "EXPR",
					"TERM", "TERM", "FACTOR", "FACTOR", "FACTOR", "FACTOR", "COND", "RETURN" };

/*
lexical analyzer 실행 후 만들어진 output(토큰 셋)을 input으로 받아 진행한다.
*/
void syntaxAnalyzer(const string& inputFile)
{
	// accept 여부를 정해주기 위한 bool 타입 변수
	bool acceptance = true;
	
	char line[10] = { 0 };
	ifstream input(inputFile, fstream::in);

	// output 파일이 열리지 않았을 때 에러 메세지 발생
	if (!input.is_open()) {
		cout << "error while opening the file\n";
		exit(0);
	}
	// output 파일이 열렸을 경우
	else {
		// 현재 스테이트를 나타내는 변수. startState가 0이므로 0으로 선언.
		int currentState = 0;
		// state의 변화를 stack에 담기 위해 선언한 stack
		stack<int> state;
		/* 
		symbol을 담기위한 tempsymbol과 symbol vector. tempsymbol은 토큰셋에서 필요한 부분만 담기 위해 거쳐가기 위한 vector임. 실제로 코드가
		돌아갈 때 사용될 것은 symbol vector.
		*/
		vector<string> tempsymbol;
		vector<string> symbol;
		// viable prefix를 담을 vector
		vector<string> viable;

		// input으로 들어온 파일을 줄마다 입력받아 tempsymbol에 넣는다. tempsymbol에 저장 된 값 ex) <vtype>, <id>, <for> .....
		while (input) {
			char line[20];
			input.getline(line, 20);
			
			tempsymbol.push_back(line);
		}
		// tempsymbol에 들어간 마지막 값을 pop해줌
		tempsymbol.pop_back();
		
		// tempsymbol에 저장된 값 중 < 와 > 를 제거하고 남은 부분만 symbol에 저장한다.
		for (int i = 0; i < tempsymbol.size(); i++) {
			int slen = tempsymbol[i].length();
			string temp = tempsymbol[i].substr(1, slen-2);
			
			symbol.push_back(temp);
		}
		// endmarker를 symbol에 추가
		symbol.push_back("$");

		/*
		reduce가 직전에 진행되었는지 알기 위해 bool 타입 변수를 선언한다.
		reduce 가 true라면 symbol의 맨 앞 element를 한번 더 제거해주기 위함
				  false라면 추가 제거없이 진행한다.
		*/
		bool reduce = false;

		state.push(currentState);
		while (true)
		{
			// stack의 맨 윗부분이 currentState가 된다. top은 table의 row부분
			int top = state.top();
			// tempSymbol이 nextSymbol이 된다.
			string tempSymbol = symbol.front();
			int symbolNum;

			// tempSymbol과 문자열 배열 s의 element를 비교해서 index를 찾기 위함. symbolNum은 table의 col부분
			for (int i = 0; i < 36; i++)
			{
				if (s[i] == tempSymbol)
					symbolNum = i;
			}
			// 위에서 설명한 부분. 직전 행동이 reduce였다면 symbol에서 맨 앞 element를 제거해준다.
			if (reduce == true)
			{
				symbol.erase(symbol.begin());
				reduce = false;
			}

			// currentState와 currentSymbol을 출력
			cout << "top :" << top << ", symbol : " << tempSymbol << endl;
			/*
			change = table의 element
			change의 범위에 따라 shift, reduce, goto, accept가 나뉜다.
			해당 설명은 table.h 에 나와있음.
			*/
			int change = table[top][symbolNum];
			if (change)
			{
				// accept
				if (change == 10000)
					break;
				// reduce
				else if (change >= 1000)
				{
					// production 번호를 알기 위함.
					int production = change - 1000;
					// 변경되어야하는 viable prefix의 크기를 받아오기 위한 poplength 변수
					int poplength = before[production];

					// poplength 크기만큼 viable prefix가 저장되어 있는 vector에서 제거해주고, stack에 저장되어있는 state도 제거해준다.
					for (int k = 0; k < poplength; k++) {
						viable.pop_back();
						state.pop();
					}
					// 변경될 symbol은 after[production]과 같으므로 viable vector의 맨 뒤에 추가해준다.
					viable.push_back(after[production]);
					// reduce가 진행되었음을 알려주기 위해 true로 변경
					reduce = true;
					
					/* 
					if문을 실행하고나서 symbol의 첫번째를 보고 변경이 일어나므로 symbol에도 after[production]을 추가해준다.
					위에서 reduce가 true일 때 symbol의 맨 앞을 한번 더 제거해준다.
					*/
					symbol.insert(symbol.begin(), after[production]);
					// reduce는 현재스테이트의 변화가 없으므로 stack에 push하지 않는다.
				}
				// shift
				else if (change >= 100)
				{
					// 이동할 state의 번호
					currentState = change - 100;
					// viable prefix에 symbol을 추가한다.
					viable.push_back(tempSymbol);
					// symbol의 맨 앞 원소를 지워준다.
					symbol.erase(symbol.begin());
					// state stack에 state를 추가해준다.
					state.push(currentState);
				}
				// goto
				else
				{
					// 이동할 state의 번호
					currentState = change;
					// state stack에 state를 추가해준다.
					state.push(change);
				}

			}
			// table의 element가 0인 경우 이동할 수 없다는 뜻이므로 잘못된 토큰이 들어왔음을 뜻한다. 따라서 while문 종료
			else {
				acceptance = false;
				break;
			}
		}
	}
	// acceptance : true => Accept 출력, false => Reject 출력
	if (acceptance)
		cout << "Accept" << endl;
	else
		cout << "Reject" << endl;
}